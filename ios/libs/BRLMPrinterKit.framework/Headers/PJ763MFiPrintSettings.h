//
//  PJ763MFiPrintSettings.h
//  BMSPrinterKit
//
//  Created by BMS on 9/11/12.
//  Copyright (c) 2012 Brother Mobile Solutions. All rights reserved.
//

#import "GenericPocketJetPrintSettings.h"

@interface PJ763MFiPrintSettings : GenericPocketJetPrintSettings <PrintSettingsDelegate>

// These constants below are provided for reference only. DO NOT MODIFY!
#define PJ763MFi_RESOLUTION_HORZ 300
#define PJ763MFi_RESOLUTION_VERT 300

// Arbitrarily restrict extraFeed to 11" at 300 dpi. There is no reason to be larger.
#define PJ763MFi_MAX_EXTRAFEEDDOTS (11*PJ763MFi_RESOLUTION_VERT)

// NOTE: These custom MIN/MAX settings will be built into framework, so you can't change them.
// And, the setter functions will clip to these values. The reasons for these choices are described below.
// If you require something different for some reason, please contact Technical Support.
#define PJ763MFi_CUSTOM_WIDTHDOTS_MIN 1200 // 4" due to likely skewing of narrower paper. Otherwise can be lower.
#define PJ763MFi_CUSTOM_WIDTHDOTS_MAX 2550 // 8.5" = max paper width
#define PJ763MFi_CUSTOM_LENGTHDOTS_MIN 600 // 2" to allow PerfRoll paper to print anything due to large unprintable area
#define PJ763MFi_CUSTOM_LENGTHDOTS_MAX 30000 // 100", arbitrary

//*** IMPORTANT ***
// SDK Users:
// You should NEVER instantiate an object of the GenericPocketJetPrintSettings class.
// Instead, you should instantiate one of the model-specific subclasses (i.e. PJ763MFiPrintSettings).
//
// For a list of important properties in the settings that your application needs to set,
// refer to the GenericPocketJetPrintSettings superclass.

// SDK Designers:
// The GenericPocketJetPrintSettings class handles all the important work.
// This subclass only needs to provide the model-specific data to the generic class.

//*** ADDITIONAL PROPERTIES not handled by GenericPocketJetPrintSettings
@property (nonatomic, assign) ROLL_CASE_OPTION rollCaseOption;
@property (nonatomic, assign) PRINT_SPEED_OPTION printSpeedOption;


@end
