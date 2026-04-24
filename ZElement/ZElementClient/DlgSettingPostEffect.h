/********************************************************************
	created:	2013.3.22
	file name:	DlgSettingPostEffect.h
	author:		Fu Chonggang
	
	purpose:	
*********************************************************************/

#pragma once

#include "DlgBase.h"

class CDlgSettingPostEffect : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgSettingPostEffect();
	virtual ~CDlgSettingPostEffect();

	int GetMask() { return m_iMask; }
	bool IsOpen();
	bool IsRealTimeLight();

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();

/*	void OnCommandSelAll(const char *szCommand);
	void OnCommandUnSelAll(const char *szCommand);
	void OnCommandCC(const char *szCommand);
	void OnCommandGodRay(const char *szCommand);
	void OnCommandBloom(const char *szCommand);
	void OnCommandAA(const char *szCommand);
	void OnCommandDof(const char *szCommand);
	void OnCommandRadialBlur(const char *szCommand);
	void OnCommandWarp(const char *szCommand);
	void OnCommandFullGlow(const char *szCommand);
	void OnCommandSunMoon(const char *szCommand);
	void OnCommandFlare(const char *szCommand);
	void OnCommandRadialBlurFast(const char *szCommand);
*/	void OnCommandSilhouette(const char *szCommand);
	
	void OnCommandFog(const char *szCommand);
	void OnCommandConfirm(const char *szCommand);

private:
	int m_iMask;
};

