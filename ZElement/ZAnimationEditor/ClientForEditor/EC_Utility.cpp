/*
 * FILE: EC_Utility.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/9/2
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#define _WIN32_WINDOWS 0x0410 

#include "EC_Global.h"
#include "EC_Utility.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_World.h"
#include "EC_CDS.h"
#include "EC_SoundCache.h"
#include "EC_Configs.h"
#include "EC_ShadowRender.h"
#include "EC_SceneLights.h"
#include "EC_PortraitRender.h"
#include "EC_Model.h"
#include "A3DGfxExMan.h"
#include "A3DSkillGfxComposer.h"
#ifdef BACK_VERSION
#include "EC_Viewport.h"
#else
#include "ECViewport.h"
#endif

#include "A3DMacros.h"
#include "A3DLight.h"
#include "A3DFuncs.h"
#include "A3DSkin.h"
#include "A3DShader.h"
#include "A3DShaderMan.h"
#include "AC.h"
#include "AFI.h"
#include "A3DMathUtility.h"
#include "A3DCamera.h"
#include "AMiniDump.h"
//#include "AUI\AUICommon.h"
#include "A3DTerrain2.h"
#include "EC_TriangleMan.h"
#include "EC_ManOrnament.h"

extern "C"
{
	#define XMD_H
	#define NEED_FAR_POINTERS
	#undef FAR
	#include "JpegLib\jpeglib.h"
	#include "JpegLib\cdjpeg.h"
}

#include <winbase.h>
//	#include <time.h>

#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

#define	MAX_MOVECLIP_PLANES		10

///////////////////////////////////////////////////////////////////////////
//	
//	Reference to External variables and functions
//	
///////////////////////////////////////////////////////////////////////////

bool g_bExceptionOccured = false;
DWORD g_dwFatalErrorFlag = 0;

///////////////////////////////////////////////////////////////////////////
//	
//	Local Types and Variables and Global variables
//	
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////

/*	Clip velocity along specified plane.

	vInVel: original velocity.
	vNormal: plane's normal
	vOutVel: result velocity.
	fOverBounce: over bounce.
*/
static void _ClipVelocity(const A3DVECTOR3& vInVel, const A3DVECTOR3& vNormal, A3DVECTOR3& vOutVel, float fOverBounce)
{
	float fBackOff = DotProduct(vInVel, vNormal);
	
	if (fBackOff < 0.0f)
		fBackOff *= fOverBounce;
	else
		fBackOff = 0.0f;// /= fOverBounce;
	
	vOutVel = vInVel - vNormal * fBackOff;

	if (Magnitude(vOutVel) > 1000)
		ASSERT(0);
}

