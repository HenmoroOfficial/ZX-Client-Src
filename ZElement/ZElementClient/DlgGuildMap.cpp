// Filename	: DlgGuildMap.cpp
// Creator	: Xiao Zhou
// Date		: 2005/12/31

#include <AFileImage.h>

#include "DlgGuildMap.h"
#include "DlgGuildMapStatus.h"
#include "DlgGuildMapTravel.h"
#include "EC_GameUIMan.h"
#include "EC_Game.h"
#include "EC_Faction.h"
#include "EC_HostPlayer.h"
#include "EC_World.h"
#include "EL_Precinct.h"
#include "A2DSprite.h"
#include "A3DDevice.h"
#include "A3DGdi.h"
#include "globaldataman.h"
#include "A3DFlatCollector.h"
#include "EC_GameSession.h"
#include "battleenter_re.hpp"
#include "ClosedArea.h"

#define new A_DEBUG_NEW

enum
{
	AREA_TYPE_NORMAL = 0,
	AREA_TYPE_OWNER = 1,
	AREA_TYPE_CANCHALLENGE = 2,
	AREA_TYPE_TEMPCHALLENGED = 4,
	AREA_TYPE_CHALLENGED = 8,
	AREA_TYPE_BECHALLENGED = 16,
	AREA_TYPE_CANENTER = 32,
	AREA_TYPE_HOVER = 64,
};

AUI_BEGIN_COMMAND_MAP(CDlgGuildMap, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",		OnCommandCancel)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgGuildMap, CDlgBase)

AUI_ON_EVENT("GuildMap",	WM_MOUSEMOVE,	OnEventMouseMove)
AUI_ON_EVENT("GuildMap",	WM_LBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("GuildMap",	WM_RBUTTONDOWN,	OnEventRButtonDown)

AUI_END_EVENT_MAP()

CDlgGuildMap::CDlgGuildMap()
{
	m_pImg_GuildMap = NULL;
	m_nHoverAreaIndex = -1;
	m_nType = FACTIONMAP_TYPE_NORMAL;
	m_bGetMap = false;
	m_bGetChallengeMap = false;
	m_pA2DSpriteArrow1 = NULL;
	m_pA2DSpriteArrow2 = NULL;
	m_nMyFactionColor = 0;
}

CDlgGuildMap::~CDlgGuildMap()
{
	A3DRELEASE(m_pA2DSpriteArrow1);
	A3DRELEASE(m_pA2DSpriteArrow2);
}

A3DPOINT2 CDlgGuildMap::ScreenToPixel(A3DPOINT2 pt)
{
	A3DPOINT2 ptCenter = m_pImg_GuildMap->GetRect().CenterPoint();
	SIZE size;
	size.cx = m_pImg_GuildMap->GetImage()->GetWidth();
	size.cy = m_pImg_GuildMap->GetImage()->GetHeight();
	A3DRECT rc = m_pImg_GuildMap->GetRect();
	pt.x = size.cx / 2 + (pt.x - ptCenter.x) * size.cx / rc.Width();
	pt.y = size.cy / 2 + (pt.y - ptCenter.y) * size.cy / rc.Height();
	return pt;
}


A3DPOINT2 CDlgGuildMap::PixelToScreen(A3DPOINT2 pt)
{
	A3DPOINT2 ptCenter = m_pImg_GuildMap->GetRect().CenterPoint();
	SIZE size;
	size.cx = m_pImg_GuildMap->GetImage()->GetWidth();
	size.cy = m_pImg_GuildMap->GetImage()->GetHeight();
	A3DRECT rc = m_pImg_GuildMap->GetRect();
	pt.x = (pt.x - size.cx / 2) * rc.Width() / size.cx + ptCenter.x;
	pt.y = (pt.y - size.cy / 2) * rc.Height() / size.cy + ptCenter.y;
	return pt;
}


void CDlgGuildMap::OnEventMouseMove(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();

	A3DPOINT2 pt;
	pt.x = GET_X_LPARAM(lParam) - p->X;
	pt.y = GET_Y_LPARAM(lParam) - p->Y;
	pt = ScreenToPixel(pt);
	vector<DOMAIN_INFO> *info = globaldata_getdomaininfos();
	DWORD i, j;
	POINT_FLOAT pf;
	pf.x = (float)pt.x;
	pf.y = (float)pt.y;
	m_nHoverAreaIndex = -1;
	CClosedArea cArea;
	for(i = 0; i < m_vecAreaRect.size(); i++)
		if( m_vecAreaRect[i].PtInRect(pt.x, pt.y) )
		{
			cArea.Clear();
			for(j = 0; j < (*info)[i].verts.size(); j++)
			{
				POINT_FLOAT temp;
				temp.x = (float)(*info)[i].verts[j].x;
				temp.y = (float)(*info)[i].verts[j].y;
				cArea.AddPoint(temp);
			}
			cArea.AreaFinish();
			if( cArea.IsInArea(pf) )
			{
				m_nHoverAreaIndex = i;
				break;
			}
		}

// 	if( m_nType == FACTIONMAP_TYPE_NORMAL || 
// 		m_nType == FACTIONMAP_TYPE_ENTER )
// 		GetGameUIMan()->m_pDlgGMapStatus1->SetIndex(m_nHoverAreaIndex);
// 	else if( m_nType == FACTIONMAP_TYPE_CHALLENGE )
// 		GetGameUIMan()->m_pDlgGMapStatus2->SetIndex(m_nHoverAreaIndex);
}

void CDlgGuildMap::OnEventMButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
}

void CDlgGuildMap::OnEventRButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	OnCommandCancel("");
}

