/********************************************************************
	created:	2012/10/15
	file name:	CDlgAgentCombined.h
	author:		zhuyan
	
	purpose:		
*********************************************************************/

#pragma once

#include "DlgBase.h"
#include "AUI\\AUIImagePicture.h"


class CECIvtrItem;


class CDlgAgentCombined : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP();
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgAgentCombined();
	virtual ~CDlgAgentCombined();
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnHideDialog();
	void OnCommand_Perfusion(const char * szCommand);	// 灌注
	void SetAgentCombiendType(CECIvtrItem* pIvtrSrc,int nIndex);
	void IsAgentEffective(CECIvtrItem * pEffective,int nIndex);	// 判断是否是有效
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);	// 点击配方

	bool IsItemIDEffective(int nItemId);

	void CheckAgentCombiend();
private:

	CECIvtrItem* m_pAgentItem;
	CECIvtrItem * m_pEffective;
	int m_nSrcIndex;
	int m_nEffectiveIndex;
};