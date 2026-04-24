// Filename	: DlgAwardNew.h
// Creator	: Fuchonggang
// Date		: 2005/11/07

#include "AFI.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUICheckBox.h"
#include "AUI\\AUIProgress.h"

#include "DlgLotteryNew.h"
#include "DlgSkill.h"
#include "EC_Game.h"
#include "EC_IvtrConsume.h"
#include "EC_HostPlayer.h"
#include "EC_GameSession.h"
#include "EC_GameUIMan.h"
#include "EC_Inventory.h"
#include "EC_Configs.h"
#include "EC_GameUIMisc.h"
#include "EC_Resource.h"
#include "AUI\\AUICTranslate.h"
#include "A2DSprite.h"
#include "A3DGFXExMan.h"
#include "luastate.h"
#include "LuaScript.h"
#include "LuaUtil.h"
#include "ScriptValue.h"
#include <vector.h>
#include "LuaEvent.h"

#define new A_DEBUG_NEW

#define  LOTTERY_NEW_TBL_NAME    "Lottery_Desc"
const char szLotteryDescFile[] = "Interfaces\\script\\Lottery_Desc.lua";


const int	CDlgLotteryBase::m_siInterval	= 500;

void CDlgLotteryBase::SetItemCoverByIconNew(PAUIIMAGEPICTURE pItemImage, int index)
{
	if (m_pLottery2)
		SetItemCoverByIcon2(pItemImage, index);
	else if (m_pLottery3)
		SetItemCoverByIcon3(pItemImage, index);
}

void CDlgLotteryBase::SetItemCoverByIcon2(PAUIIMAGEPICTURE pItemImage, int index)
{
	if (!m_pLottery2)
	{
		return;
	}
	if(m_pLottery2->GetDBEssence()->candidates[index-1].icon == 0)
	{
		return;
	}
	AString strFile;
	af_GetFileTitle(GetGame()->GetDataPath(m_pLottery2->GetDBEssence()->candidates[index-1].icon), strFile);
	strFile.MakeLower();
	pItemImage->SetCover(GetGameUIMan()->GetIconSet(CECGameUIMan::ICONS_INVENTORY),
		GetGameUIMan()->GetIconId(CECGameUIMan::ICONS_INVENTORY, strFile));
	
	AWString itemHint = _AL("");
	itemHint = m_pLottery2->GetDBEssence()->candidates[index-1].desc;
	pItemImage->SetHint(itemHint);
}

void CDlgLotteryBase::SetItemCoverByIcon3(PAUIIMAGEPICTURE pItemImage, int index)
{
	if (!m_pLottery3)
	{
		return;
	}
	
	GetGameUIMan()->SetIvtrInfo(pItemImage, m_pLottery3->GetDBEssence()->candidate_items[index-1].item_id,
		m_pLottery3->GetDBEssence()->candidate_items[index-1].item_count);
}

bool CDlgLotteryBase::Release()
{
	if (m_pLottery2)
	{
		delete m_pLottery2;
		m_pLottery2 = NULL;
	}
	if (m_pLottery3)
	{
		delete m_pLottery3;
		m_pLottery3 = NULL;
	}
	
	return CDlgBase::Release();
}


AUI_BEGIN_COMMAND_MAP(CDlgLotteryNew, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",		OnCommandCancel)
AUI_ON_COMMAND("getaward",		OnCommandGetAward)
AUI_ON_COMMAND("startstop",		OnCommandStartStop)

AUI_END_COMMAND_MAP()

A2DSprite* CDlgLotteryNew::m_pA2DSpriteLotteryCur = NULL;

CDlgLotteryNew::CDlgLotteryNew()
{
}

CDlgLotteryNew::~CDlgLotteryNew()
{
}

int CDlgLotteryNew::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="set_award")
	{
		if(wParam>0&&wParam<=16)
		{
			m_iResult	= wParam;
			m_iGfxIndex = m_iResult;
			m_iLevel	= lParam;
			
			POINT ptPos = m_pImg_Word[m_iGfxIndex-1]->GetPos();
			ptPos.x += m_pImg_Word[m_iGfxIndex-1]->GetSize().cx / 2;
			ptPos.y += m_pImg_Word[m_iGfxIndex-1]->GetSize().cy / 2;
			m_pGfx->SetParentTM(a3d_Translate((float)ptPos.x, (float)ptPos.y, 0));
			m_pGfx->Start();
		}
	}
	return 1;
}

void CDlgLotteryNew::OnCommandCancel(const char* szCommand)
{
	if( GetData() != 9999 )
	{
		CECIvtrItem *pItem = GetHostPlayer()->GetPack()->GetItem(GetData());
		if( pItem )
			pItem->Freeze(false);
		SetData(9999);
	}
	
	m_pTxt_Award->Show(false);
	m_pImg_Congrat->Show(false);
	m_pTxt_AwardMoney->SetText(_AL(""));
	m_pTxt_AwardLevel->SetText(_AL(""));
	Show(false);
}

void CDlgLotteryNew::OnCommandStartStop(const char* szCommand)
{
	if (!m_bStart)
	{
		if (m_bNewLottery&&GetHostPlayer()->GetPack()->SearchEmpty()<0)
		{
			GetGameUIMan()->MessageBox(10347);
			m_pBtn_StartStop->Check(false);
			return;
		}
		if (m_bNewLottery)
		{
			m_iGfxIndex = 0;
			m_pGfx->Stop();
		}
		m_bStart = true;
		m_bFinish = false;
		
		m_pTxt_Award->Show(false);
		m_pImg_Congrat->Show(false);
		m_pTxt_AwardMoney->SetText(_AL(""));
		m_pTxt_AwardLevel->SetText(_AL(""));
	}
	else if (!m_pBtn_StartStop->IsChecked())
	{
		if (m_bNewLottery&&GetHostPlayer()->GetPack()->SearchEmpty()<0)
		{
			GetGameUIMan()->MessageBox(10347);
			InitLotteryNew();
			return;
		}
		m_pBtn_StartStop->Check(true);
		CECIvtrItem *pItem = GetHostPlayer()->GetPack()->GetItem(GetData());
		if( pItem )
		{
			pItem->Freeze(false);
			GetHostPlayer()->UseItemInPack(IVTRTYPE_PACK, GetData());
			m_pBtn_StartStop->Enable(false);
			pItem->Freeze(true);
		}
	}
}

void CDlgLotteryNew::OnCommandGetAward(const char* szCommand)
{
	CECIvtrItem *pItem = GetHostPlayer()->GetPack()->GetItem(GetData());
	if( pItem )
		pItem->Freeze(false);
	GetGameSession()->c2s_CmdLotteryCashing(GetData());
	m_pBtn_StartStop->Show(true);
	m_pBtn_GetAward->Show(false);
}

