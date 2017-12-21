/* Copyright (C) 2017 Roman Koshelev */
#pragma once
#include <iostream>

inline void log(const char * s)
{
	std::clog << s << "\n";
}

#define DEBUG_LOGIC

#define DEBUG_LOGIC
#ifdef DEBUG_LOGIC

#define assert_debug(flag,  ...) \
	if ((flag)) fprintf(stderr,  __VA_ARGS__)

#define debug_call(variable, expression)  \
	printf("begin - "#expression" \n");   \
	variable = expression;                \
	printf("end   - "#expression" \n")

#else

#define assert_debug(arg1, ...) ;
#define debug_call(variable, expression) variable = expression

#endif