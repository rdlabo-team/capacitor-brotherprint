//
//  BRLMTDPrintSettings.h
//  BRLMPrinterKit
//
//  Copyright © 2018 Brother Industries, Ltd. All rights reserved.
//

#import "BRLMPrintSettingsProtocol.h"
#import "BRLMPrintImageSettings.h"

#import "BRLMCustomPaperSize.h"


typedef float BRLMTDPrintLengthScale;

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

// override
- (nullable instancetype)initDefaultPrintSettingsWithPrinterModel:(BRLMPrinterModel)model;
- (nullable instancetype)copyWithPrinterModel:(BRLMPrinterModel)model;
@end


NS_ASSUME_NONNULL_END