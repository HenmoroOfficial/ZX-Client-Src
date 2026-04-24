#ifndef _TASKINTERFACE_H_
#define _TASKINTERFACE_H_

#include <stddef.h>
#include "vector.h"

#ifdef	WIN32
	#define _TASK_CLIENT
	#ifndef int64_t
	#define int64_t __int64
	#endif
#endif

// Task Prerequisite Error Code
#define TASK_PREREQU_FAIL_INDETERMINATE	1
#define TASK_PREREQU_FAIL_NOT_ROOT		2
#define TASK_PREREQU_FAIL_SAME_TASK		3
#define TASK_PREREQU_FAIL_NO_SPACE		4
#define TASK_PREREQU_FAIL_FULL			5
#define TASK_PREREQU_FAIL_CANT_REDO		6
#define TASK_PREREQU_FAIL_BELOW_LEVEL	7
#define TASK_PREREQU_FAIL_ABOVE_LEVEL	8
#define TASK_PREREQU_FAIL_NO_ITEM		9
#define TASK_PREREQU_FAIL_BELOW_REPU	10
#define TASK_PREREQU_FAIL_NOT_IN_CLAN	11
#define TASK_PREREQU_FAIL_WRONG_GENDER	12
#define TASK_PREREQU_FAIL_NOT_IN_OCCU	13
#define TASK_PREREQU_FAIL_WRONG_PERIOD	14
#define TASK_PREREQU_FAIL_PREV_TASK		15
#define TASK_PREREQU_FAIL_MAX_RCV		16
#define TASK_PREREQU_FAIL_NO_DEPOSIT	17
#define TASK_PREREQU_FAIL_NO_TASK		18
#define TASK_PREREQU_FAIL_NOT_CAPTAIN	19
#define TASK_PREREQU_FAIL_ILLEGAL_MEM	20
#define TASK_PREREQU_FAIL_WRONG_TIME	21
#define TASK_PREREQU_FAIL_NO_SUCH_SUB	22
#define TASK_PREREQU_FAIL_MUTEX_TASK	23
#define TASK_PREREQU_FAIL_NOT_IN_ZONE	24
#define TASK_PREREQU_FAIL_WRONG_SUB		25
#define TASK_PREREQU_FAIL_OUTOF_DIST	26
#define TASK_PREREQU_FAIL_GIVEN_ITEM	27
#define TASK_PREREQU_FAIL_LIVING_SKILL	28
#define TASK_PREREQU_FAIL_SPECIAL_AWARD	29
#define TASK_PREREQU_FAIL_PK_VALUE		30
#define TASK_PREREQU_FAIL_GM			31
#define TASK_PREREQU_FAIL_FRIENDSHIP	32
#define	TASK_PREREQU_FAIL_TITLE			33
#define	TASK_PREREQU_FAIL_CONTRIBUTION	34
#define	TASK_PREREQU_FAIL_NOT_FAMILY	35
#define	TASK_PREREQU_FAIL_NOT_HEADER	36
#define	TASK_PREREQU_FAIL_F_SKILL_LEV	37
#define	TASK_PREREQU_FAIL_F_SKILL_PROF	38
#define	TASK_PREREQU_FAIL_RECORD		39
#define TASK_PREREQU_FAIL_TALISMAN_VAL	40
#define	TASK_PREREQU_FAIL_FINISH_COUNT	41
#define TASK_PREREQU_FAIL_BATTLE_SCORE	42
#define TASK_PREREQU_FAIL_GLOBAL_COUNT	43
#define TASK_PREREQU_FAIL_LIFEAGAIN_CNT 44
#define TASK_PREREQU_FAIL_LIFEAGAIN_OCC 45
#define TASK_PREREQU_FAIL_PET           46
#define TASK_PREREQU_FAIL_NOT_MASTER    47
#define TASK_PREREQU_FAIL_NOT_PRENTICE  48
#define TASK_PREREQU_FAIL_NOT_P_IN_TEAM 49
#define TASK_PREREQU_FAIL_MASTER_MORAL  53
#define TASK_PREREQU_FAIL_CULT          54
#define TASK_PREREQU_FAIL_TEAM_NUM      55
#define TASK_PREREQU_FAIL_PRENTICE_LEV  56
#define TASK_PREREQU_FAIL_CONSUME_TREAS 57
#define TASK_PREREQU_FAIL_GLOBAL_EXP    58
#define TASK_PREREQU_FAIL_EXP_RUN       59
#define TASK_PREREQU_FAIL_ACCOMP		60
#define TASK_PREREQU_FAIL_TRANSFORM     61
#define TASK_PREREQU_FAIL_NO_REINFORCE_ITEM	62
#define	TASK_PREREQU_FAIL_OPEN_COUNT	63
#define TASK_PREREQU_FAIL_ACHIEVEMENT	64
#define TASK_PREREQU_FAIL_SUMMON_MONSTER 65
#define TASK_PREREQU_FAIL_CIRCLEGROUP	66
#define TASK_PREREQU_FAIL_TERRITORY_SCORE	67
#define TASK_PREREQU_FAIL_IN_CROSS_SERVER	68
#define TASK_PREREQU_FAIL_CROSSSERVER_GLOBAL	69
#define TASK_PREREQU_FAIL_FENGSHEN_LVL_OR_EXP 70
#define TASK_PREREQU_FAIL_CREATE_ROLE_TIME		71
#define TASK_PREREQU_FAIL_NATION_POSTION		72
#define TASK_PREREQU_FAIL_KING_BELOW_SCORE		73
#define TASK_PREREQU_FAIL_KING_ABOVE_SCORE		74
#define TASK_PREREQU_FAIL_KING_NOT_ENOUGH_SCORE	75
#define TASK_PREREQU_FAIL_HAS_KING				76
#define TASK_PREREQU_FAIL_TOTAL_CASEADD		77
#define TASK_PREREQU_FAIL_DYNCLEAR_TIME_ZERO 78  // 动态清空的全局变量值为0
#define TASK_PREREQU_FAIL_DYNFORBIDDEN		 79  // 服务器动态关闭任务
#define TASK_PREREQU_FAIL_FACTION_TASK_COUNT 80
#define TASK_PREREQU_FAIL_FACTION_GOLD_NOTE 81
#define TASK_PREREQU_FAIL_FACTION_GRASS 82
#define TASK_PREREQU_FAIL_FACITON_MINE 83
#define TASK_PREREQU_FAIL_FACTION_MONSTER_CORE 84
#define TASK_PREREQU_FAIL_FACTION_MONSTER_FOOD 85
#define TASK_PREREQU_FAIL_FACTION_MONEY	86
#define TASK_PREREQU_FAIL_FACTION_BUILDING 87
#define TASK_PREREQU_FAIL_INTEROBJ		88
#define TASK_PREREQU_FAIL_BUILDINGCON  89

