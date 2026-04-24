/*
 * FILE: d:\Dev\Projects2.2\A3DPlatform\A3DMirrorPlane.h
 *
 * DESCRIPTION: A mirror plane which can reflect the scene.
 *
 * CREATED BY: LiChen, 2012/8/9
 *
 * HISTORY: 
 *
 * Copyright (c) 2012 Perfect World Entertainment Inc, All Rights Reserved.
 */

#ifndef _A3D_MIRROR_PLANE_H_
#define _A3D_MIRROR_PLANE_H_


#include "AArray.h"
#include "A3DVector.h"
#include "A3DGeometry.h"
#include "A3DCoordinate.h"


class AFile;
class A3DDevice;
class A3DStream;
class A3DTexture;
class A3DViewport;
class ElementRender;


///////////////////////////////////////////////////////////////////////////
//	
//	Class A3DMirrorPlane
//	
///////////////////////////////////////////////////////////////////////////


class A3DMirrorPlane
{
public:
	friend class A3DMirrorPlaneRender;

	static const unsigned int	FILE_VERSION_10		= 0x10000000;
	static const unsigned int	FILE_VERSION_11		= 0x10000001;
	static const unsigned int	FILE_VERSION_12		= 0x10000002;
	static const unsigned int	FILE_VERSION_13		= 0x10000002;
	static const unsigned int	FILE_VERSION		= FILE_VERSION_13;

	static const char			DEFAULT_UV_NOISE_TEXTURE[];
	static const char			DEFAULT_FAKE_REFLECTION_TEXTURE[];

	// Mirror plane mode
	enum MODE
	{
		MODE_VERTEX_COLOR,
		MODE_LIGHT_MAP,
	};

	// Vertex format for mirror plane
	struct Vertex
	{
		A3DVECTOR3		vPos;
		A3DVECTOR3		vNormal;

		A3DCOLOR		DiffuseDay;			// Pre-baked diffuse lighting color in the day
		A3DCOLOR		DiffuseNight;		// Pre-baked diffuse lighting color in the night

		float			fU;					// UV for sampling custom texture
		float			fV;

		float			fLightMapU;			// UV for sampling light map texture
		float			fLightMapV;
	};

public:
	A3DMirrorPlane();
	explicit A3DMirrorPlane( A3DMirrorPlane& Other );
	~A3DMirrorPlane();

	// Creates a mirror plane, only generates the mesh with white vertex color, no light map used.
	bool Init( A3DMirrorPlaneRender* pMirrorPlaneRender, const MODE eMode, const float fWidth, const float fHeight, const unsigned int nNumSegmentsX = 1, const unsigned int nNumSegmentsY = 1 );
	void Release();

	void SetMode( const MODE eMode );
	inline MODE GetMode() const;

	inline void SetVisiblity( const bool bVisiblity );
	inline bool GetVisiblity() const;

	void SetPos( const A3DVECTOR3& vPos );
	void SetDirAndUp( const A3DVECTOR3& vDir, const A3DVECTOR3& vUp );
	void SetScaling( const A3DVECTOR3& vScaling );

	inline const A3DVECTOR3& GetPos();
	inline const A3DVECTOR3& GetDir();
	inline const A3DVECTOR3& GetUp();
	inline const A3DVECTOR3& GetScaling();
	inline const A3DMATRIX4& GetWorldMatrix();

	// Sets the size of plane. It will reset the vertex color and light map UV.
	void SetSize( const float fWidth, const float fHeight );

	inline float GetWidth() const;
	inline float GetHeight() const;

	// Sets the number of segments of plane mesh. It will reset the vertex color and light map UV. ( Sets the number of segments greater than 1 is not allowed when light maps are enabled. )
	bool SetSegments( const unsigned int nNumSegmentsX, const unsigned int nNumSegmentsY );

	inline unsigned int GetNumSegmentsX() const;
	inline unsigned int GetNumSegmentsZ() const;

	// Gets reflection 3D plane.
	A3DPLANE GetReflectionPlane() const;

	// Alpha mask texture manipulators.
	void SetUseAlphaMaskTexture( const bool bEnable );
	inline bool GetUseAlphaMaskTexture() const;

	bool SetAlphaMaskTexture( const char* pszAlphaMaskTextureName );
	inline const AString& GetAlphaMaskTextureName() const;

	inline void SetAlphaMaskValue( const float fAlphaMaskValue );
	inline float GetAlphaMaskValue() const;

