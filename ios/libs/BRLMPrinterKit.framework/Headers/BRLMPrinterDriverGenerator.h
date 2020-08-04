//
//  BRLMPrinterDriverGenerator.h
//  BRLMPrinterKit
//
//  Copyright © 2020 Brother Industries, Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "BRLMPrinterDriver.h"
#import "BRLMChannel.h"
#import "BRLMOpenChannelError.h"

NS_ASSUME_NONNULL_BEGIN

@interface BRLMPrinterDriverGenerateResult : NSObject
@property (nonatomic, readonly) BRLMOpenChannelError *error;
@property (nonatomic, readonly, nullable) BRLMPrinterDriver *driver;
- (instancetype)init __unavailable;
- (instancetype)copy __unavailable;
@end

@interface BRLMPrinterDriverGenerator : NSObject

@property(nonatomic, readonly, nullable) BRLMOpenChannelError *openChannelError;

- (instancetype)init __unavailable;
- (instancetype)copy __unavailable;

+ (BRLMPrinterDriverGenerateResult *)openChannel:(BRLMChannel *)channel;

@end

NS_ASSUME_NONNULL_END
