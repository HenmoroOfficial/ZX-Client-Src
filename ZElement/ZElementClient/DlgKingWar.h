// Filename	: DlgKingWar.h
// Creator	: Wang Dongliang
// Date		: 2013/02/28
// Desc		: 国王战相关界面

#pragma once

#include "DlgBase.h"
#include "vector"

#define CECDLGKINGSHOP_PACKMAX 6
#define CECDLGKINGSHOP_PACKLINEMAX 3

enum
{
	   EKINGTITLE_NONE,
	   EKINGTITLE_KING,
	   EKINGTITLE_QUEEN,
	   EKINGTITLE_GENERAL,
	   EKINGTITLE_MINISTER,
       EKINGTITLE_MEMBER,
	   EKINGTITLE_GUARD,
	   EKINGTITLE_COUNT,
};
class AUILabel;
class AUIEditBox;
class AUIListBox;
class AUIImagePicture;
class AUIStillImageButton;
class AUIScroll;
class AUIProgress;
class CECIvtrItem;
class AUISubDialog;
class A2DSprite;
struct S2C::kingdom_mafia_info;

//最小化进入界面
class CDlgKingMin : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP()
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgKingMin(){}
	virtual ~CDlgKingMin(){}

	virtual void OnTick();
	//打开国王信息界面
	void OnCommand_Open(const char * szCommand);

};
//国号更改界面(5月版本用)
class CDlgKingNameChange : public CDlgBase
{
	AUI_DECLARE_EVENT_MAP()
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgKingNameChange(){}
	virtual ~CDlgKingNameChange(){}
private:
};
//圣旨发布界面
class CDlgKingChangeSlogan : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP()
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgKingChangeSlogan(){}
	virtual ~CDlgKingChangeSlogan(){}
	virtual void OnShowDialog();
	virtual void OnTick();
	void OnCommand_Confirm(const char * szCommand);
	void OnCommand_Cancel(const char * szCommand);
	void ProcessSloganChange(bool isAgree);
private:
	
};
//职务授予界面
class CDlgKingGrantOfficial : public CDlgBase
{
	enum
	{
	   EVOCATION_GENERAL,
	   EVOCATION_CHANCELLOR,
	};
	AUI_DECLARE_EVENT_MAP()
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgKingGrantOfficial(){}
	virtual ~CDlgKingGrantOfficial(){}
	virtual void OnShowDialog();
	virtual void OnTick();
	void OnCommand_Confirm(const char * szCommand);
	void OnCommand_Cancel(const char * szCommand);
private:
};
//授予权利菜单(三月版本)
class CDlgKingGrantRightContext : public CDlgBase
{
	AUI_DECLARE_EVENT_MAP()
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgKingGrantRightContext(){}
	virtual ~CDlgKingGrantRightContext(){}
	virtual void OnTick();
	void OnCommand_Dismiss(const char * szCommand);
	//void OnCommand_Cancel(const char * szCommand); 
	void ProcessKingDismiss(bool isAgree,int roleid);
private:
	
};
//授予权利界面(5月版本用)
class CDlgKingGrantRight : public CDlgBase
{
	AUI_DECLARE_EVENT_MAP()
		AUI_DECLARE_COMMAND_MAP()
public:
	CDlgKingGrantRight(){}
	virtual ~CDlgKingGrantRight(){}
private:
};

class CDlgKingShop;
class CDlgKingOfficialInfo;
class CDlgKingContinueAward;
class CDlgKingAnnounce;
//国王信息界面
class CDlgKingInfo : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP()
	AUI_DECLARE_COMMAND_MAP()

	enum
    {
	   EKING_PAGE_OFFICIAL,
	   EKING_PAGE_SHOP,
	   EKING_PAGE_AWARD,
	   EKING_PAGE_TASK,
	   EKING_PAGE_COUNT,
	};
