/*
 * FILE: EC_CustomizeMgr.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Liyi, 2005/8/18
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_CustomizeMgr.h"
#include "EC_Player.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_ShadowRender.h"
#include "EC_GameSession.h"

#ifdef BACK_VERSION
#include "EC_Viewport.h"
#else
#include "ECViewport.h"
#endif
#include "EC_World.h"
#include "EC_Player.h"
#include "EC_LoginPlayer.h"
#include "EC_Face.h"
#include "EC_LoginUIMan.h"
#include "EC_UIManager.h"

#include "aui/AUIManager.h"
#include "DlgCustomizeFace.h"
#include "EC_Face.h"
#include "EC_CustomizeBound.h"
#include "elementdataman.h"
#include "EC_Model.h"

#include "aui/AUIStillImageButton.h"
#include "DlgCustomizeEar.h"
#include "DlgCustomizeEye.h"
#include "DlgCustomizeEyebrow.h"
#include "DlgCustomizeFace.h"
#include "DlgCustomizeHair.h"
#include "DlgCustomizeMouth.h"
#include "DlgCustomizeNose.h"
#include "DlgCustomizePaint.h"
#include "DlgCustomizePre.h"
#include "DlgCustomizeStature.h"
#include "DlgCustomizeAdv.h"
#include "DlgCustomizeUserPic.h"
#include "DlgCustomizeMoveCamera.h"
#include "DlgCustomize.h"


//-------------------------------------------------------------------
CECCustomizeMgr::CECCustomizeMgr()
{
	m_fMoveZDist = 0;
	m_fMoveXDist = 0;
	m_fMoveYDist = 0;

	m_bUnsymmetrical = false;

	//是否载入变形丸相关的一些特殊数据
	m_bLoadFacePillData = false;

	m_vPosDaltaM = A3DVECTOR3(0.0f, 0.7f, -12.0f);
	m_vPosDaltaF = A3DVECTOR3(0.0f, 0.5f, -13.0f);
}
//-------------------------------------------------------------------
CECCustomizeMgr::~CECCustomizeMgr()
{

}

//--------------------------------------------------------------------
bool CECCustomizeMgr::Init(CustomizeInitParam_t cusInitParam)
{
	m_pAUIManager = cusInitParam.pAUImanager;
	m_pCurPlayer = cusInitParam.pCurPlayer;
	m_bCallForCreate = cusInitParam.bCallForCreate;
	m_vNPCPos = cusInitParam.vPos;
	m_vNPCDir = cusInitParam.vDir;
	
	HideAllOldDlg();
	
	do {
		
		if( cusInitParam.nFaceTicketID == FACE_TICKET_DEFAULT)
		{
			m_pSliderBound = g_CustomizeBoundMgr.GetCustomizeBound("facedata\\整容卷\\普通整容卷限制参数.ini");
			m_bUnsymmetrical = false;
		}
		else 
		{
			elementdataman* pElementDataMan;
			pElementDataMan= g_pGame->GetElementDataMan ();
			DATA_TYPE dtFaceData;
			FACETICKET_ESSENCE* pFaceTicket = (FACETICKET_ESSENCE*)pElementDataMan->get_data_ptr(cusInitParam.nFaceTicketID, ID_SPACE_ESSENCE, dtFaceData);
			
			if(pFaceTicket)
			{
				m_pSliderBound = g_CustomizeBoundMgr.GetCustomizeBound(pFaceTicket->bound_file);
				
				if( pFaceTicket->unsymmetrical == 0)
				{
					m_bUnsymmetrical = false;
				}
				else
				{
					m_bUnsymmetrical = true;
				}
			}
			else
			{
				assert(0 && "Face Ticket not found !");
			}

		}

		if (!m_pSliderBound) 
		{
			assert(0 && "Couldn't Load the CustomizeBound File!");
			break;
		}
		
		//判断是否载入变形丸特有的数据
		if( strstr(GetCommandLineA(), "facepill"))
		{
			m_bLoadFacePillData = true;
		}

		m_pCurPlayer->StoreCustomizeData();
		
		if (!InitDefaultData()) break;
		
		if (!InitDialogs()) break;
		
		if (!InitSwithButtons()) break;
		
		return true;

	} while(false);

	return false;
}


void CECCustomizeMgr::SwitchDialog(const char * szCommand)
{
	AString strCommand = szCommand;

	CDlgCustomizeBase *pDlg = NULL;
	if (strCommand == "ChooseFace")
		pDlg = m_pDlgCustomizeFace;
	else if (strCommand == "ChooseEye")
		pDlg = m_pDlgCustomizeEye;
	else if (strCommand == "ChooseEyebrow")
		pDlg = m_pDlgCustomizeEyebrow;
	else if (strCommand == "ChooseNose")
		pDlg = m_pDlgCustomizeNose;
	else if (strCommand == "ChooseMouth")
		pDlg = m_pDlgCustomizeMouth;
	else if (strCommand == "ChooseEar")
		pDlg = m_pDlgCustomizeEar;
	else if (strCommand == "ChooseHair")
		pDlg = m_pDlgCustomizeHair;
	else if (strCommand == "ChooseFacePainting")
		pDlg = m_pDlgCustomizePaint;
	else if (strCommand == "ChooseStature")
		pDlg = m_pDlgCustomizeStature;
	else if (strCommand == "ChoosePreCustomize")
		pDlg = m_pDlgCustomizePre;
	else if (strCommand == "AdvancedOption")
		pDlg = m_pDlgCustomizeAdv;

	ASSERT(pDlg);

	// set button state
	m_pBtnCurChoose->SetPushed(false);
	
	if( pDlg->GetSwithButton())
	{
		m_pBtnCurChoose = pDlg->GetSwithButton();
	}

	m_pBtnCurChoose->SetPushed(true);

	// hide/show dialogs
	m_pCurDialog->Show(false);
	m_pCurDialog = pDlg;
	m_pCurDialog->Show(true);

	//Set move camera var
	m_fMoveZDist = 0;
	m_fMoveXDist = 0;
	m_fMoveYDist = 0;
}


bool CECCustomizeMgr::InitDefaultData(void)
{
	CECFace* pCurFace = m_pCurPlayer->GetFaceModel();

	CECFace::FACE_CUSTOMIZEDATA* pFaceData = pCurFace->GetFaceData();
	
	//脸盘融合
	

	//3庭3
	m_nDefaultScaleUp = MakeSureSlider(pFaceData->scaleUp, m_pSliderBound->m_nScaleUpPartMax, m_pSliderBound->m_nScaleUpPartMin);
	m_nDefaultScaleMiddle = MakeSureSlider(pFaceData->scaleMiddle, m_pSliderBound->m_nScaleMiddlePartMax, m_pSliderBound->m_nScaleMiddlePartMin);
	m_nDefaultScaleDown = MakeSureSlider(pFaceData->scaleDown, m_pSliderBound->m_nScaleDownPartMax, m_pSliderBound->m_nScaleDownPartMin);

	//脸盘缩放2
	m_nDefaultScaleFaceH = MakeSureSlider(pFaceData->scaleFaceH, m_pSliderBound->m_nScaleFaceHMax, m_pSliderBound->m_nScaleFaceHMin);
	m_nDefaultScaleFaceV = MakeSureSlider(pFaceData->scaleFaceV, m_pSliderBound->m_nScaleFaceVMax, m_pSliderBound->m_nScaleFaceVMin);

	//眼睛14
	m_nDefaultScaleEyeH = MakeSureSlider(pFaceData->scaleEyeH, m_pSliderBound->m_nScaleEyeHMax, m_pSliderBound->m_nScaleEyeHMin);
	m_nDefaultScaleEyeV = MakeSureSlider(pFaceData->scaleEyeV, m_pSliderBound->m_nScaleEyeVMax, m_pSliderBound->m_nScaleEyeVMin);
	m_nDefaultRotateEye = MakeSureSlider(pFaceData->rotateEye, m_pSliderBound->m_nRotateEyeMax, m_pSliderBound->m_nRotateEyeMin);
	m_nDefaultOffsetEyeH = MakeSureSlider(pFaceData->offsetEyeH, m_pSliderBound->m_nOffsetEyeHMax, m_pSliderBound->m_nOffsetEyeHMin);
	m_nDefaultOffsetEyeV = MakeSureSlider(pFaceData->offsetEyeV, m_pSliderBound->m_nOffsetEyeVMax, m_pSliderBound->m_nOffsetEyeVMin);
	m_nDefaultOffsetEyeZ = MakeSureSlider(pFaceData->offsetEyeZ, m_pSliderBound->m_nOffsetEyeZMax, m_pSliderBound->m_nOffsetEyeZMin);
	m_nDefaultScaleEyeBall = MakeSureSlider(pFaceData->scaleEyeBall, m_pSliderBound->m_nScaleEyeBallMax, m_pSliderBound->m_nScaleEyeBallMin);

	m_nDefaultScaleEyeH2 = MakeSureSlider(pFaceData->scaleEyeH2, m_pSliderBound->m_nScaleEyeHMax, m_pSliderBound->m_nScaleEyeHMin);
	m_nDefaultScaleEyeV2 = MakeSureSlider(pFaceData->scaleEyeV2, m_pSliderBound->m_nScaleEyeVMax, m_pSliderBound->m_nScaleEyeVMin);
	m_nDefaultRotateEye2 = MakeSureSlider(pFaceData->rotateEye2, m_pSliderBound->m_nRotateEyeMax, m_pSliderBound->m_nRotateEyeMin);
	m_nDefaultOffsetEyeH2 = MakeSureSlider(pFaceData->offsetEyeH2, m_pSliderBound->m_nOffsetEyeHMax, m_pSliderBound->m_nOffsetEyeHMin);
	m_nDefaultOffsetEyeV2 = MakeSureSlider(pFaceData->offsetEyeV2, m_pSliderBound->m_nOffsetEyeVMax, m_pSliderBound->m_nOffsetEyeVMin);
	m_nDefaultOffsetEyeZ2 = MakeSureSlider(pFaceData->offsetEyeZ2, m_pSliderBound->m_nOffsetEyeZMax, m_pSliderBound->m_nOffsetEyeZMin);
	m_nDefaultScaleEyeBall2 = MakeSureSlider(pFaceData->scaleEyeBall2, m_pSliderBound->m_nScaleEyeBallMax, m_pSliderBound->m_nScaleEyeBallMin);


	//眉毛6
	m_nDefaultScaleBrowH = MakeSureSlider(pFaceData->scaleBrowH, m_pSliderBound->m_nScaleBrowHMax, m_pSliderBound->m_nScaleBrowHMin);
	m_nDefaultScaleBrowV = MakeSureSlider(pFaceData->scaleBrowV, m_pSliderBound->m_nScaleBrowVMax, m_pSliderBound->m_nScaleBrowVMin);
	m_nDefaultRotateBrow = MakeSureSlider(pFaceData->rotateBrow, m_pSliderBound->m_nRotateBrowMax, m_pSliderBound->m_nRotateBrowMin);
	m_nDefaultOffsetBrowH = MakeSureSlider(pFaceData->offsetBrowH, m_pSliderBound->m_nOffsetBrowHMax, m_pSliderBound->m_nOffsetBrowHMin);
	m_nDefaultOffsetBrowV = MakeSureSlider(pFaceData->offsetBrowV, m_pSliderBound->m_nOffsetBrowVMax, m_pSliderBound->m_nOffsetBrowVMin);
	m_nDefaultOffsetBrowZ = MakeSureSlider(pFaceData->offsetBrowZ, m_pSliderBound->m_nOffsetBrowZMax, m_pSliderBound->m_nOffsetBrowZMin);
	

	m_nDefaultScaleBrowH2 = MakeSureSlider(pFaceData->scaleBrowH2, m_pSliderBound->m_nScaleBrowHMax, m_pSliderBound->m_nScaleBrowHMin);
	m_nDefaultScaleBrowV2 = MakeSureSlider(pFaceData->scaleBrowV2, m_pSliderBound->m_nScaleBrowVMax, m_pSliderBound->m_nScaleBrowVMin);
	m_nDefaultRotateBrow2 = MakeSureSlider(pFaceData->rotateBrow2, m_pSliderBound->m_nRotateBrowMax, m_pSliderBound->m_nRotateBrowMin);
	m_nDefaultOffsetBrowH2 = MakeSureSlider(pFaceData->offsetBrowH2, m_pSliderBound->m_nOffsetBrowHMax, m_pSliderBound->m_nOffsetBrowHMin);
	m_nDefaultOffsetBrowV2 = MakeSureSlider(pFaceData->offsetBrowV2, m_pSliderBound->m_nOffsetBrowVMax, m_pSliderBound->m_nOffsetBrowVMin);
	m_nDefaultOffsetBrowZ2 = MakeSureSlider(pFaceData->offsetBrowZ2, m_pSliderBound->m_nOffsetBrowZMax, m_pSliderBound->m_nOffsetBrowZMin);

	//鼻子5
	m_nDefaultScaleNoseTipH = MakeSureSlider(pFaceData->scaleNoseTipH, m_pSliderBound->m_nScaleNoseTipHMax, m_pSliderBound->m_nScaleNoseTipHMin);
	m_nDefaultScaleNoseTipV = MakeSureSlider(pFaceData->scaleNoseTipV, m_pSliderBound->m_nScaleNoseTipVMax, m_pSliderBound->m_nScaleNoseTipVMin);
	m_nDefaultScaleNoseTipZ = MakeSureSlider(pFaceData->scaleNoseTipZ, m_pSliderBound->m_nScaleNoseTipZMax, m_pSliderBound->m_nScaleNoseTipZMin);
	m_nDefaultOffsetNoseTipV = MakeSureSlider(pFaceData->offsetNoseTipV, m_pSliderBound->m_nOffsetNoseTipVMax, m_pSliderBound->m_nOffsetNoseTipVMin);
	m_nDefaultScaleBridgeTipH = MakeSureSlider(pFaceData->scaleBridgeTipH, m_pSliderBound->m_nScaleBridgeTipHMax, m_pSliderBound->m_nScaleBridgeTipHMin);
	m_nDefaultOffsetBridegTipZ = MakeSureSlider(pFaceData->offsetBridgeTipZ, m_pSliderBound->m_nOffsetBridgeTipZMax, m_pSliderBound->m_nOffsetBridgeTipZMin);
	
	//嘴7
	m_nDefaultThickUpLip = MakeSureSlider(pFaceData->thickUpLip, m_pSliderBound->m_nThickUpLipMax, m_pSliderBound->m_nThickUpLipMin);
	m_nDefaultThickDownLip = MakeSureSlider(pFaceData->thickDownLip, m_pSliderBound->m_nThickDownLipMax, m_pSliderBound->m_nThickDownLipMin);
	m_nDefaultScaleMouthH = MakeSureSlider(pFaceData->scaleMouthH, m_pSliderBound->m_nScaleMouthHMax, m_pSliderBound->m_nScaleMouthHMin);
	m_nDefaultOffsetMouthV = MakeSureSlider(pFaceData->offsetMouthV, m_pSliderBound->m_nOffsetMouthVMax, m_pSliderBound->m_nOffsetMouthVMin);
	m_nDefaultOffsetMouthZ = MakeSureSlider(pFaceData->offsetMouthZ, m_pSliderBound->m_nOffsetMouthZMax, m_pSliderBound->m_nOffsetMouthZMin);
	m_nDefaultOffsetCornerOfMouthSpecial = MakeSureSlider(pFaceData->offsetCornerOfMouthSpecial, m_pSliderBound->m_nOffsetCornerOfMouthSpecialMax, m_pSliderBound->m_nOffsetCornerOfMouthSpecialMin);

	m_nDefaultScaleMouthH2 = MakeSureSlider(pFaceData->scaleMouthH2, m_pSliderBound->m_nScaleMouthH2Max, m_pSliderBound->m_nScaleMouthH2Min);
	m_nDefaultOffsetCornerOfMouthSpecial2 = MakeSureSlider(pFaceData->offsetCornerOfMouthSpecial2, m_pSliderBound->m_nOffsetCornerOfMouthSpecial2Max, m_pSliderBound->m_nOffsetCornerOfMouthSpecial2Min);

	//耳朵2
	m_nDefaultScaleEar = MakeSureSlider(pFaceData->scaleEar, m_pSliderBound->m_nScaleEarMax, m_pSliderBound->m_nScaleEarMin);
	m_nDefaultOffsetEarV = MakeSureSlider(pFaceData->offsetEarV, m_pSliderBound->m_nOffsetEarVMax, m_pSliderBound->m_nOffsetEarVMin);
	
	//额头5
	m_nDefaultOffsetForeheadH = MakeSureSlider(pFaceData->offsetForeheadH, m_pSliderBound->m_nOffsetForeheadHMax, m_pSliderBound->m_nOffsetForeheadHMin);
	m_nDefaultOffsetForeheadV = MakeSureSlider(pFaceData->offsetForeheadV, m_pSliderBound->m_nOffsetForeheadVMax, m_pSliderBound->m_nOffsetForeheadVMin);
	m_nDefaultOffsetForeheadZ = MakeSureSlider(pFaceData->offsetForeheadZ, m_pSliderBound->m_nOffsetForeheadZMax, m_pSliderBound->m_nOffsetForeheadZMin);
	m_nDefaultRotateForehead = MakeSureSlider(pFaceData->rotateForehead, m_pSliderBound->m_nRotateForeheadMax, m_pSliderBound->m_nRotateForeheadMin);
	m_nDefaultScaleForehead = MakeSureSlider(pFaceData->scaleForehead, m_pSliderBound->m_nScaleForeheadMax, m_pSliderBound->m_nScaleForeheadMin);

	//颧骨5
	m_nDefaultOffsetYokeBoneH = MakeSureSlider(pFaceData->offsetYokeBoneH, m_pSliderBound->m_nOffsetYokeBoneHMax, m_pSliderBound->m_nOffsetYokeBoneHMin);
	m_nDefaultOffsetYokeBoneV = MakeSureSlider(pFaceData->offsetYokeBoneV, m_pSliderBound->m_nOffsetYokeBoneVMax, m_pSliderBound->m_nOffsetYokeBoneVMin);
	m_nDefaultOffsetYokeBoneZ = MakeSureSlider(pFaceData->offsetYokeBoneZ, m_pSliderBound->m_nOffsetYokeBoneZMax, m_pSliderBound->m_nOffsetYokeBoneZMin);
	m_nDefaultRotateYokeBone = MakeSureSlider(pFaceData->rotateYokeBone, m_pSliderBound->m_nRotateYokeBoneMax, m_pSliderBound->m_nRotateYokeBoneMin);
	m_nDefaultScaleYokeBone = MakeSureSlider(pFaceData->scaleYokeBone, m_pSliderBound->m_nScaleYokeBoneMax, m_pSliderBound->m_nScaleYokeBoneMin);

	//脸颊4
	m_nDefaultOffsetCheekH = MakeSureSlider(pFaceData->offsetCheekH, m_pSliderBound->m_nOffsetCheekHMax, m_pSliderBound->m_nOffsetCheekHMin);
	m_nDefaultOffsetCheekV = MakeSureSlider(pFaceData->offsetCheekV, m_pSliderBound->m_nOffsetCheekVMax, m_pSliderBound->m_nOffsetCheekVMin);
	m_nDefaultOffsetCheekZ = MakeSureSlider(pFaceData->offsetCheekZ, m_pSliderBound->m_nOffsetCheekZMax, m_pSliderBound->m_nOffsetCheekZMin);
	m_nDefaultScaleCheek = MakeSureSlider(pFaceData->scaleCheek, m_pSliderBound->m_nScaleCheekMax, m_pSliderBound->m_nScaleCheekMin);

	//下巴4
	m_nDefaultOffsetChainV = MakeSureSlider(pFaceData->offsetChainV, m_pSliderBound->m_nOffsetChainVMax, m_pSliderBound->m_nOffsetChainVMin);
	m_nDefaultOffsetChainZ = MakeSureSlider(pFaceData->offsetChainZ, m_pSliderBound->m_nOffsetChainZMax, m_pSliderBound->m_nOffsetChainZMin);
	m_nDefaultRotateChain = MakeSureSlider(pFaceData->rotateChain, m_pSliderBound->m_nRotateBrowMax, m_pSliderBound->m_nRotateBrowMin);
	m_nDefaultScaleChainH = MakeSureSlider(pFaceData->scaleChainH, m_pSliderBound->m_nScaleChainHMax, m_pSliderBound->m_nScaleChainHMin);

	//颌骨6
	m_nDefaultOffsetJawH = MakeSureSlider(pFaceData->offsetJawH, m_pSliderBound->m_nOffsetJawHMax, m_pSliderBound->m_nOffsetJawHMin);
	m_nDefaultOffsetJawV = MakeSureSlider(pFaceData->offsetJawV, m_pSliderBound->m_nOffsetJawVMax, m_pSliderBound->m_nOffsetJawVMin);
	m_nDefaultOffsetJawZ = MakeSureSlider(pFaceData->offsetJawZ, m_pSliderBound->m_nOffsetJawZMax, m_pSliderBound->m_nOffsetJawZMin);
	m_nDefaultScaleJawSpecial = MakeSureSlider(pFaceData->scaleJawSpecial, m_pSliderBound->m_nScaleJawSpecialMax, m_pSliderBound->m_nScaleJawSpecialMin);
	m_nDefaultScaleJawH = MakeSureSlider(pFaceData->scaleJawH, m_pSliderBound->m_nScaleJawHMax, m_pSliderBound->m_nScaleJawHMin);
	m_nDefaultScaleJawV = MakeSureSlider(pFaceData->scaleJawV, m_pSliderBound->m_nScaleJawVMax, m_pSliderBound->m_nScaleJawVMin);
	

	CECPlayer::PLAYER_CUSTOMIZEDATA pCustomizeData = m_pCurPlayer->GetCustomizeData();

	//身材
	m_nDefaultHeadScale =MakeSureSlider(pCustomizeData.headScale, m_pSliderBound->m_nHeadScaleMax, m_pSliderBound->m_nHeadScaleMin);
	m_nDefaultUpScale = MakeSureSlider(pCustomizeData.upScale, m_pSliderBound->m_nUpScaleMax, m_pSliderBound->m_nUpScaleMin);
	m_nDefaultWaistScale = MakeSureSlider(pCustomizeData.waistScale, m_pSliderBound->m_nWaistScaleMax, m_pSliderBound->m_nWaistScaleMin);
	m_nDefaultArmWidth =MakeSureSlider(pCustomizeData.armWidth, m_pSliderBound->m_nArmWidthMax, m_pSliderBound->m_nArmWidthMin);
	m_nDefaultLegWidth = MakeSureSlider(pCustomizeData.legWidth, m_pSliderBound->m_nLegWidthMax, m_pSliderBound->m_nLegWidthMin);
	m_nDefaultBreastScale = MakeSureSlider(pCustomizeData.breastScale, m_pSliderBound->m_nBreastScaleMax, m_pSliderBound->m_nBreastScaleMin);

	return true;
}


int CECCustomizeMgr::MakeSureSlider(unsigned char &level, int nMax, int nMin)
{
	if( level < nMin)
	{
		level = nMin;
		
	}
	if( level > nMax)
	{
		level = nMax;
		
	}
	
	return level;
}

#define INIT_BUTTON_POINTER(variableName, dialogPointer, buttonName) \
{ \
	variableName = static_cast<AUIStillImageButton *>( \
		dialogPointer->GetDlgItem(buttonName)); \
	ASSERT(variableName); \
}

bool CECCustomizeMgr::InitSwithButtons()
{
	INIT_BUTTON_POINTER(m_pBtnChoosePreCustomize, m_pDlgCustomizePre, "Btm_ChoosePreCustomize");
	INIT_BUTTON_POINTER(m_pBtnChooseFace, m_pDlgCustomizeFace, "Btm_ChooseFace");
	INIT_BUTTON_POINTER(m_pBtnChooseEye, m_pDlgCustomizeEye, "Btm_ChooseEye");
	INIT_BUTTON_POINTER(m_pBtnChooseBrow, m_pDlgCustomizeEyebrow, "Btm_ChooseBrow");
	INIT_BUTTON_POINTER(m_pBtnChooseNose, m_pDlgCustomizeNose, "Btm_ChooseNose");
	INIT_BUTTON_POINTER(m_pBtnChooseMouth, m_pDlgCustomizeMouth, "Btm_ChooseMouth");
	INIT_BUTTON_POINTER(m_pBtnChooseEar, m_pDlgCustomizeEar, "Btm_ChooseEar");
	INIT_BUTTON_POINTER(m_pBtnChooseHair, m_pDlgCustomizeHair, "Btm_ChooseHair");
	INIT_BUTTON_POINTER(m_pBtnChooseFacePainting, m_pDlgCustomizePaint, "Btm_ChooseFacePainting");
	INIT_BUTTON_POINTER(m_pBtnChooseStature, m_pDlgCustomizeStature, "Btm_ChooseStature");
	
	
	m_pBtnChoosePreCustomize->SetPushLike(true);
	m_pBtnChooseFace ->SetPushLike(true);
	m_pBtnChooseEye->SetPushLike(true); 
	m_pBtnChooseBrow->SetPushLike(true); 
	m_pBtnChooseNose->SetPushLike(true); 
	m_pBtnChooseMouth->SetPushLike(true); 
	m_pBtnChooseEar->SetPushLike(true); 
	m_pBtnChooseHair->SetPushLike(true); 
	m_pBtnChooseFacePainting->SetPushLike(true); 
	m_pBtnChooseStature->SetPushLike(true); 


	m_pBtnCurChoose = m_pBtnChoosePreCustomize;
	m_pBtnCurChoose->SetPushed(true);

	return true;
}

bool CECCustomizeMgr::CalculateAdvaceOption(void)
{
	CECFace* pCurFace = m_pCurPlayer->GetFaceModel();

	pCurFace->CalculateForehead();//计算额头
	
	pCurFace->CalculateYokeBone();//计算颧骨
	
	pCurFace->CalculateCheek();//计算脸颊
	
	pCurFace->CalculateChain();//计算下巴
	
	pCurFace->CalculateJaw();//计算颌骨
	
	return true;
}

#define INIT_DIALOG_POINTER(variableName, className, dialogName) \
{ \
	variableName = static_cast<className *>(m_pAUIManager->GetDialog(dialogName)); \
	ASSERT(variableName); \
}

bool CECCustomizeMgr::InitDialogs()
{
	INIT_DIALOG_POINTER(m_pDlgCustomizeEar, CDlgCustomizeEar, "Win_ChooseEar");
	INIT_DIALOG_POINTER(m_pDlgCustomizeEye, CDlgCustomizeEye, "Win_ChooseEye");
	INIT_DIALOG_POINTER(m_pDlgCustomizeEyebrow, CDlgCustomizeEyebrow, "Win_ChooseEyebrow");
	INIT_DIALOG_POINTER(m_pDlgCustomizeFace, CDlgCustomizeFace, "Win_ChooseFace");
	INIT_DIALOG_POINTER(m_pDlgCustomizeHair, CDlgCustomizeHair, "Win_ChooseHair");
	INIT_DIALOG_POINTER(m_pDlgCustomizeMouth, CDlgCustomizeMouth, "Win_ChooseMouth");
	INIT_DIALOG_POINTER(m_pDlgCustomizeNose, CDlgCustomizeNose, "Win_ChooseNose");
	INIT_DIALOG_POINTER(m_pDlgCustomizePaint, CDlgCustomizePaint, "Win_ChooseFacePainting");
	INIT_DIALOG_POINTER(m_pDlgCustomizePre, CDlgCustomizePre, "Win_ChoosePreCustomize");
	INIT_DIALOG_POINTER(m_pDlgCustomizeStature, CDlgCustomizeStature, "Win_ChooseStature");
	INIT_DIALOG_POINTER(m_pDlgCustomizeAdv, CDlgCustomizeAdv, "Win_AdvancedOption");
	INIT_DIALOG_POINTER(m_pDlgCustomizeUserPic, CDlgCustomizeUserPic, "Win_UserPic");
	INIT_DIALOG_POINTER(m_pDlgCustomizeMoveCamera, CDlgCustomizeMoveCamera, "Win_MoveCamera");
	INIT_DIALOG_POINTER(m_pDlgCustomize, CDlgCustomize, "Win_Customize");

	bool bOK[14];
	
	bOK[0] = m_pDlgCustomizeEar->InitContext(this);
	bOK[1] = m_pDlgCustomizeEye->InitContext(this);
	bOK[2] = m_pDlgCustomizeEyebrow->InitContext(this);
	bOK[3] = m_pDlgCustomizeFace->InitContext(this);
	bOK[4] = m_pDlgCustomizeHair->InitContext(this);
	bOK[5] = m_pDlgCustomizeMouth->InitContext(this);
	bOK[6] = m_pDlgCustomizeNose->InitContext(this);
	bOK[7] = m_pDlgCustomizePaint->InitContext(this);
	bOK[8] = m_pDlgCustomizePre->InitContext(this);
	bOK[9] = m_pDlgCustomizeStature->InitContext(this);
	bOK[10] = m_pDlgCustomizeAdv->InitContext(this);
	bOK[11] = m_pDlgCustomizeUserPic->InitContext(this);
	bOK[12] = m_pDlgCustomizeMoveCamera->InitContext(this);
	bOK[13] = m_pDlgCustomize->InitContext(this);


	m_pCurDialog = m_pDlgCustomizePre;

	m_pDlgCustomizePre->Show(true);
	m_pDlgCustomizeMoveCamera->Show(true);
	m_pDlgCustomize->Show(true);
	
	bool bRet = true;

	for( int i = 0; i < 14; i++)
	{
		bRet = bRet && bOK[i];
	}
	
	return bRet;
}


//-------------------------------------------------------------------------------
void CECCustomizeMgr::HideAllOldDlg()
{
	int nNumDlg = m_pAUIManager->GetDialogCount();
	for( int i = 0; i < nNumDlg; i++)
	{
		AUIDialog* pDlg = m_pAUIManager->GetDialog(i);
		if( pDlg->IsShow())
		{
			m_vecOldDlg.push_back(pDlg);
			pDlg->Show(false);
		}
	}
}

//-------------------------------------------------------------------------------
void CECCustomizeMgr::ShowAllOldDlg()
{
	OldDlgVecIter_t iter = m_vecOldDlg.begin();
	for( ; iter != m_vecOldDlg.end(); iter++)
	{
		(*iter)->Show(true);
	}
	m_vecOldDlg.clear();
}

//------------------------------------------------------------------------------
void CECCustomizeMgr::ChangeScene(int nScene)
{
		A3DMATRIX4 matNPC = TransformMatrix(m_vNPCDir, A3DVECTOR3(0.0f, 1.0f, 0.0f), m_vNPCPos);
		A3DVECTOR3 vecPlayerPos = A3DVECTOR3(0.0f, -9.0f, 0.0f) * matNPC;
		A3DVECTOR3 vecCamPos;
		if( nScene == FACE_SCENE_STATURE)
		{
			vecCamPos = A3DVECTOR3(0.0f, -8.0f, 3.0f) * matNPC;
		}
		else if( nScene == FACE_SCENE_HEAD)
		{
			if( m_pCurPlayer->GetGender() == 0)//男
			{
				vecCamPos = (A3DVECTOR3(0.0f, -8.0f, 17.0f) + m_vPosDaltaM)* matNPC;
			}
			else//女
			{
				vecCamPos = (A3DVECTOR3(0.0f, -8.0f, 17.0f) + m_vPosDaltaF) * matNPC;
			}
		}

		A3DVECTOR3 vecCamToPlayer = a3d_VectorMatrix3x3(A3DVECTOR3(0.0f, 0.0f, -1.0f), matNPC);

		A3DCamera * pCamera = g_pGame->GetViewport()->GetA3DCamera();
		pCamera->SetPos(vecCamPos);
		pCamera->SetDirAndUp(vecCamToPlayer, A3DVECTOR3(0.0f, 1.0f, 0.0f));

		if( nScene == FACE_SCENE_STATURE)
		{
			pCamera->SetFOVSet(DEG2RAD(DEFCAMERA_FOV));
			pCamera->SetFOV(DEG2RAD(DEFCAMERA_FOV));
		}
		else if ( nScene == FACE_SCENE_HEAD)
		{
			pCamera->SetFOVSet(DEG2RAD(10.0f));
			pCamera->SetFOV(DEG2RAD(10.0f));
		}

		if(m_pCurPlayer && m_pCurPlayer->GetPlayerModel() )
		{
			m_pCurPlayer->GetPlayerModel()->SetPos(vecPlayerPos);
			m_pCurPlayer->GetPlayerModel()->SetDirAndUp(-vecCamToPlayer, A3DVECTOR3(0.0f, 1.0f, 0.0f));
		}
}
