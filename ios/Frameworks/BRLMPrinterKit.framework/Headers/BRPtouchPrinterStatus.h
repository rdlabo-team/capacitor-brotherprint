//
//  BRPtouchPrinterStatus.h
//  BRPtouchPrinterKit
//
//  Copyright (c) 2016-2018 Brother Industries, Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef struct _PTSTATUSINFO {
    Byte	byHead;						// Head mark
    Byte	bySize;						// Size
    Byte	byBrotherCode;				// Brother code
    Byte	bySeriesCode;				// Serial code
    Byte	byModelCode;				// Model code
    Byte	byNationCode;				// Nation code
    Byte	byFiller;					// information about cover
    Byte	byFiller2;					// Not used
    Byte	byErrorInf;					// Error information 1
    Byte	byErrorInf2;				// Error information 2
    Byte	byMediaWidth;				// Media width
    Byte	byMediaType;				// Media type
    Byte	byColorNum;					// The number of colors
    Byte	byFont;						// Font
    Byte	byJapanesFont;				// Japanese font
    Byte	byMode;						// Mode
    Byte	byDensity;					// Density
    Byte	byMediaLength;				// Media Length
    Byte	byStatusType;				// Status Type
    Byte	byPhaseType;				// Phase type
    Byte	byPhaseNoHi;				// Upper bytes of phase number
    Byte	byPhaseNoLow;				// Lower bytes of phase number
    Byte	byNoticeNo;					// Notice number
    Byte	byExtByteNum;				// Total bytes of extended part
    Byte	byLabelColor;				// Color of label
    Byte	byFontColor;				// Color of font
    Byte	byHardWareSetting[4];		// Settings of hardware
    Byte	byNoUse[2];                 // Not Use
} PTSTATUSINFO, *LPPTSTATUSINFO;

typedef NS_ENUM(NSInteger, BRPtouchPrinterStatusBatteryTernary) {
    BRPtouchPrinterStatusBatteryTernaryUnknown = -1,
    BRPtouchPrinterStatusBatteryTernaryYes = 1,
    BRPtouchPrinterStatusBatteryTernaryNo = 0,
};
@interface BRPtouchPrinterStatus : NSObject
@property (nonatomic) PTSTATUSINFO statusInfo;

// battery
@property (nonatomic) int32_t batteryResidualQuantityLevel;
@property (nonatomic) int32_t maxOfBatteryResidualQuantityLevel;

@property (nonatomic) BRPtouchPrinterStatusBatteryTernary isACConnected;
@property (nonatomic) BRPtouchPrinterStatusBatteryTernary isBatteryMounted;

@property (nonatomic) int16_t batteryLevel __deprecated;

@end
