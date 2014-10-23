/**
 * @file calendar.cc
 *
 * Time-stamp: <2012-05-05 17:01:12 +0800 by kerwin>
 *
 * Yearly calendar in 12 month-per-view LaTeX pages
 *
 * @author kerwin\@localhost
 */

#include "include/debug.h"
#include "include/calendar.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
#include <sstream>


// static initialisation
const char* const Calendar::_nSolarTermName[] = {
	"小寒","大寒","立春","雨水","驚蟄","春分",
	"清明","穀雨","立夏","小滿","芒種","夏至",
	"小暑","大暑","立秋","處暑","白露","秋分",
	"寒露","霜降","立冬","小雪","大雪","冬至"
};
const char* const Calendar::_nChineseDayName[] = {
	"*",
	"初一","初二","初三","初四","初五",
	"初六","初七","初八","初九","初十",
	"十一","十二","十三","十四","十五",
	"十六","十七","十八","十九","二十",
	"廿一","廿二","廿三","廿四","廿五",
	"廿六","廿七","廿八","廿九","三十"
};
const char* const Calendar::_nChineseMonthName[] = {
	"*",
	"正月","二月","三月","四月","五月","六月",
	"七月","八月","九月","十月","十一月","臘月","*","*","*","*",
	"閏正月","閏二月","閏三月","閏四月","閏五月","閏六月",
	"閏七月","閏八月","閏九月","閏十月","閏十一月","閏臘月"
};
const char* const Calendar::_nGregorianLongName[] = {
	"December",
	"January", "February", "March", "April", "May", "June",
	"July", "August", "September", "October", "November", "December",
	"January"
};
const char* const Calendar::_nGregorianShortName[] = {
	"lastdec",
	"thisjan", "thisfeb", "thismar", "thisapr", "thismay", "thisjun",
	"thisjul", "thisaug", "thissep", "thisoct", "thisnov", "thisdec",
	"nextjan"
};
const char* const Calendar::_nCellType[]={
	"\\caldate", "\\calsatd", "\\calpubd"
};

const char* const Calendar::_nDayOfWeekHeading[]={
	"\\myday{\\holcol SUN}%%",
	"\\myday{MON}%%",
	"\\myday{TUE}%%",
	"\\myday{WED}%%",
	"\\myday{THU}%%",
	"\\myday{FRI}%%",
	"\\myday{\\satcol SAT}%%"
};

// useful constants, hiding
namespace {
	const std::string LATEX_NEWLINE="\\\\";
	const std::string LATEX_CJK_BEGIN="\\cjktext{";
	const char LATEX_CJK_END='}';
};

// dynamic initialisation
/**
 * @brief force default initialisation of public holiday and solar terms list
 *
 * This function takes no argument.
 *
 * @return true if initialisation is successful.
 */
bool
Calendar::forcedInitialise()
{
#ifdef DEBUG
	MY_ERR << this << "->Calendar::forcedInitialised() called."
		   << std::endl;
#endif
	std::ifstream isolar("solar.dat");
	std::ifstream ipubhol("pubhol.dat");

	setList(isolar,ipubhol);

	if(updateList()) return true;

	return false;
}

// other functions

/**
 * @brief set the solar terms and holidays according to the file streams
 *
 * This method will call the updateList() method to update the hash.
 *
 * @param solar input file stream for solar terms data
 * @param holiday input file stream for holiday data
 *
 * @return true if initialisation is successful.
 */
bool
Calendar::setList(std::ifstream& solar, std::ifstream& holiday)
{
#ifdef DEBUG
	MY_ERR << this << "->Calendar::setList((ifstream*)" << &solar
		   << ", (ifstream*)" << &holiday << ") called."
		   << std::endl;
#endif
	initialised=false;

	// block 1 --- read solar
	setSolar(solar);

	// block 2 --- read public holiday
	setPublicHoliday(holiday);

	// merge
	if(updateList()) initialised=true;

	return true;
}

/**
 * @brief Update the hashes of solar terms/chinese date/public holiday
 *
 * This method takes no argument.
 *
 * @return true if initialisation is successful.
 */
