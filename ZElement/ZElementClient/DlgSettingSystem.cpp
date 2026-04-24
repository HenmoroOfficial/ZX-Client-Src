/********************************************************************
	created:	2005/08/11
	created:	11:8:2005   17:50
	file name:	DlgSettingSystem.cpp
	author:		yaojun
	
	purpose:	
*********************************************************************/

#include "A3DDevice.h"
#include "EC_ShadowRender.h"
#include "DlgSettingSystem.h"
#include "aui/AUIComboBox.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_World.h"
#include "EC_GameUIMan.h"
#include "aui/AUIStillImageButton.h"
#include "aui/AUICheckBox.h"
#ifdef BACK_VERSION
#include <A3DTerrainWater.h>
#else
#include <A3DTerrainWater2.h>
#endif
#include <EC_FullGlowRender.h>
#include <A3DGFXExMan.h>
#include "EL_BackMusic.h"


#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgSettingSystem, CDlgSetting)

AUI_ON_COMMAND("total",		OnCommandTotal)
AUI_ON_COMMAND("bright",	OnCommandBright)
AUI_ON_COMMAND("music",		OnCommandMusic)
AUI_ON_COMMAND("ambience",	OnCommandSFX)
AUI_ON_COMMAND("res",		OnCommandRes)
AUI_ON_COMMAND("speffect",	OnCommandSpEffect)
AUI_ON_COMMAND("Chk_ModelLimit", OnCommandModeLimit)
AUI_ON_COMMAND("modellimit", OnCommandModeLimit)
AUI_ON_COMMAND("IDCANCEL",	OnCommandCancel)


AUI_END_COMMAND_MAP()


CDlgSettingSystem::CDlgSettingSystem()
{
	m_pComboRes = NULL;
	m_pComboFS = NULL;
	m_pSldBright = NULL;
	m_pSldWater = NULL;
	m_pSldSpEffect = NULL;
	m_pSldModeLimit = NULL;

	memset(&m_SysSetting, 0, sizeof(m_SysSetting));
}

CDlgSettingSystem::~CDlgSettingSystem()
{

}

bool CDlgSettingSystem::OnInitDialog()
{
	if (!CDlgSetting::OnInitDialog())
		return false;

	// init control
	m_pSldBright->SetTotal(200, 30);
	RefillResCombo();
	FillFSCombo();

	// init diaplay
	UpdateView();

	return true;
}


void CDlgSettingSystem::SetToDefault()
{
	GetGame()->GetConfigs()->DefaultUserSettings(&m_SysSetting, NULL, NULL);
	
	UpdateData(false);
	UpdateControlState();
	MakeChangeNow();
}

bool CDlgSettingSystem::Validate()
{
	UpdateData(true);
	
	// 窗口模式下不能选择比当前屏幕更大的分辨率。
	if (!m_SysSetting.bFullScreen && 
		(m_SysSetting.iRndWidth > m_pA3DDevice->GetDesktopDisplayMode().nWidth ||
		m_SysSetting.iRndHeight > m_pA3DDevice->GetDesktopDisplayMode().nHeight))
	{
		AUIDialog *pMsgBox = NULL;
		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(299), MB_OK,
			A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		pMsgBox->SetLife(3);
		return false;
	}
	
	// 自定义的非标准窗口尺寸下，不能切换到全屏模式。
	if (m_SysSetting.bFullScreen && 
		m_bIncludeCustomSize && 
		m_pComboRes->GetCurSel() == m_pComboRes->GetCount() - 1)
	{
		AUIDialog *pMsgBox = NULL;
		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(513), MB_OK,
			A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		pMsgBox->SetLife(3);
		return false;
	}

	return true;
}


void CDlgSettingSystem::Apply()
{
	UpdateData(true);

	// save setting
	m_SysSetting.fRndScale = (float)m_SysSetting.iRndHeight / m_SysSetting.iRndWidth;
	GetGame()->GetConfigs()->SetVideoSettings(m_VideoSetting);
	GetGame()->GetConfigs()->SetSystemSettings(m_SysSetting);
	AfxGetGFXExMan()->Resize2DViewport();
	// there's some setting can not be changed by SetSystemSetting()
	// we do it manually
	MakeChangeNow();
}

