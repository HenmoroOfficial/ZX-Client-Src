// Filename	: DlgMidMap.h
// Creator	: Xiao Zhou
// Date		: 2006/5/25

#pragma once

#include "DlgBase.h"
#include "vector.h"
#include "hashmap.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUISlider.h"
#include "AUI\\AUIEditBox.h"
#include "AUI\\AUILabel.h"
#include "DlgMapTransfer.h"

class A3DStream;
class A3DTexture;
class A3DFTFont;

class CDlgMidMap : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
	
public:
	enum
	{
	   MAP_TYPE_CITY,
	   MAP_TYPE_TOWN,
	   MAP_TYPE_NUM,
	};
	typedef struct _MAPMARK
	{
		ACString strHint;
		A3DVECTOR3 vecPos;
		A3DRECT rect;
	} MAPMARK, * P_MAPMARK;
	
	CDlgMidMap();
	virtual ~CDlgMidMap();
	
	void OnCommand_ZoomIn(const char * szCommand);
	void OnCommand_ZoomOut(const char * szCommand);
	void OnCommand_AddMark(const char * szCommand);
	void OnCommand_Search(const char * szCommand);
	void OnCommand_WorldMap(const char * szCommand);
	void OnCommand_TerrMap(const char * szCommand);
	void OnCommand_KfWorldMap(const char * szCommand);
	void OnCommand_edit(const char * szCommand);
	void OnCommand_automove(const char * szCommand);
	void OnCommand_delete(const char * szCommand);
	void OnCommand_deleteall(const char * szCommand);
	void OnCommand_CANCEL(const char * szCommand);
	void OnCommand_CityMap(const char * szCommand);
	void OnCommand_TownMap(const char * szCommand);

	void OnEventMouseMove_MidMap(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonUp_MidMap(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDown_MidMap(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventRButtonDown_ListRegion(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventRButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventRButtonUp(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	// 取得指定地图的传送阵图标
	void GetMapTransferMark(unsigned int instanceId);

	static void MidMapRenderCallback(const A3DRECT &rc, DWORD param1, DWORD param2, DWORD param3);

	void SetEvent(int id, bool bevent);

	typedef struct _STownArea
	{
		float left,top,right,bottom;
		_STownArea()
		{
		    memset(this,0,sizeof(_STownArea));
		}
	}TownArea;
	
	struct MAPTLVERTEX
	{
		FLOAT x, y, z, rhw;
		DWORD diffuse;
		DWORD specular;
		FLOAT tu, tv, tu2, tv2;
	};
	
	struct NpcMonsterPos
	{
		ACString strName;
		int x, y, z;
	};
	typedef abase::vector<NpcMonsterPos> vecMonsterPos;

	struct TownNpcMonsterPos
	{
	    ACString strName;
		int x,y,z;
		AString strIconPath;
	};
	typedef abase::vector<TownNpcMonsterPos> vecTownNpcMonsterPos;

protected:
	virtual void OnTick();
	virtual bool Render();
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	bool UpdateMidMap();
	void CheckRenderArea();
	bool CheckNpcArea(int x, int z);
	bool IsStringMatch(ACString strName, ACString strSearch);
	static bool _ChinaPinYin(ACHAR chSearch, ACHAR chName);
    bool IsInTownPrecinct();
	bool GenerateTownArea();
	void GetMidMapOffsetParam(float& fScale,float& fOffsetX,float& fOffsetZ);
	void RenderTownPrecinct();
	void InitTownNpcMonsterPos();
	static abase::hash_map<int, vecMonsterPos> m_mapNpcMonsterPos;
	static abase::hash_map<int, vecTownNpcMonsterPos> m_mapTownNpcMonsterPos;
	static abase::vector<MAPMARK> m_vecMidMapMark;
	static MAPTLVERTEX	m_vertexMidMap[4];
	static A3DStream	*m_pMidMapStream;
	static A3DTexture	*m_pTexRadar;
	PAUIIMAGEPICTURE	m_pImg_MidMap;
	PAUISLIDER			m_pSld_MidMap;
	PAUISTILLIMAGEBUTTON m_pBtn_ZoomIn;
	PAUISTILLIMAGEBUTTON m_pBtn_ZoomOut;
	PAUIEDITBOX			m_pEdt_Search;
	PAUILABEL			m_pLab_SearchHint;
	AString				m_szPathName;
	ACString			m_strSearch;

	int					m_nSearchRet;
	int					m_nSearchTime;

	float				m_fZoom;
	A3DPOINT2			m_hostPt;
	int					m_nDownPosX, m_nDownPosY;
	int					m_nStartX;
	int					m_nStartY;
	DWORD				m_dwLBtnDownTime;
	A3DRECT				m_rcRenderArea;

	abase::vector<CDlgMapTransfer::MAPMARK> m_vecTransferMapMarks; // 上古传送阵图标信息
	
	static bool				m_bEvent[3];

    int                 m_iCurMapType;     
	TownArea            m_TownArea;
	A3DFTFont*          m_pFont;
};
