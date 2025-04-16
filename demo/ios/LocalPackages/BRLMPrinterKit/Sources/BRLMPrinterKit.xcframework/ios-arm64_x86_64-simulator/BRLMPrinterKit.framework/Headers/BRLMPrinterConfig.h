//
//  BRLMPrinterConfig.h
//  BRLMPrinterKit
//
//  Copyright © 2024 Brother Industries. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "BRLMPrinterConfigEnums.h"
#import "BRLMPrinterDefine.h"
#import "BRLMLog.h"

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, BRLMPrinterConfigAutoPowerOffTimeCreateResultErrorCode) {
    BRLMPrinterConfigAutoPowerOffTimeCreateResultErrorCodeSuccess = 20600,
    BRLMPrinterConfigAutoPowerOffTimeCreateResultErrorCodeUnsupported,
    BRLMPrinterConfigAutoPowerOffTimeCreateResultErrorCodeUnsupportedFunctionInsteadUseCreateWithMinutes,
    BRLMPrinterConfigAutoPowerOffTimeCreateResultErrorCodeUnsupportedFunctionInsteadUseCreateWithMinutesLegacy,
    BRLMPrinterConfigAutoPowerOffTimeCreateResultErrorCodeInvalidValue,
};

@class BRLMPrinterConfigAutoPowerOffTime;

@interface BRLMPrinterConfigAutoPowerOffTimeCreateResult : NSObject

@property (nonatomic, readonly, nullable) BRLMPrinterConfigAutoPowerOffTime *time;
@property (nonatomic, readonly) BRLMPrinterConfigAutoPowerOffTimeCreateResultErrorCode errorCode;
@property (nonatomic, readonly) NSArray<BRLMLog *> *allLogs;

@end


@interface BRLMPrinterConfigAutoPowerOffTime : NSObject

@property (nonatomic, readonly) NSInteger minutesOnPower;
@property (nonatomic, readonly) NSInteger minutesOnBattery;

- (instancetype)init __unavailable;

@end

typedef NSInteger BRLMPrinterConfigAutoPowerOffTimeMinutes;

@interface BRLMPrinterConfigAutoPowerOffTimeFactory : NSObject
+ (BRLMPrinterConfigAutoPowerOffTimeCreateResult*)createWithMinutes:(BRLMPrinterModel)model minutes:(BRLMPrinterConfigAutoPowerOffTimeMinutes)minutes;
+ (BRLMPrinterConfigAutoPowerOffTimeCreateResult*)createWithMinutesLegacy:(BRLMPrinterModel)model minutesOnPower:(BRLMPrinterConfigAutoPowerOffTimeMinutes)minutesOnPower minutesOnBattery:(BRLMPrinterConfigAutoPowerOffTimeMinutes)minutesOnBattery;

+ (BRLMPrinterConfigAutoPowerOffTimeMinutes)minutesNeverPowerOff;
+ (BRLMPrinterConfigAutoPowerOffTimeMinutes)minutesKeepCurrent;
+ (BRLMPrinterConfigAutoPowerOffTimeMinutes)minutesInvalid;
@end


typedef NS_ENUM(NSInteger, BRLMPrinterConfigPrintSpeedPolicy) {
    BRLMPrinterConfigPrintSpeedPolicyAsFastAsPossible = 0,
    BRLMPrinterConfigPrintSpeedPolicyMiddleSpeed,
    BRLMPrinterConfigPrintSpeedPolicyAsSlowAsPossible,
};

typedef NS_ENUM(NSInteger, BRLMPrinterConfigPrintSpeedRate) {
    BRLMPrinterConfigPrintSpeedRate1ips = 1,
    BRLMPrinterConfigPrintSpeedRate2ips = 2,
    BRLMPrinterConfigPrintSpeedRate3ips = 3,
    BRLMPrinterConfigPrintSpeedRate4ips = 4,
    BRLMPrinterConfigPrintSpeedRate5ips = 5,
    BRLMPrinterConfigPrintSpeedRate6ips = 6,
    BRLMPrinterConfigPrintSpeedRate7ips = 7,
    BRLMPrinterConfigPrintSpeedRate8ips = 8,
};

@class BRLMPrinterConfigPrintSpeed;

typedef NS_ENUM(NSInteger, BRLMPrinterConfigPrintSpeedCreateResultErrorCode) {
    BRLMPrinterConfigPrintSpeedCreateResultErrorCodeSuccess = 20700,
    BRLMPrinterConfigPrintSpeedCreateResultErrorCodeUnsupported,
    BRLMPrinterConfigPrintSpeedCreateResultErrorCodeInvalidValue
};

@interface BRLMPrinterConfigPrintSpeedCreateResult : NSObject

@property (nonatomic, readonly, nullable) BRLMPrinterConfigPrintSpeed *speed;
@property (nonatomic, readonly) BRLMPrinterConfigPrintSpeedCreateResultErrorCode errorCode;
@property (nonatomic, readonly) NSArray<BRLMLog *> *allLogs;

@end

@interface BRLMPrinterConfigPrintSpeed : NSObject

@property (nonatomic, readonly) NSInteger rate;
@property (nonatomic, readonly) NSInteger rawModelID;

- (instancetype)init __unavailable;

@end

@interface BRLMPrinterConfigPrintSpeedFactory : NSObject
+ (BRLMPrinterConfigPrintSpeedCreateResult*)createWithPolicy:(BRLMPrinterModel)model policy:(BRLMPrinterConfigPrintSpeedPolicy)policy;
+ (BRLMPrinterConfigPrintSpeedCreateResult*)createWithRate:(BRLMPrinterModel)model rate:(BRLMPrinterConfigPrintSpeedRate)rate;
@end


NS_ASSUME_NONNULL_END
