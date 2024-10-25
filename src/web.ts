import { WebPlugin } from '@capacitor/core';
import { BrotherPrintPlugin, BrotherPrintOptions } from './definitions';

export class BrotherPrintWeb extends WebPlugin implements BrotherPrintPlugin {
  /**
   * Print with Base64
   */
  async printImage(_options: BrotherPrintOptions): Promise<void> {}

  /**
   * Search Wifi Printer
   */
  async searchWiFiPrinter(): Promise<void> {}

  /**
   * search Bluetooth Printer
   */
  async searchBLEPrinter(): Promise<void> {}
}
