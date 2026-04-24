// Filename	: DlgTarget.h
// Creator	: Tom Zhou
// Date		: October 12, 2005

#pragma once

#include "DlgBase.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIProgress.h"
#include "AUI\\AUIImagePicture.h"

#define CDLGTARGET_ST_MAX 24

class A2DSprite;

class CDlgTarget : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP();
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgTarget();
	virtual ~CDlgTarget();

	void OnCommand_CANCEL(const char * szCommand);
	void OnCommand_Speacial(const char * szCommand);
	void OnCommand_GuildBattle(const char * szCommand);

	void OnEventMouseMove(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDown_Size(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonUp_Size(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventRButtonUp(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	void OnEventLButtonDown_Prof(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDown_Monster(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	
protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	virtual bool Release();

	PAUIIMAGEPICTURE	m_pImg_Head;
	PAUILABEL			m_pTxt_Name;
	PAUILABEL			m_pTxt_Lv;
	PAUIPROGRESS		m_pPrg_HP;
	PAUIPROGRESS		m_pPrg_HP_TopLayer;
	PAUIPROGRESS		m_pPrg_MP;
	PAUIPROGRESS		m_pPrg_DP;
	int					m_nShowLevel;
	int					m_nMonsterID;

	int					m_nMouseLastX;
	int					m_nMouseOffset;
	int					m_nMouseOffsetThis;
	
	int					m_iCultivation;
	bool				m_bFengsheng;
	bool				m_bHighLevel;
	static A2DSprite	*m_pA2DSpriteTargetHead;
};

class CDlgTargetHP : public CDlgBase
{
public:
	CDlgTargetHP() {}
	virtual ~CDlgTargetHP() {}

protected:
	virtual bool OnInitDialog();
	virtual void OnTick();

	PAUIPROGRESS	m_pPrgs_Blood;
	PAUILABEL		m_pTxt_Blood;
};
