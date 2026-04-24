/********************************************************************
	created:	2013/2/27
	file name:	DlgFlower.h 
	author:		zhuyan
	
	purpose:    鲜花宝贝
*********************************************************************/

#pragma once

#include "DlgBase.h"
#include "EC_Friend.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUITreeView.h"
#include "AUI\\AUICheckBox.h"
#include "AUI\\AUIStillImageButton.h"
#include "hashmap.h"


class CECIvtrItem;


bool checkFlowerTime(tm tmNow);

typedef struct FlowerData
{
	int nIndex;
	int roleid;
	ACString rolename;
	int stamp;
	int count;
	int gender;
} FLOWER_DATA; 

class CDlgFlowerMin : public CDlgBase 
{
	AUI_DECLARE_COMMAND_MAP();	
public:
	CDlgFlowerMin() {}
	virtual ~CDlgFlowerMin() {}
	virtual bool OnInitDialog();
	virtual void OnTick();
	void OnCommandOpen(const char * szCommand);
};


class CDlgFlowerRankaward : public CDlgBase 
{
	AUI_DECLARE_COMMAND_MAP();	
public:
	CDlgFlowerRankaward() {}
	virtual ~CDlgFlowerRankaward() {}
	virtual bool OnInitDialog();
};


class CDlgFlowerManifesto : public CDlgBase 
{
	AUI_DECLARE_COMMAND_MAP();	
public:
	CDlgFlowerManifesto() {}
	virtual ~CDlgFlowerManifesto() {}
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void Resize(A3DRECT rcOld, A3DRECT rcNew);
	void OnCommandCancel(const char * szCommand);	
	virtual void OnTick();
	DWORD	m_dwTimeClose;

	void setCloseTime(DWORD nTime)
	{
		m_nCloseTime = nTime;
	}
	void setWordsText(ACString szText);
private:
	bool m_bIsStart;
	DWORD m_nCloseTime;
};


class CDlgFlowerRankOption : public CDlgBase 
{
	AUI_DECLARE_COMMAND_MAP();	
public:
	CDlgFlowerRankOption() {}
	virtual ~CDlgFlowerRankOption() {}
	virtual bool OnInitDialog();
	void OnCommandSendFlower(const char * szCommand);
	bool m_bIsSend;
private:

};




class CDlgFlowerPopup : public CDlgBase 
{
	AUI_DECLARE_COMMAND_MAP();	
public:
	CDlgFlowerPopup() {}
	virtual ~CDlgFlowerPopup() {}
	virtual bool OnInitDialog();
	void OnCommandConfirm(const char * szCommand);		// 确定充值
	void OnCommandCancel(const char * szCommand);		// 取消充值
};


class CDlgFlowerGfx : public CDlgBase 
{
	AUI_DECLARE_COMMAND_MAP();	
public:
	CDlgFlowerGfx() {}
	virtual ~CDlgFlowerGfx() {}
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnHideDialog();

	virtual void OnTick();
	void setCloseTime(DWORD nTime)
	{
		m_nCloseTime = nTime;
	}
	void setPlayGfx(int nFlowerNum);

	void setContentToChat(int nChannel,ACString szContent);
private:
	PAUIIMAGEPICTURE	m_pGfx;
	DWORD	m_dwTimeClose;
	DWORD m_nCloseTime;
};




class CDlgFlowerAnounce : public CDlgBase 
{
	AUI_DECLARE_COMMAND_MAP();	
public:
	CDlgFlowerAnounce() {}
	virtual ~CDlgFlowerAnounce() {}
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	void addMsg(ACString szTemp);
private:
	PAUILABEL pLabel1;
	PAUILABEL pLabel2;
	PAUILABEL pLabel3;
	int m_nIndex;
	POINT m_nPos1;
	POINT m_nPos2;
	POINT m_nPos3;
};



class DlgFlowers: public CDlgBase 
{
	AUI_DECLARE_COMMAND_MAP();	
	AUI_DECLARE_EVENT_MAP();
public:

	DlgFlowers() {}
	virtual ~DlgFlowers() {}
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnHideDialog();

	virtual void OnTick();
	void OnCommandConfirm(const char * szCommand);	
	void OnCommandFriendlist(const char * szCommand);
	void OnCommandPrev01(const char * szCommand);		// 金钱上翻
	void OnCommandNext01(const char * szCommand);		// 金钱下翻

	void OnCommandRdoType(const char* szCommand);
	void OnCommandBuy(const char* szCommand);
	void UpdateUI(int roleid, ACString szName);
	bool checkSendFlowers();
private:
	PAUIEDITBOX m_pEdit;
	PAUIEDITBOX m_pEditWords;
	AUILabel *m_pTxt_Num;			
	AUILabel *m_pTxt_Money;			
	int m_nType;
	int m_nRoleID;
	int m_nFlowerNum; 
	int m_nBuyFlowerNum;
	int m_nFlowerIndex;
	ACString m_szFlowerName;
};



class DlgFlowersList: public CDlgBase 
{
	AUI_DECLARE_COMMAND_MAP();	
	AUI_DECLARE_EVENT_MAP();
public:

	DlgFlowersList() {}
	virtual ~DlgFlowersList() {}
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnHideDialog();

	
	void OnCommandReceivePrev(const char * szCommand);		// 收花榜上翻
	void OnCommandReceiveNext(const char * szCommand);		// 收花榜下翻
	void OnCommandPrev02(const char * szCommand);		// 送花榜上翻	
	void OnCommandNext02(const char * szCommand);		// 送花榜下翻
	void OnCommandGetAward01(const char * szCommand);	// 收花榜领取奖励
	void OnCommandGetAward02(const char * szCommand);	// 送花榜领取奖励
	void OnCommandLink(const char * szCommand);			// 全服领取奖励链接
	void OnCommandPresent(const char * szCommand);		// 送花
	void OnCommandRefresh(const char * szCommand);		// 刷新
	void OnCommandAwardDisplay(const char * szCommand);		// 奖品展示

	void OnEventLButtonList01(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventLButtonList02(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventRButtonList01(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventRButtonList02(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	void UpdateUiData(int isRecv,int page, int my_pos, int my_count,int all_count);

	void ClearReceiveMap();
	void AddMap(int nRece,FLOWER_DATA pData);
	void ClearSend();


	void UpdateFlowerList(int isRece);

	AUIListBox *m_pLst_Receive;				// list
	AUIListBox *m_pLst_Send;				// list

	FLOWER_DATA* getFlowerData(bool isRece, int nIndex);
private:


	PAUISTILLIMAGEBUTTON m_pBtn_LeftAward;
	PAUISTILLIMAGEBUTTON m_pBtn_RightPocket;
	AUIImagePicture *m_Image_LeftAward;
	AUIImagePicture *m_Image_RightPocket;
	AUILabel *m_pTxt_ReceiveRanking;		
	AUILabel *m_pTxt_ReceiveNum;			
	AUILabel *m_pTxt_SendRanking;			
	AUILabel *m_pTxt_SendNum;				
	AUILabel *m_pTxt_ReceivePage;			
	AUILabel *m_pTxt_SendPage;


	int m_nReceive;
	int m_nSend;
	int m_nReceive_All_count;
	int m_nSend_All_count;
	int m_nRecePos;
	int m_nSendPos;
	bool m_bReceive;
	bool m_bSend;
	abase::vector<FLOWER_DATA> m_vecRece;
	abase::vector<FLOWER_DATA> m_vecSend;
};
