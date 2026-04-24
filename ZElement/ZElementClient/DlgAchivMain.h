// Filename	: DlgAchivMain.h
// Creator	: Fu Chonggang
// Date		: 2010/1/27

#pragma once

#include "DlgBase.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUITreeView.h"
#include "AUI\\AUITextArea.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIRadioButton.h"
#include "AUI\\AUIProgress.h"

class CECPlayer;
struct sAchievement;

class CDlgAchivMain : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

	enum
	{
		ACHIEVEMENT_TYPE_MAIN = 6,
		ACHIEVEMENT_TYPE_SUB  = 10,
	};

public:
	CDlgAchivMain();
	virtual ~CDlgAchivMain();

	void OnCommandRdoMain(const char* szCommand);
	void OnCommandRdoSub(const char* szCommand);
	void OnCommandCancel(const char* szCommand);
	void SetTarget(int iTarget);
	CECPlayer* GetTargetPlayer() { return m_pPlayer; }
	void CalcTargetPos();
	void ShowAchiv(sAchievement* pAchiv);

protected:
	virtual void OnTick();
	virtual	bool OnInitDialog();
	virtual void OnShowDialog();
	void DynamicAction(int index = 0);
	void UpdateInfo();

	PAUILABEL				m_pLab_Point;
	PAUILABEL				m_pLab_DoneNum;
	PAUIRADIOBUTTON			m_pRdo_Main[ACHIEVEMENT_TYPE_MAIN];
	PAUIPROGRESS			m_pPrg_Main[ACHIEVEMENT_TYPE_MAIN];
	PAUILABEL				m_pLab_Main[ACHIEVEMENT_TYPE_MAIN];
	PAUIIMAGEPICTURE		m_pImg_Main[ACHIEVEMENT_TYPE_MAIN];
	PAUISTILLIMAGEBUTTON	m_pGA_Main[ACHIEVEMENT_TYPE_MAIN];
	PAUIRADIOBUTTON			m_pRdo_Sub[ACHIEVEMENT_TYPE_SUB];
	int						m_iTarget;
	CECPlayer*				m_pPlayer;
	
	//for dynamic effect
	int		m_nCurIndex;
	int		m_nMovIndex;
	int		m_nTargetPosY;
};
