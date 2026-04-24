// Filename	:	DlgUpgrade.h
// Creator	:	Fu Chonggang
// Date		:	2012.5.19
// 每次更新内容

#include "AIniFile.h"
#include "DlgUpgrade.h"
#include "windowsx.h"
#include "aui\\CSplit.h"
#include "AUI\\AUITextArea.h"
#include "DlgTask.h"
#include "DlgMiniMap.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_AutoMove.h"
#include "EC_Game.h"


AUI_BEGIN_COMMAND_MAP(CDlgFormatText, CDlgBase)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgFormatText, CDlgBase)

AUI_ON_EVENT("Txt_*",	WM_LBUTTONDOWN,	OnEventLButtonDownText)

AUI_ON_EVENT("Txt_*",	WM_MOUSEMOVE,	OnEventMouseMoveText)

AUI_END_EVENT_MAP()


extern ACString g_AutoMoveDestName;

CDlgFormatText::CDlgFormatText()
{
}

CDlgFormatText::~CDlgFormatText()
{
}

void CDlgFormatText::OnEventLButtonDownText(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
	
	AUITEXTAREA_EDITBOX_ITEM Item;
	Item.m_pItem = NULL;
	CDlgTask::GetItemLinkItemOn(x, y, pObj, &Item);

	if( Item.m_pItem != NULL )
	{
		if( Item.m_pItem->GetType() == enumEICoord )
		{
			A3DVECTOR3 vec;
			int idInstance;
			a_sscanf(Item.m_pItem->GetInfo(), _AL("%f%f%f%d"), 
				&vec.x, &vec.y, &vec.z, &idInstance);
			{
				CDlgMiniMap* pDlgMiniMap = (CDlgMiniMap*)GetGameUIMan()->GetDialog("Win_MiniMap");
				pDlgMiniMap->m_dwTempTargetTime = GetTickCount();
				pDlgMiniMap->m_vecTempTarget = vec;
				pDlgMiniMap->m_idTempTargetInstance = idInstance;
				
				g_ECAutoMove.SearchPath(GetHostPlayer()->GetPos(), vec, idInstance);
				g_AutoMoveDestName = Item.m_pItem->GetName();
			}
		}
	}
}

void CDlgFormatText::OnEventMouseMoveText(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
	
	AUITEXTAREA_EDITBOX_ITEM Item;
	Item.m_pItem = NULL;
	CDlgTask::GetItemLinkItemOn(x, y, pObj, &Item);
}

void CDlgFormatText::UpdateTextArea(AUITextArea* pTxt)
{
	if (!pTxt)
		return;

	ACString strText;
	ACString strOrg = pTxt->GetText();
	while(strOrg.GetLength()>0)
	{
		if ( strOrg.GetLength()<(int)a_strlen(_AL("&location")) )
		{
			strText += strOrg;
			break;
		}
		else if ( strOrg.Left(a_strlen(_AL("&location")))==_AL("&location") )
		{
			int i;
			ACString strInfo;
			for (i=1; i<strOrg.GetLength(); ++i)
			{
				if (strOrg[i]==_AL('&'))
				{
					strInfo = strOrg.Mid(a_strlen(_AL("&location")), i-a_strlen(_AL("&location")));
					break;
				}
			}

			if (i==strOrg.GetLength())
			{
				strText += strOrg;
				break;
			}
			else
			{
				strInfo.CutLeft(1);
				strInfo.CutRight(1);
				CSplit s = strInfo;
				CSPLIT_STRING_VECTOR vec = s.Split(_AL(","));
				if (vec.size()==5)
				{
					EditBoxItemBase item(enumEICoord);
					item.SetName(vec[0]);
					ACString strPos;
					strPos = vec[1]+_AL(" ")+vec[2]+_AL(" ")+vec[3]+_AL(" ")+vec[4];
					item.SetInfo(strPos);
					item.SetColor(A3DCOLORRGB(0, 255, 0));
					strText += (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
					strOrg.CutLeft(i+1);
				}
				else
				{
					strText += strOrg[0];
					strOrg.CutLeft(1);
					continue;
				}
			}
		}
		else
		{
			strText += strOrg[0];
			strOrg.CutLeft(1);
		}
	}
	
	pTxt->SetText(strText);
}


AUI_BEGIN_COMMAND_MAP(CDlgUpgrade, CDlgFormatText)

AUI_ON_COMMAND("Btn_Link",		OnCommandLink)
AUI_ON_COMMAND("IDCANCEL",		OnCommandCancel)

AUI_END_COMMAND_MAP()

bool CDlgUpgrade::OnInitDialog()
{
	UpdateTextArea((PAUITEXTAREA)GetDlgItem("Txt_1"));
	UpdateTextArea((PAUITEXTAREA)GetDlgItem("Txt_2"));
	UpdateTextArea((PAUITEXTAREA)GetDlgItem("Txt_3"));
	UpdateTextArea((PAUITEXTAREA)GetDlgItem("Txt_4"));
	UpdateTextArea((PAUITEXTAREA)GetDlgItem("Txt_5"));
	UpdateTextArea((PAUITEXTAREA)GetDlgItem("Txt_6"));
	return true;
}

void CDlgUpgrade::OnCommandLink(const char* szCommand)
{
	AString strText = "";
	AIniFile iniFile;
	if(iniFile.Open("configs\\url.ini"))
	{
		strText = iniFile.GetValueAsString("ingame", "updatepage", "");
		iniFile.Close();
	}
	if( strText != "" )
		GetGame()->OpenURL(strText);
}

void CDlgUpgrade::OnCommandCancel(const char* szCommand)
{
	Show(false);
}