// FogSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ElementEditor.h"
#include "FogSettingDlg.h"
#include "SceneBoxArea.h"
#include "ElementMap.h"
#include "MainFrm.h"
#include "SceneObjectManager.h"
#include <AIniFile.h>
// CFogSettingDlg dialog

IMPLEMENT_DYNAMIC(CFogSettingDlg, CDialog)

static CFogSettingDlg* g_pDlg = NULL;

inline DWORD SwapRedBlue(DWORD color, DWORD dwAlpha)
{
	DWORD dwRed = (color & 0x00FF0000) >> 16;
	DWORD dwBlue = (color & 0x000000FF);

	return (dwAlpha << 24) | (color & 0x0000FF00) | (dwRed) | (dwBlue << 16);
}

CFogSettingDlg::CFogSettingDlg(CWnd* pParent /*=NULL*/)
: CDialog(CFogSettingDlg::IDD, pParent)
{
	g_pDlg = this;
}

CFogSettingDlg::~CFogSettingDlg()
{
	g_pDlg = NULL;
}

void CFogSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_COLOR_UP, m_ctrlSelColorUp);
	DDX_Control(pDX, IDC_STATIC_COLOR_DOWN, m_ctrlSelColorDown);
}


BEGIN_MESSAGE_MAP(CFogSettingDlg, CDialog)
	ON_MESSAGE(WM_SELCOLCHANGED, OnColorChange)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_RADIO_DAY, &CFogSettingDlg::OnBnClickedRadioDay)
	ON_BN_CLICKED(IDC_RADIO_NIGHT, &CFogSettingDlg::OnBnClickedRadioNight)
	ON_EN_CHANGE(IDC_EDIT_NEAR, &CFogSettingDlg::OnEnChangeEditNear)
	ON_EN_CHANGE(IDC_EDIT_FAR, &CFogSettingDlg::OnEnChangeEditFar)
	ON_EN_CHANGE(IDC_EDIT_HEIGHT1, &CFogSettingDlg::OnEnChangeEditHeight1)
	ON_EN_CHANGE(IDC_EDIT_HEIGHT2, &CFogSettingDlg::OnEnChangeEditHeight2)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, &CFogSettingDlg::OnBnClickedButtonImport)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, &CFogSettingDlg::OnBnClickedButtonExport)
END_MESSAGE_MAP()


// CFogSettingDlg message handlers
BOOL CFogSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//	Change edit boxes to flat style
	AUX_FlatEditBox((CEdit*)GetDlgItem(IDC_EDIT_NEAR));
	AUX_FlatEditBox((CEdit*)GetDlgItem(IDC_EDIT_FAR));
	AUX_FlatEditBox((CEdit*)GetDlgItem(IDC_EDIT_HEIGHT1));
	AUX_FlatEditBox((CEdit*)GetDlgItem(IDC_EDIT_HEIGHT2));

	CButton *pButton = (CButton *)GetDlgItem(IDC_RADIO_DAY);
	if(pButton) pButton->SetCheck(BST_CHECKED);

	pButton = (CButton *)GetDlgItem(IDC_RADIO_NIGHT);
	if(pButton) pButton->SetCheck(BST_UNCHECKED);

	CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_DENSITY_UP);
	pSlider->SetRange(0, 500);
	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_INTENSITY_UP);
	pSlider->SetRange(0, 500);
	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_DENSITY_DOWN);
	pSlider->SetRange(0, 500);
	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_INTENSITY_DOWN);
	pSlider->SetRange(0, 500);
	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_HORZE_DENSITY);
	pSlider->SetRange(0, 500);
	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_HORZE_POWER);
	pSlider->SetRange(0, 300);
	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_HEIGHT_DENSITY);
	pSlider->SetRange(0, 1000);

	m_ctrlSelColorUp.SetNotifyWindow(this);
	m_ctrlSelColorDown.SetNotifyWindow(this);

	UpdateControl();


	return TRUE;
}

bool CFogSettingDlg::IsDay()
{
	CButton *pButton = (CButton *)GetDlgItem(IDC_RADIO_DAY);
	return  pButton->GetCheck() == BST_CHECKED;
}

