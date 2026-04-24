// Filename	: DlgZhuxian5Year.cpp
// Creator	: Fu Chonggang
// Date		: 2012/3/16

#include "DlgZhuxian5Year.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_SendC2SCmds.h"
#include "AUI\\AUIImagepicture.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgZhuxian5Year, CDlgBase)

AUI_ON_COMMAND("Btn_Congratulations",	OnCommand_Active)
AUI_ON_COMMAND("Btn_Get",				OnCommand_Get)

AUI_END_COMMAND_MAP()

const int CDlgZhuxian5Year::ms_MaxStars = 5;

CDlgZhuxian5Year::CDlgZhuxian5Year()
{
}

CDlgZhuxian5Year::~CDlgZhuxian5Year()
{
}

bool CDlgZhuxian5Year::OnInitDialog()
{
	m_iState = 0;

	return true;
}

void CDlgZhuxian5Year::OnShowDialog()
{
	static int lastSend = 0;
	if ((int)GetTickCount()>(lastSend+1000))
	{
		c2s_SendFiveAnniRequest(0);
		lastSend = GetTickCount();
	}

	UpdateDisplay();
}

void CDlgZhuxian5Year::OnCommand_Active(const char * szCommand)
{
	c2s_SendFiveAnniRequest(1);
}

void CDlgZhuxian5Year::OnCommand_Get(const char * szCommand)
{
	c2s_SendFiveAnniRequest(2);
}

int CDlgZhuxian5Year::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="info")
	{
		m_iStars = wParam;
		m_iState = lParam;

		UpdateDisplay();
	}

	return 0;
}

void CDlgZhuxian5Year::UpdateDisplay()
{
	GetDlgItem("Btn_Congratulations")->Enable(m_iState==0);
	GetDlgItem("Btn_Get")->Enable(m_iState==1);
	
	int i;
	PAUIIMAGEPICTURE pImg;
	AString strName;
	for (i=0; i<ms_MaxStars; ++i)
	{
		strName.Format("Img_Star%d", i+1);
		pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		pImg->FixFrame(i<(int)m_iStars ? 0 : 1);
	}
}



AUI_BEGIN_COMMAND_MAP(CDlgZhuxian5YearMin, CDlgBase)

AUI_ON_COMMAND("Btn_Max",	OnCommand_Active)

AUI_END_COMMAND_MAP()

CDlgZhuxian5YearMin::CDlgZhuxian5YearMin()
{
}

CDlgZhuxian5YearMin::~CDlgZhuxian5YearMin()
{
}

void CDlgZhuxian5YearMin::OnCommand_Active(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_AniversarySign5");
	pDlg->Show(!pDlg->IsShow());
}