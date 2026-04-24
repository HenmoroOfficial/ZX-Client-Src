/*
 * FILE: EC_CustomizeMgr.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Liyi, 2005/8/18
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#ifndef _EC_CUSTOMIZE_MGR_H_
#define _EC_CUSTOMIZE_MGR_H_

#ifdef _WIN32
#pragma once
#endif


#include "A2DSprite.h"
#include "A2DSpriteItem.h"
#include "A3DFont.h"
#include "A3DSurfaceMan.h"
#include "A3DSurface.h"
#include "A3DCamera.h"
#include "A3DFuncs.h"

#include "AIniFile.h"


class AUIManager;
class CECPlayer;
class AUIDialog;
class CECCustomizeBound;
class CECFace;
class AUIStillImageButton;



#define FACE_TICKET_DEFAULT -1//缺省限制范围

#define FACE_SCENE_STATURE 0
#define FACE_SCENE_HEAD 1

class CECCustomizeMgr
{
	friend class CDlgCustomizeBase;
	friend class CDlgCustomizeEar;
	friend class CDlgCustomizeEye;
	friend class CDlgCustomizeEyebrow;
	friend class CDlgCustomizeFace;
	friend class CDlgCustomizeHair;
	friend class CDlgCustomizeMouth;
	friend class CDlgCustomizeNose;
	friend class CDlgCustomizePaint;
	friend class CDlgCustomizePre;
	friend class CDlgCustomizeStature;
	friend class CDlgCustomizeAdv;
	friend class CDlgCustomizeUserPic;
	friend class CDlgCustomizeMoveCamera;
	friend class CDlgCustomize;

public:
	

	struct CustomizeInitParam_t
	{
		AUIManager* pAUImanager;
		CECPlayer* pCurPlayer;
		bool bCallForCreate;
		A3DVECTOR3 vPos;
		A3DVECTOR3 vDir;
		int nFaceTicketID;
	};

	bool Init(CustomizeInitParam_t cusInitParam);

	CECCustomizeMgr();
	virtual ~CECCustomizeMgr();
	
	CECCustomizeBound* GetSliderBound(void){ return m_pSliderBound;}
	void ChangeScene(int nScene);
protected:
	bool InitDefaultData(void);
	bool InitSwithButtons();
	bool InitDialogs();
	
	bool CalculateAdvaceOption(void);
	int MakeSureSlider(unsigned char &level, int nMax, int nMin);
	void SwitchDialog(const char * szCommand);

	void HideAllOldDlg();
	void ShowAllOldDlg();
	
protected:
	abase::vector<AUIDialog*> m_vecOldDlg;
	typedef abase::vector<AUIDialog*>::iterator OldDlgVecIter_t;
	
	CECCustomizeBound *m_pSliderBound;
	AUIManager * m_pAUIManager;
	CECPlayer * m_pCurPlayer;
	bool m_bCallForCreate;
	A3DVECTOR3 m_vNPCPos;
	A3DVECTOR3 m_vNPCDir;
	A3DVECTOR3 m_vPosDaltaM;
	A3DVECTOR3 m_vPosDaltaF;

	bool m_bUnsymmetrical;//非对称
	bool m_bLoadFacePillData;//是否载入变形丸相关的一些特殊数据

	// Dialogs
	CDlgCustomizeEar * m_pDlgCustomizeEar;
	CDlgCustomizeEye * m_pDlgCustomizeEye;
	CDlgCustomizeEyebrow * m_pDlgCustomizeEyebrow;
	CDlgCustomizeFace * m_pDlgCustomizeFace;
	CDlgCustomizeHair * m_pDlgCustomizeHair;
	CDlgCustomizeMouth * m_pDlgCustomizeMouth;
	CDlgCustomizeNose * m_pDlgCustomizeNose;
	CDlgCustomizePaint * m_pDlgCustomizePaint;
	CDlgCustomizePre * m_pDlgCustomizePre;
	CDlgCustomizeStature * m_pDlgCustomizeStature;
	CDlgCustomizeAdv* m_pDlgCustomizeAdv;
	CDlgCustomizeBase * m_pCurDialog; // current dialog
	
	CDlgCustomizeUserPic* m_pDlgCustomizeUserPic;
	CDlgCustomizeMoveCamera* m_pDlgCustomizeMoveCamera;
	CDlgCustomize* m_pDlgCustomize;

	//----------------设置按钮控件---------------------
	AUIStillImageButton* m_pBtnChoosePreCustomize;
	AUIStillImageButton* m_pBtnChooseFace;
	AUIStillImageButton* m_pBtnChooseEye;
	AUIStillImageButton* m_pBtnChooseBrow;
	AUIStillImageButton* m_pBtnChooseNose;
	AUIStillImageButton* m_pBtnChooseMouth;
	AUIStillImageButton* m_pBtnChooseEar;
	AUIStillImageButton* m_pBtnChooseHair;
	AUIStillImageButton* m_pBtnChooseFacePainting;
	AUIStillImageButton* m_pBtnChooseStature;

	AUIStillImageButton* m_pBtnCurChoose;//当前按钮
	
	//------------定义所有与自定义相关的滚动条初始位置

	//3庭3
	int m_nDefaultScaleUp;
	int m_nDefaultScaleMiddle;
	int m_nDefaultScaleDown ;

	//脸盘缩放2
	int m_nDefaultScaleFaceH;
	int m_nDefaultScaleFaceV;
	

	//眼睛14
	int m_nDefaultScaleEyeH;
	int m_nDefaultScaleEyeV;
	int m_nDefaultRotateEye;
	int m_nDefaultOffsetEyeH;
	int m_nDefaultOffsetEyeV;
	int m_nDefaultOffsetEyeZ;
	int m_nDefaultScaleEyeBall;	

	int m_nDefaultScaleEyeH2;
	int m_nDefaultScaleEyeV2;
	int m_nDefaultRotateEye2;
	int m_nDefaultOffsetEyeH2;
	int m_nDefaultOffsetEyeV2;
	int m_nDefaultOffsetEyeZ2;
	int m_nDefaultScaleEyeBall2;	


	//眉毛6
	int m_nDefaultScaleBrowH;
	int m_nDefaultScaleBrowV;
	int m_nDefaultRotateBrow;
	int m_nDefaultOffsetBrowH;
	int m_nDefaultOffsetBrowV;
	int m_nDefaultOffsetBrowZ;

	int m_nDefaultScaleBrowH2;
	int m_nDefaultScaleBrowV2;
	int m_nDefaultRotateBrow2;
	int m_nDefaultOffsetBrowH2;
	int m_nDefaultOffsetBrowV2;
	int m_nDefaultOffsetBrowZ2;
		
	//鼻子5
	int m_nDefaultScaleNoseTipH;
	int m_nDefaultScaleNoseTipV;
	int m_nDefaultScaleNoseTipZ;
	int m_nDefaultOffsetNoseTipV;
	int m_nDefaultScaleBridgeTipH;
	int m_nDefaultOffsetBridegTipZ;
	
	
	//嘴6
	int m_nDefaultThickUpLip;
	int m_nDefaultThickDownLip;
	int m_nDefaultScaleMouthH;
	int m_nDefaultOffsetMouthV ;
	int m_nDefaultOffsetMouthZ ;
	int m_nDefaultOffsetCornerOfMouthSpecial;
	int m_nDefaultScaleMouthH2;
	int m_nDefaultOffsetCornerOfMouthSpecial2;

	//耳朵2
	int m_nDefaultScaleEar;
	int m_nDefaultOffsetEarV;
	
	//额头5
	int m_nDefaultOffsetForeheadH;
	int m_nDefaultOffsetForeheadV;
	int m_nDefaultOffsetForeheadZ;
	int m_nDefaultRotateForehead;
	int m_nDefaultScaleForehead;

	//颧骨5
	int m_nDefaultOffsetYokeBoneH ;
	int m_nDefaultOffsetYokeBoneV;
	int m_nDefaultOffsetYokeBoneZ ;
	int m_nDefaultRotateYokeBone;
	int m_nDefaultScaleYokeBone;

	//脸颊4
	int m_nDefaultOffsetCheekH ;
	int m_nDefaultOffsetCheekV;
	int m_nDefaultOffsetCheekZ;
	int m_nDefaultScaleCheek ;


	//下巴4
	int m_nDefaultOffsetChainV;
	int m_nDefaultOffsetChainZ;
	int m_nDefaultRotateChain;
	int m_nDefaultScaleChainH;


	//颌骨6
	int m_nDefaultOffsetJawH;
	int m_nDefaultOffsetJawV;
	int m_nDefaultOffsetJawZ;
	int m_nDefaultScaleJawSpecial;
	int m_nDefaultScaleJawH;
	int m_nDefaultScaleJawV;

	//身材
	int m_nDefaultHeadScale;
	int m_nDefaultUpScale;
	int m_nDefaultWaistScale;
	int m_nDefaultArmWidth;
	int m_nDefaultLegWidth;
	int m_nDefaultBreastScale;

	//MoveCamera 对话框 用于设置摄像机移动
	float m_fMoveZDist;
	float m_fMoveXDist;
	float m_fMoveYDist;

};


#endif