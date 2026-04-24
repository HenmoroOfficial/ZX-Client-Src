// Filename	: DlgBattleFactionWarWatch.h
// Creator	: Wang Dongliang
// Date		: 2012/08/21
// Desc		: 帮战观战界面

#pragma once

#include "DlgBase.h"
#include "vector.h"
class AUIListBox;
class AUILabel;
struct SFactionWarInfo
{
	   //副本房间号
	   int         roomid;
	   //显示信息前半段
	   ACString    attackerInfo;
	   //显示信息后半段
       ACString    defenderInfo;
	   //辅助数据1
	   int         data1;
	   //辅助数据2
	   int         data2;
	   SFactionWarInfo();
	   void SetData(int index,int data);
	   int  GetData(int index);
	   
	   
};
class CDlgBattleFactionWarWatch : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP()
    AUI_DECLARE_COMMAND_MAP()
public:
	CDlgBattleFactionWarWatch();
	virtual ~CDlgBattleFactionWarWatch();
	virtual bool OnInitDialog();
	virtual void OnShowDialog() ;
	virtual void OnHideDialog() ;
	virtual void OnTick() ;
	virtual int  CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
public:
	void OnCommand_ApplyWatch(const char * szCommand);
	void OnCommand_PrevPage(const char * szCommand);
	void OnCommand_NextPage(const char * szCommand);
	void OnCommand_Close(const char * szCommand);
	/*
	  功能：获取帮战列表
	  参数：无
	  返回值：无
	*/
	void GetFactionWarList();
    
	/*
	  功能：处理帮战观看对话框
	  参数：isAgree 是否观看
	  返回值：无
	*/
	void ProcessFactionWarWatchApply(bool isAgree);

	/*
	  功能：获取当前观看的帮战副本的房间号
	  参数：无
	  返回值：int 房间号
	*/
	static int  GetCurRoomId();
private:
	/*
	  功能：刷新帮战列表(用于翻页)
	  参数：无
	  返回值：无
	*/
	void RefreshMemberList();
	/*
	  功能：刷新页码信息
	  参数：无
	  返回值：无
	*/
	void RefreshPageInfo();

private:
    
	//当前页码(从0开始)
	int m_curPage;
	//当前总页数
	int m_pageCount;

	AUIListBox*  m_pLstMembers;
	AUILabel*    m_pLabelPage;
    
	//帮战信息列表
	abase::vector<SFactionWarInfo>  m_factionwarinfo;
    //当前申请观看的帮战索引
	int         m_ApplyIndex;


};