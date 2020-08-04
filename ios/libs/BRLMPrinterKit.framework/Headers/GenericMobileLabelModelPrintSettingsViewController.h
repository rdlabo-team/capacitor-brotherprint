//
//  GenericMobileLabelModelPrintSettingsViewController.h
//  BMSPrinterKit
//
//  Created by BMS on 9/27/13.
//  Copyright (c) 2013 Brother Mobile Solutions. All rights reserved.
//

#import "PrintSettingsViewController.h"
#import "GenericMobileLabelModelPrintSettings.h"
#import "PrintSettingTableViewController.h" // for PrintSettingTableViewResponder protocol, internal use only


////////////////////////////////////////////////////////////////////////////////


//*********************************************************************
// GenericMobileLabelModelPrintSettingsViewController INTERFACE
//
//*** IMPORTANT: *** DO NOT INSTANTIATE AN OBJECT OF THIS CLASS!!!!! ***
// The GenericMobileLabelModelPrintSettingsViewController class is essentially an
// ABSTRACT class.
// Instead, you should instantiate the model-specific class,
// e.g. RJ4040PrintSettingsViewController.
//*********************************************************************


@interface GenericMobileLabelModelPrintSettingsViewController : PrintSettingsViewController < UITableViewDelegate, UITableViewDataSource, PrintSettingTableViewResponder>
{
}

// "style" MUST be set to UITableViewStyleGrouped!!
-(id)initWithStyle:(UITableViewStyle)style;


// SDK Designers: The PrintSettingsViewController superclass defines several properties:
//   printSettings, popoverSize, rowHeight, fontSize, printSettingsDelegate,
//   showIPAddressHelpButton, IPAddressHelpFileName, and showSettingsOptionMask.
// So, they are not defined in here.
// However, this class and/or a model-specific subclass should "init" these properties
// and handle setter overrides (if necessary).
//
// SDK Users: You should be familiar with the superclass properties so you can set
// them accordingly, in particular the printSettingsDelegate (which handles
// the PrintSettingsResponder protocol).


@end


