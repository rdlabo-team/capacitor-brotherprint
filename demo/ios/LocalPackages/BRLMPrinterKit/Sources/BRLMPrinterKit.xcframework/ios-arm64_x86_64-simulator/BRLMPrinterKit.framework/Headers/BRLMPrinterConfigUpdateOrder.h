//
//  BRLMPrinterConfigUpdateRequest.h
//  BRLMPrinterKit
//
//  Copyright © 2024 Brother Industries. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BRLMPrinterConfig.h"

NS_ASSUME_NONNULL_BEGIN

@interface BRLMPrinterConfigUpdateOrder : NSObject

@property (nonatomic, strong) BRLMPrinterConfigAutoPowerOffTime* autoPowerOffTime;
@property (nonatomic, strong) NSString* bluetoothDeviceName;
@property (nonatomic, strong) NSNumber*/*(bool)*/ jpegPrintAutoScaling;
@property (nonatomic, strong) NSNumber*/*(BRLMPrinterConfigJpegPrintHalftoneStyle)*/ jpegPrintHalftoneStyle;
@property (nonatomic, strong) NSNumber*/*(BRLMPrinterConfigPrintDensity)*/ printDensity;
@property (nonatomic, strong) BRLMPrinterConfigPrintSpeed* printSpeed;
@property (nonatomic, strong) NSNumber*/*(BRLMPrinterConfigPrintSpeedPJ)*/ printSpeedPJ;

- (instancetype)init;

@end

NS_ASSUME_NONNULL_END