bool
Calendar::updateList()
{
#ifdef DEBUG
	MY_ERR << this << "->Calendar::updateList() called."
		   << std::endl;
#endif
	// maybe split to generate
	generateChineseSolar();
	generateSolarPublicHoliday();
	// if split merge back
	return true;
}

/**
 * @brief check if a date is on the public holiday list
 *
 * @param d the date we want to check
 *
 * @retval true date is a holiday
 * @retval false date is not a holiday
 */
bool
Calendar::isPublicHoliday(Date const& d) const
{
#ifdef DEBUG
	MY_ERR << this << "->Calendar::isPublicHoliday(Date(" << d.getYear() << ","
		   << d.getMonth() << "," << d.getDay() << ")) called."
		   << std::endl;
#endif
	return (_nPublicHoliday.count(d));
}

/**
 * @brief check if a date is a solar term
 *
 * @param d date to check
 *
 * @retval true if d is a solar term
 * @retval false if d is not a solar term
 */
bool
Calendar::isSolar(Date const& d) const
{
#ifdef DEBUG
	MY_ERR << this << "->Calendar::isSolar(Date(" << d.getYear() << ","
		   << d.getMonth() << "," << d.getDay() << ")) called."
		   << std::endl;
#endif
	return (_nSolar.count(d));
}

/**
 * @brief populate the Chinese-SolarTerm hash
 *
 * This function takes no argument
 *
 * This function has no return value
 */
void
Calendar::generateChineseSolar()
{
#ifdef DEBUG
	MY_ERR << this << "->Calendar::generateChineseSolar() called."
		   << std::endl;
#endif
		//first clear everything
	if ( ! _nChineseSolar.empty() ) {
#ifdef DEBUG
	MY_ERR << "\t_nChineseSolar not empty, cleaning."
		   << std::endl;
#endif
		_nChineseSolar.clear();
	}
	else {
#ifdef DEBUG
	MY_ERR << "\t_nChineseSolar is empty.  Proceed."
		   << std::endl;
#endif
	}
	Date dStart(_year,1,1);
	Date dEnd(_year+1,1,1);
	for (Date d=dStart; d<dEnd; d++) {
#ifdef DEBUG
		MY_ERR << "\t Testing " << d.getYear() << "-" << d.getMonth() << "-"
			   << d.getDay() << " for Chinese/Solar."
			   << std::endl;
#endif
		std::string res;
		if (isSolar(d)) {
#ifdef DEBUG
		MY_ERR << "\t\t Date is Solar, add solar name "
			   << std::endl;
#endif
			res = getSolarName(d);
		}
		else if (d.getDay()==1) {
#ifdef DEBUG
		MY_ERR << "\t\t Date is first day of Gregorian Month, add both chinese month and day "
			   << std::endl;
#endif
			res = _nChineseMonthName[d.getChineseMonth()];
			res+= _nChineseDayName[d.getChineseDay()];
		}
		else if (d.getChineseDay()==1) {
#ifdef DEBUG
		MY_ERR << "\t\t Date is first day of Chinese Month, just add chinese momonth "
			   << std::endl;
#endif
			res = _nChineseMonthName[d.getChineseMonth()];
		}
		else {
#ifdef DEBUG
		MY_ERR << "\t\t Date is neither, just add chinese day "
			   << std::endl;
#endif
			res = _nChineseDayName[d.getChineseDay()];
		}
		_nChineseSolar[d]=res;
	}
}

/**
 * @brief populate the SolarTerm-Holiday hash
 *
 * This function takes no argument
 *
 * This function has no return value
 */
void
Calendar::generateSolarPublicHoliday()
{
#ifdef DEBUG
	MY_ERR << this << "->Calendar::generateSolarPublicHoliday() called."
		   << std::endl;
#endif
		//first clear everything
	if ( ! _nSolarPublicHoliday.empty() ) _nSolarPublicHoliday.clear();
	for (Date d(_year,1,1); d<Date(_year+1,1,1); d++) {
		std::string res;
		bool s=isSolar(d), p=isPublicHoliday(d);
			// solar term first
		if (s) {
			res = getSolarTime(d);
			if (p) {
				res += LATEX_NEWLINE + LATEX_CJK_BEGIN +
					getPublicHolidayName(d) + LATEX_CJK_END;
			}
		}
		else if (p) {
			res = LATEX_CJK_BEGIN + getPublicHolidayName(d) + LATEX_CJK_END;
		}
		if (s || p) _nSolarPublicHoliday[d]=res;
	}
}

