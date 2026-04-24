#ifndef _TASKSERVER_H_
#define _TASKSERVER_H_

#include "TaskInterface.h"

#ifndef _TASK_CLIENT

class elementdataman;

// 加载任务文件
bool LoadTasksFromPack(elementdataman* pDataMan, const char* szPackPath, const char* szDynPackPath, const char* szForbiddenTaskPath, const char* szNPCInfoPath);

// 释放任务文件
void ReleaseTaskTemplMan();

// 策划导出的版本
int TaskGetExportVersion();

// 通知队长建立
void OnTeamSetup(TaskInterface* pTask);

// 通知队长解散
void OnTeamDisband(TaskInterface* pTask);

// 通知队长加入成员
void OnTeamAddMember(TaskInterface* pTask, const task_team_member_info* pInfo);

// 通知队长成员退出
void OnTeamDismissMember(TaskInterface* pTask, unsigned long ulPlayerId);

// 通知队员退出
void OnTeamMemberLeave(TaskInterface* pTask);

// 通知队员队伍队长更替
void OnTeamCaptainChange(TaskInterface* pTask);

// 接收其他玩家通知
void OnPlayerNotify(TaskInterface* pTask, unsigned long ulPlayerId, const void* pBuf, size_t sz);

// 接收客户端通知
void OnClientNotify(TaskInterface* pTask, const void* pBuf, size_t sz);

// 检查任务能否发放
bool OnTaskCheckDeliver(TaskInterface* pTask, unsigned long ulTaskId, unsigned long ulStorageId);

// NPC发放任务物品
void OnNPCDeliverTaskItem(TaskInterface* pTask, unsigned long ulNPCId, unsigned long ulTaskId);

// 检查在NPC处完成的任务
void OnTaskCheckAward(TaskInterface* pTask, unsigned long ulTasklId, int nChoice);

// 玩家被杀
void OnTaskPlayerKilled(TaskInterface* pTask);

// 定时检查整个任务系统限时且有接收者上限任务的状态
void OnTaskCheckAllTimeLimits(unsigned long ulCurTime);

//单人收集互动物品
void OnTaskCollectInterObj(TaskInterface* pTask, unsigned long ulTemplId);

//互动物品到达指定地点
void OnTaskInterReachSite(TaskInterface* pTask,unsigned long ulInterObjId);

//互动物品离开指定地点
void OnTaskInterLeaveSite(TaskInterface* pTask,unsigned long ulInterObjId);


// 单人杀怪
void OnTaskKillMonster(TaskInterface* pTask, unsigned long ulTemplId, unsigned long ulLev, float fRand);

// 组队杀怪
void OnTaskTeamKillMonster(TaskInterface* pTask, unsigned long ulTemplId, unsigned long ulLev, float fRand);

// 非组队共享杀怪 Added 2012-04-09.
void OnTaskShareKillMonster(TaskInterface* pTask, unsigned long ulTemplId, unsigned long ulLev, float fRand);

// 挖任务矿
void OnTaskMining(TaskInterface* pTask, unsigned long ulTaskId);

// 请求任务全局数据
void TaskQueryGlobalData(
	unsigned long ulTaskId,
	unsigned long ulPlayerId,
	const void* pPreservedData,
	size_t size);

// 更新任务全局数据
void TaskUpdateGlobalData(
	unsigned long ulTaskId,
	const unsigned char pData[TASK_GLOBAL_DATA_SIZE]);

// 获得全局数据
void OnTaskReceivedGlobalData(
	TaskInterface* pTask,
	unsigned long ulTaskId,
	unsigned char pData[TASK_GLOBAL_DATA_SIZE],
	const void* pPreservedData,
	size_t size);

// 获得家族数据
void OnTaskReceiveFamilyData(TaskInterface* pTask, const void* pData, int nSize, int nRetCode);

// 加入家族通知
void OnTaskPlayerJoinFamily(TaskInterface* pTask);

// 退出家族通知
void OnTaskPlayerLeaveFamily(TaskInterface* pTask);

// 遗忘生活技能
void OnForgetLivingSkill(TaskInterface* pTask);

// 玩家升级
void OnTaskPlayerLevelUp(TaskInterface* pTask);

// 定时检查玩家任务状态
void OnTaskCheckState(TaskInterface* pTask);

// 获得圈子数据
void OnTaskReceiveCircleGroupData(TaskInterface* pTask, const void* pData, int nSize, int nRetCode);

// 获得好友数量
void OnTaskReceiveFriendNumData(TaskInterface* pTask, unsigned long ulTaskId, unsigned long ulFriendNum);

// 护送NPC通知
void OnTaskProtectNPCNotify(TaskInterface* pTask, unsigned long ulTaskId, int nReason);

// 添加或者移出某任务到任务完成列表中，目前暂时仅供调试使用的接口 Added 2011-03-23.
void OnUpdateFinishTaskList( TaskInterface* pTask, unsigned long ulTaskId, int param1, int param2 );

// 改变已知ID任务的完成次数，目前仅供调试使用的接口 Added 2011-03-23.
void OnUpdateFinishTaskCount( TaskInterface* pTask, unsigned long ulTaskId, int nFinishTaskCount );

// 通过调试命令发放任务，目前暂时提供有条件判断和无条件判断的任务发放功能 Added 2011-04-21.
void OnDeliverTaskForDebug( TaskInterface* pTask, unsigned long ulTaskId, int param );

// 改变已知ID任务的状态，目前仅供调试使用 Added 2013-02-21.
void OnUpdateClearTaskState( TaskInterface* pTask, unsigned long ulTaskId );

// 玩家失去族长职位时通知(传位或退家族)
void  OnTaskPlayerLeaveFamilyHeader( TaskInterface* pTask );

// 玩家失去帮主职位时通知(传位或退帮派)
void  OnTaskPlayerLeaveFactionHeader( TaskInterface* pTask );

#endif

#endif
