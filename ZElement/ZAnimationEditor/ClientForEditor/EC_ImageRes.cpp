/*
 * FILE: EC_ImageRes.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/3/2
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_ImageRes.h"

#include "A3DTypes.h"
#include "A2DSprite.h"
#include "A2DSpriteItem.h"
#include "A3DEngine.h"
#include "A2DSpriteBuffer.h"

#ifndef BACK_VERSION
#include "A3DDevice.h"
#include "EC_World.h"
#include "EC_GameRun.h"
#include "EC_ManDecal.h"
#endif

#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Reference to External variables and functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Local Types and Variables and Global variables
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECImageRes
//	
///////////////////////////////////////////////////////////////////////////

#ifdef BACK_VERSION
CECImageRes::CECImageRes()
#else
CECImageRes::CECImageRes(A3DDevice* pA3DDevice)
: m_pA3DDevice(pA3DDevice)
#endif
{
	memset(m_aImages, 0, sizeof (m_aImages));
}

CECImageRes::~CECImageRes()
{
}

//	Load all images
bool CECImageRes::LoadAllImages()
{
	int i;
	A3DRECT aRects[64];

	//	Popup number
	for (i=0; i < 11; i++)
		aRects[i] = A3DRECT(i*20, 0, i*20+20, 32);

	LoadImage(IMG_POPUPNUM, "InGame\\头顶数字.tga", 220, 32, 11, aRects, 64);

	//	Temporary faction icons
	for (i=0; i < 5; i++)
		aRects[i] = A3DRECT(i*16, 0, i*16+16, 16);

	LoadImage(IMG_FACTION, "InGame\\Faction.bmp", 100, 16, 5, aRects, 30);
	
	//	Pate quest icons
	for (i=0; i < 6; i++)
		aRects[i] = A3DRECT(i*46, 0, i*46+46, 46);

	LoadImage(IMG_PATEQUEST, "InGame\\PateQuest.tga", 276, 46, 6, aRects, 16);

	//	Booth name's background
/*	for (i=0; i < 3; i++)
	{
		for (int j=0; j < 3; j++)
			aRects[i*3+j] = A3DRECT(j*5, i*5, j*5+5, i*5+5);
	}

	LoadImage(IMG_BOOTHBAR, "InGame\\boothbar.tga", 15, 15, 9, aRects, 128);
*/
	//	Other images ...
	LoadImage(IMG_HITMISSED, "InGame\\未命中.tga", 0, 0, 10);
	LoadImage(IMG_LEVELUP, "InGame\\升级了.tga", 0, 0);
	LoadImage(IMG_DEITY_LEVELUP, "InGame\\元神升级.tga", 0, 0);
	LoadImage(IMG_GOTEXP, "InGame\\经验.tga", 0, 0);
	LoadImage(IMG_GOTDEITYEXP, "InGame\\经验.tga", 0, 0);
	LoadImage(IMG_GOTMONEY, "InGame\\金钱.tga", 0, 0);
	LoadImage(IMG_DEADLYSTRIKE, "InGame\\爆击.tga", 0, 0);
	LoadImage(IMG_DT_DEADLYSTRIKE, "InGame\\元力暴击.tga", 0, 0);
	LoadImage(IMG_GOTSP, "InGame\\元神.tga", 0, 0);
	LoadImage(IMG_GOTREPUTATION, "InGame\\威望.tga", 0, 0);
	LoadImage(IMG_GOTREGIONREPU, "InGame\\友好度.tga", 0, 0);
	LoadImage(IMG_INVALIDHIT, "InGame\\无效.tga", 0, 0);
	LoadImage(IMG_TEAMLEADER, "Window\\LeaderMark.tga", 0, 0);
	LoadImage(IMG_HPWARN, "InGame\\hp_warn.tga", 0, 0);
	LoadImage(IMG_MPWARN, "InGame\\mp_warn.tga", 0, 0);
	LoadImage(IMG_DPWARN, "InGame\\神力值过低.tga", 0, 0);
	LoadImage(IMG_RETORT, "InGame\\反震.tga", 0, 0);
	LoadImage(IMG_IMMUNE, "InGame\\免疫.tga", 0, 0);
	LoadImage(IMG_TEAMMATE, "Window\\TeammateMark.tga", 0, 0);
	LoadImage(IMG_PKSTATE, "InGame\\PK状态标记.tga", 0, 0);
	LoadImage(IMG_FACTIONFLAG, "InGame\\帮派标.tga", 0, 0);
	LoadImage(IMG_GMFLAG, "InGame\\GM标志.dds", 0, 0);
	LoadImage(IMG_SKILL_SPEC0, "InGame\\技能特色文字1.dds", 0, 0);
	LoadImage(IMG_SKILL_SPEC1, "InGame\\技能特色文字2.dds", 0, 0);
	LoadImage(IMG_SKILL_SPEC2, "InGame\\技能特色文字3.dds", 0, 0);
	LoadImage(IMG_SKILL_SPEC3, "InGame\\技能特色文字4.dds", 0, 0);
	LoadImage(IMG_SKILL_SPEC4, "InGame\\技能特色文字5.dds", 0, 0);
	LoadImage(IMG_SKILL_SPEC5, "InGame\\技能特色文字6.dds", 0, 0);
	LoadImage(IMG_RESIST, "InGame\\抵抗.dds", 0, 0);
	A3DRECT rects[10];
	for(i=0; i<10; i++)
	{
		rects[i].left = i * 18;
		rects[i].top = 0;
		rects[i].right = rects[i].left + 18;
		rects[i].bottom = rects[i].top + 18;
	}
	LoadImage(IMG_REPUTATIONICONS, "InGame\\威望图标.tga", 180, 18, 10, rects, 64);

	LoadImage(IMG_FAMILY_RECORD_1, "InGame\\一星.tga", 0, 0);
	LoadImage(IMG_FAMILY_RECORD_2, "InGame\\二星.tga", 0, 0);
	LoadImage(IMG_FAMILY_RECORD_3, "InGame\\三星.tga", 0, 0);
	LoadImage(IMG_FAMILY_RECORD_4, "InGame\\四星.tga", 0, 0);
	LoadImage(IMG_FAMILY_RECORD_5, "InGame\\五星.tga", 0, 0);
	LoadImage(IMG_FAMILY_RECORD_6, "InGame\\六星.tga", 0, 0);
	LoadImage(IMG_FAMILY_RECORD_7, "InGame\\七星.tga", 0, 0);
	LoadImage(IMG_FAMILY_RECORD_8, "InGame\\八星.tga", 0, 0);
	LoadImage(IMG_FAMILY_RECORD_9, "InGame\\九星.tga", 0, 0);
	
	LoadImage(IMG_FACTION_NIMBUS_1, "InGame\\1星.tga", 0, 0);
	LoadImage(IMG_FACTION_NIMBUS_2, "InGame\\2星.tga", 0, 0);
	LoadImage(IMG_FACTION_NIMBUS_3, "InGame\\3星.tga", 0, 0);
	LoadImage(IMG_FACTION_NIMBUS_4, "InGame\\4星.tga", 0, 0);
	LoadImage(IMG_FACTION_NIMBUS_5, "InGame\\5星.tga", 0, 0);

	for (i=0; i < 6; i++)
		aRects[i] = A3DRECT(0, i*20, 20, i*20+20);
	
	LoadImage(IMG_VIP, "icon\\VipSymbol.tga", 20, 120, 6, aRects, 6);

	LoadImage(IMG_KINGDOM_KING, "InGame\\King20.tga", 0, 0);

	return true;
}

