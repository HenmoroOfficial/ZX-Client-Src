// File		: DlgSkillEdit.cpp
// Creator	: Xiao Zhou
// Date		: 2005/12/20

#include "AFI.h"
#include "DlgSkillEdit.h"
#include "DlgSkill.h"
#include "DlgSkillMan.h"
#include "EC_GameUIMan.h"
#include "EC_Skill.h"
#include "EC_Game.h"
#include "EC_Configs.h"
#include "EC_HostPlayer.h"
#include "DlgSettingVideo.h"
#include "LuaEvent.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgSkillEdit, CDlgBase)

AUI_ON_COMMAND("confirm",		OnCommandConfirm)
AUI_ON_COMMAND("moveleft",		OnCommandMoveLeft)
AUI_ON_COMMAND("moveright",		OnCommandMoveRight)
AUI_ON_COMMAND("edit",			OnCommandEdit)
AUI_ON_COMMAND("delete",		OnCommandDelete)
AUI_ON_COMMAND("new",			OnCommandNew)


AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgSkillEdit, CDlgBase)

AUI_ON_EVENT("Item_*",		WM_LBUTTONDOWN,		OnEventLButtonDown_Item)
AUI_ON_EVENT("Item_*",		WM_LBUTTONDBLCLK,	OnEventLButtonDown_Item)
AUI_ON_EVENT("Edt_Item_*",	WM_LBUTTONDOWN,		OnEventLButtonDown_Item)
AUI_ON_EVENT("Edt_Item_*",	WM_LBUTTONDBLCLK,	OnEventLButtonDown_Item)
AUI_ON_EVENT("Special_*",	WM_LBUTTONDOWN,		OnEventLButtonDown_Item)
AUI_ON_EVENT("Special_*",	WM_LBUTTONDBLCLK,	OnEventLButtonDown_Item)
AUI_ON_EVENT("Img_Icon*",	WM_LBUTTONDOWN,		OnEventLButtonDown_Icon)

AUI_END_EVENT_MAP()

CDlgSkillEdit::CDlgSkillEdit()
{
	m_nItemSelect = 0;
	m_nIcon = 1;
	m_pImg_Item = NULL;
	m_pTxt_Name = NULL;
	for (int i = 0; i<EC_COMBOSKILL_NUM; i++)
	{
		m_pImg_SkillCont[i] = NULL;
	}
}

CDlgSkillEdit::~CDlgSkillEdit()
{
}

bool CDlgSkillEdit::OnInitDialog()
{
	m_nComboSelect = 0;

	DDX_Control("Img_Item", m_pImg_Item);
	DDX_Control("Txt_Name", m_pTxt_Name);
	for (int i = 0; i<EC_COMBOSKILL_NUM; i++)
	{
		AString strName;
		strName.Format("Item_%d", i + 1);
		DDX_Control(strName, m_pImg_SkillCont[i]);
	}

	return true;
}

void CDlgSkillEdit::OnCommandConfirm(const char *szCommand)
{
	EC_VIDEO_SETTING setting = GetGame()->GetConfigs()->GetVideoSettings();
	setting.comboSkill[m_nComboSelect - 1].nIcon = m_nIcon;
	int i;
	int j = 0;
	for (i = 0; ; i++)
	{
		AString strName;
		strName.Format("Edt_Item_%d", i + 1);
		PAUIIMAGEPICTURE pImage = static_cast<PAUIIMAGEPICTURE>(GetDlgItem(strName));
		if (!pImage) break;

		CECSkill *pSkill = (CECSkill *)pImage->GetDataPtr("ptr_CECSkill");
		int iType = pImage->GetData();
		if( iType == 0 && pSkill )
		{
			setting.comboSkill[m_nComboSelect - 1].idSkill[j] = pSkill->GetSkillID();
			j++;
		}
		else
		{
			setting.comboSkill[m_nComboSelect - 1].idSkill[j] = -iType;
			j++;
		}
	}
	GetGame()->GetConfigs()->SetVideoSettings(setting);
	UpdateComboSkill();
//	GetGameUIMan()->m_pDlgSettingVideo->UpdateView();
}

