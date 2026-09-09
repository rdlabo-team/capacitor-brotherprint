// Generated from printer-models.json. Run npm run generate:models.
import BRLMPrinterKit

func nativePrinterModel(_ name: String) -> BRLMPrinterModel {
    switch name {
    case "QL_810W": return .QL_810W
    case "QL_820NWB": return .QL_820NWB
    case "TD_2320D_203": return .TD_2320D_203
    case "TD_2030AD": return .TD_2030A
    case "TD_2350D_300": return .TD_2350D_300
    default: return .unknown
    }
}
