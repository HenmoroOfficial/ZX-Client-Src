// EffectProperty.cpp : implementation file
//

#include "EPStdAfx.h"
//#include "EffectPropRes.h"
#include "EffectProperty.h"	// 这个要取MMORPG上EditorShare\EffectProperty
#include <A3DHLSL.h>
#include <A3DHLSLDataSrc.h>
#include <A3DEffect.h>
#include <PropertyList.h>
#include <AObject.h>
//#include "DlgPasteMtlParam.h"
#include "TextureProperty.h"
#include <A3DEnvironment.h>

//#define	ANGELICA_MTL_CLIPBOARDHEADER	("[Angelica 2.2 Mtl]\n")
#define STRICMP _stricmp

/////////////////////////////////////////////////////////////////////////////
// CEffectProperty dialog


CEffectProperty::CEffectProperty(A3DEngine* pA3DEngine, UINT uIDTemplate, CWnd* pParent /* = NULL */)
: CDialog(uIDTemplate, pParent)
, m_pA3DEngine(pA3DEngine)
//, m_bAlphaTexture(0)
{

	//m_pModel = NULL;
	m_pPropertyList = 0;
	m_pProperty = 0;
}

CEffectProperty::~CEffectProperty()
{

	//SaveMtlFile();
	delete m_pPropertyList;
	m_pPropertyList = 0;

	delete m_pProperty;
	m_pProperty = 0;
}

void CEffectProperty::clearPropList()
{
	m_pPropertyList->CleanItems();
	m_pProperty->Clear();
	m_arrPropIndex.clear();
	for (IndexCustomFuncsMap::const_iterator it = m_mapIndexCustomFuncs.begin();
		it != m_mapIndexCustomFuncs.end(); ++it)
	{
		delete it->second;
	}
	m_mapIndexCustomFuncs.clear();
	m_pPropertyList->InvalidateRect(NULL, TRUE);
}

void CEffectProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEffectProperty)
	//DDX_Control(pDX, IDC_MTLLIST, m_MtlList);
	DDX_Control(pDX, IDC_MACROLIST, m_MacroList);
	DDX_Control(pDX, IDC_APPROXIMATELY, m_Approximately);
	//if(m_EffectUI.pCurEffect != NULL)
	//{
	//	m_bAlphaTexture = m_EffectUI.pCurEffect->IsAlphaTexture() ? 1 : 0;
	//	DDX_Check(pDX, IDC_CHK_ALPHA, m_bAlphaTexture);
	//	m_EffectUI.pCurEffect->SetAlphaTexture((bool)m_bAlphaTexture);
	//}
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEffectProperty, CDialog)
	//{{AFX_MSG_MAP(CEffectProperty)
	//ON_NOTIFY(LVN_ITEMCHANGED, IDC_MTLLIST, OnItemchangedMtl)
	//ON_NOTIFY(NM_RCLICK, IDC_MTLLIST, OnRClickedMtl)
	ON_LBN_SELCHANGE(IDC_MACROLIST, OnMacroSelChange)
	//ON_COMMAND(IDM_SETORCHANGEMTL, OnMenuSetOrChangeMtl)
	//ON_COMMAND(IDM_RESTORE, OnMenuRestorePrevMtl)
	//ON_COMMAND(IDM_RELOADMTL, OnMenuReloadMtl)
	//ON_COMMAND(IDM_DELETEMTL, OnMenuDeleteMtl)
	//ON_COMMAND(IDM_COPYMTL, OnMenuCopyMtl)
	//ON_COMMAND(IDM_PASTEMTL, OnMenuPasteMtl)
	ON_BN_CLICKED(IDC_CHK_ALPHA, OnAlphaTex)
	ON_UPDATE_COMMAND_UI(IDC_CHK_ALPHA, OnUpdateAlphaTex)
	ON_WM_DRAWITEM()
	ON_MESSAGE(WM_LIST_DATA_CHANGED, OnPropListDataChanged)
	ON_MESSAGE(WM_LIST_DATA_CHANGING, OnPropListDataChanging)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP

	//ON_MESSAGE(WM_ACTIVATEPAGE, OnActivatePage)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEffectProperty message handlers

BOOL CEffectProperty::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//m_strWarning = "这里进行的骨骼缩放调整只用于观看其的效果，缩放结果并不会被保存到模型文件中。";

	////	Initialize joint list
	//m_MtlList.InsertColumn(MI_NAME, "材质名称", LVCFMT_LEFT, 70);
	//m_MtlList.InsertColumn(MI_RELATIVETEX, "相关纹理", LVCFMT_LEFT, 170);
	//m_MtlList.InsertColumn(MI_MTLNAME, "材质名", LVCFMT_LEFT, 140);
	//m_MtlList.SetExtendedStyle(m_MtlList.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	CRect rc;
	GetDlgItem(IDC_FRM_CONST_LIST)->GetWindowRect(&rc);
	ScreenToClient(&rc);
	m_pPropertyList = new CPropertyList;
	m_pPropertyList->Create("Dialog", WS_CHILD | WS_VISIBLE, rc, this, 1);
//#if 0
	m_pPropertyList->EnableDesc();
//#endif // 0
	m_pProperty = new ADynPropertyObject;


	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CEffectProperty::OnDestroy()
{
	//ClearAllInfo();
	clearPropList();
	m_EffectUI.Cleanup();
	CDialog::OnDestroy();
}

//BOOL CEffectProperty::PreTranslateMessage(MSG* pMsg) 
//{
//	//	Disable 'return' and 'escape' because these keys will close dialog
//	if (pMsg->message == WM_KEYDOWN)
//	{
//		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
//			return TRUE;
//	}
//
//	return CDialog::PreTranslateMessage(pMsg);
//}

//	Bind model object with this dialog
//bool CEffectProperty::BindModel(CModel* pModel)
//{
//	clearPropList();
//	//	Clear old information
//	if (m_pModel)
//	{
//		ClearAllInfo();
//	}
//	UpdateMacroList(false);
//	m_pModel = pModel;
//
//	if (!pModel)
//	{
//		return true;
//	}
//
//
//
//	//	Reset material list
//	UpdateMtlList();
//
//	return true;
//}

