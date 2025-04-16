//
//  BRLMCustomPaperSize.h
//  BRLMPrinterKit
//
//  Copyright © 2020 Brother Industries, Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <BRLMPrinterKit/BRLMPrintImageSettings.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, BRLMCustomPaperSizeLengthUnit) {
    BRLMCustomPaperSizeLengthUnitInch,
    BRLMCustomPaperSizeLengthUnitMm,
    // BRLMCustomPaperSizeLengthUnitDot,
};

typedef NS_ENUM(NSInteger, BRLMCustomPaperSizePaperKind) {
    BRLMCustomPaperSizePaperKindRoll,
    BRLMCustomPaperSizePaperKindDieCut,
    BRLMCustomPaperSizePaperKindMarkRoll,
    BRLMCustomPaperSizePaperKindByFile,
};

typedef struct
{
    CGFloat top;
    CGFloat left;
    CGFloat bottom;
    CGFloat right;
} BRLMCustomPaperSizeMargins;
BRLMCustomPaperSizeMargins BRLMCustomPaperSizeMarginsMake(CGFloat top, CGFloat left, CGFloat bottom, CGFloat right);

/// immutable
@interface BRLMCustomPaperSize : NSObject <NSCoding>

- (instancetype) initRollWithTapeWidth:(CGFloat) tapeWidth
                                     margins:(BRLMCustomPaperSizeMargins)margins // disable bottomMargin
                                   unitOfLength:(BRLMCustomPaperSizeLengthUnit) unit;

- (instancetype) initDieCutWithTapeWidth:(CGFloat) tapeWidth
                                       tapeLength:(CGFloat) tapeLength
                                       margins:(BRLMCustomPaperSizeMargins)margins
                                        gapLength:(CGFloat) gapLength
                                     unitOfLength:(BRLMCustomPaperSizeLengthUnit) unit;

- (instancetype) initMarkRollWithTapeWidth:(CGFloat) tapeWidth
                                         tapeLength:(CGFloat) tapeLength
                                         margins:(BRLMCustomPaperSizeMargins)margins
                                       markPosition:(CGFloat) markVerticalOffset
                                         markHeight:(CGFloat) markLength
                                       unitOfLength:(BRLMCustomPaperSizeLengthUnit) unit;

- (instancetype) initWithFile:(NSURL*)filePath;

- (instancetype) initRollWithTapeWidth:(CGFloat) tapeWidth
                               margins:(BRLMCustomPaperSizeMargins)margins // disable bottomMargin
                          unitOfLength:(BRLMCustomPaperSizeLengthUnit) unit
                            energyRank:(unsigned int)energyRank;

- (instancetype) initDieCutWithTapeWidth:(CGFloat) tapeWidth
                              tapeLength:(CGFloat) tapeLength
                                 margins:(BRLMCustomPaperSizeMargins)margins
                               gapLength:(CGFloat) gapLength
                            unitOfLength:(BRLMCustomPaperSizeLengthUnit) unit
                              energyRank:(unsigned int)energyRank;

- (instancetype) initMarkRollWithTapeWidth:(CGFloat) tapeWidth
                                tapeLength:(CGFloat) tapeLength
                                   margins:(BRLMCustomPaperSizeMargins)margins
                              markPosition:(CGFloat) markVerticalOffset
                                markHeight:(CGFloat) markLength
                              unitOfLength:(BRLMCustomPaperSizeLengthUnit) unit
                                energyRank:(unsigned int)energyRank;

@property (nonatomic, readonly) BRLMCustomPaperSizePaperKind paperKind;
@property (nonatomic, nullable, readonly) NSURL *paperBinFilePath;
@property (nonatomic, readonly) CGFloat tapeWidth;
@property (nonatomic, readonly) CGFloat tapeLength;
@property (nonatomic, readonly) BRLMCustomPaperSizeMargins margins;
@property (nonatomic, readonly) CGFloat gapLength;
@property (nonatomic, readonly) CGFloat markVerticalOffset;
@property (nonatomic, readonly) CGFloat markLength;
@property (nonatomic, readonly) BRLMCustomPaperSizeLengthUnit unit;
@property (nonatomic, nullable, readonly) NSNumber* energyRank;

@end

NS_ASSUME_NONNULL_END
