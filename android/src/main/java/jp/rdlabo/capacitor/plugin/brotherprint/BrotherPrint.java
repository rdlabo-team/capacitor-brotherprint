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
    public void echo(PluginCall call) {
        // object.encodedImageで値を入力
        final String encodedImage = call.getString("encodedImage");
        final String pureBase64Encoded = encodedImage.substring(encodedImage.indexOf(",")  + 1);
        byte[] decodedString = Base64.decode(pureBase64Encoded, Base64.DEFAULT);
        final Bitmap decodedByte = BitmapFactory.decodeByteArray(decodedString, 0, decodedString.length);

        final Printer printer = new Printer();
        PrinterInfo settings = printer.getPrinterInfo();

        // プリンタ設定
        settings.printerModel = Model.QL_800;

        // プリンタへの接続方法
//        settings.port = PrinterInfo.Port.NET;
//        settings.ipAddress = "your-printer-ip";
//        settings.workPath = "Context.writable-dir-path";
        settings.port = PrinterInfo.Port.USB;

        // 用紙サイズ
        settings.labelNameIndex = LabelInfo.QL700.W62.ordinal();  // 800の62mm幅
        settings.printMode = PrinterInfo.PrintMode.FIT_TO_PAGE;
        settings.isAutoCut = true;
        printer.setPrinterInfo(settings);

        Log.d(getLogTag(), "Start Printer Thread");

        try {
            new Thread(new Runnable() {
                @Override
                public void run() {
                    if (printer.startCommunication()) {
                        PrinterStatus result = printer.printImage(decodedByte);
                        if (result.errorCode != PrinterInfo.ErrorCode.ERROR_NONE) {
                            Log.d("TAG", "ERROR - " + result.errorCode);
                        }
                        printer.endCommunication();
                    } else {
                        Log.d(getLogTag(), "Do not find Printer");
                    }
                }
            }).start();
            call.success(new JSObject().put("value", true));
        } catch (Exception ex) {
            call.error(ex.getLocalizedMessage(), ex);
        }
    }

//    void searchWiFiPrinter() {
//        new Thread(new Runnable() {
//            @Override
//            public void run() {
//                Printer printer = new Printer();
//                NetPrinter[] printerList = printer.getNetPrinters("QL-820NWB");
//                for (NetPrinter device: printerList) {
//                    Log.d("TAG", String.format("Model: %s, IP Address: %s", device.modelName, device.ipAddress));
//                }
//            }
//        }).start();
//    }
//
//    void searchBLEPrinter() {
//        new Thread(new Runnable() {
//            @Override
//            public void run() {
//                Printer printer = new Printer();
//                List<BLEPrinter> printerList = printer.getBLEPrinters(BluetoothAdapter.getDefaultAdapter(), 30);
//                for (BLEPrinter device: printerList) {
//                    Log.d("TAG", "Local Name: " + device.localName);
//                }
//            }
//        }).start();
//    }
}
