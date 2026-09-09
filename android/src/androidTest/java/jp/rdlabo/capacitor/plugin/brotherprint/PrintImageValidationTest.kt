package jp.rdlabo.capacitor.plugin.brotherprint

import androidx.test.ext.junit.runners.AndroidJUnit4
import com.getcapacitor.JSObject
import com.getcapacitor.PluginCall
import org.junit.Assert.assertEquals
import org.junit.Test
import org.junit.runner.RunWith

@RunWith(AndroidJUnit4::class)
class PrintImageValidationTest {
    private class RecordingCall(encodedImage: String) : PluginCall(
        null, "BrotherPrint", "test", "printImage", JSObject().put("encodedImage", encodedImage)
    ) {
        val errors = mutableListOf<String>()
        override fun reject(message: String) {
            errors.add(message)
        }
    }

    @Test
    fun invalidImagesRejectOnceBeforeOpeningPrinter() {
        for (image in listOf("", "A", "SGVsbG8=", "iVBORw0KGgo=")) {
            val call = RecordingCall(image)
            // No bridge or printer is configured: validation must finish before SDK access.
            BrotherPrint().printImage(call)
            assertEquals("Input: $image", 1, call.errors.size)
        }
    }
}