HRESULT CFogSettingDlg::OnColorChange(WPARAM wParam, LPARAM lParam)
{
	CSceneBoxArea* pBox = GetCurrentBox();
	if (!pBox)
		return S_OK;

	BYTE r, g, b;

	if (wParam == (WPARAM)m_ctrlSelColorUp.m_hWnd)
	{
		DWORD dwCol = SwapRedBlue(m_ctrlSelColorUp.GetColor(), 255);
		if (IsDay())
		{
			pBox->m_PropertyData.m_dwFogColor = dwCol;
		}
		else
		{
			pBox->m_PropertyData.m_dwFogColorNight = dwCol;
		}

	}
	else if (wParam == (WPARAM)m_ctrlSelColorDown.m_hWnd)
	{
		DWORD dwCol = SwapRedBlue(m_ctrlSelColorDown.GetColor(), 255);
		if (IsDay())
		{
			pBox->m_PropertyData.m_dwFogColor2 = dwCol;
		}
		else
		{
			pBox->m_PropertyData.m_dwFogColor2Night = dwCol;
		}
	}
	UpdateBox(pBox);
	return S_OK;

}

void CFogSettingDlg::UpdateControl()
{
	CSceneBoxArea* pBox = GetCurrentBox();
	if (!pBox)
		return;

	CString strText;
	if (IsDay())
	{
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_NEAR);
		strText.Format("%g", pBox->m_PropertyData.m_fFogStart);
		pEdit->SetWindowText(strText);

		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_FAR);
		strText.Format("%g", pBox->m_PropertyData.m_fFogEnd);
		pEdit->SetWindowText(strText);

		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_HEIGHT1);
		strText.Format("%g", pBox->m_PropertyData.m_fFogHeightStart);
		pEdit->SetWindowText(strText);

		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_HEIGHT2);
		strText.Format("%g", pBox->m_PropertyData.m_fFogHeightEnd);
		pEdit->SetWindowText(strText);

		m_ctrlSelColorUp.SetColor(SwapRedBlue(pBox->m_PropertyData.m_dwFogColor, 0));
		m_ctrlSelColorDown.SetColor(SwapRedBlue(pBox->m_PropertyData.m_dwFogColor2, 0));
		m_ctrlSelColorUp.Redraw();
		m_ctrlSelColorDown.Redraw();

		CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_DENSITY_UP);
		pSlider->SetPos((int)(pBox->m_PropertyData.m_fFogDensityStart * 100));
		pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_INTENSITY_UP);
		pSlider->SetPos((int)(pBox->m_PropertyData.m_fFogColorDensityStart * 100));
		pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_DENSITY_DOWN);
		pSlider->SetPos((int)(pBox->m_PropertyData.m_fFogDensityEnd * 100));
		pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_INTENSITY_DOWN);
		pSlider->SetPos((int)(pBox->m_PropertyData.m_fFogColorDensityEnd * 100));
		pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_HORZE_DENSITY);
		pSlider->SetPos((int)(pBox->m_PropertyData.m_fFogHorzeDensity * 100));
		pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_HORZE_POWER);
		pSlider->SetPos((int)(pBox->m_PropertyData.m_fFogHorzePower * 100));
		pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_HEIGHT_DENSITY);
		pSlider->SetPos((int)(pBox->m_PropertyData.m_fFogDeepness * 100));
	}
	else
	{
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_NEAR);
		strText.Format("%g", pBox->m_PropertyData.m_fFogStartNight);
		pEdit->SetWindowText(strText);

		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_FAR);
		strText.Format("%g", pBox->m_PropertyData.m_fFogEndNight);
		pEdit->SetWindowText(strText);

		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_HEIGHT1);
		strText.Format("%g", pBox->m_PropertyData.m_fFogHeightStartNight);
		pEdit->SetWindowText(strText);

		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_HEIGHT2);
		strText.Format("%g", pBox->m_PropertyData.m_fFogHeightEndNight);
		pEdit->SetWindowText(strText);

		m_ctrlSelColorUp.SetColor(SwapRedBlue(pBox->m_PropertyData.m_dwFogColorNight, 0));
		m_ctrlSelColorDown.SetColor(SwapRedBlue(pBox->m_PropertyData.m_dwFogColor2Night, 0));
		m_ctrlSelColorUp.Redraw();
		m_ctrlSelColorDown.Redraw();

		CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_DENSITY_UP);
		pSlider->SetPos((int)(pBox->m_PropertyData.m_fFogDensityStartNight * 100));
		pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_INTENSITY_UP);
		pSlider->SetPos((int)(pBox->m_PropertyData.m_fFogColorDensityStartNight * 100));
		pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_DENSITY_DOWN);
		pSlider->SetPos((int)(pBox->m_PropertyData.m_fFogDensityEndNight * 100));
		pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_INTENSITY_DOWN);
		pSlider->SetPos((int)(pBox->m_PropertyData.m_fFogColorDensityEndNight * 100));
		pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_HORZE_DENSITY);
		pSlider->SetPos((int)(pBox->m_PropertyData.m_fFogHorzeDensityNight * 100));
		pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_HORZE_POWER);
		pSlider->SetPos((int)(pBox->m_PropertyData.m_fFogHorzePowerNight * 100));
		pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_HEIGHT_DENSITY);
		pSlider->SetPos((int)(pBox->m_PropertyData.m_fFogDeepnessNight * 100));
	}
	UpdateData(FALSE);

}

