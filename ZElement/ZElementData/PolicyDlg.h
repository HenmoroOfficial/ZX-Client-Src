#if !defined(AFX_POLICYDLG_H__4226DD46_5D03_42DA_ABCF_BF2890AA9373__INCLUDED_)
#define AFX_POLICYDLG_H__4226DD46_5D03_42DA_ABCF_BF2890AA9373__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PolicyDlg.h : header file
//

#include "MltiTree.h"

class CPolicyData;
class CTriggerData;

/////////////////////////////////////////////////////////////////////////////
// CPolicyDlg dialog

class CPolicyDlg : public CDialog
{
// Construction
public:
	struct TIMER_TEST
	{
		unsigned int id;

		unsigned int counter;
		unsigned int skipCounter;
		
		unsigned int period; 
		unsigned int last_time;
	};

	struct TRIGGER_STATUS
	{
		unsigned int id;
		bool         bActive;
		bool         bRun;
	};
	
	CPolicyDlg(CWnd* pParent = NULL);   // standard constructor

	bool LoadIDTemplate();
	void EnumFiles(CString strPathName, HTREEITEM hTreeItemp);
	CString GetTriggerName( unsigned int id);
	unsigned int m_nPolicyID;
	CPolicyData *m_pCurrentPolicy;
	CString     m_strCurrentPathName;
	bool        m_bReadOnly;
	bool        m_bStartTest;

	//事件触发
	bool	m_bSkipKillPlayer;
	bool	m_bSkipRandom;
	bool	m_bSkipStartAttack;
	bool    m_bSkipBorn;
	bool    m_bSkipDied;
	bool    m_bPathEndPoint;
	bool    m_bAttackBySkill;
// Dialog Data
	//{{AFX_DATA(CPolicyDlg)
	enum { IDD = IDD_DIALOG_POLICY };
	CEdit	m_editRunLog;
	CListBox	m_listTrigger;
	CMultiTree	m_Tree;
	float	m_fHp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPolicyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnCancel();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void UpdateMenu(CMenu *pMenu);
	void FreshTriggerList();
	bool TraverselTree( void *pTree , CTriggerData *pTrigger);
	
	//删除冗余的执行触发器
	void DelRedundancy();
	bool TriggerIsUse( unsigned int id);
	bool TraceTrigger( CTriggerData *pTrigger,unsigned int id);
	
	//模拟函数
	bool IsTimeCome(unsigned int iId);
	bool HpLess( float fLess);
	bool KillPlayer();
	bool Born();
	bool Died();
	bool Random(float fProbability);
	bool StartAttack();
	void AddLogMsg(const char *szMsg);
	bool RunOperaion(void *pTriggerPtr, void *pOperation);
	void RunTrigger(unsigned int id);
	void SetTriggerActiveStatus( unsigned int id, bool bActive);
	void SetTriggerRunStatus( unsigned int id, bool bRun);
	bool GetTriggerActiveStatus( unsigned int id);
	bool GetTriggerRunStatus( unsigned int id);
	bool IsPathEndPoint();
	bool IsAttackBySkill();
	abase::vector<TIMER_TEST> listTimer;
	abase::vector<TRIGGER_STATUS> listGriggerStatus;
	bool m_bTriggerDisable;
	
	
	CImageList m_ImageList;
	// Generated message map functions
	//{{AFX_MSG(CPolicyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRclickTreePolicy(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCreatePolicy();
	afx_msg void OnPolicyCheckIn();
	afx_msg void OnPolicyCheckOut();
	afx_msg void OnButtonDelTrigger();
	afx_msg void OnButtonAddTrigger();
	afx_msg void OnButtonMoveUp();
	afx_msg void OnButtonMoveDown();
	afx_msg void OnButtonPolicySave();
	afx_msg void OnDblclkTreePolicy(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListTrigger();
	afx_msg void OnButtonRunTrigger();
	afx_msg void OnButtonStopTrigger();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonEventRandom();
	afx_msg void OnButtonEventKillplayer();
	afx_msg void OnButtonEventAttackstart();
	afx_msg void OnButtonEventBorn();
	afx_msg void OnButtonEventDied();
	afx_msg void OnButtonEventEndPoint();
	afx_msg void OnButtonEventAttackSkill();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POLICYDLG_H__4226DD46_5D03_42DA_ABCF_BF2890AA9373__INCLUDED_)