#define TASK_AWARD_FAIL_GIVEN_ITEM		50
#define TASK_AWARD_FAIL_NEW_TASK		51
#define TASK_AWARD_FAIL_BELOW_LEV		52
#define TASK_AWARD_FAIL_NO_MORE_DEITY_EXP		72		//封神经验不足Added 2011-10-13.
#define TASK_AWARD_FAIL_INVALID_OCCUPATION		73		//职业条件不符合 Added 2011-12-14.

#define TASK_TEAM_RELATION_MARRIAGE		1

// Task messages
#define TASK_MSG_NEW					1
#define TASK_MSG_SUCCESS				2
#define TASK_MSG_FAIL					3

#define TASK_MAX_SIMULTANEOUS_COUT		50	//Modified from 20 2012-04-21.
#define TASK_HIDDEN_COUNT               3
#define	TASK_MAX_DELIVER_COUNT			5
#define	TASK_STORAGE_COUNT				9
#define	TASK_STORAGE_LEN				10
#define	TASK_STORAGE_BUF_SIZE			(TASK_STORAGE_COUNT * TASK_STORAGE_LEN * sizeof(short))

#define TASK_ACTIVE_LIST_HEADER_LEN		12
#define TASK_ACTIVE_LIST_MAX_LEN		120	//Modified from 50 --> 80 2012-04-18.
#define TASK_FINISHED_LIST_MAX_LEN		2040
#define TASK_DATA_BUF_MAX_LEN			32
#define TASK_FINISH_TIME_MAX_LEN		1700

