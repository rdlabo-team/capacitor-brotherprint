package jp.rdlabo.capacitor.plugin.brotherprint

import java.util.concurrent.Executors
import android.Manifest
import android.app.PendingIntent
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothManager
import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.graphics.BitmapFactory
import android.hardware.usb.UsbDevice
import android.hardware.usb.UsbManager
import android.os.Build
import android.util.Base64
import android.util.Log
import androidx.core.content.ContextCompat
import com.brother.sdk.lmprinter.BLESearchOption
import com.brother.sdk.lmprinter.Channel
import com.brother.sdk.lmprinter.NetworkSearchOption
import com.brother.sdk.lmprinter.OpenChannelError
import com.brother.sdk.lmprinter.PrintError
import com.brother.sdk.lmprinter.PrinterDriverGenerator
import com.brother.sdk.lmprinter.PrinterModel
import com.brother.sdk.lmprinter.PrinterSearcher
import com.brother.sdk.lmprinter.PrinterSearchError
import com.brother.sdk.lmprinter.PrinterSearcher.cancelNetworkSearch
import com.brother.sdk.lmprinter.setting.PrintSettings
import com.brother.sdk.lmprinter.setting.QLPrintSettings
import com.brother.sdk.lmprinter.setting.TDPrintSettings
import com.getcapacitor.JSObject
import com.getcapacitor.PermissionState
import com.getcapacitor.Plugin
import com.getcapacitor.PluginCall
import com.getcapacitor.PluginMethod
import com.getcapacitor.annotation.CapacitorPlugin
import com.getcapacitor.annotation.Permission
import com.getcapacitor.annotation.PermissionCallback
import jp.rdlabo.capacitor.plugin.brotherprint.models.BrotherPrintEvent
import jp.rdlabo.capacitor.plugin.brotherprint.models.BrotherPrintSettings


@CapacitorPlugin(
    name = "BrotherPrint",
    permissions = [
        Permission(
            alias = "bluetooth",
            strings = [
                Manifest.permission.BLUETOOTH_CONNECT,
                Manifest.permission.BLUETOOTH_SCAN,
            ],
        ),
        Permission(
            alias = "location",
            strings = [
                Manifest.permission.ACCESS_COARSE_LOCATION,
                Manifest.permission.ACCESS_FINE_LOCATION,
            ],
        ),
    ],
)
class BrotherPrint : Plugin() {
    private class NativePrinterException(val category: String, message: String) : Exception(message)

    companion object {
        // One SDK worker across plugin instances. Cancellation uses a separate thread.
        private val sdkWorker = Executors.newSingleThreadExecutor()
    }
    private val lifecycle = PrinterLifecycle<PluginCall>()

    private fun runNative(call: PluginCall, operation: () -> Unit) {
        sdkWorker.execute {
            try {
                if (!lifecycle.begin(call, call.methodName == "printImage")) { rejectCall(call, "Plugin destroyed", "CANCELLED"); return@execute }
                operation()
            }
            catch (error: NativePrinterException) { rejectOperation(call, error.message ?: "Printer operation failed", error.category, error) }
            catch (error: SecurityException) { rejectOperation(call, error.message ?: "Permission denied", "PERMISSION_DENIED", error) }
            catch (error: IllegalArgumentException) { rejectOperation(call, error.message ?: "Invalid argument", "INVALID_ARGUMENT", error) }
            catch (error: Exception) { rejectOperation(call, error.message ?: "SDK operation failed", "COMMUNICATION", error) }
            finally { lifecycle.clearCancellation() }
        }
    }
    private fun resolveCall(call: PluginCall, data: JSObject? = null) {
        lifecycle.settle(call) { destroyed ->
            if (destroyed) call.reject("Plugin destroyed", "CANCELLED")
            else if (data == null) call.resolve() else call.resolve(data)
        }
    }

    private fun rejectCall(call: PluginCall, message: String, code: String? = null, error: Exception? = null, data: JSObject? = null) {
        lifecycle.settle(call) { destroyed ->
            if (destroyed) call.reject("Plugin destroyed", "CANCELLED")
            else call.reject(message, code, error, data)
        }
    }

