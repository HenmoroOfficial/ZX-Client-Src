/********************************************************************
	created:	2013/2/27
	file name:	DlgFlower.cpp
	author:		zhuyan
	
	purpose:    鲜花宝贝
*********************************************************************/

#include "AFI.h"
#include "AIniFile.h"
#include "DlgFlower.h"
#include "DlgInputNO.h"
#include "DlgChat.h"
#include "DlgQShop.h"
#include "EC_GameRun.h"
#include "EC_Game.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "EC_UIManager.h"
#include "EC_World.h"
#include "EC_IvtrItem.h"
#include "AUI\\AUICTranslate.h"
#include "A2DSprite.h"
#include "A3DGFXExMan.h"

#include "globaldataman.h"
int g_NumLevel[] =
{
	99,
	299,
	520,
	999,
	9999
};

int g_Ranking[] =
{
	1,
	2,
	3,
	10,
	100,
	500
};

AUI_BEGIN_COMMAND_MAP(CDlgFlowerMin, CDlgBase)

AUI_ON_COMMAND("Btn_Max",		OnCommandOpen)

AUI_END_COMMAND_MAP()

bool CDlgFlowerMin::OnInitDialog()
{
	return true;
}

void CDlgFlowerMin::OnTick()
{
	if(GetHostPlayer()->GetSrcServerID() != 0)
	{
		GetDlgItem("Btn_Max")->Enable(false);
	}
	else
	{
		GetDlgItem("Btn_Max")->Enable(true);
	}
}

void CDlgFlowerMin::OnCommandOpen(const char * szCommand)
{
	PAUIDIALOG	pDlg = GetGameUIMan()->GetDialog("Win_Roserank");
	pDlg->Show(!pDlg->IsShow());
}

AUI_BEGIN_COMMAND_MAP(CDlgFlowerRankaward, CDlgBase)


AUI_END_COMMAND_MAP()

bool CDlgFlowerRankaward::OnInitDialog()
{
	PAUIOBJECTLISTELEMENT pElement = GetFirstControl();
	CECIvtrItem* pItem = NULL;
	AString strFile;
	AUICTranslate trans;
	while( pElement )
	{
		if( strstr(pElement->pThis->GetName(), "Item_") )
		{
			int id = atoi(pElement->pThis->GetName() + strlen("Item_")); // 后strlen只是获取字符串长度
			PAUIIMAGEPICTURE pImage = static_cast<PAUIIMAGEPICTURE>(pElement->pThis);
			if( id == 0 )
			{
				pElement = pElement->pNext;
				continue;
			}	
			CECIvtrItem* pItem = CECIvtrItem::CreateItem(id, 0,1);
			AString strFile;
			PAUIIMAGEPICTURE pPicture = (PAUIIMAGEPICTURE)GetDlgItem(pElement->pThis->GetName());
			if (pItem )
			{
				pItem->SetNeedUpdate(false);
				af_GetFileTitle(pItem->GetIconFile(), strFile);
				strFile.MakeLower();
				pPicture->SetCover(GetGameUIMan()->GetIconSet(CECGameUIMan::ICONS_INVENTORY),
					GetGameUIMan()->GetIconId(CECGameUIMan::ICONS_INVENTORY,strFile));
				pPicture->SetCover(NULL, 0, 1);
				pPicture->SetHint(trans.Translate(pItem->GetDesc()));
				delete pItem;
			}
		}
		pElement = pElement->pNext;
	}
	
	return true;	
}


