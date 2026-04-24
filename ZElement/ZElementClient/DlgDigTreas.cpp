// Filename	: DlgDigTreas.cpp
// Creator	: Fu Chonggang
// Date		: 2012/3/16

#include "DlgDigTreas.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_SendC2SCmds.h"
#include "AUI\\AUICTranslate.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUIImagepicture.h"
#include "A3DGfxEx.h"
#include "A2DSprite.h"
#include "AFI.h"
#include "EC_IvtrItem.h"
#include "EC_GPDataType.h"
#include "EC_Configs.h"
#include "elementdataman.h"
#include "ExpTypes.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgDlgTreas, CDlgBase)

AUI_ON_COMMAND("Btn_Refresh",		OnCommand_Refresh)
AUI_ON_COMMAND("Btn_Explore",		OnCommand_Search)
AUI_ON_COMMAND("Btn_Unlock",		OnCommand_Unlock)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgDlgTreas, CDlgBase)

AUI_ON_EVENT("Img_Normal*",			WM_LBUTTONDOWN,		OnEventLButtonDownImage)
AUI_ON_EVENT("Gfx_Normal*",			WM_LBUTTONDOWN,		OnEventLButtonDownImage)
AUI_ON_EVENT("Gfx_Seal*",			WM_LBUTTONDOWN,		OnEventLButtonDownImage)
AUI_ON_EVENT("Img_Done*",			WM_LBUTTONDOWN,		OnEventLButtonDownImage)
AUI_ON_EVENT("Gfx_Open*",			WM_LBUTTONDOWN,		OnEventLButtonDownImage)

AUI_END_EVENT_MAP()

CDlgDlgTreas::CDlgDlgTreas()
{
}

CDlgDlgTreas::~CDlgDlgTreas()
{
}

bool CDlgDlgTreas::OnInitDialog()
{
	m_pCurRegin = 0;
	m_iCurIndex = -1;
	m_iCurStars = -1;
	m_iDstStars = -1;
	AString strName;
	int i;
	for (i=0; i<NormalRegionNum; ++i)
	{
		strName.Format("Img_Normal%d", i);
		m_pImgRegions[i] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		strName.Format("Lab_Normal%d", i);
		m_pLabRegionName[i] = (PAUILABEL)GetDlgItem(strName);
	}
	m_pLabRefreshItem = (PAUILABEL)GetDlgItem("Txt_RefreshItem");
	PAUIIMAGEPICTURE pImgRefreshItem = (PAUIIMAGEPICTURE)GetDlgItem("Img_RefreshItem");
	int id = GetGameRun()->GetSpecialIDConfig().id_treasure_region_upgrade_item;
	GetGameUIMan()->SetIvtrInfo(pImgRefreshItem, id);
	
	m_vecItemPic.push_back(GetGameUIMan()->GetIconSet(CECGameUIMan::ICONS_INVENTORY));

	m_pRandomAward = new A2DSprite;
	char szFile[MAX_PATH];
	sprintf(szFile, "%s\\surfaces\\icon\\ZhuxianExplore_ListSymbol.dds",af_GetBaseDir());
	m_pRandomAward->SetDynamicTex(true);
	if ( !m_pRandomAward->Init(m_pA3DDevice, szFile, AUI_COLORKEY) )
	{
		a_LogOutput(0, "CDlgDlgTreas::Init(), Error while loading icon");
		return false;
	}
	m_vecItemPic.push_back(m_pRandomAward);

	PAUILISTBOX pLstAward = (PAUILISTBOX)GetDlgItem("Lst_Award");
	pLstAward->SetImageList(&m_vecItemPic);
	pLstAward->SetItemMask(0xffffffff);
	pLstAward->SetItemTextSmallImage(true);

	m_pImgRefresh = (PAUIIMAGEPICTURE)GetDlgItem("Gfx_Refresh");
	m_pImgRefresh->Show(false);

	GetDlgItem("Gfx_Time")->Show(false);

	m_bRefresh = false;
	m_iCurTime = 0;

	// 隐藏区域存储起来方便查找
	m_vecRegionsHide.clear();
	elementdataman* pDB = GetGame()->GetElementDataMan();
	DATA_TYPE DataType = DT_INVALID;
	
	int tid = pDB->get_first_data_id(ID_SPACE_CONFIG, DataType);
	while (tid)
	{
		const void* pData = pDB->get_data_ptr(tid, ID_SPACE_CONFIG, DataType);
		
		if (DataType == DT_SCROLL_REGION_CONFIG)
		{
			SCROLL_REGION_CONFIG* pConf = (SCROLL_REGION_CONFIG*)pData;
			if (pConf->region_pos>=9)
			{
				m_vecRegionsHide.push_back(pConf);
			}
		}
		
		//	Get next item
		tid = pDB->get_next_data_id(ID_SPACE_CONFIG, DataType);
	}

	return true;
}