void CDlgSkillEdit::OnCommandMoveLeft(const char *szCommand)
{
	if( m_nItemSelect == 0 )
		return;

	AString strName;
	strName.Format("Img_Icon%d", m_nItemSelect);
	PAUIIMAGEPICTURE pImage1 = static_cast<PAUIIMAGEPICTURE>(GetDlgItem(strName));
	strName.Format("Img_Icon%d", m_nItemSelect - 1);
	PAUIIMAGEPICTURE pImage2 = static_cast<PAUIIMAGEPICTURE>(GetDlgItem(strName));
	if( pImage1 && pImage2 )
	{
		CECSkill *pSkill1 = (CECSkill *)pImage1->GetDataPtr("ptr_CECSkill");
		CECSkill *pSkill2 = (CECSkill *)pImage2->GetDataPtr("ptr_CECSkill");
		int iType1 = pImage1->GetData();
		int iType2 = pImage2->GetData();
		if( iType2 != 0 )
			SetSpecialIcon(pImage1, iType2);
		else
			GetGameUIMan()->m_pDlgSkill->SetImage(pImage1, pSkill2);
		if( iType1 != 0 )
			SetSpecialIcon(pImage2, iType1);
		else
			GetGameUIMan()->m_pDlgSkill->SetImage(pImage2, pSkill1);
		SelectSkill(m_nItemSelect - 1);
	}
}

void CDlgSkillEdit::OnCommandMoveRight(const char *szCommand)
{
	if( m_nItemSelect == 0 )
		return;

	AString strName;
	strName.Format("Img_Icon%d", m_nItemSelect);
	PAUIIMAGEPICTURE pImage1 = static_cast<PAUIIMAGEPICTURE>(GetDlgItem(strName));
	strName.Format("Img_Icon%d", m_nItemSelect + 1);
	PAUIIMAGEPICTURE pImage2 = static_cast<PAUIIMAGEPICTURE>(GetDlgItem(strName));
	if( pImage1 && pImage2 )
	{
		CECSkill *pSkill1 = (CECSkill *)pImage1->GetDataPtr("ptr_CECSkill");
		CECSkill *pSkill2 = (CECSkill *)pImage2->GetDataPtr("ptr_CECSkill");
		int iType1 = pImage1->GetData();
		int iType2 = pImage2->GetData();
		if( iType2 != 0 )
			SetSpecialIcon(pImage1, iType2);
		else
			GetGameUIMan()->m_pDlgSkill->SetImage(pImage1, pSkill2);
		if( iType1 != 0 )
			SetSpecialIcon(pImage2, iType1);
		else
			GetGameUIMan()->m_pDlgSkill->SetImage(pImage2, pSkill1);
		SelectSkill(m_nItemSelect + 1);
	}
}

void CDlgSkillEdit::OnCommandEdit(const char *szCommand)
{
	EC_VIDEO_SETTING setting = GetGame()->GetConfigs()->GetVideoSettings();
	if( m_nComboSelect == 0 )
	{
		for (int i = 0; i < EC_COMBOSKILL_NUM; i++)
			if( setting.comboSkill[i].nIcon == 0 )
			{
				m_nComboSelect = i + 1;
				break;
			}
	}

	if(m_nComboSelect == 0)
	{
		return;
	}

	UpdateSkill();
	LuaEvent::FirstEditContinuousSkill();
}

void CDlgSkillEdit::OnCommandNew(const char *szCommand)
{
	EC_VIDEO_SETTING setting = GetGame()->GetConfigs()->GetVideoSettings();
	m_nComboSelect =0;
	
	for (int i = 0; i < EC_COMBOSKILL_NUM; i++)
		if( setting.comboSkill[i].nIcon == 0 )
		{
			m_nComboSelect = i + 1;
			break;
		}
		if(m_nComboSelect == 0)
		{
			return;
		}
		
	UpdateSkill();
}

