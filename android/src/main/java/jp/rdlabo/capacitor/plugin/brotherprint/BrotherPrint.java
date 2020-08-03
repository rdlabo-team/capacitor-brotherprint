package jp.rdlabo.capacitor.plugin.brotherprint;

import android.bluetooth.BluetoothAdapter;
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
    public void print(PluginCall call) {
        // object.encodedImageで値を入力
        final String encodedImage = call.getString("encodedImage");
        final String pureBase64Encoded = encodedImage.substring(encodedImage.indexOf(",")  + 1);
        byte[] decodedString = Base64.decode(pureBase64Encoded, Base64.DEFAULT);
        final Bitmap decodedByte = BitmapFactory.decodeByteArray(decodedString, 0, decodedString.length);

        final Printer printer = new Printer();
        PrinterInfo settings = printer.getPrinterInfo();

        // プリンタ設定
        final String printerType = call.getString("printerType");

        switch (printerType) {
            case "QL-800":
                settings.printerModel = Model.QL_800;
                settings.port = PrinterInfo.Port.USB;
                break;
            case "QL-820NW":
                settings.printerModel = Model.QL_820NWB;
                settings.port = PrinterInfo.Port.USB;

                // 検索からデバイス情報が得られた場合
                final String localName = call.getString("localName");
                final String ipAddress = call.getString("ipAddress");

                if (localName != null) {
                    settings.port = PrinterInfo.Port.NET;
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

        settings.labelNameIndex = LabelInfo.QL700.W62.ordinal();
        settings.printMode = PrinterInfo.PrintMode.FIT_TO_PAGE;
        settings.isAutoCut = true;
        printer.setPrinterInfo(settings);

        try {
            Log.d(getLogTag(), "Start Printer Thread");
            new Thread(new Runnable() {
                @Override
                public void run() {
                    if (printer.startCommunication()) {
                        PrinterStatus result = printer.printImage(decodedByte);
                        if (result.errorCode != PrinterInfo.ErrorCode.ERROR_NONE) {
                            Log.d("TAG", "ERROR - " + result.errorCode);
                        }
                        notifyListeners("onPrint", new JSObject().put("value", true));
                        printer.endCommunication();
                    } else {
                        notifyListeners("onPrintFailedCommunication", new JSObject().put("value", ""));
                    }
                }
            }).start();
            call.success(new JSObject().put("value", true));
        } catch (Exception ex) {
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
