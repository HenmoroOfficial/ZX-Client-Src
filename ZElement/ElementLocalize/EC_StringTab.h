/*
 * FILE: EC_StringTab.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2006/7/3
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#pragma warning (disable: 4284)

#include <AArray.h>
#include <vector.h>

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECStringTab
//	
///////////////////////////////////////////////////////////////////////////

class CECStringTab
{
public:		//	Types

	enum STRSET
	{
		SET_ITEMDESC = 0,
		SET_ITEMEXTDESC,
		SET_SKILLDESC,
		SET_FIXMSG,
		SET_SERVERERR,
		SET_BUFFDESC,
		SET_TASKERROR,
		SET_LOADINGHINT,
		SET_PETTALK,
		SET_ADDONSTR,
	};

	struct TEXT
	{
		int		id;
		TCHAR*	szText;
	};

public:		//	Constructor and Destructor

	CECStringTab(STRSET StrSet, char *szWorkPath);
	virtual ~CECStringTab();

public:		//	Attributes

public:		//	Operations

	//	Load strings from file
	bool Load();
	//	Save strings to file
	bool Save();

	//	Get string number
	int GetTextNum();
	//	Get specified text by index
	TEXT* GetText(int iIndex);
	//	Get specified text by id
	TEXT* GetTextByID(int id);

protected:	//	Attributes

	APtrArray<TEXT*>	m_aStrings;

	STRSET	m_StrSet;			//	String set
	char	m_szFile[MAX_PATH];	//	File name
	bool	m_bIndexed;			//	Indexed flag

protected:	//	Operations

	//	Add a string item
	bool AddStringItem(int id, const TCHAR* szText);
};

//For configs's instance.txt
class CECInstanceText
{
public:

	struct _REGION_TEXT
	{
		CString sItem;
		CString sIntValue;
		TCHAR*  szText;	// To be Translated
		_REGION_TEXT()
		{
			szText = NULL;
		}
	};

	struct _INSTANCE_TEXT 
	{
		CECStringTab::TEXT*	pText;
		abase::vector<CString> listItem;
		abase::vector<_REGION_TEXT> listRegion;
		_INSTANCE_TEXT()
		{
			pText = NULL;
		}
	};

	CECInstanceText();
	~CECInstanceText();

	bool Load(const char * szPath);
	bool Save(const char * szPath);

	int  GetTextNum();
	_INSTANCE_TEXT* GetText( int idx);
	_INSTANCE_TEXT* GetTextByID(int id);

protected:
	APtrArray<_INSTANCE_TEXT*>	m_aStrings;
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////


