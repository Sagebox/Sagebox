
// ***************************************************
// Calculator -- SageBox Simple Calculator Application 
// ***************************************************
//
// Calculator is a simple SageBox program consisting of roughly 100 executable lines of C++ code.
// It provides simple calculator functions such as +,-,/,*, as well as more advanced ones such as 
// Sin, Cos, Log, etc.  It also features memory store, recall, and clear.
//
// See Calculator.cpp for more information and documentation
//
// 
#include "CSageBox.h"
#include "Calculator.h"

int main()
{
	// Create Sagebox and the run the calulator. 
	// There is no reason to not just create Sagebox within the calculator itself. 
	// I just do it here out of habit, since many programs can later be converted into 
	// reusable routines where you can pass an existing CSageBox or CWindow element as a
	// starting base.	
	//
	// In this example, cSageBox is sent in (when usually I would send in the window) because the Calculator
	// wants a specific size for the window, and it is better to let it create it and set its size. 
	//
	// Although, a window could be sent in that the Calculator could just resize anyway.
	// In this case, it works out better to send int CSageBox because of the interaction
	// between the Window Menu and Display graphic -- See notes in Calculator::InitWindow() for 
	// more information

	CSageBox cSageBox("SageBox -- Simple Calculator Example"); 
	Calculator calc(cSageBox);		// Instantiate Calculator
	calc.Run();						// Run it (i.e. it's main() function)
	return 0;

}

// Windows Entry (i.e. non-console version)
//
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR lpCmdLine,_In_ int nCmdShow)
{
   	main();
}
