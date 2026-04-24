/*
 * FILE: A3DPostCC.cpp
 *
 * DESCRIPTION: 色彩校正后处理
 *
 * CREATED BY: Panyupin, 2012/2/1
 *
 * HISTORY: 
 *
 * Copyright (c) 2012 Archosaur Studio, All Rights Reserved.
 */
#include "A3DPostCC.h"
#include "A3DPostSystem.h"
#include "A3DVolumeTexture.h"


#ifdef _ANGELICA_2_1
#include "A3DHLSLWrapper.h"
static const char* szHLSL = "shaders\\HLSL\\PostProcess\\Post_ColorCorrection.hlsl";
static const char* szDefaultCCTextuer = "shaders\\hlsl\\PostProcess\\ccdefault.dds";
#else
#include "A3DHLSL.h"
static const char* szHLSL = "shaders\\2.2\\HLSL\\PostProcess\\Post_ColorCorrection.hlsl";
static const char* szDefaultCCTextuer = "Shaders\\Textures\\ccdefault.dds";
#endif


extern ALog g_A3DErrLog;

A3DPostCC::~A3DPostCC()
{

}

bool A3DPostCC::Init( A3DPostSystem* pSystem )
{
	m_pSystem = pSystem;
	m_pHLSLcc = CreateA3DHLSL_Common(NULL, NULL, szHLSL, "ps_main", 0, NULL, 
		m_pSystem->GetA3DDevice(), m_pSystem->GetD3D9VertElement());

	if(!m_pHLSLcc)
	{
		g_A3DErrLog.Log("A3DPostBloom::Init Failed! Post_ColorCorrection.hlsl load failed!");
		Release();
		return false;
	}


	if(!SetCCTexture(szDefaultCCTextuer, szDefaultCCTextuer))
	{
		return false;
	}

	return true;
}

void A3DPostCC::Release()
{
	A3DRELEASE(m_pColorCorrectionTex1);
	A3DRELEASE(m_pColorCorrectionTex2);

	ReleaseA3DHLSL_Common(m_pSystem->GetA3DDevice(), m_pHLSLcc);
}

A3DPOST_EFFECT A3DPostCC::GetEffectType() const
{
	return A3DPOST_EFFECT_CC;
}

