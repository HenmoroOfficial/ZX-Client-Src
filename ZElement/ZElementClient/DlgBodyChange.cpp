// Filename	: DlgBodyChange.cpp
// Creator	: Fu Chonggang
// Date		: 2009/08/19

#include "AUI\\AUICTranslate.h"
#include "AFI.h"
#include "DlgBodyChange.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_IvtrTalismanMain.h"
#include "EC_IvtrStone.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_GameRun.h"
#include "EC_Skill.h"
#include "EC_Game.h"
#include "DlgSkill.h"
#include "ExpTypes.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgBodyChange, CDlgBase)

AUI_ON_COMMAND("Btn_Enchase",	OnCommand_Begin)
AUI_ON_COMMAND("Btn_Cancel",	OnCommand_Close)
AUI_ON_COMMAND("Btn_Close",		OnCommand_Close)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_Close)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgBodyChange, CDlgBase)

AUI_ON_EVENT("Item_a", WM_LBUTTONDBLCLK,OnEventLButtonDblClk)
AUI_ON_EVENT("Item_d", WM_LBUTTONDBLCLK,OnEventLButtonDblClk)

AUI_ON_EVENT("ChangeSkill_*", WM_LBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("ChangeSkill_*", WM_MOUSEMOVE,		OnEventMouseMove)

AUI_END_EVENT_MAP()

#define MSGBOX(str)			GetGameUIMan()->MessageBox("",str, MB_OK, A3DCOLORRGBA(255, 255, 255, 160))
#define GETSTRING(id)		GetGameUIMan()->GetStringFromTable(id)

CDlgBodyChange::CDlgBodyChange()
{
}

CDlgBodyChange::~CDlgBodyChange()
{
}

void CDlgBodyChange::OnCommand_Begin(const char * szCommand)
{
	if (stricmp(m_szName,"Win_BodyChange_Enchanting")==0)
	{
		PAUIIMAGEPICTURE pImagea = (PAUIIMAGEPICTURE)GetDlgItem("Item_a");
		CECIvtrChangeShapeCard *pIvtrA = (CECIvtrChangeShapeCard *)pImagea->GetDataPtr("ptr_CECIvtrItem");
		PAUIIMAGEPICTURE pImaged = (PAUIIMAGEPICTURE)GetDlgItem("Item_d");
		CECIvtrChangeShapeStone *pIvtrD = (CECIvtrChangeShapeStone *)pImaged->GetDataPtr("ptr_CECIvtrItem");
		if (pIvtrA && pIvtrD)
		{
			const CHANGE_SHAPE_STONE_ESSENCE* stoneEss = pIvtrD->GetDBEssence();
			bool bOk = true;
			if (GetData()==0)
			{
				if (pIvtrA->GetEssence().ess.master_id == GetHostPlayer()->GetCharacterID())
				{
					MSGBOX(GETSTRING(8811));
					bOk = false;
				}
				else if (pIvtrA->GetEssence().ess.master_id == 0)
				{
					MSGBOX(GETSTRING(8812));
					bOk = false;
				}
			}
			else if (GetData()==2)
			{
				if (pIvtrA->GetEssence().ess.master_id == 0)
				{
					MSGBOX(GETSTRING(8815));
					bOk = false;
				}
				else if (pIvtrA->GetEssence().ess.master_id != GetHostPlayer()->GetCharacterID())
				{
					MSGBOX(GETSTRING(8821));
					bOk = false;
				}
				else if (pIvtrA->GetEssence().ess.exp_level == 5)
				{
					MSGBOX(GETSTRING(8814));
					bOk = false;
				}
			}
			else if (GetData()==1)
			{
				if (pIvtrA->GetEssence().ess.master_id == 0)
				{
					MSGBOX(GETSTRING(8818));
					bOk = false;
				}
				else if (pIvtrA->GetEssence().ess.master_id != GetHostPlayer()->GetCharacterID())
				{
					MSGBOX(GETSTRING(8822));
					bOk = false;
				}
				else if (pIvtrA->GetEssence().ess.exp_level != 5)
				{
					MSGBOX(GETSTRING(8817));
					bOk = false;
				}
				else
				{
					int iRebornCount = GetHostPlayer()->GetRebornInfo().size();
					if (iRebornCount == 0)
					{
						if (GetHostPlayer()->GetBasicProps().iLevel<pIvtrA->GetEssence().ess.level*10)
						{
							MSGBOX(GETSTRING(8819));
							bOk = false;
						}		

					}
					else
					{
						if (pIvtrA->GetEssence().ess.level>3 && GetHostPlayer()->GetBasicProps().iLevel<(pIvtrA->GetEssence().ess.level-3)*10)
						{
							MSGBOX(GETSTRING(8819));
							bOk = false;
						}
					}
				}
			}
			if (bOk)
			{
				GetGameSession()->c2s_CmdNPCSevRefineChangeCard(pImagea->GetData(),pIvtrA->GetTemplateID(),pImaged->GetData(),pIvtrD->GetTemplateID(),stoneEss->refine_type);
				OnEventLButtonDown(0, 0, pImagea);
				OnEventLButtonDown(0, 0, pImaged);
			}
		}
		else if (!pIvtrD)
		{
			GetGameUIMan()->MessageBox( "", GetGameUIMan()->GetStringFromTable( 8589+GetData()), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		}
	}
	else if (stricmp(m_szName,"Win_BodyChange_Recover")==0)
	{
		PAUIIMAGEPICTURE pImagea = (PAUIIMAGEPICTURE)GetDlgItem("Item_a");
		CECIvtrChangeShapeCard *pIvtrA = (CECIvtrChangeShapeCard *)pImagea->GetDataPtr("ptr_CECIvtrItem");
		if (pIvtrA)
		{
			bool bOk = true;
			if (pIvtrA->GetEssence().ess.master_id == 0)
			{
				MSGBOX(GETSTRING(8823));
				bOk = false;
			}
			else if ( pIvtrA->GetEssence().ess.master_id != GetHostPlayer()->GetCharacterID())
			{
				MSGBOX(GETSTRING(8820));
				bOk = false;
			}
			if (bOk)
				GetGameSession()->c2s_CmdNPCSevRecoverCardWakan(pImagea->GetData(),pIvtrA->GetTemplateID());
			OnEventLButtonDown(0, 0, pImagea);
		}
	}
}

void CDlgBodyChange::OnCommand_Close(const char * szCommand)
{
	Show(false);
	GetGameUIMan()->UnfreezeAllItems();
	GetGameUIMan()->EndNPCService();
	PAUIIMAGEPICTURE pImage;
	if (stricmp(m_szName,"Win_BodyChange_Enchanting")==0)
	{
		pImage = (PAUIIMAGEPICTURE)GetDlgItem("Item_a");
		pImage->SetCover(NULL, -1);
		pImage->SetDataPtr(NULL);
		pImage->SetData(0);
		pImage->SetHint(_AL(""));
		pImage = (PAUIIMAGEPICTURE)GetDlgItem("Item_d");
		pImage->SetCover(NULL, -1);
		pImage->SetDataPtr(NULL);
		pImage->SetData(0);
		pImage->SetHint(_AL(""));
	}
	else if (stricmp(m_szName,"Win_BodyChange_Recover")==0)
	{
		pImage = (PAUIIMAGEPICTURE)GetDlgItem("Item_a");
		pImage->SetCover(NULL, -1);
		pImage->SetDataPtr(NULL);
		pImage->SetData(0);
		pImage->SetHint(_AL(""));
	}
}

void CDlgBodyChange::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if (stricmp(m_szName, "Win_BodyChangeSkill")==0)
	{
		if( !pObj->GetDataPtr("ptr_CECSkill") )
			return;
		
		A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
		POINT pt = 
		{
			GET_X_LPARAM(lParam) - p->X,
				GET_Y_LPARAM(lParam) - p->Y,
		};
		
		GetGameUIMan()->m_ptLButtonDown = pt;
		GetGameUIMan()->InvokeDragDrop(this, pObj, pt);
	}
	else
	{
		if( !pObj || !pObj->GetDataPtr("ptr_CECIvtrItem") ) return;
		PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)pObj;
		if (stricmp(pImage->GetName(), "Item_a")==0)
		{
			m_pTxt_Money1->SetText(_AL("0"));
			m_pTxt_Money2->SetText(_AL("0"));
			m_pTxt_Money3->SetText(_AL("0"));
			m_pTxt_Money1->SetColor(A3DCOLORRGB(255, 255, 255));
			m_pTxt_Money2->SetColor(A3DCOLORRGB(255, 255, 255));
			m_pTxt_Money3->SetColor(A3DCOLORRGB(255, 255, 255));
		}
		CECIvtrItem *pItem = (CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem");
		pItem->Freeze(false);
		pImage->SetDataPtr(NULL);
		pImage->SetData(0);
		pImage->SetHint(_AL(""));
		pImage->SetCover(NULL, -1);
		GetGameUIMan()->PlayItemSound(pItem, false);
	}

}

void CDlgBodyChange::OnEventLButtonDblClk(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	OnEventLButtonDown(0,0,pObj);
}

void CDlgBodyChange::OnEventMouseMove(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if (strstr(pObj->GetName(), "ChangeSkill_") && pObj->GetDataPtr("ptr_CECSkill"))
	{
		m_dwShowTime = 0;
	}
}

int CDlgBodyChange::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="align_skill_bar" && m_szName=="Win_BodyChangeSkill")
	{
		PAUIDIALOG pDlg = NULL;
		if(GetGameUIMan()->GetDialog("Win_QuickBar8H_4")->IsShow())
			pDlg = GetGameUIMan()->GetDialog("Win_QuickBar8H_4");
		else if(GetGameUIMan()->GetDialog("Win_QuickBar8H_3")->IsShow())
			pDlg = GetGameUIMan()->GetDialog("Win_QuickBar8H_3");
		else if(GetGameUIMan()->GetDialog("Win_QuickBar8H_2")->IsShow())
			pDlg = GetGameUIMan()->GetDialog("Win_QuickBar8H_2");
		else
			pDlg = GetGameUIMan()->GetDialog("Win_QuickBar8H_1");
		if(pDlg&&pDlg->GetDlgItem("Item_1"))
		{
			AlignTo(pDlg, AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT, AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_TOP);
			SetPosEx( pDlg->GetDlgItem("Item_1")->GetPos().x, GetPos().y);
		}
	}
	return true;
}

