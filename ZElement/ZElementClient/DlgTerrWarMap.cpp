// Filename	: DlgTerrWarMap.cpp
// Creator	: Fu Chonggang
// Date		: 2010/3/10

#include "AFI.h"
#include "AUI\\AUIDef.h"
#include "AUI\\AUISubDialog.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUICTranslate.h"
#include "AUI\\A3DFTFont.h"
#include "DlgTerrWarMap.h"
#include "DlgMidMap.h"
#include "DlgMiniMap.h"
#include "DlgGuildMap.h"
#include "DlgGuildMapStatus.h"
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
#include "EC_Faction.h"
#include "EC_Global.h"
#include "EC_IvtrItem.h"
#include "EC_GameSession.h"
#include "EC_ManNpc.h"
#include "EC_NPCServer.h"
#include "EC_Inventory.h"
#include "A2DSprite.h"
#include "A3DDevice.h"
#include "globaldataman.h"
#include "ClosedArea.h"
#include "A3DFlatCollector.h"
#include "ids.hxx"

#define new A_DEBUG_NEW

unsigned long ulColor[] =
{
		0x00000000,
		0x4f6C2DC7,
		0x4fF87431,
		0x4f4CC417,
		0x4f79BAEC,
		0x4fFF00FF,
		0x4f4E387E,
		0x4f306EFF,
		0x4f2B3856,
		0x4fB93B8F,
		0x4fF75D59,
		0x4f7F525D,
		0x4f4AA02C,
		0x4f566D7E,
		0x4f43BFC7,
		0x4f153E7E,
		0x4fC6AEC7,
		0x4f92C7C7,
		0x4f8D38C9,
		0x4fE56717,
		0x4f387C44,
		0x4fFAAFBE,
		0x4f52D017,
		0x4f736F6E,
		0x4fF660AB,
		0x4f7E3517,
		0x4fFF0000,
		0x4f4C7D7E,
		0x4fAF7817,
		0x4f7D1B7E,
		0x4fA0C544,
		0x4f77BFC7,
		0x4fC25A7C,
		0x4fFFFF00,
		0x4f8AFB17,
		0x4f7E3817,
		0x4fF52887,
		0x4f52F3FF,
		0x4fC3FDB8,
		0x4f616D7E,
		0x4fC11B17,
		0x4f347C2C,
		0x4f736AFF,
		0x4fE0FFFF,
		0x4fC8BBBE,
		0x4f461B7E,
		0x4f565051,
		0x4f817679,
		0x4f5EFB6E,
		0x4fECD872,
		0x4f504A4B,
};

bool CDlgTerrWarMap::m_bMOpen = false;

AUI_BEGIN_COMMAND_MAP(CDlgTerrWarMap, CDlgBase)

AUI_ON_COMMAND("Btn_TerriDetail",	OnCommandTerriDetail)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgTerrWarMap, CDlgBase)

AUI_ON_EVENT("Img_WorldMap",	WM_MOUSEMOVE,	OnEventMouseMove)
AUI_ON_EVENT("Img_WorldMap",	WM_LBUTTONDOWN,	OnEventLButtonUp)
AUI_ON_EVENT("Img_WorldMap",	WM_RBUTTONDOWN,	OnEventRButtonDown)
AUI_ON_EVENT("Img_WorldMap",	WM_RBUTTONUP,	OnEventRButtonUp)

AUI_END_EVENT_MAP()

CDlgTerrWarMap::CDlgTerrWarMap()
{
	for (int i=0; i<SI_NUM; i++)
	{
		m_pA2DSpriteIcon[i] = NULL;
	}
}

CDlgTerrWarMap::~CDlgTerrWarMap()
{
	for (int i=0; i<SI_NUM; i++)
	{
		A3DRELEASE(m_pA2DSpriteIcon[i]);
	}
}

bool CDlgTerrWarMap::OnInitDialog()
{
	m_pImg_WorldMap = (PAUIIMAGEPICTURE)GetDlgItem("Img_WorldMap");
	m_vecAreaInfo.clear();
	m_nHoverAreaIndex = -1;
	int i,j;
	
	char* path[] = {"worldmaps\\Worldmap_Terri.dds",
		"icon\\Terri_WarArrow.tga",
		"icon\\Terri_WarArrow2.tga",
		"icon\\Terri_WarArrow3.tga",
		"icon\\Terri_WarArrow4.tga",
		"icon\\Terri_WarArrow5.tga",
		"icon\\Terri_WarArrow6.tga",
		"icon\\Terri_Declare.tga",
		"icon\\Terri_Occupy.tga",
		"icon\\Terri_InWar.tga",
		"icon\\Terri_Treasure.tga" };
	for (i=0; i<SI_NUM; i++)
	{
		if( !m_pA2DSpriteIcon[i] )
		{
			m_pA2DSpriteIcon[i] = new A2DSprite;
			if( !m_pA2DSpriteIcon[i] ) return AUI_ReportError(__LINE__, __FILE__);
			
			bool bval = m_pA2DSpriteIcon[i]->Init(m_pA3DDevice, path[i], 0);
			if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
			
			//m_pA2DSpriteIcon[SI_MAP]->SetLinearFilter(true);
			//m_pImg_WorldMap->SetCover(m_pA2DSpriteIcon[SI_MAP], 0);
		}
		if (i>=SI_ARROW1 && i<=SI_ARROW_DOUBLE3)
		{
			m_pA2DSpriteIcon[i]->SetLocalCenterPos(
				m_pA2DSpriteIcon[i]->GetWidth() / 2,
				m_pA2DSpriteIcon[i]->GetHeight() / 2 );
		}
	}
	
	m_pImageMapIcon = new A2DSprite;
	if( !m_pImageMapIcon ) return AUI_ReportError(__LINE__, __FILE__);
	bool bval = m_pImageMapIcon->Init(m_pA3DDevice, "iconset\\IconList_territory.dds", AUI_COLORKEY);
	if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
	
	A3DRECT rc[56];
	int W = 120;
	int H = 120;
	for( i = 0; i < 7; i++ )
	{
		for (j=0; j<8; j++)
		{
			rc[i*8 + j].SetRect(W*j, H*i, W*(j+1), H*(i+1));
		}
	}
	m_pImageMapIcon->ResetItems(56, rc);

	GetGameUIMan()->AddImageListItem(m_pImageMapIcon, m_nImageMapIconIndex);
	
	m_nHintImageFrame = 0;
	m_bHintImageAdd = false;
	
	m_bMOpen = false;
	m_iStatus = 0;
	m_nScale = 1;
	m_dwFlashTime = 0;
	ChangeSize();
	m_bDebug = false;
	return true;
}

