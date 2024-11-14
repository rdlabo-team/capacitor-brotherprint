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
import com.brother.sdk.lmprinter.BLESearchOption
import com.brother.sdk.lmprinter.Channel
import com.brother.sdk.lmprinter.NetworkSearchOption
import com.brother.sdk.lmprinter.OpenChannelError
import com.brother.sdk.lmprinter.PrintError
import com.brother.sdk.lmprinter.PrinterDriverGenerator
import com.brother.sdk.lmprinter.PrinterModel
import com.brother.sdk.lmprinter.PrinterSearcher
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
                Manifest.permission.BLUETOOTH,
                Manifest.permission.BLUETOOTH_ADMIN,
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

    @PluginMethod
    fun printImage(call: PluginCall) {
        val encodedImage = call.getString("encodedImage", "")
        if (encodedImage == "") {
            call.reject("Error - Image data is not found.")
            return
        }

        val decodedString = Base64.decode(encodedImage, Base64.DEFAULT)
        val decodedByte = BitmapFactory.decodeByteArray(decodedString, 0, decodedString.size)

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
    fun search(call: PluginCall) {
        when(call.getString("port", "wifi")) {
            "usb" -> this.searchUsbPrinter(call)
            "wifi" -> this.searchWiFiPrinter(call)
            "bluetooth" -> this.checkBLEChannel(call)
            "bluetoothLowEnergy" -> this.searchBLEPrinter(call)
            else -> call.reject("port is not 'wifi' | 'bluetooth' | 'bluetoothLowEnergy'")
        }
    }

    private fun searchUsbPrinter(call: PluginCall) {
        if (!this.requestUsbPermission(call)) {
            this.storeCall = call;
            return
        }
        this.storeCall = null

        Thread {
            val result = PrinterSearcher.startUSBSearch(bridge.context)

            when (result.error.code) {
                com.brother.sdk.lmprinter.PrinterSearchError.ErrorCode.NoError -> {
                }
                com.brother.sdk.lmprinter.PrinterSearchError.ErrorCode.NotPermitted -> {
                    // TODO: has error
                }
                com.brother.sdk.lmprinter.PrinterSearchError.ErrorCode.Canceled,
                com.brother.sdk.lmprinter.PrinterSearchError.ErrorCode.InterfaceInactive,
                com.brother.sdk.lmprinter.PrinterSearchError.ErrorCode.InterfaceUnsupported,
                com.brother.sdk.lmprinter.PrinterSearchError.ErrorCode.AlreadySearching,
                com.brother.sdk.lmprinter.PrinterSearchError.ErrorCode.UnknownError -> {
                }
                null -> {}
            }

            for (channel in result.channels){
                this.notifyListeners(
                    BrotherPrintEvent.onPrinterAvailable.webEventName,
                    this.chanelToPrinter("usb", channel)
                );
            }
        }.start()
        call.resolve();
    }

    private fun searchWiFiPrinter(call: PluginCall) {
        Log.d("brother", "searchWiFiPrinter")
        Thread {
            this.cancelRoutineWiFi = {
                cancelNetworkSearch()
            }
            val intDuration: Int = call.getInt("searchDuration") ?: 15 ;
            val option = NetworkSearchOption(intDuration.toDouble(), false);
            PrinterSearcher.startNetworkSearch(bridge.context, option){ channel ->
                run {
                    Log.d("brother", this.chanelToPrinter("wifi", channel).toString())
                    this.notifyListeners(
                        BrotherPrintEvent.onPrinterAvailable.webEventName,
                        this.chanelToPrinter("wifi", channel)
                    );
                }
            }
            this.cancelRoutineWiFi = null
        }.start()
        call.resolve();
    }

    private fun checkBLEChannel(call: PluginCall) {
        if (!isBluetoothPermissionGranted()) {
            requestPermissionForAlias("bluetooth", call, "permissionCallback");
        } else {
            Log.d("brother", "checkBLEChannel")
            Thread {
                for (channel in PrinterSearcher.startBluetoothSearch(bridge.context).channels){
                    Log.d("brother", this.chanelToPrinter("bluetooth", channel).toString())
                    this.notifyListeners(BrotherPrintEvent.onPrinterAvailable.webEventName, this.chanelToPrinter("bluetooth", channel));
                }
            }.start()
            call.resolve();
        }
    }

    private fun searchBLEPrinter(call: PluginCall) {
        if (!isBluetoothPermissionGranted()) {
            requestPermissionForAlias("bluetooth", call, "permissionCallback");
            return;
        } else if (!isLocationPermissionGranted()) {
            requestPermissionForAlias("location", call, "permissionCallback");
            return;
        } else {
            Log.d("brother", "searchBLEPrinter")
            Thread {
                this.cancelRoutineBluetooth = {
                    cancelNetworkSearch()
                }
                val intDuration: Int = call.getInt("searchDuration") ?: 15 ;
                val option = BLESearchOption(intDuration.toDouble())
                PrinterSearcher.startBLESearch(bridge.context, option){ channel ->
                    run {
                        Log.d("brother", this.chanelToPrinter("bluetoothLowEnergy", channel).toString())
                        this.notifyListeners(
                            BrotherPrintEvent.onPrinterAvailable.webEventName,
                            this.chanelToPrinter("bluetoothLowEnergy", channel)
                        );
                    }
                }
                this.cancelRoutineBluetooth = null;
            }.start()
            call.resolve();
        }
    }

    private fun chanelToPrinter(port: String, channel: Channel): JSObject? {
        Log.d("brother", channel.toString());
        val modelName = channel.extraInfo[Channel.ExtraInfoKey.ModelName] ?: ""
        val serialNumber = channel.extraInfo[Channel.ExtraInfoKey.SerialNubmer] ?: ""
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
        if (!isLocationPermissionGranted()) {
            Log.d("brother", "!isLocationPermissionGranted()")
            call.reject(PERMISSION_DENIED_ERROR)
            return
        }
        when (call.methodName) {
            "search" -> this.search(call)
        }
    }

    /**
     * TODO: This is not called for in spite of registration.
     * Therefore, it is now necessary for the user to run it again after permission is granted.
     */
    private val usbReceiver: BroadcastReceiver = object : BroadcastReceiver() {
        override fun onReceive(context: Context?, intent: Intent?) {
            if (intent?.action == ActionUSBPermission && intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false)) {
                storeCall?.let { searchUsbPrinter(it) }
            } else {
                storeCall?.reject("Error - usbReceiver can't current receiver");
            }
        }
    }

    private fun requestUsbPermission(call: PluginCall): Boolean {
        val permissionIntent = PendingIntent.getBroadcast(
            bridge.context, 0, Intent(ActionUSBPermission), PendingIntent.FLAG_IMMUTABLE
        )

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
            bridge.context.registerReceiver(
                usbReceiver, IntentFilter(ActionUSBPermission),
                Context.RECEIVER_NOT_EXPORTED
            )
        } else {
            bridge.context.registerReceiver(
                usbReceiver, IntentFilter(ActionUSBPermission)
            )
        }

        var connectDevice: UsbDevice? = null
        val usbManager = bridge.context.getSystemService(Context.USB_SERVICE) as UsbManager
        for (device in usbManager.deviceList.values) {
            connectDevice = device
        }

        if (connectDevice == null) {
            call.reject("Error - connection failed: device not found")
            return false
        }

        usbManager.requestPermission(connectDevice, permissionIntent)

        return usbManager.hasPermission(connectDevice)
    }

    private fun isBluetoothPermissionGranted(): Boolean {
        return getPermissionState("bluetooth") == PermissionState.GRANTED
    }

    private fun isLocationPermissionGranted(): Boolean {
        return getPermissionState("location") == PermissionState.GRANTED
    }
}
