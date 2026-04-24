// Filename	: DlgBook.cpp
// Creator	: Xiao Zhou
// Date		: 2008/8/11

#include "AFI.h"
#include "AFileImage.h"
#include "DlgBook.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_IvtrConsume.h"
#include "EC_HostPlayer.h"
#include "EC_GameUIMan.h"
#include "EC_Inventory.h"
#include "ExpTypes.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgBookBase, CDlgBase)

AUI_ON_COMMAND("next",		OnCommandNext)
AUI_ON_COMMAND("Btn_Next",	OnCommandNext)
AUI_ON_COMMAND("prev",		OnCommandPrev)
AUI_ON_COMMAND("Btn_Prev",	OnCommandPrev)
AUI_ON_COMMAND("Btn_Close",	OnCommandCancel)
AUI_ON_COMMAND("IDCANCEL",	OnCommandCancel)

AUI_END_COMMAND_MAP()

//////////////////////////////////////////////////////////////////////////

//CDlgBookBase

//////////////////////////////////////////////////////////////////////////

CDlgBookBase::CDlgBookBase()
{
	m_pItemBook = NULL;
	m_nPage		= -1;
}

CDlgBookBase::~CDlgBookBase()
{
}

void CDlgBookBase::OnCommandNext(const char* szCommand)
{
	if( m_pItemBook )
	{
		if( m_nPage < 32 && m_pItemBook->GetDBEssence()->file_content[m_nPage + 1] != 0 )
		{
			m_nPage++;
			Update();
		}
	}
}

void CDlgBookBase::OnCommandPrev(const char* szCommand)
{
	if( m_pItemBook )
	{
		if( m_nPage > 0 )	
		{
			m_nPage--;
			Update();
		}
	}
}

void CDlgBookBase::OnCommandCancel(const char* szCommand)
{
	if( m_pItemBook )
	{
		m_pItemBook->Freeze(false);
		m_pItemBook = NULL;
	}
	m_nPage	 = -1;
	SetData(9999);
	Show(false);
}

bool CDlgBookBase::OnInitDialog()
{
	m_pBtn_Next = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Next");
	m_pBtn_Prev = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Prev");
	m_pBtn_Close = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Close");
	SetData(9999);

	return true;
}

void CDlgBookBase::Update()
{
	if( !m_pItemBook )
		return;
	
	m_pBtn_Next->Show(m_nPage < 32 && m_pItemBook->GetDBEssence()->file_content[m_nPage + 1] != 0);
	m_pBtn_Prev->Show(m_nPage > 0);
	
	AString strFile;
	strFile = g_pGame->GetDataPath(m_pItemBook->GetDBEssence()->file_content[m_nPage]);
	if( strFile.Right(4).CompareNoCase(".txt") == 0 )
	{
		AFileImage aFile;
		aFile.Open(strFile, AFILE_OPENEXIST | AFILE_BINARY | AFILE_TEMPMEMORY);
		ACHAR szText[1024];
		DWORD dwLen;
		ACString strText;
		while( aFile.Read(szText, 1022, &dwLen) )
		{
			szText[dwLen / 2] = 0;
			const BOOK_ESSENCE* ess = m_pItemBook->GetDBEssence();
			strText += szText;
		}
		SetBookText(strText);
		aFile.Close();
	}
	else
	{
		SetBookText(NULL);
	}
}

void CDlgBookBase::SetBookInfo(int nIndex, CECIvtrBook* pBook)
{
	CDlgBookBase *pDlg = NULL;
	const BOOK_ESSENCE* ess = pBook->GetDBEssence();
	if (ess->book_type == 0)
		pDlg = (CDlgBookBase*)GetGameUIMan()->GetDialog("Win_Book");
	else if (ess->book_type == 1)
		pDlg = (CDlgBookBase*)GetGameUIMan()->GetDialog("Win_Book_Type1");
	else if (ess->book_type == 2)
		pDlg = (CDlgBookBase*)GetGameUIMan()->GetDialog("Win_Book_Type2");
	else
		return;

	if (pDlg!=this)
	{
		pDlg->SetBookInfo(nIndex, pBook);
		return;
	}
	if( m_pItemBook)
	{
		m_pItemBook->Freeze(false);
	}

	m_pItemBook = pBook;
	m_pItemBook->Freeze(true);
	pDlg->SetData(nIndex);
	pDlg->Show(true);
}

