/**
 * @file exception.h
 *
 * Time-stamp: <2011-12-04 22:35:44 +0800 by kerwin>
 *
 * Basic exception class header file, for all other custom exception class to
 * inherit from.
 *
 * @author kerwin\@localhost
 */

#ifndef KERWIN_EXCEPTION_H
#define KERWIN_EXCEPTION_H

/**
 * @brief General purpose Exception class
 *
 * idea shamelessly lifted from
 *
 * http://caterpillar.onlyfun.net/Gossip/CppGossip/CustomExceptionClass.html
 */
class Exception {
  public:
		/**
		 * @brief constructor
		 */
	Exception(){}
		/**
		 * @brief another constructor, this time with message
		 * @param message Exception message string
		 */
	Exception(const char* message) : _message(message) {}
		/**
		 * @brief get the exception message
		 * @return Exception message string (char array)
		 */
	virtual const char* message()
	{
		return _message;
	}
  protected:
	const char * _message;
		///< The exception message, hopefully in readable format ;-)
};

#endif	// KERWIN_EXCEPTION_H