	inline void SetUVOffsetMultipler( const float fUVOffsetMultipler );
	inline float GetUVOffsetMultipler() const;

	inline void SetFakeReflectionPower( const float fFakeReflectionPower );
	inline float GetFakeReflectionPower() const;

	void ReleaseAlphaMaskTexture();
	bool LoadAlphaMaskTexture();

	bool SetUVNoiseTexture( const char* pszUVNoiseTextureName );
	bool SetFakeReflectionTexture( const char* pszFakeReflectionName );

	// Light map manipulators. ( To enable light maps will reset the number of segments to 1. )
	inline A3DTexture* GetDayLightMap();
	inline A3DTexture* GetNightLightMap();
	inline const AString& GetDayLightMapName() const;
	inline const AString& GetNightLightMapName() const;

	bool SetDayLightMap( const char* pszLightMapName );
	bool SetNightLightMap( const char* pszLightMapName );

	// Functions to get and set vertex data.
	inline unsigned int GetNumVertices() const;
	inline unsigned int GetNumFaces() const;

	inline Vertex* GetVertices();
	inline unsigned short* GetIndices();

	// Updates vertices and indices data to stream.
	void UpdateStream();

	// Gets the bounding box.
	inline const A3DAABB& GetAABB();

	// Calculates the bounding box by parameters.
	static void CalculateAABB( A3DAABB& OutAABB, const float fWidth, const float fHeight, const A3DMATRIX4& WorldMatrix );

	// Load and Save
	bool Save( const char* pszFileName );
	bool Load( A3DMirrorPlaneRender* pMirrorPlaneRender, const char* pszFileName );

	bool Save( AFile* pFileToSave );
	bool Load( A3DMirrorPlaneRender* pMirrorPlaneRender, AFile* pFileToLoad );

	// Performs view frustum culling and commits this object to renderer.
	bool Render( A3DViewport* pViewport );

private:
	// Recalculates the world matrix by transformation vectors.
	void _UpdateWorldMatrix();

	// Generates vertices and indices of plane mesh.
	bool _GenerateMeshData();

	// Clears vertices and indices of plane mesh.
	void _ClearMeshData();

	void _ReleaseLightMap( bool bDay = true, bool bNight = true );

	bool _LoadLightMap( bool bDay = true, bool bNight = true );

	// Creates all device resources used to render the mirror plane mesh.
	bool _CreateDeviceResources();

	// Release all device resources used to render the mirror plane mesh.
	void _ReleaseDeviceResources();

	// Loads light map texture, uses error light map texture instead if failed.
	bool _LoadLightMapTexture( const AString& strLightMapTextureName, A3DTexture** ppTexture );

	bool _LoadUVNoiseTexture();
	bool _LoadFakeReflectionTexture();

	// Recalculates the bounding box after transformation was changed.
	void _UpdateAABB();

protected:
	A3DMirrorPlaneRender*			m_pMirrorPlaneRender;

	A3DMATRIX4						m_WorldMatrix;					// The matrix for world space transformation.
	A3DVECTOR3						m_vPos;							// Translation
	A3DVECTOR3						m_vDir;							// +Z of rotation matrix. 
	A3DVECTOR3						m_vUp;							// +Y of rotation matrix.
	A3DVECTOR3						m_vScaling;						// Scaling

	A3DAABB							m_AABB;							// Bounding box.

	bool							m_bVisiblity;					// Visibility
	MODE							m_eMode;						// Indicates whether this mirror plane use light maps or vertex color.

	A3DStream*						m_pStream;						// Vertex stream

	float							m_fAlphaMaskValue;				// The comparison value for alpha mask. ( If the alpha value of given pixel on the alpha mask texture less than this value, it will be clipped. )
	float							m_fUVOffsetMultipler;			// UV offset multipler applied to noise UV
	float							m_fFakeReflectPower;			// Power of fake reflection.

	AString							m_strLightMapTextureDayName;	// Name of light map texture used in the day
	AString							m_strLightMapTextureNightName;	// Name of light map texture used in the night

	AString							m_strAlphaMaskTextureName;		// Name of alpha mask texture.
	AString							m_strUVNoiseTextureName;
	AString							m_strFakeReflectionTextureName;

	A3DTexture*						m_pLightMapTextureDay;			// Light map texture used in the day
	A3DTexture*						m_pLightMapTextureNight;		// Light map texture used in the night

	A3DTexture*						m_pAlphaMaskTexture;			// Texture used to mask the mirror plane by its alpha channel.
	A3DTexture*						m_pUVNoiseTexture;				// Noise map to offset reflection sampling UV
	A3DTexture*						m_pFakeReflectionTexture;		// A fake reflection texture provided by artist.

