//
//  BRLMPrinterModelSpec.h
//  BRLMPrinterKit
//
//  Copyright © 2022 Brother Industries, Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BRLMPrinterDefine.h"


typedef NSNumber BRLMPTPrintSettingsLabelSizeNumber;
typedef NSNumber BRLMQLPrintSettingsLabelSizeNumber;

@interface BRLMPrinterModelSpec : NSObject

- (nonnull instancetype) initWithPrinterModel:(BRLMPrinterModel)printerModel;
- (nonnull instancetype) init __unavailable;
- (nonnull instancetype) copy __unavailable;

@property (nonatomic, readonly) BRLMPrinterModel printerModel;

@property (nonatomic, readonly) double Xdpi;
@property (nonatomic, readonly) double Ydpi;
@property (nonatomic, readonly) NSDictionary* _Nonnull rawData;

@property (nonatomic, readonly) int modelCode;
@property (nonatomic, readonly) int seriesCode;
@property (nonatomic, readonly) NSArray<BRLMPTPrintSettingsLabelSizeNumber*>* _Nonnull supportedPTLabels;
@property (nonatomic, readonly) NSArray<BRLMQLPrintSettingsLabelSizeNumber*>* _Nonnull supportedQLLabels;

@end