/*	Load image which has only one item

	Return true for success, otherwise return false.

	dwSlot: slot will be used by the image
	szFile: image file name
	iWidth, iHeight: iamge size
	iNumElem: element number used to initialize image buffer
*/
bool CECImageRes::LoadImage(DWORD dwSlot, char* szFile, int iWidth, int iHeight, int iNumElem/* 0 */)
{
	A3DRECT Rect(0, 0, iWidth, iHeight);
	if (!LoadImage(dwSlot, szFile, iWidth, iHeight, 1, &Rect, iNumElem))
		return false;

	if (!iWidth || !iHeight)
	{
		IMAGE* pSlot = &m_aImages[dwSlot];
		if (pSlot->pImage)
		{
			A3DRECT rect(0, 0, pSlot->iWidth, pSlot->iHeight);
			pSlot->pImage->ResetItems(1, &rect);
		}
	}

	return true;
}

/*	Load image from file

	Return true for success, otherwise return false.

	dwSlot: slot will be used by the image
	szFile: image file name
	iWidth, iHeight: iamge size
	iNumItem: number of image item in this image
	aRects: rectanges for image item
*/
bool CECImageRes::LoadImage(DWORD dwSlot, char* szFile, int iWidth, int iHeight,
							int iNumItem, A3DRECT* aRects, int iNumElem/* 0 */)
{
	A3DDevice* pA3DDevice = g_pGame->GetA3DDevice();

	IMAGE* pSlot = &m_aImages[dwSlot];
	if (pSlot->pImage)
	{
		a_LogOutput(1, "CECImageRes::LoadImage, Slot %d has been used by other image", dwSlot);
		return false;
	}

	A2DSprite* pImage = new A2DSprite;
	if (!pImage)
	{
		a_LogOutput(1, "CECImageRes::LoadImage, Failed to create A2DSprite object");
		return false;
	}

	if (!pImage->Init(pA3DDevice, szFile, iWidth, iHeight, 0, iNumItem, aRects))
	{
		delete pImage;
		a_LogOutput(1, "CECImageRes::LoadImage, Failed to initialize A2DSprite object from file %s", szFile);
		return false;
	}

	pImage->SetNeedStereo(true);
	pSlot->pImage		= pImage;
	pSlot->iNumItem		= iNumItem;
	pSlot->iWidth		= pImage->GetWidth();
	pSlot->iHeight		= pImage->GetHeight();
	pSlot->iCurItem		= -1;
	pSlot->pImageBuf	= NULL;

	if (iNumElem)
	{
		if (!(pSlot->pImageBuf = new A2DSpriteBuffer))
		{
			a_LogOutput(1, "CECImageRes::LoadImage, Failed to new A2DSpriteBuffer");
		}
		else if (!pSlot->pImageBuf->Init(pA3DDevice, pSlot->pImage->GetTextures(), iNumElem))
		{
			delete pSlot->pImageBuf;
			pSlot->pImageBuf = NULL;
			a_LogOutput(1, "CECImageRes::LoadImage, Failed to initialize A2DSpriteBuffer");
		}
	}

	return true;
}