void CDlgGuildMap::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
// 	vector<DOMAIN_INFO> *info = globaldata_getdomaininfos();
// 	int idFaction = GetHostPlayer()->GetFactionID();
// 	if( m_nType == FACTIONMAP_TYPE_ENTER )
// 	{
// 		if( m_nHoverAreaIndex != -1 && 
// 			(m_vecAreaType[m_nHoverAreaIndex] & AREA_TYPE_CANENTER) &&
// 			((int)(*info)[m_nHoverAreaIndex].id_owner == idFaction || 
// 			(int)(*info)[m_nHoverAreaIndex].id_challenger1 == idFaction ||
// 			(int)(*info)[m_nHoverAreaIndex].id_challenger2 == idFaction) )
// 		{
// 		}
// 	}
// 	else if( m_nType == FACTIONMAP_TYPE_CHALLENGE )
// 	{
// 		if( m_nHoverAreaIndex != -1 )
// //			m_vecAreaType[m_nHoverAreaIndex] & AREA_TYPE_CANCHALLENGE )
// 		{
// 			GetGameUIMan()->m_pDlgGMapChallenge->SetIndex(m_nHoverAreaIndex);
// 			GetGameUIMan()->m_pDlgGMapChallenge->Show(true, true);
// 			GetGameUIMan()->m_pDlgGMapChallenge->SetPosEx(0, 0, alignCenter, alignCenter);
// 			GetGameUIMan()->m_pDlgGMapChallenge->SetCanMove(false);
// 		}
// 	}
// 	else if( m_nType == FACTIONMAP_TYPE_NORMAL )
// 	{
// 		if( idFaction != 0 && m_nHoverAreaIndex != -1 )
// 		{
// 			bool bCanTravel = false;
// 			DWORD i;
// 			for(i = 0; i < info->size(); i++ )
// 				if( (*info)[i].id_owner == idFaction )
// 				{
// 					if( (int)i == m_nHoverAreaIndex )
// 					{
// 						bCanTravel = true;
// 						break;
// 					}
// 				}
// 
// 			if( bCanTravel )
// 			{
// 				GetGameUIMan()->m_pDlgGMapTravel->SetDomainId((*info)[m_nHoverAreaIndex].id);
// 				GetGameUIMan()->m_pDlgGMapTravel->Show(true, true);
// 				GetGameUIMan()->m_pDlgGMapTravel->SetPosEx(0, 0, alignCenter, alignCenter);
// 				GetGameUIMan()->m_pDlgGMapTravel->SetCanMove(false);
// 			}
// 		}
// 	}
}

