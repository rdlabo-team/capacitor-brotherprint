//
//  BRPtouchPrinterData.h
//  BRPtouchPrinterKit
//
//  Copyright (c) 2012-2018 Brother Industries, Ltd. All rights reserved.
//
#import <Foundation/Foundation.h>

@interface BRPtouchPrinterData : NSObject
{
	NSArray*	aryPrinterList;
}

- (NSArray*)getPrinterList;

@end
