/*
 * FILE: d:\Dev\Projects2.2\A3DPlatform\A3DMirrorPlane.cpp
 *
 * DESCRIPTION: A mirror plane which can reflect the scene.
 *
 * CREATED BY: LiChen, 2012/8/9
 *
 * HISTORY: 
 *
 * Copyright (c) 2012 Perfect World Entertainment Inc, All Rights Reserved.
 */

#include "ABaseDef.h"
#include "A3DPI.h"
#include "A3DFuncs.h"
#include "AFile.h"
#include "A3DMirrorPlane.h"
#include "A3DMirrorPlaneRender.h"
#include "A3DDevice.h"
#include "A3DStream.h"
#include "A3DFrame.h"
#include "A3DLitModel.h"
#include "A3DTextureMan.h"
#include "A3DEngine.h"
#include "A3DMacros.h"
#include "A3DViewport.h"
#include "A3DCamera.h"



///////////////////////////////////////////////////////////////////////////
//	
//	Class A3DMirrorPlane
//	
///////////////////////////////////////////////////////////////////////////


const char			A3DMirrorPlane::DEFAULT_UV_NOISE_TEXTURE[]				= "Shaders\\Textures\\Common\\FlatNormal.dds";
const char			A3DMirrorPlane::DEFAULT_FAKE_REFLECTION_TEXTURE[]		= "Shaders\\Textures\\Common\\Black.dds";


A3DMirrorPlane::A3DMirrorPlane()
	: m_pMirrorPlaneRender		(NULL)
	, m_bVisiblity				(true)
	, m_eMode					(MODE_VERTEX_COLOR)
	, m_pStream					(NULL)
	, m_fAlphaMaskValue			(0.0f)
	, m_fUVOffsetMultipler		(1.0f)
	, m_fFakeReflectPower		(1.0f)
	, m_pLightMapTextureDay		(NULL)
	, m_pLightMapTextureNight	(NULL)
	, m_pAlphaMaskTexture		(NULL)
	, m_pUVNoiseTexture			(NULL)
	, m_pFakeReflectionTexture	(NULL)
	, m_fPlaneWidth				(0.0f)
	, m_fPlaneHeight			(0.0f)
	, m_nNumSegmentsX			(0)
	, m_nNumSegmentsY			(0)
{
}

A3DMirrorPlane::A3DMirrorPlane( A3DMirrorPlane& Other )
{
}

A3DMirrorPlane::~A3DMirrorPlane()
{
	Release();
}

bool A3DMirrorPlane::Init( A3DMirrorPlaneRender* pMirrorPlaneRender, const MODE eMode, const float fWidth, const float fHeight, const unsigned int nNumSegmentsX, const unsigned int nNumSegmentsY )
{
	Release();

	m_pMirrorPlaneRender = pMirrorPlaneRender;

	m_eMode = eMode;

	m_strLightMapTextureDayName = ERROR_LIGHT_MAP;
	m_strLightMapTextureNightName = ERROR_LIGHT_MAP;

	m_strUVNoiseTextureName = DEFAULT_UV_NOISE_TEXTURE;
	m_strFakeReflectionTextureName = DEFAULT_FAKE_REFLECTION_TEXTURE;

	if( m_eMode == MODE_LIGHT_MAP )
	{
		m_nNumSegmentsX = 1;
		m_nNumSegmentsY = 1;

		_LoadLightMap( true, true );
	}
	else
	{
		m_nNumSegmentsX = a_Max( nNumSegmentsX, 1U );
		m_nNumSegmentsY = a_Max( nNumSegmentsY, 1U );
	}

	m_fPlaneWidth = a_Max( fWidth, 0.0f );
	m_fPlaneHeight = a_Max( fHeight, 0.0f );
	
	if( !_GenerateMeshData() )
	{
		g_A3DErrLog.Log( __FUNCTION__ " Failed to generate vertices and indices of mirror plane mesh." );
		return false;
	}

	if( !_CreateDeviceResources() )
	{
		return false;
	}

	return true;
}

void A3DMirrorPlane::Release()
{
	_ReleaseDeviceResources();

	m_pMirrorPlaneRender = NULL;
}