//	Format the specified FILETIME to output in a human readable format,
//	without using the C run time.
static void _FormatTime(LPTSTR output, FILETIME TimeToPrint)
{
	output[0] = _AL('\0');
	WORD Date, Time;
	if (FileTimeToLocalFileTime(&TimeToPrint, &TimeToPrint) &&
		FileTimeToDosDateTime(&TimeToPrint, &Date, &Time))
	{
		wsprintf(output, _AL("%d-%d-%d-%02d-%02d-%02d"),
				(Date / 512) + 1980, (Date / 32) & 15, Date & 31,
				(Time >> 11), (Time >> 5) & 0x3F, (Time & 0x1F) * 2);
	}
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement
//	
///////////////////////////////////////////////////////////////////////////

//	Create a directory
bool glb_CreateDirectory(const char* szDir)
{
	AString strDir = szDir;
	int iLen = strDir.GetLength();

	if (iLen <= 3)
		return true;

	//	Clear the last '\\'
	if (strDir[iLen-1] == '\\')
		strDir[iLen-1] = '\0';

	//	Save current directory
	char szCurDir[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, szCurDir);

	AString strTemp;

	int iPos = strDir.Find('\\', 0);
	while (iPos > 0)
	{
		strTemp = strDir.Mid(0, iPos);
		CreateDirectoryA(strTemp, NULL);
		iPos = strDir.Find('\\', iPos+1);
	}

	CreateDirectoryA(szDir, NULL);

	//	Restore current directory
	SetCurrentDirectoryA(szCurDir);

	return true;
}

/*	Check whether specified file exist.

	Return true specified file exist, otherwise return false.

	szFile: file's full name
*/
bool glb_FileExist(const char* szFile)
{
	if (!szFile || !szFile[0])
		return false;

	FILE* fp = fopen(szFile, "rb");
	if (!fp)
		return false;

	fclose(fp);
	return true;
}

/*	Change file's extension

	szFile (in, out): file name.
	szNewExt: new extension of file
*/
void glb_ChangeExtension(char* szFile, char* szNewExt)
{
#ifdef _DEBUG
	
	int iLen = strlen(szFile);
	if (iLen <= 4)
	{
		ASSERT(iLen > 4);
		return;
	}

#endif

	char* pTemp = strrchr(szFile, '.');
	if (!pTemp)
	{
		strcat(szFile, ".");
		strcat(szFile, szNewExt);
	}
	else
	{
		strcpy(pTemp+1, szNewExt);
	}
}

void glb_ClearExtension(char* szFile)
{
	char* pTemp = strrchr(szFile, '.');
	if (pTemp)
		*pTemp = '\0';
}

bool glb_WriteString(FILE* fp, const char* str)
{
	ASSERT(str);

	//	Write length of string
	int iLen = strlen(str);
	fwrite(&iLen, 1, sizeof (int), fp);

	//	Write string data
	if (iLen)
		fwrite(str, 1, iLen, fp);

	return true;
}

bool glb_PercentProbability(int iPercent)
{
	int iRand = a_Random() % 100;
	return iRand < iPercent ? true : false;
}

bool glb_PercentProbability(float fPercent)
{
	return a_Random(0.0f, 100.0f) < fPercent;
}

A3DVECTOR3 glb_RandomVectorH(int* piDegree/* NULL */)
{
	ASSERT(g_pA3DMathUtility);

	int iDegree = a_Random(0, 359);

	if (piDegree)
		*piDegree = iDegree;

	return A3DVECTOR3(g_pA3DMathUtility->COS((float)iDegree), 0.0f, g_pA3DMathUtility->SIN((float)iDegree));
}

A3DVECTOR3 glb_RandomVector()
{
	ASSERT(g_pA3DMathUtility);

	A3DVECTOR3 vVec;
	float fScale;

	int iDegree = a_Random(0, 179);
	vVec.y = g_pA3DMathUtility->SIN((float)iDegree);
	fScale = g_pA3DMathUtility->COS((float)iDegree);

	iDegree = a_Random(0, 359);
	vVec.x = g_pA3DMathUtility->COS((float)iDegree) * fScale;
	vVec.z = g_pA3DMathUtility->SIN((float)iDegree) * fScale;

	return vVec;
}

//	Compress horizontal direction to a byte
BYTE glb_CompressDirH(float x, float z)
{
	static const float fInvInter = 256.0f / 360.0f;

	if (fabs(x) < 0.00001)
	{
		if (z > 0.0f)
			return 64;
		else
			return 192;
	}
	else
	{
		float fDeg = RAD2DEG((float)atan2(z, x));
		return (BYTE)(fDeg * fInvInter);
	}
}

//	Decompress horizontal direction
A3DVECTOR3 glb_DecompressDirH(BYTE byDir)
{
	static const float fInter = 360.0f / 256.0f;

	float fRad = DEG2RAD(byDir * fInter);
	A3DVECTOR3 v;
	v.x = (float)cos(fRad);
	v.z = (float)sin(fRad);
	v.y = 0.0f;

	return v;
}

//	Integer compare function used by qsort
int glb_IntCompare(const void* arg1, const void* arg2)
{
	int i1 = *(int*)arg1;
	int i2 = *(int*)arg2;
	
	if (i1 < i2)
		return -1;
	else if (i1 > i2)
		return 1;
	else
		return 0;
}

int glb_WordCompare(const void* arg1, const void* arg2)
{
	WORD i1 = *(WORD*)arg1;
	WORD i2 = *(WORD*)arg2;
	
	if (i1 < i2)
		return -1;
	else if (i1 > i2)
		return 1;
	else
		return 0;
}

unsigned long glb_HandleException(LPEXCEPTION_POINTERS pExceptionPointers)
{
//	if( IsDebuggerPresent() )
//		return EXCEPTION_CONTINUE_SEARCH;

	/*
	//	Use current time to build file name
	FILETIME CurrentTime;
	GetSystemTimeAsFileTime(&CurrentTime);
	TCHAR szTimeBuffer[100];
	_FormatTime(szTimeBuffer, CurrentTime);
	
	TCHAR szFile[MAX_PATH];

#ifdef UNICODE
	wsprintf(szFile, _AL("%S\\Logs\\ec_%s.dmp"), g_szWorkDir, szTimeBuffer);
#else
	wsprintf(szFile, _AL("%s\\Logs\\ec_%s.dmp"), g_szWorkDir, szTimeBuffer);
#endif
	*/

	TCHAR szFile[MAX_PATH];
	extern DWORD GAME_BUILD;
	wsprintf(szFile, _AL("%S\\Logs\\ec_build%d.dmp"), g_szWorkDir, GAME_BUILD);

	if( GetVersion() < 0x80000000 )
	{
		// WinNT or Win2000
		//AMiniDump::Create(NULL, pExceptionPointers, szFile, &g_csException);
	}
	else
	{
		// Win95 or Win98 or Win32		
	}

	// if dump success, we should launch bug report program here
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));
	CreateProcess(L"ReportBugs\\ZReportBugs.exe", NULL, NULL, NULL, FALSE, 0, NULL, L"ReportBugs", &si, &pi);

	g_bExceptionOccured = true;

	return EXCEPTION_EXECUTE_HANDLER;
}

