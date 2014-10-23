/**
 * @file debug.h
 *
 * Time-stamp: <2011-12-03 15:57:10 +0800 by kerwin>
 *
 * Define the ASSERT macro, and for the DEBUG pass, declare a file stream to
 * log the debugging information.
 *
 * @author kerwin\@localhost
 */

#ifndef KERWIN_DEBUG_H
#define KERWIN_DEBUG_H
#ifdef DEBUG
	#include <iostream>
#endif // DEBUG
//macros

/**
 * @brief preprocessor ASSERT macro
 *
 * @param x boolean expression that we want is true
 */
#include "exception.h"
#ifdef DEBUG
	#define ASSERT(x) \
		if (! (x)) { \
			std::cerr << "ERROR!! Assert " << #x << " failed\n";	\
			std::cerr << " on line " << __LINE__  << "\n";			\
			std::cerr << " in function " << __PRETTY_FUNCTION__ << "\n";\
			std::cerr << " in file " << __FILE__ << "\n";			\
		}
	#include <fstream>
	extern std::ofstream MY_ERR;
#else
	#define ASSERT(x)
#endif // DEBUG

#endif // KERWIN_DEBUG_H
