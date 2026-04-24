/*
 * FILE: A3DPostMotionBlur.cpp
 *
 * DESCRIPTION: 运动模糊后处理
 *
 * CREATED BY: Panyupin, 2012/3/6
 *
 * HISTORY: 
 *
 * Copyright (c) 2012 Archosaur Studio, All Rights Reserved.
 */

#include "A3DPostMotionBlur.h"
#include "A3DPostSystem.h"
#include "A3DGeometry.h"
#include "A3DCameraBase.h"
#include "A3DFuncs.h"

#ifdef _ANGELICA_2_1
#include "A3DHLSLWrapper.h"
static const char* szPost_MotionBlur_vs_hlsl = "shaders\\HLSL\\PostProcess\\post_MotionBlur_vs.hlsl";
static const char* szPost_MotionBlur_ps_hlsl = "shaders\\HLSL\\PostProcess\\post_MotionBlur_ps.hlsl";
static const char* szPost_MotionBlurDownsamp_ps_hlsl = "shaders\\HLSL\\PostProcess\\post_MotionBlurDownsamp_ps.hlsl";
static const char* szPost_MotionBlurMerge_ps_hlsl = "shaders\\HLSL\\PostProcess\\Post_MotionBlurMerge_ps.hlsl";
#else
#include "A3DHLSL.h"
static const char* szPost_MotionBlur_vs_hlsl = "shaders\\2.2\\HLSL\\PostProcess\\post_MotionBlur_vs.hlsl";
static const char* szPost_MotionBlur_ps_hlsl = "shaders\\2.2\\HLSL\\PostProcess\\post_MotionBlur_ps.hlsl";
static const char* szPost_MotionBlurDownsamp_ps_hlsl = "shaders\\2.2\\HLSL\\PostProcess\\post_MotionBlurDownsamp_ps.hlsl";
static const char* szPost_MotionBlurMerge_ps_hlsl = "shaders\\2.2\\HLSL\\PostProcess\\Post_MotionBlurMerge_ps.hlsl";
#endif


extern ALog g_A3DErrLog;

// FLOAT x, y, z;
// FLOAT nx, ny, nz;
// FLOAT tu, tv;

