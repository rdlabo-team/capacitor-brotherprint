//
//  BRPtouchBatteryInfo.h
//  BRPtouchPrinterKit
//
//  Copyright (c) 2018 Brother Industries, Ltd. All rights reserved.
//
#import <Foundation/Foundation.h>

typedef NS_ENUM(NSUInteger, BRPtouchBatteryInfoBatteryHealthStatus) {
    BRPtouchBatteryInfoBatteryHealthStatusExcellent,
    BRPtouchBatteryInfoBatteryHealthStatusGood,
    BRPtouchBatteryInfoBatteryHealthStatusReplaceSoon,
    BRPtouchBatteryInfoBatteryHealthStatusReplaceBattery,
    BRPtouchBatteryInfoBatteryHealthStatusNotInstalled,
};

@interface BRPtouchBatteryInfo : NSObject
@property (nonatomic, readonly) int8_t  batteryChargeLevel; // 0~100[%] Not Installed: -1
@property (nonatomic, readonly) int8_t  batteryHealthLevel; // 0~100[%] Not Installed: -1
@property (nonatomic, readonly) BRPtouchBatteryInfoBatteryHealthStatus batteryHealthStatus;

- (instancetype)initWithChargeLevel:(int8_t)chargeLevel
                        healthLevel:(int8_t)healthLevel
                   healthStatusData:(int8_t)healthStatusData;

@end
