
// ----------------------------------------
// SageBox -- Console Mode Unit Test / Demo
// ----------------------------------------

#include "CSageBox.h"

// Simple Button Dialog -- Put up a dialog with a few button selections.
//
// Test/Shows how to create a simple dialog in the console mode. 
//
int SimpleButtonDialog(CSageBox & cSageBox)
{
	// Create the dialog with the NoClose() (i.e. don't let user close it with 'X' button), and CenterXY() i.e. center on the screen)

	auto& cDialog = cSageBox.NewDialog(100,100,440,250,"Select a Button",NoClose() | CenterXY());

	cDialog.NewTextWidget(0,15,500,-10,"Select a Button", JustCenterX() | bgColor({50,50,170}) | TextCenterX() | Font("arial,17")); 
	cDialog.NewRadioButton(180,60+25*0,"Easy"		,Group(1)); 
	cDialog.NewRadioButton(180,60+25*1,"Medium"		,Group(1)); 
	cDialog.NewRadioButton(180,60+25*2,"Difficult"	,Group(1)); 
	cDialog.NewRadioButton(180,60+25*3,"Hardest"	,Group(1)); 
	auto& CloseButton = cDialog.NewButton(0,60+25*4+10,-50,0,"Ok",Center()); 

	cDialog.Go();	// Launch the dialog and show the window

	printf("Select a Button in the Dialog Window to continue....\n");

	// Look for cerain events.
	//
	// Look for button selections, but don't act until the window is closed.

	// This also shows WindowsClosing() vs. ClosePressed(), where WindowClosing() can be 
	// active even if the user didn't press the close button (i.e a system shutdown, F4, etc.

	while (cSageBox.EventLoop())
	{
		if (CloseButton.Pressed()) break;
		if (cDialog.WindowClosing()) printf("Window Closing...\n");
		if (cDialog.ClosePressed()) printf("Window Close Pressed...\n");
	}
	int iButton = cDialog.GetSelectedRadioButton(1);

	printf("\nButton %d was selected.\n",iButton);
	return 0;
}
int main()
{  
	CSageBox cSageBox;
    SimpleButtonDialog(cSageBox);
}
