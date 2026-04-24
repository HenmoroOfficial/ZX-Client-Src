#if !defined(AFX_ANIMATIONBAR_H__1BCEC55D_783B_4DEF_BC53_1DEFDD81B292__INCLUDED_)
#define AFX_ANIMATIONBAR_H__1BCEC55D_783B_4DEF_BC53_1DEFDD81B292__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnimationBar.h : header file
//
#include "InitDialogBar.h"
/////////////////////////////////////////////////////////////////////////////
// CAnimationBar dialog

class CAnimationBar : public CInitDialogBar
{
// Construction
public:
	CAnimationBar();   // standard constructor
	void AddAction(CString strAction);
	void AddSkill( CString strSkill, int id);
	void ClearActions();
	void ClearSkill();
	void SetDefaultSel();
	void SetSkillDefaultSel();
	CString GetSelAction();
	int GetSelSkill();
	CString GetSillActionName( int _id_skill, int _id_weapon );
	//void UpdateButton();
	void SetActionCurSel(CString strAction);
	bool IsPauseButtonEnabled();
	void EnablePauseButton();
	void DisablePauseButton();
	void UpdateStopButton(bool);
	int GetSkillCategory();
// Dialog Data
	//{{AFX_DATA(CAnimationBar)
	enum { IDD = IDD_DIALOG_ANIMATION_BAR };
	CComboBox	m_cbSkill;
	CSliderCtrl	m_SpeedCtrl;
	CComboBox	m_cbAction;
	BOOL	m_bOccupLife;
	BOOL	m_bOccupFamily;
	BOOL	m_bOccupSect;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnimationBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void OnUpdateButtonPause(CCmdUI *pCmdUI);
	void OnUpdateButtonSkillPlay(CCmdUI *pCmdUI);
	void OnUpdateButtonStop(CCmdUI *pCmdUI);
	BOOL	m_bLoop;
	bool    m_bPauseButtonEnabled;
	// Generated message map functions
	//{{AFX_MSG(CAnimationBar)
	afx_msg void OnButtonPause();
	afx_msg void OnSelchangeComboAction();
	afx_msg void OnReleasedcaptureSliderSpeed(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOutofmemorySliderSpeed(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckLoop();
	afx_msg void OnSelchangeComboSkill();
	afx_msg void OnButtonSkillPlay();
	afx_msg void OnButtonStop();
	afx_msg void OnCheckOccupLife();
	afx_msg void OnCheckOccupFamily();
	afx_msg void OnCheckOccupSect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANIMATIONBAR_H__1BCEC55D_783B_4DEF_BC53_1DEFDD81B292__INCLUDED_)