bool CDlgDlgTreas::Release()
{
	A3DRELEASE(m_pRandomAward);
	return CDlgBase::Release();
}

void CDlgDlgTreas::OnShowDialog()
{
	int i;
	AString strName;
	for (i=9; i<=11; ++i)
	{
		strName.Format("Gfx_Open%d", i);
		GetDlgItem(strName)->Show(false);
	}
	
	for (i=0; i<(int)m_vecRegionsHide.size(); ++i)
	{
		SCROLL_REGION_CONFIG *pCurRegin = m_vecRegionsHide[i];
		int j;
		for (j=9; j<=11; ++j)
		{
			if ((int)pCurRegin->id==m_vecRegionInfo[j].id)
			{
				if (!HideRegionExplored(pCurRegin->region_pos, pCurRegin->id))
				{
					GetGameUIMan()->CommonMessage("Win_ZhuxianExploreIntro", "show_intro", (unsigned int)pCurRegin, 0);
				}
				break;
			}
		}
	}

	m_pImgRefresh->Show(false);
	if (m_iCurIndex<0)
	{
		OnEventLButtonDownImage(0, 0, GetDlgItem("Img_Normal0"));
	}
	else
	{
		UpdateRegions();
		UpdateRegionSel();
	}
}

void CDlgDlgTreas::OnTick()
{
	ACString strText;
	int id = GetGameRun()->GetSpecialIDConfig().id_treasure_region_upgrade_item;
	int num = GetHostPlayer()->GetPack()->GetItemTotalNum(id);
	strText.Format(_AL("%d"), num);
	m_pLabRefreshItem->SetText(strText);
	GetDlgItem("Btn_Refresh")->Enable(num>0 && m_vecRegionInfo[m_iCurIndex].type==1);
	GetDlgItem("Btn_Explore")->Enable( (m_iRemainDigCount>0&&m_iCurIndex<9) ||
	(m_iCurIndex>=9&&m_vecRegionInfo[m_iCurIndex].status==1) );
	TickStars();
	if (GetDlgItem("Gfx_Time")->IsShow())
	{
		GetDlgItem("Gfx_Time")->SetData(GetDlgItem("Gfx_Time")->GetData("disp_time")+GetGame()->GetTickTime(), "disp_time");
		if (GetDlgItem("Gfx_Time")->GetData("disp_time")>2000)
		{
			GetDlgItem("Gfx_Time")->Show(false);
		}
	}

	if (!m_pCurRegin)
	{
		return;
	}
	PAUILABEL	pLabUnlockItemNum = (PAUILABEL)GetDlgItem("Txt_UnlockItem");
	if (m_pCurRegin->hide_region_unlock_item_id>0)
	{
		num = GetHostPlayer()->GetPack()->GetItemTotalNum(m_pCurRegin->hide_region_unlock_item_id);
		strText.Format(_AL("%d"), num);
		pLabUnlockItemNum->SetText(strText);
		GetDlgItem("Btn_Unlock")->Enable(m_iCurIndex>=9 && m_vecRegionInfo[m_iCurIndex].status==3 &&
			GetHostPlayer()->GetPack()->FindItem(m_pCurRegin->hide_region_unlock_item_id)>=0);
	}
	else
	{
		pLabUnlockItemNum->SetText(_AL(""));
		GetDlgItem("Btn_Unlock")->Enable(false);
	}
}

void CDlgDlgTreas::TickStars()
{
	if (m_bRefresh)
	{
		int i;
		AString strName;
		PAUIIMAGEPICTURE pImg;
		m_iCurTime += GetGame()->GetTickTime();
		if (m_iCurTime>3000 || m_iCurStars>=5)
		{
			m_bRefresh = false;
			m_iCurStars = m_iDstStars;
			m_iDstStars = -1;
			UpdateRegionSel();			
			
			if (m_bRefreshSucc)
			{
				ACString strText;
				strText.Format(GetGameUIMan()->GetStringFromTable(13204), m_iCurStars);
				GetGameUIMan()->AddChatMessage(strText, GP_CHAT_MISC);
			}
			else
				GetGameUIMan()->AddChatMessage(GetGameUIMan()->GetStringFromTable(13203), GP_CHAT_MISC);
			return;
		}

		int iPassStars = m_iCurTime / 100;
		int iNumFlashStar = iPassStars % (6-m_iCurStars);

		for (i=0; i<m_iCurStars+iNumFlashStar; ++i)
		{
			strName.Format("Img_Star%d", i+1);
			pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
			pImg->FixFrame(0);
		}
		for (i=m_iCurStars+iNumFlashStar; i<5; ++i)
		{
			strName.Format("Img_Star%d", i+1);
			pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
			pImg->FixFrame(1);
		}
	}
}

