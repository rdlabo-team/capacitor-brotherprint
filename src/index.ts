import { registerPlugin } from '@capacitor/core';

import type { BrotherPrintPlugin } from './definitions';
import {
  BRLMPrinterModelName,
  BRLMPrinterLabelName,
} from './brother-printer.enum';

/**
 * platform can select web, but this plugin is not work.
 */
const BRLMPlatformName = {
  model: (
    platform: 'web' | 'ios' | 'android',
    supportModelName: BRLMPrinterModelName,
  ): string => {
    if (platform !== 'android') {
      return supportModelName;
    }
    switch (supportModelName) {
      case BRLMPrinterModelName.QL_810W:
        return 'QL_810W';
      case BRLMPrinterModelName.QL_820NWB:
        return 'QL_820NWB';
      default:
        throw new Error('Invalid modelName. Please check support Model Name.');
    }
  },
  label: (
    platform: 'web' | 'ios' | 'android',
    label: BRLMPrinterLabelName,
  ): string => {
    if (platform !== 'android') {
      return label;
    }
    switch (label) {
      case BRLMPrinterLabelName.BRLMQLPrintSettingsLabelSizeRollW62:
        return 'W62';
      case BRLMPrinterLabelName.BRLMQLPrintSettingsLabelSizeRollW62RB:
        return 'W62RB';
      default:
        throw new Error('Invalid labelName. Please check support Label Name.');
    }
  },
};

const BrotherPrint = registerPlugin<BrotherPrintPlugin>('BrotherPrint', {
  web: () => import('./web').then(m => new m.BrotherPrintWeb()),
});

export * from './definitions';
export * from './web';
export * from './events.enum';
export * from './brother-printer.enum';
export { BrotherPrint, BRLMPlatformName };
