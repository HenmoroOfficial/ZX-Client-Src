// DlgOptions.cpp : implementation file
//

#include "Global.h"
#include "DlgOptions.h"
#include "MainFrm.h"
#include "AC.h"
#include "EL_BackMusic.h"
#include "ElementMap.h"
#include "HostPlayer.h"

//#define new A_DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// CDlgOptions dialog


CDlgOptions::CDlgOptions(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOptions::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOptions)
	m_bAlongGround = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOptions)
	DDX_Control(pDX, IDC_SLIDER_SFX, m_sliderSfx);
	DDX_Control(pDX, IDC_SLIDER_MUSIC, m_sliderMusic);
	DDX_Control(pDX, IDC_EDIT_CAMERAROT, m_CamRotEdit);
	DDX_Control(pDX, IDC_EDIT_CAMERAMOVE, m_CamMoveEdit);
	DDX_Control(pDX, IDC_SPIN_CAMERAROT, m_CamRotSpin);
	DDX_Control(pDX, IDC_SPIN_CAMERAMOVE, m_CamMoveSpin);
	DDX_Check(pDX, IDC_CHECK_ALONGGROUND, m_bAlongGround);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgOptions, CDialog)
	//{{AFX_MSG_MAP(CDlgOptions)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_CAMERAMOVE, OnDeltaposSpinCameramove)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_CAMERAROT, OnDeltaposSpinCamerarot)
	ON_BN_CLICKED(IDC_CHECK_ALONGGROUND, OnCheckAlongground)
	ON_BN_CLICKED(IDC_CHECK_OBJECT_ADD_CONTINUE, OnCheckObjectAddContinue)
	ON_EN_KILLFOCUS(IDC_EDIT_CAMERAMOVE, OnKillfocusEditCameramove)
	ON_EN_KILLFOCUS(IDC_EDIT_CAMERAROT, OnKillfocusEditCamerarot)
	ON_BN_CLICKED(IDC_CHECK_PICK_STATICOBJECT, OnCheckPickStaticobject)
	ON_BN_CLICKED(IDC_CHECK_PICK_LIGHT, OnCheckPickLight)
	ON_BN_CLICKED(IDC_CHECK_PICK_BOXAREA, OnCheckPickBoxarea)
	ON_BN_CLICKED(IDC_CHECK_PICK_DUMMY, OnCheckPickDummy)
	ON_BN_CLICKED(IDC_CHECK_PICK_SPECIALLY, OnCheckPickSpecially)
	ON_BN_CLICKED(IDC_CHECK_PICK_WATER, OnCheckPickWater)
	ON_BN_CLICKED(IDC_CHECK_PICK_AIAREA, OnCheckPickAiarea)
	ON_BN_CLICKED(IDC_CHECK_PICK_PATH, OnCheckPickPath)
	ON_BN_CLICKED(IDC_CHECK_PICK_AUDIO, OnCheckPickAudio)
	ON_BN_CLICKED(IDC_CHECK_PICK_CRITTER, OnCheckPickCritter)
	ON_BN_CLICKED(IDC_CHECK_PICK_SKIN_MODEL, OnCheckPickSkinModel)
	ON_BN_CLICKED(IDC_CHECK_PICK_FIXEDNPC, OnCheckPickFixednpc)
	ON_BN_CLICKED(IDC_CHECK_SHOW_HULL, OnCheckShowHull)
	ON_BN_CLICKED(IDC_CHECK_PICK_PRECINCT, OnCheckPickPrecinct)
	ON_BN_CLICKED(IDC_CHECK_PICK_GATHER, OnCheckPickGather)
	ON_BN_CLICKED(IDC_CHECK_BEZIER_PT_SNAP, OnCheckBezierPtSnap)
	ON_BN_CLICKED(IDC_CHECK_PICK_RANGE, OnCheckPickRange)
	ON_BN_CLICKED(IDC_CHECK_PICK_CLOUD_BOX, OnCheckPickCloudBox)
	ON_BN_CLICKED(IDC_CHECK_PICK_INSTANCE_BOX, OnCheckPickInstanceBox)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_MUSIC, OnReleasedcaptureSliderMusic)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_SFX, OnReleasedcaptureSliderSfx)
	ON_BN_CLICKED(IDC_CHECK_PICK_DYNAMIC, OnCheckPickDynamic)
	ON_BN_CLICKED(IDC_CHECK_SHOW_BOUNDBOX, OnCheckShowBoundbox)
	ON_BN_CLICKED(IDC_BUTTON_SELECTEDALL, OnSelectedAll)
	ON_BN_CLICKED(IDC_BUTTON_SELECTEDNULL, OnSelectedNull)
	ON_BN_CLICKED(IDC_CHECK_PICK_BUFF_REGION, OnCheckPickBuffRegion)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_CAMERA_FOLLOW, &CDlgOptions::OnBnClickedCheckCameraFollow)
	ON_BN_CLICKED(IDC_CHECK_NEIGHBOUR_TERRAIN_ONLY, &CDlgOptions::OnBnClickedCheckNeighbourTerrainOnly)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOptions message handlers

