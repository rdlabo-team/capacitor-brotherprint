package jp.rdlabo.capacitor.plugin.brotherprint

import org.junit.Test
import org.junit.Assert.assertThrows

class PrinterValidationTest {
    private val td = mapOf<String, Any?>("modelName" to "TD_2350D_300", "paperType" to "dieCutPaper", "paperUnit" to "mm", "tapeWidth" to 60, "tapeLength" to 60)
    @Test fun validSettings() { validatePrinterOptions(td); validatePrinterOptions(mapOf("modelName" to "QL_820NWB")) }
    @Test fun invalidTdSettingsReject() {
        for (override in listOf(mapOf("paperType" to ""), mapOf("paperUnit" to "px"), mapOf("tapeWidth" to 0), mapOf("tapeLength" to -1), mapOf("marginLeft" to -1), mapOf("tapeWidth" to Double.NaN), mapOf("numberOfCopies" to 1.5), mapOf("numberOfCopies" to "2"), mapOf("halftoneThreshold" to 256))) {
            assertThrows(IllegalArgumentException::class.java) { validatePrinterOptions(td + override) }
        }
        assertThrows(IllegalArgumentException::class.java) { validatePrinterOptions(mapOf("modelName" to "TD_2350D_300")) }
    }
    @Test fun usbCannotChooseBetweenBrotherDevices() {
        requireSingleBrotherUsb(listOf(0x04f9, 0x1234))
        assertThrows(IllegalArgumentException::class.java) { requireSingleBrotherUsb(emptyList()) }
        assertThrows(IllegalArgumentException::class.java) { requireSingleBrotherUsb(listOf(0x04f9, 0x04f9)) }
    }

    @Test fun usbSdkEmptyAddressBecomesAnIdentityToken() {
        val token = usbChannelToken("/dev/bus/usb/001/003", 0x04f9, 123, "serial-a")
        org.junit.Assert.assertEquals(token, bridgeChannelInfo("usb", "", token))
        org.junit.Assert.assertTrue(token.isNotBlank())
        org.junit.Assert.assertNotEquals(token, usbChannelToken("/dev/bus/usb/001/004", 0x04f9, 123, "serial-a"))
        org.junit.Assert.assertNotEquals(token, usbChannelToken("/dev/bus/usb/001/003", 0x04f9, 123, "serial-b"))
        org.junit.Assert.assertEquals("192.0.2.1", bridgeChannelInfo("wifi", "192.0.2.1", null))
        assertThrows(IllegalArgumentException::class.java) { bridgeChannelInfo("usb", "", null) }
    }
    @Test fun validationFailureEmitsOnceThenRejectsOnceForPrintOnly() {
        for (method in listOf("printImage", "search", "isChannelAvailable")) {
            val sequence = mutableListOf<String>()
            reportPrinterFailure(method, "Invalid model", "INVALID_ARGUMENT", { payload ->
                org.junit.Assert.assertEquals(0, payload["code"])
                org.junit.Assert.assertEquals("INVALID_ARGUMENT", payload["category"])
                sequence.add("event")
            }, { sequence.add("reject") })
            org.junit.Assert.assertEquals(if (method == "printImage") listOf("event", "reject") else listOf("reject"), sequence)
        }
    }
}
