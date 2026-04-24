#include "AFI.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_World.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrConsume.h"
#include "DlgCrssvrTeams.h"

#include "AUI\\AUICommon.h"
#include "AUI\\AUIDef.h"
#include "AUI\\AUICTranslate.h"
#include "AUI\\AUISubDialog.h"
#include "AUI\\AUITreeView.h"
#include "aui\\AUICheckBox.h"
#include "DlgCrssvrTeamsSub.h"
#include "globaldataman.h"
#include "elementdataman.h"
#include "gettoptable_re.hpp"
#include "gcrssvrteamsrole"

AUI_BEGIN_COMMAND_MAP(DlgCrssvrTeamsImg, CDlgBase)
AUI_END_COMMAND_MAP()

void Unpack(std::vector<GCrssvrTeamsRole>& rolelist, const Octets & src)
{
	int zoneid;
	if (src.size() <= 13)
		return;
	Marshal::OctetsStream os(src);
	try
	{
		//GCrssvrTeamsRole 
		os >> rolelist >> zoneid;
	}
	catch (...)
	{

	}
}

int checkAwardType(tm tmNow)
{
	tmNow.tm_mon  = tmNow.tm_mon+1;
	if ( ((tmNow.tm_mon== 3 && ((tmNow.tm_mday == 26 && tmNow.tm_hour >=0) ))
		|| (tmNow.tm_mon== 3 && (tmNow.tm_mday <=31 ))))
	{
		return 1;
	}
	if ( ((tmNow.tm_mon== 6 && ((tmNow.tm_mday == 26 && tmNow.tm_hour >=0) ))
		|| (tmNow.tm_mon== 6 && (tmNow.tm_mday <=30 ))))
	{
		return 2;
	}
	if ( ((tmNow.tm_mon== 9 && ((tmNow.tm_mday == 26 && tmNow.tm_hour >=0) ))
		|| (tmNow.tm_mon== 9 && (tmNow.tm_mday <=30 ))))
	{
		return 3;
	}
	if ( ((tmNow.tm_mon== 12 && ((tmNow.tm_mday == 26 && tmNow.tm_hour >=0) ))
		|| (tmNow.tm_mon== 12 && (tmNow.tm_mday <=31 ))))
	{
		return 4;
	}
	return 0;
}

bool checkAwardTime(tm tmNow)
{
	tmNow.tm_mon  = tmNow.tm_mon+1;
	if ( ((tmNow.tm_mon== 3 && ((tmNow.tm_mday == 26 && tmNow.tm_hour >=0) ))
		|| (tmNow.tm_mon== 3 && (tmNow.tm_mday <=31 ))))
	{
		return true;
	}
	if ( ((tmNow.tm_mon== 6 && ((tmNow.tm_mday == 26 && tmNow.tm_hour >=0) ))
		|| (tmNow.tm_mon== 6 && (tmNow.tm_mday <=30 ))))
	{
		return true;
	}
	if ( ((tmNow.tm_mon== 9 && ((tmNow.tm_mday == 26 && tmNow.tm_hour >=0) ))
		|| (tmNow.tm_mon== 9 && (tmNow.tm_mday <=30 ))))
	{
		return true;
	}
	if ( ((tmNow.tm_mon== 12 && ((tmNow.tm_mday == 26 && tmNow.tm_hour >=0) ))
		|| (tmNow.tm_mon== 12 && (tmNow.tm_mday <=31 ))))
	{
		return true;
	}
	return false;
}

DlgCrssvrTeamsImg::DlgCrssvrTeamsImg()
{

}

DlgCrssvrTeamsImg::~DlgCrssvrTeamsImg()
{

}

bool DlgCrssvrTeamsImg::OnInitDialog()
{
	AString szTemp;
	for(int nIndex = 0; nIndex < 10; nIndex++)
	{
		szTemp.Format("Img_Member%02d",nIndex+1);
		m_pPicture[nIndex] = (PAUIIMAGEPICTURE)GetDlgItem(szTemp);
	}
	return true;
}