void A3DMirrorPlane::SetMode( const MODE eMode )
{
	if( eMode == MODE_LIGHT_MAP )
	{
		if( m_nNumSegmentsX > 1 || m_nNumSegmentsY > 1 )
		{
			m_nNumSegmentsX = 1;
			m_nNumSegmentsY = 1;
			_GenerateMeshData();
			UpdateStream();
		}
	}
	else
	{
		_ReleaseLightMap( true, true );
	}

	m_eMode = eMode;
}

void A3DMirrorPlane::SetPos( const A3DVECTOR3& vPos )
{
	m_vPos = vPos;
	_UpdateWorldMatrix();
}

void A3DMirrorPlane::SetDirAndUp( const A3DVECTOR3& vDir, const A3DVECTOR3& vUp )
{
	m_vDir = vDir;
	m_vUp = vUp;
	m_vDir.Normalize();
	m_vUp.Normalize();
	_UpdateWorldMatrix();
}

void A3DMirrorPlane::SetScaling( const A3DVECTOR3& vScaling )
{
	m_vScaling = vScaling;
	_UpdateWorldMatrix();
}

// Sets the size of plane. It will reset the vertex color and light map UV.
void A3DMirrorPlane::SetSize( const float fWidth, const float fHeight )
{
	m_fPlaneWidth = a_Max( fWidth, 0.0f );
	m_fPlaneHeight = a_Max( fHeight, 0.0f );
	_GenerateMeshData();
	UpdateStream();
}

// Sets the number of segments of plane mesh. It will reset the vertex color and light map UV. ( Sets the number of segments greater than 1 is not allowed when light maps are enabled. )
bool A3DMirrorPlane::SetSegments( const unsigned int nNumSegmentsX, const unsigned int nNumSegmentsY )
{
	if( ( m_eMode == MODE_LIGHT_MAP ) && ( nNumSegmentsX > 1 || nNumSegmentsY > 1 ) )
	{
		g_A3DErrLog.Log( __FUNCTION__ " Sets the number of segments greater than 1 is not allowed when light maps are enabled." );
		return false;
	}

	m_nNumSegmentsX = a_Max( nNumSegmentsX, 1U );
	m_nNumSegmentsY = a_Max( nNumSegmentsY, 1U );
	_GenerateMeshData();
	UpdateStream();

	return true;
}

// Gets reflection 3D plane.
A3DPLANE A3DMirrorPlane::GetReflectionPlane() const
{
	return A3DPLANE( m_vDir, DotProduct( m_vDir, m_vPos ) );
}

bool A3DMirrorPlane::SetAlphaMaskTexture( const char* pszAlphaMaskTextureName )
{
	ReleaseAlphaMaskTexture();

	m_strAlphaMaskTextureName = pszAlphaMaskTextureName;

	return LoadAlphaMaskTexture();
}

void A3DMirrorPlane::ReleaseAlphaMaskTexture()
{
	if( m_pAlphaMaskTexture == NULL )
		return;

	ASSERT( m_pMirrorPlaneRender != NULL );
	A3DTextureMan* pTextureManager = m_pMirrorPlaneRender->m_pA3DEngine->GetA3DTextureMan();

	pTextureManager->ReleaseTexture( m_pAlphaMaskTexture );
	m_pAlphaMaskTexture = NULL;
}

bool A3DMirrorPlane::LoadAlphaMaskTexture()
{
	if( m_strAlphaMaskTextureName.IsEmpty() )
	{
		return true;
	}

	ASSERT( m_pMirrorPlaneRender != NULL );
	A3DTextureMan* pTextureManager = m_pMirrorPlaneRender->m_pA3DEngine->GetA3DTextureMan();

	const char* pszAlphaMaskTextureName = (const char*)m_strAlphaMaskTextureName;
	if( !pTextureManager->LoadTextureFromFile( pszAlphaMaskTextureName, &m_pAlphaMaskTexture ) )
	{
		g_A3DErrLog.Log( __FUNCTION__ " Failed to load alpha mask texture [%s]!", pszAlphaMaskTextureName );
		return false;
	}

	return true;
}

bool A3DMirrorPlane::SetUVNoiseTexture( const char* pszUVNoiseTextureName )
{
	m_strUVNoiseTextureName = pszUVNoiseTextureName;
	return _LoadUVNoiseTexture();
}

