// Filename	: DlgBattleDungeonTowerIngame.h
// Creator	: Fu Chonggang
// Date		: 2012.4.19
// Desc		: 爬塔副本游戏过程中一些界面：老虎机、技能...
#pragma once

#include "DlgBaseFadeOut.h"

class AUIImagePicture;
class AUILabel;
class CECSkill;

//////////////////////////////////////////////////////////////////////////
/*
 CDlgBattleDungeonTowerRandom		老虎机界面
 Battle_Dungeon_Tower_Random.xml
 */
//////////////////////////////////////////////////////////////////////////

class CDlgBattleDungeonTowerRandom : public CDlgBaseFadeOut  
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();
	
public:
	CDlgBattleDungeonTowerRandom(){}
	virtual ~CDlgBattleDungeonTowerRandom(){}

	void OnCommand_Start(const char * szCommand);
	void OnCommand_IDCANCEL(const char * szCommand);
	void OnEventLButtonDownItem(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	
	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	
protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();

	void RandomOneMonster();		// 超过ms_SelectTime自动随机选择一张
	void PickOneCard(int index);	// 收到服务器选牌协议，更新信息
	void TickDisOrderCard();		// 洗牌

	AUIImagePicture *m_pImgPoker[12];
	AUIImagePicture *m_pImgSelected[4];

	AUILabel	*m_pLabTime;
	AUILabel	*m_pLabTimes;

	int			m_iTime;			// 每一轮选怪倒计时
	int			m_iPickIndex[4];	// 当前选中的牌index，选中哪个都是random，不重要1-12
	int			m_iResult[4];		// 选中的monsterid，0表示空
	int			m_iPickCount;		// 已经选几个了
	bool		m_bSend;

	int			m_iState;			// -1:打开接卖弄 0:展示牌  1:洗牌   2:选牌阶段 3:选牌完毕
	int			m_iDisOrderTime;	// 模拟洗牌倒计时
	int			m_iCurHintIndex;

	static const int ms_SelectTime;		// 每轮选牌最大时间，超过自动选择
	static const int ms_DisOrderTime;	// 洗牌总时间
};



//////////////////////////////////////////////////////////////////////////
// CDlgBattleDungeonTowerStage		战场基本信息：倒计时、当前关卡
// Battle_Dungeon_Tower_CharScore.xml
// 爬塔副本中使用，在副本线中始终存在或在普通线中始终不存在
//////////////////////////////////////////////////////////////////////////

class CDlgBattleDungeonTowerStage : public CDlgBase  
{
public:
	CDlgBattleDungeonTowerStage() {}
	virtual ~CDlgBattleDungeonTowerStage() {}

	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	
protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();

	AUILabel	*m_pLabTime;
	AUILabel	*m_pLabLevel;
	AUILabel	*m_pLabLevelCur;
};


//////////////////////////////////////////////////////////////////////////
// CDlgBattleDungeonTowerLuckySkill		幸运手牌技能和最小化按钮 处理手牌技能逻辑
// 界面显示改变用m_pDlgLuckySkill 和 m_pDlgLuckySkillMin 操作
//////////////////////////////////////////////////////////////////////////
class CDlgBattleDungeonTowerLuckySkill : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();

public:
	struct SkillState
	{
		CECSkill *pSkill;
		int		skill_id;		//技能ID
		int		skill_level;	//技能等级
		bool	used;			//是否已经使用
	};

	CDlgBattleDungeonTowerLuckySkill();
	virtual ~CDlgBattleDungeonTowerLuckySkill();
	
	void OnCommandOpen(const char * szCommand);

	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();

	static CDlgBattleDungeonTowerLuckySkill *m_pDlgLuckySkill;
	static CDlgBattleDungeonTowerLuckySkill *m_pDlgLuckySkillMin;

	static abase::vector<SkillState> m_vecSkill;
	static int	m_iUsedCount;
	static int	m_bSetSkillToPlayer;
};