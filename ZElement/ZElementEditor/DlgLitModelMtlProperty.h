#pragma once

#include "../../EditorShare/EffectProperty/EffectProperty.h"

// CEffectProperty dialog

class CDlgLitModelMtlProperty : public CEffectProperty
{
	DECLARE_DYNAMIC(CDlgLitModelMtlProperty)

public:
	CDlgLitModelMtlProperty(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgLitModelMtlProperty();

	enum{
		MI_NAME = 0,
		MI_RELATIVETEX,
		MI_MTLNAME,
	};
// Dialog Data
	enum { IDD = IDD_MTLPROPERTY };
	CListCtrl	m_MtlList;

	void AddMtlToList(LPCSTR szName, A3DTexture* pTexture);
	void ClearAllInfo();
	void SetLitModel(A3DLitModel* pLitModel);
	bool ReplaceTexture(A3DTexture* pCurTexture, A3DTexture* pNewTextrure);
	void UpdateMtlList();
	void DeleteMtl(int nListItem, bool bSetToDefault);

protected:
	A3DLitModel* m_pLitModel;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnDestroy();
	virtual BOOL OnInitDialog();
	
	afx_msg void OnMenuRestorePrevMtl();
	afx_msg void OnMenuSetOrChangeMtl();
	afx_msg void OnMenuDeleteMtl();
	afx_msg void OnRClickedMtl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg	void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnItemchangedMtllist(NMHDR *pNMHDR, LRESULT *pResult);
};
