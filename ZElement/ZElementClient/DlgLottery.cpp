// Filename	: DlgLottery.cpp
// Creator	: Xiao Zhou
// Date		: 2006/10/18

#include "AFI.h"
#include "DlgLottery.h"
#include "EC_Game.h"
#include "EC_IvtrConsume.h"
#include "EC_HostPlayer.h"
#include "EC_GameSession.h"
#include "EC_GameUIMan.h"
#include "EC_Inventory.h"
#include "AUI\\AUICTranslate.h"
#include "A2DSprite.h"
#include "luastate.h"
#include "LuaScript.h"
#include "LuaUtil.h"
#include "ScriptValue.h"
#include <vector.h>
#include "LuaEvent.h"

#define new A_DEBUG_NEW

#define  LOTTERY_TBL_NAME    "Lottery_Desc"
const char szLotteryDescFile[] = "Interfaces\\script\\Lottery_Desc.lua";

AUI_BEGIN_COMMAND_MAP(CDlgLottery, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",		OnCommandCancel)
AUI_ON_COMMAND("getaward",		OnCommandGetAward)
AUI_ON_COMMAND("select",		OnCommandSelect)

AUI_END_COMMAND_MAP()

CDlgLottery::CDlgLottery()
{
}

CDlgLottery::~CDlgLottery()
{
}

void CDlgLottery::OnCommandCancel(const char* szCommand)
{
	if( GetData() != 9999 )
	{
		CECIvtrItem *pItem = GetHostPlayer()->GetPack()->GetItem(GetData());
		if( pItem )
			pItem->Freeze(false);
		SetData(9999);
	}
	Show(false);
}

void CDlgLottery::OnCommandSelect(const char* szCommand)
{
	CECIvtrItem *pItem = GetHostPlayer()->GetPack()->GetItem(GetData());
	if( pItem )
	{
		pItem->Freeze(false);
		GetHostPlayer()->UseItemInPack(IVTRTYPE_PACK, GetData());
		pItem->Freeze(true);
	}
}

void CDlgLottery::OnCommandGetAward(const char* szCommand)
{
	CECIvtrItem *pItem = GetHostPlayer()->GetPack()->GetItem(GetData());
	if( pItem )
		pItem->Freeze(false);
	GetGameSession()->c2s_CmdLotteryCashing(GetData());
}

bool CDlgLottery::OnInitDialog()
{
	m_pBtn_Select = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Select");
	m_pBtn_Select1 = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Select1");
	m_pBtn_GetAward = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_GetAward");
	m_pTxt_Award = (PAUILABEL)GetDlgItem("Txt_Award");
	m_pTxt_AwardMoney = (PAUILABEL)GetDlgItem("Txt_AwardMoney");
	m_pTxt_AwardLevel = (PAUILABEL)GetDlgItem("Txt_AwardLevel");
	m_pTxt_Title = (PAUILABEL)GetDlgItem("Title");
	m_pTxt_Title->SetAlign(AUIFRAME_ALIGN_CENTER);
	m_pImg_Item = (PAUIIMAGEPICTURE)GetDlgItem("Img_Item");
	for(int i = 0; i < 3; i++)
	{
		char szName[10];
		sprintf(szName, "Img_Word%d", i + 1);
		m_pImg_Word[i] = (PAUIIMAGEPICTURE)GetDlgItem(szName);
	}
	SetData(9999);
	m_pState = g_LuaStateMan.GetAIState();
	if (!m_pState)
	{
		a_LogOutput(1, "CDlgLottery::InitDialog, null lua state\n");
		return false;
	}

	
// 	//	load lottery description script
// 	CLuaScript tmpScript(szLotteryDescFile, m_pState);
// 	CLuaScript * pScript = g_LuaFileMan.Register(tmpScript);
// 	if (pScript->Execute() == CLuaScript::EXECUTE_FAIL)
// 	{
// 		assert(0);
// 		g_LuaFileMan.UnRegister(pScript->GetName(), pScript->GetLuaState());
// 		AString msg;
// 		msg.Format("preload %s fail\n", szLotteryDescFile);
// 		LUA_DEBUG_INFO(msg);
// 		return false;
// 	}
// 
// 	return true;

	CLuaScript * pScript = m_pState->RegisterFile(szLotteryDescFile);
	if (!pScript)
	{
		return false;
	}

	return true;
}

bool CDlgLottery::Release()
{
// 	if (m_pState && m_pState->GetVM())
// 		LuaBind::ReleaseTbl(m_pState->GetVM(), LOTTERY_TBL_NAME);
// 	g_LuaFileMan.UnRegister(szLotteryDescFile, m_pState);
	
	if (m_pState)
	{
		m_pState->ReleaseTbl(LOTTERY_TBL_NAME);
		m_pState->UnRegister(szLotteryDescFile);
	}

	return CDlgBase::Release();
}

void CDlgLottery::OnShowDialog()
{
	CECIvtrItem *pItem = GetHostPlayer()->GetPack()->GetItem(GetData());
	if( pItem )
		pItem->Freeze(true);
	LuaEvent::FirstLottery();
}

void CDlgLottery::OnTick()
{
	if( GetData() == 9999 )
	{
		m_pTxt_Award->Show(true);
		m_pTxt_AwardMoney->Show(m_pTxt_AwardMoney->GetData() == 1);
		m_pImg_Item->Show(m_pImg_Item->GetData() == 1);
		m_pBtn_Select->Show(false);
		m_pBtn_Select1->SetCommand("");
		m_pBtn_GetAward->Show(false);
		m_pTxt_AwardLevel->Show(false);
		for(int i = 0; i < 3; i++)
			m_pImg_Word[i]->Show(false);
	}
	else
	{
		for(int i = 0; i < 3; i++)
			m_pImg_Word[i]->Show(true);
		m_pTxt_Award->Show(false);
		m_pTxt_AwardMoney->Show(false);
		m_pImg_Item->Show(false);
		CECIvtrItem *pItem = GetHostPlayer()->GetPack()->GetItem(GetData());
		if( pItem && pItem->GetClassID() == CECIvtrItem::ICID_LOTTERY )
		{
			float fScale = GetGameUIMan()->GetWindowScale();
			CECIvtrLottery *pLottery = (CECIvtrLottery*)pItem;
			m_pTxt_Title->SetText(pLottery->GetDBEssence()->name);
			int range = 1;
			int i;
			for(i = 1; i < 32; i++)
				if( pLottery->GetDBEssence()->candidates[i].desc[0] == 0 )
				{
					range = i;
					break;
				}
			for(i = 0; i < pLottery->GetDBEssence()->dice_count; i++)
			{
				if( pLottery->GetEssence().dice_data[i] == 0 )
					break;

				AString strFile;
				af_GetFileTitle(GetGame()->GetDataPath(pLottery->GetDBEssence()->candidates[pLottery->GetEssence().dice_data[i] - 1].icon), strFile);
				strFile.MakeLower();

				m_pImg_Word[i]->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_LOTTERY],
					GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_LOTTERY][strFile]);
			}
			if( i < pLottery->GetDBEssence()->dice_count)
			{
				m_pBtn_Select->Show(true);
				m_pBtn_Select1->SetCommand("select");
				m_pBtn_GetAward->Show(false);
				m_pTxt_AwardLevel->Show(false);
			}
			else
			{
				m_pBtn_Select->Show(false);
				m_pBtn_Select1->SetCommand("");
				m_pBtn_GetAward->Show(true);
				m_pTxt_AwardLevel->Show(true);
				ACString strDesc = _AL("");
				CECIvtrItem *pItem = GetHostPlayer()->GetPack()->GetItem(GetData());
				if( pItem && pItem->GetClassID() == CECIvtrItem::ICID_LOTTERY )
				{
					CECIvtrLottery *pLottery = (CECIvtrLottery*)pItem;
					abase::vector<CScriptValue> args, dicedatas;
					abase::vector<CScriptValue> results;
					CScriptValue dicedata;

					for(int i = 0; i < pLottery->GetDBEssence()->dice_count; i++)
						dicedatas.push_back(CScriptValue((double)pLottery->GetEssence().dice_data[i]));
					dicedata.SetArray(dicedatas);
					args.push_back((double)pLottery->GetTemplateID());
					args.push_back(dicedata);

					m_pState->Call(LOTTERY_TBL_NAME, "GetLotteryDesc", args, results);

// 					if (results.size() == 1 && results[0].m_Type == CScriptValue::SVT_STRING)
// 						strDesc = results[0].GetAWString();
					if (results.size() == 1 && results[0].m_Type == CScriptValue::SVT_STRING)
					{
						results[0].RetrieveAWString(strDesc);
					}

				}
				m_pTxt_AwardLevel->SetText(strDesc);
			}
			for(; i < 3; i++)
			{
				AString strFile;
				af_GetFileTitle(GetGame()->GetDataPath(pLottery->GetDBEssence()->candidates[rand() % range].icon), strFile);
				strFile.MakeLower();
				m_pImg_Word[i]->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_LOTTERY],
					GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_LOTTERY][strFile]);
			}
		}
	}
}

