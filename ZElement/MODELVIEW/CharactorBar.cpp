// CharactorBar.cpp : implementation file
//

#include "Global.h"
#include "ModelView.h"
#include "MainFrm.h"
#include "CharactorBar.h"
#include "..\ZElementClient\EC_RoleTypes.h"
#include "Player.h"

#define new A_DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// CCharactorBar dialog


CCharactorBar::CCharactorBar()
{
	//{{AFX_DATA_INIT(CCharactorBar)
	m_strHair = _T("");
	m_strHead = _T("");
	m_strFashionFoot = _T("");
	m_strFashionHeadwear = _T("");
	m_strFashionUpper = _T("");
	m_strFoot = _T("");
	m_strHelm = _T("");
	m_strUpper = _T("");
	m_strWeapon = _T("");
	m_strRide = _T("");
	m_strEar = _T("");
	m_strFashionEar = _T("");
	m_strFashionTail = _T("");
	m_strTail = _T("");
	//}}AFX_DATA_INIT
}


void CCharactorBar::DoDataExchange(CDataExchange* pDX)
{
	CInitDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCharactorBar)
	DDX_Control(pDX, IDC_COMBO_TRANSFORM, m_comboShape);
	DDX_Text(pDX, IDC_STATIC_HAIR, m_strHair);
	DDX_Text(pDX, IDC_STATIC_HEAD, m_strHead);
	DDX_Text(pDX, IDC_STATIC_FASHION_FOOT, m_strFashionFoot);
	DDX_Text(pDX, IDC_STATIC_FASHION_HEADWEAR, m_strFashionHeadwear);
	DDX_Text(pDX, IDC_STATIC_FASHION_UPPER, m_strFashionUpper);
	DDX_Text(pDX, IDC_STATIC_FOOT, m_strFoot);
	DDX_Text(pDX, IDC_STATIC_HELM, m_strHelm);
	DDX_Text(pDX, IDC_STATIC_UPPER, m_strUpper);
	DDX_Text(pDX, IDC_STATIC_MOUNT_WEAPON, m_strWeapon);
	DDX_Text(pDX, IDC_STATIC_MOUNT_RIDE, m_strRide);
	DDX_Text(pDX, IDC_STATIC_EAR, m_strEar);
	DDX_Text(pDX, IDC_STATIC_FASHION_EAR, m_strFashionEar);
	DDX_Text(pDX, IDC_STATIC_FASHION_TAIL, m_strFashionTail);
	DDX_Text(pDX, IDC_STATIC_TAIL, m_strTail);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCharactorBar, CInitDialogBar)
	//{{AFX_MSG_MAP(CCharactorBar)
	ON_BN_CLICKED(IDC_BUTTON_HEAD, OnButtonHead)
	ON_BN_CLICKED(IDC_BUTTON_HAIR, OnButtonHair)
	ON_BN_CLICKED(IDC_BUTTON_SHOE, OnButtonShoe)
	ON_BN_CLICKED(IDC_BUTTON_UPPER, OnButtonUpper)
	ON_BN_CLICKED(IDC_BUTTON_HELM, OnButtonHelm)
	ON_BN_CLICKED(IDC_BUTTON_GLASSES, OnButtonGlasses)
	ON_BN_CLICKED(IDC_BUTTON_NOSE, OnButtonNose)
	ON_BN_CLICKED(IDC_BUTTON_MUSTACHE, OnButtonMustache)
	ON_BN_CLICKED(IDC_BUTTON_CLOAK, OnButtonCloak)
	ON_BN_CLICKED(IDC_BUTTON_FASHION_HEADWEAR, OnButtonFashionHeadwear)
	ON_BN_CLICKED(IDC_BUTTON_FASHION_UPPER, OnButtonFashionUpper)
	ON_BN_CLICKED(IDC_BUTTON_FASHION_LOWER, OnButtonFashionLower)
	ON_BN_CLICKED(IDC_BUTTON_FASHION_SHOE, OnButtonFashionShoe)
	ON_BN_CLICKED(IDC_BUTTON_MOUNT, OnButtonMount)
	ON_BN_CLICKED(IDC_RADIO_NORMAL, OnRadioNormal)
	ON_BN_CLICKED(IDC_RADIO_FASHION, OnRadioFashion)
	ON_BN_CLICKED(IDC_BUTTON_MOUNT_RIDE, OnButtonMountRide)
	ON_BN_CLICKED(IDC_BUTTON_EAR, OnButtonEar)
	ON_BN_CLICKED(IDC_BUTTON_TAIL, OnButtonTail)
	ON_BN_CLICKED(IDC_BUTTON_FASHION_EAR, OnButtonFashionEar)
	ON_BN_CLICKED(IDC_BUTTON_FASHION_TAIL, OnButtonFashionTail)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_EAR, OnUpdateButtonEar)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_TAIL, OnUpdateButtonTail)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_FASHION_EAR, OnUpdateButtonFashionEar)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_FASHION_TAIL, OnUpdateButtonFashionTail)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_HEAD, OnUpdateButtonHead)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_HAIR, OnUpdateButtonHair)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_SHOE, OnUpdateButtonShoe)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_UPPER, OnUpdateButtonUpper)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_HELM, OnUpdateButtonHelm)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_GLASSES, OnUpdateButtonGlasses)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_NOSE, OnUpdateButtonNose)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_MUSTACHE, OnUpdateButtonMustache)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_CLOAK, OnUpdateButtonCloak)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_FASHION_HEADWEAR, OnUpdateButtonFashionHeadwear)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_FASHION_UPPER, OnUpdateButtonFashionUpper)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_FASHION_LOWER, OnUpdateButtonFashionLower)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_FASHION_SHOE, OnUpdateButtonFashionShoe)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_MOUNT, OnUpdateButtonMount)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_MOUNT_RIDE, OnUpdateButtonMountRide)
	ON_CBN_SELCHANGE(IDC_COMBO_TRANSFORM, OnEditchangeComboShape)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCharactorBar message handlers