/*	Release speicfied image

	dwSlot: image's slot.
*/
void CECImageRes::ReleaseImage(DWORD dwSlot)
{
	assert(dwSlot < NUM_IMAGE);

	IMAGE* pImage = &m_aImages[dwSlot];

	if (pImage->pImage)
	{
		pImage->pImage->Release();
		delete pImage->pImage;
		pImage->pImage = NULL;
	}

	if (pImage->pImageBuf)
	{
		pImage->pImageBuf->Release();
		delete pImage->pImageBuf;
		pImage->pImageBuf = NULL;
	}

	pImage->iNumItem = 0;
	pImage->iCurItem = -1;
}

//	Release all images
void CECImageRes::Release()
{
	int i;
	for (i=0; i < NUM_IMAGE; i++)
	{
		IMAGE* pImage = &m_aImages[i];
		
		if (pImage->pImage)
		{
			pImage->pImage->Release();
			delete pImage->pImage;
		}

		if (pImage->pImageBuf)
		{
			pImage->pImageBuf->Release();
			delete pImage->pImageBuf;
		}
	}

	memset(m_aImages, 0, sizeof (m_aImages));

	int iNumDraw = m_aRegDraws.GetSize();
	for (i=0; i < iNumDraw; i++)
		delete m_aRegDraws[i];

	m_aRegDraws.RemoveAll();
}

/*	Draw image item to backbuffer considing scale factor

	dwSlot: image slot.
	x, y: image's local center position on screen. x, y will also are be effected
		  by scale factors
	col: image color
	iItem: iamge item index
	z: z value
	iDstWid, iDstHei: image destination size on screen
*/
void CECImageRes::DrawImage(DWORD dwSlot, int x, int y, A3DCOLOR col, int iItem/* 0 */, 
						float z/* 0.0f */, float rhw, int iDstWid/* 1.0f */, int iDstHei/* 1.0f */)
{
	assert(dwSlot < NUM_IMAGE);

	IMAGE* pSlot = &m_aImages[dwSlot];
	assert(iItem >= 0 && iItem < pSlot->iNumItem);

	A2DSprite* pImage = pSlot->pImage;

	if (iItem != pSlot->iCurItem)
	{
		pImage->SetCurrentItem(iItem);
		pSlot->iCurItem = iItem;
	}

	A3DRECT Rect = pImage->GetItem(iItem)->GetRect();
	float fScaleX = iDstWid ? (float)iDstWid / Rect.Width() : 1.0f;
	float fScaleY = iDstHei ? (float)iDstHei / Rect.Height() : 1.0f;

	pImage->SetScaleX(fScaleX);
	pImage->SetScaleY(fScaleY);
#ifdef BACK_VERSION
	pImage->SetZPos(z, rhw);
#else
	pImage->SetZPos(z);
#endif
	pImage->SetColor(col);

	
	if (pSlot->pImageBuf)
		pImage->DrawToBuffer(pSlot->pImageBuf, x, y);
	else
		pImage->DrawToBack(x, y);
}

/*	Draw image item to backbuffer without scale.

	dwSlot: image slot.
	x, y: image's local center position on screen. x, y will also are be effected
		  by scale factors
	col: image color
	iItem: iamge item index
	z: z value
*/
void CECImageRes::DrawImageNoScale(DWORD dwSlot, int x, int y, A3DCOLOR col, 
								int iItem/* 0 */, float z/* 0.0f */, float rhw)
{
	assert(dwSlot < NUM_IMAGE);

	IMAGE* pSlot = &m_aImages[dwSlot];
	assert(iItem >= 0 && iItem < pSlot->iNumItem);

	A2DSprite* pImage = pSlot->pImage;

	if (iItem != pSlot->iCurItem)
	{
		pImage->SetCurrentItem(iItem);
		pSlot->iCurItem = iItem;
	}

	pImage->SetScaleX(1.0f);
	pImage->SetScaleY(1.0f);
	pImage->SetZPos(z, rhw);
	pImage->SetColor(col);

	if (pSlot->pImageBuf)
		pImage->DrawToBuffer(pSlot->pImageBuf, x, y);
	else
		pImage->DrawToBack(x, y);
}

