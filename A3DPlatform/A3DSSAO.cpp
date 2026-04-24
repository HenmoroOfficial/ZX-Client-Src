#include "A3DPI.h"
#include "A3DDevice.h"
#include "A3DEngine.h"
#include "A3DPixelShader.h"
#include "A3DVertexShader.h"
#include "A3DShaderMan.h"
#include "A3DCameraBase.h"
#include "A3DCamera.h"
#include "A3DViewport.h"
#include "A3DRenderTarget.h"
#include "A3DStream.h"
#include "a3drendertargetman.h"
#include "A3DTexture.h"
#include "A3DDeviceCaps.h"

//#define _DEBUGSSAO_

#ifdef _ANGELICA_2_1
#define A3DSSAO A3DAO
#include "A3DAO.h"
#include "A3DHLSLWrapper.h"
#include "A3DSceneRenderConfig.h"

#define AO2_COMMON_VS_HLSL					"Shaders/hlsl/ao2_common_vs.hlsl"
#define AO2_LINEAR_DEPTH_HLSL				"Shaders/hlsl/ao2_linear_depth.hlsl"
#define AO2_DOWN_SAMPLE_HLSL				"Shaders/hlsl/ao2_down_sample.hlsl"
#define AO2_SSAO_VS_HLSL					"Shaders/hlsl/ao2_ssao_vs.hlsl"
#define AO2_SSAO_CRY3_HLSL					"Shaders/hlsl/ao2_ssao_cry3.hlsl"
#define AO2_SSAO_BLURSMALLX_HLSL			"Shaders/hlsl/ao2_blursmallX.hlsl"
#define AO2_SSAO_BLURSMALLY_HLSL			"Shaders/hlsl/ao2_blursmallY.hlsl"
#define AO2_BLUR_NEW_HLSL					"Shaders/hlsl/ao2_blur_new.hlsl"

#else
#include "A3DSSAO.h"
#include "A3DHLSL.h"
#include "A3DEnvironment.h"

#define AO2_COMMON_VS_HLSL					"Shaders\\2.2\\HLSL\\Shadow\\ao2_common_vs.hlsl"
#define AO2_LINEAR_DEPTH_HLSL				"Shaders\\2.2\\HLSL\\Shadow\\ao2_linear_depth.hlsl"
#define AO2_DOWN_SAMPLE_HLSL				"Shaders\\2.2\\HLSL\\Shadow\\ao2_down_sample.hlsl"
#define AO2_SSAO_VS_HLSL					"Shaders\\2.2\\HLSL\\Shadow\\ao2_ssao_vs.hlsl"
#define AO2_SSAO_CRY3_HLSL					"Shaders\\2.2\\HLSL\\Shadow\\ao2_ssao_cry3.hlsl"
#define AO2_SSAO_BLURSMALLX_HLSL			"Shaders\\2.2\\HLSL\\Shadow\\ao2_blursmallX.hlsl"
#define AO2_SSAO_BLURSMALLY_HLSL			"Shaders\\2.2\\HLSL\\Shadow\\ao2_blursmallY.hlsl"
#define AO2_BLUR_NEW_HLSL					"Shaders\\2.2\\HLSL\\Shadow\\ao2_blur_new.hlsl"

#endif


struct SSAOVert
{
	A3DVECTOR3 xyz;
	float uv[2];
	float fogparam[3];

	void Set(const A3DVECTOR3& vPos, float u, float v, float dx, float dy, float dz)
	{
		xyz = vPos;
		uv[0] = u;
		uv[1] = v;
		fogparam[0] = dx;
		fogparam[1] = dy;
		fogparam[2] = dz;
	}
};
static SSAOVert s_ssaoVert[4];