AUI_BEGIN_COMMAND_MAP(CDlgFlowerManifesto, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",	OnCommandCancel)
AUI_END_COMMAND_MAP()

bool CDlgFlowerManifesto::OnInitDialog()
{
	return true;	
}

void CDlgFlowerManifesto::OnShowDialog()
{
	SetAlpha(255);
	m_dwTimeClose = GetTickCount();
	m_bIsStart = false;
	
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_ChatInfo2");
	POINT pPoint = pDlg->GetPos();
	SetPosEx((int)pPoint.x + pDlg->GetSize().cx - GetSize().cx / 2, (int)pPoint.y - GetSize().cy / 2);
}

void CDlgFlowerManifesto::OnCommandCancel(const char * szCommand)
{
	Show(false);
}

void CDlgFlowerManifesto::OnTick()
{
	static DWORD dwTime = GetTickCount();
	DWORD dwTimeNow = GetTickCount();
	if(m_bIsStart == false)
	{
		if ((dwTimeNow - m_dwTimeClose) >= m_nCloseTime)
		{
			m_bIsStart = true;
			m_dwTimeClose = GetTickCount();
		}
	}
	else if(m_bIsStart == true)
	{
		if (GetAlpha()==0)
			Show(false);
		int alpha = 255-(dwTimeNow - m_dwTimeClose)/10;
		if (alpha<0)
			alpha = 0;
		SetAlpha(alpha);
	}

}

void CDlgFlowerManifesto::Resize(A3DRECT rcOld, A3DRECT rcNew)
{
	AUIDialog::Resize(rcOld, rcNew);
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_ChatInfo2");
	APointF pPoint = pDlg->GetPercentPos();
	SetPosEx((int)pPoint.x + pDlg->GetSize().cx - GetSize().cx / 2, (int)pPoint.y - GetSize().cy / 2);
}

void CDlgFlowerManifesto::setWordsText(ACString szText)
{
	AUILabel* pLabel = (AUILabel*)GetDlgItem("Txt_Words");

	pLabel->SetText(szText);
}


AUI_BEGIN_COMMAND_MAP(CDlgFlowerRankOption, CDlgBase)
AUI_ON_COMMAND("Btn_Rose",	OnCommandSendFlower)

AUI_END_COMMAND_MAP()

bool CDlgFlowerRankOption::OnInitDialog()
{
	m_bIsSend = false;
	return true;	
}


void CDlgFlowerRankOption::OnCommandSendFlower(const char * szCommand)
{
	Show(false);
	DlgFlowersList* pDlgFlowersList = (DlgFlowersList*)GetGameUIMan()->GetDialog("Win_Roserank");
	FLOWER_DATA* pData = NULL;
	PAUILISTBOX pList = NULL;
	ACString strText;
	if(m_bIsSend)
	{
		pList = pDlgFlowersList->m_pLst_Receive;
	}
	else
	{
		pList = pDlgFlowersList->m_pLst_Send;
	}
	int nSel = pList->GetCurSel();
	if( nSel < 0 || nSel >= pList->GetCount() )
		return;
	int nIndxe = pList->GetItemData(nSel, 0);
	pData = pDlgFlowersList->getFlowerData(m_bIsSend,nIndxe);
	if(pData)
	{
		ACString strName = pData->rolename;
		int idPlayer = pData->roleid;
		
		DlgFlowers*	pDlg = (DlgFlowers*)GetGameUIMan()->GetDialog("Win_Rose");
		if(pDlg)
		{
			pDlg->Show(true);
			pDlg->UpdateUI(idPlayer,strName);
		}
	}
	
}


AUI_BEGIN_COMMAND_MAP(CDlgFlowerPopup, CDlgBase)

AUI_ON_COMMAND("Btn_Confirm",		OnCommandConfirm)
AUI_ON_COMMAND("Btn_Cancel",	OnCommandCancel)

AUI_END_COMMAND_MAP()

bool CDlgFlowerPopup::OnInitDialog()
{
	return true;	
}

void CDlgFlowerPopup::OnCommandConfirm(const char * szCommand)
{
	PAUIDIALOG	pDlg = GetGameUIMan()->GetDialog("Win_QShop");
	pDlg->Show(true);	
	Show(false);
}

void CDlgFlowerPopup::OnCommandCancel(const char * szCommand)
{
	PAUIDIALOG	pDlg = GetGameUIMan()->GetDialog("Win_Roserank");
	pDlg->Show(!pDlg->IsShow());	
}

AUI_BEGIN_COMMAND_MAP(CDlgFlowerGfx, CDlgBase)

AUI_END_COMMAND_MAP()

bool CDlgFlowerGfx::OnInitDialog()
{
	m_pGfx = (PAUIIMAGEPICTURE)GetDlgItem("Gfx_1");
	m_pGfx->SetGfx("");
	return true;	
}


void CDlgFlowerGfx::OnShowDialog()
{
	m_dwTimeClose = GetTickCount();
}

void CDlgFlowerGfx::OnHideDialog()
{

}

void CDlgFlowerGfx::OnTick()
{
	static DWORD dwTime = GetTickCount();
	DWORD dwTimeNow = GetTickCount();
	if ((dwTimeNow - m_dwTimeClose) >= m_nCloseTime)
	{
		Show(false);
	}
}

void CDlgFlowerGfx::setPlayGfx(int nFlowerNum)
{
	int nIndex = 0;
	if(g_NumLevel[0] <= nFlowerNum && nFlowerNum < g_NumLevel[1])
	{
		nIndex = 1;
	}
	else if(g_NumLevel[1] <= nFlowerNum && nFlowerNum < g_NumLevel[2])
	{
		nIndex = 2;
	}
	else if(g_NumLevel[2] <= nFlowerNum && nFlowerNum < g_NumLevel[3])
	{
		nIndex = 3;
	}
	else if(g_NumLevel[3] <= nFlowerNum && nFlowerNum < g_NumLevel[4])
	{
		nIndex = 4;
	}
	else if(g_NumLevel[4] <= nFlowerNum)
	{
		nIndex = 5;
	}
	if(nIndex == 0)
	{
		m_pGfx->SetGfx("");
	}
	else
	{
		char szName[80];
		sprintf(szName, "界面\\玫瑰花绽放0%d.gfx", nIndex);
		m_pGfx->SetGfx(szName);
	}
}



void CDlgFlowerGfx::setContentToChat(int nChannel,ACString szContent)
{
	GetGameUIMan()->AddChatMessage(szContent, nChannel);
}

AUI_BEGIN_COMMAND_MAP(CDlgFlowerAnounce, CDlgBase)


AUI_END_COMMAND_MAP()

bool CDlgFlowerAnounce::OnInitDialog()
{
	pLabel1 = (PAUILABEL)GetDlgItem("Txt_Words01");
	pLabel2 = (PAUILABEL)GetDlgItem("Txt_Words02");
	pLabel3 = (PAUILABEL)GetDlgItem("Txt_Words03");
	m_nIndex = 1;

	m_nPos1.x = 22;
	m_nPos1.y = 23;
	m_nPos2.x = 22;
	m_nPos2.y = 38;
	m_nPos3.x = 22;
	m_nPos3.y = 53;

	pLabel1->SetText(_AL(""));
	pLabel2->SetText(_AL(""));
	pLabel3->SetText(_AL(""));
	return true;	
}

void CDlgFlowerAnounce::OnShowDialog()
{

}

void CDlgFlowerAnounce::OnTick()
{
	if(IsShow() == true && pLabel1->IsMarquee() == false
		&& pLabel2->IsMarquee() == false
		&& pLabel3->IsMarquee() == false)
	{
		Show(false);
		m_nIndex = 1;
	}
}


void CDlgFlowerAnounce::addMsg(ACString szTemp)
{
	if( !IsShow() ) 
	{
		Show(true, false, false);
	}

	if(m_nIndex == 1)
	{
		if(pLabel2->IsMarquee() && pLabel3->IsMarquee())
		{
			pLabel1->SetPos(m_nPos3.x,m_nPos3.y);
			pLabel2->SetPos(m_nPos1.x,m_nPos1.y);
			pLabel3->SetPos(m_nPos2.x,m_nPos2.y);
		}
		else
		{
			pLabel1->SetPos(m_nPos1.x,m_nPos1.y);
		}
		pLabel1->SetText(szTemp);
		pLabel1->SetMarquee(true, 2, 40);
		m_nIndex++;
	}
	else if(m_nIndex == 2)
	{
		if(pLabel3->IsMarquee())
		{
			pLabel1->SetPos(m_nPos2.x,m_nPos2.y);
			pLabel2->SetPos(m_nPos3.x,m_nPos3.y);
			pLabel3->SetPos(m_nPos1.x,m_nPos1.y);
		}
		else
		{
			pLabel1->SetPos(m_nPos1.x,m_nPos1.y);
			pLabel2->SetPos(m_nPos2.x,m_nPos2.y);
		}
		pLabel2->SetText(szTemp);
		pLabel2->SetMarquee(true, 2, 40);

		m_nIndex++; 
	}
	else if(m_nIndex == 3)
	{
		if(!pLabel1->IsMarquee())
		{
			pLabel2->SetPos(m_nPos1.x,m_nPos1.y);
			pLabel3->SetPos(m_nPos2.x,m_nPos2.y);
		}
		else
		{
			pLabel1->SetPos(m_nPos1.x,m_nPos1.y);
			pLabel2->SetPos(m_nPos2.x,m_nPos2.y);
			pLabel3->SetPos(m_nPos3.x,m_nPos3.y);
		}
		
		pLabel3->SetText(szTemp);
		pLabel3->SetMarquee(true, 2, 40);
		m_nIndex = 1;
	}
	int nHigh = 0;
	if(pLabel1->IsMarquee())
	{
		nHigh = 18;
	}
	if(pLabel2->IsMarquee())
	{
		nHigh += 18;
	}
	if(pLabel3->IsMarquee())
	{
		nHigh += 18;
	}
	AUIOBJECT_SETPROPERTY p;
	p.i = nHigh;
	SetProperty("Height",&p);
}


	


AUI_BEGIN_COMMAND_MAP(DlgFlowers, CDlgBase)
AUI_ON_COMMAND("Btn_Confirm",		OnCommandConfirm)
AUI_ON_COMMAND("Btn_Friendlist",	OnCommandFriendlist)
AUI_ON_COMMAND("Btn_Buy",	OnCommandBuy)

AUI_ON_COMMAND("Btn_Add",		OnCommandPrev01)
AUI_ON_COMMAND("Btn_Minus",	OnCommandNext01)
AUI_ON_COMMAND("Rdo_0*", OnCommandRdoType)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(DlgFlowers, CDlgBase)

AUI_END_EVENT_MAP()


bool DlgFlowers::OnInitDialog()
{
	m_pEdit = (PAUIEDITBOX)GetDlgItem("Edt_Num");
	m_pTxt_Num = (AUILabel*)GetDlgItem("Txt_Num");
	m_pTxt_Money = (AUILabel*)GetDlgItem("Txt_Num02");
	
	m_pEditWords = (PAUIEDITBOX)GetDlgItem("Edt_Words");
	m_pEditWords->Enable(false);
	m_pEditWords->ApplyMaxLengthToRender();

	GetDlgItem("Btn_Add")->Enable(false);
	GetDlgItem("Btn_Minus")->Enable(false);
	m_nType = 1;


	m_nFlowerIndex = globaldata_getgshopitemindex_by_id(55649);

	return true;
}

void DlgFlowers::OnShowDialog()
{
// 	int index = GetCheckedRadioButton(1);
// 	if (index<1)
// 	{
	int	index = 1;
		CheckRadioButton(1,1);
// 	}
	char szText[20];
	sprintf(szText,"Rdo_%d",index);
	OnCommandRdoType(szText);
	m_pEdit->SetText(_AL(""));
	m_pEditWords->SetText(_AL(""));



}


void DlgFlowers::OnHideDialog()
{

}

void DlgFlowers::OnTick()
{
	if(m_nType == 6)
	{
		if(m_pEdit && m_pEdit->IsFocus() == false)
		{
			int nNum = a_atoi(m_pEdit->GetText());
			if(nNum >= g_NumLevel[2])
			{
				m_pEditWords->Enable(true);
			}
			else
			{
				m_pEditWords->Enable(false);
			}	
		}
	}
	int nFree = GetHostPlayer()->GetPack()->GetItemTotalNum(55648);
	int nPay = GetHostPlayer()->GetPack()->GetItemTotalNum(55649);
	m_nFlowerNum = nFree + nPay;
	ACString strText;
	strText.Format(_AL("%d"),m_nFlowerNum);
	m_pTxt_Num->SetText(strText);

	int nNum = 0;
	if(m_nType != 6)
	{
		nNum = g_NumLevel[m_nType - 1];
	}
	else
	{
		nNum = a_atoi(m_pEdit->GetText());
	}
	strText = _AL("0");
	if(nNum > m_nFlowerNum)
	{
		m_nBuyFlowerNum = nNum - m_nFlowerNum;
		//strText.Format(GetStringFromTable(1403), m_nBuyFlowerNum / 10, (m_nBuyFlowerNum % 10) * 10);
		strText.Format(_AL("%d"), m_nBuyFlowerNum);
	}
	m_pTxt_Money->SetText(strText);
}

void DlgFlowers::UpdateUI(int roleid, ACString szName)
{
	m_pEdit->SetText(_AL(""));
	m_pEditWords->SetText(_AL(""));

	m_nRoleID = roleid;
	ACString strText;
	strText.Format(_AL("%s"),szName);
	GetDlgItem("Edt_Name")->SetText(szName);
	m_szFlowerName = szName;
	
}

bool DlgFlowers::checkSendFlowers()
{
	tm tmNow = GetGame()->GetServerLocalTime();
	if ( checkFlowerTime(tmNow))
	{
		if(GetHostPlayer()->GetSrcServerID() == 0)
		{
			return true;
		}
	}
	return false;
}

void DlgFlowers::OnCommandConfirm(const char * szCommand)
{
	if(GetHostPlayer()->GetSrcServerID() != 0)
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(17833), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	if(GetDlgItem("Edt_Name")->GetText() == _AL(""))
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(261), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	}
	int nNum = 0;
	if(m_nType != 6)
	{
		nNum = g_NumLevel[m_nType - 1];
	}
	else
	{
		nNum = a_atoi(m_pEdit->GetText());
	}


	if(nNum > m_nFlowerNum)
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(17830), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}

	ACString strText;
	strText.Format(_AL("%s"),GetDlgItem("Edt_Name")->GetText());
	if(m_szFlowerName != strText)
	{
		m_nRoleID = 0;	
	}
	strText = m_pEditWords->GetText();
	GetGameUIMan()->FilterBadWords(strText);
	GetGame()->GetGameSession()->flower_send(m_nRoleID,GetDlgItem("Edt_Name")->GetText(),strText,nNum);

	Show(false);
}