AString GetOccupationIcon(int nOccupation)
{
	AString szTemp;
	if(nOccupation == 0)
	{
		szTemp.Format("ingame\\achivicon\\27鬼道名宿.dds");
		return szTemp;
	}
	int iProfRace = PROFRACE_UNKNOWN;
	int iProf = nOccupation;
	if (iProf<=12)
		iProfRace = (iProf+2)/3;
	else if (iProf<=24)
		iProfRace = (iProf-12+2)/3;
	else if (iProf<=30)
	{
		iProfRace = PROFRACE_GUIDAO;
		szTemp.Format("ingame\\achivicon\\27鬼道名宿.dds");
	}
	else if (iProf<=38)
	{
		szTemp.Format("ingame\\achivicon\\208九黎氏供奉.dds");
		iProfRace = PROFRACE_JIULI;
	}
	else if (iProf<=44)
	{
		szTemp.Format("ingame\\achivicon\\209烈山氏供奉.dds");
		iProfRace = PROFRACE_LIESHAN;
	}
	else if (iProf<=50)
	{
		szTemp.Format("ingame\\achivicon\\259.dds");
		iProfRace = PROFRACE_HUAIGUANG;
	}
	else if (iProf<=55)
	{
		szTemp.Format("ingame\\achivicon\\258.dds");
		iProfRace = PROFRACE_TIANHUA;
	}
	else if (iProf<=68&&iProf>=64)
	{
		szTemp.Format("ingame\\achivicon\\283.dds");
		iProfRace = PROFRACE_FENGXIANG;
	}
	else if (iProf<=100&&iProf>=96)
	{
		szTemp.Format("ingame\\achivicon\\284.dds");
		iProfRace = PROFRACE_TAIHAO;
	}
	else if (iProf<=60&&iProf>=56)
	{
		szTemp.Format("ingame\\achivicon\\295.dds");
		iProfRace = PROFRACE_CHENHUANG;
	}
	else if(iProf<=106&&iProf>=102)
		iProfRace = PROFRACE_QIANJI;
	else if(iProf<=112&&iProf>=108)
		iProfRace = PROFRACE_YINGZHAO;


	if((nOccupation > 1 && nOccupation < 5)
		|| (nOccupation > 13 && nOccupation < 17))
	{	
		szTemp.Format("ingame\\achivicon\\23鬼王宗名宿.dds");
		return szTemp; // 鬼王
	}
	if((nOccupation > 4 && nOccupation < 9)
		|| (nOccupation > 16 && nOccupation < 20))
	{
		szTemp.Format("ingame\\achivicon\\25合欢派名宿.dds");
		return szTemp; // 合欢
	}
	if((nOccupation > 7 && nOccupation < 11)
		|| (nOccupation > 19 && nOccupation < 23))
	{
		szTemp.Format("ingame\\achivicon\\24青云门名宿.dds");
		return szTemp; // 青云
	}
	if((nOccupation > 10 && nOccupation < 14)
		|| (nOccupation > 22 && nOccupation < 26))
	{
		szTemp.Format("ingame\\achivicon\\26天音寺名宿.dds");
		return szTemp; // 天音
	}
	//if((nOccupation > 25 && nOccupation < 32))
	//{

	//	return szTemp; // 鬼道
	//}
	//if((nOccupation > 32 && nOccupation < 40))
	//{

	//	return szTemp; // 九黎
	//}
	//if((nOccupation > 39 && nOccupation < 46))
	//{
	//	
	//	return szTemp; // 烈山
	//}
	//if((nOccupation > 45 && nOccupation < 52))
	//{
	//	
	//	return szTemp; // 怀光
	//}
	//if((nOccupation > 51 && nOccupation < 57))
	//{
	//	
	//	return szTemp; // 天华
	//}
	//if((nOccupation > 56 && nOccupation < 65))
	//{
	//	
	//	return szTemp; // 辰皇
	//}
	//if((nOccupation > 61 && nOccupation < 71))
	//{
	//	
	//	return szTemp; // 焚香
	//}
	//if((nOccupation > 96 && nOccupation < 102))
	//{
	//	
	//	return szTemp; // 太昊
	//}
	if((nOccupation > 102 && nOccupation < 108))
	{
		szTemp.Format("ingame\\achivicon\\26天音寺名宿.dds");
		return szTemp; // 牵机
	}
	if((nOccupation > 108 && nOccupation < 114))
	{
		szTemp.Format("ingame\\achivicon\\26天音寺名宿.dds");
		return szTemp; // 英招
	}
	return szTemp;
}

void DlgCrssvrTeamsImg::Clear()
{
	AUIOBJECT_SETPROPERTY p;
	for(int nIndex = 0; nIndex < 10; nIndex++)
	{
		sprintf(p.fn, "");
		m_pPicture[nIndex]->	SetProperty("Image File", &p);
		m_pPicture[nIndex]->SetHint(_AL(""));
	}
}
void DlgCrssvrTeamsImg::UpDateImg(CrssvrTeamsRole* pInfo, int nIndex)
{
	ACString szTemp;
	AString strFile = GetOccupationIcon(pInfo->occupation);
	AUIOBJECT_SETPROPERTY p;
	sprintf(p.fn, strFile);
	m_pPicture[nIndex]->	SetProperty("Image File", &p);
	szTemp.Format(GetStringFromTable(20044),pInfo->szName,pInfo->level,GetStringFromTable(PROFESSION_START + pInfo->occupation));
	m_pPicture[nIndex]->SetHint(szTemp);
}


AUI_BEGIN_COMMAND_MAP(DlgCrssvrTeamsItem, CDlgBase)
AUI_ON_COMMAND("Btn_Display", OnCommand_Display)
AUI_END_COMMAND_MAP()

DlgCrssvrTeamsItem::DlgCrssvrTeamsItem()
{

}

DlgCrssvrTeamsItem::~DlgCrssvrTeamsItem()
{

}
bool DlgCrssvrTeamsItem::OnInitDialog()
{
	m_pTxt_Rank= (AUILabel*)GetDlgItem("Lab_Rank");
	m_pTxt_Name = (AUILabel*)GetDlgItem("Lab_Name");
	m_pTxt_Win= (AUILabel*)GetDlgItem("Lab_Win");
	m_pTxt_Lose= (AUILabel*)GetDlgItem("Lab_Lose");
	m_pTxt_Rate = (AUILabel*)GetDlgItem("Lab_Rate");
	m_pTxt_Score= (AUILabel*)GetDlgItem("Lab_Score");

	return true;
}


void DlgCrssvrTeamsItem::OnCommand_Display(const char * szCommand)
{
	DlgCrssvrTeamsImg*	pDlg = (DlgCrssvrTeamsImg*)GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_GroupRank_Member");
	pDlg->Clear();
	for(int nIndex = 0;nIndex < (int)m_vecTeamsRole.size();nIndex++)
	{
		CrssvrTeamsRole* pInfo = &m_vecTeamsRole[nIndex];
		pDlg->UpDateImg(pInfo,nIndex);
	}
	pDlg->Show(!pDlg->IsShow());
}

void DlgCrssvrTeamsItem::UpDateItemBase(void* pData,int nPage,int nIndex)
{
	GTopItem* pInfo = (GTopItem*)pData;

	m_pTxt_Name->SetText(ACString((const ACHAR*)(pInfo->name.begin()),pInfo->name.size()/sizeof(ACHAR)));

	
	ACString szTemp;
	szTemp.Format(_AL("%d"),nIndex + nPage*20 + 1);
	m_pTxt_Rank->SetText(szTemp);
	szTemp.Format(_AL("%d"),pInfo->value);
	m_pTxt_Score->SetText(szTemp);

}

