//
//  BRPtouchLabelInfo.h
//  BRPtouchPrinterKit
//
//  Copyright (c) 2016-2018 Brother Industries, Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSUInteger, LabelIdType) {
    
    //QL
    LabelIdTypeW17H54 = 0x01,
    LabelIdTypeW17H87 = 0x02,
    LabelIdTypeW23H23 = 0x03,
    LabelIdTypeW29H42 = 0x04,
    LabelIdTypeW29H90 = 0x05,
    LabelIdTypeW38H90 = 0x06,
    LabelIdTypeW39H48 = 0x07,
    LabelIdTypeW52H29 = 0x08,
    LabelIdTypeW62H29 = 0x09,
    LabelIdTypeW62H100 = 0x0a,
    LabelIdTypeW12 = 0x0b,  //also used in PT
    LabelIdTypeW29 = 0xc,
    LabelIdTypeW38 = 0xd,
    LabelIdTypeW50 = 0xe,
    LabelIdTypeW54 = 0xf,
    LabelIdTypeW62 = 0x10,
    LabelIdTypeW60H86 = 0x11,
    
    //PT
    LabelIdTypeW3_5 = 0x12,
    LabelIdTypeW6 = 0x13,
    LabelIdTypeW9 = 0x14,
    LabelIdTypeW18 = 0x15,
    LabelIdTypeW24 = 0x16,
    LabelIdTypeHS_W6 = 0x17,
    LabelIdTypeHS_W9 = 0x18,
    LabelIdTypeHS_W12 = 0x19,
    LabelIdTypeHS_W18 = 0x1a,
    LabelIdTypeHS_W24 = 0x1b,
    LabelIdTypeW36 = 0x1c,
    
    LabelIdTypeR6_5 = 0x1d,
    LabelIdTypeR6_0 = 0x1f,
    LabelIdTypeR5_0 = 0x20,
    LabelIdTypeR4_0 = 0x21,
    LabelIdTypeR3_5 = 0x22,
    LabelIdTypeR3_0 = 0x23,
    LabelIdTypeR2_5 = 0x24,
    
    LabelIdTypeFLE_W21H45 = 0x25,
    
    //QL-8対応
    LabelIdTypeW62RB = 0x26,
    LabelIdTypeW54H29 = 0x27,

    //QL_1100
    LabelIdTypeW102 = 0x28,
    LabelIdTypeW102H51 = 0x29,
    LabelIdTypeW102H152 = 0x30,
    LabelIdTypeW103 = 0x31,
    LabelIdTypeW103H164 = 0x32,
    LabelIdTypeDT_W90 = 0x33,
    LabelIdTypeDT_W102 = 0x34,
    LabelIdTypeDT_W102H51 = 0x35,
    LabelIdTypeDT_W102H152 = 0x36,

    LabelIdTypeLABEL_MAX
    
};

typedef NS_ENUM(NSUInteger, ColorType) {
    WHITE = 0x01,
    OTHERS,
    CLEAR,
    RED,
    BLUE,
    YELLOW,
    GREEN,
    BLACK,
    CLEAR_WHITE,
    GOLD = 0x0A ,
    GOLD_PREMIUM = 0x0B,
    SILVER_PREMIUM = 0x0C,
    OTHERS_PREMIUM = 0x0D,
    OTHERS_MASKING = 0x0E,
    SATIN_LIGHTBLUE = 0x0F,
    SATIN_MINT = 0x10,
    MATTE_WHITE = 0x20,
    MATTE_CLEAR,
    MATTE_SILVER,
    SATIN_GOLD,
    SATIN_SILVER = 0x24,
    BLUE_WHITE = 0x30,
    RED_WHITE = 0x31,
    FLOURESCENT_ORANGE = 0x40,
    FLOURESCENT_YELLOW = 0x41,
    BERRY_PINK = 0x50,
    LIGHT_GLAY,
    LIME_GREEN = 0x52,
    FABRIC_YELLOW = 0x60,
    FABRIC_PINK = 0x61,
    FABRIC_BLUE = 0x62,
    TUBE_WHITE = 0x70,
    SELF_WHITE = 0x80,
    FLEXIBLE_WHITE = 0x90,
    FLEXIBLE_YELLOW = 0x91,
    CLEANING = 0xF0,
    STENCIL = 0xF1,
    UNSUPPORT = 0xFF,
};

@interface BRPtouchLabelInfoStatus : NSObject
@property (nonatomic) LabelIdType labelID;
@property (nonatomic) ColorType labelColor;
@property (nonatomic) ColorType fontColor;

@end
