// CDlgLitModelMtlProperty.cpp : implementation file
//

#include "stdafx.h"
#include "../../EditorShare/EffectProperty/EPStdAfx.h"
#include "ElementEditor.h"
#include "DlgLitModelMtlProperty.h"
#include "render.h"
#include "DlgMtlList.h"
#include "mainfrm.h"
#include "viewFrame.h"
#include <a3dhlsldatasrc.h>
#define IDM_SETORCHANGEMTL              32812

// CDlgLitModelMtlProperty dialog

IMPLEMENT_DYNAMIC(CDlgLitModelMtlProperty, CEffectProperty)

CDlgLitModelMtlProperty::CDlgLitModelMtlProperty(CWnd* pParent /*=NULL*/)
	: CEffectProperty(g_Render.GetA3DEngine(), CDlgLitModelMtlProperty::IDD, pParent)
	, m_pLitModel(NULL)
{

}

CDlgLitModelMtlProperty::~CDlgLitModelMtlProperty()
{
}

void CDlgLitModelMtlProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MTLLIST, m_MtlList);
	CEffectProperty::DoDataExchange(pDX);
}

BOOL CDlgLitModelMtlProperty::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_MtlList.InsertColumn(MI_NAME, "材质名称", LVCFMT_LEFT, 70);
	m_MtlList.InsertColumn(MI_RELATIVETEX, "相关纹理", LVCFMT_LEFT, 170);
	m_MtlList.InsertColumn(MI_MTLNAME, "材质名", LVCFMT_LEFT, 140);
	m_MtlList.SetExtendedStyle(m_MtlList.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	CEffectProperty::OnInitDialog();
	UpdateMtlList();
	UpdateData(FALSE);
	return TRUE;
}

void CDlgLitModelMtlProperty::OnDestroy()
{
	ClearAllInfo();
	//clearPropList();
	//m_EffectUI.Cleanup();
	CEffectProperty::OnDestroy();
	//CDialog::OnDestroy();
}

void CDlgLitModelMtlProperty::OnMenuRestorePrevMtl()
{

}

void CDlgLitModelMtlProperty::OnMenuSetOrChangeMtl()
{
	CDlgMtlList DlgMtlList;
	AStringArray aStrings, aMacros;

	DlgMtlList.DoModal();

	if(DlgMtlList.m_strHLSLFilename.GetLength() == 0)
		return;

	int nFocus = m_MtlList.GetNextItem(-1, LVNI_FOCUSED);
	MTLLIST_DATA* pMtlListData = (MTLLIST_DATA*)m_MtlList.GetItemData(nFocus);

	A3DTexture* pCurTexture = pMtlListData->pShaderTex;
	A3DEffect* pEffect = NULL;

	pMtlListData->bModified = true;

	if(pCurTexture->GetClassID() == A3D_CID_HLSLSHADER)
	{
		pEffect = (A3DEffect*)pCurTexture;

		// 保存属性值
		pMtlListData->HoldEffectParam(pEffect);	// 保存参数设置
		GetSelectedMacro(aStrings);				// 保存宏选择
		pMtlListData->SavePrevMtl();

		if( ! pEffect->SetHLSL(DlgMtlList.m_strHLSLFilename))
		{
			MessageBox("加载Effect文件失败,可能是由于HLSL编译错误导致.","错误", MB_OK|MB_ICONERROR);
			OnMenuRestorePrevMtl();
			return;
		}
		m_MtlList.SetItemText(nFocus, MI_MTLNAME, GetHLSLName(pCurTexture));
	}
	else
	{
		AString strTexFile = pCurTexture->GetMapFile();
		pEffect = A3DEffect::CreateHLSLShader(g_Render.GetA3DDevice(), strTexFile, DlgMtlList.m_strHLSLFilename);
		if(pEffect == NULL)
		{
			MessageBox("加载Effect文件失败,可能是由于HLSL编译错误导致.","错误", MB_OK|MB_ICONERROR);
			return;
		}

		//pShaderTex->aParam.clear();
		pMtlListData->strPrevMtlFile.Empty();
		pMtlListData->pShaderTex = pEffect;
		AString strMtlFile = pEffect->GetMtlFilename();
		m_MtlList.SetItemText(nFocus, MI_RELATIVETEX, &strMtlFile[strMtlFile.ReverseFind('\\') + 1]);
		m_MtlList.SetItemText(nFocus, MI_MTLNAME, GetHLSLName(pEffect));

		//m_pModel->ReplaceTexture(pCurTexture, pEffect);
		ReplaceTexture(pCurTexture, pEffect);
		//g_Render.GetA3DEngine()->GetA3DTextureMan()->ReleaseTexture(pCurTexture);
	}
	A3DHLSLDataSrc* pDataSrc = GetDataSrc();
	if(pDataSrc != NULL)
	{
		pDataSrc->ProvingMacro(aStrings, aMacros);
		if(aMacros.size() > 0)  {
			pEffect->SetMacros(aMacros, NULL);
		}
	}
	UpdateMacroList(false);
	pMtlListData->RestoreEffectParam(pEffect);
	UpdateFocusMtlPropData(/*nFocus*/);
}

