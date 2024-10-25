import { WebPlugin } from '@capacitor/core';
import {
  BrotherPrintPlugin,
  BRLMPrintOptions,
  BRLMSearchOption,
} from './definitions';

export class BrotherPrintWeb extends WebPlugin implements BrotherPrintPlugin {
  async printImage(_options: BRLMPrintOptions): Promise<void> {}
  async search(_options: BRLMSearchOption): Promise<void> {}
  async cancelSearchWiFiPrinter(): Promise<void> {}
  async cancelSearchBluetoothPrinter(): Promise<void> {}
}