//////////////////////////////////////////////////////////////////////////

//CDlgBookScroll

//////////////////////////////////////////////////////////////////////////


AUI_BEGIN_COMMAND_MAP(CDlgBookScroll, CDlgBookBase)

AUI_END_COMMAND_MAP()

CDlgBookScroll::CDlgBookScroll()
{
	m_dwTick	= 0;
}

CDlgBookScroll::~CDlgBookScroll()
{
}

bool CDlgBookScroll::OnInitDialog()
{
	m_pTxt_Content = (PAUITEXTAREA)GetDlgItem("Txt_Content");
	m_pImg_Book = (PAUIIMAGEPICTURE)GetDlgItem("Img_Book");
	m_pImg_Content = (PAUIIMAGEPICTURE)GetDlgItem("Img_Content");

	return CDlgBookBase::OnInitDialog();
}

void CDlgBookScroll::OnTick()
{
	DWORD dwTick = GetTickCount();
	if( m_dwTick == 0 )
		return;
	if (m_dwTick < dwTick)
	{
		m_pBtn_Next->Show(true);
		m_pBtn_Prev->Show(true);
		m_pBtn_Close->Show(true);
		Update();
		return;
	}

	int nFrame = (m_dwTick - dwTick) / 200 ;
	if (m_pImg_Book)
	{
		if( nFrame < 5 )
			m_pImg_Book->FixFrame(4 - nFrame);
		else
			m_pImg_Book->FixFrame(nFrame - 5);
	}
}

void CDlgBookScroll::OnShowDialog()
{
	if( m_pItemBook )
	{
		m_pItemBook->Freeze(true);
		m_pImg_Book->FixFrame(0);
		m_dwTick = GetTickCount() + 1000;
		m_nPage = 0;
		
		m_pBtn_Next->Show(false);
		m_pBtn_Prev->Show(false);
		m_pBtn_Close->Show(false);
		m_pImg_Content->Show(false);
		m_pTxt_Content->Show(false);
	}
}

void CDlgBookScroll::SetBookText(const ACHAR *pszText)
{
	m_dwTick = 0;
	m_pTxt_Content->Show(false);
	m_pImg_Content->Show(true);
	if (pszText)
	{
		m_pTxt_Content->SetText(pszText);
		m_pTxt_Content->Show(true);
		m_pImg_Content->Show(false);
	}
}

//////////////////////////////////////////////////////////////////////////

//CDlgBookScroll

//////////////////////////////////////////////////////////////////////////

AUI_BEGIN_COMMAND_MAP(CDlgBookType1, CDlgBookBase)

AUI_END_COMMAND_MAP()

CDlgBookType1::CDlgBookType1()
{
}

CDlgBookType1::~CDlgBookType1()
{
}

bool CDlgBookType1::OnInitDialog()
{
	m_pTxt_Content = (PAUITEXTAREA)GetDlgItem("Txt_Content");

	return CDlgBookBase::OnInitDialog();
}

void CDlgBookType1::OnTick()
{
}

void CDlgBookType1::OnShowDialog()
{
	if( m_pItemBook )
	{
		m_nPage = 0;
		Update();
	}
}

void CDlgBookType1::SetBookText(const ACHAR *pszText)
{
	if (pszText)
		m_pTxt_Content->SetText(pszText);
	else
		m_pTxt_Content->SetText(_AL(""));
}

//////////////////////////////////////////////////////////////////////////

//CDlgBookScroll

//////////////////////////////////////////////////////////////////////////

AUI_BEGIN_COMMAND_MAP(CDlgBookVertical, CDlgBookBase)

AUI_END_COMMAND_MAP()

CDlgBookVertical::CDlgBookVertical()
{
}

CDlgBookVertical::~CDlgBookVertical()
{
}

bool CDlgBookVertical::OnInitDialog()
{
	m_pVeT_Content = (PAUIVERTICALTEXT)GetDlgItem("Txt_VerText");
	
	return CDlgBookBase::OnInitDialog();
}

void CDlgBookVertical::OnTick()
{
}

void CDlgBookVertical::OnShowDialog()
{
	if( m_pItemBook )
	{
		m_nPage = 0;
		Update();
	}
}

void CDlgBookVertical::SetBookText(const ACHAR *pszText)
{
	if (pszText)
		m_pVeT_Content->SetText(pszText);
	else
		m_pVeT_Content->SetText(_AL(""));
}
