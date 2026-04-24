// TerrainModifyColorMapPanel.cpp : implementation file
//

#include "stdafx.h"
#include "ElementEditor.h"
#include "TerrainModifyColorMapPanel.h"
#include "OperationManager.h"
#include "TerrainColorMapOperation.h"
#include "MainFrm.h"
#include "DlgBrushProperty.h"

// CTerrainModifyColorMapPanel dialog

IMPLEMENT_DYNAMIC(CTerrainModifyColorMapPanel, CDialog)

CTerrainModifyColorMapPanel::CTerrainModifyColorMapPanel(CWnd* pParent /*=NULL*/)
	: CDialog(CTerrainModifyColorMapPanel::IDD, pParent)
{

}

CTerrainModifyColorMapPanel::~CTerrainModifyColorMapPanel()
{
}

void CTerrainModifyColorMapPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PAINT_TYPE, m_wndComboPaintType);
	DDX_Control(pDX, IDC_COMBO_MAP_SIZE, m_wndComboMapSize);
	DDX_Control(pDX, IDC_SEL_COLOR, m_wndSelColor);
	DDX_Control(pDX, IDC_SLIDER_POWER, m_wndSliderPower);
	DDX_Control(pDX, IDC_EDIT_POWER, m_wndEditPower);
	DDX_Control(pDX, IDC_SPIN_POWER, m_wndSpinPower);
}


BEGIN_MESSAGE_MAP(CTerrainModifyColorMapPanel, CDialog)
	ON_MESSAGE(WM_SELCOLCHANGED, OnColorChange)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_POWER, &CTerrainModifyColorMapPanel::OnNMReleasedcaptureSliderPower)
	ON_EN_CHANGE(IDC_EDIT_POWER, &CTerrainModifyColorMapPanel::OnEnChangeEditPower)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_POWER, &CTerrainModifyColorMapPanel::OnDeltaposSpinPower)
	ON_CBN_SELCHANGE(IDC_COMBO_PAINT_TYPE, &CTerrainModifyColorMapPanel::OnCbnSelchangeComboPaintType)
	ON_CBN_SELCHANGE(IDC_COMBO_MAP_SIZE, &CTerrainModifyColorMapPanel::OnCbnSelchangeComboMapSize)
	ON_BN_CLICKED(IDC_BUTTON_GET_CUR_SIZE, &CTerrainModifyColorMapPanel::OnBnClickedButtonGetCurSize)
	ON_BN_CLICKED(IDC_BUTTON_BRUSH, &CTerrainModifyColorMapPanel::OnBnClickedButtonBrush)
END_MESSAGE_MAP()


// CTerrainModifyColorMapPanel message handlers

BOOL CTerrainModifyColorMapPanel::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_wndComboPaintType.InsertString(-1, "设置颜色");
	m_wndComboPaintType.InsertString(-1, "叠加混合");
	m_wndComboPaintType.InsertString(-1, "模糊颜色");
//	m_wndComboPaintType.InsertString(-1, "擦除");	

	m_wndComboMapSize.InsertString(-1, "128x128");
	m_wndComboMapSize.InsertString(-1, "256x256");
	m_wndComboMapSize.InsertString(-1, "512x512");
	m_wndComboMapSize.InsertString(-1, "1024x1024");
	m_wndComboMapSize.InsertString(-1, "2048x2048");
	m_wndComboMapSize.SetCurSel(2);

	m_wndSelColor.SetNotifyWindow(this);
	m_wndSliderPower.SetRange(10, 100);

	CTerrainColorMapOperation* pOperation = dynamic_cast<CTerrainColorMapOperation*>(g_OperationContainer.GetOperation(OPERATION_TERRAIN_COLORMAP));
	if(!pOperation)
		return FALSE;
	CString csText;
	int iPower = pOperation->GetPower();
	csText.Format("%d", iPower);
	m_wndEditPower.SetWindowText(csText);

	DWORD dwColor = pOperation->GetColor();
	DWORD rgb = RGB(A3DCOLOR_GETRED(dwColor), A3DCOLOR_GETGREEN(dwColor), A3DCOLOR_GETBLUE(dwColor));
	m_wndSelColor.SetColor(rgb);
	m_wndComboPaintType.SetCurSel((int)pOperation->GetPaintType());

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HRESULT CTerrainModifyColorMapPanel::OnColorChange(WPARAM wParam, LPARAM lParam)
{
	BYTE r, g, b;

	if (wParam == (WPARAM)m_wndSelColor.m_hWnd)
	{
		DWORD dwCol = m_wndSelColor.GetColor();
		r = GetRValue(dwCol);
		g = GetGValue(dwCol);
		b = GetBValue(dwCol);
		CTerrainColorMapOperation* pOperation = dynamic_cast<CTerrainColorMapOperation*>(g_OperationContainer.GetOperation(OPERATION_TERRAIN_COLORMAP));
		if(!pOperation)
			return S_FALSE;
		pOperation->SetColor(A3DCOLORRGBA(r,g,b,pOperation->GetAlpha()));
	}

	return S_OK;
}