// 当前激活的任务列表缓冲区大小
#define TASK_ACTIVE_LIST_BUF_SIZE		(TASK_ACTIVE_LIST_MAX_LEN * TASK_DATA_BUF_MAX_LEN + TASK_ACTIVE_LIST_HEADER_LEN + TASK_STORAGE_BUF_SIZE)
// 已完成的任务列表缓冲区大小
#define TASK_FINISHED_LIST_BUF_SIZE		4096
// 任务全局数据大小
#define TASK_GLOBAL_DATA_SIZE			256
// 任务完成时间
#define TASK_FINISH_TIME_LIST_BUF_SIZE	20480

// Masks
#define	TASK_MASK_KILL_MONSTER			0x00000001
#define TASK_MASK_COLLECT_ITEM			0x00000002
#define TASK_MASK_TALK_TO_NPC			0x00000004
#define TASK_MASK_REACH_SITE			0x00000008
#define TASK_MASK_ANSWER_QUESTION		0x00000010
#define TASK_MASK_TINY_GAME				0x00000020
#define TASK_MASK_INTER_REACH_SITE		0x00000040
#define TASK_MASK_INTER_LEAVE_SITE		0x00000080
#define TASK_MASK_COLLECT_INTEROBJ		0x00000100

#define MAX_MONSTER_WANTED				3	// 受ActiveTaskEntry大小限制，最大3
#define MAX_ITEM_WANTED					8
#define MAX_ITEM_AWARD					32
#define TASK_ZONE_FRIENDSHIP_COUNT		32
#define MAX_INTEROBJ_WANTED             3	// 受ActiveTaskEntry大小限制，最大3

#define MAX_MONSTER_SUMMONED_CAND		32	// 召唤归属怪的最大种类，允许ID重复 

#define TASK_MSG_CHANNEL_LOCAL			0
#define TASK_MSG_CHANNEL_WORLD			1
#define TASK_MSG_ChANNEL_FACTION        3
#define TASK_MSG_CHANNEL_BROADCAST		9


#define TASK_AWARD_MAX_CHANGE_VALUE     32

#ifndef _TASK_CLIENT
class TaskFamilyAsyncData;
#endif

struct task_team_member_info
{
	unsigned long	m_ulId;
	unsigned long   m_ulMasterID;
	unsigned long	m_ulLevel;
	unsigned long	m_ulOccupation;
	bool			m_bMale;
	unsigned long   m_ulLifeAgainCnt; //转生次数
	int				m_nFamilyId;
	unsigned long	m_ulWorldId;
	float			m_Pos[3];
};

struct special_award
{
	unsigned long	id;
	unsigned long	param;
};

struct Task_Log_AwardRegionRepu
{
	long lRegionIndex;
	long lRegionReputation;
};

struct Task_Log_AwardItem
{
	unsigned long ulItemID;
	unsigned long ulItemCnt;
};

#ifdef _TASK_CLIENT

struct Task_State_info
{
	unsigned long	m_ulTimeLimit;
	unsigned long	m_ulTimePassed;
	unsigned long	m_ulNPCToProtect;
	unsigned long	m_ulProtectTime;
	unsigned long	m_ulWaitTime;
	unsigned long	m_ulErrCode;

	int m_iFactionPropWanted[5];

	struct
	{
		unsigned long	m_ulMonsterId;
		unsigned long	m_ulMonstersToKill;
		unsigned long	m_ulMonstersKilled;
	} m_MonsterWanted[MAX_MONSTER_WANTED];

	struct
	{
		unsigned long	m_ulItemId;
		unsigned long	m_ulItemsToGet;
		unsigned long	m_ulItemsGained;
		unsigned long	m_ulMonsterId;
		unsigned long	m_ulReplaceItemId;
	} m_ItemsWanted[MAX_ITEM_WANTED];

	
	struct  
	{
		unsigned long m_ulInterObjId;
		unsigned long m_ulInterObjToCollect;
		unsigned long m_ulInterObjCollected;
	}m_InterObjWanted[MAX_INTEROBJ_WANTED];
};

struct AWARD_DATA;

