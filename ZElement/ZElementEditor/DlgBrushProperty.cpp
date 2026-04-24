// DlgBrushProperty.cpp : implementation file
//

#include "stdafx.h"
#include "ElementEditor.h"
#include "DlgBrushProperty.h"
#include "Brush.h"
#include "TerrainBrushSelDlg.h"
#include "Global.h"

#include <AAssist.h>

// CDlgBrushProperty dialog

IMPLEMENT_DYNAMIC(CDlgBrushProperty, CDialog)

CDlgBrushProperty::CDlgBrushProperty(Brush* pBrush, CWnd* pParent /*=NULL*/)
: CDialog(CDlgBrushProperty::IDD, pParent)
, m_strBrushName(_T("默认刷子"))
, m_pBrush(pBrush)
{

}

CDlgBrushProperty::~CDlgBrushProperty()
{
}

void CDlgBrushProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_HARD, m_wndSliderHard);
	DDX_Control(pDX, IDC_SPIN_HARD, m_wndSpinHard);
	DDX_Control(pDX, IDC_EDIT_HARD, m_wndEditHard);

	DDX_Control(pDX, IDC_EDIT_DISPEL_NUM, m_wndEditDispelNum);
	DDX_Control(pDX, IDC_SLIDER_DISPEL_NUM, m_wndSliderDispelNum);
	DDX_Check(pDX, IDC_CHECK_DISPEL_NUM, m_bPressureDispelNum);

	DDX_Control(pDX, IDC_EDIT_DITHER_COLOR, m_wndEditDitherColor);
	DDX_Control(pDX, IDC_SLIDER_DITHER_COLOR, m_wndSliderDitherColor);
	DDX_Check(pDX, IDC_CHECK_DITHER_COLOR, m_bPressureDitherColor);

	DDX_Control(pDX, IDC_EDIT_DITHER_AREA, m_wndEditDitherArea);
	DDX_Control(pDX, IDC_SLIDER_DITHER_AREA, m_wndSliderDitherArea);
	DDX_Check(pDX, IDC_CHECK_DITHER_AREA, m_bPressureDitherArea);

	DDX_Control(pDX, IDC_EDIT_DITHER_ANGLE, m_wndEditDitherAngle);
	DDX_Control(pDX, IDC_SLIDER_DITHER_ANGLE, m_wndSliderDitherAngle);
	DDX_Check(pDX, IDC_CHECK_DITHER_ANGLE, m_bPressureDitherAngle);

	DDX_Control(pDX, IDC_EDIT_DITHER_CIRCLE, m_wndEditDitherCircle);
	DDX_Control(pDX, IDC_SLIDER_DITHER_CIRCLE, m_wndSliderDitherCircle);
	DDX_Check(pDX, IDC_CHECK_DITHER_CIRCLE, m_bPressureDitherCircle);

	DDX_Control(pDX, IDC_EDIT_DISPEL, m_wndEditDispel);
	DDX_Control(pDX, IDC_SLIDER_DISPEL, m_wndSliderDispel);

	DDX_Check(pDX, IDC_CHECK_TURNX, m_bTurnX);
	DDX_Check(pDX, IDC_CHECK_TURNY, m_bTurnY);
	DDX_Text(pDX, IDC_EDIT_BRUSH_NAME, m_strBrushName);
}


