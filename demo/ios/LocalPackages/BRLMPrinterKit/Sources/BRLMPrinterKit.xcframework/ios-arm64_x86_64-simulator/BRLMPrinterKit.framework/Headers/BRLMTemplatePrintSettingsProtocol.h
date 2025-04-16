//
//  BRLMTemplatePrintSettingsProtocol.h
//  BRLMPrinterKit
//
//  Created by shintaro on 2023/05/15.
//  Copyright © 2023 Brother Industries. All rights reserved.
//
#import <Foundation/Foundation.h>

#import <BRLMPrinterKit/BRLMPrinterDefine.h>

NS_ASSUME_NONNULL_BEGIN

@protocol BRLMTemplatePrintSettingsProtocol <NSObject>

- (nonnull instancetype)init __unavailable;
- (nonnull instancetype)copy __unavailable;

@required
@property (nonatomic, readonly) BRLMPrinterModel printerModel;
@property (nonatomic) NSUInteger numCopies;
- (nullable instancetype)initDefaultPrintSettingsWithPrinterModel:(BRLMPrinterModel)model;
- (nullable instancetype)copyWithPrinterModel:(BRLMPrinterModel)model;
@end

NS_ASSUME_NONNULL_END
