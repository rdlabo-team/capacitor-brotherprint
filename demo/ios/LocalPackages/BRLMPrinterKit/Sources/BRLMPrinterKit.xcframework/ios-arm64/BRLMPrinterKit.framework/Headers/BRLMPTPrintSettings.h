// ------------------------------------------------------
//  BRLMPTPrintSettings.h
//  BRLMPrinterKit
//
//  Copyright © 2020 Brother Industries, Ltd. All rights reserved.
// ------------------------------------------------------

#import <BRLMPrinterKit/BRLMPrintSettingsProtocol.h>
#import <BRLMPrinterKit/BRLMPrintImageSettings.h>

    typedef NS_ENUM(NSInteger, BRLMPTPrintSettingsLabelSize) {
        BRLMPTPrintSettingsLabelSizeWidth3_5mm,
        BRLMPTPrintSettingsLabelSizeWidth6mm,
        BRLMPTPrintSettingsLabelSizeWidth9mm,
        BRLMPTPrintSettingsLabelSizeWidth12mm,
        BRLMPTPrintSettingsLabelSizeWidth18mm,
        BRLMPTPrintSettingsLabelSizeWidth24mm,
        BRLMPTPrintSettingsLabelSizeWidth36mm,
        BRLMPTPrintSettingsLabelSizeWidthHS_5_8mm,
        BRLMPTPrintSettingsLabelSizeWidthHS_8_8mm,
        BRLMPTPrintSettingsLabelSizeWidthHS_11_7mm,
        BRLMPTPrintSettingsLabelSizeWidthHS_17_7mm,
        BRLMPTPrintSettingsLabelSizeWidthHS_23_6mm,
        BRLMPTPrintSettingsLabelSizeWidthFL_21x45mm,
        BRLMPTPrintSettingsLabelSizeWidthHS_5_2mm,
        BRLMPTPrintSettingsLabelSizeWidthHS_9_0mm,
        BRLMPTPrintSettingsLabelSizeWidthHS_11_2mm,
        BRLMPTPrintSettingsLabelSizeWidthHS_21_0mm,
        BRLMPTPrintSettingsLabelSizeWidthHS_31_0mm,
    };
    
NS_ASSUME_NONNULL_BEGIN

@interface BRLMPTPrintSettings : NSObject <NSCoding, BRLMPrintSettingsProtocol, BRLMPrintImageSettings>

@property (nonatomic) BRLMPTPrintSettingsLabelSize labelSize;
@property (nonatomic) BOOL cutmarkPrint;
@property (nonatomic) BOOL cutPause;
@property (nonatomic) BOOL autoCut;
@property (nonatomic) BOOL halfCut;
@property (nonatomic) BOOL chainPrint;
@property (nonatomic) BOOL specialTapePrint;
@property (nonatomic) BRLMPrintSettingsResolution resolution;
/**
Description : Specify the page number in “cut each # labels”
Range(#) : 1 - 255
Default : 1
 */
@property (nonatomic) UInt8 autoCutForEachPageCount;
@property (nonatomic) BOOL forceVanishingMargin;


// override
- (nullable instancetype)initDefaultPrintSettingsWithPrinterModel:(BRLMPrinterModel)model;
- (nullable instancetype)copyWithPrinterModel:(BRLMPrinterModel)model;
@end


NS_ASSUME_NONNULL_END