BEGIN_MESSAGE_MAP(CDlgBrushProperty, CDialog)
	ON_EN_CHANGE(IDC_EDIT_HARD, &CDlgBrushProperty::OnEnChangeEditHard)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_HARD, &CDlgBrushProperty::OnNMReleasedcaptureSliderHard)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_HARD, &CDlgBrushProperty::OnDeltaposSpinHard)

	ON_EN_CHANGE(IDC_EDIT_DISPEL_NUM, &CDlgBrushProperty::OnEnChangeEditDispelNum)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_DISPEL_NUM, &CDlgBrushProperty::OnNMReleasedcaptureSliderDispelNum)

	ON_EN_CHANGE(IDC_EDIT_DITHER_COLOR, &CDlgBrushProperty::OnEnChangeEditDitherColor)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_DITHER_COLOR, &CDlgBrushProperty::OnNMReleasedcaptureSliderDitherColor)

	ON_EN_CHANGE(IDC_EDIT_DITHER_AREA, &CDlgBrushProperty::OnEnChangeEditDitherArea)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_DITHER_AREA, &CDlgBrushProperty::OnNMReleasedcaptureSliderDitherArea)

	ON_EN_CHANGE(IDC_EDIT_DITHER_ANGLE, &CDlgBrushProperty::OnEnChangeEditDitherAngle)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_DITHER_ANGLE, &CDlgBrushProperty::OnNMReleasedcaptureSliderDitherAngle)

	ON_EN_CHANGE(IDC_EDIT_DITHER_CIRCLE, &CDlgBrushProperty::OnEnChangeEditDitherCircle)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_DITHER_CIRCLE, &CDlgBrushProperty::OnNMReleasedcaptureSliderDitherCircle)

	ON_EN_CHANGE(IDC_EDIT_DISPEL, &CDlgBrushProperty::OnEnChangeEditDispel)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_DISPEL, &CDlgBrushProperty::OnNMReleasedcaptureSliderDispel)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_BRUSH, &CDlgBrushProperty::OnBnClickedButtonSelectBrush)
	ON_BN_CLICKED(IDC_CHECK_DITHER_AREA, &CDlgBrushProperty::OnBnClickedCheckDitherArea)
	ON_BN_CLICKED(IDC_CHECK_DISPEL_NUM, &CDlgBrushProperty::OnBnClickedCheckDispelNum)
	ON_BN_CLICKED(IDC_CHECK_DITHER_COLOR, &CDlgBrushProperty::OnBnClickedCheckDitherColor)
	ON_BN_CLICKED(IDC_CHECK_DITHER_ANGLE, &CDlgBrushProperty::OnBnClickedCheckDitherAngle)
	ON_BN_CLICKED(IDC_CHECK_DITHER_CIRCLE, &CDlgBrushProperty::OnBnClickedCheckDitherCircle)
	ON_BN_CLICKED(IDC_CHECK_TURNX, &CDlgBrushProperty::OnBnClickedCheckTurnx)
	ON_BN_CLICKED(IDC_CHECK_TURNY, &CDlgBrushProperty::OnBnClickedCheckTurny)
END_MESSAGE_MAP()

BOOL CDlgBrushProperty::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_wndSliderHard.SetRange(0, 100);
	m_wndSliderDispelNum.SetRange(0, 500);
	m_wndSliderDitherColor.SetRange(0, 100);
	m_wndSliderDitherArea.SetRange(0, 100);
	m_wndSliderDitherAngle.SetRange(0, 360);
	m_wndSliderDitherCircle.SetRange(0, 60);
	m_wndSliderDispel.SetRange(0, 10);
	if(!m_pBrush)
		return FALSE;
	CString csText;
	csText.Format("%d", m_pBrush->GetHard());
	m_wndEditHard.SetWindowText(csText);

	csText.Format("%d", m_pBrush->GetDispelNum());
	m_wndEditDispelNum.SetWindowText(csText);

	csText.Format("%d", m_pBrush->GetDitherColor());
	m_wndEditDitherColor.SetWindowText(csText);

	csText.Format("%d", m_pBrush->GetDitherArea());
	m_wndEditDitherArea.SetWindowText(csText);

	csText.Format("%d", m_pBrush->GetDitherAngle());
	m_wndEditDitherAngle.SetWindowText(csText);

	csText.Format("%d", m_pBrush->GetDitherCircle());
	m_wndEditDitherCircle.SetWindowText(csText);

	csText.Format("%d", m_pBrush->GetDispel());
	m_wndEditDispel.SetWindowText(csText);

	if(!g_bWTPacket)
	{
		CWnd *pwnd = GetDlgItem(IDC_CHECK_DISPEL_NUM);
		pwnd->EnableWindow(FALSE);
		pwnd = GetDlgItem(IDC_CHECK_DITHER_COLOR);
		pwnd->EnableWindow(FALSE);
		pwnd = GetDlgItem(IDC_CHECK_DITHER_AREA);
		pwnd->EnableWindow(FALSE);
		pwnd = GetDlgItem(IDC_CHECK_DITHER_ANGLE);
		pwnd->EnableWindow(FALSE);
		pwnd = GetDlgItem(IDC_CHECK_DITHER_CIRCLE);
		pwnd->EnableWindow(FALSE);
	}

	m_strBrushName = m_pBrush->GetBrushName();
	m_bTurnX = m_pBrush->IsTurnX();
	m_bTurnY = m_pBrush->IsTurnY();
	m_bPressureDispelNum = m_pBrush->IsPressureDispelNum();
	m_bPressureDitherColor = m_pBrush->IsPressureDitherColor();
	m_bPressureDitherArea = m_pBrush->IsPressureDitherArea();
	m_bPressureDitherAngle = m_pBrush->IsPressureDitherAngle();
	m_bPressureDitherCircle = m_pBrush->IsPressureDitherCircle();
	UpdateData(FALSE);
	return TRUE;
}

