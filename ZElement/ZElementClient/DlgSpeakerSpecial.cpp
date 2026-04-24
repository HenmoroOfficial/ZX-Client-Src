// Filename	: DlgSpeakerSpecial.cpp
// Creator	: Fu Chonggang
// Date		: 2012.6.5

#include "AUI\\AUIDef.h"
#include "AFI.h"
#include "DlgSpeakerSpecial.h"
#include "EC_GameUIMan.h"
#include "DlgChat.h"
#include "DlgFriendChat.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_GameSession.h"
#include "EC_Achievement.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgSpeakerSpecial, CDlgBase)

AUI_ON_COMMAND("popface",			OnCommandPopface)
AUI_ON_COMMAND("send",				OnCommandSend)
AUI_ON_COMMAND("Btn_Hotkey",		OnCommandHotKey)
AUI_ON_COMMAND("Btn_Close",			OnCommandCancel)
AUI_ON_COMMAND("Btn_Cancel",		OnCommandCancel)
AUI_ON_COMMAND("IDCANCEL",			OnCommandCancel)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgSpeakerSpecial, CDlgBase)

AUI_ON_EVENT("Txt_Content",	WM_SYSCHAR,		OnEventSysChar)
AUI_ON_EVENT("Txt_Content",	WM_KEYDOWN,		OnEventKeyDown)

AUI_END_EVENT_MAP()

CDlgSpeakerSpecial::CDlgSpeakerSpecial()
{
	m_pTxtContent = NULL;
}

CDlgSpeakerSpecial::~CDlgSpeakerSpecial()
{
}

bool CDlgSpeakerSpecial::OnInitDialog()
{
	DDX_Control("Txt_Content", m_pTxtContent);
	m_pTxtContent->ApplyMaxLengthToRender(true);
	GetGameUIMan()->AttachEmotion(m_pTxtContent);

	SetData(-1, "index");
	m_iChannel = 0;
	return true;
}

void CDlgSpeakerSpecial::OnTick()
{
	int nPack, nSlot;
	if( m_pTxtContent->HasIvtrItem() )
		nPack = m_pTxtContent->GetIvtrItemPack();
	else
		nPack = -1;
	nSlot = m_pTxtContent->GetIvtrItemIndex();
	
	ACString strText = m_pTxtContent->GetText();
	int iIndex;
	bool bCut = false;
	while( (iIndex=strText.Find(_AL('\r'))) >=0 )
	{
		ACString strTemp = strText;
		strText = strTemp.Left(iIndex);
		strTemp.CutLeft(iIndex+1);
		strText += strTemp;
		bCut = true;
	}
	if (bCut)
	{
		// Get achieve id from editor.
		unsigned int achivID = -1;
		if (m_pTxtContent->GetItemCount(enumEITask) > 0)
		{
			EditBoxItemMap::iterator& it = ((EditBoxItemsSet*) (&m_pTxtContent->GetItemSet()))->GetItemIterator();
			while(!it.is_eof())
			{
				EditBoxItemBase* pItem = it->second;
				if (pItem->GetType() == enumEITask && pItem->GetExtraData() != NULL)
				{
					achivID = *(unsigned int*) pItem->GetExtraData();
					break;
				}

				++it;
			}
		}

		m_pTxtContent->SetText(strText);
		if (nPack>=0)
		{
			CECIvtrItem *pItem = GetHostPlayer()->GetPack(nPack)->GetItem(nSlot);
			if( pItem )
			{
				ACString strName;
				A3DCOLOR color;
				CDlgChat::TransformItemColorName(pItem, strName, color);
				m_pTxtContent->SetTextAndItem(strText, strName, color);
				m_pTxtContent->SetIvtrItemLocInfo(nPack, nSlot);
			}
		}
		else if (m_pTxtContent->GetItemCount(enumEITask) > 0 && achivID != -1)
		{
			// Set achieve id to editor.
			EditBoxItemMap::iterator& it = ((EditBoxItemsSet*) (&m_pTxtContent->GetItemSet()))->GetItemIterator();
			while(!it.is_eof())
			{
				EditBoxItemBase* pItem = it->second;
				if (pItem->GetType() == enumEITask)
				{
					CECAchievementMan* pMan = GetHostPlayer()->GetAchievementMan();
					sAchievement* achiv = pMan->GetAchievement(achivID);
					if (achiv)
					{
						ACString strName;
						A3DCOLOR color;
						CDlgChat::TransformAchievementColorName(achiv, strName, color);

						pItem->SetColor(color);
						pItem->SetName(strName);

						pItem->SetExtraData(&achiv->id, sizeof(unsigned int));
						break;
					}
				}

				++it;
			}
		}
	}
}