void CCharactorBar::OnButtonHead() 
{
	// TODO: Add your control notification handler code here
	if(g_pRenderableObject)
	{
		int nType = g_pRenderableObject->GetObjectType();
		switch(nType) 
		{
		case RENDERABLE_OBJECT_PLAYER:
			{
				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.SetPartType(PART_HEAD,m_strHead);
				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.ShowWindow(SW_SHOW);
				break;
			}
		}
	}
}

void CCharactorBar::OnButtonHair() 
{
	// TODO: Add your control notification handler code here
	if(g_pRenderableObject)
	{
		int nType = g_pRenderableObject->GetObjectType();
		switch(nType) 
		{
		case RENDERABLE_OBJECT_PLAYER:
			{
				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.SetPartType(PART_HAIR,m_strHair);
				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.ShowWindow(SW_SHOW);
				break;
			}
		}
	}
}

void CCharactorBar::OnButtonShoe() 
{
	// TODO: Add your control notification handler code here
	if(g_pRenderableObject)
	{
		int nType = g_pRenderableObject->GetObjectType();
		switch(nType) 
		{
		case RENDERABLE_OBJECT_PLAYER:
			{
				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.SetPartType(PART_FOOT,m_strFoot);
				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.ShowWindow(SW_SHOW);
				break;
			}
		}
	}
}

void CCharactorBar::OnButtonUpper() 
{
	// TODO: Add your control notification handler code here
	if(g_pRenderableObject)
	{
		int nType = g_pRenderableObject->GetObjectType();
		switch(nType) 
		{
		case RENDERABLE_OBJECT_PLAYER:
			{
				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.SetPartType(PART_BODY,m_strUpper);
				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.ShowWindow(SW_SHOW);
				break;
			}
		}
	}
}

void CCharactorBar::OnButtonHelm() 
{
	// TODO: Add your control notification handler code here
	if(g_pRenderableObject)
	{
		int nType = g_pRenderableObject->GetObjectType();
		switch(nType) 
		{
		case RENDERABLE_OBJECT_PLAYER:
			{
				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.SetPartType(PART_HELM,m_strHelm);
				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.ShowWindow(SW_SHOW);
				break;
			}
		}
	}
}

void CCharactorBar::OnButtonGlasses() 
{
//	// TODO: Add your control notification handler code here
//	if(g_pRenderableObject)
//	{
//		int nType = g_pRenderableObject->GetObjectType();
//		switch(nType) 
//		{
//		case RENDERABLE_OBJECT_PLAYER:
//			{
//				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.SetPartType(PART_GLASSES,m_strGlasses);
//				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.ShowWindow(SW_SHOW);
//				break;
//			}
//		}
//	}
}

