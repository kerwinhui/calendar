/**
 * @file date.h
 *
 * Time-stamp: <2011-12-03 15:57:35 +0800 by kerwin>
 *
 * This is a simple Date class for storing a date and some associated
 * functions.
 *
 * @author kerwin\@localhost
 */

#ifndef KERWIN_DATE_H
#define KERWIN_DATE_H

// header containing debugging stuff.
#include "debug.h"
#include "beautyexception.h"

/* forward declaration */
class Date;

/* class definition */
/**
 * @brief the Date class for storing and manipulating dates
 *
 * The Date class for storing a date, and converting them between different
 * calendars.
 *
 * Currently only Gregorian and Chinese calendar is supported, and must
 * be within year range 1901--2099 in Gregorian year.
 */
class Date {
  public:
		/* enums declaration here to avoid polluting global namespace */
		/**
		 * @brief enum for calendar type
		 */
	enum CalendarType {
		CALTYPE_GREGORIAN,		///< Gregorian calendar format
		CALTYPE_CHINESE,		///< Chinese calendar format
		CALTYPE_JULIAN,			///< Julian calendar format
		CALTYPE_JD,				///< Julian period
								///< day 0=Julian 4713BCE NY noon
		CALTYPE_MJD				///< Modified Julian Day = JD-2400000.5
	};

		/**
		 * @brief enum for ISO day of week
		 */
	enum DayOfWeek {
		DOW_SUNDAY = 0,				///< Sunday is 0 in ISO
		DOW_MONDAY = 1,				///< Monday is 1 in ISO
		DOW_TUESDAY = 2,			///< Tuesday is 2 in ISO
		DOW_WEDNESDAY = 3,			///< Wednesday is 3 in ISO
		DOW_THURSDAY = 4,			///< Thursday is 4 in ISO
		DOW_FRIDAY = 5,				///< Friday is 5 in ISO
		DOW_SATURDAY = 6			///< Saturday is 6 in ISO
	};

		/**
		 * @brief prototyping date extraction
		 */
	enum DatePart {
		DATEPART_MJD=0,				///< Modified Julian Day
									///< starts ISO 1858-11-17 0h
		DATEPART_JD,				///< Julian Day,
									///< starts Julian 4713BCE NY noon
		DATEPART_GREGORIAN_YEAR,	///< Gregorian Year
		DATEPART_GREGORIAN_MONTH,	///< Gregorian Month
		DATEPART_GREGORIAN_DAY,		///< Gregorian Day
		DATEPART_DAYOFWEEK,			///< ISO day of week
		DATEPART_CHINESE_YEAR,		///< Chinese Year
		DATEPART_CHINESE_MONTH,		///< Chinese Month
		DATEPART_CHINESE_DAY,		///< Chinese Day
		DATEPART_JULIAN_YEAR,		///< Julian Year
		DATEPART_JULIAN_MONTH,		///< Julian Month
		DATEPART_JULIAN_DAY,		///< Julian Day
	};
		/* Constructor */
	Date(int jd=55941, enum CalendarType t=CALTYPE_MJD);
	Date(int year, unsigned int month, unsigned int day, enum CalendarType t=CALTYPE_GREGORIAN);
		/* other methods */
	int getJD() const;
	int getMJD() const;
	int getYear() const;
	unsigned int getMonth() const;
	unsigned int getDay() const;
	int getGregorianYear() const;
	unsigned int getGregorianMonth() const;
	unsigned int getGregorianDay() const;
	int getChineseYear() const;
	unsigned int getChineseMonth() const;
	unsigned int getChineseDay() const;
	int getJulianYear() const;
	unsigned int getJulianMonth() const;
	unsigned int getJulianDay() const;
	enum Date::DayOfWeek getDayOfWeek() const;
	unsigned long long getLunarCalendarData() const;
	unsigned int getDayOfYear() const;
	bool isLeap() const;
	bool isIntercalary() const;
		/* static functions */
	static unsigned long long getLunarCalendarData(unsigned int);
		/* operators */
	Date& operator++();
	Date operator++(int);
	friend bool operator+=(Date &, int);
	friend bool operator-=(Date &, int);
		/* other friends */
  private:
	int _mjd;			///< Modified Julian Day.
};

