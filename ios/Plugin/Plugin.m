#import <Foundation/Foundation.h>
#import <Capacitor/Capacitor.h>

// Define the plugin using the CAP_PLUGIN Macro, and
// each method the plugin supports using the CAP_PLUGIN_METHOD macro.
CAP_PLUGIN(BrotherPrint, "BrotherPrint",
           CAP_PLUGIN_METHOD(printImage, CAPPluginReturnPromise);
           CAP_PLUGIN_METHOD(search, CAPPluginReturnPromise);
           CAP_PLUGIN_METHOD(stopSearchBLEPrinter, CAPPluginReturnPromise);
           CAP_PLUGIN_METHOD(cancelSearchWiFiPrinter, CAPPluginReturnPromise);
           CAP_PLUGIN_METHOD(cancelSearchBluetoothPrinter, CAPPluginReturnPromise);
)
