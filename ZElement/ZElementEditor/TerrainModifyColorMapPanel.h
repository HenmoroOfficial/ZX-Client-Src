#pragma once
#include "afxwin.h"
#include "SelColorWnd.h"
#include "afxcmn.h"


// CTerrainModifyColorMapPanel dialog

class CTerrainModifyColorMapPanel : public CDialog
{
	DECLARE_DYNAMIC(CTerrainModifyColorMapPanel)

public:
	CTerrainModifyColorMapPanel(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTerrainModifyColorMapPanel();

	// Dialog Data
	enum { IDD = IDD_TERRAIN_MODIFY_COLORMAP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK(){}
	virtual void OnCancel(){}
	virtual BOOL OnInitDialog();
	afx_msg HRESULT OnColorChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNMReleasedcaptureSliderPower(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditPower();
	afx_msg void OnDeltaposSpinPower(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeComboPaintType();
	afx_msg void OnCbnSelchangeComboMapSize();	
	afx_msg void OnBnClickedButtonGetCurSize();
	afx_msg void OnBnClickedButtonBrush();

	void SelMapSize(int size);
	DECLARE_MESSAGE_MAP()
protected:
	CComboBox m_wndComboPaintType;
	CComboBox m_wndComboMapSize;
	CSelColorWnd m_wndSelColor;
	CSliderCtrl m_wndSliderPower;
	CEdit m_wndEditPower;
	CSpinButtonCtrl m_wndSpinPower;
public:
	void SetColor(DWORD dwColor);
};