public:
	struct SOfficailInfo
	{
		int      roleid;
		ACString name;
		unsigned int privilegesmask;
		bool     isOnline;
	};
	struct SKingWarBaseInfo
	{
		//角色职业
		int             nSelfTitle;
		//权利
		unsigned int    nPrivilegesMask;
		//帮派
		int             nFaction;
		//国王
		int             nKing;
		//王后
		int             nQueen;
		//连任次数
		int             nContinue;
		//是否领取奖励
		//char            bAward;
		unsigned int             reward_mask;
		//国王帮名字
		ACString        strFactionName;
		//国号名字
		ACString        strCountryName;
		//国家积分数
		int             nCountryScore;
		//国王名字
		ACString        strKingName;
		//天后名字
		ACString        strQueenName;
		//圣旨内容
		ACString        strSlogan;
		//个人积分数
		int             nSelfScore;
		
		//将军
		abase::vector<SOfficailInfo>  vecGenerals;
		//宰相
		abase::vector<SOfficailInfo>  vecChancellors;
		//亲卫
		abase::vector<SOfficailInfo>  vecGuards;
	};
	CDlgKingInfo(){}
	virtual ~CDlgKingInfo(){}
	virtual int  CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	//-----------一级按钮
	void OnCommand_Close(const char * szCommand);
	//更改国号
	void OnCommand_ChangeCountryName(const char * szCommand);
	//圣旨发布
	void OnCommand_ChangeSlogan(const char * szCommand);
	//职位授予
	void OnCommand_GrantOfficial(const char * szCommand);
	//积分使用
	void OnCommand_UseScores(const char * szCommand);
	//连任奖励
	void OnCommand_Award(const char * szCommand); 
	//任务发布
	void OnCommand_PostTask(const char * szCommand); 
	//远程包裹
	void OnCommand_RemoteBag(const char * szCommand);
	//三倍经验
	void OnCommand_TriExp(const char * szCommand);
	//双倍掉落
	void OnCommand_DoubleDrop(const char * szCommand);
	//查看任务
	void OnCommand_ViewTask(const char * szCommand);
	//亲卫传送
	void OnCommand_Transmit(const char* szCommand);
	//泡澡
	void OnCommand_OpenBathroom(const char* szCommand);

	//-----------一级按钮

	void ProcessGameAttri(bool isAgree,int flag);
	void ProcessOpenBathroom(bool isAgree);


private:
	//基本信息
	void SetKingBaseInfo();
	//刷新职位信息
	void RefreshKingOfficialInfo();
	//刷新积分兑换信息
	void RefreshKingShopInfo();
	//刷新奖励信息
	void RefreshKingAwardInfo();
	//刷新任务信息
	void RefreshKingTaskInfo();
	//设置控件状态
	void SetKingBaseState();
	//响应三倍经验与双倍掉落
	void OnGameAttri(int flag);
	//职位改变时刷新界面信息
	void UpdateKingInfoState();
public:
	//使用物品信息填充图片
    void SetImageInfoByItem(CECIvtrItem* pItem,AUIImagePicture* pObj,bool bUsePic = true);
private:
	//缓存界面信息
	static SKingWarBaseInfo   m_KingBaseInfo;

	AUISubDialog*             m_pSubDlgOfficial;
	AUISubDialog*             m_pSubDlgShop;
	AUISubDialog*             m_pSubDlgAward;
	AUISubDialog*             m_pSubDlgAcceptTask;
	AUISubDialog*             m_pSubDlgReleaseTask;

	int                       m_iCurPage;
    
	//亲卫传送冷却
    //DWORD         m_dwCallGuardTime;
	//泡澡申请冷却
	DWORD         m_dwBathApplayTime;

public:
	//获取职位
	static bool GetCadidateTitleByRoleId(int roleid,char& title,ACString& name);
private:
	//授予职位
	void    GrantOfficial(int roleid,char title,bool isOnline,const ACString& name);
	//解除职位
	void    DismissOfficial(int roleid,char title);

	friend CDlgKingShop;
	friend CDlgKingOfficialInfo;
	friend CDlgKingContinueAward;
	friend CDlgKingAnnounce;
	friend CDlgKingChangeSlogan;
	friend CDlgKingGrantOfficial;
	friend CDlgKingGrantRightContext;


	
};
//贡献度排行榜界面
class CDlgKingRankList : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP()
	AUI_DECLARE_COMMAND_MAP()
	enum
    {
	    ELINESOFONEPAGE = 20,
	};
    
public:
	CDlgKingRankList();
	virtual ~CDlgKingRankList(){}
	virtual void OnShowDialog();
	void OnCommand_Close(const char * szCommand);
	void OnCommand_PrefPage(const char * szCommand);
	void OnCommand_NextPage(const char * szCommand);

