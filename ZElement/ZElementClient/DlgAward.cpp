// Filename	: DlgAward.cpp
// Creator	: Tom Zhou
// Date		: October 11, 2005

#include "DlgAward.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "AUI\\AUIRadioButton.h"
#include "ExpTypes.h"

#include "AFI.h"
#include "AUI\\AUICTranslate.h"
#include "EC_IvtrItem.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgAward, CDlgBase)

AUI_ON_COMMAND("confirm",		OnCommand_confirm)
AUI_ON_COMMAND("select",		OnCommand_select)
AUI_ON_COMMAND("back",			OnCommand_back)
AUI_ON_COMMAND("front",			OnCommand_front)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_CANCEL)

AUI_END_COMMAND_MAP()

CDlgAward::CDlgAward()
{
}

CDlgAward::~CDlgAward()
{
}

bool CDlgAward::OnInitDialog()
{
	m_pBtn_Confirm	= (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Confirm");
	m_pBtn_Back		= (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Back");
	m_pBtn_Front	= (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Front");

	m_ulAwardNum	= 0;
	m_ulValidPage	= 0;
	m_ulCurrPage	= 0;

	return true;
}

void CDlgAward::OnCommand_confirm(const char * szCommand)
{
	int i;
	char szName[40];
	PAUIRADIOBUTTON pRadio;
	talk_proc::option *opt = (talk_proc::option *)GetDataPtr("ptr_talk_proc");
	if( !opt ) return;

	for( i = 1; ; i++ )
	{
		sprintf(szName, "Rdo_Award%d", i);
		pRadio = (PAUIRADIOBUTTON)GetDlgItem(szName);
		if( !pRadio || !pRadio->IsShow() ) break;

		if( pRadio->IsChecked() )
		{
			GetGameSession()->c2s_CmdNPCSevReturnTask(opt->param, 8*m_ulCurrPage + i - 1);
			GetGameUIMan()->EndNPCService();
			Show(false);
			break;
		}
	}
}

void CDlgAward::OnCommand_select(const char * szCommand)
{
	m_pBtn_Confirm->Enable(true);
}

void CDlgAward::OnCommand_back(const char * szCommand)
{
	if (m_ulCurrPage>0)
	{
		m_ulCurrPage--;
		ShowFeatAward();
		CheckRadioButton(1,0);
		m_pBtn_Confirm->Enable(false);
	}
}

void CDlgAward::OnCommand_front(const char * szCommand)
{
	if (m_ulCurrPage<m_ulValidPage)
	{
		m_ulCurrPage++;
		ShowFeatAward();
		CheckRadioButton(1,0);
		m_pBtn_Confirm->Enable(false);
	}
}

void CDlgAward::OnCommand_CANCEL(const char * szCommand)
{
	GetGameUIMan()->EndNPCService();
	Show(false);
}

void CDlgAward::ChooseAward(AWARD_DATA &ad)
{
	m_Award			= ad;
	m_ulAwardNum	= ad.m_ulCandItems;
	m_ulValidPage	= (m_ulAwardNum-1)/8;
	m_ulCurrPage	= 0;

	if ( 0 == m_ulValidPage )
	{
		m_pBtn_Back->Show(false);
		m_pBtn_Front->Show(false);
	}
	else
	{
		m_pBtn_Back->Show(true);
		m_pBtn_Front->Show(true);
	}

	ShowFeatAward();
}

void CDlgAward::ShowFeatAward()
{
	char szName[40];
	AString strFile;
	ACHAR szText[40];
	CECIvtrItem *pItem;
	AUICTranslate trans;
	unsigned long i, j, idItem, nNum;
	PAUIIMAGEPICTURE pImage;

	for(i=0;i<8;i++)
		for (j=0;j<5;j++)
		{
			sprintf(szName, "Item_%02d%02d", i+1, j+1);
			pImage = (PAUIIMAGEPICTURE)GetDlgItem(szName);
			if( !pImage ) break;	// Line done.
			pImage->ClearCover();
			pImage->SetHint(_AL(""));
			pImage->SetText(_AL(""));
		}

	for( i = (8*m_ulCurrPage + 1); i< (8*m_ulCurrPage + 9); i++ )
	{
		sprintf(szName, "Item_%02d01", i-8*m_ulCurrPage);
		pImage = (PAUIIMAGEPICTURE)GetDlgItem(szName);
		if( !pImage ) break;		// All done.
		
		sprintf(szName, "Rdo_Award%d", i-8*m_ulCurrPage);
		GetDlgItem(szName)->Show(false);
		
		for( j = 1; ; j++ )
		{
			sprintf(szName, "Item_%02d%02d", i-8*m_ulCurrPage, j);
			pImage = (PAUIIMAGEPICTURE)GetDlgItem(szName);
			if( !pImage ) break;	// Line done.
			
			if( i - 1 < (int)m_Award.m_ulCandItems &&
				j - 1 < (int)m_Award.m_CandItems[i - 1].m_ulAwardItems )
			{
				idItem = m_Award.m_CandItems[i - 1].m_AwardItems[j - 1].m_ulItemTemplId;
				pItem = CECIvtrItem::CreateItem(idItem, 0, 1);
				
				nNum = m_Award.m_CandItems[i - 1].m_AwardItems[j - 1].m_ulItemNum;
				if( nNum > 1 )
				{
					a_sprintf(szText, _AL("%d"), nNum);
					pImage->SetText(szText);
				}
				else
					pImage->SetText(_AL(""));
				
				pItem->GetDetailDataFromLocal();
				pImage->SetHint(trans.Translate(pItem->GetDesc()));
				af_GetFileTitle(pItem->GetIconFile(), strFile);
				strFile.MakeLower();
				int ItemIndex;
				A2DSprite* pSprite = GetGameUIMan()->GetSpriteIcon(CECGameUIMan::ICONS_INVENTORY, strFile.GetBuffer(0), &ItemIndex);
				pImage->SetCover( pSprite, ItemIndex);
				pImage->SetColor(GetGameUIMan()->GetItemColor(pItem));

			//	pImage->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
			//		GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
				
				delete pItem;
				
				sprintf(szName, "Rdo_Award%d", i-8*m_ulCurrPage);
				GetDlgItem(szName)->Show(true);
			}
			else
			{
				pImage->ClearCover();
				pImage->SetColor(A3DCOLORRGB(255,255,255));
				pImage->SetHint(_AL(""));
				pImage->SetText(_AL(""));
			}
		}
	}

	//disable useless button
	m_pBtn_Back->Enable(0!=m_ulCurrPage);
	m_pBtn_Front->Enable(m_ulCurrPage<m_ulValidPage );
	
}