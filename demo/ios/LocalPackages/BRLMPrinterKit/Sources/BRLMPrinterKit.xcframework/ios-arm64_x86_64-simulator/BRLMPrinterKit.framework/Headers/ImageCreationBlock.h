//
//  ImageCreationBlock.h
//  BRLMPrinterKit
//
//  Created by banro on 2021/11/15.
//  Copyright © 2021 Brother Industries. All rights reserved.
//

#ifndef ImageCreationBlock_h
#define ImageCreationBlock_h

#import <Foundation/Foundation.h>

// _Nonnullを一つでも使うと、そのヘッダーファイル内全ての関数に警告が表示されてしまうのでヘッダーを分離している
typedef _Nonnull CGImageRef (^ImageCreationBlock)(void);

#endif /* ImageCreationBlock_h */
