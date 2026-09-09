/** Stable application-level error categories; native SDK values remain available separately. */
export type BrotherPrinterErrorCode =
  | 'INVALID_ARGUMENT'
  | 'PERMISSION_DENIED'
  | 'NOT_FOUND'
  | 'UNSUPPORTED'
  | 'CANCELLED'
  | 'BUSY'
  | 'COMMUNICATION'
  | 'PRINT_FAILED';
export class BrotherPrinterError extends Error {
  constructor(
    public readonly code: BrotherPrinterErrorCode,
    message: string,
    public override readonly cause?: unknown,
    public readonly nativeCode?: string | number,
  ) {
    super(message);
    this.name = 'BrotherPrinterError';
  }
}
export function printerError(error: unknown, fallback: BrotherPrinterErrorCode): BrotherPrinterError {
  if (error instanceof BrotherPrinterError) return error;
  const info = error as { code?: string; message?: string; data?: { nativeCode?: string | number } } | null;
  const codes: readonly string[] = [
    'INVALID_ARGUMENT',
    'PERMISSION_DENIED',
    'NOT_FOUND',
    'UNSUPPORTED',
    'UNIMPLEMENTED',
    'CANCELLED',
    'BUSY',
    'COMMUNICATION',
    'PRINT_FAILED',
  ];
  const code =
    info?.code && codes.includes(info.code)
      ? info.code === 'UNIMPLEMENTED'
        ? 'UNSUPPORTED'
        : (info.code as BrotherPrinterErrorCode)
      : fallback;
  return new BrotherPrinterError(
    code,
    info?.message ?? 'Printer operation failed',
    error,
    info?.data?.nativeCode ?? info?.code,
  );
}