bool CDlgTerrWarMap::Release()
{
	int i;
	for (i=0; i<SI_NUM; i++)
	{
		A3DRELEASE(m_pA2DSpriteIcon[i]);
	}
	A3DRELEASE(m_pImageMapIcon);
	vector<DOMAIN_INFO>* infos = globaldata_getdomaininfos2();
	
	for(i = 0; i< (int)infos->size(); ++i)
	{
		DOMAIN_INFO& info = (*infos)[i];
		info.id_owner		= 0;
		info.id_attacker	= 0;
		info.id_defender	= 0;
		info.flag			= 0;
		info.battle_time_start = 0;
		info.battle_time_end = 0;
		info.deposit		= 0;
		info.assis_num		= 0;
		info.assis_drawn_num= 0;
		info.attacker_num	= 0;
		info.defender_num	= 0;
		info.random_award	= 0;
		info.random_award_num = 0;
		info.random_award_drawn = 0;
		info.player_limit	= 0;
		info.color			= 0;
		info.challenged_byself = 0;
	}

	return CDlgBase::Release();
}

void CDlgTerrWarMap::OnShowDialog()
{
	if (!m_bHintImageAdd)
	{
		int nItem;
		GetGameUIMan()->AddImageListItem(GetGameUIMan()->GetSpriteIcon(CECGameUIMan::ICONS_INVENTORY, NULL, &nItem), m_nHintImageIndex);
		m_bHintImageAdd = true;
	}
	PAUIDIALOG pDlgButton = GetGameUIMan()->GetDialog("Win_Worldmap_Button");
	pDlgButton->GetDlgItem("Btn_Worldmap")->Show(true);
	pDlgButton->GetDlgItem("Img_Worldmap1")->Show(true);
	pDlgButton->GetDlgItem("Btn_Territory")->Show(false);
	pDlgButton->GetDlgItem("Img_Terri")->Show(false);
	pDlgButton->GetDlgItem("Btn_Return")->Show(false);
	pDlgButton->GetDlgItem("Img_Return")->Show(false);
	pDlgButton->Show(true);
	{
		SetScale(1.0f/GetGameUIMan()->GetCustomUIScale());
	}
	m_nScale = 1;
	ChangeSize();
}

A3DPOINT2 CDlgTerrWarMap::ScreenToPixel(A3DPOINT2 pt)
{
	A3DPOINT2 ptCenter = m_pImg_WorldMap->GetRect().CenterPoint();
	A3DRECT rc = m_pImg_WorldMap->GetRect();
	pt.x = 1024 / 2 + (pt.x - ptCenter.x) * 1024 / rc.Width();
	pt.y = 768 / 2 + (pt.y - ptCenter.y) * 768 / rc.Height();
	return pt;
}

A3DPOINT2 CDlgTerrWarMap::PixelToScreen(A3DPOINT2 pt)
{
	A3DPOINT2 ptCenter = m_pImg_WorldMap->GetRect().CenterPoint();
	A3DRECT rc = m_pImg_WorldMap->GetRect();
	pt.x = (pt.x/m_nScale - 1024 / 2) * rc.Width() / 1024 + ptCenter.x;
	pt.y = (pt.y/m_nScale - 768 / 2) * rc.Height() / 768 + ptCenter.y;
	return pt;
}

void CDlgTerrWarMap::Resize(A3DRECT rcOld, A3DRECT rcNew)
{
	CDlgBase::Resize(rcOld, rcNew);
	float fWindowScale = GetGameUIMan()->GetWindowScale();


	{
		fWindowScale = fWindowScale/GetGameUIMan()->GetCustomUIScale();
	}
	
	for (int i=0; i<SI_NUM; i++)
	{
		m_pA2DSpriteIcon[i]->SetScaleX(fWindowScale);
		m_pA2DSpriteIcon[i]->SetScaleY(fWindowScale);
	}

	m_pA2DSpriteIcon[SI_MAP]->SetScaleX(fWindowScale/m_nScale);
	m_pA2DSpriteIcon[SI_MAP]->SetScaleY(fWindowScale/m_nScale);

	m_nX = int(m_nX*rcNew.Width()/rcOld.Width());
	m_nY = int(m_nY*rcNew.Width()/rcOld.Width());
}

void CDlgTerrWarMap::OnCommandTerriDetail(const char* szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Battle_TerriDetail");
	pDlg->Show(!pDlg->IsShow());
}

