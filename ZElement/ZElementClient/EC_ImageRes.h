/*
 * FILE: EC_ImageRes.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/3/2
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "A3DTypes.h"
#include "AArray.h"
#ifndef BACK_VERSION
#include "ECRObjBase.h"
#endif

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////

class A2DSprite;
class A2DSpriteBuffer;

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECImageRes
//	
///////////////////////////////////////////////////////////////////////////
#ifdef BACK_VERSION
class CECImageRes
#else
class CECImageRes : public ECRObjBase
#endif
{
public:		//	Types

	//	Image index
	enum
	{
		IMG_POPUPNUM = 0,
		IMG_HITMISSED,
		IMG_FACTION,
		IMG_PATEQUEST,
		IMG_LEVELUP,
		IMG_DEITY_LEVELUP,
		IMG_GOTEXP,
		IMG_GOTDEITYEXP,
		IMG_GOTMONEY,
		IMG_DEADLYSTRIKE,
		IMG_DT_DEADLYSTRIKE,
		IMG_GOTSP,
		IMG_GOTREPUTATION,
		IMG_GOTREGIONREPU,
		IMG_INVALIDHIT,
		IMG_TEAMLEADER,
		IMG_BOOTHBAR,
		IMG_HPWARN,
		IMG_MPWARN,
		IMG_DPWARN,
		IMG_RETORT,
		IMG_IMMUNE,
		IMG_TEAMMATE,
		IMG_PKSTATE,
		IMG_FACTIONFLAG,
		IMG_REPUTATIONICONS,
		IMG_GMFLAG,
		IMG_SKILL_SPEC0,
		IMG_SKILL_SPEC1,
		IMG_SKILL_SPEC2,
		IMG_SKILL_SPEC3,
		IMG_SKILL_SPEC4,
		IMG_SKILL_SPEC5,
		IMG_RESIST,
		IMG_FAMILY_RECORD_1,
		IMG_FAMILY_RECORD_2,
		IMG_FAMILY_RECORD_3,
		IMG_FAMILY_RECORD_4,
		IMG_FAMILY_RECORD_5,
		IMG_FAMILY_RECORD_6,
		IMG_FAMILY_RECORD_7,
		IMG_FAMILY_RECORD_8,
		IMG_FAMILY_RECORD_9,
		IMG_FACTION_NIMBUS_1,
		IMG_FACTION_NIMBUS_2,
		IMG_FACTION_NIMBUS_3,
		IMG_FACTION_NIMBUS_4,
		IMG_FACTION_NIMBUS_5,
		IMG_FACTION_NIMBUS_6,
		IMG_VIP,
		IMG_KINGDOM_KING,
		IMG_IMMUNE_DAMAGE,
		NUM_IMAGE,
	};

	//	Images
	struct IMAGE
	{
		A2DSprite*	pImage;		//	Image
		int			iNumItem;	//	Number of image item
		int			iCurItem;	//	Current item
		int			iWidth;		//	Image width
		int			iHeight;	//	Image height

		A2DSpriteBuffer*	pImageBuf;	//	Image buffer
	};
	
	//	Registered draw
	struct REGDRAW
	{
		DWORD	dwSlot;
		int		x;
		int		y;
		DWORD	dwCol;
		int		iItem;
		float	z;
		float	rhw;
		int		iDstWid;
		int		iDstHei;
	};

public:		//	Constructor and Destructor
#ifdef BACK_VERSION
	CECImageRes();
#else
	CECImageRes(A3DDevice* pA3DDevice);
#endif
	virtual ~CECImageRes();

public:		//	Attributes

public:		//	Operations

	//	Load all images
	bool LoadAllImages();
	//	Release all images
	void Release();

	//	Get image item size
	void GetImageItemSize(DWORD dwSlot, int iItem, int* piWid, int* piHei);
	//	Draw image item to backbuffer
	void DrawImage(DWORD dwSlot, int x, int y, A3DCOLOR col, int iItem=0, float z=0.0f, float rhw=1.0f, int iDstWid=0, int iDstHei=0);
	//	Draw image item to backbuffer without scale
	void DrawImageNoScale(DWORD dwSlot, int x, int y, A3DCOLOR col, int iItem=0, float z=0.0f, float rhw=1.0f);
	//	Flush all rendered images
	void Flush();

	//	Register draw
	void RegisterDraw(DWORD dwSlot, int x, int y, A3DCOLOR col, int iItem=0, float z=0.0f, float rhw=1.0f, int iDstWid=0, int iDstHei=0);
	//	Present all registered draws
	void PresentDraws();

#ifndef BACK_VERSION
	virtual bool Render(const ECRENDERPARAM* pRenderParam);
#endif

protected:	//	Attributes

#ifndef BACK_VERSION
	A3DDevice*	m_pA3DDevice;
#endif
	IMAGE	m_aImages[NUM_IMAGE];	//	Image array

	APtrArray<REGDRAW*>		m_aRegDraws;	//	Registered draw

protected:	//	Operations

	//	Release speicfied image
	void ReleaseImage(DWORD dwSlot);

	//	Load image from file
	bool LoadImage(DWORD dwSlot, char* szFile, int iWidth, int iHeight, int iNumItem, A3DRECT* aRects, int iNumElem=0);
	//	Load image which has only one item
	bool LoadImage(DWORD dwSlot, char* szFile, int iWidth, int iHeight, int iNumElem=0);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////



