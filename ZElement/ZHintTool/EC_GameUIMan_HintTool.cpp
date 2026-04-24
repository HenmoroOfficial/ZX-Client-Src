//
//
#include "EC_GameUIMan_HintTool.h"
//-----------------------------------------------------------------------------
void CECGameUIMan::Int64ToStr(INT64 num, ACString& str)
{
	str.Empty();
	INT64 temp = num;
	bool bNeg = false;;
	if (temp==0)
	{
		str.Format(_AL("%d"),0);
		return;
	}
	else if (temp<0)
	{
		bNeg = true;
		temp = -temp;
	}
	while (temp>0)
	{
		int b = (int)(temp%10);
		ACHAR szText[2];
		a_sprintf(szText,_AL("%d"), b);
		str = szText + str;
		temp/=10;
	}
	if (bNeg)
		str = _AL("-") + str;
}