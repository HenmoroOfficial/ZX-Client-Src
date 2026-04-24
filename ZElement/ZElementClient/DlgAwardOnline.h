// Filename	: DlgAwardOnline.h
// Creator	: Fu Chonggang
// Date		: 2012/3/16
// Desc		: 在线奖励

#pragma once

#include "DlgBase.h"

class AUIImagePicture;
class AUILabel;
struct ONLINE_GIFT_BAG_CONFIG;

#define AWARD_ONLINE_NUM_MAIN	3	// 目前不能多于3个大礼包
#define AWARD_ONLINE_NUM_SUB	6	// 每个大礼包中的小礼包中的物品数模板最大值为16，界面是6

class CDlgAwardOnline : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgAwardOnline();
	virtual ~CDlgAwardOnline();

	void OnCommand_confirm(const char * szCommand);
	void OnEventLButtonDownImage(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

protected:
	virtual bool OnInitDialog();
	virtual bool Release();
	virtual void OnTick();

	void UpdateDisplaySub();
	void strBuildHint(ACString& strHint, ONLINE_GIFT_BAG_CONFIG* pConfig);
	ONLINE_GIFT_BAG_CONFIG* GetGiftConfig(int tid);

private:
	AUILabel		*m_pLabTimeCur;
	AUILabel		*m_pLabTime[AWARD_ONLINE_NUM_MAIN];
	AUIImagePicture	*m_pImgMain[AWARD_ONLINE_NUM_MAIN];
	AUIImagePicture	*m_pImgMainSel[AWARD_ONLINE_NUM_MAIN];
	AUIImagePicture	*m_pImgSub[AWARD_ONLINE_NUM_SUB];

	int				m_iAwardIds[AWARD_ONLINE_NUM_MAIN];
	int				m_iCurSel;				// 当前选中哪个大礼包
	int				m_iCurSmallBagIndex;	// 当前发到了第几个小礼包
};