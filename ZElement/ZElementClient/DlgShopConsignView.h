// Filename	:	DlgShopConsignView.h
// Creator	:	Lei Dongyu
// Date		:	2010/10/29
// Description:	寻宝网游戏内商品浏览
// 修改：2012/3/14 增加寄售角色功能，因为时间紧迫，不想影响以前功能，新加一个类
// 处理CDlgDataConsignViewChar

#pragma once

#include "DlgBase.h"
#include "gnbyteorder.h"

class AUIDialog;
class AUIRadioButton;
class AUITreeView;
class AUIEditBox;
class AUILabel;
class AUIStillImageButton;
class AUIListBox;
class AUITreeView;
class CDlgShopConsignAction;
class CDlgDataConsignViewChar;

#include "gconsignitem"
#include "GConsignListNode"

#define new A_DEBUG_NEW

#define NUMSHOWPERPAGE  10	//每页显示物品条数
#define NUMSTOREPERPAGE 10	//每次取得的物品条数，与服务器商定，应为NUMSHWOPERPAGE的倍数
							//目前未作分页处理，默认两者相等

#define CACHETIME	300		//缓存失效的时间,单位秒

#define MIN_CATEGORY	90001
#define	MAX_CATEGORY	90073
#define	MONEY_CATEGORY	90073
#define ALL_CATEGORY	80000	//全部商品

class ItemData
{
public:

public:
	GConsignItem	data; 
	ACString	vendor;
	int			commodityid;

	ItemData(GConsignItem d , ACString v , int id);
	virtual ~ItemData();
};

//class ItemForRButton
//{
//public:
//	int64_t sn;
//	ACString	vendor;
//
//	ItemForRButton(int64_t s, ACString v);
//};

typedef abase::vector<ItemData> ItemList;

class CachePage
{
public:
	bool buildCache(int , int , DWORD);
	bool addItem(ItemData);
	int getPage();
	int getType();
	DWORD getTime();
	ItemList* getList();

	void clear();

	CachePage(int ,int ,std::vector<GConsignListNode>);
	CachePage(int ,int);
	CachePage();
	~CachePage();

public:
	int64_t	startIndex;
	int64_t	endIndex;

protected:
	int		iPage;		//缓存的页码
	int		iType;		//缓存的物品种类
	DWORD	dwRefreshTime;	//缓存更新的时间

	ItemList	vList;	//缓存的List
};

class CacheCategory
{
public:
	int lastPage;
	abase::hash_map<int , CachePage* > cache;

	void clear();
};
typedef abase::hash_map<int , CacheCategory*> cacheMap;

class CDlgShopConsignView : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgShopConsignView();
	~CDlgShopConsignView();

	bool OnInitDialog();
	void OnShowDialog();
	void OnTick();
	virtual bool Release();
	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);

	void OnCommand_MyList(const char* szCommand);
	void OnCommand_AllList(const char* szCommand);
	void OnCommand_CharList(const char* szCommand);
	void OnCommand_WebSite(const char* szCommand);
	void OnCommand_Refresh(const char* szCommand);
	void OnCommand_FirstPage(const char* szCommand);
	void OnCommand_PrevPage(const char* szCommand);
	void OnCommand_NextPage(const char* szCommand);
	void OnCommand_LastPage(const char* szCommand);
	void OnCommand_SortByProf(const char* szCommand);
	void OnCommand_SortByLevel(const char* szCommand);
	void OnCommand_SortByName(const char* szCommand);
	void OnCommand_SortByPrice(const char* szCommand);
	void OnCommand_SortByTime(const char* szCommand);
	void OnCommand_SortBySeller(const char* szCommand);
	void OnCommand_CANCEL(const char* szCommand);
	void OnCommand_Reset(const char* szCommand);
	void OnCommand_Help(const char* szCommand);

	void OnEventLButtonDownTree(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventRButtonUpGoods(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventRButtonUpChars(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	void Refresh(int iPage, int iType ,char dir = 2);
	void BuildItemList();
	void requestList(int category , int page , char direction);
	void requestDetail(int64_t sn);
	void SendData(int type , int page , int64_t index , char dir);

	bool ClearCache(DWORD dwTime ,bool clearAll = false);
	bool ClearCacheCategory(int type);

	//bool searchSN(int64_t sn);

protected:
	AUITreeView*			m_pTree_ItemTree;
	CDlgShopConsignAction*	m_pAction;
	AUIListBox*				m_pList_Item;
	AUIListBox*				m_pList_Char;
	AUILabel*				m_pLabel;
	AUILabel*				m_pLabelStart;
	AUILabel*				m_pLabelSub;

	abase::vector<A2DSprite*> vecItemPic;

	int m_iCurPageItems;
	int iCurrentType;
	int m_iLastPageCurType;
	bool m_bLoaded;
	DWORD lastRequestTime;
	DWORD	lastReqDetailTime;
	int64_t lastReqSN;

	abase::hash_map<int64_t , ACString> m_ItemDetail;
	cacheMap m_cache;
	CachePage curCache;
	AXMLFile TreeFile;
	P_AUITREEVIEW_ITEM pOldItem;
	//P_AUITREEVIEW_ITEM pCurTreeItem;

	CacheCategory cacheAll;
	int	m_iTimestampItems;					//用于全部商品缓存的更新

	int64_t startIndex;
	int64_t endIndex;

	CDlgDataConsignViewChar* m_DataViewChar;

	void _InsertSellTypes();
	void _TickItems();
	void _TickCharList();
};

class CompareListByVendorAscent
{
public:
	bool operator ()(ItemData p1, ItemData p2)
	{
		bool result = p1.vendor < p2.vendor;
		return result;
	}
};

class CompareListByVendorDescent
{
public:
	bool operator ()(ItemData p1, ItemData p2)
	{
		return p1.vendor > p2.vendor;
	}
};

class CompareListByTimeAscent
{
public:
	bool operator ()(ItemData p1, ItemData p2)
	{
		return p1.data.sell_endtime < p2.data.sell_endtime;
	}
};

class CompareListByTimeDescent
{
public:
	bool operator ()(ItemData p1, ItemData p2)
	{
		return p1.data.sell_endtime > p2.data.sell_endtime;
	}
};

class CompareListByPriceAscent
{
public:
	bool operator ()(ItemData  p1, ItemData  p2)
	{
		return p1.data.price < p2.data.price;
	}
};

class CompareListByPriceDescent
{
public:
	bool operator ()(ItemData  p1, ItemData  p2)
	{
		return p1.data.price > p2.data.price;
	}
};

class CompareListByNameAscent
{
public:
	bool operator ()(ItemData p1, ItemData p2)
	{
		if (p1.data.money>0 && p2.data.money==0)
			return true;
		else if (p1.data.money==0 && p2.data.money>0)
			return false;
		else if (p1.data.money>0 && p2.data.money>0)
			return p1.data.money<p2.data.money;
		else
			return p1.data.item_id < p2.data.item_id;
	}
};

class CompareListByNameDescent
{
public:
	bool operator ()(ItemData p1, ItemData p2)
	{
		CompareListByNameAscent temp;
		return !temp(p1,p2);
		//.data.item_id > p2.data.item_id;
	}
};
