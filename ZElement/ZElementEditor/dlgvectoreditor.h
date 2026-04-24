#pragma once
//#include "EffectPropRes.h"

// CDlgVectorEditor dialog
struct VectorXProperty;

class CDlgVectorEditor : public CDialog
{
	DECLARE_DYNAMIC(CDlgVectorEditor)

public:
	CDlgVectorEditor(VectorXProperty* pVectorXProperty, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgVectorEditor();

// Dialog Data
	enum { IDD = IDD_DIALOG_VECTORX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnBnClickedOk();
	afx_msg void OnDestroy();
protected:
	VectorXProperty* m_pVectorXProperty;
};