    private fun emit(event: String, data: JSObject?) {
        lifecycle.deliver { notifyListeners(event, data) }
    }


    private val ActionUSBPermission = "jp.rdlabo.capacitor.plugin.brotherprint.USB_PERMISSION"
    private val PERMISSION_DENIED_ERROR =
        "Unable to do call operation, user denied permission request"

    private var storeCall: PluginCall? = null
    private var usbReceiverRegistered = false

    @PluginMethod
    fun printImage(call: PluginCall) {
        runNative(call) {
            validatePrintOptions(call)
            val encodedImage = call.getString("encodedImage", "")
            if (encodedImage == "") {
                emit(BrotherPrintEvent.onPrintError.webEventName,
                    JSObject().put("code", 0).put("message", "Error - Image data is not found.")
                )
                rejectCall(call, "Error - Image data is not found.", "INVALID_ARGUMENT")
                return@runNative
            }

            val decodedString = try {
                Base64.decode(encodedImage, Base64.DEFAULT)
            } catch (error: IllegalArgumentException) {
                emit(BrotherPrintEvent.onPrintError.webEventName,
                    JSObject().put("code", 0).put("message", "Error - Invalid Base64 image data")
                )
                rejectCall(call, "Error - Invalid Base64 image data", "INVALID_ARGUMENT")
                return@runNative
            }
            val decodedByte = BitmapFactory.decodeByteArray(decodedString, 0, decodedString.size)
            if (decodedByte == null) {
                emit(BrotherPrintEvent.onPrintError.webEventName,
                    JSObject().put("code", 0).put("message", "Error - Create decodedByte From ImageData is failed.")
                )
                rejectCall(call, "Error - Create decodedByte From ImageData is failed.", "INVALID_ARGUMENT")
                return@runNative
            }

            val port: String? = call.getString("port", "wifi")
            val channelInfo: String? = call.getString("channelInfo", "")
    //        val localName: String? = call.getString("localName", "")
    //        val serialNumber: String? = call.getString("serialNumber", "")
    //        val macAddress: String? = call.getString("macAddress", "")

            lateinit var settings: PrintSettings;
            val modelName = call.getString("modelName", "QL_820NWB")!!
            // Preserve the public legacy spelling while using the native SDK model name.
            val sdkModelName = printerModels[modelName]
            val printerModel = PrinterModel.entries.find { it.name == sdkModelName }

            if (printerModel != null && modelName.startsWith("QL")) {
                settings = QLPrintSettings(printerModel);
                settings = BrotherPrintSettings().modelQLSettings(call, settings)
                settings.workPath = bridge.context.cacheDir.path;
            } else if (printerModel != null && modelName.startsWith("TD")) {
                settings = TDPrintSettings(printerModel)
                settings = BrotherPrintSettings().modelTDSettings(call, settings)
                settings.workPath = bridge.context.cacheDir.path;
            } else {
                emit(BrotherPrintEvent.onPrintError.webEventName,
                    JSObject().put("code", 0).put("message", "Error - modelName:$modelName is not supported")
                )
                rejectCall(call, "Error - modelName:$modelName is not supported")
                return@runNative;
            }

            val channel: Channel = when (port) {
                "usb" -> usbChannel(call)
                "wifi" -> Channel.newWifiChannel(channelInfo)
                "bluetooth" -> Channel.newBluetoothChannel(channelInfo, getBluetoothAdapter(bridge.context))
                "bluetoothLowEnergy" -> Channel.newBluetoothLowEnergyChannel(
                    channelInfo, bridge.context, getBluetoothAdapter(bridge.context)
                )
                else -> {
                    emit(BrotherPrintEvent.onPrintError.webEventName,
                        JSObject().put("code", 0).put("message", "Error - port:$port is not supported")
                    )
                    rejectCall(call, "Error - port:$port is not supported")
                    return@runNative
                }
            }

            val result = PrinterDriverGenerator.openChannel(channel)
            if (result.error.code != OpenChannelError.ErrorCode.NoError) {
                this.emit(BrotherPrintEvent.onPrintFailedCommunication.webEventName,
                    JSObject().put("code", result.error.code.ordinal).put("nativeCode", result.error.code.name).put("category", "COMMUNICATION")
                        .put("message", result.error.code.toString())
                )
                rejectCall(call, "Error - openChannel: " + result.error.code.toString(), "COMMUNICATION", null, JSObject().put("nativeCode", result.error.code.name))
                return@runNative
            }

            val printerDriver = result.driver

            val printError: PrintError = try {
                printerDriver.printImage(decodedByte, settings)
            } finally { printerDriver.closeChannel() }

            if (printError.code != PrintError.ErrorCode.NoError) {
                emit(
                    BrotherPrintEvent.onPrintError.webEventName,
                    JSObject().put("code", printError.code.ordinal).put("nativeCode", printError.code.name).put("category", "PRINT_FAILED")
                        .put("message", printError.code.toString())
                )
                rejectCall(call, "Error - Print Image: " + printError.code.toString(), "PRINT_FAILED", null, JSObject().put("nativeCode", printError.code.name))
                return@runNative
            }


            emit(
                BrotherPrintEvent.onPrint.webEventName,
                JSObject()
            )
            resolveCall(call)
        }
    }

