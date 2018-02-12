/* Copyright (C) 2017 Roman Koshelev */
#pragma once
#include <iostream>

inline void log(const char * s)
{
	//std::clog << s << "\n";
}

//#define DEBUG_LOGIC

//#define DEBUG_LOGIC
#ifdef DEBUG_LOGIC

#define assert_debug(arg1, ...) ;
#define debug_call(variable, expression) variable = expression

#else

#define assert_debug(arg1, ...) ;
#define debug_call(variable, expression) variable = expression

#endif
