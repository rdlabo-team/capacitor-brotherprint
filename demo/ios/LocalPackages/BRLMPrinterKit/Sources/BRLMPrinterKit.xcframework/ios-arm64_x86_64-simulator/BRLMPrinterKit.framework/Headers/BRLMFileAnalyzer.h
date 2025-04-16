//
//  BRLMFileAnalyzer.h
//  BRLMPrinterKit
//
//  Created by shintaro on 2024/07/03.
//  Copyright © 2024 Brother Industries. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BRLMPtouchDeviceDependedDataHeader.h"
#import "BRLMFileAnalyzeResult.h"
@interface BRLMFileAnalyzer : NSObject
+ (BRLMFileAnalyzeResult<BRLMPtouchDeviceDependedDataHeader *> * _Nullable)analyzePD3:(NSURL *_Nonnull)url;

@end

