/*
 * FILE: A3DGrassland.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Hedi, 2004/10/21
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "A3DTypes.h"
#include "A3DVertex.h"
#include "AArray.h"
#include "ALog.h"
#include "AC.h"
#include "A3DTerrain2.h"
#include "A3DGrassType.h"

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

class A3DCamera;
class A3DTerrain2;
class A3DGrassInteractor;
class A3DGrassRender;
class A3DPixelShader;
class A3DVertexShader;

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CELArchive
//	
///////////////////////////////////////////////////////////////////////////

#define WIND_BUF_LEN	80

///////////////////////////////////////////////////////////////////////////
// class to wave in the wind
///////////////////////////////////////////////////////////////////////////
class A3DWindWaver
{
private:
	A3DVECTOR3		m_vecPos;
	A3DVECTOR3		m_vecVelocity;
	A3DVECTOR3		m_vecAccel;
	A3DVECTOR3		m_vecSpringForce;
	A3DVECTOR3		m_vecWindForce;
	A3DVECTOR3		m_vecForceCombined;

	float			m_vMass;

	float			m_k1;
	float			m_k2;
	float			m_f;

public:
	inline void SetWindForce(A3DVECTOR3 &vecForce) { m_vecWindForce = vecForce; }
	inline const A3DVECTOR3 & GetPos() { return m_vecPos; }

protected:
	bool CalculateForce(const A3DVECTOR3& vecPos, const A3DVECTOR3& vecVelocity);
	bool UpdatePos(float dt);

public:
	A3DWindWaver();
	~A3DWindWaver();

	// Calculate the particle's state after nDeltaTime (in milliseconds)
	bool Tick(int nDeltaTime);
};

class A3DGrassLand
{
protected:
	A3DDevice *			m_pA3DDevice;			// a3d device object pointer
	A3DTerrain2 *		m_pA3DTerrain;			// terrain object for terrain height calculation
	ALog *				m_pLogFile;				// the log file pointer
	A3DGrassInteractor* m_pGrassInteractor;		// grass interactor
	A3DGrassRender*		m_pGrassRender;			// grass render
	
	bool				m_bUseSW;				// flag indicates use software vertex processing

	bool				m_bProgressLoad;		// flag indicates we load the grasses progressed
	bool				m_bForceRebuild;		// force rebuild block each time when needed

	int					m_nGrassCount;			// current grass count for rendering

	A3DWindWaver		m_windWaver;			// wind waver
	A3DVECTOR3			m_vecWindDir;			// wind direction
	APerlinNoise1D		m_wind;					// wind
	float				m_vTime;				// time counter
	A3DVECTOR4			m_vecWindOffset[WIND_BUF_LEN]; // offset caused by the wind
			 
	A3DVertexShader *	m_pGrassVertexShader;	// grass waving vertex shader
	A3DPixelShader *	m_pGrassRenderShader;	// grass render shader

	bool				m_bInteractorEnable;	// flag indicates we enable grass interactor
	bool				m_bIsRenderForRefract;	// flag indicates we are rendering for refract now
	float				m_vRefractSurfaceHeight;// current water surface height
	A3DVertexShader *	m_pRefractVertexShader;	// vertex shader used to do refract rendering

	float				m_vLODLevel;			// grass lod level

	APtrArray<A3DGrassType*>	m_GrassTypes; // the grass types on the land

	A3DStream *			m_pStream;				// stream used to render grasses
	A3DStream *			m_pSoftStream;			// stream used to do software vertex processing

	int					m_nMaxVerts;			// max verts that can be drawn in one pass
	int					m_nMaxFaces;			// max faces that can be drawn in one pass

	float				m_fDNFactor;			// day or night factor
	DWORD				m_dwUpdateStart;		// time that start update
	A3DVECTOR3			m_vHostDelta;
	A3DVECTOR3			m_vHostPos;
	A3DVECTOR3			m_vHostPress;

	unsigned int		m_nGrassVertsBufSize;	// all grass types vertex buffer size
	unsigned int		m_nGrassInstDataSize;	// all grass blocks used instance data size

public:
	inline A3DGrassRender * GetGrassRender() const		{ return m_pGrassRender; }
	inline A3DDevice * GetA3DDevice() const				{ return m_pA3DDevice; }
	inline A3DStream * GetStream() const				{ return m_pStream; }

	inline int GetMaxVerts() const						{ return m_nMaxVerts; }
	inline int GetMaxFaces() const						{ return m_nMaxFaces; }

	inline A3DVertexShader * GetGrassVertexShader()	const { return m_pGrassVertexShader; }
	inline A3DVertexShader * GetCurGrassVertexShader() const
	{
		return m_pGrassVertexShader;
	}

	inline A3DPixelShader * GetGrassRenderShader() const{ return m_pGrassRenderShader; }

	int GetGrassCount() const;
	unsigned int GetGrassVertsBufSize() const	{ return m_nGrassVertsBufSize; }
	void AddGrassVertsBufSize(unsigned int nSize) { m_nGrassVertsBufSize += nSize; }
	unsigned int GetGrassInstDataSize() const	{ return m_nGrassInstDataSize; }
	void AddGrassInstDataSize(unsigned int nSize) { m_nGrassInstDataSize += nSize; }
	void DecGrassInstDataSize(unsigned int nSize) { m_nGrassInstDataSize -= nSize; }

	inline void SetA3DTerrain(A3DTerrain2 * pTerrain)	{ m_pA3DTerrain = pTerrain; }
	inline A3DTerrain2 * GetA3DTerrain() const			{ return m_pA3DTerrain; }

	inline bool GetProgressLoad() const					{ return m_bProgressLoad; }
	inline void SetProgressLoad(bool bFlag)				{ m_bProgressLoad = bFlag; }
	inline bool GetForceRebuild() const					{ return m_bForceRebuild; }
	inline void SetForceRebuild(bool bFlag)				{ m_bForceRebuild = bFlag; }

	inline float GetHeightAt(const A3DVECTOR3& vecPos) const
	{
		return m_pA3DTerrain ? m_pA3DTerrain->GetPosHeight(vecPos) : 0.0f;
	}

	inline bool GetTerrainLMAt(const A3DVECTOR3& vecPos, ARectF *pArea, A3DTexture** ppDayLM, A3DTexture** ppNightLM) const
	{
		if (m_pA3DTerrain)
		{
			return m_pA3DTerrain->GetPosLightMap(vecPos, pArea, ppDayLM, ppNightLM);
		}
		else
		{
			*ppDayLM = NULL;
			*ppNightLM = NULL;
			return false;
		}
	}

	inline float GetLODLevel() const					{ return m_vLODLevel; }
	inline const A3DVECTOR4 * GetWindOffset()	const	{ return m_vecWindOffset; }

	inline DWORD GetUpdateStartTime() const				{ return m_dwUpdateStart; }
	inline A3DGrassInteractor* GetGrassInteractor() const { return m_pGrassInteractor; }

public:		//	Types

public:		//	Constructor and Destructor

	A3DGrassLand();
	~A3DGrassLand();

	bool Init(A3DDevice * pA3DDevice, A3DTerrain2 * pTerrain, ALog * pLogFile);
	bool Release();

	bool Update(const A3DVECTOR3& vecCenter, DWORD dwDeltaTime);
	bool Render(A3DViewport * pViewport, bool bRenderAlpha);
	bool RenderForRefract(A3DViewport* pViewport, float vRefractSurfaceHeight);

	bool Load(const char * szGrassLandFile);
	bool Save(const char * szGrassLandFile);

	bool SetLODLevel(float level);

	// grass type manipulations.
	bool AddGrassType(DWORD dwTypeID, A3DGrassType::GRASSDEFINE_DATA& defData, int nMaxVisibleCount, const char * szGrassFile, A3DGrassType ** pGrassType);
	bool DeleteGrassType(A3DGrassType * pGrassType);

	inline int GetNumGrassType() const				{ return m_GrassTypes.GetSize(); }
	inline A3DGrassType* GetGrassType(int index) const { return m_GrassTypes[index]; }
	inline int GetGrassBlkSize() const;

	A3DGrassType * GetGrassTypeByID(DWORD dwTypeID);

	inline float GetDNFactor() const				{ return m_fDNFactor; }
	void SetDNFactor(float f);
	
	inline bool GetGrassInteractorEnable() const	{ return m_bInteractorEnable; }
	void SetGrassInteractorEnable(bool bEnable)		{ m_bInteractorEnable = bEnable; }

	// Delete all grass types, by jdl
	bool DeleteAllGrassTypes();

	inline float GetTimeCounter() const;
	inline void SetHostPos(A3DVECTOR3& vHostPos);
	inline const A3DVECTOR3& GetHostPos() const;
	inline const A3DVECTOR3& GetHostPosDelta() const;
	inline const A3DVECTOR3& GetHostPress() const;
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////
float A3DGrassLand::GetTimeCounter() const
{
	return m_vTime;
}
void A3DGrassLand::SetHostPos(A3DVECTOR3& vHostPos)
{
	const float fEpsilon = 1e-5f;
	m_vHostDelta = vHostPos - m_vHostPos;
	m_vHostPos = vHostPos;
	if( fabs(m_vHostDelta.x) > fEpsilon && 
		fabs(m_vHostDelta.y) > fEpsilon &&
		fabs(m_vHostDelta.z) > fEpsilon)
	{
		m_vHostPress = m_vHostDelta;
	}
}
const A3DVECTOR3& A3DGrassLand::GetHostPos() const
{
	return m_vHostPos;
}
const A3DVECTOR3& A3DGrassLand::GetHostPosDelta() const
{
	return m_vHostDelta;
}
const A3DVECTOR3& A3DGrassLand::GetHostPress() const
{
	return m_vHostPress;
}