//	Get format local time
tm* glb_GetFormatLocalTime()
{
	time_t t;
	time(&t);
	return localtime(&t);
}

ACString glb_FilterEditboxText(const ACHAR* szText)
{
	return L"";
}

bool glb_ConvertBmpToJpeg(const char* szBmp, const char* szJpeg, int nQuality)
{
	FILE * input_file;
	FILE * output_file;
	jpeg_compress_struct cinfo;
	jpeg_error_mgr jerr;
	cjpeg_source_ptr src_mgr;
	JDIMENSION num_scanlines;

	input_file = fopen(szBmp, "rb");

	if (!input_file)
		return false;

	output_file = fopen(szJpeg, "wb");

	if (!output_file)
	{
		fclose(input_file);
		return false;
	}

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);

	cinfo.in_color_space = JCS_RGB;
	jpeg_set_defaults(&cinfo);

	/* Figure out the input file format, and set up to read it. */
	src_mgr = jinit_read_bmp(&cinfo);
	src_mgr->input_file = input_file;

	/* Read the input file header to obtain file size & colorspace. */
	(*src_mgr->start_input)(&cinfo, src_mgr);

	/* Now that we know input colorspace, fix colorspace-dependent defaults */
	jpeg_default_colorspace(&cinfo);

	/* Specify data destination for compression */
	jpeg_stdio_dest(&cinfo, output_file);

	jpeg_set_quality(&cinfo, nQuality, TRUE);

	/* Start compressor */
	jpeg_start_compress(&cinfo, TRUE);

	/* Process data */
	while (cinfo.next_scanline < cinfo.image_height)
	{
		num_scanlines = (*src_mgr->get_pixel_rows) (&cinfo, src_mgr);
		(void) jpeg_write_scanlines(&cinfo, src_mgr->buffer, num_scanlines);
	}

	/* Finish compression and release memory */
	(*src_mgr->finish_input) (&cinfo, src_mgr);
	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);

	/* Close files, if we opened them */
	fclose(input_file);
    fclose(output_file);
	return true;
}