void DlgFlowers::OnCommandFriendlist(const char * szCommand)
{
	PAUIDIALOG	pDlg = GetGameUIMan()->GetDialog("Win_FriendList");
	pDlg->Show(!pDlg->IsShow());
}

void DlgFlowers::OnCommandPrev01(const char * szCommand)
{
	int money = 0;
	money = 1 + a_atoi(m_pEdit->GetText());
	if(money > 9999)
	{
		money =  9999;
	}
	ACString strText;
	strText.Format(_AL("%d"),money);
	m_pEdit->SetText(strText);
}

void DlgFlowers::OnCommandNext01(const char * szCommand)
{
	int money = 0;
	money = a_atoi(m_pEdit->GetText()) - 1;
	if(money == 0)
	{
		money = 1;
	}
	ACString strText;
	strText.Format(_AL("%d"),money);
	m_pEdit->SetText(strText);
}

void DlgFlowers::OnCommandRdoType(const char* szCommand)
{
	m_nType = atoi(szCommand+4);
	m_pEditWords->SetText(_AL(""));
	m_pEditWords->Enable(false);
	if(m_nType >= 3)
	{
		m_pEditWords->Enable(true);
	}
	m_pEdit->Enable(false);
	GetDlgItem("Btn_Add")->Enable(false);
	GetDlgItem("Btn_Minus")->Enable(false);
	if(m_nType == 6)
	{
		m_pEdit->Enable(true);
		GetDlgItem("Btn_Add")->Enable(true);
		GetDlgItem("Btn_Minus")->Enable(true);
	}
	
}