bool A3DMirrorPlane::SetFakeReflectionTexture( const char* pszFakeReflectionName )
{
	m_strFakeReflectionTextureName = pszFakeReflectionName;
	return _LoadFakeReflectionTexture();
}

bool A3DMirrorPlane::SetDayLightMap( const char* pszLightMapName )
{
	_ReleaseLightMap( true, false );

	m_strLightMapTextureDayName = pszLightMapName;

	return _LoadLightMap( true, false );
}

bool A3DMirrorPlane::SetNightLightMap( const char* pszLightMapName )
{
	_ReleaseLightMap( false, true );

	m_strLightMapTextureNightName = pszLightMapName;

	return _LoadLightMap( false, true );
}

// Updates vertices and indices data to stream.
void A3DMirrorPlane::UpdateStream()
{
	ASSERT( m_pStream != NULL );

	const unsigned int nNumVertices = m_ArrayVertices.GetSize();
	const unsigned int nNumIndices = m_ArrayIndices.GetSize();

	Vertex* pVertices = m_ArrayVertices.GetData();
	unsigned short* pIndices = m_ArrayIndices.GetData();

	m_pStream->SetVerts( (void*)pVertices, nNumVertices );
	m_pStream->SetIndices( pIndices, nNumIndices );
}

// Calculates the bounding box by parameters.
void A3DMirrorPlane::CalculateAABB( A3DAABB& OutAABB, const float fWidth, const float fHeight, const A3DMATRIX4& WorldMatrix )
{
	// Builds the bounding box of plane in world space.
	const float fMinX = fWidth / -2.0f;
	const float fMaxX = fWidth / 2.0f;
	const float fMinY = fHeight / -2.0f;
	const float fMaxY = fHeight / 2.0f;
	const float fMinZ = -0.001f;
	const float fMaxZ = 0.001f;

	A3DVECTOR3 pCorners[8];
	pCorners[0].x = fMinX;	pCorners[0].y = fMinY;	pCorners[0].z = fMinZ;
	pCorners[1].x = fMinX;	pCorners[1].y = fMaxY;	pCorners[1].z = fMinZ;
	pCorners[2].x = fMaxX;	pCorners[2].y = fMinY;	pCorners[2].z = fMinZ;
	pCorners[3].x = fMaxX;	pCorners[3].y = fMaxY;	pCorners[3].z = fMinZ;
	pCorners[4].x = fMinX;	pCorners[4].y = fMinY;	pCorners[4].z = fMaxZ;
	pCorners[5].x = fMinX;	pCorners[5].y = fMaxY;	pCorners[5].z = fMaxZ;
	pCorners[6].x = fMaxX;	pCorners[6].y = fMinY;	pCorners[6].z = fMaxZ;
	pCorners[7].x = fMaxX;	pCorners[7].y = fMaxY;	pCorners[7].z = fMaxZ;

	for( int i=0; i<8; ++i )
	{
		pCorners[i] = pCorners[i] * WorldMatrix;
	}

	OutAABB.Clear();
	OutAABB.Build( pCorners, 8 );
}

bool A3DMirrorPlane::Save( const char* pszFileName )
{
	AFile File;
	if( !File.Open( pszFileName, AFILE_CREATENEW | AFILE_BINARY ) )
	{
		g_A3DErrLog.Log( __FUNCTION__ ", Failed to save file [%s].", pszFileName );
		return false;
	}

	Save( &File );

	File.Close();
	return true;
}

bool A3DMirrorPlane::Load( A3DMirrorPlaneRender* pMirrorPlaneRender, const char* pszFileName )
{
	AFile File;
	if( !File.Open( pszFileName, AFILE_OPENEXIST | AFILE_BINARY ) )
	{
		g_A3DErrLog.Log( __FUNCTION__ ", Failed to open file [%s].", pszFileName );
		return false;
	}

	Load( pMirrorPlaneRender, &File );

	File.Close();
	return true;
}

