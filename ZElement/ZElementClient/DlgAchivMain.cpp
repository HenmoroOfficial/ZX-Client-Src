// Filename	: DlgAchivMain.cpp
// Creator	: Fu Chonggang
// Date		: 2010/1/27

#include "AUI\\AUICTranslate.h"
#include "AUI\\AUISubDialog.h"
#include "AFI.h"
#include "AUI\\AUIDef.h"
#include "AFileImage.h"
#include "DlgAchivMain.h"
#include "DlgAchivItemMan.h"
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

AUI_BEGIN_COMMAND_MAP(CDlgAchivMain, CDlgBase)

AUI_ON_COMMAND("Rdo_MainType*",		OnCommandRdoMain)
AUI_ON_COMMAND("Rdo_SubType*",		OnCommandRdoSub)
AUI_ON_COMMAND("IDCANCEL",			OnCommandCancel)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgAchivMain, CDlgBase)

AUI_END_EVENT_MAP()

CDlgAchivMain::CDlgAchivMain()
{
	m_iTarget		= 0;

	m_pLab_Point = NULL;
	m_pLab_DoneNum = NULL;
	int i;
	for( i=0; i<ACHIEVEMENT_TYPE_MAIN; i++)
	{
		m_pRdo_Main[i]	= NULL;
		m_pPrg_Main[i]	= NULL;
		m_pLab_Main[i]	= NULL;
		m_pImg_Main[i]	= NULL;
		m_pGA_Main[i]	= NULL;
	}
	for(i=0; i<ACHIEVEMENT_TYPE_SUB; i++)
		m_pRdo_Sub[i]	= NULL;
}

CDlgAchivMain::~CDlgAchivMain()
{
}

void CDlgAchivMain::OnCommandRdoMain(const char* szCommand)
{
	PAUIOBJECT pObj = GetDlgItem(szCommand);
	CECAchievementMan* pMan = m_pPlayer->GetAchievementMan();
	const sAchievementClass* aClass = pMan->GetAchievementClass(pObj->GetData());
	UINT i;
	for (i=0; i<aClass->vecSubClass.size(); i++)
	{
		if (i>=ACHIEVEMENT_TYPE_SUB)
		{
			break;
		}
		m_pRdo_Sub[i]->SetText(aClass->vecSubClass[i].name);
		m_pRdo_Sub[i]->SetData(aClass->vecSubClass[i].id);
		m_pRdo_Sub[i]->Show(true);
	}
	for (;i<ACHIEVEMENT_TYPE_SUB; i++)
	{
		m_pRdo_Sub[i]->SetData(0);
		m_pRdo_Sub[i]->Show(false);
	}
	CheckRadioButton(2, 1);
	OnCommandRdoSub("Rdo_SubType1");
	DynamicAction(GetCheckedRadioButton(1)-1);
}

void CDlgAchivMain::OnCommandRdoSub(const char* szCommand)
{
	UINT uMain = GetCheckedRadioButton(1);
	UINT uSub = GetCheckedRadioButton(2);
	CDlgAchivItemMan *pDlgAchivMain = (CDlgAchivItemMan*)GetGameUIMan()->GetDialog("Win_Achievement_Sub");
	abase::vector<sAchievement>& vecAchiv = pDlgAchivMain->GetAchivs();
	vecAchiv.clear();
	CECAchievementMan* pMan = m_pPlayer->GetAchievementMan();
	abase::hash_map<unsigned int, sAchievement>& achivSet = pMan->GetAchievementSet();
	abase::hash_map<unsigned int,sAchievement>::iterator iter;
	for ( iter = achivSet.begin(); iter!=achivSet.end(); ++iter)
	{
		sAchievement& achievement = iter->second;
		if (achievement.iClass==uMain && achievement.iSubClass==uSub)
		{
			vecAchiv.push_back(achievement);
		}
	}
	pDlgAchivMain->InitItemDlgs();
}

