//
//  BRCustomPaperInfoCommand.h
//  BRPtouchPrinterKit
//
//  Copyright (c) 2018 Brother Industries, Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface BRCustomPaperInfoCommand : NSObject

typedef enum {
    Inch = 1,
    Mm = 2
}UnitOfLengthParameter;

typedef enum  {
    Roll = 1,
    DieCut = 2,
    MarkRoll = 3
}PaperKindParameter;

typedef enum  {
    InternalError = -1,
    NoErrorParameters = 0,
    // Basic
    TapeWidth = 1,
    TapeLength = 2,
    RightMargin = 3,
    LeftMargin = 4,
    TopMargin = 5,
    BottomMargin = 6,
    LabelPitch = 7,
    MarkPosition = 8,
    MarkHeight = 9,
    DisplayName = 10,
}ErrorParameterName;

typedef enum  {
    NoError = 0,
    ExceptionError = 1,
    OverTheUpperLimit = 2,
    BelowTheLowerLimit = 3,
    FailToCreateCommand = 4,
    FailToReadPTDFile = 5,
}ErrorParameterDetail;


// Basic Settings (readonly)
@property(nonatomic, strong, readonly) NSString *printerName_;
@property(readonly) float tapeWidth_;
@property(readonly) float tapeLength_;
@property(readonly) float rightMargin_;
@property(readonly) float leftMagin_;
@property(readonly) float topMargin_;
@property(readonly) float bottomMargin_;
@property(readonly) float labelPitch_;
@property(readonly) float markPosition_;
@property(readonly) float markHeight_;
@property(readonly) UnitOfLengthParameter unitOfLengthParameter_;
@property(readonly) PaperKindParameter paperKindParameter_;

// Advance Settings
@property(nonatomic, strong) NSString *displayName;
@property(readwrite) int energyRank;
@property(readwrite) float dieStartRevPlus;
@property(readwrite) float dieStartFwdPlus;
@property(readwrite) float afterFeedPlus;
@property(readwrite) float mediaSensorSensitivity;
@property(readwrite) float mediaSensorIntensity;

- (id) initWithPrinterNameForRoll: (NSString *) printerName
                        tapeWidth: (float) tapeWidth
                      rightMargin: (float) rightMargin
                        leftMagin: (float) leftMagin
                        topMargin: (float) topMargin
                     unitOfLength: (UnitOfLengthParameter) unitOfLengthParameter;

- (id) initWithPrinterNameForDieCut: (NSString *) printerName
                          tapeWidth: (float) tapeWidth
                         tapeLength: (float) tapeLength
                        rightMargin: (float) rightMargin
                          leftMagin: (float) leftMagin
                          topMargin: (float) topMargin
                       bottomMargin: (float) bottomMargin
                         labelPitch: (float) labelPitch
                       unitOfLength: (UnitOfLengthParameter) unitOfLengthParameter;

- (id) initWithPrinterNameForMarkRoll: (NSString *) printerName
                            tapeWidth: (float) tapeWidth
                           tapeLength: (float) tapeLength
                          rightMargin: (float) rightMargin
                            leftMagin: (float) leftMagin
                            topMargin: (float) topMargin
                         bottomMargin: (float) bottomMargin
                         markPosition: (float) markPosition
                           markHeight: (float) markHeight
                         unitOfLength: (UnitOfLengthParameter) unitOfLengthParameter;

- (NSData *) getCustomPaperInfoCommandData: (NSMutableArray **)errorArr;

- (NSString *) valueOfErrorParameter:(ErrorParameterName)errorParameterName;
- (NSString *) valueOfErrorParameterDetail:(ErrorParameterDetail)errorParameterDetail;

@end