void CDlgBodyChange::OnShowDialog()
{
	if (stricmp(m_szName,"Win_BodyChange_Enchanting")==0)
	{
		PAUILABEL pTitle = (PAUILABEL)GetDlgItem("Title");
		pTitle->SetText(GetGameUIMan()->GetStringFromTable(9050+GetData()));
	}
	else if (stricmp(m_szName,"Win_BodyChangeSkill")==0)
	{
		m_dwShowTime = 10000;
		CommonMessage("align_skill_bar", 0, 0);
	}
}

void CDlgBodyChange::OnTick()
{
	if (stricmp(m_szName,"Win_BodyChange_Enchanting")==0)
	{
		PAUIIMAGEPICTURE pImagea = (PAUIIMAGEPICTURE)GetDlgItem("Item_a");
		m_pBtn_Enchase->Enable(pImagea->GetDataPtr("ptr_CECIvtrItem")!=NULL && (int)m_pTxt_Money1->GetData()<=GetHostPlayer()->GetMoneyAmount());
		return;
	}
	else if (stricmp(m_szName,"Win_BodyChange_Recover")==0)
	{
		PAUIIMAGEPICTURE pImagea = (PAUIIMAGEPICTURE)GetDlgItem("Item_a");
		m_pBtn_Enchase->Enable(pImagea->GetDataPtr("ptr_CECIvtrItem")!=NULL && (int)m_pTxt_Money1->GetData()<=GetHostPlayer()->GetMoneyAmount());
		return;
	}
	else if (stricmp(m_szName,"Win_BodyChangeSkill")==0)
	{
		int inum = 0;
		for( int i = 0; i < GetHostPlayer()->GetTransfigureSkillNum(); i++ )
		{
			CECSkill * pSkill = GetHostPlayer()->GetTransfigureSkillByIndex(i);
			if( pSkill->GetSkillLevel() > 0 && 
				!pSkill->IsCategory(GNET::CATEGORY_TALISMAN) && 
				!pSkill->IsCategory(GNET::CATEGORY_FAMILY) && 
				!pSkill->IsCategory(GNET::CATEGORY_TALENT) &&
				!pSkill->IsCategory(GNET::CATEGORY_SECT) &&
				!pSkill->IsCategory(GNET::CATEGORY_MYSTERY) &&
				!pSkill->IsCategory(GNET::CATEGORY_REBORN) &&
				!pSkill->IsCategory(GNET::CATEGORY_SPOUSE))
			{
				AString strName;
				strName.Format("ChangeSkill_%d", inum + 1);
				PAUIIMAGEPICTURE pImage = static_cast<PAUIIMAGEPICTURE>(GetDlgItem(strName));
				if( !pImage ) break;
				SetImage(pImage, pSkill, true);
				inum++;
			}
		}
		for (;inum<8;inum++)
		{
			AString strName;
			strName.Format("ChangeSkill_%d", inum + 1);
			PAUIIMAGEPICTURE pImage = static_cast<PAUIIMAGEPICTURE>(GetDlgItem(strName));
			if( !pImage ) break;
			SetImage(pImage, NULL, true);
		}
		if (m_dwShowTime>0)
		{
			DWORD dwTick = GetGame()->GetTickTime();
			if (m_dwShowTime>dwTick)
				m_dwShowTime -= dwTick;
			else
				m_dwShowTime = 0;
		}
	}
}