struct Task_Award_Preview
{
	unsigned long	m_ulGold;
	int64_t			m_ulExp;
	unsigned long	m_ulSP;
	unsigned long	m_ulReputation;
	bool			m_bHasItem;
	bool			m_bItemKnown;
	unsigned long	m_ulItemTypes;
	unsigned long	m_ItemsId[MAX_ITEM_AWARD];
	unsigned long	m_ItemsNum[MAX_ITEM_AWARD];

	unsigned long	m_ulItemTypes2;	//随机奖励
	unsigned long	m_ItemsId2[MAX_ITEM_AWARD];
	unsigned long	m_ItemsNum2[MAX_ITEM_AWARD];

	long			m_aFriendships[TASK_ZONE_FRIENDSHIP_COUNT];
	int				m_nFamilySkillLev;
	int				m_nFamilySkillProficiency;
	int				m_nFamilySkillIndex;
	unsigned long	m_ulFengshenExp;//封神经验，神元 Added 2011-02-18
	unsigned long	m_ulBonus;//鸿利元宝, Added 2011-02-18
	unsigned long	m_ulBattleScore;//仙基，战场声望，战场积分, Added 2011-02-18
	int				m_iFactionGoldNote;
	int				m_iFactionGrass;
	int				m_iFactionMine;
	int				m_iFactionMonsterCore;
	int				m_iFactionMonsterFood;
	int				m_iFactionMoney;
};

#endif

#ifdef _TASK_CLIENT

	struct talk_proc;

#else

	struct TaskTeamInterface
	{
		virtual void SetMarriage(int nPlayer) = 0;
	};

#endif

struct TaskInterface
{
	virtual ~TaskInterface(){}
	virtual unsigned long GetPlayerLevel() = 0;
	virtual int GetTalismanValue() = 0;
	virtual void* GetActiveTaskList() = 0;
	virtual void* GetFinishedTaskList() = 0;
	virtual void* GetFinishedTimeList() = 0;
	virtual unsigned long* GetTaskMask() = 0;
	virtual void DeliverGold(unsigned long ulGoldNum, unsigned long ulPlayerID = 0) = 0;
	virtual void DeliverExperience(int64_t ulExp, unsigned long ulPlayerID = 0) = 0;
	virtual void DeliverSP(unsigned long ulSP, unsigned long ulPlayerID = 0) = 0;
	virtual void DeliverReputation(long ulReputation, unsigned long ulPlayerID = 0) = 0;
	virtual void DeliverRegionReputation(int index, long lValue, unsigned long ulPlayerID = 0) = 0;
	virtual void ResetRegionReputation(int index) = 0;
	virtual int GetTaskItemCount(unsigned long ulTaskItem) = 0;
	virtual int GetCommonItemCount(unsigned long ulCommonItem) = 0;
	virtual int GetReinforceItemCount(unsigned long ulItemId, unsigned long ulLevel, unsigned char cond_type) = 0;
	virtual bool IsInFaction() = 0;
	virtual int GetFactionLev() = 0;
	virtual int GetFactionRole() = 0;
	virtual unsigned long GetGoldNum() = 0;
	virtual void TakeAwayGold(unsigned long ulNum) = 0;
	virtual void TakeAwayTaskItem(unsigned long ulTemplId, unsigned long ulNum) = 0;
	virtual void TakeAwayCommonItem(unsigned long ulTemplId, unsigned long ulNum) = 0;
	virtual void TakeAwayReinforceItem(unsigned long ulTemplId, unsigned long ulItemLevel, unsigned char cond_type) = 0;
	virtual long GetReputation() = 0;
	virtual long GetRegionReputation(int index) = 0;
	virtual unsigned long GetCurPeriod() = 0;
	virtual unsigned long GetPlayerId() = 0;
	virtual unsigned long GetPlayerRace() = 0;
	virtual unsigned long GetPlayerLifeAgainCnt() = 0; //获得转生次数
	virtual unsigned long GetPlayerLifeAgainOccup(int nLifeAgainIndex) = 0; //根据转生索引获得对应转生职业
	virtual unsigned long GetPlayerOccupation() = 0;
	virtual bool IsDeliverLegal() = 0;
	virtual bool CanDeliverCommonItem(unsigned long ulItemTypes) = 0;
	virtual bool CanDeliverTaskItem(unsigned long ulItemTypes) = 0;
	virtual void DeliverCommonItem(unsigned long ulItemId, unsigned long ulCount, bool bBind, long lPeriod, unsigned long ulPlayerID = 0) = 0;
	virtual void DeliverTaskItem(unsigned long ulItemId, unsigned long ulCount, unsigned long ulPlayerID = 0) = 0;
	virtual void DeliverReinforceItem(unsigned long ulItemId, unsigned long ulLevel, bool bBind, long lPeriod) = 0;
	virtual unsigned long GetPos(float pos[3]) = 0;
	virtual bool HasLivingSkill(unsigned long ulSkill) = 0;
	virtual int GetPetCon() = 0;
	virtual int GetPetCiv() = 0;
	virtual long GetLivingSkillProficiency(unsigned long ulSkill) = 0;
	virtual void GetSpecailAwardInfo(special_award* p) = 0;
	virtual long GetPKValue() = 0;
	virtual bool IsGM() = 0;
	virtual bool HasTitle(short title) = 0;
	virtual void TakeAwayTitle(short title) = 0;
	virtual void DeliverPKValue(long lVal) = 0;
	virtual void ResetPKValue() = 0;
	virtual long GetFactionContribution() = 0;
	virtual void DeliverFactionContribution(long lVal) = 0;
	virtual int GetFamilyContribution() = 0;
	virtual int GetBattleScore() = 0;
	virtual int GetSJBattleScore() = 0;
	virtual unsigned long GetCultivation() = 0;			     //获得造化值
	virtual void SetCultivation(unsigned long ulCult) = 0;   //设置造化值
	virtual void ClearCultSkill(unsigned long ulCult) = 0;   //清空造化技能
	virtual void ClearNormalSkill() = 0;	  //清空普通技能
	virtual void ClearTalentSkill() = 0;	  //清空天书技能
	virtual int GetTreasureConsumed() = 0; //消费元宝数
	virtual int GetTotalCaseAdd() = 0;	   //充值元宝数
	
