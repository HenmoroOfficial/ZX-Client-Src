/*
 * FILE: EC_HomeDlgsMgr.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Liyi, 2005/12/2
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#ifndef _EC_HOME_DLGS_MGR_H_
#define _EC_HOME_DLGS_MGR_H_

#include "vector.h"
#include "AutoScene.h"

#include "AutoTexture.h"

inline void AutoHomeAssert() 
{
#ifdef _DEBUG
	_asm int 3
#endif
}
					

class AUIManager;
class AUIDialog;
class AUIStillImageButton;
class CAutoScene;

struct TextureParamInfo_t
{
	float	fTextureRatio;
	float	fMainTexture;
	float	fPatchTexture;
	float	fPatchDivision;
	float	fMainTextureU;
	float	fMainTextureV;
	float	fPatchTextureU;
	float	fPatchTextureV;
	DWORD	dwTextureSeed;
	AString	strMainTextureFile;
	AString	strPatchTextureFile;
	unsigned int nMainTexID;
	unsigned int nPatchTexID;
	int nMainTexIdx;
	int nPatchTexIdx;
};

enum ChooseTexInitParam_e
{
	CTIP_NULL,		//NULL
	CTIP_TERRAIN,
	CTIP_HILL,
	CTIP_LOCALTEX,
	CTIP_ROAD,
	CTIP_WATER,
};

class CECHomeDlgsMgr
{
	friend class CDlgAutoBuilding;
	friend class CDlgAutoLight;
	friend class CDlgAutoLocalTexture;
	friend class CDlgAutoMountain;
	friend class CDlgAutoPlant;
	friend class CDlgAutoRoad;
	friend class CDlgAutoTerrain;
	friend class CDlgAutoWater;
	friend class CDlgAutoChooseTexture;
	friend class CDlgHomeBase;
	friend class CDlgCurve;
	friend class CDlgAutoChoosePlant;
	friend class CDlgAutoChooseModel;
	friend class CDlgAutoChooseSky;
	friend class CDlgAutoChooseColor;
	friend class CDlgAutoChooseLight;
	

public:
	struct HomeDlgsMgrInitParams_t
	{
		AUIManager* pAUIMgr;

		HomeDlgsMgrInitParams_t(): pAUIMgr(0) {};

	};

public:
	CECHomeDlgsMgr();
	virtual ~CECHomeDlgsMgr();
	
	bool Init(HomeDlgsMgrInitParams_t params);

	void Show(bool bShow = true);

	bool LoadAutoSceneAction(const char* szActionFile);

	bool SetPersCamera();
	bool SetOrthCamera();

	inline CAutoScene* GetAutoScene() { return m_pAutoScene;}

	inline CDlgAutoBuilding* GetAutoBuildingDlg() { return m_pDlgAutoBuilding;}
	inline CDlgAutoLight* GetAutoLightDlg() { return m_pDlgAutoLight;}
	inline CDlgAutoLocalTexture* GetAutoLocalTextureDlg() { return m_pDlgAutoLocalTexture;}
	inline CDlgAutoMountain* GetAutoMountainDlg() { return m_pDlgAutoMountain;}
	inline CDlgAutoPlant* GetAutoPlantDlg() { return m_pDlgAutoPlant;}
	inline CDlgAutoRoad* GetAutoRoadDlg(){ return m_pDlgAutoRoad;}
	inline CDlgAutoTerrain* GetAutoTerrainDlg() { return m_pDlgAutoTerrain;}
	inline CDlgAutoWater* GetAutoWaterDlg(){ return m_pDlgAutoWater;}
	inline CDlgAutoChooseTexture* GetAutoChooseTextureDlg() { return m_pDlgAutoChooseTexture;}
	inline CDlgCurve* GetCurveDlg() { return m_pDlgCurve;}
	inline CDlgAutoChoosePlant* GetAutoChoosePlantDlg() { return m_pDlgAutoChoosePlant;}
	inline CDlgAutoChooseModel* GetAutoChooseModelDlg() { return m_pDlgAutoChooseModel;}
	inline CDlgAutoChooseSky* GetAutoChooseSkyDlg() { return m_pDlgAutoChooseSky;}
	inline CDlgAutoChooseColor* GetAutoChooseColorDlg() { return m_pDlgAutoChooseColor;}
	inline CDlgAutoChooseLight* GetAutoChooseLightDlg() { return m_pDlgAutoChooseLight;}
	
	inline AUIManager* GetAUIManager() { return m_pAUIMgr;}
	
	//Utility Function
	bool LoadTextureParamInfo(TextureParamInfo_t& param, TEXTURETYPE mainTexture, TEXTURETYPE patchTexture);
	bool SaveTextureParamInfo(TextureParamInfo_t& param, TEXTURETYPE mainTexture, TEXTURETYPE patchTexture);
	bool GetTextureIDAndIdx(const AString& strTexName, unsigned int& uTexID, int& uTexIdx);

protected:
	bool InitDialogs();
	void HideAllOldDlg();
	void ShowAllOldDlg();

	bool InitSwitchBtn();

    void SwitchDialog(const char * szCommand);

protected:

	CAutoScene* m_pAutoScene;

	abase::vector<AUIDialog*> m_vecOldDlg;
	typedef abase::vector<AUIDialog*>::iterator OldDlgVecIter_t;

	AUIManager * m_pAUIMgr;

	//-------------------------------------------------
	CDlgAutoBuilding* m_pDlgAutoBuilding;
	CDlgAutoLight* m_pDlgAutoLight;
	CDlgAutoLocalTexture* m_pDlgAutoLocalTexture;
	CDlgAutoMountain* m_pDlgAutoMountain;
	CDlgAutoPlant* m_pDlgAutoPlant;
	CDlgAutoRoad* m_pDlgAutoRoad;
	CDlgAutoTerrain* m_pDlgAutoTerrain;
	CDlgAutoWater* m_pDlgAutoWater;
	
	AUIDialog* m_pCurDlg;

	CDlgAutoChooseTexture* m_pDlgAutoChooseTexture;
	CDlgCurve* m_pDlgCurve;
	CDlgAutoChoosePlant* m_pDlgAutoChoosePlant;
	CDlgAutoChooseModel* m_pDlgAutoChooseModel;
	CDlgAutoChooseSky* m_pDlgAutoChooseSky;
	CDlgAutoChooseColor* m_pDlgAutoChooseColor;
	CDlgAutoChooseLight* m_pDlgAutoChooseLight;


	//------------------------------------------------
	AUIStillImageButton* m_pBtnChooseTerrain;
	AUIStillImageButton* m_pBtnChooseMountain;
	AUIStillImageButton* m_pBtnChooseWater;
	AUIStillImageButton* m_pBtnChooseRoad;
	AUIStillImageButton* m_pBtnChooseLocalTexture;
	AUIStillImageButton* m_pBtnChoosePlant;
	AUIStillImageButton* m_pBtnChooseBuilding;
	AUIStillImageButton* m_pBtnChooseLight;
	AUIStillImageButton* m_pBtnCurChoose;

	//----------------------------------------------
	
	bool m_bInitReady;

};

#endif