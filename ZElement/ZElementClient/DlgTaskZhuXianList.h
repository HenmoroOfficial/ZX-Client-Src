// Filename	: CDlgTaskZhuXianList.h
// Creator	: Fu Chonggang
// Date		: 2011/3/31

#pragma once
#include "DlgTaskBase.h"
#include "vector.h"
#include "AUI\\AUIStillImageButton.h"
class AUIVerticalText;
class AUIRadioButton;
class AUIEditBox;
class AUIImagePicture;

extern CECGame* g_pGame;

class CDlgTaskZhuXianList : public CDlgTaskBase
{
	friend class CDlgTask;
	AUI_DECLARE_COMMAND_MAP();

	enum
	{
		TASK_PERPAGE = 10,
	};

public:
	CDlgTaskZhuXianList();
	virtual ~CDlgTaskZhuXianList();
	
	void OnCommand_Left(const char * szCommand);
	void OnCommand_Right(const char * szCommand);
	void OnCommand_Page(const char * szCommand);			// 打开某一页
	void OnCommand_TaskDaily(const char * szCommand);
	void OnCommand_RdoType(const char * szCommand);			// 切换其它任务列表
	void OnCommand_CANCEL(const char * szCommand);
	void OnCommand_Ancient(const char * szCommand);
	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);

protected:
	virtual bool OnInitDialog();
	virtual bool Release();
	virtual void OnShowDialog();
	virtual void OnTick();
	virtual bool Render();

	void UpdateDisplay();
	void UpdateTaskSuitable();
	
	int				m_iStartIndex;
	int				m_iClickPageTime;
	int				m_iClickPage;

	AUIVerticalText*	m_pVTxtName[TASK_PERPAGE];
	AUIRadioButton*		m_pRdoPage[TASK_PERPAGE];
	AUIEditBox*			m_pImgFinish[TASK_PERPAGE];
	AUIRadioButton*		m_pRdo3;
	AUIImagePicture*	m_pGfxOpen;
	PAUISTILLIMAGEBUTTON  m_pAncient;	
	static abase::vector<int> m_vecTaskSuit;
};
