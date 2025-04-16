//
//  BRPtouchLabelParam.h
//  BRPtouchPrinterKit
//
//  Copyright (c) 2016-2018 Brother Industries, Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface BRPtouchLabelParam : NSObject
@property (nonatomic) unsigned int  paperID;
@property (nonatomic, copy) NSString* paperName;
@property (nonatomic) unsigned int  paperSize;
@property (nonatomic) unsigned int  pinOffsetLeft;
@property (nonatomic) unsigned int  pinOffsetRight;
@property (nonatomic) unsigned int  physicalOffsetX;
@property (nonatomic) unsigned int  physicalOffsetY;
@property (nonatomic) unsigned int  paperWidth;         //dot
@property (nonatomic) float         paperWidth_mm;      //mm
@property (nonatomic) unsigned int  paperLength;        //dot
@property (nonatomic) float         paperLength_mm;     //mm
@property (nonatomic) unsigned int  imageAreaWidth;     //dot
@property (nonatomic) unsigned int  imageAreaWidth_mm;  //mm
@property (nonatomic) unsigned int  imageAreaLength;    //dot
@property (nonatomic) unsigned int  imageAreaLength_mm; //mm

@end
