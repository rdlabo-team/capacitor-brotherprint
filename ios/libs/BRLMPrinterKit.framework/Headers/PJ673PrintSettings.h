//
//  PJ673PrintSettings.h
//  BMSPrinterKit
//
//  Created by BMS on 9/11/12.
//  Copyright (c) 2012 Brother Mobile Solutions. All rights reserved.
//

#import "GenericPocketJetPrintSettings.h"

@interface PJ673PrintSettings : GenericPocketJetPrintSettings <PrintSettingsDelegate>

// These constants below are provided for reference only. DO NOT MODIFY!
#define PJ673_RESOLUTION_HORZ 300
#define PJ673_RESOLUTION_VERT 300

// Arbitrarily restrict extraFeed to 11" at 300 dpi. There is no reason to be larger.
#define PJ673_MAX_EXTRAFEEDDOTS (11*PJ673_RESOLUTION_VERT)

// NOTE: These custom MIN/MAX settings will be built into framework, so you can't change them.
// And, the setter functions will clip to these values. The reasons for these choices are described below.
// If you require something different for some reason, please contact Technical Support.
#define PJ673_CUSTOM_WIDTHDOTS_MIN 1200 // 4" due to likely skewing of narrower paper. Otherwise can be lower.
#define PJ673_CUSTOM_WIDTHDOTS_MAX 2550 // 8.5" = max paper width
#define PJ673_CUSTOM_LENGTHDOTS_MIN 600 // 2" to allow PerfRoll paper to print anything due to large unprintable area
#define PJ673_CUSTOM_LENGTHDOTS_MAX 30000 // 100", arbitrary

//*** IMPORTANT ***
// SDK Users:
// You should NEVER instantiate an object of the GenericPocketJetPrintSettings class.
// Instead, you should instantiate one of the model-specific subclasses (i.e. PJ673PrintSettings).
//
// For a list of important properties in the settings that your application needs to set,
// refer to the GenericPocketJetPrintSettings superclass.

// SDK Designers:
// The GenericPocketJetPrintSettings class handles all the important work.
// This subclass only needs to provide the model-specific data to the generic class.

//*** ADDITIONAL PROPERTIES not handled by GenericPocketJetPrintSettings


@end
