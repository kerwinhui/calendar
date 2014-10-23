/**
 * @file date.cc
 *
 * Time-stamp: <2011-12-09 01:13:13 +0800 by kerwin>
 *
 * This is a simple Date class for storing a date and some associated
 * functions.
 *
 * @author kerwin\@localhost
 */

#include "include/date.h"

/* constants goes in an anonymous namespace */
namespace {
	// Using the (Chinese-American) convention
	// Chinese calendar year 1 in 2698 BCE.
	const int CHINESE_CALENDAR_BEGIN=2698;
	// JD data for CNY
	const int GREGORIAN_ZERO_JULIAN_OFFSET = 1721425; // JD of Greg 0001-01-00 12h
	const int DAYS_IN_FOUR_CENTURY = 400*365+97*1;	  // 4 Gregorian century
	const int DAYS_IN_HALF_CENTURY = 365*50+12;		  // 50 year with 12 leap
	const int DAYS_IN_FOUR_YEAR = 365*4+1;			  // 4 year, 1 leap
	const int LUNAR_TABLE_START_MJD = 0x3C4A;						 // 1901CNY
	const int LUNAR_TABLE_END_MJD= 0x1121B+DAYS_IN_HALF_CENTURY-365; // 2099EOY
/*
const unsigned long long LUNAR_TABLE[] = {
		0x04AE53,0x0A5748,0x5526BD,0x0D2650,0x0D9544,0x46AAB9,0x056A4D,0x09AD42,0x24AEB6,0x04AE4A,//1901-1910
		0x6A4DBE,0x0A4D52,0x0D2546,0x5D52BA,0x0B544E,0x0D6A43,0x296D37,0x095B4B,0x749BC1,0x049754,//1911-1920
		0x0A4B48,0x5B25BC,0x06A550,0x06D445,0x4ADAB8,0x02B64D,0x095742,0x2497B7,0x04974A,0x664B3E,//1921-1930
		0x0D4A51,0x0EA546,0x56D4BA,0x05AD4E,0x02B644,0x393738,0x092E4B,0x7C96BF,0x0C9553,0x0D4A48,//1931-1940
		0x6DA53B,0x0B554F,0x056A45,0x4AADB9,0x025D4D,0x092D42,0x2C95B6,0x0A954A,0x7B4ABD,0x06CA51,//1941-1950
		0x0B5546,0x555ABB,0x04DA4E,0x0A5B43,0x352BB8,0x052B4C,0x8A953F,0x0E9552,0x06AA48,0x6AD53C,//1951-1960
		0x0AB54F,0x04B645,0x4A5739,0x0A574D,0x052642,0x3E9335,0x0D9549,0x75AABE,0x056A51,0x096D46,//1961-1970
		0x54AEBB,0x04AD4F,0x0A4D43,0x4D26B7,0x0D254B,0x8D52BF,0x0B5452,0x0B6A47,0x696D3C,0x095B50,//1971-1980
		0x049B45,0x4A4BB9,0x0A4B4D,0xAB25C2,0x06A554,0x06D449,0x6ADA3D,0x0AB651,0x093746,0x5497BB,//1981-1990
		0x04974F,0x064B44,0x36A537,0x0EA54A,0x86B2BF,0x05AC53,0x0AB647,0x5936BC,0x092E50,0x0C9645,//1991-2000
		0x4D4AB8,0x0D4A4C,0x0DA541,0x25AAB6,0x056A49,0x7AADBD,0x025D52,0x092D47,0x5C95BA,0x0A954E,//2001-2010
		0x0B4A43,0x4B5537,0x0AD54A,0x955ABF,0x04BA53,0x0A5B48,0x652BBC,0x052B50,0x0A9345,0x474AB9,//2011-2020
		0x06AA4C,0x0AD541,0x24DAB6,0x04B64A,0x69573D,0x0A4E51,0x0D2646,0x5E933A,0x0D534D,0x05AA43,//2021-2030
		0x36B537,0x096D4B,0xB4AEBF,0x04AD53,0x0A4D48,0x6D25BC,0x0D254F,0x0D5244,0x5DAA38,0x0B5A4C,//2031-2040
		0x056D41,0x24ADB6,0x049B4A,0x7A4BBE,0x0A4B51,0x0AA546,0x5B52BA,0x06D24E,0x0ADA42,0x355B37,//2041-2050
		0x09374B,0x8497C1,0x049753,0x064B48,0x66A53C,0x0EA54F,0x06B244,0x4AB638,0x0AAE4C,0x092E42,//2051-2060
		0x3C9735,0x0C9649,0x7D4ABD,0x0D4A51,0x0DA545,0x55AABA,0x056A4E,0x0A6D43,0x452EB7,0x052D4B,//2061-2070
		0x8A95BF,0x0A9553,0x0B4A47,0x6B553B,0x0AD54F,0x055A45,0x4A5D38,0x0A5B4C,0x052B42,0x3A93B6,//2071-2080
		0x069349,0x7729BD,0x06AA51,0x0AD546,0x54DABA,0x04B64E,0x0A5743,0x452738,0x0D264A,0x8E933E,//2081-2090
		0x0D5252,0x0DAA47,0x66B53B,0x056D4F,0x04AE45,0x4A4EB9,0x0A4D4C,0x0D1541,0x2D92B5          //2091-2099
};
 */
	const unsigned long long CHINESE_JULIAN_DAY[] =
	{
		// This table is automatically generated from LUNAR_TABLE
			// 1901-1-0 is MJD 0x3C18
			// 1951-1-0 is MJD 0x836E
			// 2001-1-0 is MJD 0xCAC5
			// 2051-1-0 is MJD 0x1121B
			// 0xF0000000 = leap month
			// 0x08000000 = 1st month big?
			// 0x00010000 = 12th month big?
			// 0x00008000 = 13th month big?
			// 0x00007FFF = modified julian day of CNY offset
//1901-1910
0x04AE0032,0x0A570194,0x552682F7,0x0D260476,0x0D9505D8,0x46AA873B,0x056A08BB,0x09AD0A1D,0x24AE8B80,0x04AE0D00,
//1911-1920
0x6A4D8E62,0x0A4D0FE2,0x0D251144,0x5D5292A6,0x0B541426,0x0D6A1588,0x296D16EB,0x095B186B,0x749B99CE,0x04971B4E,
//1921-1930
0x0A4B1CB0,0x5B259E12,0x06A51F92,0x06D420F4,0x4ADAA256,0x02B623D7,0x09572539,0x2497A69C,0x0497281C,0x664B297E,
//1931-1940
0x0D4A2AFD,0x0EA52C5F,0x56D4ADC2,0x05AD2F42,0x02B630A5,0x39373207,0x092E3387,0x7C96B4E9,0x0C953669,0x0D4A37CB,
//1941-1950
0x6DA5392D,0x0B553AAD,0x056A3C10,0x4AADBD72,0x025D3EF3,0x092D4055,0x2C95C1B7,0x0A954337,0x7B4AC499,0x06CA4619,
//1951-1960
0x0B550025,0x555A8188,0x04DA0308,0x0A5B046A,0x352B85CD,0x052B074D,0x8A9508AF,0x0E950A2E,0x06AA0B91,0x6AD50CF3,
//1961-1970
0x0AB50E73,0x04B60FD6,0x4A571138,0x0A5712B8,0x0526141B,0x3E93157C,0x0D9516FC,0x75AA985F,0x056A19DF,0x096D1B41,
//1971-1980
0x54AE9CA4,0x04AD1E24,0x0A4D1F86,0x4D26A0E8,0x0D252268,0x8D52A3CA,0x0B54254A,0x0B6A26AC,0x696D280F,0x095B298F,
//1981-1990
0x049B2AF2,0x4A4BAC54,0x0A4B2DD4,0xAB25AF36,0x06A530B6,0x06D43218,0x6ADA337A,0x0AB634FA,0x0937365D,0x5497B7C0,
//1991-2000
0x04973940,0x064B3AA2,0x36A53C04,0x0EA53D83,0x86B2BEE6,0x05AC4066,0x0AB641C8,0x5936C32B,0x092E44AB,0x0C96460D,
//2001-2010
0x4D4A8018,0x0D4A0198,0x0DA502FA,0x25AA845D,0x056A05DD,0x7AAD873F,0x025D08C0,0x092D0A22,0x5C958B84,0x0A950D04,
//2011-2020
0x0B4A0E66,0x4B550FC8,0x0AD51148,0x955A92AB,0x04BA142B,0x0A5B158D,0x652B96F0,0x052B1870,0x0A9319D2,0x474A9B34,
//2021-2030
0x06AA1CB4,0x0AD51E16,0x24DA9F79,0x04B620F9,0x6957225B,0x0A4E23DB,0x0D26253D,0x5E93269F,0x0D53281F,0x05AA2982,
//2031-2040
0x36B52AE4,0x096D2C64,0xB4AEADC7,0x04AD2F47,0x0A4D30A9,0x6D25B20B,0x0D25338B,0x0D5234ED,0x5DAA364F,0x0B5A37CF,
//2041-2050
0x056D3932,0x24ADBA95,0x049B3C15,0x7A4BBD77,0x0A4B3EF7,0x0AA54059,0x5B52C1BB,0x06D2433B,0x0ADA449D,0x355B4600,
//2051-2060
0x0937002A,0x8497818D,0x0497030D,0x064B046F,0x66A505D1,0x0EA50750,0x06B208B3,0x4AB60A15,0x0AAE0B95,0x092E0CF8,
//2061-2070
0x3C970E5A,0x0C960FDA,0x7D4A913C,0x0D4A12BC,0x0DA5141E,0x55AA9581,0x056A1701,0x0A6D1863,0x452E99C6,0x052D1B46,
//2071-2080
0x8A959CA8,0x0A951E28,0x0B4A1F8A,0x6B5520EC,0x0AD5226C,0x055A23CF,0x4A5D2531,0x0A5B26B1,0x052B2814,0x3A93A976,
//2081-2090
0x06932AF6,0x7729AC58,0x06AA2DD8,0x0AD52F3A,0x54DAB09D,0x04B6321D,0x0A57337F,0x452734E2,0x0D263661,0x8E9337C3,
//2091-2100
0x0D523943,0x0DAA3AA5,0x66B53C08,0x056D3D88,0x04AE3EEB,0x4A4EC04D,0x0A4D41CD,0x0D15432F,0x2D92C491
	};

