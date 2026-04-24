// Filename	: CDlgKf2012Guess.cpp
// Creator	: Fu Chonggang
// Date		: 2012 跨服pk竞猜界面

#include "AIniFile.h"
#include "DlgKf2012Guess.h"
#include "EC_GameUIMan.h"
#include "EC_Game.h"
#include "elementdataman.h"
#include "ExpTypes.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_GPDataType.h"
#include "EC_GameSession.h"
#include "EC_SendC2SCmds.h"
#include "time.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIEditBox.h"

#define new A_DEBUG_NEW

template<> abase::vector<CDlgKf2012GuessBase*> CDlgTabContainer<CDlgKf2012GuessBase*>::ms_Pages;
template<> CDlgKf2012GuessBase* CDlgTabContainer<CDlgKf2012GuessBase*>::ms_LastShow = 0;

PK2012_GUESS_CONFIG* CDlgKf2012GuessBase::m_pConfig = NULL;

bool CDlgKf2012GuessBase::OnInitDialog()
{
	if (!m_pConfig)
	{
		DATA_TYPE DataType;
		unsigned int id = GetGame()->GetElementDataMan()->get_first_data_id(ID_SPACE_CONFIG , DataType);
		while(id)
		{
			if (DataType == DT_PK2012_GUESS_CONFIG)
			{
				m_pConfig = (PK2012_GUESS_CONFIG*)GetGame()->GetElementDataMan()->get_data_ptr(id , ID_SPACE_CONFIG , DataType);
				break;
			}
			id = GetGame()->GetElementDataMan()->get_next_data_id(ID_SPACE_CONFIG , DataType);
		}
	}
	AddTabDialog(this);

	return true;
}

void CDlgKf2012GuessBase::OnShowDialog()
{
	static DWORD iLastGet = 0;
	if (GetTickCount()>iLastGet)
	{
		c2s_SendCmdGetPlayerBetData();
		iLastGet = GetTickCount() + 10000;
	}
}


AUI_BEGIN_COMMAND_MAP(CDlgKf2012GuessChampion, CDlgBase)

AUI_ON_COMMAND("Rdo_2",			OnCommandRdoType)
AUI_ON_COMMAND("Btn_Buy*",		OnCommandGuess)
AUI_ON_COMMAND("Btn_Get1",		OnCommandGetPackage)
AUI_ON_COMMAND("Btn_Get",		OnCommandGetAward)
AUI_ON_COMMAND("Btn_Link",		OnCommandLink)

AUI_END_COMMAND_MAP()

const int	CDlgKf2012GuessChampion::ms_iMaxBetNum = 10000;

void CDlgKf2012GuessChampion::OnCommandRdoType(const char * szCommand)
{
	CDlgKf2012GuessBase* pDlg = (CDlgKf2012GuessBase*)GetGameUIMan()->GetDialog("Win_Kf2012Bet2");
	ChangeDialog(pDlg);
	pDlg->CheckRadioButton(1, 2);
}

void CDlgKf2012GuessChampion::OnCommandGuess(const char * szCommand)
{
	int index = atoi(szCommand+strlen("Btn_Buy"))-1;
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Kf2012BetInput1");
	AString strName;
	strName.Format("Lab_Servergroup%d", index+1);
	PAUILABEL pLabGroup = (PAUILABEL)GetDlgItem(strName);
	strName.Format("Lab_Server%d", index+1);
	PAUILABEL pLabServer = (PAUILABEL)GetDlgItem(strName);
	PAUILABEL pLab = (PAUILABEL)pDlg->GetDlgItem("Txt_Server");
	pLab->SetText(ACString(pLabGroup->GetText())+ACString(_AL(" "))+ACString(pLabServer->GetText()));
	pDlg->SetData(index, "guess_index");
	pDlg->GetDlgItem("DEFAULT_Txt_No")->SetData(GetDlgItem(szCommand)->GetData("max_bet_num"), "max_bet_num");
	pDlg->Show(true, true);
}

void CDlgKf2012GuessChampion::OnCommandGetPackage(const char * szCommand)
{
	c2s_SendCmdPK1stGuessReward();
}

void CDlgKf2012GuessChampion::OnCommandGetAward(const char * szCommand)
{
	c2s_SendCmdPK1stGuessResultReward();
}

