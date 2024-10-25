import { WebPlugin } from '@capacitor/core';

import type {
  BrotherPrintPlugin,
  BRLMPrintOptions,
  BRLMSearchOption,
} from './definitions';

export class BrotherPrintWeb extends WebPlugin implements BrotherPrintPlugin {
  async printImage(options: BRLMPrintOptions): Promise<void> {
    console.log('printImage', options);
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
