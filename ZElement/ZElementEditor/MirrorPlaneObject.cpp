/*
 * FILE: d:\Dev\Projects2.2\SDSElement\SDSElementEditor\MirrorPlaneObject.cpp
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

#include "stdafx.h"
#include "elementeditor.h"
#include "A3DPI.h"
#include "A3DMirrorPlane.h"
#include "A3DMirrorPlaneRender.h"
#include "BakingGeometryData.h"
#include "MirrorPlaneObject.h"
#include "A3D.h"
#include "Render.h"
#include "PlatformRenderer.h"
#include "ElementRender.h"
#include "LightMapSizeDlg.h"
#include "SceneObjectManager.h"
#include "MainFrm.h"
#include "A3DLitModel.h"



CMirrorPlaneObject::CMirrorPlaneObject()
	: m_pA3DMirrorPlane			(NULL)
	, m_nLitBakingMode			((int)A3DMirrorPlane::MODE_VERTEX_COLOR)
	, m_fPlaneWidth				(10.0f)
	, m_fPlaneHeight			(10.0f)
	, m_nNumSegmentsX			(1)
	, m_nNumSegmentsY			(1)
	, m_fAlphaMaskValue			(0.0f)
	, m_fUVOffsetMultipler		(1.0f)
	, m_fFakeReflectPower		(1.0f)
	, m_nAutoLightmapSize		(-1)
	, m_nCustomLightmapSize		(-1)
	, m_fLitDirectScale			(1.0f)
	, m_fLitIndirectScale		(1.0f)
	, m_fLitAOScale				(1.0f)
{
	m_nObjectType = SO_TYPE_MIRROR_PLANE;

	m_LitBakingModeSet.AddElement( "顶点色", (unsigned int)A3DMirrorPlane::MODE_VERTEX_COLOR );
	m_LitBakingModeSet.AddElement( "光照图", (unsigned int)A3DMirrorPlane::MODE_LIGHT_MAP );

	m_pLightMapSizeFuns = new MIRROR_PLANE_LIGHTMAP_SIZE_FUNCS( this );
}

CMirrorPlaneObject::~CMirrorPlaneObject()
{
	Release();
}

void CMirrorPlaneObject::Release()
{
	if( m_pLightMapSizeFuns != NULL )
	{
		delete m_pLightMapSizeFuns;
		m_pLightMapSizeFuns = NULL;
	}

	DeleteRenderData();
}

// Creates a rendering object. It must be called after adding a mirror plane object into the scene.
bool CMirrorPlaneObject::BuildRenderData()
{
	DeleteRenderData();

	PlatformRenderer* pPlatformRender = g_Render.GetPlatformRender();
	if( pPlatformRender == NULL )
		return false;

	ElementRender* pElementRender = pPlatformRender->GetElementRender();
	if( pElementRender == NULL )
		return false;

	A3DMirrorPlaneRender* pMirrorPlaneRender = pElementRender->GetMirrorPlaneRender();
	if( pMirrorPlaneRender == NULL )
		return false;

	m_pA3DMirrorPlane = new A3DMirrorPlane;
	const A3DMirrorPlane::MODE eMode = (A3DMirrorPlane::MODE)m_nLitBakingMode;
	if( !m_pA3DMirrorPlane->Init( pMirrorPlaneRender, eMode, m_fPlaneWidth, m_fPlaneHeight, m_nNumSegmentsX, m_nNumSegmentsY ) )
		return false;

	m_pA3DMirrorPlane->SetDayLightMap(m_strLightMapTextureDayName);
	m_pA3DMirrorPlane->SetNightLightMap(m_strLightMapTextureNightName);

	if( eMode == A3DMirrorPlane::MODE_VERTEX_COLOR )
	{
		const unsigned int nNumVertices = m_pA3DMirrorPlane->GetNumVertices();

		if( ( nNumVertices != m_ArrayDayColors.GetSize() ) || ( nNumVertices != m_ArrayNightColors.GetSize() ) )
		{
			m_ArrayDayColors.SetSize( nNumVertices, 0 );
			m_ArrayNightColors.SetSize( nNumVertices, 0 );

			A3DCOLOR* pDayColors = m_ArrayDayColors.GetData();
			A3DCOLOR* pNightColors = m_ArrayNightColors.GetData();

			memset( pDayColors, 0, sizeof(A3DCOLOR) * nNumVertices );
			memset( pNightColors, 0, sizeof(A3DCOLOR) * nNumVertices );
		}

		A3DCOLOR* pDayColors = m_ArrayDayColors.GetData();
		A3DCOLOR* pNightColors = m_ArrayNightColors.GetData();

		A3DMirrorPlane::Vertex* pVertices = m_pA3DMirrorPlane->GetVertices();

		for( unsigned int nVertex=0; nVertex<nNumVertices; ++nVertex )
		{
			A3DMirrorPlane::Vertex& rVertex = pVertices[ nVertex ];

			rVertex.DiffuseDay = pDayColors[ nVertex ];
			rVertex.DiffuseNight = pNightColors[ nVertex ];
		}

		m_pA3DMirrorPlane->UpdateStream();
	}

	m_pA3DMirrorPlane->SetAlphaMaskValue(m_fAlphaMaskValue);
	m_pA3DMirrorPlane->SetAlphaMaskTexture(m_strAlphaMaskTextureName);

	m_pA3DMirrorPlane->SetUVOffsetMultipler( m_fUVOffsetMultipler );
	m_pA3DMirrorPlane->SetUVNoiseTexture( m_strUVNoiseTextureName );

	m_pA3DMirrorPlane->SetFakeReflectionPower( m_fFakeReflectPower );
	m_pA3DMirrorPlane->SetFakeReflectionTexture( m_strFakeReflectionTextureName );

	_UpdateTransformation();

	return true;
}

// Deletes the rendering object.
void CMirrorPlaneObject::DeleteRenderData()
{
	if( m_pA3DMirrorPlane != NULL )
	{
		delete m_pA3DMirrorPlane;
		m_pA3DMirrorPlane = NULL;
	}
}

A3DVECTOR3 CMirrorPlaneObject::GetMin()
{
	A3DAABB AABB;
	A3DMirrorPlane::CalculateAABB( AABB, m_fPlaneWidth, m_fPlaneHeight, m_matAbs );
	return AABB.Mins;
}

A3DVECTOR3 CMirrorPlaneObject::GetMax()
{
	A3DAABB AABB;
	A3DMirrorPlane::CalculateAABB( AABB, m_fPlaneWidth, m_fPlaneHeight, m_matAbs );
	return AABB.Maxs;
}

//void CMirrorPlaneObject::PostRender(A3DViewport* pA3DViewport)
//{
//}

void CMirrorPlaneObject::Render(A3DViewport* pA3DViewport)
{
	if( m_pA3DMirrorPlane == NULL )
		return;

	m_pA3DMirrorPlane->Render( pA3DViewport );
}

void CMirrorPlaneObject::RenderForLightMapShadow(A3DViewport* pA3DViewport)
{
}

void CMirrorPlaneObject::RenderForLight(A3DViewport* pA3DViewport, float fOffsetX, float fOffsetZ)
{
}

bool CMirrorPlaneObject::BuildProperty()
{
//	if(CSceneObject::BuildProperty())
	{
		m_pProperty->DynAddProperty(AVariant(m_strName), "名字");
		m_pProperty->DynAddProperty(AVariant(m_vPos), "位置坐标");

		m_pProperty->DynAddProperty(AVariant(m_fPlaneWidth), "宽度");
		m_pProperty->DynAddProperty(AVariant(m_fPlaneHeight), "高度");
		m_pProperty->DynAddProperty(AVariant(m_nNumSegmentsX), "横向分段数");
		m_pProperty->DynAddProperty(AVariant(m_nNumSegmentsY), "纵向分段数");

		m_pProperty->DynAddProperty(AVariant(m_fAlphaMaskValue), "Alpha 遮罩比较值");
		m_pProperty->DynAddProperty(AVariant(m_strAlphaMaskTextureName), "Alpha 遮罩纹理文件", NULL, NULL, WAY_FILENAME, "");

		m_pProperty->DynAddProperty(AVariant(m_fUVOffsetMultipler), "Noise 采样坐标偏移系数");
		m_pProperty->DynAddProperty(AVariant(m_strUVNoiseTextureName), "Noise 采样坐标偏移纹理文件", NULL, NULL, WAY_FILENAME, "");

		m_pProperty->DynAddProperty(AVariant(m_fFakeReflectPower), "假反射强度");
		m_pProperty->DynAddProperty(AVariant(m_strFakeReflectionTextureName), "假反射纹理文件", NULL, NULL, WAY_FILENAME, "");

		m_pProperty->DynAddProperty(AVariant(m_nLitBakingMode),"光照烘焙模式",&m_LitBakingModeSet);

		m_pProperty->DynAddProperty(AVariant(m_nAutoLightmapSize),"自动光照图大小",NULL,NULL,WAY_READONLY);
		m_pProperty->DynAddProperty(AVariant(m_nCustomLightmapSize),"自定义光照图大小",(ASet*)m_pLightMapSizeFuns, NULL, WAY_CUSTOM);
		m_pProperty->DynAddProperty(AVariant(m_fLitDirectScale),"直接光照系数");
		m_pProperty->DynAddProperty(AVariant(m_fLitIndirectScale),"间接光照系数");
		m_pProperty->DynAddProperty(AVariant(m_fLitAOScale),"环境遮蔽系数");
		return true;
	}
	return false;
}

void CMirrorPlaneObject::UpdateProperty(bool bGet)
{
	if(!m_pProperty) return;
	if(bGet)
	{
		AString strNewName = (AString)m_pProperty->GetPropVal(0);
		CElementMap *pMap = AUX_GetMainFrame()->GetMap();
		if(pMap)
		{
			CSceneObjectManager *pSManager = pMap->GetSceneObjectMan();
			CSceneObject*pObj = pSManager->FindSceneObject(strNewName);
			if(pObj != NULL && pObj != this)
			{
				MessageBox(NULL,"该对象的名字已经存在，编辑器将恢复原对象名字！",NULL,MB_OK);

				m_pProperty->SetPropVal(0,m_strName);
				m_pProperty->SetPropVal(1,m_vPos);

				m_pProperty->SetPropVal(2,m_fPlaneWidth);
				m_pProperty->SetPropVal(3,m_fPlaneHeight);
				m_pProperty->SetPropVal(4,m_nNumSegmentsX);
				m_pProperty->SetPropVal(5,m_nNumSegmentsY);

				m_pProperty->SetPropVal(6,m_fAlphaMaskValue);
				m_pProperty->SetPropVal(7,m_strAlphaMaskTextureName);

				m_pProperty->SetPropVal(8,m_fUVOffsetMultipler);
				m_pProperty->SetPropVal(9,m_strUVNoiseTextureName);

				m_pProperty->SetPropVal(10,m_fFakeReflectPower);
				m_pProperty->SetPropVal(11,m_strFakeReflectionTextureName);

				m_pProperty->SetPropVal(12,m_nLitBakingMode);

				m_pProperty->SetPropVal(13,m_nAutoLightmapSize);
				m_pLightMapSizeFuns->OnSetValue(m_nCustomLightmapSize);
				m_pProperty->SetPropVal(15,m_fLitDirectScale);
				m_pProperty->SetPropVal(16,m_fLitIndirectScale);
				m_pProperty->SetPropVal(17,m_fLitAOScale);

				AUX_GetMainFrame()->GetToolTabWnd()->UpdatePropertyData(false);
				return;
			}
		}

		m_strName = strNewName;

		m_vPos = m_pProperty->GetPropVal(1);
		m_fPlaneWidth = m_pProperty->GetPropVal(2);
		m_fPlaneHeight = m_pProperty->GetPropVal(3);
		m_nNumSegmentsX = m_pProperty->GetPropVal(4);
		m_nNumSegmentsY = m_pProperty->GetPropVal(5);

		m_fAlphaMaskValue = m_pProperty->GetPropVal(6);
		m_strAlphaMaskTextureName = (AString)m_pProperty->GetPropVal(7);

		m_fUVOffsetMultipler = m_pProperty->GetPropVal(8);
		m_strUVNoiseTextureName = (AString)m_pProperty->GetPropVal(9);

		m_fFakeReflectPower = m_pProperty->GetPropVal(10);
		m_strFakeReflectionTextureName = (AString)m_pProperty->GetPropVal(11);

		m_nLitBakingMode = m_pProperty->GetPropVal(12);

		m_nAutoLightmapSize = GetAdaptLightmapSize(MIRROR_PLANE_OBJ_LITMAP_SIZE_FACTOR);
		m_nAutoLightmapSize = m_pProperty->GetPropVal(13);

		m_nCustomLightmapSize = m_pLightMapSizeFuns->OnGetValue();

		m_fLitDirectScale = m_pProperty->GetPropVal(15);
		m_fLitIndirectScale = m_pProperty->GetPropVal(16);
		m_fLitAOScale = m_pProperty->GetPropVal(17);

		m_matTrans.Translate(m_vPos.x,m_vPos.y,m_vPos.z);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
		BuildRenderData();
	}
	else
	{
		m_pProperty->SetPropVal(0,m_strName);
		m_pProperty->SetPropVal(1,m_vPos);

		m_pProperty->SetPropVal(2,m_fPlaneWidth);
		m_pProperty->SetPropVal(3,m_fPlaneHeight);
		m_pProperty->SetPropVal(4,m_nNumSegmentsX);
		m_pProperty->SetPropVal(5,m_nNumSegmentsY);

		m_pProperty->SetPropVal(6,m_fAlphaMaskValue);
		m_pProperty->SetPropVal(7,m_strAlphaMaskTextureName);

		m_pProperty->SetPropVal(8,m_fUVOffsetMultipler);
		m_pProperty->SetPropVal(9,m_strUVNoiseTextureName);

		m_pProperty->SetPropVal(10,m_fFakeReflectPower);
		m_pProperty->SetPropVal(11,m_strFakeReflectionTextureName);

		m_pProperty->SetPropVal(12,m_nLitBakingMode);

		m_pProperty->SetPropVal(13,m_nAutoLightmapSize);
		m_pLightMapSizeFuns->OnSetValue(m_nCustomLightmapSize);
		m_pProperty->SetPropVal(15,m_fLitDirectScale);
		m_pProperty->SetPropVal(16,m_fLitIndirectScale);
		m_pProperty->SetPropVal(17,m_fLitAOScale);
	}
}

// Copy object
CSceneObject* CMirrorPlaneObject::CopyObject()
{
	return NULL;
}

// Load data
bool CMirrorPlaneObject::Load(CELArchive& ar,DWORD dwVersion,int iLoadFlags)
{
	if(dwVersion < 0x73)
		return true;
	// common fields
	m_strName = AString(ar.ReadString());
	if(!ar.Read(&m_nObjectID,sizeof(int)))
		return false;

	// coordinate fields
	A3DVECTOR3 vr,vu,vl,vs;
	if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
		return false;
	if(!ar.Read(&vr,sizeof(A3DVECTOR3)))
		return false;
	if(!ar.Read(&vu,sizeof(A3DVECTOR3)))
		return false;
	if(!ar.Read(&vl,sizeof(A3DVECTOR3)))
		return false;
	if(!ar.Read(&vs,sizeof(A3DVECTOR3)))
		return false;

	m_matTrans.SetRow(3,m_vPos);
	m_matRotate.SetRow(2,vr);
	m_matRotate.SetRow(1,vu);
	m_matRotate.SetRow(0,vl);
	m_matScale._11 = vs.x;
	m_matScale._22 = vs.y;
	m_matScale._33 = vs.z;

	m_matAbs = m_matScale * m_matRotate * m_matTrans;

	// SO fields
	if(!ar.Read(&m_fPlaneWidth, sizeof(float)))
		return false;
	if(!ar.Read(&m_fPlaneHeight, sizeof(float)))
		return false;
	if(!ar.Read(&m_nNumSegmentsX, sizeof(unsigned int)))
		return false;
	if(!ar.Read(&m_nNumSegmentsY, sizeof(unsigned int)))
		return false;

	if(dwVersion >= 0x74)
	{
		if(!ar.Read(&m_nLitBakingMode, sizeof(m_nLitBakingMode)))
			return false;
	}
	else
	{
		m_nLitBakingMode = (int)A3DMirrorPlane::MODE_VERTEX_COLOR;
	}

	if(!ar.Read(&m_nAutoLightmapSize, sizeof(int)))
		return false;
	if(!ar.Read(&m_nCustomLightmapSize, sizeof(int)))
		return false;
	if(!ar.Read(&m_fLitDirectScale, sizeof(float)))
		return false;
	if(!ar.Read(&m_fLitIndirectScale, sizeof(float)))
		return false;
	if(!ar.Read(&m_fLitAOScale, sizeof(float)))
		return false;

	// light-map fields
	if(!ar.Read(&m_fAlphaMaskValue, sizeof(float)))
		return false;

	m_strAlphaMaskTextureName = ar.ReadString();
	m_strLightMapTextureDayName = ar.ReadString();
	m_strLightMapTextureNightName = ar.ReadString();

	const unsigned int nNumColumns = m_nNumSegmentsX + 1;
	const unsigned int nNumRows = m_nNumSegmentsY + 1;
	const unsigned int nNumVertices = nNumColumns * nNumRows;

	m_ArrayDayColors.SetSize( nNumVertices, 0 );
	m_ArrayNightColors.SetSize( nNumVertices, 0 );

	if( dwVersion >= 0x75 )
	{
		if(!ar.Read(&m_fUVOffsetMultipler, sizeof(float)))
			return false;
		if(!ar.Read(&m_fFakeReflectPower, sizeof(float)))
			return false;

		m_strUVNoiseTextureName = ar.ReadString();
		m_strFakeReflectionTextureName = ar.ReadString();

		if( m_nLitBakingMode == (int)A3DMirrorPlane::MODE_VERTEX_COLOR )
		{
			A3DCOLOR* pDayColors = m_ArrayDayColors.GetData();
			A3DCOLOR* pNightColors = m_ArrayNightColors.GetData();

			if( !ar.Read( pDayColors, sizeof(A3DCOLOR) * nNumVertices ) )
				return false;
			if( !ar.Read( pNightColors, sizeof(A3DCOLOR) * nNumVertices ) )
				return false;
		}
	}
	else
	{
		m_fUVOffsetMultipler = 1.0f;
		m_fFakeReflectPower = 1.0f;

		m_strUVNoiseTextureName.Empty();
		m_strFakeReflectionTextureName.Empty();

		A3DCOLOR* pDayColors = m_ArrayDayColors.GetData();
		A3DCOLOR* pNightColors = m_ArrayNightColors.GetData();

		for( unsigned int nVertex=0; nVertex<nNumVertices; ++nVertex )
		{
			pDayColors[ nVertex ] = 0;
			pNightColors[ nVertex ] = 0;
		}
	}

	BuildRenderData();
	return true;
}

// Save data
bool CMirrorPlaneObject::Save(CELArchive& ar,DWORD dwVersion)
{
	if(dwVersion < 0x73)
		return true;

	// common fields
	ar.WriteString(m_strName);
	if(!ar.Write(&m_nObjectID,sizeof(int)))
		return false;

	// coordinate fields
	m_matRotate = GetRotateMatrix();
	A3DVECTOR3 vr,vu,vl,vs;
	vr = m_matRotate.GetRow(2);
	vu = m_matRotate.GetRow(1);
	vl = m_matRotate.GetRow(0);
	vs = A3DVECTOR3(m_matScale._11,m_matScale._22,m_matScale._33);
	if(!ar.Write(&m_vPos,sizeof(A3DVECTOR3)))
		return false;
	if(!ar.Write(&vr,sizeof(A3DVECTOR3)))
		return false;
	if(!ar.Write(&vu,sizeof(A3DVECTOR3)))
		return false;
	if(!ar.Write(&vl,sizeof(A3DVECTOR3)))
		return false;
	if(!ar.Write(&vs,sizeof(A3DVECTOR3)))
		return false;

	// SO fields
	if(!ar.Write(&m_fPlaneWidth, sizeof(float)))
		return false;
	if(!ar.Write(&m_fPlaneHeight, sizeof(float)))
		return false;
	if(!ar.Write(&m_nNumSegmentsX, sizeof(unsigned int)))
		return false;
	if(!ar.Write(&m_nNumSegmentsY, sizeof(unsigned int)))
		return false;

	// light-map fields
	if(dwVersion >= 0x74)
	{
		if(!ar.Write(&m_nLitBakingMode, sizeof(m_nLitBakingMode)))
			return false;
	}

	if(!ar.Write(&m_nAutoLightmapSize, sizeof(int)))
		return false;
	if(!ar.Write(&m_nCustomLightmapSize, sizeof(int)))
		return false;
	if(!ar.Write(&m_fLitDirectScale, sizeof(float)))
		return false;
	if(!ar.Write(&m_fLitIndirectScale, sizeof(float)))
		return false;
	if(!ar.Write(&m_fLitAOScale, sizeof(float)))
		return false;

	if(!ar.Write(&m_fAlphaMaskValue, sizeof(float)))
		return false;
	ar.WriteString(m_strAlphaMaskTextureName);
	ar.WriteString(m_strLightMapTextureDayName);
	ar.WriteString(m_strLightMapTextureNightName);

	if(!ar.Write(&m_fUVOffsetMultipler, sizeof(float)))
		return false;
	if(!ar.Write(&m_fFakeReflectPower, sizeof(float)))
		return false;

	ar.WriteString(m_strUVNoiseTextureName);
	ar.WriteString(m_strFakeReflectionTextureName);

	if( m_nLitBakingMode == (int)A3DMirrorPlane::MODE_VERTEX_COLOR )
	{
		const unsigned int nNumColumns = m_nNumSegmentsX + 1;
		const unsigned int nNumRows = m_nNumSegmentsY + 1;
		const unsigned int nNumVertices = nNumColumns * nNumRows;

		ASSERT( nNumVertices == m_ArrayDayColors.GetSize() );
		ASSERT( nNumVertices == m_ArrayNightColors.GetSize() );

		const A3DCOLOR* pDayColors = m_ArrayDayColors.GetData();
		const A3DCOLOR* pNightColors = m_ArrayNightColors.GetData();

		if( !ar.Write( pDayColors, sizeof(A3DCOLOR) * nNumVertices ) )
			return false;
		if( !ar.Write( pNightColors, sizeof(A3DCOLOR) * nNumVertices ) )
			return false;
	}

	return true;
}

int	CMirrorPlaneObject::GetTriangleNum()
{
	if( m_pA3DMirrorPlane == NULL )
		return 0;

	return m_pA3DMirrorPlane->GetNumFaces();
}

bool CMirrorPlaneObject::SetVertexColorDay( const A3DCOLOR* pColors, const unsigned int nNumVertices )
{
	if( m_pA3DMirrorPlane != NULL )
	{
		const unsigned int nPlaneNumVertices = m_pA3DMirrorPlane->GetNumVertices();
		if( nPlaneNumVertices != nNumVertices )
		{
			return false;
		}
	}

	m_ArrayDayColors.SetSize( nNumVertices, 0 );
	A3DCOLOR* pDestColors = m_ArrayDayColors.GetData();
	memcpy( pDestColors, pColors, sizeof(A3DCOLOR) * nNumVertices );

	if( m_pA3DMirrorPlane != NULL )
	{
		A3DMirrorPlane::Vertex* pVertices = m_pA3DMirrorPlane->GetVertices();
		for( unsigned int i=0; i<nNumVertices; ++i )
		{
			pVertices[i].DiffuseDay = pColors[i];
		}
	}

	return true;
}

bool CMirrorPlaneObject::SetVertexColorNight( const A3DCOLOR* pColors, const unsigned int nNumVertices )
{
	if( m_pA3DMirrorPlane != NULL )
	{
		const unsigned int nPlaneNumVertices = m_pA3DMirrorPlane->GetNumVertices();
		if( nPlaneNumVertices != nNumVertices )
		{
			return false;
		}
	}

	m_ArrayNightColors.SetSize( nNumVertices, 0 );
	A3DCOLOR* pDestColors = m_ArrayNightColors.GetData();
	memcpy( pDestColors, pColors, sizeof(A3DCOLOR) * nNumVertices );

	if( m_pA3DMirrorPlane != NULL )
	{
		A3DMirrorPlane::Vertex* pVertices = m_pA3DMirrorPlane->GetVertices();
		for( unsigned int i=0; i<nNumVertices; ++i )
		{
			pVertices[i].DiffuseNight = pColors[i];
		}
	}

	return true;
}

bool CMirrorPlaneObject::UpdateStream()
{
	if( m_pA3DMirrorPlane == NULL )
		return false;

	m_pA3DMirrorPlane->UpdateStream();
	return true;
}

// Automatically calculates the suitable light map size by its area.
int CMirrorPlaneObject::GetAdaptLightmapSize( float fScale ) const
{
	float fSize = m_fPlaneWidth * m_fPlaneHeight * fScale;

	if (fSize < 20.0f) return 1;
	if (fSize < 40) return 64;
	if (fSize <= 128.0f) return 128;
	if (fSize <= 256.0f) return 256;
	return 512;
}

// Sets light map texture.
bool CMirrorPlaneObject::SetDayLightMap( const char* pszLightMapName )
{
	if( m_pA3DMirrorPlane == NULL )
		return false;

	if( !m_pA3DMirrorPlane->SetDayLightMap( pszLightMapName ) )
		return false;

	m_strLightMapTextureDayName = pszLightMapName;
	return true;
}

bool CMirrorPlaneObject::SetNightLightMap( const char* pszLightMapName )
{
	if( m_pA3DMirrorPlane == NULL )
		return false;

	if( !m_pA3DMirrorPlane->SetNightLightMap( pszLightMapName ) )
		return false;

	m_strLightMapTextureDayName = pszLightMapName;
	return true;
}

// Outputs all necessary geometry data for baking lighting information into a file.
bool CMirrorPlaneObject::OutputBakingGeometryFile( const char* pszFileName )
{
	if( m_pA3DMirrorPlane == NULL )
		return false;

	BAKING_GEOMETRY_DATA BakingGeometryData;

	const A3DMirrorPlane::MODE eMode = m_pA3DMirrorPlane->GetMode();
	BakingGeometryData.eBakingType = ( eMode == A3DMirrorPlane::MODE_VERTEX_COLOR ) ? BAKING_VERTEX_COLOR : BAKING_LIGHT_MAP;

	m_nAutoLightmapSize = GetAdaptLightmapSize( MIRROR_PLANE_OBJ_LITMAP_SIZE_FACTOR );
	BakingGeometryData.nLightMapSize = ( m_nCustomLightmapSize > 0 ) ? m_nCustomLightmapSize : m_nAutoLightmapSize;
	BakingGeometryData.fLitDirectScale = m_fLitDirectScale;
	BakingGeometryData.fLitIndirectScale = m_fLitIndirectScale;
	BakingGeometryData.fLitAOScale = m_fLitAOScale;

	BakingGeometryData.nNumVertices = m_pA3DMirrorPlane->GetNumVertices();
	BakingGeometryData.nNumIndices = m_pA3DMirrorPlane->GetNumFaces() * 3;

	BakingGeometryData.bHasNormal = true;

	const AString& strAlphaMaskTextureName = m_pA3DMirrorPlane->GetAlphaMaskTextureName();
	BakingGeometryData.bHasAlphaTestTexture = !strAlphaMaskTextureName.IsEmpty();


	A3DMirrorPlane::Vertex* pVertices = m_pA3DMirrorPlane->GetVertices();

	const A3DMATRIX4& WorldMatrix = m_pA3DMirrorPlane->GetWorldMatrix();

	AArray<A3DVECTOR3> ArrayPositions;
	ArrayPositions.SetSize( BakingGeometryData.nNumVertices, 0 );
	A3DVECTOR3* pPositions = ArrayPositions.GetData();

	for( unsigned int nVertex=0; nVertex<BakingGeometryData.nNumVertices; ++nVertex )
	{
		pPositions[ nVertex ] = pVertices[ nVertex ].vPos * WorldMatrix;
	}

	AArray<A3DVECTOR3> ArrayNormals;
	ArrayNormals.SetSize( BakingGeometryData.nNumVertices, 0 );
	A3DVECTOR3* pNormals = ArrayNormals.GetData();

	A3DMATRIX4 InvWorldMatrix = WorldMatrix.GetInverseTM();
	A3DMATRIX4 ITWorldMatrix = InvWorldMatrix.GetTranspose();

	for( unsigned int nVertex=0; nVertex<BakingGeometryData.nNumVertices; ++nVertex )
	{
		pNormals[ nVertex ] = a3d_VectorMatrix3x3( pVertices[ nVertex ].vNormal, ITWorldMatrix );
	}

	AArray<BAKING_UV> ArrayUVs;
	ArrayUVs.SetSize( BakingGeometryData.nNumVertices, 0 );
	BAKING_UV* pUVs = ArrayUVs.GetData();

	for( unsigned int nVertex=0; nVertex<BakingGeometryData.nNumVertices; ++nVertex )
	{
		const A3DMirrorPlane::Vertex& rSrc = pVertices[ nVertex ];
		BAKING_UV& rDst = pUVs[ nVertex ];
		rDst.fU = rSrc.fLightMapU;
		rDst.fV = rSrc.fLightMapV;
	}


	A3DLitModel* pA3DLitModel = new A3DLitModel;
	A3DDevice* pA3DDevice = g_Render.GetA3DDevice();
	if( !pA3DLitModel->Init( pA3DDevice ) )
		goto FAILED;

	const char* pszAlphaMaskTextureName = strAlphaMaskTextureName.IsEmpty() ? ERROR_LIGHT_MAP : (const char*)strAlphaMaskTextureName;
	const bool bSupportLightMap = ( BakingGeometryData.eBakingType == BAKING_LIGHT_MAP );
	A3DLitMesh* pA3DLitMesh = pA3DLitModel->AddNewMesh( BakingGeometryData.nNumVertices, BakingGeometryData.nNumIndices / 3, pszAlphaMaskTextureName, bSupportLightMap );
	if( pA3DLitMesh == NULL )
		goto FAILED;

	pA3DLitMesh->SetUseLightMap( bSupportLightMap );

	{
		unsigned short* pIndices = m_pA3DMirrorPlane->GetIndices();
		WORD* pLitMeshIndices = pA3DLitMesh->GetIndices();
		memcpy( pLitMeshIndices, pIndices, sizeof(WORD) * BakingGeometryData.nNumIndices );

		A3DLIGHTMAPCOORD* pLitMeshUVs = NULL;
		if( bSupportLightMap )
		{
			pLitMeshUVs = pA3DLitMesh->GetLightMapCoords();
			ASSERT( pLitMeshUVs != NULL );
		}

		A3DLMVERTEX* pLitMeshVertices = pA3DLitMesh->GetVerts();
		ASSERT( pLitMeshVertices != NULL );

		for( unsigned int nVertex=0; nVertex<BakingGeometryData.nNumVertices; ++nVertex )
		{
			BAKING_UV& rUV = pUVs[ nVertex ];

			A3DLMVERTEX& rLitMeshVertex = pLitMeshVertices[ nVertex ];
			rLitMeshVertex.pos = pPositions[ nVertex ];
			rLitMeshVertex.normal = pNormals[ nVertex ];
			rLitMeshVertex.diffuse = A3DCOLORRGBA( 255, 255, 255, 255 );
			rLitMeshVertex.u = rUV.fU;
			rLitMeshVertex.v = rUV.fV;

			if( bSupportLightMap )
			{
				A3DLIGHTMAPCOORD& rLightMapUV = pLitMeshUVs[ nVertex ];
				rLightMapUV.u = rUV.fU;
				rLightMapUV.v = rUV.fV;
			}
		}

		A3DVECTOR3* pLitMeshNormals = pA3DLitMesh->GetNormals();
		ASSERT( pLitMeshNormals != NULL );
		memcpy( pLitMeshNormals, pNormals, sizeof(A3DVECTOR3) * BakingGeometryData.nNumVertices );

		A3DCOLOR* pLitMeshDayColors = pA3DLitMesh->GetDayColors();
		ASSERT( pLitMeshDayColors != NULL );
		memset( pLitMeshDayColors, 0, sizeof(A3DCOLOR) * BakingGeometryData.nNumVertices );

		A3DCOLOR* pLitMeshNightColors = pA3DLitMesh->GetNightColors();
		ASSERT( pLitMeshNightColors != NULL );
		memset( pLitMeshNightColors, 0, sizeof(A3DCOLOR) * BakingGeometryData.nNumVertices );

		A3DAABB LitMeshAABB;
		LitMeshAABB.Build( pPositions, BakingGeometryData.nNumVertices );
		pA3DLitMesh->SetAABB( LitMeshAABB );
	}

	if( !pA3DLitModel->Save( pszFileName ) )
		goto FAILED;

	A3DRELEASE( pA3DLitModel );

	/*AFile File;
	if( !File.Open( pszFileName, AFILE_CREATENEW | AFILE_BINARY ) )
		goto FAILED;

	DWORD nWriteLength = 0;

	if( !File.Write( &BakingGeometryData, sizeof(BAKING_GEOMETRY_DATA), &nWriteLength ) )
		goto FAILED;

	if( BakingGeometryData.bHasAlphaTestTexture )
	{
		if( !File.WriteString( strAlphaMaskTextureName ) )
			goto FAILED;
	}

	if( !File.Write( pPositions, sizeof(A3DVECTOR3) * BakingGeometryData.nNumVertices, &nWriteLength ) )
		goto FAILED;

	if( !File.Write( pNormals, sizeof(A3DVECTOR3) * BakingGeometryData.nNumVertices, &nWriteLength ) )
		goto FAILED;

	if( !File.Write( pUVs, sizeof(BAKING_UV) * BakingGeometryData.nNumVertices, &nWriteLength ) )
		goto FAILED;

	unsigned short* pIndices = m_pA3DMirrorPlane->GetIndices();
	if( !File.Write( pIndices, sizeof(unsigned int) * BakingGeometryData.nNumIndices, &nWriteLength ) )
		goto FAILED;

	File.Close();*/
	return true;

