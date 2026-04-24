// Filename	: DlgAchivMain.cpp
// Creator	: Fu Chonggang
// Date		: 2010/1/27

#include "AUI\\AUICTranslate.h"
#include "AUI\\AUISubDialog.h"
#include "AUI\\AUIDef.h"
#include "AFI.h"
#include "AFileImage.h"
#include "DlgAchivItemMan.h"
#include "DlgAchivItem.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_IvtrConsume.h"
#include "EC_HostPlayer.h"
#include "EC_GameUIMan.h"
#include "EC_Inventory.h"
#include "EC_Achievement.h"
#include "EC_ManPlayer.h"
#include "EC_World.h"
#include "time.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgAchivItemMan, CDlgBase)
AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgAchivItemMan, CDlgBase)

AUI_END_EVENT_MAP()

CDlgAchivItemMan::CDlgAchivItemMan()
{
}

CDlgAchivItemMan::~CDlgAchivItemMan()
{
}

bool CDlgAchivItemMan::OnInitDialog()
{
	m_vecAchivs.clear();
	m_vecDlgItems.clear();
	m_vecSubDialogs.clear();
	m_vecDlgItemsOther.clear();
	m_pDlgItemExpand = NULL;
	m_pSubItem = (PAUISUBDIALOG)GetDlgItem("Sub_1");
	m_rcWindowOld = GetGameUIMan()->GetRect();
	m_nIndexCur = 0;

	m_vecSubDialogsOther.clear();
	m_vecDlgItemsOther.clear();

	return true;
}

bool CDlgAchivItemMan::Release()
{
	m_vecAchivs.clear();
	int i;
	int nSize = m_vecSubDialogs.size();
	for (i=0; i<nSize; i++)
	{
		DeleteControl(m_vecSubDialogs[i]);
	}
	m_vecSubDialogs.clear();
	//创建的界面uiman统一管理，不必单独destroy
	m_vecDlgItems.clear();

	m_vecSubDialogsOther.clear();
	m_vecDlgItemsOther.clear();

	return CDlgBase::Release();
}

