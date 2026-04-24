/*
 * FILE: Terrain.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/6/15
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "AArray.h"
#include "ARect.h"
#include "EditObject.h"
#include "A3DTrace.h"
#ifdef _ANGELICA22
#include "BitChunk.h"
#endif // #ifdef _ANGELICA22

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

//	Default mask texture size
#define DEFMASK_SIZE	1024

///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////

#pragma pack (push, 1)

//	Terrain file data struct
struct TERRAINFILEDATA
{
	float	fTileSize;		//	Terrain tile size
	float	fYOffset;		//	Terrain's offset on Y axis
	float	fMaxHeight;		//	Terrain's maximum height value
	int		iHMWidth;		//	Size of height map
	int		iHMHeight;
	int		iNumLayer;		//	Number of layer
	DWORD	dwRndUpdate1;	//	Render udpate counter 1
	bool	bNormalData;	//	true, save normal data
	DWORD	dwVersion;		//	File version
	BYTE	aReserved[55];	//	Reserved
};

#pragma pack (pop)

class A3DCameraBase;
class A3DViewport;
#ifdef _ANGELICA22
class A3DTexture;
#endif // #ifdef _ANGELICA22
class CTerrainRender;
class CTerrainLayer;
class CElementMap;
class CMaskModifier;
class CDataChunkFile;

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CTerrain
//	
///////////////////////////////////////////////////////////////////////////

class CTerrain : public CEditObject
{
public:		//	Types

	//	Height map type
	enum
	{
		HM_8BIT = 0,
		HM_16BIT,
		HM_FLOAT,
	};
	
	//	Initialize data
	struct INITDATA
	{
		float	fTileSize;		//	Terrain size in meters
		float	fMinHeight;		//	Minimum absolute height 
		float	fMaxHeight;		//	Maximum absolute height 
	};

	//	Height map data
	struct HEIGHTMAP
	{
		int			iWidth;			//	Size of height map
		int			iHeight;
		float*		pHeightData;	//	Raw height data
		float*		pAbsHeight;		//	Absolute height data
		A3DVECTOR3*	aNormals;		//	Vertex normal data
	};

#ifdef _ANGELICA22
	// Color map data
	struct VERTEXCOLOR
	{
		int			iWidth;
		int			iHeight;
		unsigned int*	pColor;
	};
#endif // #ifdef _ANGELICA22

public:		//	Constructor and Destructor

	CTerrain(CElementMap* pMap);
	virtual ~CTerrain();

public:		//	Attributes

public:		//	Operations

	//	Initialize object
	bool Init(const INITDATA& InitData);
	//	Release object
	virtual void Release();

	//	Load data
	virtual bool Load(CELArchive& ar);
	//	Save data
	virtual bool Save(CELArchive& ar);

	//	Frame move routine
	bool FrameMove(DWORD dwDeltaTime, A3DCameraBase* pA3DCamera=NULL);
	//	render routine
#ifdef _ANGELICA22
	bool Render(A3DViewport* pA3DViewport, bool bHDR);
#else
	bool Render(A3DViewport* pA3DViewport);
#endif // #ifdef _ANGELICA22

	//	Load height map from raw data file
	bool LoadHeightMap(int iWid, int iHei, int iType, const char* szFile, bool bExtraData);
#ifdef _ANGELICA22
	bool LoadHeightMapFromChunk( int iWid, int iHei, const char* szFile);
#endif // #ifdef _ANGELICA22

	//	Release terrain render
	void ReleaseRender();
	//	Reload terrain render
	bool ReloadRender(bool bNoRender);
	//	Update vertex positions 
	bool UpdateVertexPos(const ARectF& rcArea);
	//	Update vertex normals
	bool UpdateVertexNormals(const ARectF& rcArea);

	//	Ray trace function
	virtual bool RayTrace(const A3DVECTOR3& vStart, const A3DVECTOR3& vVel, float fTime, RAYTRACERT* pTraceRt);
	//	Get height of specified position
	virtual float GetPosHeight(const A3DVECTOR3& vPos);
	//	Get normal of specified position
	virtual bool GetPosNormal(const A3DVECTOR3& vPos, A3DVECTOR3& vNormal);
	//	Set map offset
	virtual void SetMapOffset(float x, float z);

	//	Get layer number
	int GetLayerNum() { return m_aLayers.GetSize(); }
	//	Get layer by index
	CTerrainLayer* GetLayer(int n) { return m_aLayers[n]; }
	//	Set layer by index
	void SetLayer(int n, CTerrainLayer* pLayer) { m_aLayers[n] = pLayer; }
	//	Create a new layer
	CTerrainLayer* CreateLayer(const char* szName, bool bDefault);
	//	Release a layer and remove it from array
	void ReleaseLayer(int iIndex);
	//	Remove a layer but not release it
	CTerrainLayer* RemoveLayer(int iIndex);
	//	Search layer base on name
	int SearchLayer(const char* szName);
	//	Get layer's index
	int GetLayerIndex(CTerrainLayer* pLayer);

	//	Get height map data
	HEIGHTMAP* GetHeightMap() { return &m_HeightMap; }
	float GetHeightAt(float x, float z);
#ifdef _ANGELICA22
	//	Get color map data
	VERTEXCOLOR* GetColorMap() { return &m_ColorMap; }
#endif // #ifdef _ANGELICA22
	//	Get terrain size
	float GetTerrainSize() { return m_fTerrainSize; }
	//	Get height scale of terrain
	float GetMaxHeight() { return m_fMaxHeight; }
	//	Get offset of terrain on Y axis
	float GetYOffset() { return m_fYOffset; }
	//	Get terrain tile size
	float GetTileSize() { return m_fTileSize; }
	//	Get terrain area in meters
	const ARectF& GetTerrainArea() { return m_rcTerrain; }
	//	Get map object
	CElementMap* GetMap() { return m_pMap; }
	//	Get terrain render object
	CTerrainRender* GetRender() { return m_pTrnRender; }
	//	Get mask modifier
	CMaskModifier* GetMaskModifier() { return m_pMaskModify; }
	//	Set update render flag
	void SetUpdateRenderFlag(bool bTrue)
	{ 
		if (bTrue)
			m_dwRndUpdate1++;
		else
			m_dwRndUpdate2 = m_dwRndUpdate1;
	}
	//	Get update render flag ?
	bool GetUpdateRenderFlag() { return m_dwRndUpdate1 == m_dwRndUpdate2 ? false : true; }
	//	Get render update counter 1
	DWORD GetRenderUpdateCnt1() { return m_dwRndUpdate1; }
	//	Get render update counter 2
	DWORD GetRenderUpdateCnt2() { return m_dwRndUpdate2; }
	//	Set render update counter 2
	void SetRenderUpdateCnt2(DWORD dwCnt) { m_dwRndUpdate2 = dwCnt; }
	//	Set save normal flag
	void SetSaveNormalFlag(bool bSave) { m_bSaveNormal = bSave; }
	//	Get save normal flag
	bool GetSaveNormalFlag() { return m_bSaveNormal; }
	//	Get terrain file version
	DWORD GetVersion() { return m_dwVersion; }
	//	Get mask map file object
	CDataChunkFile* GetMaskMapFile() { return m_pMaskMapFile; }
	
	//	Modify terrain properties. Below functions won's change terrain data
	//	immediately. Call UpdateTerrainMeshPosAndNormal() to apply these modifies
	void SetTileSize(float fSize);
	void SetYOffset(float fOffset) { m_fYOffset = fOffset; }
	void SetMaxHeight(float fMaxHei) { m_fMaxHeight = fMaxHei; }
#ifdef _ANGELICA22

	//	Get mesh data
	A3DVECTOR3* GetVertexPos() { return m_aPos; }

	void UpdateVertexColorMapToRender( ARectI& rc);
	static bool FillLightMap(int iMapSize, BYTE* pSrcData, A3DFORMAT DstFmt, A3DTexture* pTexture);
	void RecreateColorMap(int iWidth, int iHeight);
	bool ExportHeightMap(const char* szPath);

	void RecreateVisibleBlock();
	bool LoadVisibleBlock(const char* szPath);
	void SaveVisibleBlock(const char* szPath);
	void SetVisibleBlock(int w, int h, bool bVisible){ m_btVisible.SetValue(w,h,bVisible); }
	bool IsVisibleBlock(int w, int h)
	{  
		if(w < m_btVisible.GetChunkWidth() && h < m_btVisible.GetChunkHeight())
			return m_btVisible.GetVaule(w,h) > 0 ? true : false;
		else return false;
	}
	void UpdateVisibleFlag();

#endif // #ifdef _ANGELICA22
protected:	//	Attributes

	CElementMap*	m_pMap;			//	Map object
	CTerrainRender*	m_pTrnRender;	//	Terrain render
	CMaskModifier*	m_pMaskModify;	//	Mask modify
	CDataChunkFile*	m_pMaskMapFile;	//	Mask map file
	
	float		m_fTerrainSize;		//	Terrain size
	HEIGHTMAP	m_HeightMap;		//	Height map data
	float		m_fYOffset;			//	Terrain's offset on Y axis
	float		m_fMaxHeight;		//	Terrain's height scale, this isn't the maximum absolute height of terrain
	float		m_fTileSize;		//	Terrain tile size in meters
	ARectF		m_rcTerrain;		//	Terrain area in meters
	DWORD		m_dwRndUpdate1;		//	Render update counter 1
	DWORD		m_dwRndUpdate2;		//	Render update counter 2
	bool		m_bSaveNormal;		//	true, save normal data
	DWORD		m_dwVersion;		//	Terrain file version
#ifdef _ANGELICA22
	VERTEXCOLOR	m_ColorMap;			//	Terrain color map
#endif // #ifdef _ANGELICA22

	APtrArray<CTerrainLayer*>	m_aLayers;		//	Layers

#ifdef _ANGELICA22
	CBitChunk	m_btVisible;

	A3DVECTOR3* m_aPos;			//	Terrain mesh
#endif // #ifdef _ANGELICA22

protected:	//	Operations

	//	Initialize some common data
	bool CommonInit();
	//	Calculate terrain area
	void CalcTerrainArea();
	//	Create terrain render
	bool CreateTerrainRender(bool bNoRender);

	//	Create empty height map
	bool CreateEmptyHeightMap(int iWid, int iHei);
#ifdef _ANGELICA22
	//	Save height map data
	bool SaveHeightMap();
	//	Save color map data
	bool SaveColorMap();
	//	Load color map from terrain file
	bool LoadColorMap();
#else
	//	Save height map data
	bool SaveHeightMap(CELArchive& ar);
#endif // #ifdef _ANGELICA22
	//	Load height map from terrain file
	bool LoadHeightMap(const TERRAINFILEDATA& Data);	// Ver: 4
	bool LoadHeightMap(CELArchive& ar, const TERRAINFILEDATA& Data); // Ver: 2
	//	Load layers from terrain file
	bool LoadLayers(CELArchive& ar, const TERRAINFILEDATA& Data);
	//	Save layers data
	bool SaveLayers(CELArchive& ar);
	//	Release current height map
	void ReleaseHeightMap();
	//	Release all layers
	void ReleaseAllLayers();
	//	Create vertex normals
	bool CreateVertexNormals();
	//	Create absolute height data
	bool CreateAbsHeightData();
	//	Save terrain normal data
	bool SaveVertexNormals();
	//	Load terrain normal data
	bool LoadVertexNormals();

	//	Get real height
	float GetRealHeight(int n) { return m_fYOffset + m_HeightMap.pHeightData[n] * m_fMaxHeight; }
#ifdef _ANGELICA22

	//	Create terrain mesh
	bool CreateTerrainMesh();	
#endif // #ifdef _ANGELICA22
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////

#ifdef _ANGELICA22
bool read_file_to_buffer(const char* szPath, void* pBuffer, size_t lenInBytes);
#endif // #ifdef _ANGELICA22