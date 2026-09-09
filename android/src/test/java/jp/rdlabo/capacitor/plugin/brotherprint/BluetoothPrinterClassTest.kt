package jp.rdlabo.capacitor.plugin.brotherprint

import org.junit.Assert.assertFalse
import org.junit.Assert.assertTrue
import org.junit.Test

class BluetoothPrinterClassTest {
    @Test
    fun includesPrintersRegardlessOfManufacturerOrOtherCapabilities() {
        assertTrue(matchesBluetoothPrinterFilter(true) { 0x0680 }) // Printer, including SATO.
        assertTrue(matchesBluetoothPrinterFilter(true) { 0x140680 }) // Observed Brother QL/TD class.
        assertTrue(matchesBluetoothPrinterFilter(true) { 0x06C0 }) // Printer and scanner.
    }

    @Test
    fun excludesNonPrintersAndUnknownClasses() {
        for (deviceClass in listOf(null, 0, 0x1F00, 0x0600, 0x0640, 0x0418, 0x0580)) {
            assertFalse("Class: $deviceClass", matchesBluetoothPrinterFilter(true) { deviceClass })
        }
    }

    @Test
    fun disabledOrOmittedFilterDoesNotReadDeviceClass() {
        assertTrue(matchesBluetoothPrinterFilter(false) { error("Must not access Bluetooth") })
        assertTrue(matchesBluetoothPrinterFilter { error("Must not access Bluetooth") })
    }

    @Test
    fun enabledFilterReadsDeviceClassOnce() {
        var reads = 0
        assertTrue(matchesBluetoothPrinterFilter(true) { reads++; 0x0680 })
        assertTrue(reads == 1)
    }
}
