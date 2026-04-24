#pragma once

#include "NumEdit.h"

class Brush;

// CDlgBrushProperty dialog

class CDlgBrushProperty : public CDialog
{
	DECLARE_DYNAMIC(CDlgBrushProperty)

public:
	CDlgBrushProperty(Brush* pBrush, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgBrushProperty();

	// Dialog Data
	enum { IDD = IDD_DIALOG_BRUSH_PROPERTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg void OnEnChangeEditHard();
	afx_msg void OnNMReleasedcaptureSliderHard(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinHard(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnEnChangeEditDispelNum();
	afx_msg void OnNMReleasedcaptureSliderDispelNum(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnEnChangeEditDitherColor();
	afx_msg void OnNMReleasedcaptureSliderDitherColor(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnEnChangeEditDitherArea();
	afx_msg void OnNMReleasedcaptureSliderDitherArea(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnEnChangeEditDitherAngle();
	afx_msg void OnNMReleasedcaptureSliderDitherAngle(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnEnChangeEditDitherCircle();
	afx_msg void OnNMReleasedcaptureSliderDitherCircle(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnEnChangeEditDispel();
	afx_msg void OnNMReleasedcaptureSliderDispel(NMHDR *pNMHDR, LRESULT *pResult);


	DECLARE_MESSAGE_MAP()
protected:
	CNumEdit		m_wndEditHard;
	CSliderCtrl		m_wndSliderHard;
	CSpinButtonCtrl	m_wndSpinHard;

	CNumEdit		m_wndEditDispelNum;
	CSliderCtrl		m_wndSliderDispelNum;
	BOOL			m_bPressureDispelNum;

	CNumEdit		m_wndEditDitherColor;
	CSliderCtrl		m_wndSliderDitherColor;
	BOOL			m_bPressureDitherColor;

	CNumEdit		m_wndEditDitherArea;
	CSliderCtrl		m_wndSliderDitherArea;
	BOOL			m_bPressureDitherArea;

	CNumEdit		m_wndEditDitherAngle;
	CSliderCtrl		m_wndSliderDitherAngle;
	BOOL			m_bPressureDitherAngle;

	CNumEdit		m_wndEditDitherCircle;
	CSliderCtrl		m_wndSliderDitherCircle;
	BOOL			m_bPressureDitherCircle;

	CNumEdit		m_wndEditDispel;
	CSliderCtrl		m_wndSliderDispel;

	BOOL			m_bTurnX;
	BOOL			m_bTurnY;
public:
	afx_msg void OnBnClickedButtonSelectBrush();
protected:
	CString m_strBrushName;
	Brush*	m_pBrush;
public:
	afx_msg void OnBnClickedCheckDitherArea();
	afx_msg void OnBnClickedCheckDispelNum();
	afx_msg void OnBnClickedCheckDitherColor();
	afx_msg void OnBnClickedCheckDitherAngle();
	afx_msg void OnBnClickedCheckDitherCircle();
	afx_msg void OnBnClickedCheckTurnx();
	afx_msg void OnBnClickedCheckTurny();
};