void CDlgLitModelMtlProperty::OnMenuDeleteMtl()
{
	if(MessageBox("将要直接删除这个模型上的材质设置和对应的MTL文件?注意:此操作不能Undo.", "确定", MB_YESNO | MB_ICONWARNING) != IDYES)
	{
		return;
	}

	int nFocus = m_MtlList.GetNextItem(-1, LVNI_FOCUSED);
	DeleteMtl(nFocus, true);
}

void CDlgLitModelMtlProperty::DeleteMtl(int nListItem, bool bSetToDefault)
{
	ASSERT(bSetToDefault);	// 不支持其他方式了
	MTLLIST_DATA* pShaderTex = (MTLLIST_DATA*)m_MtlList.GetItemData(nListItem);
	ASSERT(pShaderTex->pShaderTex->GetClassID() == A3D_CID_HLSLSHADER);

	A3DTextureMan* pTextureMan = g_Render.GetA3DEngine()->GetA3DTextureMan();
	A3DEffect* pEffect = (A3DEffect*)pShaderTex->pShaderTex;
	AString strMtlFilename = pEffect->GetMtlFilename();
	AString strOriginTex;
	A3DTexture* pOriginTex;

	if(bSetToDefault)
	{
		pEffect->SetHLSL("shaders\\2.2\\hlsl\\LitModel\\litmodel_mtl_default.hlsl");
		pOriginTex = pEffect;
		strOriginTex = pEffect->GetMtlFilename();
	}
	else
	{
		//strOriginTex = pEffect->GetMapFile();
		//pTextureMan->LoadTextureFromFile(pEffect->GetMapFile(), &pOriginTex);
		////m_pModel->ReplaceTexture(pEffect, pOriginTex);
		//pTextureMan->ReleaseTexture((A3DTexture*&)pEffect);
		ASSERT(0);
	}

	// 更新UI
	m_MtlList.SetItemText(nListItem, MI_MTLNAME, GetHLSLName(pOriginTex));
	m_MtlList.SetItemText(nListItem, MI_RELATIVETEX, &strOriginTex[strOriginTex.ReverseFind('\\') + 1]);

	clearPropList();
	pShaderTex->strPrevMtlFile.Empty();
	pShaderTex->pShaderTex = pOriginTex;
	//m_EffectUI.SetEffect(pShaderTex);
	SetEffectUI(pShaderTex);
	UpdateMacroList(false);
}