bool CDlgLotteryNew::OnInitDialog()
{
	m_pBtn_StartStop = (PAUICHECKBOX)GetDlgItem("Chk_Start");
	m_pBtn_GetAward = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_GetAward");
	m_pTxt_Award = (PAUILABEL)GetDlgItem("Txt_Award");
	m_pTxt_AwardMoney = (PAUILABEL)GetDlgItem("Txt_AwardMoney");
	m_pTxt_AwardMoney->SetAlign(AUIFRAME_ALIGN_CENTER);
	m_pTxt_AwardLevel = (PAUILABEL)GetDlgItem("Txt_AwardLevel");
	m_pTxt_Title = (PAUILABEL)GetDlgItem("Title");
	m_pTxt_Title->SetAlign(AUIFRAME_ALIGN_CENTER);
	m_pImg_Award = (PAUIIMAGEPICTURE)GetDlgItem("Img_Award");
	m_pImg_Congrat = (PAUIIMAGEPICTURE)GetDlgItem("Img_Congratulation");
	m_pImg_Congrat->Show(false);
	
	m_bStart	= false;
	m_bFinish	= false;
	m_iCurPos	= 1;	// 1 - 16
	m_iRange	= 0;
	m_iResult	= 0;
	m_pLottery2 = NULL;
	m_pLottery3 = NULL;
	m_iItemId	= 0;
	m_iInterval = 0;

	for(int i = 0; i < 16; i++)
	{
		AString strName;
		strName.Format("Img_Word%d", i + 1);
		m_pImg_Word[i] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
	}
	SetData(9999);
	m_pState = g_LuaStateMan.GetAIState();
	if (!m_pState)
	{
		a_LogOutput(1, "CDlgLottery::InitDialog, null lua state\n");
		return false;
	}

	m_pGfx = AfxGetGFXExMan()->LoadGfx(m_pA3DDevice, "界面\\方框36x36.gfx");
	if (!m_pGfx)
	{
		return AUI_ReportError(__LINE__, __FILE__);
	}

/** CDlgLottery中已经注册
	CLuaScript * pScript = m_pState->RegisterFile(szLotteryDescFile);
	if (!pScript)
	{
		return false;
	}
*/
	if( !m_pA2DSpriteLotteryCur )
	{
		m_pA2DSpriteLotteryCur = new A2DSprite;
		if( !m_pA2DSpriteLotteryCur ) return AUI_ReportError(__LINE__, __FILE__);

		bool bval = m_pA2DSpriteLotteryCur->Init(m_pA3DDevice, "Icon\\光效.tga", 0);
		if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
		m_pA2DSpriteLotteryCur->SetLinearFilter(true);
	}
	return true;
}

bool CDlgLotteryNew::Release()
{
// 	if (m_pState && m_pState->GetVM())
// 		LuaBind::ReleaseTbl(m_pState->GetVM(), LOTTERY_TBL_NAME);
// 	g_LuaFileMan.UnRegister(szLotteryDescFile, m_pState);
	A3DRELEASE(m_pA2DSpriteLotteryCur)
	
	if (m_pState)
	{
		m_pState->ReleaseTbl(LOTTERY_NEW_TBL_NAME);
		m_pState->UnRegister(szLotteryDescFile);
	}
	
	if(m_pGfx)
	{
		AfxGetGFXExMan()->CacheReleasedGfx(m_pGfx);
		m_pGfx = NULL;
	}

	return CDlgLotteryBase::Release();
}

void CDlgLotteryNew::OnShowDialog()
{
	CECIvtrItem *pItem = GetHostPlayer()->GetPack()->GetItem(GetData());
	if( pItem )
		pItem->Freeze(true);
	else
		return;
	
	m_bNewLottery = pItem->GetClassID()==CECIvtrItem::ICID_LOTTERY2 ||
		pItem->GetClassID()==CECIvtrItem::ICID_LOTTERY3;
	if (m_bNewLottery)
	{
		m_iGfxIndex = 0;
		m_pGfx->Stop();
		InitLotteryNew();
	}
	else
		InitLottery();

	LuaEvent::FirstLottery(); 
}

void CDlgLotteryNew::InitLottery()
{
	CECIvtrItem *pItem = GetHostPlayer()->GetPack()->GetItem(GetData());
	m_iItemId = pItem->GetTemplateID();

	m_bBeginSlow= false;
	m_bStart	= false;
	m_bFinish	= false;
	m_iRange	= 16;
	m_iResult	= 1;
	m_dwTime	= 0;
	
	m_iCurPos	= 1;
	m_pImg_Congrat->Show(false);
	CECIvtrLottery *pLottery = (CECIvtrLottery*)pItem;
	m_pTxt_Title->SetText(pLottery->GetDBEssence()->name);
	
	for (int i=0; i<16; i++)
	{
		SetItemCoverByIcon( m_pImg_Word[i], 1, pLottery->GetDBEssence()->candidates[i].icon, i+1);
		m_pImg_Word[i]->SetCover(NULL, 0, 1);
	}
	
	if (pLottery->GetEssence().dice_data[0] == 0 )
	{
		m_pBtn_StartStop->Enable(true);
		m_pBtn_StartStop->Show(true);
		m_pBtn_StartStop->Check(false);
		m_pBtn_GetAward->Show(false);
		m_pTxt_Award->Show(false);
		m_pTxt_AwardMoney->SetText(_AL(""));
		m_pTxt_AwardLevel->SetText(_AL(""));
		
		m_pImg_Word[0]->SetCover(m_pA2DSpriteLotteryCur, 0, 1);
		SetItemCoverByIcon( m_pImg_Award, 1, pLottery->GetDBEssence()->candidates[0].icon, 1);
	}
	else
	{
		if (pLottery->GetDBEssence()->dice_count == 1)
		{
			m_iResult = pLottery->GetEssence().dice_data[0];
		}
		else if (pLottery->GetDBEssence()->dice_count == 2)
		{
			m_iResult = (pLottery->GetEssence().dice_data[0]-1)*4 + pLottery->GetEssence().dice_data[1];
		}
		else
			assert(0);
		m_iCurPos = m_iResult;
		m_bFinish  = true;
		m_pBtn_StartStop->Show(false);
		m_pBtn_GetAward->Show(true);
		m_pTxt_Award->Show(true);
		m_pTxt_AwardMoney->Show(true);
		m_pTxt_AwardLevel->Show(true);
		m_pImg_Word[m_iResult-1]->SetCover(m_pA2DSpriteLotteryCur, 0, 1);
		SetItemCoverByIcon( m_pImg_Award, 1, pLottery->GetDBEssence()->candidates[m_iResult-1].icon, m_iResult);
		
		abase::vector<CScriptValue> args, dicedatas;
		abase::vector<CScriptValue> results;
		CScriptValue dicedata;
		ACString strDesc = _AL("");
		
		for(int i = 0; i < pLottery->GetDBEssence()->dice_count; i++)
			dicedatas.push_back(CScriptValue((double)pLottery->GetEssence().dice_data[i]));
		dicedata.SetArray(dicedatas);
		args.push_back((double)pLottery->GetTemplateID());
		args.push_back(dicedata);
		
		m_pState->Call(LOTTERY_NEW_TBL_NAME, "GetLotteryDesc", args, results);
		if (results.size() == 1 && results[0].m_Type == CScriptValue::SVT_STRING)
		{
			results[0].RetrieveAWString(strDesc);
		}
		m_pTxt_AwardLevel->SetText(strDesc);
		strDesc = pLottery->GetDBEssence()->candidates[m_iResult-1].desc;
		m_pTxt_AwardMoney->SetText(strDesc);
	}		
}