bool A3DMirrorPlane::Save( AFile* pFileToSave )
{
	DWORD nWriteLength = 0;

	unsigned int nFileVersion = FILE_VERSION;
	if( !pFileToSave->Write( &nFileVersion, sizeof(nFileVersion), &nWriteLength ) )
		goto FAILED;

	if( !pFileToSave->Write( &m_bVisiblity, sizeof(m_bVisiblity), &nWriteLength ) )
		goto FAILED;

	if( !pFileToSave->Write( &m_eMode, sizeof(m_eMode), &nWriteLength ) )
		goto FAILED;

	if( !pFileToSave->Write( &m_fAlphaMaskValue, sizeof(m_fAlphaMaskValue), &nWriteLength ) )
		goto FAILED;

	if( !pFileToSave->Write( &m_fUVOffsetMultipler, sizeof(m_fUVOffsetMultipler), &nWriteLength ) )
		goto FAILED;

	if( !pFileToSave->Write( &m_fFakeReflectPower, sizeof(m_fFakeReflectPower), &nWriteLength ) )
		goto FAILED;

	if( !pFileToSave->WriteString( m_strUVNoiseTextureName ) )
		goto FAILED;

	if( !pFileToSave->WriteString( m_strFakeReflectionTextureName ) )
		goto FAILED;

	if( !pFileToSave->WriteString( m_strAlphaMaskTextureName ) )
		goto FAILED;

	if( !pFileToSave->WriteString( m_strLightMapTextureDayName ) )
		goto FAILED;

	if( !pFileToSave->WriteString( m_strLightMapTextureNightName ) )
		goto FAILED;

	if( !pFileToSave->Write( &m_fPlaneWidth, sizeof(m_fPlaneWidth), &nWriteLength ) )
		goto FAILED;

	if( !pFileToSave->Write( &m_fPlaneHeight, sizeof(m_fPlaneHeight), &nWriteLength ) )
		goto FAILED;

	if( !pFileToSave->Write( &m_nNumSegmentsX, sizeof(m_nNumSegmentsX), &nWriteLength ) )
		goto FAILED;

	if( !pFileToSave->Write( &m_nNumSegmentsY, sizeof(m_nNumSegmentsY), &nWriteLength ) )
		goto FAILED;

	if( !pFileToSave->Write( &m_vPos, sizeof(m_vPos), &nWriteLength ) )
		goto FAILED;

	if( !pFileToSave->Write( &m_vDir, sizeof(m_vDir), &nWriteLength ) )
		goto FAILED;

	if( !pFileToSave->Write( &m_vUp, sizeof(m_vUp), &nWriteLength ) )
		goto FAILED;

	if( !pFileToSave->Write( &m_vScaling, sizeof(m_vScaling), &nWriteLength ) )
		goto FAILED;

	if( m_eMode == MODE_VERTEX_COLOR )
	{
		unsigned int nNumVertices = (unsigned int)m_ArrayVertices.GetSize();
		if( !pFileToSave->Write( &nNumVertices, sizeof(nNumVertices), &nWriteLength ) )
			goto FAILED;

		Vertex* pVertices = m_ArrayVertices.GetData();
		for( unsigned int nVertex=0; nVertex<nNumVertices; ++nVertex )
		{
			Vertex& rVertex = pVertices[ nVertex ];

			if( !pFileToSave->Write( &rVertex.DiffuseDay, sizeof(A3DCOLOR), &nWriteLength ) )
				goto FAILED;

			if( !pFileToSave->Write( &rVertex.DiffuseNight, sizeof(A3DCOLOR), &nWriteLength ) )
				goto FAILED;
		}
	}

	return true;

FAILED:
	g_A3DErrLog.Log( __FUNCTION__ ", Failed to save file." );
	return false;
}