void CDlgSettingSystem::DoDataExchange(bool bSave)
{
	CDlgSetting::DoDataExchange(bSave);

	DDX_Control("Slider_ModelLimit", m_pSldModeLimit);
	DDX_Control("Slider_SpEffect", m_pSldSpEffect);

	DDX_Control("Combo_Res", m_pComboRes);
	DDX_Control("Combo_Full", m_pComboFS);
	DDX_Control("Slider_Bright", m_pSldBright);
	DDX_Control("Slider_Water", m_pSldWater);

	DDX_Slider(bSave, "Slider_Total", m_SysSetting.nLevel);
	DDX_Slider(bSave, "Slider_Bright", m_SysSetting.iGamma);
	DDX_Slider(bSave, "Slider_Range", m_SysSetting.nSight);
	DDX_Slider(bSave, "Slider_Water", m_SysSetting.nWaterEffect);
	//DDX_Slider(bSave, "Slider_Tree", m_SysSetting.nTreeDetail);
	DDX_Slider(bSave, "Slider_Grass", m_SysSetting.nGrassDetail);
	DDX_Slider(bSave, "Slider_Cloud", m_SysSetting.nCloudDetail);
	DDX_Slider(bSave, "Slider_BackAnimLevel", m_SysSetting.nBackAnimLevel);
	DDX_CheckBox(bSave, "Chk_Mipmap", m_SysSetting.bMipMapBias);
	//DDX_CheckBox(bSave, "Chk_Sunflare", m_SysSetting.bSunFlare);
	DDX_CheckBox(bSave, "Chk_VerticalSync", m_SysSetting.bVSync);
	//DDX_CheckBox(bSave, "Chk_FullGlow", m_SysSetting.bFullGlow);
	DDX_Slider(bSave, "Slider_Music", m_SysSetting.nMusicVol);
	DDX_Slider(bSave, "Slider_Ambience", m_SysSetting.nSoundVol);
	DDX_Slider(bSave, "Slider_ModelLimit", m_VideoSetting.nDistance);
	DDX_CheckBox(bSave, "Chk_ModelLimit", m_VideoSetting.bModelLimit);
	m_pSldModeLimit->Enable(m_VideoSetting.bModelLimit);

	if( bSave )
	{
		int nIndex;
		DDX_RadioButton(bSave, 0, nIndex);
		m_SysSetting.bAdvancedWater = nIndex == 0 ? true : false;
	}
	else
	{
		int nIndex = m_SysSetting.bAdvancedWater ? 0 : 1;
		DDX_RadioButton(bSave, 0, nIndex);
	}
	if (bSave)
	{
		int nIndex = 0;
		DDX_ComboBox(bSave, "Combo_Res", nIndex);
		ComboIndex2WidthHeight(nIndex, m_SysSetting.iRndWidth, m_SysSetting.iRndHeight);
	}
	else
	{
		int nIndex = 0;
		WidthHeight2ComboIndex(nIndex, m_SysSetting.iRndWidth, m_SysSetting.iRndHeight);
		DDX_ComboBox(bSave, "Combo_Res", nIndex);
	}
	if (bSave)
	{
		int nIndex = 0;
		DDX_ComboBox(bSave, "Combo_Full", nIndex);
		m_SysSetting.bFullScreen = nIndex == 1 ? true : false;
	}
	else
	{
		int nIndex = m_SysSetting.bFullScreen ? 1 : 0;
		DDX_ComboBox(bSave, "Combo_Full", nIndex);
	}
	if (bSave)
	{
		int nLevel = 0;
		DDX_Slider(bSave, "Slider_Texture", nLevel);
		m_SysSetting.iTexDetail = 2 - nLevel;
	}
	else
	{
		int nLevel = 2 - m_SysSetting.iTexDetail;
		DDX_Slider(bSave, "Slider_Texture", nLevel);
	}
	if (bSave)
	{
		int nLevel = 0;
		DDX_Slider(bSave, "Slider_Terrain", nLevel);
		m_SysSetting.bSimpleTerrain = nLevel == 0 ? true : false;
	}
	else
	{
		int nLevel = m_SysSetting.bSimpleTerrain ? 0 : 1;
		DDX_Slider(bSave, "Slider_Terrain", nLevel);
	}
	if (bSave)
	{
		int nLevel = 0;
		DDX_Slider(bSave, "Slider_Sample", nLevel);
		m_SysSetting.iSoundQuality = 2 - nLevel;
	}
	else
	{
		int nLevel = 2 - m_SysSetting.iSoundQuality;
		DDX_Slider(bSave, "Slider_Sample", nLevel);
	}
	if (bSave)
	{
		int nLevel = 0;
		DDX_Slider(bSave, "Slider_Shadow", nLevel);
		m_SysSetting.bShadow = nLevel > 0;
		m_SysSetting.bSimpleShadow = nLevel == 1;
	}
	else
	{
		int nLevel = 0;
		if( m_SysSetting.bShadow )
		{
			if( m_SysSetting.bSimpleShadow )
				nLevel = 1;
			else
				nLevel = 2;
		}
		DDX_Slider(bSave, "Slider_Shadow", nLevel);
	}
	if( bSave )
	{
		int nIndex;
		DDX_RadioButton(bSave, 1, nIndex);
		m_SysSetting.bLockCamera = nIndex == 0 ? true : false;
	}
	else
	{
		int nIndex = m_SysSetting.bLockCamera ? 0 : 1;
		DDX_RadioButton(bSave, 1, nIndex);
	}
	if (bSave)
	{
		int nLevel = 0;
		DDX_Slider(bSave, "Slider_SpEffect", nLevel);
		m_VideoSetting.nEffect = nLevel + 1;
	}
	else
	{
		int nLevel = m_VideoSetting.nEffect - 1;
		DDX_Slider(bSave, "Slider_SpEffect", nLevel);
	}
	if( bSave )
	{
		int nIndex;
		DDX_RadioButton(bSave, 3, nIndex);
		m_SysSetting.bCloseShake = nIndex == 1 ? false : true;
	}
	else
	{
		int nIndex = m_SysSetting.bCloseShake ? 2 : 1;
		DDX_RadioButton(bSave, 3, nIndex);
	}
}