	float							m_fPlaneWidth;					// Plane size along X axis
	float							m_fPlaneHeight;					// Plane size along Z axis

	unsigned int					m_nNumSegmentsX;				// Number of segments of the mesh along X axis
	unsigned int					m_nNumSegmentsY;				// Number of segments of the mesh along Y axis

	AArray< Vertex >				m_ArrayVertices;				// Vertex data of mesh
	AArray< unsigned short >		m_ArrayIndices;					// Index data of mesh
};


///////////////////////////////////////////////////////////////////////////
//	
//	Class A3DMirrorPlane inline functions
//	
///////////////////////////////////////////////////////////////////////////


inline A3DMirrorPlane::MODE A3DMirrorPlane::GetMode() const
{
	return m_eMode;
}

inline void A3DMirrorPlane::SetVisiblity( const bool bVisiblity )
{
	m_bVisiblity = bVisiblity;
}

inline bool A3DMirrorPlane::GetVisiblity() const
{
	return m_bVisiblity;
}

inline const A3DVECTOR3& A3DMirrorPlane::GetPos()
{
	return m_vPos;
}

inline const A3DVECTOR3& A3DMirrorPlane::GetDir()
{
	return m_vDir;
}

inline const A3DVECTOR3& A3DMirrorPlane::GetUp()
{
	return m_vUp;
}

inline const A3DVECTOR3& A3DMirrorPlane::GetScaling()
{
	return m_vScaling;
}

inline const A3DMATRIX4& A3DMirrorPlane::GetWorldMatrix()
{
	return m_WorldMatrix;
}

inline float A3DMirrorPlane::GetWidth() const
{
	return m_fPlaneWidth;
}

inline float A3DMirrorPlane::GetHeight() const
{
	return m_fPlaneHeight;
}

inline unsigned int A3DMirrorPlane::GetNumSegmentsX() const
{
	return m_nNumSegmentsX;
}

inline unsigned int A3DMirrorPlane::GetNumSegmentsZ() const
{
	return m_nNumSegmentsY;
}

inline const AString& A3DMirrorPlane::GetAlphaMaskTextureName() const
{
	return m_strAlphaMaskTextureName;
}

inline void A3DMirrorPlane::SetAlphaMaskValue( const float fAlphaMaskValue )
{
	m_fAlphaMaskValue = fAlphaMaskValue;
}

inline float A3DMirrorPlane::GetAlphaMaskValue() const
{
	return m_fAlphaMaskValue;
}

inline void A3DMirrorPlane::SetUVOffsetMultipler( const float fUVOffsetMultipler )
{
	m_fUVOffsetMultipler = fUVOffsetMultipler;
}

inline float A3DMirrorPlane::GetUVOffsetMultipler() const
{
	return m_fUVOffsetMultipler;
}

inline void A3DMirrorPlane::SetFakeReflectionPower( const float fFakeReflectionPower )
{
	m_fFakeReflectPower = fFakeReflectionPower;
}

inline float A3DMirrorPlane::GetFakeReflectionPower() const
{
	return m_fFakeReflectPower;
}

inline A3DTexture* A3DMirrorPlane::GetDayLightMap()
{
	return m_pLightMapTextureDay;
}

inline A3DTexture* A3DMirrorPlane::GetNightLightMap()
{
	return m_pLightMapTextureNight;
}

inline const AString& A3DMirrorPlane::GetDayLightMapName() const
{
	return m_strLightMapTextureDayName;
}

inline const AString& A3DMirrorPlane::GetNightLightMapName() const
{
	return m_strLightMapTextureNightName;
}

inline unsigned int A3DMirrorPlane::GetNumVertices() const
{
	return (unsigned int)m_ArrayVertices.GetSize();
}

inline unsigned int A3DMirrorPlane::GetNumFaces() const
{
	return (unsigned int)m_ArrayIndices.GetSize() / 3;
}

inline A3DMirrorPlane::Vertex* A3DMirrorPlane::GetVertices()
{
	return m_ArrayVertices.GetData();
}

inline unsigned short* A3DMirrorPlane::GetIndices()
{
	return m_ArrayIndices.GetData();
}

inline const A3DAABB& A3DMirrorPlane::GetAABB()
{
	_UpdateAABB();
	return m_AABB;
}


#endif // #ifndef _A3D_MIRROR_PLANE_H_