static const D3DVERTEXELEMENT9 s_ssaoVertDecl[] = 
{
	{0, 0,  D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
	{0, 12, D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
	{0, 20, D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
	D3DDECL_END()
};

static int AOphase=0;
static bool AOmulti = true;

A3DSSAO::A3DSSAO() : m_pDevice(0), m_pAo2CommonVS(0), m_pAo2SSAOVS(0), m_pAo2DownSampPS(0),
m_pAo2Cry3PS(0), m_pAo2LinearDepthPS(0),
m_pAo2BlurSmallXPS(0), m_pAo2BlurSmallYPS(0), m_pAo2BlurNew(0),
m_pSmallGBufferRT(0), m_pSSAORT(0), m_pLinearDepthRT(0), m_pInputFrameBuffer(0),
m_pD3DRandom4x4Texture(0), m_pVertDecl(NULL)
{

}


bool A3DSSAO::Init( A3DDevice * pDevice )
{
	m_pDevice = pDevice;

	if(m_pDevice->GetIntZRenderTarget() ==  NULL) //如果硬件不支持IntZ,则返回
		return true;

	bool bSupportR16F = pDevice->GetA3DDeviceCaps()->SupportR16F_RT();
	bool bSupportR32F = pDevice->GetA3DDeviceCaps()->SupportR32F_RT();

	if(!bSupportR16F && !bSupportR32F)	//如果不支持浮点RenderTarget,则返回
		return false;

	int m_nWidth			= m_pDevice->GetDevFormat().nWidth;
	int m_nHeight			= m_pDevice->GetDevFormat().nHeight;
	A3DRenderTarget::RTFMT devFmt;
	if(bSupportR16F)
		devFmt.fmtTarget = (A3DFORMAT)D3DFMT_R16F;
	else
		devFmt.fmtTarget = (A3DFORMAT)D3DFMT_R32F;

	devFmt.fmtDepth		= A3DFMT_D24X8;

	devFmt.iWidth		= 0;
	devFmt.iHeight		= 0;

	m_pLinearDepthRT = new A3DRenderTarget;
	if(!m_pLinearDepthRT->Init(m_pDevice, devFmt, true, false))
	{
		delete m_pLinearDepthRT;
		m_pLinearDepthRT = NULL;
		return false;
	}	

	devFmt.iWidth		= -2;
	devFmt.iHeight		= -2;

	m_pSmallGBufferRT = new A3DRenderTarget;
	if(!m_pSmallGBufferRT->Init(m_pDevice, devFmt, true, false))
	{
		Release();
		return false;
	}
	m_pSmallGBufferRTLast = new A3DRenderTarget;
	if(!m_pSmallGBufferRTLast->Init(m_pDevice, devFmt, true, false))
	{
		Release();
		return false;
	}


	devFmt.fmtTarget	= A3DFMT_A8R8G8B8;
	m_pSSAORT = new A3DRenderTarget;
	if(!m_pSSAORT->Init(m_pDevice, devFmt, true, false))
	{
		Release();
		return false;
	}
	m_pSSAORTLast = new A3DRenderTarget;
	if(!m_pSSAORTLast->Init(m_pDevice, devFmt, true, false))
	{
		Release();
		return false;
	}

	devFmt.fmtTarget = m_pDevice->GetDevFormat().fmtAdapter;
	devFmt.iWidth		= 0;
	devFmt.iHeight		= 0;
	m_pInputFrameBuffer = new A3DRenderTarget;
	if(!m_pInputFrameBuffer->Init(m_pDevice, devFmt, true, false))
	{
		Release();
		return false;
	}

	m_pSSAORT->ApplyToDevice();
	m_pDevice->Clear(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, A3DCOLORRGB(255, 255, 255), 1.0f, 0);
	m_pSSAORT->WithdrawFromDevice();

	m_pSSAORTLast->ApplyToDevice();
	m_pDevice->Clear(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, A3DCOLORRGB(255, 255, 255), 1.0f, 0);
	m_pSSAORTLast->WithdrawFromDevice();


	m_pD3DRandom4x4Texture = new A3DTexture;
	bool bRandomTex = m_pD3DRandom4x4Texture->Create(m_pDevice, 4, 4, A3DFMT_A32B32G32R32F);

	if(!bRandomTex)
	{
		Release();
		return false;
	}

	void* pData;
	int nPitch;
	bRandomTex = m_pD3DRandom4x4Texture->LockRect(0, &pData, &nPitch, NULL, 0);
	if (!bRandomTex)
	{
		Release();
		return false;
	}

	BYTE* rands = (BYTE*)pData;
	int rc = 0;

	A3DVECTOR3 vAngleList[16];
	float fStep;
	fStep = 0.f;	vAngleList[0] =		A3DVECTOR3(sinf(fStep * A3D_PI / 8.f), cosf(fStep * A3D_PI / 8.f), 0);
	fStep = 1.f;	vAngleList[4] =		A3DVECTOR3(sinf(fStep * A3D_PI / 8.f), cosf(fStep * A3D_PI / 8.f), 0);
	fStep = 2.f;	vAngleList[8] =		A3DVECTOR3(sinf(fStep * A3D_PI / 8.f), cosf(fStep * A3D_PI / 8.f), 0);
	fStep = 3.f;	vAngleList[12] =	A3DVECTOR3(sinf(fStep * A3D_PI / 8.f), cosf(fStep * A3D_PI / 8.f), 0);

	fStep = 4.f;	vAngleList[1] =		A3DVECTOR3(sinf(fStep * A3D_PI / 8.f), cosf(fStep * A3D_PI / 8.f), 0);
	fStep = 5.f;	vAngleList[5] =		A3DVECTOR3(sinf(fStep * A3D_PI / 8.f), cosf(fStep * A3D_PI / 8.f), 0);
	fStep = 6.f;	vAngleList[9] =		A3DVECTOR3(sinf(fStep * A3D_PI / 8.f), cosf(fStep * A3D_PI / 8.f), 0);
	fStep = 7.f;	vAngleList[13] =	A3DVECTOR3(sinf(fStep * A3D_PI / 8.f), cosf(fStep * A3D_PI / 8.f), 0);

	fStep = 8.f;	vAngleList[2] =		A3DVECTOR3(sinf(fStep * A3D_PI / 8.f), cosf(fStep * A3D_PI / 8.f), 0);
	fStep = 9.f;	vAngleList[6] =		A3DVECTOR3(sinf(fStep * A3D_PI / 8.f), cosf(fStep * A3D_PI / 8.f), 0);
	fStep = 10.f;	vAngleList[10] =	A3DVECTOR3(sinf(fStep * A3D_PI / 8.f), cosf(fStep * A3D_PI / 8.f), 0);
	fStep = 11.f;	vAngleList[14] =	A3DVECTOR3(sinf(fStep * A3D_PI / 8.f), cosf(fStep * A3D_PI / 8.f), 0);

	fStep = 12.f; 	vAngleList[3] =		A3DVECTOR3(sinf(fStep * A3D_PI / 8.f), cosf(fStep * A3D_PI / 8.f), 0);
	fStep = 13.f; 	vAngleList[7] =		A3DVECTOR3(sinf(fStep * A3D_PI / 8.f), cosf(fStep * A3D_PI / 8.f), 0);
	fStep = 14.f; 	vAngleList[11] =	A3DVECTOR3(sinf(fStep * A3D_PI / 8.f), cosf(fStep * A3D_PI / 8.f), 0);
	fStep = 15.f; 	vAngleList[15] =	A3DVECTOR3(sinf(fStep * A3D_PI / 8.f), cosf(fStep * A3D_PI / 8.f), 0);

	for(int i = 0; i < 4; i++)
	{
		float* pBits = (float*)rands;
		for(int j = 0; j < 4; j++)
		{
			pBits[j * 4] = vAngleList[rc].y;		//注意: 这里的顺序是反过来的
			pBits[j * 4 + 1] = vAngleList[rc].x;
			pBits[j * 4 + 2] = 0;
			pBits[j * 4 + 3] = 0;
			rc++;
		}
		rands += nPitch;
	}
	m_pD3DRandom4x4Texture->UnlockRect();

	m_pAo2CommonVS = CreateA3DHLSL_Common(AO2_COMMON_VS_HLSL, "vs", NULL, NULL, 0, NULL, m_pDevice, s_ssaoVertDecl);
	m_pAo2LinearDepthPS = CreateA3DHLSL_Common(NULL, NULL, AO2_LINEAR_DEPTH_HLSL, "ps", 0, NULL, m_pDevice, s_ssaoVertDecl);
	m_pAo2DownSampPS = CreateA3DHLSL_Common(NULL, NULL, AO2_DOWN_SAMPLE_HLSL, "ps", 0, NULL, m_pDevice, s_ssaoVertDecl);
	m_pAo2SSAOVS = CreateA3DHLSL_Common(AO2_SSAO_VS_HLSL, "vs", NULL, NULL, 0, NULL, m_pDevice, s_ssaoVertDecl);
	m_pAo2Cry3PS = CreateA3DHLSL_Common(NULL, NULL, AO2_SSAO_CRY3_HLSL, "ps", 0, NULL, m_pDevice, s_ssaoVertDecl);
	m_pAo2BlurSmallXPS = CreateA3DHLSL_Common(NULL, NULL, AO2_SSAO_BLURSMALLX_HLSL, "ps", 0, NULL, m_pDevice, s_ssaoVertDecl);
	m_pAo2BlurSmallYPS = CreateA3DHLSL_Common(NULL, NULL, AO2_SSAO_BLURSMALLY_HLSL, "ps", 0, NULL, m_pDevice, s_ssaoVertDecl);
	m_pAo2BlurNew = CreateA3DHLSL_Common(NULL, NULL, AO2_BLUR_NEW_HLSL, "ps", 0, NULL, m_pDevice, s_ssaoVertDecl);

	if(!m_pAo2CommonVS || !m_pAo2LinearDepthPS || !m_pAo2DownSampPS || !m_pAo2SSAOVS || !m_pAo2Cry3PS
		|| !m_pAo2BlurSmallXPS || !m_pAo2BlurSmallYPS || !m_pAo2BlurNew)
	{
		Release();
		return false;
	}

	m_pVertDecl = new A3DVertexDeclCommon;
	if(m_pVertDecl)
		m_pVertDecl->Init(m_pDevice, s_ssaoVertDecl);

	m_fRadius = 0.15f;
	m_fContrast = 0.f;
	m_fDarkest = 0.28f;

	return true;
}


bool A3DSSAO::Render(A3DViewport * pViewPort, A3DRenderTarget* pInputRT)
{	
#ifdef _DEBUGSSAO_
	if(GetKeyState(VK_F1) & 0x8000)
		AOmulti = false;
	else
#endif
		AOmulti = true;

#ifdef _DEBUGSSAO_	
 	if((GetKeyState(VK_F3) & 0x8000))
		return;
#endif
	bool bAlreadyHasFog = false;


	A3DCamera* pCamera = dynamic_cast<A3DCamera*>(pViewPort->GetCamera());
	if(!pCamera)
		return false;

	s_ssaoVert[0].Set(A3DVECTOR3(-1, 1, 0),		0, 0, 0,0,0);
	s_ssaoVert[1].Set(A3DVECTOR3(1,  1, 0),		1, 0, 0,0,0);
	s_ssaoVert[2].Set(A3DVECTOR3(-1, -1, 0),	0, 1, 0,0,0);
	s_ssaoVert[3].Set(A3DVECTOR3(1,  -1, 0),	1, 1, 0,0,0);


	m_pVertDecl->Appear();

	bool bOldAlphaBlendEnable = m_pDevice->GetAlphaBlendEnable();
	bool bOldZTestEnable = m_pDevice->GetZTestEnable();
	bool bOldZWriteEnable = m_pDevice->GetZWriteEnable();

	m_pDevice->SetAlphaBlendEnable(false);
	m_pDevice->SetZTestEnable(false);
	m_pDevice->SetZWriteEnable(false);

	A3DRenderTarget *pMainDepthRT = m_pDevice->GetIntZRenderTarget();


	float fScreenWidth			= (float)m_pDevice->GetDevFormat().nWidth;
	float fScreenHeight			= (float)m_pDevice->GetDevFormat().nHeight;
	float fSmallWidth			= (float)(fScreenWidth / 2);
	float fSmallHeight			= (float)(fScreenHeight / 2);

	float m_FocalLen[2];
	float m_InvFocalLen[2];
	m_FocalLen[0]      = 1.0f / tanf(pCamera->GetFOV() * 0.5f) *  fSmallHeight / fSmallWidth;
	m_FocalLen[1]      = 1.0f / tanf(pCamera->GetFOV() * 0.5f);
	m_InvFocalLen[0]   = 1.0f / m_FocalLen[0];
	m_InvFocalLen[1]   = 1.0f / m_FocalLen[1];

	m_pDevice->SetDeviceRenderState(D3DRS_COLORWRITEENABLE, 
		D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE|D3DCOLORWRITEENABLE_ALPHA);

	//convert to linear depth 
	m_pLinearDepthRT->ApplyToDevice();
	m_pAo2CommonVS->Appear();
	m_pAo2LinearDepthPS->Appear();

	m_pDevice->SetTextureAddress(0, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	m_pDevice->SetTextureFilterType(0, A3DTEXF_POINT);

	pMainDepthRT->AppearAsTexture(0, true);

	A3DVECTOR4 vLinaerPixelSize(1.f / fScreenWidth, 1.f / fScreenHeight, 0, 0);
	m_pDevice->SetVertexShaderConstants(0, &vLinaerPixelSize.x, 1);
	A3DVECTOR4 vLinearDepthParam(pCamera->GetZFront(), pCamera->GetZBack(), m_InvFocalLen[0], m_InvFocalLen[1]);
	m_pDevice->SetPixelShaderConstants(0, &vLinearDepthParam.x, 1);
	A3DVECTOR4 vLinearDepthParam2(1.f / fScreenWidth, 1.f / fScreenHeight, 0, 0);
	m_pDevice->SetPixelShaderConstants(1, &vLinearDepthParam2.x, 1);

	m_pDevice->DrawPrimitiveUP(A3DPT_TRIANGLESTRIP, 2, s_ssaoVert, sizeof(SSAOVert));

	m_pAo2CommonVS->Disappear();
	m_pAo2LinearDepthPS->Disappear();
	m_pLinearDepthRT->WithdrawFromDevice();

	//down sample depth
	m_pSmallGBufferRT->ApplyToDevice();
	m_pAo2CommonVS->Appear();
	m_pAo2DownSampPS->Appear();

	m_pDevice->SetTextureAddress(0, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	m_pDevice->SetTextureFilterType(0, A3DTEXF_POINT);

	m_pLinearDepthRT->AppearAsTexture(0);

	A3DVECTOR4 vSmallGBufferPixelSize(1.f / fSmallWidth, 1.f / fSmallHeight, 0, 0);
	m_pDevice->SetVertexShaderConstants(0, &vSmallGBufferPixelSize.x, 1);
	A3DVECTOR4 vDownSampparam(1.f / (float)fScreenWidth, 1.f / (float)fScreenHeight, 0, 0);
	m_pDevice->SetPixelShaderConstants(0, &vDownSampparam.x, 1);

	m_pDevice->DrawPrimitiveUP(A3DPT_TRIANGLESTRIP, 2, s_ssaoVert, sizeof(SSAOVert));

	m_pAo2CommonVS->Disappear();
	m_pAo2DownSampPS->Disappear();

	m_pSmallGBufferRT->WithdrawFromDevice();

	//SSAO
	m_pSSAORT->ApplyToDevice();

	m_pDevice->Clear(D3DCLEAR_TARGET, A3DCOLORRGB(255, 255, 255), 1.0f, 0);

	m_pAo2SSAOVS->Appear();
	m_pAo2Cry3PS->Appear();

	m_pDevice->SetTextureAddress(0, A3DTADDR_WRAP, A3DTADDR_WRAP);
	m_pDevice->SetTextureFilterType(0, A3DTEXF_POINT);

	m_pDevice->SetTextureAddress(1, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	m_pDevice->SetTextureFilterType(1, A3DTEXF_LINEAR);

	m_pDevice->SetTextureAddress(2, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	m_pDevice->SetTextureFilterType(2, A3DTEXF_LINEAR);

	m_pDevice->SetTextureAddress(3, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	m_pDevice->SetTextureFilterType(3, A3DTEXF_LINEAR);

	m_pDevice->SetTextureAddress(4, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	m_pDevice->SetTextureFilterType(4, A3DTEXF_LINEAR);

	m_pD3DRandom4x4Texture->Appear(0);
	m_pLinearDepthRT->AppearAsTexture(1);
	m_pSmallGBufferRT->AppearAsTexture(2);
	m_pSmallGBufferRTLast->AppearAsTexture(3);
	m_pSSAORTLast->AppearAsTexture(4);


	A3DVECTOR4 vResolutionSmall(1.f / fSmallWidth, 1.f / fSmallHeight, fSmallWidth, fSmallHeight);
	m_pDevice->SetVertexShaderConstants(0, &vResolutionSmall.x, 1);

	static int currentFrame = 0;
	float cf = static_cast<float>(currentFrame % (int)4);
	A3DVECTOR4 vCurrentFrame(cf, 0, 0, 0);
	m_pDevice->SetVertexShaderConstants(1, &vCurrentFrame.x, 1);

	currentFrame++;

#ifdef _DEBUGSSAO_	
	if(GetKeyState(VK_F4) & 0x8000)
		ReloadAo2Shader();
	//if(GetKeyState(VK_F7) & 0x8000)
	//{
	//	SetRaidus(GetRadius() - 0.002f);
	//	g_A3DErrLog.Log("m_fRadius = %f \n", GetRadius());
	//}
	//if(GetKeyState(VK_F8) & 0x8000)
	//{
	//	SetRaidus(GetRadius() + 0.002f);
	//	g_A3DErrLog.Log("m_fRadius = %f \n", GetRadius());
	//}


	//if(GetKeyState(VK_F5) & 0x8000)
	//{
	//	SetContrast(GetContrast() - 0.1f);
	//	g_A3DErrLog.Log("m_fContrast = %f \n", GetContrast());
	//}
	//if(GetKeyState(VK_F6) & 0x8000)
	//{
	//	SetContrast(GetContrast() + 0.1f);
	//	g_A3DErrLog.Log("m_fContrast = %f \n", GetContrast());
	//}


	//if(GetKeyState(VK_HOME) & 0x8000)
	//{
	//	SetDarkest(GetDarkest() - 0.02f);
	//	g_A3DErrLog.Log("m_fDarkest = %f \n", GetDarkest());
	//}
	//if(GetKeyState(VK_END) & 0x8000)
	//{
	//	SetDarkest(GetDarkest() + 0.02f);
	//	g_A3DErrLog.Log("m_fDarkest = %f \n", GetDarkest());
	//}
#endif
	A3DVECTOR4 vSSAOPSParam1(pCamera->GetZBack(), m_fRadius, 1.f / fScreenWidth, 1.f / fScreenHeight);
	m_pDevice->SetPixelShaderConstants(0, &vSSAOPSParam1.x, 1);
	A3DVECTOR4 vSSAOPSParam2(m_FocalLen[0], m_FocalLen[1],  m_InvFocalLen[0], m_InvFocalLen[1]);
	m_pDevice->SetPixelShaderConstants(1, &vSSAOPSParam2.x, 1);
	A3DVECTOR4 vSSAOPSParam3(m_fContrast, m_fDarkest, 0, 0);
	m_pDevice->SetPixelShaderConstants(2, &vSSAOPSParam3.x, 1);

	A3DMATRIX4 trans, scale;
	trans.Translate(0.5, 0.5, 0);
	scale.Scale(0.5, -0.5, 0);
	A3DMATRIX4 bias = scale * trans;

	A3DMATRIX4 viewInv = pCamera->GetViewTM();
	viewInv.InverseTM();
	A3DMATRIX4 deltaMatrix = viewInv * m_matViewProjLastTM * bias;
	deltaMatrix.Transpose();
	m_pDevice->SetPixelShaderConstants(3, &deltaMatrix._11, 4);

	m_pDevice->DrawPrimitiveUP(A3DPT_TRIANGLESTRIP, 2, s_ssaoVert, sizeof(SSAOVert));

	m_pAo2SSAOVS->Disappear();
	m_pAo2Cry3PS->Disappear();

	m_matViewProjLastTM = pCamera->GetViewTM() * pCamera->GetProjectionTM();

	m_pSSAORT->WithdrawFromDevice();

	ASSERT(pInputRT);
	HRESULT r = m_pDevice->StretchRect(pInputRT->GetTargetSurface()->m_pD3DSurface, NULL, 
		m_pInputFrameBuffer->GetTargetSurface()->m_pD3DSurface, NULL, A3DTEXF_POINT);

	//对小的SSAO做过滤
	{
		A3DRenderTarget* pSmallSSAORT1 = m_pDevice->GetRenderTargetMan()->RentRenderTargetColor(-2, -2, A3DFMT_A8R8G8B8);

		m_pDevice->SetDeviceRenderState(D3DRS_COLORWRITEENABLE,  D3DCOLORWRITEENABLE_RED |
			D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);

		m_pDevice->SetTextureAddress(0, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
		m_pDevice->SetTextureFilterType(0, A3DTEXF_POINT);
		m_pDevice->SetTextureAddress(1, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
		m_pDevice->SetTextureFilterType(1, A3DTEXF_POINT);

		m_pAo2CommonVS->Appear();
		m_pAo2BlurSmallXPS->Appear();

		pSmallSSAORT1->ApplyToDevice();
		m_pSSAORT->AppearAsTexture(0);
		m_pSmallGBufferRT->AppearAsTexture(1);
		A3DVECTOR4 vBlurPixelSize(2.f / fScreenWidth, 2.f / fScreenHeight, 0, 0);
		m_pDevice->SetVertexShaderConstants(0, &vBlurPixelSize.x, 1);
		A3DVECTOR4 vBlurPSParam0(2.f / fScreenWidth, 2.f / fScreenHeight, 0, 0);
		m_pDevice->SetPixelShaderConstants(0, &vBlurPSParam0.x, 1);

		m_pDevice->DrawPrimitiveUP(A3DPT_TRIANGLESTRIP, 2, s_ssaoVert, sizeof(SSAOVert));
		m_pAo2BlurSmallXPS->Disappear();
		pSmallSSAORT1->WithdrawFromDevice();


		m_pSSAORT->ApplyToDevice();
		m_pAo2BlurSmallYPS->Appear();

		pSmallSSAORT1->AppearAsTexture(0);
		m_pSmallGBufferRT->AppearAsTexture(1);
		m_pDevice->SetVertexShaderConstants(0, &vBlurPixelSize.x, 1);
		m_pDevice->SetPixelShaderConstants(0, &vBlurPSParam0.x, 1);

		m_pDevice->DrawPrimitiveUP(A3DPT_TRIANGLESTRIP, 2, s_ssaoVert, sizeof(SSAOVert));
		m_pAo2BlurSmallYPS->Disappear();
		m_pSSAORT->WithdrawFromDevice();

		m_pAo2CommonVS->Disappear();

		m_pDevice->GetRenderTargetMan()->ReturnRenderTarget(pSmallSSAORT1);

#ifdef _DEBUGSSAO_
		if(GetKeyState(VK_F2) & 0x8000)
		{
			D3DXSaveSurfaceToFileA("SSAOTemp.dds", D3DXIFF_DDS, pSmallSSAORT1->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
			D3DXSaveSurfaceToFileA("SSAO.dds", D3DXIFF_DDS, m_pSSAORT->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
		}
#endif
	}
	//Blur
	m_pDevice->SetAlphaBlendEnable(false);

	//注意, 似乎Alpha通道被用来做了一些事情, 绝对不要写入
	m_pDevice->SetDeviceRenderState(D3DRS_COLORWRITEENABLE,  D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);

	m_pAo2CommonVS->Appear();

	
#ifdef _ANGELICA_2_1
	m_pAo2BlurNew->Appear();
	m_pDevice->GetA3DEngine()->GetSceneRenderConfig()->AppearFog(3, false);
#else
	m_pAo2BlurNew->Appear(m_pDevice->GetA3DEngine()->GetA3DEnvironment()->GetCommonConstTable());
#endif

	m_pSSAORT->AppearAsTexture(0);
	m_pDevice->SetTextureAddress(0, A3DTADDR_BORDER, A3DTADDR_BORDER);
	m_pDevice->SetTextureFilterType(0, A3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_BORDERCOLOR, 0xFFFFFFFF);

	m_pLinearDepthRT->AppearAsTexture(1);
	m_pDevice->SetTextureAddress(1, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	m_pDevice->SetTextureFilterType(1, A3DTEXF_LINEAR);

	m_pDevice->GetIntZRenderTarget()->AppearAsTexture(2, true);
	m_pDevice->SetTextureAddress(2, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	m_pDevice->SetTextureFilterType(2, A3DTEXF_LINEAR);

	m_pInputFrameBuffer->AppearAsTexture(3);
	m_pDevice->SetTextureAddress(3, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	m_pDevice->SetTextureFilterType(3, A3DTEXF_POINT);

	m_pSmallGBufferRT->AppearAsTexture(4);
	m_pDevice->SetTextureAddress(4, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	m_pDevice->SetTextureFilterType(4, A3DTEXF_LINEAR);


	A3DVECTOR4 vBlurPixelSize(1.f / fScreenWidth, 1.f / fScreenHeight, 0, 0);
	m_pDevice->SetVertexShaderConstants(0, &vBlurPixelSize.x, 1);

	A3DVECTOR4 vBlurPSParam0(1.f / fSmallWidth, 1.f / fSmallHeight, 0, 0);
	m_pDevice->SetPixelShaderConstants(0, &vBlurPSParam0.x, 1);
	float fDarkest = 0.0;
	static float fBlurLength = 1.25f;

	A3DVECTOR4 vBlurPSParam1(pCamera->GetZBack(), fDarkest, 0, 0);
	m_pDevice->SetPixelShaderConstants(1, &vBlurPSParam1.x, 1);

	A3DCamera* cam = pCamera;
	A3DMATRIX4 invView = cam->GetViewTM().GetInverseTM();

	float fovy_2 = cam->GetFOV() / 2.f;
	float aspRatio = cam->GetRatio();

	float f = cam->GetZBack();
	float top = f * tan(fovy_2);
	float bottom = -top;
	float right = top * aspRatio;
	float left = -right;
	A3DVECTOR3 v[4];
	v[0] = A3DVECTOR3(left, top, f);
	v[1] = A3DVECTOR3(right, top, f);
	v[2] = A3DVECTOR3(left, bottom, f);
	v[3] = A3DVECTOR3(right, bottom, f);

	for(int i=0;i<4;i++)
	{
		A3DVECTOR4 temp = v[i] * invView;
		A3DVECTOR3 temp2;
		temp2 = A3DVECTOR3(temp.x, temp.y, temp.z) - cam->GetPos();
		*(A3DVECTOR3 *)s_ssaoVert[i].fogparam = temp2;
	}

	A3DVECTOR4 g_cameraPos(cam->GetPos().x, cam->GetPos().y, cam->GetPos().z, 1);
	m_pDevice->SetPixelShaderConstants(8, &g_cameraPos.x, 1);

	m_pDevice->DrawPrimitiveUP(A3DPT_TRIANGLESTRIP, 2, s_ssaoVert, sizeof(SSAOVert));

	m_pAo2CommonVS->Disappear();


	m_pAo2BlurNew->Disappear();

#ifdef _DEBUGSSAO_
	//   	if(GetKeyState(VK_F2) & 0x8000)
	//   	{
	// 		D3DXSaveSurfaceToFileA("LinearDepth.dds", D3DXIFF_DDS, m_pLinearDepthRT->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
	//  		D3DXSaveSurfaceToFileA("SmallDepth.dds", D3DXIFF_DDS, m_pSmallGBufferRT->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
	// 		D3DXSaveSurfaceToFileA("SSAOLast.dds", D3DXIFF_DDS, m_pSSAORTLast->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
	// 		D3DXSaveSurfaceToFileA("InputFrameBuffer.dds", D3DXIFF_DDS, m_pInputFrameBuffer->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
	// 		D3DXSaveSurfaceToFileA("AfterFrameBuffer.dds", D3DXIFF_DDS, pFrameBufferSurface, NULL, NULL);
	//   	}
#endif

	abase::swap(m_pSSAORT, m_pSSAORTLast);
	abase::swap(m_pSmallGBufferRT, m_pSmallGBufferRTLast);


	//还原渲染状态
	m_pDevice->ClearTexture(0);
	m_pDevice->ClearTexture(1);
	m_pDevice->ClearTexture(2);
	m_pDevice->ClearTexture(3);
	m_pDevice->ClearTexture(4);
	m_pDevice->SetSourceAlpha(A3DBLEND_SRCALPHA);
	m_pDevice->SetDestAlpha(A3DBLEND_INVSRCALPHA);
#ifdef _ANGELICA_2_1
	m_pDevice->SetBlendEquation(D3DBLENDOP_ADD);
#endif
	m_pDevice->SetAlphaBlendEnable(bOldAlphaBlendEnable);
	m_pDevice->SetZTestEnable(bOldZTestEnable);
	m_pDevice->SetZWriteEnable(bOldZWriteEnable);

	m_pDevice->SetTextureAddress(0,A3DTADDR_WRAP,A3DTADDR_WRAP);
	m_pDevice->SetTextureAddress(1,A3DTADDR_WRAP,A3DTADDR_WRAP);
	m_pDevice->SetTextureAddress(2,A3DTADDR_WRAP,A3DTADDR_WRAP);
	m_pDevice->SetTextureAddress(3,A3DTADDR_WRAP,A3DTADDR_WRAP);
	m_pDevice->SetTextureAddress(4,A3DTADDR_WRAP,A3DTADDR_WRAP);
	m_pDevice->SetTextureFilterType(3, A3DTEXF_LINEAR);

	return true;
}


void A3DSSAO::ReloadAo2Shader()
{
	ReloadA3DHLSL_Common(m_pAo2CommonVS);
	ReloadA3DHLSL_Common(m_pAo2DownSampPS);
	ReloadA3DHLSL_Common(m_pAo2LinearDepthPS);
	ReloadA3DHLSL_Common(m_pAo2SSAOVS);
	ReloadA3DHLSL_Common(m_pAo2Cry3PS);
	ReloadA3DHLSL_Common(m_pAo2BlurNew);
	ReloadA3DHLSL_Common(m_pAo2BlurSmallXPS);
	ReloadA3DHLSL_Common(m_pAo2BlurSmallYPS);
}

void A3DSSAO::Release()
{
	A3DRELEASE(m_pSmallGBufferRT);
	A3DRELEASE(m_pSmallGBufferRTLast);
	A3DRELEASE(m_pLinearDepthRT);
	A3DRELEASE(m_pSSAORT);
	A3DRELEASE(m_pSSAORTLast);
	A3DRELEASE(m_pInputFrameBuffer);
	A3DRELEASE(m_pD3DRandom4x4Texture);

	ReleaseA3DHLSL_Common(m_pDevice, m_pAo2CommonVS);
	ReleaseA3DHLSL_Common(m_pDevice, m_pAo2DownSampPS);
	ReleaseA3DHLSL_Common(m_pDevice, m_pAo2LinearDepthPS);
	ReleaseA3DHLSL_Common(m_pDevice, m_pAo2SSAOVS);
	ReleaseA3DHLSL_Common(m_pDevice, m_pAo2Cry3PS);
	ReleaseA3DHLSL_Common(m_pDevice, m_pAo2BlurNew);
	ReleaseA3DHLSL_Common(m_pDevice, m_pAo2BlurSmallXPS);
	ReleaseA3DHLSL_Common(m_pDevice, m_pAo2BlurSmallYPS);

	A3DRELEASE(m_pVertDecl);
}

float A3DSSAO::GetRadius() const
{
	return m_fRadius;
}

void A3DSSAO::SetRaidus(float fRadius)
{
	m_fRadius = fRadius;
	m_fRadius = min2(2.f, m_fRadius);
	m_fRadius = max2(0.f, m_fRadius);
}

float A3DSSAO::GetContrast() const
{
	return m_fContrast;
}

void A3DSSAO::SetContrast(float fContrast)
{
	m_fContrast = fContrast;
	m_fContrast = min2(6.f, m_fContrast);
	m_fContrast = max2(0.f, m_fContrast);
}

float A3DSSAO::GetDarkest() const
{
	return m_fDarkest;
}

void A3DSSAO::SetDarkest(float fDarkest)
{
	m_fDarkest = fDarkest;
	m_fDarkest = min2(1.f, m_fDarkest);
	m_fDarkest = max2(0.f, m_fDarkest);
}
