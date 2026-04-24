#pragma once

#include <A3D.h>

class CAnimator;
class CAnmActionGroup;

const int ANM_FILE_MAGIC = 0xABCD;
const int ANM_FILE_VERSION = 5;
const int ANM_CONFIG_FILE_VERSION = 2;

struct AnmFileHead
{
    int		m_iMagic;
    int		m_iVersion;
    DWORD	m_dwTotalTime;
    int		m_iObjectAmount;
    int		m_iBezierAmount;
    int		m_iGroupAmount;
    int		m_iObjectOffset;
    int		m_iBezierOffset;
    int		m_iActionOffset;
    int		m_iWorldInstance;
    bool	m_bUseHostPlayer;
    bool	m_bShowHostPlayerWeapon;
    bool	m_bSpecifyTime;
    int		m_iHour;
    int		m_iMinute;
    int		m_iSecond;
    bool	m_bSpecifyWeather;
    bool	m_bRain;
    bool	m_bSnow;
};

struct AnmObjectIn
{
    DWORD	m_uiObjectID;
    DWORD	m_uiObjectType;
    wchar_t m_sModelName[256];
    wchar_t m_sDisplayName[256];
};

struct AnmObject
{
    DWORD	m_uiObjectID;
    DWORD	m_uiObjectType;
    float	m_fOffsetX;
    float	m_fOffsetY;
    float	m_fOffsetZ;
    wchar_t m_sModelName[256];
    wchar_t m_sDisplayName[256];
};

struct AnmBezier
{
    DWORD	m_uiBezierID;
    int		m_iPointAmount;
    wchar_t m_sDisplayName[256];
};

struct AnmBezierPoint
{
    float	m_fPosX;
    float	m_fPosY;
    float	m_fPosZ;
    float	m_fOrientationW;
    float	m_fOrientationX;
    float	m_fOrientationY;
    float	m_fOrientationZ;
    float	m_fFOV;
    int		m_iTotalFrames;
};

struct AnmBezierSegment
{
    float	m_fAnchorAX;
    float	m_fAnchorAY;
    float	m_fAnchorAZ;
    float	m_fAnchorBX;
    float	m_fAnchorBY;
    float	m_fAnchorBZ;
};

struct AnmAction
{
    DWORD	m_uiActionID;
    DWORD	m_uiObjectID;
    DWORD	m_uiActionType;
    DWORD	m_dwStartTime;
};

struct AnmMove
{
    float	m_fStartPosX;
    float	m_fStartPosY;
    float	m_fStartPosZ;
    float	m_fEndPosX;
    float	m_fEndPosY;
    float	m_fEndPosZ;
    float	m_fOrientationW;
    float	m_fOrientationX;
    float	m_fOrientationY;
    float	m_fOrientationZ;
    int		m_iRotateAxisType;
    float	m_fRotateAngle;
    float	m_fAccelerationPercent;
    int		m_iTotalFrames;
    bool	m_bCloseTerrain;
};

struct AnmRotate
{
    float	m_fStartRotateW;
    float	m_fStartRotateX;
    float	m_fStartRotateY;
    float	m_fStartRotateZ;
    float	m_fEndRotateW;
    float	m_fEndRotateX;
    float	m_fEndRotateY;
    float	m_fEndRotateZ;
    int		m_iTotalFrames;
};

struct AnmScale
{
    float	m_fStartScale;
    float	m_fEndScale;
    int		m_iTotalFrames;
};

struct AnmFOV
{
    float	m_fStartFOV;
    float	m_fEndFOV;
    int		m_iTotalFrames;
};

struct AnmJump
{
    float	m_fStartPosX;
    float	m_fStartPosY;
    float	m_fStartPosZ;
    float	m_fEndPosX;
    float	m_fEndPosY;
    float	m_fEndPosZ;
    int		m_iTotalFrames;
};

struct AnmMotionIn
{
    int		m_iStartMotionFrame;
    int		m_iBlendFrames;
    int		m_iTotalFrames;
    DWORD	m_uiMotionType;
    wchar_t m_sActionName[128];
    bool	m_bShowGFX;
};

struct AnmMotion
{
    int		m_iStartMotionFrame;
    int		m_iBlendFrames;
    int		m_iTotalFrames;
    DWORD	m_uiMotionType;
    wchar_t m_sActionName[128];
    float	m_fActionSpeed;
    bool	m_bShowGFX;
};

struct AnmDisplayHide
{
    DWORD	m_uiVisableType;
};

struct AnmDialogue
{
    int		m_iTotalFrames;
    wchar_t m_sDialogue[512];
};

struct AnmSound
{
    int		m_iTotalFrames;
    int		m_iStartSeconds;
    DWORD	m_uiVolume;
    wchar_t m_sSoundFileName[128];
};

struct AnmBezierMove
{
    DWORD	m_uiBezierID;
    bool	m_bCloseTerrain;
};