BOOL CDlgOptions::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//	Change edit boxes to flat style
	AUX_FlatEditBox(&m_CamMoveEdit);
	AUX_FlatEditBox(&m_CamRotEdit);

	CString str;
	str.Format("%.2f", g_Configs.fCameraMove);
	m_CamMoveEdit.SetWindowText(str);

	str.Format("%.2f", g_Configs.fCameraRot);
	m_CamRotEdit.SetWindowText(str);

	m_bAlongGround	= g_Configs.bAlongGround ? TRUE : FALSE;

	CButton *pButton = (CButton *)GetDlgItem(IDC_CHECK_OBJECT_ADD_CONTINUE);
	if(pButton) pButton->SetCheck(g_Configs.bObjectAddContinue);
	
	pButton = (CButton *)GetDlgItem(IDC_CHECK_PICK_STATICOBJECT);
	if(pButton) pButton->SetCheck(g_Configs.bPickStaticModel);

	pButton = (CButton *)GetDlgItem(IDC_CHECK_PICK_LIGHT);
	if(pButton) pButton->SetCheck(g_Configs.bPickLight);

	pButton = (CButton *)GetDlgItem(IDC_CHECK_PICK_AUDIO);
	if(pButton) pButton->SetCheck(g_Configs.bPickAudio);

	pButton = (CButton *)GetDlgItem(IDC_CHECK_PICK_BOXAREA);
	if(pButton) pButton->SetCheck(g_Configs.bPickBoxArea);

	pButton = (CButton *)GetDlgItem(IDC_CHECK_PICK_DUMMY);
	if(pButton) pButton->SetCheck(g_Configs.bPickDummy);

	pButton = (CButton *)GetDlgItem(IDC_CHECK_PICK_SPECIALLY);
	if(pButton) pButton->SetCheck(g_Configs.bPickSpecially);

	pButton = (CButton *)GetDlgItem(IDC_CHECK_PICK_WATER);
	if(pButton) pButton->SetCheck(g_Configs.bPickWater);

	pButton = (CButton *)GetDlgItem(IDC_CHECK_PICK_AIAREA);
	if(pButton) pButton->SetCheck(g_Configs.bPickAIArea);

	pButton = (CButton *)GetDlgItem(IDC_CHECK_PICK_PATH);
	if(pButton) pButton->SetCheck(g_Configs.bPickPath);

	pButton = (CButton *)GetDlgItem(IDC_CHECK_PICK_CRITTER);
	if(pButton) pButton->SetCheck(g_Configs.bPickCritterGroup);

	pButton = (CButton *)GetDlgItem(IDC_CHECK_PICK_SKIN_MODEL);
	if(pButton) pButton->SetCheck(g_Configs.bPickSkinModel);
	
	pButton = (CButton *)GetDlgItem(IDC_CHECK_PICK_FIXEDNPC);
	if(pButton) pButton->SetCheck(g_Configs.bPickFixedNpc);

	pButton = (CButton *)GetDlgItem(IDC_CHECK_SHOW_HULL);
	if(pButton) pButton->SetCheck(g_Configs.bShowModelHull);
	
	pButton = (CButton *)GetDlgItem(IDC_CHECK_SHOW_BOUNDBOX);
	if(pButton) pButton->SetCheck(g_Configs.bShowBoundBox);

	pButton = (CButton *)GetDlgItem(IDC_CHECK_CAMERA_FOLLOW);
	if(pButton) pButton->SetCheck(g_Configs.bHostMode);

	pButton = (CButton *)GetDlgItem(IDC_CHECK_NEIGHBOUR_TERRAIN_ONLY);
	if(pButton) pButton->SetCheck(g_Configs.bNeighborTerrainOnly);
	
	pButton = (CButton *)GetDlgItem(IDC_CHECK_PICK_GATHER);
	if(pButton) pButton->SetCheck(g_Configs.bPickGather);

	pButton = (CButton *)GetDlgItem(IDC_CHECK_PICK_PRECINCT);
	if(pButton) pButton->SetCheck(g_Configs.bPickPrecinct);

	pButton = (CButton *)GetDlgItem(IDC_CHECK_BEZIER_PT_SNAP);
	if(pButton) pButton->SetCheck(g_bBezierPtSnap);

	pButton = (CButton *)GetDlgItem(IDC_CHECK_PICK_RANGE);
	if(pButton) pButton->SetCheck(g_Configs.bPickRange);

	pButton = (CButton *)GetDlgItem(IDC_CHECK_PICK_CLOUD_BOX);
	if(pButton) pButton->SetCheck(g_Configs.bPickCloudBox);

	pButton = (CButton *)GetDlgItem(IDC_CHECK_PICK_INSTANCE_BOX);
	if(pButton) pButton->SetCheck(g_Configs.bPickInstanceBox);

	pButton = (CButton *)GetDlgItem(IDC_CHECK_PICK_DYNAMIC);
	if(pButton) pButton->SetCheck(g_Configs.bPickDynamic);
	
	pButton = (CButton *)GetDlgItem(IDC_CHECK_PICK_BUFF_REGION);
	if(pButton) pButton->SetCheck(g_Configs.bPickBuffRegion);

	UpdateData(FALSE);
	m_sliderMusic.SetRange(1,100);
	m_sliderSfx.SetRange(1,100);
	m_sliderMusic.SetPos(g_Configs.iMusicVolume);
	m_sliderSfx.SetPos(g_Configs.iSfxVolume);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgOptions::OnDeltaposSpinCameramove(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	*pResult = 0;

	g_Configs.fCameraMove += -pNMUpDown->iDelta * 0.1f;
	a_ClampFloor(g_Configs.fCameraMove, 0.0f);

	CString str;
	str.Format("%.2f", g_Configs.fCameraMove);
	m_CamMoveEdit.SetWindowText(str);
}