void CDlgKf2012GuessChampion::OnCommandLink(const char * szCommand)
{
	AString strText = "";
	AIniFile iniFile;
	if(iniFile.Open("configs\\url.ini"))
	{
		strText = iniFile.GetValueAsString("ingame", "kfbet", "");
		iniFile.Close();
	}
	if( strText != "" )
		ShellExecuteA(NULL,"open", strText, NULL, NULL, SW_SHOWNORMAL);
}

int CDlgKf2012GuessChampion::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="set_award")
	{
		S2C::pk_bet_data *pData = (S2C::pk_bet_data*)wParam;
		int i;
		AString strName;
		PAUILABEL pLab;
		ACString strText;
		int iNumFirstBet = 0;
		time_t serverTime = GetGame()->GetServerGMTTime();
		int tNow = serverTime - GetGame()->GetTimeZoneBias() * 60;
		bool bGuessing = tNow>=m_pConfig->guess_start_time && tNow<=m_pConfig->guess_end_time;
		bool bAwarding = tNow>=m_pConfig->accept_award_start_time && tNow<=m_pConfig->accept_award_end_time;
		bool bGuessed = false;
		for (i=0; i<8; ++i)
		{
			strName.Format("Lab_Chip%d", i+1);
			pLab = (PAUILABEL)GetDlgItem(strName);
			strText.Format(GetGameUIMan()->GetStringFromTable(15007), pData->pk_bets[i].bet_1st_num);
			pLab->SetText(strText);
			strName.Format("Btn_Buy%d", i+1);
			GetDlgItem(strName)->Enable(bGuessing && pData->pk_bets[i].bet_1st_num<ms_iMaxBetNum);
			GetDlgItem(strName)->SetData(ms_iMaxBetNum-pData->pk_bets[i].bet_1st_num, "max_bet_num");
			strName.Format("Img_Champion%d", i+1);
			GetDlgItem(strName)->Show(bAwarding && i==m_pConfig->first_id);
			if (bAwarding && m_pConfig->first_id==i)
			{
				iNumFirstBet = pData->pk_bets[i].bet_1st_num;
			}
			if (pData->pk_bets[i].bet_1st_num>0)
			{
				bGuessed = true;
			}
		}
		GetDlgItem("Btn_Get1")->Enable(bGuessed && !pData->first_bet_reward_deliveried);
		GetDlgItem("Btn_Get")->Enable(bGuessed && bAwarding && !pData->first_bet_reward_result_deliveried);
		pLab = (PAUILABEL)GetDlgItem("Txt_A01");
		pLab->SetText(bAwarding ? (GetGameUIMan()->GetStringFromTable(iNumFirstBet>0 ? 15008 : 15009)):_AL("----"));
		pLab = (PAUILABEL)GetDlgItem("Txt_A02");
		strText.Format(_AL("%d"), iNumFirstBet);
		pLab->SetText(bAwarding ? strText:_AL("----"));
		pLab = (PAUILABEL)GetDlgItem("Txt_A03");
		strText.Format(_AL("1:%.1f"), m_pConfig->award_back_ratio);
		pLab->SetText(bAwarding ? strText:_AL("----"));
		pLab = (PAUILABEL)GetDlgItem("Txt_A04");
		strText.Format(_AL("%d"), int(iNumFirstBet*10*m_pConfig->award_back_ratio + 0.5f));
		pLab->SetText(bAwarding ? strText:_AL("----"));
	}

	return 0;
}

bool CDlgKf2012GuessChampion::OnInitDialog()
{
	CDlgKf2012GuessBase::OnInitDialog();
	if (!m_pConfig)
	{
		return true;
	}
	ACString strText;

	tm t1 = *gmtime((time_t*)&m_pConfig->guess_start_time);
	tm t2 = *gmtime((time_t*)&m_pConfig->guess_end_time);
	PAUILABEL pLab = (PAUILABEL)GetDlgItem("Lab_05");	// 竞猜起止时间
	strText.Format(_AL("%d.%d.%d-%d.%d.%d"), 1900+t1.tm_year, t1.tm_mon+1, t1.tm_mday, 1900+t2.tm_year, t2.tm_mon+1, t2.tm_mday);
	pLab->SetText(strText);
	
	t1 = *gmtime((time_t*)&m_pConfig->accept_award_start_time);
	t2 = *gmtime((time_t*)&m_pConfig->accept_award_end_time);
	pLab = (PAUILABEL)GetDlgItem("Lab_07");				// 竞猜领奖时间
	strText.Format(_AL("%d.%d.%d-%d.%d.%d"), 1900+t1.tm_year, t1.tm_mon+1, t1.tm_mday, 1900+t2.tm_year, t2.tm_mon+1, t2.tm_mday);
	pLab->SetText(strText);

	PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)GetDlgItem("Img_Item");	// 竞猜礼包
	GetGameUIMan()->SetIvtrInfo(pImg, m_pConfig->champion_guess_award);
	pImg = (PAUIIMAGEPICTURE)GetDlgItem("Img_ItemNum");	// 竞猜奖励
	GetGameUIMan()->SetIvtrInfo(pImg, m_pConfig->champion_guess_item);
	CheckRadioButton(1, 1);

	return true;
}


