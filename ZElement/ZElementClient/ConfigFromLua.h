/********************************************************************
	created:	2006/08/16
	author:		kuiwu
	
	purpose:	get config info from lua script
	Copyright (C) 2006 - All Rights Reserved
*********************************************************************/

#pragma  once

#include "hashtab.h"
#include "AWString.h"
#include "ScriptValue.h"
#include "vector.h"

class CLuaState;


class CConfigFromLua  
{
public:
	typedef abase::hashtab<AWString, int, abase::_hash_function>	TitleDefTab;
	typedef abase::hashtab<AWString, int, abase::_hash_function>	TitleDescTab;

	CConfigFromLua();
	virtual ~CConfigFromLua();
	bool Init();
	void Release();


	/*
	const wchar_t * GetTitleDef(int id)
	{
		TitleDefTab::pair_type  Pair = m_TitleDef.get(id);
		return Pair.second? **Pair.first : NULL;
	};
	*/
	TitleDefTab  m_TitleDef;
	TitleDescTab  m_TitleDesc;

	abase::vector<CScriptValue>  m_TitleDir;

private:
	CLuaState * m_pState;

	/**
	 * \brief get titledef table from lua
	 * \param[in] assume the table name???
	 * \param[out]
	 * \return
	 * \note
	 * \warning
	 * \todo   
	 * \author kuiwu 
	 * \date 16/8/2006
	 * \see 
	 */
	bool _GetTitleDefTab();

	bool _GetTitleDir();

	
};