private:

	 //成员信息
	 struct SMemberInfo
	 {
	     //角色名称
		 ACString rolename;
		 //贡献度
		 int       score;
	 };
     //排行榜信息
	 struct  SRankListInfo
	 {
		 int                       selfscore;
		 int                       selfrank;
		 abase::vector<SMemberInfo>  ranklist;
	 }m_RankInfo;

	 //当前页码(从0开始)
	 int m_curPage;
	 //当前总页数
	 int m_pageCount;

protected:
	  void  RefreshSelfInfo();
	  void  RefreshRankListInfo();
	  void  RefreshPageInfo();

};
//战斗信息界面
class CDlgKingBattleInfo : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP()
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgKingBattleInfo(){}
	virtual ~CDlgKingBattleInfo(){}
	virtual void OnShowDialog();
	void SetEndTime(int endTime);
	void RefreshState(int  curnum,int maxnum,int numA,int numB);
	virtual void OnTick();
	void OnCommand_ExitKingWar(const char * szCommand);
private:
	int  m_nEndTime;
	
};
//战斗信息界面3
class CDlgKingBattleInfo3 : public CDlgBase  
{
	enum
	{
	   ERANK_COUNT_KILL = 10,
	   ERANK_COUNT_DEATH = 10,
	};
	enum
    {
		EBATTALE_PAGE_KILL,
		EBATTALE_PAGE_DEATH,
		EBATTALE_PAGE_DEFENDER,
		EBATTALE_PAGE_ATTACKER1,
		EBATTALE_PAGE_ATTACKER2,
		EBATTALE_PAGE_ATTACKER3,
		EBATTALE_PAGE_ATTACKER4,
	};

	AUI_DECLARE_EVENT_MAP()
	AUI_DECLARE_COMMAND_MAP()
public:
	struct SPlayerInfo 
	{
		SPlayerInfo()
		{
			memset(this,0,sizeof(SPlayerInfo));
		}
		int roleid;
		int mafiaid;
		int kill;
		int death;
	};
	enum SortType
	{
		ST_RANKKILL,
		ST_RANKDEATH,
	};

	CDlgKingBattleInfo3(){}
	virtual ~CDlgKingBattleInfo3(){}
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	void OnCommand_Close(const char * szCommand);
	void OnCommand_ViewRankKill(const char * szCommand);
	void OnCommand_ViewRankDeath(const char * szCommand);
	void OnCommand_ViewDefender(const char * szCommand);
	void OnCommand_ViewAttacker1(const char * szCommand);
	void OnCommand_ViewAttacker2(const char * szCommand);
	void OnCommand_ViewAttacker3(const char * szCommand);
	void OnCommand_ViewAttacker4(const char * szCommand);
	void OnCommand_ExitKingWar(const char * szCommand);
	void SetMafiaInfo(S2C::kingdom_mafia_info* info);
	void SetMafiaIds(int defender,int attackercount,int *attackers);
	void SetEndTime(int endTime,int wartype);
	void ProcessKingWarLeave(bool isAgree);
private:
	AUIListBox*            m_pListMembers;
	AUIStillImageButton*   m_pBtnDefender;
	AUIStillImageButton*   m_pBtnAttacker1;
	AUIStillImageButton*   m_pBtnAttacker2;
	AUIStillImageButton*   m_pBtnAttacker3;
	AUIStillImageButton*   m_pBtnAttacker4;
	int                    m_nEndTime;
	char                   m_cCurPage;
	int                    m_nDefender;
	int                    m_nAttacker1;
	int                    m_nAttacker2;
	int                    m_nAttacker3;
	int                    m_nAttacker4;

