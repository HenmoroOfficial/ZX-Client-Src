// SceneBoxArea.h: interface for the CSceneBoxArea class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCENEBOXAREA_H__73631896_32C9_416D_96F8_C6A0F53196C4__INCLUDED_)
#define AFX_SCENEBOXAREA_H__73631896_32C9_416D_96F8_C6A0F53196C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "A3DSky.h"
#include "SceneObject.h"

#ifdef _ANGELICA22
#include "A3DEnvironment.h"
#endif // #ifdef _ANGELICA22

class AMSoundStream;
class A3DRain;
class A3DSnow;

//存在时间段的优先于时间段全部为任意的
struct TIME_CTR 
{
	int nYear;//正常区间 [1900-2100] -1 代表任意
	int nMouth;//正常取值区间[0,11]  -1 代表任意
	int nWeek;//正常取值区间[0,6]  0代表星期日 -1 代表任意
	int nDay; //正常取值区间[1,31]  -1 代表任意
	int nHours; //正常取值区间[0,23]  -1 代表任意
	int nMinutes;//正常取值区间[0,59] -1 代表任意
}; 

typedef struct BOX_AREA_PROPERTY
{
	float   m_fLength;
	float   m_fHeight;
	float   m_fWidth;

	//fog 2
	float   m_fFogStart;		// 距离开始
	float   m_fFogEnd;			// 距离结束
	float   m_fFogDeepness;		// 垂直雾浓度
	DWORD   m_dwFogColor;		// 水平雾上半球颜色

	float   m_fFogStartNight;		// 距离开始-夜晚
	float   m_fFogEndNight;			// 距离结束-夜晚
	float   m_fFogDeepnessNight;	// 垂直雾浓度-夜晚
	DWORD   m_dwFogColorNight;		// 水平雾上半球颜色-夜晚

	//fog of under water 1
	float   m_fUnderWaterFogStart;
	float   m_fUnderWaterFogEnd;
	float   m_fUnderWaterFogDeepness;
	DWORD   m_dwUnderWaterClr;

	float   m_fUnderWaterFogStartNight;
	float   m_fUnderWaterFogEndNight;
	float   m_fUnderWaterFogDeepnessNight;
	DWORD   m_dwUnderWaterClrNight;
	
	//sky box textures
	AString m_strSky0;
	AString m_strSky1;
	AString m_strSky2;
	
	AString m_strSkyNight0;//Night sky
	AString m_strSkyNight1;
	AString m_strSkyNight2;

	float   m_fCloudSpeedU;
	float   m_fCloudSpeedV;
	
	DWORD   m_dwCloudDirClr;//云层白天直射光颜色
	DWORD   m_dwCloudAmbientClr;//云层白天环境颜色
	DWORD   m_dwCloudNightDirClr;//云层黑夜直射光颜色
	DWORD   m_dwCloudNightAmbientClr;//云层的黑夜的环境光颜色
	//light
	A3DVECTOR3 m_vLightDir; //白天直射光方向 new
	A3DVECTOR3 m_vLightDirNight;//夜晚直射光方向 new
	
	DWORD      m_dwDirLightClr;//直射光
	DWORD      m_dwAmbient;
	float      m_fSunPower;//new
	int	       m_nMapSize;//new

	DWORD      m_dwDirLightClrNight;//Night light
	DWORD      m_dwAmbientNight;
	float      m_fSunPowerNight;//new
	int	       m_nMapSizeNight;//new
	
	//transition time
	DWORD   m_dwTransTime;
	
	//music
	//AString m_strMusic;
	AString m_strSound;
	

	//Nature
	int     m_nRainSpeed;
	float   m_fRainSize;
	DWORD   m_dwRainClr;
	
	int     m_nSnowSpeed;
	float   m_fSnowSize;
	DWORD   m_dwSnowClr;
	DWORD   m_dwFlag;//1b 表示有没有灯光，2b 表示下雨，3b表示下雪。4b,表是有没太阳，5b，表示有没月亮
	
	DWORD   m_dwTerrainLight;

	TIME_CTR m_StartTime;
	TIME_CTR m_EndTime;
	
	// add by ice 2011/03/17
	int		m_nBoxAreaType; // 0为A3DSkySphere，1为A3DSkyBox

	//sky box textures
	AString m_strSkyTop;
	AString m_strSkyBottom;
	AString m_strSkyLeft;
	AString m_strSkyRight;
	AString m_strSkyFront;
	AString m_strSkyBack;
	
	// 暂不生效，需要引擎添加支持
	AString m_strSkyNightTop;
	AString m_strSkyNightBottom;
	AString m_strSkyNightLeft;
	AString m_strSkyNightRight;
	AString m_strSkyNightFront;
	AString m_strSkyNightBack;

	// version 17
	float	m_fCapTile; //重复次数(0 - @ 1表示不重复,默认8)
	float	m_fCapRot; //角度 1/30秒为单位，输出时弧度

	int	    m_nPriority; //越小越高

	AString m_strColorMappingTex;

	// new fog
	DWORD   m_dwFogColor2;			// 水平雾下半球
	float   m_fFogHeightStart;		// 垂直雾高度开始
	float   m_fFogHeightEnd;		// 垂直雾高度结束
	float   m_fFogDensityStart;		// 上半球浓度
	float   m_fFogDensityEnd;		// 下半球浓度
	float   m_fFogColorDensityStart;// 上半球不透明度
	float   m_fFogColorDensityEnd;	// 下半球不透明度
	float   m_fFogHorzeDensity;		// 水平雾浓度
	float   m_fFogHorzePower;		// 上下半球过渡

	DWORD   m_dwFogColor2Night;				// 水平雾下半球-夜晚
	float   m_fFogHeightStartNight;			// 垂直雾高度开始-夜晚
	float   m_fFogHeightEndNight;			// 垂直雾高度结束-夜晚
	float   m_fFogDensityStartNight;		// 上半球浓度-夜晚
	float   m_fFogDensityEndNight;			// 下半球浓度-夜晚
	float   m_fFogColorDensityStartNight;	// 上半球不透明度-夜晚
	float   m_fFogColorDensityEndNight;		// 下半球不透明度-夜晚
	float   m_fFogHorzeDensityNight;		// 水平雾浓度-夜晚
	float   m_fFogHorzePowerNight;			// 上下半球过渡-夜晚

	DWORD   m_dwSunShineColor;
	float   m_fBloomThredhold;

	DWORD   m_dwSunShineColorNight;
	float   m_fBloomThredholdNight;
}BOXAREA_PROPERTY;