void CDlgSkillEdit::OnCommandDelete(const char *szCommand)
{
	if( m_nComboSelect < 0 || m_nComboSelect > EC_COMBOSKILL_NUM )
		return;
	
	EC_VIDEO_SETTING setting = GetGame()->GetConfigs()->GetVideoSettings();
	setting.comboSkill[m_nComboSelect - 1].nIcon = 0;
	m_nComboSelect = 0;
	GetGame()->GetConfigs()->SetVideoSettings(setting);
	UpdateSkill();
	//	GetGameUIMan()->m_pDlgSettingVideo->UpdateView();
}

void CDlgSkillEdit::OnCommandCancel(const char *szCommand)
{
	Show(false);
}

void CDlgSkillEdit::OnShowDialog()
{
	UpdateSkill();
}

void CDlgSkillEdit::OnEventLButtonDown_Item(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();

	GetGameUIMan()->m_ptLButtonDown.x = GET_X_LPARAM(lParam) - p->X;
	GetGameUIMan()->m_ptLButtonDown.y = GET_Y_LPARAM(lParam) - p->Y;
	GetGameUIMan()->InvokeDragDrop(this, pObj, GetGameUIMan()->m_ptLButtonDown);
}

void CDlgSkillEdit::OnEventLButtonDown_Icon(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	int iSlot = atoi(pObj->GetName() + strlen("Img_Icon"));
	m_pImg_Item->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_SKILLGRP], iSlot + 1);
	m_nIcon = iSlot;
}

void CDlgSkillEdit::DragDropItem(PAUIDIALOG pDlgSrc, PAUIOBJECT pObjSrc, PAUIOBJECT pObjOver)
{
	if( pObjSrc == pObjOver )
	{
		PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)pObjOver;
		pImg->SetData(0);
		pImg->SetDataPtr(NULL, "ptr_CECSkill");
		pImg->ClearCover();
		pImg->SetHint(_AL(""));
	}
	else if( !pObjOver )
	{
		if( !(strstr(pObjSrc->GetName(), "Special")) )
			GetGameUIMan()->m_pDlgSkill->SetImage((PAUIIMAGEPICTURE)pObjSrc, NULL);
	}
	else
	{
		int iSlot = atoi(pObjOver->GetName() + strlen("Edt_Item_"));
		if( strstr(pObjSrc->GetName(), "Special_") )
		{
			int iType = atoi(pObjSrc->GetName() + strlen("Special_"));
			SetSpecialIcon((PAUIIMAGEPICTURE)pObjOver, iType);
		}
		else
		{
			CECSkill *pSkill = (CECSkill *)pObjSrc->GetDataPtr("ptr_CECSkill");
			unsigned long iType;
			AString strTypeName;
			// 新加的封神技能为圆形，是否圆形这个信息记录在GetData()里面，所以这样处理，记在技能身上更好点
			pObjSrc->ForceGetData(iType, strTypeName);
			if( pDlgSrc != this )
			{
				if( iType==1 || iType==2)
					SetSpecialIcon((PAUIIMAGEPICTURE)pObjOver, iType);
				else
					GetGameUIMan()->m_pDlgSkill->SetImage((PAUIIMAGEPICTURE)pObjOver, pSkill);
			}
			else
			{
				CECSkill *pSkill1 = (CECSkill *)pObjOver->GetDataPtr("ptr_CECSkill");
				int iType1 = pObjOver->GetData();
				if( iType != 0)
					SetSpecialIcon((PAUIIMAGEPICTURE)pObjOver, iType);
				else
					GetGameUIMan()->m_pDlgSkill->SetImage((PAUIIMAGEPICTURE)pObjOver, pSkill);
				if( iType1 != 0)
					SetSpecialIcon((PAUIIMAGEPICTURE)pObjSrc, iType1);
				else
					GetGameUIMan()->m_pDlgSkill->SetImage((PAUIIMAGEPICTURE)pObjSrc, pSkill1);
			}
		}
	}
}

