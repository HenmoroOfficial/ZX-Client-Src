// Filename	:	DlgFengshenSoul.cpp
// Creator	:	Fu Chonggang
// Date		:	Sep 7, 2010

#include "DlgFengshenSoul.h"
#include "DlgCharacter.h"
#include "AUI\\AUIEditBox.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIRadioButton.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIProgress.h"
#include "AUI\\AUIListBox.h"
#include "EC_GameUIMan.h"
#include "EC_Model.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_World.h"
#include "EC_ManPlayer.h"
#include "EC_Global.h"
#include "EC_InputCtrl.h"
#include "EC_ModelRender.h"
#include "EC_HostPlayer.h"
#include "EC_Secretary.h"
#include "EC_PortraitRender.h"
#include "EC_SendC2SCmds.h"
#include "elementdataman.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgFengshenSoul, CDlgBase)

AUI_ON_COMMAND("Rdo_*",					OnCommand_Page)
AUI_ON_COMMAND("Btn_LvUp",				OnCommand_Upgrade)
AUI_ON_COMMAND("Btn_Help",				OnCommand_Help)
AUI_ON_COMMAND("Btn_Detail",			OnCommand_Detail)
AUI_ON_COMMAND("IDCANCEL",				OnCommand_CANCEL)
AUI_ON_COMMAND("Btn_Cancel",			OnCommand_CANCEL)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgFengshenSoul, CDlgBase)

AUI_ON_EVENT("Img_Soul",		WM_LBUTTONDOWN,			OnEventLButtonDown)
AUI_ON_EVENT("Img_Soul",		WM_LBUTTONDBLCLK,		OnEventLButtonDown)

AUI_END_EVENT_MAP()

const static char* s_camp[] = { " ", "仙", "魔", "仙魔", "佛", "仙佛", "魔佛", "仙魔佛"};	//GFX名字，不需要本地化
const static char* s_level[] = { "一", "二", "三", "四", "五", "六", "七", "八", "九"};		//GFX名字，不需要本地化
const int CDlgFengshenSoul::STAGELEVEL = 9;
const int CDlgFengshenSoul::UPBOUNDLEVEL = 81;

CDlgFengshenSoul::CDlgFengshenSoul()
{
	m_pModel = NULL;
	m_pPortraitRender = NULL;
}

CDlgFengshenSoul::~CDlgFengshenSoul()
{
}

bool CDlgFengshenSoul::Release()
{
	A3DRELEASE(m_pModel);
	A3DRELEASE(m_pPortraitRender);
	return CDlgBase::Release();
}

bool CDlgFengshenSoul::OnInitDialog()
{
	m_pShape4 = (AUIEditBox *)GetDlgItem("Shape4");
	m_pTitle = (AUILabel *)GetDlgItem("Title");
	m_pTxt_DeTaoism = (AUILabel *)GetDlgItem("Txt_DeTaoism");
	m_pTxt_DeBuddhism = (AUILabel *)GetDlgItem("Txt_DeBuddhism");
	m_pTxt_DeEvil = (AUILabel *)GetDlgItem("Txt_DeEvil");
	m_pRdo_1 = (AUIRadioButton *)GetDlgItem("Rdo_1");
	m_pRdo_2 = (AUIRadioButton *)GetDlgItem("Rdo_2");
	m_pRdo_3 = (AUIRadioButton *)GetDlgItem("Rdo_3");
	m_pRdo_4 = (AUIRadioButton *)GetDlgItem("Rdo_4");
	m_pBtn_Cancel = (AUIStillImageButton *)GetDlgItem("Btn_Cancel");
	m_pBtn_Upgrade = (AUIStillImageButton *)GetDlgItem("Btn_LvUp");
	m_pLab_Combine = (AUILabel *)GetDlgItem("Lab_Combine");
	m_pTxt_Sp = (AUILabel *)GetDlgItem("Txt_SP");
	m_pImg_Soul = (AUIImagePicture *)GetDlgItem("Img_Soul");
	m_pImg_Item = (AUIImagePicture *)GetDlgItem("Img_Badge");
	m_pPrg_DeityExp = (AUIProgress *)GetDlgItem("Prg_DeityExp");
	m_pLab_DeityExp = (AUILabel *)GetDlgItem("Txt_Exp");
	m_pPrg_Dp = (AUIProgress *)GetDlgItem("Prg_Dp");
	m_pLab_Dp = (AUILabel *)GetDlgItem("Txt_Num");
	m_pLab_Lv = (AUILabel *)GetDlgItem("Txt_Lv");

	m_nCultivation = 0;
	m_nDeityLv = 0;
	m_pModel = new CECModel;
	
	m_pModel->SetPos(A3DVECTOR3(0));
	m_pModel->SetDirAndUp(A3DVECTOR3(0, 0, -1.0f), A3DVECTOR3(0, 1.0f, 0));
	m_pModel->SetAutoUpdateFlag(false);
	m_pPortraitRender		= NULL;
	m_pPortraitRender = new CECPortraitRender;
	m_pPortraitRender->Init(100);

	return true;
}

