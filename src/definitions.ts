declare module '@capacitor/core' {
  interface PluginRegistry {
    BrotherPrint: BrotherPrintPlugin;
  }
}

export interface BrotherPrintPlugin {
  echo(options: { value: string }): Promise<{ value: string }>;
}