/* other related functions */
bool isLeapYear(int const);
enum Date::DayOfWeek dayOfWeek(Date const&);
unsigned int daysInMonth(int, unsigned int);
unsigned int daysInMonth(Date const&);
bool isIntercalary(Date&); // chinese leap month
bool operator==(Date const&, Date const&);
bool operator<(Date const&, Date const&);
bool operator!=(Date const&, Date const&);
bool operator<=(Date const&, Date const&);
bool operator>=(Date const&, Date const&);
bool operator>(Date const&, Date const&);
Date operator+(Date const&, int);
Date operator-(Date const&, int);
int chineseFromMJD(int, enum Date::DatePart);
int gregorianFromMJD(int, enum Date::DatePart);
int julianFromMJD(int, enum Date::DatePart);



















































/* inline functions */
/**
 * @brief get the Gregorian year
 *
 * This calls the getGregorianYear() method on the this object, and is
 * provided for convenience only.
 *
 * This function should be inlined.
 *
 * This function takes no argument.
 *
 * @return Gregorian year of the date stored.
 */
inline
int
Date::getYear() const {
#ifdef DEBUG
	MY_ERR << "inline " << this << "->Date::getYear() called." << std::endl;
#endif	// DEBUG
	return getGregorianYear();
};

/**
 * @brief get the Gregorian month
 *
 * This calls the getGregorianMonth() method on the this object, and is
 * provided for convenience only.
 *
 * This function should be inlined.
 *
 * This function takes no argument.
 *
 * @return Gregorian month of the date stored.
 */
inline
unsigned int
Date::getMonth() const {
#ifdef DEBUG
	MY_ERR << "inline " << this << "->Date::getMonth() called." << std::endl;
#endif	// DEBUG
	return getGregorianMonth();
};

/**
 * @brief get the Gregorian day
 *
 * This calls the getGregorianDay() method on the this object, and is
 * provided for convenience only.
 *
 * This function should be inlined.
 *
 * This function takes no argument.
 *
 * @return Gregorian day of the date stored.
 */
inline
unsigned int
Date::getDay() const {
#ifdef DEBUG
	MY_ERR << "inline " << this << "->Date::getDay() called." << std::endl;
#endif	// DEBUG
	return getGregorianDay();
};

/**
 * @brief Julian Day at noon
 *
 * This function should be inlined for performance.
 *
 * This function takes no argument
 *
 * @return Julian Day of this date at noon
 */
inline
int
Date::getJD() const {
	return _mjd+2400001;
}

/**
 * @brief Modified Julian Day at midnight
 *
 * This function should be inlined for performance.
 *
 * This function takes no argument
 *
 * @return Modified Julian Day of this date at midnight
 */
inline
int
Date::getMJD() const
{
	return _mjd;
}

/**
 * @brief obtain Gregorian calendar year
 *
 * This function should be inlined for performance.
 *
 * This function takes no argument
 *
 * @return Year number of this date in Gregorian calendar
 */
inline
int
Date::getGregorianYear() const
{
	return gregorianFromMJD(_mjd,Date::DATEPART_GREGORIAN_YEAR);
}

/**
 * @brief obtain Gregorian calendar month
 *
 * This function should be inlined for performance.
 *
 * This function takes no argument
 *
 * @return Month number of this date in Gregorian calendar.
 */
inline
unsigned int
Date::getGregorianMonth() const
{
	return gregorianFromMJD(_mjd,Date::DATEPART_GREGORIAN_MONTH);
}

/**
 * @brief obtain Gregorian calendar day
 *
 * This function should be inlined for performance.
 *
 * This function takes no argument
 *
 * @return Day number of this date in Gregorian calendar.
 */
inline
unsigned int
Date::getGregorianDay() const
{
	return gregorianFromMJD(_mjd,Date::DATEPART_GREGORIAN_DAY);
}

/**
 * @brief obtain Chinese calendar year
 *
 * This function should be inlined for performance.
 *
 * This function takes no argument
 *
 * @return Year number of this date in Chinese calendar, using convention
 * year 1 in chinese calendar starts CNY of 2698BCE.
 */
inline
int
Date::getChineseYear() const
{
#ifdef DEBUG
	MY_ERR << this << "->getChineseYear() called." << std::endl;
#endif	// DEBUG
	return chineseFromMJD(_mjd,Date::DATEPART_CHINESE_YEAR);
}

/**
 * @brief obtain Chinese calendar month
 *
 * This function should be inlined for performance.
 *
 * This function takes no argument
 *
 * @return Month number of this date in Chinese calendar.
 */
inline
unsigned int
Date::getChineseMonth() const
{
#ifdef DEBUG
	MY_ERR << this << "->getChineseMonth() called." << std::endl;
#endif	// DEBUG
	return chineseFromMJD(_mjd,Date::DATEPART_CHINESE_MONTH);
}

