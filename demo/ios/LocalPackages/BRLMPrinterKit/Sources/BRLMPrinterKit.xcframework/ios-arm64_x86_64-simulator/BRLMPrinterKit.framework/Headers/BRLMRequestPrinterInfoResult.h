//
//  BRLMRequestPrinterInfoResult.h
//  BRLMPrinterKit
//
//  Created by shintaro on 2023/05/30.
//  Copyright © 2023 Brother Industries. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BRLMRequestPrinterInfoError.h"

@interface BRLMRequestPrinterInfoResult<ObjectType> : NSObject
- (instancetype _Nonnull)init __unavailable;
- (instancetype _Nonnull)copy __unavailable;
@property (nonatomic, readonly, nullable) ObjectType printerInfo;
@property (nonatomic, readonly, nonnull) BRLMRequestPrinterInfoError* error;

@end