void CDlgOptions::OnDeltaposSpinCamerarot(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	*pResult = 0;

	g_Configs.fCameraRot += -pNMUpDown->iDelta * 0.1f;
	a_ClampFloor(g_Configs.fCameraRot, 0.0f);

	CString str;
	str.Format("%.2f", g_Configs.fCameraRot);
	m_CamRotEdit.SetWindowText(str);
}

//	Accept settings
void CDlgOptions::OnOK()
{
	if (!UpdateData(TRUE))
		return;

	CString str;
	m_CamMoveEdit.GetWindowText(str);
	g_Configs.fCameraMove = (float)atof(str);

	m_CamRotEdit.GetWindowText(str);
	g_Configs.fCameraRot = (float)atof(str);

	str.Format("%.2f", g_Configs.fCameraMove);
	m_CamMoveEdit.SetWindowText(str);

	str.Format("%.2f", g_Configs.fCameraRot);
	m_CamRotEdit.SetWindowText(str);

	//	Activate main window
	AUX_GetMainFrame()->SetFocus();
}

void CDlgOptions::OnCheckAlongground() 
{
	m_bAlongGround = !m_bAlongGround;
	g_Configs.bAlongGround = m_bAlongGround ? true : false;
}



void CDlgOptions::OnCheckObjectAddContinue() 
{
	// TODO: Add your control notification handler code here
	g_Configs.bObjectAddContinue = !g_Configs.bObjectAddContinue;
}

