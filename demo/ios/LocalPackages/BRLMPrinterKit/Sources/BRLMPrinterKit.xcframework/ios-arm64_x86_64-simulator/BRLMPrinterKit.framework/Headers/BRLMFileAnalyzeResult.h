//
//  BRLMFileAnalyzeResult.h
//  BRLMPrinterKit
//
//  Created by shintaro on 2024/07/03.
//  Copyright © 2024 Brother Industries. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BRLMFileAnalyzeError.h"

@interface BRLMFileAnalyzeResult<ObjectType> : NSObject
- (instancetype _Nonnull)init __unavailable;
- (instancetype _Nonnull)copy __unavailable;
@property (nonatomic, readonly, nullable) ObjectType report;
@property (nonatomic, readonly, nonnull) BRLMFileAnalyzeError* error;

@end
