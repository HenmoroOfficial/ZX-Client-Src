// Filename	:	DlgClassmateManage.h
// Creator	:	Fu Chonggang
// Date		:	May 17, 2010

#pragma once


#include "DlgBase.h"
class AUIEditBox;
class AUIImagePicture;
class AUILabel;
class AUIStillImageButton;
class AUIListBox;
class AUITextArea;
class AUIComboBox;
class A2DSprite;

class CDlgClassmateManage : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
	enum{ NUM_PER_PAGE = 20};

public:
	CDlgClassmateManage();
	virtual ~CDlgClassmateManage();

	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	virtual bool Release();
	
	void OnEventRButtonUp(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventLButtonUp(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	void OnCommand_CANCEL(const char * szCommand);
	void OnCommand_help(const char * szCommand);
	void OnCommand_sortname(const char * szCommand);
	void OnCommand_sortgender(const char * szCommand);
	void OnCommand_sortlevel(const char * szCommand);
	void OnCommand_sortprof(const char * szCommand);
	void OnCommand_timein(const char * szCommand);
	void OnCommand_timeout(const char * szCommand);
	void OnCommand_sortline(const char * szCommand);

	void OnCommand_top(const char * szCommand);
	void OnCommand_prec(const char * szCommand);
	void OnCommand_next(const char * szCommand);
	void OnCommand_bottom(const char * szCommand);
	void OnCommand_refresh(const char * szCommand);
	void OnCommand_GetAwardExp(const char * szCommand);
	void OnCommand_GetAward(const char * szCommand);
	void OnCommand_Family(const char * szCommand);
	void OnCommand_Guild(const char * szCommand);
	void OnCommand_GiveTask(const char * szCommand);
	
	void OnCommand_ShowType(const char * szCommand);

	void BuildList();
	void BuildClassMateList();
	void SetLastRefreshTime(DWORD dwTime);

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	virtual bool Render();

	AUIEditBox *m_pShape12;
	AUIImagePicture *m_pImg_Title;
	AUILabel *m_pTxt_ClassName;
	AUIStillImageButton *m_pBtn_Help;
	AUIStillImageButton *m_pBtn_SortName;
	AUIStillImageButton *m_pBtn_SortLevel;
	AUIStillImageButton *m_pBtn_SortProf;
	AUIStillImageButton *m_pBtn_SortOL;
	AUIStillImageButton *m_pBtn_Timein;
	AUIStillImageButton *m_pBtn_SortJob;
	AUIListBox *m_pList_Member;
	AUILabel *m_pLab_PplCap;
	AUILabel *m_pTxt_PplCap;
	AUILabel *m_pLab_PplOnline;
	AUILabel *m_pTxt_PplOnline;
	AUILabel *m_pTxt_PplGrad;
	AUITextArea *m_pTxt_Message;
	AUIStillImageButton *m_pBtn_GuildInvite;
	AUILabel *m_pTxt_DeleteTime;
	AUIStillImageButton *m_pBtn_Timeout;
	AUIComboBox *m_pCombo_Display;
	AUIStillImageButton *m_pBtn_Top;
	AUIStillImageButton *m_pBtn_Prec;
	AUIStillImageButton *m_pBtn_Next;
	AUIStillImageButton *m_pBtn_Bottom;
	AUIStillImageButton *m_pBtn_Refresh;
	AUIStillImageButton *m_pBtn_Award;
	AUIStillImageButton *m_pBtn_AwardExp;
	AUIStillImageButton *m_pBtn_GiveTask;
	
	AUILabel *m_pLab_Developments;
	AUILabel *m_pLab_History;
	AUIListBox *m_pList_History;
	AUILabel *m_pLab_Point;
	AUILabel *m_pLab_AwardExp;

	int m_nSortMode;
	bool m_bFirstOpen;
	DWORD m_dwLastRefreshTime;
	DWORD m_dwLastGiveTaskTime;
	DWORD m_dwLastGiveAwardTime;
	int m_nPage;
	int m_nSuitAwardIndex;
	int m_nGradNum;
	abase::vector<A2DSprite*> m_vecImageList;
	A2DSprite* m_pImageList;
	
	enum
	{
		SORT_BY_ORDER,
		SORT_BY_NAME,
		SORT_BY_GENDER,
		SORT_BY_LEVEL,
		SORT_BY_PROFESSION,
		SORT_BY_TIMEIN,
		SORT_BY_TIMEOUT,
		SORT_BY_ONLINE,
	};
};