void CDlgOptions::OnKillfocusEditCameramove() 
{
	// TODO: Add your control notification handler code here
	CString str;
	m_CamMoveEdit.GetWindowText(str);
	g_Configs.fCameraMove = (float)atof(str);
}

void CDlgOptions::OnKillfocusEditCamerarot() 
{
	// TODO: Add your control notification handler code here
	CString str;
	m_CamRotEdit.GetWindowText(str);
	g_Configs.fCameraRot = (float)atof(str);
}



void CDlgOptions::OnCheckPickStaticobject() 
{
	// TODO: Add your control notification handler code here
	g_Configs.bPickStaticModel = !g_Configs.bPickStaticModel;
}

void CDlgOptions::OnCheckPickLight() 
{
	// TODO: Add your control notification handler code here
	g_Configs.bPickLight = !g_Configs.bPickLight;
}

void CDlgOptions::OnCheckPickBoxarea() 
{
	// TODO: Add your control notification handler code here
	g_Configs.bPickBoxArea = !g_Configs.bPickBoxArea;
}

void CDlgOptions::OnCheckPickDummy() 
{
	// TODO: Add your control notification handler code here
	g_Configs.bPickDummy = !g_Configs.bPickDummy;
}

void CDlgOptions::OnCheckPickSpecially() 
{
	// TODO: Add your control notification handler code here
	g_Configs.bPickSpecially = !g_Configs.bPickSpecially;
}

void CDlgOptions::OnCheckPickWater() 
{
	// TODO: Add your control notification handler code here
	g_Configs.bPickWater = !g_Configs.bPickWater;
}

void CDlgOptions::OnCheckPickAiarea() 
{
	// TODO: Add your control notification handler code here
	g_Configs.bPickAIArea = !g_Configs.bPickAIArea;
}

void CDlgOptions::OnCheckPickPath() 
{
	// TODO: Add your control notification handler code here
	g_Configs.bPickPath = !g_Configs.bPickPath;
}

void CDlgOptions::OnCheckPickAudio() 
{
	// TODO: Add your control notification handler code here
	g_Configs.bPickAudio = !g_Configs.bPickAudio;
}

void CDlgOptions::OnCheckPickCritter() 
{
	// TODO: Add your control notification handler code here
	g_Configs.bPickCritterGroup = !g_Configs.bPickCritterGroup;
}

void CDlgOptions::OnCheckPickSkinModel() 
{
	// TODO: Add your control notification handler code here
	g_Configs.bPickSkinModel = !g_Configs.bPickSkinModel;
}

void CDlgOptions::OnCheckPickFixednpc() 
{
	// TODO: Add your control notification handler code here
	g_Configs.bPickFixedNpc = !g_Configs.bPickFixedNpc;
}

void CDlgOptions::OnCheckShowHull() 
{
	// TODO: Add your control notification handler code here
	g_Configs.bShowModelHull = !g_Configs.bShowModelHull;
}

void CDlgOptions::OnCheckPickPrecinct() 
{
	// TODO: Add your control notification handler code here
	g_Configs.bPickPrecinct = !g_Configs.bPickPrecinct;
}

void CDlgOptions::OnCheckPickGather() 
{
	// TODO: Add your control notification handler code here
	g_Configs.bPickGather = !g_Configs.bPickGather;
}