/*	Get image item size

	dwSlot: image slot.
	iItem: item index.
	piWid (out): used to receive image item's width
	piHei (out): used to receive image item's height.
*/
void CECImageRes::GetImageItemSize(DWORD dwSlot, int iItem, int* piWid, int* piHei)
{
	if (dwSlot >= NUM_IMAGE)
	{
		assert(0);
		return;
	}

	IMAGE* pImage = &m_aImages[dwSlot];

	assert(iItem >= 0 && iItem < pImage->iNumItem);

	A2DSpriteItem* pItem = pImage->pImage->GetItem(iItem);
	A3DRECT Rect = pItem->GetRect();

	if (piWid)
		*piWid = Rect.right - Rect.left;

	if (piHei)
		*piHei = Rect.bottom - Rect.top;
}

//	Flush all rendered images
void CECImageRes::Flush()
{
	//	Flush image buffer
	for (int i=0; i < NUM_IMAGE; i++)
	{
		IMAGE* pSlot = &m_aImages[i];
		if (pSlot->pImageBuf)
			pSlot->pImageBuf->FlushBuffer();
	}
}

//	Register draw
void CECImageRes::RegisterDraw(DWORD dwSlot, int x, int y, A3DCOLOR col, int iItem/* 0 */, 
						float z/* 0.0f */, float rhw, int iDstWid/* 0 */, int iDstHei/* 0 */)
{
	REGDRAW* pd = new REGDRAW;
	if (!pd)
		return;

	pd->dwSlot	= dwSlot;
	pd->x		= x;
	pd->y		= y;
	pd->z		= z;
	pd->rhw		= rhw;
	pd->dwCol	= col;
	pd->iItem	= iItem;
	pd->iDstWid	= iDstWid;
	pd->iDstHei	= iDstHei;

	m_aRegDraws.Add(pd);
}

//	Present all registered draws
void CECImageRes::PresentDraws()
{
	int iNumDraw = m_aRegDraws.GetSize();
	if (!iNumDraw)
		return;

	for (int i=0; i < iNumDraw; i++)
	{
		REGDRAW* pd = m_aRegDraws[i];
		DrawImage(pd->dwSlot, pd->x, pd->y, pd->dwCol, pd->iItem, pd->z, pd->rhw, pd->iDstWid, pd->iDstHei);
		delete pd;
	}
	
	m_aRegDraws.RemoveAll(false);

	Flush();
}
#ifndef BACK_VERSION
bool CECImageRes::Render(const ECRENDERPARAM* pRenderParam)
{
	bool bZWriteEnable = m_pA3DDevice->GetZWriteEnable();
	bool bZTestEnable = m_pA3DDevice->GetZTestEnable();
	bool bAlphaBlendEnable = m_pA3DDevice->GetAlphaBlendEnable();
	bool bAlphaTestEnable = m_pA3DDevice->GetAlphaTestEnable();
	
	
	
	//	Flush images behind text
	m_pA3DDevice->SetZWriteEnable(false);
	m_pA3DDevice->SetZTestEnable(true);
	m_pA3DDevice->SetAlphaBlendEnable(true);
	m_pA3DDevice->SetAlphaTestEnable(true);
	//if(bSplashRender)
	PresentDraws(/*CECImageRes::ORDER_BEHIND_TEXT*/);
	
	//	Flush images sorted with text
	//m_pA3DDevice->SetZWriteEnable(true);
	//PresentDraws(CECImageRes::ORDER_SORT_WITH_TEXT);
	
	CECWorld* pWorld = g_pGame->GetGameRun()->GetWorld();
	
	if (pWorld && pWorld->GetDecalMan())
	{
		m_pA3DDevice->SetZTestEnable(false);
		m_pA3DDevice->SetAlphaTestEnable(false);
		pWorld->GetDecalMan()->RenderPateTexts(pRenderParam->pViewport);
	}
	
	m_pA3DDevice->SetZWriteEnable(bZWriteEnable);
	m_pA3DDevice->SetZTestEnable(bZTestEnable);
	m_pA3DDevice->SetAlphaBlendEnable(bAlphaBlendEnable);
	m_pA3DDevice->SetAlphaTestEnable(bAlphaTestEnable);
	
	return true;
}
#endif