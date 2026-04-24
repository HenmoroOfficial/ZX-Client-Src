// Filename	: DlgGardenQShop.cpp
// Creator	: Fu Chonggang
// Date		: 2009/08/21

#include "AFI.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIScroll.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIDef.h"
#include "AUI\\AUICTranslate.h"
#include "A2DSprite.h"
#include "DlgGardenQShop.h"
#include "DlgInputNO.h"
#include "EC_GameUIMan.h"
#include "EC_HTPDataType.h"
#include "EC_HTC2SRawCmds.h"
#include "EC_Game.h"
#include "EC_Global.h"
#include "EC_IvtrItem.h"
#include "hometowntemplate.h"


#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgGardenQShop, CDlgBase)

AUI_ON_COMMAND("Btn_Confirm",	OnCommand_Confirm)
AUI_ON_COMMAND("Btn_Close",		OnCommand_Close)
AUI_ON_COMMAND("Btn_Cancel",	OnCommand_Close)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgGardenQShop, CDlgBase)

AUI_ON_EVENT("Img_*",				WM_LBUTTONDOWN,		OnEventLButtonDownItem)
AUI_ON_EVENT("*",					WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_ON_EVENT(NULL,					WM_MOUSEWHEEL,		OnEventMouseWheel)

AUI_END_EVENT_MAP()

A2DSprite* CDlgGardenQShop::m_pA2DSpriteItemSelect = NULL;

#define CDLGGARDENQSHOPNUMPERPAGE	6
#define CDLGGARDENQSHOPNUMPERLINE	3

using namespace HT_S2C;
using namespace HT_S2C::CMD;

CDlgGardenQShop::CDlgGardenQShop()
{
	for (int i=0; i<GARDENQSHOPITEM_NUM; i++)
	{
		m_pImgItem[i] = NULL;
		for (int j=0; j<3; j++)
			m_pLabItem[i][j] = NULL;
	}
	m_pScl_Item = NULL;
}

CDlgGardenQShop::~CDlgGardenQShop()
{
}

bool CDlgGardenQShop::OnInitDialog()
{
	AString strName;
	UINT i;
	for (i=0; i<GARDENQSHOPITEM_NUM; i++)
	{
		strName.Format("Img_%d", i+1);
		DDX_Control(strName,m_pImgItem[i]);
		for (int j=0; j<3; j++)
		{
			strName.Format("Txt_%d_%d", i+1, j+1);
			DDX_Control(strName,m_pLabItem[i][j]);
		}
	}
	DDX_Control("Scl_Item", m_pScl_Item);
	if( !m_pA2DSpriteItemSelect )
	{
		m_pA2DSpriteItemSelect = new A2DSprite;
		if( !m_pA2DSpriteItemSelect ) return AUI_ReportError(__LINE__, __FILE__);
		
		bool bval = m_pA2DSpriteItemSelect->Init(m_pA3DDevice, "InGame\\SkillHilight.tga", 0);
		if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
		m_pA2DSpriteItemSelect->SetLinearFilter(true);
		A3DRECT rc[6];
		int W = m_pA2DSpriteItemSelect->GetWidth();
		int H = m_pA2DSpriteItemSelect->GetHeight() / 6;
		for(int i = 0; i < 6; i++ )
			rc[i].SetRect(0, H * i, W, H * (i + 1));
		m_pA2DSpriteItemSelect->ResetItems(6, rc);
	}
	
	HOMETOWN::HometownTemplate* homeTemplate = GetGame()->GetHomeTownDataMan();
	const FARM_CONFIG *farmConfig = homeTemplate->get_farm_config();
	for (i=0; i<farmConfig->tool_list.size(); i++)
	{
		DATA_TYPE dt;
		const HOMETOWN::TOOL_ESSENCE* toolEss;
		toolEss = homeTemplate->get_data_ptr(farmConfig->tool_list[i],HOMETOWN::ID_SPACE_ESSENCE, dt, toolEss);
		m_vecToolItems.push_back(toolEss);
	}
	if (m_vecToolItems.size() > CDLGGARDENQSHOPNUMPERPAGE)
	{
		m_pScl_Item->Show(true);
		m_pScl_Item->SetScrollRange(0,m_vecToolItems.size()-CDLGGARDENQSHOPNUMPERPAGE);
		m_pScl_Item->SetBarLength(-1);
		m_pScl_Item->SetScrollStep(1);
	}
	else
	{
		m_pScl_Item->Show(false);
	}
	m_iCurIndex = -1;
	return true;
}

void CDlgGardenQShop::OnShowDialog()
{
	UpdateItems();
}

void CDlgGardenQShop::OnTick()
{
	if (m_pScl_Item->IsShow() && m_iFirstLine != m_pScl_Item->GetBarLevel())
	{
		m_iFirstLine = m_pScl_Item->GetBarLevel();
		UpdateItems();
	}
	PAUIDIALOG pDlgMouseOn;
	PAUIOBJECT pObjMouseOn;
	GetGameUIMan()->GetMouseOn(&pDlgMouseOn, &pObjMouseOn);
	
// 	if( pDlgMouseOn && pObjMouseOn && strncmp(pObjMouseOn->GetName(), "Img_Plant_", strlen("Img_Plant_") ) == 0 )
// 	const wchar_t* szExtDesc = g_pGame->GetHomeTownDesc()->GetWideString(m_tid);
}

bool CDlgGardenQShop::Release()
{
	A3DRELEASE(m_pA2DSpriteItemSelect)
	m_vecToolItems.clear();

	return CDlgBase::Release();
}

void CDlgGardenQShop::OnCommand_Confirm(const char * szCommand)
{
	if (m_iCurIndex>=0)
	{
		int id = m_vecToolItems[m_iCurIndex]->id;
		GetGameUIMan()->InvokeNumberDialog(NULL, (PAUIOBJECT)id, CDlgInputNO::INPUTNO_GARDENQSHOPITEM_BUY, 99);
	}
// 	ACString strText;
// 	strText.Format(GetGameUIMan()->GetStringFromTable(8661),_AL(""));
// 	PAUIDIALOG pMsgBox;
// 	GetGameUIMan()->MessageBox("game_GardenExchange", strText, MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
// 	pMsgBox->SetData(m_vecToolItems[m_iCurIndex]->id);
}

void CDlgGardenQShop::OnCommand_Close(const char * szCommand)
{
	Show(false);
}

void CDlgGardenQShop::OnPrtc_BuyItem(void *pData)
{
	buy_item* pCmd = (buy_item*)pData;	
}

void CDlgGardenQShop::OnEventLButtonDownItem(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if( !strstr(pObj->GetName(), "Img_") )
		return;
	m_iCurType = atoi(pObj->GetName()+strlen("Img_"));
	for (int i=0; i<GARDENQSHOPITEM_NUM; i++)
	{
		if (m_pImgItem[i] == pObj)
		{
			m_pImgItem[i]->SetCover(m_pA2DSpriteItemSelect,0,1);
			m_iCurIndex = m_pImgItem[i]->GetData();
		}
		else
			m_pImgItem[i]->SetCover(NULL,0,1);
	}
}

void CDlgGardenQShop::OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if (!m_pScl_Item->IsShow()) return;

	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
	int zDelta = (short)HIWORD(wParam);
	if( zDelta > 0 )
		zDelta = 1;
	else
		zDelta = -1;
	m_pScl_Item->SetBarLevel(m_pScl_Item->GetBarLevel() - zDelta);
}

