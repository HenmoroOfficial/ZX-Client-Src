// Filename	: DlgWorldMap.cpp
// Creator	: Tom Zhou
// Date		: October 16, 2005

#include "AUI\\AUIDef.h"
#include "AUI\\AUISubDialog.h"
#include "AUI\\AUIStillImageButton.h"
#include "DlgWorldMap.h"
#include "DlgMidMap.h"
#include "DlgMiniMap.h"
#include "DlgGuildMap.h"
#include "DlgGuildMapStatus.h"
#include "DlgTerrWarMap.h"
#include "EC_Configs.h"
#include "EC_World.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_Instance.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_TeamMan.h"
#include "EC_Team.h"
#include "EC_Model.h"
#include "EC_AutoMove.h"
#include "EC_Global.h"
#include "EC_GameSession.h"
#include "A2DSprite.h"
#include "A3DDevice.h"
#include "globaldataman.h"
#include "ClosedArea.h"
#include "A3DFlatCollector.h"

#define new A_DEBUG_NEW

#define CELL	512

/***********************************************************************

将屏幕坐标与世界坐标互转
p->X p->Y通常为0
rcMap表示地图在显示器上显示区域大小
世界地图与中间地图显示不同在于中间地图是方形的，1024*768分辨率下大小为512*512
世界地图为1024*768，但是有效区域大小为768*768，两边美术会画成类似山峰的样子
对于POINTTOSCREEN_X(x)
rcMap.left + rcMap.Width()/2 - rcMap.Height()/2 表示地图左边有效区域开始位置
rcMap.Height() * (x + CELL*nCol) / (CELL * nCol *2) 表示当前地图坐标x值映射到显示器上x轴方向的偏移

************************************************************************/

#define POINTTOSCREEN_X(x) int(int(p->X) + (rcMap.left + rcMap.Width()/2 - rcMap.Height()/2) + rcMap.Height() * (x + CELL*nCol) / (CELL * nCol * 2))
#define POINTTOSCREEN_Y(y) int(int(p->Y) + rcMap.top + (CELL*nRow - y) * rcMap.Height() / (CELL*nRow*2))
#define SCREENTOPOINT_X(x) float((x - int(p->X) - (rcMap.left + rcMap.Width()/2 - rcMap.Height()/2)) * (CELL * nCol * 2) / rcMap.Height() - CELL*nCol)
#define SCREENTOPOINT_Y(y) float(CELL*2*nRow - (y - int(p->Y) - rcMap.top) * CELL * 2 * nRow / rcMap.Height() - CELL * nRow)

AUI_BEGIN_COMMAND_MAP(CDlgWorldMap, CDlgBase)

AUI_ON_COMMAND("return",		OnCommandReturn)
AUI_ON_COMMAND("Btn_Territory",	OnCommandTerrMap)
AUI_ON_COMMAND("Btn_ZoomIn",	OnCommandSize)
AUI_ON_COMMAND("Btn_ZoomOut",	OnCommandSize)
AUI_ON_COMMAND("Btn_Worldmap",	OnCommandWorldMap)
AUI_ON_COMMAND("IDCANCEL",		OnCommandCancel)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgWorldMap, CDlgBase)