void CDlgDlgTreas::ShowRefreshGfx()
{
	m_pImgRefresh->Show(true);
	A3DGFXEx* pGfx = m_pImgRefresh->GetGfx();
	pGfx->Start(true);
}

bool CDlgDlgTreas::HideRegionExplored(int index, int id)
{
	if (index<9 || index>11)
	{
		return true;
	}
	EC_GAME_CLIENTDATA gcd = GetGame()->GetConfigs()->GetGameClientData();
	if (id==gcd.nDigTreaId[index-9])
	{
		return true;
	}

	gcd.nDigTreaId[index-9] = id;
	GetGame()->GetConfigs()->SetGameClientData(gcd);
	GetGameRun()->SaveConfigsToServer();

	return false;
}

bool CDlgDlgTreas::RegionInTime(SCROLL_REGION_CONFIG *pCurRegin)
{
	
	int j;
	for (j=0; j<3; ++j)
	{
		if (pCurRegin->hide_region_active_time[j].active_start_time<=GetGame()->GetServerGMTTime() && 
			pCurRegin->hide_region_active_time[j].active_stop_time>=GetGame()->GetServerGMTTime())
		{
			return true;
		}
	}

	return false;
}

void CDlgDlgTreas::OnCommand_Refresh(const char * szCommand)
{ 
	if(!m_pCurRegin || m_iCurIndex<0 || m_iCurIndex>=(int)m_vecRegionInfo.size())
		return;

	int id = GetGameRun()->GetSpecialIDConfig().id_treasure_region_upgrade_item;
	int index = GetHostPlayer()->GetPack()->FindItem(id);
	if (m_vecRegionInfo[m_iCurIndex].level==5) // 已经是最高星等
	{
		GetGameUIMan()->MessageBox(13205);
		if (m_bRefresh)
		{
			m_bRefresh = false;
			m_iDstStars = -1;
			UpdateRegionSel();
			if (m_bRefreshSucc)
			{
				ACString strText;
				strText.Format(GetGameUIMan()->GetStringFromTable(13204), m_iCurStars);
				GetGameUIMan()->AddChatMessage(strText, GP_CHAT_MISC);
			}
			else
				GetGameUIMan()->AddChatMessage(GetGameUIMan()->GetStringFromTable(13203), GP_CHAT_MISC);
		}
	}
	else if (index>=0)
	{
		ShowRefreshGfx();
		GetGameSession()->c2s_CmdUpgradeTreasureRegion(id, index, m_pCurRegin->id, m_pCurRegin->region_pos);
	}
	else
	{
		GetGameUIMan()->MessageBox(13201);
	}
}

void CDlgDlgTreas::OnCommand_Search(const char * szCommand)
{
	if (m_iCurIndex>=0 && m_iCurIndex<(int)m_vecRegionInfo.size())
	{
		if ( m_vecRegionInfo[m_iCurIndex].status==1 )
			GetGameSession()->c2s_CmdDigTreasureRegion(m_pCurRegin->id, m_pCurRegin->region_pos);
		else
			GetGameUIMan()->MessageBox(13202);
	}
}

void CDlgDlgTreas::OnCommand_Unlock(const char * szCommand)
{
	if (m_pCurRegin->hide_region_unlock_item_id>0)
	{
		int index = GetHostPlayer()->GetPack()->FindItem(m_pCurRegin->hide_region_unlock_item_id);
		if (index>=0)
		{
			c2s_SendCmdUnlockTreasureRegion(m_pCurRegin->hide_region_unlock_item_id, index, 
			m_pCurRegin->id, m_pCurRegin->region_pos);
		}
	}
}