//bool CEffectProperty::Command(const char* cmd, WPARAM wParam, LPARAM lParam)
//{
//	//if(m_pModel == NULL)  {
//	//	return false;
//	//}
//	//if(STRICMP(cmd, "save") == 0)
//	//{
//	//	int nCount = m_MtlList.GetItemCount();
//	//	for(int i = 0; i < nCount; i++)
//	//	{
//	//		MTLLIST_DATA* pShaderTex = (MTLLIST_DATA*)m_MtlList.GetItemData(i);
//	//		pShaderTex->bModified = false;
//	//	}
//	//}
//	//else if(STRICMP(cmd, "reloadmtl") == 0)
//	//{
//	//	int nFocus = m_MtlList.GetNextItem(-1, LVNI_FOCUSED);
//	//	int nCount = m_MtlList.GetItemCount();
//	//	bool bNeedSave = false;
//	//	for(int i = 0; i < nCount; i++)
//	//	{
//	//		MTLLIST_DATA* pShaderTex = (MTLLIST_DATA*)m_MtlList.GetItemData(i);
//	//		if(pShaderTex->bModified)  {
//	//			bNeedSave = true;
//	//		}
//	//		if(pShaderTex->pShaderTex->GetClassID() == A3D_CID_HLSLSHADER)
//	//		{
//	//			pShaderTex->HoldEffectParam((A3DEffect*)pShaderTex->pShaderTex);
//	//		}
//	//	}
//
//	//	if(bNeedSave)
//	//	{
//	//		if(::MessageBox(NULL, "重新加载需要保存当前材质设置,保存并重新载入吗?","警告", MB_YESNO|MB_ICONWARNING) == IDNO)  {
//	//			return true;
//	//		}
//
//	//		m_pModel->SaveMtl();
//	//		Command("save", NULL, NULL);			
//	//	}
//
//	//	g_Render.GetA3DEngine()->GetA3DTextureMan()->ReloadAllTextures(true);
//	//	g_Render.GetA3DEngine()->GetA3DHLSLMan()->RebuildAll();
//
//	//	for(int i = 0; i < nCount; i++)
//	//	{
//	//		MTLLIST_DATA* pShaderTex = (MTLLIST_DATA*)m_MtlList.GetItemData(i);
//	//		if(pShaderTex->pShaderTex->GetClassID() == A3D_CID_HLSLSHADER)
//	//		{
//	//			pShaderTex->RestoreEffectParam((A3DEffect*)pShaderTex->pShaderTex);
//	//		}
//	//	}
//
//	//	MTLLIST_DATA* pShaderTex = (MTLLIST_DATA*)m_MtlList.GetItemData(nFocus);
//	//	A3DEffect* pEffect = (A3DEffect*)pShaderTex->pShaderTex;
//	//	m_MtlList.SetItemText(nFocus, MI_MTLNAME, GetHLSLName(pEffect));
//	//	UpdateMtlPropData(nFocus);
//	//	UpdateMacroList(true);
//	//}
//	//else if(STRICMP(cmd, "MtlListChanged") == 0)
//	//{
//	//	BindModel(m_pModel);
//	//}
//	return true;
//}

//	Clear all information in dialog
//void CEffectProperty::ClearAllInfo()
//{
//	//int nCount = m_MtlList.GetItemCount();
//	//for(int i = 0; i < nCount; i++)
//	//{
//	//	MTLLIST_DATA* pShaderTex = (MTLLIST_DATA*)m_MtlList.GetItemData(i);
//	//	if(pShaderTex != NULL)
//	//	{
//	//		delete pShaderTex;
//	//		pShaderTex = NULL;
//	//	}
//	//}
//	//m_MtlList.DeleteAllItems();
//	m_EffectUI.Cleanup();
//
//	UpdateData(FALSE);
//}

//	On activate / disactivate this page
//#ifdef _ANGELICA22
//LRESULT CEffectProperty::OnActivatePage(WPARAM wParam, LPARAM lParam)
//#else
//void CEffectProperty::OnActivatePage(WPARAM wParam, LPARAM lParam)
//#endif
//{
//	if (wParam)	//	Activate this page
//	{
//	}
//	else	//	Disactivate this page
//	{
//	}
//#ifdef _ANGELICA22
//	return 0L;
//#endif
//}

//void CEffectProperty::AddMtlToList(LPCSTR pszName, A3DTexture* pTexture)
//{
//	int nItem = m_MtlList.InsertItem(0, pszName);
//
//	if(pTexture->GetClassID() == A3D_CID_HLSLSHADER)
//	{
//		A3DEffect* pHLSLShader = (A3DEffect*)pTexture;
//		AString strMtlFilename = pHLSLShader->GetMtlFilename();
//
//		m_MtlList.SetItemText(nItem, MI_RELATIVETEX, &strMtlFilename[strMtlFilename.ReverseFind('\\') + 1]);
//		m_MtlList.SetItemText(nItem, MI_MTLNAME, GetHLSLName(pTexture));
//	}
//	else
//	{
//		AString strTextureFile = pTexture->GetMapFile();
//		m_MtlList.SetItemText(nItem, MI_RELATIVETEX, &strTextureFile[strTextureFile.ReverseFind('\\') + 1]);
//	}
//	MTLLIST_DATA* pShaderTex = new MTLLIST_DATA;
//	pShaderTex->pShaderTex = pTexture;
//	pShaderTex->bModified  = false;
//	m_MtlList.SetItemData(nItem, (DWORD_PTR)pShaderTex);
//}

//	Update bone list
//void CEffectProperty::UpdateMtlList()
//{
//	//m_pCurShader = NULL;
//	m_EffectUI.Cleanup();
//
//	if (!m_pModel)
//	{
//		clearPropList();
//		return;
//	}
//
//	A3DSkinModel* pModel = m_pModel->GetA3DSkinModel();
//	int nNumSkin = pModel->GetSkinNum();
//	for(int i = 0; i < nNumSkin; i++)
//	{
//		A3DSkin* pSkin = pModel->GetA3DSkin(i);
//		int nNumSkinMesh = pSkin->GetSkinMeshNum();
//
//		for(int n = 0; n < nNumSkinMesh; n++)
//		{
//			A3DSkinMesh* pSkinMesh = pSkin->GetSkinMesh(n);
//			const char* pszName = pSkinMesh->GetName();
//			int nTexIdx = pSkinMesh->GetTextureIndex();
//			if(nTexIdx < 0)  {
//				continue;
//			}
//			A3DTexture* pTexture = pSkin->GetTexture(nTexIdx);
//			AddMtlToList(pszName, pTexture);
//		}
//
//		int nNumRigidMesh = pSkin->GetRigidMeshNum();
//
//		for(int n = 0; n < nNumRigidMesh; n++)
//		{
//			A3DRigidMesh* pRigidMesh = pSkin->GetRigidMesh(n);
//			const char* pszName = pRigidMesh->GetName();
//			int nTexIdx = pRigidMesh->GetTextureIndex();
//			if(nTexIdx < 0)  {
//				continue;
//			}
//			A3DTexture* pTexture = pSkin->GetTexture(nTexIdx);
//			AddMtlToList(pszName, pTexture);
//		}
//	}
//
//	m_MtlList.SetItemState(0, LVIS_FOCUSED|LVIS_SELECTED, LVIS_FOCUSED|LVIS_SELECTED);
//}

LRESULT CEffectProperty::OnPropListDataChanged(WPARAM wParam, LPARAM lParam)
{
	if (lParam < 0)
		return 0;
	savePropData();
	if(!m_EffectUI.pCurEffect || m_EffectUI.pCurEffect->GetClassID() != A3D_CID_HLSLSHADER)
	{
		return 0;
	}
	if(m_arrPropIndex.size() >= (size_t)lParam || m_arrPropIndex[lParam] < 0) {
		return 0;
	}

	A3DHLSL* pHLSL = m_EffectUI.pCurEffect->GetHLSL();
	if(!pHLSL)
		return 0;
	int idx = 0;
	const A3DHLSL::CONSTMAPDESC* ConstName = pHLSL->GetConstMapArray();
	const A3DHLSL::CONSTMAPDESC& cd = ConstName[m_arrPropIndex[lParam]];
	if (cd.cd.cd.Class == D3DXPC_OBJECT)
	{
		TextureProperty* pTextureProperty = dynamic_cast<TextureProperty*>(m_mapIndexCustomFuncs[m_arrPropIndex[lParam]]);
		if(!pTextureProperty)
			return 0;
		A3DTexture* pTexture = m_EffectUI.pCurEffect->GetTextureByName(cd.cd.cd.Name);
		if(pTexture)
			pTextureProperty->m_csText = pTexture->GetMapFile();
#if 0
		m_pPropertyList->SetDesc(pTextureProperty->m_csText);
#endif // 0
	}
	return 0;
}