	virtual unsigned long GetAchievePoint() = 0;	// 获取成就点数
	virtual int GetCircleGroupId() = 0;				// 获取圈子id
	virtual unsigned long GetLastLogoutTime() = 0;	// 获取上次下线时间

	virtual unsigned long GetTerritoryScore() = 0;	// 获取领土战积分

	// 师徒
	virtual bool IsMaster() = 0; 
	virtual bool IsPrentice() = 0;
	virtual unsigned long GetInMasterLev() = 0;

	/* 组队任务信息*/
	virtual int GetTeamMemberNum() = 0;
	virtual void GetTeamMemberInfo(int nIndex, task_team_member_info* pInfo) = 0;
	virtual unsigned long GetTeamMemberId(int nIndex) = 0;
	virtual bool IsCaptain() = 0;
	virtual bool IsInTeam() = 0;
	virtual bool IsMale() = 0;
	virtual bool IsMarried() = 0;

	/* 生产技能信息*/
	virtual int GetProduceSkillLev() = 0;
	virtual void SetProduceSkillFirstLev() = 0;
	virtual int GetProduceSkillExp() = 0;
	virtual void AddProduceSkillExp(int nExp) = 0;

	/* 家族信息 */
	virtual bool IsInFamily() = 0;
	virtual int GetFamilyID() = 0;
	virtual void DeliverFamilyExperience(int nFamilyID, int64_t ulExp) = 0; //给同一线的家族成员(包括族长)发奖

	/* 随身包裹 */
	virtual bool IsMaxInvSize() = 0;
	
	/* 成就 */
	virtual bool IsAchievementFinish(unsigned short id) = 0;
	
	/* 变身 */
	virtual int GetTransformID() = 0;
	virtual int GetTransformLevel() = 0;
	virtual int GetTransformExpLevel() = 0;
	virtual void SetTransform(int iID, int iDuration, int iLevel, int iExpLevel, bool bCover) = 0;	
	
	/* 封神 */
	virtual unsigned long GetFengshenLevel() = 0;
	virtual bool IsFengshenExpFull() = 0;
	virtual void DeliverFengshenExp(unsigned long ulExp, unsigned long ulPlayerID = 0) = 0;
	virtual void DeliverFengshenDujie(unsigned long ulPlayerID = 0) = 0;