void Init_BoxArea_Data(BOX_AREA_PROPERTY &data);


struct NATURE_DATA_SET_FUNCS : public CUSTOM_FUNCS
{
	virtual BOOL CALLBACK OnActivate(void);
	virtual LPCTSTR CALLBACK OnGetShowString(void) const;
	virtual AVariant CALLBACK OnGetValue(void) const;
	virtual void CALLBACK OnSetValue(const AVariant& var);

	AVariant	m_var;
	CString		m_strText;
};

struct LIGHTING_SET_FUNCS : public CUSTOM_FUNCS
{
	virtual BOOL CALLBACK OnActivate(void);
	virtual LPCTSTR CALLBACK OnGetShowString(void) const;
	virtual AVariant CALLBACK OnGetValue(void) const;
	virtual void CALLBACK OnSetValue(const AVariant& var);

	AVariant	m_var;
	CString		m_strText;
};

struct START_TIME_SET_FUNCS : public CUSTOM_FUNCS
{
	virtual BOOL CALLBACK OnActivate(void);
	virtual LPCTSTR CALLBACK OnGetShowString(void) const;
	virtual AVariant CALLBACK OnGetValue(void) const;
	virtual void CALLBACK OnSetValue(const AVariant& var);

	AVariant	m_var;
	CString		m_strText;
};

struct END_TIME_SET_FUNCS : public CUSTOM_FUNCS
{
	virtual BOOL CALLBACK OnActivate(void);
	virtual LPCTSTR CALLBACK OnGetShowString(void) const;
	virtual AVariant CALLBACK OnGetValue(void) const;
	virtual void CALLBACK OnSetValue(const AVariant& var);

	AVariant	m_var;
	CString		m_strText;
};

class CSceneBoxArea : public CSceneObject
{
public:
	CSceneBoxArea();
	CSceneBoxArea(bool bDefault);
	virtual ~CSceneBoxArea();

	virtual void ReInit() /*override*/; // 在拷贝构造或者赋值后调用，将指针字段初始化为NULL或者新对象。赋值前还应调用Release
	virtual void UpdateProperty(bool bGet);
	virtual void Render(A3DViewport* pA3DViewport);
	virtual void PostRender(A3DViewport* pA3DViewport);
	virtual void Tick(DWORD timeDelta);

	//Load data
	virtual bool Load(CELArchive& ar,DWORD dwVersion,int iLoadFlags);
	//Save data
	virtual bool Save(CELArchive& ar,DWORD dwVersion);
	
