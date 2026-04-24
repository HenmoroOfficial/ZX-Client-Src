// Filename	: DlgGameHelp.cpp
// Creator	: Fu Chonggang
// Date		: 2009/08/18

#include "AFileImage.h"
#include "DlgGameHelp.h"
#include "EC_GameUIMan.h"


#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgGameHelp, CDlgBase)

AUI_ON_COMMAND("Btn_Close",		OnCommand_Close)

AUI_END_COMMAND_MAP()

CDlgGameHelp::CDlgGameHelp()
{
}

CDlgGameHelp::~CDlgGameHelp()
{
}

bool CDlgGameHelp::OnInitDialog()
{
	AFileImage f;
	if (!f.Open("surfaces\\Ddz\\ddzhelp.txt", AFILE_OPENEXIST|AFILE_TEXT|AFILE_TEMPMEMORY))
	{
		AUI_ReportError(__LINE__, 1,"CDlgGameHelp::OnInitDialog(), surfaces\\Ddz\\ddzhelp.txt file error!");
		return false;
	}

	PAUITEXTAREA pTxt = (PAUITEXTAREA)GetDlgItem("Txt_Instruction");
	int nLength = f.GetFileLength();
	BYTE * buf = f.GetFileBuffer();
	ACString strText = AS2AC(AString((char*)buf, nLength));
	pTxt->SetText(strText);
	f.Close();

	return true;
}

void CDlgGameHelp::OnCommand_Close(const char * szCommand)
{
	Show(false);
}

void CDlgGameHelp::OnTick()
{
}