void CDlgSkillEdit::SelectSkill(int n)
{
	AString strName;
	if( m_nItemSelect == n )
	{
		strName.Format("Img_Icon%d", n);
		PAUIIMAGEPICTURE pImage = static_cast<PAUIIMAGEPICTURE>(GetDlgItem(strName));
		if( pImage )
			pImage->SetColor(A3DCOLORRGB(255, 255, 255));
		m_nItemSelect = 0;
	}
	else
	{
		if( m_nItemSelect != 0 )
		{
			strName.Format("Img_Icon%d", m_nItemSelect);
			PAUIIMAGEPICTURE pImage = static_cast<PAUIIMAGEPICTURE>(GetDlgItem(strName));
			if( pImage )
				pImage->SetColor(A3DCOLORRGB(255, 255, 255));
		}
		m_nItemSelect = n;
		strName.Format("Img_Icon%d", m_nItemSelect);
		PAUIIMAGEPICTURE pImage = static_cast<PAUIIMAGEPICTURE>(GetDlgItem(strName));
		if( pImage )
			pImage->SetColor(A3DCOLORRGB(160, 160, 160));
	}
}

void CDlgSkillEdit::SetSpecialIcon(PAUIIMAGEPICTURE pImage, int iType)
{
	if( iType == 1 )
	{
		pImage->SetCover(
			GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_SKILLGRP], 0);
		pImage->SetDataPtr((void*)1, "ptr_CECSkill");
		pImage->SetData(1);
		pImage->SetHint(GetStringFromTable(796));
	}
	else
	{
		pImage->SetCover(
			GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_SKILLGRP], 1);
		pImage->SetDataPtr((void*)1, "ptr_CECSkill");
		pImage->SetData(2);
		pImage->SetHint( GetStringFromTable(797));
	}
}

void CDlgSkillEdit::UpdateSkill()
{
	int i;
	for (i=0; i<EC_COMBOSKILL_NUM; i++)
	{
		m_pImg_SkillCont[i]->SetColor(A3DCOLORRGB(255, 255, 255));
	}

	if (m_nComboSelect != 0)
		m_pImg_SkillCont[m_nComboSelect-1]->SetColor(A3DCOLORRGB(160, 160, 160));

	EC_VIDEO_SETTING setting = GetGame()->GetConfigs()->GetVideoSettings();
	
	ACString strText;
	strText.Format(GetStringFromTable(795), m_nComboSelect - 1);
	m_pTxt_Name->SetText(strText);
	if( m_nComboSelect == 0 ||setting.comboSkill[m_nComboSelect - 1].nIcon == 0 )
	{
		m_nIcon = 1;
		if (m_nComboSelect == 0)
			m_pImg_Item->SetCover(NULL, 0);
		else
			m_pImg_Item->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_SKILLGRP], 2);
		for (i = 1; ; i++)
		{
			AString strName;
			strName.Format("Edt_Item_%d", i);
			PAUIIMAGEPICTURE pImage = static_cast<PAUIIMAGEPICTURE>(GetDlgItem(strName));
			if (!pImage) break;
			
			GetGameUIMan()->m_pDlgSkill->SetImage(pImage, NULL);
		}
	}
	else
	{
		m_nIcon = setting.comboSkill[m_nComboSelect - 1].nIcon;
		m_pImg_Item->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_SKILLGRP], 
			m_nIcon + 1);
		for (i = 0; i < EC_COMBOSKILL_LEN; i++)
		{
			AString strName;
			strName.Format("Edt_Item_%d", i + 1);
			PAUIIMAGEPICTURE pImage = static_cast<PAUIIMAGEPICTURE>(GetDlgItem(strName));
			if (!pImage) break;
			
			if( setting.comboSkill[m_nComboSelect - 1].idSkill[i] > 0 )
			{
				CECSkill *pSkill = GetHostPlayer()->GetSkill(setting.comboSkill[m_nComboSelect - 1].idSkill[i]);
				GetGameUIMan()->m_pDlgSkill->SetImage(pImage, pSkill);
			}
			else
			{
				GetGameUIMan()->m_pDlgSkill->SetImage(pImage, NULL);
				int iType = -setting.comboSkill[m_nComboSelect - 1].idSkill[i];
				pImage->SetData(iType);
				if( setting.comboSkill[m_nComboSelect - 1].idSkill[i] < 0 )
					SetSpecialIcon(pImage, iType);
			}
		}
	}
	for (i = 1; ; i++)
	{
		AString strName;
		strName.Format("Img_Icon%02d", i);
		PAUIIMAGEPICTURE pImage = static_cast<PAUIIMAGEPICTURE>(GetDlgItem(strName));
		if (!pImage) break;
		
		pImage->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_SKILLGRP], i + 1);
	}
	if( m_nItemSelect != 0 )
	{
		AString strName;
		strName.Format("Item_%d", m_nItemSelect);
		PAUIIMAGEPICTURE pImage = static_cast<PAUIIMAGEPICTURE>(GetDlgItem(strName));
		if( pImage )
			pImage->SetColor(A3DCOLORRGB(255, 255, 255));
	}
	PAUIIMAGEPICTURE pImage;
	pImage = (PAUIIMAGEPICTURE)GetDlgItem("Special_1");
	SetSpecialIcon(pImage, 1);
	pImage = (PAUIIMAGEPICTURE)GetDlgItem("Special_2");
	SetSpecialIcon(pImage, 2);
	m_nItemSelect = 0;
	UpdateComboSkill();
}