bool CDlgGuildMap::OnInitDialog()
{
	m_bBattleOpen = false;
	m_bBattleChallengeOpen = false;
	DDX_Control("GuildMap", m_pImg_GuildMap);
	m_vecAreaRect.clear();
	m_vecAreaType.clear();
	m_vecFactionColor.clear();

	A2DSprite *pA2DSprite = m_pImg_GuildMap->GetImage();
	pA2DSprite->SetLinearFilter(true);
	
	vector<DOMAIN_INFO> *info = globaldata_getdomaininfos();
	DWORD i, j;
	for(i = 0; i < info->size(); i++ )
	{
		A3DRECT rect;
		rect.SetRect(9999, 9999, 0, 0);
		(*info)[i].id_owner = 0;
//		(*info)[i].id_challenger1 = 0;
//		(*info)[i].battle_time1 = 0;
//		(*info)[i].id_challenger2 = 0;
//		(*info)[i].battle_time2 = 0;
//		(*info)[i].id_tempchallenger1 = 0;
//		(*info)[i].id_tempchallenger2 = 0;
		(*info)[i].deposit = 0;
//		(*info)[i].cutoff_time = 0;
		(*info)[i].color = 0;
		for(j = 0; j < (*info)[i].verts.size(); j++)
		{
			if( (*info)[i].verts[j].x < rect.left )
				rect.left = (*info)[i].verts[j].x;
			else if( (*info)[i].verts[j].x > rect.right )
				rect.right = (*info)[i].verts[j].x;
			if( (*info)[i].verts[j].y < rect.top )
				rect.top = (*info)[i].verts[j].y;
			else if( (*info)[i].verts[j].y > rect.bottom )
				rect.bottom = (*info)[i].verts[j].y;
		}
		m_vecAreaRect.push_back(rect);
		m_vecAreaType.push_back(AREA_TYPE_NORMAL);
	}

	if( !m_pA2DSpriteArrow1 )
	{
		m_pA2DSpriteArrow1 = new A2DSprite;
		if( !m_pA2DSpriteArrow1 ) return AUI_ReportError(__LINE__, __FILE__);

		bool bval = m_pA2DSpriteArrow1->Init(m_pA3DDevice, "InGame\\城战箭头1.tga", 0);
		if( !bval ) return AUI_ReportError(__LINE__, __FILE__);

		m_pA2DSpriteArrow1->SetLocalCenterPos(
			m_pA2DSpriteArrow1->GetWidth() / 2,
			m_pA2DSpriteArrow1->GetHeight() / 2 );
		m_pA2DSpriteArrow1->SetLinearFilter(true);
	}

	if( !m_pA2DSpriteArrow2 )
	{
		m_pA2DSpriteArrow2 = new A2DSprite;
		if( !m_pA2DSpriteArrow2 ) return AUI_ReportError(__LINE__, __FILE__);

		bool bval = m_pA2DSpriteArrow2->Init(m_pA3DDevice, "InGame\\城战箭头2.tga", 0);
		if( !bval ) return AUI_ReportError(__LINE__, __FILE__);

		m_pA2DSpriteArrow2->SetLocalCenterPos(
			m_pA2DSpriteArrow2->GetWidth() / 2,
			m_pA2DSpriteArrow2->GetHeight() / 2 );
		m_pA2DSpriteArrow2->SetLinearFilter(true);
	}

	AFileImage aFile;
	if( !aFile.Open("Surfaces\\InGame\\FactionColor.txt", AFILE_OPENEXIST | AFILE_TEMPMEMORY) )
		return AUI_ReportError(__LINE__, __FILE__);
	char szText[100];
	DWORD len;
	int r, g, b;
	do 
	{
		if( !aFile.ReadLine(szText, 100, &len) || len == 0 )
			break;
		sscanf(szText, "%d%d%d", &r, &g, &b);
		m_vecFactionColor.push_back(A3DCOLORRGBA(r, g, b, 64));
	} while(true);
	aFile.Close();
	return true;
}

