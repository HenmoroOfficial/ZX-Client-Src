// Filename	:	DlgAniversaryPet5.cpp
// Creator	:	Lei Dongyu
// Date		:	Apr 17, 2012

#include "AUI\\AUI.h"
#include "AFI.h"
#include "EC_GameUIMan.h"
#include "DlgAniversaryPet5.h"
#include "DlgBaseExplorer.h"
#include "DlgInputNO.h"
#include "elementdataman.h"
#include "EC_Model.h"
#include "EC_Game.h"
#include "EC_HostPlayer.h"
#include "DlgDailyPromp.h"
#include "EC_Utility.h"
#include "EC_ModelRender.h"
#include "EC_GameSession.h"
#include "EC_IvtrItem.h"

#include "EC_Inventory.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgAniversaryPet5, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",				OnCommand_CANCEL)
AUI_ON_COMMAND("resumeequip",				OnCommand_Adopt)
AUI_ON_COMMAND("Btn_Get",					OnCommand_get)
AUI_ON_COMMAND("Btn_Raise",					OnCommand_raise)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgAniversaryPet5, CDlgBase)

AUI_ON_EVENT("Img_Raise*" ,		WM_LBUTTONDOWN ,	OnEventLButtonDown)

AUI_END_EVENT_MAP()

extern const char*	szActName[];

extern CECGame* g_pGame;

static const A3DCOLOR GREY_COLOR = A3DCOLORRGB(128, 128, 128);
static const A3DCOLOR NORMAL_COLOR = A3DCOLORRGB(255, 255, 255);

DWORD ms_dwActionTime = 10000;

CDlgAniversaryPet5::CDlgAniversaryPet5()
{
	m_pModel = NULL;
	m_pConfig = NULL;
	m_iSelectFood = 0;
	m_iLevel = 0;
	m_iExp = 0;
	m_iNextAward = 0;
}

CDlgAniversaryPet5::~CDlgAniversaryPet5()
{
}

bool CDlgAniversaryPet5::Release()
{
	if(m_bLoaded)
	{
		A3DRELEASE(m_pModel);
		m_bLoaded = false;
	}
	for(int i =0 ;i< 5;i++)
	{
		m_pImg[i]->SetData(0);
		m_pImg[i]->SetDataPtr(NULL);
		m_pImg[i]->ClearCover();
		m_pImg[i]->SetHint(_AL(""));

	}

	return CDlgBase::Release();
}

bool CDlgAniversaryPet5::OnInitDialog()
{
	if (!CDlgBase::OnInitDialog())
		return false;

	m_pPic_Part = (AUIImagePicture *)GetDlgItem("Pic_Part");
	m_pBtn_Adopt = (AUIStillImageButton *)GetDlgItem("Btn_Adopt");
	m_pTxt_Lv = (AUILabel *)GetDlgItem("Txt_Lv");
	m_pPrgs_Exp = (AUIProgress *)GetDlgItem("Prgs_Exp");
	m_pBtn_Get = (AUIStillImageButton *)GetDlgItem("Btn_Get");
	m_pBtn_Raise = (AUIStillImageButton *)GetDlgItem("Btn_Raise");
	m_pImg[0] = (AUIImagePicture *)GetDlgItem("Img_1");
	m_pImg[1] = (AUIImagePicture *)GetDlgItem("Img_2");
	m_pImg[2] = (AUIImagePicture *)GetDlgItem("Img_3");
	m_pImg[3] = (AUIImagePicture *)GetDlgItem("Img_4");
	m_pImg[4] = (AUIImagePicture *)GetDlgItem("Img_5");
	m_pImg_Cover[0] = (AUIImagePicture *)GetDlgItem("Img_Cover1");
	m_pImg_Cover[1] = (AUIImagePicture *)GetDlgItem("Img_Cover2");
	m_pImg_Cover[2] = (AUIImagePicture *)GetDlgItem("Img_Cover3");
	m_pImg_Cover[3] = (AUIImagePicture *)GetDlgItem("Img_Cover4");
	m_pImg_Cover[4] = (AUIImagePicture *)GetDlgItem("Img_Cover5");
	m_pImg_Raise[0] = (AUIImagePicture *)GetDlgItem("Img_Raise1");
	m_pImg_Raise[1] = (AUIImagePicture *)GetDlgItem("Img_Raise2");
	m_pImg_Choose[0] = (AUIImagePicture *)GetDlgItem("Img_ChooseBg1");
	m_pImg_Choose[1] = (AUIImagePicture *)GetDlgItem("Img_ChooseBg2");
	m_pLab_Hint1 = (AUILabel *)GetDlgItem("Lab_Hint1");
	m_pLab_Hint2 = (AUILabel *)GetDlgItem("Lab_Hint2");
	m_pTxt_Exp = (AUILabel *)GetDlgItem("Txt_Exp");

	m_iSelectFood = 0;
	m_iLevel = 0;
	m_iExp = 0;
	m_iNextAward = 0;

	m_pModel = new CECModel;

	DATA_TYPE DataType;
	unsigned int id = g_pGame->GetElementDataMan()->get_first_data_id(ID_SPACE_CONFIG , DataType);
	while(id)
	{
		if (DataType == DT_LITTLE_PET_UPGRADE_CONFIG)
		{
			m_pConfig = (LITTLE_PET_UPGRADE_CONFIG*)g_pGame->GetElementDataMan()->get_data_ptr(id , ID_SPACE_CONFIG , DataType);
			break;
		}
		id = g_pGame->GetElementDataMan()->get_next_data_id(ID_SPACE_CONFIG , DataType);
	}
	return true;
}

