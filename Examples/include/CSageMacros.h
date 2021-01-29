// This file copyright(c) 2021 Rob Nelson, All Rights Reserved.    E-mail rob@projectsagebox.com for more information.
//

// --------------------------------------------------------------------------------------------------------------
// PRELIMINARY VERSION.  This file has been brought into the Sagebox project from the original sources and has 
// not yet been commented for Sagebox, or properly formatted (I used tabs and am converting to spaces).
// --------------------------------------------------------------------------------------------------------------
//
// ****** UNDER CONSTRUCTION ******
//
// This file is still under construction and may not yet include specifics, awaiting proper testing and integration into Sagebox.
//

//#pragma once
#if !defined(_CSageMacros_H_)
#define _CSageMacros_H_


#define QuickSandbox(_Class)	class _Class : public CWindow			\
								{										\
										SageControlDelete				\
									public:								\
										void Main();					\
								};		



#define QuickConsole(_Class)	QuickSandbox(_Class)						\
								int main(int argc, _TCHAR* argv[])			\
								{ 											\
									CSageBox cSageBox;						\
									cSageBox.Main(new _Class);				\
																			\
									return(0);								\
								}											

#define AutoSandbox(Class) QuickSandbox(Class) int main() { return CSageBox::AutoMain(new Class); }

#endif // _CSageMacros_H_