bool CDlgBodyChange::OnInitDialog()
{
	m_pBtn_Enchase = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Enchase");
	m_pTxt_Money1 = (PAUILABEL)GetDlgItem("Txt_Money1");
	m_pTxt_Money2 = (PAUILABEL)GetDlgItem("Txt_Money2");
	m_pTxt_Money3 = (PAUILABEL)GetDlgItem("Txt_Money3");
	if (m_pTxt_Money1)
	{
		m_pTxt_Money1->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_Money1->SetText(_AL("0"));
	}
	if (m_pTxt_Money2)
	{
		m_pTxt_Money2->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_Money2->SetText(_AL("0"));
	}
	if (m_pTxt_Money3)
	{
		m_pTxt_Money3->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_Money3->SetText(_AL("0"));
	}
	
	return true;
}


void CDlgBodyChange::DragDropItem(int iSrc, PAUIOBJECT pObjOver)
{
	if( !pObjOver )
		return;

	CECIvtrItem *pIvtrSrc = GetHostPlayer()->GetPack()->GetItem(iSrc);
	OnEventLButtonDown(0, 0, pObjOver);
	AString strFile;
	af_GetFileTitle(pIvtrSrc->GetIconFile(), strFile);
	strFile.MakeLower();
	PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)pObjOver;
	pImage->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
		GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
	
	GetGameUIMan()->PlayItemSound(pIvtrSrc, true);
	pIvtrSrc->Freeze(true);
	pObjOver->SetData(iSrc);
	pObjOver->SetDataPtr(pIvtrSrc, "ptr_CECIvtrItem");
	AUICTranslate trans;
	const wchar_t* szDesc = pIvtrSrc->GetDesc();
	pObjOver->SetHint(trans.Translate(szDesc));
	if( 0 == stricmp(pObjOver->GetName(), "Item_a") )
	{
		const CHANGE_SHAPE_CARD_ESSENCE *pDBEssence = (CHANGE_SHAPE_CARD_ESSENCE *)(((CECIvtrChangeShapeCard*)pIvtrSrc)->GetDBEssence());
		int fee = 0;
		if (stricmp(m_szName,"Win_BodyChange_Enchanting")==0)
		{
			if (GetData()==0)
				fee = pDBEssence->fee_restore;
			else if (GetData()==1)
				fee = pDBEssence->fee_refine;
			else if (GetData()==2)
				fee = pDBEssence->fee_contract;
		}
		else
		{
			fee = pDBEssence->fee_recover_wakan * (((CECIvtrChangeShapeCard*)pIvtrSrc)->GetEssence().ess.max_stamina - ((CECIvtrChangeShapeCard*)pIvtrSrc)->GetEssence().ess.stamina);
		}
		m_pTxt_Money1->SetData(fee);
		ACHAR szMoney[20];
		a_sprintf(szMoney, _AL("%d"), fee % 100);
		m_pTxt_Money3->SetText(szMoney);
		a_sprintf(szMoney, _AL("%d"), (fee % 10000) / 100);
		m_pTxt_Money2->SetText(szMoney);
		a_sprintf(szMoney, _AL("%d"), fee / 10000);
		m_pTxt_Money1->SetText(szMoney);
		if( fee > GetHostPlayer()->GetMoneyAmount() )
		{
			m_pTxt_Money1->SetColor(A3DCOLORRGB(255, 0, 0));
			m_pTxt_Money2->SetColor(A3DCOLORRGB(255, 0, 0));
			m_pTxt_Money3->SetColor(A3DCOLORRGB(255, 0, 0));
		}
		else
		{
			m_pTxt_Money1->SetColor(A3DCOLORRGB(255, 255, 255));
			m_pTxt_Money2->SetColor(A3DCOLORRGB(255, 255, 255));
			m_pTxt_Money3->SetColor(A3DCOLORRGB(255, 255, 255));
		}
	}
}


