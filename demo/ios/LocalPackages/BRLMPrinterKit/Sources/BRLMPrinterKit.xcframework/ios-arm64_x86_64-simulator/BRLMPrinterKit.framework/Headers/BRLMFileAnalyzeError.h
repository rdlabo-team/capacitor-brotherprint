//
//  BRLMFileAnalyzeError.h
//  BRLMPrinterKit
//
//  Created by shintaro on 2024/07/03.
//  Copyright © 2024 Brother Industries. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BRLMLog.h"

typedef NS_ENUM(NSUInteger, BRLMFileAnalyzeErrorCode) {
    BRLMFileAnalyzeErrorNoError = 40000,
    BRLMFileAnalyzeErrorFileNotFound,
    BRLMFileAnalyzeErrorFileFormatIncorrectly,
    BRLMFileAnalyzeErrorUnknownError = 40099,
};


@interface BRLMFileAnalyzeError : NSObject
- (instancetype)init __unavailable;
- (instancetype)copy __unavailable;
@property(nonatomic, readonly) BRLMFileAnalyzeErrorCode code;
@property(nonatomic, readonly) NSArray<BRLMLog*>* allLogs;
@property(nonatomic, readonly) NSString* errorDescription;

@end
