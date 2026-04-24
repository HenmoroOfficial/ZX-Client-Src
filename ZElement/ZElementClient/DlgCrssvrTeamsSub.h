/********************************************************************
	created:	2013/07/19
	created:	19:7:2013   15:25
	filename: 	e:\Works\ZElementClient\DlgCrssvrTeamsSub.h
	file path:	e:\Works\ZElementClient
	file base:	DlgCrssvrTeamsSub
	file ext:	h
	author:		zhuyan
	
	purpose:	战队子项
*********************************************************************/
#pragma once

#include "DlgBase.h"
#include "AUI\\AUIScroll.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIListBox.h"

class AUISubDialog;
class DlgCrssvrTeamsItem;

struct CrssvrTeamsRole
{
	char occupation;			
	int level;				
	ACString szName;	// 
};

class DlgCrssvrTeamsImg :  public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
public:
	DlgCrssvrTeamsImg();
	~DlgCrssvrTeamsImg();
	virtual bool OnInitDialog();
	void Clear();
	void UpDateImg(CrssvrTeamsRole* pInfo, int nIndex);
private:
	PAUIIMAGEPICTURE m_pPicture[10];
};


class DlgCrssvrTeamsItem :  public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
public:

	typedef abase::vector<CrssvrTeamsRole> vecTeamsRole;
	DlgCrssvrTeamsItem();
	~DlgCrssvrTeamsItem();
	virtual bool OnInitDialog();

	void OnCommand_Display(const char * szCommand);
	void UpDateItemBase(void* pData,int nPage,int nIndex);
	void UpDateSaveTeamsInfo(void* pData);
private:
	AUILabel			*m_pTxt_Rank;	// 顺序
	AUILabel			*m_pTxt_Name;	// 名称
	AUILabel			*m_pTxt_Win;	// 胜利
	AUILabel			*m_pTxt_Lose;	// 失败
	AUILabel			*m_pTxt_Rate;	// 胜率
	AUILabel			*m_pTxt_Score;	// 分数
	vecTeamsRole    m_vecTeamsRole;
};



class DlgCrssvrTeamsSub :  public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
public:
	typedef abase::vector<AUISubDialog*> vecSubDialog;
	typedef abase::vector<DlgCrssvrTeamsItem*> vecDlgItems;

	DlgCrssvrTeamsSub();
	~DlgCrssvrTeamsSub();
	virtual bool OnInitDialog();
	virtual bool Release();
	bool Render();
	void UpDateCrssveInfo(void* pData, int nPage);
private:
	AUISubDialog*			m_pSubItem;			// 单个成就界面容器
	vecDlgItems				m_vecDlgItems;		// 对话框列表
	vecSubDialog			m_vecSubDialogs;	// 子对话框控件列表
	A3DRECT					m_rcWindowOld;
};


// 战队奖励---周
class DlgCrssvrTeamsAward_Week: public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
		AUI_DECLARE_EVENT_MAP();
public:
	DlgCrssvrTeamsAward_Week();
	~DlgCrssvrTeamsAward_Week();
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual bool Release();
	void ClearData() ;
	void OnCommand_Rdo(const char * szCommand);
	void OnCommand_Confirm(const char * szCommand);
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void SetUpDate(int nMaxScore);
	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
private:
		AUILabel			*m_pTxt_Money;	// 钱
		AUILabel			*m_pTxt_Score;	// 分数
		PAUIIMAGEPICTURE m_pPicture[9];
		int	m_iCurSel;
		int cs_personal_award_timestamp;  //跨服6v6个人跨服每周分档奖励 
		A2DSprite *m_pSprite;
		int m_nMaxScore;
};

// 战队奖励---兑换
class DlgCrssvrTeamsAward_Exchange: public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
		AUI_DECLARE_EVENT_MAP();
public:
	DlgCrssvrTeamsAward_Exchange();
	~DlgCrssvrTeamsAward_Exchange();
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	virtual bool Release();
	void ClearData() ;
	void OnCommand_Rdo(const char * szCommand);
	void OnCommand_Confirm(const char * szCommand);
	void OnCommand_RdoCoin(const char * szCommand);
	void OnCommand_RdoCoinType(const char * szCommand);
	void OnCommand_Btn_GetCoin(const char * szCommand);
	void OnCommand_Btn_Get(const char * szCommand);
	
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void SetUpDate(int nMaxScore);
	int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
private:
	AUILabel			*m_pTxt_Money;	// 钱
	AUILabel			*m_pTxt_Score;	// 分数
	PAUIIMAGEPICTURE m_pPicture[15];
	AUILabel			*m_pTxt_ExMoney[15];	// 兑换钱
	int	m_iCurSel;
	CROSS6V6_ITEM_EXCHANGE_CONFIG* m_pConfig;
	A2DSprite *m_pSprite;
	int m_nHeight;		// 1 金 2银
	int m_nLow;			//1 银 2铜
	int cs_exchange_award_timestamp; //跨服6v6每周兑换币奖励
};

// 战队奖励---赛季
class DlgCrssvrTeamsAward_Stage: public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
		AUI_DECLARE_EVENT_MAP();
public:
	DlgCrssvrTeamsAward_Stage();
	~DlgCrssvrTeamsAward_Stage();
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual bool Release();
	void ClearData() ;

	void OnCommand_Rdo(const char * szCommand);
	void OnCommand_Confirm(const char * szCommand);
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void SetUpDate(int nMaxScore);
	void SetTeamPos(int team_pos);
	int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
private:
	AUILabel			*m_pTxt_Money;	// 钱
	AUILabel			*m_pTxt_Score;	// 分数
	PAUIIMAGEPICTURE m_pPicture[21];
	int	m_iCurSel;
	int cs_team_award_timestamp;    //跨服6v6战队赛季奖励
	A2DSprite *m_pSprite;
	int m_nTeamPos;
};

