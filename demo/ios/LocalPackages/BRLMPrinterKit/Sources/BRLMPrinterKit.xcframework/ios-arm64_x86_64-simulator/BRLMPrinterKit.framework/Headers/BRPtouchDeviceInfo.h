//
//  BRPtouchDevice.h
//  BRSearchModule
//
//  Copyright (c) 2015-2018 Brother Industries, Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface BRPtouchDeviceInfo : NSObject

@property (copy,nonatomic)NSString* strIPAddress;
@property (copy,nonatomic)NSString* strLocation;
@property (copy,nonatomic)NSString* strModelName;
@property (copy,nonatomic)NSString* strPrinterName;
@property (copy,nonatomic)NSString* strSerialNumber;
@property (copy,nonatomic)NSString* strNodeName;
@property (copy,nonatomic)NSString* strMACAddress;
@property (copy,nonatomic)NSString* strBLEAdvertiseLocalName;

- (NSString *)description;

@end