void CDlgSettingSystem::UpdateView()
{
	RefillResCombo();

	// read setting
	m_SysSetting = GetGame()->GetConfigs()->GetSystemSettings();
	m_VideoSetting = GetGame()->GetConfigs()->GetVideoSettings();
	
	// show setting
	UpdateData(false);
	UpdateControlState();	
	MakeChangeNow();
}



void CDlgSettingSystem::OnShowDialog()
{
	CDlgSetting::OnShowDialog();
	
	CheckRadioButton(2,1);

	UpdateView();
}

void CDlgSettingSystem::ComboIndex2WidthHeight(int nIndex, int &nWidth, int &nHeight)
{
	nWidth = m_vecRes[nIndex].nWidth;
	nHeight = m_vecRes[nIndex].nHeight;
}

void CDlgSettingSystem::WidthHeight2ComboIndex(int &nIndex, int nWidth, int nHeight)
{
	for (int i = 0; i < (int)m_vecRes.size(); i++)
	{
		if (m_vecRes[i].nWidth == nWidth &&
			m_vecRes[i].nHeight == nHeight)
		{
			nIndex = i;
			return;
		}
	}
	nIndex = 0;
}



void CDlgSettingSystem::RefillResCombo()
{
	m_vecRes.clear();
	m_pComboRes->ResetContent();

	m_bIncludeCustomSize = true;
	A3DDEVFMT devFmt = m_pA3DEngine->GetA3DDevice()->GetDevFormat();
	for (int i = 0; i < m_pA3DDevice->GetNumDisplayModes(); i++ )
	{
		const D3DDISPLAYMODE &dm = m_pA3DDevice->GetDisplayMode(i);
		if( dm.Width > 400 && dm.Height > 400 )
		{
			Res res = {dm.Width, dm.Height};
			m_vecRes.push_back(res);
			
			ACString strItem;
			strItem.Format(_AL("%dx%d"), dm.Width, dm.Height);
			m_pComboRes->AddString(strItem);

			if (dm.Width == (UINT)devFmt.nWidth && dm.Height == (UINT)devFmt.nHeight)
				m_bIncludeCustomSize = false;
		}
	}

	if (m_bIncludeCustomSize)
	{
		Res res = {devFmt.nWidth, devFmt.nHeight};
		m_vecRes.push_back(res);

		// 自定义尺寸
		m_pComboRes->AddString(GetGameUIMan()->GetStringFromTable(512));
	}
}

