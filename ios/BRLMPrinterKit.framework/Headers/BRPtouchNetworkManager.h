//
//  BRPtouchNetwork.h
//  BRPtouchPrinterKit
//
//  Copyright (c) 2012-2018 Brother Industries, Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

#include "BRPtouchDeviceInfo.h"

//  Return value
#define RET_FALSE       0
#define RET_TRUE        1

@protocol BRPtouchNetworkDelegate;

@interface BRPtouchNetworkManager : NSObject <NSNetServiceBrowserDelegate,NSNetServiceDelegate>


@property(retain, nonatomic) NSMutableArray* registeredPrinterNames;
@property(assign, nonatomic) BOOL isEnableIPv6Search;


- (int)startSearch: (int)searchTime;
- (NSArray*)getPrinterNetInfo;

- (BOOL)setPrinterNames:(NSArray*)strPrinterNames;
- (BOOL)setPrinterName:(NSString*)strPrinterName;

- (id)initWithPrinterNames:(NSArray*)strPrinterNames;
- (id)initWithPrinterName:(NSString*)strPrinterName;

@property (nonatomic, assign) id <BRPtouchNetworkDelegate> delegate;

@end

@protocol BRPtouchNetworkDelegate <NSObject>

- (void)didFinishSearch:(id)sender;
@optional
- (void)didFindDevice:(BRPtouchDeviceInfo *)deviceInfo;

@end