void DlgFlowers::OnCommandBuy(const char* szCommand)
{

	vector<GSHOP_ITEM> *items;
	items = globaldata_getgshopitems();
	int m_nMoney = (*items)[m_nFlowerIndex].buy[0].price;
	if(GetHostPlayer()->GetCash() <  (m_nMoney *  m_nBuyFlowerNum))
	{
		PAUIDIALOG	pDlg = GetGameUIMan()->GetDialog("Win_RosePop01");
		pDlg->Show(true);
		return;
	}
	

	ACHAR szText[40];
	PAUIOBJECT pEdit = GetGameUIMan()->m_pDlgInputGNO->GetDlgItem("DEFAULT_Txt_No.");

	
	pEdit->SetData(9999);
	GetGameUIMan()->m_pDlgInputGNO->SetType(CDlgInputNO::INPUTNO_QSHOPITEM_BUY);

	
	GetGameUIMan()->m_pDlgInputGNO->Show(true, true);
	a_sprintf(szText, _AL("%d"), m_nBuyFlowerNum);
	pEdit->SetText(szText);

	PAUIOBJECT pObj = GetGameUIMan()->m_pDlgInputGNO->GetDlgItem("Txt_Confirm");
	int nId = 55649;
	pObj->SetData(m_nFlowerIndex);
	pObj->SetDataPtr((void*)nId);
	GetGameUIMan()->m_pDlgInputGNO->SetData(10);
	
	CECIvtrItem *pItem = CECIvtrItem::CreateItem((*items)[m_nFlowerIndex].id, 0, (*items)[m_nFlowerIndex].num);
	if (pItem)
	{
		GetGameUIMan()->m_pDlgInputGNO->SetText(pItem->GetName());
		delete pItem;
	}

}


