package jp.rdlabo.capacitor.plugin.brotherprint

enum class BrotherPrintEvent(val webEventName: String) {
    onPrinterAvailable("onPrinterAvailable"),
    onPrint("onPrint"),
    onPrintFailedCommunication("onPrintFailedCommunication"),
    onPrintError("onPrintError")
}