LRESULT CEffectProperty::OnPropListDataChanging(WPARAM wParam, LPARAM lParam)
{
	if (lParam < 0)
		return 0;
	savePropData();
	return 0;
}

void CEffectProperty::loadPropData()
{
	clearPropList();
	if(!m_EffectUI.pCurEffect || m_EffectUI.pCurEffect->GetClassID() != A3D_CID_HLSLSHADER)
	{
		return;
	}

	A3DHLSL* pHLSL = m_EffectUI.pCurEffect->GetHLSL();
	if(!pHLSL)
		return;

	const A3DHLSL::CONSTMAPDESC* ConstName = pHLSL->GetConstMapArray();
	for(UIElementArray::iterator it = m_EffectUI.aElementArray.begin();
		it != m_EffectUI.aElementArray.end(); ++it)
	{
		const A3DHLSL::CONSTMAPDESC& cd = ConstName[it->nIdx];
		A3DVECTOR4 vValue;

		if(cd.uMemberOffset >= 0)
			continue;

		switch(cd.cd.cd.Class)
		{
		case D3DXPC_SCALAR:
			{
				pHLSL->GetConstantArrayF(it->ConstName, &vValue, 1);
				m_pProperty->DynAddProperty(vValue.x, it->UIName, NULL, it->pRange, it->pRange ? WAY_SLIDER : WAY_DEFAULT);
			}
			break;
		case D3DXPC_VECTOR:
			{
				pHLSL->GetConstantArrayF(it->ConstName, &vValue, 1);
				CString csName(it->ConstName);
				//if(_isColorParam(csName))
				if(it->Type == CT_COLOR)
				{
					A3DCOLOR rgbColor;
					if (cd.cd.cd.Columns ==3)
					{
						rgbColor = A3DCOLORRGB((int)(vValue.x*255+0.5f), (int)(vValue.y*255+0.5f), (int)(vValue.z*255+0.5f));
					}
					else
					{
						rgbColor = A3DCOLORRGBA((int)(vValue.x*255+0.5f), (int)(vValue.y*255+0.5f), (int)(vValue.z*255+0.5f), (int)(vValue.w*255+0.5f));
					}
					m_pProperty->DynAddProperty(rgbColor, it->UIName, 0, 0, WAY_COLOR);
#ifdef COLOR_TONE_TEST
					// <测试程序调色用>
					if(it->UIName == "目标色调")
					{
						A3DColorTone ClrTone;
						ClrTone.InitHue();
						A3DCOLORVALUE crSrc;
						A3DCOLORVALUE crDest;
						A3DMATRIX4 mat;
						m_EffectUI.pCurEffect->GetHLSL()->GetConstantArrayF("g_SrcColorTone", (A3DVECTOR4*)&crSrc, 1);
						m_EffectUI.pCurEffect->GetHLSL()->GetConstantArrayF("g_DestColorTone", (A3DVECTOR4*)&crDest, 1);
						ClrTone.ComputeColorMatrix(mat, crSrc, crDest);
						m_EffectUI.pCurEffect->GetHLSL()->SetConstantMatrix("g_matColorTone", mat);
					}
					// </测试程序调色用>
#endif // COLOR_TONE_TEST
				}
				else
				{
					int iFloatSize = sizeof(float);
					BYTE* pbyValueArr = new BYTE[cd.cd.cd.Columns*iFloatSize];
					memcpy(pbyValueArr, &vValue, cd.cd.cd.Columns*iFloatSize);
					VectorXProperty* pVectorXProperty = new VectorXProperty;
					m_mapIndexCustomFuncs[it->nIdx] = pVectorXProperty;
					pVectorXProperty->m_pBytes = pbyValueArr;
					pVectorXProperty->m_nBytes = cd.cd.cd.Columns*iFloatSize;
					pVectorXProperty->m_pParentWnd = this;
					pVectorXProperty->m_nType = VectorXProperty::VECTORX_FLOAT;
					pVectorXProperty->GetDisplay();
					m_pProperty->DynAddProperty(0, it->UIName, (ASet*)pVectorXProperty, 0, WAY_CUSTOM);
				}
			}
			break;
		case D3DXPC_MATRIX_ROWS:
			break;
		case D3DXPC_MATRIX_COLUMNS:
			break;
		case D3DXPC_OBJECT:
			{
				TextureProperty* pTextureProperty = new TextureProperty(m_pA3DEngine);
				m_mapIndexCustomFuncs[it->nIdx] = pTextureProperty;
				A3DTexture* pTexture = m_EffectUI.pCurEffect->GetTextureByName(cd.cd.cd.Name);
				CString csRootDir;
				if(pTexture) {
					pTextureProperty->m_csText = pTexture->GetMapFile();
				}

				// 纹理浏览的限制路径
				if(m_EffectUI.pCurEffect->GetClassID() == A3D_CID_HLSLSHADER) {
					csRootDir = ((A3DEffect*)m_EffectUI.pCurEffect)->GetMtlFilename();
				}
				else {
					csRootDir = pTextureProperty->m_csText;
				}
			
				csRootDir = csRootDir.Left(csRootDir.ReverseFind('\\'));
				CString csBaseDir(af_GetBaseDir());
				if(PathIsPrefix(csBaseDir, csRootDir)) {
					csRootDir = csRootDir.Right(csRootDir.GetLength() - csBaseDir.GetLength()-1);
				}
				pTextureProperty->m_csRootDir = csRootDir;
				m_pProperty->DynAddProperty(pTextureProperty->m_var, it->UIName, (ASet*)(pTextureProperty), 0, WAY_CUSTOM,0,true,0,pTextureProperty->m_csText);				
			}
			break;
		case D3DXPC_STRUCT:
			break;
		}
		m_arrPropIndex.push_back(it->nIdx);
	}
	m_pPropertyList->AttachDataObject(m_pProperty);
}

