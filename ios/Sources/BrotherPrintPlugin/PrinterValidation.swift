import CoreFoundation
import Foundation

func validatePrinterOptions(_ values: [AnyHashable: Any]) -> String? {
    // Reject malformed supplied numbers before getters can apply defaults or overflow integer conversions.
    let numericKeys = [
        "numberOfCopies", "halftoneThreshold", "scaleValue", "tapeWidth", "tapeLength", "marginTop", "marginRight", "marginBottom",
        "marginLeft", "gapLength", "paperMarkPosition", "paperMarkLength",
    ]
    for key in numericKeys where values[key] != nil {
        guard let value = values[key] as? NSNumber, CFGetTypeID(value) != CFBooleanGetTypeID(), value.doubleValue.isFinite else {
            return "\(key) must be a finite number"
        }
        if ["numberOfCopies", "halftoneThreshold"].contains(key),
            value.doubleValue.rounded(.towardZero) != value.doubleValue || value.doubleValue >= Double(Int.max)
                || value.doubleValue <= Double(Int.min)
        {
            return "\(key) must be an integer"
        }
    }
    for key in ["modelName", "port", "channelInfo", "paperType", "paperUnit", "labelName", "scaleMode"] where values[key] != nil {
        if !(values[key] is String) { return "\(key) must be a string" }
    }
    func string(_ key: String, _ fallback: String = "") -> String { values[key] as? String ?? fallback }
    func integer(_ key: String, _ fallback: Int) -> Int { (values[key] as? NSNumber)?.intValue ?? fallback }
    func number(_ key: String) -> Double? { (values[key] as? NSNumber)?.doubleValue }
    func number(_ key: String, _ fallback: Double) -> Double { number(key) ?? fallback }

    let model = string("modelName", "QL_820NWB")
    guard printerModelNames.contains(model) else { return "Unsupported model on iOS" }
    guard ["wifi", "bluetooth", "bluetoothLowEnergy"].contains(string("port", "wifi")),
        !string("channelInfo", "").trimmingCharacters(in: .whitespacesAndNewlines).isEmpty
    else { return "Invalid channel" }
    guard integer("numberOfCopies", 1) > 0 else { return "numberOfCopies must be positive" }
    guard (0...255).contains(integer("halftoneThreshold", 128)) else { return "halftoneThreshold must be 0...255" }
    if string("scaleMode") == "ScaleValue" {
        guard let value = number("scaleValue"), value.isFinite, value > 0 else { return "scaleValue must be positive and finite" }
    }
    if model.hasPrefix("TD") {
        guard ["rollPaper", "dieCutPaper", "markRollPaper"].contains(string("paperType", "")) else { return "Invalid paperType" }
        guard ["mm", "inch"].contains(string("paperUnit", "mm")) else { return "Invalid paperUnit" }
        guard let width = number("tapeWidth"), width.isFinite, width > 0 else { return "tapeWidth must be positive and finite" }
        if string("paperType") != "rollPaper" {
            guard let length = number("tapeLength"), length.isFinite, length > 0 else { return "tapeLength must be positive and finite" }
        }
        for key in ["marginTop", "marginRight", "marginBottom", "marginLeft", "gapLength", "paperMarkPosition", "paperMarkLength"] {
            let value = number(key, 0)
            if !value.isFinite || value < 0 { return "\(key) must be nonnegative and finite" }
        }
    } else {
        guard
            [
                "DieCutW17H54", "DieCutW17H87", "DieCutW23H23", "DieCutW29H42", "DieCutW29H90", "DieCutW38H90", "DieCutW39H48",
                "DieCutW52H29", "DieCutW62H29", "DieCutW62H60", "DieCutW62H75", "DieCutW62H100", "DieCutW60H86", "DieCutW54H29",
                "DieCutW102H51", "DieCutW102H152", "DieCutW103H164", "RollW12", "RollW29", "RollW38", "RollW50", "RollW54", "RollW62",
                "RollW62RB", "RollW102", "RollW103", "DTRollW90", "DTRollW102", "DTRollW102H51", "DTRollW102H152", "RoundW12DIA",
                "RoundW24DIA", "RoundW58DIA",
            ].contains(string("labelName", "RollW62"))
        else { return "Invalid labelName" }
    }
    return nil
}

/// Emits the legacy validation event before the promise rejection, exactly once.
func reportPrintFailure(_ message: String, category: String, emit: ([String: Any]) -> Void, reject: () -> Void) {
    emit(["code": 0, "message": message, "category": category])
    reject()
}