void DlgCrssvrTeamsItem::UpDateSaveTeamsInfo(void* pData)
{
	m_vecTeamsRole.clear();
	GTopDetail* pInfo = (GTopDetail*)pData;
	std::vector<GCrssvrTeamsRole> vecRoleList;
	Unpack(vecRoleList,pInfo->content);
	for(int nIndex = 0; nIndex <(int) vecRoleList.size(); nIndex++)
	{
		CrssvrTeamsRole pInfopRole;
		GCrssvrTeamsRole* pRole = &vecRoleList[nIndex];
		pInfopRole.level = pRole->level;
		pInfopRole.szName =  ACString((const ACHAR*)pRole->name.begin(), pRole->name.size() / sizeof (ACHAR));
		pInfopRole.occupation = pRole->occupation;
		m_vecTeamsRole.push_back(pInfopRole);
	}
	ACString szTemp;
	szTemp.Format(_AL("%d"),pInfo->value1);
	m_pTxt_Win->SetText(szTemp);
	szTemp.Format(_AL("%d"),pInfo->value2);
	m_pTxt_Lose->SetText(szTemp);

	int nTemp = 0;
	if(pInfo->value1 != 0)
	{
		nTemp = int((float)(pInfo->value1 / float(pInfo->value1 + pInfo->value2 + pInfo->value3)) * 100);
		szTemp.Format(_AL("%d"),nTemp);
		szTemp +=_AL("%");
	}
	else
	{
		szTemp = _AL("0%");
	}
	m_pTxt_Rate->SetText(szTemp);
}



AUI_BEGIN_COMMAND_MAP(DlgCrssvrTeamsSub, CDlgBase)
AUI_END_COMMAND_MAP()


DlgCrssvrTeamsSub::DlgCrssvrTeamsSub()
{
	m_vecDlgItems.clear();
	m_vecSubDialogs.clear();
}

DlgCrssvrTeamsSub::~DlgCrssvrTeamsSub()
{

}

bool DlgCrssvrTeamsSub::Release()
{
	int i;
	int nSize = m_vecSubDialogs.size();
	for (i=0; i<nSize; i++)
	{
		DeleteControl(m_vecSubDialogs[i]);
	}
	m_vecSubDialogs.clear();
	//创建的界面uiman统一管理，不必单独destroy
	m_vecDlgItems.clear();

	return CDlgBase::Release();
}


bool DlgCrssvrTeamsSub::OnInitDialog()
{
	m_vecDlgItems.clear();
	m_vecSubDialogs.clear();
	m_pSubItem = (PAUISUBDIALOG)GetDlgItem("Sub_1");
	m_rcWindowOld = GetGameUIMan()->GetRect();
	return true;
}

bool DlgCrssvrTeamsSub::Render()
{
	return AUIDialog::Render();
}



