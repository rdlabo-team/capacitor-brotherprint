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

- (instancetype)init __unavailable;
- (instancetype)copy __unavailable;

- (instancetype)initWithWidthDots:(NSUInteger)width lengthDots:(NSUInteger)length;

@end

NS_ASSUME_NONNULL_END