void CDlgAchivMain::OnCommandCancel(const char* szCommand)
{
	m_iTarget = 0;
	m_pPlayer = GetHostPlayer();
	Show(false);
}

void CDlgAchivMain::SetTarget(int iTarget) 
{
	//下次tick更新数值
	if (0==iTarget)
	{
		m_pPlayer = GetHostPlayer();
		UpdateInfo();
	}
	else
		m_iTarget = iTarget;
}

bool CDlgAchivMain::OnInitDialog()
{
	char szText[30];
	int i;
	CheckRadioButton(1,1);
	CheckRadioButton(2,1);
	DDX_Control("Txt_SuccessPoint", m_pLab_Point);
	DDX_Control("Txt_DoneNum", m_pLab_DoneNum);
	for (i=0; i<ACHIEVEMENT_TYPE_MAIN; i++)
	{
		sprintf(szText,"Rdo_MainType%d",i+1);
		DDX_Control(szText, m_pRdo_Main[i]);
		sprintf(szText,"Prgs_MainType%d",i+1);
		DDX_Control(szText, m_pPrg_Main[i]);
		sprintf(szText,"Lab_%d",i+1);
		DDX_Control(szText, m_pLab_Main[i]);
		sprintf(szText,"Img_%d",i+1);
		DDX_Control(szText, m_pImg_Main[i]);
		sprintf(szText,"Rdo_MainType_GA%d",i+1);
		DDX_Control(szText, m_pGA_Main[i]);
	}
	for (i=0; i<ACHIEVEMENT_TYPE_SUB; i++)
	{
		sprintf(szText,"Rdo_SubType%d",i+1);
		DDX_Control(szText, m_pRdo_Sub[i]);
	}
	m_pPlayer = GetHostPlayer();
	CECAchievementMan* pMan = m_pPlayer->GetAchievementMan();
	abase::hash_map<unsigned int, sAchievementClass>& aClasses = pMan->GetAchievementClassSet();
	abase::hash_map<unsigned int, sAchievementClass>::iterator it = aClasses.begin();
	i = 0;
	while(it != aClasses.end())
	{
		sAchievementClass& aClass = it->second;
		m_pLab_Main[i]->SetText(aClass.name);
		m_pRdo_Main[i]->SetData(aClass.id);
		++i;
		++it;
		if (i>=ACHIEVEMENT_TYPE_MAIN)
			break;
	}
	
	m_nMovIndex = -1;
	m_nCurIndex = 0;
	CalcTargetPos();
	return true;
}

