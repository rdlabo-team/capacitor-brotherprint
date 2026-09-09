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

    private class EventCall(eventName: String) : PluginCall(
        null, "BrotherPrint", eventName, "addListener", JSObject().put("eventName", eventName)
    ) {
        val events = mutableListOf<JSObject>()
        override fun resolve(data: JSObject) {
            events.add(data)
        }
    }

    @Test
    fun invalidImagesRejectOnceBeforeOpeningPrinter() {
        for (image in listOf("", "A", "SGVsbG8=", "iVBORw0KGgo=")) {
            val call = RecordingCall(image)
            val plugin = BrotherPrint()
            val errorListener = EventCall("onPrintError")
            val successListener = EventCall("onPrint")
            val communicationListener = EventCall("onPrintFailedCommunication")
            listOf(errorListener, successListener, communicationListener).forEach { plugin.addListener(it) }
            // No bridge or printer is configured: validation must finish before SDK access.
            plugin.printImage(call)
            assertEquals("Input: $image", 1, call.errors.size)
            assertEquals(1, errorListener.events.size)
            assertEquals(0, errorListener.events.single().getInt("code"))
            assertEquals(call.errors.single(), errorListener.events.single().getString("message"))
            assertEquals(0, successListener.events.size)
            assertEquals(0, communicationListener.events.size)
        }
    }
}
