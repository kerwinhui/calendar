/**
 * @file beautyexception.h
 *
 * Time-stamp: <2011-12-04 22:35:11 +0800 by kerwin>
 *
 * Pretty exception class header file
 *
 * @author kerwin\@localhost
 */

#ifndef KERWIN_BEAUTYEXCEPTION_H
#define KERWIN_BEAUTYEXCEPTION_H

#include "exception.h"
#include <string>
#include <sstream>

/**
 * @brief Beautified Exception class
 *
 * idea partially shamelessly lifted from
 *
 * http://caterpillar.onlyfun.net/Gossip/CppGossip/CustomExceptionClass.html
 *
 * http://stackoverflow.com/questions/679021/how-to-find-the-name-of-the-current-function-at-runtime-c
 */
class BeautyException : public Exception {
  public:
		/**
		 * @brief constructor
		 *
		 * @param errtype  Error type (e.g. "invalid parameter")
		 * @param file     __FILE__ where it is detected
		 * @param function __PRETTY_FUNCTION__ where it is detected
		 * @param line     __LINE__ where it is detected
		 * @param varname  variable name that is causing this exception
		 * @param value    value that is causing this exception
		 */
	BeautyException(const char* errtype, const char* file, const char* function, int line, const char* varname, int value)
	{
		std::string str1;
		std::stringstream sstr;
		sstr << "Exception " << errtype << ": file " << file << " function "
			 << function << " line " << line << " cannot have " << varname
			 << " = " << value << std::endl;
		sstr >> str1;
		_message = str1.c_str();
	}
		/**
		 * @brief get the exception message
		 *
		 * @return exception message string
		 */
	virtual const char* message()
	{
		return _message;
	}
};

/**
 * @relates BeautyException
 * @brief standard template for integer invalid parameter.
 * @param x variable name of invalid parameter
 */
#define INVALID_PARAM(x) \
	BeautyException("(InvalidParameter) ", __FILE__ , __PRETTY_FUNCTION__ , __LINE__ , #x , (int)x)

#endif	// KERWIN_BEAUTYEXCEPTION_H
