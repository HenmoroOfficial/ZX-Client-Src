// Filename	:	DlgSettingAudioBasic.h
// Creator	:	Fu Chonggang
// Date		:	Sep 11, 2009

#pragma once

#pragma once

#include "DlgSetting.h"
#include "EC_Configs.h"

class AUILabel;
class AUICheckBox;
class AUIStillImageButton;

class CDlgSettingAudioBasic : public CDlgSetting  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
public:
	CDlgSettingAudioBasic();
	virtual ~CDlgSettingAudioBasic();
	
	virtual void SetToDefault();
	virtual void Apply();
	virtual void UpdateView();
	
protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	void DoDataExchange(bool bSave);
public:
	void OnCommandMusic(const char *szCommand);
	void OnCommandSFX(const char *szCommand);
	void OnCommandSample(const char *szCommand);
	void MakeChangeNow();
	
};