void CDlgLotteryNew::InitLotteryNew(bool bFirst)
{
	CECIvtrItem *pItem = GetHostPlayer()->GetPack()->GetItem(GetData());
	m_iItemId = pItem->GetTemplateID();

	m_bBeginSlow= false;
	m_bStart	= false;
	m_bFinish	= false;
	m_iRange	= 16;
	m_dwTime	= 0;
	if (m_pLottery2)
	{
		delete m_pLottery2;
		m_pLottery2 = NULL;
	}
	if (m_pLottery3)
	{
		delete m_pLottery3;
		m_pLottery3 = NULL;
	}
	if (pItem->GetClassID()==CECIvtrItem::ICID_LOTTERY2)
	{
		m_pLottery2 = (CECIvtrLottery2*)pItem->Clone();
		m_pTxt_Title->SetText(m_pLottery2->GetDBEssence()->name);
	}
	else
	{
		m_pLottery3 = (CECIvtrLottery3*)pItem->Clone();
		m_pTxt_Title->SetText(m_pLottery3->GetDBEssence()->name);
	}
	
	if (bFirst)
	{
		m_iCurPos	= 1;
		for (int i=0; i<16; i++)
		{
			SetItemCoverByIconNew(m_pImg_Word[i], i+1);
			m_pImg_Word[i]->SetCover(NULL, 0, 1);
		}
		
		m_pTxt_Award->Show(false);
		m_pTxt_AwardMoney->SetText(_AL(""));
		m_pTxt_AwardLevel->SetText(_AL(""));
		m_pImg_Word[0]->SetCover(m_pA2DSpriteLotteryCur, 0, 1);
		SetItemCoverByIconNew( m_pImg_Award, 1);
	}
	
	m_pBtn_StartStop->Enable(true);
	m_pBtn_StartStop->Show(true);
	m_pBtn_StartStop->Check(false);
	m_pBtn_GetAward->Show(false);
	m_iResult	= 0;
}

void CDlgLotteryNew::OnTick()
{
	if( GetData() == 9999 || !m_bNewLottery&&m_bFinish )
	{
		return;
	}
	CECIvtrItem *pItem = GetHostPlayer()->GetPack()->GetItem(GetData());

	if (m_bNewLottery && m_bFinish && m_iInterval>0)
	{
		m_iInterval -= GetGame()->GetTickTime();
		if (m_iInterval<=0)
		{
			if (pItem&&pItem->GetTemplateID()==m_iItemId)
			{
				InitLotteryNew(false);
			}
			else
			{
				int newIndex = GetHostPlayer()->GetPack()->FindItem(m_iItemId);
				if (newIndex>=0)
				{
					SetData(newIndex);
					pItem = GetHostPlayer()->GetPack()->GetItem(GetData());
					InitLotteryNew(false);
				}
			}
		}
	}
	if (m_bNewLottery&&m_iGfxIndex!=0)
	{
		POINT ptPos = m_pImg_Word[m_iGfxIndex-1]->GetPos();
		ptPos.x += m_pImg_Word[m_iGfxIndex-1]->GetSize().cx / 2;
		ptPos.y += m_pImg_Word[m_iGfxIndex-1]->GetSize().cy / 2;
		m_pGfx->SetParentTM(a3d_Translate((float)ptPos.x, (float)ptPos.y, 0));
		m_pGfx->TickAnimation(GetGame()->GetTickTime());
	}
	if (!m_bStart)
	{
		return;
	}
	// 老彩票领奖之前这个彩票都存在，新彩票抽完直接不存在，所以用clone的物品
	if( pItem && pItem->GetClassID() == CECIvtrItem::ICID_LOTTERY)
		TickLottery(pItem);
	else if( m_bNewLottery)
	{
		m_dwTime += GetGame()->GetTickTime();
		if (m_dwTime>100)
		{
			m_dwTime = 0;
			TickLotteryNew();
		}
	}
}

bool CDlgLotteryNew::Render()
{
	bool bRet = CDlgBase::Render();
	if( m_pGfx&&m_bNewLottery&&m_iGfxIndex!=0 )
	{
		bool bval = m_pA3DEngine->FlushInternalSpriteBuffer();
		if( !bval ) return AUI_ReportError(__LINE__, 1, "AUIImagePicture::Render(), Failed to call m_pA3DEngine->FlushInternalSpriteBuffer()");
		
		GetGameUIMan()->GetA3DFTFontMan()->Flush();
		
		AfxGetGFXExMan()->RegisterGfx(m_pGfx);
		AfxGetGFXExMan()->Render2DGfx(false);
	}
	return bRet;
}

void CDlgLotteryNew::TickLottery(CECIvtrItem* pItem)
{
	CECIvtrLottery *pLottery = (CECIvtrLottery*)pItem;
	if( pLottery->GetEssence().dice_data[0] == 0 )
	{
		m_iCurPos++;
		if (m_iCurPos>CDLGLOTTERYITEM_MAX)
		{
			m_iCurPos = 1;
		}
		m_pImg_Word[m_iCurPos-1]->SetCover(m_pA2DSpriteLotteryCur, 0, 1);
		if (m_iCurPos>1)
		{
			m_pImg_Word[m_iCurPos-2]->SetCover(NULL, 0, 1);
		}
		else
		{
			m_pImg_Word[15]->SetCover(NULL, 0, 1);
		}
		SetItemCoverByIcon( m_pImg_Award, 1, pLottery->GetDBEssence()->candidates[m_iCurPos-1].icon, m_iCurPos);
	}
	else if (pLottery->GetEssence().dice_data[0] > 0)
	{
		if (pLottery->GetDBEssence()->dice_count == 1)
		{
			m_iResult = pLottery->GetEssence().dice_data[0];
		}
		else if (pLottery->GetDBEssence()->dice_count == 2)
		{
			m_iResult = (pLottery->GetEssence().dice_data[0]-1)*4 + pLottery->GetEssence().dice_data[1];
		}
		else
			assert(0);
		static float uiPlus = 1.0;
		static UINT uiSlow = 1;
		if ( m_bBeginSlow || (m_iResult+16-m_iCurPos)==7 || (m_iResult-m_iCurPos)==7 )
		{
			m_bBeginSlow = true;
			uiSlow--;
			if ( m_iCurPos == m_iResult )
			{
				m_pBtn_StartStop->Show(false);
				m_pBtn_GetAward->Show(true);
				m_pTxt_AwardLevel->Show(true);
				m_pTxt_AwardMoney->Show(true);
				m_pTxt_Award->Show(true);
				
				abase::vector<CScriptValue> args, dicedatas;
				abase::vector<CScriptValue> results;
				CScriptValue dicedata;
				ACString strDesc = _AL("");
				
				for(int i = 0; i < pLottery->GetDBEssence()->dice_count; i++)
					dicedatas.push_back(CScriptValue((double)pLottery->GetEssence().dice_data[i]));
				dicedata.SetArray(dicedatas);
				args.push_back((double)pLottery->GetTemplateID());
				args.push_back(dicedata);
				
				m_pState->Call( LOTTERY_NEW_TBL_NAME, "GetLotteryDesc", args, results);
				if (results.size() == 1 && results[0].m_Type == CScriptValue::SVT_STRING)
				{
					results[0].RetrieveAWString(strDesc);
				}
				m_pTxt_AwardLevel->SetText(strDesc);
				if ( (strDesc == GetStringFromTable(7020)) || (strDesc == GetStringFromTable(7021)) )
				{
					m_pImg_Congrat->Show(true);
				}
				strDesc = pLottery->GetDBEssence()->candidates[m_iResult-1].desc;
				m_pTxt_AwardMoney->SetText(strDesc);
				uiPlus = 1.0f;
				uiSlow = 1;
				m_bStart = false;
				m_bFinish = true;
			}
			else if (uiSlow<=0)
			{
				m_iCurPos++;
				if (m_iCurPos>16)
				{
					m_iCurPos = 1;
				}
				m_pImg_Word[m_iCurPos-1]->SetCover(m_pA2DSpriteLotteryCur, 0, 1);
				if (m_iCurPos>1)
				{
					m_pImg_Word[m_iCurPos-2]->SetCover(NULL, 0, 1);
				}
				else
				{
					m_pImg_Word[15]->SetCover(NULL, 0, 1);
				}
				SetItemCoverByIcon( m_pImg_Award, 1, pLottery->GetDBEssence()->candidates[m_iCurPos-1].icon, m_iCurPos);
				uiPlus += 0.4f;
				uiSlow = (UINT)uiPlus;
			}
		}
		else
		{
			m_iCurPos++;
			if (m_iCurPos>CDLGLOTTERYITEM_MAX)
			{
				m_iCurPos = 1;
			}
			m_pImg_Word[m_iCurPos-1]->SetCover(m_pA2DSpriteLotteryCur, 0, 1);
			if (m_iCurPos>1)
			{
				m_pImg_Word[m_iCurPos-2]->SetCover(NULL, 0, 1);
			}
			else
			{
				m_pImg_Word[15]->SetCover(NULL, 0, 1);
			}
			SetItemCoverByIcon( m_pImg_Award, 1, pLottery->GetDBEssence()->candidates[m_iCurPos-1].icon, m_iCurPos);
		}
	}
}