void glb_ReplaceShader(A3DSkin * pSkin)
{
	if( !pSkin )
		return;

	int i;
	for(i=0; i < pSkin->GetTextureNum(); i++)
	{
		A3DTexture* pTex = pSkin->GetTexture(i);
		const char* szTexMap = pTex->GetMapFile();
		bool bAlphaTex = pTex->IsAlphaTexture();

		if (szTexMap && szTexMap[0])
		{
			if( pTex->IsShaderTexture() )
			{
			}
			else
			{
				char szFileTitle[MAX_PATH];
				af_GetFileTitle(szTexMap, szFileTitle, MAX_PATH);
				strlwr(szFileTitle);

				if( strstr(szFileTitle, "a_") == szFileTitle )
				{
					A3DShader* pReplaceShader;
					pReplaceShader = g_pGame->GetA3DDevice()->GetA3DEngine()->GetA3DShaderMan()->LoadShaderFile("armor1.sdr");
					if (pReplaceShader)
					{
						pReplaceShader->ChangeStageTexture(0, A3DSDTEX_NORMAL, (DWORD)szTexMap);
						pSkin->ChangeSkinTexturePtr(i, pReplaceShader);
						pReplaceShader->SetOnlyActiveStagesFlag(true);
						pReplaceShader->SetAlphaTextureFlag(true);
					}
					continue;
				}
				
				if( strstr(szFileTitle, "r_") == szFileTitle )
				{
					A3DShader* pReplaceShader;
					pReplaceShader = g_pGame->GetA3DDevice()->GetA3DEngine()->GetA3DShaderMan()->LoadShaderFile("armor.sdr");
					if (pReplaceShader)
					{
						pReplaceShader->ChangeStageTexture(0, A3DSDTEX_NORMAL, (DWORD)szTexMap);
						pSkin->ChangeSkinTexturePtr(i, pReplaceShader);
						pReplaceShader->SetOnlyActiveStagesFlag(true);
						pReplaceShader->SetAlphaTextureFlag(false);
					}
					continue;
				}

				if( strstr(szFileTitle, "zx_") == szFileTitle )
				{
					A3DShader* pReplaceShader = g_pGame->GetA3DDevice()->GetA3DEngine()->GetA3DShaderMan()->LoadShaderFile("armor2.sdr");
					if (pReplaceShader)
					{
						pReplaceShader->ChangeStageTexture(0, A3DSDTEX_NORMAL, (DWORD)szTexMap);
						
						if( pReplaceShader->GetGeneralProps().pPShader )
						{
							char szTexMap2[MAX_PATH];
							af_GetFilePath(szTexMap, szTexMap2, MAX_PATH);
							strcat(szTexMap2, "\\nb_");
							strcat(szTexMap2, szFileTitle);
							pReplaceShader->ChangeStageTexture(1, A3DSDTEX_NORMAL, (DWORD)szTexMap2);
						}
						else
						{
							pReplaceShader->ShowStage(2, false);
						}

						pSkin->ChangeSkinTexturePtr(i, pReplaceShader);
						pReplaceShader->SetOnlyActiveStagesFlag(true);
						pReplaceShader->SetAlphaTextureFlag(true);
					}
				}
			}
		}
	}
}

#ifdef BACK_VERSION
// ECModel's render for shadow call back function
bool ECModelRenderForShadow(A3DViewport * pViewport, void * pArg)
{
	A3DSkinModel * pModel = (A3DSkinModel *) pArg;
	g_pGame->GetA3DDevice()->SetZTestEnable(true);
	g_pGame->GetA3DDevice()->SetZWriteEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(false);
	g_pGame->GetA3DDevice()->SetAlphaFunction(A3DCMP_GREATEREQUAL);
	g_pGame->GetA3DDevice()->SetAlphaRef(84);

	pModel->RenderAtOnce(pViewport, A3DSkinModel::RAO_NOTEXTURE | A3DSkinModel::RAO_NOMATERIAL, false);

	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(false);
	g_pGame->GetA3DDevice()->SetZTestEnable(true);
	g_pGame->GetA3DDevice()->SetZWriteEnable(true);
	return true;
}
#endif

void ECModelRenderDemonstration(const A3DRECT &rc, DWORD param1, DWORD param2, DWORD param3)
{
	static const float _dist_co = 6.8f;

	if( param1 == 0 )
		return;

	CECModel * pModel = (CECModel*)param1;
	float vDeg = DEG2RAD(param2);

	A3DViewport * pCurViewport = g_pGame->GetA3DEngine()->GetActiveViewport();

	A3DViewport * pPortraitViewport = g_pGame->GetPortraitRender()->GetPortraitViewport();
	A3DCameraBase * pCamera = pPortraitViewport->GetCamera();

	A3DVIEWPORTPARAM viewParam = *pPortraitViewport->GetParam();
	viewParam.X = rc.left;
	viewParam.Y = rc.top;
	viewParam.Width = rc.right - rc.left;
	viewParam.Height = rc.bottom - rc.top;
	pPortraitViewport->SetParam(&viewParam);
	((A3DCamera *) pCamera)->SetProjectionParam(-1.0f, -1.0f, -1.0f, ((float)viewParam.Width) / viewParam.Height);

	const A3DAABB& aabb = pModel->HasCHAABB() ? pModel->GetCHAABB() : pModel->GetModelAABB();
	float radius = sqrtf(aabb.Extents.x * aabb.Extents.x + aabb.Extents.z * aabb.Extents.z);
	float width = radius * viewParam.Height / viewParam.Width;
	float size = max(aabb.Extents.y, width);
	if (size < .1f) size = .1f;

	A3DMATRIX4 mat = a3d_RotateY(vDeg);
	A3DMATRIX4 matRot = a3d_RotateX(DEG2RAD(30.0f)) * mat;
	A3DVECTOR3 vecPos = A3DVECTOR3(0, aabb.Extents.y, -size * _dist_co) * matRot;
	A3DVECTOR3 vecDir = A3DVECTOR3(0, 0, 1.0f) * matRot;
	A3DVECTOR3 vecUp = A3DVECTOR3(0, 1.0f, 0) * matRot;

	pCamera->SetPos(vecPos);
	pCamera->SetDirAndUp(vecDir, vecUp);
	pPortraitViewport->Active();
	pPortraitViewport->ClearDevice();
	pModel->Render(pPortraitViewport, true);

	pCurViewport->Active();
}

