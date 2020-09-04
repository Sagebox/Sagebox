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
								}											\

#endif // _CSageMacros_H_