	//检查当前角色的创建时间长度 Added 2011-04-11.
	virtual unsigned long GetCreateRoleTimeDuration() = 0;

	/* 国战 */
	virtual bool HasKing() = 0;		// 是否有国王
	virtual bool IsKing() = 0;		// 玩家是否国王
	virtual bool IsQueen() = 0;		// 玩家是否王后
	virtual bool IsGeneral() = 0;	// 玩家是否将军
	virtual bool IsOfficial() = 0;	// 玩家是否大臣
	virtual bool IsMember() = 0;	// 玩家是否国王帮成员
	virtual bool IsGuard() = 0;		// 玩家是否亲卫
	virtual int  GetKingScore() = 0;// 获取国王积分


	//帮派基地属性
	virtual int GetBuildingLevel(int iBuildId) = 0;
	virtual int GetFactionGoldNote() = 0;
	virtual void DeliverFactionGoldNote(int iGoldNoteNum) = 0;
	virtual void TakeAwayFactionGoldNote(int iGoldNum) = 0;
	
	virtual int GetFactionGrass() = 0;
	virtual void DeliverFactionGrass(int iGrassNum) = 0;
	virtual void TakeAwayFactionGrass(int iGrasNum) = 0;

	virtual int GetFactionMine() = 0;
	virtual void DeliverFactionMine(int iMineNum) = 0;
	virtual void TakeAwayFactionMine(int iMineNum) = 0;

	virtual int GetFactionMonsterFood() = 0;
	virtual void DeliverFactionMonsterFood(int iMonsterFoodNum) = 0;
	virtual void TakeAwayFactionMosnterFood(int iMonsterFoodNum) = 0;

	virtual int GetFactionMonsterCore() = 0;
	virtual void DeliverFactionMonsterCore(int iMonsterCoreNum) = 0;
	virtual void TakeAwayFactionMonsterCore(int iMonsterCoreNum) = 0;

	virtual int GetFactionMoney() = 0;
	virtual void DeliverFactionMoney(int iMoneyNum) = 0;
	virtual void TakeAwayFactionMoney(int iMoneyNum) = 0;
	
	virtual void OnFBaseBuildingProgress(unsigned long task_id, int ivalue) = 0;

	virtual int GetBuildLevelInConstruct() = 0;


	virtual bool	InInterAction(int iInterObjId) = 0;

	

	//特殊接口,专用于寻宝网角色寄售时特殊任务判断 true:通过检查,不存在特殊任务,false:没通过检查 Added 20120-4-27.
	bool CheckSpecialTasksForConsign();

	void InitActiveTaskList();
	unsigned long GetActLstDataSize();
	unsigned long GetFnshLstDataSize();
	unsigned long GetFnshTimeLstDataSize();
	bool CanDoMining(unsigned long ulTaskId);

	bool CheckVersion();

	bool HasTask(unsigned long ulTaskId);

	unsigned short GetFactionTaskFinishedCount();
	
	// 是否在跨服服务器上
	virtual bool IsInCrossServer() = 0;

	// 获取指定id和数量的物品所占包裹槽个数
	virtual int GetItemSlotCnt(unsigned long ulItemTemplID, unsigned long ulItemNum) = 0;


	// static funcs
	static unsigned long GetCurTime();
	static void WriteLog(int nPlayerId, int nTaskId, int nType, const char* szLog);
	static void WriteKeyLog(int nPlayerId, int nTaskId, int nType, const char* szLog);
	static bool CalcFileMD5(const char* szFile, unsigned char md5[16]);
	
	static void WriteTaskAwardLog(int nPlayerID, int nTaskID,
								  unsigned long ulExp,
								  unsigned long ulGold, 
								  const abase::vector<Task_Log_AwardRegionRepu>& RegionRepuArr, 
								  const abase::vector<Task_Log_AwardItem>& ItemArr);
	static void WriteTaskDeliverLog(int nPlayerID, int nTaskID);

#ifdef _TASK_CLIENT
	static long m_tmFinishDlgShown;
	static void UpdateTaskUI();
	static void PopUpTaskMonsterInfo(unsigned long ulTask, unsigned long ulMonsterID);
	static void PopupTaskFinishDialog(unsigned long ulTaskId, const talk_proc* pTalk);
	static void PopupTaskCheckInfoDialog(unsigned long ulTaskId, bool bRemove = false);
	static void ShowMessage(const wchar_t* szMsg, int nIndex);
	static void ShowTaskMessage(unsigned long ulTask, int reason);
	static long GetFinishDlgShowTime() { return m_tmFinishDlgShown; }
	static void SetFinishDlgShowTime(long t) { m_tmFinishDlgShown = t; }
	static int GetTimeZoneBias();

