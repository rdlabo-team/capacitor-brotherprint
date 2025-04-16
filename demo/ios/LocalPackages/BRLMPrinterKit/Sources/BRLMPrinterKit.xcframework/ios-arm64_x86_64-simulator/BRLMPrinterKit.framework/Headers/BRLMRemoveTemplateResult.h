//
//  BRLMRemoveTemplateResult.h
//  BRLMPrinterKit
//
//  Copyright © 2024 Brother Industries. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BRLMLog.h"

typedef NS_ENUM(NSUInteger, BRLMRemoveTemplateSummaryErrorCode) {
    BRLMRemoveTemplateSummaryErrorCodeAllSuccess = 21000,
    BRLMRemoveTemplateSummaryErrorCodePartialSuccess,
    BRLMRemoveTemplateSummaryErrorCodeAllFailed,
    BRLMRemoveTemplateSummaryErrorCodeInitializationFailed,
    BRLMRemoveTemplateSummaryErrorCodeInvalidArgument,
    BRLMRemoveTemplateSummaryErrorCodeUnknownError = 21099,
};


typedef NS_ENUM(NSInteger, BRLMRemoveTemplateErrorCode) {
    BRLMRemoveTemplateErrorCodeNoError = 21100,
    BRLMRemoveTemplateErrorCodeUnsupported,
    BRLMRemoveTemplateErrorCodeKeyNotFound,
    BRLMRemoveTemplateErrorCodeConnectionFailed,
    BRLMRemoveTemplateErrorCodeUnresponsiveState,
    BRLMRemoveTemplateErrorCodeAborted,
    BRLMRemoveTemplateErrorCodeUnknownError = 21199,
};

@interface BRLMRemoveTemplateResult : NSObject
- (instancetype _Nonnull)init __unavailable;
- (instancetype _Nonnull)copy __unavailable;
@property (nonatomic, readonly) BRLMRemoveTemplateSummaryErrorCode errorCode;
@property (nonatomic, readonly, nonnull) NSDictionary<NSNumber*/*int*/,NSNumber*/*BRLMRemoveTemplateErrorCode*/>* errorCodeDetails;
@property (nonatomic, readonly, nonnull) NSArray<BRLMLog*>* allLogs;
@end