void CDlgOptions::OnCheckBezierPtSnap() 
{
	// TODO: Add your control notification handler code here
	g_bBezierPtSnap = !g_bBezierPtSnap;
}

void CDlgOptions::OnCheckPickRange() 
{
	// TODO: Add your control notification handler code here
	g_Configs.bPickRange = !g_Configs.bPickRange;
}

void CDlgOptions::OnCheckPickCloudBox() 
{
	// TODO: Add your control notification handler code here
	g_Configs.bPickCloudBox = !g_Configs.bPickCloudBox;
}

void CDlgOptions::OnCheckPickInstanceBox() 
{
	// TODO: Add your control notification handler code here
	g_Configs.bPickInstanceBox = !g_Configs.bPickInstanceBox;
}

void CDlgOptions::OnReleasedcaptureSliderMusic(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
    g_Configs.iMusicVolume = m_sliderMusic.GetPos();
	g_BackMusic.SetVolume(g_Configs.iMusicVolume);
	*pResult = 0;
}

void CDlgOptions::OnReleasedcaptureSliderSfx(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	g_Configs.iSfxVolume = m_sliderSfx.GetPos();
	g_BackMusic.SetSFXVolume(g_Configs.iSfxVolume);
	*pResult = 0;
}

void CDlgOptions::OnCheckPickDynamic() 
{
	// TODO: Add your control notification handler code here
	g_Configs.bPickDynamic = !g_Configs.bPickDynamic;
}

void CDlgOptions::OnCheckShowBoundbox() 
{
	// TODO: Add your control notification handler code here
	g_Configs.bShowBoundBox = !g_Configs.bShowBoundBox;	
}

void CDlgOptions::OnCheckPickBuffRegion() 
{
	// TODO: Add your control notification handler code here
	g_Configs.bPickBuffRegion = !g_Configs.bPickBuffRegion;
}


void CDlgOptions::OnSelectedAll()
{
	g_Configs.bPickStaticModel = true;   
	g_Configs.bPickLight = true;
	g_Configs.bPickAudio = true;
	g_Configs.bPickBoxArea = true;
	g_Configs.bPickDummy = true;
	g_Configs.bPickSpecially = true;
	g_Configs.bPickWater = true;
	g_Configs.bPickAIArea = true;
	g_Configs.bPickPath = true;
	g_Configs.bPickSkinModel = true;
	g_Configs.bPickCritterGroup = true;
	g_Configs.bPickFixedNpc = true;
	g_Configs.bPickPrecinct = true;
	g_Configs.bPickGather = true;
	g_Configs.bPickRange = true;
	g_Configs.bPickCloudBox = true;
	g_Configs.bPickInstanceBox = true;
	g_Configs.bPickDynamic = true;

	
	((CButton*)GetDlgItem(IDC_CHECK_PICK_STATICOBJECT))->SetCheck(BST_CHECKED);
	((CButton*)GetDlgItem(IDC_CHECK_PICK_LIGHT))->SetCheck(BST_CHECKED);
	((CButton*)GetDlgItem(IDC_CHECK_PICK_BOXAREA))->SetCheck(BST_CHECKED);
	((CButton*)GetDlgItem(IDC_CHECK_PICK_DUMMY))->SetCheck(BST_CHECKED);
	((CButton*)GetDlgItem(IDC_CHECK_PICK_SPECIALLY))->SetCheck(BST_CHECKED);
	((CButton*)GetDlgItem(IDC_CHECK_PICK_WATER))->SetCheck(BST_CHECKED);
	((CButton*)GetDlgItem(IDC_CHECK_PICK_AIAREA))->SetCheck(BST_CHECKED);
	((CButton*)GetDlgItem(IDC_CHECK_PICK_PATH))->SetCheck(BST_CHECKED);
	((CButton*)GetDlgItem(IDC_CHECK_PICK_AUDIO))->SetCheck(BST_CHECKED);
	((CButton*)GetDlgItem(IDC_CHECK_PICK_CRITTER))->SetCheck(BST_CHECKED);
	((CButton*)GetDlgItem(IDC_CHECK_PICK_SKIN_MODEL))->SetCheck(BST_CHECKED);
	((CButton*)GetDlgItem(IDC_CHECK_PICK_FIXEDNPC))->SetCheck(BST_CHECKED);
	((CButton*)GetDlgItem(IDC_CHECK_PICK_PRECINCT))->SetCheck(BST_CHECKED);
	((CButton*)GetDlgItem(IDC_CHECK_PICK_GATHER))->SetCheck(BST_CHECKED);
	((CButton*)GetDlgItem(IDC_CHECK_PICK_RANGE))->SetCheck(BST_CHECKED);
	((CButton*)GetDlgItem(IDC_CHECK_PICK_CLOUD_BOX))->SetCheck(BST_CHECKED);
	((CButton*)GetDlgItem(IDC_CHECK_PICK_INSTANCE_BOX))->SetCheck(BST_CHECKED);
	((CButton*)GetDlgItem(IDC_CHECK_PICK_DYNAMIC))->SetCheck(BST_CHECKED);


}

