//
//  BRLMPrinterModelSpec.h
//  BRLMPrinterKit
//
//  Copyright © 2022 Brother Industries, Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BRLMPrinterDefine.h"

@interface BRLMPrinterModelSpec : NSObject

- (nonnull instancetype) initWithPrinterModel:(BRLMPrinterModel)printerModel;
- (nonnull instancetype) init __unavailable;
- (nonnull instancetype) copy __unavailable;

@property (nonatomic, readonly) BRLMPrinterModel printerModel;

@property (nonatomic, readonly) double Xdpi;
@property (nonatomic, readonly) double Ydpi;
@end