void CEffectProperty::savePropData()
{
	if(!m_EffectUI.pCurEffect || m_EffectUI.pCurEffect->GetClassID() != A3D_CID_HLSLSHADER)
	{
		return;
	}

	A3DHLSL* pHLSL = m_EffectUI.pCurEffect->GetHLSL();
	if(!pHLSL)
		return;

	m_pPropertyList->UpdateData();

	m_EffectUI.pMtlListData->bModified = true;

	AString strValue;
	int idx = 0;
	const A3DHLSL::CONSTMAPDESC* ConstName = pHLSL->GetConstMapArray();
	for(UIElementArray::iterator it = m_EffectUI.aElementArray.begin();
		it != m_EffectUI.aElementArray.end(); ++it)
	{
		const A3DHLSL::CONSTMAPDESC& cd = ConstName[it->nIdx];
		A3DVECTOR4 vValue;

		if(cd.uMemberOffset >= 0)
			continue;

		switch(cd.cd.cd.Class)
		{
		case D3DXPC_SCALAR:
			{				
				strValue.Format("%f", (float)m_pProperty->GetPropVal(idx));
				m_EffectUI.pCurEffect->SetValueByString(cd, strValue);
			}
			break;
		case D3DXPC_VECTOR:
			{
				CString csName(cd.cd.cd.Name);
				float fArr[4];
				if(it->Type == CT_COLOR)
				{
					A3DCOLOR clr = (A3DCOLOR)m_pProperty->GetPropVal(idx);
					fArr[0] = A3DCOLOR_GETRED(clr)/255.0f;
					fArr[1] = A3DCOLOR_GETGREEN(clr)/255.0f;
					fArr[2] = A3DCOLOR_GETBLUE(clr)/255.0f;
					if(cd.cd.cd.Columns == 3)
					{
						strValue.Format("(%f,%f,%f)", fArr[0], fArr[1], fArr[2]);
					}
					else if(cd.cd.cd.Columns == 4)
					{
						fArr[3] = A3DCOLOR_GETALPHA(clr)/255.0f;
						strValue.Format("(%f,%f,%f,%f)", fArr[0], fArr[1], fArr[2], fArr[3]);
					}
#ifdef COLOR_TONE_TEST
					// <测试程序调色用>
					if(it->UIName == "目标色调")
					{
						A3DColorTone ClrTone;
						ClrTone.InitHue();
						A3DCOLORVALUE crSrc;
						A3DCOLORVALUE crDest;
						A3DMATRIX4 mat;
						m_EffectUI.pCurEffect->GetHLSL()->GetConstantArrayF("g_SrcColorTone", (A3DVECTOR4*)&crSrc, 1);
						m_EffectUI.pCurEffect->GetHLSL()->GetConstantArrayF("g_DestColorTone", (A3DVECTOR4*)&crDest, 1);
						ClrTone.ComputeColorMatrix(mat, crSrc, crDest);
						m_EffectUI.pCurEffect->GetHLSL()->SetConstantMatrix("g_matColorTone", mat);
					}
					// </测试程序调色用>
#endif // COLOR_TONE_TEST
				}
				else
				{
					VectorXProperty* pVectorXProperty = dynamic_cast<VectorXProperty*>(m_mapIndexCustomFuncs[idx]);
					if(!pVectorXProperty)
						break;
					memcpy(fArr, pVectorXProperty->GetData(), pVectorXProperty->GetDataLength());
					int iNum = pVectorXProperty->GetDataLength()/sizeof(float);
					switch(iNum)
					{
					case 1:
						strValue.Format("(%f)", fArr[0]);
						break;
					case 2:
						strValue.Format("(%f,%f)", fArr[0], fArr[1]);
						break;
					case 3:
						strValue.Format("(%f,%f,%f)", fArr[0], fArr[1], fArr[2]);
						break;
					case 4:
						strValue.Format("(%f,%f,%f,%f)", fArr[0], fArr[1], fArr[2], fArr[3]);
						break;
					}
				}
				m_EffectUI.pCurEffect->SetValueByString(cd, strValue);
			}
			break;
		case D3DXPC_MATRIX_ROWS:
			break;
		case D3DXPC_MATRIX_COLUMNS:
			break;		
		case D3DXPC_OBJECT:
			{
				TextureProperty* pTextureProperty = dynamic_cast<TextureProperty*>(m_mapIndexCustomFuncs[idx]);
				if(!pTextureProperty)
					break;
				m_EffectUI.pCurEffect->SetTextureByName(cd.cd.cd.Name, pTextureProperty->GetFilePath());
				m_pProperty->SetPropDesc(idx, pTextureProperty->GetFilePath());			
			}
			break;
		case D3DXPC_STRUCT:
			break;
		default:
			continue;
		}
		idx++;
	}
}

//void CEffectProperty::OnItemchangedMtl(NMHDR* pNMHDR, LRESULT* pResult) 
//{
//	*pResult = 0;
//
//	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
//	if (pNMListView->iItem < 0 || pNMListView->iSubItem != 0)
//		return;
//
//	UpdateFocusMtlPropData(/*pNMListView->iItem*/);
//	UpdateMacroList(false);
//}

//void CEffectProperty::OnRClickedMtl(NMHDR* pNMHDR, LRESULT* pResult)
//{
//	int nFocus = m_MtlList.GetNextItem(-1, LVNI_FOCUSED);
//	if(nFocus < 0)
//		return;
//	MTLLIST_DATA* pShaderTex = (MTLLIST_DATA*)m_MtlList.GetItemData(nFocus);
//	CMenu menu;
//	POINT ptCursor;
//
//	menu.LoadMenu(IDR_MTLMENU);
//	CMenu* pSubMenu = menu.GetSubMenu(0);
//	GetCursorPos(&ptCursor);
//
//	pSubMenu->EnableMenuItem(IDM_RESTORE, pShaderTex->strPrevMtlFile.GetLength() != 0 ? MF_ENABLED : MF_DISABLED | MF_GRAYED);
//
//	//#ifdef _DEBUG
//	if(pShaderTex->pShaderTex == NULL && pShaderTex->pShaderTex->GetClassID() != A3D_CID_HLSLSHADER) {
//		return;
//	}
//
//	AString strFilename = ((A3DEffect*)(pShaderTex->pShaderTex))->GetHLSLFilename();
//	AString strFileSpec;
//
//	// 如果文件名有前缀'!', 则认为是内部材质
//	af_GetFileTitle(strFilename, strFileSpec);
//	if(strFileSpec.CompareNoCase("!CheckTangentError.hlsl") == 0) {
//		return;
//	}
//
//	if(strFileSpec.CompareNoCase("default.hlsl") != NULL)
//	{
//		MENUITEMINFO MenuItemInfo;
//		memset(&MenuItemInfo, 0, sizeof(MENUITEMINFO));
//		MenuItemInfo.cbSize		= sizeof(MENUITEMINFO);
//		MenuItemInfo.fMask		= MIIM_TYPE | MIIM_ID;
//		MenuItemInfo.fType		= MFT_STRING;				// used if MIIM_TYPE (4.0) or MIIM_FTYPE (>4.0)
//
//		MenuItemInfo.wID		= IDM_DELETEMTL;
//		MenuItemInfo.dwTypeData = "删除材质";
//		pSubMenu->InsertMenuItem(IDM_DELETEMTL, &MenuItemInfo, FALSE);
//
//		MenuItemInfo.fType		= MFT_SEPARATOR;
//		pSubMenu->InsertMenuItem(0, &MenuItemInfo, FALSE);
//		MenuItemInfo.fType		= MFT_STRING;
//
//		MenuItemInfo.wID		= IDM_COPYMTL;
//		MenuItemInfo.dwTypeData = "复制材质参数";
//		pSubMenu->InsertMenuItem(IDM_COPYMTL, &MenuItemInfo, FALSE);
//
//		if(OpenClipboard())
//		{
//			HANDLE hData = GetClipboardData(CF_TEXT);
//			if(hData != NULL)
//			{
//				LPCSTR lpData = (LPCSTR)GlobalLock(hData);
//				if(lpData != NULL && StrCmpN(lpData, ANGELICA_MTL_CLIPBOARDHEADER, lstrlen(ANGELICA_MTL_CLIPBOARDHEADER)) == 0)
//				{
//					MenuItemInfo.wID		= IDM_PASTEMTL;
//					MenuItemInfo.dwTypeData = "粘贴材质参数";
//					pSubMenu->InsertMenuItem(IDM_PASTEMTL, &MenuItemInfo, FALSE);
//				}
//				GlobalUnlock(hData);
//			}
//			CloseClipboard();
//		}
//
//	}
//	//#endif // _DEBUG
//	pSubMenu->TrackPopupMenu(0, ptCursor.x, ptCursor.y, this);
//}

