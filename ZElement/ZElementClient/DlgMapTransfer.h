// Filename	: DlgMapTransfer.h
// Creator	: WuWeixin
// Date		: 2012/8/20
/************************************************************************
 modified by fuchonggang 2013.09.07
 修改为跟世界地图一样，3级地图跳转，如果点击地图内部没有可传送点则不可点击
 类似CDlgWorldMap

************************************************************************/

#pragma once

#include "DlgBase.h"
#include "vector.h"
#include "AUI\AUIImagePicture.h"
#include "AUI\AUILabel.h"
#include "ExpTypes.h"

class A3DStream;
class A3DTexture;


class CDlgMapTransfer : public CDlgBase  
{	
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
	
public:
	typedef struct _MAPMARK
	{
		int        index;
		ACString   strHint;
		A3DRECT    rect;
		A3DRECT    transRect;
		BOOL       active;
	} MAPMARK, * P_MAPMARK;

	struct MAPTLVERTEX
	{
		FLOAT x, y, z, rhw;
		DWORD diffuse;
		DWORD specular;
		FLOAT tu, tv, tu2, tv2;
	};
	
	CDlgMapTransfer();
	virtual ~CDlgMapTransfer();
	
	void OnCommand_CANCEL(const char * szCommand);
	void OnCommand_Return(const char * szCommand);

	// 处理鼠标移动消息以显示传送阵图标的悬浮信息
	void OnEventMouseMove_Map(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	// 处理鼠标点击传送阵图标
	void OnEventLButtonUp_Map(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	void OnEventRButtonUp_Map(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	// 在npc回应后用于弹出界面
	void PopupDlgMapTransfer(unsigned int currentTransId, const NPC_UI_TRANSFER_SERVICE *pService);

	// 绘制地图和图标
	static void MapRenderCallback(const A3DRECT &rc, DWORD param1, DWORD param2, DWORD param3);
	
protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnHideDialog();
	virtual void OnTick();
	virtual bool Render();

	// 创建纹理
	void InitTex();

	// 绘制图标
	void RenderMapMark();

	// 更新图标
	void UpdateMark();


	void FillAreaRects(int nType);

	// 显示指定的地图
	void ShowMap(unsigned int mapId);

	// 返回上一级地图 
	void ReturnBackMap();
	
	bool IsMapWithTransferPoint(int iMapId);


	bool IsHongHuangMap(int iMapId);
	void DrawMap();

	A3DPOINT2 ScreenToPixel(A3DPOINT2 pt);
	A3DPOINT2 PixelToScreen(A3DPOINT2 pt);

	void RenderArea(int nIndex, A3DCOLOR cl);


	static MAPTLVERTEX	m_vertexMidMap[4];
	static A3DStream	*m_pMapStream;
	static A2DSprite    *m_pA2DMapInactiveMark;
	static A2DSprite    *m_pA2DMapActiveMark;
	static A3DTexture	*m_pTexMap;
	PAUIIMAGEPICTURE	m_pImg_Map;
	PAUILABEL			m_pLable_Title;
	AString				m_szPathName;
	A3DRECT				m_rcMap;
	A3DPOINT2			m_instanceSize;
	unsigned int		m_currentTransId;
	abase::vector<MAPMARK> m_vecMapMark;
	abase::vector<UI_TRANSFER_CONFIG::TRANS_POINT> m_vecTransPoints;
	abase::vector<int>  m_vecAactiveTransid;
	unsigned int		m_iIdMap;
	int					m_iMapType;		// CDlgWorldMap中EnumWorldMapType
	int					m_nHoverAreaIndex;
	abase::vector<A3DRECT>	m_vecAreaRect;
	const NPC_UI_TRANSFER_SERVICE * m_pTransService;
	static const int	m_isTransIdCount;
	static const int	m_isTransPointCount;


public:
	static const int ms_uitransferConfigId; // 传送阵模板id

	A2DSprite* GetA2DMapActiveMark();
	A2DSprite* GetA2DMapInactiveMark();
};
