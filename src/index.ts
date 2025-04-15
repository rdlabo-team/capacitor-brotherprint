import { registerPlugin } from '@capacitor/core';

import type { BrotherPrintPlugin } from './definitions';

const BrotherPrint = registerPlugin<BrotherPrintPlugin>('BrotherPrint', {
  web: () => import('./web').then((m) => new m.BrotherPrintWeb()),
});

export * from './definitions';
export * from './web';
export * from './events.enum';
export * from './brother-printer.enum';
export { BrotherPrint };
