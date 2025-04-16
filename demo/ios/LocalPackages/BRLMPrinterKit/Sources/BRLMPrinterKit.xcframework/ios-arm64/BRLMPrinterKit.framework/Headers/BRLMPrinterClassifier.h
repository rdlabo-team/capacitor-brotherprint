//
//  BRLMPrinterClassifier.h
//  BRLMPrinterKit
//
//  Copyright © 2020 Brother Industries, Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <BRLMPrinterKit/BRLMPrinterDefine.h>

typedef NS_ENUM(NSInteger, BRLMPrinterClassifierSeries) {
    BRLMPrinterClassifierSeriesPJ,
    BRLMPrinterClassifierSeriesRJ,
    BRLMPrinterClassifierSeriesTD,
    BRLMPrinterClassifierSeriesPT,
    BRLMPrinterClassifierSeriesQL,
    BRLMPrinterClassifierSeriesMW,
    BRLMPrinterClassifierSeriesUnknown,
};

NS_ASSUME_NONNULL_BEGIN

@interface BRLMPrinterClassifier : NSObject

+ (enum BRLMPrinterClassifierSeries)classifyPrinterSerieseFromModel:(BRLMPrinterModel)model;
+ (enum BRLMPrinterModel)transferEnumFromString:(NSString*)modelName;

@end

NS_ASSUME_NONNULL_END