AUI_BEGIN_COMMAND_MAP(DlgFlowersList, CDlgBase)
AUI_ON_COMMAND("Btn_Prev01",		OnCommandReceivePrev)
AUI_ON_COMMAND("Btn_Next01",		OnCommandReceiveNext)
AUI_ON_COMMAND("Btn_Prev02",		OnCommandPrev02)
AUI_ON_COMMAND("Btn_Next02",		OnCommandNext02)

AUI_ON_COMMAND("Btn_GetAward01",		OnCommandGetAward01)
AUI_ON_COMMAND("Btn_GetAward02",		OnCommandGetAward02)

AUI_ON_COMMAND("Btn_Link",		OnCommandLink)
AUI_ON_COMMAND("Btn_Present",		OnCommandPresent)
AUI_ON_COMMAND("Btn_Refresh",		OnCommandRefresh)

AUI_ON_COMMAND("Btn_AwardDisplay",		OnCommandAwardDisplay)


AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(DlgFlowersList, CDlgBase)
AUI_ON_EVENT("Lst_Member01",			WM_LBUTTONDOWN,	OnEventLButtonList01)
AUI_ON_EVENT("Lst_Member02",			WM_LBUTTONDOWN,	OnEventLButtonList02)
AUI_ON_EVENT("Lst_Member01",			WM_RBUTTONUP,	OnEventRButtonList01)
AUI_ON_EVENT("Lst_Member02",			WM_RBUTTONUP,	OnEventRButtonList02)

AUI_END_EVENT_MAP()

bool DlgFlowersList::OnInitDialog()
{
	m_pLst_Receive = (AUIListBox *)GetDlgItem("Lst_Member01");
	m_pLst_Send = (AUIListBox *)GetDlgItem("Lst_Member02");

	m_pBtn_LeftAward = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_GetAward01");
	m_pBtn_RightPocket = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_GetAward02");
	m_Image_LeftAward= (AUIImagePicture *)GetDlgItem("Img_Award01");
	m_Image_RightPocket= (AUIImagePicture *)GetDlgItem("Img_Award02");

	m_pTxt_ReceiveRanking = (AUILabel *)GetDlgItem("Txt_SelfRank01");
	m_pTxt_ReceiveNum = (AUILabel *)GetDlgItem("Txt_Num01");
	m_pTxt_SendRanking = (AUILabel*)GetDlgItem("Txt_SelfRank02");
	m_pTxt_SendNum = (AUILabel*)GetDlgItem("Txt_Num02");
	m_pTxt_ReceivePage = (AUILabel*)GetDlgItem("Txt_Page01");
	m_pTxt_SendPage = (AUILabel*)GetDlgItem("Txt_Page02");




	
	m_pTxt_ReceiveRanking->SetText(_AL(""));
	m_pTxt_ReceiveNum->SetText(_AL(""));
	m_pTxt_SendRanking->SetText(_AL(""));
	m_pTxt_SendNum->SetText(_AL(""));
	m_pTxt_ReceivePage->SetText(_AL("1/1"));
	m_pTxt_SendPage->SetText(_AL("1/1"));
	

	m_nReceive = 1;
	m_nSend = 1;
	m_nRecePos = -1;
	m_nSendPos = -1;
	m_bReceive = false;
	m_bSend = false;
	return true;
}


bool checkFlowerTime(tm tmNow)
{
	if ( tmNow.tm_year==113 &&	//2012年
		((tmNow.tm_mon== 2 && ((tmNow.tm_mday == 26 && tmNow.tm_hour >=10) || tmNow.tm_mday > 26))
		|| (tmNow.tm_mon== 3 && (tmNow.tm_mday < 2 || (tmNow.tm_mday == 2 && tmNow.tm_hour < 8)))))
	{
		return true;
	}
	return false;
}

void DlgFlowersList::OnShowDialog()
{
	OnCommandRefresh("");
	m_pLst_Receive->SetCurSel(-1);
	m_pLst_Send->SetCurSel(-1);
	GetDlgItem("Btn_Present")->Enable(false);
	tm tmNow = GetGame()->GetServerLocalTime();
	if (checkFlowerTime(tmNow))
	{
		GetDlgItem("Btn_Present")->Enable(true);
	}
	bool bIsBtnShow = false;
	if ( tmNow.tm_year==113 &&	//2013年
		(tmNow.tm_mon  >= 3 && tmNow.tm_mday >= 2 && tmNow.tm_hour >= 10)	
		||(tmNow.tm_mon >= 3 && tmNow.tm_mday > 2))
	{
		bIsBtnShow = true;
	}

	m_pBtn_LeftAward->Show(false);
	m_pBtn_RightPocket->Show(false);
	m_Image_LeftAward->Show(false);
	m_Image_RightPocket->Show(false);
	if(bIsBtnShow == true)
	{
		m_pBtn_LeftAward->Enable(false);
		m_pBtn_RightPocket->Enable(false);
		m_pBtn_LeftAward->Show(true);
		m_pBtn_RightPocket->Show(true);
	}
}


void DlgFlowersList::OnHideDialog()
{
	
}