AUI_BEGIN_COMMAND_MAP(CDlgKf2012GuessTop3, CDlgBase)

AUI_ON_COMMAND("Rdo_1",				OnCommandRdoType)
AUI_ON_COMMAND("Btn_Buy*",			OnCommandGuess)
AUI_ON_COMMAND("Btn_Get1",			OnCommandGetBaseAward)
AUI_ON_COMMAND("Btn_GetAward*",		OnCommandGetAward)
AUI_ON_COMMAND("Btn_Link",			OnCommandLink)

AUI_END_COMMAND_MAP()

void CDlgKf2012GuessTop3::OnCommandRdoType(const char * szCommand)
{
	CDlgKf2012GuessBase* pDlg = (CDlgKf2012GuessBase*)GetGameUIMan()->GetDialog("Win_Kf2012Bet1");
	ChangeDialog(pDlg);
	pDlg->CheckRadioButton(1, 1);
}

void CDlgKf2012GuessTop3::OnCommandGuess(const char * szCommand)
{
	int index = atoi(szCommand+strlen("Btn_Buy"))-1;

	PAUISTILLIMAGEBUTTON pButton = (PAUISTILLIMAGEBUTTON)GetDlgItem(szCommand);
	if (ACString(pButton->GetText())==ACString(GetGameUIMan()->GetStringFromTable(15011)))
	{
		PAUIDIALOG pDlg;
		GetGameUIMan()->MessageBox("Game_GuessTop3Cancel", GetGameUIMan()->GetStringFromTable(15012), MB_YESNO, A3DCOLORRGBA(255,255,255,160), &pDlg);
		pDlg->SetData(index, "area");
		return;
	}
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Kf2012BetInput2");
	ACString strText;
	PAUILABEL pLab = (PAUILABEL)pDlg->GetDlgItem("Txt_Server");
	pLab->SetText(strText);	// todo:
	pLab = (PAUILABEL)pDlg->GetDlgItem("Txt_Item");
	CECIvtrItem *pItem = CECIvtrItem::CreateItem(m_pConfig->guess_item, 0, 1);
	if (pItem)
	{
		pLab->SetText(pItem->GetName());
		delete pItem;
	}
	pLab = (PAUILABEL)pDlg->GetDlgItem("Txt_ItemNum");
	strText.Format(_AL("%d"), m_pConfig->guess_item_num);
	pLab->SetText(strText);	
	pLab->SetData(m_pConfig->guess_item, "item_id");
	pLab->SetDataPtr((void*)m_pConfig->guess_item_num, "item_num");

	pDlg->SetData(index, "guess_index");
	pDlg->Show(true, true);

	AString strName;
	strName.Format("Lab_Servergroup%d", index+1);
	PAUILABEL pLabGroup = (PAUILABEL)GetDlgItem(strName);
	strName.Format("Lab_Server%d", index+1);
	PAUILABEL pLabServer = (PAUILABEL)GetDlgItem(strName);
	pLab = (PAUILABEL)pDlg->GetDlgItem("Txt_Server");
	pLab->SetText(ACString(pLabGroup->GetText())+ACString(_AL(" "))+ACString(pLabServer->GetText()));
}

void CDlgKf2012GuessTop3::OnCommandGetBaseAward(const char * szCommand)
{
	c2s_SendCmdPKTop3GuessReward();
}

void CDlgKf2012GuessTop3::OnCommandGetAward(const char * szCommand)
{
	int iType = atoi(szCommand+strlen("Btn_GetAward"))-1;
	c2s_SendCmdPKTop3GuessResultReward(iType);
}