void CDlgLotteryNew::TickLotteryNew()
{
	if( m_iResult == 0 )
	{
		if (m_iCurPos==0)
			m_iCurPos = 1;
		else 
			m_iCurPos = 0;
		for (int i=0; i<CDLGLOTTERYITEM_MAX; ++i)
		{
			if (i%2==m_iCurPos)
				m_pImg_Word[i]->SetCover(NULL, 0, 1);
			else
				m_pImg_Word[i]->SetCover(m_pA2DSpriteLotteryCur, 0, 1);
		}
		int pos = a_Random(1,16);
		SetItemCoverByIconNew(m_pImg_Award, pos);
	}
	else
	{
		for (int i=0; i<CDLGLOTTERYITEM_MAX; ++i)
		{
			m_pImg_Word[i]->SetCover(NULL, 0, 1);
		}
//		m_pImg_Word[m_iResult-1]->SetCover(m_pA2DSpriteLotteryCur, 0, 1);
		SetItemCoverByIconNew(m_pImg_Award, m_iResult);
		m_pBtn_StartStop->Enable(false);
		m_pTxt_Award->Show(true);
		m_pTxt_AwardLevel->SetText(GetStringFromTable(7019+m_iLevel));
		if (m_iLevel==1 || m_iLevel==2)
		{
			m_pImg_Congrat->Show(true);
		}
		
		ACString strText;
		if(m_pLottery2)
		{
			strText = m_pLottery2->GetDBEssence()->candidates[m_iResult-1].desc;
		}
		else if (m_pLottery3)
		{
			CECIvtrItem *pItem = CECIvtrItem::CreateItem(m_pLottery3->GetDBEssence()->candidate_items[m_iResult-1].item_id, 0, 1);
			if (pItem)
			{
				strText = pItem->GetName();
				if (m_pLottery3->GetDBEssence()->candidate_items[m_iResult-1].item_count>1)
				{
					ACHAR strNum[20];
					a_sprintf(strNum, GetStringFromTable(834), m_pLottery3->GetDBEssence()->candidate_items[m_iResult-1].item_count);
					strText += strNum;
				}
			}
		}

		m_pTxt_AwardMoney->SetText(strText);
		
		m_bStart = false;
		m_bFinish = true;
		m_iInterval = m_siInterval;
	}
}

void CDlgLotteryNew::SetItemCoverByIcon(PAUIIMAGEPICTURE pItemImage, int count, unsigned int id, int index)
{
	AString strFile;
	af_GetFileTitle(GetGame()->GetDataPath(id), strFile);
	strFile.MakeLower();
	
	pItemImage->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
		GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
	
	CECIvtrItem *pItem = GetHostPlayer()->GetPack()->GetItem(GetData());
	CECIvtrLottery *pLottery = (CECIvtrLottery*)pItem;

	AWString itemHint = _AL("");
	itemHint = pLottery->GetDBEssence()->candidates[index-1].desc;
	if ( count>1 )
	{
		ACHAR strNum[20];
		a_sprintf(strNum, GetStringFromTable(834), count);
		itemHint += strNum;
	}
	pItemImage->SetHint(itemHint);
}

#undef  LOTTERY_NEW_TBL_NAME


AUI_BEGIN_COMMAND_MAP(CDlgLottery3, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",		OnCommandCancel)
AUI_ON_COMMAND("Btn_Start",		OnCommandStart)
AUI_ON_COMMAND("Btn_GetAward",	OnCommandGetAward)
AUI_ON_COMMAND("Btn_Auto",		OnCommandStartAuto)
AUI_ON_COMMAND("Btn_StopAuto",	OnCommandStopAuto)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgLottery3, CDlgBase)

AUI_ON_EVENT("Txt_Description",			WM_LBUTTONDOWN,		OnEventLButtonDown_Desc)
AUI_ON_EVENT("Txt_Description",			WM_MOUSEMOVE,		OnEventMouseMove_Desc)

AUI_END_EVENT_MAP()

const int	CDlgLottery3::ms_iAward1Color	= 0xff8635;
abase::hash_map<int, int>	CDlgLottery3::ms_mapItem2Color;

CDlgLottery3::CDlgLottery3()
{
}

CDlgLottery3::~CDlgLottery3()
{
}

void CDlgLottery3::OnCommandStart(const char* szCommand)
{
	GetDlgItem("Gfx_1")->Show(false);
	GetDlgItem("Gfx_2")->Show(true);
	GetDlgItem("Gfx_3")->Show(false);
	GetDlgItem("Btn_GetAward")->Show(true);
	GetDlgItem("Btn_Start")->Show(false);
	PAUILABEL pLabAward = (PAUILABEL)GetDlgItem("Txt_AwardMoney");
	pLabAward->Show(false);

	CECIvtrItem *pItem = (m_pLottery2!=NULL ? (CECIvtrItem*)m_pLottery2 : (CECIvtrItem*)m_pLottery3);
	GetGameUIMan()->SetIvtrInfo((PAUIIMAGEPICTURE)GetDlgItem("Img_Award"), pItem);

	m_iState = 1;
	if (GetHostPlayer()->GetPack()->SearchEmpty()<0)
	{
		OnCommandStopAuto("");
	}
}

void CDlgLottery3::OnCommandGetAward(const char* szCommand)
{
	GetDlgItem("Gfx_2")->Show(false);
	GetDlgItem("Gfx_3")->Show(true);
	GetDlgItem("Btn_GetAward")->Show(false);
	GetDlgItem("Btn_Start")->Show(true);
	GetDlgItem("Btn_Start")->Enable(false);

	CECIvtrItem *pItem = GetHostPlayer()->GetPack()->GetItem(GetData());
	if( pItem )
	{
		pItem->Freeze(false);
		GetHostPlayer()->UseItemInPack(IVTRTYPE_PACK, GetData());
	}

	m_iState = 2;
}