/**
 * @brief extracts the solar term time for a date from hash
 *
 * Program will crash if the date is not in hash.  Use with care.
 *
 * @param d a date that should be solar term
 *
 * @return constant string of solar term time
 */
std::string const&
Calendar::getSolarTime(Date const& d) const
{
#ifdef DEBUG
	MY_ERR << this << "->Calendar::getSolarTime(Date(" << d.getYear()
		   << "," << d.getMonth() << "," << d.getDay() << ")) called."
		   << std::endl;
#endif
	std::map<Date,std::string>::const_iterator i=_nSolar.find(d);
	if(i == _nSolar.end()){
		throw Exception("Invalid Parameter d --- is not a solar term date");
	}
	return (i->second);
}

/**
 * @brief extracts the public holiday description for a date from hash
 *
 * Program will crash if the date is not in hash.  Use with care.
 *
 * @param d a date that should be a holiday
 *
 * @return const string of public holiday description.
 */
std::string const&
Calendar::getPublicHolidayName(Date const& d) const
{
#ifdef DEBUG
	MY_ERR << this << "->Calendar::getPublicHolidayName(Date(" << d.getYear()
		   << "," << d.getMonth() << "," << d.getDay() << ")) called."
		   << std::endl;
#endif
	std::map<Date,std::string>::const_iterator i=_nPublicHoliday.find(d);
	if(i == _nPublicHoliday.end() ) {
		throw Exception("Invalid Parameter d --- is not a public holiday");
	}
	return (i->second);
}


/**
 * @brief extracts the solar term name for a date from hash
 *
 * Program will crash if the date is not in hash.  Use with care.
 *
 * @param d a date that should be a holiday
 *
 * @return const string of solar term name.
 */
std::string
Calendar::getSolarName(Date const& d) const
{
#ifdef DEBUG
	MY_ERR << this << "->Calendar::getSolarName(Date(" << d.getYear()
		   << "," << d.getMonth() << "," << d.getDay() << ")) called."
		   << std::endl;
#endif
	if(!isSolar(d)){
		throw Exception("Invalid parameter d --- is not a solar term date");
	}
	return _nSolarTermName[2*(d.getMonth()-1)+(d.getDay()>>4)];
}

/**
 * @brief set the solar hash according to file
 *
 * @param[in] file plain text in file stream, format in CSV, per line:
 * year,month,day,hour,minute
 *
 * @retval true if successfully set
 * @retval false should never happen
 */
bool
Calendar::setSolar(std::ifstream& file)
{
#ifdef DEBUG
	MY_ERR << this << "->Calendar::setSolar((ifstream*)" << &file
		   << ")) called." << std::endl;
#endif
	unsigned int year,month,day,hour,minute;
	char comma;
	
	_nSolar.clear();
	std::string s;
	while (file >> s) {
#ifdef DEBUG
		MY_ERR << "\tCalendar::setSolar(): read line" << std::endl
			   << "\t\t" << s << std::endl << "\tfrom (ifstream*)" << &file
			   << std::endl;
#endif

		std::istringstream ist (s);
		ist >> year >> comma
			>> month >> comma
			>> day >> comma
			>> hour >> comma
			>> minute;
		std::ostringstream ost;
		ost << std::setw(2) << std::setfill('0') << hour << ":"
			<< std::setw(2) << std::setfill('0') << minute;
#ifdef DEBUG
		MY_ERR << "\t adding _nSolar[Date(" << year << "," << month << ","
			   << day << ")]=" << '"' << ost.str() << '"'
			   << std::endl;
#endif
		_nSolar[Date(year,month,day)]=ost.str();
	}

	return true;
}