void CDlgDlgTreas::OnEventLButtonDownImage(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	m_bRefresh = false;
	int iCurIndexArea;
	int iCurIndex;
	if (strstr(pObj->GetName(), "Img_Normal"))
		iCurIndexArea = atoi(pObj->GetName()+strlen("Img_Normal"));
	else if (strstr(pObj->GetName(), "Gfx_Normal"))
		iCurIndexArea = atoi(pObj->GetName()+strlen("Gfx_Normal"));
	else if (strstr(pObj->GetName(), "Gfx_Seal"))
		iCurIndexArea = atoi(pObj->GetName()+strlen("Gfx_Seal"));
	else if (strstr(pObj->GetName(), "Img_Done"))
		iCurIndexArea = atoi(pObj->GetName()+strlen("Img_Done"));
	else if (strstr(pObj->GetName(), "Gfx_Open"))
		iCurIndexArea = atoi(pObj->GetName()+strlen("Gfx_Open"));
	
	SCROLL_REGION_CONFIG *pCurRegin = 0;
	int i;
	for (i=0; i<(int)m_vecRegionInfo.size(); ++i)
	{
		DATA_TYPE DataType;
		const void* pData = GetGame()->GetElementDataMan()->get_data_ptr(m_vecRegionInfo[i].id, ID_SPACE_CONFIG, DataType);
		if (pData && DataType == DT_SCROLL_REGION_CONFIG)
		{
			pCurRegin = (SCROLL_REGION_CONFIG*)pData;
			if (pCurRegin->region_pos==iCurIndexArea)
			{
				iCurIndex = i;
				break;
			}
		}
		
		pCurRegin = 0;
	}
	
	if (!pCurRegin)
	{
		GetGameUIMan()->MessageBox(13207);
		return;
	}

	PAUIIMAGEPICTURE pImgNormal, pImgGfxNormal;
	if (m_pCurRegin)
	{
		AString strName;
		strName.Format("Img_Normal%d", m_pCurRegin->region_pos);
		pImgNormal = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		pImgNormal->FixFrame(m_iRemainDigCount==0 ? 2:1);
		strName.Format("Gfx_Normal%d", m_pCurRegin->region_pos);
		pImgGfxNormal = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		pImgGfxNormal->Show((m_iRemainDigCount>0 || m_iCurIndex>=9)&&
			m_vecRegionInfo[m_iCurIndex].status==1);
	}
	
	m_pCurRegin = pCurRegin;
	m_iCurIndex = iCurIndex;
	if (m_pCurRegin)
	{
		AString strName;
		strName.Format("Img_Normal%d", m_pCurRegin->region_pos);
		pImgNormal = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		pImgNormal->FixFrame(0);
		strName.Format("Gfx_Normal%d", m_pCurRegin->region_pos);
		pImgGfxNormal = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		pImgGfxNormal->Show(false);
	}
	UpdateRegionSel();
}

