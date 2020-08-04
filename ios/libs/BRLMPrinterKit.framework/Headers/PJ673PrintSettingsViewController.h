//
//  PJ673PrintSettingsViewController.h
//  BMSPrinterKit
//
//  Created by BMS on 8/8/12.
//  Copyright (c) 2012 Brother Mobile Solutions. All rights reserved.
//

#import "GenericPocketJetPrintSettingsViewController.h"
#import "PJ673PrintSettings.h"

// This is the DEFAULT popup contentSize for PJ673PrintSettingsViewController.
//
// You can override this by setting the "popoverSize" property (defined in the
// PrintSettingsViewController superclass) to something different
// before showing the ViewController.
//
// The popoverSize setting will be propagated to all of the "child" ViewControllers
// of the main ViewController so all of the detail views will be the SAME size as each other.
//
// NOTE: iPad1 and iPad2 size = 1024 x 768, iPad3 = 2048 x 1536
#define PJ673POPUPCONTENTSIZE CGSizeMake(400,480)
#define PJ673POPUPCONTENTSIZE_IOS7 CGSizeMake(400,588)

/*
 This class is a subclass of GenericPocketJetPrintSettingsViewController,
 which implements all the major functionality.
 
 *** SDK Users:
 DO NOT INSTANTIATE an instance of GenericPocketJetPrintSettingsViewController.
 Always instantiate a model-specific class, e.g. PJ673PrintSettingsViewController
 */

@interface PJ673PrintSettingsViewController : GenericPocketJetPrintSettingsViewController
{
}

@end


