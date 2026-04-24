#if !defined(AFX_EFFECTPROPERTY_H__75BA4E38_B4CB_4B56_907B_39227F60E1F2__INCLUDED_)
#define AFX_EFFECTPROPERTY_H__75BA4E38_B4CB_4B56_907B_39227F60E1F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageScaleBone.h : header file
//

//#include "WndInput.h"
//#include "WndCombo.h"
#include "Resource.h"
#include <A3DHLSL.h>
#include <a3deffect.h>
#include <AProperty.h>
#include <map>
#include <vector>

#define IDM_RELOADMTL                     10001
#define IDM_DELETEMTL                     10002
#define IDM_COPYMTL                       10003
#define IDM_PASTEMTL                      10004

class CModel;
class A3DEffect;
class CPropertyList;
class ADynPropertyObject;
struct CUSTOM_FUNCS;

typedef std::map<int, CUSTOM_FUNCS*> IndexCustomFuncsMap;
typedef std::vector<int> IndexArray;

/////////////////////////////////////////////////////////////////////////////

enum CONST_TYPE
{
	CT_UNKNOWN,
	CT_COLOR,
	CT_SLIDER,
};

typedef abase::hash_map<AString, AString> ParamDict;

struct MTLLIST_DATA
{
	A3DTexture*  pShaderTex;
	ParamDict    paramDict;
	AString      strPrevMtlFile;
	AString      strPrevMtlMacros;
	unsigned int bModified : 1;
	bool HoldEffectParam(A3DEffect* pEffect);
	bool RestoreEffectParam(A3DEffect* pEffect);

	bool SavePrevMtl();
};

//CONST_TYPE GetConstType(const char* pszName);
AString GetHLSLName(A3DTexture* pTexture);

class CEffectProperty : public CDialog
{
public:		//	Types

	////	Sub item index of bone list
	enum{
		MI_NAME = 0,
		MI_RELATIVETEX,
		MI_MTLNAME,
	};

	enum{
		CI_NAME = 0,
		CI_TYPE,
		CI_VALUE,
	};
	struct EFFECT_UI_ELEMENT	// 这个是为了提高loadPropData和savePropData里的调用效率搞的
	{
		AString		ConstName;
		AString		UIName;
		int			nIdx;
		CONST_TYPE	Type;
		ARange*		pRange;
		EFFECT_UI_ELEMENT();
		void Cleanup();
	};
	typedef abase::vector<EFFECT_UI_ELEMENT> UIElementArray;

	struct EFFECT_UI	// 为了提高loadPropData和savePropData调用效率,把应该在PropDlg里显示的属性预先放在UIElementArray中
	{
		MTLLIST_DATA*	pMtlListData;
		A3DEffect*		pCurEffect;
		UIElementArray	aElementArray;
		EFFECT_UI();
		void Cleanup();
		void SetEffect(MTLLIST_DATA* _pMtlListData);
	};

public:		//	Construction
	CEffectProperty(A3DEngine* pA3DEngine, UINT uIDTemplate, CWnd* pParent = NULL);   // standard constructor
	virtual ~CEffectProperty();
private:		//	Construction
	//enum { IDD = IDD_MTLPROPERTY };
	//CListCtrl	m_MtlList;
	CListBox	m_MacroList;
	CStatic		m_Approximately;
	CPropertyList*	m_pPropertyList;
	IndexCustomFuncsMap m_mapIndexCustomFuncs;
	IndexArray m_arrPropIndex;
	ADynPropertyObject* m_pProperty;

public:

	//bool BindModel(CModel* pModel);
	//bool Command(const char* cmd, WPARAM wParam, LPARAM lParam);
	bool	m_bShowPrgParam;

//public:
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

private:	//	Attributes

	//CModel*			m_pModel;
	EFFECT_UI		m_EffectUI;
	//bool            m_bAlphaTexture;
	A3DEngine*		m_pA3DEngine;
protected:	//	Operations

	//void ClearAllInfo();
	//void UpdateMtlList();

	// Generated message map functions
	//{{AFX_MSG(CPageScaleBone)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//afx_msg void OnItemchangedMtl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMacroSelChange();
	//afx_msg void OnMenuSetOrChangeMtl();
	//afx_msg void OnMenuRestorePrevMtl();
	//afx_msg void OnMenuReloadMtl();
	//afx_msg void OnMenuDeleteMtl();
	//afx_msg void OnMenuCopyMtl();
	//afx_msg void OnMenuPasteMtl();
	afx_msg void OnUpdateAlphaTex(CCmdUI* pCmdUI);
	afx_msg void OnAlphaTex();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg LRESULT OnPropListDataChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPropListDataChanging(WPARAM wParam, LPARAM lParam);

	void clearPropList();
	void loadPropData();
	void savePropData();
	void UpdateFocusMtlPropData();
	//void DeleteMtl(int nListItem, bool bSetToDefault);
	void UpdateMacroList(bool bForce);
	void GetSelectedMacro(AStringArray& aMacros);
	void SetSelectedMacro(const AStringArray& aMacros);
	//void AddMtlToList(LPCSTR pszName, A3DTexture* pTexture);
	A3DHLSLDataSrc* GetDataSrc();
	MTLLIST_DATA*	GetCurMtlListData();
public:
	void SetEffectUI(MTLLIST_DATA* pMtlListData);

	//	On activate / disactivate this page
//#ifdef _ANGELICA22
//	afx_msg LRESULT OnActivatePage(WPARAM wParam, LPARAM lParam);
//#else
//	afx_msg void OnActivatePage(WPARAM wParam, LPARAM lParam);
//#endif

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EFFECTPROPERTY_H__75BA4E38_B4CB_4B56_907B_39227F60E1F2__INCLUDED_)
