/********************************************************************
	created:	20011/04/13
	file base:	DlgTabContainer
	file ext:	h
	author:		Fu Chonggang
	
	purpose:	container of dialogs to implement a tab control
*********************************************************************/

#pragma once

#include "vector.h"

class AUIDialog;

template <typename T>
class CDlgTabContainer
{
public:

	CDlgTabContainer() {}
	virtual ~CDlgTabContainer() { ms_Pages.clear(); ms_LastShow = 0; }

	void AddTabDialog(T dlg) { ms_Pages.push_back(dlg); }
	void ChangeDialog(T dlg);
	void ChangeShowStatus();		// 快捷键开关界面
	void ShowSuitDialog() { ChangeDialog(ms_LastShow); }

protected:
	static abase::vector<T> ms_Pages;
	static T ms_LastShow;
};

template <typename T>
void CDlgTabContainer<T>::ChangeDialog(T dlg) 
{
	T curShow = 0;
	UINT i;
	for (i=0; i<ms_Pages.size(); ++i)
	{
		if(ms_Pages[i]->IsShow())
		{
			curShow = ms_Pages[i];
			break;
		}
	}
	
	if(curShow)
	{
		curShow->Show(false);
		dlg->AlignTo(curShow, AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT, AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
	}
	dlg->Show(true);
	ms_LastShow = dlg;
}

template <typename T>
void CDlgTabContainer<T>::ChangeShowStatus()
{
	T curShow = 0;
	UINT i;
	for (i=0; i<ms_Pages.size(); ++i)
	{
		if(ms_Pages[i]->IsShow())
		{
			curShow = ms_Pages[i];
			break;
		}
	}
	
	if(curShow)
	{
		curShow->Show(false);
	}
	else
	{
		if(ms_LastShow)
			ms_LastShow->Show(true);
		else
			ms_Pages[0]->Show(true);
	}
}
