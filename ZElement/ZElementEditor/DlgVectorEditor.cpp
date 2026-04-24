// DlgVectorEditor.cpp : implementation file
//

#include "EPstdafx.h"
#include <resource.h>
//#include "EffectPropRes.h"
#include "DlgVectorEditor.h"
#include "TextureProperty.h"
//#include "CustomProperty.h"


// CDlgVectorEditor dialog

IMPLEMENT_DYNAMIC(CDlgVectorEditor, CDialog)

CDlgVectorEditor::CDlgVectorEditor(VectorXProperty* pVectorXProperty, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgVectorEditor::IDD, pParent)
{
	m_pVectorXProperty = pVectorXProperty;
}

CDlgVectorEditor::~CDlgVectorEditor()
{
}

void CDlgVectorEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgVectorEditor, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgVectorEditor::OnBnClickedOk)
	//	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDlgVectorEditor message handlers
BOOL CDlgVectorEditor::OnInitDialog()
{
	CRect ParentRect;
	m_pParentWnd->GetWindowRect(&ParentRect);
	CRect ThisRect;
	GetWindowRect(&ThisRect);
	CRect TargetRect;
	TargetRect = ParentRect;
	TargetRect.right = TargetRect.left + ThisRect.Width();
	TargetRect.bottom = TargetRect.top + ThisRect.Height();
	MoveWindow(&TargetRect);

	CString csDisplay;
	int nSize = 0;
	if (m_pVectorXProperty->m_nType == VectorXProperty::VECTORX_INT)
	{
		nSize = m_pVectorXProperty->m_nBytes/sizeof(int);
		csDisplay.Format("%d", ((int*)m_pVectorXProperty->m_pBytes)[0]);
		SetDlgItemText(IDC_EDIT_X, csDisplay);
		csDisplay.Format("%d", ((int*)m_pVectorXProperty->m_pBytes)[1]);
		SetDlgItemText(IDC_EDIT_Y, csDisplay);
		switch(nSize)
		{		
		case 4:
			GetDlgItem(IDC_EDIT_W)->EnableWindow();
			csDisplay.Format("%d", ((int*)m_pVectorXProperty->m_pBytes)[3]);
			SetDlgItemText(IDC_EDIT_W, csDisplay);
		case 3:
			GetDlgItem(IDC_EDIT_Z)->EnableWindow();
			csDisplay.Format("%d", ((int*)m_pVectorXProperty->m_pBytes)[2]);
			SetDlgItemText(IDC_EDIT_Z, csDisplay);
			break;
		}
	}
	else
		if (m_pVectorXProperty->m_nType == VectorXProperty::VECTORX_FLOAT)
		{
			nSize = m_pVectorXProperty->m_nBytes/sizeof(float);
			csDisplay.Format("%.3f", ((float*)m_pVectorXProperty->m_pBytes)[0]);
			SetDlgItemText(IDC_EDIT_X, csDisplay);
			csDisplay.Format("%.3f", ((float*)m_pVectorXProperty->m_pBytes)[1]);
			SetDlgItemText(IDC_EDIT_Y, csDisplay);
			switch(nSize)
			{
			case 4:			
				GetDlgItem(IDC_EDIT_W)->EnableWindow();
				csDisplay.Format("%.3f", ((float*)m_pVectorXProperty->m_pBytes)[3]);
				SetDlgItemText(IDC_EDIT_W, csDisplay);
			case 3:			
				GetDlgItem(IDC_EDIT_Z)->EnableWindow();
				csDisplay.Format("%.3f", ((float*)m_pVectorXProperty->m_pBytes)[2]);
				SetDlgItemText(IDC_EDIT_Z, csDisplay);
				break;
			}
		}
		return CDialog::OnInitDialog();
}
void CDlgVectorEditor::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here	
	int nSize = 0;
	CString csValue;
	if (m_pVectorXProperty->m_nType == VectorXProperty::VECTORX_INT)
	{
		nSize = m_pVectorXProperty->m_nBytes/sizeof(int);
		int* pInt = (int*)m_pVectorXProperty->m_pBytes;
		GetDlgItemText(IDC_EDIT_X, csValue);
		pInt[0] = atoi(csValue);
		GetDlgItemText(IDC_EDIT_Y,csValue);
		pInt[1] = atoi(csValue);
		switch(nSize)
		{		
		case 4:
			GetDlgItemText(IDC_EDIT_W,csValue);
			pInt[3] = atoi(csValue);
		case 3:
			GetDlgItemText(IDC_EDIT_Z,csValue);
			pInt[2] = atoi(csValue);
			break;
		}
	}
	else
		if (m_pVectorXProperty->m_nType == VectorXProperty::VECTORX_FLOAT)
		{
			nSize = m_pVectorXProperty->m_nBytes/sizeof(float);
			float* pFloat = (float*)m_pVectorXProperty->m_pBytes;
			GetDlgItemText(IDC_EDIT_X, csValue);
			pFloat[0] = (float)atof(csValue);
			GetDlgItemText(IDC_EDIT_Y,csValue);
			pFloat[1] = (float)atof(csValue);
			switch(nSize)
			{
			case 4:
				GetDlgItemText(IDC_EDIT_W,csValue);
				pFloat[3] = (float)atof(csValue);
			case 3:
				GetDlgItemText(IDC_EDIT_Z,csValue);
				pFloat[2] = (float)atof(csValue);
				break;
			}
		}
		m_pVectorXProperty->GetDisplay();
		//m_pVectorXProperty->m_pFXUIParam->SetValue(m_pVectorXProperty->m_pBytes, m_pVectorXProperty->m_nBytes);
		OnOK();
}

void CDlgVectorEditor::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}
//////////////////////////////////////////////////////////////////////////