void CDlgFengshenSoul::OnShowDialog()
{
	if (m_szName=="Win_Fengshen_Soul")
	{
		if (GetHostPlayer()->GetBasicProps().iDeityLevel > 0)
		{
			GetGameUIMan()->CommonMessage("Win_Character", "stop_flash", 0, 0);
			GetDlgItem("Lab_Mismatch")->Show(false);
			GetDlgItem("Txt_Lv")->Show(true);
			GetDlgItem("Lab_Lv")->Show(true);
			GetDlgItem("Txt_Num")->Show(true);
			GetDlgItem("Txt_Exp")->Show(true);
			GetDlgItem("Lab_SP")->Show(true);
			GetDlgItem("Txt_SP")->Show(true);
			GetDlgItem("Btn_LvUp")->Show(true);
			GetDlgItem("Lab_Num")->Show(true);
			GetDlgItem("Lab_Exp")->Show(true);
			GetDlgItem("Prg_DeityExp")->Show(true);
			GetDlgItem("Prg_Dp")->Show(true);
			GetDlgItem("Txt_DeEvil")->Show(true);
			GetDlgItem("Lab_DeEvil")->Show(true);
			GetDlgItem("Txt_DeBuddhism")->Show(true);
			GetDlgItem("Lab_DeBuddhism")->Show(true);
			GetDlgItem("Txt_DeTaoism")->Show(true);
			GetDlgItem("Lab_DeTaoism")->Show(true);
			GetDlgItem("Img_Soul")->Show(true);
			GetDlgItem("Img_Badge")->Show(true);
			GetDlgItem("Txt_Lv")->Show(true);
			GetDlgItem("Shape_Bg")->Show(true);
			GetDlgItem("Shape5")->Show(true);
			GetDlgItem("Btn_Detail")->Show(true);
			CECSecretary* pSecretary = g_pGame->GetGameRun()->GetSecretary();
			pSecretary->SetEventState(CECSecretary::FIRST_DEITY_LEVELUP);
			_UpdateSoulInfo();
			OnCommand_Detail("");
			GetGameUIMan()->GetDialog("Win_FengshenAttributeDetail")->Show(true);
		}
		else
		{
			GetDlgItem("Lab_Mismatch")->Show(true);
			GetDlgItem("Txt_Lv")->Show(false);
			GetDlgItem("Lab_Lv")->Show(false);
			GetDlgItem("Txt_Num")->Show(false);
			GetDlgItem("Txt_Exp")->Show(false);
			GetDlgItem("Lab_SP")->Show(false);
			GetDlgItem("Txt_SP")->Show(false);
			GetDlgItem("Btn_LvUp")->Show(false);
			GetDlgItem("Lab_Num")->Show(false);
			GetDlgItem("Lab_Exp")->Show(false);
			GetDlgItem("Prg_DeityExp")->Show(false);
			GetDlgItem("Prg_Dp")->Show(false);
			GetDlgItem("Txt_DeEvil")->Show(false);
			GetDlgItem("Lab_DeEvil")->Show(false);
			GetDlgItem("Txt_DeBuddhism")->Show(false);
			GetDlgItem("Lab_DeBuddhism")->Show(false);
			GetDlgItem("Txt_DeTaoism")->Show(false);
			GetDlgItem("Lab_DeTaoism")->Show(false);
			GetDlgItem("Img_Soul")->Show(false);
			GetDlgItem("Img_Badge")->Show(false);
			GetDlgItem("Txt_Lv")->Show(false);
			GetDlgItem("Shape_Bg")->Show(false);
			GetDlgItem("Shape5")->Show(false);
			GetDlgItem("Btn_Detail")->Show(false);
		}
		CheckRadioButton(1, 5);
	}
}

