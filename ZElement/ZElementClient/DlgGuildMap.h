// Filename	: DlgGuildMap.h
// Creator	: Xiao Zhou
// Date		: 2005/12/31

#pragma once

#include "DlgBase.h"
#include "vector.h"
#include "AUI\\AUIImagePicture.h"

class CDlgGuildMap : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP()
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgGuildMap();
	virtual ~CDlgGuildMap();

	void OnCommandCancel(const char * szCommand);

	void OnEventMouseMove(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventMButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventRButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	void BattleAction(int idAction, void *pData);

	enum
	{
		BATTLE_ACTION_BATTLECHALLENGE_RE,
		BATTLE_ACTION_BATTLECHALLENGEMAP_RE,
		BATTLE_ACTION_BATTLEGETMAP_RE,
		BATTLE_ACTION_BATTLEENTER_RE,
		BATTLE_ACTION_BATTLESTATUS_RE,
	};

	enum
	{
		AREA_TYPE_NORMAL = 0,
		AREA_TYPE_OWNER = 1,
		AREA_TYPE_CANCHALLENGE = 2,
		AREA_TYPE_TEMPCHALLENGED = 4,
		AREA_TYPE_CHALLENGED = 8,
		AREA_TYPE_BECHALLENGED = 16,
		AREA_TYPE_CANENTER = 32,
		AREA_TYPE_HOVER = 64,
	};

	enum
	{
		FACTIONMAP_TYPE_NORMAL,
		FACTIONMAP_TYPE_CHALLENGE,
		FACTIONMAP_TYPE_ENTER,
	};

	void SetType(int nType) { m_nType = nType; }
	int GetType() { return m_nType;	}
	void SetMapStatus(bool b) { m_bGetMap = b; }
	bool GetMapStatus() { return m_bGetMap; }
	void GetMap();
	bool IsBattleOpen() { return m_bBattleOpen; }
	void SetBattleChallengeOpen(bool b) { m_bBattleChallengeOpen = b; }
	bool IsBattleChallengeOpen() { return m_bBattleChallengeOpen; }

	void UpdateNormalMap();
	void UpdateChallengeMap();

public:
	abase::vector<int>			m_vecAreaType;

protected:
	virtual void OnTick();
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual bool Render();
	
	void RenderArea(int nIndex, A3DCOLOR cl);
	void RenderArrow(int nIndex1, int nIndex2, A2DSprite *pArrow);

	A3DPOINT2 ScreenToPixel(A3DPOINT2 pt);
	A3DPOINT2 PixelToScreen(A3DPOINT2 pt);

	PAUIIMAGEPICTURE			m_pImg_GuildMap;
	int							m_nHoverAreaIndex;
	abase::vector<A3DRECT>		m_vecAreaRect;
	abase::vector<A3DCOLOR>		m_vecFactionColor;
	int							m_nType;
	bool						m_bGetMap;
	bool						m_bGetChallengeMap;
	A2DSprite*					m_pA2DSpriteArrow1;
	A2DSprite*					m_pA2DSpriteArrow2;
	bool						m_bBattleOpen;
	bool						m_bBattleChallengeOpen;
	int							m_nMyFactionColor;

};