CECViewport* AfxGetViewport()
{
	return g_pGame->GetViewport();
}

A3DLight*  AfxGetDirLight()
{
	return g_pGame->GetDirLight();
}

CECGameRun* AfxGetGameRun()
{
	return g_pGame->GetGameRun();
}
CECConfigs* AfxGetConfigs()
{
	return g_pGame->GetConfigs();
}

#ifdef BACK_VERSION?
A3DSkinRender* AfxGetSkinRender1()
#else
A3DSkinRenderBase* AfxGetSkinRender1()
#endif
{
	return g_pGame->GetSkinRender1();
}

CECGFXCaster* AfxGetGfxCaster()
{
	return g_pGame->GetGFXCaster();
}

#ifdef BACK_VERSION
A3DSkinRender* AfxGetSkinRender2()
#else
A3DSkinRenderBase* AfxGetSkinRender2()
#endif
{
	return g_pGame->GetSkinRender2();
}

//	Below funcions are used by GFX lib
A3DEngine* AfxGetA3DEngine()
{
	return g_pGame->GetA3DEngine();
}

A3DDevice* AfxGetA3DDevice()
{
	return g_pGame->GetA3DDevice();
}

float AfxGetAverageFrameRate()
{
	return g_pGame->GetAverageFrameRate();
}

A3DGFXExMan* AfxGetGFXExMan()
{
	return g_pGame->GetA3DGFXExMan();
}

CECModelMan* AfxGetECModelMan()
{
	return g_pGame->GetECModelMan();
}

A3DSkinModel* AfxLoadA3DSkinModel(const char* szFile, int iSkinFlag)
{
	return g_pGame->LoadA3DSkinModel(szFile, iSkinFlag);
}

void AfxReleaseA3DSkinModel(A3DSkinModel* pModel)
{
	g_pGame->ReleaseA3DSkinModel(pModel);
}

const A3DLIGHTPARAM& AfxGetLightparam()
{
	return g_pGame->GetDirLight()->GetLightparam();
}

float AfxGetGrndNorm(const A3DVECTOR3& vPos, A3DVECTOR3* pNorm)
{
	return g_pGame->GetGameRun()->GetWorld()->GetTerrainHeight(vPos, pNorm);
}

bool AfxRayTrace(const A3DVECTOR3& vStart, const A3DVECTOR3& vVel)
{
	CECCDS* pCDS = g_pGame->GetGameRun()->GetWorld()->GetCDS();
	RAYTRACERT TraceRt;

	return pCDS->RayTrace(vStart, vVel, 1.0f, &TraceRt, TRACEOBJ_LBTNCLICK, 0);
}

A3DVECTOR3 _cam_shake_offset(0);
bool _shaking_cam = false;
static int _shaking_count = 0;

A3DCamera * AfxGetA3DCamera(void)
{
	return g_pGame->GetViewport()->GetA3DCamera();
}

bool AfxGetModelUpdateFlag(void)
{
	return g_pGame->GetConfigs()->GetModelUpdateFlag();
}

void AfxBeginShakeCam()
{
	_shaking_count++;
	_shaking_cam = true;
}

void AfxEndShakeCam()
{
	if (--_shaking_count == 0)
	{
		_shaking_cam = false;
		_cam_shake_offset.Clear();
	}
}

