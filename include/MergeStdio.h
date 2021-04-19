#pragma once

// Merge Stdio -- Merge stdio for now until we write some routines to take over the stdio
//                more properly. 
//
// This file is a separate include file because the following #define statements confuse the 
// IDE 'inellisense' parsing 

#ifndef __merge_stdio_h
#define __merge_stdio_h
// Merge stdio for now until we write some routines to take over the stdio
// more properly. 

#ifdef cin
#undef cin
#endif

#define cin win
#endif // __merge_stdio_h