    private fun rejectOperation(call: PluginCall, message: String, category: String, error: Exception? = null) {
        reportPrinterFailure(call.methodName, message, category, { info ->
            val payload = JSObject()
            info.forEach { (key, value) -> payload.put(key, value) }
            emit(BrotherPrintEvent.onPrintError.webEventName, payload)
        }, { rejectCall(call, message, category, error) })
    }

    private fun soleUsbDevice(): UsbDevice {
        val manager = bridge.context.getSystemService(Context.USB_SERVICE) as UsbManager
        val devices = manager.deviceList.values.filter { it.vendorId == 0x04f9 }
        if (devices.isEmpty()) throw NativePrinterException("NOT_FOUND", "No Brother USB device connected")
        if (devices.size > 1) throw NativePrinterException("UNSUPPORTED", "Multiple Brother USB devices are unsupported")
        requireSingleBrotherUsb(manager.deviceList.values.map { it.vendorId })
        val device = devices.single()
        if (!manager.hasPermission(device)) throw SecurityException("USB permission denied; search again")
        return device
    }

    private fun currentUsbToken(): String {
        val device = soleUsbDevice()
        return usbChannelToken(device.deviceName, device.vendorId, device.productId, device.serialNumber)
    }

    private fun usbChannel(call: PluginCall): Channel {
        val token = currentUsbToken()
        require(token == call.getString("channelInfo", "")) { "USB destination changed; search again" }
        val result = PrinterSearcher.startUSBSearch(bridge.context)
        require(result.error.code == PrinterSearchError.ErrorCode.NoError && result.channels.size == 1) { "USB discovery must identify exactly one printer" }
        require(currentUsbToken() == token) { "USB destination changed during discovery; search again" }
        return result.channels.single()
    }

    private fun validateChannel(call: PluginCall) {
        val port = call.getString("port", "wifi")
        require(port in listOf("wifi", "usb", "bluetooth", "bluetoothLowEnergy")) { "Unsupported port" }
        require(!call.getString("channelInfo", "").isNullOrBlank()) { "channelInfo is required" }
    }

    private fun validatePrintOptions(call: PluginCall) {
        validateChannel(call)
        validatePrinterOptions(call.data.keys().asSequence().associateWith { call.data.opt(it) })
    }

    private fun rejectSearch(call: PluginCall, operation: String, code: PrinterSearchError.ErrorCode) {
        val category = when (code) {
            PrinterSearchError.ErrorCode.Canceled -> "CANCELLED"
            PrinterSearchError.ErrorCode.NotPermitted -> "PERMISSION_DENIED"
            PrinterSearchError.ErrorCode.InterfaceUnsupported -> "UNSUPPORTED"
            PrinterSearchError.ErrorCode.AlreadySearching -> "BUSY"
            else -> "COMMUNICATION"
        }
        rejectCall(call, "$operation: ${code.name}", category, null, JSObject().put("nativeCode", code.name))
    }