int CDlgDlgTreas::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	using namespace S2C;

	if (strMsg=="TREASURE_INFO")
	{
		treasure_info *pInfo = (treasure_info*)wParam;
		m_iRemainDigCount = pInfo->remain_dig_count;
		m_vecRegionInfo.clear();
		int i;
		for (i=0; i<pInfo->region_count; ++i)
		{
			m_vecRegionInfo.push_back(Treasure_Info(pInfo->treasure_region[i].type,
				pInfo->treasure_region[i].status,
				pInfo->treasure_region[i].level,
				pInfo->treasure_region[i].id));
		}

		UpdateRegions();
		UpdateRegionSel();
	}
	else if (strMsg=="INC_TREASURE_DIG_COUNT")
	{
		UpdateRegions();
		GetDlgItem("Gfx_Time")->Show(true);
		GetDlgItem("Gfx_Time")->SetData(0, "disp_time");
		Show(true);
	}
	else if (strMsg=="TREASURE_REGION_UPGRADE_RESULT")
	{
		m_bRefresh = true;
		m_iCurTime = 0;
		if(m_iDstStars>=0)
		{
			m_iCurStars = m_iDstStars;
			m_iDstStars = -1;
			if (m_bRefreshSucc)
			{
				ACString strText;
				strText.Format(GetGameUIMan()->GetStringFromTable(13204), m_iCurStars);
				GetGameUIMan()->AddChatMessage(strText, GP_CHAT_MISC);
			}
			else
				GetGameUIMan()->AddChatMessage(GetGameUIMan()->GetStringFromTable(13203), GP_CHAT_MISC);
		}

		treasure_region_upgrade_result *pInfo = (treasure_region_upgrade_result*)wParam;
		int i;
		for (i=0; i<(int)m_vecRegionInfo.size(); ++i)
		{
			if (pInfo->region_id==m_vecRegionInfo[i].id)
			{
				m_vecRegionInfo[i].level = pInfo->new_level;
				m_vecRegionInfo[i].status = 1;
				m_iDstStars = pInfo->new_level;
			}
		}

		m_bRefreshSucc = pInfo->success;
	}
	else if (strMsg=="TREASURE_REGION_UNLOCK_RESULT")
	{
		treasure_region_unlock_result *pInfo = (treasure_region_unlock_result*)wParam;
		int i;
		for (i=0; i<(int)m_vecRegionsHide.size(); ++i)
		{
			SCROLL_REGION_CONFIG *pCurRegin = m_vecRegionsHide[i];
			if ((int)pCurRegin->id==pInfo->region_id)
			{
				m_vecRegionInfo[pCurRegin->region_pos].id = pCurRegin->id;
				m_vecRegionInfo[pCurRegin->region_pos].type = 2;
				m_vecRegionInfo[pCurRegin->region_pos].status = 1;
				m_pCurRegin = pCurRegin;
				m_iCurIndex = pCurRegin->region_pos;
			}
		}
		if (!IsShow())
			Show(true);

		UpdateRegions();
		UpdateRegionSel();
	}
	else if (strMsg=="TREASURE_REGION_DIG_RESULT")
	{
		treasure_region_dig_result *pInfo = (treasure_region_dig_result*)wParam;
		if (!pInfo->success)
		{
			GetGameUIMan()->MessageBox(13206);
			return 0;
		}
		
		PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_ZhuxianExploreIssue");
		PAUILISTBOX pLstAward = (PAUILISTBOX)pDlg->GetDlgItem("Lst_Award");
		pLstAward->ResetContent();
		ACString strText;
		int j;
		for (j=0; j<pInfo->award_item_num; ++j)
		{
			CECIvtrItem* pItem = CECIvtrItem::CreateItem(pInfo->award_item_list[j].item_id, 0, 1);
			if (pItem)
			{
				strText = pItem->GetName();
				if (pInfo->award_item_list[j].item_count>1)
				{
					ACString strNum;
					strNum.Format(GetGameUIMan()->GetStringFromTable(834), pInfo->award_item_list[j].item_count);
					strText += strNum;
				}
				pLstAward->AddString(strText);
				
				AUICTranslate trans;
				AWString strHint = trans.Translate(pItem->GetDesc());
				pLstAward->SetItemHint(j, strHint);
				delete pItem;
			}
		}
		if(pInfo->event_id>=0)
		{
			DATA_TYPE DataType;
			const void* pData = GetGame()->GetElementDataMan()->get_data_ptr(pInfo->region_id, ID_SPACE_CONFIG, DataType);
			if (pData && DataType == DT_SCROLL_REGION_CONFIG)
			{
				SCROLL_REGION_CONFIG *pCurRegin = (SCROLL_REGION_CONFIG*)pData;
				int i;
				int iSuiteIndex = 0;
				int iTestNum = 0;
				for (i=0; i<10; ++i)
				{
					if (pCurRegin->event_list[i].event_id==pInfo->event_id)
					{
						// 服务器发过来事件id是0则从所有0的时间里面随机取一个
						if (pInfo->event_id==0)
						{
							int iSel = a_Random(0, iTestNum);
							if (iSel==0)
								iSuiteIndex = i;
							iTestNum++;
						}
						else
						{
							iSuiteIndex = i;
							break;
						}
					}
				}
				PopEventDialog(pCurRegin, iSuiteIndex);
			}
		}
	}

	return 0;
}

void CDlgDlgTreas::PopEventDialog(SCROLL_REGION_CONFIG *pCurRegin, int eventIndex)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_ZhuxianExploreIssue");
	PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)pDlg->GetDlgItem("Img_Pic");
	AString strFile = GetGame()->GetDataPath(pCurRegin->event_list[eventIndex].event_icon);
	if (strFile.GetLength()>(int)strlen("Surfaces\\"))
		strFile.CutLeft(strlen("Surfaces\\"));
	AUIOBJECT_SETPROPERTY p;
	strcpy(p.fn, strFile);
	pImg->SetProperty("Image File", &p);
	
	PAUITEXTAREA pTxt = (PAUITEXTAREA)pDlg->GetDlgItem("Txt_Intro");
	pTxt->SetText(pCurRegin->event_list[eventIndex].event_desc);
	
	pDlg->Show(true);
}