void CDlgFengshenSoul::OnTick()
{
	if (!m_pModel->IsLoaded())
		return;
	m_pModel->Tick(GetGame()->GetTickTime());
	if(m_szName=="Win_Fengshen_Soul")
	{
		_UpdateSoulInfo();
		
		A3DCombActDynData* modelData = m_pModel->GetCurComAct();
		if (modelData)
			m_nStatus = m_pModel->GetCurActionUserData();
		
		int x, y;
		g_pGame->GetGameRun()->GetInputCtrl()->GetMousePos(&x, &y);
		A3DRECT rc = m_pImg_Soul->GetRect();
		if ( rc.PtInRect(x, y) && STATUS_CIRCLE==m_nStatus )
		{
			EnterState(STATUS_MOUSEON);
		}
		else if ( !rc.PtInRect(x, y) && STATUS_MOUSEON==m_nStatus )
		{
			EnterState(STATUS_CIRCLE);
		}
		m_pImg_Soul->SetRenderCallback(	DlgDeityModelRender, (DWORD)m_pModel, MRT_FENGSHEN, 0);
	}
	else
		m_pImg_Soul->SetRenderCallback(	DlgDeityModelRender, (DWORD)m_pModel, MRT_FENGSHEN_EP, 1);

	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_FengshenAttributeDetail");
	if (pDlg->IsShow() && (int)pDlg->GetData("deity_level")!=GetHostPlayer()->GetBasicProps().iDeityLevel)
	{
		pDlg->Show(false);
		OnCommand_Detail("");
	}
}

