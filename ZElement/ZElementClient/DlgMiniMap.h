// Filename	: DlgMiniMap.h
// Creator	: Tom Zhou
// Date		: October 15, 2005

#pragma once

#include "hashmap.h"
#include "DlgBase.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUICheckBox.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIStillImageButton.h"

#define AUTOMOVE_POINTDEC 10

class A3DStream;
class A3DTexture;

class CDlgMiniMap : public CDlgBase  
{
	friend class CECGameUIMan;
	friend class CDlgInputName;
	friend class CDlgTerrWarMap;
	friend class CDlgWorldMap;
	friend class CDlgMidMap;
	friend class CDlgChat;
	friend class CDlgTask;
	friend class CDlgTaskDaily;
	friend class CDlgMapTransfer;

	AUI_DECLARE_EVENT_MAP();
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgMiniMap();
	virtual ~CDlgMiniMap();
	virtual bool Render(void);

	void OnCommand_close(const char * szCommand);
	void OnCommand_openmarkwin(const char * szCommand);
	void OnCommand_bigmap(const char * szCommand);
	void OnCommand_midmap(const char * szCommand);
	void OnCommand_roomout(const char * szCommand);
	void OnCommand_roomin(const char * szCommand);
	void OnCommand_mode(const char * szCommand);
	void OnCommand_makemark(const char * szCommand);
	void OnCommand_listregion(const char * szCommand);
	void OnCommand_edit(const char * szCommand);
	void OnCommand_arrow(const char * szCommand);
	void OnCommand_automove(const char * szCommand);
	void OnCommand_delete(const char * szCommand);
	void OnCommand_deleteall(const char * szCommand);
	void OnCommand_RadarHelp(const char * szCommand);
	void OnCommand_daily(const char * szCommand);
	void OnCommand_CANCEL(const char * szCommand);
	void OnCommand_onexp(const char * szCommand);
	void OnCommand_Auction(const char * szCommand);
	void OnCommandBroadcast(const char * szCommand);
	void OnCommand_NewHelp(const char * szCommand);
	void OnCommand_Vip(const char * szCommand);
	void OnCommand_GMBbs(const char * szCommand);
	void OnCommand_Tower(const char * szCommand);
	void OnCommand_UpdateLog(const char * szCommand);
	void OnCommand_AttiUp(const char * szCommand);

	void OnCommand_Recommand(const char* szCommand);
	void OnCommand_ZhuxianKnows(const char* szCommand);
	void OnCommand_Activity(const char* szCommand);
	void OnCommand_Dungeon_6V6(const char* szCommand);
	void OnCommand_Group(const char * szCommand);
	void OnCommand_6V6(const char * szCommand);
	void OnCommand_Hot(const char* szCommand);
	void OnCommand_King(const char* szCommand);
	void OnCommand_RedRadio(const char* szCommand);
	void OnCommand_ZhuxianExplore(const char* szCommand);
	void OnCommand_QuestKf(const char* szCommand);
	void OnCommand_Ancient(const char* szCommand);
	void OnCommand_ThemeLink(const char* szCommand);

