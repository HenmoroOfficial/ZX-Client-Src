#ifndef _BASEDATAORG_H_
#define _BASEDATAORG_H_

#include "AString.h"
#include "vector.h"
#include "hashmap.h"
#include "AVariant.h"

class BaseDataOrg
{
public:
	BaseDataOrg();
	virtual ~BaseDataOrg();

protected:
	struct DATA_ITEM
	{
		AString		m_strItemName;
		AString		m_strItemType;
		AVariant	m_defVal;
	};

	typedef abase::vector<DATA_ITEM*> ItemArray;
	ItemArray m_ItemArray;

	typedef abase::hash_map<AString, DATA_ITEM*> ItemMap;
	ItemMap m_ItemMap;

public:
	int GetItemNum() const;
	const AString& GetItemName(int nIndex) const;
	const AString& GetItemType(int nIndex) const;
	const AVariant& GetItemDefValue(int nIndex) const;
	/*
	 *	0 成功
	 *	-1 打开文件失败
	 *	-2 Item重名和值
	 */
	int Load(const char* szPath);
	void Release();
	bool FindItem(const AString& strName, const AString& strType)
	{
		return m_ItemMap.find(strName + strType) != m_ItemMap.end();
	}
};

#endif