void DlgFlowersList::UpdateUiData(int isRecv,int page, int my_pos, int my_count,int all_count)
{
	if(isRecv == 1)
	{
		m_nReceive = page;
		if(all_count / 20 > 0 && all_count % 20 >= 0)
		{
			if(all_count % 20 == 0 && all_count / 20 == 25)
			{
				m_nReceive_All_count = 25;
			}
			else
			{
				m_nReceive_All_count = all_count / 20 + 1;
			}
		}
		else if(all_count / 20 == 0)
		{
			m_nReceive_All_count = 1;
		}	
		m_nRecePos = my_pos;
		ACString szTemp;

		if(my_pos  <= 0)
		{
			m_pTxt_ReceiveRanking->SetText(_AL("0"));
		}
		else
		{
			szTemp.Format(_AL("%d"),my_pos);
			m_pTxt_ReceiveRanking->SetText(szTemp);
		}

		szTemp.Format(_AL("%d"),my_count);
		m_pTxt_ReceiveNum->SetText(szTemp);
		
		szTemp.Format(_AL("%d/%d"),m_nReceive, m_nReceive_All_count);
		m_pTxt_ReceivePage->SetText(szTemp);

		if(m_pBtn_LeftAward->IsShow() && m_bReceive == false)
		{
			/// 显示领取按钮
			if(m_nRecePos >= 0 && my_count >= 9)
			{
				m_pBtn_LeftAward->Enable(true);
				m_Image_LeftAward->Show(true);
				int nItemID = -1;
				if(g_Ranking[0] == m_nRecePos) // 1
				{
					nItemID = 55678;
				}
				else if(g_Ranking[1] == m_nRecePos) // 2
				{
					nItemID = 55679;	
				}
				else if(g_Ranking[2] == m_nRecePos)// 3
				{
					nItemID = 55680;	
				}
				else if(g_Ranking[2] < m_nRecePos && m_nRecePos <= g_Ranking[3]) // 3 - 10
				{
					nItemID = 55681;
				}
				else if(g_Ranking[3] < m_nRecePos && m_nRecePos <= g_Ranking[4]) // 10 - 100
				{
					nItemID = 55682;
				}
				else if(g_Ranking[4] < m_nRecePos && m_nRecePos <= g_Ranking[5]) // 100- 500
				{
					nItemID = 55683;
				}
				else if(m_nRecePos == 0 && my_count >= 9)
				{
					nItemID = 55690;
				}	
				if(nItemID != -1)
				{
					AUICTranslate trans;
					CECIvtrItem* pItem = CECIvtrItem::CreateItem(nItemID, 0,1);
					AString strFile;
					PAUIIMAGEPICTURE pPicture = (PAUIIMAGEPICTURE)GetDlgItem("Img_Award01");
					if (pItem )
					{
						pItem->SetNeedUpdate(false);
						af_GetFileTitle(pItem->GetIconFile(), strFile);
						strFile.MakeLower();
						pPicture->SetCover(GetGameUIMan()->GetIconSet(CECGameUIMan::ICONS_INVENTORY),
							GetGameUIMan()->GetIconId(CECGameUIMan::ICONS_INVENTORY,strFile));
						pPicture->SetCover(NULL, 0, 1);
						pPicture->SetHint(trans.Translate(pItem->GetDesc()));
						delete pItem;
					}
				}
		
			}
			else
			{ 
				m_pBtn_LeftAward->Enable(false);
				m_Image_LeftAward->Show(false);	
			}
		}
	}
	else if(isRecv == 0)
	{
		m_nSend = page;
		if(all_count / 20 > 0 && all_count % 20 >= 0)
		{
			if(all_count % 20 == 0 && all_count / 20 == 25)
			{
				m_nSend_All_count = 25;
			}
			else
			{
				m_nSend_All_count = all_count / 20 + 1;
			}
		}
		else if(all_count / 20 == 0)
		{
			m_nSend_All_count = 1;
		}
		m_nSendPos = my_pos;
		ACString szTemp;
		if(my_pos <= 0)
		{
			m_pTxt_SendRanking->SetText(_AL("0"));
		}
		else
		{
			szTemp.Format(_AL("%d"),my_pos);
			m_pTxt_SendRanking->SetText(szTemp);
		}
		szTemp.Format(_AL("%d"),my_count);
		m_pTxt_SendNum->SetText(szTemp);
		
		szTemp.Format(_AL("%d/%d"),m_nSend, m_nSend_All_count);
		m_pTxt_SendPage->SetText(szTemp);
		
		if(m_pBtn_RightPocket->IsShow() && m_bSend == false)
		{
			if(m_nSendPos >= 0  && my_count >= 9)
			{
				m_pBtn_RightPocket->Enable(true);
				m_Image_RightPocket->Show(true);
				
				int nItemID = -1;
				if(g_Ranking[0] == m_nSendPos) // 1
				{
					nItemID = 55684;
				}
				else if(g_Ranking[1] == m_nSendPos) // 2
				{
					nItemID = 55685;	
				}
				else if(g_Ranking[2] == m_nSendPos)// 3
				{
					nItemID = 55686;	
				}
				else if(g_Ranking[2] < m_nSendPos && m_nSendPos <= g_Ranking[3]) // 3 - 10
				{
					nItemID = 55687;
				}
				else if(g_Ranking[3] < m_nSendPos && m_nSendPos <= g_Ranking[4]) // 10 - 100
				{
					nItemID = 55688;
				}
				else if(g_Ranking[4] < m_nSendPos && m_nSendPos <= g_Ranking[5]) // 100 - 500
				{
					nItemID = 55689;
				}
				else if(m_nSendPos == 0 && my_count >= 9)
				{
					nItemID = 55691;
				}	
				if(nItemID != -1)
				{
					AUICTranslate trans;
					CECIvtrItem* pItem = CECIvtrItem::CreateItem(nItemID, 0,1);
					AString strFile;
					PAUIIMAGEPICTURE pPicture = (PAUIIMAGEPICTURE)GetDlgItem("Img_Award02");
					if (pItem )
					{
						pItem->SetNeedUpdate(false);
						af_GetFileTitle(pItem->GetIconFile(), strFile);
						strFile.MakeLower();
						pPicture->SetCover(GetGameUIMan()->GetIconSet(CECGameUIMan::ICONS_INVENTORY),
							GetGameUIMan()->GetIconId(CECGameUIMan::ICONS_INVENTORY,strFile));
						pPicture->SetCover(NULL, 0, 1);
						pPicture->SetHint(trans.Translate(pItem->GetDesc()));
						delete pItem;
					}
				}
			}
			else
			{
				m_pBtn_RightPocket->Enable(false);
				m_Image_RightPocket->Show(false);
			}
		}
	}
}


