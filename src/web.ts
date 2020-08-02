import { WebPlugin } from '@capacitor/core';
import { BrotherPrintPlugin } from './definitions';

export class BrotherPrintWeb extends WebPlugin implements BrotherPrintPlugin {
  constructor() {
    super({
      name: 'BrotherPrint',
      platforms: ['web'],
    });
  }

  async echo(options: { value: string }): Promise<{ value: string }> {
    console.log('ECHO', options);
    return options;
  }
}

const BrotherPrint = new BrotherPrintWeb();

export { BrotherPrint };

import { registerWebPlugin } from '@capacitor/core';
registerWebPlugin(BrotherPrint);