AUI_ON_EVENT("*",				WM_MOUSEWHEEL,	OnEventMouseWheel)
AUI_ON_EVENT(NULL,				WM_MOUSEWHEEL,	OnEventMouseWheel)
AUI_ON_EVENT("Img_WorldMap",	WM_MOUSEMOVE,	OnEventMouseMove)
AUI_ON_EVENT("Img_WorldMap",	WM_LBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("Img_WorldMap",	WM_RBUTTONUP,	OnEventRButtonUp)

AUI_END_EVENT_MAP()

abase::vector<DOMAIN_INFO>* GetSuitDomainInfo(int nType)
{
	abase::vector<DOMAIN_INFO> *info;
	switch (nType)
	{
	case MAP_MAIN:
		info = globaldata_getdomaininfos3();
		break;
	case MAP_NORMAL:
		info = globaldata_getdomaininfos();
		break;
	case MAP_CROSS:
		info = globaldata_getdomaininfos4();
		break;
	case MAP_HONGHUANG:
		info = globaldata_getdomaininfos5();
		break;
	default:
		info = globaldata_getdomaininfos();
	}
	return info;
}

int				CDlgWorldMap::m_snMapType = MAP_MAIN;

CDlgWorldMap::CDlgWorldMap()
{
}

CDlgWorldMap::~CDlgWorldMap()
{
}

A3DPOINT2 CDlgWorldMap::ScreenToPixel(A3DPOINT2 pt)
{
	A3DPOINT2 ptCenter = m_pImg_WorldMap->GetRect().CenterPoint();
	SIZE size;
	size.cx = m_pImg_WorldMap->GetImage()->GetWidth();
	size.cy = m_pImg_WorldMap->GetImage()->GetHeight();
	A3DRECT rc = m_pImg_WorldMap->GetRect();
	pt.x = size.cx / 2 + (pt.x - ptCenter.x) * size.cx / rc.Width();
	pt.y = size.cy / 2 + (pt.y - ptCenter.y) * size.cy / rc.Height();

	return pt;
}


A3DPOINT2 CDlgWorldMap::PixelToScreen(A3DPOINT2 pt)
{
	A3DPOINT2 ptCenter = m_pImg_WorldMap->GetRect().CenterPoint();
	SIZE size;
	size.cx = m_pImg_WorldMap->GetImage()->GetWidth();
	size.cy = m_pImg_WorldMap->GetImage()->GetHeight();
	A3DRECT rc = m_pImg_WorldMap->GetRect();
	if(m_snMapType==MAP_MAIN||m_snMapType==MAP_CROSS||m_snMapType==MAP_HONGHUANG)
	{
		pt.x = (pt.x - size.cx / 2) * rc.Width() / size.cx + ptCenter.x;
		pt.y = (pt.y - size.cy / 2) * rc.Height() / size.cy + ptCenter.y;
	}
	else
	{
		pt.x = (pt.x/2 - size.cx / 2) * rc.Width() / size.cx + ptCenter.x;
		pt.y = (pt.y/2 - size.cy / 2) * rc.Height() / size.cy + ptCenter.y;
	}
	return pt;
}

void CDlgWorldMap::OnCommandReturn(const char* szCommand)
{
	CDlgWorldMap* pDlgWorldMapSub = (CDlgWorldMap*)GetGameUIMan()->GetDialog("WorldMap_Sub");
	if (m_snMapType==MAP_NORMALXX)
		pDlgWorldMapSub->Return(MAP_NORMAL);
	else if (m_snMapType==MAP_CROSSXX)
		pDlgWorldMapSub->Return(MAP_CROSS);
	else if (m_snMapType==MAP_HONGHUANGXX)
		pDlgWorldMapSub->Return(MAP_HONGHUANG);
	else
		pDlgWorldMapSub->Return(MAP_MAIN);

}

void CDlgWorldMap::Return(int nType)
{
	m_snMapType = nType;
	AUIOBJECT_SETPROPERTY p;
	if(nType==MAP_MAIN)
		strcpy(p.fn, "WorldMaps\\worldmaps_main.dds");
	else if(nType==MAP_CROSS)
		strcpy(p.fn, "WorldMaps\\worldmap_heaven.dds");
	else if(nType==MAP_NORMAL)
		strcpy(p.fn, "WorldMaps\\WorldMap.dds");
	else
		strcpy(p.fn, "WorldMaps\\WorldMap_02.dds");
	m_pImg_WorldMap->SetProperty("Image File", &p);
	m_idMap = 0;
	m_nHoverAreaIndex = -1;
	float sx = g_pGame->GetConfigs()->GetSystemSettings().iRndWidth / 1024.0f;
	float sy = g_pGame->GetConfigs()->GetSystemSettings().iRndHeight / 768.0f;
	float s = sx<sy ? sx : sy;
	int width = (int)(1024*s);
	int height = (int)(768*s);
	SetSize(width, height);
	SetScale(1.0f/GetGameUIMan()->GetCustomUIScale());
	m_pImg_WorldMap->SetSize(width, height);
	m_pImg_WorldMap->SetPos(0,0);
	PAUISUBDIALOG pSubD = (PAUISUBDIALOG)GetGameUIMan()->m_pDlgWorldMap->GetDlgItem("Sub_Worldmap");
	pSubD->SetSubDialogOrgSize(width, height);
	GetGameUIMan()->GetDialog("Win_Worldmap_Button")->GetDlgItem("Img_Return")->Show(nType!=MAP_MAIN);
	GetGameUIMan()->GetDialog("Win_Worldmap_Button")->GetDlgItem("Btn_Return")->Show(nType!=MAP_MAIN);

	

	m_vecAreaRect.clear();
	vector<DOMAIN_INFO> *info = GetSuitDomainInfo(nType);
	DWORD i, j;
	for(i = 0; i < info->size(); i++ )
	{
		A3DRECT rect;
		rect.SetRect(9999, 9999, 0, 0);
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
		if(!(m_snMapType==MAP_MAIN||m_snMapType==MAP_CROSS||m_snMapType==MAP_HONGHUANG))
		{
			rect.left/=2;
			rect.right/=2;
			rect.top/=2;
			rect.bottom/=2;
		}
		m_vecAreaRect.push_back(rect);
	}

}

void CDlgWorldMap::OnCommandTerrMap(const char* szCommand)
{
	GetGameSession()->territory_get_map();
	PAUISUBDIALOG pSub = (PAUISUBDIALOG)GetGameUIMan()->GetDialog("Win_WorldMap")->GetDlgItem("Sub_Worldmap");
	pSub->SetDialog(GetGameUIMan()->GetDialog("WorldMap_TerriSub"));
}

void CDlgWorldMap::OnCommandSize(const char* szCommand)
{
	CDlgTerrWarMap* pDlgTerrWarMap = (CDlgTerrWarMap*)GetGameUIMan()->GetDialog("WorldMap_TerriSub");
	pDlgTerrWarMap->ChangeSize();
}

void CDlgWorldMap::OnCommandWorldMap(const char* szCommand)
{
	PAUISUBDIALOG pSub = (PAUISUBDIALOG)GetGameUIMan()->GetDialog("Win_WorldMap")->GetDlgItem("Sub_Worldmap");
	pSub->SetDialog(GetGameUIMan()->GetDialog("WorldMap_Sub"));
}

void CDlgWorldMap::OnCommandCancel(const char* szCommand)
{
	Show(false);
	GetGameUIMan()->GetDialog("Win_WorldMap")->Show(false);
	GetGameUIMan()->GetDialog("WorldMap_Sub")->Show(false);
	GetGameUIMan()->GetDialog("WorldMap_TerriSub")->Show(false);
	
	GetGameUIMan()->GetDialog("Win_Battle_TerriWar")->Show(false);
	GetGameUIMan()->GetDialog("Win_Battle_TerriWarAdd")->Show(false);
	GetGameUIMan()->GetDialog("Win_Battle_TerriAddNum")->Show(false);
	GetGameUIMan()->GetDialog("Win_Battle_TerriDeclare")->Show(false);
}

void CDlgWorldMap::OnEventMouseMove(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if( m_idMap == 0 )
	{
		A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();

		A3DPOINT2 pt;
		pt.x = GET_X_LPARAM(lParam) - p->X;
		pt.y = GET_Y_LPARAM(lParam) - p->Y;
		pt = ScreenToPixel(pt);
		abase::vector<DOMAIN_INFO> *info = GetSuitDomainInfo(m_snMapType);
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
					
					if(m_snMapType==MAP_MAIN||m_snMapType==MAP_CROSS||m_snMapType==MAP_HONGHUANG)
					{
						temp.x = (float)(*info)[i].verts[j].x;
						temp.y = (float)(*info)[i].verts[j].y;
					}
					else
					{
						temp.x = (float)(*info)[i].verts[j].x/2;
						temp.y = (float)(*info)[i].verts[j].y/2;
					}
					cArea.AddPoint(temp);
				}
				cArea.AreaFinish();
				if( cArea.IsInArea(pf) )
				{
					m_nHoverAreaIndex = i;
					break;
				}
			}
//		GetGameUIMan()->m_pDlgGMapStatus1->Show(true);
//		GetGameUIMan()->m_pDlgGMapStatus1->SetCanMove(false);
//		GetGameUIMan()->m_pDlgGMapStatus1->SetIndex(m_nHoverAreaIndex);
	}
	else if( m_idMap == GetWorld()->GetInstanceID() )
	{
//		GetGameUIMan()->m_pDlgGMapStatus1->Show(false);
		int i;
		A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);
		int nRow = 1;
		int nCol = 1;
		CECInstance *pInstance = GetGameRun()->GetInstance(m_idMap);
		nRow = pInstance->GetRowNum();
		nCol = pInstance->GetColNum();

		A3DRECT rcMap;
		A3DVECTOR3 vecPosHost = GetHostPlayer()->GetPos();
		rcMap = m_pImg_WorldMap->GetRect();
		GetGameUIMan()->m_pDlgMiniMap->m_vecNowTarget.x = SCREENTOPOINT_X(x);
		GetGameUIMan()->m_pDlgMiniMap->m_vecNowTarget.y = vecPosHost.y;
		GetGameUIMan()->m_pDlgMiniMap->m_vecNowTarget.z = SCREENTOPOINT_Y(y);
		pObj->SetHint(_AL(""));
		for( i = 0; i < (int)m_vecWorldMapMark.size(); i++ )
		{
			if( m_vecWorldMapMark[i].rect.PtInRect(x, y) )
			{
				pObj->SetHint(m_vecWorldMapMark[i].strHint);
				GetGameUIMan()->m_pDlgMiniMap->m_vecNowTarget = m_vecWorldMapMark[i].vecPos;
				return;
			}
		}
	}
}

