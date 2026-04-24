// Filename	: EC_Calendar.h
// Creator	: Fu Chonggang
// Date		: 2010/09/13
// 封装年、月、日，执行各种运算

#pragma once
#include "time.h"

class CECCalendar
{
public:
	enum
	{
		YEAR_BASE = 1900,
	};

	enum
	{
		JANUARY,
		FEBRUARY,
		MARCH,
		APRIL,
		MAY,
		JUNE,
		JULY,
		AUGUST,
		SEPTEMBER,
		OCTOBER,
		NOVEMBER,
		DECEMBER,
		NUMMONTH,
	};
	CECCalendar();
	CECCalendar(tm t);
	CECCalendar(int y, int m, int d);
	~CECCalendar() {}
	int CalWeekStart();	// 0-6
	
	const CECCalendar& operator ++ ();
	const CECCalendar operator ++ (int);
	const CECCalendar& operator -- ();
	const CECCalendar operator -- (int);
	const CECCalendar& operator += (const int num);
	const CECCalendar& operator -= (const int num);
	friend CECCalendar operator + (const CECCalendar& cal, const int& num);
	friend CECCalendar operator - (const CECCalendar& cal, const int& num);
	friend int operator - (const CECCalendar& cal1, const CECCalendar& cal2);

	bool operator >(const CECCalendar cal) const{ return m_iYear>cal.m_iYear || (m_iYear==cal.m_iYear&&m_iMonth>cal.m_iMonth) || (m_iYear==cal.m_iYear&&m_iMonth==cal.m_iMonth&&m_iDay>cal.m_iDay);}
	bool operator <(const CECCalendar cal) const{ return m_iYear<cal.m_iYear || (m_iYear==cal.m_iYear&&m_iMonth<cal.m_iMonth) || (m_iYear==cal.m_iYear&&m_iMonth==cal.m_iMonth&&m_iDay<cal.m_iDay);}
	bool operator >=(const CECCalendar cal) const{ return !(*this<cal);}
	bool operator <=(const CECCalendar cal) const{ return !(*this>cal);}
	bool operator ==(const CECCalendar cal) const{ return m_iYear==cal.m_iYear&&m_iMonth==cal.m_iMonth&&m_iDay==cal.m_iDay; }
	bool operator !=(const CECCalendar cal) const{ return !(*this==cal);}

	void AddMonth() { if (m_iMonth==DECEMBER) { m_iYear++; m_iMonth=JANUARY; } else m_iMonth++; }
	void SubMonth() { if (m_iMonth==JANUARY) { m_iYear--; m_iMonth=DECEMBER; } else m_iMonth--; }
	int year() { return m_iYear+YEAR_BASE; }
	int month() { return m_iMonth+1; }
	int day() { return m_iDay; }

	static int Days(int y, int m) 
	{
		if (y%4==0 && y%100!=0)
			return ms_iDaysOneMonthSpecial[m];
		else
			return ms_iDaysOneMonth[m];
	}

	// same as struct tm
	int				m_iYear;	// years since 1900 
	int				m_iMonth;	// months since January - [0,11]
	int				m_iDay;		// day of the month - [1,31]
private:
	static int ms_iDaysOneMonth[NUMMONTH];
	static int ms_iDaysOneMonthSpecial[NUMMONTH];
};