void CDlgLottery3::OnCommandStartAuto(const char* szCommand)
{
	m_bContinue = true;
	GetDlgItem("Btn_StopAuto")->Show(true);
	GetDlgItem("Btn_Auto")->Show(false);
}

void CDlgLottery3::OnCommandStopAuto(const char* szCommand)
{
	m_bContinue = false;
	GetDlgItem("Btn_StopAuto")->Show(false);
	GetDlgItem("Btn_Auto")->Show(true);
}

void CDlgLottery3::OnCommandCancel(const char* szCommand)
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

void CDlgLottery3::OnEventLButtonDown_Desc(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - p->X;
	int y = GET_Y_LPARAM(lParam) - p->Y;
	AUITEXTAREA_EDITBOX_ITEM Item;
	Item.m_pItem = NULL;

	GetItemLinkItemOn(x, y, pObj, &Item);
	if( Item.m_pItem != NULL )
	{
		if( Item.m_pItem->GetType() == enumEIIvtrlItem )
			GetGameUIMan()->CommonMessage("Win_ItemDesc", "set_id", Item.m_pItem->GetMsgIndex(), (long)this);
	}
}

void CDlgLottery3::OnEventMouseMove_Desc(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - p->X;
	int y = GET_Y_LPARAM(lParam) - p->Y;
	AUITEXTAREA_EDITBOX_ITEM Item;
	Item.m_pItem = NULL;

	GetItemLinkItemOn(x, y, pObj, &Item);
}

bool CDlgLottery3::OnInitDialog()
{
	m_pItemNum = (PAUILABEL)GetDlgItem("Txt_Num");

	m_iInterval = 0;
	ms_mapItem2Color.clear();
	ms_mapItem2Color[18794] = 0xf7ff1e;
	ms_mapItem2Color[42401] = 0xfcff9c;
	ms_mapItem2Color[49293] = 0xffb400;
	
	m_bContinue = false;
	m_iState	= 0;
	m_iLastStart = 0;
	return true;
}

void CDlgLottery3::OnShowDialog()
{
	GetDlgItem("Txt_AwardMoney")->Show(false);
	InitLottery();
	
	CECIvtrItem *pItem = (m_pLottery2!=NULL ? (CECIvtrItem*)m_pLottery2 : (CECIvtrItem*)m_pLottery3);
	GetGameUIMan()->SetIvtrInfo((PAUIIMAGEPICTURE)GetDlgItem("Img_Award"), pItem);
	GetDlgItem("Gfx_1")->Show(true);
	GetDlgItem("Gfx_3")->Show(false);
	if(pItem && pItem->GetTemplateID() == 58780)
	{
		GetDlgItem("Lab_Hint2")->Show(false);
		GetDlgItem("Lab_Hint")->Show(false);
	}
	else
	{
		GetDlgItem("Lab_Hint2")->Show(true);
		GetDlgItem("Lab_Hint")->Show(true);
	}

	OnCommandStopAuto("");
}

