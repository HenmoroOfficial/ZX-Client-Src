// Filename	: DlgCharacterDungeonBook.cpp
// Creator	: Fu Chonggang
// Date		: 2012.4.19

#include "DlgCharacterDungeonBook.h"
#include "AUI//AUILabel.h"
#include "AUI//AUITextArea.h"
#include "AUI//AUIImagePicture.h"
#include "EC_Game.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_BattleDungeonTower.h"
#include "elementdataman.h"
#include "ExpTypes.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgCharacterDungeonBook, CDlgBase)

AUI_ON_COMMAND("last",			OnCommandLast)
AUI_ON_COMMAND("next",			OnCommandNext)
AUI_ON_COMMAND("head",			OnCommandHead)
AUI_ON_COMMAND("tail",			OnCommandTail)

AUI_END_COMMAND_MAP()

const int CDlgCharacterDungeonBook::ms_iNumTotal = 100;

CDlgCharacterDungeonBook::CDlgCharacterDungeonBook()
{
}

CDlgCharacterDungeonBook::~CDlgCharacterDungeonBook()
{
}

bool CDlgCharacterDungeonBook::OnInitDialog()
{
	AString strName;
	
	int i;
	for (i=0; i<MONSTER_PERPAGE; ++i)
	{
		strName.Format("Img_%d", i+1);
		m_pImgMonster[i] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
	}

	m_iCurPage = -1;
	return true;
}

bool CDlgCharacterDungeonBook::Release()
{
	return CDlgBase::Release();
}

void CDlgCharacterDungeonBook::OnShowDialog()
{
	if (m_iCurPage<0)
	{
		m_iCurPage = 0;
	}
	UpdatePage();
}

void CDlgCharacterDungeonBook::OnTick()
{
	PAUIDIALOG pDlg;
	PAUIOBJECT pObj;
	
	GetGameUIMan()->GetMouseOn(&pDlg, &pObj);
	int i;
	for (i=0; i<MONSTER_PERPAGE; ++i)
	{
		if (m_pImgMonster[i]==pObj && i!=m_iCurHintIndex)
		{
			m_iCurHintIndex = i;
			int id = m_pImgMonster[i]->GetData("book_id");
			PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Character_Dungeon_BookHover");
			PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)pDlg->GetDlgItem("Img_Pic");
			PAUITEXTAREA	pTxt  = (PAUITEXTAREA)pDlg->GetDlgItem("Txt_Intro");
			CECBattleDungeonTower *pDungeonTower = GetHostPlayer()->GetBattleDungeonTower();
			if(pDungeonTower->GetMonsterDesc(id))
				pTxt->SetText(pDungeonTower->GetMonsterDesc(id));
			SetBookIcon(pImg, id);
		}
	}
}


void CDlgCharacterDungeonBook::OnCommandLast(const char * szCommand)
{
	if (m_iCurPage==0)
		return;
	
	m_iCurPage--;
	UpdatePage();
}

void CDlgCharacterDungeonBook::OnCommandNext(const char * szCommand)
{
	if (m_iCurPage>=(ms_iNumTotal-1)/MONSTER_PERPAGE)
		return;
	
	m_iCurPage++;
	UpdatePage();
}

void CDlgCharacterDungeonBook::OnCommandHead(const char * szCommand)
{
	if (m_iCurPage==0)
		return;
	
	m_iCurPage = 0;
	UpdatePage();
}

void CDlgCharacterDungeonBook::OnCommandTail(const char * szCommand)
{
	if (m_iCurPage==(ms_iNumTotal-1)/MONSTER_PERPAGE)
		return;
	
	m_iCurPage = (ms_iNumTotal-1)/MONSTER_PERPAGE;
	UpdatePage();
}

void CDlgCharacterDungeonBook::UpdatePage()
{
	CECBattleDungeonTower *pDungeonTower = GetHostPlayer()->GetBattleDungeonTower();
	PAUILABEL pLab = (PAUILABEL)GetDlgItem("Txt_Frequency");
	ACString strText;
	strText.Format(_AL("%d/%d"), m_iCurPage+1, (ms_iNumTotal+MONSTER_PERPAGE-1)/MONSTER_PERPAGE);
	pLab->SetText(strText);

	int i;
	for (i=0; i<MONSTER_PERPAGE; ++i)
	{
		AUIOBJECT_SETPROPERTY p;
		if (m_iCurPage*MONSTER_PERPAGE+i<pDungeonTower->GetMonsterCount())
		{
			int id = pDungeonTower->GetMonster(m_iCurPage*MONSTER_PERPAGE+i);
			SetBookIcon(m_pImgMonster[i], id);
			m_pImgMonster[i]->SetHint(_AL("^DIALOG:Win_Character_Dungeon_BookHover"));
			m_pImgMonster[i]->SetData(id, "book_id");
		}
		else
		{
			sprintf(p.fn, "icon\\Dungeon_Tower_Bg.tga");
			m_pImgMonster[i]->SetProperty("Image File", &p);
			m_pImgMonster[i]->SetHint(_AL(""));
			m_pImgMonster[i]->SetData(-1, "book_id");
		}
	}

	m_iCurHintIndex = -1;
}

void CDlgCharacterDungeonBook::SetBookIcon(AUIImagePicture *pImg, int id)
{
	if (!pImg)
	{
		return;
	}
	AUIOBJECT_SETPROPERTY p;
	if (id==0)	//¿ÕÅÆ
	{
		sprintf(p.fn, "icon\\dungeon_tower_default.tga");
		pImg->SetProperty("Image File", &p);
		return;
	}

	AString strFile = GetGame()->GetDataPath(id);
	if(strFile.GetLength()>(int)strlen("Surfaces\\") && strFile.Left(strlen("Surfaces\\"))=="Surfaces\\")
		strFile.CutLeft(strlen("Surfaces\\"));
	sprintf(p.fn, strFile);
	pImg->SetProperty("Image File", &p);
}

void CDlgCharacterDungeonBook::SetMonsterIcon(AUIImagePicture *pImg, int id)
{
	if (id==0)
	{
		SetBookIcon(pImg, 0);
		return;
	}
	elementdataman* pElementDataMan = GetGame()->GetElementDataMan();
	DATA_TYPE dt;
	MONSTER_ESSENCE *pEss = (MONSTER_ESSENCE*)pElementDataMan->get_data_ptr(
		id, ID_SPACE_ESSENCE,dt); 
	if (dt != DT_MONSTER_ESSENCE)
	{
		return;
	}
	SetBookIcon(pImg, pEss->big_icon);
}
