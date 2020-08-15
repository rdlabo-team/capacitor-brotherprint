//
//  BRLMOpenChannelError.h
//  BRLMPrinterKit
//
//  Copyright © 2020 Brother Industries, Ltd. All rights reserved.
//

#import "BRLMError.h"

typedef NS_ENUM(NSInteger, BRLMOpenChannelErrorCode) {
    BRLMOpenChannelErrorCodeNoError,
    BRLMOpenChannelErrorCodeOpenStreamFailure,
    BRLMOpenChannelErrorCodeTimeout,
};

NS_ASSUME_NONNULL_BEGIN

@interface BRLMOpenChannelError : BRLMError

@property(nonatomic, readonly) BRLMOpenChannelErrorCode code;

- (instancetype)init __unavailable;
- (instancetype)copy __unavailable;

@end

NS_ASSUME_NONNULL_END
