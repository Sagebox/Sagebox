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

#if !defined(_CSageTypes_H_)
#define _CSagTypes_H_


// /D NoSageSpace       - then Sage:: and Sage::opt must be referred to explicitly for things like Sage::CWindow or Sage::CStringg
// /D SageOptOnly       - is specified, Sage::opt functions can be referred to as opt:: diretly, i.e. opt::Transparent(), opt::fgColor(), etc. -- otherwise Sage::opt::fgColor(), etc. must be used.
// /D SageOnly          - Sage namespace is defined, but opt:: must be used, i.e. opt::Transparent(), opt:: fgColor, etc.
//
#ifndef NoSageSpace

using namespace Sage;
using namespace opt;

#elif SageOptOnly
using opt = Sage::opt;
#elif SageOnly
using namespace Sage;
#endif


#endif // _CSageTypes_H_