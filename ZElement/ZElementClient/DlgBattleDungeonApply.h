/**
* @File DlgBattleDungeonApply.h
* @Brief 副本报名界面
* @Author Fu Chonggang
* @Date 2011-7-25 10:18:29
* @Copyright (c) 2011 ZhuXian Studio, All Rights Reserved. 
*/
#pragma once

#include "DlgBase.h"
#include "hashmap.h"

class AUIListBox;
class AUIStillImageButton;
class AUILabel;
struct TRANSCRIPTION_CONFIG;

class CDlgBattleDungeonApply : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgBattleDungeonApply();
	virtual ~CDlgBattleDungeonApply();
	
	void OnCommandActive(const char * szCommand);
	void OnCommandApply(const char * szCommand);
	void OnCommandRefresh(const char * szCommand);
	void OnCommandClose(const char * szCommand);
	void OnLButtonDblClkList(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	
	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);

	static int GetIndexFromTmplId(int id);
protected:
	virtual void OnTick();
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual bool Release();

	bool CheckCondiction();

private:
	AUIListBox*	m_pLstRoom;
	AUIStillImageButton*	m_pBtnActive;
	AUIStillImageButton*	m_pBtnJoin;

	abase::vector<A2DSprite*>			m_vecImageList;

	static abase::hash_map<int, int> ms_tid2desc;	// 模板id对应ingame.stf里面描述
};
