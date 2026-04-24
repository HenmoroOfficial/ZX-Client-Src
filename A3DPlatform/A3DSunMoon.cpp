/*
 * FILE: EC_SunMoon.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Hedi, 2005/3/12
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#include "A3D.h"
#include "A3DSunMoon.h"
#include "A3DRenderTargetMan.h"
#include "A3DHLSL.h"
#include "A3DEnvironment.h"
				  
#define TIME_SCALE				(6.0f)

#define SKY_FAR					100.0f
#define _EA(x)					(x)

#define SUN_UNIT_SIZE			10.0f
#define MOON_UNIT_SIZE			20.0f

static const D3DVERTEXELEMENT9 s_aVertexDecl[] =
{
    {0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
    {0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
    {0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 1},
    {0, 20, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
    D3DDECL_END()
};

static const D3DVERTEXELEMENT9 s_aTLVertexDecl[] =
{
    {0, 0,  D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0},
    {0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
    {0, 20, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 1},
    {0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
    D3DDECL_END()
};


static const D3DVERTEXELEMENT9 s_aOccTestVertexDecl[] =
{
    {0, 0,  D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0},
    D3DDECL_END()
};


A3DSunMoon::A3DSunMoon(A3DDevice* pA3DDevice)
:m_pA3DDevice(pA3DDevice)
{
	m_pSunTexture		= NULL;
	m_pSunShineTexture	= NULL;
	m_pMoonTexture		= NULL;

	m_bCanDoFlare		= false;
	m_pFlareAlphaTarget	= NULL;
	m_pStreamFlareAlpha	= NULL;
	m_pFlaresTexture	= NULL;
	m_pStreamFlares		= NULL;

	m_pSunFlareTexture	= NULL;
	m_pMoonFlareTexture	= NULL;
	m_pStreamSun		= NULL;
	m_pStreamSunShine	= NULL;
	m_pStreamMoon		= NULL;
	m_pStreamSunPoint	= NULL;

	m_fDNFactor			= 0.0f;
	m_fDNFactorDest		= 0.0f;

	m_bTimeHasBeenSet	= false;
	m_bDirHasBeenSet	= false;
	m_bFirstAlpha		= true;

	m_bSunVisible		= true;
	m_bMoonVisible		= true;
	m_vSunVisibility	= true;
	m_vMoonVisibility	= true;

    m_pHLSLMain         = NULL;
    m_pHLSLTestOcc      = NULL;
    m_pVertDecl         = NULL;
    m_pOccTestDecl      = NULL;
    m_pTLVertDecl       = NULL;
    m_dwLastTickCount   = GetTickCount();
}

A3DSunMoon::~A3DSunMoon()
{
	Release();
}

bool A3DSunMoon::Init()
{
	Release();

	m_vSunSize		= 1.0f;
	m_vMoonSize		= 1.0f;
	m_colorSun		= A3DCOLORRGB(255, 255, 255);
	m_colorMoon		= A3DCOLORRGB(255, 255, 255);
	m_vSunPitch		= 0.0f;
	m_vMoonPitch	= 0.0f;
	m_vDeg			= 0.0f;
	m_vTimeOfTheDay = 0.0f;
	m_vecLightDir	= A3DVECTOR3(-1.0f, 0.0f, 0.0f);
    m_dwLastTickCount   = GetTickCount();

	A3DEngine* pA3DEngine = m_pA3DDevice->GetA3DEngine();

	A3DTextureMan * pTextureMan = pA3DEngine->GetA3DTextureMan();

	if( !pTextureMan->LoadTextureFromFile(_EA("textures\\nature\\sun.bmp"), &m_pSunTexture) )
	{
		a_LogOutput(1, _EA("A3DSunMoon::Init, Falied to load sun texture"));
		return false;
	}

	if( !pTextureMan->LoadTextureFromFile(_EA("textures\\nature\\sunshine.bmp"), &m_pSunShineTexture) )
	{
		a_LogOutput(1, _EA("A3DSunMoon::Init, Falied to load sun shine texture"));
		return false;
	}

	if( !pTextureMan->LoadTextureFromFile(_EA("textures\\nature\\moon.tga"), &m_pMoonTexture) )
	{
		a_LogOutput(1, _EA("A3DSunMoon::Init, Falied to load moon texture"));
		return false;
	}

	m_pStreamSun = new A3DStream();
	if( !m_pStreamSun->Init(m_pA3DDevice, A3DVT_LVERTEX, 6, 0, A3DSTRM_STATIC, 0) )
	{
		a_LogOutput(1, _EA("A3DSunMoon::Init, Falied to create sun stream"));
		return false;
	}

	m_pStreamSunShine = new A3DStream();
	if( !m_pStreamSunShine->Init(m_pA3DDevice, A3DVT_LVERTEX, 8, 18, A3DSTRM_STATIC, A3DSTRM_STATIC) )
	{
		a_LogOutput(1, _EA("A3DSunMoon::Init, Falied to create sun shine stream"));
		return false;
	}

	m_pStreamMoon = new A3DStream();
	if( !m_pStreamMoon->Init(m_pA3DDevice, A3DVT_LVERTEX, 6, 0, A3DSTRM_STATIC, 0) )
	{
		a_LogOutput(1, _EA("A3DSunMoon::Init, Falied to create moon stream"));
		return false;
	}
    
    m_pVertDecl = new A3DVertexDecl();
    if (!m_pVertDecl->Init(m_pA3DDevice, s_aVertexDecl))
    {
        a_LogOutput(1, _EA("A3DSunMoon::Init, Falied to create vertex declaration"));
        return false;
    }

	A3DLVERTEX * pVerts = NULL;
	if( !m_pStreamSun->LockVertexBuffer(0, 0, (BYTE **)&pVerts, NULL) )
	{
		a_LogOutput(1, _EA("A3DSunMoon::Init, failed to lock sun stream!"));
		return false;
	}
	pVerts[0] = A3DLVERTEX(A3DVECTOR3(-SUN_UNIT_SIZE, SUN_UNIT_SIZE, 0.0f), 0xffffffff, 0xff000000, 0.0f, 0.0f);
	pVerts[1] = A3DLVERTEX(A3DVECTOR3(SUN_UNIT_SIZE, SUN_UNIT_SIZE, 0.0f), 0xffffffff, 0xff000000, 1.0f, 0.0f);
	pVerts[2] = A3DLVERTEX(A3DVECTOR3(-SUN_UNIT_SIZE, -1.0f * SUN_UNIT_SIZE, 0.0f), 0xffffffff, 0xff000000, 0.0f, 1.0f);

	pVerts[3] = pVerts[2];
	pVerts[4] = pVerts[1];
	pVerts[5] = A3DLVERTEX(A3DVECTOR3(SUN_UNIT_SIZE, -1.0f * SUN_UNIT_SIZE, 0.0f), 0xffffffff, 0xff000000, 1.0f, 1.0f);
	m_pStreamSun->UnlockVertexBuffer();

	if( !m_pStreamSunShine->LockVertexBuffer(0, 0, (BYTE **)&pVerts, NULL) )
	{
		a_LogOutput(1, _EA("A3DSunMoon::Init, failed to lock sun shine stream!"));
		return false;
	}
	pVerts[0] = A3DLVERTEX(A3DVECTOR3(-SUN_UNIT_SIZE * 6.0f, SUN_UNIT_SIZE, 0.0f), 0xffffffff, 0xff000000, 0.0f, 0.0f);
	pVerts[1] = A3DLVERTEX(A3DVECTOR3(-SUN_UNIT_SIZE * 2.0f, SUN_UNIT_SIZE, 0.0f), 0xffffffff, 0xff000000, 1.0f / 3.0f, 0.0f);
	pVerts[2] = A3DLVERTEX(A3DVECTOR3(SUN_UNIT_SIZE * 2.0f, SUN_UNIT_SIZE, 0.0f), 0xffffffff, 0xff000000, 2.0f / 3.0f, 0.0f);
	pVerts[3] = A3DLVERTEX(A3DVECTOR3(SUN_UNIT_SIZE * 6.0f, SUN_UNIT_SIZE, 0.0f), 0xffffffff, 0xff000000, 1.0f, 0.0f);

	pVerts[4] = A3DLVERTEX(A3DVECTOR3(-SUN_UNIT_SIZE * 6.0f, -3.0f * SUN_UNIT_SIZE, 0.0f), 0xffffffff, 0xff000000, 0.0f, 1.0f);
	pVerts[5] = A3DLVERTEX(A3DVECTOR3(-SUN_UNIT_SIZE * 2.0f, -3.0f * SUN_UNIT_SIZE, 0.0f), 0xffffffff, 0xff000000, 1.0f / 3.0f, 1.0f);
	pVerts[6] = A3DLVERTEX(A3DVECTOR3(SUN_UNIT_SIZE * 2.0f, -3.0f * SUN_UNIT_SIZE, 0.0f), 0xffffffff, 0xff000000, 2.0f / 3.0f, 1.0f);
	pVerts[7] = A3DLVERTEX(A3DVECTOR3(SUN_UNIT_SIZE * 6.0f, -3.0f * SUN_UNIT_SIZE, 0.0f), 0xffffffff, 0xff000000, 1.0f, 1.0f);
	m_pStreamSunShine->UnlockVertexBuffer();

	WORD * pIndices = NULL;
	if( !m_pStreamSunShine->LockIndexBuffer(0, 0, (BYTE **)&pIndices, NULL) )
	{
		a_LogOutput(1, _EA("A3DSunMoon::Init, failed to lock sun shine stream!"));
		return false;
	}
	for(int i=0; i<3; i++)
	{
		pIndices[i * 6 + 0] = i;
		pIndices[i * 6 + 1] = i + 1;
		pIndices[i * 6 + 2] = i + 4;

		pIndices[i * 6 + 3] = i + 4;
		pIndices[i * 6 + 4] = i + 1;
		pIndices[i * 6 + 5] = i + 5;
	}
	m_pStreamSunShine->UnlockIndexBuffer();

	if( !m_pStreamMoon->LockVertexBuffer(0, 0, (BYTE **)&pVerts, NULL) )
	{
		a_LogOutput(1, _EA("A3DSunMoon::Init, failed to lock moon stream!"));
		return false;
	}
	pVerts[0] = A3DLVERTEX(A3DVECTOR3(-MOON_UNIT_SIZE, MOON_UNIT_SIZE, 0.0f), 0xffffffff, 0xff000000, 0.0f, 0.0f);
	pVerts[1] = A3DLVERTEX(A3DVECTOR3(MOON_UNIT_SIZE, MOON_UNIT_SIZE, 0.0f), 0xffffffff, 0xff000000, 1.0f, 0.0f);
	pVerts[2] = A3DLVERTEX(A3DVECTOR3(-MOON_UNIT_SIZE, -1.0f * MOON_UNIT_SIZE, 0.0f), 0xffffffff, 0xff000000, 0.0f, 1.0f);

	pVerts[3] = pVerts[2];
	pVerts[4] = pVerts[1];
	pVerts[5] = A3DLVERTEX(A3DVECTOR3(MOON_UNIT_SIZE, -1.0f * MOON_UNIT_SIZE, 0.0f), 0xffffffff, 0xff000000, 1.0f, 1.0f);
	m_pStreamMoon->UnlockVertexBuffer();

	
	m_bCanDoFlare = false;
	if( !InitFlare() )
	{
		ReleaseFlare();
	}
	m_bCanDoFlare = true;

    A3DHLSLMan* pHLSLMan = m_pA3DDevice->GetA3DEngine()->GetA3DHLSLMan();
    AString strShaderFile = "shaders\\2.2\\HLSL\\Sky\\SunMoon.hlsl";
    m_pHLSLMain = pHLSLMan->LoadShader(strShaderFile, "vs_main", strShaderFile, "ps_main", "", 0);
    if (!m_pHLSLMain)
    {
        a_LogOutput(1, _EA("A3DSunMoon::Init, failed to load shader!"));
        return false;
    }
    m_pHLSLTestOcc = pHLSLMan->LoadShader(strShaderFile, "vs_main", strShaderFile, "ps_main_occ", "", 0);
    if (!m_pHLSLTestOcc)
    {
        a_LogOutput(1, _EA("A3DSunMoon::Init, failed to load shader!"));
        return false;
    }
	return true;
}

bool A3DSunMoon::Release()
{
	A3DTextureMan * pTextureMan = m_pA3DDevice->GetA3DEngine()->GetA3DTextureMan();

	if( m_pSunTexture )
	{
		pTextureMan->ReleaseTexture(m_pSunTexture);
		m_pSunTexture = NULL;
	}

	if( m_pSunShineTexture )
	{
		pTextureMan->ReleaseTexture(m_pSunShineTexture);
		m_pSunShineTexture = NULL;
	}

	if( m_pMoonTexture )
	{
		pTextureMan->ReleaseTexture(m_pMoonTexture);
		m_pMoonTexture = NULL;
	}

	A3DRELEASE(m_pStreamSun);
	A3DRELEASE(m_pStreamSunShine);
	A3DRELEASE(m_pStreamMoon);
    A3DRELEASE(m_pVertDecl);

    A3DHLSLMan* pHLSLMan = m_pA3DDevice->GetA3DEngine()->GetA3DHLSLMan();
    if (!m_pHLSLMain)
    {
        pHLSLMan->ReleaseShader(m_pHLSLMain);
        m_pHLSLMain = NULL;
    }
    if (!m_pHLSLTestOcc)
    {
        pHLSLMan->ReleaseShader(m_pHLSLTestOcc);
        m_pHLSLMain = NULL;
    }

	ReleaseFlare();
	return true;
}

bool A3DSunMoon::InitFlare()
{
	A3DEngine* pA3DEngine = m_pA3DDevice->GetA3DEngine();
	A3DTextureMan * pTextureMan = pA3DEngine->GetA3DTextureMan();

	if( !pTextureMan->LoadTextureFromFile(_EA("textures\\nature\\sunflare.bmp"), &m_pSunFlareTexture) )
	{
		a_LogOutput(1, _EA("A3DSunMoon::Init, Falied to load moon texture"));
		return false;
	}

	if( !pTextureMan->LoadTextureFromFile(_EA("textures\\nature\\moonflare.bmp"), &m_pMoonFlareTexture) )
	{
		a_LogOutput(1, _EA("A3DSunMoon::Init, Falied to load moon texture"));
		return false;
	}

	if( !pTextureMan->LoadTextureFromFile(_EA("textures\\nature\\flares.bmp"), &m_pFlaresTexture) )
	{
		a_LogOutput(1, _EA("A3DSunMoon::Init, Falied to load flares texture"));
		return false;
	}

	A3DRenderTarget::RTFMT fmt;
	fmt.iWidth		= 1;
	fmt.iHeight		= 1;
	fmt.fmtTarget	= A3DFMT_A8R8G8B8;
	fmt.fmtDepth		= A3DFMT_D24X8;

	m_pFlareAlphaTarget = new A3DRenderTarget();
	if( !m_pFlareAlphaTarget->Init(m_pA3DDevice, fmt, true, false) )
	{
		a_LogOutput(1, _EA("A3DSunMoon::Init, failed to create flare alpha accumulat target!"));
		return false;
	}
	
	// now create a rectangle
	m_pStreamSunPoint = new A3DStream();
	if( !m_pStreamSunPoint->Init(m_pA3DDevice, A3DVT_TLVERTEX, 9 * 6, 0, A3DSTRM_REFERENCEPTR, 0) )
	{
		a_LogOutput(1, _EA("A3DSunMoon::Init, Falied to create sun point stream"));
		return false;
	}
	m_pStreamFlareAlpha = new A3DStream();
	if( !m_pStreamFlareAlpha->Init(m_pA3DDevice, sizeof(FLARE_ALPHA_VERT), 0, 1, 0, A3DSTRM_REFERENCEPTR, 0) )
	{
		a_LogOutput(1, _EA("A3DSunMoon::Init, Falied to create flare alpha stream"));
		return false;
	}
	m_pStreamFlares = new A3DStream();
	if( !m_pStreamFlares->Init(m_pA3DDevice, A3DVT_TLVERTEX, 6 * 15, 0, A3DSTRM_REFERENCEPTR, 0) )
	{
		a_LogOutput(1, _EA("A3DSunMoon::Init, Falied to create flares stream"));
		return false;
	}

    m_pOccTestDecl = new A3DVertexDecl();
    if (!m_pOccTestDecl->Init(m_pA3DDevice, s_aOccTestVertexDecl))
    {
        a_LogOutput(1, _EA("A3DSunMoon::Init, Falied to create flare vertex declaration"));
        return false;
    }

    m_pTLVertDecl = new A3DVertexDecl();
    if (!m_pTLVertDecl->Init(m_pA3DDevice, s_aTLVertexDecl))
    {
        a_LogOutput(1, _EA("A3DSunMoon::Init, Falied to create TL vertex declaration"));
        return false;
    }

	return true;
}

bool A3DSunMoon::ReleaseFlare()
{
	A3DEngine* pA3DEngine = m_pA3DDevice->GetA3DEngine();
	A3DTextureMan * pTextureMan = pA3DEngine->GetA3DTextureMan();

	if( m_pSunFlareTexture )
	{
		pTextureMan->ReleaseTexture(m_pSunFlareTexture);
		m_pSunFlareTexture = NULL;
	}

	if( m_pMoonFlareTexture )
	{
		pTextureMan->ReleaseTexture(m_pMoonFlareTexture);
		m_pMoonFlareTexture = NULL;
	}

	if( m_pFlaresTexture )
	{
		pTextureMan->ReleaseTexture(m_pFlaresTexture);
		m_pFlaresTexture = NULL;
	}

	if( m_pFlareAlphaTarget )
	{
		m_pFlareAlphaTarget->Release();
		delete m_pFlareAlphaTarget;
		m_pFlareAlphaTarget = NULL;
	}

	A3DRELEASE(m_pStreamSunPoint);
	A3DRELEASE(m_pStreamFlareAlpha);
	A3DRELEASE(m_pStreamFlares);
    A3DRELEASE(m_pOccTestDecl);
    A3DRELEASE(m_pTLVertDecl);

	return true;
}

static void Orthonormalize(A3DVECTOR3& v1, A3DVECTOR3& v2, A3DVECTOR3& v3)
{
    // Schmidt process
    v1 = Normalize(v1);
    v2 = Normalize(v2 - DotProduct(v2, v1) * v1);
    v3 = Normalize(v3 - DotProduct(v3, v1) * v1 - DotProduct(v3, v2) * v2);
}

bool A3DSunMoon::Render(A3DViewport * pViewport)
{
	// see if not set time yet.
	if( !m_bTimeHasBeenSet || !m_bDirHasBeenSet )
		return true;
    
    if (!m_pHLSLMain || !m_pVertDecl)
        return false;

	DWORD dwClipState;
	dwClipState = m_pA3DDevice->GetDeviceRenderState(D3DRS_CLIPPLANEENABLE);
	
	A3DCameraBase * pCamera = pViewport->GetCamera();
    if (!pCamera)
        return true;

	A3DVECTOR3 vecCamPos;
	if( pCamera->IsMirrored() )
	{
		vecCamPos = pCamera->GetMirrorCamera()->GetPos();
		A3DMATRIX4 matMirror = pCamera->GetMirrorMatrix();
		vecCamPos = vecCamPos * InverseTM(matMirror);
		m_pA3DDevice->SetDeviceRenderState(D3DRS_CLIPPLANEENABLE, 0);
	}
	else
	{
		vecCamPos = pCamera->GetPos();
	}

	A3DDevice * pA3DDevice = m_pA3DDevice;
    A3DMATRIX4 matProjOld = pA3DDevice->GetProjectionMatrix();
    A3DMATRIX4 matProj = matProjOld;
    matProj._13 = 0; matProj._23 = 0; matProj._33 = 1; matProj._43 = 0; 
	pA3DDevice->SetSourceAlpha(A3DBLEND_SRCALPHA);
	pA3DDevice->SetDestAlpha(A3DBLEND_ONE);
	pA3DDevice->SetZWriteEnable(false);
	pA3DDevice->SetZTestEnable(true);
	bool bFog = pA3DDevice->GetFogEnable();
	pA3DDevice->SetFogEnable(false);
    m_pVertDecl->Appear();

    // Compute visibility
	BYTE sunVis = BYTE(m_vSunVisibility * 255.0f);
	BYTE moonVis = (BYTE)(m_vMoonVisibility * 255.0f);
    A3DCOLORVALUE vSunBrightness = (a3d_ColorRGBAToColorValue(m_colorSun));
    vSunBrightness.a = m_vSunVisibility;
    A3DCOLORVALUE vMoonBrightness = (a3d_ColorRGBAToColorValue(m_colorMoon));
    vMoonBrightness.a = m_vMoonVisibility;
    if (m_pA3DDevice->GetA3DEngine()->GetA3DEnvironment()->GetIsUnderWater())
    {
        vSunBrightness.a *= 0.5f;
        vMoonBrightness.a *= 0.5f;
    }
    A3DMATRIX4 matVP = pA3DDevice->GetViewMatrix() * matProj;

        // first render sun
	if( m_vSunPitch > 0.0f )
	{
		m_pStreamSun->Appear(0, false);

		float	vSin = (float)sin(m_vSunPitch);
		float	vCos = (float)cos(m_vSunPitch);
		A3DVECTOR3 vecDir = A3DVECTOR3(vCos * (float)cos(m_vDeg), vSin, vCos * (float)sin(m_vDeg));
		A3DVECTOR3 vecRight = CrossProduct(A3DVECTOR3(0.0f, 1.0f, 0.0f), vecDir);
		A3DVECTOR3 vecUp = Normalize(CrossProduct(vecDir, vecRight));
		A3DVECTOR3 vecPos = vecCamPos + vecDir * SKY_FAR;

		float vSunScale = 1.0f + m_fDNFactor * 2.0f;
		A3DMATRIX4 matWorld = Scaling(vSunScale, vSunScale, vSunScale) * TransformMatrix(vecDir, vecUp, vecPos);

        m_pHLSLMain->SetTexture("g_TextureSampler", m_pSunTexture);
        m_pHLSLMain->SetTexture("g_OccSampler", NULL);
        m_pHLSLMain->SetValue2f("g_vSampleFlags", 1, 0);
        A3DMATRIX4 matWVP = matWorld * matVP;
        m_pHLSLMain->SetConstantMatrix("g_matWVP", matWVP);
        A3DVECTOR4 vBright = A3DVECTOR4(vSunBrightness.r, vSunBrightness.g, vSunBrightness.b, vSunBrightness.a);
        m_pHLSLMain->SetValue4f("g_vBrightness", &vBright);
        m_pHLSLMain->Appear();

		pA3DDevice->DrawPrimitive(A3DPT_TRIANGLELIST, 0, 2);

        m_pHLSLMain->Disappear();
	}

	// then render the sun shine
	if( m_fDNFactor != 0.0f && m_fDNFactor != 1.0f )
	{
		m_pStreamSunShine->Appear(0, false);

		float vSunShine = 1.0f;
		if( m_fDNFactor < 0.3f )
		{
			vSunShine = 0.0f;
		}
		else if( m_fDNFactor < 0.6f )
		{
			vSunShine = (m_fDNFactor - 0.3f) / 0.3f;
		}
		else if( m_fDNFactor < 0.8f )
		{
			vSunShine = 1.0f;
		}
		else
		{
			vSunShine = (1.0f - m_fDNFactor) / 0.2f;
		}
		
		if( m_fDNFactorDest == 1.0f )
			vSunShine *= 0.8f; // sunset, just not too bright
		else
			vSunShine *= 1.0f; // sun rise, just make a little bright

		float	vSin = (float)sin(m_vSunPitch);
		float	vCos = (float)cos(m_vSunPitch);
		A3DVECTOR3 vecDir = A3DVECTOR3(vCos * (float)cos(m_vDeg), vSin, vCos * (float)sin(m_vDeg));
		A3DVECTOR3 vecRight = CrossProduct(A3DVECTOR3(0.0f, 1.0f, 0.0f), vecDir);
		A3DVECTOR3 vecUp = Normalize(CrossProduct(vecDir, vecRight));
		A3DVECTOR3 vecPos = vecCamPos + vecDir * SKY_FAR;

		float vSunScale = 1.0f + m_fDNFactor * 2.0f;
		A3DMATRIX4 matWorld = Scaling(vSunScale, vSunScale, vSunScale) * TransformMatrix(vecDir, vecUp, vecPos);

        m_pHLSLMain->SetTexture("g_TextureSampler", m_pSunShineTexture);
        m_pHLSLMain->SetTexture("g_OccSampler", NULL);
        m_pHLSLMain->SetValue2f("g_vSampleFlags", 1, 0);
        A3DMATRIX4 matWVP = matWorld * matVP;
        m_pHLSLMain->SetConstantMatrix("g_matWVP", matWVP);
        A3DVECTOR4 vBright = A3DVECTOR4(vSunBrightness.r * vSunShine, vSunBrightness.g * vSunShine, vSunBrightness.b * vSunShine, vSunBrightness.a);
        m_pHLSLMain->SetValue4f("g_vBrightness", &vBright);

        m_pHLSLMain->Appear();

		pA3DDevice->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, 8, 0, 6);

        m_pHLSLMain->Disappear();
	}

	// then render the moon
	if( m_vMoonPitch > 0.0f )
	{
		m_pStreamMoon->Appear(0, false);

		float	vMoonSin = (float)sin(m_vMoonPitch);
		float	vMoonCos = (float)cos(m_vMoonPitch);
		
        A3DVECTOR3 vecMoonDir = A3DVECTOR3(vMoonCos * (float)cos(m_vDeg), vMoonSin, vMoonCos * (float)sin(m_vDeg));
        A3DVECTOR3 vecMoonPos = vecCamPos + vecMoonDir * SKY_FAR;

        A3DVECTOR3 vecMoonDirCam = pCamera->GetDir();
        A3DVECTOR3 vecMoonRight = CrossProduct(A3DVECTOR3(0.0f, 1.0f, 0.0f), vecMoonDir);
        //vecMoonDir = CrossProduct(vecMoonRight, A3DVECTOR3(0.0f, 1.0f, 0.0f));
        A3DVECTOR3 vecMoonUp = Normalize(CrossProduct(vecMoonDir, vecMoonRight));
        Orthonormalize(vecMoonDirCam, vecMoonRight, vecMoonUp);
        A3DMATRIX4 matWorld = TransformMatrix(vecMoonDirCam, vecMoonUp, vecMoonPos);
        
        m_pHLSLMain->SetTexture("g_TextureSampler", m_pMoonTexture);
        m_pHLSLMain->SetTexture("g_OccSampler", NULL);
        m_pHLSLMain->SetValue2f("g_vSampleFlags", 1, 0);
        A3DMATRIX4 matWVP = matWorld * matVP;
        m_pHLSLMain->SetConstantMatrix("g_matWVP", matWVP);
        A3DVECTOR4 vBright = A3DVECTOR4(vMoonBrightness.r / 1.5f, vMoonBrightness.g / 1.5f, vMoonBrightness.b / 1.5f, vMoonBrightness.a);
        m_pHLSLMain->SetValue4f("g_vBrightness", &vBright);

        m_pHLSLMain->Appear();

		pA3DDevice->DrawPrimitive(A3DPT_TRIANGLELIST, 0, 2);

        m_pHLSLMain->Disappear();
	}

    pA3DDevice->SetProjectionMatrix(matProjOld);

	pA3DDevice->SetFogEnable(bFog);
	pA3DDevice->SetZWriteEnable(true);
	pA3DDevice->SetZTestEnable(true);
	pA3DDevice->SetLightingEnable(true);
	pA3DDevice->SetSourceAlpha(A3DBLEND_SRCALPHA);
	pA3DDevice->SetDestAlpha(A3DBLEND_INVSRCALPHA);

	if( pCamera->IsMirrored() )
	{
		m_pA3DDevice->SetDeviceRenderState(D3DRS_CLIPPLANEENABLE, dwClipState);
	}
	return true;
}

inline A3DVECTOR3& CLAMPVECTOR(A3DVECTOR3& v, int minx, int maxx, int miny, int maxy)
{
	if( v.x < minx ) v.x = minx + 0.5f;
	else if( v.x > maxx ) v.x = (float)maxx - 0.5f;

	if( v.y < miny ) v.y = miny + 0.5f;
	else if( v.y > maxy ) v.y = (float)maxy - 0.5f;

	return v;
}

bool A3DSunMoon::RenderSolarFlare(A3DViewport * pViewport)
{
	// see if not set time yet.
	if( !m_bCanDoFlare || !m_bTimeHasBeenSet || !m_bDirHasBeenSet )
		return true;

	A3DCameraBase * pCamera = pViewport->GetCamera();

    if (!pCamera)
        return true;

    if (!m_pHLSLMain || !m_pHLSLTestOcc || !m_pOccTestDecl || !m_pTLVertDecl)
        return false;

    DWORD uScreenWidth = pViewport->GetParam()->Width;
    DWORD uScreenHeight = pViewport->GetParam()->Height;

    A3DRenderTarget* pTarget = m_pA3DDevice->GetRenderTargetMan()->RentRenderTargetColor(uScreenWidth, uScreenHeight, A3DFMT_A8R8G8B8); // liuchenglong ÁÙÊ±Ð´µÄ
    if (!pTarget)
        return false;

	A3DVECTOR3 vecCamPos, vecCamDir;
	vecCamPos = pCamera->GetPos();
	vecCamDir = pCamera->GetDir();

	A3DDevice * pA3DDevice = m_pA3DDevice;

	float	vPitch = max(m_vSunPitch, m_vMoonPitch);
	float	vSin = (float)sin(vPitch);
	float	vCos = (float)cos(vPitch);
	A3DVECTOR3 vecDir = A3DVECTOR3(vCos * (float)cos(m_vDeg), vSin, vCos * (float)sin(m_vDeg));
	A3DVECTOR3 vecRight = CrossProduct(A3DVECTOR3(0.0f, 1.0f, 0.0f), vecDir);
	A3DVECTOR3 vecUp = Normalize(CrossProduct(vecDir, vecRight));
	A3DVECTOR3 vecPos = vecCamPos + vecDir * SKY_FAR;

	float vScale;
	A3DVECTOR3 vecCenterPos;
	A3DVECTOR3 vecCorner;
	A3DMATRIX4 mat;

	if (vPitch == m_vSunPitch)
	{
		vScale = 1.0f + m_fDNFactor * 2.0f;
		mat = Scaling(vScale, vScale, vScale) * TransformMatrix(vecDir, vecUp, vecPos);
		vecCenterPos = A3DVECTOR3(0.0f, -1.0f * SUN_UNIT_SIZE, 0.0f) * mat;
		vecCorner = A3DVECTOR3(-1.0f * SUN_UNIT_SIZE, 0.0f * SUN_UNIT_SIZE, 0.0f) * mat;
	}
	else
	{
		vScale = 1.0f;
		mat = Scaling(vScale, vScale, vScale) * TransformMatrix(vecDir, vecUp, vecPos);
		vecCenterPos = A3DVECTOR3(0.0f, -1.0f * MOON_UNIT_SIZE, 0.0f) * mat;
		vecCorner = A3DVECTOR3(-1.0f * MOON_UNIT_SIZE, 0.0f * MOON_UNIT_SIZE, 0.0f) * mat;
	}

	pViewport->Transform(vecCenterPos, vecCenterPos);
	pViewport->Transform(vecCorner, vecCorner);

	A3DVIEWPORTPARAM param = *pViewport->GetParam();
	int minx, maxx, miny, maxy;
	minx = param.X; maxx = param.X + param.Width;
	miny = param.Y; maxy = param.Y + param.Height;

	float v = max(float(fabs(vecCorner.x - vecCenterPos.x)), float(fabs(vecCorner.y - vecCenterPos.y))) * 0.2f;

	int cx, cy;
	cx = (int) vecCenterPos.x;
	cy = (int) vecCenterPos.y;

	A3DTLVERTEX * pVerts;

	int i;
	const float dx[] = {0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f};
	const float dy[] = {0.0f, -1.0f, -0.5f, 1.0f, 0.0f, 0.0f};

	pA3DDevice->SetAlphaBlendEnable(false);
    pA3DDevice->SetZFunc(A3DCMP_LESSEQUAL);
	pA3DDevice->SetDeviceRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA);
	
    m_pHLSLMain->SetTexture("g_TextureSampler", NULL);
    m_pHLSLMain->SetTexture("g_OccSampler", NULL);
    m_pHLSLMain->SetValue2f("g_vSampleFlags", 0, 0);
    A3DVECTOR4 vBright(1, 1, 1, 1);
    m_pHLSLMain->SetValue4f("g_vBrightness", &vBright);

    m_pHLSLMain->Appear();

    pTarget->ApplyToDevice(0);
    pA3DDevice->Clear(D3DCLEAR_TARGET, A3DCOLORRGBA(0,0,0,0), 1.0f, 0);

	if( vecCenterPos.z >= 0.0f && vecCenterPos.z <= 1.0f )
	{
		if( !m_pStreamSunPoint->LockVertexBufferDynamic(0, 0, (BYTE **)&pVerts, NULL) )
		{
            pTarget->WithdrawFromDevice();
            m_pA3DDevice->GetRenderTargetMan()->ReturnRenderTarget(pTarget);
			a_LogOutput(1, _EA("A3DSunMoon::RenderSolarFlare, failed to lock sun point stream!"));
			return false;
		}

		for(i=0; i<6; i++)
		{
			pVerts[i * 9 + 0] = A3DTLVERTEX(A3DVECTOR3(cx + dx[i] * v - 1.0f, cy + dy[i] * v - 1.0f, 0.99999f), 0xffff0000, 0xff000000, 0.0f, 0.0f);
			pVerts[i * 9 + 1] = A3DTLVERTEX(A3DVECTOR3(cx + dx[i] * v - 1.0f, cy + dy[i] * v + 0.0f, 0.99999f), 0xffff0000, 0xff000000, 0.0f, 0.0f);
			pVerts[i * 9 + 2] = A3DTLVERTEX(A3DVECTOR3(cx + dx[i] * v - 1.0f, cy + dy[i] * v + 1.0f, 0.99999f), 0xffff0000, 0xff000000, 0.0f, 0.0f);
			pVerts[i * 9 + 3] = A3DTLVERTEX(A3DVECTOR3(cx + dx[i] * v + 0.0f, cy + dy[i] * v - 1.0f, 0.99999f), 0xffff0000, 0xff000000, 0.0f, 0.0f);
			pVerts[i * 9 + 4] = A3DTLVERTEX(A3DVECTOR3(cx + dx[i] * v + 0.0f, cy + dy[i] * v + 0.0f, 0.99999f), 0xffff0000, 0xff000000, 0.0f, 0.0f);
			pVerts[i * 9 + 5] = A3DTLVERTEX(A3DVECTOR3(cx + dx[i] * v + 0.0f, cy + dy[i] * v + 1.0f, 0.99999f), 0xffff0000, 0xff000000, 0.0f, 0.0f);
			pVerts[i * 9 + 6] = A3DTLVERTEX(A3DVECTOR3(cx + dx[i] * v + 1.0f, cy + dy[i] * v - 1.0f, 0.99999f), 0xffff0000, 0xff000000, 0.0f, 0.0f);
			pVerts[i * 9 + 7] = A3DTLVERTEX(A3DVECTOR3(cx + dx[i] * v + 1.0f, cy + dy[i] * v + 0.0f, 0.99999f), 0xffff0000, 0xff000000, 0.0f, 0.0f);
			pVerts[i * 9 + 8] = A3DTLVERTEX(A3DVECTOR3(cx + dx[i] * v + 1.0f, cy + dy[i] * v + 1.0f, 0.99999f), 0xffff0000, 0xff000000, 0.0f, 0.0f);
		}

		m_pStreamSunPoint->UnlockVertexBufferDynamic();
        m_pStreamSunPoint->Appear(0, false);
        m_pTLVertDecl->Appear();

        pA3DDevice->SetZTestEnable(true);
		pA3DDevice->SetZWriteEnable(false);
        pA3DDevice->SetZFunc(A3DCMP_LESSEQUAL);
		pA3DDevice->DrawPrimitive(A3DPT_POINTLIST, 0, 9 * 6);
	}
    pTarget->WithdrawFromDevice();

    m_pHLSLMain->Disappear();

	pA3DDevice->SetZTestEnable(true);
	pA3DDevice->SetZWriteEnable(true);
	
	pA3DDevice->SetAlphaBlendEnable(true);
	pA3DDevice->SetDeviceRenderState(D3DRS_COLORWRITEENABLE, 
		D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE|D3DCOLORWRITEENABLE_ALPHA);


    //if (GetAsyncKeyState(VK_F8) < 0)
    //    D3DXSaveTextureToFile("frame.dds", D3DXIFF_DDS, pTarget->GetTargetTexture(), NULL);

    m_pFlareAlphaTarget->ApplyToDevice();
	
	param.X = 0;
	param.Y = 0;
	param.Width = 1;
	param.Height = 1;
	param.MinZ = 0.0f;
	param.MaxZ = 1.0f;
	pA3DDevice->SetViewport(&param);
	pA3DDevice->SetZWriteEnable(false);
	pA3DDevice->SetZTestEnable(false);

	m_pStreamFlareAlpha->Appear(0, false);
    m_pOccTestDecl->Appear();

	FLARE_ALPHA_VERT * pVert;
	if( !m_pStreamFlareAlpha->LockVertexBufferDynamic(0, 0, (BYTE **)&pVert, NULL) )
	{
		a_LogOutput(1, _EA("A3DSunMoon::Init, failed to lock flare alpha stream!"));
		m_pFlareAlphaTarget->WithdrawFromDevice();
        m_pA3DDevice->GetRenderTargetMan()->ReturnRenderTarget(pTarget);
		return false;
	}
	pVert[0].pos = A3DVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
    m_pStreamFlareAlpha->UnlockVertexBufferDynamic();

    float vSamplePos[12];

	for(i=0; i<6; i++)
	{
        vSamplePos[i * 2 + 0] = (cx + dx[i] * v + 0.5f) / uScreenWidth;
        vSamplePos[i * 2 + 1] = (cy + dy[i] * v + 0.5f) / uScreenHeight;
	}

    DWORD dwTickCount =  GetTickCount();   
    DWORD dwTickTime = dwTickCount - m_dwLastTickCount;
    m_dwLastTickCount = dwTickCount;

    A3DVECTOR4 vBrightness(1, 1, 1, 0);
    if( m_bFirstAlpha )
	{
		vBrightness.w = 1.0f;
		m_bFirstAlpha = false;
	}
	else
    {
		vBrightness.w = dwTickTime / 200.0f;
    }
    m_pA3DDevice->SetSourceAlpha(A3DBLEND_SRCALPHA);
    m_pA3DDevice->SetDestAlpha(A3DBLEND_INVSRCALPHA);
    m_pHLSLTestOcc->SetTexture("g_OccSampler", pTarget);
    m_pHLSLTestOcc->SetValue2f("g_vSampleFlags", 1, 1);
    m_pHLSLTestOcc->SetConstantArrayF("g_vSamplePos", (A3DVECTOR4*)vSamplePos, 3);
    m_pHLSLTestOcc->SetValue4f("g_vBrightness", &vBrightness);
    m_pHLSLTestOcc->Appear();
    pA3DDevice->SetTextureAddress(0, A3DTADDR_CLAMP, A3DTADDR_CLAMP);

	pA3DDevice->DrawPrimitive(A3DPT_POINTLIST, 0, 1);

    m_pHLSLTestOcc->Disappear();
    pA3DDevice->SetTextureAddress(0, A3DTADDR_WRAP, A3DTADDR_WRAP);

	m_pFlareAlphaTarget->WithdrawFromDevice();
    m_pA3DDevice->GetRenderTargetMan()->ReturnRenderTarget(pTarget);
	pViewport->Active();

	pA3DDevice->SetLightingEnable(false);
	pA3DDevice->SetSourceAlpha(A3DBLEND_SRCALPHA);
	pA3DDevice->SetDestAlpha(A3DBLEND_ONE);
	bool bFog = pA3DDevice->GetFogEnable();
	pA3DDevice->SetFogEnable(false);

	BYTE sunVis = BYTE(m_vSunVisibility * 255.0f);
	BYTE moonVis = BYTE(m_vMoonVisibility * 255.0f);

    A3DCOLORVALUE vSunBrightness = (a3d_ColorRGBAToColorValue(m_colorSun));
    vSunBrightness.a = m_vSunVisibility;
    A3DCOLORVALUE vMoonBrightness = (a3d_ColorRGBAToColorValue(m_colorMoon));
    vMoonBrightness.a = m_vMoonVisibility;

    if (m_pA3DDevice->GetA3DEngine()->GetA3DEnvironment()->GetIsUnderWater())
    {
        vSunBrightness.a *= 0.5f;
        vMoonBrightness.a *= 0.5f;
    }

	if( m_vSunPitch > 0.0f && m_vSunVisibility > 0 )
	{
		// render lens flares
		UpdateLensFlares(pViewport, vecCamPos, vecCamDir, A3DVECTOR3(0.0f, -1.0f * SUN_UNIT_SIZE, 0.0f) * mat);

        m_pTLVertDecl->Appear();
		m_pStreamFlares->Appear(0, false);
		
        m_pHLSLMain->SetTexture("g_TextureSampler", m_pFlaresTexture);
        m_pHLSLMain->SetTexture("g_OccSampler", m_pFlareAlphaTarget);
        m_pHLSLMain->SetValue2f("g_vSampleFlags", 1, 1);
        A3DVECTOR4 vBright = A3DVECTOR4(vSunBrightness.r * 0.8f, vSunBrightness.g * 0.8f, vSunBrightness.b * 0.8f, vSunBrightness.a);
        m_pHLSLMain->SetValue4f("g_vBrightness", &vBright);

        m_pHLSLMain->Appear();

		pA3DDevice->DrawPrimitive(A3DPT_TRIANGLELIST, 0, 15 * 2);
	
        m_pHLSLMain->Disappear();
	}

	if( m_vMoonPitch > 0.0f  && m_vMoonVisibility > 0)
	{
        m_pVertDecl->Appear();
		m_pStreamMoon->Appear(0, false);
		
		float vMoonScale = 1.0f;
		A3DMATRIX4 mat = Scaling(vMoonScale, vMoonScale, vMoonScale) * TransformMatrix(vecDir, vecUp, vecPos);
		pA3DDevice->SetWorldMatrix(mat);
		
        m_pHLSLMain->SetTexture("g_TextureSampler", m_pMoonFlareTexture);
        m_pHLSLMain->SetTexture("g_OccSampler", m_pFlareAlphaTarget);
        m_pHLSLMain->SetValue2f("g_vSampleFlags", 1, 1);
        A3DMATRIX4 matWVP = pA3DDevice->GetWorldMatrix() * pA3DDevice->GetViewMatrix() * pA3DDevice->GetProjectionMatrix();
        m_pHLSLMain->SetConstantMatrix("g_matWVP", matWVP);
        A3DVECTOR4 vBright = A3DVECTOR4(vMoonBrightness.r * 0.5f, vMoonBrightness.g * 0.5f, vMoonBrightness.b * 0.5f, vMoonBrightness.a);
        m_pHLSLMain->SetValue4f("g_vBrightness", &vBright);

        m_pHLSLMain->Appear();

		pA3DDevice->DrawPrimitive(A3DPT_TRIANGLELIST, 0, 2);

        m_pHLSLMain->Disappear();
	}

	if( m_fDNFactor != 0.0f && m_fDNFactor != 1.0f )
	{
		float vSunShine = 1.0f;
		if( m_fDNFactor < 0.1f )
		{					
			vSunShine = 0.0f;
		}
		else if( m_fDNFactor < 0.6f )
		{
			vSunShine = (m_fDNFactor - 0.1f) / 0.5f;
		}
		else if( m_fDNFactor < 0.8f )
		{
			vSunShine = (0.8f - m_fDNFactor) / 0.2f;
		}
		else
		{
			vSunShine = 0.0f;
		}
		
		if( m_fDNFactorDest == 1.0f )
			vSunShine *= 0.5f; // sunset, just not too bright
		else
			vSunShine *= 0.6f; // sun rise, just make a little bright

		m_pStreamSunShine->Appear(0, false);
		
		float vSunScale = 1.0f + m_fDNFactor * 2.0f;
		A3DMATRIX4 mat = Scaling(vSunScale, vSunScale, vSunScale) * TransformMatrix(vecDir, vecUp, vecPos);
		pA3DDevice->SetWorldMatrix(mat);
		
        m_pHLSLMain->SetTexture("g_TextureSampler", m_pSunShineTexture);
        m_pHLSLMain->SetTexture("g_OccSampler", m_pFlareAlphaTarget);
        m_pHLSLMain->SetValue2f("g_vSampleFlags", 1, 1);
        A3DVECTOR4 vBright = A3DVECTOR4(vSunBrightness.r * vSunShine, vSunBrightness.g * vSunShine, vSunBrightness.b * vSunShine, vSunBrightness.a);
        m_pHLSLMain->SetValue4f("g_vBrightness", &vBright);
        m_pHLSLMain->Appear();

		pA3DDevice->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, 8, 0, 6);
        m_pHLSLMain->Disappear();

	}
	
	pA3DDevice->SetFogEnable(bFog);
	pA3DDevice->SetZWriteEnable(true);
	pA3DDevice->SetZTestEnable(true);
	pA3DDevice->SetLightingEnable(true);
	pA3DDevice->SetSourceAlpha(A3DBLEND_SRCALPHA);
	pA3DDevice->SetDestAlpha(A3DBLEND_INVSRCALPHA);
	return true;
}

bool A3DSunMoon::Tick(DWORD dwDeltaTime)
{
	double vDeltaTime = 0.001 * dwDeltaTime / 3600.0 / 24.0 * TIME_SCALE;

	m_vTimeOfTheDay += vDeltaTime;
	while( m_vTimeOfTheDay > 1.0 )
		m_vTimeOfTheDay -= 1.0;

	UpdateWithTime();

	// update sun moon visibility
	if( m_bSunVisible )
		m_vSunVisibility += dwDeltaTime * 0.0003f;
	else
		m_vSunVisibility -= dwDeltaTime * 0.0003f;
	a_Clamp(m_vSunVisibility, 0.0f, 1.0f);

	if( m_bMoonVisible )
		m_vMoonVisibility += dwDeltaTime * 0.0003f;
	else
		m_vMoonVisibility -= dwDeltaTime * 0.0003f;
	a_Clamp(m_vMoonVisibility, 0.0f, 1.0f);

	return true;
}

bool A3DSunMoon::SetTimeOfTheDay(float vTime)
{
	m_bTimeHasBeenSet = true;

	while(vTime < 0.0f )
		vTime += 1.0f;
	while(vTime > 1.0f )
		vTime -= 1.0f;
	m_vTimeOfTheDay = vTime;

	UpdateWithTime();
	return true;
}

bool A3DSunMoon::SetSunColor(A3DCOLOR color)
{
	m_colorSun = color;
	return true;
}

bool A3DSunMoon::SetMoonColor(A3DCOLOR color)
{
	m_colorMoon = color;
	return true;
}

bool A3DSunMoon::SetLightDir(const A3DVECTOR3& vecLightDir)
{
	m_bDirHasBeenSet = true;

	m_vDeg = (float)atan2(-vecLightDir.z, -vecLightDir.x);
	m_vecLightDir = vecLightDir;
	return true;
}

bool A3DSunMoon::UpdateWithTime()
{
	float vTimeOfTheDay = (float)m_vTimeOfTheDay;

	float vPitch;
	if( vTimeOfTheDay < DAWN_TIME )
	{
		m_vSunPitch = 0.0f;
		m_vMoonPitch = (1.0f - vTimeOfTheDay / DAWN_TIME) * MOON_MAX_PITCH;
		vPitch = m_vMoonPitch;
	}
	else if( vTimeOfTheDay < MID_NOON )
	{
		m_vSunPitch = (vTimeOfTheDay - DAWN_TIME) / (MID_NOON - DAWN_TIME) * SUN_MAX_PITCH;
		m_vMoonPitch = 0.0f;
		vPitch = m_vSunPitch;
	}
	else if( vTimeOfTheDay < SUN_SET )
	{
		m_vSunPitch = (1.0f - (vTimeOfTheDay - MID_NOON) / (SUN_SET - MID_NOON)) * SUN_MAX_PITCH;
		m_vMoonPitch = 0.0f;
		vPitch = m_vSunPitch;
	}
	else
	{
		m_vSunPitch = 0.0f;
		m_vMoonPitch = (vTimeOfTheDay - SUN_SET) / (MID_NIGHT - SUN_SET) * MOON_MAX_PITCH;
		vPitch = m_vMoonPitch;
	}

	// update day night factor
	if( vTimeOfTheDay < NIGHT_DAY_START )
	{
		m_fDNFactor = 1.0f;
		m_fDNFactorDest = 1.0f;
	}
	else if( vTimeOfTheDay < NIGHT_DAY_END )
	{
		m_fDNFactor = 1.0f - (vTimeOfTheDay - NIGHT_DAY_START) / (NIGHT_DAY_END - NIGHT_DAY_START);
		m_fDNFactorDest = 0.0f;
	}
	else if( m_vTimeOfTheDay < DAY_NIGHT_START )
	{
		m_fDNFactor = 0.0f;
		m_fDNFactorDest = 0.0f;
	}
	else if( vTimeOfTheDay < DAY_NIGHT_END )
	{
		m_fDNFactor = (vTimeOfTheDay - DAY_NIGHT_START) / (DAY_NIGHT_END - DAY_NIGHT_START);
		m_fDNFactorDest = 1.0f;
	}
	else
	{
		m_fDNFactor = 1.0f;
		m_fDNFactorDest = 1.0f;
	}

	float vCos = (float)cos(vPitch);
	m_vecLightDir.y = -(float)sin(vPitch);
	m_vecLightDir.x = -vCos * (float)cos(m_vDeg);
	m_vecLightDir.z = -vCos * (float)sin(m_vDeg);
	return true;
}

int A3DSunMoon::GetDNTransTime()
{
	if( m_fDNFactorDest == 0.0f )
	{
		return int((NIGHT_DAY_END - NIGHT_DAY_START) * 24 * 3600 * (1.0f / TIME_SCALE) * 1000);
	}
	else 
	{
		return int((DAY_NIGHT_END - DAY_NIGHT_START) * 24 * 3600 * (1.0f / TIME_SCALE) * 1000);
	}

	return 0;
}

bool A3DSunMoon::UpdateLensFlares(A3DViewport * pViewport, const A3DVECTOR3 vecCamPos, const A3DVECTOR3 vecCamDir, const A3DVECTOR3 vecSunPos)
{
	const float vSize[] = {
		0.06f, 
		0.45f, 0.35f,
		0.04f, 0.09f, 0.05f, 0.04f, 0.015f, 
		0.02f, 0.07f, 0.12f, 0.03f,
		0.03f, 0.15f, 0.3f};
	const float vDis[] = {
		860.0f, 
		100.0f, 100.0f,
		40.0f, 25.0f, 20.0f, 10.0f, 4.0f, 
		-2.5f, -4.0f, -4.4f, -4.5f, 
		-6.2f, -8.0f, -9.0f};
	
#define WHITERING {0.75f, 0.25f}
#define BLUEDISK {0.0f, 0.0f}
#define BLUEDISKWEAK {0.25f, 0.0f}
#define BROWNRING {0.5f, 0.0f}
#define WHITEGRADIENT {0.75f, 0.0f}
#define BROWNDISK {0.0f, 0.25f}
#define GREENRING {0.25f, 0.25f}
#define RAINBOWRING {0.5f, 0.25f}
#define WHITERING {0.75f, 0.25f}
#define BLUEGRADIENT {0.0f, 0.5f}
#define WHITEREDRING {0.25f, 0.5f}
#define WHITEREDRING2 {0.5f, 0.5f}

	const float uv[][2] = { 
		WHITERING,
		WHITEREDRING, WHITEREDRING2,
		BLUEDISK, BLUEDISKWEAK, BLUEDISK, BROWNRING, WHITEGRADIENT,
		WHITEGRADIENT, BROWNRING, BROWNRING, BROWNRING,
		BLUEGRADIENT, GREENRING, RAINBOWRING
	};

	const float ts[] = {
		0.25f, 
		0.25f, 0.5f, 
		0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 
		0.25f, 0.25f, 0.25f, 0.25f, 
		0.25f, 0.25f, 0.25f};

	A3DVIEWPORTPARAM param = *pViewport->GetParam();
	A3DVECTOR3 vecCenterPos = vecCamPos + vecCamDir * 15.0f;
	A3DVECTOR3 vecRayDir = Normalize(vecSunPos - vecCenterPos);

	A3DTLVERTEX * pVerts;
	if( !m_pStreamFlares->LockVertexBufferDynamic(0, 0, (BYTE **)&pVerts, NULL) )
	{
		return false;
	}

	int i;
	for(i=0; i<15; i++)
	{
		float u = uv[i][0];
		float v = uv[i][1];

		A3DVECTOR3 vecPos = vecCenterPos + vecRayDir * vDis[i];
		A3DVECTOR3 vecScreenPos;

		A3DVECTOR3 vecHalfX(0.0f);
		A3DVECTOR3 vecHalfY(0.0f);
		if( pViewport->Transform(vecPos, vecScreenPos) )
		{
			vecScreenPos.x = -1.0f;
			vecScreenPos.y = -1.0f;
		}
		else
		{
			vecScreenPos.z = 0.0f;
            vecScreenPos.y -= SUN_UNIT_SIZE * 10;
			vecHalfX = A3DVECTOR3(vSize[i] * param.Height, 0.0f, 0.0f);
			vecHalfY = A3DVECTOR3(0.0f, vSize[i] * param.Height, 0.0f);
		}
	
		pVerts[i * 6 + 0] = A3DTLVERTEX(vecScreenPos - vecHalfX - vecHalfY, 0xffffffff, 0xff000000, u, v);
		pVerts[i * 6 + 1] = A3DTLVERTEX(vecScreenPos + vecHalfX - vecHalfY, 0xffffffff, 0xff000000, u + ts[i], v);
		pVerts[i * 6 + 2] = A3DTLVERTEX(vecScreenPos - vecHalfX + vecHalfY, 0xffffffff, 0xff000000, u, v + ts[i]);
		pVerts[i * 6 + 3] = pVerts[i * 6 + 2];
		pVerts[i * 6 + 4] = pVerts[i * 6 + 1];
		pVerts[i * 6 + 5] = A3DTLVERTEX(vecScreenPos + vecHalfX + vecHalfY, 0xffffffff, 0xff000000, u + ts[i], v + ts[i]);
	}

	m_pStreamFlares->UnlockVertexBufferDynamic();

	return true;
}

A3DVECTOR3 A3DSunMoon::ComputeSunDir() const
{
    float	vPitch = max(m_vSunPitch, m_vMoonPitch);
    float	vSin = (float)sin(vPitch);
    float	vCos = (float)cos(vPitch);
    A3DVECTOR3 vecDir = A3DVECTOR3(vCos * (float)cos(m_vDeg), vSin, vCos * (float)sin(m_vDeg));
    return -vecDir;
}