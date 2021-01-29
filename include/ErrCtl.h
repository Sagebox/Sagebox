// File copyright(c) 2021 Rob Nelson, all rights reserved.
// rob@projectsagebox.com.   Sagebox is free for personal use. 

// ***********************************************************************************************
// ErrCtl.h
//
// This file is used for internal Sagebox development and contains various debug macros used.
// Some of these may appear as code in various .H files and will be not be compiled within any
// programs.
// 
// ***********************************************************************************************
#pragma once

#if !defined(_ErrCtl_H_)
#define _ErrCtl_H_
#define errMsg(_msg) stdNoMsg
constexpr const char * errPassthrough = stdNoMsg; 

constexpr const char * kErrWarn = "[warn]";
constexpr const char * kErrErr = "[error]";

#ifdef _LOCALTEST
#define _sbDebug(_x) _x;
#define sbDebug 1
#define debugMsg(_x) { std::printf("%s\n",(const char *) (_x)); }
#else
#define _sbDebug(_x) 
#define sbDebug 0
#define debugMsg(_x)
#endif
#endif