void AfxSetCamOffset(const A3DVECTOR3& vOffset)
{
	if ( _isnan(vOffset.x) || _isnan(vOffset.y) || _isnan(vOffset.z) )	//如果传入的offset值非法(溢出)，则不进行累加，防止相机位置非法 Modified 2012-04-20.
	{
		//是否需要清掉相机的累加偏移值？
		return;
	}
	else
	{
		_cam_shake_offset += vOffset;
	}
}

AM3DSoundBuffer* AfxLoadNonLoopSound(const char* szFile, int nPriority)
{
	return g_pGame->GetSoundCache()->LoadSound(szFile, enumSoundLoadNonLoop);
}

AM3DSoundBuffer* AfxLoadLoopSound(const char* szFile)
{
	return g_pGame->GetSoundCache()->LoadSound(szFile, enumSoundLoadLoop);
}

void AfxReleaseSoundNonLoop(AM3DSoundBuffer*& pSound)
{
	g_pGame->GetSoundCache()->ReleaseSound(pSound, enumSoundLoadNonLoop);
}

void AfxReleaseSoundLoop(AM3DSoundBuffer*& pSound)
{
	g_pGame->GetSoundCache()->ReleaseSound(pSound, enumSoundLoadLoop);
}

void AfxECModelAddShadower(const A3DVECTOR3& vecCenter, const A3DAABB& aabb, A3DSkinModel* pA3DSkinModel)
{
#ifdef BACK_VERSION
	if (g_pGame->GetShadowRender() && !g_pGame->GetConfigs()->GetSystemSettings().bSimpleShadow)
		g_pGame->GetShadowRender()->AddShadower(aabb.Center, aabb, SHADOW_RECEIVER_TERRAIN, ECModelRenderForShadow, pA3DSkinModel, false);
#endif
}

void AfxSetA3DSkinModelSceneLightInfo(A3DSkinModel* pA3DSkinModel)
{
	CECSceneLights* pSceneLights = g_pGame->GetGameRun()->GetWorld()->GetSceneLights();
	A3DSkinModel::LIGHTINFO LightInfo = pA3DSkinModel->GetLightInfo();
	LightInfo.colAmbient	= g_pGame->GetA3DDevice()->GetAmbientValue();
	LightInfo.vLightDir		= g_pGame->GetDirLight()->GetLightparam().Direction;
	LightInfo.colDirDiff	= g_pGame->GetDirLight()->GetLightparam().Diffuse;

	if( pSceneLights )
	{
		CECSceneLights::SCENELIGHT theLight;
		if( pSceneLights->GetSceneLight(pA3DSkinModel->GetAbsoluteTM().GetRow(3), theLight) )
		{
			LightInfo.bPtLight = true;
			LightInfo.colPtDiff = theLight.diffuse;
			LightInfo.colPtAmb =  theLight.ambient;
			LightInfo.fPtRange = theLight.vRange;
			LightInfo.vPtAtten = A3DVECTOR3(theLight.vAttenuation0, theLight.vAttenuation1, theLight.vAttenuation2);
			LightInfo.vPtLightPos = theLight.vecPos;
		}
		else
			LightInfo.bPtLight = false;
	}

	pA3DSkinModel->SetLightInfo(LightInfo);
}

bool AfxPlaySkillGfx(const AString& strAtkFile, unsigned char SerialID, clientid_t nCasterID, clientid_t nCastTargetID, const A3DVECTOR3* pFixedPoint, int nDivisions, TargetDataVec& Targets)
{
//	if (g_pGame->GetGameRun()->IsAnmPlaying())
//		return true;

	using namespace _SGC;
	A3DSkillGfxComposerMan* pMan = AfxGetSkillGfxComposerMan();
	A3DSkillGfxMan* pEventMan = AfxGetSkillGfxEventMan();

	pMan->Play(
		strAtkFile,
		SerialID,
		nCasterID,
		nCastTargetID,
		pFixedPoint);

	A3DSkillGfxEvent* pEvent = pEventMan->GetSkillGfxEvent(nCasterID, SerialID);

	if (!pEvent)
		return true;

	//pEvent = pEvent->GetLast();
	pEvent->SetDivisions(nDivisions);

	for (size_t i = 0; i < Targets.size(); i++)
		pEvent->AddOneTarget(Targets[i]);

	return true;
}