void CDlgAchivMain::CalcTargetPos()
{
	int i;
	AUIOBJECT_SETPROPERTY p;
	A3DRECT rcShape = GetDlgItem("Shape2")->GetRect();
	POINT ptShape = GetDlgItem("Shape2")->GetPos(true);
	int nHeightMain = m_pRdo_Main[0]->GetSize().cy;
	int nPosPrg = m_pPrg_Main[0]->GetPos(true).y - m_pRdo_Main[0]->GetPos(true).y;
	int nPosLab = m_pLab_Main[0]->GetPos(true).y - m_pRdo_Main[0]->GetPos(true).y;
	int nPosImg = m_pImg_Main[0]->GetPos(true).y - m_pRdo_Main[0]->GetPos(true).y;
	int nHeightSub = m_pRdo_Sub[0]->GetSize().cy;
	int nPosGa = m_pGA_Main[0]->GetPos(true).y - m_pRdo_Main[0]->GetPos(true).y;

	int xMain = ptShape.x + (rcShape.Width() - m_pRdo_Main[0]->GetSize().cx)/2;
	int xSub = ptShape.x + (rcShape.Width() - m_pRdo_Sub[0]->GetSize().cx)/2;
	int xLab = ptShape.x + rcShape.Width()/2;
	int xPrg = ptShape.x + (rcShape.Width() - m_pPrg_Main[0]->GetSize().cx)/2;
	int y = ptShape.y + (m_pRdo_Main[0]->GetPos(true).y - ptShape.y);
	for (i=1; i<=m_nCurIndex; i++)
	{
		p.i = y + i*nHeightMain;
		m_pRdo_Main[i]->SetProperty("Y", &p);
		p.i = y + i*nHeightMain + nPosImg;
		m_pImg_Main[i]->SetProperty("Y", &p);
		p.i = y + i*nHeightMain + nPosPrg;
		m_pPrg_Main[i]->SetProperty("Y", &p);
		p.i = y + i*nHeightMain + nPosLab;
		m_pLab_Main[i]->SetProperty("Y", &p);
		p.i = y + i*nHeightMain + nPosGa;
		m_pGA_Main[i]->SetProperty("Y", &p);
	}
	y = ptShape.y + rcShape.Height() - (m_pRdo_Main[0]->GetPos(true).y - ptShape.y);
	for (i=ACHIEVEMENT_TYPE_MAIN-1; i>m_nCurIndex; i--)
	{
		y -= nHeightMain;
		p.i = y;
		m_pRdo_Main[i]->SetProperty("Y", &p);
		p.i = y + nPosImg;
		m_pImg_Main[i]->SetProperty("Y", &p);
		p.i = y + nPosPrg;
		m_pPrg_Main[i]->SetProperty("Y", &p);
		p.i = y + nPosLab;
		m_pLab_Main[i]->SetProperty("Y", &p);
		p.i = y + nPosGa;
		m_pGA_Main[i]->SetProperty("Y", &p);
	}
	y = ptShape.y + (m_pRdo_Main[0]->GetPos(true).y - ptShape.y) + (m_nCurIndex+1)*nHeightMain;
	for (i=0; i<ACHIEVEMENT_TYPE_SUB; i++)
	{
		p.i = y;
		m_pRdo_Sub[i]->SetProperty("Y", &p);
		y += nHeightSub;
	}
}

void CDlgAchivMain::OnTick()
{
	if (m_iTarget>0)
	{
		CECPlayerMan *pPlayerMan = GetWorld()->GetPlayerMan();
		CECPlayer *pPlayer = (CECPlayer *)pPlayerMan->GetPlayer(m_iTarget);
		if (pPlayer)
		{
			if(m_pPlayer!=pPlayer)
			{
				m_pPlayer = pPlayer;
				UpdateInfo();
			}
		}
		else
		{
			OnCommandCancel("");
			return;
		}
	}
	else if (!m_pPlayer)
	{
		m_pPlayer = GetHostPlayer();
		UpdateInfo();
	}
	CECAchievementMan* pMan = m_pPlayer->GetAchievementMan();
	ACString strText;
	strText.Format(_AL("%d"),pMan->GetAchievementPoint());
	m_pLab_Point->SetText(strText);

	UINT i, iTotal = 0;
	int iFinished = 0;
	
	abase::hash_map<unsigned int, sAchievement>& achivSet = pMan->GetAchievementSet();
	abase::hash_map<unsigned int,sAchievement>::iterator iter;
	for (i=0; i<ACHIEVEMENT_TYPE_MAIN; i++)
	{
		iTotal = 0;
		iFinished = 0;
		for ( iter = achivSet.begin(); iter!=achivSet.end(); ++iter)
		{
			sAchievement& achievement = iter->second;
			if (achievement.iClass==(i+1))
			{
				iTotal++;
				if (achievement.bFinish)
					iFinished++;
			}
		}
		strText.Format(_AL("%d/%d"), iFinished, iTotal);
		m_pPrg_Main[i]->SetProgress(int(double(iFinished) / iTotal * AUIPROGRESS_MAX));
		m_pPrg_Main[i]->SetHint(strText);
		m_pRdo_Main[i]->SetHint(strText);
	}
	//防止rdomain按钮点击后遮挡进度条按钮
	if (GetFocus() && GetFocus()->GetType() == AUIOBJECT_TYPE_RADIOBUTTON)
	{
		ChangeFocus(NULL);
	}
	DynamicAction(-1);
	if (GetAsyncKeyState('7')&0x8000)
	{
		CDlgAchivItemMan *pDlgAchivMain = (CDlgAchivItemMan*)GetGameUIMan()->GetDialog("Win_Achievement_Sub");
		pDlgAchivMain->ShowAchivItem(7);
	}
}

