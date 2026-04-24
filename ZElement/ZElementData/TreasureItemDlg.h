#if !defined(AFX_TREASUREITEMDLG_H__BD4D6BBB_FCDB_4C31_8D63_A0D0F1215DFF__INCLUDED_)
#define AFX_TREASUREITEMDLG_H__BD4D6BBB_FCDB_4C31_8D63_A0D0F1215DFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TreasureItemDlg.h : header file

#include "Treasure.h"
#include "AObject.h"
//
class CPropertyList;
class ADynPropertyObject;
class CTemplIDSel;



struct MULTILINE_TEXT_FUNCS : public CUSTOM_FUNCS
{
	~MULTILINE_TEXT_FUNCS() {}
	virtual BOOL CALLBACK OnActivate(void);
	virtual LPCTSTR CALLBACK OnGetShowString(void) const;
	virtual AVariant CALLBACK OnGetValue(void) const;
	virtual void CALLBACK OnSetValue(const AVariant& var);
	
	AVariant	m_var;
	CString		m_strText;
};

struct MASK_SEL_FUNCS : public CUSTOM_FUNCS
{
	~MASK_SEL_FUNCS() {}
	virtual BOOL CALLBACK OnActivate(void);
	virtual LPCTSTR CALLBACK OnGetShowString(void) const;
	virtual AVariant CALLBACK OnGetValue(void) const;
	virtual void CALLBACK OnSetValue(const AVariant& var);
	
	AVariant	m_var;
	CString		m_strText;
};

//Added 2011-11-07.
struct VALID_TIME_SEL_FUNCS : public CUSTOM_FUNCS
{
	~VALID_TIME_SEL_FUNCS() {}
	virtual BOOL CALLBACK OnActivate(void);
	virtual LPCTSTR CALLBACK OnGetShowString(void) const;
	virtual AVariant CALLBACK OnGetValue(void) const;
	virtual void CALLBACK OnSetValue(const AVariant& var);
	BOOL CALLBACK OnGetAllValues( int& nType, int& nStartTime, int& nEndTime, int& nParam ); //获得四个变量的值，不同于OnGetValue接口是：自动解析m_var；
	void CALLBACK OnSetAllValues( int nType, int nStartTime, int nEndTime, int nParam ); //设置四个变量的值，不同于OnSetValue接口是：自动解析m_var；

	AVariant	m_var;
	CString		m_strText;
};
//Added end.

/////////////////////////////////////////////////////////////////////////////
// CTreasureItemDlg dialog

class CTreasureItemDlg : public CDialog
{
// Construction
public:
	CTreasureItemDlg(CWnd* pParent = NULL);   // standard constructor
	TREASURE_ITEM_VERSION11 m_Item;
	void SetReadOnly(bool bReadOnly = true){ m_bReadOnly = bReadOnly; }
// Dialog Data
	//{{AFX_DATA(CTreasureItemDlg)
	enum { IDD = IDD_DIALOG_TREASURE_ITEM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreasureItemDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	//}}AFX_VIRTUAL
	
// Implementation
protected:
	ADynPropertyObject *m_pProperty;
	CPropertyList *m_pList;
	CTemplIDSel* m_pListIDSel;
	MULTILINE_TEXT_FUNCS * m_pMultiLineFunc;
	MASK_SEL_FUNCS*	m_pMaskDiscount;
	MASK_SEL_FUNCS*	m_pMaskSale;
	VALID_TIME_SEL_FUNCS* m_pValidTime;	//Added 2011-11-07.

	CTemplIDSel*			m_pListIDSel_ForPresent;
	MULTILINE_TEXT_FUNCS*	m_pMultiLineFunc_ForPresent;
	MULTILINE_TEXT_FUNCS*	m_pMultiLineFunc_ForKey;

	bool m_bReadOnly;
	void BuildProperty();
	void UpdateProperty(bool bGet);
	// Generated message map functions
	//{{AFX_MSG(CTreasureItemDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREASUREITEMDLG_H__BD4D6BBB_FCDB_4C31_8D63_A0D0F1215DFF__INCLUDED_)