    private fun getBluetoothAdapter(context: Context): BluetoothAdapter? {
        val manager = context.getSystemService(Context.BLUETOOTH_SERVICE) as BluetoothManager
        return manager.adapter
    }


    @PluginMethod
    fun isChannelAvailable(call: PluginCall) {
        if (call.getString("port") in listOf("bluetooth", "bluetoothLowEnergy") && !isBluetoothPermissionGranted()) {
            rejectCall(call, "Bluetooth permission denied", "PERMISSION_DENIED")
            return
        }
        val port: String? = call.getString("port", "wifi")
        val channelInfo: String? = call.getString("channelInfo", "")

        runNative(call) {
            validateChannel(call)
            val channel: Channel = when (port) {
                "usb" -> usbChannel(call)
                "wifi" -> Channel.newWifiChannel(channelInfo)
                "bluetooth" -> Channel.newBluetoothChannel(channelInfo, getBluetoothAdapter(bridge.context))
                "bluetoothLowEnergy" -> Channel.newBluetoothLowEnergyChannel(
                    channelInfo, bridge.context, getBluetoothAdapter(bridge.context)
                )
                else -> {
                    rejectCall(call, "Error - port:$port is not supported")
                    return@runNative
                }
            }

            val result = PrinterDriverGenerator.openChannel(channel)
            if (result.error.code != OpenChannelError.ErrorCode.NoError) {
                resolveCall(call, JSObject().put("result", false))
                return@runNative
            }
            val printerDriver = result.driver
            printerDriver.closeChannel()
            resolveCall(call, JSObject().put("result", true))
        }
    }

    @PluginMethod
    fun search(call: PluginCall) {
        if (lifecycle.isDestroyed) { rejectCall(call, "Plugin destroyed", "CANCELLED"); return }
        val duration = call.getDouble("searchDuration", 15.0) ?: 0.0
        if (!duration.isFinite() || duration <= 0 || duration % 1 != 0.0 || duration > Int.MAX_VALUE) {
            rejectCall(call, "searchDuration must be a positive integer", "INVALID_ARGUMENT")
            return
        }
        when(call.getString("port", "wifi")) {
            "usb" -> this.searchUsbPrinter(call)
            "wifi" -> this.searchWiFiPrinter(call)
            "bluetooth" -> this.checkBLEChannel(call)
            "bluetoothLowEnergy" -> this.searchBLEPrinter(call)
            else -> rejectCall(call, "port is not 'wifi' | 'bluetooth' | 'bluetoothLowEnergy'")
        }
    }

    @Synchronized
    private fun searchUsbPrinter(call: PluginCall) {
        if (lifecycle.isDestroyed) { rejectCall(call, "Plugin destroyed", "CANCELLED"); return }
        if (this.storeCall != null) {
            rejectCall(call, "Error - USB permission request is already pending", "BUSY")
            return
        }
        if (!this.requestUsbPermission(call)) {
            return
        }

        runNative(call) {
            val token = currentUsbToken()
            val result = PrinterSearcher.startUSBSearch(bridge.context)

            if (result.error.code != PrinterSearchError.ErrorCode.NoError) {
                rejectSearch(call, "startUSBSearch", result.error.code)
                return@runNative
            }

            require(currentUsbToken() == token) { "USB destination changed during discovery; search again" }
            for (channel in result.channels){
                this.emit(
                    BrotherPrintEvent.onPrinterAvailable.webEventName,
                    this.chanelToPrinter("usb", channel, token)
                );
            }
            resolveCall(call);
        }
    }