void CDlgSettingSystem::FillFSCombo()
{
	// 窗口模式
	m_pComboFS->AddString(GetGameUIMan()->GetStringFromTable(297));
	// 全屏模式
	m_pComboFS->AddString(GetGameUIMan()->GetStringFromTable(298));
}

void CDlgSettingSystem::OnCommandTotal(const char *szCommand)
{
	UpdateData(true);

	switch (m_SysSetting.nLevel) {
	case 0:// 
		{
			m_SysSetting.nSight = 0;
			m_SysSetting.nWaterEffect = 0;
#ifdef BACK_VERSION
			if (GetGame()->GetShadowRender()->CanDoShadow())
#endif
			{
				//m_SysSetting.bSimpleTerrain = true;
				m_SysSetting.bShadow = false;
				m_SysSetting.bSimpleShadow = false;
				m_SysSetting.nCloudDetail = 0;
			}
			m_SysSetting.nTreeDetail = 0;
			m_SysSetting.nGrassDetail = 0;
			m_SysSetting.nBackAnimLevel = 0;
			m_SysSetting.bMipMapBias = false;
			m_SysSetting.bVSync      = false;
			m_SysSetting.bSunFlare = false;
			m_SysSetting.bFullGlow = false;
			m_SysSetting.bAdvancedWater = false;
			m_SysSetting.bSimpleTerrain = true;
			m_SysSetting.bCloseShake = true;
		}
		break;
	case 1:
		{
			m_SysSetting.nSight = 1;
#ifdef BACK_VERSION
			if (GetWorld()->GetTerrainWater()->CanRelfect())
#endif
				m_SysSetting.nWaterEffect = 1;
#ifdef BACK_VERSION
			if (GetGame()->GetShadowRender()->CanDoShadow())
#endif
			{
//				m_SysSetting.bSimpleTerrain = false;
				m_SysSetting.bShadow = true;
				m_SysSetting.bSimpleShadow = true;
				m_SysSetting.nCloudDetail = 1;
			}
			m_SysSetting.nTreeDetail = 1;
			m_SysSetting.nGrassDetail = 1;
			m_SysSetting.nBackAnimLevel = 1;
			m_SysSetting.bMipMapBias = true;
			m_SysSetting.bVSync      = true;
			m_SysSetting.bSunFlare = false;
			m_SysSetting.bFullGlow = false;
			m_SysSetting.bAdvancedWater = false;
			m_SysSetting.bSimpleTerrain = false;
			m_SysSetting.bCloseShake = false;
		}
		break;
	case 2:
		{
			m_SysSetting.nSight = 2;
#ifdef BACK_VERSION
			if (GetGame()->GetGameRun()->GetWorld()->GetTerrainWater()->CanRelfect())
#endif
				m_SysSetting.nWaterEffect = 1;
#ifdef BACK_VERSION
			if (GetGame()->GetShadowRender()->CanDoShadow())
#endif
			{
//				m_SysSetting.bSimpleTerrain = false;
				m_SysSetting.bShadow = true;
				m_SysSetting.bSimpleShadow = true;
				m_SysSetting.nCloudDetail = 2;
			}
			m_SysSetting.nTreeDetail = 2;
			m_SysSetting.nGrassDetail = 2;
			m_SysSetting.nBackAnimLevel = 2;
			m_SysSetting.bMipMapBias = true;
			m_SysSetting.bVSync      = true;
			if (GetGame()->GetGameRun()->GetFullGlowRender()->CanDoFullGlow())
			{
				m_SysSetting.bSunFlare = true;
				m_SysSetting.bFullGlow = true;
				m_SysSetting.bAdvancedWater = true;
			}
			m_SysSetting.bSimpleTerrain = false;
			m_SysSetting.bCloseShake = false;
		}
		break;
	case 3:
		{
			m_SysSetting.nSight = 3;
#ifdef BACK_VERSION
			if (GetGame()->GetGameRun()->GetWorld()->GetTerrainWater()->CanRelfect())
#endif
				m_SysSetting.nWaterEffect = 1;
#ifdef BACK_VERSION
			if (GetGame()->GetShadowRender()->CanDoShadow())
#endif
			{
//				m_SysSetting.bSimpleTerrain = false;
				m_SysSetting.bShadow = true;
				m_SysSetting.bSimpleShadow = false;
				m_SysSetting.nCloudDetail = 3;
			}
			m_SysSetting.nTreeDetail = 3;
			m_SysSetting.nGrassDetail = 3;
			m_SysSetting.nBackAnimLevel = 3;
			m_SysSetting.bMipMapBias = true;
			m_SysSetting.bVSync      = true;
			if (GetGame()->GetGameRun()->GetFullGlowRender()->CanDoFullGlow())
			{
				m_SysSetting.bSunFlare = true;
				m_SysSetting.bFullGlow = true;
				m_SysSetting.bAdvancedWater = true;
			}
			m_SysSetting.bSimpleTerrain = false;
			m_SysSetting.bCloseShake = false;
		}
		break;
	case 4:
		{
			m_SysSetting.nSight = 4;
#ifdef BACK_VERSION
			if (GetGame()->GetGameRun()->GetWorld()->GetTerrainWater()->CanRelfect())
#endif
				m_SysSetting.nWaterEffect = 2;
#ifdef BACK_VERSION
			if (GetGame()->GetShadowRender()->CanDoShadow())
#endif
			{
//				m_SysSetting.bSimpleTerrain = false;
				m_SysSetting.bShadow = true;
				m_SysSetting.bSimpleShadow = false;
				m_SysSetting.nCloudDetail = 4;
			}
			m_SysSetting.nTreeDetail = 4;
			m_SysSetting.nGrassDetail = 4;
			m_SysSetting.nBackAnimLevel = 4;
			m_SysSetting.bMipMapBias = true;
			m_SysSetting.bVSync      = true;
			if (GetGame()->GetGameRun()->GetFullGlowRender()->CanDoFullGlow())
			{
				m_SysSetting.bSunFlare = true;
				m_SysSetting.bFullGlow = true;
				m_SysSetting.bAdvancedWater = true;
			}
			m_SysSetting.bSimpleTerrain = false;
			m_SysSetting.bCloseShake = false;
		}
		break;
	}

	UpdateData(false);
}