void DlgCrssvrTeamsSub::UpDateCrssveInfo(void* pData, int nPage)
{
	GNET::GetTopTable_Re* pInfo = static_cast<GNET::GetTopTable_Re*>(pData);
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
	nSize = pInfo->items.size();
	int y=0;
	for (i=0; i<nSize; i++)
	{
		//这个文件不能写到ingame.dcf里，否则创建失败
		GetGameUIMan()->CreateDlg(GetGame()->GetA3DEngine(), GetGame()->GetA3DDevice(), "Battle_Dungeon_6v6_GroupRank_SubDetail.xml");
		DlgCrssvrTeamsItem* pDlg = (DlgCrssvrTeamsItem*)GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_GroupRank_SubDetail");
		AString strName;
		strName.Format("Win_Battle_Dungeon_6v6_GroupRank_SubDetail%d", i);
		pDlg->SetName(strName);
		pDlg->UpDateItemBase(&pInfo->items[i],nPage,i);
		if(pInfo->details.size() > 0)
		{
			pDlg->UpDateSaveTeamsInfo(&pInfo->details[i]);
		}
		
		pDlg->Resize(m_rcWindowOld, rcNew);
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

	PAUISUBDIALOG pSubD = (PAUISUBDIALOG)GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_GroupRank")->GetDlgItem("Sub_Detail");
	pSubD->SetSubDialogOrgSize(w, h);

}




AUI_BEGIN_COMMAND_MAP(DlgCrssvrTeamsAward_Week, CDlgBase)
AUI_ON_COMMAND("Rdo_0*", OnCommand_Rdo)
AUI_ON_COMMAND("Btn_Confirm0*", OnCommand_Confirm)
AUI_END_COMMAND_MAP()
	
AUI_BEGIN_EVENT_MAP(DlgCrssvrTeamsAward_Week, CDlgBase)
AUI_ON_EVENT("Img_Award0*",			WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_END_EVENT_MAP()
DlgCrssvrTeamsAward_Week::DlgCrssvrTeamsAward_Week()
{
	m_nMaxScore = 0;
}
DlgCrssvrTeamsAward_Week::~DlgCrssvrTeamsAward_Week()
{
	A3DRELEASE(m_pSprite);
}

void DlgCrssvrTeamsAward_Week::OnShowDialog()
{
	CheckRadioButton(1,1);
	CheckRadioButton(2,0);
	CheckRadioButton(3,0);

	ClearData();

	int nNum = GetHostPlayer()->GetPack()->GetItemTotalNum(60460);
	ACString szTemp;
	szTemp.Format(_AL("%d"),nNum);
	m_pTxt_Money->SetText(szTemp);
	nNum = GetHostPlayer()->GetPack()->GetItemTotalNum(60461);
	szTemp.Format(_AL("%d"),nNum);
	GetDlgItem("Txt_Silver")->SetText(szTemp);
	nNum = GetHostPlayer()->GetPack()->GetItemTotalNum(60462);
	szTemp.Format(_AL("%d"),nNum);
	GetDlgItem("Txt_Bronze")->SetText(szTemp);

	bool isAward = false;
	AString szTemp1;
	for(int index = 0; index < 3; index++)
	{
		szTemp1.Format("Btn_Confirm0%d",index+1);
		GetDlgItem(szTemp1)->Enable(isAward);
	}
	const long t = cs_personal_award_timestamp;
	tm *gtime = localtime((const time_t*)&t);
	tm dTime = *gtime;
	tm tmNow = GetGame()->GetServerLocalTime();
	if(checkAwardTime(tmNow))
	{
		if(cs_personal_award_timestamp == 0)
		{
			isAward = true;
		}
		else if(checkAwardType(dTime) != checkAwardType(tmNow))
		{
			isAward = true;
		}
		if(m_nMaxScore >= 1000 && isAward)
		{
			szTemp1 = "Btn_Confirm01";
			GetDlgItem(szTemp1)->Enable(true);
		}
		if(m_nMaxScore >= 1899 && isAward)
		{
			szTemp1 = "Btn_Confirm02";
			GetDlgItem(szTemp1)->Enable(true);
		}
		if(m_nMaxScore >= 2200 && isAward)
		{
			szTemp1 = "Btn_Confirm03";
			GetDlgItem(szTemp1)->Enable(true);
		}
	}
}

bool DlgCrssvrTeamsAward_Week::OnInitDialog()
{
	m_pTxt_Money =  (AUILabel*)GetDlgItem("Txt_Gold");
	m_pTxt_Score =  (AUILabel*)GetDlgItem("Txt_Score");
	m_pTxt_Money->SetText(_AL(""));
	m_pTxt_Score->SetText(_AL(""));
	AString strName;
	for(int nIndex = 0; nIndex < 9; nIndex++)
	{
		strName.Format("Img_Award0%d", nIndex+1);
		m_pPicture[nIndex] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		m_pPicture[nIndex]->SetData(nIndex, "index");
		m_pPicture[nIndex]->SetDataPtr(NULL);
	}
	DATA_TYPE DataType;
	int idEssence = GetGame()->GetElementDataMan()->get_first_data_id(ID_SPACE_CONFIG, DataType);
	while(idEssence)
	{
		if(DataType == DT_PERSONAL_LEVEL_AWARD_CONFIG)
			break;
		idEssence = GetGame()->GetElementDataMan()->get_next_data_id(ID_SPACE_CONFIG, DataType);
	}
	PERSONAL_LEVEL_AWARD_CONFIG* m_pConfig = (PERSONAL_LEVEL_AWARD_CONFIG *)GetGame()->GetElementDataMan()->
		get_data_ptr(idEssence, ID_SPACE_CONFIG, DataType);
	m_iCurSel = -1;
	for(int nIndex = 0; nIndex < 15; nIndex++)
	{
		if(m_pConfig->personal_level_award_info[nIndex].inception_points == 0)
		{
			break;
		}
		if(m_pConfig->personal_level_award_info[nIndex].inception_points >= 1000 && m_pConfig->personal_level_award_info[nIndex].cutoff_points <= 1899)
		{
			for(int nItemIndex = 0; nItemIndex < 5; nItemIndex++)
			{
				if(m_pConfig->personal_level_award_info[nIndex].item_info[nItemIndex].item_id != 0)
				{
					int nId = m_pConfig->personal_level_award_info[nIndex].item_info[nItemIndex].item_id;
					CECIvtrItem* pItem = CECIvtrItem::CreateItem(nId, 0,1);
					AString strFile;
					if (pItem )
					{
						pItem->SetNeedUpdate(false);
						GetGameUIMan()->SetIvtrInfo(m_pPicture[nItemIndex], pItem,
							m_pConfig->personal_level_award_info[nIndex].item_info[nItemIndex].item_num);
						delete pItem;
					}
					m_pPicture[nItemIndex]->SetDataPtr((void*)m_pConfig->personal_level_award_info[nIndex].level);
					GetDlgItem("Btn_Confirm01")->SetData(m_pConfig->personal_level_award_info[nIndex].level);
				}
			}
		}
		else 	if(m_pConfig->personal_level_award_info[nIndex].inception_points >= 1900 && m_pConfig->personal_level_award_info[nIndex].cutoff_points <= 2199)
		{
			for(int nItemIndex = 0; nItemIndex < 3; nItemIndex++)
			{
				if(m_pConfig->personal_level_award_info[nIndex].item_info[nItemIndex].item_id != 0)
				{
					int nId = m_pConfig->personal_level_award_info[nIndex].item_info[nItemIndex].item_id;
					CECIvtrItem* pItem = CECIvtrItem::CreateItem(nId, 0,1);
					AString strFile;
					if (pItem )
					{
						pItem->SetNeedUpdate(false);
						GetGameUIMan()->SetIvtrInfo(m_pPicture[nItemIndex+ 3], pItem,
							m_pConfig->personal_level_award_info[nIndex].item_info[nItemIndex].item_num);
						delete pItem;
					}
					m_pPicture[nItemIndex + 3]->SetDataPtr((void*)m_pConfig->personal_level_award_info[nIndex].level);
					GetDlgItem("Btn_Confirm02")->SetData(m_pConfig->personal_level_award_info[nIndex].level);
				}
			}
		}
		else 	if(m_pConfig->personal_level_award_info[nIndex].inception_points >= 2200)
		{
			for(int nItemIndex = 0; nItemIndex < 3; nItemIndex++)
			{
				if(m_pConfig->personal_level_award_info[nIndex].item_info[nItemIndex].item_id != 0)
				{
					int nId = m_pConfig->personal_level_award_info[nIndex].item_info[nItemIndex].item_id;
					CECIvtrItem* pItem = CECIvtrItem::CreateItem(nId, 0,1);
					AString strFile;
					if (pItem )
					{
						pItem->SetNeedUpdate(false);
						GetGameUIMan()->SetIvtrInfo(m_pPicture[nItemIndex+ 6], pItem,
							m_pConfig->personal_level_award_info[nIndex].item_info[nItemIndex].item_num);
						delete pItem;
					}
					m_pPicture[nItemIndex + 6]->SetDataPtr((void*)m_pConfig->personal_level_award_info[nIndex].level);
					GetDlgItem("Btn_Confirm03")->SetData(m_pConfig->personal_level_award_info[nIndex].level);
				}
			}
		}
	}
	m_pSprite = new A2DSprite;
	if( !m_pSprite ) return AUI_ReportError(__LINE__, __FILE__);

	bool bval = m_pSprite->Init(m_pA3DDevice, "icon\\Battle_Dungeon_6v6_SkillFocus.tga", 0);
	if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
	return true;
}

bool DlgCrssvrTeamsAward_Week::Release()
{
	A3DRELEASE(m_pSprite);
	return CDlgBase::Release();
}
void DlgCrssvrTeamsAward_Week::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	ClearData();
	m_iCurSel = pObj->GetData("index");
	if((int)m_pPicture[m_iCurSel]->GetDataPtr() > 0)
	{
		m_pPicture[m_iCurSel]->SetCover(m_pSprite, 0, 1);
	}
}


void DlgCrssvrTeamsAward_Week::OnCommand_Rdo(const char * szCommand)
{
	int nType = atoi(szCommand+5);
	DlgCrssvrTeamsMain* pTeamsMainDlg = (DlgCrssvrTeamsMain*)GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_GroupMain");
	pTeamsMainDlg->SetDialog(nType);
}
void DlgCrssvrTeamsAward_Week::OnCommand_Confirm(const char * szCommand)
{
	c2s_SendCmdCS_6v6Award(0,GetDlgItem(szCommand)->GetData());
}

void DlgCrssvrTeamsAward_Week::ClearData() 
{
	if (m_iCurSel>=0)
	{
		int i;
		for (i=0; i<9; ++i)
		{
			m_pPicture[i]->SetCover(0, 0, 1);
		}
	}
}

void DlgCrssvrTeamsAward_Week::SetUpDate(int nMaxScore)
{
	m_nMaxScore = nMaxScore;
	ACString szTemp;
	szTemp.Format(_AL("%d"),m_nMaxScore);
	m_pTxt_Score->SetText(szTemp);
}

int DlgCrssvrTeamsAward_Week::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="info")
	{
		AString szTemp;
		for(int index = 0; index < 3; index++)
		{
			szTemp.Format("Btn_Confirm0%d",index + 1);
			GetDlgItem(szTemp)->Enable(false);
		}
		S2C::collision_raid_info *p = (S2C::collision_raid_info*)wParam;
		cs_personal_award_timestamp = p->cs_personal_award_timestamp;
		bool isAward = false;
		const long t = cs_personal_award_timestamp;
		tm *gtime = localtime((const time_t*)&t);
		tm dTime = *gtime;
		tm tmNow = GetGame()->GetServerLocalTime();
		if(checkAwardTime(tmNow))
		{
			if(cs_personal_award_timestamp == 0)
			{
				isAward = true;
			}
			else if(checkAwardType(dTime) != checkAwardType(tmNow))
			{
				isAward = true;
			}
			if(m_nMaxScore >= 1000 && isAward)
			{
				szTemp = "Btn_Confirm01";
				GetDlgItem(szTemp)->Enable(true);
			}
			if(m_nMaxScore >= 1899 && isAward)
			{
				szTemp = "Btn_Confirm02";
				GetDlgItem(szTemp)->Enable(true);
			}
			if(m_nMaxScore >= 2200 && isAward)
			{
				szTemp = "Btn_Confirm03";
				GetDlgItem(szTemp)->Enable(true);
			}
		}

	}
	return 0;
}

