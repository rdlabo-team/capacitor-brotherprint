//
//  BRLMPTTemplatePrintSettings.h
//  BRLMPrinterKit
//
//  Created by shintaro on 2023/05/11.
//  Copyright © 2023 Brother Industries. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <BRLMPrinterKit/BRLMTemplatePrintSettingsProtocol.h>


@interface BRLMPTTemplatePrintSettings : NSObject <BRLMTemplatePrintSettingsProtocol>
- (nonnull instancetype)init __unavailable;
- (nonnull instancetype)copy __unavailable;
@property (nonatomic, readonly) BRLMPrinterModel printerModel;
@property (nonatomic) NSUInteger numCopies;
- (nullable instancetype)initDefaultPrintSettingsWithPrinterModel:(BRLMPrinterModel)model;
- (nullable instancetype)copyWithPrinterModel:(BRLMPrinterModel)model;
@end

