// File		: DlgCityBuilding.h
// Creator	: Guo Jie
// Date		: 2008/2/27

#pragma once

#include "DlgBase.h"

namespace S2C {
	struct war_construction_info;
	struct war_contribution_info;
}
struct WAR_ROLE_CONFIG;
struct NPC_WAR_BUY_ARCHER_SERVICE;

class CDlgCityBuilding : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgCityBuilding();
	virtual ~CDlgCityBuilding();

	void OnCommandConstruct(const char* szCmd);
	void OnCommandUpgrade(const char* szCmd);
	void OnCommandDonateRes(const char* szCmd);

	void OnCommandPrevPage(const char* szCmd);
	void OnCommandNextPage(const char* szCmd);
	void OnCommandRefreshList(const char* szCmd);

	void OnEventLButtonDown_Icon(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	
	// From NPC
	void OnCityBuildService();
	void OnBuyArcherService(const NPC_WAR_BUY_ARCHER_SERVICE* p);

	// From Server
	void OnGetConstructionInfo(const S2C::war_construction_info* p);
	void OnGetContributionInfo(const S2C::war_contribution_info* p);

protected:
	virtual void OnTick();
	virtual bool OnInitDialog();

	bool FillBuildingDlg(PAUIDIALOG pDlg);
	bool FillBuildingResInfo(PAUIDIALOG pDlg);
	bool FillCityDlg();
	bool FillCityResInfo();
	bool FillCitySelectedInfoArea();
	void FillCityConstructButton(bool bEnable);

	S2C::war_construction_info* m_pConsInfo; // construction info cache
	WAR_ROLE_CONFIG** m_pWarRoleCfgs; // building essenses
	bool	m_bInBuildService;
	DWORD	m_lastRefreshAllConsInfoTime;
	DWORD	m_lastRefreshSingleConsInfoTime;
	int		m_curPage, m_maxPage;	// 0, ...
	int		m_curSelectedBuilding;	// 1, ...
	int		m_idRes[2];
};