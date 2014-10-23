/**
 * @file calendar.h
 *
 * Time-stamp: <2011-11-29 20:43:04 +0800 by kerwin>
 *
 * Yearly calendar in 12 month-per-view LaTeX pages
 *
 * @author kerwin\@localhost
 */
#include "debug.h"
#include "date.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

#ifndef KERWIN_CALENDAR_H
#define KERWIN_CALENDAR_H

// forward declaration
//class Calendar;

// definition
/**
 * @brief Yearly calendar
 *
 * A holder for things needed to generate TeX calendar
 */
class Calendar {
  public:
	Calendar();
	Calendar(unsigned int);
	std::string const& getTexPreamble() const;
	std::string const& getTexMonth(unsigned int) const;
	std::string const& getTexMonthSmall(unsigned int) const;
	std::string const& getTexEnd() const;
	std::string const& getFullYearTex() const;
	bool isSolar(Date const&) const;
	bool isPublicHoliday(Date const&) const;
	std::string getSolarName(Date const&) const;
	std::string const& getPublicHolidayName(Date const&) const;
	std::string const& getSolarTime(Date const&) const;
	bool setSolar(std::ifstream&);
	bool setPublicHoliday(std::ifstream&);
	bool setList(std::ifstream&,std::ifstream&);
	bool updateList();
  private:
	unsigned int _year;
	///< Gregorian year of calendar
	static const char* const _nSolarTermName[];
	///< array holding Chinese names of the 24 Solar Terms
	static const char* const _nChineseMonthName[];
	///< array holding Chinese name of the months of Chinese calendar
	static const char* const _nChineseDayName[];
	///< array holding Chinese name of the days of Chinese calendar
	static const char* const _nGregorianLongName[];
	///< array holding English names of the Gregorian calendar months
	static const char* const _nGregorianShortName[];
	///< array holding short English name alias of the Gregorian calendar
	///< months
	static const char* const _nCellType[];
	///< LaTeX command choosing color of cells
	static const char* const _nDayOfWeekHeading[];
	///< LaTeX command for the day of week header
	std::map<Date,std::string> _nPublicHoliday;
	///< Hash of public holiday description strings hashed by date
	std::map<Date,std::string> _nSolar;
	///< Hash of only solar terms name string, keyed by date
	std::map<Date,std::string> _nChineseSolar;
	///< Hash combining Chinese calendar day and Solar term, keyed by date
	std::map<Date,std::string> _nSolarPublicHoliday;
	///< Hash combining Solar Term times and public holiday description,
	///< keyed by date
	bool initialised;
	///< have we read the files
	bool forcedInitialise();
	bool initialise();
	void generateChineseSolar();
	void generateSolarPublicHoliday();
	std::string dayCellTex(Date const& d) const;
	std::string& emptyCellTex() const;
};

// associated functions

// inline function declaration
/**
 * @brief default Calendar constructor
 *
 * This method should be inlined.
 * The constructor sets the year to 2012 and call methods to initialise the
 * variables.
 *
 * @return Calendar object for year 2012.
 */
inline
Calendar::Calendar()
{
#ifdef DEBUG
	MY_ERR << "inline Calendar::Calendar() called."
		   << std::endl;
#endif
	_year = 2012; initialised=false;
	initialise();
}

/**
 * @brief Calendar constructor
 *
 * This method should be inlined.
 * The constructor sets the year to the argument and call methods to
 * initialise the variables.
 *
 * @param year Gregorian year of calendar.
 *
 * @return Calendar object for year given.
 */
inline
Calendar::Calendar(unsigned int year)
{
#ifdef DEBUG
	MY_ERR << "inline Calendar::Calendar(" << year << ") called."
		   << std::endl;
#endif
	_year = year; initialised=false;
	initialise();
}

/**
 * @brief Calendar initialiser
 *
 * This method should be inlined.
 *
 * This method has no argument
 *
 * Checks if the calendar was initialised.  If not, initialise the calendar.
 *
 * @retval true if initialised
 * @retval forcedInitialise() if forced to initialise.
 */
inline
bool
Calendar::initialise() {
#ifdef DEBUG
	MY_ERR << "inline " << this << "->Calendar::initialise() called."
		   << std::endl;
#endif
	if (initialised) return true;
	return forcedInitialise();
}

#endif	// KERWIN_CALENDAR_H
