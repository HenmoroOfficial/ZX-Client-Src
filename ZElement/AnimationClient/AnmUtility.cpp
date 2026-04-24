#include "AnmUtility.h"
#include "AnmSystem.h"
#include "EC_World.h"
#include "EC_Global.h"
#include <crtdbg.h>


A3DEngine* AfxGetA3DEngine() { return g_AnmSystem.m_pA3DEngine; }
A3DDevice* AfxGetA3DDevice() { return g_AnmSystem.m_pA3DDevice; }
CECViewport* AfxGetViewport() { return g_AnmSystem.m_pECViewport; }
A3DCamera* AfxGetA3DCamera() { return (A3DCamera*)g_AnmSystem.m_pA3DEngine->GetActiveCamera(); }
void AfxSetCamOffset( const A3DVECTOR3& vOffset )
{
	A3DCamera* pCamera = (A3DCamera*)g_AnmSystem.m_pA3DEngine->GetActiveCamera();
	pCamera->SetPos( pCamera->GetPos() + vOffset );
}
A3DSkinRender* AfxGetSkinRender1() { return g_AnmSystem.m_pSkinRender1; }
A3DSkinRender* AfxGetSkinRender2() { return g_AnmSystem.m_pSkinRender2; }
A3DGFXExMan* AfxGetGFXExMan() { return g_AnmSystem.m_pGfxExMan; }
const CECInstance* AfxGetInstance( int iInstID ) { return g_AnmSystem.GetInstance( iInstID ); }
CECShadowRender* AfxGetShadowRender() { return g_AnmSystem.m_pShadowRender; }
CECFullGlowRender* AfxGetFullGlowRender() { return g_AnmSystem.m_pFullGlowRender; }
CELBackMusic* AfxGetBackMusic() { return g_AnmSystem.m_pBackMusic; }
CECModelMan* AfxGetECModelMan() { return g_AnmSystem.m_pModelMan; }
A3DLight* AfxGetDirLight() { return g_AnmSystem.m_pDirLight; }
A3DVECTOR3 AfxGetHostPos() { return g_AnmSystem.m_pCamera->GetPos(); }
A3DVECTOR3 AfxGetHostDir() { return g_AnmSystem.m_pCamera->GetDir(); }
CECOrnamentMan*	AfxGetOrnamentMan() { return g_AnmSystem.m_pWorld->GetOrnamentMan(); }
//float AfxGetTerrainHeight(const A3DVECTOR3& vPos) { return g_AnmSystem.m_pWorld->GetTerrainHeight( vPos ); }
A3DTerrainWater * AfxGetTerrainWater() { return g_AnmSystem.m_pWorld->GetTerrainWater(); }
CELGrassLand* AfxGetGrassLand() { return g_AnmSystem.m_pWorld->GetGrassLand(); }
A3DFTFontMan* AfxGetFTFontMan() { return g_AnmSystem.m_pFTFontMan; }
CECWorld* AfxGetWorld() { return g_AnmSystem.m_pWorld; }
//CELTerrainOutline2* AfxGetWorldTerrainOutline() { return g_AnmSystem.m_pWorld->GetTerrainOutline(); }
//void AfxRenderWorld( CECViewport* pViewport ) { g_AnmSystem.m_pWorld->Render( pViewport ); }
void AfxRenderHightLightModelList( A3DViewport* pViewport ) { g_AnmSystem.RenderHightLightModelList( pViewport ); }
CECGFXCaster* AfxGetGFXCaster() { return g_AnmSystem.m_pGFXCaster; }
A3DVertexShader* AfxGetSMEdge() { return g_AnmSystem.m_pvsSMEdge; }
A3DVertexShader* AfxGetRMEdge() { return g_AnmSystem.m_pvsRMEdge; }
A3DPixelShader*	AfxGetEdgePS() { return g_AnmSystem.m_pEdgePS; }
A3DPixelShader*	AfxGetGlowPS() { return g_AnmSystem.m_pGlowPS; }
A3DShader* AfxGetEdgeGlowPS() { return g_AnmSystem.m_pEdgeGlowShader; }
A3DPixelShader*	AfxGetEdgeObPS() { return g_AnmSystem.m_pEdgeObPS; }
A3DVertexShader* AfxGetObstructSkinModelVS() { return g_AnmSystem.m_pvsObstructSkinModel; }
A3DVertexShader* AfxGetObstructRigidModelVS() { return g_AnmSystem.m_pvsObstructRigidModel; }
const GfxPSConstVec& AfxGetEdgePSConst() { return g_AnmSystem.m_EdgePsConsts; }
const A3DLIGHTPARAM& AfxGetLightparam()
{
	static A3DLIGHTPARAM p;
	ZeroMemory( & p, sizeof(p) );

	static const A3DCOLORVALUE diffuse( 0.8f, 0.8f, 0.9f, 1.0f );
	static const A3DCOLORVALUE spec( 1.0f, 1.0f, 1.0f, 1.0f );
	static const A3DCOLORVALUE ambient( 0.0f, 0.0f, 0.0f, 1.0f );

	p.Type		= A3DLIGHT_DIRECTIONAL;
	p.Diffuse	= diffuse;
	p.Specular	= spec;
	p.Ambient	= ambient;
	p.Position	= A3DVECTOR3( 0.0f, 0.0f, 0.0f );
	p.Direction	= Normalize( A3DVECTOR3 ( 0.0f, -0.7f, 1.0f ) );
	return p;
}
float AfxGetGrndNorm( const A3DVECTOR3& vPos, A3DVECTOR3* pNorm )
{
// 	CECWorld* pWorld = g_AnmSystem.m_pWorld;
// 	if( pWorld->IsWorldLoaded() )
// 		return 0.0f;
// 	float fHeight = pWorld->GetTerrainHeight( vPos, pNorm );
// 	float fWater  = pWorld->GetWaterHeight( vPos );
// 
// 	if( fWater > fHeight )
// 	{
// 		fHeight = fWater;
// 		if( pNorm )
// 			*pNorm = g_vAxisY;
// 	}
// 	return fHeight;
	return 0;
}
A3DSkinModel* AfxLoadA3DSkinModel( const char* szFile, int iSkinFlag )
{
	A3DSkinModel* pSkinModel = new A3DSkinModel;
	if( ! pSkinModel->Init( g_AnmSystem.m_pA3DEngine ) || ! pSkinModel->Load( szFile, iSkinFlag ) )
	{
		delete pSkinModel;
		pSkinModel = NULL;
	}
	return pSkinModel;
}
void AfxReleaseA3DSkinModel( A3DSkinModel* pModel ) { A3DRELEASE( pModel ); }
void AfxSetA3DSkinModelSceneLightInfo( A3DSkinModel* pA3DSkinModel )
{
	A3DSkinModel::LIGHTINFO LightInfo = pA3DSkinModel->GetLightInfo();
	GfxLightParamList& ls = g_AnmSystem.m_pGfxExMan->GetLightParamList();
	if( ls.size() )
	{
		A3DLIGHTPARAM* param = &ls[0]->GetLightParam();
		LightInfo.bPtLight		= true;
		LightInfo.colPtDiff		= param->Diffuse * param->Diffuse.a * 2.0f;
		LightInfo.colPtAmb		= param->Ambient * param->Ambient.a * 2.0f;
		LightInfo.fPtRange		= param->Range;
		LightInfo.vPtAtten		= A3DVECTOR3( param->Attenuation0, param->Attenuation1, param->Attenuation2 );
		LightInfo.vPtLightPos	= param->Position;
	}
	pA3DSkinModel->SetLightInfo( LightInfo );
}
bool gGfxGetSurfaceData( const A3DVECTOR3& vCenter, float fRadus, A3DVECTOR3* pVerts, int& nVertCount, WORD* pIndices, int& nIndexCount )
{
	pVerts[0]	= A3DVECTOR3( vCenter.x-fRadus, vCenter.y, vCenter.z+fRadus );
	pVerts[1]	= A3DVECTOR3( vCenter.x+fRadus, vCenter.y, vCenter.z+fRadus );
	pVerts[2]	= A3DVECTOR3( vCenter.x-fRadus, vCenter.y, vCenter.z-fRadus );
	pVerts[3]	= A3DVECTOR3( vCenter.x+fRadus, vCenter.y, vCenter.z-fRadus );
	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;
	pIndices[3] = 1;
	pIndices[4] = 3;
	pIndices[5] = 2;
	nVertCount	= 4;
	nIndexCount = 6;
	return true;
}
const char* AfxGetECMHullPath() { return "Models\\ECModelHull"; }
AM3DSoundBuffer* AfxLoadNonLoopSound( const char* szFile, int nPriority ) { return g_AnmSystem.m_pA3DEngine->GetAMSoundEngine()->GetAMSoundBufferMan()->Load3DSound( szFile, true ); }
AM3DSoundBuffer* AfxLoadLoopSound(const char* szFile) { return AfxLoadNonLoopSound( szFile, 0 ); }
void AfxReleaseSoundNonLoop( AM3DSoundBuffer*& pSound ) { g_AnmSystem.m_pA3DEngine->GetAMSoundEngine()->GetAMSoundBufferMan()->Release3DSound( & pSound ); }
void AfxReleaseSoundLoop( AM3DSoundBuffer*& pSound ) { AfxReleaseSoundNonLoop( pSound ); }
void AfxBeginShakeCam()
{
}
void AfxEndShakeCam()
{
}
void AfxECModelAddShadower( const A3DVECTOR3& vecCenter, const A3DAABB& aabb, A3DSkinModel* pA3DSkinModel )
{
}
bool AfxPlaySkillGfx( const AString& strAtkFile, unsigned char SerialID, clientid_t nCasterID, clientid_t nCastTargetID, const A3DVECTOR3* pFixedPoint, int nDivisions, TargetDataVec& Targets )
{
	return true;
}
void AfxSkillGfxShowDamage( clientid_t idCaster, clientid_t idTarget, int nDamage, int nDivisions, DWORD dwModifier )
{
}
void AfxSkillGfxShowCaster( clientid_t idCaster, DWORD dwModifier )
{
}
bool AfxSkillGfxAddDamageData( clientid_t nCaster, clientid_t nTarget, unsigned char SerialId, DWORD dwModifier, int nDamage )
{
	return true;
}
void AfxRenderDofObj( CECViewport* pViewport,const A3DVECTOR3& vEyePos, float fCurFocus, float fClearFactor, A3DPixelShader* pDofDepthPixShader )
{

}
void glb_EnableShaderShadow( CECModel* pModel, bool bStaticShadow, bool bDynShadow )
{
}


