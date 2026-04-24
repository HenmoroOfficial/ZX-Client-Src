// Filename	: DlgPetFlyAttrUp.cpp
// Creator	: Fu Chonggang
// Date		: 2013.8.16

#include "AUI\\AUIProgress.h"
#include "AUI\\AUILabel.h"
#include "DlgPetFlyAttrUp.h"
#include "DlgPetCombine.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_IvtrConsume.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgPetFlyAttrUp, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",			OnCommandMainAttr)
AUI_ON_COMMAND("Btn_Return",		OnCommandMainAttr)
AUI_ON_COMMAND("Btn_Add*",			OnCommandFlyAttrUp)

AUI_END_COMMAND_MAP()


// 第一个值设置为0为了计算进度条方便，下面代码使用
// int iProgress = AUIPROGRESS_MAX*(pPet->GetEssence().reborn_prop_add[i]-ms_iLvUpNum[i][j-1])/(ms_iLvUpNum[i][j]-ms_iLvUpNum[i][j-1]);
int CDlgPetFlyAttrUp::ms_iLvUpNum[14][11] =
{
	{ 0, 6000, 12000, 18000, 24000, 30000, 36000, 42000, 48000, 54000, 60000}, // 生命
	{ 0, 6000, 12000, 18000, 24000, 30000, 36000, 42000, 48000, 54000, 60000},// 精力
	{ 0, 500, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500, 5000},// 小功
	{ 0, 600, 1200, 1800, 2400, 3000, 3600, 4200, 4800, 5400, 6000},// 大功
	{ 0, 200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000},// 防御
	{ 0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500},// 命中
	{ 0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500},// 闪躲
	{ 0, 60, 120, 180, 240, 300, 360, 420, 480, 540, 600},// 暴击率--千分制
	{ 0, 180, 360, 540, 720, 900, 1080, 1260, 1440, 1620, 1800},// 暴击伤害---千分制
	{ 0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500},// 眩晕抗性
	{ 0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500},// 魅惑抗性
	{ 0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500},// 虚弱抗性
	{ 0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500},// 昏睡抗性
	{ 0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500},// 定身抗性
};


DWORD CDlgPetFlyAttrUp::ms_dwColor[10] =
{
	0xFFFFFFFF,
	0xFFffcb4a,
	0xFF00FF00,
	0xFF0000FF,
	0xFFffff00,
	0xFFff00ff,
	0xFFFF0000,
	0xFFB00000,
	0xFFFF00B0,
	0xFFff2222,
};

CDlgPetFlyAttrUp::CDlgPetFlyAttrUp()
{
}

CDlgPetFlyAttrUp::~CDlgPetFlyAttrUp()
{
}