static const D3DVERTEXELEMENT9 s_aVertDecl[] = 
{
	{0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
	{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
	{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
	D3DDECL_END()
};

bool A3DPostMotionBlur::Init( A3DPostSystem* pSystem )
{
	m_pSystem = pSystem;

	m_pHLSL = CreateA3DHLSL_Common(szPost_MotionBlur_vs_hlsl, "vs_main", szPost_MotionBlur_ps_hlsl, "ps_main", 0, NULL, 
							   m_pSystem->GetA3DDevice(), m_pSystem->GetD3D9VertElement());
	if(!m_pHLSL)
	{
		g_A3DErrLog.Log("A3DPostRadialBlur::Init Failed! post_MotionBlur.hlsl load failed!");
		Release();
		return false;
	}


	m_pDownSampHLSL = CreateA3DHLSL_Common(NULL, NULL, szPost_MotionBlurDownsamp_ps_hlsl, "ps_main", 0, NULL, 
		m_pSystem->GetA3DDevice(), m_pSystem->GetD3D9VertElement());

	if(!m_pDownSampHLSL)
	{
		g_A3DErrLog.Log("A3DPostMotionBlur::Init Failed! Can not load DownSamp HLSL file!");
		Release();
		return false;
	}

	m_pMergeHLSL = CreateA3DHLSL_Common(szPost_MotionBlur_vs_hlsl, "vs_main", szPost_MotionBlurMerge_ps_hlsl, "ps_main", 0, NULL, 
		m_pSystem->GetA3DDevice(), m_pSystem->GetD3D9VertElement());

	if(!m_pMergeHLSL)
	{
		g_A3DErrLog.Log("A3DPostMotionBlur::Init Failed! Can not load Merge HLSL file!");
		Release();
		return false;
	}

	int nRow = 20;
	int nCol = 20;
	int vertexNum = (2 + (nRow-1) * nCol);
	int indexNum = ((nRow-1) * nCol * 6);

	A3DVECTOR3* vertexs = new A3DVECTOR3[vertexNum];
	WORD* indexes = new WORD[indexNum];

	if(!a3d_CreateIndexedSphere(A3DVECTOR3(0,0,0), 1.f, nRow, nCol, 
		vertexs, vertexNum, indexes, indexNum))
	{
		Release();
		return false;
	}

	m_pA3DStreamSphere = new A3DStream();
	if(!m_pA3DStreamSphere->Init(m_pSystem->GetA3DDevice(), 
		A3DVT_VERTEX, vertexNum, indexNum, A3DSTRM_STATIC, A3DSTRM_STATIC))
	{
		Release();
		return false;
	}	


	A3DVERTEX* vp;
	m_pA3DStreamSphere->LockVertexBuffer(0, vertexNum * sizeof(A3DVERTEX), (BYTE**)(&vp), 0);
	for(int i = 0; i < vertexNum; i++)
		vp[i] = A3DVERTEX(vertexs[i], A3DVECTOR3(0,0,0), 0,0);
	m_pA3DStreamSphere->UnlockVertexBuffer();

	WORD* ip;
	m_pA3DStreamSphere->LockIndexBuffer(0, 6 * sizeof(WORD), (BYTE**)(&ip), 0);
	for(int i = 0; i < indexNum; i++)
		ip[i] = indexes[i];
	m_pA3DStreamSphere->UnlockIndexBuffer();

	delete[] vertexs;
	delete[] indexes;

	if(m_pSystem->GetA3DEngine()->GetActiveCamera())
	{
		m_vCameraPosLast = m_pSystem->GetA3DEngine()->GetActiveCamera()->GetPos();
		m_vCameraDirLast = m_pSystem->GetA3DEngine()->GetActiveCamera()->GetDir();
	}

	return true;
}

void A3DPostMotionBlur::Release()
{
	ReleaseA3DHLSL_Common(m_pSystem->GetA3DDevice(), m_pHLSL);
	ReleaseA3DHLSL_Common(m_pSystem->GetA3DDevice(), m_pDownSampHLSL);
	ReleaseA3DHLSL_Common(m_pSystem->GetA3DDevice(), m_pMergeHLSL);
	A3DRELEASE(m_pA3DStreamSphere);
}

bool A3DPostMotionBlur::IsHardwareSupport( A3DDevice* pA3DDev )
{
	if (!pA3DDev->TestVertexShaderVersion(2, 0) || !pA3DDev->TestPixelShaderVersion(2, 0))
		return false;
	return true;
}

bool A3DPostMotionBlur::Render( A3DViewport* pViewport )
{
	if(!m_pSystem->IsLinearZReady())
		return false;

	A3DDevice* pDev = m_pSystem->GetA3DDevice();
	A3DRenderTarget* pSrc = m_pSystem->GetSrcRT();
	A3DRenderTarget* pDst = m_pSystem->GetDstRT();
	A3DRenderTarget* pHalfRT1 = pDev->GetRenderTargetMan()->RentRenderTargetColor(-2, -2, A3DFMT_A8R8G8B8);
	A3DRenderTarget* pHalfRT2 = pDev->GetRenderTargetMan()->RentRenderTargetColor(-2, -2, A3DFMT_A8R8G8B8);

	ASSERT(pDev && pSrc && pDst);

	//设置渲染状态
	pDev->SetAlphaBlendEnable(false);
	pDev->SetZTestEnable(false);
	pDev->SetZWriteEnable(false);
	pDev->SetFaceCull(A3DCULL_NONE);

	//降采样
	pDev->SetTextureFilterType(0, A3DTEXF_LINEAR);
	pDev->SetTextureAddress(0, A3DTADDR_CLAMP, A3DTADDR_CLAMP);

	pHalfRT1->ApplyToDevice();
	pDev->Clear(D3DCLEAR_TARGET, A3DCOLORRGB(255, 255, 255), 1.0f, 0);
	m_pDownSampHLSL->Setup2f("g_vPixelSize", 1.f / float(m_pSystem->GetWidth()), 1.f / (float)m_pSystem->GetHeight());
	m_pDownSampHLSL->SetupTextureFromRT("g_Sampler", pSrc);
	m_pDownSampHLSL->Appear();
	m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 2, m_pSystem->GetHeight() / 2);
	m_pSystem->GetCommonVS()->Appear();
	m_pSystem->GetA3DDevice()->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, 4, 0, 2);
	m_pDownSampHLSL->Disappear();
	m_pSystem->GetCommonVS()->Disappear();
	pHalfRT1->WithdrawFromDevice();


	//准备参数
	A3DCameraBase* pCamera = pViewport->GetCamera();
	float width = static_cast<float>(m_pSystem->GetWidth() / 2);
	float height = static_cast<float>(m_pSystem->GetHeight() / 2);
	float invWidth = 0.5f / width;
	float invHeight = 0.5f / height;

	const A3DVECTOR3& cameraPos = pCamera->GetPos();
	m_pHLSL->SetupVec3("g_cameraPos", cameraPos);
	m_pMergeHLSL->SetupVec3("g_cameraPos", cameraPos);
	const A3DVECTOR3& cameraDir = pCamera->GetDir();
	const A3DVECTOR3& cameraUp = pCamera->GetUp();

	if(m_bFirstRun)	//应对刚刚开启效果的情况
	{
		m_vCameraPosLast = cameraPos;
		m_vCameraDirLast = cameraDir;
		m_vCameraUpLast = cameraUp;

		m_vRealCameraPosLast = cameraPos;
		m_vRealCameraDirLast = cameraDir;
		m_vRealCameraUpLast = cameraUp;

		m_bFirstRun = false;
	}

	static float fLastTime = 0;

	float fDeltaTime;
	if(fLastTime == 0)
		fDeltaTime = 100;
	else
	{
		fDeltaTime = (float)a_GetTime() - fLastTime;
	}

	fLastTime = (float)a_GetTime();

	float fScale = 100.f / fDeltaTime;   
	fScale = min(fScale, 20.f);
	fScale = max(fScale, 0.1f);

	float fScalePos = fScale * m_fBlurScalePosition / 10.f * 10.f;
	float fScaleRot = fScale * m_fBlurScaleRotation / 10.f;

	static A3DVECTOR3 vPosToLastLast = m_vRealCameraPosLast - cameraPos;

	A3DVECTOR3 vPosToLast = m_vRealCameraPosLast - cameraPos;
	A3DVECTOR3 vPosToLastScale = vPosToLast	* fScalePos;
	
	//如果出现跳帧, 则进行更加平缓的平滑
	if( vPosToLastLast.Magnitude() > 1e-2f && (vPosToLastScale - vPosToLastLast).Magnitude() / vPosToLastLast.Magnitude() > 1.f)
		vPosToLastScale = vPosToLastLast * 0.9f + vPosToLastScale * 0.1f;
	else //否则进行比较陡峭的平滑
		vPosToLastScale = vPosToLastLast * 0.8f + vPosToLastScale * 0.2f;
 	
	m_vCameraPosLast = cameraPos + vPosToLastScale ;

	vPosToLastLast = vPosToLastScale;

	if( (cameraDir - m_vRealCameraDirLast).SquaredMagnitude() > 1e-4)
	{
		A3DVECTOR3 vDirPivot;
		vDirPivot.CrossProduct(cameraDir, m_vRealCameraDirLast);
		float fDirAngle = acosf(DotProduct(cameraDir, m_vRealCameraDirLast));
		fDirAngle = fDirAngle * fScaleRot;
		A3DMATRIX4 matRotDir;
		matRotDir.RotateAxis(vDirPivot, fDirAngle);
		m_vCameraDirLast = cameraDir * matRotDir;
	}
	else
		m_vCameraDirLast = m_vRealCameraDirLast;

	if( (cameraUp - m_vRealCameraUpLast).SquaredMagnitude() > 1e-4)
	{
		A3DVECTOR3 vUpPivot;
		vUpPivot.CrossProduct(cameraUp, m_vRealCameraUpLast);
		float fUpAngle = acosf(DotProduct(cameraUp, m_vRealCameraUpLast));
		fUpAngle = fUpAngle * fScaleRot;
		A3DMATRIX4 matRotDir;
		matRotDir.RotateAxis(vUpPivot, fUpAngle);
		m_vCameraUpLast = cameraUp * matRotDir;
	}
	else
		m_vCameraUpLast = m_vRealCameraUpLast;


	//模糊
	pDev->SetTextureFilterType(0, A3DTEXF_LINEAR);
	pDev->SetTextureAddress(0, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	pDev->SetTextureFilterType(1, A3DTEXF_POINT);
	pDev->SetTextureAddress(1, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	pDev->SetTextureFilterType(2, A3DTEXF_LINEAR);
	pDev->SetTextureAddress(2, A3DTADDR_CLAMP, A3DTADDR_CLAMP);

	//m_pA3DStreamSphereDecl->Appear();

	m_pHLSL->Setup1f("g_ZFar", pCamera->GetZBack());
	m_pMergeHLSL->Setup1f("g_ZFar", pCamera->GetZBack());

	A3DVECTOR4 v4(width, height, invWidth, invHeight);
	m_pHLSL->SetupVec4("g_ScreenSize", v4);
	m_pHLSL->Setup1f("g_maxScreenSpeed", m_fBlurMax);
	m_pHLSL->SetupTextureFromRT("g_DepthSamp", m_pSystem->GetLinearDepthRT());
	m_pMergeHLSL->SetupVec4("g_ScreenSize", v4);
	m_pMergeHLSL->Setup1f("g_maxScreenSpeed", m_fBlurMax);
	m_pMergeHLSL->SetupTextureFromRT("g_DepthSamp", m_pSystem->GetLinearDepthRT());
//#if _POST_DEBUG_
//   	if(GetAsyncKeyState(VK_F1) & 0x8000)
//   	{
//   		Sleep(100);
//   	}
//  	if(GetAsyncKeyState(VK_F2) & 0x8000)
//  	{
//  		Sleep(50);
//  	}
//  	if(GetAsyncKeyState(VK_F3) & 0x8000)
//  	{
//  		Sleep(30);
//  	}
//#endif
	m_pHLSL->Setup1f("g_blurLengthScale", m_fBlurScale);
	m_pHLSL->Setup1f("g_fInFocusDistance", m_fInFocusDistance);
	m_pHLSL->Setup1f("g_fOutFocusDistance", m_fOutFocusDistance);
	m_pMergeHLSL->Setup1f("g_blurLengthScale", m_fBlurScale);
	m_pMergeHLSL->Setup1f("g_fInFocusDistance", m_fInFocusDistance);
	m_pMergeHLSL->Setup1f("g_fOutFocusDistance", m_fOutFocusDistance);
	A3DMATRIX4 vpmLast;

 	m_vRealCameraPosLast = cameraPos;
 	m_vRealCameraDirLast = cameraDir;
 	m_vRealCameraUpLast = cameraUp;

	m_vCameraDirLast.Normalize();
	m_vCameraUpLast.Normalize();

	vpmLast = a3d_LookAtMatrix(m_vCameraPosLast, m_vCameraPosLast + m_vCameraDirLast, m_vCameraUpLast, 0);
	vpmLast = vpmLast * pCamera->GetProjectionTM();

	m_pHLSL->SetupMatrix("g_viewProjLast", vpmLast);
	m_pHLSL->SetupMatrix("g_viewProjCurrent", pCamera->GetVPTM());
	m_pMergeHLSL->SetupMatrix("g_viewProjLast", vpmLast);
	m_pMergeHLSL->SetupMatrix("g_viewProjCurrent", pCamera->GetVPTM());

	
	m_pA3DStreamSphere->Appear(0, true);//注意，第二个参数如果不写，则会自动设置fvf，同时也会设置vertex shader为NULL
	m_pHLSL->Appear();


	pHalfRT2->ApplyToDevice();
	pHalfRT1->AppearAsTexture(0);
	m_pSystem->GetA3DDevice()->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, 0, 
		m_pA3DStreamSphere->GetVertCount(), 0, m_pA3DStreamSphere->GetIndexCount() / 3);
	pHalfRT2->WithdrawFromDevice();


 	pHalfRT1->ApplyToDevice();
 	pHalfRT2->AppearAsTexture(0);
 	m_pSystem->GetA3DDevice()->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, 0, 
 		m_pA3DStreamSphere->GetVertCount(), 0, m_pA3DStreamSphere->GetIndexCount() / 3);
 	pHalfRT1->WithdrawFromDevice();
 
 	pHalfRT2->ApplyToDevice();
 	pHalfRT1->AppearAsTexture(0);
 	m_pSystem->GetA3DDevice()->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, 0, 
 		m_pA3DStreamSphere->GetVertCount(), 0, m_pA3DStreamSphere->GetIndexCount() / 3);
 	pHalfRT2->WithdrawFromDevice();

	m_pHLSL->Disappear();

#ifdef _DEBUG_POST_
// 	if(GetAsyncKeyState(VK_F7) & 0x8000)
// 	{
// 		D3DXSaveSurfaceToFileA("MotionBlur.dds", D3DXIFF_DDS, pDst->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
// 	}
#endif

	//Merge
	pDst->ApplyToDevice();

	width = static_cast<float>(m_pSystem->GetWidth());
	height = static_cast<float>(m_pSystem->GetHeight());
	invWidth = 0.5f / width;
	invHeight = 0.5f / height;
	v4 = A3DVECTOR4(width, height, invWidth, invHeight);
	m_pMergeHLSL->SetupVec4("g_ScreenSize", v4);

	m_pMergeHLSL->SetupTextureFromRT("g_Sampler", pSrc);
	m_pMergeHLSL->SetupTextureFromRT("g_DepthSamp", m_pSystem->GetLinearDepthRT());
	m_pMergeHLSL->SetupTextureFromRT("g_SamplerBlured", pHalfRT2);
	
	
	m_pA3DStreamSphere->Appear(0, true);//注意，第二个参数如果不写，则会自动设置fvf，同时也会设置vertex shader为NULL
	m_pMergeHLSL->Appear();

	m_pSystem->GetA3DDevice()->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, 0, 
		m_pA3DStreamSphere->GetVertCount(), 0, m_pA3DStreamSphere->GetIndexCount() / 3);
	m_pMergeHLSL->Disappear();

	pDst->WithdrawFromDevice();

	//交换RT
	m_pSystem->SwapSrcDstRT();

	pDev->SetTextureFilterType(1, A3DTEXF_LINEAR);

	pDev->GetRenderTargetMan()->ReturnRenderTarget(pHalfRT1);
	pDev->GetRenderTargetMan()->ReturnRenderTarget(pHalfRT2);
	return true;
}

