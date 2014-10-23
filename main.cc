/**
 * @file main.cc
 *
 * Time-stamp: <2012-12-04 01:02:01 +0800 by kerwin>
 *
 * command line program to generate TeX, and output to std::cout.
 *
 * @author kerwin\@localhost
 */
#include "include/debug.h"
#include "include/date.h"
#include "include/calendar.h"
#include <iostream>
#ifdef DEBUG
#include <fstream>
std::ofstream MY_ERR;
#endif

/**
 * @brief Our main function
 *
 * Invoke it from command line, either passing no arguments, or the first
 * argument as year.
 *
 * Spits out the TeX code for calendar for year to std::cout.  Redirect them
 * if wished.
 *
 * @return 0 if command executed successfully.
 */
int
main(int argc, char** argv)
{
#ifdef DEBUG
	MY_ERR.open("calendar.log");
	MY_ERR << "main() called with " << argc << " argument(s)." << std::endl;
#endif
	unsigned int year=2012;
	try {
		if (argc > 1) {
			std::string s(argv[1]);
			std::stringstream ss;
			ss << s;
			ss >> year;
			if ((year<=1900) || (year>=2100)) {
				throw Exception("Invalid parameter passed");
			}
		}
		Calendar c(year);
		std::cout << c.getFullYearTex();
#ifdef DEBUG
		MY_ERR << "Closing debugging log." << std::endl;
		MY_ERR.close();
#endif
	}
	catch (BeautyException h) {
		std::cerr << h.message() << std::endl
				  << "See debug log for more details." << std::endl;
	}
	catch (Exception h) {
		std::cerr << h.message() << std::endl
				  << "See debug log for more details." << std::endl;
	}
	return 0;
}

// now documentation
/**
 * @mainpage LaTeX calendar main page
 *
 * @section intro_sec Introduction
 *
 * I got annoyed by the amount of edits I need to do to make Emacs calendar
 * usable, so I wrote my own C++ program to do so, re-learning some C++ and
 * Doxygen along the way.
 *
 * @section install_sec Installation
 *
 * Make and run
 *     @verbatim ./calendar <year> @endverbatim
 * from the command line.  Omitting the argument \e \<year\> gives a 2012 calendar.
 *
 * The default is output to cout(stdout).  Redirect if desired.
 *
 * @section warning_sec Warning
 *
 * No warranty implied.  Use at your own risk.
 */
