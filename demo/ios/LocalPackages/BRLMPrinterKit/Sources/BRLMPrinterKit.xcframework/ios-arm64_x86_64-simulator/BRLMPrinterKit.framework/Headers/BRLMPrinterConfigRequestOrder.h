//
//  BRLMPrinterConfigRequestOrder.h
//  BRLMPrinterKit
//
//  Copyright © 2024 Brother Industries. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface BRLMPrinterConfigRequestOrder : NSObject

@property (nonatomic, assign) BOOL autoPowerOffTime;
@property (nonatomic, assign) BOOL bluetoothDeviceName;
@property (nonatomic, assign) BOOL jpegPrintAutoScaling;
@property (nonatomic, assign) BOOL jpegPrintHalftoneStyle;
@property (nonatomic, assign) BOOL printDensity;
@property (nonatomic, assign) BOOL printSpeed;
@property (nonatomic, assign) BOOL printSpeedPJ;

- (instancetype)init;

@end

NS_ASSUME_NONNULL_END