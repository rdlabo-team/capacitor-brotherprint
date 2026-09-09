package jp.rdlabo.capacitor.plugin.brotherprint

import org.junit.Assert.assertFalse
import org.junit.Assert.assertTrue
import org.junit.Test

class BluetoothPrinterClassTest {
    @Test
    fun includesPrintersRegardlessOfManufacturerOrOtherCapabilities() {
        assertTrue(isBluetoothPrinterClass(0x0680)) // Printer, including SATO.
        assertTrue(isBluetoothPrinterClass(0x140680)) // Observed Brother QL/TD class.
        assertTrue(isBluetoothPrinterClass(0x06C0)) // Printer and scanner.
    }

    @Test
    fun excludesNonPrintersAndUnknownClasses() {
        for (deviceClass in listOf(null, 0, 0x1F00, 0x0600, 0x0640, 0x0418, 0x0580)) {
            assertFalse("Class: $deviceClass", isBluetoothPrinterClass(deviceClass))
        }
    }
}
