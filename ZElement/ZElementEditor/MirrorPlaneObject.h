/*
 * FILE: d:\Dev\Projects2.2\SDSElement\SDSElementEditor\MirrorPlaneObject.h
 *
 * DESCRIPTION: Encapsulation of A3DMirrorPlane object for editor.
 *
 * CREATED BY: LiChen, 2012/8/15
 *
 * HISTORY: 
 *
 * 2012/8/15 LiChen : Initial defines the class and functions.
 *
 * Copyright (c) 2012 Perfect World Entertainment Inc, All Rights Reserved.
 */

#if !defined(AFX_MIRRORPLANEOBJECT_H)
#define AFX_MIRRORPLANEOBJECT_H


#define MIRROR_PLANE_OBJ_LITMAP_SIZE_FACTOR 16.0f

#include "SceneObject.h"
#include "ModelContainer.h"
#include "ElementResMan.h"


class A3DMirrorPlane;


class CMirrorPlaneObject : public CSceneObject
{
public:
	CMirrorPlaneObject();
	virtual ~CMirrorPlaneObject();

	virtual void Release();

	// Creates a rendering object. It must be called after adding a mirror plane object into the scene.
	bool BuildRenderData();

	// Deletes the rendering object.
	void DeleteRenderData();

	A3DMirrorPlane* GetMirrorPlane() { return m_pA3DMirrorPlane; }

	virtual A3DVECTOR3 GetMin();
	virtual A3DVECTOR3 GetMax();

	// Rendering
	//virtual void PostRender(A3DViewport* pA3DViewport);
	virtual void Render(A3DViewport* pA3DViewport);
	virtual void RenderForLightMapShadow(A3DViewport* pA3DViewport);
	virtual void RenderForLight(A3DViewport* pA3DViewport, float fOffsetX, float fOffsetZ);

	// Transformation
	virtual void Translate(const A3DVECTOR3& vDelta){ CSceneObject::Translate(vDelta); _UpdateTransformation();}
	virtual void RotateX(float fRad){ CSceneObject::RotateX(fRad); _UpdateTransformation();}
	virtual void RotateY(float fRad){ CSceneObject::RotateY(fRad); _UpdateTransformation();}
	virtual void RotateZ(float fRad){ CSceneObject::RotateZ(fRad); _UpdateTransformation();}
//	virtual void SetRotateMatrix(const A3DMATRIX4& mat) { CSceneObject::SetRotateMatrix(mat); _UpdateTransformation(); }
	virtual void Rotate(const A3DVECTOR3& axis,float fRad){ CSceneObject::Rotate(axis,fRad); _UpdateTransformation();}
	virtual void Scale(float fScale){ CSceneObject::Scale(fScale); _UpdateTransformation();}
//	virtual void SetScaleMatrix(const A3DMATRIX4& mat) { CSceneObject::SetScaleMatrix(mat); _UpdateTransformation(); }
	virtual void SetPos(const A3DVECTOR3& vPos){ CSceneObject::SetPos(vPos); _UpdateTransformation();}
	virtual void ResetRotateMat(){ CSceneObject::ResetRotateMat(); _UpdateTransformation();}
	virtual void SetDirAndUp(const A3DVECTOR3& vdir, const A3DVECTOR3& vu){ CSceneObject::SetDirAndUp(vdir,vu); _UpdateTransformation();}
	virtual void SetScale(const A3DVECTOR3& vs){ CSceneObject::SetScale(vs); _UpdateTransformation();}			

	virtual bool BuildProperty() /*override*/;
	virtual void UpdateProperty(bool bGet);

	// Copy object
	virtual CSceneObject* CopyObject();

	// Load data
	virtual bool Load(CELArchive& ar,DWORD dwVersion,int iLoadFlags);

	// Save data
	virtual bool Save(CELArchive& ar,DWORD dwVersion);

	virtual int	GetTriangleNum();

	bool SetVertexColorDay( const A3DCOLOR* pColors, const unsigned int nNumVertices );
	bool SetVertexColorNight( const A3DCOLOR* pColors, const unsigned int nNumVertices );

	inline const AArray<A3DCOLOR>& GetVertexColorDay() const	{ return m_ArrayDayColors; }
	inline const AArray<A3DCOLOR>& GetVertexColorNight() const	{ return m_ArrayNightColors; }

	bool UpdateStream();