void CTerrainModifyColorMapPanel::OnNMReleasedcaptureSliderPower(NMHDR *pNMHDR, LRESULT *pResult)
{
	int iPos = m_wndSliderPower.GetPos();
	CString csText;
	csText.Format("%d", iPos);
	m_wndEditPower.SetWindowText(csText);
	*pResult = 0;
}

void CTerrainModifyColorMapPanel::OnEnChangeEditPower()
{
	CTerrainColorMapOperation* pOperation = dynamic_cast<CTerrainColorMapOperation*>(g_OperationContainer.GetOperation(OPERATION_TERRAIN_COLORMAP));
	if(!pOperation)
		return;
	CString csText;
	m_wndEditPower.GetWindowText(csText);
	int iPower = atoi(csText);
	int fOldPower = iPower;
	a_Clamp(iPower, 10, 100);
	if(fOldPower != iPower)
	{
		csText.Format("%d",iPower);
		m_wndEditPower.SetWindowText(csText);
	}
	m_wndSliderPower.SetPos(iPower);
	pOperation->SetPower(iPower);
}

void CTerrainModifyColorMapPanel::OnDeltaposSpinPower(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	CString csText;
	m_wndEditPower.GetWindowText(csText);
	int iPower = atoi(csText);
	int fOldPower = iPower;
	iPower += -pNMUpDown->iDelta;
	a_Clamp(iPower, 10, 100);
	if(fOldPower != iPower)
	{
		csText.Format("%d", iPower);
		m_wndEditPower.SetWindowText(csText);
	}
	*pResult = 0;
}

void CTerrainModifyColorMapPanel::OnCbnSelchangeComboPaintType()
{
	CTerrainColorMapOperation* pOperation = dynamic_cast<CTerrainColorMapOperation*>(g_OperationContainer.GetOperation(OPERATION_TERRAIN_COLORMAP));
	if(!pOperation)
		return;
	pOperation->SetPaintType((CTerrainColorMapOperation::PAINT_TYPE)m_wndComboPaintType.GetCurSel());
}

void CTerrainModifyColorMapPanel::SelMapSize(int size)
{
	int iSel = -1;
	switch(size)
	{
	case 128:
		iSel = 0;
		break;
	case 256:
		iSel = 1;
		break;
	case 512:
		iSel = 2;
		break;
	case 1024:
		iSel = 3;
		break;
	case 2048:
		iSel = 4;
		break;
	}
	if(iSel == -1)
		return;
	m_wndComboMapSize.SetCurSel(iSel);
}

void CTerrainModifyColorMapPanel::OnCbnSelchangeComboMapSize()
{
	/*
	int iSel = m_wndComboMapSize.GetCurSel();
	if(iSel == -1)
		return;

	int size = 512;
	switch(iSel)
	{
	case 0:
		size = 128;
		break;
	case 1:
		size = 256;
		break;
	case 2:
		size = 512;
		break;
	case 3:
		size = 1024;
		break;
	case 4:
		size = 2048;
		break;
	}

	if(AUX_GetMainFrame()->GetMap())
	{
		CTerrain* pTerrain = AUX_GetMainFrame()->GetMap()->GetTerrain();
		if(pTerrain->GetColorMap()->iWidth != size)
		{
			if(IDYES == AfxMessageBox("改变颜色图的大小会导致已经刷好的颜色数据丢失！你确定要这么做么？",MB_YESNO))
			{
				pTerrain->RecreateColorMap(size,size);
				AUX_GetMainFrame()->GetMap()->RecreateTerrainRender(true,false);
			}
			else
			{
				SelMapSize(pTerrain->GetColorMap()->iWidth);
			}
		}
	}*/
}

void CTerrainModifyColorMapPanel::OnBnClickedButtonGetCurSize()
{
	if(!AUX_GetMainFrame()->GetMap())
		return;
	CTerrain* pTerrain = AUX_GetMainFrame()->GetMap()->GetTerrain();
	if(!pTerrain)
		return;
	SelMapSize(pTerrain->GetColorMap()->iWidth);
}

#include "Brush.h"

void CTerrainModifyColorMapPanel::OnBnClickedButtonBrush()
{
	CTerrainColorMapOperation* pOperation = dynamic_cast<CTerrainColorMapOperation*>(g_OperationContainer.GetOperation(OPERATION_TERRAIN_COLORMAP));
	if(!pOperation)
		return;
	CDlgBrushProperty dlg(pOperation->GetBrush());
	dlg.DoModal();
}

void CTerrainModifyColorMapPanel::SetColor(DWORD dwColor)
{
	COLORREF clr = RGB(A3DCOLOR_GETRED(dwColor), A3DCOLOR_GETGREEN(dwColor), A3DCOLOR_GETBLUE(dwColor));
	m_wndSelColor.SetColor(clr);
	m_wndSelColor.Redraw();
}