/**
 * @brief set the holiday hash according to file
 *
 * @param[in] file plain text in file stream, format in CSV, per line:
 * year,month,day,description
 *
 * @retval true if successfully set
 * @retval false should never happen
 */
bool
Calendar::setPublicHoliday(std::ifstream& file)
{
#ifdef DEBUG
	MY_ERR << this << "->Calendar::setPublicHoliday((ifstream*)" << &file
		   << ")) called." << std::endl;
#endif
	unsigned int year,month,day;
	char comma;
	std::string desc;

	_nPublicHoliday.clear();
	std::string s;
	while (file >> s) {
		std::istringstream ist (s);
		ist >> year >> comma
			>> month >> comma
			>> day >> comma
			>> desc;
#ifdef DEBUG
		MY_ERR << "\t adding _nPublicHoliday[Date(" << year << "," << month
			   << "," << day << ")]=" << '"' << desc << '"'
			   << std::endl;
#endif
		_nPublicHoliday[Date(year,month,day)]=desc;
	}

	return true;
}


/**
 * @brief get an empty cell TeX code.
 *
 * This function takes no argument.
 *
 * @return TeX formatting code string for an empty calendar cell
 */
std::string&
Calendar::emptyCellTex() const
{
#ifdef DEBUG
	MY_ERR << this << "->Calendar::emptyCellTex() called" << std::endl;
#endif
	static std::string res=std::string(_nCellType[0])+"{}{}{}{}{}{}%";
	return res;
}

/**
 * @brief get an day cell TeX code.
 *
 * @param d date of the day cell
 *
 * @return string object containing TeX formatting code for the day-cell,
 * which looks like
 *    \verbatim\caldate{January}{4}{十一}{}{4/362}{}%\endverbatim
 * for a normal day
 *    \verbatim\calpubd{January}{1}{十二月初八}{\cjktext{元旦}}{1/365}{}%\endverbatim
 * for public holiday
 *    \verbatim\calsatd{January}{21}{大寒}{00:09}{21/345}{}%\endverbatim
 * for other saturdays
 */
std::string
Calendar::dayCellTex(Date const& d) const
{
#ifdef DEBUG
	MY_ERR << this << "->Calendar::dayCellTex(Date(" << d.getYear() << ","
		   << d.getMonth() << "," << d.getDay() << ")) called"
		   << std::endl;
#endif
	std::ostringstream out;
	int index=0;
	switch(d.getDayOfWeek()){
		case Date::DOW_SATURDAY:
			index=1;
			break;
		case Date::DOW_SUNDAY:
			index=2;
			break;
		default:
			;
	}
	if (isPublicHoliday(d)) index=2;
	out << _nCellType[index];
	out << "{" << _nGregorianLongName[d.getMonth()] << "}"; // #1
#ifdef DEBUG
	MY_ERR << "\tCalendar::dayCellTex(): argument #1 generated without errors."
		   << std::endl;
#endif
	out << "{" << d.getDay() << "}";						// #2
#ifdef DEBUG
	MY_ERR << "\tCalendar::dayCellTex(): argument #2 generated without errors."
		   << std::endl;
#endif
	out << "{" << _nChineseSolar.find(d)->second << "}";					// #3
#ifdef DEBUG
	MY_ERR << "\tCalendar::dayCellTex(): argument #3 generated without errors."
		   << std::endl;
#endif
	out << "{";
	std::map<Date,std::string>::const_iterator i=_nSolarPublicHoliday.find(d);
	if ( i != _nSolarPublicHoliday.end()) {
		out << i->second;
	}
	out << "}"; // #4
#ifdef DEBUG
	MY_ERR << "\tCalendar::dayCellTex(): argument #4 generated without errors."
		   << std::endl;
#endif
	out << "{";					// #5
	int dd=d.getDayOfYear();
	out << dd << "/" << (d.isLeap()?366:365)-dd << "}";
#ifdef DEBUG
	MY_ERR << "\tCalendar::dayCellTex(): argument #5 generated without errors."
		   << std::endl;
#endif
	out << "{}%";				// #6
#ifdef DEBUG
	MY_ERR << "\tCalendar::dayCellTex(): argument #6 generated without errors."
		   << std::endl;
#endif

	return out.str();
}




