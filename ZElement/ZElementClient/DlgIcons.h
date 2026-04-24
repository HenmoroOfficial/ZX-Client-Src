// Filename	: DlgIcons.cpp
// Creator	: Fu Chonggang
// Date		: 2013/4/22
// Desc		: minimap下面小按钮展开收起

#pragma once

#include "DlgBase.h"
#include "vector.h"

class CDlgIcons : public CDlgBase  
{
	struct DlgPos 
	{
		PAUIDIALOG pDlg;
		int x;				// pDlg展开以后x位置
	};
	AUI_DECLARE_COMMAND_MAP();
public:
	CDlgIcons() : m_dwActionTime(0) {}
	virtual ~CDlgIcons() {}
	
	void OnCommandMinimize(const char* szCommand);

	void BuildValidIcons();

protected:	
	virtual void OnTick();

private:
	abase::vector<DlgPos>	m_vecDlgs;	// 展开需要显示的所有按钮

	bool			m_bAction;
	DWORD			m_dwActionTime;			// 
	static const int	m_isTime;		// 按钮收起展开所用时间(ms)
};