bool CDlgPetFlyAttrUp::OnInitDialog()
{
	m_pLabAttr[0] = (PAUILABEL)GetDlgItem("Txt_Hp");
	m_pLabAttr[1] = (PAUILABEL)GetDlgItem("Txt_Mp");
	m_pLabAttr[2] = (PAUILABEL)GetDlgItem("Txt_MinAttack");
	m_pLabAttr[3] = (PAUILABEL)GetDlgItem("Txt_MaxAttack");
	m_pLabAttr[4] = (PAUILABEL)GetDlgItem("Txt_Defence");
	m_pLabAttr[5] = (PAUILABEL)GetDlgItem("Txt_Definition");
	m_pLabAttr[6] = (PAUILABEL)GetDlgItem("Txt_Evade");
	m_pLabAttr[7] = (PAUILABEL)GetDlgItem("Txt_CriticalRate");
	m_pLabAttr[8] = (PAUILABEL)GetDlgItem("Txt_CriticalDamage");
	m_pLabAttr[9] = (PAUILABEL)GetDlgItem("Txt_StunRes");
	m_pLabAttr[10] = (PAUILABEL)GetDlgItem("Txt_WeaknessRes");
	m_pLabAttr[11] = (PAUILABEL)GetDlgItem("Txt_EnlaceRes");
	m_pLabAttr[12] = (PAUILABEL)GetDlgItem("Txt_SilenceRes");
	m_pLabAttr[13] = (PAUILABEL)GetDlgItem("Txt_SleepRes");

	m_pLabAttrPrg[0] = (PAUILABEL)GetDlgItem("Txt_TalentHp");
	m_pLabAttrPrg[1] = (PAUILABEL)GetDlgItem("Txt_TalentMp");
	m_pLabAttrPrg[2] = (PAUILABEL)GetDlgItem("Txt_TalentMinAttack");
	m_pLabAttrPrg[3] = (PAUILABEL)GetDlgItem("Txt_TalentMaxAttack");
	m_pLabAttrPrg[4] = (PAUILABEL)GetDlgItem("Txt_TalentDefence");
	m_pLabAttrPrg[5] = (PAUILABEL)GetDlgItem("Txt_TalentDefinition");
	m_pLabAttrPrg[6] = (PAUILABEL)GetDlgItem("Txt_TalentEvade");
	m_pLabAttrPrg[7] = (PAUILABEL)GetDlgItem("Txt_PrgsCriticalRate");
	m_pLabAttrPrg[8] = (PAUILABEL)GetDlgItem("Txt_PrgsCriticalDamage");
	m_pLabAttrPrg[9] = (PAUILABEL)GetDlgItem("Txt_TalentStunRes");
	m_pLabAttrPrg[10] = (PAUILABEL)GetDlgItem("Txt_TalentWeaknessRes");
	m_pLabAttrPrg[11] = (PAUILABEL)GetDlgItem("Txt_TalentEnlaceRes");
	m_pLabAttrPrg[12] = (PAUILABEL)GetDlgItem("Txt_TalentSilenceRes");
	m_pLabAttrPrg[13] = (PAUILABEL)GetDlgItem("Txt_TalentSleepRes");

	m_pPrgAttr[0] = (PAUIPROGRESS)GetDlgItem("Prgs_Hp");
	m_pPrgAttr[1] = (PAUIPROGRESS)GetDlgItem("Prgs_Mp");
	m_pPrgAttr[2] = (PAUIPROGRESS)GetDlgItem("Prgs_MinAttack");
	m_pPrgAttr[3] = (PAUIPROGRESS)GetDlgItem("Prgs_MaxAttack");
	m_pPrgAttr[4] = (PAUIPROGRESS)GetDlgItem("Prgs_Defence");
	m_pPrgAttr[5] = (PAUIPROGRESS)GetDlgItem("Prgs_Definition");
	m_pPrgAttr[6] = (PAUIPROGRESS)GetDlgItem("Prgs_Evade");
	m_pPrgAttr[7] = (PAUIPROGRESS)GetDlgItem("Prgs_CriticalRate");
	m_pPrgAttr[8] = (PAUIPROGRESS)GetDlgItem("Prgs_CriticalDamage");
	m_pPrgAttr[9] = (PAUIPROGRESS)GetDlgItem("Prgs_StunRes");
	m_pPrgAttr[10] = (PAUIPROGRESS)GetDlgItem("Prgs_WeaknessRes");
	m_pPrgAttr[11] = (PAUIPROGRESS)GetDlgItem("Prgs_EnlaceRes");
	m_pPrgAttr[12] = (PAUIPROGRESS)GetDlgItem("Prgs_SilenceRes");
	m_pPrgAttr[13] = (PAUIPROGRESS)GetDlgItem("Prgs_SleepRes");

	CheckRadioButton(2, 1);
	return true;
}

