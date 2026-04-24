// Filename	:	DlgMenology.h
// Creator	:	Fu Chonggang
// Date		:	Sep 2, 2010

#pragma once


#include "DlgBase.h"
#include "EC_Menology.h"
#include <map>

class AUILabel;
class AUIStillImageButton;
class AUIImagePicture;
class AUIRadioButton;
class CECCalendar;
class A2DSprite;

class CDlgMenology : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
	enum
	{
		eNumPerPage = 42,
	};

public:
	CDlgMenology();
	virtual ~CDlgMenology();

	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	virtual bool Release();

	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventMouseMove(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	void OnCommand_CANCEL(const char * szCommand);
	void OnCommand_taskdaily(const char * szCommand);
	void OnCommand_help(const char * szCommand);
	void OnCommand_monthprev(const char * szCommand);
	void OnCommand_monthnext(const char * szCommand);

	typedef std::multimap<int, MenologyInfo, std::greater<int> > PriorityMap;

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	virtual bool Render();
	void Update();
	int _CheckIndexMouseOn(AUIObject *pObj);
	void _LoadFromLua();
	
	AUILabel *m_pLab_Year;
	AUILabel *m_pLab_Month;
 	AUIImagePicture *m_pImg_ItemNo[eNumPerPage];		//没有信息显示		layer 1 选中 layer 2 今天
 	AUIImagePicture *m_pImg_Item[eNumPerPage];			//item
 	AUIImagePicture *m_pImg_ItemLace[eNumPerPage];		//花边
 	AUIImagePicture *m_pImg_ItemHover[eNumPerPage];		//没用，策划既然做了，也就没有删
	AUILabel *m_pLab_Item[eNumPerPage];
	AUILabel *m_pLab_ItemName[eNumPerPage][3];
	ACString m_strName[eNumPerPage];
	ACString m_strTime[eNumPerPage];

	A2DSprite* m_pA2DSpriteToday;
	A2DSprite* m_pA2DSpriteSelect;
	CECCalendar* m_pCalendar;
	int			m_nDaySel;
	PriorityMap m_mapPriority;
	bool		m_bUpdateHint;

	int _DayInCurPage(CECCalendar cal);	// cal是否在日历当前页显示，返回对应index，不显示返回-1
	bool _TodayIsFestival(MenologyInfo& info, CECCalendar* pCalendar);
};
