// Generated from printer-models.json. Run npm run generate:models.
import { BRLMPrinterModelName as Model, BRLMPrinterPort as Port } from './brother-printer.enum';

export const printerPorts: Record<Model, readonly Port[]> = {
  [Model.QL_800]: [Port.usb],
  [Model.QL_810W]: [Port.wifi, Port.usb],
  [Model.QL_820NWB]: [Port.wifi, Port.bluetooth, Port.usb],
  [Model.TD_2320D_203]: [Port.wifi, Port.usb],
  [Model.TD_2030AD]: [Port.usb],
  [Model.TD_2350D_300]: [Port.wifi, Port.bluetooth, Port.usb, Port.bluetoothLowEnergy],
};
export const printerAliases: Record<string, Model> = {
  QL800: Model.QL_800,
  QL810W: Model.QL_810W,
  QL820NWB: Model.QL_820NWB,
  QL820NWBC: Model.QL_820NWB,
  TD2320D: Model.TD_2320D_203,
  TD2320D203: Model.TD_2320D_203,
  TD2030A: Model.TD_2030AD,
  TD2030AD: Model.TD_2030AD,
  TD2350D: Model.TD_2350D_300,
  TD2350D300: Model.TD_2350D_300,
};
