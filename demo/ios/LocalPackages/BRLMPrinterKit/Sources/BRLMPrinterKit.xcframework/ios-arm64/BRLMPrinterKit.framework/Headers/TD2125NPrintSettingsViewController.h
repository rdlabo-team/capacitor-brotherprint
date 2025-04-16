//
//  TD2125NPrintSettingsViewController.h
//  BMSPrinterKit
//
//  Created by BMS on 5/5/23.
//  Copyright (c) 2023 Brother Mobile Solutions. All rights reserved.
//

#import <BRLMPrinterKit/GenericMobileLabelModelPrintSettingsViewController.h>
#import <BRLMPrinterKit/TD2125NPrintSettings.h>

// This is the DEFAULT popup contentSize for TD2125NPrintSettingsViewController.
//
// You can override this by setting the "popoverSize" property (defined in the
// PrintSettingsViewController superclass) to something different
// before showing the ViewController.
//
// The popoverSize setting will be propagated to all of the "child" ViewControllers
// of the main ViewController so all of the detail views will be the SAME size as each other.
//
// NOTE: iPad1 and iPad2 size = 1024 x 768, iPad3 = 2048 x 1536
//#define TD2125NPOPUPCONTENTSIZE CGSizeMake(400,480)
#define TD2125NPOPUPCONTENTSIZE CGSizeMake(400,548)
#define TD2125NPOPUPCONTENTSIZE_IOS7 CGSizeMake(400,588)


/*
 This class is a subclass of GenericMobileLabelModelPrintSettingsViewController,
 which implements all the major functionality.
 
 *** SDK Users:  
 DO NOT INSTANTIATE an instance of GenericMobileLabelModelPrintSettingsViewController.
 Always instantiate a model-specific class, e.g. TD2125NPrintSettingsViewController
 */

@interface TD2125NPrintSettingsViewController : GenericMobileLabelModelPrintSettingsViewController
{
}

@end


