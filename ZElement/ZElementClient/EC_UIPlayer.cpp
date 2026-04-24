/********************************************************************
  created:	   23/5/2006  
  filename:	   ECUIPlayer.cpp
  author:      Wangkuiwu  
  description: 
  Copyright (c) , All Rights Reserved.
*********************************************************************/

#ifdef LUA_UI

#include "EC_UIPlayer.h"
#include "EC_LoginPlayer.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_World.h"
#include "EC_ManPlayer.h"
#include "EC_Model.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "A3DMacros.h"
#include "A2DSprite.h"
#include "AUI\\AUICommon.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CECUIPlayer::CECUIPlayer()
:m_pDemoPlayer(NULL), m_TempPlayerId(0), m_Angle(0),
m_pA2DSprite(NULL)
{

	m_bAcceptMouseMsg = false;
}

CECUIPlayer::~CECUIPlayer()
{
	A3DRELEASE(m_pDemoPlayer);
	A3DRELEASE(m_pA2DSprite);
}

bool CECUIPlayer::CreateDemoPlayer(int templateID)
{

	if (templateID == m_TempPlayerId && m_pDemoPlayer)
	{
		return true;
	}

	CECPlayerMan * pPlayerMan =  g_pGame->GetGameRun()->GetWorld()->GetPlayerMan();
	assert(pPlayerMan);

	CECPlayer *pPlayer = pPlayerMan->GetPlayer(templateID);
	if( !pPlayer )
		return false;

	A3DRELEASE(m_pDemoPlayer);
	m_TempPlayerId = templateID;
	m_pDemoPlayer = new CECLoginPlayer(pPlayerMan);
	
	if (!m_pDemoPlayer->Load(pPlayer->GetFaceid(), pPlayer->GetHairid(), pPlayer->GetGender(), NULL))
	{
		A3DRELEASE(m_pDemoPlayer);
		return false;
	}
	else
	{
		m_pDemoPlayer->SetPos(A3DVECTOR3(0));
		m_pDemoPlayer->SetDirAndUp(A3DVECTOR3(0, 0, -1.0f), A3DVECTOR3(0, 1.0f, 0));
		m_pDemoPlayer->GetPlayerModel()->SetAutoUpdateFlag(false);
		m_pDemoPlayer->SetFashionMode(pPlayer->InFashionMode());
	}

	return true;

}

void CECUIPlayer::UpdateEquips()
{
	if (!m_pDemoPlayer)
	{
		return;
	}
	CECHostPlayer * pHost = g_pGame->GetGameRun()->GetHostPlayer();
	assert(pHost);
	if (m_TempPlayerId == pHost->GetCharacterID())
	{
		pHost->SyncDemonstrationPlayer(m_pDemoPlayer);
	}
	else
	{
		CECPlayer * pSrcPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetPlayer(m_TempPlayerId);
		if (!pSrcPlayer)
		{
			return ;
		}
		int equips[SIZE_EQUIPIVTR];
		CECInventory *pInventory = g_pGame->GetGameRun()->GetHostPlayer()->GetEPEquipPack();
		CECIvtrItem *pItem;
		for (int i = 0; i < SIZE_EQUIPIVTR; ++i)
		{
			pItem = pInventory->GetItem(i);
			equips[i] = (pItem)? (pItem->GetTemplateID()): 0;
		}
		m_pDemoPlayer->UpdateEquipments(equips, pSrcPlayer->InFashionMode());
	}

}

bool CECUIPlayer::Render()
{
	bool bval;

	if (m_pDemoPlayer)
	{
		if( m_pA2DSprite )
		{
			if( m_nWidth <= 0 )
				m_nWidth = m_pA2DSprite->GetWidth();
			if( m_nHeight <= 0 )
				m_nHeight = m_pA2DSprite->GetHeight();

			float fScaleX = float(m_nWidth) / float(m_pA2DSprite->GetWidth());
			float fScaleY = float(m_nHeight) / float(m_pA2DSprite->GetHeight());

			m_pA2DSprite->SetScaleX(fScaleX);
			m_pA2DSprite->SetScaleY(fScaleY);

			m_pA2DSprite->SetColor(m_color);
			m_pA2DSprite->SetAlpha(m_nAlpha);
			
			POINT ptPos = GetPos();
			if( m_pParent->IsNoFlush())
				bval = m_pA2DSprite->DrawToBack(ptPos.x, ptPos.y);
			else
				bval = m_pA2DSprite->DrawToInternalBuffer(ptPos.x, ptPos.y);
			if( !bval ) return AUI_ReportError(__LINE__, 1, "AUIImagePicture::Render(), failed to call m_pA2DSprite->DrawToBack() or m_pA2DSprite->DrawToInternalBuffer()");

			m_pA2DSprite->SetColor(A3DCOLORRGB(255, 255, 255));
		}

		// first flush all sprite here to make the background shown
		bval = m_pA3DEngine->FlushInternalSpriteBuffer();
		if( !bval ) return AUI_ReportError(__LINE__, 1, "AUIImagePicture::Render(), failed to call m_pA3DEngine->FlushInternalSpriteBuffer()");

		A3DRECT rc = GetRect();
		rc.Offset(m_nOffX, m_nOffY);
		DWORD dummy = 0;
		PlayerRenderDemonstration(rc,  (DWORD)m_pDemoPlayer, m_Angle, dummy);
	}
	
	return AUIObject::Render();
}

bool CECUIPlayer::Tick()
{
	if (m_pDemoPlayer)
	{
		m_pDemoPlayer->Stand();
		m_pDemoPlayer->Tick(g_pGame->GetTickTime());
	}

	return AUIObject::Tick();
}

bool CECUIPlayer::InitBkImg(const char * szFileName)
{
	if (strlen(szFileName) <= 0)
	{
		return false;
	}
	bool bval;

	ASSERT(m_pA3DEngine && m_pA3DDevice);
	A3DRELEASE(m_pA2DSprite);
	m_pA2DSprite = new A2DSprite;
	if( !m_pA2DSprite ) return false;
	bval = m_pA2DSprite->Init(m_pA3DDevice, szFileName, AUI_COLORKEY);
	if( !bval )
	{
		delete m_pA2DSprite;
		m_pA2DSprite = NULL;
		return AUI_ReportError(__LINE__, 1, "AUIImagePicture::InitIndirect(), failed to call m_pA2DSprite->Init()");
	}
		
	return true;
}

bool CECUIPlayer::SetProperty(const char *pszPropertyName, PAUIOBJECT_SETPROPERTY Property)
{
	if( 0 == strcmpi(pszPropertyName, "Image File") )
		return InitBkImg(Property->fn);

	return AUIObject::SetProperty(pszPropertyName, Property);
}

bool CECUIPlayer::RegisterPropertyType()
{
	return	SetAUIObjectPropertyType("Image File", AUIOBJECT_PROPERTY_VALUE_CHAR);

}

#endif