void CDlgFengshenSoul::_UpdateSoulInfo()
{
	if (m_szName!="Win_Fengshen_Soul")
	{
		assert(0);
		return;
	}
	CECHostPlayer *pHost = GetHostPlayer();
	const ROLEBASICPROP &rbp = pHost->GetBasicProps();
	const ROLEEXTPROP &rep = pHost->GetExtendProps();
	const ROLEEXTPROP& ep = pHost->GetExtendProps();
	const ROLEBASICPROP& bp = pHost->GetBasicProps();
	
	if (m_nDeityLv!=rbp.iDeityLevel || m_nCultivation!=pHost->GetCultivation())
	{
		m_nCultivation = pHost->GetCultivation();
		if (m_pModel->IsLoaded())
		{
			m_pModel->Release();
		}
		
		AString strText;
		int iCultivation = GetHostPlayer()->GetCultivation();
		AUIOBJECT_SETPROPERTY p;
		if ( 1 == m_nCultivation )
		{
			sprintf(p.fn, "Window\\Fengshen_Gfxbg_Taosim.tga");
		}
		else if ( 2 == m_nCultivation )
		{
			sprintf(p.fn, "Window\\Fengshen_Gfxbg_Evil.tga");
		}
		else
		{
			sprintf(p.fn, "Window\\Fengshen_Gfxbg_Buddhism.tga");
		}
		PAUIIMAGEPICTURE pImgBack = (PAUIIMAGEPICTURE)GetDlgItem("Shape_Bg");
		pImgBack->SetProperty("Frame Image", &p);
		m_nDeityLv = rbp.iDeityLevel;
		ACString strLv;
		strLv.Format(_AL("%d"), m_nDeityLv);
		m_pLab_Lv->SetText(strLv);
		strText.Format("Models\\图标界面\\%s\\%s%s阶.ecm", s_camp[m_nCultivation], s_camp[m_nCultivation], s_level[(m_nDeityLv-1)/STAGELEVEL]);
		if ( !m_pModel->Load(strText) )
		{
			AString strLog;
			strLog.Format("Failed to load %s", strText);
			AUI_ReportError(__LINE__, strLog);
		}
		m_pModel->SetPos(A3DVECTOR3(100.0f, 0.0f, 0.0f));
		m_pModel->SetGfxUseLOD(false);
		EnterState(STATUS_CIRCLE);
	}

	ACString strText;
	if (m_nDeityLv>0)
		strText.Format(GetGameUIMan()->GetStringFromTable(11010),
			GetGameUIMan()->GetStringFromTable(11001+(bp.iDeityLevel-1)%STAGELEVEL),
			GetGameUIMan()->GetStringFromTable(11000+10*GetHostPlayer()->GetCultivation()+(bp.iDeityLevel+STAGELEVEL-1)/STAGELEVEL));
	else
		strText.Empty();
	m_pTitle->SetText(strText);
	strText.Format(_AL("%d"), ep.deity_power);
	m_pTxt_Sp->SetText(strText);
	strText.Format(_AL("%.1f%%"), ep.cult_attack[0] / 10.0f);
	m_pTxt_DeTaoism->SetText(strText);
	strText.Format(_AL("%.1f%%"), ep.cult_attack[1] / 10.0f);
	m_pTxt_DeEvil->SetText(strText);
	strText.Format(_AL("%.1f%%"), ep.cult_attack[2] / 10.0f);
	m_pTxt_DeBuddhism->SetText(strText);

	double maxEXP = pHost->GetDeityLevelUpExp(rbp.iDeityLevel + 1);
	strText.Format(_AL("%.0f/%.0f"), rbp.deityExp, maxEXP);
	m_pLab_DeityExp->SetText(strText);
	m_pPrg_DeityExp->SetProgress(int(double(rbp.deityExp) * AUIPROGRESS_MAX / maxEXP));
	if (bp.iDeityLevel==UPBOUNDLEVEL || bp.iDeityLevel==0)
	{
		if (m_pBtn_Upgrade->IsShow())
			m_pBtn_Upgrade->Show(false);
	}
	else
	{
		if(!m_pBtn_Upgrade->IsShow())
			m_pBtn_Upgrade->Show(true);
		m_pBtn_Upgrade->Enable(rbp.deityExp>=maxEXP && bp.iDeityLevel%STAGELEVEL!=0);
		m_pBtn_Upgrade->SetFlash(m_pBtn_Upgrade->IsEnabled());
		if (rbp.deityExp>=maxEXP)
		{
			m_pBtn_Upgrade->SetHint(GetGameUIMan()->GetStringFromTable(bp.iDeityLevel%STAGELEVEL!=0?3680:3681));
		}
		else
			m_pBtn_Upgrade->SetHint(GetGameUIMan()->GetStringFromTable(3682));
	}
	strText.Format(_AL("%d/%d"), rbp.iCurDP, rep.max_dp);
	m_pLab_Dp->SetText(strText);
	m_pPrg_Dp->SetProgress(int(double(rbp.iCurDP) * AUIPROGRESS_MAX / rep.max_dp));

}

bool CDlgFengshenSoul::Render()
{
	if( !CDlgBase::Render() )
		return false;
	return true;
}

int CDlgFengshenSoul::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="init")
	{
		m_nCultivation = wParam;
		m_nDeityLv = lParam;
		if (m_nDeityLv==0)
		{
			Show(false);
			return 1;
		}
		if (m_pModel->IsLoaded())
		{
			m_pModel->Release();
		}
		
		AString strText;
		AUIOBJECT_SETPROPERTY p;
		if ( 1 == m_nCultivation )
		{
			sprintf(p.fn, "Window\\Fengshen_Gfxbg_Taosim.tga");
		}
		else if ( 2 == m_nCultivation )
		{
			sprintf(p.fn, "Window\\Fengshen_Gfxbg_Evil.tga");
		}
		else
		{
			sprintf(p.fn, "Window\\Fengshen_Gfxbg_Buddhism.tga");
		}
		PAUIIMAGEPICTURE pImgBack = (PAUIIMAGEPICTURE)GetDlgItem("Shape_Bg");
		pImgBack->SetProperty("Frame Image", &p);
		
		ACString strTitle;
		if (m_nDeityLv>0)
			strTitle.Format(GetGameUIMan()->GetStringFromTable(11010),
			GetGameUIMan()->GetStringFromTable(11001+(m_nDeityLv-1)%STAGELEVEL),
			GetGameUIMan()->GetStringFromTable(11000+10*m_nCultivation+(m_nDeityLv+STAGELEVEL-1)/STAGELEVEL));
		else
			strTitle.Empty();
		m_pTitle->SetText(strTitle);
		strText.Format("Models\\图标界面\\%s\\%s%s阶.ecm", s_camp[m_nCultivation], s_camp[m_nCultivation], s_level[(m_nDeityLv-1)/STAGELEVEL]);
		if ( !m_pModel->Load(strText) )
		{
			AString strLog;
			strLog.Format("Failed to load %s", strText);
			AUI_ReportError(__LINE__, strLog);
		}
		m_pModel->SetPos(A3DVECTOR3(-100.0f, 0.0f, 0.0f));
		m_pModel->SetGfxUseLOD(false);
		EnterState(STATUS_CIRCLE);
		if (!IsShow())
		{
			Show(true);
		}
	}
	return 1;
}