A3DPostMotionBlur::~A3DPostMotionBlur()
{

}

A3DPOST_EFFECT A3DPostMotionBlur::GetEffectType() const
{
	return A3DPOST_EFFECT_MotionBlur;
}

A3DRenderTarget* A3DPostMotionBlur::ApplyEffect( A3DViewport* pViewport, A3DRenderTarget* pSrcRT )
{
    if(!m_pSystem->IsLinearZReady())
        return pSrcRT;

    A3DDevice* pDev = m_pSystem->GetA3DDevice();

    int rtWidth		= pViewport->GetParam()->Width;
    int rtHeight	= pViewport->GetParam()->Height;
    int rtWidth_2	= rtWidth / 2;
    int rtHeight_2	= rtHeight / 2;
    A3DRenderTarget* pHalfRT1 = pDev->GetRenderTargetMan()->RentRenderTargetColor(rtWidth_2, rtHeight_2, A3DFMT_A8R8G8B8);
    A3DRenderTarget* pHalfRT2 = pDev->GetRenderTargetMan()->RentRenderTargetColor(rtWidth_2, rtHeight_2, A3DFMT_A8R8G8B8);

    //设置渲染状态
    pDev->SetAlphaBlendEnable(false);
    pDev->SetZTestEnable(false);
    pDev->SetZWriteEnable(false);
    pDev->SetFaceCull(A3DCULL_NONE);

    //降采样
    pDev->SetTextureFilterType(0, A3DTEXF_LINEAR);
    pDev->SetTextureAddress(0, A3DTADDR_CLAMP, A3DTADDR_CLAMP);

    pHalfRT1->ApplyToDevice();
    pDev->Clear(D3DCLEAR_TARGET, A3DCOLORRGB(255, 255, 255), 1.0f, 0);
    m_pDownSampHLSL->Setup2f("g_vPixelSize", 1.f / float(m_pSystem->GetWidth()), 1.f / (float)m_pSystem->GetHeight());
    m_pDownSampHLSL->SetupTextureFromRT("g_Sampler", pSrcRT);
    m_pDownSampHLSL->Appear();
    m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 2, m_pSystem->GetHeight() / 2);
    m_pSystem->GetCommonVS()->Appear();
    m_pSystem->GetA3DDevice()->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, 4, 0, 2);
    m_pDownSampHLSL->Disappear();
    m_pSystem->GetCommonVS()->Disappear();
    pHalfRT1->WithdrawFromDevice();


    //准备参数
    A3DCameraBase* pCamera = pViewport->GetCamera();
    float width = static_cast<float>(m_pSystem->GetWidth() / 2);
    float height = static_cast<float>(m_pSystem->GetHeight() / 2);
    float invWidth = 0.5f / width;
    float invHeight = 0.5f / height;

    const A3DVECTOR3& cameraPos = pCamera->GetPos();
    m_pHLSL->SetupVec3("g_cameraPos", cameraPos);
    m_pMergeHLSL->SetupVec3("g_cameraPos", cameraPos);
    const A3DVECTOR3& cameraDir = pCamera->GetDir();
    const A3DVECTOR3& cameraUp = pCamera->GetUp();

    if(m_bFirstRun)	//应对刚刚开启效果的情况
    {
        m_vCameraPosLast = cameraPos;
        m_vCameraDirLast = cameraDir;
        m_vCameraUpLast = cameraUp;

        m_vRealCameraPosLast = cameraPos;
        m_vRealCameraDirLast = cameraDir;
        m_vRealCameraUpLast = cameraUp;

        m_bFirstRun = false;
    }

    static float fLastTime = 0;

    float fDeltaTime;
    if(fLastTime == 0)
        fDeltaTime = 100;
    else
    {
        fDeltaTime = (float)a_GetTime() - fLastTime;
    }

    fLastTime = (float)a_GetTime();

    float fScale = 100.f / fDeltaTime;   
    fScale = min(fScale, 20.f);
    fScale = max(fScale, 0.1f);

    float fScalePos = fScale * m_fBlurScalePosition / 10.f * 10.f;
    float fScaleRot = fScale * m_fBlurScaleRotation / 10.f;

    static A3DVECTOR3 vPosToLastLast = m_vRealCameraPosLast - cameraPos;

    A3DVECTOR3 vPosToLast = m_vRealCameraPosLast - cameraPos;
    A3DVECTOR3 vPosToLastScale = vPosToLast	* fScalePos;

    //如果出现跳帧, 则进行更加平缓的平滑
    if( vPosToLastLast.Magnitude() > 1e-2f && (vPosToLastScale - vPosToLastLast).Magnitude() / vPosToLastLast.Magnitude() > 1.f)
        vPosToLastScale = vPosToLastLast * 0.9f + vPosToLastScale * 0.1f;
    else //否则进行比较陡峭的平滑
        vPosToLastScale = vPosToLastLast * 0.8f + vPosToLastScale * 0.2f;

    m_vCameraPosLast = cameraPos + vPosToLastScale ;

    vPosToLastLast = vPosToLastScale;

    if( (cameraDir - m_vRealCameraDirLast).SquaredMagnitude() > 1e-4)
    {
        A3DVECTOR3 vDirPivot;
        vDirPivot.CrossProduct(cameraDir, m_vRealCameraDirLast);
        float fDirAngle = acosf(DotProduct(cameraDir, m_vRealCameraDirLast));
        fDirAngle = fDirAngle * fScaleRot;
        A3DMATRIX4 matRotDir;
        matRotDir.RotateAxis(vDirPivot, fDirAngle);
        m_vCameraDirLast = cameraDir * matRotDir;
    }
    else
        m_vCameraDirLast = m_vRealCameraDirLast;

    if( (cameraUp - m_vRealCameraUpLast).SquaredMagnitude() > 1e-4)
    {
        A3DVECTOR3 vUpPivot;
        vUpPivot.CrossProduct(cameraUp, m_vRealCameraUpLast);
        float fUpAngle = acosf(DotProduct(cameraUp, m_vRealCameraUpLast));
        fUpAngle = fUpAngle * fScaleRot;
        A3DMATRIX4 matRotDir;
        matRotDir.RotateAxis(vUpPivot, fUpAngle);
        m_vCameraUpLast = cameraUp * matRotDir;
    }
    else
        m_vCameraUpLast = m_vRealCameraUpLast;


    //模糊
    pDev->SetTextureFilterType(0, A3DTEXF_LINEAR);
    pDev->SetTextureAddress(0, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
    pDev->SetTextureFilterType(1, A3DTEXF_POINT);
    pDev->SetTextureAddress(1, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
    pDev->SetTextureFilterType(2, A3DTEXF_LINEAR);
    pDev->SetTextureAddress(2, A3DTADDR_CLAMP, A3DTADDR_CLAMP);

    //m_pA3DStreamSphereDecl->Appear();

    m_pHLSL->Setup1f("g_ZFar", pCamera->GetZBack());
    m_pMergeHLSL->Setup1f("g_ZFar", pCamera->GetZBack());

    A3DVECTOR4 v4(width, height, invWidth, invHeight);
    m_pHLSL->SetupVec4("g_ScreenSize", v4);
    m_pHLSL->Setup1f("g_maxScreenSpeed", m_fBlurMax);
    m_pHLSL->SetupTextureFromRT("g_DepthSamp", m_pSystem->GetLinearDepthRT());
    m_pMergeHLSL->SetupVec4("g_ScreenSize", v4);
    m_pMergeHLSL->Setup1f("g_maxScreenSpeed", m_fBlurMax);
    m_pMergeHLSL->SetupTextureFromRT("g_DepthSamp", m_pSystem->GetLinearDepthRT());
    //#if _POST_DEBUG_
    //   	if(GetAsyncKeyState(VK_F1) & 0x8000)
    //   	{
    //   		Sleep(100);
    //   	}
    //  	if(GetAsyncKeyState(VK_F2) & 0x8000)
    //  	{
    //  		Sleep(50);
    //  	}
    //  	if(GetAsyncKeyState(VK_F3) & 0x8000)
    //  	{
    //  		Sleep(30);
    //  	}
    //#endif
    m_pHLSL->Setup1f("g_blurLengthScale", m_fBlurScale);
    m_pHLSL->Setup1f("g_fInFocusDistance", m_fInFocusDistance);
    m_pHLSL->Setup1f("g_fOutFocusDistance", m_fOutFocusDistance);
    m_pMergeHLSL->Setup1f("g_blurLengthScale", m_fBlurScale);
    m_pMergeHLSL->Setup1f("g_fInFocusDistance", m_fInFocusDistance);
    m_pMergeHLSL->Setup1f("g_fOutFocusDistance", m_fOutFocusDistance);
    A3DMATRIX4 vpmLast;

    m_vRealCameraPosLast = cameraPos;
    m_vRealCameraDirLast = cameraDir;
    m_vRealCameraUpLast = cameraUp;

    m_vCameraDirLast.Normalize();
    m_vCameraUpLast.Normalize();

    vpmLast = a3d_LookAtMatrix(m_vCameraPosLast, m_vCameraPosLast + m_vCameraDirLast, m_vCameraUpLast, 0);
    vpmLast = vpmLast * pCamera->GetProjectionTM();

    m_pHLSL->SetupMatrix("g_viewProjLast", vpmLast);
    m_pHLSL->SetupMatrix("g_viewProjCurrent", pCamera->GetVPTM());
    m_pMergeHLSL->SetupMatrix("g_viewProjLast", vpmLast);
    m_pMergeHLSL->SetupMatrix("g_viewProjCurrent", pCamera->GetVPTM());


    m_pA3DStreamSphere->Appear(0, true);//注意，第二个参数如果不写，则会自动设置fvf，同时也会设置vertex shader为NULL
    m_pHLSL->Appear();


    pHalfRT2->ApplyToDevice();
    pHalfRT1->AppearAsTexture(0);
    m_pSystem->GetA3DDevice()->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, 0, 
        m_pA3DStreamSphere->GetVertCount(), 0, m_pA3DStreamSphere->GetIndexCount() / 3);
    pHalfRT2->WithdrawFromDevice();


    pHalfRT1->ApplyToDevice();
    pHalfRT2->AppearAsTexture(0);
    m_pSystem->GetA3DDevice()->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, 0, 
        m_pA3DStreamSphere->GetVertCount(), 0, m_pA3DStreamSphere->GetIndexCount() / 3);
    pHalfRT1->WithdrawFromDevice();

    pHalfRT2->ApplyToDevice();
    pHalfRT1->AppearAsTexture(0);
    m_pSystem->GetA3DDevice()->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, 0, 
        m_pA3DStreamSphere->GetVertCount(), 0, m_pA3DStreamSphere->GetIndexCount() / 3);
    pHalfRT2->WithdrawFromDevice();

    m_pHLSL->Disappear();

