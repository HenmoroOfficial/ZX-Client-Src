#ifndef _A3DGRASSBITSMAP_H_
#define _A3DGRASSBITSMAP_H_

class A3DGrassBlock;
class A3DGrassRender;

///////////////////////////////////////////////////////////////////////////
//	
//	Class A3DGrassBitMap
//		a map used to determine where the grasses grow
//	
///////////////////////////////////////////////////////////////////////////

class A3DGrassBitsMap
{
public:

	friend class A3DGrassType;
	friend class A3DGrassCollectRender;
	friend class A3DGrassHWInstanceRender;
	typedef abase::vector<A3DGrassBlock *> BLOCKARRAY;

private:
	A3DDevice *			m_pA3DDevice;			// pointer to device object
	A3DGrassType *		m_pGrassType;			// pointer to the grass type
	DWORD				m_dwID;					// id of this bits map

	A3DGrassRender*		m_pGrassRender;			// grass render

	A3DTexture *		m_pTerrainLM;			// light map get from the terrain
	A3DTexture *		m_pTerrainLM_n;			// light map get from the terrain for night
	ARectF				m_rectLM;				// light map cover range

	// bits map data
	BYTE *				m_pBitsMap;				// the buffer that contains bits map data
	int					m_nWidth;				// width of the bits map (int bits)
	int					m_nHeight;				// height of the bits map

	int					m_nNumBlockX;			// block column count
	int					m_nNumBlockY;			// block row count

	// location information
	A3DVECTOR3			m_vecCenter;			// center position of this bits map
	A3DVECTOR3			m_vecOrigin;			// origin position of this bits map
	float				m_vGridSize;			// grid size for each bit

	// blocks data
	int					m_nNumBlocks;			// number of blocks this bits map is devided
	A3DGrassBlock **	m_ppBlocks;				// the blocks buffer


	BLOCKARRAY			m_VisibleBlocks;

	DWORD				m_dwExportID;			// id used when exporting game data
	bool				m_bAllBitsZero;			// All bits is zero

public:
	inline void SetID(DWORD dwID)			{ m_dwID = dwID; }
	inline DWORD GetID()					{ return m_dwID; }
	inline A3DGrassType * GetGrassType()	{ return m_pGrassType; }
	inline BYTE * GetBitsMap()				{ return m_pBitsMap; }
	inline int GetWidth()					{ return m_nWidth; }
	inline int GetHeight()					{ return m_nHeight; }

	inline const A3DVECTOR3& GetCenter() const { return m_vecCenter; }
	inline const A3DVECTOR3& GetOrigin() const { return m_vecOrigin; }
	inline float GetGridSize() const		{ return m_vGridSize; }

	//	Get / Set export ID
	inline void SetExportID(DWORD dwID)		{ m_dwExportID = dwID; }
	inline DWORD GetExportID()				{ return m_dwExportID; }

	inline bool GetAllBitsZeroFlag()		{ return m_bAllBitsZero; }
	inline ARectF GetRectLM() const			{ return m_rectLM; }
	inline A3DTexture* GetTerrainLM(bool bDay) { return bDay ? m_pTerrainLM : m_pTerrainLM_n; }

	//	Get max render distance
	float GetMaxRenderDist() const;
	inline const BLOCKARRAY& GetVisibleBlocks() const { return m_VisibleBlocks; }

protected:
	bool CreateBlocks();
	bool ReleaseBlocks();

	bool UpdateVisibleBlocks(const A3DVECTOR3& vecCenter);

public:
	A3DGrassBitsMap();
	~A3DGrassBitsMap();

	// Function Init(vecCenter, float vGridSize, nWidth, nHeight)
	//		Initialize this bits map for the type of grass
	//
	//	vecCenter	the center of this bits map
	//	vGridSize	the grid size of each bit
	//	nWidth		the width of the bits map
	//	nHeight		the height of the bits map												 
	//
	bool Init(A3DDevice * pA3DDevice, A3DGrassType * pGrassType, DWORD dwID, const A3DVECTOR3& vecCenter, float vGridSize, int nWidth, int nHeight);

	// Function Release()
	//		Dealloc the memory resources
	//
	bool Release();

	// Function Update(pBitsMap, ARectI * pRectUpdate)
	//		Update this bits map according to the passed in bits map
	//
	// IN
	//		pBitsMap		new bits map information
	//
	bool UpdateBitsMap(BYTE * pBitsMap);

	// call UpdateGrassesForRender after you called UpdateBitsMap
	bool UpdateGrassesForRender(A3DViewport * pViewport);

	// adjust after changing lod level
	bool AdjustForLODLevel();

	// Function Update(A3DViewport * pCurViewport)
	//		Load and unload some blocks according to current camera position
	//
	bool Update(const A3DVECTOR3& vecCenter, float fDeltaTime);

	// Function Render(A3DViewport * pViewport)
	//		Render these grasses in this map
	//
	// IN
	//	pViewport			viewport to show the grass
	//	pStream				stream used to render
	//
	// RETURN
	//		return the new grasses that have been put into pBuffer
	bool Render(A3DViewport * pViewport);

	// Function ShiftCenterTo(const A3DVECTOR3& vecPos)
	//		shift the center of this grass bits map to a new position
	bool ShiftCenterTo(const A3DVECTOR3& vecPos);

	//	Used when export grass data. Return true if not all bits are zero,
	//	this function also set m_bAllBitsZero flag
	bool BitsMapCheck();

private:
	bool CollectRender(A3DViewport * pViewport);
	int GetGrassBlkSize() const;
};



#endif