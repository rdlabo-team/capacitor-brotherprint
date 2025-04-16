//
//  BRLMNetworkSearchOption.h
//  BRLMPrinterKit
//
//  Created by Shintaro on 2022/12/23.
//  Copyright © 2022 Brother Industries. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface BRLMNetworkSearchOption : NSObject

@property(nonatomic, nullable)NSArray<NSString *> *printerList;
@property(nonatomic)NSTimeInterval searchDuration;
- (instancetype)copy __unavailable;
@end

NS_ASSUME_NONNULL_END
