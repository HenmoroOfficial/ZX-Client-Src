#if !defined(AFX_PARTDLG_H__B67E856A_BF74_4E52_AFB1_0CCE038E5D0A__INCLUDED_)
#define AFX_PARTDLG_H__B67E856A_BF74_4E52_AFB1_0CCE038E5D0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PartDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPartDlg dialog
enum 
{
	PART_HEAD = 0,			// 头
	PART_HAIR,				// 头发
	PART_EAR,               //烈山耳朵
	PART_TAIL,              //烈山尾巴
	PART_BODY,				// 普通衣服
	PART_FOOT,				// 普通鞋子
	PART_HELM,				// 普通头盔
	//PART_GLASSES,			// 眼镜
	//PART_NOSE,				// 鼻子
	//PART_MUSTACHE,			// 胡子
	//PART_CLOAK,				// 披风
	PART_FASHION_UPPER_BODY,// 时装上衣
	//PART_FASHION_LOWER,		// 时装裤子
	PART_FASHION_FOOT,		// 时装鞋子
	PART_FASHION_HEADWEAR,	// 时装头饰
	PART_FASHION_EAR,        //烈山时装耳朵
	PART_FASHION_TAIL,		 //烈山时装尾巴
	PART_WEAPON,
	PART_RIDE,
	NUM_PART_INDEX,
};//与player.h 中的	Skin index 保持严格的一致


enum
{
	FILTER_GENDER = 0,//性别过滤
	NUM_FILTER,
};



class CPartDlg : public CDialog
{
// Construction
public:
	CPartDlg(CWnd* pParent = NULL);   // standard constructor


	int     GetPart(){return m_nPart;}
	CString GetOldSkin(){return m_strOldSkin;}
	void SetPartType( int nPart , CString strOldSkin ,bool bFilter = false);
	void InitFilter();
// Dialog Data
	//{{AFX_DATA(CPartDlg)
	enum { IDD = IDD_DIALOG_PART };
	CListBox	m_listPart;
	CString	m_strFilter;
	//}}AFX_DATA
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPartDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_nPart;   //替换部分
	int m_nFilter; //过滤器
	CString m_strOldSkin;
	bool m_bShowPathPart;

	bool m_bFilterMan;
	bool m_bFilterWoman;
	

	void SetSkinSel(CString sel);
	void InitHead(bool bFilter);
	void InitHair(bool bFilter);
	void InitBody(bool bFilter);
	void InitFoot(bool bFilter);
	void InitHelm(bool bFilter);
	void InitEar(bool bFilter);
	void InitTail(bool bFilter);
	//void InitGlasses(bool bFilter);
	//void InitNose(bool bFilter);
	//void InitMustache(bool bFilter);
	//void InitCloak(bool bFilter);
	void InitFashionUpper(bool bFilter);
	//void InitFashionLower(bool bFilter);
	void InitFashionFoot(bool bFilter);
	void InitFashionHeadwear(bool bFilter);
	void InitFashionEar(bool bFilter);
	void InitFashionTail(bool bFilter);
	void InitWeapon(bool bFilter);
	void InitRide(bool bFilter);

	void GetEquipNameAndSetListItemData(void* pData,CString& strName,bool bFilter);

	
	bool Filter(WORD data);
	
	void ClearAllItem();
	// Generated message map functions
	//{{AFX_MSG(CPartDlg)
	afx_msg void OnSelchangeListPart();
	afx_msg void OnButtonFilter();
	afx_msg void OnCheckMan();
	afx_msg void OnCheckWoman();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARTDLG_H__B67E856A_BF74_4E52_AFB1_0CCE038E5D0A__INCLUDED_)