	float GetPlaneWidth() const			{ return m_fPlaneWidth; }
	float GetPlaneHeight() const		{ return m_fPlaneHeight; }
	UINT GetNumSegmentsX() const		{ return m_nNumSegmentsX; }
	UINT GetNumSegmentsY() const		{ return m_nNumSegmentsY; }
	float GetAlphaMaskValue() const		{ return m_fAlphaMaskValue; }
	float GetUVOffsetMultipler() const	{ return m_fUVOffsetMultipler; }
	float GetFakeReflectPower() const	{ return m_fFakeReflectPower; }
	const AString& GetAlphaMaskTexture() const		{ return m_strAlphaMaskTextureName; }
	const AString& GetUVNoiseTexture() const		{ return m_strUVNoiseTextureName; }
	const AString& GetFakeReflectionTexture() const	{ return m_strFakeReflectionTextureName; }
	int GetLitBakingMode() const		{ return m_nLitBakingMode; }
	AString GetLightMapDay() const		{ return m_strLightMapTextureDayName; }
	AString GetLightMapNight() const	{ return m_strLightMapTextureNightName; }
	int GetAutoLightmapSize() const		{ return m_nAutoLightmapSize; }
	int GetCustomLightmapSize() const	{ return m_nCustomLightmapSize; }
	float GetLitDirectScale() const		{ return m_fLitDirectScale; }
	float GetLitIndirectScale() const	{ return m_fLitIndirectScale; }
	float GetLitAOScale() const			{ return m_fLitAOScale; }
	const AArray<A3DCOLOR>& GetDayColors() const	{ return m_ArrayDayColors; }
	const AArray<A3DCOLOR>& GetNightColors() const	{ return m_ArrayNightColors; }

	// Automatically calculates the suitable light map size by its area.
	int GetAdaptLightmapSize( float fScale ) const;

	// Sets light map texture.
	bool SetDayLightMap( const char* pszLightMapName );
	bool SetNightLightMap( const char* pszLightMapName );

	// Outputs all necessary geometry data for baking lighting information into a file.
	bool OutputBakingGeometryFile( const char* pszFileName );

	bool IsIntersectionWithArea(const A3DVECTOR3& vMin, const A3DVECTOR3& vMax);

protected:
	void _UpdateTransformation();

protected:
	A3DMirrorPlane*				m_pA3DMirrorPlane;

	float						m_fPlaneWidth;					// Plane size along X axis
	float						m_fPlaneHeight;					// Plane size along Z axis

	unsigned int				m_nNumSegmentsX;				// Number of segments of the mesh along X axis
	unsigned int				m_nNumSegmentsY;				// Number of segments of the mesh along Y axis

	float						m_fAlphaMaskValue;				// The comparison value for alpha mask. ( If the alpha value of given pixel on the alpha mask texture less than this value, it will be clipped. )
	float						m_fUVOffsetMultipler;			// UV offset multipler applied to noise UV
	float						m_fFakeReflectPower;			// Power of fake reflection.

	AString						m_strAlphaMaskTextureName;		// Name of alpha mask texture.
	AString						m_strUVNoiseTextureName;
	AString						m_strFakeReflectionTextureName;

	// Light map properties
	int							m_nLitBakingMode;				// Indicates whether this object use pre-baked vertex color or light maps.
	ASetTemplate<int>			m_LitBakingModeSet;

	AString						m_strLightMapTextureDayName;	// Name of light map texture used in the day
	AString						m_strLightMapTextureNightName;	// Name of light map texture used in the night

	AArray<A3DCOLOR>			m_ArrayDayColors;
	AArray<A3DCOLOR>			m_ArrayNightColors;

	int							m_nAutoLightmapSize;
	int     					m_nCustomLightmapSize;
	float						m_fLitDirectScale;
	float						m_fLitIndirectScale;
	float						m_fLitAOScale;

	friend struct MIRROR_PLANE_LIGHTMAP_SIZE_FUNCS;
	MIRROR_PLANE_LIGHTMAP_SIZE_FUNCS*	m_pLightMapSizeFuns;
};


struct MIRROR_PLANE_LIGHTMAP_SIZE_FUNCS : public CUSTOM_FUNCS
{
	MIRROR_PLANE_LIGHTMAP_SIZE_FUNCS( CMirrorPlaneObject* pOwner ) : m_pOwner(pOwner) {}

	virtual BOOL CALLBACK OnActivate(void);
	virtual LPCTSTR CALLBACK OnGetShowString(void) const;
	virtual AVariant CALLBACK OnGetValue(void) const;
	virtual void CALLBACK OnSetValue(const AVariant& var);

	CMirrorPlaneObject*	m_pOwner;
	AVariant			m_var;
	CString				m_strText;
};


#endif // !defined(AFX_MIRRORPLANEOBJECT_H)
