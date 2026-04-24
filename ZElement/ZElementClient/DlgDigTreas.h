// Filename	: DlgDigTreas.h
// Creator	: Fu Chonggang
// Date		: 2012/3/16
// Desc		: 挖宝界面
// 未开放区域：gfx_seal
// 已开放区域且可探索：gfx_normal
// 已开放区域但不可探索：Img_Normal的第三态图
// 已开放区域且已探索过：Img_Normal的第二态图

#pragma once

#include "DlgBase.h"

class AUIImagePicture;
class AUILabel;
struct SCROLL_REGION_CONFIG;

#define NormalRegionNum	12

/************************************************************************
CDlgDlgTreas 挖宝主界面       
ZhuxianExplore.xml            
************************************************************************/
class CDlgDlgTreas : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

	enum
	{
		ICONSET_INVENTORY	= 0,
		ICONSET_RANDOM		= 1,
	};

public:
	// 仿照treasure_info::Treasure_Info，因为结构中结构无法声明，直接引用文件又不好
	struct Treasure_Info
	{
		Treasure_Info(char t, char s, char l, int i){type=t; status=s; level=l; id=i; }
		char	type;				//0-无效，1-普通，2-隐藏
		char	status;				//0-无效，1-可探索，2-关闭(普通区域)，3-锁定(隐藏区域)
		char	level;				//区域等级
		int		id;					//区域模板ID
	};

	CDlgDlgTreas();
	virtual ~CDlgDlgTreas();

	void OnCommand_Refresh(const char * szCommand);
	void OnCommand_Search(const char * szCommand);
	void OnCommand_Unlock(const char * szCommand);
	
	void OnEventLButtonDownImage(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);

	void UpdateRegions();	// 刷新各区域显示
	void UpdateRegionSel();	// 更新当前选中区域信息及奖励显示

protected:
	virtual bool OnInitDialog();
	virtual bool Release();
	virtual void OnShowDialog();
	virtual void OnTick();

	void TickStars();
	void ShowRefreshGfx();
	void PopEventDialog(SCROLL_REGION_CONFIG *pCurRegin, int eventIndex);
	/*
	index = 9,10,11
	*/
	bool HideRegionExplored(int index, int id);
	bool RegionInTime(SCROLL_REGION_CONFIG *pCurRegin);

private:
	int						m_iCurIndex;		// 当前选中区域的位置pos
	SCROLL_REGION_CONFIG	*m_pCurRegin;		// 当前选中区域数据的一个指针
	AUIImagePicture			*m_pImgRegions[NormalRegionNum];
	AUILabel				*m_pLabRegionName[NormalRegionNum];
	AUILabel				*m_pLabRefreshItem;
	AUIImagePicture			*m_pImgRefresh;

	A2DSprite				*m_pRandomAward;

	// 星星闪烁
	int		m_bRefresh;
	int		m_iCurStars;	// 1 - 5
	int		m_iDstStars;	// 1 - 5
	int		m_iCurTime;
	bool	m_bRefreshSucc;

	// 数据
	int		m_iRemainDigCount;
	abase::vector<Treasure_Info>	m_vecRegionInfo;
	
	abase::vector<SCROLL_REGION_CONFIG*> m_vecRegionsHide;
	abase::vector<A2DSprite*> m_vecItemPic;
};

/************************************************************************
CDlgDlgTreasEvent 挖宝触发事件
ZhuxianExploreIssue.xml
************************************************************************/
class CDlgDlgTreasEvent : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgDlgTreasEvent();
	virtual ~CDlgDlgTreasEvent();
	void OnCommand_Confirm(const char * szCommand);
};

/************************************************************************
CDlgDlgTreasMin 挖宝最小化按钮
ZhuxianExplore_Min.xml
************************************************************************/
class CDlgDlgTreasMin : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgDlgTreasMin();
	virtual ~CDlgDlgTreasMin();
	void OnCommand_Active(const char * szCommand);
	virtual void OnTick();
};

/************************************************************************
CDlgDlgTreasIntro 挖宝第一次打开隐藏区域显示的模态对话框
ZhuxianExploreIntro.xml
************************************************************************/
class CDlgDlgTreasIntro : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgDlgTreasIntro();
	virtual ~CDlgDlgTreasIntro();
	void OnCommand_Confirm(const char * szCommand);
	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);

protected:
	void DisplayOneIntro();
private:
	SCROLL_REGION_CONFIG* m_vecConfigToShow[3];
};