void CDlgGuildMap::OnCommandCancel(const char * szCommand)
{
// 	if( GetGameUIMan()->m_pDlgGMapChallenge->IsShow() )
// 		return;
// 
// 	m_nHoverAreaIndex = -1;
// 	Show(false);
// 	if( m_nType == FACTIONMAP_TYPE_CHALLENGE )
// 	{
// 		GetGameUIMan()->EndNPCService();
// 		GetGameUIMan()->m_pDlgGMapStatus2->Show(false);
// 	}
// 	else
// 	{
// 		if( m_nType == FACTIONMAP_TYPE_ENTER )
// 			GetGameUIMan()->EndNPCService();
// 		GetGameUIMan()->m_pDlgGMapTravel->Show(false);
// 		GetGameUIMan()->GetDialog("Win_GMapBattleTime")->Show(false);
// 		GetGameUIMan()->m_pDlgGMapStatus1->Show(false);
// 	}
// 	GetGameUIMan()->GetDialog("Win_MyFactionColor")->Show(false);
}


void CDlgGuildMap::OnTick()
{
// 	if( m_nType == FACTIONMAP_TYPE_NORMAL ||
// 		m_nType == FACTIONMAP_TYPE_ENTER )
// 	{
// 		GetGameUIMan()->BringWindowToTop(GetGameUIMan()->m_pDlgGMapTravel);
// 		GetGameUIMan()->BringWindowToTop(GetGameUIMan()->GetDialog("Win_GMapBattleTime"));
// 		GetGameUIMan()->BringWindowToTop(GetGameUIMan()->m_pDlgGMapStatus1);
// 	}
// 	else if( m_nType == FACTIONMAP_TYPE_CHALLENGE )
// 	{
// 		GetGameUIMan()->BringWindowToTop(GetGameUIMan()->m_pDlgGMapStatus2);
// 		if( GetGameUIMan()->m_pDlgGMapChallenge->IsShow() )
// 			GetGameUIMan()->BringWindowToTop(GetGameUIMan()->m_pDlgGMapChallenge);
// 	}
// 	if( GetGameUIMan()->GetDialog("Win_MyFactionColor")->IsShow() )
// 		GetGameUIMan()->BringWindowToTop(GetGameUIMan()->GetDialog("Win_MyFactionColor"));
// 	else if( m_nMyFactionColor != 0 )
// 	{
// 		GetGameUIMan()->GetDialog("Win_MyFactionColor")->Show(true);
// 		GetGameUIMan()->GetDialog("Win_MyFactionColor")->SetCanMove(false);
// 	}
}

void CDlgGuildMap::RenderArrow(int nIndex1, int nIndex2, A2DSprite *pArrow)
{
	vector<DOMAIN_INFO> *info = globaldata_getdomaininfos();
	A3DPOINT2 pt1 = PixelToScreen((*info)[nIndex1].center);
	A3DPOINT2 pt2 = PixelToScreen((*info)[nIndex2].center);
	int nDegree = 90 + (int)RAD2DEG((float)atan2(float(pt2.y - pt1.y), float(pt2.x - pt1.x)));
	pArrow->SetDegree(nDegree);
	int dx = pt2.x - pt1.x;
	int dy = pt2.y - pt1.y;
	float fScale = float(sqrt(float(dy * dy + dx * dx)) - 20) / pArrow->GetHeight();
	pArrow->SetScaleX(fScale);
	pArrow->SetScaleY(fScale);
	pArrow->DrawToInternalBuffer((pt2.x + pt1.x) / 2, (pt2.y + pt1.y) / 2);
}

void CDlgGuildMap::RenderArea(int nIndex, A3DCOLOR cl)
{
	A3DFlatCollector *pFC = GetGame()->GetA3DEngine()->GetA3DFlatCollector();
	vector<DOMAIN_INFO> *info = globaldata_getdomaininfos();
	DWORD i;
	A3DVECTOR3 *verts = new A3DVECTOR3[(*info)[nIndex].verts.size()];
	WORD *indices = new WORD[(*info)[nIndex].faces.size()];
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	for(i = 0; i < (*info)[nIndex].verts.size(); i++ )
	{
		A3DPOINT2 pt = PixelToScreen((*info)[nIndex].verts[i]);
		verts[i].x = (float)pt.x + p->X;
		verts[i].y = (float)pt.y + p->Y;
		verts[i].z = 0.0f;
	}
	for(i = 0; i < (*info)[nIndex].faces.size(); i++ )
		indices[i] = (*info)[nIndex].faces[i];

	A3DCULLTYPE oldtype = m_pA3DDevice->GetFaceCull();
	m_pA3DDevice->SetFaceCull(A3DCULL_NONE);
	pFC->AddRenderData_2D(verts, (*info)[nIndex].verts.size(), 
		indices, (*info)[nIndex].faces.size(), cl);
	pFC->Flush_2D();
	m_pA3DDevice->SetFaceCull(oldtype);
	delete indices;
	delete verts;
}