AUI_BEGIN_COMMAND_MAP(DlgCrssvrTeamsAward_Exchange, CDlgBase)
AUI_ON_COMMAND("Rdo_0*", OnCommand_Rdo)
AUI_ON_COMMAND("Btn_Confirm", OnCommand_Confirm)
AUI_ON_COMMAND("Rdo_Coin0*", OnCommand_RdoCoin)
AUI_ON_COMMAND("Rdo_CoinType0*", OnCommand_RdoCoinType)
AUI_ON_COMMAND("Btn_GetCoin", OnCommand_Btn_GetCoin)
AUI_ON_COMMAND("Btn_Get", OnCommand_Btn_Get)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(DlgCrssvrTeamsAward_Exchange, CDlgBase)
AUI_ON_EVENT("Img_Award*",			WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_END_EVENT_MAP()
DlgCrssvrTeamsAward_Exchange::DlgCrssvrTeamsAward_Exchange()
{
	m_pConfig= NULL;
}
DlgCrssvrTeamsAward_Exchange::~DlgCrssvrTeamsAward_Exchange()
{
	A3DRELEASE(m_pSprite);
}


void DlgCrssvrTeamsAward_Exchange::OnShowDialog()
{
	CheckRadioButton(2,1);
	CheckRadioButton(1,0);
	CheckRadioButton(3,0);
	ClearData();

	bool isAward = false;
	const long t = cs_exchange_award_timestamp;
	tm *gtime = localtime((const time_t*)&t);
	tm dTime = *gtime;
	tm tmNow = GetGame()->GetServerLocalTime();

	if(cs_exchange_award_timestamp == 0)
	{
		isAward = true;
	}
	GetDlgItem("Btn_Get")->Enable(isAward);
}

bool DlgCrssvrTeamsAward_Exchange::OnInitDialog()
{
	m_pTxt_Money =  (AUILabel*)GetDlgItem("Txt_Gold");
	m_pTxt_Score =  (AUILabel*)GetDlgItem("Txt_Score");
	m_pTxt_Money->SetText(_AL(""));
	m_pTxt_Score->SetText(_AL(""));
	DATA_TYPE DataType;
	int idEssence = GetGame()->GetElementDataMan()->get_first_data_id(ID_SPACE_CONFIG, DataType);
	while(idEssence)
	{
		if(DataType == DT_CROSS6V6_ITEM_EXCHANGE_CONFIG)
			break;
		idEssence = GetGame()->GetElementDataMan()->get_next_data_id(ID_SPACE_CONFIG, DataType);
	}
	m_pConfig = (CROSS6V6_ITEM_EXCHANGE_CONFIG *)GetGame()->GetElementDataMan()->
		get_data_ptr(idEssence, ID_SPACE_CONFIG, DataType);
	AString strName;
	for(int nIndex = 0; nIndex < 15; nIndex++)
	{
		strName.Format("Img_Award%d", nIndex+1);
		m_pPicture[nIndex] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		m_pPicture[nIndex]->SetData(nIndex, "index");
		strName.Format("Txt_Money%d", nIndex+1);
		m_pTxt_ExMoney[nIndex] = (AUILabel*)GetDlgItem(strName);
		m_pTxt_ExMoney[nIndex]->SetText(_AL(""));
	}
	m_nHeight = 1;
	m_nLow = 1;
	m_iCurSel = -1;
	ACString szTemp;
	for(int nIndex = 0; nIndex < 15; nIndex++)
	{
		if(m_pConfig->cross6v6_item_exchange_info[nIndex].item_id != 0)
		{
			int nId = m_pConfig->cross6v6_item_exchange_info[nIndex].item_id;
			CECIvtrItem* pItem = CECIvtrItem::CreateItem(nId, 0,1);
			AString strFile;
			if (pItem )
			{
				pItem->SetNeedUpdate(false);
				pItem->GetDetailDataFromLocal();
				GetGameUIMan()->SetIvtrInfo(m_pPicture[nIndex], pItem,
					m_pConfig->cross6v6_item_exchange_info[nIndex].item_num);
				delete pItem;
			}
			strName.Format("%d",m_pConfig->cross6v6_item_exchange_info[nIndex].require_currnecy_num);
			m_pTxt_ExMoney[nIndex]->SetText(AS2AC(strName));
			if(m_pConfig->cross6v6_item_exchange_info[nIndex].require_currency_id ==  60460)
			{
				szTemp.Format(GetStringFromTable(1701),m_pConfig->cross6v6_item_exchange_info[nIndex].require_currnecy_num);
			}
			else if(m_pConfig->cross6v6_item_exchange_info[nIndex].require_currency_id ==  60461)
			{
				szTemp.Format(GetStringFromTable(1702),m_pConfig->cross6v6_item_exchange_info[nIndex].require_currnecy_num);
			}
			else if(m_pConfig->cross6v6_item_exchange_info[nIndex].require_currency_id ==  60462)
			{
				szTemp.Format(GetStringFromTable(1703),m_pConfig->cross6v6_item_exchange_info[nIndex].require_currnecy_num);
			}
			m_pTxt_ExMoney[nIndex]->SetHint(szTemp);
		}
	}
	m_pSprite = new A2DSprite;
	if( !m_pSprite ) return AUI_ReportError(__LINE__, __FILE__);

	bool bval = m_pSprite->Init(m_pA3DDevice, "icon\\Battle_Dungeon_6v6_SkillFocus.tga", 0);
	if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
	return true;
}

bool DlgCrssvrTeamsAward_Exchange::Release()
{
	A3DRELEASE(m_pSprite);
	return CDlgBase::Release();
}

void DlgCrssvrTeamsAward_Exchange::OnTick()
{
	int nNum = GetHostPlayer()->GetPack()->GetItemTotalNum(60460);
	ACString szTemp;
	szTemp.Format(_AL("%d"),nNum);
	m_pTxt_Money->SetText(szTemp);
	nNum = GetHostPlayer()->GetPack()->GetItemTotalNum(60461);
	szTemp.Format(_AL("%d"),nNum);
	GetDlgItem("Txt_Silver")->SetText(szTemp);
	nNum = GetHostPlayer()->GetPack()->GetItemTotalNum(60462);
	szTemp.Format(_AL("%d"),nNum);
	GetDlgItem("Txt_Bronze")->SetText(szTemp);
	if(GetDlgItem("Edt_Num"))
	{
		int nNum = a_atoi(GetDlgItem("Edt_Num")->GetText());
		if(nNum == 0)
		{
			GetDlgItem("Exchange_num")->SetText(_AL(""));
			return;
		}
		if(m_nHeight == 1)
		{
			int nGoldNum = GetHostPlayer()->GetPack()->GetItemTotalNum(60460);
			if(nGoldNum < nNum)
			{
				nNum = nGoldNum;
			}
			ACString szTemp;
			if(m_nLow == 1)
			{
				nNum = nNum * 10;
			}
			else if(m_nLow == 2)
			{
				nNum = nNum * 100;
			}
			szTemp.Format(_AL("%d"),nNum);
			GetDlgItem("Exchange_num")->SetText(szTemp);
		}
		else if(m_nHeight == 2)
		{
			int nYinNum = GetHostPlayer()->GetPack()->GetItemTotalNum(60461);
			if(nYinNum < nNum)
			{
				nNum = nYinNum;
			}
			ACString szTemp;
			if(m_nLow == 1)
			{
				nNum = nNum;
			}
			else if(m_nLow == 2)
			{
				nNum = nNum * 10;
			}
			szTemp.Format(_AL("%d"),nNum);
			GetDlgItem("Exchange_num")->SetText(szTemp);
		}
	}
}

void DlgCrssvrTeamsAward_Exchange::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	ClearData() ;
	m_iCurSel = pObj->GetData("index");
	m_pPicture[m_iCurSel]->SetCover(m_pSprite, 0, 1);
}

