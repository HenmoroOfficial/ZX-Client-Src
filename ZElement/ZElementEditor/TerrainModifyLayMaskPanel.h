#if !defined(AFX_CTerrainModifyLayMaskPanel_H__22EA7C69_A362_442B_A01B_390F16659833__INCLUDED_)
#define AFX_CTerrainModifyLayMaskPanel_H__22EA7C69_A362_442B_A01B_390F16659833__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTerrainModifyLayMaskPanel.h : header file
//
#include "NumEdit.h"
/////////////////////////////////////////////////////////////////////////////
// CTerrainModifyLayMaskPanel dialog
class CTerrainModifyLayMaskPanel : public CDialog
{
// Construction
public:
	CTerrainModifyLayMaskPanel(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTerrainModifyLayMaskPanel)
	enum { IDD = IDD_TERRAIN_MODIFY_LAYMASK };
	CSliderCtrl	m_DitherGray;
	CSliderCtrl	m_DispelNum;
	CSliderCtrl	m_Dispel;
	CSliderCtrl	m_DitherCircle;
	CSliderCtrl	m_DitherArea;
	CSliderCtrl	m_DitherAngle;
	CListBox	m_listLayMask;
	CSpinButtonCtrl	m_spinBrushHard;
	CSpinButtonCtrl	m_spinBrushGray;
	CSliderCtrl	m_sliderBrushHard;
	CSliderCtrl	m_sliderBrushGray;
	CNumEdit	m_editBrushHard;
	CNumEdit	m_editBrushGray;
	CString	m_strUserBrushName;
	BOOL	m_bTurnX;
	BOOL	m_bTurnY;
	int		m_nDitherAngle;
	BOOL	m_bDitherAnglePress;
	BOOL	m_bDispelNum;
	BOOL	m_bDitherGray;
	int		m_nDitherGray;
	int		m_nDispelNum;
	BOOL	m_bDitherCircle;
	int		m_nDitherCircle;
	int		m_nDispel;
	int		m_nDitherArea;
	BOOL	m_bDitherArea;
	//}}AFX_DATA

	void UpdateLayList();
	void SetMaskLay(char* szLayName);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTerrainModifyLayMaskPanel)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
private:
	BYTE *m_pData;//This a user define's brush data;
	int  m_nWidth;
	int  m_nHeight;
	bool m_bUserBrush;
// Implementation
protected:
	virtual void OnOK(){};
	virtual void OnCancel() {};
	// Generated message map functions
	//{{AFX_MSG(CTerrainModifyLayMaskPanel)
	virtual BOOL OnInitDialog();
	afx_msg void OnReleasedcaptureSliderBrushhgray(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderBrushlayhard(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinBrushhgray(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinBrushlayhard(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEditBrushhgray();
	afx_msg void OnChangeEditBrushlayhard();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSelchangeTerrainModifyLaylist();
	afx_msg void OnFreshLayerList();
	afx_msg void OnRadioMaskpaintClrsub();
	afx_msg void OnRadioMaskpaintClradd();
	afx_msg void OnCheckUserBrush();
	afx_msg void OnLoadBrushRaw();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnReleasedcaptureSliderDitherAngle(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderDitherArea(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderDitherCircle(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderDispel(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckTurnx();
	afx_msg void OnCheckTurny();
	afx_msg void OnCheckDitherTurnx();
	afx_msg void OnCheckTurny2();
	afx_msg void OnReleasedcaptureSlider4DispelNum(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderDitherGray(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEditDitherAngle();
	afx_msg void OnCheckDitherAngle();
	afx_msg void OnCheckDispelNum();
	afx_msg void OnCheckDitherGray();
	afx_msg void OnChangeEditDispelNum();
	afx_msg void OnChangeEditDitherGray();
	afx_msg void OnChangeEditDitherCircle();
	afx_msg void OnCheckDitherCircle();
	afx_msg void OnChangeEditDispel();
	afx_msg void OnChangeEditDitherArea();
	afx_msg void OnCheckDitherArea2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CTerrainModifyLayMaskPanel_H__22EA7C69_A362_442B_A01B_390F16659833__INCLUDED_)
