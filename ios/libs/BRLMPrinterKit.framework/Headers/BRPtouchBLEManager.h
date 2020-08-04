//
//  BRPtouchBLEManager.h
//  BRPtouchPrinterKit
//
//  Copyright (c) 2017-2018 Brother Industries, Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BRPtouchDeviceInfo.h"
#import <CoreBluetooth/CoreBluetooth.h>

@interface BRPtouchBLEManager : NSObject

+ (BRPtouchBLEManager *)sharedManager;

/**
 Start searching BLE printers
 @return NO if BLE is not available, otherwise YES
 */
typedef void (^BRBLEManagerSearchCompletionHandler)(BRPtouchDeviceInfo *deviceInfo);
- (BOOL)startSearchWithCompletionHandler:(BRBLEManagerSearchCompletionHandler)handler;

/**
 Stop searching BLE printers
 */
- (void)stopSearch;

@end