void DlgCrssvrTeamsAward_Exchange::OnCommand_Rdo(const char * szCommand)
{
	int nType = atoi(szCommand+5);
	DlgCrssvrTeamsMain* pTeamsMainDlg = (DlgCrssvrTeamsMain*)GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_GroupMain");
	pTeamsMainDlg->SetDialog(nType);
}
void DlgCrssvrTeamsAward_Exchange::OnCommand_Confirm(const char * szCommand)
{
	int nNum = 0;
	int nMoney = m_pConfig->cross6v6_item_exchange_info[m_iCurSel].require_currnecy_num;
	if(m_iCurSel == -1)
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(20050), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	if(m_pConfig->cross6v6_item_exchange_info[m_iCurSel].require_currency_id ==  60460)
	{
		nNum = GetHostPlayer()->GetPack()->GetItemTotalNum(60460);
	}
	else if(m_pConfig->cross6v6_item_exchange_info[m_iCurSel].require_currency_id ==  60461)
	{
		nNum = GetHostPlayer()->GetPack()->GetItemTotalNum(60461);
	}
	else if(m_pConfig->cross6v6_item_exchange_info[m_iCurSel].require_currency_id ==  60462)
	{
		nNum = GetHostPlayer()->GetPack()->GetItemTotalNum(60462);
	}
	if(nMoney > nNum)
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(20046), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	c2s_SendCmdCS_6v6_ExchangeAward(m_iCurSel,m_pConfig->cross6v6_item_exchange_info[m_iCurSel].item_id,m_pConfig->cross6v6_item_exchange_info[m_iCurSel].require_currency_id);;

}

