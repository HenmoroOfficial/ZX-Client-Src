/********************************************************************
	created:	2013.3.22
	file name:	DlgSettingPostEffect.cpp
	author:		Fu Chonggang
	
	purpose:	
*********************************************************************/

#include "DlgSettingPostEffect.h"
#include "aui/AUICheckBox.h"

#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameUIMan.h"
#include "EC_Global.h"
#include "EC_HostInputFilter.h"
#include "EC_InputCtrl.h"
#include "EC_Secretary.h"
#include "EC_HostPlayer.h"
#include "EC_Configs.h"
#include "EC_SendC2SCmds.h"

#ifndef BACK_VERSION
#include "ElementRenderParam.h"
#endif


#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgSettingPostEffect, CDlgBase)

/*AUI_ON_COMMAND("left",	OnCommandSelAll)
AUI_ON_COMMAND("left",	OnCommandUnSelAll)
AUI_ON_COMMAND("left",	OnCommandCC)
AUI_ON_COMMAND("left",	OnCommandGodRay)
AUI_ON_COMMAND("left",	OnCommandBloom)
AUI_ON_COMMAND("left",	OnCommandAA)
AUI_ON_COMMAND("left",	OnCommandDof)
AUI_ON_COMMAND("left",	OnCommandRadialBlur)
AUI_ON_COMMAND("left",	OnCommandWarp)
AUI_ON_COMMAND("left",	OnCommandFullGlow)
AUI_ON_COMMAND("left",	OnCommandSunMoon)
AUI_ON_COMMAND("left",	OnCommandFlare)
AUI_ON_COMMAND("left",	OnCommandRadialBlurFast)
*/AUI_ON_COMMAND("Chk_Fog",	OnCommandFog)
AUI_ON_COMMAND("confirm",	OnCommandConfirm)

AUI_END_COMMAND_MAP()


CDlgSettingPostEffect::CDlgSettingPostEffect()
{
}

CDlgSettingPostEffect::~CDlgSettingPostEffect()
{

}

bool CDlgSettingPostEffect::OnInitDialog()
{
	return true;
}

void CDlgSettingPostEffect::OnShowDialog()
{

}

void CDlgSettingPostEffect::OnCommandFog(const char* szCommand)
{
	PAUICHECKBOX pChk = (PAUICHECKBOX)GetDlgItem("Chk_Fog");
#ifndef BACK_VERSION
	GetGame()->GetConfigs()->EnableGlobalFog(pChk->IsChecked());
#endif
}

void CDlgSettingPostEffect::OnCommandConfirm(const char* szCommand)
{
#ifndef BACK_VERSION
	m_iMask = A3DPOST_EFFECT_None;
	PAUICHECKBOX pChk = (PAUICHECKBOX)GetDlgItem("Chk_CC");
	if (pChk->IsChecked())
		m_iMask |= A3DPOST_EFFECT_CC;
	pChk = (PAUICHECKBOX)GetDlgItem("Chk_GodRay");
	if (pChk->IsChecked())
		m_iMask |= A3DPOST_EFFECT_GodRay;
	pChk = (PAUICHECKBOX)GetDlgItem("Chk_Bloom");
	if (pChk->IsChecked())
		m_iMask |= A3DPOST_EFFECT_Bloom;
	pChk = (PAUICHECKBOX)GetDlgItem("Chk_AA");
	if (pChk->IsChecked())
		m_iMask |= A3DPOST_EFFECT_AA;
	pChk = (PAUICHECKBOX)GetDlgItem("Chk_Dof");
	if (pChk->IsChecked())
		m_iMask |= A3DPOST_EFFECT_Dof;
	pChk = (PAUICHECKBOX)GetDlgItem("Chk_RadialBlur");
	if (pChk->IsChecked())
		m_iMask |= A3DPOST_EFFECT_RadialBlur;
	pChk = (PAUICHECKBOX)GetDlgItem("Chk_MotionBlur");
	if (pChk->IsChecked())
		m_iMask |= A3DPOST_EFFECT_MotionBlur;
	pChk = (PAUICHECKBOX)GetDlgItem("Chk_Warp");
	if (pChk->IsChecked())
		m_iMask |= A3DPOST_EFFECT_Warp;
	pChk = (PAUICHECKBOX)GetDlgItem("Chk_FullGlow");
	if (pChk->IsChecked())
		m_iMask |= A3DPOST_EFFECT_FullGlow;
	pChk = (PAUICHECKBOX)GetDlgItem("Chk_SunMoon");
	if (pChk->IsChecked())
		m_iMask |= A3DPOST_EFFECT_SunMoon;
	pChk = (PAUICHECKBOX)GetDlgItem("Chk_Flare");
	if (pChk->IsChecked())
		m_iMask |= A3DPOST_EFFECT_Flare;
	pChk = (PAUICHECKBOX)GetDlgItem("Chk_RadialBlurFast");
	if (pChk->IsChecked())
		m_iMask |= A3DPOST_EFFECT_RadialBlurFast;
	pChk = (PAUICHECKBOX)GetDlgItem("Chk_Silhouette");
	if (pChk->IsChecked())
		m_iMask |= A3DPOST_EFFECT_Silhouette;
#endif
}

bool CDlgSettingPostEffect::IsOpen()
{
	PAUICHECKBOX pChk = (PAUICHECKBOX)GetDlgItem("Chk_All");
	return pChk->IsChecked();
}

bool CDlgSettingPostEffect::IsRealTimeLight()
{
	PAUICHECKBOX pChk = (PAUICHECKBOX)GetDlgItem("Chk_Dp");
	return pChk->IsChecked();
}