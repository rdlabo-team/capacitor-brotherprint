//
//  BRLMPrintImageSettings.h
//  BRLMPrinterKit
//
//  Copyright © 2020 Brother Industries, Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BRLMPrinterDefine.h"

NS_ASSUME_NONNULL_BEGIN


typedef NS_ENUM(NSInteger, BRLMPrintSettingsCompressMode) {
    BRLMPrintSettingsCompressModeNone,
    BRLMPrintSettingsCompressModeTiff,
    BRLMPrintSettingsCompressModeMode9,
};
typedef NS_ENUM(NSInteger, BRLMPrintSettingsHalftone) {
    BRLMPrintSettingsHalftoneThreshold,
    BRLMPrintSettingsHalftoneErrorDiffusion,
    BRLMPrintSettingsHalftonePatternDither,
};
typedef NS_ENUM(NSInteger, BRLMPrintSettingsHorizontalAlignment) {
    BRLMPrintSettingsHorizontalAlignmentLeft,
    BRLMPrintSettingsHorizontalAlignmentCenter,
    BRLMPrintSettingsHorizontalAlignmentRight,
};
typedef NS_ENUM(NSInteger, BRLMPrintSettingsOrientation) {
    BRLMPrintSettingsOrientationPortrait,
    BRLMPrintSettingsOrientationLandscape,
};
typedef NS_ENUM(NSInteger, BRLMPrintSettingsPrintQuality) {
    BRLMPrintSettingsPrintQualityBest,
    BRLMPrintSettingsPrintQualityFast,
};
typedef NS_ENUM(NSInteger, BRLMPrintSettingsResolution) {
    BRLMPrintSettingsResolutionLow,
    BRLMPrintSettingsResolutionNormal,
    BRLMPrintSettingsResolutionHigh,
};
typedef NS_ENUM(NSInteger, BRLMPrintSettingsScaleMode) {
    BRLMPrintSettingsScaleModeActualSize,
    BRLMPrintSettingsScaleModeFitPageAspect,
    BRLMPrintSettingsScaleModeFitPaperAspect,
    BRLMPrintSettingsScaleModeScaleValue,
};
typedef NS_ENUM(NSInteger, BRLMPrintSettingsVerticalAlignment) {
    BRLMPrintSettingsVerticalAlignmentTop,
    BRLMPrintSettingsVerticalAlignmentCenter,
    BRLMPrintSettingsVerticalAlignmentBottom,
};

@protocol BRLMPrintImageSettings <NSObject>

@property (nonatomic, readonly) BRLMPrinterModel printerModel;
@property (nonatomic) BRLMPrintSettingsScaleMode scaleMode;
@property (nonatomic) CGFloat scaleValue;
@property (nonatomic) BRLMPrintSettingsOrientation printOrientation;
@property (nonatomic) BRLMPrintSettingsHalftone halftone;
@property (nonatomic) BRLMPrintSettingsHorizontalAlignment hAlignment;
@property (nonatomic) BRLMPrintSettingsVerticalAlignment vAlignment;
@property (nonatomic) BRLMPrintSettingsCompressMode compress;
@property (nonatomic) UInt8 halftoneThreshold;
@property (nonatomic) NSUInteger numCopies;
@property (nonatomic) BOOL skipStatusCheck;
@property (nonatomic) BRLMPrintSettingsPrintQuality printQuality;


+ (instancetype)new  __unavailable;
- (instancetype)init __unavailable;
- (instancetype)copy __unavailable;


@end

NS_ASSUME_NONNULL_END