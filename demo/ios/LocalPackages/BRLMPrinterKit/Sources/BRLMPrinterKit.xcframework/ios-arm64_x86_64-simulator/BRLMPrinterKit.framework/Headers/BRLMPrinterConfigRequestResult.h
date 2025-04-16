//
//  BRLMPrinterConfigRequestResult.h
//  BRLMPrinterKit
//
//  Copyright © 2024 Brother Industries. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "BRLMPrinterConfig.h"

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, BRLMPrinterConfigRequestResultCode) {
    BRLMPrinterConfigRequestResultCodeSuccess = 20400,
    BRLMPrinterConfigRequestResultCodeUnsupported,
    BRLMPrinterConfigRequestResultCodeUnsupportedOption,
    BRLMPrinterConfigRequestResultCodeUnsupportedConnectionInterface,
    BRLMPrinterConfigRequestResultCodeCommunicationFailed,
    BRLMPrinterConfigRequestResultCodeNotGet = 20498,
    BRLMPrinterConfigRequestResultCodeUnknownError = 20499
};

@interface BRLMPrinterConfigRequestResultReport<T> : NSObject

@property (nonatomic, assign, readonly) BRLMPrinterConfigRequestResultCode code;
@property (nonatomic, strong, readonly) T value;

@end

@interface BRLMPrinterConfigRequestResult : NSObject

@property (nonatomic, strong, readonly) BRLMPrinterConfigRequestResultReport<BRLMPrinterConfigAutoPowerOffTime*> *autoPowerOffTime;
@property (nonatomic, strong, readonly) BRLMPrinterConfigRequestResultReport<NSString*> *bluetoothDeviceName;
@property (nonatomic, strong, readonly) BRLMPrinterConfigRequestResultReport<NSNumber*/*(bool)*/> *jpegPrintAutoScaling;
@property (nonatomic, strong, readonly) BRLMPrinterConfigRequestResultReport<NSNumber*/*(BRLMPrinterConfigJpegPrintHalftoneStyle)*/> *jpegPrintHalftoneStyle;
@property (nonatomic, strong, readonly) BRLMPrinterConfigRequestResultReport<NSNumber*/*(BRLMPrinterConfigPrintDensity)*/> *printDensity;
@property (nonatomic, strong, readonly) BRLMPrinterConfigRequestResultReport<BRLMPrinterConfigPrintSpeed*> *printSpeed;
@property (nonatomic, strong, readonly) BRLMPrinterConfigRequestResultReport<NSNumber*/*(BRLMPrinterConfigPrintSpeedPJ)*/> *printSpeedPJ;
@property (nonatomic, strong, readonly) NSArray<BRLMLog *> *allLogs;

- (instancetype)init __unavailable;

@end



NS_ASSUME_NONNULL_END