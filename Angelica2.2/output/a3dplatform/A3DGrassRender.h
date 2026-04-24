/*
* FILE: A3DGrassRender.h
*
* DESCRIPTION: 
*
* CREATED BY: zhangyachuan, 2010/10/14
*
* HISTORY:
*
* Copyright (c) 2010 Archosaur Studio, All Rights Reserved.	
*/

#ifndef _A3DGrassRender_H_
#define _A3DGrassRender_H_

#include "A3DVector.h"

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

class A3DEngine;
class A3DDevice;
class A3DViewport;
class A3DGrassType;
class A3DGrassLand;
class A3DGrassBlock;
class A3DGrassBitsMap;
class A3DVertexDecl;
class A3DHLSL;
class ARandomGen;

///////////////////////////////////////////////////////////////////////////
//
//	Declare of Global functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Class A3DGrassRender
//
///////////////////////////////////////////////////////////////////////////

class A3DGrassRender
{
public:		//	Types

	enum RENDER_TYPE
	{
		RT_COLLECT		= 0,
		RT_HWINSTANCE,
		RT_NUM,
	};

public:		//	Constructors and Destructor

	A3DGrassRender(RENDER_TYPE rt);
	virtual ~A3DGrassRender();

public:		//	Attributes

public:		//	Operations

	static A3DGrassRender* CreateRender(A3DDevice* pA3DDevice);

	virtual bool Init(A3DGrassLand* pGrassLand);
	virtual void Release();

	//	Build grass block render data
	virtual A3DGrassBlock* CreateBlock() = 0;
	virtual bool DestroyBlock(A3DGrassBlock* pBlock);

	virtual bool Render(A3DViewport* pViewport, bool bAlphaBlend);
	//	Render dispatch function used when A3DGrassBitsMap render
	virtual bool DoRender(A3DViewport* pViewport, A3DGrassBitsMap* pGrassBitsMap) = 0;

	inline int GetGrassCount() const { return m_nGrassCount; }
	inline int GetGrassBlockSize() const { return m_nGrassBlkSize; }
	RENDER_TYPE GetRenderType() const { return m_Type; }

private:	//	Attributes

	A3DGrassLand* m_pGrassLand;
	A3DHLSL* m_pHLSL;
	A3DEngine* m_pA3DEngine;
	A3DDevice* m_pA3DDevice;
	A3DVertexDecl* m_pVertexDecl;
	RENDER_TYPE m_Type;
	int m_nGrassCount;
	int m_nGrassBlkSize;

protected:	//	Operations

	virtual const D3DVERTEXELEMENT9* GetD3DVertexElement() const = 0;
	inline void SetGrassBlkSizeBig() { m_nGrassBlkSize = 64; }
	inline void AddGrassCount(int nCount) { m_nGrassCount += nCount; }
	inline A3DGrassLand* GetGrassLand() const { return m_pGrassLand; }
	inline A3DEngine* GetA3DEngine() const { return m_pA3DEngine; }
	inline A3DDevice* GetA3DDevice() const { return m_pA3DDevice; }
	inline A3DHLSL* GetHLSL() const { return m_pHLSL; }
	bool LoadHLSL(const char* szFile);
	bool SetupHLSLParamsForBitsMap(A3DGrassBitsMap* pGrassBitsMap);

	bool RawRender(int iNumVert, int iNumFace);
	void ResetGrassCount() { m_nGrassCount = 0; }
};

///////////////////////////////////////////////////////////////////////////
//
//	Class A3DGrassCollectRender
//
///////////////////////////////////////////////////////////////////////////

class A3DGrassCollectRender : public A3DGrassRender
{
public:		//	Types

	typedef A3DGrassRender base_class;

public:		//	Constructors and Destructor

	A3DGrassCollectRender();
	virtual ~A3DGrassCollectRender();

public:		//	Attributes

public:		//	Operations

	virtual bool Init(A3DGrassLand* pGrassLand);
	virtual void Release();

	//	Render dispatch function used when A3DGrassBitsMap render
	virtual bool DoRender(A3DViewport* pViewport, A3DGrassBitsMap* pGrassBitsMap);

protected:	//	Attributes

protected:	//	Operations

	virtual const D3DVERTEXELEMENT9* GetD3DVertexElement() const;
	virtual A3DGrassBlock* CreateBlock();
};

///////////////////////////////////////////////////////////////////////////
//
//	Class A3DGrassHWInstanceRender
//
///////////////////////////////////////////////////////////////////////////

class A3DGrassHWInstanceRender : public A3DGrassRender
{
public:		//	Types

	typedef A3DGrassRender base_class;

public:		//	Constructors and Destructor

	A3DGrassHWInstanceRender();
	virtual ~A3DGrassHWInstanceRender();

public:		//	Attributes

public:		//	Operations

	virtual bool Init(A3DGrassLand* pGrassLand);
	virtual void Release();

	//	Render dispatch function used when A3DGrassBitsMap render
	virtual bool DoRender(A3DViewport* pViewport, A3DGrassBitsMap* pGrassBitsMap);

private:	//	Attributes

private:	//	Operations

	virtual const D3DVERTEXELEMENT9* GetD3DVertexElement() const;
	virtual A3DGrassBlock* CreateBlock();

	bool InternalRender(A3DViewport* pViewport, A3DGrassBitsMap* pGrassBitsMap);

};

///////////////////////////////////////////////////////////////////////////
//
//	Class A3DGrassRender
//
///////////////////////////////////////////////////////////////////////////



#endif	//	_A3DGrassRender_H_