void CDlgBrushProperty::OnEnChangeEditHard()
{
	CString csText;
	m_wndEditHard.GetWindowText(csText);
	int iHard = atoi(csText);
	int iOldHard = iHard;
	a_Clamp(iHard, 0, 100);
	if(iOldHard != iHard)
	{
		csText.Format("%d",iHard);
		m_wndEditHard.SetWindowText(csText);
	}
	m_wndSliderHard.SetPos(iHard);
	if(m_pBrush)
		m_pBrush->SetHard(iHard);
}

void CDlgBrushProperty::OnNMReleasedcaptureSliderHard(NMHDR *pNMHDR, LRESULT *pResult)
{
	int iPos = m_wndSliderHard.GetPos();
	CString csText;
	csText.Format("%d", iPos);
	m_wndEditHard.SetWindowText(csText);
	*pResult = 0;
}

void CDlgBrushProperty::OnDeltaposSpinHard(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	CString csText;
	m_wndEditHard.GetWindowText(csText);
	int iHard = atoi(csText);
	int iOldHard = iHard;
	iHard += -pNMUpDown->iDelta;
	a_Clamp(iHard, 0, 100);
	if(iOldHard != iHard)
	{
		csText.Format("%d", iHard);
		m_wndEditHard.SetWindowText(csText);
	}
	*pResult = 0;
}

void CDlgBrushProperty::OnEnChangeEditDispelNum()
{
	CString csText;
	m_wndEditDispelNum.GetWindowText(csText);
	int iDispelNum = atoi(csText);
	int iOldDispelNum = iDispelNum;
	a_Clamp(iDispelNum, 0, 500);
	if(iOldDispelNum != iDispelNum)
	{
		csText.Format("%d",iDispelNum);
		m_wndEditDispelNum.SetWindowText(csText);
	}
	m_wndSliderDispelNum.SetPos(500-iDispelNum);
	if(m_pBrush)
		m_pBrush->SetDispelNum(iDispelNum);
}

void CDlgBrushProperty::OnNMReleasedcaptureSliderDispelNum(NMHDR *pNMHDR, LRESULT *pResult)
{
	int iPos = m_wndSliderDispelNum.GetPos();
	CString csText;
	csText.Format("%d", 500-iPos);
	m_wndEditDispelNum.SetWindowText(csText);
	*pResult = 0;
}

void CDlgBrushProperty::OnEnChangeEditDitherColor()
{
	CString csText;
	m_wndEditDitherColor.GetWindowText(csText);
	int iDitherColor = atoi(csText);
	int iOldDitherColor = iDitherColor;
	a_Clamp(iDitherColor, 0, 100);
	if(iOldDitherColor != iDitherColor)
	{
		csText.Format("%d",iDitherColor);
		m_wndEditDitherColor.SetWindowText(csText);
	}
	m_wndSliderDitherColor.SetPos(iDitherColor);
	if(m_pBrush)
		m_pBrush->SetDitherColor(iDitherColor);
}

void CDlgBrushProperty::OnNMReleasedcaptureSliderDitherColor(NMHDR *pNMHDR, LRESULT *pResult)
{
	int iPos = m_wndSliderDitherColor.GetPos();
	CString csText;
	csText.Format("%d", iPos);
	m_wndEditDitherColor.SetWindowText(csText);
	*pResult = 0;
}