void CDlgSpeakerSpecial::OnCommandPopface(const char * szCommand)
{
	GetGameUIMan()->GetDialog("Win_PopTalk")->Show(false);
	CDlgChat* pShow = (CDlgChat*)GetGameUIMan()->GetDialog("Win_Popface");
	
	pShow->Show(!pShow->IsShow());
	pShow->AlignTo(this, AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT,
		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_BOTTOM);
	pShow->SetDataPtr(this,"ptr_AUIDialog");
}

void CDlgSpeakerSpecial::OnCommandSend(const char * szCommand)
{
	int nPack, nSlot;
	if( m_pTxtContent->HasIvtrItem() )
		nPack = m_pTxtContent->GetIvtrItemPack();
	else
		nPack = -1;
	nSlot = m_pTxtContent->GetIvtrItemIndex();

	int nAchievement = -1;
	if (m_pTxtContent->GetItemCount(enumEITask) > 0)
	{
		EditBoxItemMap::iterator& it = ((EditBoxItemsSet*) (&m_pTxtContent->GetItemSet()))->GetItemIterator();
		while(!it.is_eof())
		{
			EditBoxItemBase* pItem = it->second;
			if (pItem->GetType() == enumEITask)
			{
				nAchievement = *(unsigned int*)pItem->GetExtraData();
				break;
			}

			++it;
		}
	}

	if (nPack>=0 && nSlot>=0 && GetHostPlayer()->GetPack(nPack)->GetItem(nSlot)==NULL)
	{
		GetGameUIMan()->AddChatMessage(GetStringFromTable(9120), GP_CHAT_MISC);
		return;
	}


	ACString strText = m_pTxtContent->GetText();
	int nLength = EditBoxItemBase::GetUnserializedLength(strText);
	if (nPack>=0)
	{
		nLength += a_strlen(GetHostPlayer()->GetPack(nPack)->GetItem(nSlot)->GetName());
		nLength += 1;	// +2-1
	}
	if( strText.GetLength() == 0 )
		return;
	else if (nLength>30)
	{
		GetGameUIMan()->MessageBox(15003);
		return;
	}
	GetGameUIMan()->FilterBadWords(strText);
	GetGameSession()->SendChatData(m_iChannel, strText, nPack, nSlot, GetData("index"), nAchievement);

	OnCommandCancel("");
}

void CDlgSpeakerSpecial::OnCommandHotKey(const char * szCommand)
{
	PAUIDIALOG pDlgHotKey = GetGameUIMan()->GetDialog("Win_FriendChatHotkey");
	PAUIOBJECT pObjBase = GetDlgItem("Btn_Hotkey");
	pDlgHotKey->SetPosEx(pObjBase->GetPos().x, pObjBase->GetPos().y+pObjBase->GetSize().cy);
	pDlgHotKey->Show(!pDlgHotKey->IsShow());
	pDlgHotKey->CheckRadioButton(1, CDlgFriendChat::GetHotKey()+1);
}

void CDlgSpeakerSpecial::OnCommandCancel(const char * szCommand)
{
	if (GetData("index")!=-1 && GetHostPlayer()->GetPack()->GetItem(GetData("index")))
	{
		GetHostPlayer()->GetPack()->GetItem(GetData("index"))->Freeze(false);
	}
	SetData(-1, "index");
	Show(false);
}

void CDlgSpeakerSpecial::OnEventSysChar(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if( (TCHAR(wParam) == 's' || TCHAR(wParam) == 'S') )
		OnCommandSend("send");
}

void CDlgSpeakerSpecial::OnEventKeyDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if( int(wParam) == VK_RETURN && 
		( (AUI_PRESS(VK_CONTROL)&&CDlgFriendChat::GetHotKey()==CDlgFriendChat::FCHK_CTRLENTER) ||
		(AUI_PRESS(VK_SHIFT)&&CDlgFriendChat::GetHotKey()==CDlgFriendChat::FCHK_SHIFTENTER) ) ) 
		OnCommandSend("send");
}

int CDlgSpeakerSpecial::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="show")
	{
		if (GetData("index")!=-1 && GetHostPlayer()->GetPack()->GetItem(GetData("index")))
		{
			GetHostPlayer()->GetPack()->GetItem(GetData("index"))->Freeze(false);
		}
		Show(true);
		SetData(wParam, "index");
		GetHostPlayer()->GetPack()->GetItem(GetData("index"))->Freeze(true);
		if (lParam>0)
		{
			m_iChannel = GP_CHAT_RUMOR;
			GetDlgItem("Title2")->Show(true);
			GetDlgItem("Lab_Hint2")->Show(true);
			GetDlgItem("Title")->Show(false);
			GetDlgItem("Lab_Hint")->Show(false);
		}
		else
		{
			m_iChannel = GP_CHAT_SPEAKER;
			GetDlgItem("Title2")->Show(false);
			GetDlgItem("Lab_Hint2")->Show(false);
			GetDlgItem("Title")->Show(true);
			GetDlgItem("Lab_Hint")->Show(true);
		}
	}

	return 0;
}
