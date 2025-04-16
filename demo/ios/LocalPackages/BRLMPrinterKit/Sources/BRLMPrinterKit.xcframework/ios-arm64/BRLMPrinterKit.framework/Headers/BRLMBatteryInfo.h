//
//  BRLMBatteryInfo.h
//  BRLMPrinterKit
//
//  Created by shintaro on 2023/07/03.
//  Copyright © 2023 Brother Industries. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, BRLMBatteryInfoHealthStatus ) {
        BRLMBatteryInfoHealthStatusExcellent = 0,
        BRLMBatteryInfoHealthStatusGood = 1,
        BRLMBatteryInfoHealthStatusReplaceSoon = 2,
        BRLMBatteryInfoHealthStatusReplaceBattery = 3,
        BRLMBatteryInfoHealthStatusNotInstalled = 7,
};

@interface BRLMBatteryInfo : NSObject

- (instancetype)init __unavailable;
- (instancetype)copy __unavailable;

@property (nonatomic, readonly) NSInteger chargeLevel;
@property (nonatomic, readonly) NSInteger healthLevel;
@property (nonatomic, readonly) BRLMBatteryInfoHealthStatus healthStatus;


@end