	const unsigned int doyOffset[][2]=
	{
		{0,0},					// January
		{31,31},				// February
		{59,60},				// March
		{90,91},				// April
		{120,121},				// May
		{151,152},				// June
		{181,182},				// July
		{212,213},				// August
		{243,244},				// September
		{273,274},				// October
		{304,305},				// November
		{334,335},				// December
		{365,366}				// next January - for convenience
	};

/**
 * @brief safer way to access table
 * @param year Gregorian year (between 1901 and 2099 inclusive)
 * @return table element
 */
	inline
	unsigned long long
	LunarCalendarTable(int year)
	{
#ifdef DEBUG
		MY_ERR << "inline anonymous_Date_namespace::LunarCalendarTable("
			   << year << ") called."
			   << std::endl;
#endif
		if ((year>1900) && (year < 2100)) {
			return CHINESE_JULIAN_DAY[year-1901];
		}
		else {
			throw INVALID_PARAM(year);
		}
	}

		/**
		 * @brief compute modified julian day number for Gregorian date
		 *
		 * MJD 0 starts at Gregorian 1858-11-17 0h (Wednesday).
		 *
		 * algorithm as follows
		 @verbatim
		 a = (14-month)/12
		 y = year+4800-a
		 m = month + 12*a - 3

		 For a date in the Gregorian calendar:
		 MJD = day + (153*m+2)/5 + y*365 + y/4 - y/100 + y/400 - 2432046
		 @endverbatim
		 *
		 * @param year Gregorian year of date
		 * @param month Gregorian month of date
		 * @param day Gregorian day of date
		 *
		 * @return julian day number
		 */
	inline
	int
	mjdFromGregorian(int year, unsigned int month, unsigned int day)
	{
		unsigned int a=(14-month)/12;
		unsigned int y=year+4800-a;
		unsigned int m=month+12*a-3;
		int jd=day+(153*m+2)/5+y*365+y/4-y/100+y/400-32045;
		return jd-2400001;
	}

