// DataPathMan.h: interface for the DataPathMan class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _DATA_PATH_MAN_H_
#define _DATA_PATH_MAN_H_

#include "hashmap.h"
#include "AString.h"

class DataPathMan  
{
public:
	DataPathMan();
	virtual ~DataPathMan();

protected:
	abase::hash_map<AString, unsigned long> m_PathIDMap;
	abase::hash_map<unsigned long, AString> m_IDPathMap;

public:

	const char* GetPathByID(unsigned long ulID) const
	{
		abase::hash_map<unsigned long, AString>::const_iterator it = m_IDPathMap.find(ulID);
		return it == m_IDPathMap.end() ? "" : it->second;
	}
	
	unsigned long GetIDByPath(const AString& strPath) const
	{
		abase::hash_map<AString, unsigned long>::const_iterator it = m_PathIDMap.find(strPath);
		return it == m_PathIDMap.end() ? 0 : it->second;
	}
	
	unsigned long GetIdNum()
	{
		return m_PathIDMap.size();
	}

	void Release()
	{
		m_PathIDMap.clear();
		m_IDPathMap.clear();
	}

	//Return 0 if failed
	int Load(const char *szPath);
};

#endif // !defined(AFX_DATAPATHMAN_H__7860804D_E9E1_4152_9AA4_B69DF0A1D632__INCLUDED_)