void CDlgSettingSystem::MakeChangeNow()
{
	m_pA3DDevice->SetGammaLevel(m_SysSetting.iGamma);
	GetGame()->GetBackMusic()->SetVolume(m_SysSetting.nMusicVol);
	GetGame()->GetBackMusic()->SetSFXVolume(m_SysSetting.nMusicVol);
	GetGame()->GetBackMusic()->SetCommentatorVolume(m_SysSetting.nMusicVol);	
	m_pA3DEngine->GetAMSoundEngine()->SetVolume(m_SysSetting.nSoundVol);
}

void CDlgSettingSystem::OnCommandBright(const char *szCommand)
{
	UpdateData(true);

	MakeChangeNow();

	ACString strText;
	strText.Format(_AL("%d"), m_pA3DDevice->GetGammaLevel());
	m_pSldBright->SetHint(strText);
}

void CDlgSettingSystem::OnCommandMusic(const char *szCommand)
{
	UpdateData(true);
	MakeChangeNow();
}

void CDlgSettingSystem::OnCommandSFX(const char *szCommand)
{
	UpdateData(true);
	MakeChangeNow();
}


void CDlgSettingSystem::OnCommandRes(const char *szCommand)
{
	UpdateData(true);
}

void CDlgSettingSystem::UpdateControlState()
{
	UpdateData(true);

#ifdef BACK_VERSION
	if( GetWorld()->GetTerrainWater()->CanRelfect() )
#endif
	{
		GetDlgItem("Water_1")->SetColor(A3DCOLORRGB(255, 255, 255));
		GetDlgItem("Water_2")->SetColor(A3DCOLORRGB(255, 255, 255));
		GetDlgItem("Water_3")->SetColor(A3DCOLORRGB(255, 255, 255));
		GetDlgItem("Slider_Water")->Enable(true);
	}
#ifdef BACK_VERSION
	else
	{
		GetDlgItem("Water_1")->SetColor(A3DCOLORRGB(128, 128, 128));
		GetDlgItem("Water_2")->SetColor(A3DCOLORRGB(128, 128, 128));
		GetDlgItem("Water_3")->SetColor(A3DCOLORRGB(128, 128, 128));
		GetDlgItem("Slider_Water")->Enable(false);
	}

	if( GetGame()->GetShadowRender()->CanDoShadow() )
	{
		GetDlgItem("Slider_Shadow")->Enable(true);
		GetDlgItem("Slider_Terrain")->Enable(true);
	}
	else
#endif
	{
//		GetDlgItem("Slider_Shadow")->Enable(false);
//		((PAUISLIDER)GetDlgItem("Slider_Shadow"))->SetLevel(0);
//		GetDlgItem("Slider_Terrain")->Enable(false);
		GetDlgItem("Slider_Terrain")->Enable(true);
//		((PAUISLIDER)GetDlgItem("Slider_Cloud"))->SetLevel(0);
//		GetDlgItem("Slider_Cloud")->Enable(false);
	}

	if( GetGameRun()->GetFullGlowRender()->CanDoFullGlow() )
	{
		//GetDlgItem("Chk_Sunflare")->Enable(true);
		//GetDlgItem("Rdo_AdvancedWaterOpen")->Enable(true);
		//GetDlgItem("Rdo_AdvancedWaterClose")->Enable(true);
		//GetDlgItem("Sunflare")->SetColor(A3DCOLORRGB(255, 255, 255));
		//GetDlgItem("AdvancedWater")->SetColor(A3DCOLORRGB(255, 255, 255));
	}
	else
	{
		//GetDlgItem("Chk_Sunflare")->Enable(false);
		//GetDlgItem("Rdo_AdvancedWaterOpen")->Enable(false);
		//GetDlgItem("Rdo_AdvancedWaterClose")->Enable(false);
		//GetDlgItem("Sunflare")->SetColor(A3DCOLORRGB(128, 128, 128));
		//GetDlgItem("AdvancedWater")->SetColor(A3DCOLORRGB(128, 128, 128));
	}

	ACString strText;
	strText.Format(_AL("%d"), m_pA3DDevice->GetGammaLevel());
	m_pSldBright->SetHint(strText);
}

void CDlgSettingSystem::OnCommandCancel(const char *szCommand)
{
	CDlgSetting::OnCommandCancel(NULL);
	
	UpdateView();
	MakeChangeNow();
}

void CDlgSettingSystem::OnCommandSpEffect(const char *szCommand)
{
	UpdateData(true);
	
	ACString strHint = GetStringFromTable(523 + m_VideoSetting.nEffect - 1);
	m_pSldSpEffect->SetHint(strHint);
}

void CDlgSettingSystem::OnCommandModeLimit(const char *szCommand)
{
	UpdateData(true);
	m_pSldModeLimit->Enable(m_VideoSetting.bModelLimit);
	
	ACString strText;
	strText.Format(GetGameUIMan()->GetStringFromTable(528), 
		static_cast<int>(GetGame()->GetConfigs()->CalcPlayerVisRadius(m_VideoSetting.nDistance) + 0.5f));
	m_pSldModeLimit->SetHint(strText);
}