void CDlgAchivItemMan::OnTick()
{
	if (m_nIndexCur>0)
	{
		PAUISUBDIALOG pSubD = (PAUISUBDIALOG)GetGameUIMan()->GetDialog("Win_Achievement")->GetDlgItem("Sub_Detail");
		CDlgAchivItem* pDlgAchivItem = m_vecDlgItems[m_nIndexCur];
		int y = pDlgAchivItem->GetPos().y;
		int y1 = pSubD->GetPos().y;
		int y2 = pSubD->GetSize().cy;
		if(!pDlgAchivItem->IsShow() || y>=(y1+y2/2) || y==0)
		{
			pSubD->OnDlgItemMessage(WM_MOUSEWHEEL, 0xff880000, 0);
		}
		else
			m_nIndexCur = 0;
	}

	PAUIDIALOG pDlgOther = GetGameUIMan()->GetDialog("Win_Achievement_Other");
	if (pDlgOther && pDlgOther->IsShow())
	{
		float fWindowScale = m_pAUIManager->GetWindowScale() * m_fScale;
		pDlgOther->AlignTo(GetGameUIMan()->GetDialog("Win_Achievement"), AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT,
			AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_BOTTOM, 0, int(-35 * fWindowScale));

		PAUISUBDIALOG pSubD = (PAUISUBDIALOG)GetGameUIMan()->GetDialog("Win_Achievement")->GetDlgItem("Sub_Detail");
		PAUISUBDIALOG pSubDOther = (PAUISUBDIALOG) pDlgOther->GetDlgItem("Sub_Detail");
		pSubDOther->SetVBarPos(pSubD->GetVBarPos());
	}
#ifdef _DEBUG
	if (GetAsyncKeyState('7') & 0x8000 && m_pDlgItemExpand)
	{
		AString strText;
		strText.Format("expand: x:%d,y:%d,width:%d,height:%d\n", m_pDlgItemExpand->GetPos().x, m_pDlgItemExpand->GetPos().y, m_pDlgItemExpand->GetSize().cx, m_pDlgItemExpand->GetSize().cy);
		OutputDebugStringA(strText);
		strText.Format("expand back: x:%d,y:%d,width:%d,height:%d\n", m_pDlgItemExpand->GetDlgItem("Edt_Back")->GetPos().x, m_pDlgItemExpand->GetDlgItem("Edt_Back")->GetPos().y, m_pDlgItemExpand->GetDlgItem("Edt_Back")->GetSize().cx, m_pDlgItemExpand->GetDlgItem("Edt_Back")->GetSize().cy);
		OutputDebugStringA(strText);
		strText.Format("not expand: x:%d,y:%d,width:%d,height:%d\n", m_vecDlgItems[0]->GetPos().x, m_vecDlgItems[0]->GetPos().y, m_vecDlgItems[0]->GetSize().cx, m_vecDlgItems[0]->GetSize().cy);
		OutputDebugStringA(strText);
		strText.Format("not expand back: x:%d,y:%d,width:%d,height:%d\n", m_vecDlgItems[0]->GetDlgItem("Edt_Back")->GetPos().x, m_vecDlgItems[0]->GetDlgItem("Edt_Back")->GetPos().y, m_vecDlgItems[0]->GetDlgItem("Edt_Back")->GetSize().cx, m_vecDlgItems[0]->GetDlgItem("Edt_Back")->GetSize().cy);
		OutputDebugStringA(strText);
		strText.Format("subobj:x:%d,y:%d,width:%d,height:%d\n\n", m_vecSubDialogs[0]->GetPos().x, m_vecSubDialogs[0]->GetPos().y, m_vecSubDialogs[0]->GetSize().cx, m_vecSubDialogs[0]->GetSize().cy);
		OutputDebugStringA(strText);
	}
#endif
}

bool CDlgAchivItemMan::Render()
{
	return AUIDialog::Render();
}