void CDlgBrushProperty::OnEnChangeEditDitherArea()
{
	CString csText;
	m_wndEditDitherArea.GetWindowText(csText);
	int iDitherArea = atoi(csText);
	int iOldDitherArea = iDitherArea;
	a_Clamp(iDitherArea, 0, 100);
	if(iOldDitherArea != iDitherArea)
	{
		csText.Format("%d",iDitherArea);
		m_wndEditDitherArea.SetWindowText(csText);
	}
	m_wndSliderDitherArea.SetPos(iDitherArea);
	if(m_pBrush)
		m_pBrush->SetDitherArea(iDitherArea);
}

void CDlgBrushProperty::OnNMReleasedcaptureSliderDitherArea(NMHDR *pNMHDR, LRESULT *pResult)
{
	int iPos = m_wndSliderDitherArea.GetPos();
	CString csText;
	csText.Format("%d", iPos);
	m_wndEditDitherArea.SetWindowText(csText);
	*pResult = 0;
}

void CDlgBrushProperty::OnEnChangeEditDitherAngle()
{
	CString csText;
	m_wndEditDitherAngle.GetWindowText(csText);
	int iDitherAngle = atoi(csText);
	int iOldDitherAngle = iDitherAngle;
	a_Clamp(iDitherAngle, 0, 360);
	if(iOldDitherAngle != iDitherAngle)
	{
		csText.Format("%d",iDitherAngle);
		m_wndEditDitherAngle.SetWindowText(csText);
	}
	m_wndSliderDitherAngle.SetPos(iDitherAngle);
	if(m_pBrush)
		m_pBrush->SetDitherAngle(iDitherAngle);
}

void CDlgBrushProperty::OnNMReleasedcaptureSliderDitherAngle(NMHDR *pNMHDR, LRESULT *pResult)
{
	int iPos = m_wndSliderDitherAngle.GetPos();
	CString csText;
	csText.Format("%d", iPos);
	m_wndEditDitherAngle.SetWindowText(csText);
	*pResult = 0;
}

void CDlgBrushProperty::OnEnChangeEditDitherCircle()
{
	CString csText;
	m_wndEditDitherCircle.GetWindowText(csText);
	int iDitherCircle = atoi(csText);
	int iOldDitherCircle = iDitherCircle;
	a_Clamp(iDitherCircle, 0, 60);
	if(iOldDitherCircle != iDitherCircle)
	{
		csText.Format("%d",iDitherCircle);
		m_wndEditDitherCircle.SetWindowText(csText);
	}
	m_wndSliderDitherCircle.SetPos(iDitherCircle);
	if(m_pBrush)
		m_pBrush->SetDitherCircle(iDitherCircle);
}

void CDlgBrushProperty::OnNMReleasedcaptureSliderDitherCircle(NMHDR *pNMHDR, LRESULT *pResult)
{
	int iPos = m_wndSliderDitherCircle.GetPos();
	CString csText;
	csText.Format("%d", iPos);
	m_wndEditDitherCircle.SetWindowText(csText);
	*pResult = 0;
}

void CDlgBrushProperty::OnEnChangeEditDispel()
{
	CString csText;
	m_wndEditDispel.GetWindowText(csText);
	int iDispel = atoi(csText);
	int iOldDispel = iDispel;
	a_Clamp(iDispel, 0, 10);
	if(iOldDispel != iDispel)
	{
		csText.Format("%d",iDispel);
		m_wndEditDispel.SetWindowText(csText);
	}
	m_wndSliderDispel.SetPos(iDispel);
	if(m_pBrush)
		m_pBrush->SetDispel(iDispel);
}

void CDlgBrushProperty::OnNMReleasedcaptureSliderDispel(NMHDR *pNMHDR, LRESULT *pResult)
{
	int iPos = m_wndSliderDispel.GetPos();
	CString csText;
	csText.Format("%d", iPos);
	m_wndEditDispel.SetWindowText(csText);
	*pResult = 0;
}