void CDlgLottery3::InitLottery()
{
	abase::hash_map<ACString, int> strMap;
	int i;
	CECIvtrItem *pItem = GetHostPlayer()->GetPack()->GetItem(GetData());
	if (!pItem)
		return;

	if (m_pLottery2)
	{
		delete m_pLottery2;
		m_pLottery2 = NULL;
	}
	if (m_pLottery3)
	{
		delete m_pLottery3;
		m_pLottery3 = NULL;
	}
	
	PAUILABEL pLotteryName = (PAUILABEL)GetDlgItem("Lab_Item");
	if (pItem->GetClassID()==CECIvtrItem::ICID_LOTTERY2)
	{
		m_pLottery2 = (CECIvtrLottery2*)pItem->Clone();
		pLotteryName->SetText(m_pLottery2->GetDBEssence()->name);
	}
	else
	{
		m_pLottery3 = (CECIvtrLottery3*)pItem->Clone();
		pLotteryName->SetText(m_pLottery3->GetDBEssence()->name);
	}

	m_iItemId = pItem->GetTemplateID();
	pItem->Freeze(true);

	PAUIIMAGEPICTURE pImgLottery = (PAUIIMAGEPICTURE)GetDlgItem("Img_Item");
	GetGameUIMan()->SetIvtrInfo(pImgLottery, pItem);

	ACString strText;

	PAUITEXTAREA pTxtDesc = (PAUITEXTAREA)GetDlgItem("Txt_Description");
	pTxtDesc->SetText(_AL(""));
/* 目前不支持这个
	if (pItem->GetClassID() == CECIvtrItem::ICID_LOTTERY)
	{
		CECIvtrLottery *pLottery = dynamic_cast<CECIvtrLottery*>(pItem);
		i = 0;
		while (i<32 && pLottery->GetDBEssence()->candidates[i].desc[0])
		{
			if (strMap.find(pLottery->GetDBEssence()->candidates[i].desc)!=strMap.end())
			{
				strMap[pLottery->GetDBEssence()->candidates[i].desc] = 0;
			}
			i++;
		}
	}
	else 
*/	if (pItem->GetClassID() == CECIvtrItem::ICID_LOTTERY2)
	{
		CECIvtrLottery2 *pLottery2 = dynamic_cast<CECIvtrLottery2*>(pItem);
		i = 0;
		while (i<32 && pLottery2->GetDBEssence()->candidates[i].desc[0])
		{
			if (strMap.find(pLottery2->GetDBEssence()->candidates[i].desc)==strMap.end())
			{
				strMap[pLottery2->GetDBEssence()->candidates[i].desc] = 0;
			}
			i++;
		}
		
		abase::hash_map<ACString, int>::iterator it = strMap.begin();
		int index = 0;
		for (; it!=strMap.end(); ++it)
		{
			pTxtDesc->AppendText(it->first);
			pTxtDesc->AppendText(_AL(" "));
		}
	}
	else if (pItem->GetClassID() == CECIvtrItem::ICID_LOTTERY3)
	{
		CECIvtrLottery3 *pLottery3 = dynamic_cast<CECIvtrLottery3*>(pItem);
		i = 0;
		while (i<32 && pLottery3->GetDBEssence()->candidate_items[i].item_id)
		{
			CECIvtrItem *pItem = CECIvtrItem::CreateItem(pLottery3->GetDBEssence()->candidate_items[i].item_id, 0, 1);
			if (pItem)
			{
				if (strMap.find(pItem->GetName())==strMap.end())
				{
					EditBoxItemBase item(enumEIIvtrlItem);
					item.SetName(pItem->GetName());
					item.SetInfo(pItem->GetName());
					item.SetColor(GetItemColor(pItem->GetTemplateID()));

					strText = (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
					ACString strName;
					strName.Format(GetStringFromTable(939), pItem->GetName());
					strMap[pItem->GetName()] = pItem->GetTemplateID();
				}
				delete pItem;
			}
			i++;
		}

		abase::hash_map<ACString, int>::iterator it = strMap.begin();
		int index = 0;
		for (; it!=strMap.end(); ++it)
		{
			EditBoxItemBase item(enumEIIvtrlItem);
			item.SetName(it->first);
			item.SetInfo(it->first);
			ACString strName;
			strName.Format(GetStringFromTable(939), it->first);
			pTxtDesc->AppendText((ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize(), it->second, strName, GetItemColor(it->second));
			pTxtDesc->AppendText(_AL(" "));
		}
	}

	GetDlgItem("Gfx_1")->Show(false);
	GetDlgItem("Gfx_2")->Show(false);
	GetDlgItem("Gfx_3")->Show(true);
	GetDlgItem("Btn_GetAward")->Show(false);
	GetDlgItem("Btn_Start")->Show(true);
	GetDlgItem("Btn_Start")->Enable(true);
}

bool CDlgLottery3::GetItemLinkItemOn(int x, int y,
								 PAUIOBJECT pObj, AUITEXTAREA_EDITBOX_ITEM *pLink)
{
	bool bClickedChatPart = false;
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	POINT ptPos = pObj->GetPos();
	PAUITEXTAREA pText = (PAUITEXTAREA)pObj;
	
	GetGame()->ChangeCursor(RES_CUR_NORMAL);
	if( pText->GetHitArea(x - ptPos.x, y - ptPos.y) == AUITEXTAREA_RECT_FRAME )
	{
		int i;
		abase::vector<AUITEXTAREA_EDITBOX_ITEM> &vecItemLink = pText->GetEditBoxItems();
		
		x += p->X;
		y += p->Y;
		for( i = 0; i < (int)vecItemLink.size(); i++ )
		{
			if( vecItemLink[i].m_pItem->GetType() == enumEIIvtrlItem &&
				vecItemLink[i].rc.PtInRect(x, y) )
			{
				GetGame()->ChangeCursor(RES_CUR_HAND);
				*pLink = vecItemLink[i];
				break;
			}
		}
		bClickedChatPart = true;
	}
	
	return bClickedChatPart;
}

int CDlgLottery3::GetItemColor(int id)
{
	return ms_mapItem2Color.find(id)==ms_mapItem2Color.end() ? ms_iAward1Color : ms_mapItem2Color[id];
}

void CDlgLottery3::OnTick()
{
	ACString strText;
	int itemCount = GetHostPlayer()->GetPack()->GetItemTotalNum(m_iItemId);
	strText.Format(_AL("%d"), itemCount);
	m_pItemNum->SetText(strText);

	CECIvtrItem *pItem = GetHostPlayer()->GetPack()->GetItem(GetData());
	if (m_iInterval>0)
	{
		m_iInterval -= GetGame()->GetTickTime();
		if (m_iInterval<=0)
		{
			if (pItem&&pItem->GetTemplateID()==m_iItemId)
			{
				InitLottery();
			}
			else
			{
				int newIndex = GetHostPlayer()->GetPack()->FindItem(m_iItemId);
				if (newIndex>=0)
				{
					SetData(newIndex);
					InitLottery();
				}
			}
		}
	}
	else if ( itemCount>0 && ( !pItem || pItem->GetTemplateID()!=m_iItemId ) )
	{
		// 彩票用完以后又获得同样的彩票
		int newIndex = GetHostPlayer()->GetPack()->FindItem(m_iItemId);
		if (newIndex>=0)
		{
			SetData(newIndex);
		}
	}

	if (m_bContinue)
	{
		int tickNow = GetTickCount();
		switch (m_iState)
		{
		case 0:
			if (GetHostPlayer()->GetPack()->GetItemTotalNum(m_iItemId)>0)
			{
				OnCommandStart("");
				m_iLastStart = tickNow;
			}
			else
				OnCommandStopAuto("");
			break;
		case 1:
			if (tickNow>(m_iLastStart+m_siInterval))
			{
				OnCommandGetAward("");
				m_iLastStart  = tickNow;
			}
			break;
		case 2:
			if (tickNow>(m_iLastStart+m_siInterval*2))
			{
				m_iState = 0;
			}
			break;
		}
	}
}

int CDlgLottery3::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="set_award")
	{
		if(wParam>0&&wParam<=16)
		{
/*			m_iResult	= wParam;
			m_iGfxIndex = m_iResult;
			m_iLevel	= lParam;
			
			POINT ptPos = m_pImg_Word[m_iGfxIndex-1]->GetPos();
			ptPos.x += m_pImg_Word[m_iGfxIndex-1]->GetSize().cx / 2;
			ptPos.y += m_pImg_Word[m_iGfxIndex-1]->GetSize().cy / 2;
			m_pGfx->SetParentTM(a3d_Translate((float)ptPos.x, (float)ptPos.y, 0));
			m_pGfx->Start();
*/
			SetItemCoverByIconNew((PAUIIMAGEPICTURE)GetDlgItem("Img_Award"), wParam);
		
			ACString strText;
			m_iInterval = m_siInterval;
			if(m_pLottery2)
			{
				strText = m_pLottery2->GetDBEssence()->candidates[wParam-1].desc;
				GetDlgItem("Img_Award")->Show(true);
			}
			else if (m_pLottery3)
			{
				CECIvtrItem *pItem = CECIvtrItem::CreateItem(m_pLottery3->GetDBEssence()->candidate_items[wParam-1].item_id, 0, 1);
				if (pItem && pItem->GetClassID() != CECIvtrItem::ICID_ERRORITEM)
				{
					GetDlgItem("Img_Award")->Show(true);
					strText = pItem->GetName();
					if (m_pLottery3->GetDBEssence()->candidate_items[wParam-1].item_count>1)
					{
						ACHAR strNum[20];
						a_sprintf(strNum, GetStringFromTable(834), m_pLottery3->GetDBEssence()->candidate_items[wParam-1].item_count);
						strText += strNum;
					}
					delete pItem;
				}
			}
			PAUILABEL pLabAward = (PAUILABEL)GetDlgItem("Txt_AwardMoney");
			pLabAward->SetText(strText);
			pLabAward->Show(true);
		}
		m_iState = 2;
		m_iLastStart = GetTickCount();
	}
	return 1;
}

AUI_BEGIN_COMMAND_MAP(CDlgLotteryGift, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",		OnCommandCancel)
AUI_ON_COMMAND("Btn_Start",		OnCommandStart)
AUI_ON_COMMAND("Btn_GetAward",	OnCommandGetAward)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgLotteryGift, CDlgBase)

AUI_ON_EVENT("Txt_Description",			WM_LBUTTONDOWN,		OnEventLButtonDown_Desc)
AUI_ON_EVENT("Txt_Description",			WM_MOUSEMOVE,		OnEventMouseMove_Desc)

AUI_END_EVENT_MAP()

void CDlgLotteryGift::OnCommandCancel(const char* szCommand)
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

void CDlgLotteryGift::OnCommandStart(const char* szCommand)
{
}

void CDlgLotteryGift::OnCommandGetAward(const char* szCommand)
{
	CECIvtrItem *pItem = GetHostPlayer()->GetPack()->GetItem(GetData());
	if( pItem )
		pItem->Freeze(false);
	c2s_SendCmdGiftLotteryDeliver(GetData(), pItem->GetTemplateID());
	Show(true);
}


bool CDlgLotteryGift::OnInitDialog()
{
	m_pImgLottery = (PAUIIMAGEPICTURE)GetDlgItem("Img_Award");

	return true;
}

void CDlgLotteryGift::OnShowDialog()
{
}