bool A3DMirrorPlane::Load( A3DMirrorPlaneRender* pMirrorPlaneRender, AFile* pFileToLoad )
{
	DWORD nReadLength = 0;

	AArray<A3DCOLOR> ArrayDayColors;
	AArray<A3DCOLOR> ArrayNightColors;

	unsigned int nFileVersion = 0;
	if( !pFileToLoad->Read( &nFileVersion, sizeof(nFileVersion), &nReadLength ) )
		goto FAILED;

	if( nFileVersion < FILE_VERSION_10 || nFileVersion > FILE_VERSION )
	{
		g_A3DErrLog.Log( __FUNCTION__ ", Wrong file version [0x%X].", nFileVersion );
		goto FAILED;
	}

	if( !pFileToLoad->Read( &m_bVisiblity, sizeof(m_bVisiblity), &nReadLength ) )
		goto FAILED;

	if( nFileVersion < FILE_VERSION_13 )
	{
		bool bUseAlphaMaskTexture = false;
		if( !pFileToLoad->Read( &bUseAlphaMaskTexture, sizeof(bUseAlphaMaskTexture), &nReadLength ) )
			goto FAILED;
	}

	if( !pFileToLoad->Read( &m_eMode, sizeof(m_eMode), &nReadLength ) )
		goto FAILED;

	if( !pFileToLoad->Read( &m_fAlphaMaskValue, sizeof(m_fAlphaMaskValue), &nReadLength ) )
		goto FAILED;


	if( nFileVersion >= FILE_VERSION_12 )
	{
		if( !pFileToLoad->Read( &m_fUVOffsetMultipler, sizeof(m_fUVOffsetMultipler), &nReadLength ) )
			goto FAILED;

		if( !pFileToLoad->Read( &m_fFakeReflectPower, sizeof(m_fFakeReflectPower), &nReadLength ) )
			goto FAILED;

		if( !pFileToLoad->ReadString( m_strUVNoiseTextureName ) )
			goto FAILED;

		if( !pFileToLoad->ReadString( m_strFakeReflectionTextureName ) )
			goto FAILED;
	}
	else
	{
		m_fUVOffsetMultipler = 1.0f;
		m_fFakeReflectPower = 1.0f;
		m_strUVNoiseTextureName = DEFAULT_UV_NOISE_TEXTURE;
		m_strFakeReflectionTextureName = DEFAULT_FAKE_REFLECTION_TEXTURE;
	}

	if( !pFileToLoad->ReadString( m_strAlphaMaskTextureName ) )
		goto FAILED;

	if( !pFileToLoad->ReadString( m_strLightMapTextureDayName ) )
		goto FAILED;

	if( !pFileToLoad->ReadString( m_strLightMapTextureNightName ) )
		goto FAILED;

	if( !pFileToLoad->Read( &m_fPlaneWidth, sizeof(m_fPlaneWidth), &nReadLength ) )
		goto FAILED;

	if( !pFileToLoad->Read( &m_fPlaneHeight, sizeof(m_fPlaneHeight), &nReadLength ) )
		goto FAILED;

	if( !pFileToLoad->Read( &m_nNumSegmentsX, sizeof(m_nNumSegmentsX), &nReadLength ) )
		goto FAILED;

	if( !pFileToLoad->Read( &m_nNumSegmentsY, sizeof(m_nNumSegmentsY), &nReadLength ) )
		goto FAILED;

	if( !pFileToLoad->Read( &m_vPos, sizeof(m_vPos), &nReadLength ) )
		goto FAILED;

	if( !pFileToLoad->Read( &m_vDir, sizeof(m_vDir), &nReadLength ) )
		goto FAILED;

	if( !pFileToLoad->Read( &m_vUp, sizeof(m_vUp), &nReadLength ) )
		goto FAILED;

	if( nFileVersion >= FILE_VERSION_13 )
	{
		if( !pFileToLoad->Read( &m_vScaling, sizeof(m_vScaling), &nReadLength ) )
			goto FAILED;
	}
	else
	{
		m_vScaling = A3DVECTOR3( 1.0f, 1.0f, 1.0f );
	}

	bool bLoadColors = false;

	if( nFileVersion >= FILE_VERSION_11 && m_eMode == MODE_VERTEX_COLOR )
	{
		const unsigned int nNumColumns = m_nNumSegmentsX + 1;
		const unsigned int nNumRows = m_nNumSegmentsY + 1;
		const unsigned int nNumVertices = nNumColumns * nNumRows;

		unsigned int nReadNumVertices = (unsigned int)m_ArrayVertices.GetSize();
		if( !pFileToLoad->Read( &nReadNumVertices, sizeof(nReadNumVertices), &nReadLength ) )
			goto FAILED;

		if( nNumVertices != nReadNumVertices )
			goto FAILED;

		ArrayDayColors.SetSize( nNumVertices, 0 );
		ArrayNightColors.SetSize( nNumVertices, 0 );
		
		A3DCOLOR* pDayColors = ArrayDayColors.GetData();
		A3DCOLOR* pNightColors = ArrayNightColors.GetData();

		for( unsigned int nVertex=0; nVertex<nNumVertices; ++nVertex )
		{
			if( !pFileToLoad->Read( &pDayColors[ nVertex ], sizeof(A3DCOLOR), &nReadLength ) )
				goto FAILED;

			if( !pFileToLoad->Read( &pNightColors[ nVertex ], sizeof(A3DCOLOR), &nReadLength ) )
				goto FAILED;
		}

		bLoadColors = true;
	}


	if( !Init( pMirrorPlaneRender, m_eMode, m_fPlaneWidth, m_fPlaneHeight, m_nNumSegmentsX, m_nNumSegmentsY ) )
		goto FAILED;

	if( bLoadColors )
	{
		A3DCOLOR* pDayColors = ArrayDayColors.GetData();
		A3DCOLOR* pNightColors = ArrayNightColors.GetData();

		Vertex* pVertices = m_ArrayVertices.GetData();
		const unsigned int nNumVertices = (unsigned int)m_ArrayVertices.GetSize();

		for( unsigned int nVertex=0; nVertex<nNumVertices; ++nVertex )
		{
			Vertex& rVertex = pVertices[ nVertex ];

			rVertex.DiffuseDay = pDayColors[ nVertex ];
			rVertex.DiffuseNight = pNightColors[ nVertex ];
		}

		UpdateStream();
	}

	_UpdateWorldMatrix();

	if( !m_strAlphaMaskTextureName.IsEmpty() )
	{
		if( !LoadAlphaMaskTexture() )
			goto FAILED;
	}

	if( m_eMode == MODE_LIGHT_MAP )
	{
		if( !_LoadLightMap( true, true ) )
			goto FAILED;
	}

	return true;

FAILED:
	g_A3DErrLog.Log( __FUNCTION__ ", Failed to load file." );
	return false;
}