void DlgCrssvrTeamsAward_Exchange::OnCommand_RdoCoin(const char * szCommand)
{
	if(stricmp(szCommand, "Rdo_Coin01") == 0 )
	{
		m_nHeight = 1;
		AUICheckBox* pItem = (AUICheckBox*)GetDlgItem("Rdo_Coin01");
		pItem->Check(true);
		pItem = (AUICheckBox*)GetDlgItem("Rdo_Coin02");
		pItem->Check(false);
	}
	else if(stricmp(szCommand, "Rdo_Coin02") == 0 )
	{
		m_nHeight = 2;
		AUICheckBox* pItem = (AUICheckBox*)GetDlgItem("Rdo_Coin01");
		pItem->Check(false);
		pItem = (AUICheckBox*)GetDlgItem("Rdo_Coin02");
		pItem->Check(true);
	}
}

void DlgCrssvrTeamsAward_Exchange::OnCommand_RdoCoinType(const char * szCommand)
{
	if(stricmp(szCommand, "Rdo_CoinType01") == 0 )
	{
		m_nLow = 1;
		AUICheckBox* pItem = (AUICheckBox*)GetDlgItem("Rdo_CoinType01");
		pItem->Check(true);
		pItem = (AUICheckBox*)GetDlgItem("Rdo_CoinType02");
		pItem->Check(false);
	}
	else if(stricmp(szCommand, "Rdo_CoinType02") == 0 )
	{
		m_nLow = 2;
		AUICheckBox* pItem = (AUICheckBox*)GetDlgItem("Rdo_CoinType01");
		pItem->Check(false);
		pItem = (AUICheckBox*)GetDlgItem("Rdo_CoinType02");
		pItem->Check(true);
	}
}

void DlgCrssvrTeamsAward_Exchange::OnCommand_Btn_GetCoin(const char * szCommand)
{
	int nType = 0;
	if(m_nHeight == 1 && m_nLow == 1)
	{
		nType = 0;
	}
	if(m_nHeight == 1 && m_nLow == 2)
	{
		nType = 1;
	}
	if(m_nHeight == 2 && m_nLow == 2)
	{
		nType = 2;
	}
	int nCount = a_atoi(GetDlgItem("Edt_Num")->GetText());
	c2s_SendCmdExchangeCs6v6Money(nType,nCount);
	GetDlgItem("Edt_Num")->SetText(_AL(""));
	GetDlgItem("Exchange_num")->SetText(_AL(""));
}

void DlgCrssvrTeamsAward_Exchange::OnCommand_Btn_Get(const char * szCommand)
{
	c2s_SendCmdCS_6v6Award(1,0);
}

void DlgCrssvrTeamsAward_Exchange::ClearData() 
{
	if (m_iCurSel>=0)
	{
		int i;
		for (i=0; i<15; ++i)
		{
			m_pPicture[i]->SetCover(0, 0, 1);
		}
	}
}

void DlgCrssvrTeamsAward_Exchange::SetUpDate(int nMaxScore)
{
	ACString szTemp;
	szTemp.Format(_AL("%d"),nMaxScore);
	m_pTxt_Score->SetText(szTemp);
}

int DlgCrssvrTeamsAward_Exchange::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="info")
	{
		S2C::collision_raid_info *p = (S2C::collision_raid_info*)wParam;
		cs_exchange_award_timestamp = p->cs_exchange_award_timestamp;
		bool isAward = false;

		if(cs_exchange_award_timestamp == 0)
		{
			isAward = true;
		}
		GetDlgItem("Btn_Get")->Enable(isAward);
	}
	return 0;
}


