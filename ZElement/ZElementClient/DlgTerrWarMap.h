// Filename	: DlgTerrWarMap.h
// Creator	: Fu Chonggang
// Date		: 2010/3/10

#pragma once

#include "DlgBase.h"
#include "AUI\\AUIImagePicture.h"
#include "vector.h"

class CDlgTerrWarMap : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP();
	AUI_DECLARE_COMMAND_MAP();

public:
	enum TerriTime
	{
		ST_NONE		= 0x000,
		ST_BID		= 0x004,	// 宣战
		ST_ASSISGET	= 0x008,	// 可领佣兵凭证
		ST_FIGHT	= 0x010,	// 战斗中
	};
	enum SpriteIcon
	{
		SI_MAP				= 0,
		SI_ARROW1			= 1,
		SI_ARROW2			= 2,
		SI_ARROW3			= 3,
		SI_ARROW_DOUBLE1	= 4,
		SI_ARROW_DOUBLE2	= 5,
		SI_ARROW_DOUBLE3	= 6,
		SI_DECLARED			= 7,
		SI_DECLARE_SUCCESS	= 8,
		SI_FIGHT			= 9,
		SI_GODITEM			= 10,
		SI_NUM				= 11,
	};
	struct AreaInfoMap
	{
		int		nIndex;
		A3DRECT rc;
		int		nColorIndex;
		bool	bCanDeclare;
	};

public:
	CDlgTerrWarMap();
	virtual ~CDlgTerrWarMap();

	void OnCommandTerriDetail(const char* szCommand);

	void OnEventMouseMove(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventLButtonUp(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventRButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventRButtonUp(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	void UpdateMapInfo(int status, bool);
	void GetAward(int id);
	void ChangeSize();
	bool RenderDetail() { return m_nScale==1; }
	int	GetStatus() { return m_iStatus; }
	static void SetMOpen(bool bM = true) { m_bMOpen = bM; }

protected:
	virtual bool OnInitDialog();
	virtual bool Release();
	virtual void OnShowDialog();
	virtual void OnTick();
	virtual bool Render();

	void UpdateHint();
	void RenderArea(int nIndex, A3DCOLOR cl);
	void RenderArrow(int nIndex1, int nIndex2, bool bDouble);
	A3DPOINT2 ScreenToPixel(A3DPOINT2 pt);
	A3DPOINT2 PixelToScreen(A3DPOINT2 pt);
	virtual void Resize(A3DRECT rcOld, A3DRECT rcNew);

	abase::vector<AreaInfoMap>	m_vecAreaInfo;
	PAUIIMAGEPICTURE		m_pImg_WorldMap;
	int						m_iStatus;
	int						m_nHoverAreaIndex;

	A2DSprite*				m_pA2DSpriteIcon[SI_NUM];
	A2DSprite*				m_pImageMapIcon;
	int						m_nImageMapIconIndex;
	int						m_nHintImageIndex;
	int						m_nHintImageFrame;
	bool					m_bHintImageAdd;
	ACString				m_strItemDesc;

	int					m_nX;
	int					m_nY;
	int					m_nScale;
	bool				m_bDrag;
	int					m_nStartX;
	int					m_nStartY;
	DWORD				m_dwFlashTime;
	bool				m_bOwnTerr;
	bool				m_bDebug;
	static bool			m_bMOpen;
};
