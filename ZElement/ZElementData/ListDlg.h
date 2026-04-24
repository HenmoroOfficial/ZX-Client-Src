#if !defined(AFX_LISTDLG_H__E0AE7C8F_409F_405F_9E3E_D2FC05605528__INCLUDED_)
#define AFX_LISTDLG_H__E0AE7C8F_409F_405F_9E3E_D2FC05605528__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListDlg.h : header file
//

#include "TreeFlagMan.h"
#include "MltiTree.h"
/////////////////////////////////////////////////////////////////////////////
// CListDlg dialog

class CListDlg : public CDialog
{
// Construction
	enum FileState
	{
		FS_FOLDER_FIX,		// 有效模板文件夹
		FS_FOLDER_EMPTY,	// 非有效模板空文件夹（没有模板文件，可能被删除）
		FS_FOLDER,			// 有效模板下面的分类目录
		FS_FOLDER_CHECKOUT,
		FS_FILE,			// 普通模板文件

		FS_FILE_CHECKOUT,
		FS_UNEDITABLE,		// 老编辑器新模板
		FS_NE_OF,			// new editor，old file
		FS_NE_OF_CHECKOUT,	// new editor，old file checkout
	};
public:
	CListDlg(CWnd* pParent = NULL);   // standard constructor
	// 列出所有子目录
	void EnumObjects(CString strPathName , HTREEITEM hTreeItemp, bool bProperty, int iCount ,bool bOnlyFirst = false);
	// 列出目录下所有文件
	void EnumFiles(CString strPathName, HTREEITEM hTreeItemp, bool bProperty, bool bOnlyFirst = false);
	void DeleteAllItems();				// ???
	// 创建模板过程中根据path和*.org判断创建的path是目录还是模板
	bool IsCreateFolder(CString path);
	// 不是最底层目录的模板显示为文件夹，函数名字有点奇怪
	bool IsHideFiles(CString path);
	bool CanShowTemplate(CString path);	// 当前编辑器是否可以显示模板
	// 更新快捷菜单中哪些可用哪些不可用
	void UpdateMenu(CMenu *pMenu);
	// 是否属于根模板项
	bool IsFixedItem(CString path);
	// 记录哪些项是展开的
	void UpdateRecord();
	
	// 打开编辑器或刷新数据以后刷新显示目录 iCount:层
	void FreshObjectList(CString strPathName , HTREEITEM hTreeItemp, int iCount);
	HTREEITEM InsertItem(CTreeCtrl *pTree, HTREEITEM hRoot, CString name);

	// 选中一个目录，查找其中id或名字
	HTREEITEM FindTempl(HTREEITEM hParent, const CString& strTempl);
	HTREEITEM FindTemplByID(HTREEITEM hParent, unsigned long ulID);
	HTREEITEM FindNext(HTREEITEM hPrev, const CString& strTempl);
	void OnFindNext(const CString& str);
// Dialog Data
	//{{AFX_DATA(CListDlg)
	enum { IDD = IDD_DIALOG_LIST };
	CMultiTree	m_Tree;
	//}}AFX_DATA
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void EnumCheck(HTREEITEM root, bool bCheckOut, bool bAppend);
	// file:模板路径，BaseData/....
	void CheckOut(HTREEITEM item, CString pathName, CString fileName);
	void CheckIn(HTREEITEM item, CString pathName, CString fileName);
	void UpdateItemState(HTREEITEM item, CString pathName, CString fileName, int iVersion, bool bIsFolder=true);
	CString GetItemPath(HTREEITEM hItem);
	void AssureDirectory(CString path);
	void AssureVSSPath(CString path);
	CImageList m_ImageList;
	bool bRclick;
	// Generated message map functions
	//{{AFX_MSG(CListDlg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnRclickTreeSortList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCreateObject();
	afx_msg void OnClickTreeProperty(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkTreeSortList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRdblclkTreeProperty(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRdblclkTreeSortList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCreateObjectExt();
	afx_msg void OnUpdateCreateObjectExt(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCreateObject(CCmdUI* pCmdUI);
	afx_msg void OnVssCheckIn();
	afx_msg void OnVssCheckOut();
	afx_msg void OnSelchangedTreeSortList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingTreeSortList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditTemplateProperty();
	afx_msg void OnDestroy();
	afx_msg void OnModifiedObjectExt();
	virtual void OnOK(){};
	virtual void OnCancel(){};
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnVssCheckOutAll();
	afx_msg void OnVssCheckInAll();
	afx_msg void OnClickTreeSortList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemexpandedTreeSortList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnVssUndoCheckOut();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTDLG_H__E0AE7C8F_409F_405F_9E3E_D2FC05605528__INCLUDED_)