bool CDlgAchivItemMan::InitItemDlgs()
{
	int i;
	int nSize = m_vecSubDialogs.size();
	//先删掉控件，再删对应的dialog
	for (i=0; i<nSize; i++)
	{
		DeleteControl(m_vecSubDialogs[i]);
	}
	m_vecSubDialogs.clear();
	nSize = m_vecDlgItems.size();
	for (i=0; i<nSize; i++)
	{
		GetGameUIMan()->DestroyDialog(m_vecDlgItems[i]->GetName());
	}
	m_vecDlgItems.clear();

	A3DRECT	rcNew = GetGameUIMan()->GetRect();
	nSize = m_vecAchivs.size();
	int y=0;
	for (i=0; i<nSize; i++)
	{
		//这个文件不能写到ingame.dcf里，否则创建失败
		GetGameUIMan()->CreateDlg(g_pGame->GetA3DEngine(), g_pGame->GetA3DDevice(), "Achievement_Done.xml");
		CDlgAchivItem* pDlg = (CDlgAchivItem*)GetGameUIMan()->GetDialog("Win_Achievement_Done");
		AString strName;
		strName.Format("Win_Achievement_Done%d", i);
		pDlg->SetName(strName);
		pDlg->SetAchiv(&m_vecAchivs[i]);
		pDlg->Resize(m_rcWindowOld, rcNew);

		if (m_vecAchivs[i].awardItem != 0)
		{
			CECIvtrItem *pItem = CECIvtrItem::CreateItem(m_vecAchivs[i].awardItem, 0, 1);
			if ( pItem )
				pDlg->SetDataPtr( pItem, "ptr_CECIvtrItem" );
		}

		m_vecDlgItems.push_back(pDlg);

		PAUISUBDIALOG pSubItem = (PAUISUBDIALOG)CloneControl(m_pSubItem);
		strName.Format("Sub_Clone%d", i);
		pSubItem->SetName(strName.GetBuffer(0));
		pSubItem->SetSize(m_pSubItem->GetSize().cx, m_pSubItem->GetSize().cy);//pDlg->GetSize().cx,pDlg->GetSize().cy);
		pSubItem->SetDialog(pDlg);
		pSubItem->SetPos(0,y);
		m_vecSubDialogs.push_back(pSubItem);
		y += pSubItem->GetSize().cy;
	}
	int w = GetSize().cx;
	int h = y;
	SetSize(w, h);
	
	PAUISUBDIALOG pSubD = (PAUISUBDIALOG)GetGameUIMan()->GetDialog("Win_Achievement")->GetDlgItem("Sub_Detail");
	pSubD->SetSubDialogOrgSize(w, h);

	PAUIDIALOG pDlgOther = GetGameUIMan()->GetDialog("Win_Achievement_Other");
	PAUIDIALOG pDlgDetailOther = GetGameUIMan()->GetDialog("Win_Achievement_Sub_Other");
	if (pDlgOther && pDlgOther->IsShow())
	{
		CECPlayerMan *pPlayerMan = GetWorld()->GetPlayerMan();
		CECPlayer *elsePlayer = pPlayerMan->GetPlayer(pDlgOther->GetData());

		CECAchievementMan* achivMan = NULL;
		if (elsePlayer)
		{
			achivMan = elsePlayer->GetAchievementMan();

			// Set player name.
			((PAUILABEL) pDlgOther->GetDlgItem("Lab_Name"))->SetText(elsePlayer->GetName());

			// Set achiv points.
			if (achivMan)
			{
				ACString pointStr;
				pointStr.Format(_AL("%d"), achivMan->GetAchievementPoint());
				((PAUILABEL) pDlgOther->GetDlgItem("Txt_SuccessPoint"))->SetText(pointStr);
			}
		}

		int nSize = m_vecSubDialogsOther.size();
		//先删掉控件，再删对应的dialog
		for (i=0; i<nSize; i++)
		{
			pDlgDetailOther->DeleteControl(m_vecSubDialogsOther[i]);
		}
		m_vecSubDialogsOther.clear();
		nSize = m_vecDlgItemsOther.size();
		for (i=0; i<nSize; i++)
		{
			GetGameUIMan()->DestroyDialog(m_vecDlgItemsOther[i]->GetName());
		}
		m_vecDlgItemsOther.clear();

		nSize = m_vecAchivs.size();
		int y=0;
		for (i=0; i<nSize; i++)
		{
			//这个文件不能写到ingame.dcf里，否则创建失败
			GetGameUIMan()->CreateDlg(g_pGame->GetA3DEngine(), g_pGame->GetA3DDevice(), "Achievement_Done_Other.xml");
			CDlgAchivItemOther* pDlg = (CDlgAchivItemOther*)GetGameUIMan()->GetDialog("Win_Achievement_Done_Other");
			AString strName;
			strName.Format("Win_Achievement_Done_Other_%d", i);
			pDlg->SetName(strName);

			if (elsePlayer)
				pDlg->SetAchiv(achivMan->GetAchievement(m_vecAchivs[i].id));
			else
				pDlg->SetAchiv(&m_vecAchivs[i]);
			
			pDlg->Resize(m_rcWindowOld, rcNew);

			m_vecDlgItemsOther.push_back(pDlg);

			PAUISUBDIALOG pSubItem = (PAUISUBDIALOG) pDlgDetailOther->CloneControl(pDlgDetailOther->GetDlgItem("Sub_1"));
			strName.Format("Sub_Clone%d", i);
			pSubItem->SetName(strName.GetBuffer(0));
			//pSubItem->SetSize(m_pSubItem->GetSize().cx, m_pSubItem->GetSize().cy);
			pSubItem->SetDialog(pDlg);
			pSubItem->SetPos(0,y);
			m_vecSubDialogsOther.push_back(pSubItem);
			y += pSubItem->GetSize().cy;
		}

		pDlgDetailOther->SetSize(w, h);

		PAUISUBDIALOG pSubDOther = (PAUISUBDIALOG) pDlgOther->GetDlgItem("Sub_Detail");
		pSubDOther->SetSubDialogOrgSize(w, h);
	}
	
	return true;
}