void CDlgLitModelMtlProperty::OnRClickedMtl(NMHDR* pNMHDR, LRESULT* pResult)
{
	//int nFocus = m_MtlList.GetNextItem(-1, LVNI_FOCUSED);
	//if(nFocus < 0)
	//	return;
	//MTLLIST_DATA* pShaderTex = (MTLLIST_DATA*)m_MtlList.GetItemData(nFocus);
	MTLLIST_DATA* pShaderTex = GetCurMtlListData();
	CMenu menu;
	POINT ptCursor;

	//menu.LoadMenu(IDR_MTLMENU);
	//CMenu* pSubMenu = menu.GetSubMenu(0);

	if(menu.CreatePopupMenu() == FALSE) {
		return;
	}

	MENUITEMINFO MenuItemInfo;
	memset(&MenuItemInfo, 0, sizeof(MENUITEMINFO));
	MenuItemInfo.cbSize		= sizeof(MENUITEMINFO);
	MenuItemInfo.fMask		= MIIM_TYPE | MIIM_ID;
	MenuItemInfo.fType		= MFT_STRING;				// used if MIIM_TYPE (4.0) or MIIM_FTYPE (>4.0)

	GetCursorPos(&ptCursor);

	MenuItemInfo.wID = IDM_SETORCHANGEMTL;
	MenuItemInfo.dwTypeData = "设置/更换纹理";
	menu.InsertMenuItem(IDM_SETORCHANGEMTL, &MenuItemInfo, FALSE);

	//MenuItemInfo.wID = IDM_RESTORE;
	//MenuItemInfo.dwTypeData = "还原上一个纹理";
	//menu.InsertMenuItem(IDM_RESTORE, &MenuItemInfo, FALSE);

	//menu.EnableMenuItem(IDM_RESTORE, pShaderTex->strPrevMtlFile.GetLength() != 0 ? MF_ENABLED : MF_DISABLED | MF_GRAYED);

	//#ifdef _DEBUG
	if(pShaderTex->pShaderTex == NULL && pShaderTex->pShaderTex->GetClassID() != A3D_CID_HLSLSHADER) {
		return;
	}

	AString strFilename = ((A3DEffect*)(pShaderTex->pShaderTex))->GetHLSLFilename();
	AString strFileSpec;

	// 如果文件名有前缀'!', 则认为是内部材质
	af_GetFileTitle(strFilename, strFileSpec);
	if(strFileSpec.CompareNoCase("!CheckTangentError.hlsl") == 0) {
		return;
	}

	if(strFileSpec.CompareNoCase("default.hlsl") != NULL)
	{

		MenuItemInfo.wID		= IDM_DELETEMTL;
		MenuItemInfo.dwTypeData = "删除材质";
		menu.InsertMenuItem(IDM_DELETEMTL, &MenuItemInfo, FALSE);

		MenuItemInfo.fType		= MFT_SEPARATOR;
		menu.InsertMenuItem(0, &MenuItemInfo, FALSE);
		MenuItemInfo.fType		= MFT_STRING;

		//MenuItemInfo.wID		= IDM_COPYMTL;
		//MenuItemInfo.dwTypeData = "复制材质参数";
		//menu.InsertMenuItem(IDM_COPYMTL, &MenuItemInfo, FALSE);

		//if(OpenClipboard())
		//{
		//	HANDLE hData = GetClipboardData(CF_TEXT);
		//	if(hData != NULL)
		//	{
		//		LPCSTR lpData = (LPCSTR)GlobalLock(hData);
		//		if(lpData != NULL && StrCmpN(lpData, ANGELICA_MTL_CLIPBOARDHEADER, lstrlen(ANGELICA_MTL_CLIPBOARDHEADER)) == 0)
		//		{
		//			MenuItemInfo.wID		= IDM_PASTEMTL;
		//			MenuItemInfo.dwTypeData = "粘贴材质参数";
		//			menu.InsertMenuItem(IDM_PASTEMTL, &MenuItemInfo, FALSE);
		//		}
		//		GlobalUnlock(hData);
		//	}
		//	CloseClipboard();
		//}

	}
	//#endif // _DEBUG
	menu.TrackPopupMenu(0, ptCursor.x, ptCursor.y, this);
	menu.DestroyMenu();
}

void CDlgLitModelMtlProperty::OnShowWindow(BOOL bShow, UINT nStatus)
{
	if(bShow) {
		SetTimer(1001, 53, NULL);
	}
	else {
		KillTimer(1001);
	}
}

void CDlgLitModelMtlProperty::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == 1001)
	{
		if(IsWindowVisible() == FALSE)
		{
			KillTimer(nIDEvent);
			return;
		}
		CViewFrame *pFrame = AUX_GetMainFrame()->GetViewFrame();
		if(pFrame != NULL)
		{
			pFrame->Render();
		}
	}
}

BEGIN_MESSAGE_MAP(CDlgLitModelMtlProperty, CEffectProperty)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_MTLLIST, &CDlgLitModelMtlProperty::OnLvnItemchangedMtllist)
	ON_NOTIFY(NM_RCLICK, IDC_MTLLIST, OnRClickedMtl)
	ON_COMMAND(IDM_SETORCHANGEMTL, OnMenuSetOrChangeMtl)
	ON_COMMAND(IDM_DELETEMTL, OnMenuDeleteMtl)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////
void CDlgLitModelMtlProperty::AddMtlToList(LPCSTR szName, A3DTexture* pTexture)
{
	int nItem = m_MtlList.InsertItem(0, szName);

	if(pTexture->GetClassID() == A3D_CID_HLSLSHADER)
	{
		A3DEffect* pHLSLShader = (A3DEffect*)pTexture;
		AString strMtlFilename = pHLSLShader->GetMtlFilename();

		m_MtlList.SetItemText(nItem, MI_RELATIVETEX, &strMtlFilename[strMtlFilename.ReverseFind('\\') + 1]);
		m_MtlList.SetItemText(nItem, MI_MTLNAME, GetHLSLName(pTexture));
	}
	else
	{
		AString strTextureFile = pTexture->GetMapFile();
		m_MtlList.SetItemText(nItem, MI_RELATIVETEX, &strTextureFile[strTextureFile.ReverseFind('\\') + 1]);
	}
	MTLLIST_DATA* pShaderTex = new MTLLIST_DATA;
	pShaderTex->pShaderTex = pTexture;
	pShaderTex->bModified  = false;
	m_MtlList.SetItemData(nItem, (DWORD_PTR)pShaderTex);
}