void CDlgPetFlyAttrUp::OnTick()
{
	int iPetIndex = GetData("pet_index");
	CECPetBedge *pPet = (CECPetBedge*)GetHostPlayer()->GetPack()->GetItem(iPetIndex);
	if (!pPet)
		return;

	ACString strText;
	int i;
	// 处理所有整数类型参数显示
	for ( i=0; i<PET_FLY_ATTR_NUM; ++i )
	{
		if ( i!=7 && i!=8 )
		{
			strText.Format(_AL("%d"), (int)pPet->GetEssence().reborn_prop_add[i]);
			m_pLabAttr[i]->SetText(strText);
			int j;
			for (j=1; j<=10; ++j)
			{
				if (pPet->GetEssence().reborn_prop_add[i]<ms_iLvUpNum[i][j])
				{
					m_pPrgAttr[i]->SetColor(ms_dwColor[j-1]);
					int iProgress = AUIPROGRESS_MAX*(pPet->GetEssence().reborn_prop_add[i]-ms_iLvUpNum[i][j-1])/(ms_iLvUpNum[i][j]-ms_iLvUpNum[i][j-1]);
					m_pPrgAttr[i]->SetProgress(iProgress);
					strText.Format(GetGameUIMan()->GetStringFromTable(3630), j, iProgress);
					m_pPrgAttr[i]->SetHint(strText);
					strText.Format(_AL("LV%d"), j);
					m_pLabAttrPrg[i]->SetText(strText);
					break;
				}
			}
			if (j==11)
			{
				m_pPrgAttr[i]->SetColor(ms_dwColor[9]);
				m_pPrgAttr[i]->SetProgress(AUIPROGRESS_MAX);
				strText.Format(GetGameUIMan()->GetStringFromTable(3630), 10, AUIPROGRESS_MAX);
				m_pPrgAttr[i]->SetHint(strText);
				strText.Format(_AL("LV%d"), 10);
				m_pLabAttrPrg[i]->SetText(strText);
			}
		}
	}
	// 处理所有浮点数参数类型显示
	for (i=7; i<=8; ++i)
	{
		strText.Format(_AL("%.0f%%"), pPet->GetEssence().reborn_prop_add[i]/10.0f);
		m_pLabAttr[i]->SetText(strText);
		int j;
		for (j=1; j<=10; ++j)
		{
			if (pPet->GetEssence().reborn_prop_add[i]<ms_iLvUpNum[i][j])
			{
				m_pPrgAttr[i]->SetColor(ms_dwColor[j-1]);
				int iProgress = AUIPROGRESS_MAX*(pPet->GetEssence().reborn_prop_add[i]-ms_iLvUpNum[i][j-1])/(ms_iLvUpNum[i][j]-ms_iLvUpNum[i][j-1]);
				m_pPrgAttr[i]->SetProgress(iProgress);
				strText.Format(GetGameUIMan()->GetStringFromTable(3630), j, iProgress);
				m_pPrgAttr[i]->SetHint(strText);
				strText.Format(_AL("LV%d"), j);
				m_pLabAttrPrg[i]->SetText(strText);
				break;
			}
		}
		if (j==11)
		{
			m_pPrgAttr[i]->SetColor(ms_dwColor[9]);
			m_pPrgAttr[i]->SetProgress(AUIPROGRESS_MAX);
			strText.Format(GetGameUIMan()->GetStringFromTable(3630), 10, AUIPROGRESS_MAX);
			m_pPrgAttr[i]->SetHint(strText);
			strText.Format(_AL("LV%d"), 10);
			m_pLabAttrPrg[i]->SetText(strText);
		}
	}
}

void CDlgPetFlyAttrUp::OnCommandMainAttr(const char * szCommand)
{
	Show(false);
	GetGameUIMan()->GetDialog("Win_Pet_Detail")->Show(true);
}

void CDlgPetFlyAttrUp::OnCommandFlyAttrUp(const char * szCommand)
{
	int iAttrIndex = atoi(szCommand+strlen("Btn_Add"));
	int iPetIndex = GetData("pet_index");
	int iNum = 1;
	if (GetCheckedRadioButton(2)==2)
		iNum = 10;
	else if (GetCheckedRadioButton(2)==3)
		iNum = 100;
	int iPocketNum = GetHostPlayer()->GetPack()->GetItemTotalNum(CDlgPetCombineZiZhi::GetXiandouId());
	if(iPocketNum<iNum)
		GetGameUIMan()->AddChatMessage(GetGameUIMan()->GetStringFromTable(3669), GP_CHAT_MISC);
	else
		GetGameSession()->c2s_CmdPetFlyRefineAttr(iPetIndex, iAttrIndex, CDlgPetCombineZiZhi::GetXiandouId(), iNum);
}