    private fun searchWiFiPrinter(call: PluginCall) {
        Log.d("brother", "searchWiFiPrinter")
        runNative(call) {
            if (!lifecycle.registerCancellation("wifi") { cancelNetworkSearch() }) {
                rejectCall(call, "Plugin destroyed", "CANCELLED"); return@runNative
            }
            val intDuration: Int = call.getInt("searchDuration") ?: 15 ;
            val option = NetworkSearchOption(intDuration.toDouble(), false);
            val result = PrinterSearcher.startNetworkSearch(bridge.context, option){ channel ->
                run {
                    Log.d("brother", this.chanelToPrinter("wifi", channel).toString())
                    this.emit(
                        BrotherPrintEvent.onPrinterAvailable.webEventName,
                        this.chanelToPrinter("wifi", channel)
                    );
                }
            }
            lifecycle.clearCancellation()
            if (result.error.code != PrinterSearchError.ErrorCode.NoError) {
                rejectSearch(call, "startNetworkSearch", result.error.code)
                return@runNative
            }
            resolveCall(call);
        }
    }

    private fun checkBLEChannel(call: PluginCall) {
        if (!isBluetoothPermissionGranted()) {
            requestPermissionForAlias("bluetooth", call, "permissionCallback");
        } else {
            Log.d("brother", "checkBLEChannel")
            runNative(call) {
                val result = PrinterSearcher.startBluetoothSearch(bridge.context)
                if (result.error.code != PrinterSearchError.ErrorCode.NoError) {
                    rejectSearch(call, "startBluetoothSearch", result.error.code)
                    return@runNative
                }
                for (channel in result.channels){
                    if (!matchesBluetoothPrinterFilter(call.getBoolean("bluetoothPrintersOnly", false) == true) {
                        val device = getBluetoothAdapter(bridge.context)?.getRemoteDevice(channel.channelInfo)
                        device?.bluetoothClass?.deviceClass
                    }) continue
                    Log.d("brother", this.chanelToPrinter("bluetooth", channel).toString())
                    this.emit(BrotherPrintEvent.onPrinterAvailable.webEventName, this.chanelToPrinter("bluetooth", channel));
                }
                resolveCall(call);
            }
        }
    }

    private fun searchBLEPrinter(call: PluginCall) {
        if (!isBluetoothPermissionGranted()) {
            requestPermissionForAlias("bluetooth", call, "permissionCallback");
            return;
        } else if (Build.VERSION.SDK_INT < Build.VERSION_CODES.S && !isLocationPermissionGranted()) {
            requestPermissionForAlias("location", call, "locationPermissionCallback");
            return;
        } else {
            Log.d("brother", "searchBLEPrinter")
            runNative(call) {
                if (!lifecycle.registerCancellation("bluetooth") { PrinterSearcher.cancelBLESearch() }) {
                    rejectCall(call, "Plugin destroyed", "CANCELLED"); return@runNative
                }
                val intDuration: Int = call.getInt("searchDuration") ?: 15 ;
                val option = BLESearchOption(intDuration.toDouble())
                val result = PrinterSearcher.startBLESearch(bridge.context, option){ channel ->
                    run {
                        Log.d("brother", this.chanelToPrinter("bluetoothLowEnergy", channel).toString())
                        this.emit(
                            BrotherPrintEvent.onPrinterAvailable.webEventName,
                            this.chanelToPrinter("bluetoothLowEnergy", channel)
                        );
                    }
                }
                lifecycle.clearCancellation();
                if (result.error.code != PrinterSearchError.ErrorCode.NoError) {
                    rejectSearch(call, "startBLESearch", result.error.code)
                    return@runNative
                }
                resolveCall(call);
            }
        }
    }

    private fun chanelToPrinter(port: String, channel: Channel, usbToken: String? = null): JSObject? {
        Log.d("brother", channel.toString());
        val modelName = channel.extraInfo[Channel.ExtraInfoKey.ModelName] ?: ""
        val serialNumber = channel.extraInfo[Channel.ExtraInfoKey.SerialNumber] ?: ""
        val macAddress = channel.extraInfo[Channel.ExtraInfoKey.MACAddress] ?: ""
        val nodeName = channel.extraInfo[Channel.ExtraInfoKey.NodeName] ?: ""
        val location = channel.extraInfo[Channel.ExtraInfoKey.Location] ?: ""
        val channelInfo = bridgeChannelInfo(port, channel.channelInfo, usbToken)

        return JSObject()
            .put("port", port)
            .put("modelName", modelName)
            .put("serialNumber", serialNumber)
            .put("macAddress", macAddress)
            .put("nodeName", nodeName)
            .put("location", location)
            .put("channelInfo", channelInfo)
    }

