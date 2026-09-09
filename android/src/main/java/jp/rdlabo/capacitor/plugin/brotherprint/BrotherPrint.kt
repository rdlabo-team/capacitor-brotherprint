package jp.rdlabo.capacitor.plugin.brotherprint

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
    private var cancelRoutineWiFi: (() -> Unit)? = null
    private var cancelRoutineBluetooth: (() -> Unit)? = null

    private val ActionUSBPermission = "jp.rdlabo.capacitor.plugin.brotherprint.USB_PERMISSION"
    private val PERMISSION_DENIED_ERROR =
        "Unable to do call operation, user denied permission request"

    private var storeCall: PluginCall? = null
    private var usbReceiverRegistered = false

    @PluginMethod
    fun printImage(call: PluginCall) {
        val encodedImage = call.getString("encodedImage", "")
        if (encodedImage == "") {
            notifyListeners(BrotherPrintEvent.onPrintError.webEventName,
                JSObject().put("code", 0).put("message", "Error - Image data is not found.")
            )
            call.reject("Error - Image data is not found.")
            return
        }

        val decodedString = try {
            Base64.decode(encodedImage, Base64.DEFAULT)
        } catch (error: IllegalArgumentException) {
            notifyListeners(BrotherPrintEvent.onPrintError.webEventName,
                JSObject().put("code", 0).put("message", "Error - Invalid Base64 image data")
            )
            call.reject("Error - Invalid Base64 image data")
            return
        }
        val decodedByte = BitmapFactory.decodeByteArray(decodedString, 0, decodedString.size)
        if (decodedByte == null) {
            notifyListeners(BrotherPrintEvent.onPrintError.webEventName,
                JSObject().put("code", 0).put("message", "Error - Create decodedByte From ImageData is failed.")
            )
            call.reject("Error - Create decodedByte From ImageData is failed.")
            return
        }

        val port: String? = call.getString("port", "wifi")
        val channelInfo: String? = call.getString("channelInfo", "")
//        val localName: String? = call.getString("localName", "")
//        val serialNumber: String? = call.getString("serialNumber", "")
//        val macAddress: String? = call.getString("macAddress", "")

        lateinit var settings: PrintSettings;
        val modelName = call.getString("modelName", "QL_820NWB")!!
        val printerModel = PrinterModel.entries.find { it.name == modelName }

        if (printerModel != null && modelName.startsWith("QL")) {
            settings = QLPrintSettings(printerModel);
            settings = BrotherPrintSettings().modelQLSettings(call, settings)
            settings.workPath = bridge.context.cacheDir.path;
        } else if (printerModel != null && modelName.startsWith("TD")) {
            settings = TDPrintSettings(printerModel)
            settings = BrotherPrintSettings().modelTDSettings(call, settings)
            settings.workPath = bridge.context.cacheDir.path;
        } else {
            notifyListeners(BrotherPrintEvent.onPrintError.webEventName,
                JSObject().put("code", 0).put("message", "Error - modelName:$modelName is not supported")
            )
            call.reject("Error - modelName:$modelName is not supported")
            return;
        }

        Thread {
            val channel: Channel = when (port) {
                "usb" -> Channel.newUsbChannel(bridge.context.getSystemService(Context.USB_SERVICE) as UsbManager)
                "wifi" -> Channel.newWifiChannel(channelInfo)
                "bluetooth" -> Channel.newBluetoothChannel(channelInfo, getBluetoothAdapter(bridge.context))
                "bluetoothLowEnergy" -> Channel.newBluetoothLowEnergyChannel(
                    channelInfo, bridge.context, getBluetoothAdapter(bridge.context)
                )
                else -> {
                    notifyListeners(BrotherPrintEvent.onPrintError.webEventName,
                        JSObject().put("code", 0).put("message", "Error - port:$port is not supported")
                    )
                    call.reject("Error - port:$port is not supported")
                    return@Thread
                }
            }

            val result = PrinterDriverGenerator.openChannel(channel)
            if (result.error.code != OpenChannelError.ErrorCode.NoError) {
                this.notifyListeners(BrotherPrintEvent.onPrintFailedCommunication.webEventName,
                    JSObject().put("code", result.error.code)
                        .put("message", result.error.code.toString())
                )
                call.reject("Error - openChannel: " + result.error.code.toString())
                return@Thread
            }

            val printerDriver = result.driver

            val printError: PrintError = printerDriver.printImage(decodedByte, settings)

            if (printError.code != PrintError.ErrorCode.NoError) {
                printerDriver.closeChannel()
                notifyListeners(
                    BrotherPrintEvent.onPrintError.webEventName,
                    JSObject().put("code", printError.code)
                        .put("message", printError.code.toString())
                )
                call.reject("Error - Print Image: " + printError.code.toString())
                return@Thread
            }

            printerDriver.closeChannel()

            notifyListeners(
                BrotherPrintEvent.onPrint.webEventName,
                JSObject()
            )
            call.resolve()
        }.start()
    }

    private fun getBluetoothAdapter(context: Context): BluetoothAdapter? {
        val manager = context.getSystemService(Context.BLUETOOTH_SERVICE) as BluetoothManager
        return manager.adapter
    }


    @PluginMethod
    fun isChannelAvailable(call: PluginCall) {
        if (call.getString("port") in listOf("bluetooth", "bluetoothLowEnergy") && !isBluetoothPermissionGranted()) {
            call.resolve(JSObject().put("result", false))
            return
        }
        val port: String? = call.getString("port", "wifi")
        val channelInfo: String? = call.getString("channelInfo", "")

        Thread {
            val channel: Channel = when (port) {
                "usb" -> Channel.newUsbChannel(bridge.context.getSystemService(Context.USB_SERVICE) as UsbManager)
                "wifi" -> Channel.newWifiChannel(channelInfo)
                "bluetooth" -> Channel.newBluetoothChannel(channelInfo, getBluetoothAdapter(bridge.context))
                "bluetoothLowEnergy" -> Channel.newBluetoothLowEnergyChannel(
                    channelInfo, bridge.context, getBluetoothAdapter(bridge.context)
                )
                else -> {
                    call.reject("Error - port:$port is not supported")
                    return@Thread
                }
            }

            val result = PrinterDriverGenerator.openChannel(channel)
            if (result.error.code != OpenChannelError.ErrorCode.NoError) {
                call.resolve(JSObject().put("result", false))
                return@Thread
            }
            val printerDriver = result.driver
            printerDriver.closeChannel()
            call.resolve(JSObject().put("result", true))
        }.start()
    }

    @PluginMethod
    fun search(call: PluginCall) {
        when(call.getString("port", "wifi")) {
            "usb" -> this.searchUsbPrinter(call)
            "wifi" -> this.searchWiFiPrinter(call)
            "bluetooth" -> this.checkBLEChannel(call)
            "bluetoothLowEnergy" -> this.searchBLEPrinter(call)
            else -> call.reject("port is not 'wifi' | 'bluetooth' | 'bluetoothLowEnergy'")
        }
    }

    @Synchronized
    private fun searchUsbPrinter(call: PluginCall) {
        if (this.storeCall != null) {
            call.reject("Error - USB permission request is already pending")
            return
        }
        if (!this.requestUsbPermission(call)) {
            return
        }

        Thread {
            val result = PrinterSearcher.startUSBSearch(bridge.context)

            if (result.error.code != PrinterSearchError.ErrorCode.NoError) {
                call.reject("Error - startUSBSearch: " + result.error.code.toString())
                return@Thread
            }

            for (channel in result.channels){
                this.notifyListeners(
                    BrotherPrintEvent.onPrinterAvailable.webEventName,
                    this.chanelToPrinter("usb", channel)
                );
            }
            call.resolve();
        }.start()
    }

    private fun searchWiFiPrinter(call: PluginCall) {
        Log.d("brother", "searchWiFiPrinter")
        Thread {
            this.cancelRoutineWiFi = {
                cancelNetworkSearch()
            }
            val intDuration: Int = call.getInt("searchDuration") ?: 15 ;
            val option = NetworkSearchOption(intDuration.toDouble(), false);
            val result = PrinterSearcher.startNetworkSearch(bridge.context, option){ channel ->
                run {
                    Log.d("brother", this.chanelToPrinter("wifi", channel).toString())
                    this.notifyListeners(
                        BrotherPrintEvent.onPrinterAvailable.webEventName,
                        this.chanelToPrinter("wifi", channel)
                    );
                }
            }
            this.cancelRoutineWiFi = null
            if (result.error.code != PrinterSearchError.ErrorCode.NoError) {
                call.reject("Error - startNetworkSearch: " + result.error.code.toString())
                return@Thread
            }
            call.resolve();
        }.start()
    }

    private fun checkBLEChannel(call: PluginCall) {
        if (!isBluetoothPermissionGranted()) {
            requestPermissionForAlias("bluetooth", call, "permissionCallback");
        } else {
            Log.d("brother", "checkBLEChannel")
            Thread {
                val result = PrinterSearcher.startBluetoothSearch(bridge.context)
                if (result.error.code != PrinterSearchError.ErrorCode.NoError) {
                    call.reject("Error - startBluetoothSearch: " + result.error.code.toString())
                    return@Thread
                }
                for (channel in result.channels){
                    if (!matchesBluetoothPrinterFilter(call.getBoolean("bluetoothPrintersOnly", false) == true) {
                        val device = getBluetoothAdapter(bridge.context)?.getRemoteDevice(channel.channelInfo)
                        device?.bluetoothClass?.deviceClass
                    }) continue
                    Log.d("brother", this.chanelToPrinter("bluetooth", channel).toString())
                    this.notifyListeners(BrotherPrintEvent.onPrinterAvailable.webEventName, this.chanelToPrinter("bluetooth", channel));
                }
                call.resolve();
            }.start()
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
            Thread {
                this.cancelRoutineBluetooth = {
                    PrinterSearcher.cancelBLESearch()
                }
                val intDuration: Int = call.getInt("searchDuration") ?: 15 ;
                val option = BLESearchOption(intDuration.toDouble())
                val result = PrinterSearcher.startBLESearch(bridge.context, option){ channel ->
                    run {
                        Log.d("brother", this.chanelToPrinter("bluetoothLowEnergy", channel).toString())
                        this.notifyListeners(
                            BrotherPrintEvent.onPrinterAvailable.webEventName,
                            this.chanelToPrinter("bluetoothLowEnergy", channel)
                        );
                    }
                }
                this.cancelRoutineBluetooth = null;
                if (result.error.code != PrinterSearchError.ErrorCode.NoError) {
                    call.reject("Error - startBLESearch: " + result.error.code.toString())
                    return@Thread
                }
                call.resolve();
            }.start()
        }
    }

    private fun chanelToPrinter(port: String, channel: Channel): JSObject? {
        Log.d("brother", channel.toString());
        val modelName = channel.extraInfo[Channel.ExtraInfoKey.ModelName] ?: ""
        val serialNumber = channel.extraInfo[Channel.ExtraInfoKey.SerialNumber] ?: ""
        val macAddress = channel.extraInfo[Channel.ExtraInfoKey.MACAddress] ?: ""
        val nodeName = channel.extraInfo[Channel.ExtraInfoKey.NodeName] ?: ""
        val location = channel.extraInfo[Channel.ExtraInfoKey.Location] ?: ""
        val channelInfo = channel.channelInfo

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
            this.cancelRoutineWiFi?.invoke()
            this.cancelRoutineWiFi = null
            call.resolve()
        }.start()
    }

    @PluginMethod
    fun cancelSearchBluetoothPrinter(call: PluginCall) {
        Thread {
            this.cancelRoutineBluetooth?.invoke()
            this.cancelRoutineBluetooth = null
            call.resolve()
        }.start()
    }

    @PermissionCallback
    private fun permissionCallback(call: PluginCall) {
        if (!isBluetoothPermissionGranted()) {
            Log.d("brother", "!isBluetoothPermissionGranted()")
            call.reject(PERMISSION_DENIED_ERROR)
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
            call.reject(PERMISSION_DENIED_ERROR)
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
                    call.reject("Error - usbReceiver can't current receiver");
                }
            }
        }
    }

    private fun requestUsbPermission(call: PluginCall): Boolean {
        var connectDevice: UsbDevice? = null
        val usbManager = bridge.context.getSystemService(Context.USB_SERVICE) as UsbManager
        for (device in usbManager.deviceList.values) {
            connectDevice = device
        }

        if (connectDevice == null) {
            call.reject("Error - connection failed: device not found")
            return false
        }
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
        val call = storeCall
        storeCall = null
        call?.reject("Error - plugin destroyed while waiting for USB permission")
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
