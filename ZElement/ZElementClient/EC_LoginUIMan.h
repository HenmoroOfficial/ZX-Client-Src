/*
 * FILE: EC_LoginUIMan.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/8/27
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "AUI\\AUI.h"
#include "roleinfo"
#include "DlgTaskDaily.h"
#include "EC_RoleTypes.h"
#ifndef BACK_VERSION
#include "ECManager.h"
#include "ECManagerID.h"
#endif

class CECPlayer;
class CECLoginPlayer;
class A3DSurface;
class AMVideoClip;

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////
#define CECLOGINUIMAN_MAX_ROLES			8
#define CECLOGINUIMAN_NUM_GENDERS		2
#define CECLOGINUIMAN_NUM_PHYSIQUES		5

///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECLoginUIMan
//	
///////////////////////////////////////////////////////////////////////////

class CECLoginUIMan : public AUILuaManager
{
	friend class CDlgLoginServerList;
public:
	enum
	{
		CREATE_TYPE_MIN,
		CREATE_TYPE_HUMAN,
		CREATE_TYPE_WILD,
		CREATE_TYPE_RACE03,
		CREATE_TYPE_NUM = CREATE_TYPE_WILD,
	};

	enum
	{
		LOGIN_MODEL_NORMAL = 0,	// 人族
		LOGIN_MODEL_SMALL,		// 烈山
		LOGIN_MODEL_MIDDLE,		// 天华
		LOGIN_MODEL_BIG,		// 九黎
		LOGIN_MODEL_SMALLNOEAR,	// 怀光
		LOGIN_MODEL_BIG2,		// 太昊
		LOGIN_MODEL_MIDDLE2,		// 羲和？2012.9新职业
		LOGIN_MODEL_NORMAL2,     //人马
		LOGIN_MODEL_SMALL2,    //萝莉
		LOGIN_MODEL_NUM,
	};

	enum
	{
		SHOW_EQUIP_NUM = 3,
	};

	enum LOGIN_SCENE
	{
		LOGIN_SCENE_NULL		= -1,
		LOGIN_SCENE_SELCHAR		= 0,	//选人摄像机
		LOGIN_SCENE_CREATE_HUMAN,		//创建人族
		LOGIN_SCENE_CREATE_WILD,		//创建神族
		LOGIN_SCENE_CREATE_QIANJI,		//创建牵机
		LOGIN_SCENE_CREATE_YINGZHAO,    //创建英招
		LOGIN_SCENE_NUM,
	};

public:		//	Constructor and Destructor
	CECLoginUIMan();
	virtual ~CECLoginUIMan();

	//	Initalize manager
	virtual bool Init(A3DEngine* pA3DEngine, A3DDevice* pA3DDevice, const char* szDCFile=NULL);
	//	Release manager
	virtual bool Release(void);

	virtual void RearrangeWindows(A3DRECT rcOld, A3DRECT rcNew);

	virtual bool Tick(DWORD dwDeltaTime);
	virtual bool Render(void);

	bool RenderBackground(void);

	//	On command
	virtual bool OnCommand(const char* szCommand, AUIDialog* pDlg);
	//	On message box
	virtual bool OnMessageBox(int iRetVal, AUIDialog* pDlg);
	//	Handle windows message
	virtual bool DealWindowsMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	//	Handle event
	virtual bool OnEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, AUIDialog* pDlg, AUIObject* pObj);

	bool LaunchPreface();						// 进入游戏显示第一步
	bool LaunchCharacter(bool bLogout = false);	// 登陆成功
	bool LaunchMatrixChallenge(void* pData);
	bool LaunchLoading();						// 开始进入游戏

	bool CreateCharacter(GNET::RoleInfo &ri);	//	创建成功，客户端处理
	bool AddCharacter(GNET::RoleInfo* pri);		//	客户端添加一角色
	bool DelCharacter(int nResult, int idRole);	//  客户端删除一角色
	bool RestoreCharacter(int nResult, int idRole);	//  客户端恢复一角色
	bool ChangeCustomizeData(int idRole);
	
	bool UIControlCursor();
	void ShowErrorMsg(const ACHAR *pszMsg, const char *pszName = "");

	int GetIDCurRole() { return m_idCurRole;};
	void SetIDCurRole(int nIDCurRole) { m_idCurRole = nIDCurRole;};
	abase::vector<GNET::RoleInfo> GetVecRoleInfo() { return m_vecRoleInfo;}
	int PickRole(int x, int y);				// 弃用

	bool OnCommand_Agreement(const char* szCommand, AUIDialog* pDlg);
	bool OnCommand_SelChar(const char* szCommand, AUIDialog* pDlg);
	
	void ShowCombineDialogs(const char *szName, bool bShow = true);
	//void RenderShowModel();
	void SetPwFlag(char flag) { m_cPwWarn = flag;}
	
	bool OpenURL(const char* szSect, const char* szKey);
	bool ChangeScene(LOGIN_SCENE scene);

protected:	//	Attributes
	A2DSprite*	m_pLoginBack;
	A2DSprite*	m_pCreateBackHuman;
	A2DSprite*	m_pCreateBackWild;
	A2DSprite*	m_pLogo;			// 中间最上
	A2DSprite*	m_pKrLogo;			// x: 距右边40  y: 1/3高
	int			m_idGfxLogin;	// gfx
	int			m_idGfxCreateBack;	// gfx
	int			m_idGfxSelectBack;	// gfx
	int			m_idGfxCreateHuman;	// gfx
	int			m_idGfxCreateWild;	// gfx

	//诛仙3新添特效
	int         m_iGfxSelectRoleBack;		 //选择人物
	int         m_iGfxHumanCreateBack;       //人族创建
	int         m_iGfxWildCreateBack;        //神裔创建
	int         m_iGfxTianmaiCreateBack;     //天脉创建
	AMVideoClip* m_pIntroMovie;		// 暂时弃用，没有"video\\intro.mpg"

	A3DVECTOR3 m_PosRole[LOGIN_SCENE_NUM];
	A3DVECTOR3 m_PosCamera[LOGIN_SCENE_NUM];
	A3DVECTOR3 m_DirCamera[LOGIN_SCENE_NUM];
	A3DVECTOR3 m_CloseUpPosCamera[2][PHYSIQUE_NUM];
	A3DVECTOR3 m_CloseUpDirCamera[2][PHYSIQUE_NUM];
	int			m_iCameraPathWeight;		// 切换摄像机尾问
//	float modelPosAdjust[4][2];
	int m_iCurScene;
	
	AUIObject	*m_pwdCurObj;		// 当前选中的密保框
	float	    m_fskStartPos;		// 转轮密保
	int			m_iMouseX;			// 转轮密保
	int			m_iMouseY;			// 转轮密保
	
	// role draging when create and customize	弃用
	bool		m_bDragRole;		// flag indicates whether drag the role to rotate
	APointI		m_ptDragStart;		// drag start position
	A3DVECTOR3	m_vecDirBeforeDrag;	// role's direction before drag
	A3DVECTOR3	m_vecUpBeforeDrag;	// role's up direction before drag

	// Select role.
	int	m_idCurRole;				// 0-7 当前选择角色
	abase::vector<GNET::RoleInfo> m_vecRoleInfo;		// 角色列表
	abase::vector<CECLoginPlayer *> m_vecRoleModel;		// 角色模型列表
	abase::vector<TaskDailyInfo> m_vecTaskInfo;
	abase::vector<TaskDailyInfo> m_vecTaskInfoAllDay;


	int	m_iCurCreate;			// 当前创建种族（人or神） CREATE_TYPE_XXX
	int m_nCurProfession;		// 当前创建职业体型 LOGIN_MODEL_XXX
	int m_nCurGender;			// 当前创建性别0-male  1-female
	CECLoginPlayer *m_aModel[LOGIN_MODEL_NUM][CECLOGINUIMAN_NUM_GENDERS];	// 创建角色用，每种体型性别
	bool			m_bStand[LOGIN_MODEL_NUM][CECLOGINUIMAN_NUM_GENDERS];
	
	// 选择种族
	DWORD m_dwStartRaceTime;
	int   m_iShowTime;
	int	  m_iStopTime;
	int	  m_iNewModelTime;

	bool  m_bEnter;				// 保证不重复2个角色，客户端服务器不同步
	char  m_cPwWarn;			// 长时间没有登陆或修改密码提示

	ACHAR m_szUserName[256];					// user name remembered
	abase::vector<ACString> m_vecMailList;		// mail suffix list

	static const int msi_EquipIds[14][SHOW_EQUIP_NUM][CECLOGINUIMAN_NUM_GENDERS][4];

	bool OnCommand_Login(const char* szCommand, AUIDialog* pDlg);
	bool OnCommand_Softkb(const char* szCommand, AUIDialog* pDlg);
	bool OnCommand_NewChar(const char* szCommand, AUIDialog* pDlg);
	bool OnCommand_PwdProtect(const char* szCommand, AUIDialog* pDlg);
	bool OnCommand_PwdProtectResponse(const char* szCommand, AUIDialog* pDlg);

	void RebuildRoleList();		// 刷新账号角色
	bool RefreshPlayerList();	// login ui render players 
	void AdjustCamera();
	void AdjustRole(int x, int y);
	
	void LoadUserServer(bool bSel=true);
	void SaveServer(ACString strServerName, ACString strAddr);

	void LoadMailSuffixList();

	A2DSprite*	CreateSprite(AString path);
	int			CreateGfx(AString path);
	
	virtual PAUIDIALOG CreateDlgInstance(const AString strTemplName);

	void InitLoginLayout();
};
#ifndef BACK_VERSION
//Add by pyp 2013/1/6
class EC_ManLoginUIMan : public ECManager
{
public:
	EC_ManLoginUIMan(CECGameRun* pGameRun) : ECManager(pGameRun)
	{
		m_pLoginUIMan = NULL;
	}
	//	Render routine, for registering objects
	virtual bool Render(ECViewport* pViewport) { return true; }
	//  Render objects after scene rendered
	virtual bool RenderPost(ECViewport* pViewport)
	{
		return m_pLoginUIMan->RenderBackground();
	}
	
	void SetLoginUIMan(CECLoginUIMan* pLoginUIMan) { m_pLoginUIMan = pLoginUIMan; }
	
private:
	CECLoginUIMan* m_pLoginUIMan;
};
#endif
///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////

