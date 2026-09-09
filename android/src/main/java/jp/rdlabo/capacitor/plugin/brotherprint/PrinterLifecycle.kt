package jp.rdlabo.capacitor.plugin.brotherprint

import java.util.WeakHashMap

/** Per-instance lifecycle; the process-wide SDK worker owns operation completion. */
internal class PrinterLifecycle<T : Any> {
    @Volatile var isDestroyed = false
        private set
    private val settled = WeakHashMap<T, Boolean>()
    private val startedPrints = WeakHashMap<T, Boolean>()
    private val cancellationLock = Any()
    private var cancellation: Pair<String, () -> Unit>? = null

    /** Atomically distinguish a queued call from a print already handed to the SDK worker. */
    @Synchronized fun begin(call: T, isPrint: Boolean): Boolean {
        if (isDestroyed) return false
        if (isPrint) startedPrints[call] = true
        return true
    }

    @Synchronized fun destroy() { isDestroyed = true }

    fun registerCancellation(port: String, action: () -> Unit): Boolean = synchronized(cancellationLock) {
        if (isDestroyed) return false
        cancellation = port to action
        return true
    }

    fun clearCancellation() = synchronized(cancellationLock) { cancellation = null }

    // Keep ownership until cancellation returns: a delayed cancellation must never
    // cancel the next instance's search on the same process-wide SDK.
    fun cancel(port: String? = null): Unit = synchronized(cancellationLock) {
        val current = cancellation ?: return
        if (port != null && current.first != port) return
        try { current.second() } catch (_: Exception) { /* SDK cancellation is best effort. */ }
    }

    @Synchronized fun deliver(action: () -> Unit) {
        if (!isDestroyed) action()
    }

    @Synchronized fun settle(call: T, action: (Boolean) -> Unit) {
        if (settled.put(call, true) == null) {
            val preservePrintResult = startedPrints.remove(call) == true
            action(isDestroyed && !preservePrintResult)
        }
    }
}