void CDlgAniversaryPet5::OnShowDialog()
{
	if(!m_pConfig)
	{
		Show(false);
		return;
	}
	int i;
	for(i = 0;i< 5 ;i++)
	{
		int id = m_pConfig->pet_upgrade_info_list[i].award_item;
		CECIvtrItem* pItem = CECIvtrItem::CreateItem(id , 0 , 1);
		if(!pItem)
		{
			continue;
		}
		m_pImg[i]->SetData(id);
		//m_pImg[i]->SetDataPtr(pItem);

		AString strFile;
		af_GetFileTitle(pItem->GetIconFile(), strFile);
		strFile.MakeLower();
		int iItemIndex;
		A2DSprite* pSprite = GetGameUIMan()->GetSpriteIcon(CECGameUIMan::ICONS_INVENTORY, strFile.GetBuffer(0), &iItemIndex);
		m_pImg[i]->SetCover(pSprite  , iItemIndex);
		AUICTranslate trans;
		m_pImg[i]->SetHint(trans.Translate(pItem->GetDesc()));
		delete pItem;
	}

	for(i = 0;i< 2 ;i++)
	{
		int id = m_pConfig->feed_pet[i].feed_pet_item_id;
		CECIvtrItem* pItem = CECIvtrItem::CreateItem(id , 0 , 1);
		if(!pItem)
		{
			m_pImg_Raise[i]->SetData(0);
			continue;
		}
		m_pImg_Raise[i]->SetData(id);
		
		AString strFile;
		af_GetFileTitle(pItem->GetIconFile(), strFile);
		strFile.MakeLower();
		int iItemIndex;
		A2DSprite* pSprite = GetGameUIMan()->GetSpriteIcon(CECGameUIMan::ICONS_INVENTORY, strFile.GetBuffer(0), &iItemIndex);
		m_pImg_Raise[i]->SetCover(pSprite  , iItemIndex);
		AUICTranslate trans;
		m_pImg_Raise[i]->SetHint(trans.Translate(pItem->GetDesc()));
		delete pItem;
	}
	ChangeModel();
}