		/**
		 * @brief compute modified julian day number for Julian date
		 *
		 * JD 0 starts at noon 1 January 4713BC in the Julian calendar.
		 * MJD = JD - 2400000.5
		 *
		 * algorithm as follows
		 @verbatim
		 a = (14-month)/12
		 y = year+4800-a
		 m = month + 12*a - 3

		 JD = day + (153*m+2)/5 + y*365 + y/4 - 32083
		 @endverbatim
		 *
		 * @param year Julian calendar year of date
		 * @param month Julian calendar month of date
		 * @param day Julian calendar day of date
		 *
		 * @return modified julian day number
		 */
	inline
	int
	mjdFromJulian(int year, unsigned int month, unsigned int day)
	{
		unsigned int a=(14-month)/12;
		unsigned int y=year+4800-a;
		unsigned int m=month+12*a-3;
		int jd=day+(153*m+2)/5+y*365+y/4-32083;
		return jd-2400001;
	}

		/**
		 * @brief lookup/compute modified julian day number for Chinese date
		 *
		 * @param year  Chinese calendar year of date
		 * @param month Chinese calendar month of date
		 * @param day   Chinese calendar day of date
		 *
		 * @return modified julian day number
		 */
	inline
	int
	mjdFromChinese(int year, unsigned int month, unsigned int day)
	{
			// first look up the table for that chinese year
		unsigned long long c=LunarCalendarTable(year-CHINESE_CALENDAR_BEGIN);
			// cny from offset
		int res=(c & 0x7FFF);
			// check leap
		if ((c>>28) && month>(c>>28)) {
			month++;
			if(month>16) month-=16;
		}
			// adding offset
		if (year>2050) {
			res+=0x1121B;
		}
		else if (year>2000) {
			res+=0xCAC5;
		}
		else if (year>1950) {
			res+=0x836E;
		}
		else {
			res+=0x3C18;
		}
			// add months
		for (unsigned int j=1; j<month; j++) {
			res+=(c & ((1<<28)>>j))? 30 : 29;
		}
		return res+day-1;
	}

