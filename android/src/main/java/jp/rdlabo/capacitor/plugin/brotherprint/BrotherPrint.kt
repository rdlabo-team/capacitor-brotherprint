package jp.rdlabo.capacitor.plugin.brotherprint

import android.Manifest
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothManager
import android.content.Context
import android.graphics.BitmapFactory
import android.hardware.usb.UsbManager
import android.util.Base64
import android.util.Log
import com.brother.ptouch.sdk.Printer
import com.brother.ptouch.sdk.PrinterInfo
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
    private val PERMISSION_DENIED_ERROR =
        "Unable to do call operation, user denied permission request"

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
        val localName: String? = call.getString("localName", "")
        val ipAddress: String? = call.getString("ipAddress", "")
        val serialNumber: String? = call.getString("serialNumber", "")
        val macAddress: String? = call.getString("macAddress", "")

        lateinit var settings: PrintSettings;
        val modelName = call.getString("modelName", "QL_820NWB")!!
        val printerModel = PrinterModel.entries.find { it.name == modelName }

        if (modelName.startsWith("QL")) {
            settings = QLPrintSettings(printerModel);
            settings = BrotherPrintSettings().modelQLSettings(call, settings)
            settings.workPath = bridge.context.cacheDir.path;
        } else if (modelName.startsWith("TD")) {
            settings = TDPrintSettings(printerModel)
            settings = BrotherPrintSettings().modelTDSettings(call, settings)
            settings.workPath = bridge.context.cacheDir.path;
        } else {
            this.notifyListeners(BrotherPrintEvent.onPrintFailedCommunication.webEventName,
                JSObject().put("code", 0)
                    .put("message", "Error - $modelName is not supported")
            )
            call.reject("Error - $modelName is not supported")
            return;
        }

        try {
            Thread {
                val channel: Channel = when (port) {
                    "usb" -> Channel.newUsbChannel(bridge.context.getSystemService(Context.USB_SERVICE) as UsbManager)
                    "wifi" -> Channel.newWifiChannel(ipAddress)
                    "bluetooth" -> Channel.newBluetoothChannel(macAddress, getBluetoothAdapter(bridge.context))
                    "bluetoothLowEnergy" -> Channel.newBluetoothLowEnergyChannel(
                        localName, context, getBluetoothAdapter(context)
                    )
                    else -> {
                        this.notifyListeners(BrotherPrintEvent.onPrintFailedCommunication.webEventName,
                            JSObject().put("code", 0)
                                .put("message", "Error - $port is not supported")
                        )
                        call.reject("Error - $port is not supported")
                        return@Thread
                    }
                }

                val result = PrinterDriverGenerator.openChannel(channel)
                if (result.error.code != OpenChannelError.ErrorCode.NoError) {
                    this.notifyListeners(BrotherPrintEvent.onPrintFailedCommunication.webEventName,
                        JSObject().put("code", result.error.code)
                            .put("message", result.error.code.toString())
                    )
                    call.reject("Error - " + result.error.code.toString())
                    return@Thread
                }

                val printerDriver = result.driver

                val printError: PrintError = printerDriver.printImage(decodedByte, settings)

                if (printError.code != PrintError.ErrorCode.NoError) {
                    notifyListeners(
                        BrotherPrintEvent.onPrint.webEventName,
                        JSObject().put("code", printError.code)
                            .put("message", result.toString())
                    )
                } else {
                    notifyListeners(
                        BrotherPrintEvent.onPrintError.webEventName,
                        JSObject().put("code", printError.code)
                            .put("message", result.toString())
                    )
                }

                printerDriver.closeChannel()
                call.resolve()
            }.start()
        } catch (e: Exception) {
            notifyListeners(
                BrotherPrintEvent.onPrintFailedCommunication.webEventName, JSObject().put("code", 0)
                    .put("message", e.localizedMessage)
            )
            call.reject(e.localizedMessage, e)
        }
    }

    private fun getBluetoothAdapter(context: Context): BluetoothAdapter? {
        val manager = context.getSystemService(Context.BLUETOOTH_SERVICE) as BluetoothManager
        return manager.adapter
    }

    @PluginMethod
    fun search(call: PluginCall) {
        when(call.getString("port", "wifi")) {
            "wifi" -> this.searchWiFiPrinter(call)
            "bluetooth" -> this.checkBLEChannel(call)
            "bluetoothLowEnergy" -> this.searchBLEPrinter(call)
            else -> call.reject("port is not 'wifi' | 'bluetooth' | 'bluetoothLowEnergy'")
        }
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
            requestAllPermissions(call, "permissionCallback");
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
            requestAllPermissions(call, "permissionCallback");
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
        val modelName = channel.extraInfo[Channel.ExtraInfoKey.ModelName] ?: ""
        val serialNumber = channel.extraInfo[Channel.ExtraInfoKey.SerialNubmer] ?: ""
        val macAddress = channel.extraInfo[Channel.ExtraInfoKey.MACAddress] ?: ""
        val nodeName = channel.extraInfo[Channel.ExtraInfoKey.NodeName] ?: ""
        val location = channel.extraInfo[Channel.ExtraInfoKey.Location] ?: ""
        val ipAddress = channel.channelInfo

        return JSObject()
            .put("port", port)
            .put("modelName", modelName)
            .put("serialNumber", serialNumber)
            .put("macAddress", macAddress)
            .put("nodeName", nodeName)
            .put("location", location)
            .put("ipAddress", ipAddress)
    }

    @PluginMethod
    fun cancelSearchWiFiPrinter(call: PluginCall?) {
        Thread {
            this.cancelRoutineWiFi?.invoke()
            this.cancelRoutineWiFi = null
        }.start()
    }

    @PluginMethod
    fun cancelSearchBluetoothPrinter(call: PluginCall?) {
        Thread {
            this.cancelRoutineBluetooth?.invoke()
            this.cancelRoutineBluetooth = null
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
            "checkBLEChannel" -> checkBLEChannel(call)
            "searchBLEPrinter" -> searchBLEPrinter(call)
        }
    }

    private fun isBluetoothPermissionGranted(): Boolean {
        return getPermissionState("bluetooth") == PermissionState.GRANTED
    }

    private fun isLocationPermissionGranted(): Boolean {
        return getPermissionState("location") == PermissionState.GRANTED
    }
}