void CDlgAniversaryPet5::OnTick()
{
	if(!m_pConfig)
	{
		Show(false);
	}
	DWORD dwTick = GetGame()->GetTickTime();

	bool bChangeModel = (m_iLevel != GetHostPlayer()->GetLittlePetLv());

	m_iLevel = GetHostPlayer()->GetLittlePetLv();
	m_iExp = GetHostPlayer()->GetLittlePetExp();
	m_iNextAward = GetHostPlayer()->GetLittlePetAward();

	//	没有猪小仙的情况
	if(!m_iLevel)
	{
		m_iExp = 0;
		m_iNextAward = 0;
		//m_iSelectFood = -1;
		m_pBtn_Adopt->Show(true);
		m_pBtn_Raise->Show(false);
		m_pLab_Hint1->Show(true);
		m_pLab_Hint2->Show(false);
		m_pImg_Raise[0]->Show(false);
		m_pImg_Raise[1]->Show(false);
	}
	else
	{
		m_pBtn_Adopt->Show(false);
		m_pBtn_Raise->Show(true);
		m_pLab_Hint2->Show(true);
		m_pLab_Hint1->Show(false);
		m_pImg_Raise[0]->Show(true);
		m_pImg_Raise[1]->Show(true);
	}

	ACString strTemp;
	strTemp.Format(GetGameUIMan()->GetStringFromTable(792) , m_iLevel);
	m_pTxt_Lv->SetText(strTemp);

	int iRequireExp = (m_iLevel > 0 && m_iLevel < 5) ? m_pConfig->pet_upgrade_info_list[m_iLevel].required_exp : 0;
	strTemp.Format(_AL("%d/%d") , m_iExp , iRequireExp);
	m_pTxt_Exp->SetText(strTemp);

	m_pPrgs_Exp->SetRangeMax(iRequireExp ? iRequireExp : 1);
	m_pPrgs_Exp->SetProgress(m_iExp);
	int i;
	ASSERT(m_iLevel == 0 || m_iLevel >= m_iNextAward);
	for(i = 0; i<5 ;i++)
	{
		if(i < m_iNextAward)	//已领
		{
			m_pImg[i]->SetColor(GREY_COLOR);
			m_pImg_Cover[i]->Show(true);
		}
		else if(i < m_iLevel)	//未领
		{
			m_pImg_Cover[i]	->Show(false);
			m_pImg[i]->SetColor(NORMAL_COLOR);
		}
		else	//未到等级
		{
			m_pImg_Cover[i]	->Show(false);
			m_pImg[i]->SetColor(GREY_COLOR);
		}
	}
	if(m_iLevel == 0 || m_iNextAward >= m_iLevel)
	{
		m_pBtn_Get->Enable(false);
	}
	else
	{
		m_pBtn_Get->Enable(true);
	}

	if(bChangeModel)
	{
		ChangeModel();
	}
	if (m_bLoaded)
	{
		if (ms_dwActionTime < dwTick)
		{
			ms_dwActionTime = a_Random(8000, 20000);
			m_pModel->PlayActionByName(szActName[a_Random(3,6)], 1.0f, true);
			m_pModel->QueueAction(szActName[a_Random(0,2)]);
		}
		ms_dwActionTime -= dwTick;
		m_pModel->Tick(dwTick);
		m_pPic_Part->SetRenderCallback( DlgModelRender,	(DWORD)m_pModel, MRT_LITTLEPET);
	}
	else
	{
		m_pPic_Part->SetRenderCallback( NULL, 0, 0);
	}

	bool bFeed = false;
	for(i = 0;i<2 ;i++)
	{
		int id = m_pConfig->feed_pet[i].feed_pet_item_id;
		if(id == 0)	//为0表示不使用此食料，需要进行控件的隐藏
		{
			m_pImg_Raise[i]->Show(false);
			m_pImg_Choose[i]->Show(false);
			continue;
		}
		int index = GetHostPlayer()->GetPack()->FindItem(id);
		if(index != -1)
		{
			m_pImg_Raise[i]->SetColor(NORMAL_COLOR);
			if(i == m_iSelectFood)
			{
				bFeed = true;
			}
		}
		else
		{
			m_pImg_Raise[i]->SetColor(GREY_COLOR);
		}
		m_pImg_Choose[i]->Show(m_iSelectFood  == i && m_iLevel);
	}
	m_pBtn_Raise->Enable(bFeed && m_iLevel != 5);
}

bool CDlgAniversaryPet5::Render()
{
	if(!CDlgBase::Render())
	{
		return false;
	}
	return true;
}

void CDlgAniversaryPet5::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
}

void CDlgAniversaryPet5::OnCommand_Adopt(const char * szCommand)
{
	GetGameSession()->c2s_CmdAdoptLittlePet();
}

void CDlgAniversaryPet5::OnCommand_get(const char * szCommand)
{
	int iEmptySlot = GetHostPlayer()->GetPack()->GetEmptySlotNum();
	if(iEmptySlot < m_iLevel - m_iNextAward)
	{
		GetGameUIMan()->MessageBox("" ,GetGameUIMan()->GetStringFromTable(7324) , MB_OK);
		return;
	}
	GetGameSession()->c2s_CmdGetLittlePetAward();
}