void CDlgDlgTreas::UpdateRegions()
{
	PAUIIMAGEPICTURE pImgNormal, pImgGfxNormal, pImgGfxSeal, pImgDone;
	AString strName;
	int i;
	// 清空普通区域
	for (i=0; i<NormalRegionNum; ++i)
	{
		strName.Format("Img_Normal%d", i);
		pImgNormal = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		pImgNormal->Show(false);
		strName.Format("Gfx_Normal%d", i);
		pImgGfxNormal = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		pImgGfxNormal->Show(false);
		strName.Format("Gfx_Seal%d", i);
		pImgGfxSeal = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		pImgGfxSeal->Show(true);
		strName.Format("Img_Done%d", i);
		pImgDone = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		pImgDone->Show(false);
	}

	// 设置锁定区域
	for (i=0; i<(int)m_vecRegionsHide.size(); ++i)
	{
		SCROLL_REGION_CONFIG *pCurRegin = m_vecRegionsHide[i];
		int j;
		for (j=0; j<3; ++j)
		{
			if (pCurRegin->hide_region_active_time[j].active_start_time<=GetGame()->GetServerGMTTime() && 
				pCurRegin->hide_region_active_time[j].active_stop_time>=GetGame()->GetServerGMTTime())
			{
				strName.Format("Img_Normal%d", pCurRegin->region_pos);
				pImgNormal = (PAUIIMAGEPICTURE)GetDlgItem(strName);
				pImgNormal->Show(true);
				strName.Format("Gfx_Normal%d", pCurRegin->region_pos);
				pImgGfxNormal = (PAUIIMAGEPICTURE)GetDlgItem(strName);
				pImgGfxNormal->Show(false);
				strName.Format("Gfx_Seal%d", pCurRegin->region_pos);
				pImgGfxSeal = (PAUIIMAGEPICTURE)GetDlgItem(strName);
				pImgGfxSeal->Show(false);
				strName.Format("Img_Done%d", pCurRegin->region_pos);
				pImgDone = (PAUIIMAGEPICTURE)GetDlgItem(strName);
				pImgDone->Show(false);
				m_vecRegionInfo[pCurRegin->region_pos].id = pCurRegin->id;
				break;
			}
		}
	}

	for (i=0; i<(int)m_vecRegionInfo.size(); ++i)
	{
		DATA_TYPE DataType;
		const void* pData = GetGame()->GetElementDataMan()->get_data_ptr(m_vecRegionInfo[i].id, ID_SPACE_CONFIG, DataType);
		if (pData && DataType == DT_SCROLL_REGION_CONFIG)
		{
			SCROLL_REGION_CONFIG *pCurRegin = (SCROLL_REGION_CONFIG*)pData;
			// 当前没有开放隐藏区域，隐藏区域暂时当作未开放区域
			if (m_vecRegionInfo[i].type==0 || m_vecRegionInfo[i].status==0)
			{
				continue;
			}
			
			if (pCurRegin->region_pos>=9)
			{
				int is = m_vecRegionInfo[i].status;
				if (m_vecRegionInfo[i].status!=1 && !RegionInTime(pCurRegin))
				{
					m_vecRegionInfo[i].id = 0;
					if (m_iCurIndex==pCurRegin->region_pos)
					{
						m_pCurRegin = 0;
						m_iCurIndex = -1;
					}
					m_pLabRegionName[pCurRegin->region_pos]->SetText(GetGameUIMan()->GetStringFromTable(15006));
					continue;
				}
			}

			strName.Format("Img_Normal%d", pCurRegin->region_pos);
			pImgNormal = (PAUIIMAGEPICTURE)GetDlgItem(strName);
			pImgNormal->Show(true);
			strName.Format("Gfx_Seal%d", pCurRegin->region_pos);
			pImgGfxSeal = (PAUIIMAGEPICTURE)GetDlgItem(strName);
			pImgGfxSeal->Show(false);
			AString strFile = GetGame()->GetDataPath(pCurRegin->file_matter);
			if (strFile.GetLength()>(int)strlen("Surfaces\\"))
				strFile.CutLeft(strlen("Surfaces\\"));
			AUIOBJECT_SETPROPERTY p;
			strcpy(p.fn, strFile);
			pImgNormal->SetProperty("Image File", &p);
			pImgNormal->SetFrameNumber(3);

			strName.Format("Gfx_Normal%d", pCurRegin->region_pos);
			pImgGfxNormal = (PAUIIMAGEPICTURE)GetDlgItem(strName);
			pImgGfxNormal->Show(i!=m_iCurIndex && (m_iRemainDigCount>0||i>=9) && m_vecRegionInfo[i].status==1);
			strName.Format("Img_Done%d", pCurRegin->region_pos);
			pImgDone = (PAUIIMAGEPICTURE)GetDlgItem(strName);
			pImgDone->Show(m_vecRegionInfo[i].status==2 || m_vecRegionInfo[i].status==3);
			pImgDone->FixFrame(m_vecRegionInfo[i].status==2 ? 0 : 1);

			if (i==m_iCurIndex)
				pImgNormal->FixFrame(0);
			else if (m_iRemainDigCount==0)
				pImgNormal->FixFrame(2);
			else
				pImgNormal->FixFrame(1);

			m_pLabRegionName[pCurRegin->region_pos]->SetText(pCurRegin->name);
		}
	}

	PAUILABEL pLabCount = (PAUILABEL)GetDlgItem("Txt_Time");
	ACString strText;
	strText.Format(_AL("%d"), m_iRemainDigCount);
	pLabCount->SetText(strText);

	
	if (m_iCurIndex<0)
	{
		OnEventLButtonDownImage(0, 0, GetDlgItem("Img_Normal0"));
	}
}

