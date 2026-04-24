#if !defined(AFX_PAGEMTLLIST_H__75BA4E38_B4CB_4B56_907B_39227F60E1F2__INCLUDED_)
#define AFX_PAGEMTLLIST_H__75BA4E38_B4CB_4B56_907B_39227F60E1F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageScaleBone.h : header file
//

#include "WndInput.h"
//#include "WndCombo.h"

class CModel;
class A3DEffect;
/////////////////////////////////////////////////////////////////////////////
// CPageMtlProperty dialog
// CPageScaleBone dialog



class CDlgMtlList : public CDialog
{
public:		//	Types

	////	Sub item index of bone list
	enum{
		MI_NAME = 0,
		MI_DESC,
		MI_RELATIVETEX,
	};

	enum{
		CI_NAME = 0,
		CI_TYPE,
		CI_VALUE,
	};

	////	Editing status
	//struct EDITSTATUS
	//{
	//	bool	bEditing;	//	true, be editing
	//	int		iWndID;		//	ID of window whose content was edited
	//	int		iItem;		//	Item index
	//	int		iSubItem;	//	Sub item index
	//};

public:		//	Construction
	CDlgMtlList(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CPageScaleBone)
	enum { IDD = IDD_MTLLIST };
	CListCtrl	m_MtlList;
	//CListCtrl	m_ConstList;
	//CEditFloat	m_EditVal[16];
	//CChooseColorButton m_ChooseClr;
	//CButton		m_Submit;
	//CButton		m_BrowseTexture;
	//CEdit		m_TexFilename;
	//CString	m_strWarning;
	//}}AFX_DATA

	AString m_strHLSLFilename;

public:		//	Attributes

public:		//	Operations

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:	//	Attributes
protected:	//	Operations

	virtual BOOL OnInitDialog();
	afx_msg void OnDBClickedMtl(NMHDR* pNMHDR, LRESULT* pResult);
	void UpdateMtlLib(const char* pFileExt, const char* szPrefix);

	//	On activate / disactivate this page
#ifdef _ANGELICA22
	afx_msg LRESULT OnActivatePage(WPARAM wParam, LPARAM lParam);
#else
	afx_msg void OnActivatePage(WPARAM wParam, LPARAM lParam);
#endif

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEMTLLIST_H__75BA4E38_B4CB_4B56_907B_39227F60E1F2__INCLUDED_)