/**
 * @brief get TeX preamble, begin document, until when the first month starts
 *
 * This function takes no argument.
 *
 * @return string object containing TeX formatting code
 */
std::string const&
Calendar::getTexPreamble() const
{
#ifdef DEBUG
	MY_ERR << this << "Calendar::getTexPreamble() called."
		   << std::endl;
#endif
	static std::string preamble;
	std::ostringstream out;

		//usual preamble stuff
	out << "\\documentclass[12pt]{article}" << std::endl
		<< "\\usepackage{color}" << std::endl
		<< "\\usepackage[math]{iwona}" << std::endl
		<< "\\usepackage{fix-cm}" << std::endl
		<< "\\usepackage{CJKutf8}" << std::endl
		<< "\\usepackage[landscape,pdftex,a4paper]{geometry}" << std::endl
		<< "%\\usepackage{garamond}" << std::endl
		<< "\\newenvironment{TChinese}{%" << std::endl
		<< "  \\CJKfamily{bsmi}%" << std::endl
		<< "  \\CJKtilde" << std::endl
		<< "  \\CJKnospace}{}" << std::endl
		<< "%\\usepackage[encapsulated]{CJK}" << std::endl
		<< "%\\usepackage{ucs}" << std::endl
		<< "%\\usepackage[utf8x]{inputenc}" << std::endl
		<< "\\newcommand{\\cjktext}[1]{\\begin{CJK}{UTF8}{bsmi}#1\\end{CJK}}" << std::endl
		<< "%\\usepackage{mathpazo}" << std::endl;
		// colour definition
	out << "\\definecolor{Red}{rgb}{1.0,0.0,0.0}" << std::endl
		<< "\\definecolor{Green}{rgb}{0.0,0.7,0.0}" << std::endl
		<< std::endl
		<< "\\newcommand{\\holcol}{\\color{Red}}" << std::endl
		<< "\\newcommand{\\satcol}{\\color{Green}}" << std::endl;
	out << std::endl;

		// lengths related
	out << "\\hbadness 20000" << std::endl
		<< "\\hfuzz=1000pt" << std::endl
		<< "\\vbadness 20000" << std::endl
		<< "\\lineskip 0pt" << std::endl
		<< "\\marginparwidth 0pt" << std::endl
		<< "\\oddsidemargin  -1cm" << std::endl
		<< "\\evensidemargin -1cm" << std::endl
		<< "\\marginparsep   0pt" << std::endl
		<< "\\topmargin      0pt" << std::endl
		<< "\\textwidth      7.5in" << std::endl
		<< "\\textheight     9.5in" << std::endl
		<< "\\newlength{\\cellwidth}" << std::endl
		<< "\\newlength{\\cellheight}" << std::endl
		<< "\\newlength{\\boxwidth}" << std::endl
		<< "\\newlength{\\boxheight}" << std::endl
		<< "\\newlength{\\cellsize}" << std::endl
		<< "\\newcommand{\\myday}[1]{}" << std::endl
		<< "\\newcommand{\\caldate}[6]{}" << std::endl
		<< "\\newcommand{\\nocaldate}[6]{}" << std::endl
		<< "\\newcommand{\\calsmall}[6]{}" << std::endl;

		// paper format
	out << "%" << std::endl
		<< "\\special{landscape}{}% " << std::endl
		<< "\\textwidth 9.5in{}% " << std::endl
		<< "\\textheight 7in{}% " << std::endl
		<< "%" << std::endl;

		// caldate, our building block
	out << "\\def\\holidaymult{.08}{}% " << std::endl
		<< "\\fboxsep=0pt" << std::endl
		<< "\\long\\def\\caldate#1#2#3#4#5#6{%" << std::endl
		<< "    \\fbox{\\hbox to\\cellwidth{%" << std::endl
		<< "     \\vbox to\\cellheight{%" << std::endl
		<< "       \\hbox to\\cellwidth{%" << std::endl
		<< "          {\\hspace*{1mm}\\Large \\bf \\strut #2}\\hspace{.05\\cellwidth}%" << std::endl
		<< "          \\raisebox{\\holidaymult\\cellheight}%" << std::endl
		<< "                   {\\parbox[t]{.75\\cellwidth}{\\tiny \\raggedright %#4" << std::endl
		<< "}}}" << std::endl
		<< "       \\hbox to\\cellwidth{%" << std::endl
		<< "           \\hspace*{1mm}\\parbox[t]{.95\\cellwidth}{\\vspace*{-2ex}\\scriptsize \\raggedright {\\cjktext{#3}%" << std::endl
		<< "}}}" << std::endl
		<< "       \\hspace*{1mm}%" << std::endl
		<< "       \\hbox to\\cellwidth{#6" << std::endl
		<< "}%" << std::endl
		<< "       \\vfill%" << std::endl
		<< "       \\hbox to\\cellwidth{\\hfill \\tiny #5 \\hfill" << std::endl
		<< "}%" << std::endl
		<< "       \\vskip 1.4pt}%" << std::endl
		<< "     \\hskip -0.4pt}}}" << std::endl
		<< "{}%" << std::endl;

		// special for saturdays and holidays
	out << "\\newcommand{\\calpubd}[6]{\\caldate{#1}{\\holcol #2}{\\holcol #3}{#4}{#5}{#6}}%" << std::endl
		<< "\\newcommand{\\calsatd}[6]{\\caldate{#1}{\\satcol #2}{\\satcol #3}{#4}{#5}{#6}}%" << std::endl;

		// myday, the heading row
	out << "\\renewcommand{\\myday}[1]%" << std::endl
		<< "{\\makebox[\\cellwidth]{\\hfill\\large\\bf#1\\hfill}}" << std::endl
		<< "%" << std::endl
		<< "{}%" << std::endl
		<< "%" << std::endl;

	out << "%%%%% Define months tabular here" << std::endl;
	for (unsigned int month=0; month<=13; month++){
		out << getTexMonthSmall(month) << std::endl;
	}

	out << "\\begin{document}{}%" << std::endl
		<< "%" << std::endl
		<< "\\pagestyle{empty}{}%" << std::endl
		<< "\\setlength{\\cellwidth}{24cm}%" << std::endl
		<< "\\setlength{\\cellwidth}{0.157143\\cellwidth}" << std::endl
		<< "\\setlength{\\cellheight}{18cm}%" << std::endl
		<< "\\setlength{\\cellheight}{0.20000\\cellheight}" << std::endl
		<< "\\ \\par{}%" << std::endl
		<< "\\vspace*{-3cm}%" << std::endl
		<< "\\def\\calmonth#1#2%" << std::endl
		<< "{\\begin{center}%" << std::endl
		<< "%\\Huge\\bf\\uppercase{#1} #2 \\\\[1cm]%" << std::endl
		<< "\\hspace*{1in}\\Huge{\\fontfamily{pzc}{\\slshape \\bfseries \\uppercase{#1}}}\\quad{\\fontfamily{cmfib}#2} \\\\[1cm]%" << std::endl
		<< "\\end{center}}%" << std::endl
		<< "\\vspace*{-1.5cm}%" << std::endl
		<< "%" << std::endl
		<< "{}%" << std::endl
		<< "%" << std::endl;

	preamble=out.str();
	return preamble;
}

