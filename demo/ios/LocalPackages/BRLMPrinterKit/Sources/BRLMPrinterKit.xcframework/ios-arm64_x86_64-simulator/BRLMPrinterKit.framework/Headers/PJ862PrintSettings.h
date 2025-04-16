//
//  PJ862PrintSettings.h
//  BMSPrinterKit
//
//  Created by BMS on 5/6/22.
//  Copyright (c) 2022 BMS. All rights reserved.
//

#import <BRLMPrinterKit/GenericPocketJetPrintSettings.h>


@interface PJ862PrintSettings : GenericPocketJetPrintSettings <PrintSettingsDelegate>

// These constants below are provided for reference only. DO NOT MODIFY!
// NOTE: Printer is actually 203x203, but Command Ref Guide is defining all specs as though 200x200.
#define PJ862_RESOLUTION_HORZ 200
#define PJ862_RESOLUTION_VERT 200

// Arbitrarily restrict extraFeed to 11". There is no reason to be larger.
#define PJ862_MAX_EXTRAFEEDDOTS (11*PJ862_RESOLUTION_VERT)

// NOTE: These custom MIN/MAX settings will be built into framework, so you can't change them.
// And, the setter functions will clip to these values. The reasons for these choices are described below.
// If you require something different for some reason, please contact Technical Support.
#define PJ862_CUSTOM_WIDTHDOTS_MIN 800 // 4" due to likely skewing of narrower paper. Otherwise can be lower.
#define PJ862_CUSTOM_WIDTHDOTS_MAX 1700 // 8.5" = max paper width
#define PJ862_CUSTOM_LENGTHDOTS_MIN 400 // 2" to allow PerfRoll paper to print anything due to large unprintable area
#define PJ862_CUSTOM_LENGTHDOTS_MAX 20000 // 100", arbitrary

//*** IMPORTANT ***
// SDK Users:
// You should NEVER instantiate an object of the GenericPocketJetPrintSettings class.
// Instead, you should instantiate one of the model-specific subclasses (i.e. PJ862PrintSettings).
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