void CDlgDlgTreas::UpdateRegionSel()
{
	if (!m_pCurRegin)
	{
		return;
	}

	PAUIIMAGEPICTURE pImgRegion = (PAUIIMAGEPICTURE)GetDlgItem("Img_Pic");
	AString strFile = GetGame()->GetDataPath(m_pCurRegin->file_icon);
	if (strFile.GetLength()>(int)strlen("Surfaces\\"))
		strFile.CutLeft(strlen("Surfaces\\"));
	AUIOBJECT_SETPROPERTY p;
	strcpy(p.fn, strFile);
	pImgRegion->SetProperty("Image File", &p);

	m_iCurStars = m_vecRegionInfo[m_iCurIndex].level;
	if (m_iCurIndex>8)
	{
		m_iCurStars = 5;
	}
	AString strName;
	PAUIIMAGEPICTURE pImg;
	int i;
	for (i=0; i<5; ++i)
	{
		strName.Format("Img_Star%d", i+1);
		pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		pImg->FixFrame(i<m_iCurStars ? 0 : 1);
	}

	PAUITEXTAREA pTxtDesc = (PAUITEXTAREA)GetDlgItem("Txt_Intro");
	pTxtDesc->SetText(m_pCurRegin->region_desc);

	PAUILISTBOX pLstAward = (PAUILISTBOX)GetDlgItem("Lst_Award");
	pLstAward->ResetContent();
	ACString strText, strRandom;
	for (i=0; i<10; ++i)
	{
		if (m_pCurRegin->region_award_list[m_iCurStars][i].item_id==0)
			break;

		CECIvtrItem* pItem = CECIvtrItem::CreateItem(m_pCurRegin->region_award_list[m_iCurStars][i].item_id, 0, 1);
		if (pItem)
		{
			if (m_pCurRegin->region_award_list[m_iCurStars][i].deliver_prob<0.99999)
			{
				EditBoxItemBase rdImg(enumEIImage);
				rdImg.SetImageIndex(ICONSET_RANDOM);
				rdImg.SetImageFrame(0);
				strRandom = (ACHAR)AUICOMMON_ITEM_CODE_START + rdImg.Serialize();;
			}
			else 
				strRandom = _AL(" ");
			AString strIndex;
			af_GetFileTitle(pItem->GetIconFile(), strIndex);
			strIndex.MakeLower();
			int index = GetGameUIMan()->GetIconId(CECGameUIMan::ICONS_INVENTORY , strIndex);
			EditBoxItemBase tempItem(enumEIImage);
			tempItem.SetImageIndex(ICONSET_INVENTORY);
			tempItem.SetImageFrame(index);
			ACString strImage  = (ACHAR)AUICOMMON_ITEM_CODE_START + tempItem.Serialize();
			strText = strImage + _AL(" ") + pItem->GetName();
			if (m_pCurRegin->region_award_list[m_iCurStars][i].item_count>1)
			{
				ACString strNum;
				strNum.Format(GetGameUIMan()->GetStringFromTable(834), m_pCurRegin->region_award_list[m_iCurStars][i].item_count);
				strText += strNum;
			}
			pLstAward->AddString(strRandom + _AL("\t") + strText);
			
			AUICTranslate trans;
			AWString strHint = trans.Translate(pItem->GetDesc());
			pLstAward->SetItemColHint(i, 0, strRandom==_AL(" ") ? _AL("") : GetGameUIMan()->GetStringFromTable(13208));
			pLstAward->SetItemColHint(i, 1, strHint);
			delete pItem;
		}
	}

	PAUIIMAGEPICTURE pImgUnlockItem = (PAUIIMAGEPICTURE)GetDlgItem("Img_UnlockItem");
	if (m_iCurIndex>=9)
	{
		GetGameUIMan()->SetIvtrInfo(pImgUnlockItem, m_pCurRegin->hide_region_unlock_item_id);
	}
	else
	{
		GetGameUIMan()->SetIvtrInfo(pImgUnlockItem, 0);
	}
}