void CEffectProperty::OnMacroSelChange()
{
	A3DHLSLDataSrc* pDataSrc = GetDataSrc();
	if(pDataSrc == NULL)  {
		return;
	}
	MTLLIST_DATA* pMtlListData = GetCurMtlListData();
	pMtlListData->HoldEffectParam(m_EffectUI.pCurEffect);

	// 从UI上得到宏选择信息,经过验证后再更新到UI上
	AStringArray aStrings, aMacros;
	GetSelectedMacro(aStrings);
	pDataSrc->ProvingMacro(aStrings, aMacros);

	m_EffectUI.pCurEffect->SetMacros(aMacros, NULL);
	pMtlListData->bModified = true;

	pMtlListData->RestoreEffectParam(m_EffectUI.pCurEffect);

	UpdateFocusMtlPropData(/*-1*/);
	UpdateMacroList(false);
}

//void CEffectProperty::OnMenuSetOrChangeMtl()
//{
//	CDlgMtlList DlgMtlList;
//	AStringArray aStrings, aMacros;
//
//	DlgMtlList.DoModal();
//
//	if(DlgMtlList.m_strHLSLFilename.GetLength() == 0)
//		return;
//
//	int nFocus = m_MtlList.GetNextItem(-1, LVNI_FOCUSED);
//
//	MTLLIST_DATA* pMtlListData = (MTLLIST_DATA*)m_MtlList.GetItemData(nFocus);
//	A3DTexture* pCurTexture = pMtlListData->pShaderTex;
//	A3DEffect* pEffect = NULL;
//
//	pMtlListData->bModified = true;
//
//	if(pCurTexture->GetClassID() == A3D_CID_HLSLSHADER)
//	{
//		pEffect = (A3DEffect*)pCurTexture;
//
//		// 保存属性值
//		pMtlListData->HoldEffectParam(pEffect);	// 保存参数设置
//		GetSelectedMacro(aStrings);				// 保存宏选择
//		pMtlListData->SavePrevMtl();
//
//		if( ! pEffect->SetHLSL(DlgMtlList.m_strHLSLFilename))
//		{
//			MessageBox("加载Effect文件失败,可能是由于HLSL编译错误导致.","错误", MB_OK|MB_ICONERROR);
//			OnMenuRestorePrevMtl();
//			return;
//		}
//		m_MtlList.SetItemText(nFocus, MI_MTLNAME, GetHLSLName(pCurTexture));
//	}
//	else
//	{
//		AString strTexFile = pCurTexture->GetMapFile();
//		pEffect = A3DEffect::CreateHLSLShader(g_Render.GetA3DDevice(), strTexFile, DlgMtlList.m_strHLSLFilename);
//		if(pEffect == NULL)
//		{
//			MessageBox("加载Effect文件失败,可能是由于HLSL编译错误导致.","错误", MB_OK|MB_ICONERROR);
//			return;
//		}
//
//		//pShaderTex->aParam.clear();
//		pMtlListData->strPrevMtlFile.Empty();
//		pMtlListData->pShaderTex = pEffect;
//		AString strMtlFile = pEffect->GetMtlFilename();
//		m_MtlList.SetItemText(nFocus, MI_RELATIVETEX, &strMtlFile[strMtlFile.ReverseFind('\\') + 1]);
//		m_MtlList.SetItemText(nFocus, MI_MTLNAME, GetHLSLName(pEffect));
//
//		m_pModel->ReplaceTexture(pCurTexture, pEffect);
//		g_Render.GetA3DEngine()->GetA3DTextureMan()->ReleaseTexture(pCurTexture);
//	}
//	A3DHLSLDataSrc* pDataSrc = GetDataSrc();
//	if(pDataSrc != NULL)
//	{
//		pDataSrc->ProvingMacro(aStrings, aMacros);
//		if(aMacros.size() > 0)  {
//			pEffect->SetMacros(aMacros, NULL);
//		}
//	}
//	UpdateMacroList(false);
//	pMtlListData->RestoreEffectParam(pEffect);
//	UpdateMtlPropData(nFocus);
//}

//void CEffectProperty::OnMenuRestorePrevMtl()
//{
//	int nFocus = m_MtlList.GetNextItem(-1, LVNI_FOCUSED);
//	MTLLIST_DATA* pMtlListData = (MTLLIST_DATA*)m_MtlList.GetItemData(nFocus);
//
//	if(pMtlListData->pShaderTex->GetClassID() == A3D_CID_HLSLSHADER)
//	{
//		A3DEffect* pEffect = (A3DEffect*)pMtlListData->pShaderTex;
//		pEffect->SetHLSL(pMtlListData->strPrevMtlFile);
//
//		pMtlListData->RestoreEffectParam(pEffect);
//		m_MtlList.SetItemText(nFocus, MI_MTLNAME, GetHLSLName(pEffect));
//		pEffect->SetMacros(pMtlListData->strPrevMtlMacros, NULL);
//
//		pMtlListData->strPrevMtlFile.Empty();
//		pMtlListData->strPrevMtlMacros.Empty();
//
//		UpdateMtlPropData(nFocus);
//		UpdateMacroList(false);
//	}
//}
//
//void CEffectProperty::OnMenuReloadMtl()
//{
//	int nFocus = m_MtlList.GetNextItem(-1, LVNI_FOCUSED);
//	MTLLIST_DATA* pShaderTex = (MTLLIST_DATA*)m_MtlList.GetItemData(nFocus);
//	ASSERT(pShaderTex->pShaderTex->GetClassID() == A3D_CID_HLSLSHADER);
//
//	if(pShaderTex == NULL || pShaderTex->pShaderTex == NULL)
//	{
//		return;
//	}
//	if(pShaderTex->bModified)
//	{
//		if(MessageBox("重新加载将丢失材质所有没保存的材质信息,全部信息将从MTL文件载入,确认这样做吗?","警告", MB_YESNO|MB_ICONWARNING) == IDNO)
//			return;
//	}
//
//	//pShaderTex->aParam.clear();
//	pShaderTex->strPrevMtlFile.Empty();
//	pShaderTex->bModified = false;
//
//	A3DEffect* pEffect = (A3DEffect*)pShaderTex->pShaderTex;
//	g_Render.GetA3DEngine()->GetA3DHLSLMan()->RebuildAll();
//
//	A3DEffect::ParamArray aParam;
//	pEffect->GenerateParam(aParam);
//
//	if( ! pEffect->Reload(true))
//	{
//		DeleteMtl(nFocus, true);
//		return;
//	}
//	m_MtlList.SetItemText(nFocus, MI_MTLNAME, GetHLSLName(pEffect));
//
//	UpdateMtlPropData(nFocus);
//	UpdateMacroList(false);
//}