void CDlgTerrWarMap::OnEventMouseMove(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	A3DPOINT2 pt;
	pt.x = GET_X_LPARAM(lParam) - p->X;
	pt.y = GET_Y_LPARAM(lParam) - p->Y;
	if (m_bDrag && m_nScale==1)
	{
		if( m_nStartX != -1)
		{
			int nOffX = pt.x - m_nStartX;
			int nOffY = pt.y - m_nStartY;
			m_nX += nOffX;
			m_nY += nOffY;
			m_nStartX = pt.x;
			m_nStartY = pt.y;
			a_Clamp(m_nX, -(int)GetSize().cx, 0);
			a_Clamp(m_nY, -(int)GetSize().cy, 0);
		}
		return;
	}
	pt.x -= m_nX;
	pt.y -= m_nY;
	pt = ScreenToPixel(pt);
	abase::vector<DOMAIN_INFO> *info = globaldata_getdomaininfos2();
	DWORD i, j;
	POINT_FLOAT pf;
	pf.x = (float)pt.x;
	pf.y = (float)pt.y;
	m_nHoverAreaIndex = -1;
	CClosedArea cArea;
	GetGameUIMan()->SetObjHint(NULL);

	m_pImg_WorldMap->SetHint(_AL(""));
	for(i = 0; i < m_vecAreaInfo.size(); i++)
	{
		if( m_vecAreaInfo[i].rc.PtInRect(pt.x, pt.y) )
		{
			cArea.Clear();
			for(j = 0; j < (*info)[i].verts.size(); j++)
			{
				POINT_FLOAT temp;
				temp.x = (float)(*info)[i].verts[j].x/m_nScale;
				temp.y = (float)(*info)[i].verts[j].y/m_nScale;
				cArea.AddPoint(temp);
			}
			cArea.AreaFinish();
			if( cArea.IsInArea(pf) )
			{
				m_nHoverAreaIndex = i;
				UpdateHint();
				break;
			}
		}
	}
}

void CDlgTerrWarMap::OnEventLButtonUp(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if( !m_bMOpen && m_nHoverAreaIndex != -1 && m_iStatus & ST_BID)
	{
		A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
		int x = GET_X_LPARAM(lParam) - p->X;
		int y = GET_Y_LPARAM(lParam) - p->Y;
		vector<DOMAIN_INFO> * domainInfo = globaldata_getdomaininfos2();
		if ((*domainInfo)[m_nHoverAreaIndex].deposit>0)
		{
			GetGameUIMan()->GetDialog("Win_Battle_TerriWarAdd")->SetPosEx(x, y);
			GetGameUIMan()->GetDialog("Win_Battle_TerriWarAdd")->SetData(m_nHoverAreaIndex);
			GetGameUIMan()->GetDialog("Win_Battle_TerriWarAdd")->Show(true);
		}
		else
		{
			GetGameUIMan()->GetDialog("Win_Battle_TerriDeclare")->SetPosEx(x, y);
			GetGameUIMan()->GetDialog("Win_Battle_TerriDeclare")->SetData(m_nHoverAreaIndex);
			GetGameUIMan()->GetDialog("Win_Battle_TerriDeclare")->GetDlgItem("Btn_Declare")->Show(m_vecAreaInfo[m_nHoverAreaIndex].bCanDeclare);
			GetGameUIMan()->GetDialog("Win_Battle_TerriDeclare")->Show(true);
		}
	}
}

void CDlgTerrWarMap::OnEventRButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	m_nStartX = GET_X_LPARAM(lParam);
	m_nStartY = GET_Y_LPARAM(lParam);
	m_bDrag = true;
}

void CDlgTerrWarMap::OnEventRButtonUp(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	m_bDrag = false;
}