bool CDlgLotteryGift::GetItemLinkItemOn(int x, int y,
								 PAUIOBJECT pObj, AUITEXTAREA_EDITBOX_ITEM *pLink)
{
	bool bClickedChatPart = false;
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	POINT ptPos = pObj->GetPos();
	PAUITEXTAREA pText = (PAUITEXTAREA)pObj;
	
	GetGame()->ChangeCursor(RES_CUR_NORMAL);
	if( pText->GetHitArea(x - ptPos.x, y - ptPos.y) == AUITEXTAREA_RECT_FRAME )
	{
		int i;
		abase::vector<AUITEXTAREA_EDITBOX_ITEM> &vecItemLink = pText->GetEditBoxItems();
		
		x += p->X;
		y += p->Y;
		for( i = 0; i < (int)vecItemLink.size(); i++ )
		{
			if( vecItemLink[i].m_pItem->GetType() == enumEIIvtrlItem &&
				vecItemLink[i].rc.PtInRect(x, y) )
			{
				GetGame()->ChangeCursor(RES_CUR_HAND);
				*pLink = vecItemLink[i];
				break;
			}
		}
		bClickedChatPart = true;
	}
	
	return bClickedChatPart;
}

void CDlgLotteryGift::OnTick()
{
}

void CDlgLotteryGift::OnEventLButtonDown_Desc(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - p->X;
	int y = GET_Y_LPARAM(lParam) - p->Y;
	AUITEXTAREA_EDITBOX_ITEM Item;
	Item.m_pItem = NULL;
	
	GetItemLinkItemOn(x, y, pObj, &Item);
	if( Item.m_pItem != NULL )
	{
		if( Item.m_pItem->GetType() == enumEIIvtrlItem )
			GetGameUIMan()->CommonMessage("Win_ItemDesc", "set_id", Item.m_pItem->GetMsgIndex(), (long)this);
	}
}

void CDlgLotteryGift::OnEventMouseMove_Desc(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - p->X;
	int y = GET_Y_LPARAM(lParam) - p->Y;
	AUITEXTAREA_EDITBOX_ITEM Item;
	Item.m_pItem = NULL;
	
	GetItemLinkItemOn(x, y, pObj, &Item);
}


AUI_BEGIN_COMMAND_MAP(CDlgLotteryTangYuan, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",		OnCommandCancel)
AUI_ON_COMMAND("Btn_Start",		OnCommandStart)
AUI_ON_COMMAND("Btn_Get",		OnCommandGetAward)

AUI_END_COMMAND_MAP()

void CDlgLotteryTangYuan::OnCommandCancel(const char* szCommand)
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

void CDlgLotteryTangYuan::OnCommandStart(const char* szCommand)
{
	EC_LOCAL_SETTING setting = GetGame()->GetConfigs()->GetLocalSettings();
	if (GetHostPlayer()->GetCash()<200)
	{
		GetGameUIMan()->MessageBox(14002);
		return;
	}
	else if (!setting.bStopPromptConsumeCash)
	{
		GetGameUIMan()->GetDialog("Win_Lottery5_Popup")->Show(true);
	}
	else
		CommonMessage("confirm", 0, 0);
}

void CDlgLotteryTangYuan::OnCommandGetAward(const char* szCommand)
{
	GetGameUIMan()->MessageBox("Game_TangYuan_GetAward", GetGameUIMan()->GetStringFromTable(14003), MB_YESNO);
}

int CDlgLotteryTangYuan::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="confirm")
	{
		CECIvtrLotteryTangYuan *pItem = (CECIvtrLotteryTangYuan*)GetHostPlayer()->GetPack()->GetItem(GetData());
		GetGameSession()->c2s_CmdUseItem((BYTE)IVTRTYPE_PACK, (BYTE)GetData(), pItem->GetTemplateID(), 1);
		m_iLastStart = 0;
		int iLevel = pItem->GetLevel();
		int i;
		AString strName;

		// 闪烁GFX
		for (i=0; i<32; ++i)
		{
			strName.Format("Gfx_Item%02d", i+1);
			GetDlgItem(strName)->Show(false);
		}
		for (i=4*(iLevel-1); i<32; ++i)
		{
			strName.Format("Gfx_Item%02d", i+1);
			if (pItem->GetDBEssence()->group_gifts[i/4].gifts[i%4].id>0)
				GetDlgItem(strName)->Show(true);
		}

		// 煮元宵gfx
		if (!m_pChkGfx->IsChecked())
		{
			GetDlgItem("Gfx_Start")->Show(false);
			GetDlgItem("Gfx_Start")->Show(true);
			m_pImgAward->SetCover(NULL, -1);
			m_pImgAward->SetText(_AL(""));
		}
	}
	else if (strMsg=="getaward")
	{		
		CECIvtrItem *pItem = GetHostPlayer()->GetPack()->GetItem(GetData());
		if( pItem )
			pItem->Freeze(false);
		c2s_SendCmdLotteryCashing(GetData());
		GetDlgItem("Gfx_Get")->Show(true);
		GetDlgItem("Btn_Start")->Enable(false);
		GetDlgItem("Btn_Get")->Enable(false);
		m_iGetAward = 0;
	}
	return 0;
}

bool CDlgLotteryTangYuan::OnInitDialog()
{
	m_pImgLottery	= (PAUIIMAGEPICTURE)GetDlgItem("Img_Award");
	m_pPrgExp		= (PAUIPROGRESS)GetDlgItem("Prgs_Score");
	m_pLabCash		= (PAUILABEL)GetDlgItem("Txt_Money");
	m_pLabLevel		= (PAUILABEL)GetDlgItem("Txt_Lv");
	m_pBtnStart		= GetDlgItem("Btn_Start");
	m_pBtnGet		= GetDlgItem("Btn_Get");
	m_pChkGfx		= (PAUICHECKBOX)GetDlgItem("Chk_Gfx");
	m_pImgAward		= (PAUIIMAGEPICTURE)GetDlgItem("Item_Award");
	
	int i;
	AString strName;
	for (i=0; i<32; ++i)
	{
		strName.Format("Gfx_Frame%02d", i+1);
		GetDlgItem(strName)->Show(false);
		strName.Format("Gfx_Item%02d", i+1);
		GetDlgItem(strName)->Show(false);
	}
	for (i=0; i<4; ++i)
	{
		strName.Format("Gfx_Combo%02d", i+2);
		m_pImgExpShow[i]	= (PAUIIMAGEPICTURE)GetDlgItem(strName);
	}
	GetDlgItem("Gfx_Start")->Show(false);
	GetDlgItem("Gfx_Get")->Show(false);
	m_iLastStart = 9999;
	m_iShowExpTime = 9999;
	SetData(9999);

	return true;
}

void CDlgLotteryTangYuan::OnShowDialog()
{
	CECIvtrLotteryTangYuan *pItem = (CECIvtrLotteryTangYuan*)GetHostPlayer()->GetPack()->GetItem(GetData());
	if(!pItem)
		return;
	
	GetDlgItem("Gfx_Start")->Show(false);
	GetDlgItem("Gfx_Get")->Show(false);
	GetDlgItem("Btn_Start")->Enable(true);
	GetDlgItem("Btn_Get")->Enable(true);
	int i;
	AString strName;
	for (i=0; i<8; ++i)
	{
		strName.Format("Gfx_Drop%02d", i+1);
		GetDlgItem(strName)->Show(false);
	}
	pItem->Freeze(true);
	m_iLastStart = 9999;
	m_iGetAward = 9999;
	m_iShowExpTime = 9999;
	PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)GetDlgItem("Gfx_Start");
	pImg->SetGfx("");
	AfxGetGFXExMan()->ReleaseCache();
	pImg->SetGfx("界面\\元宵抽奖.gfx");
	
	m_pImgAward->SetCover(NULL, -1);
	m_pImgAward->SetText(_AL(""));
}