void CDlgKf2012GuessTop3::OnCommandLink(const char * szCommand)
{
	AString strText = "";
	AIniFile iniFile;
	if(iniFile.Open("configs\\url.ini"))
	{
		strText = iniFile.GetValueAsString("ingame", "kfbet", "");
		iniFile.Close();
	}
	if( strText != "" )
		ShellExecuteA(NULL,"open", strText, NULL, NULL, SW_SHOWNORMAL);
}

int CDlgKf2012GuessTop3::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="set_award")
	{
		S2C::pk_bet_data *pData = (S2C::pk_bet_data*)wParam;
		
		int i;
		AString strName;
		ACString strText;
		time_t serverTime = GetGame()->GetServerGMTTime();
		int tNow = serverTime - GetGame()->GetTimeZoneBias() * 60;
		bool bGuessing = tNow>=m_pConfig->guess_start_time && tNow<=m_pConfig->guess_end_time;
		bool bAwarding = tNow>=m_pConfig->accept_award_start_time && tNow<=m_pConfig->accept_award_end_time;
		int iNumBetRight = 0;
		int iNumBet = 0;
		PAUISTILLIMAGEBUTTON pButton;
		for (i=0; i<8; ++i)
		{
			strName.Format("Btn_Buy%d", i+1);
			pButton = (PAUISTILLIMAGEBUTTON)GetDlgItem(strName);
			pButton->SetText(GetGameUIMan()->GetStringFromTable(pData->pk_bets[i].top3 ? 15011:15010));
			pButton->Enable(bGuessing);
			strText.Format(GetGameUIMan()->GetStringFromTable(15007), pData->pk_bets[i].bet_1st_num);
			strName.Format("Img_Champion%d", i+1);
			GetDlgItem(strName)->Show(bAwarding && (i==m_pConfig->first_id || i==m_pConfig->second_id || i==m_pConfig->third_id));
			strName.Format("Img_Selected%d", i+1);
			GetDlgItem(strName)->Show(pData->pk_bets[i].top3);

			if (pData->pk_bets[i].top3 && (m_pConfig->first_id==i || m_pConfig->second_id==i || m_pConfig->third_id==i))
				iNumBetRight++;
			if (pData->pk_bets[i].top3)
				iNumBet++;
		}
		
		if (bAwarding)
		{
			strName.Format("Img_Champion%d", m_pConfig->first_id+1);
			((PAUIIMAGEPICTURE)GetDlgItem(strName))->FixFrame(0);
			strName.Format("Img_Champion%d", m_pConfig->second_id+1);
			((PAUIIMAGEPICTURE)GetDlgItem(strName))->FixFrame(1);
			strName.Format("Img_Champion%d", m_pConfig->third_id+1);
			((PAUIIMAGEPICTURE)GetDlgItem(strName))->FixFrame(2);
		}

		if (bGuessing && iNumBet>=3)
		{
			for (i=0; i<8; ++i)
			{
				strName.Format("Btn_Buy%d", i+1);
				pButton = (PAUISTILLIMAGEBUTTON)GetDlgItem(strName);
				if (!pData->pk_bets[i].top3)
				{
					pButton->Enable(false);
				}
			}
		}

		GetDlgItem("Btn_Get1")->Enable(pData->top3_bet&&!pData->top3_bet_reward_deliveried);
		GetDlgItem("Btn_GetAward1")->Enable(bAwarding && iNumBetRight>=3 && !pData->top3_bet_reward_result_deliveried[0]);
		GetDlgItem("Btn_GetAward2")->Enable(bAwarding && iNumBetRight>=2 && !pData->top3_bet_reward_result_deliveried[1]);
		GetDlgItem("Btn_GetAward3")->Enable(bAwarding && iNumBetRight>=1 &&!pData->top3_bet_reward_result_deliveried[2]);
	}
	
	return 0;
}