#ifdef _DEBUG
#pragma comment(lib, "ImageHlp.lib")
#include <ImageHlp.h>
#include <sstream>

struct SymbolInfo
{
	const void*	addr;
	std::string fun_name;
	std::string file_name;
	DWORD       line_num;
};
static LPFNDEFLOGOUTPUT l_pfnOldLog;
static char l_szPDBFilePath[1024];
static bool _IsHexNum( char c ) { return isdigit(c) || c >= 'a' && c <= 'f' || c >= 'A' && c <= 'F'; }

static std::string _GetAddrStr( const std::string& strMsg, size_t nPos )
{
	if( nPos + 2 >= strMsg.size() || strMsg[ nPos ] != '0' || strMsg[ nPos + 1 ] != 'x' || ! _IsHexNum( strMsg[ nPos + 2 ] ) ) 
		return "";

	size_t nCnt = 0;
	while( nPos + nCnt + 2 < strMsg.size() && _IsHexNum( strMsg[ nPos + nCnt + 2 ] ) )
		++ nCnt;

	return std::string( & strMsg[ nPos ], & strMsg[ nPos + 2 + nCnt ] );
}

static HANDLE _InitSymForCurProc()
{
	static HANDLE s_hProc;
	static char s_szLastPath[1024];

	if( ! s_hProc || strcmp( s_szLastPath, l_szPDBFilePath ) )
	{
		strcpy( s_szLastPath, l_szPDBFilePath );
		SymSetOptions( SYMOPT_LOAD_LINES );
		s_hProc = GetCurrentProcess();
		BOOL bRet = SymInitialize( s_hProc, l_szPDBFilePath, true );
		ASSERT( bRet );
	}
	return s_hProc;
}

