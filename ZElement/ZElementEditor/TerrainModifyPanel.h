#if !defined(AFX_TERRAINMODIFYPANEL_H__823A1B88_5CFF_438E_83F9_DF3475777459__INCLUDED_)
#define AFX_TERRAINMODIFYPANEL_H__823A1B88_5CFF_438E_83F9_DF3475777459__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TerrainModifyPanel.h : header file
//

#include "TerrainStretchOperation.h"

/////////////////////////////////////////////////////////////////////////////
// CTerrainModifyPanel dialog

class CTerrainModifyPanel : public CDialog
{
// Construction
public:
	CTerrainModifyPanel(CWnd* pParent = NULL);   // standard constructor
	void CheckOtherButtonCheck(CButton *pButton);
	void RiseUpAllButton();
// Dialog Data
	//{{AFX_DATA(CTerrainModifyPanel)
	enum { IDD = IDD_TERRAIN_MODIFY };
	CComboBox	m_cbSelPassMapLayer;
	CComboBox	m_cbSelPPassMapLayer;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTerrainModifyPanel)
	protected:
	virtual void OnOK(){};
	virtual void OnCancel(){};
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CTerrainModifyPanel)
	afx_msg void OnTerrainModifyStretch();
	afx_msg void OnTerrainModifyEmpty();
	afx_msg void OnTerrainModifySmooth();
	afx_msg void OnTerrainModifyNoise();
	afx_msg void OnTerrainModifyLaymask();
	afx_msg void OnTerrainModifyLaymaskSoft();
	afx_msg void OnTerrainModifyPlant();
	afx_msg void OnTerrainObstructRed();
	afx_msg void OnTerrainObstructGreen();
	afx_msg void OnTerrainObstructConvex();
	afx_msg void OnTerrainObstructMapTrans();
	afx_msg void OnTerrainObstructMapSetseed();
	afx_msg void OnTerrainObstructYellow();
	afx_msg void OnTerrainObstructMapSpaceCreate();
	afx_msg void OnTerrainObstructSaveAllMap();
	afx_msg void OnTerrainObstructGreen2();
	afx_msg void OnTerrainObstructRed2();
	afx_msg void OnTerrainObstructMapSetseed2();
	afx_msg void OnTerrainObstructSaveAllMap2();
	afx_msg void OnTerrainObstructYellow2();
	afx_msg void OnTerrainObstructMapReset2();
	afx_msg void OnTerrainObstructNpcUnreachable();
	afx_msg void OnTerrainObstructSkyReach();
	afx_msg void OnTerrainObstructSkyUnreach();
	afx_msg void OnTerrainObstructSkyReachSetHeight();
	afx_msg void OnBrushTreeCheck();
	afx_msg void OnBrushTreeUncheck();
	afx_msg void OnBrushTreeSplit();
	afx_msg void OnBrushTreeMerge();
	afx_msg void OnBrushTreeRebuild();
	afx_msg void OnBrushTreeReloadModel();
	afx_msg void OnSelchangeComboSelPassmapLayer();
	afx_msg void OnSelchangeComboSelPpassmapLayer();
	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedTerrainModifyColormap();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
public:
	afx_msg void OnBnClickedTerrainModifyColorSmooth();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TERRAINMODIFYPANEL_H__823A1B88_5CFF_438E_83F9_DF3475777459__INCLUDED_)