bool CDlgKf2012GuessTop3::OnInitDialog()
{
	CDlgKf2012GuessBase::OnInitDialog();
	if (!m_pConfig)
	{
		return true;
	}
	ACString strText;
	tm t1 = *gmtime((time_t*)&m_pConfig->guess_start_time);
	tm t2 = *gmtime((time_t*)&m_pConfig->guess_end_time);
	PAUILABEL pLab = (PAUILABEL)GetDlgItem("Lab_05");	// 竞猜起止时间
	strText.Format(_AL("%d.%d.%d-%d.%d.%d"), 1900+t1.tm_year, t1.tm_mon+1, t1.tm_mday, 1900+t2.tm_year, t2.tm_mon+1, t2.tm_mday);
	pLab->SetText(strText);
	
	t1 = *gmtime((time_t*)&m_pConfig->accept_award_start_time);
	t2 = *gmtime((time_t*)&m_pConfig->accept_award_end_time);
	pLab = (PAUILABEL)GetDlgItem("Lab_07");				// 竞猜领奖时间
	strText.Format(_AL("%d.%d.%d-%d.%d.%d"), 1900+t1.tm_year, t1.tm_mon+1, t1.tm_mday, 1900+t2.tm_year, t2.tm_mon+1, t2.tm_mday);
	pLab->SetText(strText);
	
	PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)GetDlgItem("Img_Item");	// 竞猜礼包
	GetGameUIMan()->SetIvtrInfo(pImg, m_pConfig->guess_award_item);
	pImg = (PAUIIMAGEPICTURE)GetDlgItem("Img_ItemNum");
	GetGameUIMan()->SetIvtrInfo(pImg, m_pConfig->guess_award_item3);
	pImg = (PAUIIMAGEPICTURE)GetDlgItem("Img_Award2");
	GetGameUIMan()->SetIvtrInfo(pImg, m_pConfig->guess_award_item2);
	pImg = (PAUIIMAGEPICTURE)GetDlgItem("Img_Award3");
	GetGameUIMan()->SetIvtrInfo(pImg, m_pConfig->guess_award_item1);
	CheckRadioButton(1, 2);
	
	return true;
}


AUI_BEGIN_COMMAND_MAP(CDlgKf2012GuessChampionInput, CDlgBase)

AUI_ON_COMMAND("Btn_Add",			OnCommandAdd)
AUI_ON_COMMAND("Btn_Minus",			OnCommandSub)
AUI_ON_COMMAND("confirm",			OnCommandGuess)

AUI_END_COMMAND_MAP()

void CDlgKf2012GuessChampionInput::OnCommandAdd(const char * szCommand)
{
	PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("DEFAULT_Txt_No");
	int iNum = a_atoi(pEdt->GetText());
	int iNumTotal = GetHostPlayer()->GetPack()->GetItemTotalNum(m_pConfig->champion_guess_item);
	if (iNum<iNumTotal/m_pConfig->champion_guess_itemNum)
	{
		iNum++;
		ACString strText;
		strText.Format(_AL("%d"), iNum);
		pEdt->SetText(strText);
	}
}

void CDlgKf2012GuessChampionInput::OnCommandSub(const char * szCommand)
{
	PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("DEFAULT_Txt_No");
	int iNum = a_atoi(pEdt->GetText());
	if (iNum>0)
	{
		iNum--;
		ACString strText;
		strText.Format(_AL("%d"), iNum);
		pEdt->SetText(strText);
	}
}

void CDlgKf2012GuessChampionInput::OnCommandGuess(const char * szCommand)
{
	PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("DEFAULT_Txt_No");
	int iNum = a_atoi(pEdt->GetText())*m_pConfig->champion_guess_itemNum;
	if (GetHostPlayer()->GetPack()->GetItemTotalNum(m_pConfig->champion_guess_item)<iNum)
	{
		GetGameUIMan()->MessageBox(15013);
		return;
	}
	abase::vector<C2S::pk_1st_guess::item_info> vecItems;
	int i;
	for (i=0; i<GetHostPlayer()->GetPack()->GetSize(); ++i)
	{
		CECIvtrItem *pItem = GetHostPlayer()->GetPack()->GetItem(i);
		if (pItem && pItem->GetTemplateID()==m_pConfig->champion_guess_item)
		{
			C2S::pk_1st_guess::item_info item;
			item.index = i;
			item.cnt = iNum<pItem->GetCount() ? iNum : pItem->GetCount();
			iNum -= pItem->GetCount();
			vecItems.push_back(item);
		}
		if (iNum<=0)
			break;
	}

	using namespace C2S;
	
	int iSize = sizeof(cmd_header) + 2*sizeof(int) + vecItems.size()*sizeof(pk_1st_guess::item_info);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}
	
	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::PK_1ST_GUESS;
	pk_1st_guess* pCmd = (pk_1st_guess*)(pBuf + sizeof(cmd_header));
	pCmd->area = GetData("guess_index");
	pCmd->cnt	=	vecItems.size();
	for (i=0; i<(int)vecItems.size(); ++i)
	{
		pCmd->items[i].cnt = vecItems[i].cnt;
		pCmd->items[i].index = vecItems[i].index;
	}
	
	GetGame()->GetGameSession()->SendGameData( pBuf, iSize );
	
	delete []pBuf;	
	
	Show(false);
}

