//
//  BRLMOpenChannelError.h
//  BRLMPrinterKit
//
//  Copyright © 2020 Brother Industries, Ltd. All rights reserved.
//

#import <BRLMPrinterKit/BRLMError.h>

typedef NS_ENUM(NSInteger, BRLMOpenChannelErrorCode) {
    BRLMOpenChannelErrorCodeNoError = 30000,
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
