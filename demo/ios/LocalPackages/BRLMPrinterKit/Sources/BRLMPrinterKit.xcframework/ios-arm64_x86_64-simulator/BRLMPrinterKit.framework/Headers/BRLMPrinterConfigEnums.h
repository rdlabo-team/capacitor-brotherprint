//
//  BRLMPrinterConfigEnums.h
//  BRLMPrinterKit
//
//  Copyright © 2024 Brother Industries. All rights reserved.
//

#import <Foundation/Foundation.h>
typedef NS_ENUM(NSInteger, BRLMPrinterConfigJpegPrintHalftoneStyle) {
    BRLMPrinterConfigJpegPrintHalftoneStyleSimpleThreshold = 0,
    BRLMPrinterConfigJpegPrintHalftoneStyleErrorDiffusion = 1,
};

typedef NS_ENUM(NSInteger, BRLMPrinterConfigPrintDensity) {
    BRLMPrinterConfigPrintDensityHigh5_Highest = 0,
    BRLMPrinterConfigPrintDensityHigh4 = 1,
    BRLMPrinterConfigPrintDensityHigh3 = 2,
    BRLMPrinterConfigPrintDensityHigh2 = 3,
    BRLMPrinterConfigPrintDensityHigh1 = 4,
    BRLMPrinterConfigPrintDensityMedium = 5,
    BRLMPrinterConfigPrintDensityLow1 = 6,
    BRLMPrinterConfigPrintDensityLow2 = 7,
    BRLMPrinterConfigPrintDensityLow3 = 8,
    BRLMPrinterConfigPrintDensityLow4 = 9,
    BRLMPrinterConfigPrintDensityLow5_Lowest = 10,
};

typedef NS_ENUM(NSInteger, BRLMPrinterConfigPrintSpeedPJ) {
    BRLMPrinterConfigPrintSpeedPJHighSpeed = 0,
    BRLMPrinterConfigPrintSpeedPJMediumHighSpeed = 1,
    BRLMPrinterConfigPrintSpeedPJMediumLowSpeed = 2,
    BRLMPrinterConfigPrintSpeedPJLowSpeed = 3,
    BRLMPrinterConfigPrintSpeedPJFast_DraftQuality = 4,
    BRLMPrinterConfigPrintSpeedPJFast_LineConversion = 5,
};

