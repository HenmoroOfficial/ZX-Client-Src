#pragma once

#include <A3D.h>
#include <AM.h>
#include <A3DGFXExMan.h>
#include <A3DFTFont.h>

class CECInstance;
class CECViewport;
class CECShadowRender;
class CECFullGlowRender;
class CELBackMusic;
class CECModelMan;
class CECOrnamentMan;
class CTerrain;
class CECWorld;
class CELTerrainOutline2;
class CECGFXCaster;
class CELGrassLand;

A3DEngine* AfxGetA3DEngine();
A3DDevice* AfxGetA3DDevice();
CECViewport* AfxGetViewport();
A3DCamera* AfxGetA3DCamera();
void AfxSetCamOffset( const A3DVECTOR3& vOffset );
A3DSkinRender* AfxGetSkinRender1();
A3DSkinRender* AfxGetSkinRender2();
A3DGFXExMan* AfxGetGFXExMan();
const CECInstance* AfxGetInstance( int iInstID );
CECShadowRender* AfxGetShadowRender();
CECFullGlowRender* AfxGetFullGlowRender();
CELBackMusic* AfxGetBackMusic();
CECModelMan* AfxGetECModelMan();
A3DLight* AfxGetDirLight();
A3DVECTOR3 AfxGetHostPos();
A3DVECTOR3	AfxGetHostDir();
CECOrnamentMan*	AfxGetOrnamentMan();
float AfxGetTerrainHeight(const A3DVECTOR3& vPos);
A3DTerrainWater * AfxGetTerrainWater();
CELGrassLand* AfxGetGrassLand();
A3DFTFontMan* AfxGetFTFontMan();
CECWorld* AfxGetWorld();
CELTerrainOutline2* AfxGetWorldTerrainOutline();
void AfxRenderWorld( CECViewport* pViewport );
void AfxRenderHightLightModelList( A3DViewport* pViewport );
CECGFXCaster* AfxGetGFXCaster();
A3DVertexShader* AfxGetSMEdge();
A3DVertexShader* AfxGetRMEdge();
A3DPixelShader*	AfxGetEdgePS();
A3DPixelShader*	AfxGetGlowPS();;
A3DShader* AfxGetEdgeGlowPS();
A3DPixelShader*	AfxGetEdgeObPS();
A3DVertexShader* AfxGetObstructSkinModelVS();
A3DVertexShader* AfxGetObstructRigidModelVS();
const GfxPSConstVec& AfxGetEdgePSConst();
const A3DLIGHTPARAM& AfxGetLightparam();
float AfxGetGrndNorm( const A3DVECTOR3& vPos, A3DVECTOR3* pNorm );
A3DSkinModel* AfxLoadA3DSkinModel( const char* szFile, int iSkinFlag );
void AfxReleaseA3DSkinModel( A3DSkinModel* pModel );
A3DSkinModel* AfxLoadA3DSkinModel( const char* szFile, int iSkinFlag );
bool gGfxGetSurfaceData( const A3DVECTOR3& vCenter, float fRadus, A3DVECTOR3* pVerts, int& nVertCount, WORD* pIndices, int& nIndexCount );
const char* AfxGetECMHullPath();
AM3DSoundBuffer* AfxLoadNonLoopSound( const char* szFile, int nPriority );
void AfxBeginShakeCam();
void AfxEndShakeCam();
void AfxECModelAddShadower( const A3DVECTOR3& vecCenter, const A3DAABB& aabb, A3DSkinModel* pA3DSkinModel );
bool AfxPlaySkillGfx( const AString& strAtkFile, unsigned char SerialID, clientid_t nCasterID, clientid_t nCastTargetID, const A3DVECTOR3* pFixedPoint, int nDivisions, TargetDataVec& Targets );
void AfxSkillGfxShowDamage( clientid_t idCaster, clientid_t idTarget, int nDamage, int nDivisions, DWORD dwModifier );
void AfxSkillGfxShowCaster( clientid_t idCaster, DWORD dwModifier );
bool AfxSkillGfxAddDamageData( clientid_t nCaster, clientid_t nTarget, unsigned char SerialId, DWORD dwModifier, int nDamage );
void AfxRenderDofObj( CECViewport* pViewport,const A3DVECTOR3& vEyePos, float fCurFocus, float fClearFactor, A3DPixelShader* pDofDepthPixShader );
void glb_EnableShaderShadow( CECModel* pModel, bool bStaticShadow, bool bDynShadow );

void EnablePDBMemDebug( const char* szPDBFilePath );