void CDlgGuildMap::GetMap()
{
}

void CDlgGuildMap::OnShowDialog()
{
}

bool CDlgGuildMap::Render()
{
	int idFaction = GetHostPlayer()->GetFactionID();
	bool bval = CDlgBase::Render();
	vector<DOMAIN_INFO> *info = globaldata_getdomaininfos();
	DWORD i;
//	A3DCOLOR *cl = new A3DCOLOR[info->size()];
//	for(i = 0; i < info->size(); i++ )
//		cl[i] = 0;
//	DWORD nColor = 1;
//	for(i = 0; i < info->size(); i++ )
//		if( (*info)[i].id_owner == idFaction )
//			cl[i] = m_vecFactionColor[0];
//	for(i = 0; i < info->size(); i++ )
//		if( (*info)[i].id_owner && (*info)[i].id_owner != idFaction && cl[i] == 0 )
//		{
//			for(j = i; j < info->size(); j++ )
//				if((*info)[j].id_owner == (*info)[i].id_owner )
//					cl[j] = m_vecFactionColor[nColor];
//			nColor++;
//			if( nColor == m_vecFactionColor.size() )
//				nColor = 1;
//		}
//	for(i = 0; i < info->size(); i++ )
//		if( cl[i] )
//			RenderArea(i, cl[i]);
//	delete []cl;
	
//	A3DFlatCollector *pFC = GetGame()->GetA3DEngine()->GetA3DFlatCollector();
//	pFC->Flush_2D();
//	GetGame()->GetA3DEngine()->GetA3DDevice()->SetSourceAlpha(A3DBLEND_DESTCOLOR);
//	GetGame()->GetA3DEngine()->GetA3DDevice()->SetDestAlpha(A3DBLEND_ZERO);
//	for(i = 0; i < info->size(); i++ )
//		if( (*info)[i].color > 0 && (*info)[i].color <= (int)m_vecFactionColor.size() )
//		{
//			if( !(m_nType == FACTIONMAP_TYPE_CHALLENGE && 
//				(!((m_vecAreaType[i] & AREA_TYPE_CANCHALLENGE) ||
//				(m_vecAreaType[i] & AREA_TYPE_TEMPCHALLENGED) ||
//				(m_vecAreaType[i] & AREA_TYPE_OWNER)) ) 
//				||
//				m_nType == FACTIONMAP_TYPE_ENTER && 
//				(!(m_vecAreaType[i] & AREA_TYPE_CANENTER) ||
//				(int)(*info)[i].id_owner != idFaction && 
//				(int)(*info)[i].id_challenger != idFaction)))
//				RenderArea(i, m_vecFactionColor[(*info)[i].color - 1]);
//		}
//	PAUILABEL  pLabel = (PAUILABEL)GetGameUIMan()->
//		GetDialog("Win_MyFactionColor")->GetDlgItem("Txt_MyFactionColor");
//
//	if( m_nMyFactionColor != 0 )
//	{
//		A3DRECT rect;
//		rect.left = pLabel->GetPos().x + pLabel->GetSize().cx;
//		rect.top = pLabel->GetPos().y + 2;
//		rect.right = rect.left + 20;
//		rect.bottom = rect.top + 13;
//		GetGame()->GetA3DEngine()->GetA3DDevice()->SetSourceAlpha(A3DBLEND_SRCALPHA);
//		GetGame()->GetA3DEngine()->GetA3DDevice()->SetDestAlpha(A3DBLEND_INVSRCALPHA);
//		g_pA3DGDI->Draw2DRectangle(rect, A3DCOLORRGB(210, 190, 140));
//		GetGame()->GetA3DEngine()->GetA3DDevice()->SetSourceAlpha(A3DBLEND_DESTCOLOR);
//		GetGame()->GetA3DEngine()->GetA3DDevice()->SetDestAlpha(A3DBLEND_ZERO);
//		g_pA3DGDI->Draw2DRectangle(rect, m_vecFactionColor[m_nMyFactionColor - 1]);
//	}
//
//	GetGame()->GetA3DEngine()->GetA3DDevice()->SetSourceAlpha(A3DBLEND_SRCALPHA);
//	GetGame()->GetA3DEngine()->GetA3DDevice()->SetDestAlpha(A3DBLEND_INVSRCALPHA);
//	if( m_nType == FACTIONMAP_TYPE_CHALLENGE )
//	{
//		for(i = 0; i < info->size(); i++ )
//			if( !((m_vecAreaType[i] & AREA_TYPE_CANCHALLENGE) ||
//				(m_vecAreaType[i] & AREA_TYPE_TEMPCHALLENGED) ||
//				(m_vecAreaType[i] & AREA_TYPE_OWNER)) )
//				RenderArea(i, A3DCOLORRGBA(64, 64, 64, 196));
//	}
	if( m_nType == FACTIONMAP_TYPE_ENTER )
	{
		for(i = 0; i < info->size(); i++ )
			if( !(m_vecAreaType[i] & AREA_TYPE_CANENTER) ||
				(int)(*info)[i].id_owner != idFaction 
//				(int)(*info)[i].id_challenger1 != idFaction &&
//				(int)(*info)[i].id_challenger2 != idFaction 
				)
				RenderArea(i, A3DCOLORRGBA(64, 64, 64, 196));
	}
//	for(i = 0; i < info->size(); i++ )
//		if( (*info)[i].id_owner )
//		{
//			int nIndex = 0;
//			A2DSprite *sprite = GetGameUIMan()->GetFactionIcon(GetGame()->GetGameInit().iServerID,
//				(*info)[i].id_owner, &nIndex);
//			sprite->SetCurrentItem(nIndex);
//			A3DPOINT2 ptCenter = PixelToScreen((*info)[i].center);
//			sprite->DrawToBack(ptCenter.x - 8, ptCenter.y - 8);
//		}
	if( m_nHoverAreaIndex != -1 )
	{
		RenderArea(m_nHoverAreaIndex, A3DCOLORRGBA(255, 255, 255, 128));
//		if( m_nType == FACTIONMAP_TYPE_NORMAL || m_nType == FACTIONMAP_TYPE_ENTER )
//			for(i = 0; i < (*info)[m_nHoverAreaIndex].neighbours.size(); i++ )
//			{
//				int neighbour = (*info)[m_nHoverAreaIndex].neighbours[i];
//				bool bAttack = false;
//				bool bDefence = false;
//				if( (*info)[neighbour].id_owner && (*info)[m_nHoverAreaIndex].id_challenger &&
//					(*info)[neighbour].id_owner == (*info)[m_nHoverAreaIndex].id_challenger )
//					bDefence = true;
//				if( (*info)[neighbour].id_challenger && (*info)[m_nHoverAreaIndex].id_owner &&
//					(*info)[neighbour].id_challenger == (*info)[m_nHoverAreaIndex].id_owner )
//					bAttack = true;
//				if( bAttack && bDefence )
//					RenderArrow(neighbour, m_nHoverAreaIndex, m_pA2DSpriteArrow2);
//				else if( bAttack )
//					RenderArrow(m_nHoverAreaIndex, neighbour, m_pA2DSpriteArrow1);
//				else if( bDefence )
//					RenderArrow(neighbour, m_nHoverAreaIndex, m_pA2DSpriteArrow1);
//			}
//		else if( m_nType == FACTIONMAP_TYPE_CHALLENGE )
//			for(i = 0; i < (*info)[m_nHoverAreaIndex].neighbours.size(); i++ )
//			{
//				int neighbour = (*info)[m_nHoverAreaIndex].neighbours[i];
//				bool bAttack = false;
//				bool bDefence = false;
//				if( (*info)[neighbour].id_owner && (*info)[m_nHoverAreaIndex].id_tempchallenger &&
//					(*info)[neighbour].id_owner == (*info)[m_nHoverAreaIndex].id_tempchallenger )
//					bDefence = true;
//				if( (*info)[neighbour].id_tempchallenger && (*info)[m_nHoverAreaIndex].id_owner &&
//					(*info)[neighbour].id_tempchallenger == (*info)[m_nHoverAreaIndex].id_owner )
//					bAttack = true;
//				if( bAttack && bDefence )
//					RenderArrow(neighbour, m_nHoverAreaIndex, m_pA2DSpriteArrow2);
//				else if( bAttack )
//					RenderArrow(m_nHoverAreaIndex, neighbour, m_pA2DSpriteArrow1);
//				else if( bDefence )
//					RenderArrow(neighbour, m_nHoverAreaIndex, m_pA2DSpriteArrow1);
//			}
	}

	return bval;
}

