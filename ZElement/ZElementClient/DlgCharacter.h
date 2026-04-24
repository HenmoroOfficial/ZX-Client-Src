// Filename	: DlgCharacter.h
// Creator	: Tom Zhou
// Date		: October 11, 2005

#pragma once

#include "DlgUpgrade.h"
#include "EC_IvtrTypes.h"
#include "EC_RoleTypes.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUICheckBox.h"
#include "AUI\\AUIComboBox.h"
#include "AUI\\AUIProgress.h"

const int reputationlevel[] =
{
	0,
	5000,
	25000,
	50000,
	100000,
	200000,
	500000,
	1000000,
	2000000,
	5000000,
	2147483647
};

const int regionreputationlevel[] =
{
	0,
	2000,
	10000,
	50000,
	130000,
	290000,
	530000,
	930000,
	1570000,
	2850000,
	2147483647
};

const int affectionlevel[] =
{
	0,
	1000,
	3000,
	5000,
	7000,
	9000,
	11000,
	13000,
	15000,
	17000,
	2147483647
};

const int teacherlevel[] =
{
	0,
	350,
	1500,
	4500,
	12000,
	30000,
	72000,
	168000,
	384000,
	864000,
	2147483647
};

const int literarytalentlevel[] =
{
	0,
	300,
	1200,
	3900,
	12000,
	36300,
	109200,
	327900,
	984000,
	2952300,
	2147483647
};

const int battlescorelevel[] =
{
	0,
	400,
	800,
	1200,
	2000,
	4800,
	8800,
	14400,
	27200,
	44000,
	2147483647
};

const int tianzong[] =
{
	0,
	5000,
	10000,
	40000,
	100000,
	2147483647
};

const int dizong[] =
{
	0,
	5000,
	10000,
	40000,
	100000,
	2147483647
};

const int renzong[] =
{
	0,
	5000,
	10000,
	40000,
	100000,
	2147483647	
};

class CECLoginPlayer;

class CDlgCharacter : public CDlgFormatText  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
		