struct AnmHook
{
    int		m_iTotalFrames;
    wchar_t m_sHangerName[128];
    wchar_t m_sHookName[128];
    DWORD	m_uiHookObjectID;
    wchar_t m_sElseHookName[128];
};

struct AnmSpeedControl
{
    int		m_iTotalFrames;
    float	m_fSpeed;
};

struct AnmCameraBind
{
	DWORD	m_uiFollowObj;
	int     m_iTotalFrames;
	float	m_fOffsetX;
    float	m_fOffsetY;
    float	m_fOffsetZ;
};
struct AnmCameraFilter
{
    DWORD	m_uiColor;
    DWORD	m_uiFilterType;
    float	m_fTransparent;
    int		m_iTotalFrames;
};

struct AnmCameraOscillatory
{
    DWORD	m_uiOscillatoryType;
    int		m_iOscillatoryAxis;
    int		m_iAttenuation;
    float	m_fDisplacement;
    float	m_fFrequency;
    int		m_iTotalFrames;
};

struct AnmTransparent
{
    float	m_fStartTransparent;
    float	m_fEndTransparent;
    int		m_iTotalFrames;
};

struct AnmHideWorldAction
{
    DWORD	m_uiHideWorldType;
    DWORD	m_uiColor;
    int		m_iTotalFrames;
};

struct AnmLightning
{
	int		m_iTimes;
	int		m_iInterval;
	int		m_iRand;
	int		m_iOnceTime;
	int		m_iOnceInterval;
	float	m_fStartEnergy;
	float	m_fEndEnergy;
	float	m_fLum;
	int		m_iTotalFrames;
};

struct AnmColorMap
{
	char	m_szColorMap[256];
	int		m_iTotalFrames;
};

struct AnmFog
{
	float	m_fStart;
	float	m_fEnd;
	float	m_fFogDensity1;
	float	m_fFogDensity2;
	float	m_fColorDensity1;
	float	m_fColorDensity2;
	float	m_fHorzDensity;
	float	m_fHorzPower;
	float	m_fHeightStart;
	float	m_fHeightEnd;
	float	m_fHeightDensity;
	DWORD	m_dwFogColor1;
	DWORD	m_dwFogColor2;
	int		m_iTotalFrames;
};

struct AnmLock
{
	BYTE	m_nType;
	float	m_fOffsetX;
	float	m_fOffsetY;
	int		m_iTotalFrames;
};

struct AnmLock_v5_
{
	BYTE	m_nType;
	float	m_fOffsetX;
	float	m_fOffsetY;
	float	m_fScale;
	int		m_iTotalFrames;
};

struct AnmActionBlur
{
	DWORD	m_Colors[5];
	bool	m_bStopWhenActChange;
	int		m_iTotalFrames;
};

struct AnmPauseGameSound
{
	BYTE	m_nType;
	int		m_iTotalFrames;
};

struct AnmPlaySound
{
	BYTE	m_nType;
	char	m_szFilePath[256];
	bool	m_bLoop;
	int		m_iFadeInTime;
	int		m_iFadeOutTime;
	int		m_iVolume;
	int		m_iTotalFrames;
};

struct AnmActionChannel
{
    DWORD	m_uiChannelID;
    DWORD	m_uiObjectID;
    DWORD	m_uiChannelType;
    int		m_iActionAmount;
    wchar_t m_sChannelName[32];
};

struct AnmActionGroup
{
    DWORD	m_uiGroupID;
    int		m_iChannelAmount;
    wchar_t m_sGroupName[32];
};

struct AnmConfig_old
{
	int		m_iID;
	char	m_szPath[ MAX_PATH ];
	bool	m_bShowElsePlayer;
	bool	m_bShowNPC;
	bool	m_bStopAutoMove;
	float	m_fDestPosX;
	float	m_fDestPosZ;
	bool	m_bCanEscStop;
	bool	m_bMuteGameMusic;
};

struct AnmConfig
{
	int		m_iID;
	char	m_szPath[ MAX_PATH ];
	bool	m_bShowElsePlayer;
	bool	m_bShowNPC;
	bool	m_bShowDynModel;
	bool	m_bStopAutoMove;
	float	m_fDestPosX;
	float	m_fDestPosZ;
	bool	m_bCanEscStop;
	bool	m_bMuteGameMusic;
};

class CAnmTranslator
{
public:

	CAnmTranslator( CAnimator* pAnimator );
	~CAnmTranslator();

	bool SaveFile( const wchar_t* szFile );
	bool LoadFile( const wchar_t* szFile, const A3DVECTOR3& c_vCameraPos );

	bool ExportActionGroup( const wchar_t* szFile, CAnmActionGroup* pActionGroup );
	bool ImportActionGroup( const wchar_t* szFile );

	bool SaveConfigFile( const char* szFile );
	bool LoadConfigFile( const char* szFile );

private:

	CAnmTranslator();

	bool SaveActionGroupToFile( AFile& file, CAnmActionGroup* pActionGroup );
	bool LoadActionGroupFromFile( AFileImage& file, int iVersion = 0 );

private:

	CAnimator* m_pAnimator;
};