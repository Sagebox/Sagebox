
// SageBox -- Windows Template

#include "CSageBox.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR lpCmdLine,_In_ int nCmdShow)
{
    CSageBox cSageBox;                              // Create the main SageBox object.  Do this only once.
    auto& cWin = cSageBox.NewWindow();            
 
    cWin.printf("Hello World! -- Press Window Close Button to End Program.\n");

    while(cWin.EventLoop());     // Wait for window to close.  
}