//void CEffectProperty::OnMenuDeleteMtl()
//{
//	if(MessageBox("将要直接删除这个模型上的材质设置和对应的MTL文件?注意:此操作不能Undo.", "确定", MB_YESNO | MB_ICONWARNING) != IDYES)
//	{
//		return;
//	}
//
//	int nFocus = m_MtlList.GetNextItem(-1, LVNI_FOCUSED);
//	DeleteMtl(nFocus, true);
//}

//void CEffectProperty::OnMenuCopyMtl()
//{
//	MTLLIST_DATA* pMtlListData = GetCurMtlListData();
//	if(pMtlListData->pShaderTex->GetClassID() == A3D_CID_HLSLSHADER)
//	{
//		A3DEffect* pEffect = (A3DEffect*)pMtlListData->pShaderTex;
//		A3DEffect::ParamArray aParam;
//		AString strParam = ANGELICA_MTL_CLIPBOARDHEADER;
//		AString strMacros;
//		pEffect->GenerateParam(aParam);
//		pEffect->GetMacros(strMacros);
//		strParam += strMacros + '\n';
//		for(A3DEffect::ParamArray::iterator it = aParam.begin();
//			it != aParam.end(); ++it)
//		{
//			AString strElement;
//			strElement.Format("%s=%s\n", it->strKey, it->strValue);
//			strParam += strElement;
//		}
//		HGLOBAL hData = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, strParam.GetLength() + 1);
//		if(hData != NULL) {
//			LPSTR lpData = (LPSTR)GlobalLock(hData);
//			if(lpData != NULL) {
//				lstrcpyA(lpData, strParam);
//				GlobalUnlock(hData);
//			}
//		}
//
//		if(OpenClipboard())
//		{
//			EmptyClipboard();
//			SetClipboardData(CF_OEMTEXT, hData);
//			CloseClipboard();
//		}
//	}
//}

//void CEffectProperty::OnMenuPasteMtl()
//{
//	MTLLIST_DATA* pMtlListData = GetCurMtlListData();
//	if(pMtlListData->pShaderTex->GetClassID() != A3D_CID_HLSLSHADER) {
//		return;
//	}
//	A3DEffect* pEffect = (A3DEffect*)pMtlListData->pShaderTex;
//
//	if(OpenClipboard())
//	{
//		HANDLE hData = GetClipboardData(CF_TEXT);
//		if(hData != NULL)
//		{
//			LPCTSTR lpData = (LPCTSTR)GlobalLock(hData);
//			if(lpData != NULL && StrCmpN(lpData, ANGELICA_MTL_CLIPBOARDHEADER, lstrlen(ANGELICA_MTL_CLIPBOARDHEADER)) == 0)
//			{
//				AStringArray aStrings;
//				SplitString(lpData, '\n', aStrings);	// 分解参数
//
//				CDlgPasteMtlParam pmp;
//				pmp.Set(pEffect, aStrings);
//				if(pmp.DoModal() == IDOK)
//				{
//					// 判断是否设置宏
//					AString strMacros;
//					if(pmp.GetMacros(strMacros)) {
//						pEffect->SetMacros(strMacros, NULL);
//					}
//
//					// 刷新用户选择粘贴的参数
//					pmp.GetParamArray(aStrings);
//
//					A3DHLSL* pHLSL = pEffect->GetHLSL();
//					if(pHLSL != NULL && aStrings.size() > 0)
//					{
//						for(AStringArray::iterator it = aStrings.begin();
//							it != aStrings.end(); ++it)
//						{
//							int pos = it->Find('=');
//							if(pos < 0 || it->GetLength() == 0)
//								continue;
//
//							const A3DHLSL::CONSTMAPDESC* pConstMapDesc = pHLSL->GetConstMapByName(it->Mid(0, pos));
//							if(pConstMapDesc != NULL)
//							{
//								pEffect->SetValueByString(*pConstMapDesc, it->Mid(pos + 1, -1));
//							}
//						}
//						UpdateMtlPropData(-1);
//						UpdateMacroList(false);
//					}
//				}
//			}
//			GlobalUnlock(hData);
//		}
//		CloseClipboard();
//	}
//}

void CEffectProperty::UpdateFocusMtlPropData()
{
	//if(nListItem < 0)  {
	//	nListItem = m_MtlList.GetNextItem(-1, LVNI_FOCUSED);
	//	ASSERT(nListItem >= 0);
	//}

	//if (!m_pModel)
	//	return;

	MTLLIST_DATA* pShaderTex = GetCurMtlListData();//(MTLLIST_DATA*)m_MtlList.GetItemData(nListItem);

	if(pShaderTex == NULL) {
		return;
	}

	m_EffectUI.Cleanup();
	m_EffectUI.SetEffect(pShaderTex);

	if(m_EffectUI.pCurEffect == NULL)
	{
		clearPropList();
		m_MacroList.ResetContent();
		return;
	}

	int nTotalInst;
	int nTexInst, nAriInst;
	AString strApproximately;
	m_EffectUI.pCurEffect->GetHLSLCore()->GetApproximately(NULL, NULL, NULL, &nTotalInst, &nTexInst, &nAriInst);
	CheckDlgButton(IDC_CHK_ALPHA, m_EffectUI.pCurEffect->IsAlphaTexture() ? BST_CHECKED : BST_UNCHECKED);

	if(nTexInst == 0 && nAriInst == 0)
	{
		strApproximately.Format("%d instruction slots used", nTotalInst);
	}
	else
	{
		strApproximately.Format("%d instruction slots used (%d texture, %d arithmetic)", nTotalInst, nTexInst, nAriInst);
	}
	m_Approximately.SetWindowText(strApproximately);

	loadPropData();
	//UpdateMacroList();
}
//void CEffectProperty::DeleteMtl(int nListItem, bool bSetToDefault)
//{
//	MTLLIST_DATA* pShaderTex = (MTLLIST_DATA*)m_MtlList.GetItemData(nListItem);
//	ASSERT(pShaderTex->pShaderTex->GetClassID() == A3D_CID_HLSLSHADER);
//
//	A3DTextureMan* pTextureMan = g_Render.GetA3DEngine()->GetA3DTextureMan();
//	A3DEffect* pEffect = (A3DEffect*)pShaderTex->pShaderTex;
//	AString strMtlFilename = pEffect->GetMtlFilename();
//	AString strOriginTex;
//	A3DTexture* pOriginTex;
//
//	if(bSetToDefault)
//	{
//		pEffect->SetHLSL("shaders\\2.2\\hlsl\\materials\\default.hlsl");
//		pOriginTex = pEffect;
//		strOriginTex = pEffect->GetMtlFilename();
//	}
//	else
//	{
//		strOriginTex = pEffect->GetMapFile();
//		pTextureMan->LoadTextureFromFile(pEffect->GetMapFile(), &pOriginTex, A3DTF_DONOTLOADASSHADER);
//		m_pModel->ReplaceTexture(pEffect, pOriginTex);
//		pTextureMan->ReleaseTexture((A3DTexture*&)pEffect);
//	}
//
//	// 删除MTL文件
//	DeleteFile(strMtlFilename);
//
//	// 更新UI
//	m_MtlList.SetItemText(nListItem, MI_MTLNAME, GetHLSLName(pOriginTex));
//	m_MtlList.SetItemText(nListItem, MI_RELATIVETEX, &strOriginTex[strOriginTex.ReverseFind('\\') + 1]);
//
//	clearPropList();
//	pShaderTex->strPrevMtlFile.Empty();
//	pShaderTex->pShaderTex = pOriginTex;
//	m_EffectUI.SetEffect(pShaderTex);
//	UpdateMacroList(false);
//}