#define DRAWSPRITE(pSprite) \
{ pSprite->DrawToBack(ptCenter.x+m_nX*(RenderDetail()? 1 : 0)-(int)(13*fWindowScale), ptCenter.y+m_nY*(RenderDetail()? 1 : 0)-(int)(8*fWindowScale)); \
bDraw = true; }
bool CDlgTerrWarMap::Render()
{
	//地图
	float fWindowScale = GetGameUIMan()->GetWindowScale();
	A3DPOINT2 pt = PixelToScreen(A3DPOINT2(0, 0));
	m_pA2DSpriteIcon[SI_MAP]->DrawToBack(pt.x+m_nX, pt.y+m_nY);
	//地图标示 帮派图标-已宣战区域-自己帮派战斗地图-神器  能宣战-所有战斗-箭头
	vector<DOMAIN_INFO> *info = globaldata_getdomaininfos2();
	UINT i, j, k;
	bool bFlash = false;
	int idFaction = GetHostPlayer()->GetFactionID();
	for(i = 0; i < m_vecAreaInfo.size(); i++)
	{
		bFlash = false;
		bool bDraw = false;
		DOMAIN_INFO di = (*info)[i];
		//能宣战
		A3DPOINT2 ptCenter = PixelToScreen(di.center);
		if (m_bOwnTerr && m_vecAreaInfo[i].bCanDeclare)
		{
			bFlash = true;
		}
		//已宣战待确定
		if ( (m_iStatus&ST_BID) && di.id_attacker==0 && (di.deposit>0 || di.challenged_byself) )
		{
			bFlash = true;
		}
		else if (m_iStatus&ST_ASSISGET)
		{
			if (di.id_attacker>0)
			{
				if (idFaction>0 && (di.id_owner==idFaction || di.id_attacker==idFaction))
					bFlash = true;
			}
		}
		//战斗中
		else if (( m_iStatus&ST_FIGHT)&& di.id_attacker>0&&di.flag<=0)
		{
			if (idFaction>0 && (di.id_attacker==idFaction || di.id_defender == idFaction))
			{
				bFlash = true;
			}
		}
		//神器
		else if (di.random_award>0)
		{
			bFlash = true;
		}
		RenderArea(i, ulColor[m_vecAreaInfo[i].nColorIndex]);				// 帮派占领颜色
		if (i==(UINT)m_nHoverAreaIndex)
			RenderArea(m_nHoverAreaIndex, A3DCOLORRGBA(255, 255, 255, 128));	// 鼠标悬浮
		else if(bFlash)
		{
			if(m_dwFlashTime > 1000)
				RenderArea(i, A3DCOLORRGBA(255, 255, 255, 100));				// Flash
		}
	}
	
	//箭头
	for(i = 0; i < m_vecAreaInfo.size(); i++)
	{
		bool bDraw = false;
		DOMAIN_INFO di = (*info)[i];
		A3DPOINT2 ptCenter = PixelToScreen(di.center);
		//已宣战待确定
		if ( (m_iStatus&ST_BID) && di.id_attacker==0 && (di.deposit>0 || di.challenged_byself) )
		{
			DRAWSPRITE(m_pA2DSpriteIcon[SI_DECLARED]);
		}
		else if (m_iStatus&ST_ASSISGET)
		{
			if (di.id_attacker>0)
			{
				DRAWSPRITE(m_pA2DSpriteIcon[SI_DECLARE_SUCCESS]);
			}
		}
		//战斗中
		else if (( m_iStatus&ST_FIGHT)&& di.id_attacker>0&&di.flag<=0)
		{
			DRAWSPRITE(m_pA2DSpriteIcon[SI_FIGHT]);
		}
		//神器
		else if (di.random_award>0)
		{
			DRAWSPRITE(m_pA2DSpriteIcon[SI_GODITEM]);
		}
		//帮派图标
		if (!bDraw && di.id_owner>0)
		{
			int nIndex = 0;
			A2DSprite *sprite = GetGameUIMan()->GetFactionIcon(GetGame()->GetGameInit().iServerID, di.id_owner, &nIndex);
			sprite->SetCurrentItem(nIndex);
			DRAWSPRITE(sprite);
		}
		if ((m_iStatus&(ST_ASSISGET|ST_FIGHT))&&di.id_attacker>0&&di.flag<=0)
		{
			for (j=0; j<di.neighbours.size(); j++)
			{
				for (k=0; k<m_vecAreaInfo.size(); k++)
				{
					if (di.neighbours[j]==(*info)[k].id && di.id_attacker == (*info)[k].id_owner)
					{
						if ((*info)[k].id_attacker>0 && (*info)[k].id_attacker == (*info)[i].id_owner)
							RenderArrow(k, i, true);
						else
							RenderArrow(k, i, false);
						j = 9999;
						break;
					}
				}
				
			}
		}
	}

	return CDlgBase::Render();
}