void DlgFlowersList::AddMap(int nRece,FLOWER_DATA pData)
{
	if(nRece)
	{
		m_vecRece.push_back(pData);
	}
	else
	{
		m_vecSend.push_back(pData);
	}
}


void  DlgFlowersList::ClearReceiveMap()
{
	m_vecRece.clear();
}

void DlgFlowersList::ClearSend()
{
	m_vecSend.clear();
}



void DlgFlowersList::UpdateFlowerList(int isRece)
{
	ACString strText;
	if(isRece == 1)
	{
		m_pLst_Receive->ResetContent();
		abase::vector<FLOWER_DATA>::iterator it = m_vecRece.begin();
		for ( ; it!= m_vecRece.end(); ++it)
		{
			FLOWER_DATA* pData = it;
			if(pData->rolename.IsEmpty() == true)
			{
				strText.Format( _AL("%d\t\t\t%d"), pData->nIndex,pData->count);
			}
			else
			{
				strText.Format( _AL("%d\t%s\t%d"), pData->nIndex,pData->rolename,pData->count);
			}
			m_pLst_Receive->AddString(strText);
			m_pLst_Receive->SetItemData(m_pLst_Receive->GetCount()-1,pData->nIndex, 0);	//index
		}
		m_pLst_Receive->SetCurSel(-1);
	}
	else
	{
		m_pLst_Send->ResetContent();
		abase::vector<FLOWER_DATA>::iterator it = m_vecSend.begin();
		int nIndex = 0;
		for ( ; it!=m_vecSend.end(); ++it)
		{
			FLOWER_DATA* pData =  it;

			if(pData->rolename.IsEmpty() == true)
			{
			strText.Format( _AL("%d\t\t\t%d"), pData->nIndex,pData->count);
			}
			else
			{
			strText.Format( _AL("%d\t%s\t%d"), pData->nIndex,pData->rolename,pData->count);
			}
				
			m_pLst_Send->AddString(strText);
			m_pLst_Send->SetItemTextColor(m_pLst_Send->GetCount() -1 ,0xff98e3fc,0);
			if(pData->gender == 0)
			{		
				m_pLst_Send->SetItemTextColor(m_pLst_Send->GetCount() -1 ,0xff98e3fc,1);
				m_pLst_Send->SetItemTextColor(m_pLst_Send->GetCount() -1 ,0xff98e3fc,2);
			}
			else
			{	
				m_pLst_Send->SetItemTextColor(m_pLst_Send->GetCount() -1,0xfff4ea8a,1);
				m_pLst_Send->SetItemTextColor(m_pLst_Send->GetCount() -1,0xfff4ea8a,2);
			}
			
			m_pLst_Send->SetItemData(m_pLst_Send->GetCount()-1,pData->nIndex, 0);	//index
		}
		m_pLst_Send->SetCurSel(-1);
	}
}


FLOWER_DATA* DlgFlowersList::getFlowerData(bool isRece, int nIndex)
{
	if(isRece == true)
	{
		for(int _i = 0; _i < (int)m_vecRece.size(); _i++)
		{
			FLOWER_DATA* pData = &m_vecRece[_i];
			if(pData->nIndex == nIndex)
			{
				return pData;
			}
		}
	}
	else
	{
		for(int _i = 0; _i < (int)m_vecSend.size(); _i++)
		{
			FLOWER_DATA* pData = &m_vecSend[_i];
			if(pData->nIndex == nIndex)
			{
				return pData;
			}
		}
	}
	return NULL;
}


void DlgFlowersList::OnCommandReceivePrev(const char * szCommand)
{
	m_nReceive -= 1;
	if(m_nReceive == 0)
	{
		m_nReceive = 1;
	}
	GetGame()->GetGameSession()->top_flower(true,m_nReceive);
}


void DlgFlowersList::OnCommandReceiveNext(const char * szCommand)
{
	m_nReceive += 1;
	if(m_nReceive >=  m_nReceive_All_count)
	{
		m_nReceive = m_nReceive_All_count;
	}
	GetGame()->GetGameSession()->top_flower(true,m_nReceive);
}

void DlgFlowersList::OnCommandPrev02(const char * szCommand)
{
	m_nSend -= 1;
	if(m_nSend == 0)
	{
		m_nSend = 1;
	}
	GetGame()->GetGameSession()->top_flower(false,m_nSend);
}