void CDlgSkillEdit::SelectComboSkill(int n)
{
	AString strName;
	if( m_nComboSelect == n )
	{
		strName.Format("Item_%d", n);
		PAUIIMAGEPICTURE pImage = static_cast<PAUIIMAGEPICTURE>(GetDlgItem(strName));
		if( pImage )
			pImage->SetColor(A3DCOLORRGB(255, 255, 255));
		m_nComboSelect = 0;
	}
	else
	{
		if( m_nComboSelect != 0 )
		{
			strName.Format("Item_%d", m_nComboSelect);
			PAUIIMAGEPICTURE pImage = static_cast<PAUIIMAGEPICTURE>(GetDlgItem(strName));
			if( pImage )
				pImage->SetColor(A3DCOLORRGB(255, 255, 255));
		}
		m_nComboSelect = n;
		strName.Format("Item_%d", m_nComboSelect);
		PAUIIMAGEPICTURE pImage = static_cast<PAUIIMAGEPICTURE>(GetDlgItem(strName));
		if( pImage )
			pImage->SetColor(A3DCOLORRGB(160, 160, 160));
	}
	UpdateSkill();
}

void CDlgSkillEdit::UpdateComboSkill()
{
	EC_VIDEO_SETTING setting = GetGame()->GetConfigs()->GetVideoSettings();
	for(int i = 0; i< EC_COMBOSKILL_NUM; i++) 
	{
		if( setting.comboSkill[i].nIcon != 0 )
		{
			m_pImg_SkillCont[i]->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_SKILLGRP], 
				setting.comboSkill[i].nIcon + 1);
			m_pImg_SkillCont[i]->SetData(i + 1);
			m_pImg_SkillCont[i]->SetDataPtr((void*)1);
			ACHAR szText[20];
			a_sprintf(szText, GetStringFromTable(795), i);
			m_pImg_SkillCont[i]->SetHint(szText);
		}
		else
		{
			m_pImg_SkillCont[i]->SetCover(NULL, -1);
			m_pImg_SkillCont[i]->SetData(0);
			m_pImg_SkillCont[i]->SetDataPtr(NULL);
			m_pImg_SkillCont[i]->SetHint(_AL(""));
		}
	}
}

void CDlgSkillEdit::OnTick()
{
	GetDlgItem("Btn_Confirm")->Enable(m_nComboSelect>0);
}

bool CDlgSkillEdit::Render()
{
	return CDlgBase::Render();
}