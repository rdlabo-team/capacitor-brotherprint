//
//  GenericPocketJetPrintSettings.h
//  BMSPrinterKit
//
//  Created by BMS on 9/23/15.
//  Copyright (c) 2012-15 Brother Mobile Solutions. All rights reserved.
//

#import "PrintSettings.h"

typedef enum
{
    kCustomPaperAlignLeft = 0,
    kCustomPaperAlignCenter,
} CUSTOMPAPERALIGN;

@interface GenericPocketJetPrintSettings : PrintSettings <PrintSettingsDelegate>
{
    // provide subclasses with access to readonly properties via ivars.
    // Because they are readonly, there is no "Setter" method available in the superclass.
    // The subclasses should be the only place where these properties are set.
    // But, they will be used in this superclass.
    // Users of this superclass can read the values if desired, but in most cases this
    // will not be necessary.
    int _resolutionHorz;
    int _resolutionVert;
    int _customPaperWidthDotsMin;
    int _customPaperWidthDotsMax;
    int _customPaperLengthDotsMin;
    int _customPaperLengthDotsMax;
    int _extraFeedMax;
}


//*** IMPORTANT (for SDK Users):
// *** DO NOT INSTANTIATE AN OBJECT OF THIS CLASS!!!!! ***
// The GenericPocketJetPrintSettings class is essentially an ABSTRACT class.
// Instead, you should instantiate the model-specific class, e.g. PJ673PrintSettings.

//*** The following properties should be set by your app code, either PROGRAMMATICALLY
// or by using one of the PrintSettingsViewControllers (which may not set all properties).

// The properties and methods in this super-class will be COMMON for all
// of the PocketJet printer models.

// SDK Users: All of the properties below (and those in the PrintSettings superclass)
// are settable by you to change the printer behavior.
// If you are not using the PrintSettingsViewController (either the SDK's or your own custom one),
// then you should set each of these properties programmatically to your desired setting.
@property (nonatomic, assign) PAPERSIZE paperSize;
@property (nonatomic, assign) PAPERTYPE paperType;
@property (nonatomic, assign) FORMFEEDMODE formFeedMode;
@property (nonatomic,assign) int extraFeed; // used with kFormFeedModeNoFeed only
@property (nonatomic, assign) DENSITY density;
// These 3 properties are used ONLY when paperSize is set to kPaperSizeCustom!
@property (nonatomic, assign) int customPaperWidthDots;
@property (nonatomic, assign) int customPaperLengthDots;
@property (nonatomic, assign) CUSTOMPAPERALIGN customPaperAlign;


//*** The properties below are READ ONLY. SDK Users can NOT set them!!
// Each subclass MUST set them to their model-specific values in their init method.
// The purpose of this GenericPocketJetPrintSettings class is to implement COMMON
// code so it does not have to be duplicated for each similar printer model. However, it
// requires these model-dependent properties to be set by the SUBCLASS.
@property (readonly, nonatomic, assign) int resolutionHorz;
@property (readonly, nonatomic, assign) int resolutionVert;
@property (readonly, nonatomic, assign) int customPaperWidthDotsMin;
@property (readonly, nonatomic, assign) int customPaperWidthDotsMax;
@property (readonly, nonatomic, assign) int customPaperLengthDotsMin;
@property (readonly, nonatomic, assign) int customPaperLengthDotsMax;
@property (readonly, nonatomic, assign) int extraFeedMax;


// *** PrintSettingsDelegate protocol
// All subclasses of PrintSettings are required to implement these methods.
// validatePrintSettings - This function assures that all properties are in range
// and that there are no conflicting settings. 
- (BOOL)validatePrintSettings;
- (void) loadPreferences;
- (void) savePreferences;
//generate strings from CURRENT settings, for convenience.
// This is part of the PrintSettingsDelegateProtocol too.
- (NSString *)stringFromCurrentPaperType;
- (NSString *)stringFromCurrentPaperSize;
- (NSString *)stringFromCurrentFormFeedMode;
- (NSString *)stringFromCurrentExtraFeed;
- (NSString *)stringFromCurrentDensity;


//*** generate strings from constants, for convenience
- (NSString *)stringFromPaperType: (PAPERTYPE)paperType;
- (NSString *)stringFromPaperSize: (PAPERSIZE)paperSize;
- (NSString *)stringFromFormFeedMode: (FORMFEEDMODE)ffmode;
- (NSString *)stringFromExtraFeed:(int)extraFeed;
- (NSString *)stringFromDensity: (DENSITY)density;


@end
