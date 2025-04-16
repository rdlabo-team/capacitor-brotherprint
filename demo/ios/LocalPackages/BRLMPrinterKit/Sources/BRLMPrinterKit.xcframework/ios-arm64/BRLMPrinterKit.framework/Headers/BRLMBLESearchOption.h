//
//  BRLMBLESearchOption.h
//  BRLMPrinterKit
//
//  Created by shintaro on 2023/01/13.
//  Copyright © 2023 Brother Industries. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface BRLMBLESearchOption : NSObject
@property(nonatomic)NSTimeInterval searchDuration;
- (instancetype)copy __unavailable;
@end

NS_ASSUME_NONNULL_END
