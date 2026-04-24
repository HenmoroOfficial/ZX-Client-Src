#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CTerrainModifyColorSmoothPanel dialog

class CTerrainModifyColorSmoothPanel : public CDialog
{
	DECLARE_DYNAMIC(CTerrainModifyColorSmoothPanel)

public:
	CTerrainModifyColorSmoothPanel(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTerrainModifyColorSmoothPanel();

// Dialog Data
	enum { IDD = IDD_TERRAIN_MODIFY_COLOR_SMOOTH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_editBrushSmooth;
	CSliderCtrl m_sliderBrushSmooth;
	afx_msg void OnEnChangeEditBrushsmooth();
	afx_msg void OnNMReleasedcaptureSliderBrushsmooth(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
};