/**
 * @brief get TeX code for the small month-to-view cell for last/next month
 *
 * @param month 0=last December, ..., 13=next January
 *
 * @return string object containing TeX formatting code such as
 *         \verbatim\def\thisjan{...}\endverbatim
 */
std::string const&
Calendar::getTexMonthSmall(unsigned int month) const
{
#ifdef DEBUG
	MY_ERR << this << "->Calendar::getTexMonthSmall() called."
		   << std::endl;
#endif
	static std::string res;
		// month 0=lastdec
		// month 13=nextjan
	if (month>13) {
		throw INVALID_PARAM(month);
	}

	std::ostringstream out;
	unsigned int a_year=_year, a_month=month, a_day=1;
	if (month==0) { a_year--; a_month=12; };
	if (month==13) { a_year++; a_month=1; };
	out << "\\def\\" << _nGregorianShortName[month];
	out << "{"; 				// now the definition begins
	out << "\\begin{tabular}{@{\\hspace{0mm}}r@{\\hspace{1mm}}r@{\\hspace{1mm}}r@{\\hspace{1mm}}r@{\\hspace{1mm}}r@{\\hspace{1mm}}r@{\\hspace{1mm}}r@{\\hspace{0mm}}}%%" << std::endl;
	out << "\\multicolumn{7}{c}{" << _nGregorianLongName[month] << " " << a_year << "}\\\\[1mm]" << std::endl;
	out << "{\\holcol Su} & Mo & Tu & We & Th & Fr & {\\satcol Sa}\\\\[0.7mm]" << std::endl;
		// figure out how many days in the month
	unsigned int days=daysInMonth(a_year,a_month);

	Date date(a_year,a_month,a_day);
	for (int i=0; i<date.getDayOfWeek(); i++) {
		out << " &";
	}
	for (;a_day<=days;a_day++,date++) { // holiday
		std::ostringstream tmp;
		tmp << std::setw(2) << a_day;
		if (isPublicHoliday(date) || (date.getDayOfWeek()==0)) {
			out << "{\\holcol " << tmp.str() << "}";
		} else if (date.getDayOfWeek()==Date::DOW_SATURDAY) { // Saturday
			out << "{\\satcol " << tmp.str() << "}";
		} else {							// normal
			out << tmp.str();
		}
		if (date.getDayOfWeek() != Date::DOW_SATURDAY) { // not Saturday, cell separator
			out << " & ";
		} else {				// Saturday
			if (a_day < days) {	// newline if not end of month
				out << "\\\\[0.5mm]" << std::endl;
			}
		}
	}
	out << std::endl;
	out << "\\end{tabular}}%";

	res=out.str();

	return res;
}

