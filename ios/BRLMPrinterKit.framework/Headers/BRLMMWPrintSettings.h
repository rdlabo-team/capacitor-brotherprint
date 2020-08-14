//
//  BRLMMWPrintSettings.h
//  BRLMPrinterKit
//
//  Copyright © 2020 Brother Industries, Ltd. All rights reserved.
//

#import "BRLMPrintSettingsProtocol.h"
#import "BRLMPrintImageSettings.h"

#import "BRLMCustomPaperSize.h"

typedef float BRLMMWPrintLengthScale;


typedef NS_ENUM(NSInteger, BRLMMWPrintSettingsPaperSize) {
    BRLMMWPrintSettingsPaperSizeA6,
    BRLMMWPrintSettingsPaperSizeA7,
};


NS_ASSUME_NONNULL_BEGIN

@interface BRLMMWPrintSettings : NSObject <NSCoding, BRLMPrintSettingsProtocol, BRLMPrintImageSettings>


@property (nonatomic, readonly) BRLMMWPrintSettingsPaperSize paperSize;

// override
- (nullable instancetype)initDefaultPrintSettingsWithPrinterModel:(BRLMPrinterModel)model;
- (nullable instancetype)copyWithPrinterModel:(BRLMPrinterModel)model;
@end


NS_ASSUME_NONNULL_END