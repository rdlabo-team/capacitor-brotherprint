package jp.rdlabo.capacitor.plugin.brotherprint

import org.junit.Assert.*
import org.junit.Test
import java.util.concurrent.CountDownLatch
import java.util.concurrent.Executors
import java.util.concurrent.TimeUnit

class PrinterLifecycleTest {
    @Test fun destructionSuppressesEventsAndSettlesOnlyOnceAsCancelled() {
        val lifecycle = PrinterLifecycle<Any>()
        val call = Any()
        var deliveries = 0
        var settlements = 0
        lifecycle.destroy()
        lifecycle.deliver { deliveries++ }
        repeat(2) { lifecycle.settle(call) { cancelled -> assertTrue(cancelled); settlements++ } }
        assertEquals(0, deliveries)
        assertEquals(1, settlements)
        assertFalse(lifecycle.registerCancellation("wifi") { fail("Must not start") })
    }

    @Test fun destroyedQueuedInstanceDoesNotStartSdkAndNextInstanceCanRun() {
        val worker = Executors.newSingleThreadExecutor()
        val started = CountDownLatch(1)
        val release = CountDownLatch(1)
        val old = PrinterLifecycle<Any>()
        val fresh = PrinterLifecycle<Any>()
        try {
            val running = worker.submit { started.countDown(); release.await() }
            assertTrue(started.await(2, TimeUnit.SECONDS))
            val queued = worker.submit<Boolean> { old.begin(Any(), true) }
            old.destroy()
            assertFalse(queued.isDone) // Destruction never releases the SDK worker early.
            release.countDown()
            running.get(2, TimeUnit.SECONDS)
            assertFalse(queued.get(2, TimeUnit.SECONDS))
            assertTrue(worker.submit<Boolean> { fresh.begin(Any(), true) }.get(2, TimeUnit.SECONDS))
        } finally { release.countDown(); worker.shutdownNow() }
    }

    @Test fun destructionPreservesStartedPrintSuccessAndFailureButCancelsQueuedPrint() {
        for (sdkResult in listOf("success", "PRINT_FAILED")) {
            val lifecycle = PrinterLifecycle<Any>()
            val activePrint = Any()
            val queuedPrint = Any()
            assertTrue(lifecycle.begin(activePrint, true))
            lifecycle.destroy()
            assertFalse(lifecycle.begin(queuedPrint, true))
            var result = ""
            lifecycle.settle(activePrint) { cancelled -> result = if (cancelled) "CANCELLED" else sdkResult }
            assertEquals(sdkResult, result)
            lifecycle.settle(activePrint) { fail("Duplicate settlement") }
            lifecycle.settle(queuedPrint) { cancelled -> assertTrue(cancelled) }
            lifecycle.deliver { fail("Destroyed instance event") }
        }
    }

    @Test fun cancellationIsPortScopedBestEffortAndCannotSurviveCompletion() {
        for (port in listOf("wifi", "bluetooth")) {
            val lifecycle = PrinterLifecycle<Any>()
            var calls = 0
            lifecycle.registerCancellation(port) { calls++; throw IllegalStateException("SDK error") }
            lifecycle.cancel("unrelated")
            assertEquals(0, calls)
            lifecycle.destroy()
            lifecycle.cancel()
            assertEquals(1, calls)
            lifecycle.clearCancellation()
            lifecycle.cancel()
            assertEquals(1, calls)
        }
    }

    @Test fun cancellationDoesNotBlockSdkResultCallbacksOrReleaseOwnershipEarly() {
        val lifecycle = PrinterLifecycle<Any>()
        val cancelling = CountDownLatch(1)
        val finish = CountDownLatch(1)
        val worker = Executors.newFixedThreadPool(2)
        lifecycle.registerCancellation("wifi") { cancelling.countDown(); finish.await() }
        lifecycle.destroy()
        try {
            val cancel = worker.submit { lifecycle.cancel() }
            assertTrue(cancelling.await(2, TimeUnit.SECONDS))
            lifecycle.deliver { fail("Old event") }
            val completion = worker.submit { lifecycle.clearCancellation() }
            assertFalse(completion.isDone)
            finish.countDown()
            cancel.get(2, TimeUnit.SECONDS)
            completion.get(2, TimeUnit.SECONDS)
        } finally { finish.countDown(); worker.shutdownNow() }
    }
}