void CDlgLotteryTangYuan::OnTick()
{
	// 领奖延时显示5s
	if (m_iGetAward<5000)
	{
		m_iGetAward += GetGame()->GetTickTime();
		return;
	}
	else if (m_iGetAward<9999)
	{
		m_iGetAward = 9999;
		Show(false);
		return;
	}

	CECIvtrLotteryTangYuan *pItem = (CECIvtrLotteryTangYuan*)GetHostPlayer()->GetPack()->GetItem(GetData());
	if (!pItem)
	{
		Show(false);
		return;
	}
	// 暴击显示
	if (m_iShowExpTime<9999)
		m_iShowExpTime += GetGame()->GetTickTime();
	if (m_iShowExpTime>1000)
	{
		m_iShowExpTime = 9999;
		int i;
		for (i=0; i<4; ++i)
		{
			m_pImgExpShow[i]->Show(false);
		}
	}
	if (m_iShowExpTime==9999)
	{
		int iShowExp = pItem->ShowOneExp();
		if (iShowExp>0)
		{
			m_pImgExpShow[iShowExp-2]->Show(true);
			m_iShowExpTime = 0;
		}
	}

	// 领奖GFX逻辑
	m_pBtnStart->Enable(m_iLastStart>1000);
	m_pBtnGet->Enable(m_iLastStart>1000);
	int i;
	ACString strText;
	AString strName;
	PAUIIMAGEPICTURE pImg, pImgBack;
	int iLevel = pItem->GetLevel();
	m_iLastStart += GetGame()->GetTickTime();

	// 煮元宵500ms以后停止闪烁gfx显示
	if (m_iLastStart>500 && m_iLastStart<1000)
	{
		for (i=0; i<32; ++i)
		{
			strName.Format("Gfx_Item%02d", i+1);
			GetDlgItem(strName)->Show(false);
		}
		m_iLastStart = 1000;
		// 如果没够禁止播放，则播放元宵掉落gfx
		if (!m_pChkGfx->IsChecked())
		{
			for (i=0; i<iLevel-1; ++i)
			{
				strName.Format("Gfx_Drop%02d", i+1);
				GetDlgItem(strName)->Show(false);
			}
			for (; i<8; ++i)
			{
				strName.Format("Gfx_Drop%02d", i+1);
				GetDlgItem(strName)->Show(false);
				GetDlgItem(strName)->Show(true);
			}			
			
			for (i=0; i<32; ++i)
			{
				strName.Format("Frame_%02d", i+1);
				pImgBack = (PAUIIMAGEPICTURE)GetDlgItem(strName);
				pImgBack->Show(false);
				strName.Format("Item_%02d", i+1);
				pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
				pImg->Show(pItem->GetDBEssence()->group_gifts[i/4].gifts[i%4].id>0);
				pImg->SetCover(NULL, 0, 1);
				if (i<((iLevel-1)*4))
				{
					pImgBack->SetColor(0xff808080);
					pImg->SetColor(0xff808080);
					GetGameUIMan()->SetIvtrInfo(pImg, pItem->GetDBEssence()->group_gifts[i/4].gifts[i%4].id,
						pItem->GetDBEssence()->group_gifts[i/4].gifts[i%4].num);
				}
				else
				{
					pImgBack->SetColor(0xffffffff);
					pImg->SetColor(0xffffffff);
					pImg->SetCover(NULL, -1);
					pImg->SetText(_AL(""));
				}
			}
		}
	}
	if (!m_pChkGfx->IsChecked() && m_iLastStart<4000)
	{
		m_pBtnStart->Enable(false);
		m_pBtnGet->Enable(false);
		return;
/*		if ()
		{
			for (i=0; i<32; ++i)
			{
				strName.Format("Frame_%02d", i+1);
				pImgBack = (PAUIIMAGEPICTURE)GetDlgItem(strName);
				pImgBack->Show(false);
				strName.Format("Item_%02d", i+1);
				pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
				pImg->Show(pItem->GetDBEssence()->group_gifts[i/4].gifts[i%4].id>0);
				pImg->SetCover(NULL, 0, 1);
				if (i<((iLevel-1)*4))
				{
					pImgBack->SetColor(0xff808080);
					pImg->SetColor(0xff808080);
					GetGameUIMan()->SetIvtrInfo(pImg, pItem->GetDBEssence()->group_gifts[i/4].gifts[i%4].id,
						pItem->GetDBEssence()->group_gifts[i/4].gifts[i%4].num);
				}
				else
				{
					pImgBack->SetColor(0xffffffff);
					pImg->SetColor(0xffffffff);
					pImg->SetCover(NULL, -1);
					pImg->SetText(_AL(""));
				}
			}
			return;
		}
*/	}
	
	m_pBtnGet->Enable(pItem->GetEssence().ticket>0);	
	GetDlgItem("Gfx_Start")->Show(false);

	m_pLabCash->SetText(GetCashText(GetHostPlayer()->GetCash()));
	strText.Format(_AL("Lv%d"), iLevel);
	m_pLabLevel->SetText(strText);

	int iLvMax = iLevel>=8 ? pItem->GetDBEssence()->exp_level[6] : pItem->GetDBEssence()->exp_level[iLevel-1];
	m_pPrgExp->SetProgress(AUIPROGRESS_MAX * pItem->GetEssence().exp / iLvMax);
	strText.Format(_AL("%d/%d"), pItem->GetEssence().exp, iLvMax);
//	m_pPrgExp->SetHint(strText);
	for (i=0; i<32; ++i)
	{
		strName.Format("Item_%02d", i+1);
		pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		pImg->Show(pItem->GetDBEssence()->group_gifts[i/4].gifts[i%4].id>0);
		strName.Format("Frame_%02d", i+1);
		pImgBack = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		pImgBack->Show(pItem->GetDBEssence()->group_gifts[i/4].gifts[i%4].id>0);
		if(pItem->GetDBEssence()->group_gifts[i/4].gifts[i%4].id)
			GetGameUIMan()->SetIvtrInfo(pImg, pItem->GetDBEssence()->group_gifts[i/4].gifts[i%4].id,
				pItem->GetDBEssence()->group_gifts[i/4].gifts[i%4].num);
		if (pItem->GetEssence().ticket>0 && pItem->GetEssence().ticket==i+1)
		{
			pImg->SetCover(CDlgSkill::m_pA2DSpriteSkillSelect, (GetTickCount() / 200) % 7, 1);
			GetGameUIMan()->SetIvtrInfo(m_pImgAward, pItem->GetDBEssence()->group_gifts[i/4].gifts[i%4].id,
				pItem->GetDBEssence()->group_gifts[i/4].gifts[i%4].num);
		}
		else
			pImg->SetCover(NULL, 0, 1);
		
		if (i<((iLevel-1)*4))
		{
			pImg->SetColor(0xff808080);
			pImgBack->SetColor(0xff808080);
		}
		else
		{
			pImg->SetColor(0xffffffff);
			pImgBack->SetColor(0xffffffff);
		}
	}
}