void AfxSkillGfxShowDamage(clientid_t idCaster, clientid_t idTarget, int nDamage, int nDivisions, DWORD dwModifier)
{
//	if (g_pGame->GetGameRun()->IsAnmPlaying())
//		return;
	
	using namespace _SGC;
	A3DSkillGfxMan::GetDamageShowFunc()(idCaster, idTarget, nDamage, 1, dwModifier);
}

void AfxSkillGfxShowCaster(clientid_t idCaster, DWORD dwModifier)
{
//	if (g_pGame->GetGameRun()->IsAnmPlaying())
//		return;

	using namespace _SGC;
	A3DSkillGfxMan::GetCasterShowFunc()(idCaster, dwModifier);
}

bool AfxSkillGfxAddDamageData(clientid_t nCaster, clientid_t nTarget, unsigned char SerialId, DWORD dwModifier, int nDamage)
{
//	if (g_pGame->GetGameRun()->IsAnmPlaying())
//		return true;

	using namespace _SGC;
	A3DSkillGfxMan* pMan = AfxGetSkillGfxEventMan();
	A3DSkillGfxEvent* pEvent = pMan->GetSkillGfxEvent(nCaster, SerialId);

	if (pEvent)
	{
		TARGET_DATA td;
		td.dwModifier = dwModifier;
		td.idTarget = nTarget;
		td.nDamage = nDamage;

		if (!pEvent->AddOneTarget(td))
			A3DSkillGfxMan::GetDamageShowFunc()(nCaster, nTarget, nDamage, 1, td.dwModifier);

		return true;
	}

	return false;
}

const char* AfxGetECMHullPath()
{
	return "Models\\ECModelHull";
}

bool gGfxGetSurfaceData(const A3DVECTOR3& vCenter, float fRadus, A3DVECTOR3* pVerts, int& nVertCount, WORD* pIndices, int& nIndexCount)
{
	A3DTerrain2* pTerrain = g_pGame->GetGameRun()->GetWorld()->GetTerrain();
	int _nVertCount = 0;
	int _nIndexCount = 0;
	int nTriVertCount = 0;
	int nTriIndexCount = 0;

	if (pTerrain)
	{
		int nx, nz;
		nx = nz = int(fRadus * 2.0f / pTerrain->GetGridSize()) + 2;
		_nVertCount = (nx + 1) * (nz + 1);
		_nIndexCount = nx * nz * 6;

		if (_nVertCount > nVertCount || _nIndexCount > nIndexCount)
			return false;

		RAYTRACERT trace;
		A3DVECTOR3 v = vCenter;
		v.y += 1.0f;

		if (!pTerrain->RayTrace(v, A3DVECTOR3(0, -4.f, 0), 1.0f, &trace)
	     || !pTerrain->GetFacesOfArea(trace.vHitPos, nx, nz, pVerts, pIndices))
		{
			_nVertCount = 0;
			_nIndexCount = 0;
		}
		else
		{
			pVerts += _nVertCount;
			pIndices += _nIndexCount;
		}
	}

	CECTriangleMan* pTriangleMan = g_pGame->GetGameRun()->GetWorld()->GetOrnamentMan()->GetTriangleMan();

	if (pTriangleMan)
	{
		TMan_Overlap_Info info;
		info.vStart = vCenter;
		info.vStart.y += 1.0f;
		info.vExtent = A3DVECTOR3(fRadus, 4.0f, fRadus);
		info.vDelta = A3DVECTOR3(0.0f, -4.0f, 0.0f);
		info.pCamera = g_pGame->GetViewport()->GetA3DCamera();
		pTriangleMan->TestOverlap(info);

		nTriVertCount = info.pVert.size();
		nTriIndexCount = info.pIndex.size();

		if (nTriVertCount && nTriIndexCount && nTriVertCount + _nVertCount <= nVertCount && nTriIndexCount + _nIndexCount <= nIndexCount)
		{
			int i;
			for (i = 0; i < nTriVertCount; i++)
				pVerts[i] = info.pVert[i];

			for (i = 0; i < nTriIndexCount; i++)
				pIndices[i] = _nVertCount + info.pIndex[i];
		}else
		{
			nTriVertCount = 0;
			nTriIndexCount = 0;
		}
	}

	nVertCount = nTriVertCount + _nVertCount;
	nIndexCount = nTriIndexCount + _nIndexCount;
	return true;
}
