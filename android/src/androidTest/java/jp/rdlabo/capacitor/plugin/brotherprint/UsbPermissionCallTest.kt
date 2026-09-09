package jp.rdlabo.capacitor.plugin.brotherprint

import android.content.BroadcastReceiver
import android.content.Intent
import android.hardware.usb.UsbManager
import androidx.test.ext.junit.runners.AndroidJUnit4
import com.getcapacitor.JSObject
import com.getcapacitor.PluginCall
import org.junit.Assert.*
import org.junit.Test
import org.junit.runner.RunWith

@RunWith(AndroidJUnit4::class)
class UsbPermissionCallTest {
    private class RecordingCall : PluginCall(null, "BrotherPrint", "test", "search", JSObject().put("port", "usb")) {
        val errors = mutableListOf<String>()
        override fun reject(message: String) {
            errors.add(message)
        }
    }

    private fun pending(plugin: BrotherPrint, call: RecordingCall) {
        BrotherPrint::class.java.getDeclaredField("storeCall").apply { isAccessible = true }.set(plugin, call)
    }

    private fun receiver(plugin: BrotherPrint): BroadcastReceiver =
        BrotherPrint::class.java.getDeclaredField("usbReceiver").apply { isAccessible = true }.get(plugin) as BroadcastReceiver

    @Test
    fun secondSearchDoesNotReplacePendingCall() {
        val plugin = BrotherPrint()
        val first = RecordingCall()
        val second = RecordingCall()
        pending(plugin, first)
        plugin.search(second)
        assertEquals(1, second.errors.size)
        assertTrue(first.errors.isEmpty())
        receiver(plugin).onReceive(null, Intent("jp.rdlabo.capacitor.plugin.brotherprint.USB_PERMISSION")
            .putExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false))
        assertEquals(1, first.errors.size)
        assertEquals(1, second.errors.size)
    }

    @Test
    fun denialClearsCallAndIgnoresDuplicateBroadcast() {
        val plugin = BrotherPrint()
        val call = RecordingCall()
        pending(plugin, call)
        val denied = Intent("jp.rdlabo.capacitor.plugin.brotherprint.USB_PERMISSION")
            .putExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false)
        receiver(plugin).onReceive(null, denied)
        receiver(plugin).onReceive(null, denied)
        assertEquals(1, call.errors.size)
        assertNull(BrotherPrint::class.java.getDeclaredField("storeCall").apply { isAccessible = true }.get(plugin))
    }

    @Test
    fun unrelatedBroadcastDoesNotRejectPendingCall() {
        val plugin = BrotherPrint()
        val call = RecordingCall()
        pending(plugin, call)
        receiver(plugin).onReceive(null, Intent("unrelated"))
        assertTrue(call.errors.isEmpty())
    }

    @Test
    fun destroyingPluginRejectsPendingCallOnce() {
        val plugin = BrotherPrint()
        val call = RecordingCall()
        pending(plugin, call)
        val destroy = BrotherPrint::class.java.getDeclaredMethod("handleOnDestroy").apply { isAccessible = true }
        destroy.invoke(plugin)
        destroy.invoke(plugin)
        assertEquals(1, call.errors.size)
    }
}
