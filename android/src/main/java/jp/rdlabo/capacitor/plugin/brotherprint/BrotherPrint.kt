package jp.rdlabo.capacitor.plugin.brotherprint

import android.bluetooth.BluetoothAdapter
import android.graphics.BitmapFactory
import android.util.Base64
import android.util.Log
import com.brother.ptouch.sdk.LabelInfo
import com.brother.ptouch.sdk.Printer
import com.brother.ptouch.sdk.PrinterInfo
import com.brother.sdk.lmprinter.BLESearchOption
import com.brother.sdk.lmprinter.Channel
import com.brother.sdk.lmprinter.NetworkSearchOption
import com.brother.sdk.lmprinter.PrinterSearcher
import com.getcapacitor.JSArray
import com.getcapacitor.JSObject
import com.getcapacitor.Plugin
import com.getcapacitor.PluginCall
import com.getcapacitor.PluginMethod
import com.getcapacitor.annotation.CapacitorPlugin

@CapacitorPlugin
class BrotherPrint : Plugin() {
    @PluginMethod
    fun printImage(call: PluginCall) {
        // object.encodedImageで値を入力
        val encodedImage = call.getString("encodedImage")
        val decodedString = Base64.decode(encodedImage, Base64.DEFAULT)
        val decodedByte = BitmapFactory.decodeByteArray(decodedString, 0, decodedString.size)

        val printer = Printer()
        val settings = printer.printerInfo
        settings.numberOfCopies = call.getInt("numberOfCopies")!!
        val labelNameIndex = call.getInt("labelNameIndex")

        if (labelNameIndex == 16) {
            settings.labelNameIndex = LabelInfo.QL700.W62.ordinal
        } else {
            settings.labelNameIndex = LabelInfo.QL700.W62RB.ordinal
        }

        when (call.getString("printerType")) {
            "QL-800" -> {
                settings.printerModel = PrinterInfo.Model.QL_800
                settings.port = PrinterInfo.Port.USB
            }

            "QL-820NWB" -> {
                settings.printerModel = PrinterInfo.Model.QL_820NWB

                // 検索からデバイス情報が得られた場合
                val localName = call.getString("localName")
                val ipAddress = call.getString("ipAddress")

                if (localName != null) {
                    Log.d("protocol", "localName")
                    settings.port = PrinterInfo.Port.BLUETOOTH
                    settings.localName = localName
                } else if (ipAddress != null) {
                    Log.d("protocol", "ipAddress")
                    settings.port = PrinterInfo.Port.NET
                    settings.ipAddress = ipAddress
                } else {
                    Log.d("protocol", "USB")
                    settings.port = PrinterInfo.Port.USB
                }
            }

            else -> {
                call.reject("[ERROR] This printerType is not available")
                return
            }
        }
        settings.paperSize = PrinterInfo.PaperSize.CUSTOM
        settings.align = PrinterInfo.Align.CENTER
        settings.valign = PrinterInfo.VAlign.MIDDLE
        settings.printMode = PrinterInfo.PrintMode.ORIGINAL
        settings.printQuality = PrinterInfo.PrintQuality.HIGH_RESOLUTION

        settings.printMode = PrinterInfo.PrintMode.FIT_TO_PAGE
        settings.isAutoCut = true

        val c = bridge.context
        settings.workPath = c.cacheDir.path

        val setPrinter = printer.setPrinterInfo(settings)
        if (!setPrinter) {
            val setResult = Printer.getResult()
            notifyListeners("onPrintError", JSObject().put("value", setResult.errorCode))
        }

        try {
            Log.d(logTag, "Start Printer Thread")
            Thread {
                if (printer.startCommunication()) {
                    val result = printer.printImage(decodedByte)

                    if (result.errorCode == PrinterInfo.ErrorCode.ERROR_NONE) {
                        notifyListeners("onPrint", JSObject().put("value", result))
                    } else {
                        Log.d("TAG", "ERROR - " + result.errorCode)
                        notifyListeners("onPrintError", JSObject().put("value", result.errorCode))
                    }

                    printer.endCommunication()
                } else {
                    notifyListeners("onPrintFailedCommunication", JSObject().put("value", ""))
                }
            }
                .start()
            call.resolve()
        } catch (ex: Exception) {
            notifyListeners("onPrintFailedCommunication", JSObject().put("value", ""))
            call.reject(ex.localizedMessage, ex)
        }
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
        val resultList = JSArray()
        Thread {
            val intDuration: Int = call.getInt("searchDuration") ?: 15 ;
            val option = NetworkSearchOption(intDuration.toDouble(), false);
            val result = PrinterSearcher.startNetworkSearch(context, option){ channel ->
                val modelName = channel.extraInfo[Channel.ExtraInfoKey.ModelName] ?: ""
                val ipaddress = channel.channelInfo
                Log.d("TAG", "Model : $modelName, ipaddress: $ipaddress")

                resultList.put(JSObject().put("model", modelName).put("ipAddress", ipaddress))
            }
            call.resolve(JSObject().put("printers", resultList))
        }.start()
    }

    private fun checkBLEChannel(call: PluginCall) {
        val resultList = JSArray()
        Thread {
            for (channel in PrinterSearcher.startBluetoothSearch(context).channels){
                val modelName = channel.extraInfo[Channel.ExtraInfoKey.ModelName] ?: ""
                val serialNumber = channel.extraInfo[Channel.ExtraInfoKey.SerialNubmer] ?: ""
                Log.d("TAG", "Model : $modelName, serialNumber: $serialNumber")

                resultList.put(JSObject().put("model", modelName).put("serialNumber", serialNumber))
            }
            call.resolve(JSObject().put("printers", resultList))
        }.start()
    }

    private fun searchBLEPrinter(call: PluginCall) {
        val resultList = JSArray()
        Thread {
            val intDuration: Int = call.getInt("searchDuration") ?: 15 ;
            val option = BLESearchOption(intDuration.toDouble())
            val result = PrinterSearcher.startBLESearch(context, option){ channel ->
                val modelName = channel.extraInfo[Channel.ExtraInfoKey.ModelName] ?: ""
                val localName = channel.channelInfo
                Log.d("TAG", "Model : $modelName, Local Name: $localName")

                resultList.put(JSObject().put("model", modelName).put("localName", localName))
            }
            call.resolve(JSObject().put("printers", resultList))
        }.start()
    }

    @PluginMethod
    fun cancelSearchWiFiPrinter(call: PluginCall?) {
        Thread {
            PrinterSearcher.cancelNetworkSearch();
        }.start()
    }

    @PluginMethod
    fun cancelSearchBluetoothPrinter(call: PluginCall?) {
        Thread {
            PrinterSearcher.cancelBLESearch();
        }.start()
    }
}
