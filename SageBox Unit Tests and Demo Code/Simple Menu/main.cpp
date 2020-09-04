
// ----------------------------------------
// SageBox -- Console Mode Unit Test / Demo
// ----------------------------------------

#include "CSageBox.h"

class CMenuWindow : public Sage::CWindow
{
	SageControlDelete;
public:
	MsgStatus OnMenu(int iMenuID) override;
};
CMenuWindow::MsgStatus CMenuWindow::OnMenu(int iMenuID)
{
	printf("Message OnMenu(): %d menu item received\n",iMenuID); 
	return MsgStatus::Ok;
}

// -------------------------------
// MenuTest -- Test a simple menu.
// -------------------------------
//
// This adds three menu items, one of which is a sub-menu with 2 items.
// This tests disabling and enabling the sub-menu.
//
// There are also some other random tests, such as a TextWidget that writes out a Hello World message with a shadow and other
// criteria (i.e. font size, transparent, allow dragging, etc.)
//
// This also attempts to load a jpeg.  On the original system, it would display the jpeg in various sizes.
// On other systems, it should fall through and print out a message to the console that the JPEG was not loaded.
// Replace the JPEG path with a valid jpeg to test the bitmap display functions.
//
// This then goes into the EventLoop(), waiting for the Menu Events or for the window to close.
//
int MenuTest(CSageBox & cSageBox)
{

	auto& cWin = cSageBox.NewWindow(new CMenuWindow);

	// Test a random TextWidget with a shadow.  AllowDrag() allows the text to be moved around the screen with the mouse.

	auto& cText = cWin.TextWidget(0,200,"This is Text",JustCenterX() | TextCenterX() | Font("arial,100") | Transparent() | AllowDrag() | AddShadow());

	auto cMenu		= cWin.CreateMenu();					// Create a menu (in this case, the main menu)
	auto cSubMenu	= cWin.CreateMenu();					// Create a menu (a sub menu)
	cMenu.AddMenuItem("&Disable Sub Menu (#1000)",1000);
	cMenu.AddMenuItem("&Sub Menu (#2)",cSubMenu);			// Add the sub menu to the main menu
	cMenu.AddMenuItem("&Enable Sub Menu (#1003)",1003);

	cSubMenu.AddMenuItem("Sub Menu Item &1 (#2000)",2000);
	cSubMenu.AddMenuItem("Sub Menu Item &2 (#2001)",2001);

	auto cMenu3 = cWin.GetMenu(cSubMenu.hMenu);

	// Ensure we were able to retrieve a menu object from the menu handle. 

	if (cMenu3.hMenu != cSubMenu.hMenu) printf("Error: cMenu3 != cMenu2\n");

	// Activate the main menu

	cMenu.ActivateMenu();

	int iCount = 0;

	// Test outputting jpeg images -- on the original test system, this would show a jack-of-hearts card in 
	// the original size and as a QuickThumbnail.
	//
	// On other systems, this should all fall through passively

	auto cBitmap = cWin.ReadJpegFile("c:\\work\\test-jack.jpg");	// Put in a valid JPEG path here.

	// If we didn't find it (or some other error), display a message. 
	// The display attempts should just fallthrough with no error, since there is no bitmap in cBitmap

	if (cWin.GetJpegError() != CJpeg::Status::Ok)
		printf("JPEG File not loaded (Probably not found).  No other error should occur.\n");

	cSageBox.BitmapWindow(cBitmap,Title("This is a Title"));	// Create a bitmap window with the full-sized bitmap
	cSageBox.QuickThumbnail(cBitmap,100,0);						// Create a Quick Thumbnail window, resizing the bitmap to 100 pixels in width
	
	// Look for events until the window is closed or we decided to exit because te
	// Close button is pressed.

	while(cWin.EventLoop())
	{
		// print out events as they happen to make sure
		// we don't have a out-of-control loop 
		// This should print out only mouse movements, clicks,
		// menu entries, etc.

		printf("Event #%d\n",++iCount);		// Print event count to the console

		int iMenuItem;

		// if a menu item is selected, print it out.
		// Also, as a test, test enabling and disabling the 
		// sub menu (based on menu selection), testing
		// the disable and enable functions.
		//

		if (cWin.MenuItemSelected(iMenuItem))
		{
			if (iMenuItem == 1000) 
			{
				printf("Enabling SubMenu.\n");
				cMenu.EnableItem(cSubMenu,false);
			}
			else
			if (iMenuItem == 1003) 
			{
				printf("Enabling SubMenu.\n");
				cMenu.EnableItem(cSubMenu,true);
			}

			printf("Menu Item Selected = %d\n",iMenuItem);
		}
	}
	return 0;
}
int main()
{  
	CSageBox cSageBox;
    MenuTest(cSageBox);
}