bool A3DPostCC::Render( A3DViewport* pViewport )
{	
	A3DDevice* pDev = m_pSystem->GetA3DDevice();
	A3DRenderTarget* pSrc = m_pSystem->GetSrcRT();
	A3DRenderTarget* pDst = m_pSystem->GetDstRT();
	
	if(!m_pColorCorrectionTex1 || !m_pColorCorrectionTex2)
	{
		return false;
	}

	//设置渲染状态
	pDev->SetAlphaBlendEnable(false);
	pDev->SetZTestEnable(false);
	pDev->SetZWriteEnable(false);
	pDev->SetFaceCull(A3DCULL_NONE);

	//设置RnederTarget
	pDst->ApplyToDevice();

	//设置Shader参数
	m_pSystem->SetupCommonVS(m_pSystem->GetWidth(), m_pSystem->GetHeight());
	m_pSystem->GetCommonVS()->Appear();

	m_pHLSLcc->Setup1f("fFactor", m_fFactor);
	
#ifdef _ANGELICA_2_2
	if(m_pSystem->GetScreenIlumRT())
		m_pHLSLcc->Setup1f("enableScreenIlum", 1.0f);
	else
		m_pHLSLcc->Setup1f("enableScreenIlum", 0.0f);
#endif
	
	m_pHLSLcc->Appear();	
	
	pSrc->AppearAsTexture(0);
	m_pColorCorrectionTex1->Appear(1);
	m_pColorCorrectionTex2->Appear(2);

#ifdef _ANGELICA_2_2
	if(m_pSystem->GetScreenIlumRT())
		m_pSystem->GetScreenIlumRT()->AppearAsTexture(3);
#endif

	pDev->SetTextureFilterType(0, A3DTEXF_POINT);

	pDev->SetTextureFilterType(1, A3DTEXF_LINEAR);
	pDev->SetTextureAddress(1, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	pDev->SetSamplerState(1, D3DSAMP_ADDRESSW, A3DTADDR_CLAMP);

	pDev->SetTextureFilterType(2, A3DTEXF_LINEAR);
	pDev->SetTextureAddress(2, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	pDev->SetSamplerState(2, D3DSAMP_ADDRESSW, A3DTADDR_CLAMP);

	pDev->SetTextureFilterType(3, A3DTEXF_LINEAR);
	pDev->SetTextureAddress(3, A3DTADDR_CLAMP, A3DTADDR_CLAMP);


	//绘制方块
	m_pSystem->RenderQuad();

	m_pSystem->GetCommonVS()->Disappear();
	m_pHLSLcc->Disappear();

	pDst->WithdrawFromDevice();

	//交换RT
	m_pSystem->SwapSrcDstRT();

	return true;
}

void A3DPostCC::SetFactor( float f )
{
	f = min(1.f, f);
	f = max(0.f, f);
	m_fFactor = f;
}

bool A3DPostCC::IsHardwareSupport(A3DDevice* pA3DDev)
{
	if (!pA3DDev->TestVertexShaderVersion(2, 0) ||
		!pA3DDev->TestPixelShaderVersion(2, 0))
		return false;
	return true;
}

bool A3DPostCC::SetCCTexture( const char* pVolTex1, const char* pVolTex2 )
{
	if(!SetCCTexture1(pVolTex1))
		return false;
	if(!SetCCTexture2(pVolTex2))
		return false;
	return true;
}

bool A3DPostCC::SetCCTexture1( const char* pVolTex1 )
{
	if(m_strCurTex1 == AString(pVolTex1)
#ifdef _ANGELICA_2_2
		&& !m_bIsManual1
#endif		
		)
		return true;
	if(!ReLoadVolumeTexture(&m_pColorCorrectionTex1, pVolTex1))
		return false;

	m_strCurTex1 = AString(pVolTex1);
#ifdef _ANGELICA_2_2
	m_bIsManual1 = false;
#endif
	return true;
}

bool A3DPostCC::SetCCTexture2( const char* pVolTex2 )
{
	if(m_strCurTex2 == AString(pVolTex2)
#ifdef _ANGELICA_2_2
&& !m_bIsManual2
#endif
		)
		return true;
	if(!ReLoadVolumeTexture(&m_pColorCorrectionTex2, pVolTex2))
		return false;

	m_strCurTex2 = AString(pVolTex2);
#ifdef _ANGELICA_2_2
	m_bIsManual2 = false;
#endif
	return true;
}

//制作VolumeTexture
static const float lumR = 0.3086f;
static const float lumG = 0.6094f;
static const float lumB = 0.0820f;
static const float rad = D3DX_PI / 180.0f;

D3DXMATRIX	m_preHueMatrix;
D3DXMATRIX	m_postHueMatrix;

static void RotateColor(D3DXMATRIX &matrix,float phi,int axiIndex)
{
	phi *= rad;
	D3DXMATRIX m;
	switch(axiIndex)
	{
	case 0:// x轴
		{
			D3DXMatrixRotationX(&m,phi);
		}
		break;
	case 1:// y轴
		{
			D3DXMatrixRotationY(&m,phi);
		}
		break;
	case 2:// z轴
		{
			D3DXMatrixRotationZ(&m,phi);
		}
		break;
	default:
		{
			D3DXMatrixRotationX(&m,phi);
		}
	}
	D3DXMatrixMultiply(&matrix,&matrix,&m);
}

static void ShearColor(D3DXMATRIX& matrix,int whichColor,int color1,int color2)
{
	D3DXMATRIX mShearBlue;
	D3DXMatrixIdentity(&mShearBlue);

	switch(whichColor)
	{
	case 0:// 去掉红色
		mShearBlue.m[1][0] = (float)color1;
		mShearBlue.m[2][0] = (float)color2;
		break;
	case 1:// 剔除绿色
		mShearBlue.m[0][1] = (float)color1;
		mShearBlue.m[2][1] = (float)color2;
		break;
	case 2:// 剔除蓝色
		mShearBlue.m[0][2] = (float)color1;
		mShearBlue.m[1][2] = (float)color2;
		break;
	default:
		mShearBlue.m[1][0] = (float)color1;
		mShearBlue.m[2][0] = (float)color2;
		break;
	}


	D3DXMatrixMultiply(&matrix,&matrix,&mShearBlue);

}

bool A3DPostCC::ReLoadVolumeTexture( A3DVolumeTexture** ppVolumeTexture, const char* szFileName )
{
	A3DVolumeTexture*& pTextuer = *ppVolumeTexture;
	A3DRELEASE(pTextuer);

	if(AString(szFileName) == "")
	{
		pTextuer = CreateDefaultCCTexture(m_pSystem->GetA3DDevice());
		if(!pTextuer)
		{
			return false;
		}
		return true;
	}

	pTextuer = new A3DVolumeTexture();
	if(pTextuer == NULL || pTextuer->Init(m_pSystem->GetA3DDevice()) == false)
	{
		g_A3DErrLog.Log("A3DPostCC::SetCCTexture, can not init Volume Texture.");
		A3DRELEASE(pTextuer);
		return false;
	}
	if(!pTextuer->Load(szFileName))
	{
		g_A3DErrLog.Log("A3DPostCC::SetCCTexture, can not load Volume Texture[%s].", szFileName);
		A3DRELEASE(pTextuer);
		pTextuer = CreateDefaultCCTexture(m_pSystem->GetA3DDevice());
		if(!pTextuer)
		{
			return false;
		}
		return true;
	}
	return true;
}

A3DVolumeTexture* A3DPostCC::CreateDefaultCCTexture( A3DDevice* pDev )
{
	int nSize = 32;
	A3DVolumeTexture*pTextuer = new A3DVolumeTexture();

	if(!pTextuer->Create(pDev, nSize, nSize, nSize, A3DFMT_A8R8G8B8, 1))
	{
		A3DRELEASE(pTextuer);
		return NULL;
	}

	void* pData;
	int nSlicePitch;
	int nRowPitch;
	if(!pTextuer->LockBox(0, &pData, &nSlicePitch, &nRowPitch, 0))
	{
		A3DRELEASE(pTextuer);
		return NULL;
	}
	unsigned char* pSlice = (unsigned char*)pData;
	for(int w = 0; w < nSize; w++)
	{
		unsigned char* pRow = pSlice;
		for(int v = 0; v < nSize; v++)
		{
			for(int u = 0; u < nSize; u++)
			{
				DWORD* color = &(((DWORD*)pRow)[u]);
				unsigned char* pByte = (unsigned char*)(color);
				unsigned char& ca = pByte[3];
				unsigned char& cr = pByte[2];
				unsigned char& cg = pByte[1];
				unsigned char& cb = pByte[0];
				ca = 255;
				cr = (unsigned char)(u * 255 / nSize);
				cg = (unsigned char)(v * 255 / nSize);
				cb = (unsigned char)(w * 255 / nSize);
			}
			pRow += nRowPitch;
		}
		pSlice += nSlicePitch;
	}
	pTextuer->UnlockBox(0);
	return pTextuer;
}



#ifdef _ANGELICA_2_2
bool A3DPostCC::CreateCCVolumeTexture( A3DDevice* pDev, const char* szOutputTextureFileName,
									  const char* szInputTextureFileName, const A3DPOST_EFFECT_CC_PARAM::ManualParam& manualParam)
{
	A3DVolumeTexture* pVolumeTex = NULL;
	if(!szInputTextureFileName)
	{
		pVolumeTex = CreateDefaultCCTexture(pDev);
		if( !pVolumeTex)
			return false;
	}
	else
	{
		pVolumeTex = new A3DVolumeTexture;
		pVolumeTex->Init(pDev);
		if(!pVolumeTex->Load(szInputTextureFileName))
		{
			A3DRELEASE(pVolumeTex);
			return false;
		}
	}

	pVolumeTex = ConvertVolumeTextureByManualParam(pDev, pVolumeTex, manualParam);
	pVolumeTex->ExportToDDSFile(szOutputTextureFileName);
	A3DRELEASE(pVolumeTex);

	return true;

}

A3DVolumeTexture* A3DPostCC::ConvertVolumeTextureByManualParam(A3DDevice* pDev, A3DVolumeTexture* pVolumeTex,
															   const A3DPOST_EFFECT_CC_PARAM::ManualParam& manualParam )
{
	HRESULT r;

	if(!pVolumeTex)
	{
		pVolumeTex = CreateDefaultCCTexture(pDev);
		if(!pVolumeTex)
			return NULL;
	}

	D3DXMATRIX m_colorMatrix;
	D3DXMatrixIdentity(&m_colorMatrix);

	D3DXMATRIX grayMatrix;
	D3DXMatrixIdentity(&grayMatrix);
	for(int i = 0;i < 3;i++)
	{
		grayMatrix.m[0][i] = lumR;
		grayMatrix.m[1][i] = lumG;
		grayMatrix.m[2][i] = lumB;
	}
	// 是否变灰色
	if(manualParam.bGrey)
	{
		D3DXMatrixMultiply(&m_colorMatrix,&grayMatrix,&m_colorMatrix);
	}

	// 饱和度
	float fSaturation = (manualParam.nSaturation + 100) / 100.0f;
	if(manualParam.nSaturation > 0)
		fSaturation = fSaturation * fSaturation;
	float satCompl = 1.0f - fSaturation;
	float satComplR = lumR * satCompl;
	float satComplG = lumG * satCompl;
	float satComplB = lumB * satCompl;


	D3DXMATRIX mSaturation;
	D3DXMatrixIdentity(&mSaturation);
	mSaturation.m[0][0] = satComplR + fSaturation;
	mSaturation.m[0][1] = satComplR;
	mSaturation.m[0][2] = satComplR;
	mSaturation.m[1][0] = satComplG;
	mSaturation.m[1][1] = satComplG + fSaturation;
	mSaturation.m[1][2] = satComplG;
	mSaturation.m[2][0] = satComplB;
	mSaturation.m[2][1] = satComplB;
	mSaturation.m[2][2] = satComplB+ fSaturation;

	D3DXMatrixMultiply(&m_colorMatrix,&mSaturation,&m_colorMatrix);


	//InitHue
	const float greenRotation = 35.0f;
	D3DXMatrixIdentity(&m_preHueMatrix);

	RotateColor(m_preHueMatrix,45.0,0);
	RotateColor(m_preHueMatrix,-greenRotation,1);

	D3DXVECTOR3 lum;
	lum.x = lumR;
	lum.y = lumG;
	lum.z = lumB;

	D3DXVec3TransformCoord(&lum,&lum,&m_preHueMatrix);

	float red = lum.x / lum.z;
	float green = lum.y / lum.z;

	ShearColor(m_preHueMatrix,2,red,green);

	D3DXMatrixIdentity(&m_postHueMatrix);

	ShearColor(m_postHueMatrix,2,-red,-green);

	RotateColor(m_postHueMatrix,greenRotation,1);

	RotateColor(m_postHueMatrix,-45.0,0);

	// 色相矩阵
	D3DXMatrixMultiply(&m_colorMatrix,&m_colorMatrix,&m_preHueMatrix);
	RotateColor(m_colorMatrix, manualParam.nHue, 2);
	D3DXMatrixMultiply(&m_colorMatrix,&m_colorMatrix,&m_postHueMatrix);

	// Lock进行变换.
	D3DVOLUME_DESC vd;
	pVolumeTex->GetD3DVolumeTexture()->GetLevelDesc(0, &vd);
	int width = vd.Width;
	int height = vd.Height;
	int depth = vd.Depth;

	if(width != depth || width != height)
		return NULL;

	int nContrast = manualParam.nContrast;
	nContrast += 100;
	/*D3DLOCKED_BOX lockedBox;*/
	void* pBits;
	int nSlicePitch;
	int nRowPitch;
	pVolumeTex->LockBox(0, &pBits, &nSlicePitch, &nRowPitch, NULL, 0);
	unsigned char* pSlice = (unsigned char*)pBits;
	for(int w = 0; w < depth; w++)
	{
		unsigned char* pRow = pSlice;
		for(int v = 0; v < height; v++)
		{
			for(int u = 0; u < width; u++)
			{
				DWORD* color = &(((DWORD*)pRow)[u]);
				unsigned char* pByte = (unsigned char*)(color);
				unsigned char& cr = pByte[2];
				unsigned char& cg = pByte[1];
				unsigned char& cb = pByte[0];
				//Shader中做的事情
				{
					//float4 texcolor = tex2D(Tex0S, texpos);
					D3DXVECTOR4 texcolor(cr / 255.f, cg / 255.f, cb / 255.f, 1.f);

					D3DXVec4Transform(&texcolor, &texcolor, &m_colorMatrix);
					double fStep = nContrast * 0.01;

					if (nContrast > 99) 
					{
						fStep = fStep * fStep * fStep;
					}
					float fStepB = floor(manualParam.nBright * 0.01 * 128)/255.0f;

					texcolor.x += fStepB;
					texcolor.y += fStepB;
					texcolor.z += fStepB;

					texcolor.x = floor((texcolor.x - 0.5) * 255 * fStep)/255.0  + 0.5;
					texcolor.x = (texcolor.x < 0.0f) ? 0.0 : (texcolor.x  > 1.0) ? 1.0 : texcolor.x;
					texcolor.y = floor((texcolor.y - 0.5) * 255 * fStep)/255.0 + 0.5;
					texcolor.y = (texcolor.y < 0.0f) ? 0.0 : (texcolor.y > 1.0) ? 1.0 : texcolor.y;
					texcolor.z = floor((texcolor.z - 0.5) * 255 * fStep)/255.0  + 0.5;
					texcolor.z = (texcolor.z < 0.0f) ? 0.0 : (texcolor.z  > 1.0) ? 1.0 : texcolor.z;

					if(manualParam.bOpposition)
					{
						texcolor.x = 1.0 - texcolor.x;
						texcolor.y = 1.0 - texcolor.y;
						texcolor.z = 1.0 - texcolor.z;
					}
					cr = texcolor.x * 255;
					cg = texcolor.y * 255;
					cb = texcolor.z * 255;
				}
			}
			pRow += nRowPitch;
		}
		pSlice += nSlicePitch;
	}
	pVolumeTex->UnlockBox(0);
	return pVolumeTex;
}

bool A3DPostCC::SetCCTexture1ByManualParam( const A3DPOST_EFFECT_CC_PARAM::ManualParam& manualParam )
{
	if(m_bIsManual1 && m_manualParam1.EqualTo(manualParam))
		return true;

	m_bIsManual1 = true;
	m_manualParam1 = manualParam;
	m_strCurTex1 = "";
	A3DRELEASE(m_pColorCorrectionTex1);
	m_pColorCorrectionTex1 = ConvertVolumeTextureByManualParam(m_pSystem->GetA3DDevice(), NULL, manualParam);

	return true;
}

bool A3DPostCC::SetCCTexture2ByManualParam( const A3DPOST_EFFECT_CC_PARAM::ManualParam& manualParam )
{
	if(m_bIsManual2 && m_manualParam2.EqualTo(manualParam))
		return true;

	m_bIsManual2 = true;
	m_manualParam2 = manualParam;
	m_strCurTex2 = "";
	A3DRELEASE(m_pColorCorrectionTex2);
	m_pColorCorrectionTex2 = ConvertVolumeTextureByManualParam(m_pSystem->GetA3DDevice(), NULL, manualParam);

	return true;
}

A3DRenderTarget* A3DPostCC::ApplyEffect(A3DViewport* pViewport, A3DRenderTarget* pSrcRT)
{
    A3DDevice* pDev = m_pSystem->GetA3DDevice();

    if(!m_pColorCorrectionTex1 || !m_pColorCorrectionTex2)
    {
        return pSrcRT;
    }

    //设置渲染状态
    pDev->SetAlphaBlendEnable(false);
    pDev->SetZTestEnable(false);
    pDev->SetZWriteEnable(false);
    pDev->SetFaceCull(A3DCULL_NONE);

    //设置RnederTarget
    A3DRenderTarget* pDst = pDev->GetRenderTargetMan()->RentRenderTargetColor(m_pSystem->GetWidth(), m_pSystem->GetHeight(), A3DFMT_A8R8G8B8);
    pDst->ApplyToDevice();

    //设置Shader参数
    m_pSystem->SetupCommonVS(m_pSystem->GetWidth(), m_pSystem->GetHeight());
    m_pSystem->GetCommonVS()->Appear();

    m_pHLSLcc->Setup1f("fFactor", m_fFactor);

#ifdef _ANGELICA_2_2
    if(m_pSystem->GetScreenIlumRT())
        m_pHLSLcc->Setup1f("enableScreenIlum", 1.0f);
    else
        m_pHLSLcc->Setup1f("enableScreenIlum", 0.0f);
#endif

    m_pHLSLcc->Appear();	

    pSrcRT->AppearAsTexture(0);
    m_pColorCorrectionTex1->Appear(1);
    m_pColorCorrectionTex2->Appear(2);

#ifdef _ANGELICA_2_2
    if(m_pSystem->GetScreenIlumRT())
        m_pSystem->GetScreenIlumRT()->AppearAsTexture(3);
#endif

    pDev->SetTextureFilterType(0, A3DTEXF_POINT);

    pDev->SetTextureFilterType(1, A3DTEXF_LINEAR);
    pDev->SetTextureAddress(1, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
    pDev->SetSamplerState(1, D3DSAMP_ADDRESSW, A3DTADDR_CLAMP);

    pDev->SetTextureFilterType(2, A3DTEXF_LINEAR);
    pDev->SetTextureAddress(2, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
    pDev->SetSamplerState(2, D3DSAMP_ADDRESSW, A3DTADDR_CLAMP);

    pDev->SetTextureFilterType(3, A3DTEXF_LINEAR);
    pDev->SetTextureAddress(3, A3DTADDR_CLAMP, A3DTADDR_CLAMP);


    //绘制方块
    m_pSystem->RenderQuad();

    m_pSystem->GetCommonVS()->Disappear();
    m_pHLSLcc->Disappear();

    pDst->WithdrawFromDevice();

    pDev->GetRenderTargetMan()->ReturnRenderTarget(pSrcRT);

    return pDst;
}

#endif