CSceneBoxArea* CFogSettingDlg::GetCurrentBox()
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if (!pMap)
		return NULL;
	if (!pMap->GetSceneObjectMan())
		return NULL;
	return pMap->GetSceneObjectMan()->GetCurrentArea();
}

void CFogSettingDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CSceneBoxArea* pBox = GetCurrentBox();
	if (!pBox)
		return;

	CSliderCtrl* pSlider = (CSliderCtrl*) pScrollBar;

	switch (pSlider->GetDlgCtrlID())
	{
	case IDC_SLIDER_DENSITY_UP:
		if (IsDay())
		{
			pBox->m_PropertyData.m_fFogDensityStart = pSlider->GetPos() / 100.0f;
		}
		else
		{
			pBox->m_PropertyData.m_fFogDensityStartNight = pSlider->GetPos() / 100.0f;
		}
		break;
	case IDC_SLIDER_INTENSITY_UP:
		if (IsDay())
		{
			pBox->m_PropertyData.m_fFogColorDensityStart = pSlider->GetPos() / 100.0f;
		}
		else
		{
			pBox->m_PropertyData.m_fFogColorDensityStartNight = pSlider->GetPos() / 100.0f;
		}
		break;
	case IDC_SLIDER_DENSITY_DOWN:
		if (IsDay())
		{
			pBox->m_PropertyData.m_fFogDensityEnd = pSlider->GetPos() / 100.0f;
		}
		else
		{
			pBox->m_PropertyData.m_fFogDensityEndNight = pSlider->GetPos() / 100.0f;
		}
		break;
	case IDC_SLIDER_INTENSITY_DOWN:
		if (IsDay())
		{
			pBox->m_PropertyData.m_fFogColorDensityEnd = pSlider->GetPos() / 100.0f;
		}
		else
		{
			pBox->m_PropertyData.m_fFogColorDensityEndNight = pSlider->GetPos() / 100.0f;
		}
		break;
	case IDC_SLIDER_HORZE_DENSITY:
		if (IsDay())
		{
			pBox->m_PropertyData.m_fFogHorzeDensity = pSlider->GetPos() / 100.0f;
		}
		else
		{
			pBox->m_PropertyData.m_fFogHorzeDensityNight = pSlider->GetPos() / 100.0f;
		}
		break;
	case IDC_SLIDER_HORZE_POWER:
		if (IsDay())
		{
			pBox->m_PropertyData.m_fFogHorzePower = pSlider->GetPos() / 100.0f;
		}
		else
		{
			pBox->m_PropertyData.m_fFogHorzePowerNight = pSlider->GetPos() / 100.0f;
		}
		break;
	case IDC_SLIDER_HEIGHT_DENSITY:
		if (IsDay())
		{
			pBox->m_PropertyData.m_fFogDeepness = pSlider->GetPos() / 100.0f;
		}
		else
		{
			pBox->m_PropertyData.m_fFogDeepnessNight = pSlider->GetPos() / 100.0f;
		}
		break;
	}
	UpdateBox(pBox);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
void CFogSettingDlg::OnBnClickedRadioDay()
{
	UpdateControl();
}

void CFogSettingDlg::OnBnClickedRadioNight()
{
	UpdateControl();
}

void CFogSettingDlg::OnEnChangeEditNear()
{
	CSceneBoxArea* pBox = GetCurrentBox();
	if (!pBox)
		return;
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_NEAR);
	CString strText;
	pEdit->GetWindowText(strText);
	if (IsDay())
		pBox->m_PropertyData.m_fFogStart = (float) atof(strText);
	else
		pBox->m_PropertyData.m_fFogStartNight = (float) atof(strText);
	UpdateBox(pBox);

}

