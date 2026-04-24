/********************************************************************
	created:	2013/09/05
	created:	5:9:2013   15:08
	filename: 	e:\Works\ZElementClient\EC_LandBuild.h
	file path:	e:\Works\ZElementClient
	file base:	EC_LandBuild
	file ext:	h
	author:		zhuyan
	
	purpose:	帮派基地建筑, 虽放在NPC这个目录 但实际上不是NPC
*********************************************************************/
#pragma once

#include "EC_MsgDef.h"
#include "EC_StringTab.h"
#include "EC_Object.h"
#include "EC_RoleTypes.h"
#include "EC_Counter.h"
#include "EC_CDS.h"
#include "A3DGeometry.h"
#include "AAssist.h"
#include "CDWithCH.h"
#include "aabbcd.h"

struct BUILDING_ESSENCE;

class A3DViewport;
#ifdef BACK_VERSION
class CECViewport;
#else
class ECViewport;
#define CECViewport ECViewport
#endif
class CECModel;
class CECSkill;
class CECPateText;
class CECBubbleDecalList;
class A3DGFXEx;
class CELRegionSet;
class CECBrushMan;
class A3DSkin;
using namespace CHBasedCD;
class EC_LandBuild : public CECObject
{
public:	
public:
	EC_LandBuild();
	virtual ~EC_LandBuild();

	void LoadGuildBuildingObject(int nGuiBuildTempID,int nStatus,int nLevel); 

	void LandBuildComplete(int nGuiBuildTempID,int nLevel,int nStatus);
	void SetModelShow(bool isShow);
	A3DVECTOR3 GetModelPos();
	virtual bool Render(CECViewport* pViewport, int iRenderFlag=0);
	virtual void Release();
	virtual bool Tick(DWORD dwDeltaTime);
	const BUILDING_ESSENCE*	GetDBEssence()					{ return m_pDBEssence; }
	int GetFieldIndex() const { return m_nFieldIndex; }
	void SetFieldIndex(int val);
	bool TraceWithBrush(BrushTraceInfo * pInfo);
protected:	//	Attributes
	int m_nFieldIndex;
	CECModel	*m_pModel;
	A3DSkin* m_pSkin;
	BUILDING_ESSENCE*	m_pDBEssence;
	int			nBrushes;	//	number of brush object used in collision
	CCDBrush**	ppBrushes;	 //	Brush object used in collision
	A3DVECTOR3	vInitPos;
	A3DVECTOR3	vInitDir;
	A3DVECTOR3	vInitUp;
};