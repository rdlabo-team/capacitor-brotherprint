//
//  BRPtouchPrintInfo.h
//  BRPtouchPrinterKit
//
//  Copyright (c) 2012-2018 Brother Industries, Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

#define OPTION_AUTOCUT      0x00000001
#define OPTION_CUTATEND     0x00000002

#define PRINT_ORIGINAL      0x00
#define PRINT_FIT           0x01 // Deprecated
#define PRINT_SCALE         0x02
#define PRINT_FIT_TO_PAGE   0x03
#define PRINT_FIT_TO_PAPER  0x04

#define ORI_LANDSCAPE       0x00
#define ORI_PORTRATE        0x01

//  Image processing Setting
#define HALFTONE_BINARY     0x00
#define HALFTONE_DITHER     0x01
#define HALFTONE_ERRDIF     0x02

#define ALIGN_LEFT          0x00
#define ALIGN_CENTER        0x01
#define ALIGN_RIGHT         0x02

#define ALIGN_TOP           0x00
#define ALIGN_MIDDLE        0x01
#define ALIGN_BOTTOM        0x02

#define PAPERALIGN_LEFT     0x00
#define PAPERALIGN_CENTER   0x01
#define PAPERALIGN_RIGHT    0x02

#define RET_ERR_PAPERNAME   -1
#define RET_ERR_OPTION      -2
#define RET_ERR_DENSITY     -3
#define RET_ERR_PRINTMODE   -4
#define RET_ERR_ORIENTATION -5
#define RET_ERR_HALFTONE    -6
#define RET_ERR_HORIZONTAL  -7
#define RET_ERR_VERTICAL    -8
#define RET_ERR_PAPERALIGN  -9
#define RET_ERR_EXTFLAG     -10


//  Cut Mode
#define FLAG_M_AUTOCUT      0x40
#define FLAG_M_MIRROR       0x80

//  拡張モード設定フラグ
#define FLAG_K_DRAFT        0x01
#define FLAG_K_HALFCUT      0x04
#define FLAG_K_NOCHAIN      0x08
#define FLAG_K_SPTAPE       0x10
#define FLAG_K_AFTERCUT     0x20
#define FLAG_K_HGPRINT      0x40
#define FLAG_K_COPY         0x80

//  拡張モード設定フラグ(PJ)
#define    EXT_PJ673_ENCRYPT    0x01
#define    EXT_PJ673_CARBON     0x02
#define    EXT_PJ673_DASHPRINT  0x04
#define    EXT_PJ673_NFD        0x08
#define    EXT_PJ673_EOP        0x10
#define    EXT_PJ673_EPR        0x20
#define EXT_PJ700_FP            0x40

//　圧縮モードフラグ
#define COMPRESS_ID         0x4D
#define COMPRESS_DISABLED   0x00
#define COMPRESS_PRESERVED  0x01
#define COMPRESS_ENABLED    0x02

#define PJROLLCASE_OFF           1  //Do not user printer case
#define PJROLLCASE_ON            2  //Use printer case with anti-curling mechanism
#define PJROLLCASE_WITH_ANTICURL 3   // Use printer case without anti-curling mechanism

//用紙種類
#define PJ_ROLL      0x01
#define PJ_CUT_PAPER 0x02

//Print Quality
#define PRINTQUALITY_LOW_RESOLUTION  1 // 高速
#define PRINTQUALITY_NORMAL          2 // ノーマル(高品質)
#define PRINTQUALITY_DOUBLE_SPEED    3 // ノーマル(高速)
#define PRINTQUALITY_HIGH_RESOLUTION 4 // 高品質
#define PRINTQUALITY_HIGH_RESOLUTION_USING_HR_IMAGE 5 // 高品質（高解像度用画像を使用時）

@interface BRPtouchPrintInfo : NSObject


@property    (copy,nonatomic)NSString*  strPaperName;
@property    (assign,nonatomic)int      nPrintMode;
@property    (assign,nonatomic)double   scaleValue;
@property    (assign,nonatomic)int      nDensity;
@property    (assign,nonatomic)int      nOrientation;
@property    (assign,nonatomic)int      nHalftone;
@property    (assign,nonatomic)int      nHalftoneBinaryThreshold;
@property    (assign,nonatomic)int      nHorizontalAlign;
@property    (assign,nonatomic)int      nVerticalAlign;
@property    (assign,nonatomic)int      nPaperAlign;
@property    (assign,nonatomic)int      nExtFlag;
@property    (assign,nonatomic)int      nAutoCutFlag;
@property   (assign,nonatomic)BOOL      bEndcut;
@property    (assign,nonatomic)int      nAutoCutCopies;
@property   (assign,nonatomic)BOOL      bSpecialTape;
@property   (assign,nonatomic)BOOL      bHalfCut;
//@property   (assign,nonatomic)int       nNumberofCopies;
@property   (assign,nonatomic)int       nTopMargin;
@property   (assign,nonatomic)int       nLeftMargin;
@property   (assign,nonatomic)int       nSpeed;
@property   (assign,nonatomic)BOOL      bBidirection;
@property   (assign,nonatomic)int       nCustomLength;
@property   (assign,nonatomic)int       nCustomWidth;
@property   (assign,nonatomic)int       nCustomFeed;
@property   (copy,nonatomic)NSString*   strSaveFilePath;
@property   (assign,nonatomic)BOOL      bOverWrite;
@property   (assign,nonatomic)int       nRollPrinterCase;
@property   (assign,nonatomic)BOOL      bRotate180;
@property   (assign,nonatomic)BOOL      bPeel;
@property   (assign,nonatomic)BOOL      bCutMark;
@property   (assign,nonatomic)int       nLabelMargine;
@property   (assign,nonatomic)int       nPJPaperKind;
@property   (assign,nonatomic)int       nPrintQuality;
@property   (assign,nonatomic)BOOL      bMode9;
@property   (assign,nonatomic)BOOL      bRawMode;
@property   (assign,nonatomic)BOOL      bBanishMargin;
@property   (assign,nonatomic)BOOL      bUseLegacyHalftoneEngine;
@property   (assign,nonatomic)BOOL      bUseCopyCommandInTemplatePrint;

@end