AUI_BEGIN_COMMAND_MAP(CDlgDlgTreasEvent, CDlgBase)

AUI_ON_COMMAND("Btn_Confirm",	OnCommand_Confirm)

AUI_END_COMMAND_MAP()

CDlgDlgTreasEvent::CDlgDlgTreasEvent()
{
}

CDlgDlgTreasEvent::~CDlgDlgTreasEvent()
{
}

void CDlgDlgTreasEvent::OnCommand_Confirm(const char * szCommand)
{
	Show(false);
}



AUI_BEGIN_COMMAND_MAP(CDlgDlgTreasMin, CDlgBase)

AUI_ON_COMMAND("Btn_Max",	OnCommand_Active)

AUI_END_COMMAND_MAP()

CDlgDlgTreasMin::CDlgDlgTreasMin()
{
}

CDlgDlgTreasMin::~CDlgDlgTreasMin()
{
}


void CDlgDlgTreasMin::OnCommand_Active(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_ZhuxianExplore");
	if (pDlg)
	{
		if (GetHostPlayer()->GetBasicProps().iLevel>=30 || GetHostPlayer()->GetRebornInfo().size()>0)
			pDlg->Show(!pDlg->IsShow());
	}
}

void CDlgDlgTreasMin::OnTick()
{
	GetDlgItem("Btn_Max")->SetHint((GetHostPlayer()->GetBasicProps().iLevel>=30 || GetHostPlayer()->GetRebornInfo().size()>0)? 
		_AL("") : GetGameUIMan()->GetStringFromTable(10389));
}


AUI_BEGIN_COMMAND_MAP(CDlgDlgTreasIntro, CDlgBase)

AUI_ON_COMMAND("Btn_Confirm",	OnCommand_Confirm)

AUI_END_COMMAND_MAP()

CDlgDlgTreasIntro::CDlgDlgTreasIntro()
{
	int i;
	for (i=0; i<3; ++i)
	{
		m_vecConfigToShow[i] = 0;
	}
}

CDlgDlgTreasIntro::~CDlgDlgTreasIntro()
{
}

void CDlgDlgTreasIntro::OnCommand_Confirm(const char * szCommand)
{
	DisplayOneIntro();
}

void CDlgDlgTreasIntro::DisplayOneIntro()
{
	int i;
	for (i=0; i<3; ++i)
	{
		if (m_vecConfigToShow[i])
		{
			SCROLL_REGION_CONFIG *pCurRegin = m_vecConfigToShow[i];
			AString strName;
			PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)GetDlgItem("Img_Pic");
			AString strFile = GetGame()->GetDataPath(pCurRegin->file_icon);
			if (strFile.GetLength()>(int)strlen("Surfaces\\"))
				strFile.CutLeft(strlen("Surfaces\\"));
			AUIOBJECT_SETPROPERTY p;
			strcpy(p.fn, strFile);
			pImg->SetProperty("Image File", &p);
			PAUITEXTAREA pTxt = (PAUITEXTAREA)GetDlgItem("Txt_Intro");
			pTxt->SetText(pCurRegin->region_desc);

			// 显示gfx
			strName.Format("Gfx_Open%d", pCurRegin->region_pos);
			pImg = (PAUIIMAGEPICTURE)GetGameUIMan()->GetDialog("Win_ZhuxianExplore")->GetDlgItem(strName);
			pImg->Show(true);

			m_vecConfigToShow[i] = 0;
			break;
		}
	}
	if (i==3)
	{
		Show(false);
	}
}

int CDlgDlgTreasIntro::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="show_intro")
	{
		int i;
		for (i=0; i<3; ++i)
		{
			if (!m_vecConfigToShow[i])
			{
				m_vecConfigToShow[i] = (SCROLL_REGION_CONFIG*)wParam;
				break;
			}
		}
		if (!IsShow())
		{
			Show(true);
			SetIsModal(true);
			DisplayOneIntro();
		}
	}

	return 0;
}