package jp.rdlabo.capacitor.plugin.brotherprint.models

import com.brother.ptouch.sdk.LabelInfo
import com.brother.ptouch.sdk.PrinterInfo
import com.brother.sdk.lmprinter.setting.PrintImageSettings
import com.brother.sdk.lmprinter.setting.QLPrintSettings
import com.brother.sdk.lmprinter.setting.TDPrintSettings
import com.getcapacitor.PluginCall

class BrotherPrintSettings {
    public fun modelTDSettings(call: PluginCall, settings: TDPrintSettings): TDPrintSettings {
        val baseSettings = this.baseSettings(call, settings);

        baseSettings.isAutoCut = call.getBoolean("autoCut", true)!!

        return baseSettings
    }

    public fun modelQLSettings(call: PluginCall, settings: QLPrintSettings): QLPrintSettings {
        val baseSettings = this.baseSettings(call, settings);

        baseSettings.isAutoCut = call.getBoolean("autoCut", true)!!
        baseSettings.labelSize = QLPrintSettings.LabelSize.entries.find { it.name == call.getString("labelName", "W62") }

        return baseSettings;
    }

    private fun <T: PrintImageSettings> baseSettings(call: PluginCall, settings: T): T {
        settings.numCopies = call.getInt("numberOfCopies", 1)!!

        when (call.getString("scaleMode")) {
            "ActualSize" -> settings.scaleMode = PrintImageSettings.ScaleMode.ActualSize
            "FitPageAspect" ->  settings.scaleMode = PrintImageSettings.ScaleMode.FitPageAspect
            "FitPaperAspect" ->  settings.scaleMode = PrintImageSettings.ScaleMode.FitPaperAspect
            "ScaleValue" -> {
                settings.scaleMode = PrintImageSettings.ScaleMode.ScaleValue
                if (call.getDouble("scaleValue") !== null) {
                    settings.scaleValue = call.getFloat("scaleValue")!!
                }
            }
        }

        when (call.getString("halftone")) {
            "Threshold" -> {
                settings.halftone = PrintImageSettings.Halftone.Threshold
                if (call.getDouble("halftoneThreshold") !== null) {
                    settings.halftoneThreshold = call.getInt("halftoneThreshold")!!
                }
            }
            "ErrorDiffusion" ->  settings.halftone = PrintImageSettings.Halftone.ErrorDiffusion
            "PatternDither" ->  settings.halftone = PrintImageSettings.Halftone.PatternDither
        }

        when (call.getString("imageRotation")) {
            "Rotate0" ->  settings.imageRotation = PrintImageSettings.Rotation.Rotate0
            "Rotate90" ->  settings.imageRotation = PrintImageSettings.Rotation.Rotate90
            "Rotate180" ->  settings.imageRotation = PrintImageSettings.Rotation.Rotate180
            "Rotate270" ->  settings.imageRotation = PrintImageSettings.Rotation.Rotate270
        }

        when (call.getString("verticalAlignment")) {
            "Top" ->  settings.vAlignment = PrintImageSettings.VerticalAlignment.Top
            "Center" ->  settings.vAlignment = PrintImageSettings.VerticalAlignment.Center
            "Bottom" ->  settings.vAlignment = PrintImageSettings.VerticalAlignment.Bottom
        }

        when (call.getString("horizontalAlignment")) {
            "Left" ->  settings.hAlignment = PrintImageSettings.HorizontalAlignment.Left
            "Center" ->  settings.hAlignment = PrintImageSettings.HorizontalAlignment.Center
            "Right" ->  settings.hAlignment = PrintImageSettings.HorizontalAlignment.Right
        }

//        when (call.getString("compressMode")) { }

        when (call.getString("printQuality")) {
            "Best" ->  settings.printQuality = PrintImageSettings.PrintQuality.Best
            "Fast" ->  settings.printQuality = PrintImageSettings.PrintQuality.Fast
        }

//        when (call.getString("resolution")) { }

        return settings
    }
}