void CDlgWorldMap::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if( m_idMap == 0 )
	{
		if( m_nHoverAreaIndex != -1 )
		{
			vector<DOMAIN_INFO> *info = GetSuitDomainInfo(m_snMapType);
			m_idMap = (*info)[m_nHoverAreaIndex].id;
			if (m_snMapType==MAP_MAIN)
			{
				/*if (m_idMap==MAP_NORMAL)
				{
					m_snMapType = GetNormalMapType();
				}
				else*/
					m_snMapType = m_idMap;
				Return(m_snMapType);
			}
			else
			{
				m_snMapType+=3;
				CECInstance *pInstance = GetGameRun()->GetInstance(m_idMap);
				AUIOBJECT_SETPROPERTY p;
				sprintf(p.fn, "WorldMaps\\%s.dds", pInstance->GetPath());
				m_pImg_WorldMap->SetProperty("Image File", &p);
				MapResize();
				GetGameUIMan()->GetDialog("Win_Worldmap_Button")->GetDlgItem("Img_Return")->Show(true);
				GetGameUIMan()->GetDialog("Win_Worldmap_Button")->GetDlgItem("Btn_Return")->Show(true);
			}
			m_nHoverAreaIndex = -1;
		}
	}
	else
	{
		A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);

		int posX = GetDlgItem("Img_WorldMap")->GetPos().x;
		int posY = GetDlgItem("Img_WorldMap")->GetPos().y;
