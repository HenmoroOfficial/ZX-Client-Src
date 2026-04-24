//
//
//
//-----------------------------------------------------------------------------
#ifndef _AUI_COMMON_H_
#define _AUI_COMMON_H_

void AUI_ConvertChatString(const ACHAR *pszChat, ACHAR *pszConv, bool bName)
{
	int i, nLen = 0;

	pszConv[0] = 0;
	for( i = 0; i < (int)a_strlen(pszChat); i++ )
	{
		if( pszChat[i] == '^' )
		{
			pszConv[nLen] = '^';
			pszConv[nLen + 1] = '^';
			nLen += 2;
		}
		else if( pszChat[i] == '&' )
		{
			pszConv[nLen] = '^';
			pszConv[nLen + 1] = '&';
			nLen += 2;
		}
		else
		{
			pszConv[nLen] = pszChat[i];
			nLen++;
		}
	}
	pszConv[nLen] = 0;
}



#endif //_AUI_COMMON_H_