const int DYNAMIC_SPEED = 1000;
void CDlgAchivMain::DynamicAction(int index)
{
	// 函数调用rdomain，非点击调用
	if (index == m_nCurIndex)
		return;
	// 非操作过程
	if (m_nMovIndex == -1 && index == -1)
		return;
	
	AUIOBJECT_SETPROPERTY p;
	A3DRECT rcShape = GetDlgItem("Shape2")->GetRect();
	POINT ptShape = GetDlgItem("Shape2")->GetPos(true);
	int nHeightMain = m_pRdo_Main[0]->GetSize().cy;
	int nPosPrg = m_pPrg_Main[0]->GetPos(true).y - m_pRdo_Main[0]->GetPos(true).y;
	int nPosLab = m_pLab_Main[0]->GetPos(true).y - m_pRdo_Main[0]->GetPos(true).y;
	int nPosImg = m_pImg_Main[0]->GetPos(true).y - m_pRdo_Main[0]->GetPos(true).y;
	int nPosGa = m_pGA_Main[0]->GetPos(true).y - m_pRdo_Main[0]->GetPos(true).y;
	int nHeightSub = m_pRdo_Sub[0]->GetSize().cy;

	static DWORD dwTimeLast = GetTickCount();
	DWORD dwTimeNow = GetTickCount();
	int dwTime = dwTimeNow - dwTimeLast;
	dwTimeLast = dwTimeNow;
	int xLab = ptShape.x + rcShape.Width()/2;

	if (index == -1)	// tick
	{
		int deltaY = dwTime * DYNAMIC_SPEED / 1000;
		int x = m_pRdo_Main[0]->GetPos(true).x;
		int y = m_pRdo_Main[m_nMovIndex]->GetPos(true).y;
		bool bOk = false;
		if (m_nMovIndex > m_nCurIndex)
		{
			y -= deltaY;
			if (y<=m_nTargetPosY)
			{
				bOk = true;
				y = m_nTargetPosY;
			}
		}
		else
		{
			y += deltaY;
			if (y>=m_nTargetPosY)
			{
				bOk = true;
				y = m_nTargetPosY;
			}
		}
		
		if (m_nMovIndex > m_nCurIndex)
		{
			for (int i=m_nMovIndex; i>m_nCurIndex; i--)
			{
				p.i = y;
				m_pRdo_Main[i]->SetProperty("Y", &p);
				p.i = y+nPosImg;
				m_pImg_Main[i]->SetProperty("Y", &p);
				p.i = y+nPosPrg;
				m_pPrg_Main[i]->SetProperty("Y", &p);
				p.i = y+nPosLab;
				m_pLab_Main[i]->SetProperty("Y", &p);
				p.i = y+nPosGa;
				m_pGA_Main[i]->SetProperty("Y", &p);
				y -= nHeightMain;
			}
		}
		else
		{
			for (int i=m_nMovIndex; i<=m_nCurIndex; i++)
			{
				p.i = y;
				m_pRdo_Main[i]->SetProperty("Y", &p);
				p.i = y+nPosImg;
				m_pImg_Main[i]->SetProperty("Y", &p);
				p.i = y+nPosPrg;
				m_pPrg_Main[i]->SetProperty("Y", &p);
				p.i = y+nPosLab;
				m_pLab_Main[i]->SetProperty("Y", &p);
				p.i = y+nPosGa;
				m_pLab_Main[i]->SetProperty("Y", &p);
				y += nHeightMain;
			}
		}

		if (bOk)
		{
			m_nMovIndex = -1;
			m_nCurIndex = GetCheckedRadioButton(1)-1;
			for (int i=0; i<ACHIEVEMENT_TYPE_SUB; i++)
			{
				if (m_pRdo_Sub[i]->GetData()>0)
					m_pRdo_Sub[i]->Show(true);
			}
			CalcTargetPos();
		}
	}
	else //点击
	{
		if (index>m_nCurIndex)
		{
			m_nMovIndex = index;
			m_nTargetPosY = ptShape.y+nHeightMain*m_nMovIndex+(m_pRdo_Main[0]->GetPos(true).y - ptShape.y);
		}
		else if(index<m_nCurIndex)
		{
			m_nMovIndex = index+1;
			m_nTargetPosY = ptShape.y+rcShape.Height()-nHeightMain*(ACHIEVEMENT_TYPE_MAIN-m_nMovIndex)-(m_pRdo_Main[0]->GetPos(true).y - ptShape.y);
		}
		for (int i=0; i<ACHIEVEMENT_TYPE_SUB; i++)
		{
			m_pRdo_Sub[i]->Show(false);
		}
	}
}

