declare module '@capacitor/core' {
  interface PluginRegistry {
    BrotherPrint: BrotherPrintPlugin;
  }
}

export interface BrotherPrintPlugin {
  print(options: BrotherPrintOptions): Promise<{ value: boolean }>;
  searchWiFiPrinter(): Promise<void>;
  searchBLEPrinter(): Promise<void>;
}

export interface BrotherPrintOptions {
  encodedImage: string;
  printerType: 'QL-820NWB' | 'QL-800';
}