		// julian day to gregorian
/**
 * @brief compute julian day number for date at noon
 *
 * JD 0 starts at noon 1 January 4713BC in the Julian calendar.
 *
 * algorithm as follows
 *
@verbatim
     a = JD + 32044
     b = (4*a+3)/146097
     c = a - (b*146097)/4

     d = (4*c+3)/1461
     e = c - (1461*d)/4
     m = (5*e+2)/153
  
     day   = e - (153*m+2)/5 + 1
     month = m + 3 - 12*(m/10)
     year  = b*100 + d - 4800 + m/10
@endverbatim
 *
 * @param jd Julian Day
 * @param dp enum indicating the requested part (only DATEPART_GREGORIAN_* is supposed to be here).
 *
 * @return Gregorian date part requested
 */
	int gregorianFromJD(int jd, enum Date::DatePart dp)
	{
		static int _myjd,_myYear,_myMonth,_myDay,res;
		if (_myjd!=jd) {
			int a = jd + 32044,
				b = (4*a+3)/146097,
				c = a - (b*146097)/4,
				d = (4*c+3)/1461,
				e = c - (1461*d)/4,
				m = (5*e+2)/153;
			_myDay = e - (153*m+2)/5 + 1;
			_myMonth = m + 3 - 12*(m/10);
			_myYear  = b*100 + d - 4800 + m/10;
			_myjd=jd;
		}
		switch(dp){
			case Date::DATEPART_GREGORIAN_YEAR :
				res=_myYear;
				break;
			case Date::DATEPART_GREGORIAN_MONTH :
				res=_myMonth;
				break;
			case Date::DATEPART_GREGORIAN_DAY :
				res = _myDay;
				break;
			default:
					// shouldn't be here
				res=0;
		};
		return res;
	}

/**
 * @brief compute julian day number for date at noon
 *
 * JD 0 starts at noon 1 January 4713BC in the Julian calendar.
 *
 * algorithm as follows
@verbatim
     b = 0
     c = JD + 32082

     d = (4*c+3)/1461
     e = c - (1461*d)/4
     m = (5*e+2)/153
  
     day   = e - (153*m+2)/5 + 1
     month = m + 3 - 12*(m/10)
     year  = b*100 + d - 4800 + m/10
@endverbatim
 *
 * @param jd Julian Day
 * @param dp enum indicating the requested part (only DATEPART_JULIAN_* is supposed to be here).
 *
 * @return requested Julian calendar part
 */
	int julianFromJD(int jd, enum Date::DatePart dp)
	{
		static int _myjd,_myYear,_myMonth,_myDay,res;
		if (_myjd!=jd) {
			int b = 0,
				c = jd + 32082,
				d = (4*c+3)/1461,
				e = c - (1461*d)/4,
				m = (5*e+2)/153;
			_myDay = e - (153*m+2)/5 + 1;
			_myMonth = m + 3 - 12*(m/10);
			_myYear  = b*100 + d - 4800 + m/10;
			_myjd=jd;
		}
		switch(dp){
			case Date::DATEPART_JULIAN_YEAR :
				res=_myYear;
				break;
			case Date::DATEPART_JULIAN_MONTH :
				res=_myMonth;
				break;
			case Date::DATEPART_JULIAN_DAY :
				res = _myDay;
				break;
			default:
					// shouldn't be here
				res=0;
		};
		return res;
	}

/**
 * @brief compute chinese day and return
 *
 * The intermediary result is cached, so it should help performance.
 *
 * @param jd Julian Day number
 * @param dp the requested part of chinese date (DATEPART_CHINESE_*)
 *
 * @return requested part of Chinese date.
 */
	int
	chineseFromJD(int jd,enum Date::DatePart const dp)
	{
#ifdef DEBUG
		MY_ERR << "chineseFromJD(" << jd << "," << dp << ") called." << std::endl;
#endif	// DEBUG
		static int cyear,cmonth,cday,myjd;
		int gyear;
		if ((jd<LUNAR_TABLE_START_MJD+2400001) ||
			(jd>LUNAR_TABLE_END_MJD+2400001)) {
			throw INVALID_PARAM(jd);
		}
		while (jd != myjd) {
#ifdef DEBUG
			MY_ERR << "\tDate changed since last computed." << std::endl;
#endif	// DEBUG
			gyear=gregorianFromJD(jd,Date::DATEPART_GREGORIAN_YEAR);
			cday=((jd-GREGORIAN_ZERO_JULIAN_OFFSET)   // days from 1AD-Jan-0
				  % DAYS_IN_FOUR_CENTURY);				// extract 4 centuries
			unsigned long long cCal = LunarCalendarTable(gyear);
			if ((cCal & 0x7FFF)> (cday % DAYS_IN_HALF_CENTURY)) {
#ifdef DEBUG
			MY_ERR << "\tDate is before CNY, getting previous year." << std::endl;
#endif	// DEBUG
					// day before CNY of Gregorian year
				gyear--;
				cCal=LunarCalendarTable(gyear);
			}
			cyear = gyear + CHINESE_CALENDAR_BEGIN;
			cday -= (cCal & 0x7FFF); // day from CNY, starting 0 at CNY
#ifdef DEBUG
			MY_ERR << "\t\tDate is " << cday << " days from CNY of year" << std::endl;
#endif	// DEBUG

				// now compute the month and day
			int leap = cCal >> 28;
				// now keep subtracting enough 29/30 days until we get there.
			cmonth=1;
			int curmonthlen=(cCal & ((1<<28)>>cmonth))?30:29;
			while ((cday>=curmonthlen) && (cmonth<(leap?13:12))) {
				cday-=curmonthlen;
				cmonth++;
				curmonthlen=(cCal & ((1<<28)>>cmonth))?30:29;
			}
			if((leap>0) && (cmonth>leap)){
				cmonth--;
				if (cmonth==leap) cmonth+=16;
			}
			cday++;
			myjd = jd;
		}
#ifdef DEBUG
			MY_ERR << "\tComputation finished.  Deciding what to return." << std::endl;
#endif	// DEBUG
		switch(dp){
			case Date::DATEPART_CHINESE_YEAR  : return cyear;
			case Date::DATEPART_CHINESE_MONTH : return cmonth;
			case Date::DATEPART_CHINESE_DAY   : return cday;
			default :
					//shouldn't be here
#ifdef DEBUG
				MY_ERR << "\tError: requested date part is not handled by this method!" << std::endl;
#endif	// DEBUG
				throw INVALID_PARAM(dp);
		}
	}

}