// Performs view frustum culling and commits this object to renderer.
bool A3DMirrorPlane::Render( A3DViewport* pViewport )
{
	if( !m_bVisiblity )
		return true;

	A3DCameraBase* pCamera = pViewport->GetCamera();

	// Tests if the camera can not see the positive face of the reflection plane.
	const A3DVECTOR3& vCameraPos = pCamera->GetPos();
	A3DVECTOR3 vLookDir = vCameraPos - m_vPos;
	vLookDir.Normalize();
	if( DotProduct( m_vDir, vLookDir ) <= 0.0f )
		return true;

	_UpdateAABB();
	if( !pCamera->AABBInViewFrustum( m_AABB ) )
		return true;

	return m_pMirrorPlaneRender->RegisterRenderMirrorPlane( pViewport, this );
}

// Recalculates the world matrix by transformation vectors.
void A3DMirrorPlane::_UpdateWorldMatrix()
{
	const A3DMATRIX4 RotationMatrix = a3d_TransformMatrix( m_vDir, m_vUp, A3DVECTOR3( 0.0f, 0.0f, 0.0f ) );
	const A3DMATRIX4 ScalingMatrix = a3d_Scaling( m_vScaling.x, m_vScaling.y, m_vScaling.z );
	m_WorldMatrix = ScalingMatrix * RotationMatrix;
	m_WorldMatrix._41 += m_vPos.x;
	m_WorldMatrix._42 += m_vPos.y;
	m_WorldMatrix._43 += m_vPos.z;
}