void CDlgLottery::SetAward(int iLevel, int idItem, int iCount, int iMoney)
{
	SetData(9999);
	ACHAR szText[256];
	a_sprintf(szText, GetStringFromTable(852), iLevel);
	m_pTxt_Award->SetText(szText);
	if( iMoney > 0 )
	{
		CECIvtrItem::GetPriceText(szText, iMoney);
		m_pTxt_AwardMoney->SetText(szText);
		m_pTxt_AwardMoney->SetData(1);
	}
	else
		m_pTxt_AwardMoney->SetData(0);
	m_pImg_Item->SetData(0);

	if( idItem > 0 && iCount > 0 )
	{
		CECIvtrItem *pItem = CECIvtrItem::CreateItem(idItem, 0, iCount);
		if( pItem )
		{
			AString strFile;
			af_GetFileTitle(pItem->GetIconFile(), strFile);
			strFile.MakeLower();
			m_pImg_Item->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
				GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
			pItem->GetDetailDataFromLocal();
			const wchar_t* szDesc = pItem->GetDesc();
			AUICTranslate trans;
			if( szDesc )
				m_pImg_Item->SetHint(trans.Translate(szDesc));
			else
				m_pImg_Item->SetHint(_AL(""));
			delete pItem;
			a_sprintf(szText, _AL("%d"), iCount);
			m_pImg_Item->SetText(szText);
			m_pImg_Item->SetData(1);
		}
	}
}

#undef  LOTTERY_TBL_NAME