bool CDlgAchivItemMan::ExpandItem(CDlgAchivItem* pDlg, bool bExpand)
{
	UINT index = 0;
	UINT i;
	UINT nSize = m_vecDlgItems.size();
	m_pDlgItemExpand = (CDlgAchivItemExpand*)GetGameUIMan()->GetDialog("Win_Achievement_DoneFolder");
	if (bExpand)
	{
		for (i=0; i<nSize; i++)
		{
			if (m_vecDlgItems[i] == pDlg)
			{
				index = i;
				break;
			}
		}
		m_pDlgItemExpand->SetAchiv(&m_vecAchivs[index]);
		// 这次show作用：调整这个界面大小
		m_pDlgItemExpand->Show(true);
	}
	else
	{
		for (i=0; i<nSize; i++)
		{
			if (m_vecSubDialogs[i]->GetSubDialog() == pDlg)
			{
				index = i;
				break;
			}
		}
	}
	if (index >= m_vecDlgItems.size())
	{
		return false;
	}
	int w = GetSize().cx;
	int h = m_pSubItem->GetSize().cy*nSize;
	int hDelta = m_pDlgItemExpand->GetSize().cy-m_pSubItem->GetSize().cy;
	int x = m_vecSubDialogs[i]->GetPos(true).x;
	int y=m_vecSubDialogs[0]->GetPos(true).y;
	if (bExpand)
	{
		for (i=0; i<nSize; i++)
		{
			if (i==index)
			{
				m_vecSubDialogs[i]->SetDialog(m_pDlgItemExpand);
				m_vecSubDialogs[i]->SetSize(m_pSubItem->GetSize().cx, m_pSubItem->GetSize().cy+hDelta);
				m_vecSubDialogs[i]->SetPos(x, y);
				y += m_pDlgItemExpand->GetSize().cy;
			}
			else
			{
				m_vecSubDialogs[i]->SetDialog(m_vecDlgItems[i]);
				m_vecSubDialogs[i]->SetSize(m_pSubItem->GetSize().cx, m_pSubItem->GetSize().cy);
				m_vecSubDialogs[i]->SetPos(x, y);
				y += m_pSubItem->GetSize().cy;
			}
		}
		// setdialog后原dialog会不显示，这里将其强制显示
		m_pDlgItemExpand->Show(true);
		h += hDelta;
	}
	else
	{
		for (i=nSize-1; i>index; i--)
		{
			int x = m_vecSubDialogs[i]->GetPos(true).x;
			int y = m_vecSubDialogs[i]->GetPos(true).y-m_pDlgItemExpand->GetSize().cy+m_pSubItem->GetSize().cy;
			m_vecSubDialogs[i]->SetPos(x, y);
		}
		m_vecSubDialogs[index]->SetDialog(m_vecDlgItems[index]);
		m_vecSubDialogs[index]->SetSize(m_pSubItem->GetSize().cx, m_pSubItem->GetSize().cy);
	}
	
	SetSize(w, h);
	
 	PAUISUBDIALOG pSubD = (PAUISUBDIALOG)GetGameUIMan()->GetDialog("Win_Achievement")->GetDlgItem("Sub_Detail");
 	pSubD->SetSubDialogOrgSize(w, h, false);
	pSubD->UpdateViewport();
	return true;
}


void CDlgAchivItemMan::ShowAchivItem(int nIndex)
{
	if (nIndex>=(int)m_vecDlgItems.size())
	{
		return;
	}
	m_nIndexCur = nIndex;
}