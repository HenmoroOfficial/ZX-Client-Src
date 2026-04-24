// TerrainModifyColorSmoothPanel.cpp : implementation file
//

#include "stdafx.h"
#include "ElementEditor.h"
#include "TerrainModifyColorSmoothPanel.h"
#include "TerrainColorSmoothOperation.h"
#include "OperationManager.h"

// CTerrainModifyColorSmoothPanel dialog

IMPLEMENT_DYNAMIC(CTerrainModifyColorSmoothPanel, CDialog)

CTerrainModifyColorSmoothPanel::CTerrainModifyColorSmoothPanel(CWnd* pParent /*=NULL*/)
	: CDialog(CTerrainModifyColorSmoothPanel::IDD, pParent)
{

}

CTerrainModifyColorSmoothPanel::~CTerrainModifyColorSmoothPanel()
{
}

void CTerrainModifyColorSmoothPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_BRUSHSMOOTH, m_editBrushSmooth);
	DDX_Control(pDX, IDC_SLIDER_BRUSHSMOOTH, m_sliderBrushSmooth);
}


BEGIN_MESSAGE_MAP(CTerrainModifyColorSmoothPanel, CDialog)
	ON_EN_CHANGE(IDC_EDIT_BRUSHSMOOTH, &CTerrainModifyColorSmoothPanel::OnEnChangeEditBrushsmooth)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_BRUSHSMOOTH, &CTerrainModifyColorSmoothPanel::OnNMReleasedcaptureSliderBrushsmooth)
END_MESSAGE_MAP()


// CTerrainModifyColorSmoothPanel message handlers

void CTerrainModifyColorSmoothPanel::OnEnChangeEditBrushsmooth()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CTerrainColorSmoothOperation *pOperation = (CTerrainColorSmoothOperation *)g_OperationContainer.GetOperation(OPERATION_TERRAIN_COLOR_SMOOTH);
	if(pOperation==NULL) return;

	int Num;
	CString str;
	m_editBrushSmooth.GetWindowText(str);
	Num = (int)atof(str);
	if(Num>100) 
	{
		Num = 100;
		str.Format("%d", Num);
		m_editBrushSmooth.SetWindowText(str);
	}else if(Num<1)
	{
		Num = 1;
		str.Format("%d", Num);
		m_editBrushSmooth.SetWindowText(str);
	}

	m_sliderBrushSmooth.SetPos(Num);
	pOperation->SetSmoothFactory(Num);
}

void CTerrainModifyColorSmoothPanel::OnNMReleasedcaptureSliderBrushsmooth(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here

	CTerrainColorSmoothOperation *pOperation = (CTerrainColorSmoothOperation *)g_OperationContainer.GetOperation(OPERATION_TERRAIN_COLOR_SMOOTH);
	if(pOperation)
	{

		int Num;
		CString str;
		Num = m_sliderBrushSmooth.GetPos();
		str.Format("%d", Num);
		m_editBrushSmooth.SetWindowText(str);
		pOperation->SetSmoothFactory(Num);
	}
	*pResult = 0;
}

BOOL CTerrainModifyColorSmoothPanel::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	AUX_FlatEditBox(&m_editBrushSmooth);
	m_sliderBrushSmooth.SetRange(1,100);

	CTerrainColorSmoothOperation *pOperation = (CTerrainColorSmoothOperation *)g_OperationContainer.GetOperation(OPERATION_TERRAIN_COLOR_SMOOTH);
	if(pOperation)
	{
		int Num = pOperation->GetSmoothFactory();
		m_sliderBrushSmooth.SetPos(1);
		CString str;
		str.Format("%d",Num);
		m_editBrushSmooth.SetWindowText(str);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