void CFogSettingDlg::OnEnChangeEditFar()
{
	CSceneBoxArea* pBox = GetCurrentBox();
	if (!pBox)
		return;
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_FAR);
	CString strText;
	pEdit->GetWindowText(strText);
	if (IsDay())
		pBox->m_PropertyData.m_fFogEnd = (float) atof(strText);
	else
		pBox->m_PropertyData.m_fFogEndNight = (float) atof(strText);

	UpdateBox(pBox);

}

void CFogSettingDlg::OnEnChangeEditHeight1()
{
	CSceneBoxArea* pBox = GetCurrentBox();
	if (!pBox)
		return;
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_HEIGHT1);
	CString strText;
	pEdit->GetWindowText(strText);
	if (IsDay())
		pBox->m_PropertyData.m_fFogHeightStart = (float) atof(strText);
	else
		pBox->m_PropertyData.m_fFogHeightStartNight = (float) atof(strText);

	UpdateBox(pBox);
}

void CFogSettingDlg::OnEnChangeEditHeight2()
{
	CSceneBoxArea* pBox = GetCurrentBox();
	if (!pBox)
		return;
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_HEIGHT2);
	CString strText;
	pEdit->GetWindowText(strText);
	if (IsDay())
		pBox->m_PropertyData.m_fFogHeightEnd = (float) atof(strText);
	else
		pBox->m_PropertyData.m_fFogHeightEndNight = (float) atof(strText);

	UpdateBox(pBox);
}

void CFogSettingDlg::UpdateDialog()
{
	if (g_pDlg)
	{
		g_pDlg->UpdateControl();
	}
}

