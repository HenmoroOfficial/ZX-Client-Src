#ifndef _ANIMATOR_H_
#define _ANIMATOR_H_

#include <A3D.h>
#include <map>

class CECWorld;
class CECGameRun;
class CECModel;
class CAnmCamera;
class CAnmPlayer;
class CAnmObjectManager;
class CAnmBezierManager;
class CAnmActionManager;
class CAnmActionGroup;
class CAnmTranslator;
class CAnmRayTrace;
class CAnmAxes;
class CAnmInstance;
class CAnmObject;
struct AnmConfig;
class SmartCamera;
class CAnmCommonActionManager;
#ifdef BACK_VERSION
class CECViewport;
#else
class ECViewport;
#define CECViewport ECViewport
#endif

typedef std::map< int, AnmConfig* >		AnimationMap;

#ifdef _PLATFORM_RENDER
class CAnimator : public ECRObjBase
#else
class CAnimator
#endif // _PLATFORM_RENDER
{
    public:
		enum AnimationType
		{
			TYPE_GAME_ANIMATION = 0, // current world
			TYPE_EDITOR_ANIMATION = 1, // create a new world
		};

        enum AnimatorState
        {
            STATE_WAITING = 0,
            STATE_PLAYING = 1,
            STATE_PAUSING = 2,
        };

        enum BlackWhiteType
        {
            BWT_NONE = 0,
            BWT_SLOW = 1,
            BWT_FAST = 2,
        };

		enum MoveKey
		{
			MOVEKEY_W = 0x01,
			MOVEKEY_S = 0x02,
			MOVEKEY_A = 0x04,
			MOVEKEY_D = 0x08,
			MOVEKEY_Q = 0x10,
			MOVEKEY_E = 0x20,
			MOVEKEY_Z = 0x40,
			MOVEKEY_X = 0x80,
		};

    public:
        CAnimator();

        AnimationType GetAnimationType() const;

        bool Init(CAnmInstance* pInst);

        void Release();

        bool Tick(DWORD dwDeltaTime);

        bool Render(A3DViewport *pViewport, bool bRenderShadow=true);
#ifdef _PLATFORM_RENDER
		virtual bool Render(const ECRENDERPARAM* pRenderParam);
#endif // _PLATFORM_RENDER

        void RenderMask(A3DViewport *pViewport);

        void RenderText(A3DViewport *pViewport);

        void RenderWire(A3DViewport *pViewport);

        bool IsReady() const;

        void MoveFront(const float d);

        void MoveBack(const float d);

        void MoveLeft(const float d);

        void MoveRight(const float d);

        void MoveUp(const float d);
        
        void MoveDown(const float d);

        void DegDelta(const float d);

        void PitchDelta(const float d);

        void RollDelta(const float d);

        void AddBezierPoint(const A3DVECTOR3 &v);

        void Play();

        void JumpTo(const DWORD dwNowTime);

        void Pause();

        void Stop();

        A3DEngine* GetA3DEngine() const;

        A3DDevice* GetA3DDevice() const;

        A3DCamera* GetA3DCamera() const;

        A3DViewport* GetA3DViewport() const;

        CECWorld* GetWorld() const;

		CECGameRun* GetGameRun() const;

        CAnmObjectManager* GetObjectManager() const;

        CAnmBezierManager* GetBezierManager() const;

        CAnmActionManager* GetActionManager() const;

        CAnmRayTrace* GetRayTrace() const;

        CAnmAxes* GetAxes() const;

		CAnmCommonActionManager* GetCommonActionManager() const;

        void SetUseHostPlayer(const bool b);

        bool IsUseHostPlayer() const;

        void SetShowHostPlayerWeapon(const bool b);

        bool IsShowHostPlayerWeapon() const;

        void SetSpecifyTime(const bool b);

        void SetWorldTime(const int hour, const int minute, const int second);

        bool IsSpecifyTime() const;

        int GetHour() const;

        int GetMinute() const;

        int GetSecond() const;

        void SetSpecifyWeather(const bool b);
        
        bool IsSpecifyWeahter() const;

        void SetWeatherRain(const bool b);

        bool IsRain() const;

        void SetWeatherSnow(const bool b);

        bool IsSnow() const;

		void ShowLetterbox(const bool b);

		bool IsShowLetterbox() const;

        void SetVolume(DWORD dwVolume);

        DWORD GetVolume() const;

        void SetDialogue(const ACString &st);

        ACString GetDialogue() const;

        void SetMaskColor(const unsigned int color);

        unsigned int GetMaskColor() const;

		void SetBlendFrame(bool flag);

		bool IsBlendFrame() const;

		void GeneratePreFrame();

        void SetMonoColor(const unsigned int color);
        
        unsigned int GetMonoColor() const;

        void SetBackgroundColor(const unsigned int color);
        
        unsigned int GetBackgroundColor() const;

        void SetBlackWhiteType(const BlackWhiteType &bt);
        
        BlackWhiteType GetBlackWhiteType() const;

        void SetHideWorldType(const unsigned int t);
        
        unsigned int GetHideWorldType() const;

        void SetMotionBlurLevel(const float fLevel);

        float GetMotionBlurLevel() const;

        bool IsMotionBlur() const;

        void SetTotalTime(const DWORD dwTotalTime);

        DWORD GetTotalTime() const;

        void SetNowTime(const DWORD dwNowTime);

        DWORD GetNowTime() const;

        void SetStartTime(const DWORD dwStartTime);

        DWORD GetStartTime() const;

        void SetEndTime(const DWORD dwEndTime);

        DWORD GetEndTime() const;

        void SetSpeed(const float speed);

        float GetSpeed() const;

        AnimatorState GetState() const;

        float GetPosHeight(const A3DVECTOR3 &vPos) const;

        void MoveCamera(const char key, const bool bSlow=false);

        void StopCamera();

        void DisableCamera(const bool b);

        bool IsDisabledCamera() const;

        void BindCamera(const bool b);
        
        bool IsBindCamera() const;

        void SetBindCameraOffset(const A3DVECTOR3 &v);

        A3DVECTOR3 GetBindCameraOffset() const;

        bool LoadWorld(int idInst, A3DVECTOR3 vHostPos);

		void UnloadWorld();

        int GetWorldInstance() const;

        void SetHostPlayerModel(CECModel *pModel);

        bool SaveFile(const wchar_t *szFile);

        bool ImportActionGroup(const wchar_t *szFile);

        bool ExportActionGroup(const wchar_t *szFile, CAnmActionGroup *pActionGroup);

        ~CAnimator();

		AnimationMap& GetAnimationMap() { return m_AnmMap; }
		AnmConfig* GetAnimation( int iAnimationID );
		AnmConfig* GetCurrentAnimation() { return GetAnimation( m_iCurrentAnimationID ); }
		
		int GetCurrentAnimationID() const { return m_iCurrentAnimationID; }
		void SetCurrentAnimationID( int iAnimationID ) { m_iCurrentAnimationID = iAnimationID; }
		
		bool LoadFileFromEditor( const wchar_t* szFile, const A3DVECTOR3& c_vCameraPos );
		bool LoadFileFromEditor( int iAnimationID,		const A3DVECTOR3& c_vCameraPos );
		bool LoadFileFromGame( const wchar_t* szFile,	const A3DVECTOR3& c_vCameraPos );
		bool LoadFileFromGame( int iAnimationID,		const A3DVECTOR3& c_vCameraPos );

		//
		bool LoadObject( CAnmObject* pObject );
		void UnloadObject( CAnmObject* pObject );
		
		bool TickObject( CAnmObject* pObject, DWORD dwDeltaTime );
#ifdef BACK_VERSION
		bool RenderObject( CAnmObject* pObject, bool bRenderShadow );
#else
		bool RenderObject( CAnmObject* pObject, A3DViewport* pA3DViewport, bool bRenderShadow );
#endif

		bool SaveConfigFile( const char* szFile );
		bool LoadConfigFile( const char* szFile );
		
		SmartCamera* GetSmartCamera() const { return m_pSmartCamera; }

    private:
        void AdjustCameraPos();
		bool TickSmartCamera(DWORD dwDeltaTime);

		void InitLetterboxImage();

    private:
		CAnmInstance*		m_pAnmInst;
        A3DFont *m_pFont;
        CAnmCamera *m_pCamera;
        CAnmPlayer *m_pPlayer;
        CAnmObjectManager *m_pObjectManager;
        CAnmBezierManager* m_pBezierManager;
        CAnmActionManager *m_pActionManager;
        CAnmTranslator *m_pTranslator;
        CAnmRayTrace *m_pRayTrace;
        CAnmAxes *m_pAxes;
		SmartCamera*	m_pSmartCamera;
		bool        m_bPreparePreFrame;
		A2DSprite*  m_pImagePreFrame;
		A2DSprite*	m_pImageLetterboxTop;
		A2DSprite*	m_pImageLetterboxBottom;
		CAnmCommonActionManager* m_pCommonActionManager;


        AnimationType m_eAnimationType;
        int m_iWorldInstance;
        bool m_bUseHostPlayer;
        bool m_bShowHostPlayerWeapon;
        bool m_bSpecifyTime;
        int m_iHour;
        int m_iMinute;
        int m_iSecond;
        bool m_bSpecifyWeather;
        bool m_bRain;
        bool m_bSnow;
		bool m_bShowLetterbox;
        DWORD m_dwVolume;

        ACString m_sDialogue;
		bool         m_bBlendFrame;
        unsigned int m_uiMaskColor;
        unsigned int m_uiMonoColor;
        unsigned int m_uiBackgroundColor;
        BlackWhiteType m_eBlackWhiteType;
        unsigned int m_uiHideWorldType;
        float m_fMotionBlurLevel;
        DWORD m_dwTotalTime;
        DWORD m_dwNowTime;
        DWORD m_dwStartTime;
        DWORD m_dwEndTime;
        float m_fTickTime;
        float m_fSpeed;
        bool m_bAnimatorReady;
        bool m_bDisableCamera;
        bool m_bBindCamera;
        A3DVECTOR3 m_vBindCameraOffset;
        AnimatorState m_eState;
        bool m_bMoveCamera;
        float m_fMoveCameraSpeed;
        int m_iMoveLastTime;

        A3DVECTOR3 m_aFrustum[8];

		AnimationMap		m_AnmMap;
		int					m_iCurrentAnimationID;
};

extern CAnimator g_Animator;

#endif