FAILED:
	A3DRELEASE( pA3DLitModel );
	g_A3DErrLog.Log( __FUNCTION__ ", Failed to save file [%s].", pszFileName );
	return false;
}

void CMirrorPlaneObject::_UpdateTransformation()
{
	if( m_pA3DMirrorPlane == NULL )
		return;

	m_pA3DMirrorPlane->SetPos( m_vPos );

	const A3DVECTOR3 vDir = m_matRotate.GetRow( 2 );
	const A3DVECTOR3 vUp = m_matRotate.GetRow( 1 );
	m_pA3DMirrorPlane->SetDirAndUp( vDir, vUp );

	const A3DVECTOR3 vScaling( m_matScale._11, m_matScale._22, m_matScale._33 );
	m_pA3DMirrorPlane->SetScaling( vScaling );
}

bool CMirrorPlaneObject::IsIntersectionWithArea(const A3DVECTOR3& vMin, const A3DVECTOR3& vMax)
{
	A3DVECTOR3 min = GetMin();
	A3DVECTOR3 max = GetMax();

	if(min.x>vMax.x) return false;
	if(min.z>vMax.z) return false;
	if(max.x<vMin.x) return false;
	if(max.z<vMin.z) return false;
	return true;
}


BOOL CALLBACK MIRROR_PLANE_LIGHTMAP_SIZE_FUNCS::OnActivate(void)
{
	CLightMapSizeDlg dlg;
	dlg.m_nSize = m_pOwner->m_nCustomLightmapSize;

	if (dlg.DoModal() == IDOK)
	{
		m_pOwner->m_nCustomLightmapSize = dlg.m_nSize;
		OnSetValue(0);
	}

	return TRUE;
}

LPCTSTR CALLBACK MIRROR_PLANE_LIGHTMAP_SIZE_FUNCS::OnGetShowString(void) const
{
	return m_strText;
}

AVariant CALLBACK MIRROR_PLANE_LIGHTMAP_SIZE_FUNCS::OnGetValue(void) const
{
	return m_var;
}

void CALLBACK MIRROR_PLANE_LIGHTMAP_SIZE_FUNCS::OnSetValue(const AVariant& var)
{
	m_var = var;
	int nSize = m_pOwner->m_nCustomLightmapSize;
	if (nSize < 0)
		m_strText.Format("未设置");
	else if (nSize < 64)
		m_strText.Format("使用顶点光");
	else
		m_strText.Format("%d x %d", nSize, nSize);
}