void CFogSettingDlg::UpdateBox(CSceneBoxArea* pBox)
{
	pBox->UpdateMapParams(AUX_GetMainFrame()->GetMap(), 100);

	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if (pMap)
	{
		CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
		APtrList<CSceneObject *>* pListSceneObjectSelected = &pManager->m_listSelectedObject;
		if (pListSceneObjectSelected->GetCount() == 1 && pListSceneObjectSelected->GetHead() == pBox)
		{
			pBox->UpdateProperty(false);
			AUX_GetMainFrame()->GetToolTabWnd()->UpdatePropertyData(false);        
		}
	}
}
void CFogSettingDlg::OnBnClickedButtonImport()
{
	CSceneBoxArea* pBox = GetCurrentBox();
	if (!pBox)
		return;

	char szFilter[] = "全局雾参数(*.fog)|*.fog||";
	DWORD dwFlags = OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_NOCHANGEDIR;
	CFileDialog	FileDia(TRUE, ".fog", NULL, dwFlags, szFilter, NULL);

	if (FileDia.DoModal() == IDCANCEL)
		return;


	AIniFile IniFile;
	if (!IniFile.Open(FileDia.GetPathName()))
	{
		return;
	}

	pBox->m_PropertyData.m_fFogStart = IniFile.GetValueAsFloat("Day", "m_fFogStart", pBox->m_PropertyData.m_fFogStart);
	pBox->m_PropertyData.m_fFogEnd = IniFile.GetValueAsFloat("Day", "m_fFogEnd", pBox->m_PropertyData.m_fFogEnd);
	pBox->m_PropertyData.m_fFogHeightStart = IniFile.GetValueAsFloat("Day", "m_fFogHeightStart", pBox->m_PropertyData.m_fFogHeightStart);
	pBox->m_PropertyData.m_fFogHeightEnd = IniFile.GetValueAsFloat("Day", "m_fFogHeightEnd", pBox->m_PropertyData.m_fFogHeightEnd);
	pBox->m_PropertyData.m_fFogDensityStart = (DWORD) IniFile.GetValueAsInt("Day", "m_fFogDensityStart", pBox->m_PropertyData.m_fFogDensityStart*100) /100.0f;
	pBox->m_PropertyData.m_fFogColorDensityStart = (DWORD) IniFile.GetValueAsInt("Day", "m_fFogColorDensityStart", pBox->m_PropertyData.m_fFogColorDensityStart*100) /100.0f;
	pBox->m_PropertyData.m_fFogDensityEnd = (DWORD) IniFile.GetValueAsInt("Day", "m_fFogDensityEnd", pBox->m_PropertyData.m_fFogDensityEnd*100) /100.0f;
	pBox->m_PropertyData.m_fFogColorDensityEnd = (DWORD) IniFile.GetValueAsInt("Day", "m_fFogColorDensityEnd", pBox->m_PropertyData.m_fFogColorDensityEnd*100) /100.0f;
	pBox->m_PropertyData.m_fFogHorzeDensity = (DWORD) IniFile.GetValueAsInt("Day", "m_fFogHorzeDensity", pBox->m_PropertyData.m_fFogHorzeDensity*100) /100.0f;
	pBox->m_PropertyData.m_fFogHorzePower = (DWORD) IniFile.GetValueAsInt("Day", "m_fFogHorzePower", pBox->m_PropertyData.m_fFogHorzePower*100) /100.0f;
	pBox->m_PropertyData.m_fFogDeepness = (DWORD) IniFile.GetValueAsInt("Day", "m_fFogDeepness", pBox->m_PropertyData.m_fFogDeepness*100) /100.0f;
	pBox->m_PropertyData.m_dwFogColor = (DWORD) IniFile.GetValueAsInt("Day", "m_dwFogColor", pBox->m_PropertyData.m_dwFogColor);
	pBox->m_PropertyData.m_dwFogColor2 = (DWORD) IniFile.GetValueAsInt("Day", "m_dwFogColor2", pBox->m_PropertyData.m_dwFogColor2);

	pBox->m_PropertyData.m_fFogStartNight = IniFile.GetValueAsFloat("Night", "m_fFogStartNight", pBox->m_PropertyData.m_fFogStartNight);
	pBox->m_PropertyData.m_fFogEndNight = IniFile.GetValueAsFloat("Night", "m_fFogEndNight", pBox->m_PropertyData.m_fFogEndNight);
	pBox->m_PropertyData.m_fFogHeightStartNight = IniFile.GetValueAsFloat("Night", "m_fFogHeightStartNight", pBox->m_PropertyData.m_fFogHeightStartNight);
	pBox->m_PropertyData.m_fFogHeightEndNight = IniFile.GetValueAsFloat("Night", "m_fFogHeightEndNight", pBox->m_PropertyData.m_fFogHeightEndNight);
	pBox->m_PropertyData.m_fFogDensityStartNight = (DWORD) IniFile.GetValueAsInt("Night", "m_fFogDensityStartNight", pBox->m_PropertyData.m_fFogDensityStartNight*100) /100.0f;
	pBox->m_PropertyData.m_fFogColorDensityStartNight = (DWORD) IniFile.GetValueAsInt("Night", "m_fFogColorDensityStartNight", pBox->m_PropertyData.m_fFogColorDensityStartNight*100) /100.0f;
	pBox->m_PropertyData.m_fFogDensityEndNight = (DWORD) IniFile.GetValueAsInt("Night", "m_fFogDensityEndNight", pBox->m_PropertyData.m_fFogDensityEndNight*100) /100.0f;
	pBox->m_PropertyData.m_fFogColorDensityEndNight = (DWORD) IniFile.GetValueAsInt("Night", "m_fFogColorDensityEndNight", pBox->m_PropertyData.m_fFogColorDensityEndNight*100) /100.0f;
	pBox->m_PropertyData.m_fFogHorzeDensityNight = (DWORD) IniFile.GetValueAsInt("Night", "m_fFogHorzeDensityNight", pBox->m_PropertyData.m_fFogHorzeDensityNight*100) /100.0f;
	pBox->m_PropertyData.m_fFogHorzePowerNight = (DWORD) IniFile.GetValueAsInt("Night", "m_fFogHorzePowerNight", pBox->m_PropertyData.m_fFogHorzePowerNight*100) /100.0f;
	pBox->m_PropertyData.m_fFogDeepnessNight = (DWORD) IniFile.GetValueAsInt("Night", "m_fFogDeepnessNight", pBox->m_PropertyData.m_fFogDeepnessNight*100) /100.0f;
	pBox->m_PropertyData.m_dwFogColorNight = (DWORD) IniFile.GetValueAsInt("Night", "m_dwFogColorNight", pBox->m_PropertyData.m_dwFogColorNight);
	pBox->m_PropertyData.m_dwFogColor2Night = (DWORD) IniFile.GetValueAsInt("Night", "m_dwFogColor2Night", pBox->m_PropertyData.m_dwFogColor2Night);

	IniFile.Close();

	UpdateControl();
}