/**
 * @relatesalso Date
 * @brief compute Gregorian date from Modified Julian Date
 * @param mjd Modified Julian Date
 * @param dp part (DATEPART_GREGORIAN_*)
 * @return requested calendar part
 */
int
gregorianFromMJD(int mjd, enum Date::DatePart dp)
{
	return gregorianFromJD(mjd+2400001,dp);
}

/**
 * @relatesalso Date
 * @brief compute Julian date from Modified Julian Date
 * @param mjd Modified Julian Date
 * @param dp part (DATEPART_JULIAN_*)
 * @return requested calendar part
 */
int
julianFromMJD(int mjd, enum Date::DatePart dp)
{
	return julianFromJD(mjd+2400001,dp);
}

/**
 * @brief constructor
 *
 * @param d (Modified) Julian Day (default: MJD 55941 = Gregorian 2012-1-1)
 * @param t Calendar to which day are measured MJD (default) or JD)
 */
Date::Date(int d,
		   enum CalendarType t)
{
	switch(t){
		case CALTYPE_MJD :
			_mjd = d;
			break;
		case CALTYPE_JD :
			_mjd = d-2400001;
			break;
		default :
				// shouldn't be here
			throw INVALID_PARAM(t);
	}
}

/**
 * @brief another constructor
 *
 * @param year  year of desired date
 * @param month month of desired date
 * @param day   day of desired date
 * @param t     Calendar to which year, month and day are measured
 * (currently supported Gregorian, Julian, and Chinese (experimental).)
 */