// 		x-=posX;
// 		y-=posY;
		int nRow = 1;
		int nCol = 1;
		CECInstance *pInstance = GetGameRun()->GetInstance(m_idMap);
		nRow = pInstance->GetRowNum();
		nCol = pInstance->GetColNum();

		A3DRECT rcMap;
		A3DVECTOR3 vecPosHost = GetHostPlayer()->GetPos();
		rcMap = m_pImg_WorldMap->GetRect();
		GetGameUIMan()->m_pDlgMiniMap->m_dwTempTargetTime = GetTickCount();
		GetGameUIMan()->m_pDlgMiniMap->m_vecTempTarget.x = SCREENTOPOINT_X(x);
		GetGameUIMan()->m_pDlgMiniMap->m_vecTempTarget.y = vecPosHost.y;
		GetGameUIMan()->m_pDlgMiniMap->m_vecTempTarget.z = SCREENTOPOINT_Y(y);
		GetGameUIMan()->m_pDlgMiniMap->m_idTempTargetInstance = m_idMap;
		g_ECAutoMove.SearchPath(GetHostPlayer()->GetPos(), 
								GetGameUIMan()->m_pDlgMiniMap->m_vecTempTarget,
								m_idMap, false);

		if( m_idMap == GetWorld()->GetInstanceID() && GetHostPlayer()->GetModelTempArrow() )
			GetHostPlayer()->GetModelTempArrow()->PlayActionByName("动画", 1.0f, true);
	}
}

void CDlgWorldMap::OnEventRButtonUp(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	OnCommandReturn("");
}

