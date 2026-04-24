/********************************************************************
	created:	2013/09/11
	created:	11:9:2013   10:04
	filename: 	e:\Works\ZElementClient\DlgGuildShop.h
	file path:	e:\Works\ZElementClient
	file base:	DlgGuildShop
	file ext:	h
	author:		zhuyan
	
	purpose:	帮派商城
*********************************************************************/
#pragma once

#include "DlgBase.h"
#include "AUI\\AUIScroll.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUIProgress.h"

class DlgGuildSale: public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();
public:
	DlgGuildSale();
	~DlgGuildSale();
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	void UpSaleData(void* pData);
	void OnCommand_Buy(const char * szCommand);			// 出价购买
private:
	AUIImagePicture		*m_pImg_Achievement;		// 物品图标
	AUIImagePicture		*m_pImg_Success;				// 购买成功
	AUILabel			*m_pTxt_Buyer;						// 购买者
	AUILabel			*m_pTxt_Price;						// 购买价格
	AUILabel			*m_pTxt_Time;						// 购买购买时间
	int m_nItemIndex;
	int m_nItemId;
	bool	m_bIsInit;
	int m_nItemCost;
	int m_nEndTime;
};

class DlgGuildQSale: public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();
public:
	DlgGuildQSale();
	~DlgGuildQSale();
	virtual bool OnInitDialog();
	virtual void OnTick();
	void UpQSaleData(void* pData);
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void UpDateSelect(bool isSelect);
	int m_nItemIndex;				// 物品序列
private:
	AUIImagePicture		*m_pImg_Achievement;		// 物品图标
	AUIImagePicture		*m_pImg_Select;				// 选中
	AUIImagePicture		*m_pImg_Cool;				// 冷却
	AUILabel			*m_pTxt_Name;						// 物品名称
	AUILabel			*m_pTxt_Price;						// 购买价格
	AUILabel			*m_pTxt_Time;						// 冷却时间
	AUILabel			*m_pTxt_Intro;					// 商品介绍

	int m_nTimeCount;				// 冷却总时间
	int m_nTimeCooltime;			// 冷却时间
};

class CDlgGuildShopSub : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP();
public:
	typedef abase::vector<AUISubDialog*> vecSubDialog;
	typedef abase::vector<DlgGuildSale*> vecDlgItems;
	CDlgGuildShopSub();
	~CDlgGuildShopSub();
	virtual bool OnInitDialog();
	virtual bool Release();
	bool Render();
	void InitItem();
	void UpDateItems(void* pData, int nIndex);
private:
	AUISubDialog*			m_pSubItem;			// 单个成就界面容器
	vecDlgItems				m_vecDlgItems;		// 对话框列表
	vecSubDialog			m_vecSubDialogs;	// 子对话框控件列表
	A3DRECT					m_rcWindowOld;
};


class DlgGuildShop: public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP()
public:
	enum
	{
		ADD =1, //新拍卖上架
		END, // 拍卖结束
		DEL,  // 拍卖删除
		REFRESH, // 拍卖刷新
	};
	struct ActivedItems
	{
		int nIndex;
		int nItemId;
		int nContribution;
		bool bIsActive;
	};
	struct FacBaseAucItem
	{
		int idx;					// 时间戳
		int itemid;				// 物品ID
		int winner;				// 拍卖者
		int cost;				// 拍卖价钱
		ACString szName;	// 拍卖者名称
		int status;				// 当前拍卖物品状态
		int end_time;			// 结束时间
		int baseprice;		// 竞拍底价
	};
	struct FactionAuctionItem
	{
		int		item_id;				//物品id
		int		file_icon;				//物品大图标
		ACString words;			//产出喊话
	};
	typedef abase::vector<DlgGuildSale*> vecDlgItems;
	typedef abase::vector<ActivedItems> vecActivedItems;
	typedef abase::vector<FacBaseAucItem> vecAucItems;
	typedef std::map<int,FactionAuctionItem> mapAuctionItems;
	DlgGuildShop();
	~DlgGuildShop();
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	void OnCommand_GetMoney(const char * szCommand);			// 领取返回金劵
	void OnCommand_SaleUp(const char * szCommand);				// 竞拍上一页
	void OnCommand_SaleDown(const char * szCommand);			// 竞拍下一页

	void OnCommand_SwapUp(const char * szCommand);			// 换购上一页
	void OnCommand_SwapDown(const char * szCommand);		// 换购下一页
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	void OnCommand_SwapOK(const char * szCommand);		// 换购确认
	void OnCommand_SwapCancel(const char * szCommand);		// 换购取消
	void OnCommand_Max(const char * szCommand);
	void OnCommand_Add(const char * szCommand);
	void OnCommand_Reduce(const char * szCommand);

	void UpDataSalePage();
	void UpDataSwapPage();
	void UpDateSwapItem(int nPage) ;
	void UpDataLockSwapItem(void* pData);
	void UpDateSaleItem(void* pData);
	void UpDateSaleElement(void* pData);
	void UpDateSale();
	void UpdateItemLock();

	void UpDateReturnCoupon(int nCoupon);
	void UpDateHistory(void* pData);

	void AddHistory( void* pElement);
	ActivedItems* CheckItemActived(int nIndex);
	FacBaseAucItem* GetFacBaseAucItem(int nIndex);
	FactionAuctionItem* GetFactionAuctionItem(int nItemIndex);
	void SetIsLoadAuction(bool val) { m_bIsLoadAuction = val; }