// Generates vertices and indices of plane mesh.
bool A3DMirrorPlane::_GenerateMeshData()
{
	// Calculates vertices
	{
		const unsigned int nNumColumns = m_nNumSegmentsX + 1;
		const unsigned int nNumRows = m_nNumSegmentsY + 1;
		const unsigned int nNumVertices = nNumColumns * nNumRows;
		m_ArrayVertices.SetSize( nNumVertices, 0 );
		Vertex* pVertices = m_ArrayVertices.GetData();

		const float fStartX = m_fPlaneWidth / -2.0f;
		const float fStartY = m_fPlaneHeight / 2.0f;
		const float fDeltaX = m_fPlaneWidth / (float)m_nNumSegmentsX;
		const float fDeltaY = m_fPlaneHeight / -(float)m_nNumSegmentsY;

		float fY = fStartY;

		for( unsigned int nRow=0; nRow<nNumRows; ++nRow )
		{
			float fX = fStartX;

			const float fV = (float)nRow / (float)m_nNumSegmentsY;

			for( unsigned int nColumn=0; nColumn<nNumColumns; ++nColumn )
			{
				const float fU = (float)nColumn / (float)m_nNumSegmentsX;

				pVertices->vPos.x = fX;
				pVertices->vPos.y = fY;
				pVertices->vPos.z = 0.0f;

				pVertices->vNormal.x = 0.0f;
				pVertices->vNormal.y = 0.0f;
				pVertices->vNormal.z = 1.0f;

				pVertices->DiffuseDay = 0;
				pVertices->DiffuseNight = 0;

				pVertices->fU = fU;
				pVertices->fV = fV;

				pVertices->fLightMapU = fU;
				pVertices->fLightMapV = fV;

				++pVertices;

				fX += fDeltaX;
			}

			fY += fDeltaY;
		}
	}

	// Calculates indices
	{
		const unsigned int nNumIndices = m_nNumSegmentsX * m_nNumSegmentsY * 6;
		m_ArrayIndices.SetSize( nNumIndices, 0 );
		unsigned short* pIndices = m_ArrayIndices.GetData();

		const unsigned int nNumVerticesRow = m_nNumSegmentsX + 1;

		for( unsigned int nZ=0; nZ<m_nNumSegmentsY; ++nZ )
		{
			const unsigned short nTopStartIndex = nZ * nNumVerticesRow;
			const unsigned short nBottomStartIndex = ( nZ + 1 ) * nNumVerticesRow;

			for( unsigned int nX=0; nX<m_nNumSegmentsX; ++nX )
			{
				const unsigned short nIndexTopLeft = nTopStartIndex + nX;
				const unsigned short nIndexTopRight = nIndexTopLeft + 1;
				const unsigned short nIndexBottomLeft = nBottomStartIndex + nX;
				const unsigned short nIndexBottomRight = nIndexBottomLeft + 1;

				pIndices[0] = nIndexBottomRight;
				pIndices[1] = nIndexTopRight;
				pIndices[2] = nIndexBottomLeft;
				pIndices[3] = nIndexTopLeft;
				pIndices[4] = nIndexBottomLeft;
				pIndices[5] = nIndexTopRight;

				pIndices += 6;
			}
		}
	}

	return true;
}

// Clear vertices and indices of plane mesh.
void A3DMirrorPlane::_ClearMeshData()
{
	m_ArrayVertices.RemoveAll();
	m_ArrayIndices.RemoveAll();
}

void A3DMirrorPlane::_ReleaseLightMap( bool bDay, bool bNight )
{
	ASSERT( m_pMirrorPlaneRender != NULL );
	A3DTextureMan* pTextureManager = m_pMirrorPlaneRender->m_pA3DEngine->GetA3DTextureMan();

	if( bDay )
	{
		pTextureManager->ReleaseTexture( m_pLightMapTextureDay );
		m_pLightMapTextureDay = NULL;
	}

	if( bNight )
	{
		pTextureManager->ReleaseTexture( m_pLightMapTextureNight );
		m_pLightMapTextureNight = NULL;
	}
}

bool A3DMirrorPlane::_LoadLightMap( bool bDay, bool bNight )
{
	if( m_eMode != MODE_LIGHT_MAP )
	{
		g_A3DErrLog.Log( __FUNCTION__ " Can't load light maps when light maps are disabled." );
		return false;
	}

	if( bDay )
	{
		if( !_LoadLightMapTexture( m_strLightMapTextureDayName, &m_pLightMapTextureDay ) )
		{
			return false;
		}
	}

	if( bNight )
	{
		if( !_LoadLightMapTexture( m_strLightMapTextureNightName, &m_pLightMapTextureNight ) )
		{
			return false;
		}
	}

	return true;
}

