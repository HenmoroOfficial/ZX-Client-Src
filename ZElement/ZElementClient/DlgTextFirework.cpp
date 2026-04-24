// File		: DlgTextFirework.cpp
// Creator	: Xiao Zhou
// Date		: 2006/12/18

#include "AFI.h"
#include "AUI\\A3DFTFont.h"
#include "DlgTextFirework.h"
#include "DlgInventory.h"
#include "EC_Game.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_IvtrItem.h"
#include "EC_Inventory.h"
#include "EC_IvtrConsume.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgTextFirework, CDlgBase)

AUI_ON_COMMAND("confirm",		OnCommandConfirm)
AUI_ON_COMMAND("reset",			OnCommandReset)
AUI_ON_COMMAND("IDCANCEL",		OnCommandCancel)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgTextFirework, CDlgBase)

AUI_ON_EVENT("Txt_Sel",	WM_LBUTTONDOWN,	OnEventLButtonDown)

AUI_END_EVENT_MAP()

CDlgTextFirework::CDlgTextFirework()
{
}

CDlgTextFirework::~CDlgTextFirework()
{
}

bool CDlgTextFirework::OnInitDialog()
{
	m_pTxt_Name = (PAUIEDITBOX)GetDlgItem("Txt_Name");
	if( !m_pTxt_Name ) return false;
	m_pTxt_Sel = (PAUILABEL)GetDlgItem("Txt_Sel");
	if( !m_pTxt_Sel ) return false;
	m_pTxt_Firework= (PAUIEDITBOX)GetDlgItem("Txt_Firework");
	if( !m_pTxt_Firework ) return false;
	m_pImg_Item = (PAUIIMAGEPICTURE)GetDlgItem("Img_Item");
	if( !m_pImg_Item ) return false;
	m_pBtn_Confirm = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Confirm");
	if( !m_pBtn_Confirm ) return false;
	m_pTxt_Firework->Enable(false);

	return true;
}

bool CDlgTextFirework::Render()
{
	if( !CDlgBase::Render() )
		return false;
	
	CECIvtrItem *pItem = GetHostPlayer()->GetPack()->GetItem(m_nSolt);
	if( pItem && pItem->GetClassID() == CECIvtrItem::ICID_TEXT_FIREWORK )
	{
		CECIvtrTextFirework *pFireWork = (CECIvtrTextFirework*)pItem;
		POINT pt = m_pTxt_Sel->GetPos();
		SIZE sz = m_pTxt_Sel->GetSize();
		int i;
		A3DRECT clip;
		clip.SetRect(pt.x, pt.y, pt.x + sz.cx, pt.y + sz.cy);
		int x = pt.x;
		int y = pt.y;
		int incx = sz.cx / 20;
		int incy = sz.cy / 15;
		i = 0;
		while( pFireWork->GetDBEssence()->char_can_use[i] )
		{
			ACHAR szText[2];
			szText[0] = pFireWork->GetDBEssence()->char_can_use[i];
			szText[1] = 0;
			m_pTxt_Sel->GetFont()->DrawText(x, y, szText, A3DCOLORRGB(255, 255, 255), &clip);
			i++;
			x += incx;
			if( i % 20 == 0 )
			{
				x = pt.x;
				y += incy;
			}
		}
	}

	return true;
}

void CDlgTextFirework::OnCommandConfirm(const char *szCommand)
{
	if( a_strlen(m_pTxt_Firework->GetText()) == 0 )
	{
		OnCommandCancel("");
		return;
	}
	CECIvtrItem *pItem = GetHostPlayer()->GetPack()->GetItem(m_nSolt);
	if( pItem && pItem->GetClassID() == CECIvtrItem::ICID_TEXT_FIREWORK )
	{
		CECIvtrTextFirework *pFireWork = (CECIvtrTextFirework*)pItem;
		pFireWork->Freeze(false);
		GetGameSession()->c2s_CmdUseItemWithArg(IVTRTYPE_PACK, 1, (unsigned short)m_nSolt,
			pFireWork->GetTemplateID(), m_pTxt_Firework->GetText(), a_strlen(m_pTxt_Firework->GetText()) * 2);
		Show(false);
	}
}

void CDlgTextFirework::OnCommandReset(const char *szCommand)
{
	m_pTxt_Firework->SetText(_AL(""));
}

void CDlgTextFirework::OnCommandCancel(const char *szCommand)
{
	Show(false);
	CECIvtrItem *pItem = GetHostPlayer()->GetPack()->GetItem(m_nSolt);
	if( pItem )
		pItem->Freeze(false);
}

void CDlgTextFirework::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	CECIvtrItem *pItem = GetHostPlayer()->GetPack()->GetItem(m_nSolt);
	if( pItem && pItem->GetClassID() == CECIvtrItem::ICID_TEXT_FIREWORK )
	{
		CECIvtrTextFirework *pFireWork = (CECIvtrTextFirework*)pItem;
		if( (int)a_strlen(m_pTxt_Firework->GetText()) >= pFireWork->GetDBEssence()->max_char )
			return;

		POINT pt = m_pTxt_Sel->GetPos();
		SIZE sz = m_pTxt_Sel->GetSize();
		int incx = sz.cx / 20;
		int incy = sz.cy / 15;
		A3DVIEWPORTPARAM *p = GetGame()->GetA3DEngine()->GetActiveViewport()->GetParam();
		int x = GET_X_LPARAM(lParam)  - p->X;
		int y = GET_Y_LPARAM(lParam)  - p->Y;
		int n = ((y - pt.y) / incy) * 20 + (x - pt.x) / incx;
		ACHAR szText[2];
		szText[0] = pFireWork->GetDBEssence()->char_can_use[n];
		szText[1] = 0;
		m_pTxt_Firework->SetText(ACString(m_pTxt_Firework->GetText()) + szText );
	}
}

void CDlgTextFirework::SetFirework(int nSlot)
{
	CECIvtrItem *pItem = GetHostPlayer()->GetPack()->GetItem(nSlot);
	if( pItem->GetClassID() != CECIvtrItem::ICID_TEXT_FIREWORK )
	{
		Show(false);
		return;
	}
	CECIvtrTextFirework *pFireWork = (CECIvtrTextFirework*)pItem;
	SetDataPtr(pFireWork);
	m_nSolt = nSlot;
	pItem->Freeze(true);
	AString strFile;
	af_GetFileTitle(pFireWork->GetIconFile(), strFile);
	strFile.MakeLower();
	m_pImg_Item->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
		GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
	m_pTxt_Name->SetText(pFireWork->GetName());
	m_pTxt_Firework->SetText(_AL(""));
}
