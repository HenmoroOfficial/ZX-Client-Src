// Filename	: DlgGuildMan.h
// Creator	: Tom Zhou
// Date		: October 25, 2005

#pragma once

#include "DlgBase.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUITextArea.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIProgress.h"
#include "AUI\\AUIStillImageButton.h"

#define CDLGGUILDMAN_FAMILYSKILL_MAX 4

class CDlgGuildMan : public CDlgBase  
{
	friend class CDlgSystem2;
	friend class CDlgGuildMain;
	AUI_DECLARE_EVENT_MAP()
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgGuildMan();
	virtual ~CDlgGuildMan();

	void OnCommand_recover(const char * szCommand);
	void OnCommand_confirm(const char * szCommand);
	void OnCommand_CANCEL(const char * szCommand);
	void OnCommand_sortname(const char * szCommand);
	void OnCommand_sortjob(const char * szCommand);
	void OnCommand_sorttitle(const char * szCommand);
	void OnCommand_sortlevel(const char * szCommand);
	void OnCommand_sortprof(const char * szCommand);
	void OnCommand_sortol(const char * szCommand);
	void OnCommand_sortcon(const char * szCommand);
	void OnCommand_sortdov(const char * szCommand);
	void OnCommand_sortfamily(const char * szCommand);
	void OnCommand_sortcamp(const char * szCommand);
	void OnCommand_Edit(const char * szCommand);
	void OnCommand_gquit(const char * szCommand);
	void OnCommand_gpass(const char * szCommand);
	void OnCommand_gdemiss(const char * szCommand);
	void OnCommand_gappoint(const char * szCommand);
	void OnCommand_gdemote(const char * szCommand);
	void OnCommand_gkick(const char * szCommand);
	void OnCommand_gtitle(const char * szCommand);
	void OnCommand_ginvite(const char * szCommand);
	void OnCommand_genemy(const char * szCommand);
	void OnCommand_refresh(const char * szCommand);
	void OnCommand_gwar(const char * szCommand);
	void OnCommand_gsiegeinfo(const char * szCommand);
	void OnCommand_showfaction(const char * szCommand);
	void OnCommand_showfamily(const char * szCommand);
	void OnCommand_help(const char * szCommand);
	void OnCommand_getmoney(const char* szCommand);
	void OnCommandGTEnterFaction(const char *szCommand);
	void OnCommand_money(const char* szCommand);

	void OnEventRButtonUp(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventLButtonUp(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventLButtonDownSkill(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	void BuildFactionList();
	void BuildFamilyList();
	void SetLastRefreshTime(DWORD dwTime);
	void BuildGuildWarRankList(void *pData);
	void RefreshGuildWarStatus(void *pData);
	void RefreshGuildEnemies(void *pData);

	bool IsFirstOpen()					{ return m_bFirstOpen; }
	void SetFirstOpen(bool bFirstOpen)	{ m_bFirstOpen = bFirstOpen; }

protected:
	PAUILISTBOX m_pList_Member;
	PAUILISTBOX m_pList_Family;
	PAUILABEL m_pTxt_GuildName;
	PAUILABEL m_pTxt_GuildLevel;
	PAUILABEL m_pTxt_GuildProsperity;
	PAUILABEL m_pTxt_GuildHonor;
	PAUILABEL m_pTxt_GuildTerrScore;
	PAUILABEL m_pTxt_PplCap;
	PAUILABEL m_pTxt_PplOnline;
	PAUILABEL m_pTxt_DeleteTime;
	PAUITEXTAREA m_pTxt_Message;
	PAUISTILLIMAGEBUTTON m_pBtn_Recover;
	PAUISTILLIMAGEBUTTON m_pBtn_GuildPass;
	PAUISTILLIMAGEBUTTON m_pBtn_GuildDemission;
	PAUISTILLIMAGEBUTTON m_pBtn_GuildAppoint;
	PAUISTILLIMAGEBUTTON m_pBtn_GuildKick;
	PAUISTILLIMAGEBUTTON m_pBtn_GuildTitle;
	PAUISTILLIMAGEBUTTON m_pBtn_GuildQuit;
	PAUISTILLIMAGEBUTTON m_pBtn_GuildInvite;
	PAUISTILLIMAGEBUTTON m_pBtn_Edit;
	PAUISTILLIMAGEBUTTON m_pBtn_Refresh;
	PAUISTILLIMAGEBUTTON m_pBtn_ShowFaction;
	PAUISTILLIMAGEBUTTON m_pBtn_ShowFamily;
	PAUIPROGRESS m_pPro_GuildHonor;
	PAUIPROGRESS m_pPro_GuildTerrScore;
	PAUIIMAGEPICTURE m_pImg_GuildIcon;
	PAUIIMAGEPICTURE m_pImg_Skill[CDLGGUILDMAN_FAMILYSKILL_MAX];
	PAUILABEL m_pLab_Lev_Skill[CDLGGUILDMAN_FAMILYSKILL_MAX];
	PAUILABEL m_pLab_Exp_Skill[CDLGGUILDMAN_FAMILYSKILL_MAX];

	int m_nSortMode;
	int	m_nGuildWarPage;
	bool m_bFirstOpen;
	DWORD m_dwLastRefreshTime;
	int	m_nGuildWarTime;
	int m_idCurFamily;

	virtual void OnTick();
	virtual bool OnInitDialog();
	
	enum
	{
		SORT_BY_ORDER,
		SORT_BY_NAME,
		SORT_BY_JOB,
		SORT_BY_TITLE,
		SORT_BY_LEVEL,
		SORT_BY_PROFESSION,
		SORT_BY_ONLINE,
		SORT_BY_CONTRIBUTION,
		SORT_BY_DEVOTION,
		SORT_BY_FAMILY,
		SORT_BY_CAMP,
		SORT_BY_MONEY,
	};
};
