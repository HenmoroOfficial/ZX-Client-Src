// Filename	: DlgWorldMap.h
// Creator	: Tom Zhou
// Date		: October 16, 2005
/************************************************************************
 modified by fuchonggang 2010.11.24


		 ->领土战地图
 原服点击->世界地图		<->点击进入相应场景地图
			|			->领土战地图<->详细领土战地图
		   上一级总图
			|
 跨服点击->跨服世界		<->点击进入相应场景地图
 Worldmap.xml			包含一个子界面，作为一个盒子使用
 Worldmap_Button.xml	按钮（向上、返回...）
 Worldmap_Sub.xml		普通世界地图

 Worldmap_Terr.xml		领土战地图（需要缩放及其它操作）
  
************************************************************************/
#pragma once

#include "DlgBase.h"
#include "AUI\\AUIImagePicture.h"
#include "vector.h"
enum EnumWorldMapType
{
	MAP_MAIN	= 0,	// 总图
	MAP_CROSS,			// 跨服地图
	MAP_NORMAL,			// 普通世界地图
	MAP_HONGHUANG,		// 洪荒世界地图
	MAP_CROSSXX,		// 跨服地图某个图
	MAP_NORMALXX,		// 普通地图某个图
	MAP_HONGHUANGXX,	// 洪荒地图某个图
};

class CDlgWorldMap : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP();
	AUI_DECLARE_COMMAND_MAP()


public:
	typedef struct _MAPMARK
	{
		ACString strHint;
		A3DVECTOR3 vecPos;
		A3DRECT rect;
	} MAPMARK, * P_MAPMARK;

	CDlgWorldMap();
	virtual ~CDlgWorldMap();

	void OnCommandReturn(const char* szCommand);
	void OnCommandTerrMap(const char* szCommand);
	void OnCommandSize(const char* szCommand);
	void OnCommandWorldMap(const char* szCommand);
	void OnCommandCancel(const char* szCommand);

	void OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventMouseMove(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventRButtonUp(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	void Return(int nType);

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnHideDialog();
	virtual bool Render();
	virtual bool Release();

	// 普通世界分为诛仙世界和洪荒世界，44、48、53为洪荒世界
	// 返回EnumWorldMapType中的MAP_NORMAL或洪荒世界地图
	int GetNormalMapType();

	void RenderArea(int nIndex, A3DCOLOR cl);
	A3DPOINT2 ScreenToPixel(A3DPOINT2 pt);
	A3DPOINT2 PixelToScreen(A3DPOINT2 pt);
	void MapResize();

	abase::vector<MAPMARK>	m_vecWorldMapMark;
	abase::vector<A3DRECT>	m_vecAreaRect;
	PAUIIMAGEPICTURE		m_pImg_WorldMap;
	int						m_idMap;
	int						m_nHoverAreaIndex;
	static int				m_snMapType;
	A2DSprite				*m_pImgHeaven;
	A2DSprite				*m_pImgEarth;
};