static SymbolInfo _AddrToSym( void* pAddr, HANDLE hProc )
{
	BOOL bRet;
	SymbolInfo si;
	si.addr = pAddr;

	DWORD dwAddr = reinterpret_cast< DWORD >( pAddr );
	char aBuff[ sizeof( SYMBOL_INFO ) + MAX_SYM_NAME * sizeof( TCHAR ) + sizeof( ULONG64 ) - 1 ];
	PSYMBOL_INFO pSymInfo = reinterpret_cast< PSYMBOL_INFO >( aBuff );
	pSymInfo->SizeOfStruct = sizeof( SYMBOL_INFO );
	pSymInfo->MaxNameLen = MAX_SYM_NAME;

	DWORD64 dw64Offset;
	bRet = SymFromAddr( hProc, dwAddr, & dw64Offset, pSymInfo );
	if( bRet )
		si.fun_name = pSymInfo->Name;

	IMAGEHLP_LINE line;
	line.SizeOfStruct = sizeof( IMAGEHLP_LINE );
	DWORD dwOffset = static_cast< DWORD >( dw64Offset );
	bRet = SymGetLineFromAddr( hProc, dwAddr, & dwOffset, & line );
	if( bRet )
	{
		si.file_name = line.FileName;
		si.line_num  = line.LineNumber;
	}
	return si;
}

