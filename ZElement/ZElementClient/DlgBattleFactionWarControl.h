// Filename	: DlgBattleFactionWarControl.h
// Creator	: Wang Dongliang
// Date		: 2012/08/21
// Desc		: 帮战界面

#pragma once

#include "DlgBase.h"
class AUIListBox;
class AUILabel;
class CDlgBattleFactionWarControl : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP()
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgBattleFactionWarControl();
	virtual ~CDlgBattleFactionWarControl();
	virtual bool OnInitDialog();
	virtual void OnShowDialog() ;
	virtual void OnHideDialog() ;
	virtual void OnTick() ;
	virtual int  CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
public:
	/*
	  功能：请求帮战信息
	  参数：无
	  返回值：无
	*/
	 void GetFactionWarInfo();
	/*
	  功能：验证当前是否可以查看帮战界面
	  参数：无
      返回值：true 可以查看 false 不可以查看
	*/
    bool CheckFactionWarState();
	/*
	  功能：处理帮战参战对话框
	  参数：isAgree 是否立即进入帮战副本
      返回值： 无
	*/
	void ProcessFactionWarBegin(bool isAgree);


	void OnCommand_Enter(const char * szCommand);
	void OnCommand_Leave(const char * szCommand);
	void OnCommand_Close(const char * szCommand);

	//当前帮战状态 为0，则无帮战，否则，有帮战
	static int GetCombatFaction();
	//进入游戏时，对帮战信息进行清空
	static void ResetFactionWarInfo();

	//设置和获取进入帮战副本时的阵营
	void SetCurrentFaction(unsigned char cfaction){m_cFaction = cfaction;}
	unsigned char GetCurrentFaction(){return m_cFaction;}


public:
	static int  GetFactionPKRaidTemplateId() {return m_itemplateId;}
	static int  GetFactionPKRaidMapId(){return m_imapId;}
	//当前是否在帮战副本中
    bool        IsInFactionPKRaidMap();
	
private:
	//副本模板ID
	static const int         m_itemplateId;
	//副本地图ID
	static int               m_imapId;
	AUIListBox*  m_pLstAttack;
	AUIListBox*  m_pLstDefend;
	AUILabel*    m_pLabelAttacker;
	AUILabel*    m_pLabelDefender;

	//帮战剩余时间
	int	m_nLeftTime;
	//剩余时间的刷新时间
	DWORD m_dwLastRefreshTime;

    
	//在帮派副本中时所属的阵营
	unsigned char  m_cFaction;


};