/********************************************************************
	created:	2005/08/11
	created:	11:8:2005   17:49
	file name:	DlgSettingVideo.h
	author:		yaojun
	
	purpose:	
*********************************************************************/

#pragma once

#include "DlgSetting.h"
#include "EC_Configs.h"
#include "AUI\\AUICheckBox.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIStillImageButton.h"

class AUISlider;


class CDlgSettingVideo : public CDlgSetting  
{
	AUI_DECLARE_COMMAND_MAP()
	enum
	{
		BOSSKEY_MIN = 0,
		BOSSKEY_ALTWAVE = BOSSKEY_MIN,
		BOSSKEY_NUMMULT,
	};
	
protected:
	int m_iBossKey;
	AUICheckBox *m_pChkBossKeyEnable;
	AUILabel *m_pTxtBossKey;
	AUIStillImageButton *m_pBtnBossKeyLeft;
	AUIStillImageButton *m_pBtnBossKeyRight;

	AUICheckBox *m_pCheckAll;
	AUICheckBox *m_pCheckNone;

	abase::vector<AUICheckBox*> m_pVideoCheckList;

	char        m_cWindowScale;

protected:

	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();

	void DoDataExchange(bool bSave);
	void OnCommandBossKeyLeft(const char *szCommand);
	void OnCommandBossKeyRight(const char *szCommand);
	void OnCommandBossKeyEnable(const char *szCommand);

	void OnCommandCheckAll(const char* szCommand);
	void OnCommandCheckNone(const char* szCommand);

	void UpdateBossKey();

	void CheckAll(bool isCheck = true);
	void CheckNone(bool isCheck = true);

	bool isCheckAll(bool isCheck = true);
	bool isCheckNone();

public:
	CDlgSettingVideo();
	virtual ~CDlgSettingVideo();
	
	virtual void SetToDefault();
	virtual void Apply();
	virtual void UpdateView();

};

