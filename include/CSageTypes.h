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