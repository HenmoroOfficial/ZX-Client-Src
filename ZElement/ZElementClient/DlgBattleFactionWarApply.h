// Filename	: DlgBattleFactionWarApply.h
// Creator	: Wang Dongliang
// Date		: 2012/08/21
// Desc		: 帮战申请界面

#pragma once

#include "DlgBase.h"
class AUIEditBox;
class AUILabel;
class AUIStillImageButton;

class CDlgBattleFactionWarApply : public CDlgBase  
{
	
	AUI_DECLARE_EVENT_MAP()
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgBattleFactionWarApply();
	virtual ~CDlgBattleFactionWarApply();
	virtual bool OnInitDialog();
	virtual void OnShowDialog() ;
	virtual void OnHideDialog() ;
	virtual void OnTick() ;
	virtual int  CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
public:
	/*
	   功能：验证当前角色是否能够申请帮战
	   参数：无
	   返回值：true 可以申请  false 不可以申请
	*/
	bool CheckApplyCondition();

	void OnCommand_Apply(const char * szCommand);
	void OnCommand_Cancel(const char * szCommand);
    /*
	   功能：处理帮战邀请对话框
	   参数：isAgree 是否同意
	   返回值： 无
	*/
	void ProcessFactionWarInvite(bool isAgree);
	/*
	   功能：通过玩家头像直接申请帮战(直接替换当前申请)
	   参数：name 玩家名称
       返回值：无
	*/
	void ApplyFactionWarByTargetHead(const ACHAR* name);
private:
	AUIEditBox* m_pEditName;
	AUILabel*   m_pLabelTime;
	AUIStillImageButton* m_pBtnApply;

	//申请后的开始时间
	int         m_startTime;

	//是否正在申请中
	bool        m_IsApplying;


	//收到帮战邀请协议需要保存的数据
	struct SFactionWarInvite 
	{
		//副本地图ID
		int mapid;
		//邀请方帮派ID
		int inviterid;
		//被邀请方帮派ID
		int dstid;
	} m_factionWarInvite;

private:
	//重置相关数据和相关控件的状态
	void ResetData();
	
};