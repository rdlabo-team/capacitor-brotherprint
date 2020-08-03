import { WebPlugin } from '@capacitor/core';
import { BrotherPrintPlugin } from './definitions';

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
  async print(options: {
    encodedImage: string;
    printerType: string;
  }): Promise<{ value: boolean }> {
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