#ifdef _DEBUG_POST_
    // 	if(GetAsyncKeyState(VK_F7) & 0x8000)
    // 	{
    // 		D3DXSaveSurfaceToFileA("MotionBlur.dds", D3DXIFF_DDS, pDst->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
    // 	}
#endif

    //Merge

    A3DRenderTarget* pDst = pDev->GetRenderTargetMan()->RentRenderTargetColor(m_pSystem->GetWidth(), m_pSystem->GetHeight(), A3DFMT_A8R8G8B8);
    pDst->ApplyToDevice();

    width = static_cast<float>(m_pSystem->GetWidth());
    height = static_cast<float>(m_pSystem->GetHeight());
    invWidth = 0.5f / width;
    invHeight = 0.5f / height;
    v4 = A3DVECTOR4(width, height, invWidth, invHeight);
    m_pMergeHLSL->SetupVec4("g_ScreenSize", v4);

    m_pMergeHLSL->SetupTextureFromRT("g_Sampler", pSrcRT);
    m_pMergeHLSL->SetupTextureFromRT("g_DepthSamp", m_pSystem->GetLinearDepthRT());
    m_pMergeHLSL->SetupTextureFromRT("g_SamplerBlured", pHalfRT2);

    m_pA3DStreamSphere->Appear(0, true);//注意，第二个参数如果不写，则会自动设置fvf，同时也会设置vertex shader为NULL
    m_pMergeHLSL->Appear();

    m_pSystem->GetA3DDevice()->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, 0, 
        m_pA3DStreamSphere->GetVertCount(), 0, m_pA3DStreamSphere->GetIndexCount() / 3);
    m_pMergeHLSL->Disappear();

    pDst->WithdrawFromDevice();

    pDev->SetTextureFilterType(1, A3DTEXF_LINEAR);

    pDev->GetRenderTargetMan()->ReturnRenderTarget(pHalfRT1);
    pDev->GetRenderTargetMan()->ReturnRenderTarget(pHalfRT2);
    pDev->GetRenderTargetMan()->ReturnRenderTarget(pSrcRT);

    return pDst;
}