Date::Date(int year,
		   unsigned int month,
		   unsigned int day,
		   enum CalendarType t)
{
	switch(t){
		case CALTYPE_GREGORIAN :
			_mjd = mjdFromGregorian(year,month,day);
			break;
		case CALTYPE_JULIAN :
			_mjd = mjdFromJulian(year,month,day);
			break;
		case CALTYPE_CHINESE :
			_mjd = mjdFromChinese(year,month,day);
			break;
		default :
			throw INVALID_PARAM(t);
	}
}

/**
 * @brief ISO day of week of this date object
 *
 * Epoch of modified Julian date is 1858-11-17 0h (Wednesday).
 * So day of week is (MJD+3) mod 7.
 *
 * This function takes no argument.
 *
 * @return ISO day of week
 */
enum Date::DayOfWeek
Date::getDayOfWeek() const
{
	return Date::DayOfWeek((_mjd+3)%7);
}


/**
 * @relatesalso Date
 * @brief look up the number of days in a Gregorian month
 *
 * @param year Gregorian year
 * @param month Gregorian month
 *
 * @return the number of days in the given month
 */
unsigned int
daysInMonth(int year, unsigned int month)
{
#ifdef DEBUG
	MY_ERR << "daysInMonth(" << year <<  "," << month << ") called." << std::endl;
#endif	// DEBUG
	switch (month) {
		case 1: case 3: case 5: case 7: case 8: case 10: case 12:
			return 31;
		case 2:
				// can't use Date(year,2,xxx) otherwise infinite loop.
			return (isLeapYear(year)?29:28);
		case 4: case 6: case 9: case 11:
			return 30;
		default:
		// shouldn't get here --- month is not a valid month, throwing
			throw INVALID_PARAM(month);
	}
}

