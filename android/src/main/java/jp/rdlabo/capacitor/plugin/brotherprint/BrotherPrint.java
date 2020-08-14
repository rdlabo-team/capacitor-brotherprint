package jp.rdlabo.capacitor.plugin.brotherprint;

import android.bluetooth.BluetoothAdapter;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Base64;
import android.util.Log;

import com.brother.ptouch.sdk.BLEPrinter;
import com.brother.ptouch.sdk.NetPrinter;
import com.brother.ptouch.sdk.PrinterStatus;
import com.getcapacitor.JSObject;
import com.getcapacitor.NativePlugin;
import com.getcapacitor.Plugin;
import com.getcapacitor.PluginCall;
import com.getcapacitor.PluginMethod;

import com.brother.ptouch.sdk.Printer;
import com.brother.ptouch.sdk.PrinterInfo;
import com.brother.ptouch.sdk.PrinterInfo.Model;
import com.brother.ptouch.sdk.LabelInfo;

import java.util.List;

@NativePlugin
public class BrotherPrint extends Plugin {

    @PluginMethod()
    public void printImage(PluginCall call) {
        // object.encodedImageで値を入力
        final String encodedImage = call.getString("encodedImage");
        byte[] decodedString = Base64.decode(encodedImage, Base64.DEFAULT);
        final Bitmap decodedByte = BitmapFactory.decodeByteArray(decodedString, 0, decodedString.length);

        final Printer printer = new Printer();
        PrinterInfo settings = printer.getPrinterInfo();
        settings.numberOfCopies = call.getInt("numberOfCopies");
        final Integer labelNameIndex = call.getInt("labelNameIndex");

        if (labelNameIndex == 16) {
            settings.labelNameIndex = LabelInfo.QL700.W62.ordinal();
        } else {
            settings.labelNameIndex = LabelInfo.QL700.W62RB.ordinal();
        }

        switch (call.getString("printerType")) {
            case "QL-800":
                settings.printerModel = Model.QL_800;
                settings.port = PrinterInfo.Port.USB;
                break;
            case "QL-820NWB":
                settings.printerModel = Model.QL_820NWB;

                // 検索からデバイス情報が得られた場合
                final String localName = call.getString("localName");
                final String ipAddress = call.getString("ipAddress");

                if (localName != null) {
                    settings.port = PrinterInfo.Port.BLUETOOTH;
                    settings.setLocalName(localName);
                } else if (ipAddress != null) {
                    settings.port = PrinterInfo.Port.NET;
                    settings.ipAddress = ipAddress;
                } else {
                    settings.port = PrinterInfo.Port.USB;
                }
                break;
            default:
                call.error("[ERROR] This printerType is not available");
                return;
        }

        settings.paperSize = PrinterInfo.PaperSize.CUSTOM;
        settings.align = PrinterInfo.Align.CENTER;
        settings.valign = PrinterInfo.VAlign.MIDDLE;
        settings.printMode = PrinterInfo.PrintMode.ORIGINAL;
        settings.printQuality = PrinterInfo.PrintQuality.HIGH_RESOLUTION;

        settings.printMode = PrinterInfo.PrintMode.FIT_TO_PAGE;
        settings.isAutoCut = true;

        Context c = bridge.getContext();
        settings.workPath = c.getCacheDir().getPath();

        boolean setPrinter = printer.setPrinterInfo(settings);
        if (!setPrinter) {
            PrinterStatus setResult = printer.getResult();
            notifyListeners("onPrintError", new JSObject().put("value", setResult.errorCode));
        }

        try {
            Log.d(getLogTag(), "Start Printer Thread");
            new Thread(new Runnable() {
                @Override
                public void run() {
                    if (printer.startCommunication()) {
                        PrinterStatus result = printer.printImage(decodedByte);

                        if (result.errorCode == PrinterInfo.ErrorCode.ERROR_NONE) {
                            notifyListeners("onPrint", new JSObject().put("value", result));
                        } else {
                            Log.d("TAG", "ERROR - " + result.errorCode);
                            notifyListeners("onPrintError", new JSObject().put("value", result.errorCode));
                        }

                        printer.endCommunication();
                    } else {
                        notifyListeners("onPrintFailedCommunication", new JSObject().put("value", ""));
                    }
                }
            }).start();
            call.success(new JSObject().put("value", true));
        } catch (Exception ex) {
            notifyListeners("onPrintFailedCommunication", new JSObject().put("value", ""));
            call.error(ex.getLocalizedMessage(), ex);
        }
    }

    @PluginMethod()
    public void searchWiFiPrinter(PluginCall call) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                Printer printer = new Printer();
                NetPrinter[] printerList = printer.getNetPrinters("QL-820NWB");

                JSObject ipAddressList = new JSObject();
                int i = 0;
                for (NetPrinter device: printerList) {
                    ipAddressList.put(i + "", device.ipAddress);
                    Log.d("TAG", String.format("Model: %s, IP Address: %s", device.modelName, device.ipAddress));
                    i++;
                }
                notifyListeners("onIpAddressAvailable", ipAddressList);
            }
        }).start();
        call.success(new JSObject().put("value", true));
    }

    @PluginMethod()
    public void searchBLEPrinter(PluginCall call) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                Printer printer = new Printer();
                List<BLEPrinter> printerList = printer.getBLEPrinters(BluetoothAdapter.getDefaultAdapter(), 30);


                JSObject localNameList = new JSObject();
                int i = 0;
                for (BLEPrinter device: printerList) {
                    Log.d("TAG", "Local Name: " + device.localName);
                    localNameList.put(i + "", device.localName);
                    i++;
                }
                notifyListeners("onBLEAvailable", localNameList);
            }
        }).start();
        call.success(new JSObject().put("value", true));
    }
}
