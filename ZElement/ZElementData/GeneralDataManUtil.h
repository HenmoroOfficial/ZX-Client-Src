//=============================================================================
//
//
//=============================================================================
#ifndef __GENERAL_DATA_MAN_UTIL__
#define __GENERAL_DATA_MAN_UTIL__
//=============================================================================
enum
{
	nStrNameLen = 64,
	nVIPAwardMainTypeCount = 3,
	nCurDataItemTemplVersion = 1,
	nStrMaxLen = 512,
	nSubTypeMaxNum = 20,
};

extern const char *szVIPAwardMainType[];

#ifdef _ANGELICA22
typedef wchar_t			namechar;
#else
typedef unsigned short	namechar;
#endif // #ifdef _ANGELICA22

enum GENERAL_DATA_TEMPL_TYPE_ID
{
	GENERAL_DATA_TEMPL_TYPE_DEFAULT	= 0,
	GENERAL_DATA_TEMPL_TYPE_VIPAWARD,

	GENERAL_DATA_TEMPL_TYPE_ID_MAX_NUM,
};

#endif //__GENERAL_DATA_MAN_UTIL__