// Filename	: DlgBattleDungeonHeyang.h
// Creator	: Fu Chonggang
// Date		: 2012.4.19
// Desc		: 梦回河阳副本，用任务做，每个副本就是一连串任务

#pragma once

#include "DlgBase.h"
#include "vector.h"

class AUIListBox;
class AUILabel;

/*
Battle_Dungeon_Heyang.xml
*/
class CDlgBattleDungeonHeyang : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();
public:
	struct HeYangConfig 
	{
		int			id;				// 模板中副本的全局变量
		int			iTaskStartId1;	// 开始弹出界面任务id1
		int			iTaskStartId2;	// 开始弹出界面任务id2
		int			iTaskEndId;		// 关闭界面任务id
		ACString	strNameStage;	// 左边显示的每一关名字（每一关是一个任务）
		ACString	strNameMonster; // 这一关的目标，可能是多个怪的一个综合数量
		int			iMonsterNum;	// 目标数量
		AString		strIcon;		// 这一关的图片描述
		ACString	strDesc;		// 这一关的文字描述
	};

public:
	CDlgBattleDungeonHeyang();
	virtual ~CDlgBattleDungeonHeyang();
	
	// 是否在每一关开始的时候自动弹出帮助界面，保存到本地
	void OnCommandSet(const char * szCommand);
	void OnCommandQuit(const char * szCommand);
	void OnEventLButtonDownStage(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);

	// 根据id获取图片路径和文字描述
	void GetInfo(int id, AString &strIcon, ACString &strDesc);
protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();

private:
	abase::vector<HeYangConfig> m_vecConfig;
	
	AUILabel*	m_pLabTimeLeft;
	AUIListBox*	m_pLstStage;
	int			m_iIndex;	// 当前配置level
};


/*
Battle_Dungeon_Heyang_Intro.xml
*/
class CDlgBattleDungeonHeyangIntro : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
	
public:
	CDlgBattleDungeonHeyangIntro(){}
	virtual ~CDlgBattleDungeonHeyangIntro(){}
	
	void OnCommandSet(const char * szCommand);
protected:
	virtual void OnShowDialog();
};