void CDlgOptions::OnSelectedNull()
{
	g_Configs.bPickStaticModel = false;   
	g_Configs.bPickLight = false;
	g_Configs.bPickAudio = false;
	g_Configs.bPickBoxArea = false;
	g_Configs.bPickDummy = false;
	g_Configs.bPickSpecially = false;
	g_Configs.bPickWater = false;
	g_Configs.bPickAIArea = false;
	g_Configs.bPickPath = false;
	g_Configs.bPickSkinModel = false;
	g_Configs.bPickCritterGroup = false;
	g_Configs.bPickFixedNpc = false;
	g_Configs.bPickPrecinct = false;
	g_Configs.bPickGather = false;
	g_Configs.bPickRange = false;
	g_Configs.bPickCloudBox = false;
	g_Configs.bPickInstanceBox = false;
	g_Configs.bPickDynamic = false;
	((CButton*)GetDlgItem(IDC_CHECK_PICK_STATICOBJECT))->SetCheck(BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_CHECK_PICK_LIGHT))->SetCheck(BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_CHECK_PICK_BOXAREA))->SetCheck(BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_CHECK_PICK_DUMMY))->SetCheck(BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_CHECK_PICK_SPECIALLY))->SetCheck(BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_CHECK_PICK_WATER))->SetCheck(BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_CHECK_PICK_AIAREA))->SetCheck(BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_CHECK_PICK_PATH))->SetCheck(BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_CHECK_PICK_AUDIO))->SetCheck(BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_CHECK_PICK_CRITTER))->SetCheck(BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_CHECK_PICK_SKIN_MODEL))->SetCheck(BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_CHECK_PICK_FIXEDNPC))->SetCheck(BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_CHECK_PICK_PRECINCT))->SetCheck(BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_CHECK_PICK_GATHER))->SetCheck(BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_CHECK_PICK_RANGE))->SetCheck(BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_CHECK_PICK_CLOUD_BOX))->SetCheck(BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_CHECK_PICK_INSTANCE_BOX))->SetCheck(BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_CHECK_PICK_DYNAMIC))->SetCheck(BST_UNCHECKED);

}





void CDlgOptions::OnBnClickedCheckCameraFollow()
{
	// TODO: Add your control notification handler code here
	g_Configs.bHostMode = ((CButton*)GetDlgItem(IDC_CHECK_CAMERA_FOLLOW))->GetCheck();
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if (pMap && pMap->GetHostPlayer())
	{
		pMap->GetHostPlayer()->SetStart(g_Configs.bHostMode);
	}
}

void CDlgOptions::OnBnClickedCheckNeighbourTerrainOnly()
{
	// TODO: Add your control notification handler code here
	g_Configs.bNeighborTerrainOnly = ((CButton*)GetDlgItem(IDC_CHECK_NEIGHBOUR_TERRAIN_ONLY))->GetCheck();	
}
