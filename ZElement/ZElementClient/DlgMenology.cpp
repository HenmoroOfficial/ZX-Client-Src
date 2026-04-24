// Filename	:	DlgMenology.cpp
// Creator	:	Fu Chonggang
// Date		:	Sep 2, 2010

#include "DlgMenology.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIRadioButton.h"
#include "AUI\\AUITreeView.h"
#include "AUI\\AUIDef.h"
#include "DlgCalendar.h"
#include "EC_Calendar.h"
#include "EC_GameUIMan.h"
#include "EC_Menology.h"
#include "EC_Game.h"
#include "EC_GameUIMisc.h"
#include "A2DSprite.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgMenology, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",				OnCommand_CANCEL)
AUI_ON_COMMAND("Btn_Close",				OnCommand_CANCEL)
AUI_ON_COMMAND("Btn_TaskDaily",			OnCommand_taskdaily)
AUI_ON_COMMAND("Btn_Help",				OnCommand_help)
AUI_ON_COMMAND("Btn_MonthPrev",			OnCommand_monthprev)
AUI_ON_COMMAND("Btn_MonthNext",			OnCommand_monthnext)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgMenology, CDlgBase)

AUI_ON_EVENT("Img_ItemNo*",			WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("Img_Item*",			WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("Img_ItemLace*",		WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("Img_ItemHover*",		WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("Lab_Item*",			WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("Lab_ItemName*",		WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("Img_ItemNo*",			WM_MOUSEMOVE,		OnEventMouseMove)
AUI_ON_EVENT("Img_Item*",			WM_MOUSEMOVE,		OnEventMouseMove)
AUI_ON_EVENT("Img_ItemLace*",		WM_MOUSEMOVE,		OnEventMouseMove)
AUI_ON_EVENT("Img_ItemHover*",		WM_MOUSEMOVE,		OnEventMouseMove)
AUI_ON_EVENT("Lab_Item*",			WM_MOUSEMOVE,		OnEventMouseMove)
AUI_ON_EVENT("Lab_ItemName*",		WM_MOUSEMOVE,		OnEventMouseMove)
AUI_ON_EVENT("*",					WM_MOUSEWHEEL,		OnEventWheel)
AUI_ON_EVENT("NULL",				WM_MOUSEWHEEL,		OnEventWheel)

AUI_END_EVENT_MAP()

static const ACString s_strHint(_AL("^DIALOG:Win_Menology_Expand"));

CDlgMenology::CDlgMenology()
{
}

CDlgMenology::~CDlgMenology()
{
}

bool CDlgMenology::Release()
{
	delete m_pCalendar;
	A3DRELEASE(m_pA2DSpriteToday);
	A3DRELEASE(m_pA2DSpriteSelect);
	return CDlgBase::Release();
}

bool CDlgMenology::OnInitDialog()
{
	m_pLab_Year = (AUILabel *)GetDlgItem("Lab_Year");
	m_pLab_Month = (AUILabel *)GetDlgItem("Lab_Month");
	AString strName;
	
	for (int i=0; i<eNumPerPage; i++)
	{
		strName.Format("Img_ItemNo%d", i+1);
		m_pImg_ItemNo[i] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		strName.Format("Img_Item%d", i+1);
		m_pImg_Item[i] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		strName.Format("Img_ItemLace%d", i+1);
		m_pImg_ItemLace[i] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		strName.Format("Img_ItemHover%d", i+1);
		m_pImg_ItemHover[i] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		strName.Format("Lab_Item%d", i+1);
		m_pLab_Item[i] = (PAUILABEL)GetDlgItem(strName);
		m_pLab_Item[i]->SetAcceptMouseMessage(false);
		strName.Format("Lab_ItemName%d_3", i+1);
		m_pLab_ItemName[i][0] = (PAUILABEL)GetDlgItem(strName);
		m_pLab_ItemName[i][0]->SetAcceptMouseMessage(false);
		strName.Format("Lab_ItemName%d_2", i+1);
		m_pLab_ItemName[i][1] = (PAUILABEL)GetDlgItem(strName);
		m_pLab_ItemName[i][1]->SetAcceptMouseMessage(false);
		strName.Format("Lab_ItemName%d_1", i+1);
		m_pLab_ItemName[i][2] = (PAUILABEL)GetDlgItem(strName);
		m_pLab_ItemName[i][2]->SetAcceptMouseMessage(false);
		
		m_pImg_ItemNo[i]->SetHint(s_strHint);
		m_pImg_Item[i]->SetHint(s_strHint);
		m_pImg_ItemLace[i]->SetHint(s_strHint);
		m_pImg_ItemHover[i]->Show(false);
		m_pLab_Item[i]->SetHint(s_strHint);
		m_pLab_ItemName[i][0]->SetHint(s_strHint);
		m_pLab_ItemName[i][1]->SetHint(s_strHint);
		m_pLab_ItemName[i][2]->SetHint(s_strHint);
	}
	m_pCalendar = new CECCalendar;
	m_nDaySel = 0;
	m_pA2DSpriteToday = new A2DSprite;
	if (!m_pA2DSpriteToday->Init(m_pA3DDevice, "icon\\Menology_Today.tga", AUI_COLORKEY))
	{
		AUI_ReportError(__LINE__, "failed to open icon\\Menology_Today.tga");
	}
	
	m_pA2DSpriteSelect = new A2DSprite;
	if (!m_pA2DSpriteSelect->Init(m_pA3DDevice, "frame\\frame19.tga", AUI_COLORKEY))
	{
		AUI_ReportError(__LINE__, "failed to open frame\\frame1.tga");
	}
	m_bUpdateHint = false;

	return true;
}

void CDlgMenology::OnShowDialog()
{
	m_pCalendar->m_iYear = GetGame()->GetServerLocalTime().tm_year;
	m_pCalendar->m_iMonth = GetGame()->GetServerLocalTime().tm_mon;
	m_pCalendar->m_iDay = GetGame()->GetServerLocalTime().tm_mday;
	m_nDaySel = m_pCalendar->m_iDay;
	_LoadFromLua();
	Update();
	m_bUpdateHint = true;
}

void CDlgMenology::_LoadFromLua()
{
	m_mapPriority.clear();
	// 只有这3中在此界面显示
	MenologyInfoVector vecInfo = CECMenology::GetInstance()->GetInfo(CECMenology::TYPE_RECOMMAND);
	MenologyInfoVector::iterator it = vecInfo.begin();
	for ( ; it!= vecInfo.end(); ++it)
	{
		if (it->bMenology)
			m_mapPriority.insert(std::make_pair(it->iMenologyLevel, *it));
	}
	vecInfo = CECMenology::GetInstance()->GetInfo(CECMenology::TYPE_BATTLEFIELD);
	it = vecInfo.begin();
	for ( ; it!= vecInfo.end(); ++it)
	{
		if (it->bMenology)
			m_mapPriority.insert(std::make_pair(it->iMenologyLevel, *it));
	}
	vecInfo = CECMenology::GetInstance()->GetInfo(CECMenology::TYPE_FESTIVAL);
	it = vecInfo.begin();
	for ( ; it!= vecInfo.end(); ++it)
	{
		m_mapPriority.insert(std::make_pair(it->iMenologyLevel, *it));
	}
}

void CDlgMenology::OnTick()
{
	if (m_bUpdateHint)
	{
		PAUIDIALOG pDlgMouseOn;
		PAUIOBJECT pObjMouseOn;
		
		m_pAUIManager->GetMouseOn(&pDlgMouseOn, &pObjMouseOn);
		if(pDlgMouseOn==this&&pObjMouseOn)
			OnEventMouseMove(0, 0, pObjMouseOn);
		m_bUpdateHint = false;
	}
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_DailyPrompPopup");
	if (pDlg->IsShow())
	{
		pDlg->AlignTo(this, AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT, AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_BOTTOM);
	}
}

void CDlgMenology::Update()
{
	int month = m_pCalendar->m_iMonth;
	int year  = m_pCalendar->year();
	int iStartDayInMonth = m_pCalendar->CalWeekStart();	// 0-6 0-日
	int iday;
	int i;
	ACString strText;
	strText.Format(_AL("%d"), year);

	// 显示年月
	m_pLab_Year->SetText(strText);
	m_pLab_Month->SetText(GetGameUIMan()->GetStringFromTable(7111+month));

	//今天高亮显示，lay2 今天 lay1 选中
	for ( i=0; i<eNumPerPage; i++ )
	{
		m_pImg_ItemNo[i]->SetCover(NULL, 0, 1);
		m_pImg_Item[i]->SetCover(NULL, 0, 1);
		m_pImg_ItemNo[i]->SetCover(NULL, 0, 2);
		m_pImg_Item[i]->SetCover(NULL, 0, 2);
	}
	CECCalendar calToday;	//default constructor
	CheckRadioButton(1, iStartDayInMonth+m_pCalendar->m_iDay);
	if (calToday==*m_pCalendar)
	{
		m_pImg_ItemNo[iStartDayInMonth+m_pCalendar->m_iDay-1]->SetCover(m_pA2DSpriteToday, 0, 2);
		m_pImg_Item[iStartDayInMonth+m_pCalendar->m_iDay-1]->SetCover(m_pA2DSpriteToday, 0, 2);
	}
	if(m_nDaySel>0)
	{
		m_pImg_ItemNo[iStartDayInMonth+m_nDaySel-1]->SetCover(m_pA2DSpriteSelect, 0, 1);
		m_pImg_Item[iStartDayInMonth+m_nDaySel-1]->SetCover(m_pA2DSpriteSelect, 0, 1);
	}
	
	// 初始化，显示日期
	// last month
	if (0==month)
	{
		iday = CECCalendar::Days(year-1, CECCalendar::DECEMBER);
	}
	else
	{
		iday = CECCalendar::Days(year, month-1);
	}
	for (i=iStartDayInMonth-1; i>=0; i--)
	{
		m_pImg_ItemNo[i]->Show(true);
		m_pImg_Item[i]->Show(false);
		m_pImg_ItemLace[i]->Show(false);
		strText.Format(_AL("%d"), iday--);
		m_pLab_Item[i]->SetText(strText);
	}
	// this month
	iday = 1;
	for (i=iStartDayInMonth; i<CECCalendar::Days(year, month)+iStartDayInMonth; i++)
	{
		m_pImg_ItemNo[i]->Show(true);
		m_pImg_Item[i]->Show(false);
		m_pImg_ItemLace[i]->Show(false);
		strText.Format(_AL("%d"), iday++);
		m_pLab_Item[i]->SetText(strText);
	}
	// after month
	iday = 1;
	for (i=iStartDayInMonth+CECCalendar::Days(year, month); i<eNumPerPage; i++)
	{
		m_pImg_ItemNo[i]->Show(true);
		m_pImg_Item[i]->Show(false);
		m_pImg_ItemLace[i]->Show(false);
		strText.Format(_AL("%d"), iday++);
		m_pLab_Item[i]->SetText(strText);
	}

	//添加信息
	//优先节假日活动图标
	CECCalendar calStart, calEnd;
	for (PriorityMap::iterator itMap = m_mapPriority.begin(); itMap!= m_mapPriority.end(); ++itMap)
	{
		if (itMap->second.iType==CECMenology::TYPE_FESTIVAL)
		{
			UINT nSizeTime = itMap->second.vecTimes.size();
			for (UINT i=0; i<nSizeTime; i++)
			{
				calStart = itMap->second.vecTimes[i].tmStart;
				calEnd = itMap->second.vecTimes[i].tmEnd;
				int startIndex = _DayInCurPage(calStart);
				int endIndex = _DayInCurPage(calEnd);
				if ( startIndex>=0&&endIndex>=0 )
				{
					if (m_pImg_ItemNo[startIndex]->IsShow()&&(!itMap->second.bWeekly||itMap->second.bDays[startIndex%7]))
					{
						m_pImg_ItemNo[startIndex]->Show(false);
						m_pImg_Item[startIndex]->Show(true);
						SetImageFileProperty(m_pImg_Item[startIndex], itMap->second.strIcon3);
					}
					if (m_pImg_ItemNo[endIndex]->IsShow()&&(!itMap->second.bWeekly||itMap->second.bDays[endIndex%7]))
					{
						m_pImg_ItemNo[endIndex]->Show(false);
						m_pImg_Item[endIndex]->Show(true);
						SetImageFileProperty(m_pImg_Item[endIndex], itMap->second.strIcon3);
					}
					for (int i=startIndex; i<=endIndex; i++)
					{
						if (!itMap->second.bWeekly||itMap->second.bDays[i%7])
						{
							m_pImg_ItemLace[i]->Show(true);
							SetImageFileProperty(m_pImg_ItemLace[i], itMap->second.strIcon2);
						}
					}
				}
				else if (startIndex>=0)
				{
					if (m_pImg_ItemNo[startIndex]->IsShow()&&(!itMap->second.bWeekly||itMap->second.bDays[startIndex%7]))
					{
						m_pImg_ItemNo[startIndex]->Show(false);
						m_pImg_Item[startIndex]->Show(true);
						SetImageFileProperty(m_pImg_Item[startIndex], itMap->second.strIcon3);
					}
					for (int i=startIndex; i<eNumPerPage; i++)
					{
						if (!itMap->second.bWeekly||itMap->second.bDays[i%7])
						{
							m_pImg_ItemLace[i]->Show(true);
							SetImageFileProperty(m_pImg_ItemLace[i], itMap->second.strIcon2);
						}
					}
				}
				else if (endIndex>=0)
				{
					if (m_pImg_ItemNo[endIndex]->IsShow()&&(!itMap->second.bWeekly||itMap->second.bDays[endIndex%7]))
					{
						m_pImg_ItemNo[endIndex]->Show(false);
						m_pImg_Item[endIndex]->Show(true);
						SetImageFileProperty(m_pImg_Item[endIndex], itMap->second.strIcon3);
					}
					for (int i=0; i<=endIndex; i++)
					{
						if (!itMap->second.bWeekly||itMap->second.bDays[i%7])
						{
							m_pImg_ItemLace[i]->Show(true);
							SetImageFileProperty(m_pImg_ItemLace[i], itMap->second.strIcon2);
						}
					}
				}
			}
		}
		else
		{
			for ( i=0; i<eNumPerPage; i++ )
			{
				if (m_pImg_ItemNo[i]->IsShow()&&itMap->second.bDays[i%7])
				{
					m_pImg_ItemNo[i]->Show(false);
					m_pImg_Item[i]->Show(true);
					SetImageFileProperty(m_pImg_Item[i], itMap->second.strIcon3);
				}
			}
		}
	}

	// 文字及悬浮
	CECCalendar cal = *m_pCalendar;
	cal.m_iDay = 1;
	cal-=(iStartDayInMonth+1);
	ACString strName;
	ACString strTime;
	AString strIcon;
	for ( i=0; i<eNumPerPage; i++ )
	{
		int numAdded = 0;
		int numHint  = 0;
		strName.Empty();
		strTime.Empty();
		strIcon.Empty();
		m_pLab_ItemName[i][0]->SetText(_AL(""));
		m_pLab_ItemName[i][1]->SetText(_AL(""));
		m_pLab_ItemName[i][2]->SetText(_AL(""));
		++cal;
		for (PriorityMap::iterator itMap = m_mapPriority.begin(); itMap!= m_mapPriority.end(); ++itMap)
		{
			if (itMap->second.iType==CECMenology::TYPE_FESTIVAL)
			{
				if (!_TodayIsFestival(itMap->second, &cal))
					continue;
				else if (itMap->second.bWeekly&&!itMap->second.bDays[i%7])
					continue;
			}
			else if (!itMap->second.bDays[i%7])
				continue;
			if ( numAdded<3 )
			{
				for (int iAdd=numAdded; iAdd>0; iAdd--)
					m_pLab_ItemName[i][iAdd]->SetText(m_pLab_ItemName[i][iAdd-1]->GetText());
				m_pLab_ItemName[i][0]->SetText(itMap->second.strName);
				numAdded++;
			}
			//超过20条不显示
			if (numHint++<20)
			{
				strName += itMap->second.strName;
				strTime += itMap->second.strTime;
				if (strIcon.IsEmpty())
					strIcon = itMap->second.strIcon2;
				if (numHint<20)
				{
					strName += _AL("\r");
					strTime += _AL("\r");
				}
			}
			if (numAdded>=3&&numHint>=20)
				break;
		}
		if (strName.GetLength()>0&&strName[strName.GetLength()-1]==_AL('\r'))
			strName.CutRight(1);
		if (strTime.GetLength()>0&&strTime[strTime.GetLength()-1]==_AL('\r'))
			strTime.CutRight(1);
		m_strName[i] = strName;
		m_strTime[i] = strTime;
		m_pImg_ItemNo[i]->SetHint(s_strHint);
		m_pImg_Item[i]->SetHint(s_strHint);
		m_pImg_ItemLace[i]->SetHint(s_strHint);
		m_pLab_Item[i]->SetHint(s_strHint);
		m_pLab_ItemName[i][0]->SetHint(s_strHint);
		m_pLab_ItemName[i][1]->SetHint(s_strHint);
		m_pLab_ItemName[i][2]->SetHint(s_strHint);
		if (m_pImg_ItemNo[i]->IsShow())
		{
			if (numAdded==0)
			{
				m_pImg_Item[i]->Show(false);
			}
			else if (!strIcon.IsEmpty())
			{
				m_pImg_Item[i]->Show(false);
				SetImageFileProperty(m_pImg_Item[i], strIcon);
			}
			if (!m_pImg_ItemLace[i]->IsShow())
			{
				m_pImg_ItemNo[i]->SetHint(_AL(""));
				m_pImg_Item[i]->SetHint(_AL(""));
				m_pImg_ItemLace[i]->SetHint(_AL(""));
				m_pLab_Item[i]->SetHint(_AL(""));
				m_pLab_ItemName[i][0]->SetHint(_AL(""));
				m_pLab_ItemName[i][1]->SetHint(_AL(""));
				m_pLab_ItemName[i][2]->SetHint(_AL(""));
			}
		}
	}
	// 颜色信息，非当前页暗掉
	for (i=iStartDayInMonth-1; i>=0; i--)
	{
		m_pImg_ItemNo[i]->SetColor(A3DCOLORRGB(100, 100, 100));
		m_pImg_Item[i]->SetColor(A3DCOLORRGB(100, 100, 100));
		m_pImg_ItemLace[i]->SetColor(A3DCOLORRGB(100, 100, 100));
	}
	
	iday = 1;
	for (i=iStartDayInMonth; i<CECCalendar::Days(year, month)+iStartDayInMonth; i++)
	{
		m_pImg_ItemNo[i]->SetColor(A3DCOLORRGB(255, 255, 255));
		m_pImg_Item[i]->SetColor(A3DCOLORRGB(255, 255, 255));
		m_pImg_ItemLace[i]->SetColor(A3DCOLORRGB(255, 255, 255));
	}
	
	iday = 1;
	for (i=iStartDayInMonth+CECCalendar::Days(year, month); i<eNumPerPage; i++)
	{
		m_pImg_ItemNo[i]->SetColor(A3DCOLORRGB(100, 100, 100));
		m_pImg_Item[i]->SetColor(A3DCOLORRGB(100, 100, 100));
		m_pImg_ItemLace[i]->SetColor(A3DCOLORRGB(100, 100, 100));
	}
}

int CDlgMenology::_DayInCurPage(CECCalendar cal)
{
	int iStartDayInMonth = m_pCalendar->CalWeekStart();	// 0-6 0-日
	CECCalendar calBegin = *m_pCalendar;
	calBegin.m_iDay = 1;
	calBegin -= iStartDayInMonth;
	CECCalendar calEnd;
	calEnd = calBegin+41;

	if ( cal>=calBegin&&cal<=calEnd )
		return cal-calBegin;

	return -1;
}

bool CDlgMenology::Render()
{
	if( !CDlgBase::Render() )
		return false;
	return true;
}

int CDlgMenology::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	return 1;
}

void CDlgMenology::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
	GetGameUIMan()->GetDialog("Win_DailyPrompPopup")->Show(false);
}

void CDlgMenology::OnCommand_taskdaily(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_DailyPromp_Today");
	if (pDlg->IsShow())
	{
		pDlg->Show(false);
	}
	else
	{
		OnCommand_CANCEL("");
		pDlg->Show(true);
	}
}

void CDlgMenology::OnCommand_help(const char * szCommand)
{
}

void CDlgMenology::OnCommand_monthprev(const char * szCommand)
{
	m_pCalendar->SubMonth();
	Update();
}

void CDlgMenology::OnCommand_monthnext(const char * szCommand)
{
	m_pCalendar->AddMonth();
	Update();
}

void CDlgMenology::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_DailyPrompPopup");
	PAUITREEVIEW pTree = (PAUITREEVIEW)pDlg->GetDlgItem("Tv_Questlist");
	pTree->SetAutoWrap(true);
	pTree->DeleteAllItems();
	int index = _CheckIndexMouseOn(pObj);
	if (index<0)
		return;
	
	for ( int i=0; i<eNumPerPage; i++ )
	{
		m_pImg_ItemNo[i]->SetCover(NULL, 0, 1);
		m_pImg_Item[i]->SetCover(NULL, 0, 1);
	}
	m_pImg_ItemNo[index]->SetCover(m_pA2DSpriteSelect, 0, 1);
	m_pImg_Item[index]->SetCover(m_pA2DSpriteSelect, 0, 1);
	int iStartDayInMonth = m_pCalendar->CalWeekStart();	// 0-6 0-日
	CECCalendar cal = *m_pCalendar;
	cal.m_iDay = 1;
	cal -= iStartDayInMonth;
	cal += index;
	m_nDaySel = cal.m_iDay;
	static const ACString colorTree[2] = { _AL("^ffcb4a"), _AL("^ffffff")};
	for (PriorityMap::iterator itMap = m_mapPriority.begin(); itMap!= m_mapPriority.end(); ++itMap)
	{
		if (itMap->second.iType==CECMenology::TYPE_FESTIVAL)
		{
			if (_TodayIsFestival(itMap->second, &cal))
			{
				if (!itMap->second.bWeekly||itMap->second.bDays[index%7])
				{
					P_AUITREEVIEW_ITEM pItem = pTree->InsertItem(colorTree[0]+itMap->second.strName);
					pTree->Expand(pItem, AUITREEVIEW_EXPAND_EXPAND);
					pTree->InsertItem(colorTree[1]+itMap->second.strDesc, pItem);
				}
			}
		}
		else if (itMap->second.bDays[index%7])
		{
			P_AUITREEVIEW_ITEM pItem = pTree->InsertItem(colorTree[0]+itMap->second.strName);
			pTree->Expand(pItem, AUITREEVIEW_EXPAND_EXPAND);
			pTree->InsertItem(colorTree[1]+itMap->second.strDesc, pItem);
		}
	}
	CheckRadioButton(1, index%7+1);
	if (pTree->GetCount()==0)
		pDlg->Show(false);
	else if (!pDlg->IsShow())
	{
		pDlg->Show(true);
		pDlg->AlignTo(this, AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT, AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_BOTTOM);
	}
}

void CDlgMenology::OnEventMouseMove(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	// adjust hint
	static const int s_edgeSpace = 5;
	if (_CheckIndexMouseOn(pObj)>=0)
	{
		PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Menology_Expand");
		PAUILABEL pLabDate = (PAUILABEL)pDlg->GetDlgItem("Lab_Date");
		PAUILABEL pLabName = (PAUILABEL)pDlg->GetDlgItem("Lab_Name");
		PAUILABEL pLabTime = (PAUILABEL)pDlg->GetDlgItem("Lab_Time");
		int yBase = pLabDate->GetPos(true).y+pLabDate->GetSize().cy;
		int index = _CheckIndexMouseOn(pObj);
		ACString strText;
		
		CECCalendar cal = *m_pCalendar;
		cal.m_iDay = 1;
		int iStartDayInMonth = m_pCalendar->CalWeekStart();
		cal-=iStartDayInMonth;
		cal+=index;
		strText.Format(GetGameUIMan()->GetStringFromTable(7800), cal.year(),cal.month(),cal.day(),GetGameUIMan()->GetStringFromTable(9890+index%7));
		pLabDate->SetText(strText);
		int nWidthName, nHeightName, nLinesName;
		int nWidthDate;
		A3DFTFont *pFont = pLabDate->GetFont();
		AUI_GetTextRect(pFont, strText, nWidthDate, nHeightName, nLinesName, 3);
		pLabName->SetText(m_strName[index]);
		pLabTime->SetText(m_strTime[index]);
		pFont = pLabName->GetFont();
		AUI_GetTextRect(pFont, m_strName[index], nWidthName, nHeightName, nLinesName, 3);
		A3DFTFont *pFontTime = pLabTime->GetFont();
		int nWidthTime, nHeightTime, nLinesTime;
		AUI_GetTextRect(pFontTime, m_strTime[index], nWidthTime, nHeightTime, nLinesTime, 3);
		pLabName->SetPos(s_edgeSpace, yBase+s_edgeSpace);
		pLabName->SetSize(nWidthName, nHeightName);
		pLabTime->SetPos(s_edgeSpace*3+nWidthName+nWidthTime, yBase+s_edgeSpace);
		pDlg->SetSize(a_Max(s_edgeSpace*4+nWidthName+nWidthTime, nWidthDate+s_edgeSpace*4), a_Max(nHeightName, nHeightTime)+s_edgeSpace*2+yBase);
	}
}

void CDlgMenology::OnEventWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	int zDelta = (short)HIWORD(wParam);
	if (zDelta>0)
		OnCommand_monthprev("");
	else
		OnCommand_monthnext("");
}

int CDlgMenology::_CheckIndexMouseOn(AUIObject *pObj)
{
	AString strName = pObj->GetName();
	int index = -1;
	if (strName.Find("Img_ItemNo")>=0)
	{
		strName.CutLeft(strlen("Img_ItemNo"));
		index = strName.ToInt()-1;
	}
	else if (strName.Find("Img_ItemLace")>=0)
	{
		strName.CutLeft(strlen("Img_ItemLace"));
		index = strName.ToInt()-1;
	}
	else if (strName.Find("Img_ItemHover")>=0)
	{
		strName.CutLeft(strlen("Img_ItemHover"));
		index = strName.ToInt()-1;
	}
	else if (strName.Find("Img_Item")>=0)
	{
		strName.CutLeft(strlen("Img_Item"));
		index = strName.ToInt()-1;
	}
	else if (strName.Find("Lab_ItemName")>=0)
	{
		strName.CutLeft(strlen("Lab_ItemName"));
		index = strName.ToInt()-1;
	}
	else if (strName.Find("Lab_Item")>=0)
	{
		strName.CutLeft(strlen("Lab_Item"));
		index = strName.ToInt()-1;
	}
	return index;
}

bool CDlgMenology::_TodayIsFestival(MenologyInfo& info, CECCalendar* pCalendar)
{
	if (info.iType!=CECMenology::TYPE_FESTIVAL)
		return false;
	UINT nSizeTime = info.vecTimes.size();
	for (UINT i=0; i<nSizeTime; i++)
	{
		if (*pCalendar>=info.vecTimes[i].tmStart && *pCalendar<=info.vecTimes[i].tmEnd)
			return true;
	}
	return false;
}