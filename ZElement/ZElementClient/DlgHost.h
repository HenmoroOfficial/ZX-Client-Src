// Filename	: DlgHost.h
// Creator	: Tom Zhou
// Date		: October 13, 2005

#pragma once

#include "DlgBase.h"
#include "EC_Configs.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUICheckBox.h"
#include "AUI\\AUIProgress.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"

class CDlgHost : public CDlgBase  
{
	friend class CDlgSettingVideo;

	AUI_DECLARE_EVENT_MAP();
	AUI_DECLARE_COMMAND_MAP()

public:
	enum
	{
		BATTLE_STATE_NORMAL,
		BATTLE_STATE_INTEAM,
		BATTLE_STATE_INBATTLE,
	};

	CDlgHost();
	virtual ~CDlgHost();
	virtual bool Render(void);

	void OnCommand_closeHP(const char * szCommand);
	void OnCommand_sight(const char * szCommand);
	void OnCommand_player(const char * szCommand);
	void OnCommand_mapmonster(const char * szCommand);
	void OnCommand_surround(const char * szCommand);
	void OnCommand_headhp(const char * szCommand);
	void OnCommand_headmp(const char * szCommand);
	void OnCommand_headexp(const char * szCommand);
	void OnCommand_lock(const char * szCommand);
	void OnCommand_stop(const char * szCommand);
	void OnCommand_close(const char * szCommand);
	void OnCommand_pk(const char * szCommand);
	void OnCommand_battleinfo(const char * szCommand);
	void OnCommand_battlescore(const char * szCommand);
	void OnCommand_siegescore(const char * szCommand);
	void OnCommand_TerrWarInfo(const char * szCommand);
	void OnCommand_AwardOnline(const char * szCommand);
	void OnCommand_AccountComplete(const char * szCommand);
	void OnCommand_CANCEL(const char * szCommand);
	void OnCommand_GuildWar(const char* szCommand);

	void OnEventMouseMove(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDown_Size(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonUp_Size(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventRButtonUp_Size(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	
	void SetInstanceInfo(int nState, int nTime, int nLineID = 0, int nMapID = 0);	// 设置剧情战场信息
	void SetBattleInfo(int nState, int nTime, int nLineID = 0, int nFlag = 0,int nMapID = 0);		// 设置其它战场信息 //2012-8-22 by add zy 添加跨服标志,顺序改动
	int GetInstanceTime()		{ return m_nInstanceTime; }
	int GetBattleState()	{ return m_nBattleState; }
	int GetBattleTime()		{ return m_nBattleTime; }
	void SetBattleTime(int iTime) { m_nBattleTime = iTime; }
//	bool IsEquipCompare()	{ return m_pChk_EquipCompare->IsChecked(); }
	void UpdateSkillInfo(bool bStart, int num, char* content);
	void SetSendFlowQueueCount(DWORD dw) { m_dwTimeSend = dw ;};
protected:
	static EC_SYSTEM_SETTING m_ss, m_ssTemp;
	static EC_VIDEO_SETTING m_vs, m_vsTemp;

	PAUIIMAGEPICTURE m_pImg_VIP;
	PAUIIMAGEPICTURE m_pImg_Head;
	PAUIIMAGEPICTURE m_pImgCombatMask;
	PAUIIMAGEPICTURE m_pImgProfession;
	PAUILABEL m_pTxt_Name;
	PAUILABEL m_pTxt_Prof;
	PAUILABEL m_pTxt_LV;
	PAUIPROGRESS m_pPrgs_HP;
	PAUIPROGRESS m_pPrgs_MP;
	PAUIPROGRESS m_pPrgs_DP;
	PAUIPROGRESS m_pPrgs_EXP;
	PAUIIMAGEPICTURE m_pPic_MinusExp;
	PAUILABEL m_pTxt_HP;
	PAUILABEL m_pTxt_MP;
	PAUILABEL m_pTxt_DP;
	PAUILABEL m_pTXT_EXP;
	PAUIIMAGEPICTURE m_pImg_Leader;
	PAUICHECKBOX m_pChk_Player;
	PAUICHECKBOX m_pChk_Surround;
	PAUICHECKBOX m_pChk_Sight;
	PAUICHECKBOX m_pChk_MapMonster;
	PAUICHECKBOX m_pChk_BattleInfo;
	PAUICHECKBOX m_pChk_BattleScore;
	PAUICHECKBOX m_pChk_SiegeScore;
//	PAUICHECKBOX m_pChk_EquipCompare;
	abase::vector<PAUIIMAGEPICTURE> m_vecImgAPOn;
	abase::vector<PAUIIMAGEPICTURE> m_vecImgAPOff;
	PAUIIMAGEPICTURE m_pImgSkillColor[5];
	PAUISTILLIMAGEBUTTON m_pBtnTerr;
	PAUISTILLIMAGEBUTTON m_pBtnAwardOnline;

	int m_nMouseLastX;
	int m_nMouseOffset;
	int m_nMouseOffsetThis;

	int	m_nBattleState;
	int m_nBattleTime;
	int m_nBattleLineID;
	int m_nBattleMapID;
	
	int	m_nInstanceState;
	int m_nInstanceTime;
	int m_nInstanceLineID;
	int m_nInstanceMapID;
	
	int m_iCultivation;
	bool m_bFengsheng;
	bool m_bHighLevel;

	bool m_bGuideSkillStartFlag;
	DWORD m_nSendTime;
	bool m_bIsStart;
	DWORD	m_dwTimeSend;

	void RefreshHostStats();
	void RefreshStatusBar();
	virtual bool OnInitDialog();
	virtual void OnTick();
};
