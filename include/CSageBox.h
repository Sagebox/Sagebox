#pragma once

// This file is for the purpose of being able to #include "Sagebox.h" rather than CSagebox.h and has no other function.
// The original styling was to include "CSagebox.h", which has moved to "Sagebox.h", which is currently in transition.
// 
// The plan is currently to deprecate CSagebox.h, but it is still in the testing process, and CSagebox.h may stay to 
// make the point (as standard notation) that CSagebox is the main class, even though there is an alias "Sagebox" used most of the time. 

// This file is currently not necessary, as it only includes "CSagebox.h" which may be included directly -- as mentioned, this is in-progress and TBD

#include "Sagebox.h"
