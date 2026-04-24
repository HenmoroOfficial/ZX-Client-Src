/********************************************************************
	created:	2012/10/17
	file name:	DlgRemedyMetempsychosis.h 
	author:		zhuyan
	
	purpose:		
*********************************************************************/

#pragma once

#include "DlgBase.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUILabel.h"
#include "hashmap.h"

class CECIvtrItem;


class DlgRemedyMetempsychosis : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP();
	AUI_DECLARE_COMMAND_MAP()
public:
	DlgRemedyMetempsychosis();
	virtual ~DlgRemedyMetempsychosis();
	typedef abase::hash_map<int, int>	LevelMap;
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnHideDialog();
	void OnCommand_Ok(const char * szCommand);	// 开始
	void SetRemedyMetempsychosisItem(CECIvtrItem* pIvtrSrc,int nIndex);
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);	// 点击配方

	int CheckRemedyMetempsychosisItem(int nItemIDm,int nNum);
	void UpdataRemedyMetempsychosisItem();
	
	void SetLevel(int);
	int GetLevel();
	void SetClass(int);
	int GetClass();
private:
	LevelMap		m_mapLevel;
	CECIvtrItem* m_pItem;
	int m_nItemIndex;
	int m_nLevel;
	int m_nClass;

	PAUILABEL m_pTxt_Now;
	PAUILABEL m_pTxt_Existing;
};