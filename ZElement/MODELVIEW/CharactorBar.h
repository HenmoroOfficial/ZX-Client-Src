#if !defined(AFX_CHARACTORBAR_H__B02DE75D_1B32_4F9A_8D0A_9E2C0468312F__INCLUDED_)
#define AFX_CHARACTORBAR_H__B02DE75D_1B32_4F9A_8D0A_9E2C0468312F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CharactorBar.h : header file
//
#include "InitDialogBar.h"

/////////////////////////////////////////////////////////////////////////////
// CCharactorBar dialog

class CCharactorBar : public CInitDialogBar
{
// Construction
public:
	CCharactorBar();   // standard constructor
	void Init();
	void UpdatePart(CString *pSkinPart);
	
	CComboBox* GetComboShape(){return &m_comboShape;}
	void      AddShape(CString strShapeName);
	void      ClearShapes();
	void      SetDefaultShape();
// Dialog Data
	//{{AFX_DATA(CCharactorBar)
	enum { IDD = IDD_DIALOG_CHARACTER };
	CComboBox	m_comboShape;
	CString	m_strHair;
	CString	m_strHead;
	CString	m_strFashionFoot;
	CString	m_strFashionHeadwear;
	CString	m_strFashionUpper;
	CString	m_strFoot;
	CString	m_strHelm;
	CString	m_strUpper;
	CString	m_strWeapon;
	CString	m_strRide;
	CString	m_strEar;
	CString	m_strFashionEar;
	CString	m_strFashionTail;
	CString	m_strTail;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCharactorBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	

    void OnUpdateButtonEar(CCmdUI *pCmdUI);
	void OnUpdateButtonTail(CCmdUI *pCmdUI);
	void OnUpdateButtonFashionEar(CCmdUI *pCmdUI);
	void OnUpdateButtonFashionTail(CCmdUI *pCmdUI);

	void OnUpdateButtonHead(CCmdUI *pCmdUI);
	void OnUpdateButtonHair(CCmdUI *pCmdUI);
	void OnUpdateButtonShoe(CCmdUI *pCmdUI);
	void OnUpdateButtonUpper(CCmdUI *pCmdUI);
	void OnUpdateButtonLower(CCmdUI *pCmdUI);

	void OnUpdateButtonHelm(CCmdUI *pCmdUI);
	void OnUpdateButtonGlasses(CCmdUI *pCmdUI);
	void OnUpdateButtonNose(CCmdUI *pCmdUI);
	void OnUpdateButtonMustache(CCmdUI *pCmdUI);
	void OnUpdateButtonCloak(CCmdUI *pCmdUI);
	void OnUpdateButtonFashionHeadwear(CCmdUI *pCmdUI);
	void OnUpdateButtonFashionUpper(CCmdUI *pCmdUI);
	void OnUpdateButtonFashionLower(CCmdUI *pCmdUI);
	void OnUpdateButtonFashionShoe(CCmdUI *pCmdUI);
	void OnUpdateButtonMount(CCmdUI *pCmdUI);
	void OnUpdateButtonMountRide(CCmdUI *pCmdUI);
	// Generated message map functions
	//{{AFX_MSG(CCharactorBar)
	afx_msg void OnButtonHead();
	afx_msg void OnButtonHair();
	afx_msg void OnButtonShoe();
	afx_msg void OnButtonUpper();
	afx_msg void OnButtonLower();
	afx_msg void OnButtonHelm();
	afx_msg void OnButtonGlasses();
	afx_msg void OnButtonNose();
	afx_msg void OnButtonMustache();
	afx_msg void OnButtonCloak();
	afx_msg void OnButtonFashionHeadwear();
	afx_msg void OnButtonFashionUpper();
	afx_msg void OnButtonFashionLower();
	afx_msg void OnButtonFashionShoe();
	afx_msg void OnButtonMount();
	afx_msg void OnRadioNormal();
	afx_msg void OnRadioFashion();
	afx_msg void OnButtonMountRide();
	afx_msg void OnButtonEar();
	afx_msg void OnButtonTail();
	afx_msg void OnButtonFashionEar();
	afx_msg void OnButtonFashionTail();
	afx_msg void OnEditchangeComboShape();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARACTORBAR_H__B02DE75D_1B32_4F9A_8D0A_9E2C0468312F__INCLUDED_)