/**
 * @brief compute chinese day
 *
 * This function should be inlined for performance.
 *
 * @return day in chinese calendar.
 */
inline
unsigned int
Date::getChineseDay() const
{
#ifdef DEBUG
	MY_ERR << this << "->getChineseDay() called." << std::endl;
#endif	// DEBUG
	return chineseFromMJD(_mjd,Date::DATEPART_CHINESE_DAY);
}

/**
 * @brief obtain Julian calendar year
 *
 * This function takes no argument
 *
 * This function should be inlined for performance.
 *
 * @return Year number of this date in Julian calendar
 */
inline
int
Date::getJulianYear() const
{
	return julianFromMJD(_mjd,Date::DATEPART_JULIAN_YEAR);
}

/**
 * @brief obtain Julian calendar month
 *
 * This function should be inlined for performance.
 *
 * This function takes no argument
 *
 * @return Month number of this date in Julian calendar.
 */
inline
unsigned int
Date::getJulianMonth() const
{
	return julianFromMJD(_mjd,Date::DATEPART_JULIAN_MONTH);
}

/**
 * @brief obtain Julian calendar day
 *
 * This function should be inlined for performance.
 *
 * This function takes no argument
 *
 * @return Day number of this date in Julian calendar.
 */
inline
unsigned int
Date::getJulianDay() const
{
	return julianFromMJD(_mjd,Date::DATEPART_JULIAN_DAY);
}

/**
 * @brief determines if date is on a Gregorian leap year.
 *
 * This function should be inlined.  Provided for convenience only.
 *
 * This function takes no argument.
 *
 * @return see isLeap(int)
 */
inline
bool
Date::isLeap() const
{
#ifdef DEBUG
	MY_ERR << "inline " << this << "->Date::isLeap() called." << std::endl;
#endif	// DEBUG
	return isLeapYear(getGregorianYear());
}

/**
 * @relatesalso Date
 * @brief check for intercalary month
 *
 * This calls the isIntercalary() method on the this object, and is
 * provided for convenience only.
 *
 * This function should be inlined.
 *
 * This function takes no argument.
 *
 * @return see isIntercalary()
 */
inline
bool
isIntercalary(Date& d){
#ifdef DEBUG
	MY_ERR << "inline " "isIntercalary( *" << &d << " ) called." << std::endl;
#endif	// DEBUG
	return d.isIntercalary();
}

/**
 * @brief check for intercalary month
 *
 * This function takes no argument.
 *
 * @return true only if date is on an intercalary (leap) month of the Chinese
 * lunar calendar.
 */
inline
bool
Date::isIntercalary() const{
#ifdef DEBUG
	MY_ERR << "inline " << this << "->Date::isIntercalary() called." << std::endl;
#endif	// DEBUG
	return (getChineseMonth() & 16);
}

/**
 * @brief pre-increment
 *
 * Step forward 1 day.
 *
 * @return reference to incremented.
 */
inline
Date&
Date::operator++()
{
#ifdef DEBUG
	MY_ERR << "++(*" << this << ")called." << std::endl;
#endif
	_mjd++;
	return *this;
}

/**
 * @brief post-increment method
 *
 * This function takes no argument in reality.
 *
 * @param n just there to separate between ++date and date++, since
 * date++ is implicitly date++(0).
 *
 * @return a copy of the Date object currently stored, and increment the
 * underlying date object by 1 day.
 */
inline
Date
Date::operator++(int n){
#ifdef DEBUG
	MY_ERR << "inline ++ *" << this << "(" << n << ") called." << std::endl;
#endif	// DEBUG
	Date result=*this;
	++(*this);
	return result;
}

/**
 * @relatesalso Date
 * @brief method to determines the day of week of a Date argument.
 *
 * This calls the getDayOfWeek() method on the date object, and is provided for
 * convenience only.
 *
 * This function should be inlined.
 *
 * @param d the date we want to know the day of week of
 *
 * @return see getDayOfWeek()
 */
inline
enum Date::DayOfWeek
dayOfWeek(Date const& d){
#ifdef DEBUG
	MY_ERR << "inline " << &d << "->Date::getDayOfWeek() called." << std::endl;
#endif	// DEBUG
	return d.getDayOfWeek();
}

/**
 * @relatesalso Date
 * @brief computes the number of days in the month this day is on.
 *
 * This function should be inlined.  Provided for convenience only.
 *
 * @param d our date
 *
 * @return number of days in the month.  See
 * daysInMonth(int, unsigned int).
 */