void CDlgTerrWarMap::RenderArea(int nIndex, A3DCOLOR cl)
{
	if (cl == 0)
	{
		return;
	}
	vector<DOMAIN_INFO> *info = globaldata_getdomaininfos2();
	DWORD i;
	A3DFlatCollector *pFC = GetGame()->GetA3DEngine()->GetA3DFlatCollector();
	A3DVECTOR3 *verts = new A3DVECTOR3[(*info)[nIndex].verts.size()];
	WORD *indices = new WORD[(*info)[nIndex].faces.size()];
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	for(i = 0; i < (*info)[nIndex].verts.size(); i++ )
	{
		A3DPOINT2 pt = PixelToScreen((*info)[nIndex].verts[i]);
		verts[i].x = (float)pt.x + p->X + m_nX;
		verts[i].y = (float)pt.y + p->Y + m_nY;
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

void CDlgTerrWarMap::RenderArrow(int nIndex1, int nIndex2, bool bDouble)
{
	vector<DOMAIN_INFO> *info = globaldata_getdomaininfos2();
	A3DPOINT2 pt1 = PixelToScreen((*info)[nIndex1].center);
	A3DPOINT2 pt2 = PixelToScreen((*info)[nIndex2].center);
	float fDegree = RAD2DEG((float)atan2(float(pt2.y - pt1.y), float(pt2.x - pt1.x)));
	int nDegree = (int)(fDegree>=0 ? fDegree+0.5 : fDegree-0.5);
	int dx = pt2.x - pt1.x;
	int dy = pt2.y - pt1.y;
	float fLength = (float)sqrt(float(dy * dy + dx * dx));

	A2DSprite* pArrow;
	if (!bDouble)
	{
		if (fLength<60.0)
			pArrow = m_pA2DSpriteIcon[SI_ARROW1];
		else if (fLength<200.0)
			pArrow = m_pA2DSpriteIcon[SI_ARROW2];
		else
			pArrow = m_pA2DSpriteIcon[SI_ARROW3];
	}
	else
	{
		if (fLength<60.0)
			pArrow = m_pA2DSpriteIcon[SI_ARROW_DOUBLE1];
		else if (fLength<200.0)
			pArrow = m_pA2DSpriteIcon[SI_ARROW_DOUBLE2];
		else
			pArrow = m_pA2DSpriteIcon[SI_ARROW_DOUBLE3];
	}
	float fScale = float(fLength - 20) / pArrow->GetWidth();
	pArrow->SetDegree(nDegree);
	pArrow->SetScaleX(fScale);
	pArrow->SetScaleY(fScale);
	pArrow->DrawToBack((pt2.x + pt1.x) / 2 + m_nX, (pt2.y + pt1.y) / 2 + m_nY);
}

void CDlgTerrWarMap::OnTick()
{
	static DWORD dwTimeDebug = 0;
	dwTimeDebug += GetGame()->GetTickTime();
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		if (dwTimeDebug>1000)
		{
			dwTimeDebug = 0;
			m_bDebug = !m_bDebug;
		}
	}
	m_dwFlashTime += GetGame()->GetTickTime();
	if (m_dwFlashTime>2000)
	{
		m_dwFlashTime = 0;
	}
}

void CDlgTerrWarMap::UpdateHint()
{
	ACString strText;
	AString strName;
	abase::vector<DOMAIN_INFO> *info = globaldata_getdomaininfos2();
	if (m_nHoverAreaIndex<0)
	{
		m_pImg_WorldMap->SetHint(_AL(""));
		return;
	}
	DOMAIN_INFO di = (*info)[m_nHoverAreaIndex];
	PAUIDIALOG pDlgHint = GetGameUIMan()->GetDialog("Win_HintDialog");
	PAUILABEL pLab[6];
	int i;
	for (i=0; i<6; i++)
	{
		strName.Format("Lab_%d", i+1);
		pLab[i] = (PAUILABEL)pDlgHint->GetDlgItem(strName);
	}
	PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)pDlgHint->GetDlgItem("Img_Territory");
	pImg->SetCover(m_pImageMapIcon, di.id);
	strText.Format(GetGameUIMan()->GetStringFromTable(9326), di.name);
	pLab[0]->SetText(strText);
	strText.Format(GetGameUIMan()->GetStringFromTable(9327), GetGameUIMan()->GetStringFromTable(9333+di.type));
	pLab[1]->SetText(strText);
	if ((m_iStatus&ST_FIGHT)>0 || ((m_iStatus&ST_BID) && di.id_attacker>0))
	{
		Faction_Info* fInfoOwn =  GetGame()->GetFactionMan()->GetFaction(di.id_defender, true);
		strText.Format(GetGameUIMan()->GetStringFromTable(9356), (di.id_defender>0 && fInfoOwn) ? fInfoOwn->GetName() : GetGameUIMan()->GetStringFromTable(780));
	}
	else
	{
		Faction_Info* fInfoOwn =  GetGame()->GetFactionMan()->GetFaction(di.id_owner, true);
		strText.Format(GetGameUIMan()->GetStringFromTable(9328), (di.id_owner>0 && fInfoOwn) ? fInfoOwn->GetName() : GetGameUIMan()->GetStringFromTable(780));
	}
	pLab[2]->SetText(strText);
	PAUIIMAGEPICTURE pImgGodItem = (PAUIIMAGEPICTURE)pDlgHint->GetDlgItem("Img_Treasure");
	pLab[3]->Show(false);
	pLab[4]->Show(false);
	pLab[5]->Show(false);
	pImgGodItem->Show(false);

	if (di.random_award>0)
	{
		pLab[5]->Show(true);
		pImgGodItem->Show(true);
		GetGameUIMan()->SetIvtrInfo(pImgGodItem, di.random_award, di.random_award_num);
		CECIvtrItem* pItem = CECIvtrItem::CreateItem(di.random_award, 0, 1);
		if (pItem)
		{
			pLab[5]->SetText(pItem->GetName());
			delete pItem;
		}
	}
	else if (m_iStatus & ST_BID)
	{
		pLab[3]->SetColor(A3DCOLORRGB(255, 0, 0));
		pLab[3]->Show(true);
		pLab[3]->SetText(GetGameUIMan()->GetStringFromTable(9344));
	}
	else if (m_iStatus & ST_FIGHT || m_iStatus & ST_ASSISGET)
	{
		pLab[3]->Show(true);
		
		if (di.id_attacker>0)
		{
			Faction_Info* fInfoOwn =  GetGame()->GetFactionMan()->GetFaction(di.id_attacker, true);
			if (fInfoOwn)
				strText.Format(GetGameUIMan()->GetStringFromTable(9329), fInfoOwn->GetName());
			else
				strText.Format(GetGameUIMan()->GetStringFromTable(9329), GetGameUIMan()->GetStringFromTable(780));
			pLab[3]->SetColor(A3DCOLORRGB(255, 0, 0));
		}
		else
		{
			strText.Format(GetGameUIMan()->GetStringFromTable(9329), GetGameUIMan()->GetStringFromTable(780));
			pLab[3]->SetColor(A3DCOLORRGB(55, 145, 205));
		}
		pLab[3]->SetText(strText);
		
		
		if (di.id_attacker>0)
		{
			if (di.flag>0)
			{
				strText = GetGameUIMan()->GetStringFromTable(di.flag==1 ? 9332 : 9331);
			}
			else
			{
				const long battleTime = di.battle_time_start - GetGame()->GetTimeZoneBias() * 60;
				tm *gtime = gmtime((const time_t*)&battleTime);
				int startHour = gtime->tm_hour;
				int startMin = gtime->tm_min;
				const long battleTimeEnd = di.battle_time_end - GetGame()->GetTimeZoneBias() * 60;
				tm *gtimeEnd = gmtime((const time_t*)&battleTimeEnd);
				strText.Format(GetGameUIMan()->GetStringFromTable(9330), GetGameUIMan()->GetStringFromTable(9890+gtime->tm_wday),
					startHour, startMin, gtimeEnd->tm_hour, gtimeEnd->tm_min);
			}
			
			pLab[4]->Show(true);
			pLab[4]->SetColor(A3DCOLORRGB(55, 145, 205));
			pLab[4]->SetText(strText);
		}
	}
	if (0)//m_bDebug)
	{
		strText.Format(_AL("index: %d\r id: %d\r owner:%d\r defencer:%d\r attacker:%d\r deposit:%d\r color:%d"),
					m_nHoverAreaIndex,   di.id,  di.id_owner, di.id_defender,di.id_attacker, di.deposit, di.color);
		m_pImg_WorldMap->SetHint(strText);
	}
	else
	{
		int cxLable = 0;
		for (i=0; i<6; i++)
		{
			if (pLab[i]->IsShow())
			{
				int nWidth, nHeight, nLines;
				A3DFTFont *pFont = pLab[i]->GetFont();
				pFont->Resize(pLab[i]->GetFontHeight() * GetGameUIMan()->GetWindowScale());
				AUI_GetTextRect(pFont, pLab[i]->GetText(), nWidth, nHeight, nLines, 3);
				int nDlgWidth = pLab[i]->GetPos().x - pDlgHint->GetPos().x + (int)(15 * GetGameUIMan()->GetWindowScale()) + nWidth;
				if (nDlgWidth>cxLable)
				{
					cxLable = nDlgWidth;
				}
			}
		}
		pDlgHint->SetSize(cxLable, pDlgHint->GetSize().cy);
		m_pImg_WorldMap->SetHint(_AL("^DIALOG:Win_HintDialog"));
	}
}

