
// 上古挑战副本中的界面

#include "DlgBase.h"
#include "AUI\\AUIStillImageButton.h" 
#include "AUI\\AUITextArea.h"
#include "AUI\\AUITreeView.h"
#include "AUI\\AUILabel.h"

namespace S2C
{
	struct ancient_raid_level_result;	
}

//上古副本mini界面

class CDlgAncientMini : public CDlgBase
{
	AUI_DECLARE_EVENT_MAP()
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgAncientMini();
	virtual ~CDlgAncientMini();
	void OnCommand_Open(const char * szCommand);
	virtual bool Tick();
protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();

};


//上古副本关卡信息界面

class CDlgAncientLevelInfo : public CDlgBase
{
	AUI_DECLARE_EVENT_MAP()
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgAncientLevelInfo();
	virtual ~CDlgAncientLevelInfo();
	virtual bool Tick();
	bool SetCurLevel(int level);
	void SetLevelInfo(const S2C::ancient_raid_level_result* p); 
	void SetMatterCnt(int level, int Cnt);
	void BuildConditions();
	int  GetCurLevel();
	void OnCommandPrev(const char* szCommand);
	void OnCommandNext(const char* szCommand);
	void OnCommandGet(const char* szCommand);
	void SetLevelState(int level, int state);
	void ClearState();
	void SetState(int level);
	void SetStartTime(int level, int starttime);
	int  GetLevelStartTime();
	void SetTimerFlag(int level, bool bFlag);
	void SetResult(int level, bool bResult);
protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();

	int m_nCurLevel;   //当前关卡
	int m_nCurPage;    //当前显示页面
	int m_nFinalStartTime;
	int m_nLevelStartTime;
	int m_iTime;
	int m_nCnt;   //采矿个数
	int m_nFinalCnt; //总关卡采矿数
	int m_nTotal;
	int m_nFinalTotal;
	int m_nFinalState;   // 总挑战状态   总挑战和分挑战可能会同时进行
	int m_nLevelState[10];   // 个关卡的挑战状态

	bool m_bFinalTimerFlag;
	bool m_bLevelTimerFlag;
	// 副本个关卡的信息
	bool final;
	bool level_result[10];
	bool final_award_deliveried;
	bool level_award_deliveried[10];

	AUIStillImageButton* m_pPrevPage;
	AUIStillImageButton* m_pNextPage;
	AUIStillImageButton* m_pBtn_Get;
	AUITextArea* m_pTxt_Content;
	AUIImagePicture* m_pImg_Item;
	AUITreeView* m_pTv_Conditions;
	AUILabel* m_pLabel_Name;
	AUILabel* m_pLabel_Stage;
	P_AUITREEVIEW_ITEM count;
	P_AUITREEVIEW_ITEM time;
	P_AUITREEVIEW_ITEM Item;
};


class CDlgAncientTime : public CDlgBase  
{
public:
	CDlgAncientTime();
	virtual ~CDlgAncientTime();
	void SetTime(int nTime);
	int GetTime();
protected:
	virtual bool OnInitDialog();
	virtual void OnTick();
	int m_nLeftTime;
	int m_nTime;
	AUILabel *m_pLabTime;
};