    @PluginMethod
    fun cancelSearchWiFiPrinter(call: PluginCall) {
        Thread {
            lifecycle.cancel("wifi")
            resolveCall(call)
        }.start()
    }

    @PluginMethod
    fun cancelSearchBluetoothPrinter(call: PluginCall) {
        Thread {
            lifecycle.cancel("bluetooth")
            resolveCall(call)
        }.start()
    }

    @PermissionCallback
    private fun permissionCallback(call: PluginCall) {
        if (!isBluetoothPermissionGranted()) {
            Log.d("brother", "!isBluetoothPermissionGranted()")
            rejectCall(call, PERMISSION_DENIED_ERROR, "PERMISSION_DENIED")
            return
        }
        when (call.methodName) {
            "search" -> this.search(call)
        }
    }

    @PermissionCallback
    private fun locationPermissionCallback(call: PluginCall) {
        if (!isLocationPermissionGranted()) {
            Log.d("brother", "!isLocationPermissionGranted()")
            rejectCall(call, PERMISSION_DENIED_ERROR, "PERMISSION_DENIED")
            return
        }
        this.search(call)
    }

    private val usbReceiver: BroadcastReceiver = object : BroadcastReceiver() {
        override fun onReceive(context: Context?, intent: Intent?) {
            synchronized(this@BrotherPrint) {
                if (intent?.action != ActionUSBPermission) return
                val call = storeCall ?: return
                storeCall = null
                if (intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false)) {
                    searchUsbPrinter(call)
                } else {
                    rejectCall(call, "USB permission denied", "PERMISSION_DENIED");
                }
            }
        }
    }

    private fun requestUsbPermission(call: PluginCall): Boolean {
        val usbManager = bridge.context.getSystemService(Context.USB_SERVICE) as UsbManager
        val devices = usbManager.deviceList.values.filter { it.vendorId == 0x04f9 }
        if (devices.size != 1) {
            rejectCall(call, if (devices.isEmpty()) "No Brother USB printer connected" else "Multiple Brother USB devices are unsupported", if (devices.isEmpty()) "NOT_FOUND" else "UNSUPPORTED")
            return false
        }
        val connectDevice = devices.single()
        if (usbManager.hasPermission(connectDevice)) return true

        val permissionIntent = PendingIntent.getBroadcast(
            bridge.context, 0, Intent(ActionUSBPermission), PendingIntent.FLAG_IMMUTABLE
        )

        if (!usbReceiverRegistered) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
                bridge.context.registerReceiver(
                    usbReceiver, IntentFilter(ActionUSBPermission),
                    Context.RECEIVER_NOT_EXPORTED
                )
            } else {
                ContextCompat.registerReceiver(
                    bridge.context,
                    usbReceiver,
                    IntentFilter(ActionUSBPermission),
                    ContextCompat.RECEIVER_NOT_EXPORTED
                )
            }
            usbReceiverRegistered = true
        }

        storeCall = call
        usbManager.requestPermission(connectDevice, permissionIntent)
        return false
    }

    @Synchronized
    override fun handleOnDestroy() {
        lifecycle.destroy()
        // Never queue cancellation behind the blocking SDK search itself.
        Thread { lifecycle.cancel() }.start()
        val call = storeCall
        storeCall = null
        if (call != null) rejectCall(call, "Plugin destroyed while waiting for USB permission", "CANCELLED")
        if (usbReceiverRegistered) {
            bridge.context.unregisterReceiver(usbReceiver)
            usbReceiverRegistered = false
        }
        super.handleOnDestroy()
    }

    private fun isBluetoothPermissionGranted(): Boolean {
        return Build.VERSION.SDK_INT < Build.VERSION_CODES.S || getPermissionState("bluetooth") == PermissionState.GRANTED
    }

    private fun isLocationPermissionGranted(): Boolean {
        return getPermissionState("location") == PermissionState.GRANTED
    }
}