void CCharactorBar::OnButtonNose() 
{
//	// TODO: Add your control notification handler code here
//	if(g_pRenderableObject)
//	{
//		int nType = g_pRenderableObject->GetObjectType();
//		switch(nType) 
//		{
//		case RENDERABLE_OBJECT_PLAYER:
//			{
//				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.SetPartType(PART_NOSE,m_strNose);
//				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.ShowWindow(SW_SHOW);
//				break;
//			}
//		}
//	}
}

void CCharactorBar::OnButtonMustache() 
{
//	// TODO: Add your control notification handler code here
//	if(g_pRenderableObject)
//	{
//		int nType = g_pRenderableObject->GetObjectType();
//		switch(nType) 
//		{
//		case RENDERABLE_OBJECT_PLAYER:
//			{
//				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.SetPartType(PART_MUSTACHE,m_strMustache);
//				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.ShowWindow(SW_SHOW);
//				break;
//			}
//		}
//	}
}

void CCharactorBar::OnButtonCloak() 
{
//	// TODO: Add your control notification handler code here
//	if(g_pRenderableObject)
//	{
//		int nType = g_pRenderableObject->GetObjectType();
//		switch(nType) 
//		{
//		case RENDERABLE_OBJECT_PLAYER:
//			{
//				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.SetPartType(PART_CLOAK,m_strCloak);
//				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.ShowWindow(SW_SHOW);
//				break;
//			}
//		}
//	}
}

void CCharactorBar::OnButtonFashionHeadwear() 
{
	// TODO: Add your control notification handler code here
	if(g_pRenderableObject)
	{
		int nType = g_pRenderableObject->GetObjectType();
		switch(nType) 
		{
		case RENDERABLE_OBJECT_PLAYER:
			{
				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.SetPartType(PART_FASHION_HEADWEAR,m_strFashionHeadwear);
				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.ShowWindow(SW_SHOW);
				break;
			}
		}
	}
}

void CCharactorBar::OnButtonFashionUpper() 
{
	// TODO: Add your control notification handler code here
	if(g_pRenderableObject)
	{
		int nType = g_pRenderableObject->GetObjectType();
		switch(nType) 
		{
		case RENDERABLE_OBJECT_PLAYER:
			{
				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.SetPartType(PART_FASHION_UPPER_BODY,m_strFashionUpper);
				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.ShowWindow(SW_SHOW);
				break;
			}
		}
	}
}
//
void CCharactorBar::OnButtonFashionLower() 
{
//	// TODO: Add your control notification handler code here
//	if(g_pRenderableObject)
//	{
//		int nType = g_pRenderableObject->GetObjectType();
//		switch(nType) 
//		{
//		case RENDERABLE_OBJECT_PLAYER:
//			{
//				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.SetPartType(PART_FASHION_LOWER,m_strFashionLower);
//				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.ShowWindow(SW_SHOW);
//				break;
//			}
//		}
//	}
}

void CCharactorBar::OnButtonFashionShoe() 
{
	// TODO: Add your control notification handler code here
	if(g_pRenderableObject)
	{
		int nType = g_pRenderableObject->GetObjectType();
		switch(nType) 
		{
		case RENDERABLE_OBJECT_PLAYER:
			{
				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.SetPartType(PART_FASHION_FOOT,m_strFashionFoot);
				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.ShowWindow(SW_SHOW);
				break;
			}
		}
	}
}

void CCharactorBar::OnUpdateButtonEar(CCmdUI *pCmdUI)
{
	if(g_pRenderableObject)
	{
		int nType = g_pRenderableObject->GetObjectType();
		if(nType == RENDERABLE_OBJECT_PLAYER){
			CPlayer* pPlayer = (CPlayer*)g_pRenderableObject;
			if(pPlayer->GetSect()==SECT_11) pCmdUI->Enable(TRUE);
			else pCmdUI->Enable(FALSE);
		}
	}
}

void CCharactorBar::OnUpdateButtonTail(CCmdUI *pCmdUI)
{
	if(g_pRenderableObject)
	{
		int nType = g_pRenderableObject->GetObjectType();
		if(nType == RENDERABLE_OBJECT_PLAYER){
			CPlayer* pPlayer = (CPlayer*)g_pRenderableObject;
			if(pPlayer->GetSect()==SECT_11) pCmdUI->Enable(TRUE);
			else pCmdUI->Enable(FALSE);
		}
	}
}

