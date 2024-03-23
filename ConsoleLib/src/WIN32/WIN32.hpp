#pragma once
#ifndef _WIN32
#error WIN32 folder should be moved or removed on non Windows machines
#endif // _WIN32

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "Include/ConsoleLib.hpp"

inline HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
inline HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);