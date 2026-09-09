import { expect, it, vi } from 'vitest';

import type { BRLMPrintOptions } from '../src/interfaces';
import { BrotherPrintWeb } from '../src/web';

it('rejects unsupported web printing without logging image data', async () => {
  const log = vi.spyOn(console, 'log');
  try {
    await expect(
      new BrotherPrintWeb().printImage({ encodedImage: 'private-image' } as BRLMPrintOptions),
    ).rejects.toMatchObject({ code: 'UNSUPPORTED' });
    expect(log).not.toHaveBeenCalled();
  } finally {
    log.mockRestore();
  }
});