private:
	AUILabel			*m_pTxt_Money;						//获取帮派金劵
	AUILabel			*m_pTxt_Num;							// 需要帮派贡献度
	AUIImagePicture		*m_pImg_ExchangeItem;	// 兑换物品
	PAUILISTBOX 	m_pLst_History;					// 历史记录
	AUILabel			*m_pTxt_PageSale;					// 竞拍页码
	AUILabel			*m_pTxt_PageSwap;					// 兑换页码
	AUIImagePicture		*m_pImg_Icon[28];			// 换购物品图标
	AUIImagePicture		*m_pImg_IconLock[28];	// 锁图标
	vecDlgItems				m_vecDlgItems;		// 对话框列表
	vecActivedItems		m_vecActivedItems;		// 激活物品

	ActivedItems*         m_pCurItem;					// 当前选择物品
	vecAucItems				m_vecAucItems;			// 拍卖行列表
	mapAuctionItems		m_mapAuctionItem;		// 拍卖行喊话
	int m_nSaleCurPage;
	int m_nSaleMaxPage;

	int m_nSwapCurPage;
	int m_nSwapMaxPage;
	int m_nCurItemNum;
	bool m_bIsLoadAuction;
};
class SortShopAucItem
{
public:
	ACString szName;
	int nCurTime;
	SortShopAucItem(ACString szN, int nCur) : szName(szN) ,nCurTime(nCur){}
	bool operator ()(DlgGuildShop::FacBaseAucItem & p1, DlgGuildShop::FacBaseAucItem & p2)
	{
		if(0 == wcscmp( p1.szName,szName) &&  0 != wcscmp( p2.szName,szName))
		{
			return true;
		}
		else if(0 != wcscmp( p1.szName,szName) && 0 == wcscmp( p2.szName,szName))
		{
			return false;
		}
		else 	if(0 == wcscmp( p1.szName,szName) && 0 == wcscmp( p2.szName,szName))
		{
			if((p1.end_time - nCurTime) < (p2.end_time - nCurTime))
			{
				return true;
			}
		}
		else if((p1.end_time - nCurTime) < (p2.end_time - nCurTime))
		{
			return true;
		}
		return false;
	}
};

class CDlgGuildQShopSub : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP();
public:
	typedef abase::vector<AUISubDialog*> vecSubDialog;
	typedef abase::vector<DlgGuildQSale*> vecDlgItems;
	CDlgGuildQShopSub();
	~CDlgGuildQShopSub();
	virtual bool OnInitDialog();
	virtual bool Release();
	bool Render();
	void InitItem();
	void UpDateItems(void* pData, int nIndex);
private:
	AUISubDialog*			m_pSubItem;			// 单个成就界面容器
	vecDlgItems				m_vecDlgItems;		// 对话框列表
	vecSubDialog			m_vecSubDialogs;	// 子对话框控件列表
	A3DRECT					m_rcWindowOld;
};


class DlgGuildQShop: public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP()
public:
	struct QShopItems
	{
		int nIndex;
		int nPrice;
		int nFile_Icon;
		ACString szName;
		ACString szDes;
		ACString szWord;
		int nCooltime;
		int nCurCoolTime;
	};
	typedef abase::vector<QShopItems> vecQShopItems;
	DlgGuildQShop();
	~DlgGuildQShop();
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnHideDialog();
	void CheckQShopItem( );

	void OnCommand_Up(const char * szCommand);			// 上一页
	void OnCommand_Down(const char * szCommand);		// 下一页
	void OnCommand_Buy(const char * szCommand);		// 购买

	void UpDataBuyItemInfo(void* pData);
	int GetCurItemIndex();
	void SetCurItemIndex(DlgGuildQSale* val);
	void UpDateBuyItem(int nIndex);
	void ClearItemCooltime(int nIndex);
private:
	AUILabel			*m_pTxt_Num;							// 页码
	vecQShopItems m_vecShopItem;
	DlgGuildQSale* m_pCurSelect;
	int m_nCurPage;
	int m_nMaxPage;
	int m_nCurItemIndex;
};