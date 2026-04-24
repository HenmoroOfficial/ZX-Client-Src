// Filename	: DlgAchievement.cpp
// Creator	: Fu Chonggang
// Date		: 2009/7/9

#pragma once

#include "DlgBase.h"

class AUILabel;
class AUIImagePicture;
class CECSkill;

class CDlgCommonDlg : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
public:
	CDlgCommonDlg();
	virtual ~CDlgCommonDlg();
	
	void OnCommandDetail(const char* szCommand);
	void OnCommandConfirm(const char* szCommand);
	void OnCommandGuess(const char* szCommand);
	void OnCommandRadio(const char* szCommand);
	void OnCommandCancel(const char* szCommand);

	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
protected:
	virtual	bool OnInitDialog();
	virtual void OnShowDialog();
};

class CDlgKrGM : public CDlgBase 
{
public:
	CDlgKrGM();
	virtual ~CDlgKrGM();
	
protected:
	virtual void OnShowDialog();
	virtual void OnTick();
private:
	DWORD m_dwTime;
};

// 是否界面并且定时关闭
class CDlgMessageBoxYesNoTime : public CDlgBase 
{
	AUI_DECLARE_COMMAND_MAP();
	
	enum
	{
		TYPE_RAID_ENTER,
		TYPE_RAID_VOTE,
		TYPE_RAID_6V6,
	};
public:
	CDlgMessageBoxYesNoTime();
	virtual ~CDlgMessageBoxYesNoTime();
	
protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	void OnCommandConfirm(const char* szCommand);
	void OnCommandCancel(const char* szCommand);
	void OnCommandConfirm2(const char* szCommand);
private:
	DWORD		m_dwTime;
	DWORD		m_dwTotalTime;
	AUILabel*	m_pLabTime;

	int			m_iType;
};

// 是否界面并且定时关闭
class CDlgMessageBoxYesNoCheck : public CDlgBase 
{
	AUI_DECLARE_COMMAND_MAP();
	
	enum
	{
		TYPE_DISABLE_HELP,
	};
public:
	CDlgMessageBoxYesNoCheck() {}
	virtual ~CDlgMessageBoxYesNoCheck() {}
	
protected:
	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	void OnCommandConfirm(const char* szCommand);
	void OnCommandCancel(const char* szCommand);
private:
	
	int			m_iType;
};

// 不再弹出提示消耗元宝
class CDlgMessageBoxYesNoCheckYuanBao : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
	
public:
	CDlgMessageBoxYesNoCheckYuanBao() {}
	virtual ~CDlgMessageBoxYesNoCheckYuanBao() {}
	
	void OnCommandConfirm(const char * szCommand);
	void OnCommandCheck(const char * szCommand);
	void OnCommandCancel(const char * szCommand);
	
protected:
	virtual void OnShowDialog();
};

// 触发技能
class CDlgSkillActive : public CDlgBase 
{
	AUI_DECLARE_EVENT_MAP();
public:
	CDlgSkillActive();
	virtual ~CDlgSkillActive();
	
protected:
	virtual bool OnInitDialog();
	virtual void OnTick();

	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);

	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
private:
	AUILabel*			m_pLabTime;
	AUIImagePicture*	m_pImgSkill;

	int		m_iId;				// 技能id
	int		m_iTimeId;			// 时间信息对应技能id
	DWORD	m_dwTime;			// 起始时间
	int		m_iTotalTime;		// 持续时间
	CECSkill*	m_pSkill;
};

// 韩国每一小时弹出提示持续3s
class CDlgKrAntiwallow : public CDlgBase 
{
public:
	CDlgKrAntiwallow() {}
	virtual ~CDlgKrAntiwallow() {}
	
protected:
	virtual bool OnInitDialog();
	virtual void OnTick();
	
	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
private:
	AUILabel*			m_pLabHint;
	
	int					m_iShowTime;
	static const int	ms_iTotalTime;		// 持续时间
};

// 寻宝网点确定进入寻宝网
class CDlgTreasureEnter : public CDlgBase 
{
	AUI_DECLARE_COMMAND_MAP();
public:
	CDlgTreasureEnter() {}
	virtual ~CDlgTreasureEnter() {}
	
	void OnCommandConfirm(const char* szCommand);
	
protected:
};

// 攻击帮助界面，关闭则攻击帮助结束
class CDlgHelpAttack : public CDlgBase 
{
	AUI_DECLARE_COMMAND_MAP();
public:
	CDlgHelpAttack() {}
	virtual ~CDlgHelpAttack() {}
	
	void OnCommandCancel(const char* szCommand);
	
protected:
};

// 帮助界面选择是否继续帮助
class CDlgHelpStart : public CDlgBase 
{
	AUI_DECLARE_COMMAND_MAP();
public:
	CDlgHelpStart() {}
	virtual ~CDlgHelpStart() {}
	
	void OnCommandStart(const char* szCommand);
	void OnCommandStop(const char* szCommand);
	
protected:
};

// 不接受任何消息的界面，纯提示界面
class CDlgEventImmune : public CDlgBase 
{
	AUI_DECLARE_EVENT_MAP();
public:
	CDlgEventImmune() {}
	virtual ~CDlgEventImmune() {}
	
protected:
	
	void OnEventAllKinds(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
};

class CDlgWebHot : public CDlgBase 
{
	AUI_DECLARE_COMMAND_MAP();
public:
	CDlgWebHot() {}
	virtual ~CDlgWebHot() {}
	
public:
	
	void OnCommandOpen(const char* szCommand);
};

class CDlgHelpGraphGuide : public CDlgBase 
{
	AUI_DECLARE_COMMAND_MAP();
public:
	CDlgHelpGraphGuide() {}
	virtual ~CDlgHelpGraphGuide() {}
	
public:
	
	void OnCommandOpen(const char* szCommand);
};

class CDlgGT : public CDlgBase 
{
	AUI_DECLARE_COMMAND_MAP();
public:
	CDlgGT() {}
	virtual ~CDlgGT() {}
	
public:
	
	void OnCommand_GT(const char* szCommand);
};
class CDlgKnowsMin : public CDlgBase 
{
	AUI_DECLARE_COMMAND_MAP();
public:
	CDlgKnowsMin() {}
	virtual ~CDlgKnowsMin() {}
	
public:
	
	void OnCommandOpen(const char* szCommand);
};

