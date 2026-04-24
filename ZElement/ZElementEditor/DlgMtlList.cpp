// PageScaleBone.cpp : implementation file
//

#include "StdAfx.h"
#include "Global.h"
#include "DlgMtlList.h"
//#include "Model.h"
#include <A3DTexture.h>
#include <A3DHLSL.h>
#include <A3DEffect.h>
#include <Shlwapi.h>
#include <afi.h>

#define LITMODELMTLLIBDIR "Shaders\\2.2\\HLSL\\LitModel\\"

/////////////////////////////////////////////////////////////////////////////
// CDlgMtlList dialog

CDlgMtlList::CDlgMtlList(CWnd* pParent /*=NULL*/)
: CDialog(CDlgMtlList::IDD, pParent)
{
}


void CDlgMtlList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MTLLIST, m_MtlList);
}


BEGIN_MESSAGE_MAP(CDlgMtlList, CDialog)
	//{{AFX_MSG_MAP(CDlgMtlList)
	ON_NOTIFY(NM_DBLCLK, IDC_MTLLIST, OnDBClickedMtl)
	//}}AFX_MSG_MAP

	//ON_MESSAGE(WM_ACTIVATEPAGE, OnActivatePage)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMtlList message handlers

BOOL CDlgMtlList::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_MtlList.InsertColumn(MI_NAME, "²ÄÖÊÃû³Æ", LVCFMT_LEFT, 170);
	m_MtlList.InsertColumn(MI_DESC, "ÃèÊö", LVCFMT_LEFT, 170);
	m_MtlList.SetExtendedStyle(m_MtlList.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	UpdateMtlLib("*.hlsl", NULL);
#ifdef _DEBUG
	UpdateMtlLib("*.sdrx", "[*]");
#endif // _DEBUG
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgMtlList::UpdateMtlLib(const char* pFileExt, const char* szPrefix)
{
	WIN32_FIND_DATA wfd;

	char szModelPath[MAX_PATH];

	//GetModuleFileName(NULL, szModelPath, MAX_PATH);
	//LPSTR lpFilename = PathFindFileName(af_GetBaseDir());
	strcpy(szModelPath, af_GetBaseDir());
	//lpFilename[0] = '\0';
	AString strFile = AString(LITMODELMTLLIBDIR) + pFileExt;
	PathCombine(szModelPath, szModelPath, strFile);

	HANDLE hFind = FindFirstFile(szModelPath, &wfd);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		do 
		{
			if(wfd.cFileName[0] == '!')
				continue;
			if(szPrefix != NULL)
			{
				AString strVertexFile;
				AString strPixelFile;
				A3DHLSLMan::ParseNameFromFile(AString(LITMODELMTLLIBDIR) + wfd.cFileName, 
					&strVertexFile, NULL, &strPixelFile, NULL, NULL);

				if(strVertexFile.GetLength() == 0 && strPixelFile.GetLength() != 0)
				{
					AString strName = AString(szPrefix) + wfd.cFileName;
					int nItem = m_MtlList.InsertItem(0, strName);
					m_MtlList.SetItemText(nItem, MI_DESC, strPixelFile);
				}
			}
			else
			{
				m_MtlList.InsertItem(0, wfd.cFileName);
			}
		} while (FindNextFile(hFind, &wfd));
		FindClose(hFind);
	}
}
BOOL CDlgMtlList::PreTranslateMessage(MSG* pMsg) 
{
	//	Disable 'return' and 'escape' because these keys will close dialog
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

//	On activate / disactivate this page
#ifdef _ANGELICA22
LRESULT CDlgMtlList::OnActivatePage(WPARAM wParam, LPARAM lParam)
#else
void CDlgMtlList::OnActivatePage(WPARAM wParam, LPARAM lParam)
#endif
{
	if (wParam)	//	Activate this page
	{
		//UpdateBoneSelection();
	}
	else	//	Disactivate this page
	{
		//if (m_pModel)
		//	m_pModel->ClearBoneSelection();
	}
#ifdef _ANGELICA22
	return 0L;
#endif
}

void CDlgMtlList::OnDBClickedMtl(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_strHLSLFilename.Empty();
	int nItem = m_MtlList.GetNextItem(-1, LVNI_FOCUSED);
	if(nItem >= 0)
	{
		m_strHLSLFilename = LITMODELMTLLIBDIR;
		CString strItemText = m_MtlList.GetItemText(nItem, 0);
		if(strItemText[1] == '*') {
			strItemText = strItemText.Right(strItemText.GetLength() - 3);
		}
		m_strHLSLFilename += (const char*)strItemText;
	}
	EndDialog(0);
}