void CDlgGardenQShop::UpdateItems()
{
	ACString strText;
	int i;
	for (i=0; i<CDLGGARDENQSHOPNUMPERPAGE; i++)
	{
		UINT index = i+m_iFirstLine*CDLGGARDENQSHOPNUMPERLINE;
		if (index<m_vecToolItems.size())
		{
			m_pImgItem[i]->Show(true);
			m_pLabItem[i][0]->Show(true);
			m_pLabItem[i][0]->SetText(m_vecToolItems[index]->name);
			m_pLabItem[i][1]->Show(true);
			strText.Format(GetGameUIMan()->GetStringFromTable(8667), m_vecToolItems[index]->charm_price);
			m_pLabItem[i][1]->SetText(strText);
			m_pLabItem[i][2]->Show(true);
			strText.Format(GetGameUIMan()->GetStringFromTable(8683), m_vecToolItems[index]->money_price);
			m_pLabItem[i][2]->SetText(strText);

			m_pImgItem[i]->SetData(index);
			AUIOBJECT_SETPROPERTY p;
			const char *path = g_pGame->GetHomeTownIconPath()->GetANSIString(m_vecToolItems[index]->icon);
			strcpy(p.fn, path);
			m_pImgItem[i]->SetProperty("Image File", &p);

			CECIvtrItem *pItem = CECIvtrItem::CreateHomeTownItem(m_vecToolItems[index]->id, 0, HOMETOWN::ID_SPACE_ESSENCE);
			if (pItem)
			{
				AUICTranslate trans;
				AWString strHint = trans.Translate(pItem->GetDesc());
				m_pImgItem[i]->SetHint(strHint);
				delete pItem;
			}
		}
		else
		{
			m_pLabItem[i][0]->Show(false);
			m_pLabItem[i][1]->Show(false);
			m_pLabItem[i][2]->Show(false);
			m_pImgItem[i]->Show(false);
			m_pImgItem[i]->ClearCover();
			m_pLabItem[i][0]->SetText(_AL(""));
			m_pLabItem[i][1]->SetText(_AL(""));
			m_pLabItem[i][2]->SetText(_AL(""));
			m_pImgItem[i]->SetHint(_AL(""));
		}
	}
	
}