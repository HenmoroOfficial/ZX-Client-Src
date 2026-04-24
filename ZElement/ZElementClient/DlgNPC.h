// Filename	: DlgNPC.h
// Creator	: Tom Zhou
// Date		: October 11, 2005

#pragma once

#include "DlgTaskBase.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUITextArea.h"
#include "AUI\\AUIStillImageButton.h"
#include "ExpTypes.h"

class A2DSprite;

class CDlgNPC : public CDlgTaskBase  
{
	AUI_DECLARE_EVENT_MAP();
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgNPC();
	virtual ~CDlgNPC();

	void OnCommand_back(const char * szCommand);
	void OnCommand_CANCEL(const char * szCommand);
	void OnCommand_NextPage(const char* szCommand);

	void OnEventLButtonUp_Lst_Main(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	void PopupNPCDialog(talk_proc *pTalk);
	void PopupNPCDialog(const NPC_ESSENCE *pEssence);
	void PopupTaskDialog(bool bTaskNPC = false);
	void PopupTaskFinishDialog(int idTask, talk_proc *pTalk);
	void PopupTransmitDialog(const NPC_TRANSMIT_SERVICE *pService);
	bool PopupCorrespondingServiceDialog(int idFunction, int iService, void *pData);
	void AutoPopup();
	//点到npc直接弹出对应任务int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam); 

	enum
	{
		NPC_DIALOG_NULL = 0,
		NPC_DIALOG_ESSENCE,		// 打开npc窗口
		NPC_DIALOG_TALK,		// 任务对话
		NPC_DIALOG_TASK_LIST,	// 任务列表
		NPC_DIALOG_TASK_TALK,	// 任务对话
		NPC_DIALOG_TASK_AWARD,	// 没用
		NPC_DIALOG_TRANSMIT_LIST// 传输列表
	};

protected:
	PAUILABEL m_pTxt_npc;
	PAUILISTBOX m_pLst_Main;
	PAUITEXTAREA m_pTxt_Content;
	PAUISTILLIMAGEBUTTON m_pBtn_Back;
	UINT		m_nLastFinishId;
	DWORD		m_dwFinishTime;
	//点到npc直接弹出对应任务bool		m_bAutoGoto;
	//点到npc直接弹出对应任务int			m_nTaskGoto;

	bool		m_bPopFinishTask;		// 自动弹出完成任务对话框


	int         m_iWindowIndex;    //对话索引
	int         m_iCurPage;        //当前对话页数

	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	virtual bool Release();
	virtual bool DealWindowsMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool bKeyboard);

protected:
	PAUIIMAGEPICTURE	m_pImg_NpcHead;
    PAUIIMAGEPICTURE    m_pImg_Npc;

	A2DSprite	       *m_pA2DSpriteNpcHead;
	A2DSprite	       *m_pA2DSpriteNpc;
	void   RefreshTalkContent();
	void   SetNpcHeadPic();
};
