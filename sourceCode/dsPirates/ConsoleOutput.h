/**
@file
The function defined here open a console window. Usefule to support testing, and because having a console can be handy...
In this file is defined a function to open a console and redirect cout there.

Courtesy of http://www.halcyon.com/~ast/dload/guicon.htm
May have copyright!!!*/
#include "stdafx.h"
#pragma once

#ifdef _DEBUG

///Number of lines in the console. I CANNOT GET DOXYGEN READ THIS!!!
#define CONSOLE_IO_MAX_CONSOLE_LINES 5000

///The console prints this when it starts.
#define CONSOLE_IO_OPEN_MESSAGE "Debug console"

/**This function set up a console and redirect the stdio streams to that.*/
void RedirectIOToConsole();
#endif