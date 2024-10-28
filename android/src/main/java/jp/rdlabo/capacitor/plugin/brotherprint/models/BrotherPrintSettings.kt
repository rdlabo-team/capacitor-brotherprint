package jp.rdlabo.capacitor.plugin.brotherprint.models

import com.brother.ptouch.sdk.Printer
import com.brother.ptouch.sdk.PrinterInfo
import com.getcapacitor.PluginCall

class BrotherPrintSettings {
    public fun initialize(call: PluginCall, printer: Printer): PrinterInfo {
        val settings = printer.printerInfo
        settings.paperSize = PrinterInfo.PaperSize.CUSTOM
        settings.numberOfCopies = call.getInt("numberOfCopies", 1)!!
        settings.isAutoCut = call.getBoolean("autoCut", true)!!

        when (call.getString("scaleMode")) {
            "ActualSize" -> settings.printMode = PrinterInfo.PrintMode.ORIGINAL
            "FitPageAspect" ->  settings.printMode = PrinterInfo.PrintMode.FIT_TO_PAGE
            "FitPaperAspect" ->  settings.printMode = PrinterInfo.PrintMode.FIT_TO_PAPER
            "ScaleValue" -> {
                settings.printMode = PrinterInfo.PrintMode.SCALE
                if (call.getDouble("scaleValue") !== null) {
                    settings.scaleValue = call.getDouble("scaleValue")!!
                }
            }
        }

        when (call.getString("halftone")) {
            "Threshold" -> {
                settings.halftone = PrinterInfo.Halftone.THRESHOLD
                if (call.getDouble("halftoneThreshold") !== null) {
                    settings.thresholdingValue = call.getInt("halftoneThreshold")!!
                }
            }
            "ErrorDiffusion" ->  settings.halftone = PrinterInfo.Halftone.ERRORDIFFUSION
            "PatternDither" ->  settings.halftone = PrinterInfo.Halftone.PATTERNDITHER
        }

        when (call.getString("imageRotation")) {
            "Rotate0" ->  settings.rotation = PrinterInfo.Rotation.Rotate0
            "Rotate90" ->  settings.rotation = PrinterInfo.Rotation.Rotate90
            "Rotate180" ->  settings.rotation = PrinterInfo.Rotation.Rotate180
            "Rotate270" ->  settings.rotation = PrinterInfo.Rotation.Rotate270
        }

        when (call.getString("verticalAlignment")) {
            "Top" ->  settings.valign = PrinterInfo.VAlign.TOP
            "Center" ->  settings.valign = PrinterInfo.VAlign.MIDDLE
            "Bottom" ->  settings.valign = PrinterInfo.VAlign.BOTTOM
        }

        when (call.getString("horizontalAlignment")) {
            "Left" ->  settings.align = PrinterInfo.Align.LEFT
            "Center" ->  settings.align = PrinterInfo.Align.CENTER
            "Right" ->  settings.align = PrinterInfo.Align.RIGHT
        }

//        when (call.getString("compressMode")) { }

        when (call.getString("printQuality")) {
            "Best" ->  settings.printQuality = PrinterInfo.PrintQuality.HIGH_RESOLUTION
            "Fast" ->  settings.printQuality = PrinterInfo.PrintQuality.DOUBLE_SPEED
        }

//        when (call.getString("resolution")) { }

        return settings
    }
}