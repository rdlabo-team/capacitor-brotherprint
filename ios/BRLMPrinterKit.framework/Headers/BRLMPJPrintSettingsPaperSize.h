//
//  BRLMPJPrintSettingsPaperSize.h
//  BRLMPrinterKit
//
//  Copyright © 2020 Brother Industries, Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "BRLMPJPrintSettingsCustomPaperSize.h"

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, BRLMPJPrintSettingsPaperSizeStandard) {
    BRLMPJPrintSettingsPaperSizeStandardA4,
    BRLMPJPrintSettingsPaperSizeStandardLegal,
    BRLMPJPrintSettingsPaperSizeStandardLetter,
    BRLMPJPrintSettingsPaperSizeStandardA5,
    BRLMPJPrintSettingsPaperSizeStandardCustom, // read only
};

@interface BRLMPJPrintSettingsPaperSize : NSObject <NSCoding>

@property (nonatomic, readonly) BRLMPJPrintSettingsPaperSizeStandard paperSizeStandard;
@property (nonatomic, readonly, nullable) BRLMPJPrintSettingsCustomPaperSize *customPaper;

- (instancetype)init __unavailable;
- (instancetype)copy __unavailable;

- (instancetype)initWithPaperSizeStandard:(BRLMPJPrintSettingsPaperSizeStandard) paperSizeStandard;
- (instancetype)initWithCustomPaper:(BRLMPJPrintSettingsCustomPaperSize *)customPaper;

@end

NS_ASSUME_NONNULL_END
