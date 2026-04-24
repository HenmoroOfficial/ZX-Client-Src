/*
 * FILE: A3DDistantObject.h
 *
 * DESCRIPTION: Class for distant scenery objects
 *
 * CREATED BY: Yaochunhui, 2012/10/23
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
 */

#ifndef _A3DDISTANTOBJECT_H_
#define _A3DDISTANTOBJECT_H_

#include "A3DVertex.h"
#include "A3DGeometry.h"
#include "AString.h"

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

class A3DDevice;
class A3DCameraBase;
class A3DViewport;
class A3DTexture;
class A3DStream;
class AFile;

///////////////////////////////////////////////////////////////////////////
//
//	Declare of Global functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Interface Class A3DDistantObject
//
///////////////////////////////////////////////////////////////////////////

class A3DDistantObject
{
public:		
    //	Types of object
    enum ObjectType
    {
        TYPE_BOARD
    };

public:		//	Constructors and Destructors

    A3DDistantObject();

    virtual ~A3DDistantObject();

public:		//	Attributes

public:		//	Operations
    
    virtual bool Init(A3DDevice* pA3DDevice)    = 0;
    virtual void Release()                      = 0;		//	Release

    virtual A3DDistantObject* Clone()           = 0;
    virtual ObjectType GetType() const          = 0;

    virtual bool TickAnimation(DWORD dwDeltaTime)   { return true; }
    virtual bool Render(A3DViewport* pViewport)     { return true; }
    virtual bool RayTrace(const A3DVECTOR3& vStart, const A3DVECTOR3& vVel, float* pfFrac) { return false; }

    A3DVECTOR3	GetPos() const              {	return m_vPos;      }
    float   	GetScale() const            {	return m_fScale;	}
    A3DVECTOR3	GetDir() const				{	return m_vDir;		}
    A3DVECTOR3	GetUp() const				{	return m_vUp;		}

    void        SetPos(const A3DVECTOR3& vPos)	{   m_vPos = vPos;	UpdateTransform();	}
    void		SetScale(float fScale)          {	m_fScale = fScale; 	UpdateTransform();	}
    void		SetDirAndUp(const A3DVECTOR3& vDir, const A3DVECTOR3& vUp);
    
    const A3DAABB& GetAABB() const  {   return m_AABB;      }
    const A3DMATRIX4& GetAbsTransform() const   {   return m_matTransform;  }

    virtual bool Load(AFile* pFile) = 0;
    virtual bool Save(AFile* pFile) = 0;


protected:	//	Attributes
    A3DVECTOR3	m_vPos;
    A3DVECTOR3	m_vDir;
    A3DVECTOR3	m_vUp;
    float       m_fScale;
    A3DAABB     m_AABB;
    A3DMATRIX4  m_matTransform;

    bool CopyFrom(A3DDistantObject* pOther);
    virtual void UpdateTransform();

};

///////////////////////////////////////////////////////////////////////////
//
//	Class A3DDistantBoard
//
///////////////////////////////////////////////////////////////////////////
struct A3DBOARDVERTEX
{
    A3DVECTOR3			pos;
    A3DVECTOR3			normal;
    float				u;
    float				v;
    A3DVECTOR4			tangents;
};



class A3DDistantBoard : public A3DDistantObject
{
public:		//	Constructors and Destructors

    A3DDistantBoard();
    virtual ~A3DDistantBoard();

public:		//	Attributes
    enum CurveType
    {
        CURVE_X, CURVE_Y
    };

public:		//	Operations

    virtual bool Init(A3DDevice* pA3DDevice);
    virtual void Release();		            

    virtual A3DDistantObject* Clone();
    virtual A3DDistantObject::ObjectType GetType() const { return A3DDistantObject::TYPE_BOARD; }

    virtual bool TickAnimation(DWORD dwDeltaTime);
    virtual bool Render(A3DViewport* pViewport);
    virtual bool RenderAtOnce();
    virtual bool RayTrace(const A3DVECTOR3& vStart,  const A3DVECTOR3& vVel, float* pfFrac);

    virtual void UpdateTransform();

    const A3DVECTOR3& GetSize() const { return m_vSize; }
    A3DVECTOR3& Size() { return m_vSize; }

    A3DTexture* GetDiffuseTexture() const { return m_pDiffuseTexture; }
    A3DTexture* GetSpecularTexture() const { return m_pSpecularTexture; }
    A3DTexture* GetNormalTexture() const { return m_pNormalTexture; }
    A3DTexture* GetShadowTexture() const { return m_pShadowTexture; }

    bool SetDiffuseTexture(const char* szTextureFile);
    bool SetSpecularTexture(const char* szTextureFile);
    bool SetNormalTexture(const char* szTextureFile);
    bool SetShadowTexture(A3DTexture* pTexture);

    A3DStream* GetStream() const { return m_pStream; }
    
    int GetRows() const { return m_nGridRows; }
    int GetCols() const { return m_nGridCols; }

    void SetGrid(int nRows, int nCols);
    bool        CreateStream();

    // ÇúÂÊ
    float GetTortuosity() const { return m_fTortuosity; }
    void SetTortuosity(float fTuot) { m_fTortuosity = fTuot; } 

    CurveType GetCurveType() const { return m_nCurveType; }
    void SetCurveType(CurveType curve) { m_nCurveType = curve; }

    const A3DVECTOR4& GetSpecular() const { return m_vSpecular; }
    void SetSpecular(const A3DVECTOR4& spec) { m_vSpecular = spec; }

    const A3DAABB& GetOBB() const { return m_OBB; }

    virtual bool LoadShadowMap(AFile* pFile);
    virtual bool SaveShadowMap(AFile* pFile);

    virtual bool Load(AFile* pFile);
    virtual bool Save(AFile* pFile);

protected:	//	Attributes

    A3DDevice*  m_pA3DDevice;
    A3DVECTOR3  m_vSize;
    A3DTexture* m_pDiffuseTexture;
    A3DTexture* m_pNormalTexture;
    A3DTexture* m_pSpecularTexture;
    A3DTexture* m_pShadowTexture;
    AString     m_strDiffuseTexture;
    AString     m_strNormalTexture;
    AString     m_strSpecularTexture;
    A3DStream*  m_pStream;
    int         m_nGridRows;
    int         m_nGridCols;
    
    float       m_fTortuosity;
    CurveType   m_nCurveType;
    A3DVECTOR4  m_vSpecular;
    A3DAABB     m_OBB;

    A3DTexture* LoadTexture(const char* szTextureFile);
    
};

#endif