void CCharactorBar::OnUpdateButtonFashionEar(CCmdUI *pCmdUI)
{
	if(g_pRenderableObject)
	{
		int nType = g_pRenderableObject->GetObjectType();
		if(nType == RENDERABLE_OBJECT_PLAYER){
			CPlayer* pPlayer = (CPlayer*)g_pRenderableObject;
			if(pPlayer->GetSect()==SECT_11) pCmdUI->Enable(TRUE);
			else pCmdUI->Enable(FALSE);
		}
	}
}

void CCharactorBar::OnUpdateButtonFashionTail(CCmdUI *pCmdUI)
{
	if(g_pRenderableObject)
	{
		int nType = g_pRenderableObject->GetObjectType();
		if(nType == RENDERABLE_OBJECT_PLAYER){
			CPlayer* pPlayer = (CPlayer*)g_pRenderableObject;
			if(pPlayer->GetSect()==SECT_11) pCmdUI->Enable(TRUE);
			else pCmdUI->Enable(FALSE);
		}
	}
}

void CCharactorBar::OnUpdateButtonHead(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CCharactorBar::OnUpdateButtonHair(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CCharactorBar::OnUpdateButtonShoe(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CCharactorBar::OnUpdateButtonUpper(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CCharactorBar::OnUpdateButtonHelm(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CCharactorBar::OnUpdateButtonGlasses(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CCharactorBar::OnUpdateButtonNose(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CCharactorBar::OnUpdateButtonMustache(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CCharactorBar::OnUpdateButtonCloak(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CCharactorBar::OnUpdateButtonFashionHeadwear(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CCharactorBar::OnUpdateButtonFashionUpper(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CCharactorBar::OnUpdateButtonFashionLower(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CCharactorBar::OnUpdateButtonFashionShoe(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CCharactorBar::OnUpdateButtonMount(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CCharactorBar::OnUpdateButtonMountRide(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CCharactorBar::OnButtonMount() 
{
	// TODO: Add your control notification handler code here
	if(g_pRenderableObject)
	{
		int nType = g_pRenderableObject->GetObjectType();
		switch(nType) 
		{
		case RENDERABLE_OBJECT_PLAYER:
			{
				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.SetPartType(PART_WEAPON,m_strWeapon);
				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.ShowWindow(SW_SHOW);
				break;
			}
		}
	}
}

void CCharactorBar::Init()
{
	// TODO: Add extra initialization here
	if(g_bFashion)
	{
		CButton *pButton = (CButton *)GetDlgItem(IDC_RADIO_NORMAL);
		pButton->SetCheck(false);
		pButton = (CButton *)GetDlgItem(IDC_RADIO_FASHION);
		pButton->SetCheck(true);
	}else
	{
		CButton *pButton = (CButton *)GetDlgItem(IDC_RADIO_FASHION);
		pButton->SetCheck(false);
		pButton = (CButton *)GetDlgItem(IDC_RADIO_NORMAL);
		pButton->SetCheck(true);
	}
}


void CCharactorBar::OnRadioNormal() 
{
	// TODO: Add your control notification handler code here
	g_bFashion = false;
}

void CCharactorBar::OnRadioFashion() 
{
	// TODO: Add your control notification handler code here
	g_bFashion = true;
}

void CCharactorBar::UpdatePart(CString *pSkinPart)
{
	m_strHead = pSkinPart[SKIN_HEAD_INDEX];
	m_strHair = pSkinPart[SKIN_HAIR_INDEX];
	m_strEar  = pSkinPart[SKIN_EAR_INDEX];
	m_strTail = pSkinPart[SKIN_TAIL_INDEX];
	m_strUpper = pSkinPart[SKIN_BODY_INDEX];
	m_strFoot = pSkinPart[SKIN_FOOT_INDEX];
	m_strHelm = pSkinPart[SKIN_HELM_INDEX];
	//m_strGlasses = pSkinPart[SKIN_GLASSES_INDEX];
	//m_strNose = pSkinPart[SKIN_NOSE_INDEX];
	//m_strMustache = pSkinPart[SKIN_MUSTACHE_INDEX];
	//m_strCloak = pSkinPart[SKIN_CLOAK_INDEX];
	m_strFashionUpper = pSkinPart[SKIN_FASHION_UPPER_BODY_INDEX];
	//m_strFashionLower = pSkinPart[SKIN_FASHION_LOWER_INDEX];
	m_strFashionFoot = pSkinPart[SKIN_FASHION_FOOT_INDEX];
	m_strFashionHeadwear = pSkinPart[SKIN_FASHION_HEADWEAR_INDEX];
	m_strFashionEar      = pSkinPart[SKIN_FASHION_EAR_INDEX];
	m_strFashionTail     = pSkinPart[SKIN_FASHION_TAIL_INDEX];
	m_strWeapon = pSkinPart[SKIN_FASHION_TAIL_INDEX + 1];//weapon
	m_strRide   = pSkinPart[SKIN_FASHION_TAIL_INDEX + 2];//ride
	UpdateData(false);
}

void CCharactorBar::OnButtonMountRide() 
{
	// TODO: Add your control notification handler code here
	if(g_pRenderableObject)
	{
		int nType = g_pRenderableObject->GetObjectType();
		switch(nType) 
		{
		case RENDERABLE_OBJECT_PLAYER:
			{
				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.SetPartType(PART_RIDE,m_strRide);
				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.ShowWindow(SW_SHOW);
				break;
			}
		}
	}
}

void CCharactorBar::OnButtonEar() 
{
	// TODO: Add your control notification handler code here
	if (g_pRenderableObject) {
		int nType = g_pRenderableObject->GetObjectType();
		switch(nType) {
		case RENDERABLE_OBJECT_PLAYER:
				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.SetPartType(PART_EAR,m_strEar);
				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.ShowWindow(SW_SHOW);
			break;
		default:
			break;
		}
	}	
}

void CCharactorBar::OnButtonTail() 
{
	// TODO: Add your control notification handler code here
	if (g_pRenderableObject) {
		int nType = g_pRenderableObject->GetObjectType();
		switch(nType) {
		case RENDERABLE_OBJECT_PLAYER:
				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.SetPartType(PART_TAIL,m_strEar);
				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.ShowWindow(SW_SHOW);
			break;
		default:
			break;
		}
	}	
}


void CCharactorBar::OnButtonFashionEar() 
{
	// TODO: Add your control notification handler code here
	if (g_pRenderableObject) {
		int nType = g_pRenderableObject->GetObjectType();
		switch(nType) {
		case RENDERABLE_OBJECT_PLAYER:
				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.SetPartType(PART_FASHION_EAR,m_strEar);
				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.ShowWindow(SW_SHOW);
			break;
		default:
			break;
		}
	}	
}

void CCharactorBar::OnButtonFashionTail() 
{
	// TODO: Add your control notification handler code here
	if (g_pRenderableObject) {
		int nType = g_pRenderableObject->GetObjectType();
		switch(nType) {
		case RENDERABLE_OBJECT_PLAYER:
				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.SetPartType(PART_FASHION_TAIL,m_strEar);
				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.ShowWindow(SW_SHOW);
			break;
		default:
			break;
		}
	}	
}

void CCharactorBar::OnEditchangeComboShape() 
{
	// TODO: Add your control notification handler code here
	DWORD id = m_comboShape.GetItemData(m_comboShape.GetCurSel());
	CPlayer* pPlayer;
	if(g_pRenderableObject){
		int nType = g_pRenderableObject->GetObjectType();
		switch(nType){
		case RENDERABLE_OBJECT_PLAYER:
			pPlayer = (CPlayer*)g_pRenderableObject;
			if(id){
				pPlayer->LoadChangedModel(id);
				pPlayer->SetChangedModle(true);
			}else 
				pPlayer->SetChangedModle(false);
			pPlayer->RecreateActionList();
			pPlayer->RecreateSkillList();
			break;
		default:
			break;
		}
	}
	
}

void CCharactorBar::AddShape(CString strShapeName){
	m_comboShape.AddString(strShapeName);
}


void CCharactorBar::ClearShapes(){
	int n = m_comboShape.GetCount();
	for (int i=0;i<n;i++) {
		m_comboShape.DeleteString(0);
	}
}

void CCharactorBar::SetDefaultShape(){
		if(m_comboShape.GetCount() > 0)
			m_comboShape.SetCurSel(0);
}