	virtual void		Translate(const A3DVECTOR3& vDelta);
	virtual void		RotateX(float fRad);
	virtual void		RotateY(float fRad);
	virtual void		RotateZ(float fRad);
	virtual void        Rotate(const A3DVECTOR3& axis,float fRad);
	virtual void        Scale(float fScale);
	virtual void        SetPos(const A3DVECTOR3& vPos);
	virtual void        Release();
	virtual void        EntryScene();
	virtual void        LeaveScene();
	
	//  Copy object
	virtual CSceneObject*		CopyObject();

	//判断一个点是否在区域的内部,如果在内返回为TRUE
	bool IsInside(const A3DVECTOR3& vPoint);
	bool IsInActiveTime();
	bool IsPriority();
	bool IsTimeValid(TIME_CTR& _time);
	int CompareTime(TIME_CTR& t1, SYSTEMTIME& t2);


	CSceneBoxArea* GetLastBoxArea(){ return m_pLastArea; };

	void SetLastBoxArea( CSceneBoxArea* pLastArea){ m_pLastArea = pLastArea; };

	void GetProperty(BOXAREA_PROPERTY & data)
	{
		data = m_PropertyData;
	}

	void SetProperty(BOXAREA_PROPERTY & data)
	{
		m_PropertyData = data;
		ReBuildArea();
	}

	//光线跟踪程序
	bool	RayTrace(const A3DVECTOR3& vStart,const A3DVECTOR3& vEnd);
	
	void    SetSkyTexture(const AString& str1, const AString& str2, const AString& str3)
	{
			m_PropertyData.m_strSky0 = str1;
			m_PropertyData.m_strSky1 = str2;
			m_PropertyData.m_strSky2 = str3;
	}

	//背景音乐的处理函数
	void InitMusic();
	void PlayMusic();
	void StopMusic();

	void UpdateMapParams( CElementMap * pMap, float fInvTime );

	void OnCamEntry(bool bForceUpdate=false);//摄象机进入区域事件
	void OnCamLeave();//摄象机离开区域事件

	bool IsDefaultArea(){ return m_bIsDefault; };
	bool InitNatureObjects();

	//判断区域是否跟该区域相交
	bool IsIntersectionWithArea(const A3DVECTOR3& vMin, const A3DVECTOR3& vMax);

	void ReBuildArea();
	void BuildAreaProperty();
	BOXAREA_PROPERTY m_PropertyData;
	
	int		GetCopyFlags(){ return m_nCopy; }
	void    SetCopyFlags( int nCopy){ m_nCopy = nCopy; }

	A3DRain* GetRain() { return m_pRain; }
	A3DSnow* GetSnow() { return m_pSnow; }

//	bool ShowSun() { return bShowSun; }
//	bool ShowMoon() { return bShowMoon; }
private:
	
	CSceneBoxArea *m_pLastArea;

	//测试点跟平面的关系，如果在平面的正前方返回TRUE
	bool DotWithPlane(const A3DVECTOR3& testPoint,const A3DVECTOR3& point1, const A3DVECTOR3& point2, const A3DVECTOR3& point3);
	
	//2D线的拾取
	bool IntersectionLineWithDot(int x1, int y1, int x2, int y2 , int dotX, int dotY);
	
	//保存了区域盒子的8个点
	A3DVECTOR3 Vertices[8];

	A3DVECTOR3 m_vMin;
	A3DVECTOR3 m_vMax;

	//当前区域的天空盒子
//	A3DSky*		m_pA3DSky;

	//天气效果
	A3DRain*			m_pRain;					//	rain in the world
	A3DSnow*			m_pSnow;					//	snow in the world
	NATURE_DATA_SET_FUNCS *m_pNatureDataSetFunc;
	LIGHTING_SET_FUNCS    *m_pLightingSelFunc;
	START_TIME_SET_FUNCS  *m_pStartTimeFunc;
	END_TIME_SET_FUNCS    *m_pEndTimeFunc;
	bool m_bIsInside;

	//是否是默认的区域
	bool m_bIsDefault;

#ifdef _ANGELICA22
#else
	APtrList<AString *> m_listMusic;
#endif // #ifdef _ANGELICA22
	
	/*
	 下面是插值用的临时变量
	 */
	
	DWORD m_dwEscapeTime;
	bool  m_bTransDone;

	AString strText;
	bool bSnow;
	bool bRain;
	bool bLight;
	bool bShowMoon;
	bool bShowSun;
	int  m_nCopy;//数据套数，决定是第几套数据(0,1)
};

typedef CSceneBoxArea* PSCENEBOXAREA;

#endif // !defined(AFX_SCENEBOXAREA_H__73631896_32C9_416D_96F8_C6A0F53196C4__INCLUDED_)