void CDlgGuildMap::BattleAction(int idAction, void *pData)
{
}

void CDlgGuildMap::UpdateNormalMap()
{
	vector<DOMAIN_INFO> *info = globaldata_getdomaininfos();
	int idFaction = GetHostPlayer()->GetFactionID();
	DWORD j;
	for(j = 0; j < info->size(); j++ )
		m_vecAreaType[j] &= ~(AREA_TYPE_CHALLENGED | AREA_TYPE_BECHALLENGED | AREA_TYPE_OWNER);

	if( idFaction )
	{
		for(j = 0; j < info->size(); j++ )
		{
//			if( (*info)[j].id_challenger == idFaction )
//				m_vecAreaType[j] |= AREA_TYPE_CHALLENGED;
//			if( (*info)[j].id_owner == idFaction )
//			{
//				m_vecAreaType[j] |= AREA_TYPE_OWNER;
//				if( (*info)[j].id_challenger != 0 ) 
//					m_vecAreaType[j] |= AREA_TYPE_BECHALLENGED;
//			}
		}
	}
}

void CDlgGuildMap::UpdateChallengeMap()
{
//	vector<DOMAIN_INFO> *info = globaldata_getdomaininfos();
//	int idFaction = GetHostPlayer()->GetFactionID();
//	int nDomain = 0;
//	int nIndexChallenged = -1;
//	DWORD j, k;
//	for(j = 0; j < info->size(); j++ )
//		m_vecAreaType[j] &= ~(AREA_TYPE_CANCHALLENGE | AREA_TYPE_TEMPCHALLENGED);
//	if( idFaction )
//	{
//		for(j = 0; j < info->size(); j++ )
//			if( (*info)[j].id_owner == idFaction )
//				nDomain++;
//		for(j = 0; j < info->size(); j++ )
//			if( (*info)[j].id_tempchallenger == idFaction )
//			{
//				nIndexChallenged = j;
//				break;
//			}
//		
//		if( nIndexChallenged != -1 )
//			m_vecAreaType[nIndexChallenged] |= AREA_TYPE_TEMPCHALLENGED;
//		else if( nDomain == 0 )
//		{
//			for(j = 0; j < info->size(); j++ )
//				if( (*info)[j].id_owner != idFaction &&
//					(*info)[j].type == DOMAIN_TYPE_3RD_CLASS )
//					m_vecAreaType[j] |= AREA_TYPE_CANCHALLENGE;
//		}
//		else
//		{
//			for(j = 0; j < info->size(); j++ )
//				if( (*info)[j].id_owner == idFaction ) 
//					for(k = 0; k < (*info)[j].neighbours.size(); k++)
//					{
//						int neighbour = (*info)[j].neighbours[k];
//						if( (*info)[neighbour].id_owner != idFaction &&
//							nDomain >= (*info)[neighbour].type
//							&& (*info)[neighbour].type == DOMAIN_TYPE_3RD_CLASS )
//							m_vecAreaType[neighbour] |= AREA_TYPE_CANCHALLENGE;
//					}
//		}
//	}
}