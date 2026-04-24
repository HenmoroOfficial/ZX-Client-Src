// Filename	:	DlgStorageTaskCross.h
// Creator	:	Fu Chonggang
// Date		:	Feb 25, 2011
/*	desc
	上面显示可接跨服库任务
	下面显示已接的跨服库任务
*/

#pragma once


#include "DlgTaskBase.h"
#include "AUI\\AUITreeView.h"
class AUIEditBox;
class AUIStillImageButton;
class AUILabel;
class AUITextArea;
class AUIImagePicture;
class AUIRadioButton;
class AUIComboBox;
class AUIListBox;
class AUITreeView;

extern CECGame*			g_pGame;

class CDlgStorageTaskCross : public CDlgTaskBase
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgStorageTaskCross();
	virtual ~CDlgStorageTaskCross();

	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);

	void OnEventLButtonDownStorageTask(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDown_Txt_QuestItem(WPARAM wParam, LPARAM lParam, AUIObject *pObj);	// 任务描述、所需内容
	void OnEventMouseMove_Txt_QuestItem(WPARAM wParam, LPARAM lParam, AUIObject *pObj);		// 任务描述、所需内容

	void OnCommand_CANCEL(const char * szCommand);
	void OnCommand_Accept(const char * szCommand);
	void OnCommand_TaskDaily(const char * szCommand);
	void OnCommand_Abandon(const char * szCommand);
	void OnCommand_ChangeMap(const char * szCommand);
	void OnCommand_RdoType(const char * szCommand);
	void OnCommand_Ancient(const char * szCommand);
	bool UpdateTask(int idTask = -1);

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	virtual bool Render();	
	
	void InsertTaskChildren(P_AUITREEVIEW_ITEM pRoot,
		unsigned long idTask, bool bExpand, bool bKey);

	AUITextArea *m_pTxt_Content;
	AUITextArea *m_pTxt_QuestItem;
	AUILabel *m_pTxt_BaseAward;
	AUILabel *m_pTxt_ItemAward;
	AUIImagePicture *m_pImg_Item[CDLGTASK_AWARDITEM_MAX];
	AUIStillImageButton *m_pBtn_Accept;
	AUIComboBox *m_pCombo_KfMap;
	AUILabel *m_pTxt_KfItem;
	AUIListBox *m_pLst_KfQuest;
	AUITreeView *m_pTv_KfQuestGot;
	AUIStillImageButton *m_pBtn_Abandon;
	AUIStillImageButton *m_pAncient;

	int m_idLastTask;
	int m_idSelTask;

	static const int ITEM_ID;							// 暂定使用固定物品，如有修改需要修改相应代码
	static const int CROSS_MAP_NUM;						// 跨服地图数量，库任务筛选器
};