void CDlgAniversaryPet5::OnCommand_raise(const char * szCommand)
{
	if(m_iSelectFood < 0 || m_iSelectFood > 1)
	{
		return;
	}
	int id = (int)(m_pImg_Raise[m_iSelectFood]->GetData());
	if(id != m_pConfig->feed_pet[m_iSelectFood].feed_pet_item_id)
	{
		return;
	}
	int index = GetHostPlayer()->GetPack()->FindItem(id);
	if(index == -1)
	{
		return;
	}
	
	int iTotal = GetHostPlayer()->GetPack()->GetItemTotalNum(id);
	int maxExp = - m_iExp;
	for(int i = m_iLevel ;i < 5 ;i++)
	{
		maxExp += m_pConfig->pet_upgrade_info_list[i].required_exp;
	}
	if(maxExp < 0) maxExp = 0;
	int iMax = maxExp / m_pConfig->feed_pet[m_iSelectFood].gain_exp;
	if(maxExp % m_pConfig->feed_pet[m_iSelectFood].gain_exp)
	{
		iMax ++;
	}
	if(iMax < iTotal)
	{
		iTotal = iMax;
	}
	
	PAUIOBJECT pEdit = GetGameUIMan()->m_pDlgInputNO->GetDlgItem("DEFAULT_Txt_No.");
	
	pEdit->SetData(iTotal);
	ACHAR szText[40];
	a_sprintf(szText, _AL("%d"), 1);
	pEdit->SetText(szText);
	
	GetGameUIMan()->m_pDlgInputNO->SetData(id);
	GetGameUIMan()->m_pDlgInputNO->Show(true, true);
	GetGameUIMan()->m_pDlgInputNO->SetType(CDlgInputNO::INPUTNO_LITTLEPET);

	//GetGameSession()->c2s_CmdFeedLittlePet(index ,id);
}

void CDlgAniversaryPet5::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if(0 == strcmp(pObj->GetName() , "Img_Raise1"))
	{
		m_iSelectFood = 0;
	}
	else if(0 == strcmp(pObj->GetName() , "Img_Raise2"))
	{
		m_iSelectFood = 1;
	}
	else
	{
		return;
	}
}

void CDlgAniversaryPet5::ChangeModel()
{
	if(m_bLoaded)
	{
		A3DRELEASE(m_pModel);
		m_bLoaded = false;
	}
	m_pModel = new CECModel;
	const char* modelFile = "";
	if(m_iLevel)
	{
		modelFile = GetGame()->GetDataPath(m_pConfig->pet_upgrade_info_list[m_iLevel -1].file_model);
	}
	else
	{
		modelFile = GetGame()->GetDataPath(m_pConfig->default_file_model);
	}
 	if(m_pModel->Load(modelFile))
 	{
 		A3DSkin* pSkin;
 		
 		char szSkin[MAX_PATH];
 		strncpy(szSkin, modelFile, MAX_PATH);
 		glb_ChangeExtension(szSkin, "ski");
 		
		if (pSkin = GetGame()->LoadA3DSkin(szSkin, true))
 			m_pModel->GetA3DSkinModel()->AddSkin(pSkin, true);
 		m_pModel->SetGfxUseLOD(false);
 		m_pModel->SetPos(A3DVECTOR3(0));
 		m_pModel->SetDirAndUp(A3DVECTOR3(0, 0, -1.0f), A3DVECTOR3(0, 1.0f, 0));
 		m_pModel->PlayActionByName(szActName[0], 1.0f, true);
 		m_pModel->SetAutoUpdateFlag(false);
 		m_bLoaded = true;
 	}
	
}


AUI_BEGIN_COMMAND_MAP(CDlganiversarypet5_min, CDlgBase)

AUI_ON_COMMAND("Btn_Max",					OnCommand_max)

AUI_END_COMMAND_MAP()

CDlganiversarypet5_min::CDlganiversarypet5_min()
{
}

CDlganiversarypet5_min::~CDlganiversarypet5_min()
{
}

bool CDlganiversarypet5_min::OnInitDialog()
{
	if (!CDlgBase::OnInitDialog())
		return false;
	
	m_pBtn_Max = (AUIStillImageButton *)GetDlgItem("Btn_Max");
	return true;
}


void CDlganiversarypet5_min::OnCommand_max(const char * szCommand)
{
	AUIDialog* pDialog = GetGameUIMan()->GetDialog("Win_AniversaryPet5");
	pDialog->Show(!pDialog->IsShow());
}



AUI_BEGIN_COMMAND_MAP(CDlgAniversaryMail5Min, CDlgBase)

AUI_ON_COMMAND("Btn_Max",					OnCommand_max)

AUI_END_COMMAND_MAP()

void CDlgAniversaryMail5Min::OnCommand_max(const char * szCommand)
{
	CDlgBaseExplorer* pDlgExplorer = (CDlgBaseExplorer*)GetGameUIMan()->GetDialog("win_explorer");
	PAUILABEL pLab = (PAUILABEL)pDlgExplorer->GetDlgItem("Title");
	pLab->SetText(pDlgExplorer->GetStringFromTable(9175));
	pDlgExplorer->Show(true);
	pDlgExplorer->NavigateUrl(GetStringFromTable(9174));
}