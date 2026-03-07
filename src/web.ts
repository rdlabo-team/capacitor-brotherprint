import { WebPlugin } from '@capacitor/core';

import type { BrotherPrintPlugin } from './definitions';
import type { BRLMChannelResult, BRLMPrintOptions, BRLMSearchOption, isChannelAvailableResult } from './interfaces';

export class BrotherPrintWeb extends WebPlugin implements BrotherPrintPlugin {
  async printImage(options: BRLMPrintOptions): Promise<void> {
    console.log('printImage', options);
  }
  async isChannelAvailable(options: BRLMChannelResult): Promise<isChannelAvailableResult> {
    console.log('isChannelAvailable', options);
    return {
      result: false,
    };
  }
  async search(options: BRLMSearchOption): Promise<void> {
    console.log('search', options);
  }
  async cancelSearchWiFiPrinter(): Promise<void> {
    console.log('cancelSearchWiFiPrinter');
  }
  async cancelSearchBluetoothPrinter(): Promise<void> {
    console.log('cancelSearchBluetoothPrinter');
  }
}