void CDlgBodyChange::SetImage(AUIImagePicture *pImage, CECSkill *pSkill, bool bPositiveSkill)
{
	ASSERT(pImage);
	
	// get clock icon
	AUIClockIcon *pClock = pImage->GetClockIcon();
	pImage->SetCover(NULL, 0, 1);
	
	if (pSkill)
	{
		AString strFile;
		af_GetFileTitle(pSkill->GetIconFile(), strFile);
		strFile.MakeLower();
		pImage->SetCover(
			GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_SKILL],
			GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_SKILL][strFile]);
		pImage->SetDataPtr(pSkill,"ptr_CECSkill");
		pImage->SetData(0);
		pImage->SetHint(pSkill->GetDesc());
		if( bPositiveSkill )
		{
			if( pSkill->ReadyToCast() && GetHostPlayer()->GetPrepSkill() != pSkill )
			{
				if( !GetHostPlayer()->CheckSkillCastCondition(pSkill) )
					pImage->SetColor(A3DCOLORRGB(255, 255, 255));
				else
					pImage->SetColor(A3DCOLORRGB(128, 128, 128));
			}
			else
				pClock->SetColor(A3DCOLORRGBA(0, 0, 0, 128));
			if( pSkill->GetCoolingTime() > 0 || GetHostPlayer()->GetPrepSkill() == pSkill )
			{
				pClock->SetProgressRange(0, pSkill->GetCoolingTime());
				if( GetHostPlayer()->GetPrepSkill() == pSkill )
					pClock->SetProgressPos(0);
				else
					pClock->SetProgressPos(pSkill->GetCoolingTime() - pSkill->GetCoolingCnt());
			}
		}
		if (m_dwShowTime>0)
		{
			pImage->SetCover(CDlgSkill::m_pA2DSpriteSkillSelect, (GetTickCount() / 200) % 7, 1);
		}
	}
	else
	{
		pImage->SetCover(NULL, -1);
		pImage->SetDataPtr(NULL);
		pImage->SetData(0);
		pImage->SetHint(_AL(""));
		if( bPositiveSkill )
		{
			pImage->SetColor(A3DCOLORRGB(255, 255, 255));
			pClock->SetProgressRange(0, 1);
			pClock->SetProgressPos(1);
		}
	}
}