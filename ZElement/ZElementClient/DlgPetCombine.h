// Filename	: DlgPetCombine.h
// Creator	: Fu Chonggang
// Date		: 2012-6-20
// 宠物修炼界面

#pragma once

#include "DlgItemSvrBase.h"
#include "vector.h"

class AUIImagePicture;
class AUILabel;

/************************************************************************
简单宠物修炼界面
修炼星相
修炼外观
补充寿命
************************************************************************/
class CDlgPetCombine : public CDlgItemSvrBase  
{
	AUI_DECLARE_COMMAND_MAP()
		
public:
	CDlgPetCombine() {}
	virtual ~CDlgPetCombine() {}
	
	virtual void SetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg=NULL);
	virtual void ServerStart();
	virtual void Reset();

	void OnCommandCancel(const char * szCommand);
	void OnCommandStopOpt(const char * szCommand);
	
protected:
	virtual bool OnInitDialog();
	virtual void OnHideDialog();
	virtual void OnTick();

	abase::vector<int> m_vecSuitIds;
};

/************************************************************************
宠物资质修炼
************************************************************************/
class CDlgPetCombineZiZhi : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
		
public:
	CDlgPetCombineZiZhi() : m_bStopContinue(false) {} 
	virtual ~CDlgPetCombineZiZhi() {}
	
	void OnCommandCancel(const char * szCommand);
	void OnCommandStopOpt(const char * szCommand);
	
	void OnCommandSubType(const char * szCommand);
	void OnCommandAdd(const char * szCommand);
	void OnCommandSub(const char * szCommand);
	void OnCommandMax(const char * szCommand);
	void OnCommandStart(const char * szCommand);
	
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	
	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);

	static int GetXiandouId() { return m_iIdXianDou; }
	
protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();

	int MaxOptCount();
	
	static const int m_iIds[12][3];		// 道具ids
	static const int	m_iIdXianDou;	// 仙豆id
	static const int	m_iIdXianDouNeed[12][3];	// 需要的仙豆数来那个

	int m_iCurSel;
	bool m_bStopContinue;
};

/************************************************************************
宠物修炼+物物交易
修炼舍利
修炼斗气 道具数量 连续修炼
改变特质 道具数量
改变前世 道具数量
技能修炼 道具数量 连续修炼
技能扩充 两个道具
改变限界 道具数量
************************************************************************/
class CDlgPetCombineExchange : public CDlgItemSvrBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
		
public:
	CDlgPetCombineExchange();
	virtual ~CDlgPetCombineExchange();
	
	virtual void SetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg=NULL);
	virtual void ServerStart();
	virtual void Reset();
	
	void OnCommandExchangeConfirm(const char *szCommand);
	void OnCommandSet(const char * szCommand);
	void OnCommandAdd(const char * szCommand);
	void OnCommandSub(const char * szCommand);
	void OnCommandMax(const char * szCommand);
	void OnCommandCancel(const char * szCommand);
	void OnCommandStopOpt(const char * szCommand);
	
	void OnEventLButtonDownExchange(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventRButtonDownExchange(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	
	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	
protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnHideDialog();
	virtual void OnTick();
	void SelectItem(int nSel);
	void UpdateItemInfo(int nCount = 1);
	// SetItem判断物品是否合适，如果合适调用这个函数操作
	void DoSetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg);

	int m_iIds[5][12];
	abase::vector<int> m_vecExtraIds;
	AUIImagePicture	*m_pImgSell[12];
	AUIImagePicture	*m_pImg_ItemReq[2];
	AUIStillImageButton	*m_pBtnSet[5];
	AUILabel *m_pTxt_ItemReq[2];
	AUILabel *m_pTxt_CreditReq[2];
	AUILabel *m_pTxt_SpecialReq;
	int m_iCurSel;		// 当前选中物品的index，对应商店中位置是CDlgShopExchange::m_nCurSel,m_nShopSet
	
	// 修炼斗气、修炼技能
	bool m_bCombining;	// true:正在连续操作   false:没有任何操作
	int  m_iCombineId;	//
};