void DlgFlowersList::OnCommandNext02(const char * szCommand)
{
 	m_nSend += 1;
	if(m_nSend >= m_nSend_All_count)
	{
		m_nSend = m_nSend_All_count;
	}
	GetGame()->GetGameSession()->top_flower(false,m_nSend);
}

void DlgFlowersList::OnCommandGetAward01(const char * szCommand)
{
	GetGame()->GetGameSession()->getgiftFlower(1);
	m_pBtn_LeftAward->Enable(false);
	m_Image_LeftAward->Show(false);
	m_bReceive = true;
}

void DlgFlowersList::OnCommandGetAward02(const char * szCommand)
{
	GetGame()->GetGameSession()->getgiftFlower(0);	
	m_pBtn_RightPocket->Enable(false);
	m_Image_RightPocket->Show(false);
	m_bSend = true;
}

void DlgFlowersList::OnCommandLink(const char * szCommand)
{
	AString strText = "";
	AIniFile iniFile;
	if(iniFile.Open("configs\\url.ini"))
	{
		strText = iniFile.GetValueAsString("flowers", "Link", "");
		iniFile.Close();
	}
	if( strText != "" )
	{
		ShellExecuteA(NULL,"open", strText, NULL, NULL, SW_SHOWNORMAL);
	}
}


void DlgFlowersList::OnCommandPresent(const char * szCommand)
{
	Show(false);
	int index = -1;
	int nSel = m_pLst_Send->GetCurSel();
	bool bIsRece = false;
	DlgFlowers*	pDlg = (DlgFlowers*)GetGameUIMan()->GetDialog("Win_Rose");
		
	if(nSel != -1 || nSel < m_pLst_Send->GetCount())
	{
		index = m_pLst_Send->GetItemData(nSel, 0);
		bIsRece = false;
	}
	nSel = m_pLst_Receive->GetCurSel();
	if(nSel != -1 || nSel < m_pLst_Receive->GetCount())
	{
		index = m_pLst_Receive->GetItemData(nSel, 0);
		bIsRece = true;
	}
	if(index != -1)
	{
		FLOWER_DATA* pData = getFlowerData(bIsRece,index);
		pDlg->UpdateUI(pData->roleid,pData->rolename);
	}
	else
	{
		pDlg->UpdateUI(0,_AL(""));
	}

	pDlg->Show(true);
}


void DlgFlowersList::OnCommandRefresh(const char * szCommand)
{
	GetGame()->GetGameSession()->top_flower(true,m_nReceive);
	GetGame()->GetGameSession()->top_flower(false,m_nSend);
}


void DlgFlowersList::OnCommandAwardDisplay(const char * szCommand)
{
	PAUIDIALOG	pDlg = GetGameUIMan()->GetDialog("Win_Roserankaward");
	pDlg->Show(!pDlg->IsShow());	
}

void DlgFlowersList::OnEventLButtonList01(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	m_nCurSel = m_pLst_Receive->GetCurSel();
	int nSel = m_pLst_Receive->GetCurSel();
	if( nSel < 0 || nSel >= m_pLst_Receive->GetCount() )
		return;
	int index = m_pLst_Receive->GetItemData(nSel, 0);

	m_pLst_Send->SetCurSel(-1);
}


void DlgFlowersList::OnEventLButtonList02(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	m_pLst_Receive->SetCurSel(-1);
	m_nCurSel = m_pLst_Send->GetCurSel();
	int nSel = m_pLst_Send->GetCurSel();
	if( nSel < 0 || nSel >= m_pLst_Send->GetCount() )
		return;
	int index = m_pLst_Send->GetItemData(nSel, 0);
}


void DlgFlowersList::OnEventRButtonList01(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	POINT ptPos = pObj->GetPos();
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - ptPos.x - p->X;
	int y = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;
	int nSel = m_pLst_Receive->GetCurSel();
	if( nSel < 0 || nSel >= m_pLst_Receive->GetCount() )
		return;

	CDlgFlowerRankOption* pMenu  = (CDlgFlowerRankOption*)GetGameUIMan()->GetDialog("Win_RoseRankOption");
	pMenu->m_bIsSend = true;
	x = GET_X_LPARAM(lParam) - p->X;
	y = GET_Y_LPARAM(lParam) - p->Y;
	
	// old : pMenu->SetPos(x, y);
	pMenu->SetPosEx(x, y);
	tm tmNow = GetGame()->GetServerLocalTime();
	if (checkFlowerTime(tmNow))
	{
		pMenu->Show(true);
	}

}

void DlgFlowersList::OnEventRButtonList02(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	POINT ptPos = pObj->GetPos();
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - ptPos.x - p->X;
	int y = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;
	int nSel = m_pLst_Send->GetCurSel();
	if( nSel < 0 || nSel >= m_pLst_Send->GetCount() )
		return;

	CDlgFlowerRankOption* pMenu  =  (CDlgFlowerRankOption*)GetGameUIMan()->GetDialog("Win_RoseRankOption");
	pMenu->m_bIsSend = false;
	x = GET_X_LPARAM(lParam) - p->X;
	y = GET_Y_LPARAM(lParam) - p->Y;
	
	// old : pMenu->SetPos(x, y);
	pMenu->SetPosEx(x, y);
	tm tmNow = GetGame()->GetServerLocalTime();
	if (checkFlowerTime(tmNow))
	{
		pMenu->Show(true);
	}

}