void CDlgTerrWarMap::UpdateMapInfo(int status, bool fromServer)
{
	if (status>=0)
	{
		m_iStatus = status;
	}
	int iFactionId = GetHostPlayer()->GetFactionID();
	vector<DOMAIN_INFO> *info = globaldata_getdomaininfos2();
	if (GetGameUIMan()->GetDialog("Win_CharHead"))
	{
		PAUIOBJECT pObjWar = GetGameUIMan()->GetDialog("Win_CharHead")->GetDlgItem("Btn_BattleTerri");
		bool bShow = false;
		// 在战场中
		if (GetHostPlayer()->GetTerritoryBattleInfo()>0)
		{
			bShow = true;
		}
		// 所在帮会有战争
		if (!bShow && iFactionId>0 && (m_iStatus & (ST_FIGHT|ST_ASSISGET)))
		{
			for (UINT i=0; i<info->size(); i++)
			{
				DOMAIN_INFO di = (*info)[i];
				if (di.id_attacker == iFactionId || di.id_defender == iFactionId)
				{
					bShow = true;
					break;
				}
			}
		}
		pObjWar->Show(bShow);
	}
	DWORD i, j;
	int colorIndex = 0;
	int idFRole = GetHostPlayer()->GetFRoleID();
	int idFaction = GetHostPlayer()->GetFactionID();
	int nLowNum = 0;		//占领的低级区域数量
	int nMidNum = 0;		//占领的中级区域数量
	bool bAward[4] = {false, false, false, false};	//判断领取奖励服务是否显示，共4种
	int nDeclaredId = -1;
	bool bWinType;
	m_vecAreaInfo.clear();
	m_bOwnTerr = false;
	int occupy[3] = {0,0,0};
	int unOccupy[3] = {0,0,0};
	for(i = 0; i < info->size(); i++ )
	{
		DOMAIN_INFO di = (*info)[i];
		if (di.type==DOMAIN_TYPE_3RD_CLASS)
		{
			occupy[0]++;
			if (di.id_owner==0)
				unOccupy[0]++;
		}
		else if (di.type==DOMAIN_TYPE_2ND_CLASS)
		{
			occupy[1]++;
			if (di.id_owner==0)
				unOccupy[1]++;
		}
		else if (di.type==DOMAIN_TYPE_1ST_CLASS)
		{
			occupy[2]++;
			if (di.id_owner==0)
				unOccupy[2]++;
		}
		if (di.deposit>0||di.id_attacker>0)
		{
			nDeclaredId = di.id;
		}
		if (fromServer)
		{
			if (di.id_owner>0)
			{
				Faction_Info* fInfo = GetGame()->GetFactionMan()->GetFaction(di.id_owner, true);
			}
			if (di.id_attacker>0)
			{
				Faction_Info* fInfo = GetGame()->GetFactionMan()->GetFaction(di.id_attacker, true);
			}
			if (di.id_defender>0)
			{
				Faction_Info* fInfo = GetGame()->GetFactionMan()->GetFaction(di.id_defender, true);
			}
		}
		A3DRECT rect;
		rect.SetRect(9999, 9999, 0, 0);
		for(j = 0; j < (*info)[i].verts.size(); j++)
		{
			if( di.verts[j].x < rect.left )
				rect.left = di.verts[j].x;
			else if( di.verts[j].x > rect.right )
				rect.right = di.verts[j].x;
			if( di.verts[j].y < rect.top )
				rect.top = di.verts[j].y;
			else if( di.verts[j].y > rect.bottom )
				rect.bottom = di.verts[j].y;
		}
		rect.left/=m_nScale;
		rect.right/=m_nScale;
		rect.top/=m_nScale;
		rect.bottom/=m_nScale;
		AreaInfoMap aInfo;
		aInfo.rc = rect;
		aInfo.nColorIndex = di.color;
		aInfo.bCanDeclare = false;	
		aInfo.nIndex = di.id - 1;
		m_vecAreaInfo.push_back(aInfo);
		if (idFaction == di.id_owner)
		{
			if (di.type==DOMAIN_TYPE_3RD_CLASS)
				nLowNum++;
			else if (di.type==DOMAIN_TYPE_2ND_CLASS)
				nMidNum++;
			m_bOwnTerr = true;
		}
		if (di.random_award>0)
		{
			CECIvtrItem* pItem = CECIvtrItem::CreateItem(di.random_award, 0, 1);
			if (pItem)
			{
				AString strFile;
				af_GetFileTitle(pItem->GetIconFile(), strFile);
				strFile.MakeLower();
				m_nHintImageFrame = GetGameUIMan()->GetIconId(CECGameUIMan::ICONS_INVENTORY, strFile);
				
				AUICTranslate trans;
				m_strItemDesc = trans.Translate(pItem->GetDesc());
				delete pItem;
			}
		}
		if (idFRole == GNET::_R_MASTER)
		{
			if ((m_iStatus&ST_BID)==0 && di.id_owner!=idFaction && di.deposit>0)
			{
				bAward[1] = true;
			}
			if (m_iStatus==0 && di.id_owner==idFaction && di.deposit>0)
			{
				bWinType = (di.id_owner == di.id_attacker);
				bAward[2] = true;
			}
			if (m_iStatus==0 && di.random_award>0 && di.id_owner==idFaction && di.random_award_drawn==0)
			{
				bAward[3] = true;
			}
		}
	}

	ACString strText;
	PAUILABEL pLab = (PAUILABEL)GetDlgItem("Txt_Occupy1");
	strText.Format(_AL("%d"), occupy[2]);
	pLab->SetText(strText);
	pLab = (PAUILABEL)GetDlgItem("Txt_Occupy2");
	strText.Format(_AL("%d"), occupy[1]);
	pLab->SetText(strText);
	pLab = (PAUILABEL)GetDlgItem("Txt_Occupy3");
	strText.Format(_AL("%d"), occupy[0]);
	pLab->SetText(strText);
	pLab = (PAUILABEL)GetDlgItem("Txt_Unoccupy1");
	strText.Format(_AL("%d"), unOccupy[2]);
	pLab->SetText(strText);
	pLab = (PAUILABEL)GetDlgItem("Txt_Unoccupy2");
	strText.Format(_AL("%d"), unOccupy[1]);
	pLab->SetText(strText);
	pLab = (PAUILABEL)GetDlgItem("Txt_Unoccupy3");
	strText.Format(_AL("%d"), unOccupy[0]);
	pLab->SetText(strText);

	if (m_iStatus & ST_BID && idFRole == GNET::_R_MASTER && nDeclaredId<0)
	{
		CECFactionMan *pFMan = GetGame()->GetFactionMan();
		Faction_Info *pInfo = pFMan->GetFaction(idFaction);
		int iFacLevel = pInfo->GetLevel();
		
		for(i = 0; i < info->size(); i++ )
		{
			DOMAIN_INFO di = (*info)[i];

			if (!m_bOwnTerr && di.type==DOMAIN_TYPE_3RD_CLASS && iFacLevel>=3 && di.id_owner != idFaction)
			{
				m_vecAreaInfo[i].bCanDeclare = true;
			}
			if (di.id_owner == idFaction)
			{
				for (j=0; j<di.neighbours.size(); j++)
				{
					DOMAIN_INFO din = (*info)[di.neighbours[j]-1];
					if (din.type==DOMAIN_TYPE_3RD_CLASS && iFacLevel>=3 && din.id_owner != idFaction)
					{
						m_vecAreaInfo[di.neighbours[j]-1].bCanDeclare = true;
					}
					else if (din.type==DOMAIN_TYPE_2ND_CLASS && iFacLevel>=4 && nLowNum>=2 && din.id_owner != idFaction)
					{
						m_vecAreaInfo[di.neighbours[j]-1].bCanDeclare = true;
					}
					else if (din.type==DOMAIN_TYPE_1ST_CLASS && iFacLevel>=5 && nMidNum>=2 && din.id_owner != idFaction)
					{
						m_vecAreaInfo[di.neighbours[j]-1].bCanDeclare = true;
					}
				}
			}
		}
	}

	if (GetGameUIMan()->GetDialog("Win_NPC") && GetGameUIMan()->GetDialog("Win_NPC")->IsShow())
	{
		CECNPCMan *pNPCMan = GetWorld()->GetNPCMan();
		CECNPC *pNPC = pNPCMan->GetNPC(GetHostPlayer()->GetCurServiceNPC());
		PAUILISTBOX  pLst_Main = (PAUILISTBOX)GetGameUIMan()->GetDialog("Win_NPC")->GetDlgItem("Lst_Main");

		if (idFRole == GNET::_R_MASTER && (m_iStatus&ST_ASSISGET))
		{
			for(int i = 0; i < (int)info->size(); i++ )
			{
				DOMAIN_INFO di = (*info)[i];
				if (idFaction==di.id_defender && di.assis_num>di.assis_drawn_num)
				{
					bAward[0] = true;
					break;
				}
			}
		}

		for (i=0; i<(DWORD)pLst_Main->GetCount(); i++)
		{
			if (pLst_Main->GetText(i) == GetStringFromTable(9325))
			{
				bAward[0] = false;
			}
			if (pLst_Main->GetText(i) == GetStringFromTable(9350))
			{
				bAward[1] = false;
			}
			if (pLst_Main->GetText(i) == GetStringFromTable(9351) || pLst_Main->GetText(i) == GetStringFromTable(9355))
			{
				bAward[2] = false;
			}
			if (pLst_Main->GetText(i) == GetStringFromTable(9352))
			{
				bAward[3] = false;
			}
		}
		if (pNPC && (((CECNPCServer*)pNPC)->GetDBEssence()->combined_services2 & 0x200))
		{
			if (bAward[0])
			{
				pLst_Main->InsertString(pLst_Main->GetCount() - 1, GetStringFromTable(9325));	//9325 "领取佣兵凭证"				周四晚22:00-周五晚20:00
				pLst_Main->SetItemData(pLst_Main->GetCount() - 2, 0xfffff39);
			}
			if (bAward[1])
			{
				pLst_Main->InsertString(pLst_Main->GetCount() - 1, GetStringFromTable(9350));	//9350 "领取竞标失败宣战金"		周四晚22:00-下周四20:00
				pLst_Main->SetItemData(pLst_Main->GetCount() - 2, 0xfffff39);
			}
			if (bAward[2])
			{
				pLst_Main->InsertString(pLst_Main->GetCount() - 1, GetStringFromTable(bWinType ? 9351 : 9355));	//9351 "领取竞标成功宣战金"		周日晚22:00-下周四20:00
				pLst_Main->SetItemData(pLst_Main->GetCount() - 2, 0xfffff39);
			}
			if (bAward[3])
			{
				pLst_Main->InsertString(pLst_Main->GetCount() - 1, GetStringFromTable(9352));
				pLst_Main->SetItemData(pLst_Main->GetCount() - 2, 0xfffff3a);	//9352 "领取神器"					周日晚22:00-下周四20:00
			}
		}
	}
}