	abase::vector<SPlayerInfo>  m_vecPlayerInfo;
private:
	void DisPlayCurPagePlayerInfo(abase::vector<SPlayerInfo>& playinfos);
	void RefreshCurPage();
	void SetFactionName(AUIStillImageButton* pBtn,int iFacId,bool bDefender = false);
	void GetFactionName(AUIStillImageButton* pBtn);

	
	
};
//职位信息界面
class CDlgKingOfficialInfo : public CDlgBase
{
	AUI_DECLARE_EVENT_MAP()
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgKingOfficialInfo(){}
	virtual ~CDlgKingOfficialInfo(){}
	virtual void OnShowDialog();
	//授予将军
	void OnCommand_GrantGeneral(const char * szCommand);
	//授予宰相
	void OnCommand_GrantChancellor(const char * szCommand);
	//授予亲卫
	void OnCommand_GrantGuard(const char* szCommand);
	//将军右键菜单
	void OnEventRButtonUpLstGenerals(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	//宰相右键菜单
	void OnEventRButtonUpLstChancellors(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	//亲卫右键菜单
    void OnEventRButtonUpLstGuards(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	//职位信息
	void SetKingOfficialInfo();
	//
	void RefreshKingOfficailState();
private:
	//弹出授予权利菜单
	void PopUpGrantRightMenu(WPARAM wParam, LPARAM lParam, int vocation, AUIObject * pObj);
	//弹出职位授予界面
	void PopUpGrantOfficial(int vocation);
	

	
};
//连任奖励界面
class CDlgKingContinueAward : public CDlgBase
{
	AUI_DECLARE_EVENT_MAP()
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgKingContinueAward(){}
	virtual ~CDlgKingContinueAward(){}
	virtual void OnShowDialog();

	void OnCommand_GetAward(const char * szCommand);
private:
	//连任奖励信息
	void SetKingAwardInfo();
public:
	void RefreshKingAwardState();

};
//兑换物品界面
class CDlgKingShop : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP()
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgKingShop(){}
	virtual ~CDlgKingShop(){}
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnHideDialog();
	virtual void OnTick();
	virtual bool Release();
	void OnCommand_Exchange(const char * szCommand);
	void OnCommand_BuyPoints(const char * szCommand);
	void OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void ProcessKingShopExchange(bool isAgree,int flag);
private:
	AUILabel*            m_pTxtItem[CECDLGKINGSHOP_PACKMAX];
	AUILabel*            m_pLableItem[CECDLGKINGSHOP_PACKMAX];
	AUIImagePicture*     m_pImgItem[CECDLGKINGSHOP_PACKMAX];
    AUIStillImageButton* m_pButtonItem[CECDLGKINGSHOP_PACKMAX];
	AUIScroll*           m_pSclItem;
	int                  m_nFirstLine;
	abase::vector<CECIvtrItem*> m_vecItems;
	abase::vector<int>          m_vecItemScores;
	A2DSprite*	                m_pImagePresent;
	int			                m_nHintImageIndex;

	int                   m_iCurBuyIndex;
protected:
	void RefreshShopInfo();
    void SetScoreExchangeInfo();
	void SetBuyPointsInfo();
public:
	void RefreshKingShopState();
};
//任务发布/查看界面
class CDlgKingTask : public CDlgBase
{
	AUI_DECLARE_EVENT_MAP()
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgKingTask(){}
	virtual ~CDlgKingTask(){}
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	void OnCommand_Dliver(const char * szCommand);
	void OnCommand_Accept(const char * szCommand);
	void RefreshTaskState(int status,int type,int times,int issuetime);
	
private:
	int m_TaskType;
	int m_TaskStatus;
	
};

//圣旨中央显示界面
class CDlgKingAnnounce : public CDlgBase
{
	AUI_DECLARE_EVENT_MAP()
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgKingAnnounce(){}
	virtual ~CDlgKingAnnounce(){}
	virtual void OnShowDialog();
	virtual void OnTick();
	void SetMessage(const ACString& message);
private:
	//刷新时间
	DWORD m_dwStartTime;
};
//泡澡邀请界面
class CDlgKingBathInvite : public CDlgBase
{
    AUI_DECLARE_EVENT_MAP()
	AUI_DECLARE_COMMAND_MAP()
public:
    CDlgKingBathInvite(){}
	~CDlgKingBathInvite(){}
	virtual bool OnInitDialog();
	virtual void OnTick();
	virtual int  CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	void OnCommand_InviteBath(const char* szCommand);
	void OnCommand_Close(const char* szCommand);
	void SetLeftTimes(int times){m_nLeftTimes = times;}
	void DecLeftTimes(){--m_nLeftTimes;}
	bool IsBathing(){return m_bIsBathing;}
	DWORD GetBathElapse();
private:
	int           m_nLeftTimes;
	DWORD         m_dwLastTime;
	bool          m_bIsBathing;
	PAUILABEL     m_pLabel ;
	PAUISTILLIMAGEBUTTON m_pBtn;
	CECCounter		m_CoolCnt;
	DWORD         m_StartTime;;
};
//泡澡说明
class CDlgKingBathTip : public CDlgBase
{
	AUI_DECLARE_EVENT_MAP()
	AUI_DECLARE_COMMAND_MAP()
public:
    CDlgKingBathTip(){}
	~CDlgKingBathTip(){}
	virtual void OnTick();
	void OnCommand_Close(const char* szCommand);
};