void CDlgFengshenSoul::OnCommand_Page(const char * szCommand)
{
	GetGameUIMan()->GetDialog("Win_Char_Nature")->Show(false);
	Show(false);
	PAUIDIALOG pDlg = NULL;
	switch(GetCheckedRadioButton(1))
	{
	case 1:
		pDlg = GetGameUIMan()->GetDialog("Win_Character");
		break;
	case 2:
		pDlg = GetGameUIMan()->GetDialog("Win_Credit");
		break;
	case 3:
		pDlg = GetGameUIMan()->GetDialog("Win_Char_TitleBox");
		break;
	case 4:
		pDlg = GetGameUIMan()->GetDialog("Win_CharInfo");
		break;
	case 5:
		pDlg = GetGameUIMan()->GetDialog("Win_Fengshen_Soul");
		break;
	}
	if( pDlg )
	{
		pDlg->Show(true);
		pDlg->SetPosEx(GetPos().x, GetPos().y);
	}
}

void CDlgFengshenSoul::OnCommand_Upgrade(const char * szCommand)
{
	c2s_SendCmdDeityLevelUp();
}

void CDlgFengshenSoul::OnCommand_Help(const char * szCommand)
{
	//PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Fengshen_Soul_Help");
	//pDlg->Show(!pDlg->IsShow());
	AString strDlg = "Win_Fengshen_Soul_Help";
	GetGameUIMan()->CommonMessage("Win_HelpInfo","open",(WPARAM)(&strDlg),0);
}

void CDlgFengshenSoul::OnCommand_Detail(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_FengshenAttributeDetail");
	pDlg->Show(!pDlg->IsShow());
	pDlg->AlignTo(this, AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT,
		 AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);

	PAUILISTBOX pLsg = (PAUILISTBOX)pDlg->GetDlgItem("Lst_Detail");
	pLsg->ResetContent();

	static const int DIRTY_CHARACTER_CONFIG_ID = 293;
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	DEITY_CHARACTER_CONFIG* pConfig	= (DEITY_CHARACTER_CONFIG*)pDB->get_data_ptr(DIRTY_CHARACTER_CONFIG_ID, ID_SPACE_CONFIG, DataType);
	if (DataType!=DT_DEITY_CHARACTER_CONFIG)
		return;

	int iDeityLv = GetHostPlayer()->GetBasicProps().iDeityLevel - 1;
	pDlg->SetData(GetHostPlayer()->GetBasicProps().iDeityLevel, "deity_level");

	ACString strText;
	strText.Format(_AL("^80ffff%s\t^2bfffa%d"), GetGameUIMan()->GetStringFromTable(11100), pConfig->deity_config[iDeityLv].hp);
	pLsg->AddString(strText);
	strText.Format(_AL("^80ffff%s\t^2bfffa%d"), GetGameUIMan()->GetStringFromTable(11101), pConfig->deity_config[iDeityLv].mp);
	pLsg->AddString(strText);
	strText.Format(_AL("^80ffff%s\t^2bfffa%d"), GetGameUIMan()->GetStringFromTable(11102), pConfig->deity_config[iDeityLv].dmg);
	pLsg->AddString(strText);
	strText.Format(_AL("^80ffff%s\t^2bfffa%d"), GetGameUIMan()->GetStringFromTable(11103), pConfig->deity_config[iDeityLv].defense);
	pLsg->AddString(strText);
	strText.Format(_AL("^80ffff%s\t^2bfffa%d"), GetGameUIMan()->GetStringFromTable(11104), pConfig->deity_config[iDeityLv].deity_point);
	pLsg->AddString(strText);
	strText.Format(_AL("^80ffff%s\t^2bfffa%d"), GetGameUIMan()->GetStringFromTable(11105), pConfig->deity_config[iDeityLv].deity_power);
	pLsg->AddString(strText);
	strText.Format(_AL("^80ffff%s\t^2bfffa%d"), GetGameUIMan()->GetStringFromTable(11106), pConfig->deity_config[iDeityLv].anti_stunt);
	pLsg->AddString(strText);
	strText.Format(_AL("^80ffff%s\t^2bfffa%d"), GetGameUIMan()->GetStringFromTable(11107), pConfig->deity_config[iDeityLv].anti_weak);
	pLsg->AddString(strText);
	strText.Format(_AL("^80ffff%s\t^2bfffa%d%%"), GetGameUIMan()->GetStringFromTable(11112), pConfig->deity_config[iDeityLv].anti_slow);
	pLsg->AddString(strText);
	strText.Format(_AL("^80ffff%s\t^2bfffa%d"), GetGameUIMan()->GetStringFromTable(11109), pConfig->deity_config[iDeityLv].anti_silence);
	pLsg->AddString(strText);
	strText.Format(_AL("^80ffff%s\t^2bfffa%d"), GetGameUIMan()->GetStringFromTable(11110), pConfig->deity_config[iDeityLv].anti_sleep);
	pLsg->AddString(strText);
	strText.Format(_AL("^80ffff%s\t^2bfffa%d"), GetGameUIMan()->GetStringFromTable(11111), pConfig->deity_config[iDeityLv].anti_twist);
	pLsg->AddString(strText);
	strText.Format(_AL("^80ffff%s\t^2bfffa%d"), GetGameUIMan()->GetStringFromTable(11113), pConfig->deity_config[iDeityLv].talent_point);
	pLsg->AddString(strText);
	strText.Format(_AL("^80ffff%s\t^2bfffa%d"), GetGameUIMan()->GetStringFromTable(11114), pConfig->deity_config[iDeityLv].skill_point);
	pLsg->AddString(strText);
}

