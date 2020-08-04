//
//  BRLMPJPrintSettingsCustomPaperSize.h
//  BRLMPrinterKit
//
//  Copyright © 2020 Brother Industries, Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface BRLMPJPrintSettingsCustomPaperSize : NSObject <NSCoding>

@property (nonatomic, readonly) NSUInteger widthDots;
@property (nonatomic, readonly) NSUInteger lengthDots;
@property (nonatomic, readonly) NSUInteger feedDots;

- (instancetype)init __unavailable;
- (instancetype)copy __unavailable;

- (instancetype)initWithWidthDots:(NSUInteger)width lengthDots:(NSUInteger)length feedDots:(NSUInteger)feed;

@end

NS_ASSUME_NONNULL_END