void CDlgAchivMain::OnShowDialog()
{
	UpdateInfo();
}

void CDlgAchivMain::UpdateInfo()
{
	int index = GetCheckedRadioButton(1);
	if (index<1)
	{
		index = 1;
		CheckRadioButton(1,1);
		m_nCurIndex = 0;
	}
	char szText[20];
	sprintf(szText,"Rdo_MainType%d",index);
	OnCommandRdoMain(szText);

	CECAchievementMan* pMan = m_pPlayer->GetAchievementMan();
	abase::hash_map<unsigned int, sAchievementClass>& aClasses = pMan->GetAchievementClassSet();
	abase::hash_map<unsigned int, sAchievementClass>::iterator it = aClasses.begin();
	int i = 0;
	int awardNum = 0;
	while(it != aClasses.end())
	{
		sAchievementClass& aClass = it->second;
		DWORD j;
		bool hasAward = false;
		for (j = 0; j < aClass.vecSubClass.size(); ++ j)
		{
			if (aClass.vecSubClass[j].awardNum == 0)
				continue;

			awardNum += aClass.vecSubClass[j].awardNum;
			hasAward = true;
		}

		m_pGA_Main[i]->Show(hasAward);

		++i;
		++it;
		if (i>=ACHIEVEMENT_TYPE_MAIN)
			break;
	}

	if (awardNum == 0)
	{
		m_pLab_DoneNum->Show(false);
	}
	else
	{
		ACString strText;
		strText.Format(_AL("%d"),awardNum);
		m_pLab_DoneNum->SetText(strText);
		m_pLab_DoneNum->Show(true);
	}
}

void CDlgAchivMain::ShowAchiv(sAchievement* pAchiv)
{
	char szText[20];
	UINT nMain = pAchiv->iClass;
	UINT nSub = pAchiv->iSubClass;
	CheckRadioButton(1, nMain);
	sprintf(szText,"Rdo_MainType%d", nMain);
	OnCommandRdoMain(szText);
	CheckRadioButton(2, nSub);
	sprintf(szText,"Rdo_SubType%d", nSub);
	OnCommandRdoSub(szText);

	CECAchievementMan* pMan = m_pPlayer->GetAchievementMan();
	abase::hash_map<unsigned int, sAchievement>& achivSet = pMan->GetAchievementSet();
	abase::hash_map<unsigned int,sAchievement>::iterator iter;
	int nIndex = 0;
	for ( iter = achivSet.begin(); iter!=achivSet.end(); ++iter)
	{
		sAchievement& achievement = iter->second;
		if (achievement.iClass==nMain && achievement.iSubClass==nSub)
		{
			if (achievement.id == pAchiv->id)
				break;
			nIndex++;
		}
	}
	CDlgAchivItemMan *pDlgAchivMain = (CDlgAchivItemMan*)GetGameUIMan()->GetDialog("Win_Achievement_Sub");
	pDlgAchivMain->ShowAchivItem(nIndex);
}