void CDlgTerrWarMap::GetAward(int id)
{
	if (GetHostPlayer()->GetPack()->SearchEmpty()<0)
	{
		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(7076), MB_OK, A3DCOLORRGBA(255,255,255,160));
		return;
	}
	vector<DOMAIN_INFO> *info = globaldata_getdomaininfos2();
	for(UINT i = 0; i < info->size(); i++ )
	{
		DOMAIN_INFO di = (*info)[i];
		int idFaction = GetHostPlayer()->GetFactionID();
		int idFRole = GetHostPlayer()->GetFRoleID();
		if (idFRole == GNET::_R_MASTER)
		{
			if (m_iStatus&ST_ASSISGET && idFaction==di.id_defender)
			{
				GetGameUIMan()->GetDialog("Win_Battle_TerriHire")->SetData(i);
				GetGameUIMan()->GetDialog("Win_Battle_TerriHire")->Show(true);
				return;
			}
			//0 佣兵凭证 1 竞标失败领取的宣战金 2 领土战胜利领取的宣战金 3 神器 在单独服务中
			else if ((m_iStatus&ST_BID)==0 && di.id_owner!=idFaction && di.deposit>0)
			{
				GetGameSession()->c2s_CmdNPCSevTerritoryAward(idFaction, di.id, 2, di.challenge_itemid, di.deposit, 0);
				return;
			}
			else if (m_iStatus==0 && di.id_owner==idFaction && di.deposit>0)
			{
				GetGameUIMan()->GetDialog("Win_Battle_TerriAward")->Show(true);
				return;
			}
		}
	}
}