	virtual void NotifyServer(const void* pBuf, size_t sz) = 0;
	virtual int GetFamilySkillLevel(int nIndex) = 0;
	virtual int GetFamilySkillProficiency(int nIndex) = 0;
	virtual int GetFamilyMonsterRecord(int nIndex) = 0;
	virtual int GetFamilyCommonValue(int nIndex) = 0;
	//摄像机移动
	virtual void CameraMove(unsigned long ulCameraIndex) = 0;

	// 播放动画
	virtual void PlayAnimation(unsigned long ulAnimationID) = 0;
	
	// 播放gfx
	virtual void PlayAwardGfx(unsigned long ulGfxID) = 0;

	// Play小游戏
	virtual void PlayTinyGame(unsigned long ulGameID) = 0;

	unsigned long GetTaskCount();
	unsigned long GetTaskId(unsigned long ulIndex);
	void OnUIDialogEnd(unsigned long ulTask);

	// 无返回-1
	int GetFirstSubTaskPosition(unsigned long ulParentTaskId);
	unsigned long GetNextSub(int& nPosition);
	unsigned long GetSubAt(int nPosition);

	unsigned long CanDeliverTask(unsigned long ulTaskId);
	bool CanShowTask(unsigned long ulTaskId);
	bool CanFinishTask(unsigned long ulTaskId);
	void GiveUpTask(unsigned long ulTaskId);
	void GiveUpAutoDeliverTask(unsigned long ulTaskId);
	const unsigned short* GetStorageTasks(unsigned int uStorageId);

	void GetTaskStateInfo(unsigned long ulTaskId, Task_State_info* pInfo);
	bool GetAwardCandidates(unsigned long ulTaskId, AWARD_DATA* pAward);
	void GetTaskAwardPreview(unsigned long ulTaskId, Task_Award_Preview* p);
	void GetCanDeliverTaskAwardPreview(unsigned long ulTaskId, Task_Award_Preview* p);	

	unsigned long GetTaskFinishedTime(unsigned long) { return 0; }

	virtual unsigned long GetCircleGroupPoints() = 0;	// 获取圈子积分	
	virtual unsigned long GetFriendNum() = 0;			// 获取好友数量
	
