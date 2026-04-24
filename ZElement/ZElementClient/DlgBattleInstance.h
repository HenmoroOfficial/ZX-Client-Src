// File		: CDlgBattleInstance.h
// Creator	: Fuchonggang
// Date		: 2008/12/12

#pragma once

#include "DlgBase.h"
#include "hashmap.h"
#include "instancinggetlist_re.hpp"
#include "instancinggetfield_re.hpp"

struct INSTANCING_BATTLE_CONFIG;
class CLuaState;

class CDlgBattleInstance : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

	struct BATTLE_INTEAM
	{
		int idLine;
		int idMap;
	};

	struct LOCAL_GFieldInfo
	{
		int tid;
		GInstancingFieldInfo info;
	};

	enum
	{
		BATTLEINSTANCING_STATE_MIN		=	0,
		BATTLEINSTANCING_STATE_ACTIVE,
		BATTLEINSTANCING_STATE_TOOPEN,
		BATTLEINSTANCING_STATE_FIGHTING,
		BATTLEINSTANCING_STATE_READY,
		BATTLEINSTANCING_STATE_UNACTIVE,
		BATTLEINSTANCING_STATE_MAX,
	};

public:
	CDlgBattleInstance();
	virtual ~CDlgBattleInstance();

	void OnCommandActive(const char * szCommand);
	void OnCommandJoin(const char * szCommand);
	void OnCommandStart(const char * szCommand);
	void OnCommandLeave(const char * szCommand);
	void OnCommandKick(const char * szCommand);
	void OnCommandChange(const char * szCommand);
	void OnCommandAccept(const char * szCommand);
	void OnCommandRefuse(const char * szCommand);
	void OnCommandJoinType(const char * szCommand);
	void OnCommandMiniHint(const char * szCommand);
	void OnCommandCancel(const char * szCommand);

	void OnEventLButtonDownBattleField(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventLButtonDownBattleState(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	/** 报名界面列表 */
	void UpdateBattleList(void *pData);

	/** 某个战场的所有战场情况 */
	void UpdateBattleListInfo(void *pData);

	/** 通知转移者转移了 */
	void InstancingAppointRe(void *pData);

	/** 通知其它人转移了，客户端处理方式与上一个基本相同 */
	void InstancingAppointNotifyAction(void *pData);

	/** just error code */
	void InstancingEnterRe(void *pData);
	
	/** 当前排队战场信息 */
	void InstancingGetFieldRe(void *pData);
	
	/** 加入战场成功，进入排队等待进入战场状态 */
	void InstancingJoinRe(void *pData);
	
	/** 通知其他人有人加入了？ */
	void InstancingJoinReqAction(void *pData);
	
	/** 告诉踢人者踢掉了 */
	void InstancingKickRe(void *pData);
	
	/** 告诉其他人有人被踢了，无用，直接提示被踢 */
	void InstancingKickNotifyAction(void *pData);
	
	/** just show dialog false */
	void InstancingLeaveRe(void *pData);
	
	/** just error code */
	void InstancingStartRe(void *pData);
	
	/** 通知可以进入战场，确定吗？ */
	void InstancingStartNotifyAction(void *pData);
	
	/** nothing */
	void InstancingCloseNotifyAction(void *pData);
	
	/** nothing */
	void InstancingSelfInfo(void *pData);
	
	/** 战场各个阶段 */
	void InstancingInfo(void *pData);

	const static INSTANCING_BATTLE_CONFIG* GetInstanceConfig(int tid);
	static void SetScore(int iScore) { m_iScore = iScore;}

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	virtual	bool Release();
	void RefreshBattleList(bool bRefresh = false);
	ACString  GetFormatTime(int nTime);

	abase::vector<LOCAL_GFieldInfo>	m_vecFieldList;
	abase::vector<GBattleRole>		m_vecMembers;
	abase::vector<GBattleRole>		m_vecApplyers;
	abase::hash_map<int, bool>		m_mapFieldsGot;
	abase::hash_map<int, int>		m_mapEnterTime;

	static abase::hash_map<int, int>	m_mapMap2Tid;
	static int						m_nJoinTID;			// 申请或加入的战场
	static int						m_nTID;				// 查看的战场
	static CLuaState*				m_pState;
	static int						m_iScore;
	static int						m_iCurStage;
	static int						m_iStateTime;
};
