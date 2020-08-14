//
//  BRLMQLPrintSettings.h
//  BRLMPrinterKit
//
//  Copyright © 2018 Brother Industries, Ltd. All rights reserved.
//

#import "BRLMPrintSettingsProtocol.h"
#import "BRLMPrintImageSettings.h"

    typedef NS_ENUM(NSInteger, BRLMQLPrintSettingsLabelSize) {
        BRLMQLPrintSettingsLabelSizeDieCutW17H54,
        BRLMQLPrintSettingsLabelSizeDieCutW17H87,
        BRLMQLPrintSettingsLabelSizeDieCutW23H23,
        BRLMQLPrintSettingsLabelSizeDieCutW29H42,
        BRLMQLPrintSettingsLabelSizeDieCutW29H90,
        BRLMQLPrintSettingsLabelSizeDieCutW38H90,
        BRLMQLPrintSettingsLabelSizeDieCutW39H48,
        BRLMQLPrintSettingsLabelSizeDieCutW52H29,
        BRLMQLPrintSettingsLabelSizeDieCutW62H29,
        BRLMQLPrintSettingsLabelSizeDieCutW62H100,
        BRLMQLPrintSettingsLabelSizeDieCutW60H86,
        BRLMQLPrintSettingsLabelSizeDieCutW54H29,
        BRLMQLPrintSettingsLabelSizeDieCutW102H51,
        BRLMQLPrintSettingsLabelSizeDieCutW102H152,
        BRLMQLPrintSettingsLabelSizeDieCutW103H164,
        BRLMQLPrintSettingsLabelSizeRollW12,
        BRLMQLPrintSettingsLabelSizeRollW29,
        BRLMQLPrintSettingsLabelSizeRollW38,
        BRLMQLPrintSettingsLabelSizeRollW50,
        BRLMQLPrintSettingsLabelSizeRollW54,
        BRLMQLPrintSettingsLabelSizeRollW62,
        BRLMQLPrintSettingsLabelSizeRollW62RB,
        BRLMQLPrintSettingsLabelSizeRollW102,
        BRLMQLPrintSettingsLabelSizeRollW103,
        BRLMQLPrintSettingsLabelSizeDTRollW90,
        BRLMQLPrintSettingsLabelSizeDTRollW102,
        BRLMQLPrintSettingsLabelSizeDTRollW102H51,
        BRLMQLPrintSettingsLabelSizeDTRollW102H152,
    };
    
NS_ASSUME_NONNULL_BEGIN

@interface BRLMQLPrintSettings : NSObject <NSCoding, BRLMPrintSettingsProtocol, BRLMPrintImageSettings>


@property (nonatomic) BRLMQLPrintSettingsLabelSize labelSize;
/**
Description : Specify the page number in “cut each # labels”
Range(#) : 1 - 255
Default : 1
 */
@property (nonatomic) UInt8 autoCutForEachPageCount;
@property (nonatomic) BOOL autoCut;
@property (nonatomic) BOOL cutAtEnd;
@property (nonatomic) BRLMPrintSettingsResolution resolution;

// override
- (nullable instancetype)initDefaultPrintSettingsWithPrinterModel:(BRLMPrinterModel)model;
- (nullable instancetype)copyWithPrinterModel:(BRLMPrinterModel)model;
@end


NS_ASSUME_NONNULL_END