void CDlgWorldMap::OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if (stricmp(m_szName, "Win_WorldMap")==0)
		return;
	PAUISUBDIALOG pSubD = (PAUISUBDIALOG)GetGameUIMan()->m_pDlgWorldMap->GetDlgItem("Sub_Worldmap");
	pSubD->OnDlgItemMessage(WM_MOUSEWHEEL, wParam, lParam);
}

void CDlgWorldMap::RenderArea(int nIndex, A3DCOLOR cl)
{
	/*if(m_snMapType==MAP_MAIN)
	{
		m_pImg_WorldMap->SetCover(m_nHoverAreaIndex==0?m_pImgHeaven:m_pImgEarth, 0, 1);
		return;
	}*/

	A3DFlatCollector *pFC = GetGame()->GetA3DEngine()->GetA3DFlatCollector();
	vector<DOMAIN_INFO> *info = GetSuitDomainInfo(m_snMapType);
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

bool CDlgWorldMap::Render()
{
	if( GetWorld()->GetInstanceID() == 15 && 
		GetHostPlayer()->GetPos().x > -473 && GetHostPlayer()->GetPos().x < -381 &&
		GetHostPlayer()->GetPos().z > -444 && GetHostPlayer()->GetPos().z < -194)
	{
		Show(false);
		return true;
	}

	bool bval = CDlgBase::Render();
	
	if (stricmp(m_szName, "Win_WorldMap")==0 || stricmp(m_szName, "Win_Worldmap_Button")==0)
	{
		return bval;
	}
// 	if( m_idMap == 0 )
// 	{
// 		if( !GetGameUIMan()->m_pDlgGuildMap->GetMapStatus() )
// 			GetGameUIMan()->m_pDlgGuildMap->GetMap();
// 		GetGameUIMan()->BringWindowToTop(GetGameUIMan()->m_pDlgGMapStatus1);
// 	}

	m_pImg_WorldMap->SetCover(NULL, 0, 1);
	if( m_nHoverAreaIndex != -1 )
		RenderArea(m_nHoverAreaIndex, A3DCOLORRGBA(255, 255, 255, 128));

	A3DVECTOR3 vecDir = GetHostPlayer()->GetDir();
	int nDegree = 90 - (int)RAD2DEG((float)atan2(vecDir.z, vecDir.x));
	GetGameUIMan()->m_pDlgMiniMap->m_pA2DMapMark[CDlgMiniMap::MAPMARK_HOST]->SetDegree(nDegree);
	
	m_vecWorldMapMark.clear();

	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	float fWindowScale = GetGameUIMan()->GetWindowScale();
	float fSize = fWindowScale / 2;

	CDlgMiniMap *pDlgMiniMap = GetGameUIMan()->m_pDlgMiniMap;
	EC_VIDEO_SETTING vs = GetGame()->GetConfigs()->GetVideoSettings();
	CECHostPlayer *pHost = GetHostPlayer();
	A3DPOINT2 ptPos;
	MAPMARK Mark;
	int nRow = 1;
	int nCol = 1;
	if (m_idMap == GetWorld()->GetInstanceID())
	{
		CECInstance *pInstance = GetGameRun()->GetInstance(m_idMap);
		nRow = pInstance->GetRowNum();
		nCol = pInstance->GetColNum();
	}
	
	// team member
	A3DRECT rcWorldMap, rcMap;
	A3DVECTOR3 vecPosHost = GetHostPlayer()->GetPos();
	rcMap = m_pImg_WorldMap->GetRect();
	rcWorldMap.left = -512 * nCol;//int((vecPosHost.x - 512) / 1024) * 1024 - 512;
	rcWorldMap.top = -512 * nRow;//int((vecPosHost.z - 512) / 1024) * 1024 - 512;
	rcWorldMap.right = rcWorldMap.left + 1024 * nCol;
	rcWorldMap.bottom = rcWorldMap.top + 1024 * nRow;
	rcMap = m_pImg_WorldMap->GetRect();

	if( pHost->IsInWar() && pHost->GetWarInfo().iWarAttackFID )
	{
		const int icon_warbuildings[] = {
			CDlgMiniMap::MAPMARK_WAR_DEFENDERTOWERSMALL,
			CDlgMiniMap::MAPMARK_WAR_DEFENDERTOWERSMALL,
			CDlgMiniMap::MAPMARK_WAR_DEFENDERTOWERSMALL,
			CDlgMiniMap::MAPMARK_WAR_DEFENDERTOWERSMALL,
			CDlgMiniMap::MAPMARK_WAR_DEFENDERTOWERSMALL,
			CDlgMiniMap::MAPMARK_WAR_ATTACKERZONE,
			CDlgMiniMap::MAPMARK_WAR_DEFENDERTOWER,
			CDlgMiniMap::MAPMARK_WAR_ATTACKERZONE,
			CDlgMiniMap::MAPMARK_WAR_DEFENDERWALL,
			CDlgMiniMap::MAPMARK_WAR_DEFENDERWALL,
			CDlgMiniMap::MAPMARK_WAR_DEFENDERWALL,
			CDlgMiniMap::MAPMARK_WAR_DEFENDERTOWERASSIST,
			CDlgMiniMap::MAPMARK_WAR_DEFENDERTOWERASSIST,
			CDlgMiniMap::MAPMARK_WAR_DEFENDERCENTER,
			CDlgMiniMap::MAPMARK_WAR_ATTACKERCENTER,
		};
		for(int i = 1; i <= 14; i++ )
		{
			int nIcon = 0;
			A2DSprite *pIcon = NULL;
			if( icon_warbuildings[i] == CDlgMiniMap::MAPMARK_WAR_ATTACKERZONE )
			{
				if( CDlgMiniMap::m_WarBuildingStatus[i].status == 1 )
					nIcon = CDlgMiniMap::MAPMARK_WAR_DEFENDERTOWER;
				else
					nIcon = CDlgMiniMap::MAPMARK_WAR_ATTACKERZONE;
			}
			else if( CDlgMiniMap::m_WarBuildingStatus[i].status == 1 )
				nIcon = icon_warbuildings[i];
			if( nIcon )
			{
				A3DRECT rect[2];
				pIcon = pDlgMiniMap->m_pA2DMapMark[nIcon];
				int h = int(pIcon->GetHeight() * (1 - CDlgMiniMap::m_WarBuildingStatus[i].hp));
				rect[0].SetRect(0, 0, pIcon->GetWidth(), h);
				rect[1].SetRect(0, h, pIcon->GetWidth(), pIcon->GetHeight());
				pIcon->ResetItems(2, rect);
				pIcon->SetCurrentItem(1);
				pIcon->DrawToInternalBuffer((float)POINTTOSCREEN_X(CDlgMiniMap::m_WarBuildingStatus[i].pos.x), 
					POINTTOSCREEN_Y(CDlgMiniMap::m_WarBuildingStatus[i].pos.z) + h * fWindowScale);
				pIcon = pDlgMiniMap->m_pA2DMapMark[nIcon + 8];
				pIcon->ResetItems(2, rect);
				pIcon->SetCurrentItem(0);
				pIcon->DrawToInternalBuffer(POINTTOSCREEN_X(CDlgMiniMap::m_WarBuildingStatus[i].pos.x), 
					POINTTOSCREEN_Y(CDlgMiniMap::m_WarBuildingStatus[i].pos.z));
			}
		}
	}

	if( vs.bMapPlayer && m_idMap == GetWorld()->GetInstanceID() )
	{
		CECTeamMember *pMember;
		CECTeam *pTeam = pHost->GetTeam();
		CECFriendMan *pMan = pHost->GetFriendMan();
		int i;

		for( i = 0; pTeam && i < pTeam->GetMemberNum(); i++ )
		{
			pMember = pTeam->GetMemberByIndex(i);
			if( pMember->GetCharacterID() == pHost->GetCharacterID() ||
				pMember->GetInstanceID() != GetWorld()->GetInstanceID() )
			{
				continue;
			}

			ptPos.x = int(pMember->GetPos().x);
			ptPos.y = int(pMember->GetPos().z);
			if( !rcWorldMap.PtInRect(ptPos.x, ptPos.y) )
				continue;

			int nDraw;
			if( pMember->GetCharacterID() == pTeam->GetLeaderID() )
				nDraw = CDlgMiniMap::MAPMARK_CAPTAIN;
			else
				nDraw = CDlgMiniMap::MAPMARK_TEAMMATE;
			pDlgMiniMap->m_pA2DMapMark[nDraw]->DrawToInternalBuffer(POINTTOSCREEN_X(ptPos.x), POINTTOSCREEN_Y(ptPos.y));

			int W = int(pDlgMiniMap->m_pA2DMapMark[nDraw]->GetWidth() * fSize);
			int H = int(pDlgMiniMap->m_pA2DMapMark[nDraw]->GetHeight() * fSize);
			Mark.strHint = pMember->GetName();
			Mark.vecPos = pMember->GetPos();
			Mark.rect.SetRect(POINTTOSCREEN_X(ptPos.x) - W, POINTTOSCREEN_Y(ptPos.y) - H,
					POINTTOSCREEN_X(ptPos.x) + W, POINTTOSCREEN_Y(ptPos.y) + H);
			m_vecWorldMapMark.push_back(Mark);
		}
	}

	if( m_idMap == GetWorld()->GetInstanceID() )
	{
 		int hostx = POINTTOSCREEN_X(vecPosHost.x);
 		int hosty = POINTTOSCREEN_Y(vecPosHost.z);
// 		int hostx = int(int(p->X) + (rcMap.left + rcMap.Width() / 2 - rcMap.Height() / 2) + (vecPosHost.x/nCol - rcWorldMap.left) * rcMap.Height() / 1024);
// 		int hosty = int(int(p->Y) + rcMap.top + (1024 - (vecPosHost.z/nRow - rcWorldMap.top)) * rcMap.Height() / 1024);
		pDlgMiniMap->m_pA2DMapMark[CDlgMiniMap::MAPMARK_HOST]->DrawToInternalBuffer(
			hostx, hosty);
	}
	
	// mouse click target
	DWORD dwTimePassed = GetTickCount() - CDlgMiniMap::m_dwTempTargetTime;
	if( CDlgMiniMap::m_idTempTargetInstance == m_idMap &&
		CDlgMiniMap::m_dwTempTargetTime > 0 && dwTimePassed <= 3000 && (dwTimePassed / 500) % 2 == 0 )
	{
		ptPos.x = POINTTOSCREEN_X(CDlgMiniMap::m_vecTempTarget.x);
		ptPos.y = POINTTOSCREEN_Y(CDlgMiniMap::m_vecTempTarget.z);
		pDlgMiniMap->m_pA2DMapMark[CDlgMiniMap::MAPMARK_TAGCLICK]->DrawToInternalBuffer(ptPos.x, ptPos.y);
	}

	if( m_idMap == GetWorld()->GetInstanceID() && g_ECAutoMove.GetState() == CECAutoMove::enumStateSuccess )
	{
		for(int i = g_ECAutoMove.GetWayPointCount() - 1; i >= 0; i -= AUTOMOVE_POINTDEC)
		{
			ptPos.x = POINTTOSCREEN_X(g_ECAutoMove.GetPosByIndex(i).x);
			ptPos.y = POINTTOSCREEN_Y(g_ECAutoMove.GetPosByIndex(i).z);
			pDlgMiniMap->m_pA2DMapMark[CDlgMiniMap::MAPMARK_AUTOMOVE]->DrawToInternalBuffer(ptPos.x, ptPos.y);
		}
	}

	return bval;
}

bool CDlgWorldMap::OnInitDialog()
{
	m_pImg_WorldMap = (PAUIIMAGEPICTURE)GetDlgItem("Img_WorldMap");

	if (m_szName=="Win_WorldMap")
	{
		SetCanMove(false);
	}
	else if (m_szName=="WorldMap_Sub")
	{
		
		m_pImgHeaven = new A2DSprite;
		if( !m_pImgHeaven ) return AUI_ReportError(__LINE__, __FILE__);
		bool bval = m_pImgHeaven->Init(m_pA3DDevice, "Window\\worldmap_heavenhover.tga", AUI_COLORKEY);
		if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
		
		m_pImgEarth = new A2DSprite;
		if( !m_pImgEarth ) return AUI_ReportError(__LINE__, __FILE__);
		bval = m_pImgEarth->Init(m_pA3DDevice, "Window\\worldmap_earthhover.tga", AUI_COLORKEY);
		if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
	}

	return true;
}

bool CDlgWorldMap::Release()
{
	if (m_szName=="WorldMap_Sub")
	{
		A3DRELEASE(m_pImgHeaven);
		A3DRELEASE(m_pImgEarth);
	}
	return CDlgBase::Release();
}

void CDlgWorldMap::OnShowDialog()
{
	if (stricmp(m_szName, "WorldMap_Sub")==0)
	{
		m_nHoverAreaIndex = -1;
		m_idMap = 0;
		
		bool bCrossServer = GetHostPlayer()->GetSrcServerID()!=0;
		PAUIDIALOG pDlgButton = GetGameUIMan()->GetDialog("Win_Worldmap_Button");
		pDlgButton->GetDlgItem("Btn_Worldmap")->Show(false);
		pDlgButton->GetDlgItem("Img_Worldmap1")->Show(false);
		pDlgButton->GetDlgItem("Btn_ZoomOut")->Show(false);
		pDlgButton->GetDlgItem("Img_ZoomOut")->Show(false);
		pDlgButton->GetDlgItem("Btn_ZoomIn")->Show(false);
		pDlgButton->GetDlgItem("Img_ZoomIn")->Show(false);
		pDlgButton->GetDlgItem("Btn_Territory")->Show(!bCrossServer);
		pDlgButton->GetDlgItem("Img_Terri")->Show(!bCrossServer);
		pDlgButton->GetDlgItem("Btn_Return")->Show(true);
		pDlgButton->GetDlgItem("Img_Return")->Show(true);
		pDlgButton->Show(true);
		if(!bCrossServer)
		{
			Return(GetNormalMapType());
		}
		else
			Return(MAP_CROSS);
	}
	else if(stricmp(m_szName,"Win_WorldMap") == 0)
	{
		GetGameUIMan()->m_pDlgWorldMap->SetScale(1.0f/GetGameUIMan()->GetCustomUIScale());
		GetGameUIMan()->m_pDlgWorldMap->SetPosEx(0, 0, AUIDialog::alignCenter, AUIDialog::alignCenter);
		float fWindowScale = GetGameUIMan()->GetWindowScale()/GetGameUIMan()->GetCustomUIScale();
		GetGameUIMan()->GetDialog("Win_Worldmap_Button")->AlignTo(
			GetGameUIMan()->GetDialog("Win_WorldMap"),
			AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_RIGHT,
			AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_BOTTOM);
		GetGameUIMan()->GetDialog("Win_Worldmap_Button")->SetPosEx(
			GetGameUIMan()->GetDialog("Win_Worldmap_Button")->GetPos().x-int(42*fWindowScale), 
			GetGameUIMan()->GetDialog("Win_Worldmap_Button")->GetPos().y-int(34*fWindowScale));
		
	}
}

int CDlgWorldMap::GetNormalMapType()
{
	int iInstanceId = GetWorld()->GetInstanceID();
	if (iInstanceId==44 || iInstanceId==48 || iInstanceId==53)
		return MAP_HONGHUANG;
	else
		return MAP_NORMAL;	

}

void CDlgWorldMap::OnHideDialog()
{
	if (stricmp(m_szName, "Win_WorldMap")==0)
	{
		GetGameUIMan()->GetDialog("Win_Worldmap_Button")->Show(false);
	}
}

void CDlgWorldMap::MapResize()
{
	CECInstance *pInstance = GetGameRun()->GetInstance(m_idMap);
	float sx = g_pGame->GetConfigs()->GetSystemSettings().iRndWidth / 1024.0f;
	float sy = g_pGame->GetConfigs()->GetSystemSettings().iRndHeight / 768.0f;
	float s = sx<sy ? sx : sy;
	int width = int(pInstance->GetColNum()*1024*s);
	int height = int(pInstance->GetRowNum()*768*s);
	if (pInstance->GetColNum()==2 && pInstance->GetRowNum()==2)
	{
		width = int(1024*s);
		height = int(768*s);
	}
	else
	{
		width = int(pInstance->GetColNum()*1024*s);
		height = int(pInstance->GetRowNum()*768*s);
	}
	SetSize(width, height);
	m_pImg_WorldMap->SetSize(width, height);
	m_pImg_WorldMap->SetPos(0,0);
	PAUISUBDIALOG pSubD = (PAUISUBDIALOG)GetGameUIMan()->m_pDlgWorldMap->GetDlgItem("Sub_Worldmap");
	pSubD->SetSubDialogOrgSize(width, height);
}