// Creates all device resources used to render the mirror plane mesh.
bool A3DMirrorPlane::_CreateDeviceResources()
{
	A3DDevice* pDevice = m_pMirrorPlaneRender->m_pA3DEngine->GetA3DDevice();
	ASSERT( pDevice != NULL );

	const unsigned int nNumVertices = m_ArrayVertices.GetSize();
	const unsigned int nNumIndices = m_ArrayIndices.GetSize();

	m_pStream = new A3DStream;
	if( !m_pStream || !m_pStream->Init(	pDevice, sizeof(Vertex), 0, nNumVertices, nNumIndices, A3DSTRM_STATIC, A3DSTRM_STATIC ) )
	{
		g_A3DErrLog.Log( __FUNCTION__ " Failed to create mirror plane stream." );
		return false;
	}

	UpdateStream();

	if( !_LoadUVNoiseTexture() )
		return false;

	if( !_LoadFakeReflectionTexture() )
		return false;

	return true;
}

// Release all device resources used to render the mirror plane mesh.
void A3DMirrorPlane::_ReleaseDeviceResources()
{
	if( m_pMirrorPlaneRender == NULL )
		return;

	A3DRELEASE( m_pStream );

	A3DTextureMan* pTextureMan = m_pMirrorPlaneRender->m_pA3DEngine->GetA3DTextureMan();
	pTextureMan->ReleaseTexture( m_pFakeReflectionTexture );
	pTextureMan->ReleaseTexture( m_pUVNoiseTexture );

	_ReleaseLightMap( true, true );
}

// Loads light map texture, uses error light map texture instead if failed.
bool A3DMirrorPlane::_LoadLightMapTexture( const AString& strLightMapTextureName, A3DTexture** ppTexture )
{
	ASSERT( m_pMirrorPlaneRender != NULL );
	A3DTextureMan* pTextureMan = m_pMirrorPlaneRender->m_pA3DEngine->GetA3DTextureMan();

	const char* pszLightMapTextureName = strLightMapTextureName;
	if( strLightMapTextureName.IsEmpty() || strLightMapTextureName.Find( LIGHT_MAP_VERSION ) < 0 )
	{
		pszLightMapTextureName = ERROR_LIGHT_MAP;
	}

	if( !pTextureMan->LoadTextureFromFile( pszLightMapTextureName, ppTexture ) )
	{
		pszLightMapTextureName = ERROR_LIGHT_MAP;
		if( !pTextureMan->LoadTextureFromFile( pszLightMapTextureName, ppTexture ) )
		{
			g_A3DErrLog.Log( __FUNCTION__ " Failed to load light map texture [%s]!", pszLightMapTextureName );
			return false;
		}
	}

	return true;
}

bool A3DMirrorPlane::_LoadUVNoiseTexture()
{
	A3DTextureMan* pTextureMan = m_pMirrorPlaneRender->m_pA3DEngine->GetA3DTextureMan();

	const char* pszUVNoiseTextureName = m_strUVNoiseTextureName;
	if( m_strUVNoiseTextureName.IsEmpty() )
	{
		pszUVNoiseTextureName = DEFAULT_UV_NOISE_TEXTURE;
	}

	pTextureMan->ReleaseTexture( m_pUVNoiseTexture );

	if( !pTextureMan->LoadTextureFromFile( pszUVNoiseTextureName, &m_pUVNoiseTexture ) )
	{
		g_A3DErrLog.Log( __FUNCTION__ " Failed to load UV noise texture [%s]!", pszUVNoiseTextureName );
		return false;
	}

	return true;
}

bool A3DMirrorPlane::_LoadFakeReflectionTexture()
{
	A3DTextureMan* pTextureMan = m_pMirrorPlaneRender->m_pA3DEngine->GetA3DTextureMan();

	const char* pszFakeReflectionTextureName = m_strFakeReflectionTextureName;
	if( m_strFakeReflectionTextureName.IsEmpty() )
	{
		pszFakeReflectionTextureName = DEFAULT_UV_NOISE_TEXTURE;
	}

	pTextureMan->ReleaseTexture( m_pFakeReflectionTexture );

	if( !pTextureMan->LoadTextureFromFile( pszFakeReflectionTextureName, &m_pFakeReflectionTexture ) )
	{
		g_A3DErrLog.Log( __FUNCTION__ " Failed to load fake reflection texture [%s]!", pszFakeReflectionTextureName );
		return false;
	}

	return true;
}

// Recalculates the bounding box after transformation was changed.
void A3DMirrorPlane::_UpdateAABB()
{
	CalculateAABB( m_AABB, m_fPlaneWidth, m_fPlaneHeight, m_WorldMatrix );
}