void CEffectProperty::UpdateMacroList(bool bForce)
{
	A3DHLSLDataSrc* pCtrlDataSrc = (A3DHLSLDataSrc*)GetWindowLong(m_MacroList.m_hWnd, GWL_USERDATA);
	A3DHLSLDataSrc* pDataSrc = GetDataSrc();

	SetWindowLong(m_MacroList.m_hWnd, GWL_USERDATA, (LONG)pDataSrc);

	if(pDataSrc == NULL) {
		m_MacroList.ResetContent();
		return;
	}

	if(pCtrlDataSrc != pDataSrc || bForce)  
	{
		m_MacroList.ResetContent();
		A3DHLSLDataSrc::MacroPropArray* pMacroArray = pDataSrc->GetMacroArray();

		if(pMacroArray == NULL) {
			return;
		}

		// 添加宏列表
		for(A3DHLSLDataSrc::MacroPropArray::iterator it = pMacroArray->begin(); 
			it != pMacroArray->end(); ++it)
		{
			if(it->strDesc[0] == '*' && m_bShowPrgParam == false)  {
				continue;
			}
			int nItem = m_MacroList.AddString(it->strDesc.GetLength() 
				? it->strDesc
				: it->strMacroName);

			m_MacroList.SetItemDataPtr(nItem, &*it);
		}
	}

	// 把使用的宏都选上
	AStringArray aMacros;
	AString strMacros;
	m_EffectUI.pCurEffect->GetMacros(strMacros);
	SplitString(strMacros, ';', aMacros);
	SetSelectedMacro(aMacros);
}

void CEffectProperty::GetSelectedMacro(AStringArray& aMacros)
{
	A3DHLSLDataSrc::MACROPROP* pMacroProp = NULL;
	int nCount = m_MacroList.GetSelCount();
	int nCurSel = m_MacroList.GetCurSel();
	if(nCount <= 0)  {
		return;
	}

	pMacroProp = (A3DHLSLDataSrc::MACROPROP*)m_MacroList.GetItemDataPtr(nCurSel);
	ASSERT(pMacroProp != NULL);
	if(m_MacroList.GetSel(nCurSel))  {
		aMacros.push_back(pMacroProp->strMacroName);
	}

	LPINT pItems = new INT[nCount];
	m_MacroList.GetSelItems(nCount, pItems);
	for(int i = 0; i < nCount; i++)
	{
		if(pItems[i] == nCurSel)  {
			continue;
		}

		pMacroProp = (A3DHLSLDataSrc::MACROPROP*)m_MacroList.GetItemDataPtr(pItems[i]);
		ASSERT(pMacroProp != NULL);
		aMacros.push_back(pMacroProp->strMacroName);
	}
	delete pItems;
}

void CEffectProperty::SetSelectedMacro(const AStringArray& aMacros)
{
	int nCount = m_MacroList.GetCount();
	int nTopIndex = m_MacroList.GetTopIndex();
	int nCurSel = m_MacroList.GetCurSel();
	AStringArray::const_iterator itBegin = aMacros.begin();
	for(int i = 0; i < nCount; i++)
	{
		A3DHLSLDataSrc::MACROPROP* pMacroProp = 
			(A3DHLSLDataSrc::MACROPROP*)m_MacroList.GetItemDataPtr(i);

		BOOL bSel = FALSE;
		if(aMacros.size() > 0)
		{
			AStringArray::const_iterator it = itBegin;
			do 
			{
				if(pMacroProp->strMacroName == *it)
				{
					bSel = TRUE;
					break;
				}

				if((++it) == aMacros.end()){
					it = aMacros.begin();
				}
			} while (it != itBegin);
		}

		m_MacroList.SetSel(i, bSel);
	}
	m_MacroList.SetTopIndex(nTopIndex);
}

A3DHLSLDataSrc* CEffectProperty::GetDataSrc()
{
	if(m_EffectUI.pCurEffect == NULL ||
		m_EffectUI.pCurEffect->GetHLSL() == NULL ||
		m_EffectUI.pCurEffect->GetHLSL()->GetCore() == NULL)  {
			return NULL;
	}
	return m_EffectUI.pCurEffect->GetHLSL()->GetCore()->GetDataSource(1);
}

MTLLIST_DATA* CEffectProperty::GetCurMtlListData()
{
	return m_EffectUI.pMtlListData;
}
void CEffectProperty::SetEffectUI(MTLLIST_DATA* pMtlListData)
{
	if(pMtlListData == NULL) {
		m_EffectUI.Cleanup();
	}
	else {
		m_EffectUI.SetEffect(pMtlListData);
	}
}
//////////////////////////////////////////////////////////////////////////
CEffectProperty::EFFECT_UI_ELEMENT::EFFECT_UI_ELEMENT()
: nIdx(0), Type(CT_UNKNOWN), pRange(NULL)
{
}
void CEffectProperty::EFFECT_UI_ELEMENT::Cleanup()
{
	nIdx = -1;
	Type = CT_UNKNOWN;
	UIName.Empty();
	ConstName.Empty();
	if(pRange != NULL)
	{
		delete pRange;
		pRange = NULL;
	}
}

CEffectProperty::EFFECT_UI::EFFECT_UI()
{
	pMtlListData = NULL;
	pCurEffect   = NULL;
}

void CEffectProperty::EFFECT_UI::Cleanup()
{
	pMtlListData = NULL;
	pCurEffect   = NULL;

	for(UIElementArray::iterator it = aElementArray.begin();
		it != aElementArray.end(); ++it)
	{
		it->Cleanup();
	}
	aElementArray.clear();
}