/**
 * @brief helper function to decide a year is leap in Gregorian calendar.
 *
 * May rename this later to isGregorianLeapYear, if I decide to implement
 * Julian calendar too, but since Julian leap year is easy it probably won't
 * happen.
 *
 * @param y Gregorian year
 *
 * @retval true if y is a leap year in Gregorian calendar
 * @retval false if y is not a leap year in Gregorian calendar
 */
bool
isLeapYear(int const y)
{
#ifdef DEBUG
	MY_ERR << "isLeapYear(" << y << ") called." << std::endl;
#endif	// DEBUG
	if (y % 4) return false;
	if (y % 100) return true;
	if (y % 400) return false;
	return true;
}

/**
 * @brief get the Chinese lunar calendar data for a given Gregorian year
 *
 * The information is encoded as:
 * 
\verbatim
	(value & 0xF0000000) >> 28 = intercalary month
	(value & 0x08000000) >> 27 = 1st month has 30 days?
	(value & 0x04000000) >> 26 = 2nd month has 30 days?
	...
	(value & 0x00008000) >> 15 = 13th month has 30 days?
	(value & 0x00007FFF)       = modified julian day of CNY from
			  					 gregorian NY of (year round down to 1 mod 50)
\endverbatim
 *
 * @param year Gregorian year
 *
 * @return requested data
 */
unsigned long long
Date::getLunarCalendarData(unsigned int year)
{
#ifdef DEBUG
	MY_ERR << "Date::getLunarCalendarData(" << year << ") called." << std::endl;
#endif	// DEBUG
	return LunarCalendarTable(year);
}

/**
 * @brief get the Chinese calendar table for this date's Gregorian year
 *
 * Should consider inlining this function.  Provided for convenience
 *
 * This function takes no argument.
 *
 * @return data requested.
 * @sa getLunarCalendarData(unsigned int)
 */
unsigned long long
Date::getLunarCalendarData() const{
#ifdef DEBUG
	MY_ERR << this << "->Date::getLunarCalendarData() called." << std::endl;
#endif	// DEBUG
	return LunarCalendarTable(getGregorianYear());
}

/**
 * @brief compute number of days from Gregorian new year
 *
 * Convention: Gregorian new year would be the first day, etc.
 *
 * This function takes no arguments.
 *
 * @return day of year
 */
unsigned int
Date::getDayOfYear() const
{
	unsigned int d = (((_mjd+2400000-GREGORIAN_ZERO_JULIAN_OFFSET)
					  % DAYS_IN_FOUR_CENTURY)
					 % DAYS_IN_HALF_CENTURY)
		% DAYS_IN_FOUR_YEAR;
	int	i=3;
	while (i>0) {
		if (d>=365) d-=365;
		i--;
	}
	return d+1;
}

/**
 * @relatesalso Date
 * @brief compute Chinese date from Modified Julian Date
 * @param mjd Modified Julian Date
 * @param dp part (DATEPART_CHINESE_*)
 * @return requested calendar part
 */
int
chineseFromMJD(int mjd, enum Date::DatePart dp)
{
	return chineseFromJD(mjd+2400001,dp);
}