inline
unsigned int daysInMonth(Date const& d) {
#ifdef DEBUG
	MY_ERR << "inline daysInMonth(*" << &d << ") called." << std::endl;
#endif	// DEBUG
	return daysInMonth(d.getYear(),d.getMonth());
}

/**
 * @relatesalso Date
 * @brief check equality of two dates
 *
 * @param d1 a date
 * @param d2 another date
 *
 * @retval true if two dates are equal (same year, same month, same day)
 * @retval false otherwise
 */
inline
bool
operator==(Date const& d1, Date const& d2)
{
#ifdef DEBUG
	MY_ERR << "*" << &d1 <<  " == *" << &d2 << " called." << std::endl;
#endif	// DEBUG
	return (d1.getMJD() == d2.getMJD());
}

/**
 * @relatesalso Date
 * @brief check if one date is before another
 *
 * @param d1 a date
 * @param d2 another date
 *
 * @retval true if d1 is before d2
 * @retval false othwerwise
 */
inline
bool
operator<(Date const& d1, Date const& d2)
{
#ifdef DEBUG
	MY_ERR << "*" << &d1 <<  " < *" << &d2 << " called." << std::endl;
#endif	// DEBUG
	return (d1.getMJD() < d2.getMJD());
}

/**
 * @relatesalso Date
 * @brief compare two dates
 *
 * This function should be inlined.  Provided for convenience only.
 *
 * @param d1 first date
 * @param d2 another date
 *
 * @return true if and only if the two dates are different.
 */
inline
bool
operator!=(Date const& d1, Date const& d2){
#ifdef DEBUG
	MY_ERR << "inline *" << &d1 << " != *" << &d2 << ") called." << std::endl;
#endif	// DEBUG
	return !(d1==d2);
}

/**
 * @relatesalso Date
 * @brief compare two dates
 *
 * This function should be inlined.  Provided for convenience only.
 *
 * @param d1 first date
 * @param d2 another date
 *
 * @return true if and only if date d1 is on or before date d2.
 */
inline
bool
operator<=(Date const& d1, Date const& d2){
#ifdef DEBUG
	MY_ERR << "inline *" << &d1 << " <= *" << &d2 << ") called." << std::endl;
#endif	// DEBUG
	return ((d1<d2)||(d1==d2));
}

/**
 * @relatesalso Date
 * @brief compare two dates
 *
 * This function should be inlined.  Provided for convenience only.
 *
 * @param d1 first date
 * @param d2 another date
 *
 * @return true if and only if date d1 is after date d2.
 */
inline
bool
operator>(Date const& d1, Date const& d2){
#ifdef DEBUG
	MY_ERR << "inline *" << &d1 << " > *" << &d2 << ") called." << std::endl;
#endif	// DEBUG
	return (d2<d1);
}

/**
 * @relatesalso Date
 * @brief compare two dates
 *
 * This function should be inlined.  Provided for convenience only.
 *
 * @param d1 first date
 * @param d2 another date
 *
 * @return true if and only if date d1 is on or after date d2.
 */
inline
bool
operator>=(Date const& d1, Date const& d2){
#ifdef DEBUG
	MY_ERR << "inline *" << &d1 << " >= *" << &d2 << ") called." << std::endl;
#endif	// DEBUG
	return ((d1>d2)||(d1==d2));
}


/**
 * @relatesalso Date
 * @brief step forward by n days
 *
 * @param d Date variable to step forward
 * @param n Number of days to step forward
 *
 * @retval true success!
 * @retval false shouldn't happen
 */
inline
bool
operator+=(Date & d, int n)
{
	d._mjd += n;
	return true;
}

/**
 * @relatesalso Date
 * @brief step backward by n days
 *
 * @param d Date variable to step backward
 * @param n Number of days to step backward
 *
 * @retval true success!
 * @retval false shouldn't happen
 */
inline
bool
operator-=(Date & d, int n)
{
	d._mjd -= n;
	return true;
}

/**
 * @relatesalso Date
 * @brief a new date that is n days ahead
 *
 * @param d Date variable to step forward from
 * @param n Number of days to step forward
 *
 * @retval true success!
 * @retval false shouldn't happen
 */
inline
Date
operator+(Date const& d, int n)
{
	return Date(d.getMJD()+n);
}

/**
 * @relatesalso Date
 * @brief a new date that is n days back
 *
 * @param d Date variable to step backward from
 * @param n Number of days to step backward
 *
 * @retval true success!
 * @retval false shouldn't happen
 */
inline
Date
operator-(Date const& d, int n)
{
	return Date(d.getMJD()-n);
}

#endif	// KERWIN_DATE_H
