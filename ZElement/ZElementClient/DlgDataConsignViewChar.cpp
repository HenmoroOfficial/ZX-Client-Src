// Filename	: DlgDataConsignViewChar.cpp
// Creator	: Fuchonggang
// Date		: 2012/03/14

#include "DlgDataConsignViewChar.h"

#include "A3DVector.h"
#include "AUI\\AUIListBox.h"

#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_Algorithm.h"
#include "consignlistrole.hpp"
#include "consignlistrole_re.hpp"


CDlgDataConsignViewChar::CDlgDataConsignViewChar()
{
	m_iCurPage		= 0;
	m_iNextPage		= 0;
	m_iLastPageCurTime = -1;

	m_iLastCashTime	= 0;
	m_iCasheValidTime	= 300000;
}

CDlgDataConsignViewChar::~CDlgDataConsignViewChar()
{
}

void CDlgDataConsignViewChar::LoadFromProtocol(void *data)
{
	ConsignListRole_Re* p = (ConsignListRole_Re*)data;	
	CharListVector charList;
	unsigned int i;
	if (p->page>=(int)m_VecCharLists.size())
	{
		for (i=m_VecCharLists.size(); i<=(unsigned int)p->page; ++i)
		{
			m_VecCharLists.push_back(charList);
		}
	}
	for (i=0; i<p->nodes.size(); ++i)
	{
		charList.push_back(p->nodes[i]);
	}
	m_VecCharLists[p->page] = charList;
	if (m_iNextPage>=(int)m_VecCharLists.size())
	{
		m_iNextPage = m_VecCharLists.size()-1;
	}
	if (p->retcode==17)
	{
		m_iLastPageCurTime = p->page;
		m_iNextPage = m_iLastPageCurTime;
	}
}

CharListVector* CDlgDataConsignViewChar::UpdateView()
{
	if ( m_iCurPage!=m_iNextPage && (int)m_VecCharLists.size()>m_iNextPage && m_VecCharLists[m_iNextPage].size()>0)
	{
		m_iCurPage = m_iNextPage;
		return &m_VecCharLists[m_iCurPage];
	}

	return 0;
}

void CDlgDataConsignViewChar::SortByProf()
{
	static int bAscent = true;
	if ( m_iCurPage>=0 && m_iCurPage<(int)m_VecCharLists.size() )
	{
		if (bAscent)
			BubbleSort(m_VecCharLists[m_iCurPage].begin(), m_VecCharLists[m_iCurPage].end(), CompareCharListByProfAscent());
		else
			BubbleSort(m_VecCharLists[m_iCurPage].begin(), m_VecCharLists[m_iCurPage].end(), CompareCharListByProfDescent());
		bAscent = !bAscent;
		
		m_iCurPage = -1;	// 刷新显示数据
	}
}

void CDlgDataConsignViewChar::SortByLevel()
{
	static int bAscent = true;
	if ( m_iCurPage>=0 && m_iCurPage<(int)m_VecCharLists.size() )
	{
		if (bAscent)
			BubbleSort(m_VecCharLists[m_iCurPage].begin(), m_VecCharLists[m_iCurPage].end(), CompareCharListByLevelAscent());
		else
			BubbleSort(m_VecCharLists[m_iCurPage].begin(), m_VecCharLists[m_iCurPage].end(), CompareCharListByLevelDescent());
		bAscent = !bAscent;
		
		m_iCurPage = -1;	// 刷新显示数据
	}	
}

void CDlgDataConsignViewChar::SortByName()
{
	static int bAscent = true;
	if ( m_iCurPage>=0 && m_iCurPage<(int)m_VecCharLists.size() )
	{
		if (bAscent)
			BubbleSort(m_VecCharLists[m_iCurPage].begin(), m_VecCharLists[m_iCurPage].end(), CompareCharListByNameAscent());
		else
			BubbleSort(m_VecCharLists[m_iCurPage].begin(), m_VecCharLists[m_iCurPage].end(), CompareCharListByNameDescent());
		bAscent = !bAscent;
		
		m_iCurPage = -1;	// 刷新显示数据
	}	
}

void CDlgDataConsignViewChar::SortByTime()
{
	static int bAscent = true;
	if ( m_iCurPage>=0 && m_iCurPage<(int)m_VecCharLists.size() )
	{
		if (bAscent)
			BubbleSort(m_VecCharLists[m_iCurPage].begin(), m_VecCharLists[m_iCurPage].end(), CompareCharListByTimeAscent());
		else
			BubbleSort(m_VecCharLists[m_iCurPage].begin(), m_VecCharLists[m_iCurPage].end(), CompareCharListByTimeDescent());
		bAscent = !bAscent;
		
		m_iCurPage = -1;	// 刷新显示数据
	}	
}

void CDlgDataConsignViewChar::SortByPrice()
{
	static int bAscent = true;
	if ( m_iCurPage>=0 && m_iCurPage<(int)m_VecCharLists.size() )
	{
		if (bAscent)
			BubbleSort(m_VecCharLists[m_iCurPage].begin(), m_VecCharLists[m_iCurPage].end(), CompareCharListByPriceAscent());
		else
			BubbleSort(m_VecCharLists[m_iCurPage].begin(), m_VecCharLists[m_iCurPage].end(), CompareCharListByPriceDescent());
		bAscent = !bAscent;
		
		m_iCurPage = -1;	// 刷新显示数据
	}	
}

void CDlgDataConsignViewChar::FirstPage()
{
	m_iNextPage = 0;
	Refresh();
}

void CDlgDataConsignViewChar::PrevPage()
{
	if (m_iCurPage>0)
	{
		m_iNextPage = m_iCurPage-1;
		Refresh();
	}
}

void CDlgDataConsignViewChar::NextPage()
{
	if(m_iCurPage != m_iLastPageCurTime) 
	{
		m_iNextPage = m_iCurPage+1;
		Refresh();
	}
}

void CDlgDataConsignViewChar::LastPage()
{
	m_iNextPage = m_iLastPageCurTime;
	Refresh();
}

void CDlgDataConsignViewChar::Refresh()
{
	if( ((int)GetTickCount()-m_iLastCashTime)>m_iCasheValidTime || // 超时
		m_iNextPage>=(int)m_VecCharLists.size() || // 请求下一页
		m_iNextPage==-1 ||	// 请求最后一页
		m_VecCharLists[m_iNextPage].size()==0)	// 中间页，没有信息
	{
		g_pGame->GetGameSession()->SendNetData(GNET::ConsignListRole(g_pGame->GetGameRun()->GetHostPlayer()->GetCharacterID(), m_iNextPage, 0));
		m_iCurPage = -1;
		m_iLastCashTime = GetTickCount();
	}
}	