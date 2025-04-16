//
//  BRLMPrinterStatus.h
//  BRLMPrinterKit
//
//  Copyright © 2020 Brother Industries, Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <BRLMPrinterKit/BRLMPrinterDefine.h>
#import <BRLMPrinterKit/BRLMPTPrintSettings.h>
#import <BRLMPrinterKit/BRLMQLPrintSettings.h>

typedef struct {
    Byte    byHead;                // Head mark
    Byte    bySize;                // Size
    Byte    byBrotherCode;         // Brother code
    Byte    bySeriesCode;          // Serial code
    Byte    byModelCode;           // Model code
    Byte    byNationCode;          // Nation code
    Byte    byFiller;              // information about cover
    Byte    byFiller2;             // Not used
    Byte    byErrorInf;            // Error information 1
    Byte    byErrorInf2;           // Error information 2
    Byte    byMediaWidth;          // Media width
    Byte    byMediaType;           // Media type
    Byte    byColorNum;            // The number of colors
    Byte    byFont;                // Font
    Byte    byJapanesFont;         // Japanese font
    Byte    byMode;                // Mode
    Byte    byDensity;             // Density
    Byte    byMediaLength;         // Media Length
    Byte    byStatusType;          // Status Type
    Byte    byPhaseType;           // Phase type
    Byte    byPhaseNoHi;           // Upper bytes of phase number
    Byte    byPhaseNoLow;          // Lower bytes of phase number
    Byte    byNoticeNo;            // Notice number
    Byte    byExtByteNum;          // Total bytes of extended part
    Byte    byLabelColor;          // Color of label
    Byte    byFontColor;           // Color of font
    Byte    byHardWareSetting[4];  // Settings of hardware
    Byte    byNoUse[2];            // Not Use
} BRLMPrinterStatusRawDataStructure;

typedef NS_ENUM(NSInteger, BRLMPrinterBatteryStatusTernary) {
    BRLMPrinterBatteryStatusTernaryYes,
    BRLMPrinterBatteryStatusTernaryNo,
    BRLMPrinterBatteryStatusTernaryUnknown,
};

typedef NS_ENUM(NSInteger, BRLMPrinterStatusErrorCode) {
    BRLMPrinterStatusErrorCodeNoError,
    BRLMPrinterStatusErrorCodeNoPaper,
    BRLMPrinterStatusErrorCodeCoverOpen,
    BRLMPrinterStatusErrorCodeBusy,
    BRLMPrinterStatusErrorCodePaperJam,
    BRLMPrinterStatusErrorCodeHighVoltageAdapter,
    BRLMPrinterStatusErrorCodeBatteryEmpty,
    BRLMPrinterStatusErrorCodeBatteryTrouble,
    BRLMPrinterStatusErrorCodeTubeNotDetected,
    BRLMPrinterStatusErrorCodeMotorSlow,
    BRLMPrinterStatusErrorCodeUnsupportedCharger,
    BRLMPrinterStatusErrorCodeIncompatibleOptionalEquipment,
    BRLMPrinterStatusErrorCodeSystemError,
    BRLMPrinterStatusErrorCodeAnotherError,
};

typedef NS_ENUM(NSInteger, BRLMMediaInfoMediaType) {
    BRLMMediaInfoMediaTypePTLaminate,
    BRLMMediaInfoMediaTypePTNonLaminate,
    BRLMMediaInfoMediaTypePTFabric,
    BRLMMediaInfoMediaTypeQLInfiniteLable,
    BRLMMediaInfoMediaTypeQLDieCutLable,
    BRLMMediaInfoMediaTypePTHeatShrink,
    BRLMMediaInfoMediaTypePTFLe,
    BRLMMediaInfoMediaTypePTFlexibleID,
    BRLMMediaInfoMediaTypePTSatin,
    BRLMMediaInfoMediaTypePTSelfLaminate,
    BRLMMediaInfoMediaTypeIncompatible,
    BRLMMediaInfoMediaTypeUnknown,
};