void CDlgTerrWarMap::ChangeSize() 
{
	float fWindowScale = GetGameUIMan()->GetWindowScale();
	PAUIOBJECT pButton = NULL;
	PAUIDIALOG pDlgButton = GetGameUIMan()->GetDialog("Win_Worldmap_Button");
	if (pDlgButton)
	{
		pButton = GetGameUIMan()->GetDialog("Win_Worldmap_Button")->GetDlgItem("Btn_ZoomOut");
	}
	if (m_nScale==1)
	{
		m_nScale = 2;
//		if(pButton) pButton->SetText(GetGameUIMan()->GetStringFromTable(9353));
	}
	else
	{
		m_nScale = 1;
//		if(pButton) pButton->SetText(GetGameUIMan()->GetStringFromTable(9354));
	}
	GetDlgItem("Lab_1")->Show(m_nScale==2);
	GetDlgItem("Img_1")->Show(m_nScale==2);
	GetDlgItem("Lab_2")->Show(m_nScale==2);
	GetDlgItem("Img_2")->Show(m_nScale==2);
	GetDlgItem("Lab_3")->Show(m_nScale==2);
	GetDlgItem("Img_3")->Show(m_nScale==2);
	GetDlgItem("Lab_4")->Show(m_nScale==2);
	GetDlgItem("Img_4")->Show(m_nScale==2);

	GetDlgItem("Txt_Occupy1")->Show(m_nScale==2);
	GetDlgItem("Lab_Occupy1")->Show(m_nScale==2);
	GetDlgItem("Txt_Occupy2")->Show(m_nScale==2);
	GetDlgItem("Lab_Occupy2")->Show(m_nScale==2);
	GetDlgItem("Txt_Occupy3")->Show(m_nScale==2);
	GetDlgItem("Lab_Occupy3")->Show(m_nScale==2);
	GetDlgItem("Txt_Unoccupy1")->Show(m_nScale==2);
	GetDlgItem("Lab_Unoccupy1")->Show(m_nScale==2);
	GetDlgItem("Txt_Unoccupy2")->Show(m_nScale==2);
	GetDlgItem("Lab_Unoccupy2")->Show(m_nScale==2);
	GetDlgItem("Txt_Unoccupy3")->Show(m_nScale==2);
	GetDlgItem("Lab_Unoccupy3")->Show(m_nScale==2);
	GetDlgItem("Shape_Bg")->Show(m_nScale==2);

	GetDlgItem("Btn_TerriDetail")->Show(m_nScale==2);

	if (pDlgButton)
	{
		pDlgButton->GetDlgItem("Btn_ZoomOut")->Show(m_nScale==1);
		pDlgButton->GetDlgItem("Img_ZoomOut")->Show(m_nScale==1);
		pDlgButton->GetDlgItem("Btn_ZoomIn")->Show(m_nScale==2);
		pDlgButton->GetDlgItem("Img_ZoomIn")->Show(m_nScale==2);
	}

	{
		fWindowScale = fWindowScale/GetGameUIMan()->GetCustomUIScale();
	}
	m_pA2DSpriteIcon[SI_MAP]->SetScaleX(fWindowScale/m_nScale);
	m_pA2DSpriteIcon[SI_MAP]->SetScaleY(fWindowScale/m_nScale);
	m_nX = 0;
	m_nY = 0;
	m_bDrag = false;
	m_nStartX = -1;
	m_nStartY = -1;
	UpdateMapInfo(m_iStatus, false);
}