void CDlgLitModelMtlProperty::ClearAllInfo()
{
	int nCount = m_MtlList.GetItemCount();
	for(int i = 0; i < nCount; i++)
	{
		MTLLIST_DATA* pShaderTex = (MTLLIST_DATA*)m_MtlList.GetItemData(i);
		if(pShaderTex != NULL)
		{
			delete pShaderTex;
			pShaderTex = NULL;
		}
	}
	m_MtlList.DeleteAllItems();
	//CEffectProperty::ClearAllInfo();
	//m_EffectUI.SetEffect(NULL);
	SetEffectUI(NULL);

	UpdateData(FALSE);
}

void CDlgLitModelMtlProperty::SetLitModel(A3DLitModel* pLitModel)
{
	m_pLitModel = pLitModel;
}

bool CDlgLitModelMtlProperty::ReplaceTexture(A3DTexture* pCurTexture, A3DTexture* pNewTextrure)
{
	int nCount = m_pLitModel->GetNumMeshes();
	for(int i = 0; i < nCount; i++)
	{
		A3DLitMesh* pLitMesh = m_pLitModel->GetMesh(i);
		if(pLitMesh->GetTexturePtr() == pCurTexture) {
			pLitMesh->SetTexturePtr(pNewTextrure);
			return true;
		}
	}
	ASSERT(0);
	return false;
}

void CDlgLitModelMtlProperty::UpdateMtlList()
{
	SetEffectUI(NULL);

	if (!m_pLitModel)
	{
		clearPropList();
		return;
	}
	int nMesh = m_pLitModel->GetNumMeshes();
	for(int i = 0; i < nMesh; i++)
	{
		A3DLitMesh* pLitMesh = m_pLitModel->GetMesh(i);
		const char* szName = pLitMesh->GetName();
		A3DTexture* pTexture = pLitMesh->GetTexturePtr();
		AddMtlToList(szName, pTexture);
	}

	//A3DSkinModel* pModel = m_pModel->GetA3DSkinModel();
	//int nNumSkin = pModel->GetSkinNum();
	//for(int i = 0; i < nNumSkin; i++)
	//{
	//	A3DSkin* pSkin = pModel->GetA3DSkin(i);
	//	int nNumSkinMesh = pSkin->GetSkinMeshNum();

	//	for(int n = 0; n < nNumSkinMesh; n++)
	//	{
	//		A3DSkinMesh* pSkinMesh = pSkin->GetSkinMesh(n);
	//		const char* pszName = pSkinMesh->GetName();
	//		int nTexIdx = pSkinMesh->GetTextureIndex();
	//		if(nTexIdx < 0)  {
	//			continue;
	//		}
	//		A3DTexture* pTexture = pSkin->GetTexture(nTexIdx);
	//		AddMtlToList(pszName, pTexture);
	//	}

	//	int nNumRigidMesh = pSkin->GetRigidMeshNum();

	//	for(int n = 0; n < nNumRigidMesh; n++)
	//	{
	//		A3DRigidMesh* pRigidMesh = pSkin->GetRigidMesh(n);
	//		const char* pszName = pRigidMesh->GetName();
	//		int nTexIdx = pRigidMesh->GetTextureIndex();
	//		if(nTexIdx < 0)  {
	//			continue;
	//		}
	//		A3DTexture* pTexture = pSkin->GetTexture(nTexIdx);
	//		AddMtlToList(pszName, pTexture);
	//	}
	//}

	m_MtlList.SetItemState(0, LVIS_FOCUSED|LVIS_SELECTED, LVIS_FOCUSED|LVIS_SELECTED);
}

// CDlgLitModelMtlProperty message handlers

void CDlgLitModelMtlProperty::OnLvnItemchangedMtllist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem < 0 || pNMListView->iSubItem != 0)
		return;

	MTLLIST_DATA* pMtlListData = (MTLLIST_DATA*)m_MtlList.GetItemData(pNMListView->iItem);
	SetEffectUI(pMtlListData);

	UpdateFocusMtlPropData(/*pNMListView->iItem*/);
	UpdateMacroList(false);
}
