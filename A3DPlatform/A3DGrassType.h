/*
 * FILE: EL_GrassType.h
 *
 * DESCRIPTION: Grass Type class for element
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
#include "ALog.h"
#include "A3DTerrain2.h"
#include "vector.h"

///////////////////////////////////////////////////////////////////////  
//	Grass Vertex Formats
///////////////////////////////////////////////////////////////////////
#define GRASS_FVF_VERTEX		D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1

/////////////////////////////////////////////////////////////////////// 
// Grass Vertex Structure
///////////////////////////////////////////////////////////////////////
struct GRASSVERTEX
{
	GRASSVERTEX() {}
	GRASSVERTEX(const A3DVECTOR3& _pos, const A3DVECTOR3& _normal, float _u, float _v)
	{
		pos = _pos;
		normal = _normal;
		u = _u;
		v = _v;
	}

	A3DVECTOR3			pos;
	A3DVECTOR3			normal;	// we put wind index and vertex weight in this struct (x: wind index, y: vertex weight)
	float				u;		// u, v coords
	float				v;
};

struct A3DGRASSVERTEX
{
	A3DVECTOR3	pos;
	A3DVECTOR3	uvw;	//	xy: base map uv, z: wind parameter
};

//	per grass instance data
struct A3DGRASSINSTANCEDATA
{
	A3DVECTOR3	worldPos;
	A3DVECTOR3	scaleRot;
	A3DVECTOR3	offset;	//	xz: offset, y: used as a flag dedicating that the grass is free or pressed
};

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

class A3DDevice;
class A3DEngine;
class A3DViewport;
class A3DStream;
class A3DTexture;
class A3DGrassLand;
class A3DGrassBitsMap;
class AFile;


struct GRASSCLUSTER
{
	A3DAABB    aabb;
	A3DVECTOR3 vDir;
	float      offset;
	DWORD      dwFlag;
};

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class A3DGrassType
//	
///////////////////////////////////////////////////////////////////////////

class A3DGrassType
{
public:		//	Types
	struct GRASSDEFINE_DATA
	{
		bool	bAlphaBlendEnable;		// flag indicates whether this kind of grass use alpha blend
		bool	bRandomPos;				// random generate grass around the specified position
		int		nAlphaRefValue;			// if not using alpha blend, we use alpha compare, this is the reference alpha value (0~255)

		// size section
		float	vSize;					// size scale of this type of grass
		float	vSizeVar;				// size varation (0.0~1.0)

		// softness
		float	vSoftness;				// softness for wind effect 0.0 means very hard to blow, 2.0 means too soft to blow (0.0~2.0)

		// sight range
		float	vSightRange;			// sight range of this kind of grass

		float	foo[8];					// for future expanding
	};

private:
	A3DDevice *						m_pA3DDevice;		// a3d device
	ALog *							m_pLogFile;			// log file to output messages
	A3DGrassLand *					m_pGrassLand;		// grassland managing class	object

	bool							m_bBadGrass;		// flag indicates grass file missing from disk

	DWORD							m_dwTypeID;			// id of this type of tree

	char							m_szGrassFile[256];	// grass file name

	GRASSDEFINE_DATA				m_GrassDefineData;	// grass define data

	// render data section
	char							m_szGrassMap[MAX_PATH];	// grass texture map
	A3DTexture *					m_pTextureGrass;	// grass texture

	abase::vector<A3DGrassBitsMap*>		m_GrassBitsMaps;

	// data for construst a specified grass
	int								m_nNumOneGrassVert;	// number of verts for one grass
	int								m_nNumOneGrassFace;	// number of faces for one grass
	GRASSVERTEX *					m_pVertsTemplate;	// the vertex position for one grass
	WORD *							m_pIndicesTemplate;	// the indices for one grass

	DWORD							m_dwIDNext;			// next grass map's id
	A3DStream*						m_pStreamTemplate;	// stream template
	float							m_fGrassHeight;		// grass height

public:
	inline A3DGrassLand * GetGrassLand() const	{ return m_pGrassLand; }

	inline int GetNumOneGrassVert()	const		{ return m_nNumOneGrassVert; }
	inline int GetNumOneGrassFace() const		{ return m_nNumOneGrassFace; }
	inline GRASSVERTEX * GetVertsTemplate()	const { return m_pVertsTemplate; }
	inline WORD * GetIndicesTemplate() const	{ return m_pIndicesTemplate; }

	inline const GRASSDEFINE_DATA& GetDefineData() const			{ return m_GrassDefineData; }
	
	inline const char * GetGrassFile() const	{ return m_szGrassFile; }
	inline bool IsBadGrass() const				{ return m_bBadGrass; }

	inline const char * GetGrassMap() const		{ return m_szGrassMap; }
	inline A3DTexture * GetTextureGrass() const { return m_pTextureGrass; }
	inline A3DStream * GetStreamTemplate() const { return m_pStreamTemplate; }

	inline float GetGrassHeight() const			{ return m_fGrassHeight; }

protected:
	DWORD GenerateID() { return m_dwIDNext ++; }

	// rebuild all grasses that are rendering now, you should call this function
	// after you have adjust this grass type's information
	bool RebuildAllGrasses(A3DViewport * pViewport);

public:	
	A3DGrassType();
	~A3DGrassType();

public:		//	Operations

	// initialize and finalize
	bool Init(A3DDevice * pA3DDevice, A3DGrassLand * pGrassLand, DWORD dwTypeID, int nMaxVisibleCount, const char * szGrassFile, ALog * pLogFile, GRASSDEFINE_DATA * pDefineData);
	bool Release();
	
	// load and save
	bool Load(AFile * pFileToLoad);
	bool Save(AFile * pFileToSave);

	// update methods
	bool Update(const A3DVECTOR3& vecCenter, float fDeltaTime);
	
	// Rendering methods
	bool RenderGrasses(A3DViewport * pViewport);

	// grass map management
	// add one bits map into this type grass and return the id in dwID
	bool AddGrassBitsMap(BYTE * pBitsMap, const A3DVECTOR3& vecCenter, float vGridSize, int nWidth, int nHeight, DWORD& dwID);
	// Get a grass bits map according to the id
	A3DGrassBitsMap * GetGrassBitsMap(DWORD dwID);
	// remove one tree from this type tree
	bool DeleteGrassBitsMap(DWORD dwID);
	
	// Adjust the grass type's define data
	bool AdjustDefineData(A3DViewport * pViewport, const GRASSDEFINE_DATA& data);
	// Adjust the max visible count for this grass type
	bool AdjustMaxVisibleCount(A3DViewport * pViewport, int nMaxVisibleCount);

	// Adjust the grass's lod level
	bool AdjustForLODLevel();
	
	inline DWORD GetTypeID() { return m_dwTypeID; }
	inline int GetGrassBitsMapCount() { return (int)m_GrassBitsMaps.size(); }
	inline A3DGrassBitsMap* GetGrassBitsMapByIndex(int n) { return m_GrassBitsMaps[n]; }

protected:	//	Attributes
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////

