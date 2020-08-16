//
//  BRLMRJPrintSettings.h
//  BRLMPrinterKit
//
//  Copyright © 2018 Brother Industries, Ltd. All rights reserved.
//

#import "BRLMPrintSettingsProtocol.h"
#import "BRLMPrintImageSettings.h"

#import "BRLMCustomPaperSize.h"


typedef float BRLMRJPrintLengthScale;

typedef NS_ENUM(NSInteger, BRLMRJPrintSettingsDensity) {
    BRLMRJPrintSettingsDensityWeakLevel5,
    BRLMRJPrintSettingsDensityWeakLevel4,
    BRLMRJPrintSettingsDensityWeakLevel3,
    BRLMRJPrintSettingsDensityWeakLevel2,
    BRLMRJPrintSettingsDensityWeakLevel1,
    BRLMRJPrintSettingsDensityNeutral,
    BRLMRJPrintSettingsDensityStrongLevel1,
    BRLMRJPrintSettingsDensityStrongLevel2,
    BRLMRJPrintSettingsDensityStrongLevel3,
    BRLMRJPrintSettingsDensityStrongLevel4,
    BRLMRJPrintSettingsDensityStrongLevel5,
    BRLMRJPrintSettingsDensityUsePrinterSetting,
};


NS_ASSUME_NONNULL_BEGIN

@interface BRLMRJPrintSettings : NSObject <NSCoding, BRLMPrintSettingsProtocol, BRLMPrintImageSettings>


@property (nonatomic) BRLMCustomPaperSize* customPaperSize;
@property (nonatomic) BRLMRJPrintSettingsDensity density;
@property (nonatomic) BOOL rotate180degrees;
@property (nonatomic) BOOL peelLabel;

// override
- (nullable instancetype)initDefaultPrintSettingsWithPrinterModel:(BRLMPrinterModel)model;
- (nullable instancetype)copyWithPrinterModel:(BRLMPrinterModel)model;
@end


NS_ASSUME_NONNULL_END