	virtual void DirectFinishTask(unsigned long ulTaskId);	//直接完成某任务

#else
	virtual void NotifyClient(const void* pBuf, size_t sz) = 0;
	virtual void NotifyPlayer(unsigned long ulPlayerId, const void* pBuf, size_t sz) = 0;
	virtual float UnitRand() = 0;
	virtual unsigned long GetTeamMemberPos(int nIndex, float pos[3]) = 0;
	virtual void SetCurPeriod(unsigned long ulPeriod) = 0;
	virtual void SetNewRelayStation(unsigned long ulStationId) = 0;
	virtual void SetStorehouseSize(unsigned long ulSize) = 0;
	virtual void SetFactionStorehouseSize(unsigned long ulSize) = 0;
	virtual void SetFuryUpperLimit(unsigned long ulValue) = 0;
	virtual void TransportTo(unsigned long ulWorldId, const float pos[3], long lController) = 0;
	virtual void SetPetInventorySize(unsigned long ulSize) = 0;
	virtual void SetMountInventorySize(unsigned long ulSize) = 0;
	virtual void SetNewProfession(unsigned long ulProfession) = 0;
	virtual void SetLifeAgainOccup(unsigned long ulLifeAgainFaction) = 0;
	virtual void GiveOneTitle(long lTitle, unsigned long ulPlayerID = 0) = 0;
	virtual void SetInventorySize(long lSize) = 0;
	virtual void SetPocketSize(long lSize) = 0;
	virtual void CheckTeamRelationship(int nReason) = 0;
	virtual void Divorce() = 0;
	virtual void SetMonsterController(long id, bool bTrigger) = 0;
	virtual void DeliverBuff(long lBuffId, long lBuffLev) = 0;
	virtual void QueryFamilyData(const void* pData, int nSize) = 0;
	virtual void DeliverFamilyContribution(int nValue) = 0;
	virtual void SendMessage(unsigned long task_id, int channel, int param, long lVariables[3]) = 0;
	virtual void DeliverBonus( int nBonusNum ) = 0;//发放鸿利元宝,Added 2011-02-18.
	virtual void DeliverBattleScore(int nValue) = 0;//发放仙基（战场声望，战场积分）
	virtual void DeliverSJBattleScore(int nValue) = 0;
	virtual void ResetSJBattleScore() = 0;
	virtual void SetDoubleExpTime(unsigned long t) = 0;
	virtual void OutMaster() = 0;      //出师
	virtual void DeviateMaster() = 0;  //叛师
	virtual void DeliverOpenSoulEquip() = 0;	//任务开启角色元魂装备位 Added 2012-04-18.
	
	
	void BeforeSaveData();
	void PlayerEnterWorld();
	void PlayerLeaveWorld();
	void TakeAwayItem(unsigned long ulTemplId, unsigned long ulNum, bool bCmnItem)
	{
		if (bCmnItem) TakeAwayCommonItem(ulTemplId, ulNum);
		else TakeAwayTaskItem(ulTemplId, ulNum);
	}
	bool OnCheckTeamRelationship(int nReason, TaskTeamInterface* pTeam);
	void OnCheckTeamRelationshipComplete(int nReason, TaskTeamInterface* pTeam);
	virtual bool ClearSomeIllegalStates() = 0; //清除某些非法状态
	
	//全局key/value	
	virtual long GetGlobalValue(long lKey) = 0;                  //获取key值对应全局value
	virtual void PutGlobalValue(long lKey, long lValue) = 0;     //根据key值设置全局value
	virtual void ModifyGlobalValue(long lKey, long lValue) = 0;	 //根据key值修改全局value

	/* 成就 */
	virtual void FinishAchievement(unsigned short id) = 0;
	virtual void UnfinishAchievement(unsigned short id) = 0;
	virtual void OnTaskComplete(unsigned long task_id, unsigned short uFinishCount) = 0;

	// 召唤任务归属怪
	// 归属模式：0:默认的普通怪 1:自己 2:师傅 3:徒弟 4:仙侣 5:队伍 6:队伍或自己 7:队伍与自己 8:不限
	virtual void SummonMonster(unsigned long ulTemplId, bool bIsMonster, unsigned long ulNum, unsigned long ulMapId, const float pos[3], unsigned long ulPeriod, unsigned long ulSummonMode) = 0;

	// 更新圈子积分
	virtual void UpdateCircleGroupPoint(unsigned long ulPoint) = 0;
	virtual void QueryCircleGroupData(const void* pData, int nSize) = 0;

	// 查询好友人数
	virtual void QueryFriendNum(unsigned long ulTaskId) = 0;

	// 保护特定NPC
	virtual void ProtectNPC(unsigned long ulTaskId, unsigned long ulNPCTemplId, unsigned long ulPeriod) = 0;
		
	// 获取任务完成次数
	int GetTaskFinishCount(unsigned long ulTask);

	// 扣除国王积分
	virtual void CostKingScore(int nValue) = 0;
	// 发国王积分
	virtual void DeliverKingScore(int nValue) = 0;
	
	// 清xp技能cd
	virtual void ClearXpSkillCD() = 0;

	//开启关闭相位
	virtual void SetPhase(int id, bool bTrigger) = 0;	

	virtual void DeliverFactionAuctionItem(int idItem, float prob) = 0;

	virtual void TravelRound(int iTravelItemId, int iTravelTime, float fSpeed, int iTravelPath) = 0;

	// 检查任务是否动态关闭
	virtual bool IsDynamicForbiddenTask(int ulTaskId) = 0;
		 
	// Debug cmd, set family monster record value
	void SetFamilyMonRecord(int nRecordIndex, int nValue, TaskFamilyAsyncData* pAsyncData);

#endif
};

#endif
