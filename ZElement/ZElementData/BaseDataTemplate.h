#ifndef BASEDATATEMPLATE_H_
#define BASEDATATEMPLATE_H_

#include "AString.h"
#include "vector.h"
#include "hashmap.h"
#include "AVariant.h"
#include "BaseDataIDMan.h"

bool WriteMd5CodeToTemplate(const char *szPathName);

class BaseDataTempl
{
public:
	BaseDataTempl() {}
	virtual ~BaseDataTempl() {}

protected:
	unsigned long	m_ulID;
	AString			m_strName;
	AString			m_strPath;
	AString			m_strParentPath;
	int				m_nLevel;

	struct ITEM_DATA
	{
		AString		m_strName;
		AString		m_strType;
		AVariant	m_varVal;
	};

	typedef abase::vector<ITEM_DATA*> ItemArray;
	ItemArray m_ItemArray;
	
	typedef abase::hash_map<AString, ITEM_DATA*> ItemMap;
	ItemMap m_ItemMap;

protected:
	static AString ParseOrgPath(const char* szOrgPath, const char* szPath, int& nLevel);

public:
	unsigned long GetID() const { return m_ulID; }
	const AString& GetName() const { return m_strName; }
	const AString& GetParentPathName() const;
	int GetItemNum() const;
	const AString& GetItemName(int nIndex) const;
	const AString& GetItemType(int nIndex) const;
	AVariant GetItemValue(int nIndex) const;
	int GetItemIndex(const char* szItem) const;

	void SetIDEx( unsigned long ulID ) { m_ulID = ulID; } //For special usage. generally speeking, you should not use it.
	void SetName(const AString& strName) { m_strName = strName; }
	void SetParentPathName(const AString& strPath);
	void SetItemValue(int nIndex, const AVariant& var);
	void SetItemValue( const AString& strKey, const AVariant& var );
	bool LoadHeader(const char* szPath);
	AString ParseParentPath() const;
	int GetLevel() const { return m_nLevel; }
	AString GetFilePath() const { return m_strPath; }

	virtual bool CreateTempl(const char* szPath, const char* szParentPath, BaseDataIDManBase* pIDMan, bool bNew = true, unsigned long ulTemplID = 0);
	virtual int Load(const char* szPath, bool bFromOrg = true);
	virtual bool Save(const char* szPath);
	virtual void Release();

	BaseDataTempl& operator = (const BaseDataTempl& src);
};

#endif