void CEffectProperty::EFFECT_UI::SetEffect(MTLLIST_DATA* _pMtlListData)
{

	// 清除原来的列表
	Cleanup();
	pMtlListData = _pMtlListData;

	if(pMtlListData == NULL || pMtlListData->pShaderTex->GetClassID() != A3D_CID_HLSLSHADER)
	{
		pCurEffect = NULL;
		return;
	}

	pCurEffect = (A3DEffect*)pMtlListData->pShaderTex;

	if(pCurEffect == NULL) {
		return;
	}

	A3DHLSL* pHLSL = pCurEffect->GetHLSL();
	if(pHLSL == NULL) {
		return;
	}

	const A3DHLSL::CONSTMAPDESC* ConstName = pHLSL->GetConstMapArray();
	int nCount = pHLSL->GetConstCount();
	int idx;

	UIElementArray	aEditorElements;	// 编辑器控制数据,加到材质属性最后

	for(idx = 0; idx < nCount; idx++)
	{
		EFFECT_UI_ELEMENT Element;
		const A3DHLSL::CONSTMAPDESC& cd = ConstName[idx];

		if(cd.uMemberOffset >= 0)
			continue;

		const char* szName = cd.cd.cd.Name;

		if(cd.cd.cd.Class == D3DXPC_OBJECT)
		{
			const A3DHLSL::ANNOTATION* pAnno = pHLSL->FindAnnotationByName(szName, "Texture");
			if(pAnno != NULL && pAnno->Type == D3DXPT_STRING)
			{
				pAnno = pHLSL->FindAnnotationByName((const char*)pAnno->pData, "UIName");
				if(pAnno != NULL && pAnno->Type == D3DXPT_STRING)
				{
					Element.UIName = (const char*)pAnno->pData;
					Element.Type = CT_UNKNOWN;
				}
			}
		}
		else {
			const A3DHLSL::ANNOTATION* pAnnoName = pHLSL->FindAnnotationByName(szName, "UIName");
			const A3DHLSL::ANNOTATION* pAnnoWidget = pHLSL->FindAnnotationByName(szName, "UIWidget");
			if(pAnnoName != NULL && pAnnoName->Type == D3DXPT_STRING)
			{
				Element.UIName = (const char*)pAnnoName->pData;
			}
			if(pAnnoWidget != NULL && pAnnoWidget->Type == D3DXPT_STRING)
			{
				if(STRICMP((const char*)pAnnoWidget->pData, "COLOR") == 0)
				{
					Element.Type = CT_COLOR;
				}
				else if(STRICMP((const char*)pAnnoWidget->pData, "slider") == 0)
				{
					const A3DHLSL::ANNOTATION* pMin = pHLSL->FindAnnotationByName(szName, "UIMin");
					const A3DHLSL::ANNOTATION* pMax = pHLSL->FindAnnotationByName(szName, "UIMax");
					const A3DHLSL::ANNOTATION* pStep = pHLSL->FindAnnotationByName(szName, "UIStep");
					if(pMin && pMin->Type == D3DXPT_FLOAT && pMax && pMax->Type == D3DXPT_FLOAT &&
						pStep && pStep->Type == D3DXPT_FLOAT )
					{
						Element.Type = CT_SLIDER;
						Element.pRange = new ASliderRangeTemplate<float>(pMin->fValue, pMax->fValue, pStep->fValue);
					}
				}
			}
		}
		if(Element.UIName.GetLength() > 0)
		{
			Element.ConstName = cd.cd.cd.Name;
			Element.nIdx      = cd.cd.nIdx;
			aElementArray.push_back(Element);
		}
	}
	// 追加编辑器数据
	for(UIElementArray::iterator it = aEditorElements.begin();
		it != aEditorElements.end(); ++it)
	{
		aElementArray.push_back(*it);
	}
}

void CEffectProperty::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if(lpDrawItemStruct->CtlType == ODT_LISTBOX)
	{
		if((int)lpDrawItemStruct->itemID < 0)  {
			return;
		}
		A3DHLSLDataSrc::MACROPROP*pMacroProp = 
			(A3DHLSLDataSrc::MACROPROP*)m_MacroList.GetItemDataPtr(lpDrawItemStruct->itemID);
		RECT rect = {lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top,
			lpDrawItemStruct->rcItem.left + (lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top),
			lpDrawItemStruct->rcItem.bottom};

		UINT uState = DFCS_BUTTONCHECK|DFCS_FLAT;
		if(lpDrawItemStruct->itemState & ODS_SELECTED)  {
			uState |= DFCS_CHECKED;
		}
		InflateRect(&rect, -1, -1);
		DrawFrameControl(
			lpDrawItemStruct->hDC,
			&rect,
			DFC_BUTTON,
			uState);

		SetRect(&rect, rect.right, lpDrawItemStruct->rcItem.top,
			lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom);

		AString strName = pMacroProp->strDesc + "  (";
		AString strGroup;
		for(int i = 0; i < 32; i++)
		{
			if(pMacroProp->wGroup & (1 << i)){
				strGroup.Format("%d,", i);
				strName += strGroup;
			}
		}
		strName[strName.GetLength() - 1] = ')';
		DrawText(lpDrawItemStruct->hDC, strName, -1, &rect, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	}
}
void CEffectProperty::OnUpdateAlphaTex(CCmdUI* pCmdUI)
{
	if(m_EffectUI.pCurEffect) {
		pCmdUI->SetCheck(m_EffectUI.pCurEffect->IsAlphaTexture());
		pCmdUI->Enable(true);
	}
	else {
		pCmdUI->SetCheck(false);
		pCmdUI->Enable(false);
	}
}

void CEffectProperty::OnAlphaTex()
{
	if(m_EffectUI.pCurEffect) {
		bool bAlpha = IsDlgButtonChecked(IDC_CHK_ALPHA) == BST_CHECKED;
		m_EffectUI.pCurEffect->SetAlphaTexture(bAlpha);
	}
}

//////////////////////////////////////////////////////////////////////////
bool MTLLIST_DATA::HoldEffectParam(A3DEffect* pEffect)
{
	A3DEffect::ParamArray aParam;
	pEffect->GenerateParam(aParam);
	for(A3DEffect::ParamArray::iterator it =  aParam.begin();
		it != aParam.end(); ++it)
	{
		paramDict[it->strKey] = it->strValue;
	}
	return true;
}

bool MTLLIST_DATA::RestoreEffectParam(A3DEffect* pEffect)
{
	A3DHLSL* pHLSL = pEffect->GetHLSL();
	if(pHLSL == NULL)  {
		return false;
	}

	int nCount = pHLSL->GetConstCount();
	const A3DHLSL::CONSTMAPDESC* pConstMapDesc = pHLSL->GetConstMapArray();
	for(int i = 0; i < nCount; i++)
	{
		ParamDict::iterator it = paramDict.find(pConstMapDesc[i].cd.cd.Name);
		if(it != paramDict.end())
		{
			pEffect->SetValueByString(pConstMapDesc[i], it->second);
		}
	}
	return true;
}

bool MTLLIST_DATA::SavePrevMtl()
{
	if(pShaderTex->GetClassID() != A3D_CID_HLSLSHADER)  {
		return false;
	}

	A3DEffect* pEffect = (A3DEffect*)pShaderTex;
	strPrevMtlFile = pEffect->GetHLSLFilename();
	pEffect->GetMacros(strPrevMtlMacros);
	return true;
}
//////////////////////////////////////////////////////////////////////////
AString GetHLSLName(A3DTexture* pTexture)
{
	if(pTexture->GetClassID() != A3D_CID_HLSLSHADER)
		return "";
	AString strName;
	strName = ((A3DEffect*)pTexture)->GetHLSLFilename();
	LPSTR lpFilename = PathFindFileName(strName);
	strName.CutLeft(lpFilename - strName);
	return strName;
}