bool CDlgKf2012GuessChampionInput::OnInitDialog()
{
	DATA_TYPE DataType;
	unsigned int id = GetGame()->GetElementDataMan()->get_first_data_id(ID_SPACE_CONFIG , DataType);
	while(id)
	{
		if (DataType == DT_PK2012_GUESS_CONFIG)
		{
			m_pConfig = (PK2012_GUESS_CONFIG*)GetGame()->GetElementDataMan()->get_data_ptr(id , ID_SPACE_CONFIG , DataType);
			break;
		}
		id = GetGame()->GetElementDataMan()->get_next_data_id(ID_SPACE_CONFIG , DataType);
	}
	return true;
}

void CDlgKf2012GuessChampionInput::OnTick()
{
	static int lastAction = 0;
	if( ((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Add"))->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK )
	{
		lastAction += GetGame()->GetTickTime();
		if (lastAction>150)
		{
			OnCommandAdd("");
			lastAction = 0;
		}
	}
	else if( ((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Minus"))->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK )
	{
		lastAction += GetGame()->GetTickTime();
		if (lastAction>150)
		{
			OnCommandSub("");
			lastAction = 0;
		}
	}
	
	PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("DEFAULT_Txt_No");
	int iNum = a_atoi(pEdt->GetText());
	ACString strText;
	if (iNum>(int)pEdt->GetData("max_bet_num"))
	{
		iNum = pEdt->GetData("max_bet_num");
		strText.Format(_AL("%d"), iNum);
		pEdt->SetText(strText);
	}
	PAUILABEL pLab = (PAUILABEL)GetDlgItem("Txt_ItemNum");
	strText.Format(_AL("%d"), iNum*m_pConfig->guess_item_num);
	pLab->SetText(strText);
}


AUI_BEGIN_COMMAND_MAP(CDlgKf2012GuessTop3Input, CDlgBase)

AUI_ON_COMMAND("confirm",		OnCommandGuess)

AUI_END_COMMAND_MAP()

void CDlgKf2012GuessTop3Input::OnCommandGuess(const char * szCommand)
{
	PAUILABEL pLab = (PAUILABEL)GetDlgItem("Txt_ItemNum");
	int iItemId = pLab->GetData("item_id");
	int iNum = (int)pLab->GetDataPtr("item_num");

	abase::vector<C2S::pk_top3_guess::item_info> vecItems;
	int i;
	for (i=0; i<GetHostPlayer()->GetPack()->GetSize(); ++i)
	{
		CECIvtrItem *pItem = GetHostPlayer()->GetPack()->GetItem(i);
		if (pItem && pItem->GetTemplateID()==iItemId)
		{
			C2S::pk_top3_guess::item_info item;
			item.index = i;
			item.cnt = iNum<pItem->GetCount() ? iNum : pItem->GetCount();
			iNum -= pItem->GetCount();
			vecItems.push_back(item);
		}
		if (iNum<=0)
			break;
	}
	
	if (iNum>0)
	{
		GetGameUIMan()->MessageBox(15013);
		return;
	}

	using namespace C2S;

#pragma pack(1)

	int iSize = sizeof(cmd_header) + sizeof(bool) + 2*sizeof(int) + vecItems.size()*sizeof(pk_top3_guess::item_info);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}
	
	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::PK_TOP3_GUESS;
	pk_top3_guess* pCmd = (pk_top3_guess*)(pBuf + sizeof(cmd_header));
	pCmd->cancel= false;
	pCmd->area	=	GetData("guess_index");
	pCmd->cnt	=	vecItems.size();
	for (i=0; i<(int)vecItems.size(); ++i)
	{
		pCmd->items[i].cnt = vecItems[i].cnt;
		pCmd->items[i].index = vecItems[i].index;
	}
	
	GetGame()->GetGameSession()->SendGameData( pBuf, iSize );
	
	delete []pBuf;	
	
#pragma pack()

	Show(false);
}
