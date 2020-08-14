import { WebPlugin } from '@capacitor/core';
import { BrotherPrintPlugin, BrotherPrintOptions } from './definitions';

export class BrotherPrintWeb extends WebPlugin implements BrotherPrintPlugin {
  constructor() {
    super({
      name: 'BrotherPrint',
      platforms: ['web'],
    });
  }

  /**
   * Print with Base64
   * @param options
   */
  async printImage(options: BrotherPrintOptions): Promise<{ value: boolean }> {
    console.log('ECHO', options);
    return {
      value: true,
    };
  }

  /**
   * Search Wifi Printer
   */
  async searchWiFiPrinter(): Promise<void> {}

  /**
   * search Bluetooth Printer
   */
  async searchBLEPrinter(): Promise<void> {}
}

const BrotherPrint = new BrotherPrintWeb();

export { BrotherPrint };

import { registerWebPlugin } from '@capacitor/core';
registerWebPlugin(BrotherPrint);
