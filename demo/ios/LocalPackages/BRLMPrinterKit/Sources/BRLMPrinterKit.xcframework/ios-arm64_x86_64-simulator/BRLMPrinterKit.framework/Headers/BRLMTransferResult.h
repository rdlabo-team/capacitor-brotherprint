//
//  BRLMTransferError.h
//  BRLMPrinterKit
//
//  Created by shintaro on 2024/07/11.
//  Copyright © 2024 Brother Industries. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BRLMLog.h"

typedef NS_ENUM(NSUInteger, BRLMTransferSummaryErrorCode) {
    BRLMTransferSummaryErrorAllSuccess = 21100,
    BRLMTransferSummaryErrorPartialSuccess,
    BRLMTransferSummaryErrorAllFailed,
    BRLMTransferSummaryErrorInitializationFailed,
    BRLMTransferSummaryErrorInvalidArgument,
    BRLMTransferSummaryErrorUnknownError = 21199,
};



typedef NS_ENUM(NSUInteger, BRLMTransferErrorCode) {
    BRLMTransferErrorCodeNoError = 10100,
    BRLMTransferErrorCodeUnsupportedFunction,
    BRLMTransferErrorCodeUnsupportedFunctionByCurrentConnectionInterface,
    BRLMTransferErrorCodeFileNotFound,
    BRLMTransferErrorCodeUnsupportedFile,
    BRLMTransferErrorCodeFileNotForSelectedPrinter,
    BRLMTransferErrorCodePrinterStatusError,
    BRLMTransferErrorCodeConnectionFailed,
    BRLMTransferErrorCodeAborted,
    BRLMTransferErrorCodeUnknownError = 10199,
};


@interface BRLMTransferResult<ObjectType> : NSObject
- (instancetype _Nonnull )init __unavailable;
- (instancetype _Nonnull )copy __unavailable;
@property(nonatomic, readonly) BRLMTransferSummaryErrorCode code;
@property (nonatomic, readonly, nullable) NSDictionary< ObjectType, NSNumber*/*BRLMTransferErrorCode*/>* errorDetails;
@property(nonatomic, readonly, nonnull) NSArray<BRLMLog*>* allLogs;
-(NSString * _Nonnull)description;
@end