void CDlgBrushProperty::OnBnClickedButtonSelectBrush()
{
	if(!m_pBrush)
		return;
	CTerrainBrushSelDlg dlg;
	dlg.m_strBrushPath = m_strBrushName; 
	if(IDOK != dlg.DoModal())
		return;	
	if(dlg.m_strBrushPath.CompareNoCase("默认刷子") == 0)
	{
		m_strBrushName = dlg.m_strBrushPath;
		m_pBrush->SetBrushName(m_strBrushName);
		m_pBrush->UseDefaultBrush(true);
		UpdateData(FALSE);
		return;
	}
	if(m_strBrushName.CompareNoCase(dlg.m_strBrushPath) == 0)
		return;

	m_strBrushName = dlg.m_strBrushPath;
	m_pBrush->SetBrushName(m_strBrushName);

	CString csBrushPath;
	csBrushPath.Format("%sBrush\\%s", g_szWorkDir, dlg.m_strBrushPath);
	HBITMAP hbmp = (HBITMAP)LoadImage(NULL, csBrushPath, IMAGE_BITMAP, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);

	if(NULL == hbmp)
	{
		CString msg("加载刷子位图失败--");
		msg = msg + dlg.m_strBrushPath;
		m_pBrush->UseDefaultBrush(true);
		m_strBrushName = "默认刷子"; 
		UpdateData(FALSE);
		MessageBox(msg);
		m_pBrush->SetBrushName(m_strBrushName);
		return;
	}
	BITMAP bmp;
	GetObject(hbmp,sizeof(BITMAP),&bmp);
	int bit = bmp.bmBitsPixel;
	if(bit<8) 
	{
		DeleteObject(hbmp);
		m_pBrush->UseDefaultBrush(true);
		m_strBrushName = "默认刷子"; 
		UpdateData(FALSE);
		MessageBox("Only support more than 8 bit bitmap");
		m_pBrush->SetBrushName(m_strBrushName);
		return;
	}

	COLORREF *pData = new COLORREF[bmp.bmWidth*bmp.bmHeight*(bit/8)];
	ASSERT(pData);
	CBitmap bt;
	bt.Attach(hbmp);
	bt.GetBitmapBits(bmp.bmWidth*bmp.bmHeight*(bit/8),pData);

	m_pBrush->SetBrushData(pData, bmp.bmWidth, bmp.bmHeight);

	delete []pData;
	DeleteObject(hbmp);
	UpdateData(FALSE);
	m_pBrush->UseDefaultBrush(false);
}

void CDlgBrushProperty::OnBnClickedCheckDispelNum()
{
	if(!m_pBrush)
		return;
	UpdateData();
	m_pBrush->EnablePressureDispelNum(m_bPressureDispelNum?true:false);
}

void CDlgBrushProperty::OnBnClickedCheckDitherColor()
{
	if(!m_pBrush)
		return;
	UpdateData();
	m_pBrush->EnablePressureDitherColor(m_bPressureDitherColor?true:false);
}

void CDlgBrushProperty::OnBnClickedCheckDitherArea()
{
	if(!m_pBrush)
		return;
	UpdateData();
	m_pBrush->EnablePressureDitherArea(m_bPressureDitherArea?true:false);
}

void CDlgBrushProperty::OnBnClickedCheckDitherAngle()
{
	if(!m_pBrush)
		return;
	UpdateData();
	m_pBrush->EnablePressureDitherAngle(m_bPressureDitherAngle?true:false);
}

void CDlgBrushProperty::OnBnClickedCheckDitherCircle()
{
	if(!m_pBrush)
		return;
	UpdateData();
	m_pBrush->EnablePressureDitherCircle(m_bPressureDitherCircle?true:false);
}

void CDlgBrushProperty::OnBnClickedCheckTurnx()
{
	if(!m_pBrush)
		return;
	UpdateData();
	m_pBrush->TurnX(m_bTurnX?true:false);
}

void CDlgBrushProperty::OnBnClickedCheckTurny()
{
	if(!m_pBrush)
		return;
	UpdateData();
	m_pBrush->TurnY(m_bTurnY?true:false);
}
