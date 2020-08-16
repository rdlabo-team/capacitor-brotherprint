//
//  BRLMPrintError.h
//  BRLMPrinterKit
//
//  Copyright © 2020 Brother Industries, Ltd. All rights reserved.
//

#import "BRLMError.h"

typedef NS_ENUM(NSInteger, BRLMPrintErrorCode) {
    BRLMPrintErrorCodeNoError,
    BRLMPrintErrorCodePrintSettingsError,
    BRLMPrintErrorCodeFilepathURLError,
    BRLMPrintErrorCodePDFPageError,
    BRLMPrintErrorCodePrintSettingsNotSupportError,
    BRLMPrintErrorCodeDataBufferError,
    BRLMPrintErrorCodePrinterModelError,
    BRLMPrintErrorCodeCanceled,
    BRLMPrintErrorCodeChannelTimeout,
    BRLMPrintErrorCodeSetModelError,
    BRLMPrintErrorCodeUnsupportedFile,
    BRLMPrintErrorCodeSetMarginError,
    BRLMPrintErrorCodeSetLabelSizeError,
    BRLMPrintErrorCodeCustomPaperSizeError,
    BRLMPrintErrorCodeSetLengthError,
    BRLMPrintErrorCodeTubeSettingError,
    BRLMPrintErrorCodeChannelErrorStreamStatusError,
    BRLMPrintErrorCodeChannelErrorUnsupportedChannel,
    BRLMPrintErrorCodePrinterStatusErrorPaperEmpty,
    BRLMPrintErrorCodePrinterStatusErrorCoverOpen,
    BRLMPrintErrorCodePrinterStatusErrorBusy,
    BRLMPrintErrorCodePrinterStatusErrorPrinterTurnedOff,
    BRLMPrintErrorCodePrinterStatusErrorBatteryWeak,
    BRLMPrintErrorCodePrinterStatusErrorExpansionBufferFull,
    BRLMPrintErrorCodePrinterStatusErrorCommunicationError,
    BRLMPrintErrorCodePrinterStatusErrorPaperJam,
    BRLMPrintErrorCodePrinterStatusErrorMediaCannotBeFed,
    BRLMPrintErrorCodePrinterStatusErrorOverHeat,
    BRLMPrintErrorCodePrinterStatusErrorHighVoltageAdapter,
    BRLMPrintErrorCodePrinterStatusErrorUnknownError,
    BRLMPrintErrorCodeUnknownError,
};

NS_ASSUME_NONNULL_BEGIN

@interface BRLMPrintError : BRLMError

@property(nonatomic, readonly) BRLMPrintErrorCode code;

@end

NS_ASSUME_NONNULL_END
