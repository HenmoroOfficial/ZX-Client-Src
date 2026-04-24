// Filename	: DlgCalendar.cpp
// Creator	: Fu Chonggang
// Date		: 2009/02/10

#include "EC_Calendar.h"
#include "EC_Global.h"
#include "EC_Game.h"

int CECCalendar::ms_iDaysOneMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
int CECCalendar::ms_iDaysOneMonthSpecial[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

CECCalendar::CECCalendar()
{
	*this = CECCalendar(g_pGame->GetServerLocalTime());
}

CECCalendar::CECCalendar(tm t)
{
	*this = CECCalendar(t.tm_year, t.tm_mon, t.tm_mday);
}

CECCalendar::CECCalendar(int y, int m, int d)
{
	m_iYear = y;
	m_iMonth = m;
	m_iDay = d;
}

int CECCalendar::CalWeekStart()
{
	tm tmNow = g_pGame->GetServerLocalTime();
	int iyn, imn;
	int i, iDays = 0;
	iyn = tmNow.tm_year+YEAR_BASE;
	imn = tmNow.tm_mon+1;
	for (i=imn-1; i>0; i--)
	{
		iDays -= Days(year(), i-1);
	}
	
	int iBigger, iSmaller;
	if (iyn>year())
	{
		iBigger = iyn;
		iSmaller = year();
		for (i=iSmaller; i<iBigger; i++)
		{
			if (i%4==0 && i%100!=0)
				iDays-=366;
			else
				iDays-=365;
		}
		
	}
	else
	{
		iBigger = year();
		iSmaller = iyn;
		for (i=iSmaller; i<iBigger; i++)
		{
			if (i%4==0 && i%100!=0)
				iDays+=366;
			else
				iDays+=365;
		}
	}
	
	for (i=0;i<m_iMonth;i++)
	{
		iDays+=Days(year(), i);
	}
	int iWeekStart = tmNow.tm_wday-(-iDays+tmNow.tm_mday-1)%7;
	if (iWeekStart<0)
	{
		iWeekStart+=7;
	}
	if (iWeekStart>=7)
	{
		iWeekStart-=7;
	}
	return iWeekStart;
}

const CECCalendar& CECCalendar::operator ++ ()
{
	if (m_iDay<Days(year(),m_iMonth))
		m_iDay++;
	else
	{
		m_iDay = 1;
		AddMonth();
	}
	return *this;
}

const CECCalendar CECCalendar::operator ++ (int)
{
	CECCalendar tmp(*this);
	this->operator ++();
	return tmp;
}

const CECCalendar& CECCalendar::operator -- ()
{
	if (m_iDay>1)
		m_iDay--;
	else
	{
		SubMonth();
		m_iDay = Days(year(),m_iMonth);
	}
	return *this;
}

const CECCalendar CECCalendar::operator -- (int)
{
	CECCalendar tmp(*this);
	this->operator --();
	return tmp;
}

const CECCalendar& CECCalendar::operator += (int num)
{
	m_iDay+=num;
	if (num>0)
	{
		while(m_iDay>Days(year(),m_iMonth))
		{
			m_iDay -= Days(year(),m_iMonth);
			AddMonth();
		}
	}
	else
	{
		while(m_iDay<0)
		{
			SubMonth();
			m_iDay += Days(year(),m_iMonth);
		}
	}
	return *this;
}

const CECCalendar& CECCalendar::operator -= (int num)
{
	return *this+=(-num);
}

CECCalendar operator + (const CECCalendar& cal, const int& num)
{
	CECCalendar calR = cal;
 	calR += num;
	return calR;
}

CECCalendar operator - (const CECCalendar& cal, const int& num)
{
	return cal+(-num);
}

int operator - (const CECCalendar& cal1, const CECCalendar& cal2)
{
	int ret=0;
	if (cal1>=cal2)
	{
		CECCalendar start = cal2;
		CECCalendar end = cal1;
		ret = end.m_iDay - start.m_iDay;
		start.m_iDay = 0;
		end.m_iDay = 0;
		while (start!=end)
		{
			ret += CECCalendar::Days(start.year(), start.m_iMonth);
			start.AddMonth();
		}
		return ret;
	}
	else
		return cal2-cal1;
}