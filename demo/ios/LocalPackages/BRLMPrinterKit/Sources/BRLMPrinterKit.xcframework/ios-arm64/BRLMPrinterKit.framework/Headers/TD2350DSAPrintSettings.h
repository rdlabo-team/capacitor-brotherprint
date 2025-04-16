//
//  TD2350DSAPrintSettings.h
//  BMSPrinterKit
//
//  Created by BMS on 2/16/24.
//  Copyright (c) 2024 Brother Mobile Solutions. All rights reserved.
//


#import <BRLMPrinterKit/GenericMobileLabelModelPrintSettings.h>


@interface TD2350DSAPrintSettings : GenericMobileLabelModelPrintSettings <PrintSettingsDelegate>


//*** IMPORTANT ***
// SDK Users:
// You should NEVER instantiate an object of the GenericMobileLabelModelPrintSettings superclass.
// Instead, you should instantiate one of the model-specific subclasses (i.e. TD2350DSAPrintSettings).
//
// For a list of important properties in the settings that your application needs to set,
// refer to the GenericMobileLabelModelPrintSettings superclass.

// SDK Designers:
// The GenericMobileLabelModelPrintSettings superclass handles all the important work.
// This subclass only needs to provide the model-specific data to the generic class(es).

//*** ADDITIONAL PROPERTIES not handled by GenericMobileLabelModelPrintSettings
// NOTE: TD23xx models can be either 203 or 300 dpi depending on print head installed.
@property (nonatomic, assign) RESOLUTION printerResolution;

@property (nonatomic, assign) BOOL peelerEnabled;
@property (nonatomic, assign) BOOL invert180degrees;

// The following are used with the cutter
@property (nonatomic, assign) BOOL cutAtEndOfJobEnabled; // true: cuts at end of last page of job
@property (nonatomic, assign) BOOL autoCutEnabled; // true: automatically cuts every N labels, as specified by autoCutEveryNLabels
@property (nonatomic, assign) unsigned int autoCutEveryNLabels; // default is 1 -> autocut each label. Set value to print N labels before cutting. Allowed Range is 1 to TD2350DSA_MAX_AUTOCUT_NUMLABELS.

// NOTE: This particular #define is needed in DataController.
// All other #defines were moved into the .m file.

// spec section "Select compression mode" command
// NOTE: This should be same as HEADSIZEDOTS/8. And, it should be same as the # of decompressed bytes.
#define TD2350DSA_MAX_COMPRESS_LENGTH_203 59 // 59 = 472/8
#define TD2350DSA_MAX_COMPRESS_LENGTH_300 87 // 87 = 696/8

@end