void CFogSettingDlg::OnBnClickedButtonExport()
{
	CSceneBoxArea* pBox = GetCurrentBox();
	if (!pBox)
		return;

	char szFilter[] = "全局雾参数(*.fog)|*.fog||";
	DWORD dwFlags = OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_NOCHANGEDIR;
	CFileDialog	FileDia(TRUE, ".fog", NULL, dwFlags, szFilter, NULL);

	if (FileDia.DoModal() == IDCANCEL)
		return;
	
	FILE* fp = fopen(FileDia.GetPathName(), "w");
	if (fp == NULL)
	{
		MessageBox("创建文件失败");
		return;
	}

	fprintf(fp, "[Day]\n");
	fprintf(fp, "m_fFogStart=%g\n", pBox->m_PropertyData.m_fFogStart);
	fprintf(fp, "m_fFogEnd=%g\n", pBox->m_PropertyData.m_fFogEnd);
	fprintf(fp, "m_fFogHeightStart=%g\n", pBox->m_PropertyData.m_fFogHeightStart);
	fprintf(fp, "m_fFogHeightEnd=%g\n", pBox->m_PropertyData.m_fFogHeightEnd);
	fprintf(fp, "m_fFogDensityStart=%d\n", (int)(pBox->m_PropertyData.m_fFogDensityStart * 100));
	fprintf(fp, "m_fFogColorDensityStart=%d\n", (int)(pBox->m_PropertyData.m_fFogColorDensityStart * 100));
	fprintf(fp, "m_fFogDensityEnd=%d\n", (int)(pBox->m_PropertyData.m_fFogDensityEnd * 100));
	fprintf(fp, "m_fFogColorDensityEnd=%d\n", (int)(pBox->m_PropertyData.m_fFogColorDensityEnd * 100));
	fprintf(fp, "m_fFogHorzeDensity=%d\n", (int)(pBox->m_PropertyData.m_fFogHorzeDensity * 100));
	fprintf(fp, "m_fFogHorzePower=%d\n", (int)(pBox->m_PropertyData.m_fFogHorzePower * 100));
	fprintf(fp, "m_fFogDeepness=%d\n", (int)(pBox->m_PropertyData.m_fFogDeepness * 100));
	fprintf(fp, "m_dwFogColor=%d\n", pBox->m_PropertyData.m_dwFogColor);
	fprintf(fp, "m_dwFogColor2=%d\n", pBox->m_PropertyData.m_dwFogColor2);

	fprintf(fp, "[Night]");
	fprintf(fp, "m_fFogStartNight=%g\n", pBox->m_PropertyData.m_fFogStartNight);
	fprintf(fp, "m_fFogEndNight=%g\n", pBox->m_PropertyData.m_fFogEndNight);
	fprintf(fp, "m_fFogHeightStartNight=%g\n", pBox->m_PropertyData.m_fFogHeightStartNight);
	fprintf(fp, "m_fFogHeightEndNight=%g\n", pBox->m_PropertyData.m_fFogHeightEndNight);
	fprintf(fp, "m_fFogDensityStartNight=%d\n", (int)(pBox->m_PropertyData.m_fFogDensityStartNight * 100));
	fprintf(fp, "m_fFogColorDensityStartNight=%d\n", (int)(pBox->m_PropertyData.m_fFogColorDensityStartNight * 100));
	fprintf(fp, "m_fFogDensityEndNight=%d\n", (int)(pBox->m_PropertyData.m_fFogDensityEndNight * 100));
	fprintf(fp, "m_fFogColorDensityEndNight=%d\n", (int)(pBox->m_PropertyData.m_fFogColorDensityEndNight * 100));
	fprintf(fp, "m_fFogHorzeDensityNight=%d\n", (int)(pBox->m_PropertyData.m_fFogHorzeDensityNight * 100));
	fprintf(fp, "m_fFogHorzePowerNight=%d\n", (int)(pBox->m_PropertyData.m_fFogHorzePowerNight * 100));
	fprintf(fp, "m_fFogDeepnessNight=%d\n", (int)(pBox->m_PropertyData.m_fFogDeepnessNight * 100));
	fprintf(fp, "m_dwFogColorNight=%d\n", (int)pBox->m_PropertyData.m_dwFogColorNight);
	fprintf(fp, "m_dwFogColor2Night=%d\n", (int)pBox->m_PropertyData.m_dwFogColor2Night);

	fclose(fp);
}
