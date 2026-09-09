import { WebPlugin } from '@capacitor/core';

import type { BrotherPrintPlugin } from './definitions';
import { BrotherPrinterError } from './errors';
import type { BRLMChannelResult, BRLMPrintOptions, BRLMSearchOption, isChannelAvailableResult } from './interfaces';

export class BrotherPrintWeb extends WebPlugin implements BrotherPrintPlugin {
  async printImage(_options: BRLMPrintOptions): Promise<void> {
    void _options;
    throw new BrotherPrinterError('UNSUPPORTED', 'Brother printing requires Android or iOS');
  }
  async isChannelAvailable(_options: BRLMChannelResult): Promise<isChannelAvailableResult> {
    void _options;
    throw new BrotherPrinterError('UNSUPPORTED', 'Brother printing requires Android or iOS');
  }
  async search(_options: BRLMSearchOption): Promise<void> {
    void _options;
    throw new BrotherPrinterError('UNSUPPORTED', 'Brother discovery requires Android or iOS');
  }
  async cancelSearchWiFiPrinter(): Promise<void> {
    throw new BrotherPrinterError('UNSUPPORTED', 'Brother discovery requires Android or iOS');
  }
  async cancelSearchBluetoothPrinter(): Promise<void> {
    throw new BrotherPrinterError('UNSUPPORTED', 'Brother discovery requires Android or iOS');
  }
}
