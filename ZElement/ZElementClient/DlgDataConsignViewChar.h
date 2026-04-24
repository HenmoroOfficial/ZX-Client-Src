// Filename	: DlgDataConsignViewChar.h
// Creator	: Fuchonggang
// Date		: 2012/03/14
// Desc		: 处理显示角色交易列表逻辑

#pragma once
#include "Windows.h"
#include "aassist.h"
#include "vector.h"

#include "GConsignListRoleNode"

class AUIListBox;

typedef abase::vector<GConsignListRoleNode> CharListVector;
typedef abase::vector<CharListVector> CharListsVector;

class CDlgDataConsignViewChar
{
public:
	CDlgDataConsignViewChar();
	~CDlgDataConsignViewChar();

	void LoadFromProtocol(void*);
	void Refresh();

	CharListVector* UpdateView();

	void SortByProf();
	void SortByLevel();
	void SortByName();
	void SortByTime();
	void SortByPrice();

	void FirstPage();
	void PrevPage();
	void NextPage();
	void LastPage();
protected:
	int m_iCurPage;			// 当前显示的页
	int	m_iNextPage;		// 下一步要显示哪一页，即是否需要更新显示
	int m_iLastPageCurTime;	// 当前最后一页

	int	m_iLastCashTime;
	int	m_iCasheValidTime;

	CharListsVector	m_VecCharLists;
};

class CompareCharListByNameAscent
{
public:
	bool operator ()(GConsignListRoleNode p1, GConsignListRoleNode p2)
	{
		if (p1.info.money>0 && p2.info.money==0)
			return true;
		else if (p1.info.money==0 && p2.info.money>0)
			return false;
		else if (p1.info.money>0 && p2.info.money>0)
			return p1.info.money<p2.info.money;
		else
			return p1.info.item_id < p2.info.item_id;
	}
};

class CompareCharListByNameDescent
{
public:
	bool operator ()(GConsignListRoleNode p1, GConsignListRoleNode p2)
	{
		CompareCharListByNameAscent temp;
		return !temp(p1,p2);
		//.data.item_id > p2.data.item_id;
	}
};

class CompareCharListByProfAscent
{
public:
	bool operator ()(GConsignListRoleNode p1, GConsignListRoleNode p2)
	{
		bool result = p1.cls < p2.cls;
		return result;
	}
};

class CompareCharListByProfDescent
{
public:
	bool operator ()(GConsignListRoleNode p1, GConsignListRoleNode p2)
	{
		return p1.cls > p2.cls;
	}
};

class CompareCharListByLevelAscent
{
public:
	bool operator ()(GConsignListRoleNode p1, GConsignListRoleNode p2)
	{
		if (p1.reborn_count!=p2.reborn_count)
		{
			return p1.reborn_count < p2.reborn_count;
		}
		else 
			return p1.level < p2.level;
	}
};

class CompareCharListByLevelDescent
{
public:
	bool operator ()(GConsignListRoleNode p1, GConsignListRoleNode p2)
	{
		CompareCharListByLevelAscent temp;
		return !temp(p1,p2);
	}
};

class CompareCharListByVendorAscent
{
public:
	bool operator ()(GConsignListRoleNode p1, GConsignListRoleNode p2)
	{
		ACString strName1 = ACString((const ACHAR *)p1.seller_name.begin(), p1.seller_name.size() / sizeof(ACHAR));
		ACString strName2 = ACString((const ACHAR *)p2.seller_name.begin(), p2.seller_name.size() / sizeof(ACHAR));
		bool result = strName1 < strName2;
		return result;
	}
};

class CompareCharListByVendorDescent
{
public:
	bool operator ()(GConsignListRoleNode p1, GConsignListRoleNode p2)
	{
		ACString strName1 = ACString((const ACHAR *)p1.seller_name.begin(), p1.seller_name.size() / sizeof(ACHAR));
		ACString strName2 = ACString((const ACHAR *)p2.seller_name.begin(), p2.seller_name.size() / sizeof(ACHAR));
		bool result = strName1 > strName2;
		return result;
	}
};

class CompareCharListByTimeAscent
{
public:
	bool operator ()(GConsignListRoleNode p1, GConsignListRoleNode p2)
	{
		return p1.info.sell_endtime < p2.info.sell_endtime;
	}
};

class CompareCharListByTimeDescent
{
public:
	bool operator ()(GConsignListRoleNode p1, GConsignListRoleNode p2)
	{
		return p1.info.sell_endtime > p2.info.sell_endtime;
	}
};

class CompareCharListByPriceAscent
{
public:
	bool operator ()(GConsignListRoleNode  p1, GConsignListRoleNode  p2)
	{
		return p1.info.price < p2.info.price;
	}
};

class CompareCharListByPriceDescent
{
public:
	bool operator ()(GConsignListRoleNode  p1, GConsignListRoleNode  p2)
	{
		return p1.info.price > p2.info.price;
	}
};