typedef NS_ENUM(NSInteger, BRLMMediaInfoBackgroundColor) {
    BRLMMediaInfoBackgroundColorStandard,
    BRLMMediaInfoBackgroundColorWhite,
    BRLMMediaInfoBackgroundColorOthers,
    BRLMMediaInfoBackgroundColorClear,
    BRLMMediaInfoBackgroundColorRed,
    BRLMMediaInfoBackgroundColorBlue,
    BRLMMediaInfoBackgroundColorYellow,
    BRLMMediaInfoBackgroundColorGreen,
    BRLMMediaInfoBackgroundColorBlack,
    BRLMMediaInfoBackgroundColorClearWithWhiteInk,
    BRLMMediaInfoBackgroundColorPremiumGold,
    BRLMMediaInfoBackgroundColorPremiumSilver,
    BRLMMediaInfoBackgroundColorPremiumOthers,
    BRLMMediaInfoBackgroundColorMaskingOthers,
    BRLMMediaInfoBackgroundColorMatteWhite,
    BRLMMediaInfoBackgroundColorMatteClear,
    BRLMMediaInfoBackgroundColorMatteSilver,
    BRLMMediaInfoBackgroundColorSatinGold,
    BRLMMediaInfoBackgroundColorSatinSilver,
    BRLMMediaInfoBackgroundColorPastelPurple,
    BRLMMediaInfoBackgroundColorBlueWithWhiteInk,
    BRLMMediaInfoBackgroundColorRedWithWhiteInk,
    BRLMMediaInfoBackgroundColorFluorescentOrange,
    BRLMMediaInfoBackgroundColorFluorescentYellow,
    BRLMMediaInfoBackgroundColorBerryPink,
    BRLMMediaInfoBackgroundColorLightGray,
    BRLMMediaInfoBackgroundColorLimeGreen,
    BRLMMediaInfoBackgroundColorSatinNavyBlue,
    BRLMMediaInfoBackgroundColorSatinWineRed,
    BRLMMediaInfoBackgroundColorFabricYellow,
    BRLMMediaInfoBackgroundColorFabricPink,
    BRLMMediaInfoBackgroundColorFabricBlue,
    BRLMMediaInfoBackgroundColorTubeWhite,
    BRLMMediaInfoBackgroundColorSelfLaminatedWhite,
    BRLMMediaInfoBackgroundColorFlexibleWhite,
    BRLMMediaInfoBackgroundColorFlexibleYellow,
    BRLMMediaInfoBackgroundColorCleaningWhite,
    BRLMMediaInfoBackgroundColorStencilWhite,
    BRLMMediaInfoBackgroundColorLightBlue_Satin,
    BRLMMediaInfoBackgroundColorMint_Satin,
    BRLMMediaInfoBackgroundColorSilver_Satin,
    BRLMMediaInfoBackgroundColorIncompatible,
    BRLMMediaInfoBackgroundColorUnknown,
};

typedef NS_ENUM(NSInteger, BRLMMediaInfoInkColor) {
    BRLMMediaInfoInkColorStandard,
    BRLMMediaInfoInkColorWhite,
    BRLMMediaInfoInkColorOthers,
    BRLMMediaInfoInkColorRed,
    BRLMMediaInfoInkColorBlue,
    BRLMMediaInfoInkColorBlack,
    BRLMMediaInfoInkColorGold,
    BRLMMediaInfoInkColorRedAndBlack,
    BRLMMediaInfoInkColorFabricBlue,
    BRLMMediaInfoInkColorCleaningBlack,
    BRLMMediaInfoInkColorStencilBlack,
    BRLMMediaInfoInkColorIncompatible,
    BRLMMediaInfoInkColorUnknown,
};

typedef struct {
    // min is 0
    int max;
    int current;
} BRLMPrinterBatteryStatusFraction;

@interface BRLMPrinterBatteryStatus : NSObject

@property (nonatomic, readonly) BRLMPrinterBatteryStatusTernary batteryMounted;
@property (nonatomic, readonly) BRLMPrinterBatteryStatusTernary charging;
@property (nonatomic, readonly) BRLMPrinterBatteryStatusFraction chargeLevel;

- (nonnull instancetype)init __unavailable;
- (nonnull instancetype)copy __unavailable;

@end

@interface BRLMMediaInfo : NSObject

@property (nonatomic, readonly) BRLMMediaInfoMediaType mediaType;
@property (nonatomic, readonly) BRLMMediaInfoBackgroundColor backgroundColor;
@property (nonatomic, readonly) BRLMMediaInfoInkColor inkColor;
@property (nonatomic, readonly) int width_mm;
@property (nonatomic, readonly) int height_mm;
@property (nonatomic, readonly) bool isHeightInfinite;
- (BRLMPTPrintSettingsLabelSize) getPTLabelSize:(bool * _Nonnull)succeeded;
- (BRLMQLPrintSettingsLabelSize) getQLLabelSize:(bool * _Nonnull)succeeded;

- (nonnull instancetype)init __unavailable;
- (nonnull instancetype)copy __unavailable;

@end

@interface BRLMPrinterStatus : NSObject
@property (nonatomic, readonly) BRLMPrinterStatusRawDataStructure ptStatus __deprecated;
@property (nonatomic, readonly) BRLMPrinterStatusRawDataStructure rawData;
@property (nonatomic, readonly) BRLMPrinterModel model;
@property (nonatomic, readonly) BRLMPrinterStatusErrorCode errorCode;
@property (nonatomic, readonly, nullable) BRLMPrinterBatteryStatus* batteryStatus;
@property (nonatomic, readonly, nullable) BRLMMediaInfo* mediaInfo;

- (nonnull instancetype)init __unavailable;
- (nonnull instancetype)copy __unavailable;

@end
