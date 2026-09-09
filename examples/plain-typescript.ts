import {
  BrotherPrint,
  BrotherPrinter,
  BRLMPrinterModelName,
  BRLMPrinterPort,
  BRLMPrinterLabelName,
  type BRLMChannelResult,
} from '@rdlabo/capacitor-brotherprint';

/** Framework-independent usage. A React effect can own this controller and call dispose on cleanup. */
export async function printLabel(
  encodedImage: string,
  select: (channels: readonly BRLMChannelResult[]) => Promise<BRLMChannelResult | null>,
): Promise<void> {
  const printer = new BrotherPrinter({ plugin: BrotherPrint });
  try {
    await printer.listen();
    await printer.search(BRLMPrinterPort.wifi, { model: BRLMPrinterModelName.QL_820NWB, searchDuration: 10 });
    const channel = await printer.selectChannel(select);
    if (!channel) return;
    await printer.print(
      { modelName: BRLMPrinterModelName.QL_820NWB, encodedImage, labelName: BRLMPrinterLabelName.RollW62 },
      channel,
    );
  } finally {
    await printer.dispose();
  }
}
