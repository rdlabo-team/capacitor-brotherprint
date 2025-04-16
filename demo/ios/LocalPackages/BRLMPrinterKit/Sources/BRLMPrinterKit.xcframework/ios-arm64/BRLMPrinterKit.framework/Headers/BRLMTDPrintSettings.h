// ------------------------------------------------------
//  BRLMTDPrintSettings.h
//  BRLMPrinterKit
//
//  Copyright © 2018 Brother Industries, Ltd. All rights reserved.
// ------------------------------------------------------

#import <BRLMPrinterKit/BRLMPrintSettingsProtocol.h>
#import <BRLMPrinterKit/BRLMPrintImageSettings.h>

#import <BRLMPrinterKit/BRLMCustomPaperSize.h>

typedef NS_ENUM(NSInteger, BRLMTDPrintSettingsDensity) {
    BRLMTDPrintSettingsDensityWeakLevel5,
    BRLMTDPrintSettingsDensityWeakLevel4,
    BRLMTDPrintSettingsDensityWeakLevel3,
    BRLMTDPrintSettingsDensityWeakLevel2,
    BRLMTDPrintSettingsDensityWeakLevel1,
    BRLMTDPrintSettingsDensityNeutral,
    BRLMTDPrintSettingsDensityStrongLevel1,
    BRLMTDPrintSettingsDensityStrongLevel2,
    BRLMTDPrintSettingsDensityStrongLevel3,
    BRLMTDPrintSettingsDensityStrongLevel4,
    BRLMTDPrintSettingsDensityStrongLevel5,
    BRLMTDPrintSettingsDensityUsePrinterSetting,
};


NS_ASSUME_NONNULL_BEGIN

@interface BRLMTDPrintSettings : NSObject <NSCoding, BRLMPrintSettingsProtocol, BRLMPrintImageSettings>


@property (nonatomic) BRLMCustomPaperSize* customPaperSize;
@property (nonatomic) BRLMTDPrintSettingsDensity density;
@property (nonatomic) BOOL peelLabel;
@property (nonatomic) BOOL autoCut;
@property (nonatomic) BOOL cutAtEnd;
@property (nonatomic) UInt8 autoCutForEachPageCount;
@property (nonatomic) NSString* customRecord;

// override
- (nullable instancetype)initDefaultPrintSettingsWithPrinterModel:(BRLMPrinterModel)model;
- (nullable instancetype)copyWithPrinterModel:(BRLMPrinterModel)model;
@end


NS_ASSUME_NONNULL_END