/**
 * @brief get TeX code for the full month-to-view page for month
 *
 * @param month 1=January, ..., 12=December, of _year
 *
 * @return string object containing TeX formatting code
 */
std::string const&
Calendar::getTexMonth(unsigned int month) const
{
#ifdef DEBUG
	MY_ERR << this << "->Calendar::getTexMonth() called."
		   << std::endl;
#endif
	static std::string res;
		// month 0=lastdec
		// month 13=nextjan
	if ((month>=13) || (month<=0)) {
		throw INVALID_PARAM(month);
	}

	std::ostringstream out;

	if (month==1) {
		out << "%";
	}

	out << "\\newpage\\vspace*{-4.5cm}%" << std::endl
		<< "\\def\\lastmonth{\\hbox to\\cellwidth{%" << std::endl
		<< "\\vbox to\\cellheight{%" << std::endl
		<< "\\vfil  \\hbox to\\cellwidth{%" << std::endl
		<< "\\hfil\\scriptsize\\" << _nGregorianShortName[month-1]
		<< "\\hfil}\\vfil}}}%" << std::endl;

	out << "\\def\\nextmonth{\\hbox to\\cellwidth{%" << std::endl
		<< "\\vbox to\\cellheight{%" << std::endl
		<< "\\vfil  \\hbox to\\cellwidth{%" << std::endl
		<< "\\hfil\\scriptsize\\" << _nGregorianShortName[month+1]
		<< "\\hfil}\\vfil}}}%" << std::endl;

	out << "\\calmonth{" << _nGregorianLongName[month] << "}{"
		<< _year << "}" << std::endl
		<< "\\vspace*{-0.5cm}%" << std::endl;

		// decide which layout to use
		// layout= 6 :   SAT SUN ... FRI
		// layout= 0 :   SUN MON ... SAT
		// layout= 1 :   MON TUE ... SUN
		// default layout 0
		// also decide whether small months appear in top row (default)
	int mylayout=0;
	bool mytop=true;
	Date monthStart=Date(_year,month,1);
	if ((monthStart.getDayOfWeek()==Date::DOW_SATURDAY) && (month!=2)) {
		mylayout=6;
#ifdef DEBUG
		MY_ERR << "\tCalendar::getTexMonth(): layout changed to 6." << std::endl;
#endif
	} else if ((monthStart.getDayOfWeek()==Date::DOW_FRIDAY) &&
			   (daysInMonth(_year,month)==31)) {
		mylayout=1;
#ifdef DEBUG
		MY_ERR << "\tCalendar::getTexMonth(): layout changed to 1." << std::endl;
#endif
	}
		// top = true unless there are month start on LAYOUT or LAYOUT+1 mod 7
	if ((monthStart.getDayOfWeek()==mylayout) || (monthStart.getDayOfWeek()==(mylayout+1)%7)) {
		mytop=false;
#ifdef DEBUG
		MY_ERR << "\tCalendar::getTexMonth(): top changed to false." << std::endl;
#endif
	}
		// constify
	const int layout=mylayout;
	const bool top=mytop;

		// all layout variables decided, now proceed to cook up the table.
	for(int i=layout; i<layout+7; i++){
		out << _nDayOfWeekHeading[i%7] << std::endl;
	}
	out << "\\\\[.2cm]%" << std::endl;

	Date d=monthStart;
		//special care for normal year February which start on Sunday
	if ((month==2) && dayOfWeek(monthStart)==0 && !monthStart.isLeap()) {
		out << dayCellTex(d) << std::endl;
		d++;
		for (; d.getMonth()==month; d++){
			if (d.getDayOfWeek()== layout) {
				out << "\\hfill\\\\% " << std::endl;
			}
			out << dayCellTex(d) << std::endl;
		}
		out << "\\hfill\\\\%" << std::endl;
		for (int i=0;i<7;i++){
			out << emptyCellTex() << std::endl;
		}
		out << "\\vspace*{-\\cellwidth}\\hspace*{-2\\cellwidth}\\lastmonth\\nextmonth%" << std::endl;
	} else {
		if (top) {
			out << "\\lastmonth\\nextmonth\\hspace*{-2\\cellwidth}";
		}
			// generate (dow+7-layout)%7 empty cells
		for(int i=layout;i!=d.getDayOfWeek();i++,i%=7){
			out << emptyCellTex() << std::endl;
		}
		out << dayCellTex(d) << std::endl;
		d++;
		for (; d.getMonth()==month; d++){
			if (d.getDayOfWeek()==layout) {
				out << "\\hfill\\\\%" << std::endl;
			}
			out << dayCellTex(d) << std::endl;
		}
			// generate empty cells to fill the gap
		for (int i=Date(_year,month,daysInMonth(_year,month)).getDayOfWeek();
			 i!=(layout+6)%7; i++){
			out << emptyCellTex() << std::endl;
		}
		if (!top) {
			out << "\\vspace*{-\\cellwidth}\\hspace*{-2\\cellwidth}\\lastmonth\\nextmonth%" << std::endl;
		}
	}
	if (month<12) {
		out << std::endl;
	}

	res=out.str();

	return res;
}

/**
 * @brief get the \verbatim\end{document}\endverbatim TeX code
 *
 * This function takes no arguments.
 *
 * @return string object containing TeX formatting code
 *         \verbatim\end{document}\endverbatim
 */
std::string const&
Calendar::getTexEnd() const
{
#ifdef DEBUG
	MY_ERR << this << "->Calendar::getTexEnd() called."
		   << std::endl;
#endif
	static std::string res="\\end{document}\n";
	return res;
}

/**
 * @brief get TeX code from start to end
 *
 * This function takes no argument
 *
 * @return string object containing TeX formatting code from
 *         \verbatim\documentclass{...}\endverbatim
 * to
 *         \verbatim\end{document}\endverbatim
 */
std::string const&
Calendar::getFullYearTex() const
{
#ifdef DEBUG
	MY_ERR << this << "->Calendar::getFullYearTex() called."
		   << std::endl;
#endif
	static std::string res;
	res=getTexPreamble();
	for (unsigned int i=1; i<=12; i++){
		res+=getTexMonth(i);
	}
	res+=getTexEnd();
	return res;
}
