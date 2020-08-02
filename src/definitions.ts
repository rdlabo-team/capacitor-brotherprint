declare module '@capacitor/core' {
  interface PluginRegistry {
    BrotherPrint: BrotherPrintPlugin;
  }
}

export interface BrotherPrintPlugin {
  echo(options: { encodedImage: string }): Promise<{ value: boolean }>;
}