	void OnEventMouseMove_Hide_2(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonUp_Hide_2(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDown_List_Choose(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventRButtonDown_List(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	void SetNewMail(int nRemainTime = 0);
	void ChangeWorldInstance(int idInstance);
	static void MiniMapRenderCallback(const A3DRECT &rc, DWORD param1, DWORD param2, DWORD param3);

	static void SetWarBuildStatus(int key, int status, A3DVECTOR3 pos, float hp);

public:
	typedef struct _MAPMARK
	{
		ACString strHint;
		A3DVECTOR3 vecPos;
		A3DRECT rect;
	} MAPMARK, * P_MAPMARK;
	typedef struct _MARK
	{
		int nNPC;
		ACString strName;
		A3DVECTOR3 vecPos;
		int	idInstance;
	} MARK, * P_MARK;
	
	// x,y 渲染目标位置
	// tu,tv 渲染源区域 0.0 - 1.0
	// tu2,tv2 渲染源比例， 1为原始大小
	struct MAPTLVERTEX
	{
		FLOAT x, y, z, rhw;
		DWORD diffuse;
		DWORD specular;
		FLOAT tu, tv, tu2, tv2;
	};
	struct RadarPoint
	{
		int	type;
		A3DPOINT2 pos;
	};
	struct BUILDING_STATUS
	{
		unsigned char status;
		A3DVECTOR3 pos;
		float hp;
	};

	enum
	{
		MODE_A = 0,
		MODE_B
	};
	enum
	{
		MAPMARK_PLAYER = 0,
		MAPMARK_MONSTER,
		MAPMARK_MONSTERBOSS,
		MAPMARK_MONSTERTASK,
		MAPMARK_GUILD,
		MAPMARK_FAMILY,
		MAPMARK_FRIEND,
		MAPMARK_NPC,
		MAPMARK_NPCACCEPTTASK,
		MAPMARK_NPCFINISHTASK,
		MAPMARK_TEAMMATE,
		MAPMARK_CAPTAIN,
		MAPMARK_ENEMY,
		MAPMARK_HOST,
		MAPMARK_ENEMYGUILD,
		MAPMARK_TAG0,
		MAPMARK_TAG1,
		MAPMARK_TAG2,
		MAPMARK_TAG3,
		MAPMARK_TAG4,
		MAPMARK_TAG5,
		MAPMARK_TAG6,
		MAPMARK_TAG7,
		MAPMARK_TAG8,
		MAPMARK_TAG9,
		MAPMARK_AUTOMOVE,
		MAPMARK_TAGCLICK,
		MAPMARK_TARGET,
		MAPMARK_BUILDINGA,
		MAPMARK_BUILDINGB,
		MAPMARK_WAR_ATTACKERCENTER,
		MAPMARK_WAR_DEFENDERCENTER,
		MAPMARK_WAR_ATTACKERZONE,
		MAPMARK_WAR_DEFENDERZONE,
		MAPMARK_WAR_DEFENDERTOWER,
		MAPMARK_WAR_DEFENDERTOWERSMALL,
		MAPMARK_WAR_DEFENDERTOWERASSIST,
		MAPMARK_WAR_DEFENDERWALL,
		MAPMARK_WAR_ATTACKERCENTER1,
		MAPMARK_WAR_DEFENDERCENTER1,
		MAPMARK_WAR_ATTACKERZONE1,
		MAPMARK_WAR_DEFENDERZONE1,
		MAPMARK_WAR_DEFENDERTOWER1,
		MAPMARK_WAR_DEFENDERTOWERSMALL1,
		MAPMARK_WAR_DEFENDERTOWERASSIST1,
		MAPMARK_WAR_DEFENDERWALL1,
		MAPMARK_EVENT,
		MAPMARK_NUM
	};
	enum
	{
		TIME_DAY = 0,
		TIME_MORNING,
		TIME_DUSK,
		TIME_NIGHT
	};
	static DWORD m_dwTempTargetTime;
	static A3DVECTOR3 m_vecTempTarget;
	static int m_idTempTargetInstance;

protected:
	static int m_nMode;
	static float m_fZoom;
	static bool m_bVisionLock;
	static bool m_bShowTargetArrow;
	static A3DTexture *m_pA3DTexMask;
	static abase::vector<MARK> m_vecMark;
	static abase::vector<MAPMARK> m_vecMiniMapMark;
	static MAPTLVERTEX m_vertexMiniMap[4];
	static A3DStream *m_pMiniMapStream;
	static A3DTexture *m_pTexRadar;
	static AString m_szTexRadar;
	static A2DSprite *m_pA2DSpriteCompass;
	static A2DSprite **m_pA2DMapMark;
	static A2DSprite *m_pA2DRadarBack;
	static A3DVECTOR3 m_vecNowTarget;
	static BUILDING_STATUS m_WarBuildingStatus[20];

	PAUIIMAGEPICTURE	m_pImg_ServerTime;
	PAUILABEL			m_pLab_Date;
	PAUICHECKBOX		m_pChk_Mail;
	PAUILABEL			m_pLab_MapName;
	PAUISTILLIMAGEBUTTON	m_pBtn_Auction;
	PAUISTILLIMAGEBUTTON	m_pBtn_Radio;
	PAUISTILLIMAGEBUTTON    m_pAncient;
	PAUISTILLIMAGEBUTTON    m_pQuestKf;
	
	static bool			m_bOpendAuction;
	static bool			m_bShouldOpenAuction;

	static bool			m_bGuajiCrossDay;

	bool RadioIsPlaying();
	bool UpdateMiniMap();
	virtual bool OnInitDialog();
	virtual void OnTick();
	virtual void Resize(A3DRECT rcOld, A3DRECT rcNew);
};

int compare( const void *arg1, const void *arg2 );