std::string _SymToLine( const SymbolInfo& si )
{
	std::ostringstream os;
	if( ! si.file_name.empty() )
		os << si.file_name << "(" << si.line_num << "): ";
	if( ! si.fun_name.empty() )
	{
		os << "In function: " << si.fun_name; 
		os.clear();
		os << "() ";
	}
	if( si.addr )
		os << "0x" << std::hex << si.addr;
	return os.str();
}

std::string _AddrToMemLeakInfo( const char* szMsg )
{
	std::ostringstream oss;
	HANDLE hProc = _InitSymForCurProc();

	std::istringstream iss( szMsg );
	std::string strLine;
	while( std::getline( iss, strLine ) )
	{
		oss << strLine << "\n";
		size_t i = 0;
		while( i < strLine.size() )
		{
			std::string strAddr = _GetAddrStr( strLine, i );
			if( strAddr.empty() )
			{ 
				++ i;
				continue;
			}
			size_t nPos;
			sscanf( strAddr.c_str(), "%x", & nPos );
			SymbolInfo si = _AddrToSym( reinterpret_cast< void* >( nPos ), hProc );
			if( si.addr )
				oss << "\t" << _SymToLine( si ) << "\n";
			i += strAddr.size();
		}
	}
	return oss.str();
}

static void _IDEDebugReport( const char* szReport )
{
	std::istringstream iss( szReport );
	std::string strLine;
	while( std::getline( iss, strLine ) )
		_CrtDbgReport( _CRT_WARN, 0, 0, 0, ( strLine + "\n" ).c_str() );
}

static void _ShowMemLeakInfo( const char* szMsg )
{
	if( ! szMsg || ! *szMsg )
		return;
	std::string strInfo = _AddrToMemLeakInfo( szMsg );
	if( ! strInfo.empty() )
		_IDEDebugReport( strInfo.c_str() );
}

static void _DebugMemLeak( const char* szMsg )
{
	if( l_pfnOldLog ) 
		l_pfnOldLog( szMsg );
	_ShowMemLeakInfo( szMsg );
}

static void _OnExitProcMemDebug()
{
	l_pfnOldLog = a_RedirectDefLogOutput( _DebugMemLeak );
}

void EnablePDBMemDebug( const char* szPDBFilePath )
{
	if( szPDBFilePath && *szPDBFilePath )
		strcpy( l_szPDBFilePath, szPDBFilePath );
	atexit( _OnExitProcMemDebug );	// register
}
#endif