public:
	CDlgCharacter();
	virtual ~CDlgCharacter();

	void OnCommand_Fashion(const char * szCommand);
	void OnCommand_ChangeFashion(const char * szCommand);
	void OnCommand_ShowEquip(const char * szCommand);
	void OnCommand_Page(const char * szCommand);
	void OnCommand_Credit(const char * szCommand);
	void OnCommand_HideSpecEquip(const char * szCommand);
	void OnCommand_SelectTitle(const char * szCommand);
	void OnCommand_TitleExp(const char * szCommand);
	void OnCommand_ManaHelp(const char * szCommand);
	void OnCommand_CreditHelp(const char * szCommand);
	void OnCommand_Confirm(const char * szCommand);
	void OnCommand_SetInfo(const char * szCommand);
	void OnCommand_Para(const char * szCommand);
	void OnCommand_DungeonAward(const char * szCommand);
	void OnCommand_DungeonRank(const char * szCommand);
	void OnCommand_DungeonMonster(const char * szCommand);
	void OnCommand_DungeonFly(const char *szCommand);
	void OnCommand_Cancel(const char * szCommand);

	void OnCommand_RemedyMetempsychosis(const char * szCommand); // 2012-10-17 add 显示弥补飞升按钮响应 by zy 

	void onCommand_RoleRune(const char * szCommand);
	
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonUp(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDblclk(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void ChangeFashion(int index);
	void UseFashionItems(int index);
	void ReLoadModel();
	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	
protected:
	int m_nAngle;
	int	m_nNewTitleId;
	DWORD m_dwLastSelectTitle;
	int	m_iCultivation;

	PAUILABEL m_pTxt_Name;
	PAUILABEL m_pTxt_Faction;
	PAUILABEL m_pTxt_Level;
	PAUILABEL m_pTxt_PK;
	PAUILABEL m_pTxt_Mate;
	PAUILABEL m_pTxt_Contribution;
	PAUILABEL m_pTxt_Combine;
	PAUILABEL m_pTxt_Devotion;
	PAUILABEL m_pTxt_Exp;
	PAUIPROGRESS m_pPrg_Exp;
	PAUICOMBOBOX m_pCombo_Title;
	PAUILABEL m_pTxt_HP;
	PAUILABEL m_pTxt_MP;
	PAUIPROGRESS m_pPrg_HP;
	PAUIPROGRESS m_pPrg_MP;
	PAUILABEL m_pTxt_Definition;
	PAUILABEL m_pTxt_Evade;
	PAUILABEL m_pTxt_SkillDefinition;
	PAUILABEL m_pTxt_SkillEvade;
	PAUILABEL m_pTxt_Defense;
	PAUILABEL m_pTxt_Damage;
	PAUILABEL m_pTxt_Critical;
	PAUILABEL m_pTxt_CriticalDamage;
	PAUILABEL m_pTxt_DeCritical;
	PAUILABEL m_pTxt_DeCriticalDamage;
	PAUILABEL m_pTxt_Mana;
	PAUILABEL m_pTxt_Family;
	PAUILABEL m_pTxt_StunRes;
	PAUILABEL m_pTxt_StunRes02;
	PAUILABEL m_pTxt_StunRes03;
	PAUILABEL m_pTxt_SilenceRes;
	PAUILABEL m_pTxt_SilenceRes02;
	PAUILABEL m_pTxt_SilenceRes03;
	PAUILABEL m_pTxt_WeaknessRes;
	PAUILABEL m_pTxt_WeaknessRes02;
	PAUILABEL m_pTxt_WeaknessRes03;
	PAUILABEL m_pTxt_SleepRes;
	PAUILABEL m_pTxt_SleepRes02;
	PAUILABEL m_pTxt_SleepRes03;
	PAUILABEL m_pTxt_EnlaceRes;
	PAUILABEL m_pTxt_EnlaceRes02;
	PAUILABEL m_pTxt_EnlaceRes03;

	PAUILABEL m_pTxt_DeDamageTaoism;
	PAUILABEL m_pTxt_DeDamageEvil;
	PAUILABEL m_pTxt_DeDamageBuddhism;
	PAUILABEL m_pLab_Level1;
	PAUILABEL m_pLab_Level2;
	PAUILABEL m_pLab_Level3;
	PAUIIMAGEPICTURE m_pImg_Prof1;
	PAUIIMAGEPICTURE m_pImg_Prof2;
	PAUIIMAGEPICTURE m_pImg_Prof3;

	PAUISTILLIMAGEBUTTON m_pBtn_LeftTurn;
	PAUISTILLIMAGEBUTTON m_pBtn_RightTurn;
	PAUIIMAGEPICTURE m_pImg_Char;
	PAUIIMAGEPICTURE m_pImgEquip[SIZE_EQUIPIVTR];
	PAUIPROGRESS m_pPro_Mana;
	PAUICHECKBOX m_pChk_ShowEquip;
	PAUICOMBOBOX		m_pCombo_Fashion;

	PAUICHECKBOX m_pAttrimakeup;
	CECLoginPlayer* m_pPlayer;
    AUIListBox* m_pListBox;

	void RefreshHostInfo();
	void RefreshHostNature();
	void RefreshHostNatureBasic();
	void RefreshHostNatureAdvanced();
	void RefreshHostNatuerResistance();
	void UpdateEquip();
	void UpdateTitle();
	void UpdateDungeonTower();
	ACString GetTitleHint(int idTitle, bool bOnlyDesc = false);
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	abase::vector<A2DSprite*> m_vecImageList;	
	ACString GetTitle(int idTitle, A3DCOLOR *pColor, int *type = NULL);

	void ShowRuneTipPop(); // 飞升150后，第一次打开时需要显示符文泡泡提示
};
