#ifndef _TIMESEGMENT_H_
#define _TIMESEGMENT_H_

#ifdef TS_MULTI_THREAD
#define TS_USE_MULTI_THREAD
#endif

#include "vector.h"

enum CHECK_TIMESEG{TS_SUCCESS = 0, TS_ERR_YEAR, TS_ERR_MONTH, TS_ERR_MONTHDAY, TS_ERR_WEEKDAY, TS_ERR_HOUR, TS_ERR_MIN, TS_ERR_ALL};

class CTimeSegment
{
public:

	struct DATETIMESEG
	{
		int nYear;    //年，正常区间 [1900-2100] -1 代表任意
		int nMonth;   //月，正常取值区间[0,11]  -1 代表任意
		int nDay;     //日，正常取值区间[1,31]  -1 代表任意
		int nHour;    //小时，正常取值区间[0,23]  -1 代表任意
		int nMin;     //分钟，正常取值区间[0,59]  -1 代表任意
		int nWeek;    //周，正常取值区间[0,6]  0 代表星期日 -1 代表任意
	};

public:
	virtual CHECK_TIMESEG CheckValidity(const DATETIMESEG &start, const DATETIMESEG &end) = 0; 

	virtual bool IsTimeSegOverlay(const DATETIMESEG &start, const DATETIMESEG &end, const abase::vector<DATETIMESEG> &atimeseg, bool is_intersection) = 0;

	virtual int AnalogueCalculation(const abase::vector<DATETIMESEG> &atimeseg, bool is_intersection) = 0;

	virtual void OpenAnalogueFile() = 0;

		/**
	 * @brief CheckTimeSegAvailalbe
	 *
	 * @param atimeseg
	 * @param is_intersection
	 *
	 * @return : 返回0表示成功，返回正奇数表示vector中第几项有误
	 */
	virtual int CheckTimeSegAvailalbe(const abase::vector<DATETIMESEG> &atimeseg, bool is_intersection) = 0;


	virtual ~CTimeSegment(){};

private:

};

CTimeSegment * CreateTimeSegCheck(void);


#endif
