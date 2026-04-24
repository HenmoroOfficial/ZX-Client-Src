#pragma once

#include "AnmInstance.h"
#include "AnmNPC.h"
#include "AnmPlayer.h"
#include "AnmGFX.h"
#include "EC_Configs.h"
#include <GfxCommonHeaders.h>
#include <A3D.h>
//#include <A3DFTFontMan.h>
#include <vector>
#include <map>
#ifndef BACK_VERSION
#include "ECManager.h"
#include "ECManagerID.h"
#endif

class CAnmObjLoader;
class CAnmObject;
class CECShadowRender;
class CECFullGlowRender;
class CELBackMusic;
class CECGFXCaster;
class CECInstance;
#ifdef BACK_VERSION
class CECViewport;
#else
class ECViewport;
#endif
class CECWorld;
class CECModelMan;

enum
{
	//枚举值与animation_files.txt中的ID值一一对应
	//前五个是老动画，保留
    ANIMATION_1 = 1,
	ANIMATION_2,
	ANIMATION_3,
	ANIMATION_4,
	ANIMATION_5,
	ANIMATION_TERRAN,  //人族开篇
	ANIMATION_PROTOSS,  //神族开篇
	ANIMATION_ELDAR,  //灵族开篇
	ANIMATION_QINYUNBIWU,  //青云比武
	ANIMATION_BIYAOZHISI, //碧瑶之死
	ANIMATION_HEYANGHUIMIE, //河阳毁灭
};

class CAnmSystem : public CAnmInstance
{
public:

	struct HLModel
	{
		HLModel() : pModel(0), clr(0xffffffff), fEdgeLen(-1) {}
		HLModel( CECModel* _pModel, A3DCOLOR _clr, float _fEdgeLen = -1 ) : pModel(_pModel) , clr(_clr), fEdgeLen(_fEdgeLen) {}

		CECModel* pModel;
		A3DCOLOR clr;
		float fEdgeLen;
	};

	struct SAnimatorConf
	{
		SAnimatorConf() : iId(-1),nEffectLevel(4){}
	    int       iId;
        AWString  strName;
		BYTE      nEffectLevel;
	};
	typedef std::vector< HLModel > HLModelVector;

	typedef std::map< int, CECInstance* > InstanceMap;

	typedef std::vector<SAnimatorConf>  AnimatorConfVector;

public:

	CAnmSystem();

	bool Init( HINSTANCE hInst, HWND hWnd );
	void Release();

	void Tick( DWORD dwDeltaTime );
#ifdef BACK_VERSION
	void Render(bool bPresent=true);
#else
	void Render(ECViewport* pViewport);
	void RenderPost(ECViewport* pViewport);
	virtual ECViewport* GetECViewport();
#endif

	virtual A3DEngine* GetA3DEngine();
	
	virtual A3DCamera* GetA3DCamera();
	
	virtual A3DViewport* GetA3DViewport();
	
	virtual CECWorld* GetWorld() { return m_pWorld; }

	virtual CECGameRun* GetGameRun();

	virtual bool LoadWorld( int iInstID, const A3DVECTOR3& c_vCameraPos );
	virtual void UnloadWorld();

	virtual bool LoadObject( CAnmObject* pObject );
	virtual void UnloadObject( CAnmObject* pObject );

	virtual bool TickObject( CAnmObject* pObject, DWORD dwDeltaTime );
#ifdef BACK_VERSION
	virtual bool RenderObject( CAnmObject* pObject, bool bRenderShadow );
#else
	virtual bool RenderObject( CAnmObject* pObject, A3DViewport* pA3DViewport, bool bRenderShadow );
#endif

	virtual void UpdateText(const ACString* pText);
	virtual bool RenderLetterboxAndText();

	CECInstance* GetInstance( int iInstID );

	void NotifyWndSizeChanged( int iWndWidth,int iWndHeight );

	// 设置主角职业
	void SetHostPlayerSect(unsigned int sect);

	// 设置主角性别
	void SetHostPlayerMale(bool bMale);

	// 加几个动画加载、播放控制的方法
	bool Play(int id);
	bool Play(const wchar_t* szFile);
	void Stop();
	void Pause();
	bool IsPlaying();

	SAnimatorConf*  GetCurAnmConf(){return m_curAnmConf;}

	A3DVECTOR3 GetCenterPos();

protected:

	void RenderScreenInfo();

	void RestoreScene();

	void OnBeforePlayAnm();
	void OnAfterPlayAnm();

public:

	HINSTANCE			m_hInstance;
	HWND				m_hWnd;
	DWORD				m_dwTime;
	DWORD				m_dwFrames;
	DWORD				m_dwDeltaTime;
	float				m_fPlayedTime;
	CAnmObjLoader*		m_pObjLoader;

protected:
	A3DEngine*				m_pEngine;
	A3DDevice*				m_pDevice;
#ifdef BACK_VERSION
	CECViewport*			m_pECViewport;
#else
	ECViewport*			m_pECViewport;
#endif
	A3DGFXExMan*			m_pGfxExMan;
	CECWorld*				m_pWorld;
	A3DLight*				m_pDirLight;
	A3DCamera*				m_pCamera;
	A3DViewport*			m_pViewport;
	CECGFXCaster*			m_pGfxCaster;
#ifdef BACK_VERSION
	A3DSkinRender*			m_pSkinRender1;
	A3DSkinRender*			m_pSkinRender2;
#else
	A3DSkinRenderBase*		m_pSkinRender1;
	A3DSkinRenderBase*		m_pSkinRender2;
#endif

	AnimatorConfVector      m_vecAnmConfs;

	SAnimatorConf*          m_curAnmConf;

	EC_VIDEO_SETTING        m_vsOrigin;
	EC_SYSTEM_SETTING       m_sysOrigin;
	int						m_iOriginInstanceId;
	bool					m_bOriginWeaponHide;
	A3DVECTOR3				m_vecOriginPos;
	A3DVECTOR3				m_vecOriginDir;
	A3DVECTOR3				m_vecOriginUp;
	bool					m_bUseUILetterbox;		// 如不想使用ui来表现字幕以及上下黑边, 则将该属性赋值为false
	bool					m_bLastShowUI;			// 存ui man的原始状态
	bool					m_bLastEnableTextAlpha; // 是否允许对话文本淡入淡出

protected:

	CECInstance* GetInstance( const char* szFile );

	void TickWorld( DWORD );
	void RenderWorld();

	bool TickNPC( CAnmNPC* pNPC, DWORD dwDeltaTime );
	bool TickPlayer( CAnmPlayer* pPlayer, DWORD dwDeltaTime );
	bool TickGFX( CAnmGFX* pGFX, DWORD dwDeltaTime );

#ifdef BACK_VERSION
	bool RenderNPC( CAnmNPC* pNPC, bool bRenderShadow );
	bool RenderPlayer( CAnmPlayer* pPlayer, bool bRenderShadow );
	bool RenderGFX( CAnmGFX* pGFX, bool bRenderShadow );
#else
	bool RenderNPC( CAnmNPC* pNPC, A3DViewport* pA3DViewport, bool bRenderShadow );
	bool RenderPlayer( CAnmPlayer* pPlayer, A3DViewport* pA3DViewport, bool bRenderShadow );
	bool RenderGFX( CAnmGFX* pGFX, A3DViewport* pA3DViewport, bool bRenderShadow );
#endif

	void ShowLetterbox();
	void HideLetterbox();

protected:
	void LoadAnimatorConf(const char* szFile);
	SAnimatorConf* GetAnimatorConfById(int id);
	SAnimatorConf* GetAnimatorConfByName(const wchar_t* szFile);

};

CAnmSystem& GetAnmSystem();
#define g_AnmSystem (GetAnmSystem())
#ifndef BACK_VERSION

class CManagerWrapperForAnmSystem : public ECManager
{
public:
	CManagerWrapperForAnmSystem(CAnmSystem* pAnmSystem): ECManager(NULL), m_pAnmSystem(pAnmSystem) {}

	//	Render routine, for registering objects
	virtual bool Render(ECViewport* pViewport) { m_pAnmSystem->Render(pViewport); return true; }
	//  Render objects after scene rendered
	virtual bool RenderPost(ECViewport* pViewport) { m_pAnmSystem->RenderPost(pViewport); return true; }
	//	RenderForReflected routine
	virtual bool RenderForReflect(ECViewport* pViewport) {  m_pAnmSystem->Render(pViewport); return true; }
	//	RenderForRefract routine
	virtual bool RenderForRefract(ECViewport* pViewport) {  m_pAnmSystem->Render(pViewport); return true; }
	//	RenderForShadow routine
	virtual bool RenderForShadow(ECViewport* pViewport) {  m_pAnmSystem->Render(pViewport); return true; }
	//	RenderForBloom routine
	virtual bool RenderForBloom(ECViewport * pViewport) { return true; }
private:
	CAnmSystem* m_pAnmSystem;
};
#endif