void CDlgFengshenSoul::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
	if (m_szName=="Win_Fengshen_Soul")
		GetGameUIMan()->GetDialog("Win_FengshenAttributeDetail")->Show(false);
}

void CDlgFengshenSoul::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if (m_szName=="Win_Fengshen_Soul")
	{
		if ( m_nStatus==STATUS_CIRCLE || m_nStatus==STATUS_MOUSEON )
		{
			EnterState(STATUS_CLICK);
		}
		else if ( m_nStatus==STATUS_CLICK )
		{
			EnterState(STATUS_CLICK_SECONDTIME);
		}
	}
}

void CDlgFengshenSoul::EnterState(int stat)
{
	if (m_nCultivation==0||m_nDeityLv==0)
	{
		return;
	}
	m_nStatus = stat;
	bool b = m_pModel->PlayActionByName(_GetSuitGfxName(), 1.0f, true, 200, true, m_nStatus);
	if (!b)
	{
		OutputDebugStringA(_GetSuitGfxName());
		OutputDebugStringA("\n");
	}

	if (stat==STATUS_CLICK_SECONDTIME)
		b = m_pModel->QueueAction(_GetSuitGfxName(STATUS_CIRCLE), 200, STATUS_CIRCLE);
	switch (m_nStatus)
	{
	case STATUS_CIRCLE:
	case STATUS_CLICK_SECONDTIME:
		m_pImg_Item->Show(false);
		break;
	case STATUS_MOUSEON:
		m_pImg_Item->Show(true);
		m_pImg_Item->SetAlpha(150);
		break;
	case STATUS_CLICK:
		m_pImg_Item->Show(true);
		m_pImg_Item->SetAlpha(255);
		break;
	}
}

AString CDlgFengshenSoul::_GetSuitGfxName(int status)
{
	if (status==-1)
		status = m_nStatus;
	const static char* s_fengshen_action_name[] = 
	{
		"%s%s阶_循环",
		"%s%s阶_鼠标指向",
		"%s%s阶_点击",
		"%s%s阶_加注",
	};
	AString strRes;
	strRes.Format(s_fengshen_action_name[status], s_camp[m_nCultivation], s_level[(m_nDeityLv-1)/STAGELEVEL]);
	return strRes;
}