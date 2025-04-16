//
//  BRLMPrintError.h
//  BRLMPrinterKit
//
//  Copyright © 2020 Brother Industries, Ltd. All rights reserved.
//

#import <BRLMPrinterKit/BRLMLog.h>

typedef NS_ENUM(NSInteger, BRLMPrintErrorCode) {
    BRLMPrintErrorCodeNoError = 0,
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
    BRLMPrintErrorCodePrinterStatusErrorMotorSlow,
    BRLMPrintErrorCodeUnsupportedCharger,
    BRLMPrintErrorCodePrinterStatusErrorIncompatibleOptionalEquipment,
    BRLMPrintErrorCodePrinterStatusErrorUnknownError,
    BRLMPrintErrorCodeTemplatePrintNotSupported,
    BRLMPrintErrorCodeInvalidTemplateKey,
    BRLMPrintErrorCodeUnknownError,
};

NS_ASSUME_NONNULL_BEGIN

@interface BRLMPrintError : NSObject

@property(nonatomic, readonly) BRLMPrintErrorCode code;
@property(nonatomic, readonly) NSString* errorDescription;
@property(nonatomic, readonly) NSArray<BRLMLog*>* allLogs;

@end

NS_ASSUME_NONNULL_END
