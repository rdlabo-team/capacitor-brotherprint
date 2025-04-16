//
//  BRLMPrinterConfigUpdateResult.h
//  BRLMPrinterKit
//
//  Copyright © 2024 Brother Industries. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BRLMLog.h"

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, BRLMPrinterConfigUpdateResultCode) {
    BRLMPrinterConfigUpdateResultCodeSuccess = 20500,
    BRLMPrinterConfigUpdateResultCodeInvalidValueToSet,
    BRLMPrinterConfigUpdateResultCodeUnsupported,
    BRLMPrinterConfigUpdateResultCodeUnsupportedOption,
    BRLMPrinterConfigUpdateResultCodeUnsupportedConnectionInterface,
    BRLMPrinterConfigUpdateResultCodeCommunicationFailed,
    BRLMPrinterConfigUpdateResultCodeNotSet = 20598,
    BRLMPrinterConfigUpdateResultCodeUnknownError = 20599
};

@interface BRLMPrinterConfigUpdateResult : NSObject

@property (nonatomic, assign, readonly) BRLMPrinterConfigUpdateResultCode autoPowerOffTime;
@property (nonatomic, assign, readonly) BRLMPrinterConfigUpdateResultCode bluetoothDeviceName;
@property (nonatomic, assign, readonly) BRLMPrinterConfigUpdateResultCode jpegPrintAutoScaling;
@property (nonatomic, assign, readonly) BRLMPrinterConfigUpdateResultCode jpegPrintHalftoneStyle;
@property (nonatomic, assign, readonly) BRLMPrinterConfigUpdateResultCode printDensity;
@property (nonatomic, assign, readonly) BRLMPrinterConfigUpdateResultCode printSpeed;
@property (nonatomic, assign, readonly) BRLMPrinterConfigUpdateResultCode printSpeedPJ;
@property (nonatomic, strong, readonly) NSArray<BRLMLog *> *allLogs;

- (instancetype)init __unavailable;
@end

NS_ASSUME_NONNULL_END