AUI_BEGIN_COMMAND_MAP(DlgCrssvrTeamsAward_Stage, CDlgBase)
AUI_ON_COMMAND("Rdo_0*", OnCommand_Rdo)
AUI_ON_COMMAND("Btn_Confirm", OnCommand_Confirm)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(DlgCrssvrTeamsAward_Stage, CDlgBase)
AUI_ON_EVENT("Img_Award*",			WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_END_EVENT_MAP()
DlgCrssvrTeamsAward_Stage::DlgCrssvrTeamsAward_Stage()
{
}
DlgCrssvrTeamsAward_Stage::~DlgCrssvrTeamsAward_Stage()
{
	A3DRELEASE(m_pSprite);
}

void DlgCrssvrTeamsAward_Stage::OnShowDialog()
{
	CheckRadioButton(3,1);
	CheckRadioButton(1,0);
	CheckRadioButton(2,0);
	ClearData();
	int nNum = GetHostPlayer()->GetPack()->GetItemTotalNum(60460);
	ACString szTemp;
	szTemp.Format(_AL("%d"),nNum);
	m_pTxt_Money->SetText(szTemp);
	nNum = GetHostPlayer()->GetPack()->GetItemTotalNum(60461);
	szTemp.Format(_AL("%d"),nNum);
	GetDlgItem("Txt_Silver")->SetText(szTemp);
	nNum = GetHostPlayer()->GetPack()->GetItemTotalNum(60462);
	szTemp.Format(_AL("%d"),nNum);
	GetDlgItem("Txt_Bronze")->SetText(szTemp);
	bool isAward = false;
	const long t = cs_team_award_timestamp;
	tm *gtime = localtime((const time_t*)&t);
	tm dTime = *gtime;
	tm tmNow = GetGame()->GetServerLocalTime();
	if(checkAwardTime(tmNow))
	{
		if(cs_team_award_timestamp == 0)
		{
			isAward = true;
		}
		else if(checkAwardType(dTime) != checkAwardType(tmNow))
		{
			isAward = true;
		}
	}
	if(m_nTeamPos == -1)
	{
		isAward = false;
	}
	GetDlgItem("Btn_Confirm")->Enable(isAward);
}


bool DlgCrssvrTeamsAward_Stage::OnInitDialog()
{
	m_pTxt_Money =  (AUILabel*)GetDlgItem("Txt_Gold");
	m_pTxt_Score =  (AUILabel*)GetDlgItem("Txt_Score");
	m_pTxt_Money->SetText(_AL(""));
	m_pTxt_Score->SetText(_AL(""));
	DATA_TYPE DataType;
	int idEssence = GetGame()->GetElementDataMan()->get_first_data_id(ID_SPACE_CONFIG, DataType);
	while(idEssence)
	{
		if(DataType == DT_TEAM_SEASON_AWARD_CONFIG)
			break;
		idEssence = GetGame()->GetElementDataMan()->get_next_data_id(ID_SPACE_CONFIG, DataType);
	}
	TEAM_SEASON_AWARD_CONFIG* m_pConfig = (TEAM_SEASON_AWARD_CONFIG *)GetGame()->GetElementDataMan()->
		get_data_ptr(idEssence, ID_SPACE_CONFIG, DataType);
	AString strName;
	for(int nIndex = 0; nIndex < 21; nIndex++)
	{
		strName.Format("Img_Award%d", nIndex+1);
		m_pPicture[nIndex] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		m_pPicture[nIndex]->SetData(nIndex, "index");
	}
	m_iCurSel = -1;
	for(int nIndex = 0; nIndex < 15; nIndex++)
	{
		if(m_pConfig->team_season_awrd_info[nIndex].inception_rank != 0)
		{
			for(int nItemIndex = 0; nItemIndex < 3; nItemIndex++)
			{
				if(m_pConfig->team_season_awrd_info[nIndex].item_info[nItemIndex].item_id != 0)
				{
					int nId = m_pConfig->team_season_awrd_info[nIndex].item_info[nItemIndex].item_id;
					CECIvtrItem* pItem = CECIvtrItem::CreateItem(nId, 0,1);
					AString strFile;
					if (pItem )
					{
						pItem->SetNeedUpdate(false);
						pItem->GetDetailDataFromLocal();
						GetGameUIMan()->SetIvtrInfo(m_pPicture[nItemIndex + nIndex* 3], pItem,
							m_pConfig->team_season_awrd_info[nIndex].item_info[nItemIndex].item_num);
						delete pItem;
					}
					m_pPicture[nItemIndex + nIndex* 3]->SetDataPtr((void*)m_pConfig->team_season_awrd_info[nIndex].item_info[nItemIndex].item_id);
				}
			}
		}

	}

	m_pSprite = new A2DSprite;
	if( !m_pSprite ) return AUI_ReportError(__LINE__, __FILE__);

	bool bval = m_pSprite->Init(m_pA3DDevice, "icon\\Battle_Dungeon_6v6_SkillFocus.tga", 0);
	if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
	return true;
}

bool DlgCrssvrTeamsAward_Stage::Release()
{
	A3DRELEASE(m_pSprite);
	return CDlgBase::Release();
}

void DlgCrssvrTeamsAward_Stage::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	ClearData() ;
	m_iCurSel = pObj->GetData("index");
	if((int)m_pPicture[m_iCurSel]->GetDataPtr() > 0)
	{
		m_pPicture[m_iCurSel]->SetCover(m_pSprite, 0, 1);
	}
}

void DlgCrssvrTeamsAward_Stage::OnCommand_Rdo(const char * szCommand)
{
	int nType = atoi(szCommand+5);
	DlgCrssvrTeamsMain* pTeamsMainDlg = (DlgCrssvrTeamsMain*)GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_GroupMain");
	pTeamsMainDlg->SetDialog(nType);
}
void DlgCrssvrTeamsAward_Stage::OnCommand_Confirm(const char * szCommand)
{
	tm tmNow = GetGame()->GetServerLocalTime();
	if (checkAwardTime(tmNow))
	{
		c2s_SendCmdCS_6v6Award(2,0);
	}
	else
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(20008), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	}
}

void DlgCrssvrTeamsAward_Stage::ClearData() 
{
	if (m_iCurSel>=0)
	{
		int i;
		for (i=0; i<21; ++i)
		{
			m_pPicture[i]->SetCover(0, 0, 1);
		}
	}
}

void DlgCrssvrTeamsAward_Stage::SetUpDate(int nMaxScore)
{
	ACString szTemp;
	szTemp.Format(_AL("%d"),nMaxScore);
	m_pTxt_Score->SetText(szTemp);
}

void DlgCrssvrTeamsAward_Stage::SetTeamPos(int team_pos)
{
	m_nTeamPos = team_pos;
}

int DlgCrssvrTeamsAward_Stage::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="info")
	{
		S2C::collision_raid_info *p = (S2C::collision_raid_info*)wParam;
		cs_team_award_timestamp =p->cs_team_award_timestamp;
		bool isAward = false;
		const long t = cs_team_award_timestamp;
		tm *gtime = localtime((const time_t*)&t);
		tm dTime = *gtime;
		tm tmNow = GetGame()->GetServerLocalTime();
		if(checkAwardTime(tmNow))
		{
			if(cs_team_award_timestamp == 0)
			{
				isAward = true;
			}
			else if(checkAwardType(dTime) != checkAwardType(tmNow))
			{
				isAward = true;
			}
		}
		if(m_nTeamPos == -1)
		{
			isAward = false;
		}
		GetDlgItem("Btn_Confirm")->Enable(isAward);
	}
	return 0;
}