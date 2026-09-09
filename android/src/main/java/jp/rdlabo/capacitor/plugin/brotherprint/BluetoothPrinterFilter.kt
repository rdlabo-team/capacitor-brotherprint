package jp.rdlabo.capacitor.plugin.brotherprint

internal fun matchesBluetoothPrinterFilter(printersOnly: Boolean = false, deviceClass: () -> Int?): Boolean {
    if (!printersOnly) return true
    val value = deviceClass()
    // Brother QL-820NWB / TD-2350D reported Bluetooth Class of Device 0x140680.
    // Match its Imaging/Printer bits, not the entire value: Android's deviceClass
    // omits service bits (0x140000), and other printers can report 0x000680.
    // Class of Device: major Imaging (0x0600), minor Printer bit (0x0080).
    // Other imaging capabilities (e.g. Scanner) may be set alongside Printer.
    return value != null && (value and 0x1F80) == 0x0680
}
