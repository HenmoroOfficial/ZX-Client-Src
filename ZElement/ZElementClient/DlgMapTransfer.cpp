// Filename	: DlgMapTransfer.cpp
// Creator	: WuWeixin
// Date		: 2012/8/20

#include "A3DStream.h"
#include "A2DSprite.h"
#include "A3DTexture.h"
#include "A3DDevice.h"
#include "DlgMapTransfer.h"
#include "DlgMiniMap.h"
#include "EC_Global.h"
#include "EC_GameUIMan.h"
#include "EC_World.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_Instance.h"
#include "EC_HostPlayer.h"
#include "elementdataman.h"
#include "globaldataman.h"
#include "ClosedArea.h"
#include "A3DFlatCollector.h"
#include "DlgWorldMap.h"

const int CDlgMapTransfer::ms_uitransferConfigId = 685;

CDlgMapTransfer::MAPTLVERTEX CDlgMapTransfer::m_vertexMidMap[4];
A3DStream * CDlgMapTransfer::m_pMapStream = NULL;
A2DSprite * CDlgMapTransfer::m_pA2DMapInactiveMark = NULL;
A2DSprite * CDlgMapTransfer::m_pA2DMapActiveMark = NULL;
A3DTexture * CDlgMapTransfer::m_pTexMap = NULL;

const int	CDlgMapTransfer::m_isTransIdCount	= 256;
const int	CDlgMapTransfer::m_isTransPointCount= 96;

const char* szMapInactiveMarkFile = "icon\\TransferMapActiveN.dds";
const char* szMapActiveMarkFile = "icon\\TransferMapActiveY.dds";

#define TO_SCREEN_X(posX) int(center.x + int((float)m_rcMap.Width() / m_instanceSize.x * posX))
#define TO_SCREEN_Y(posY) int(center.y - int((float)m_rcMap.Height() / m_instanceSize.y * posY))

extern abase::vector<DOMAIN_INFO>* GetSuitDomainInfo(int nType);

AUI_BEGIN_COMMAND_MAP(CDlgMapTransfer, CDlgBase)
AUI_ON_COMMAND("IDCANCEL", OnCommand_CANCEL)
AUI_ON_COMMAND("Btn_Worldmaps", OnCommand_Return)
AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgMapTransfer, CDlgBase)
AUI_ON_EVENT("Img_Map",	WM_MOUSEMOVE, OnEventMouseMove_Map)
AUI_ON_EVENT("Img_Map",	WM_LBUTTONUP, OnEventLButtonUp_Map)
AUI_ON_EVENT("Img_Map",	WM_RBUTTONUP, OnEventRButtonUp_Map)
AUI_END_EVENT_MAP()


CDlgMapTransfer::CDlgMapTransfer()
: m_pImg_Map(NULL), m_pLable_Title(NULL)
, m_currentTransId(0), m_pTransService(NULL)
, m_iIdMap(0)
, m_iMapType(MAP_MAIN)
{
	for(int i = 0; i < 4; ++ i)
	{
		m_vertexMidMap[i].z = 0.0f;
		m_vertexMidMap[i].rhw = 1.0f;
		m_vertexMidMap[i].diffuse = 0xFFFFFFFF;
		m_vertexMidMap[i].specular = 0xFF000000;
	}
}

CDlgMapTransfer::~CDlgMapTransfer()
{
	A3DRELEASE(m_pMapStream);
	A3DRELEASE(m_pA2DMapInactiveMark);
	A3DRELEASE(m_pA2DMapActiveMark);
	A3DRELEASE(m_pTexMap);
}

void CDlgMapTransfer::InitTex()
{
	if(!m_pMapStream)
	{
		m_pMapStream = new A3DStream();
		if( !m_pMapStream->Init(m_pA3DDevice, sizeof(MAPTLVERTEX), FVF_MAPTLVERTEX, 6, 0, A3DSTRM_REFERENCEPTR, 0) )
		{
			AUI_ReportError(__LINE__, __FILE__);
			return;
		}
	}

	if (!m_pA2DMapInactiveMark)
	{
		m_pA2DMapInactiveMark = new A2DSprite;
		if (!m_pA2DMapInactiveMark->Init(m_pA3DDevice, szMapInactiveMarkFile, AUI_COLORKEY))
		{
			AUI_ReportError(__LINE__, __FILE__);
			return;
		}
	}
	if (!m_pA2DMapActiveMark) 
	{
		m_pA2DMapActiveMark = new A2DSprite; 
		if (!m_pA2DMapActiveMark->Init(m_pA3DDevice, szMapActiveMarkFile, AUI_COLORKEY))
		{
			AUI_ReportError(__LINE__, __FILE__);
			return;
		}
	}
}

bool CDlgMapTransfer::OnInitDialog()
{
	m_pImg_Map = (PAUIIMAGEPICTURE)GetDlgItem("Img_Map");
	m_pLable_Title = (PAUILABEL)GetDlgItem("Title");
	
	return true;
}

void CDlgMapTransfer::OnShowDialog()
{
	m_nHoverAreaIndex = -1;
	GetDlgItem("Btn_Worldmaps")->Show(true);
	GetDlgItem("Btn_Return")->Show(false);
	m_iIdMap = GetWorld()->GetInstanceID();
	if(IsHongHuangMap(m_iIdMap))
		m_iMapType = MAP_HONGHUANGXX;
	else
		m_iMapType = MAP_NORMALXX;
	InitTex();
}

void CDlgMapTransfer::OnHideDialog()
{
	m_vecTransPoints.clear();
	m_currentTransId = 0;
	m_iIdMap = 0;
	GetGameUIMan()->EndNPCService();
}

void CDlgMapTransfer::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
}

A3DPOINT2 CDlgMapTransfer::ScreenToPixel(A3DPOINT2 pt)
{
	A3DPOINT2 ptCenter = m_pImg_Map->GetRect().CenterPoint();
	SIZE size;
	size.cx = m_pImg_Map->GetImage()->GetWidth();
	size.cy = m_pImg_Map->GetImage()->GetHeight();
	A3DRECT rc = m_pImg_Map->GetRect();
	pt.x = size.cx / 2 + (pt.x - ptCenter.x) * size.cx / rc.Width();
	pt.y = size.cy / 2 + (pt.y - ptCenter.y) * size.cy / rc.Height();

	return pt;
}

A3DPOINT2 CDlgMapTransfer::PixelToScreen(A3DPOINT2 pt)
{
	A3DPOINT2 ptCenter = m_pImg_Map->GetRect().CenterPoint();
	SIZE size;
	size.cx = m_pImg_Map->GetImage()->GetWidth();
	size.cy = m_pImg_Map->GetImage()->GetHeight();
	A3DRECT rc = m_pImg_Map->GetRect();
	if(m_iMapType==MAP_MAIN||m_iMapType==MAP_CROSS||m_iMapType==MAP_HONGHUANG)
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

void CDlgMapTransfer::OnEventMouseMove_Map(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if (m_iIdMap==0)
	{
		A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();

		A3DPOINT2 pt;
		pt.x = GET_X_LPARAM(lParam) - p->X;
		pt.y = GET_Y_LPARAM(lParam) - p->Y;
		pt = ScreenToPixel(pt);
		abase::vector<DOMAIN_INFO> *info = GetSuitDomainInfo(m_iMapType);
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
					
					if(m_iMapType==MAP_MAIN||m_iMapType==MAP_CROSS||m_iMapType==MAP_HONGHUANG)
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
	}
	else
	{
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);

		pObj->SetHint(_AL(""));

		size_t count = m_vecMapMark.size();
		for(size_t i = 0; i < count; ++ i)
		{
			if( m_vecMapMark[i].rect.PtInRect(x, y) )
			{
				pObj->SetHint(m_vecMapMark[i].strHint);
				pObj->SetHintFollow(true);
				return;
			}
		}
	}
}

void CDlgMapTransfer::OnEventLButtonUp_Map(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);

	if( m_iIdMap == 0 )
	{
		if( m_nHoverAreaIndex != -1 )
		{
			vector<DOMAIN_INFO> *info = GetSuitDomainInfo(m_iMapType);
			m_iIdMap = (*info)[m_nHoverAreaIndex].id;
			if (m_iMapType==MAP_MAIN)
			{
				/*if (m_iIdMap==MAP_NORMAL)
				{
					m_iMapType = GetNormalMapType();
				}
				else
					m_iMapType = m_iIdMap;
				*/
				m_iMapType = m_iIdMap;
				m_iIdMap = 0;
				DrawMap();
			}
			else
			{
				if (IsMapWithTransferPoint(m_iIdMap))
				{
					m_iMapType+=3;
					ShowMap(m_iIdMap);
				}
				else
					m_iIdMap = 0;
			}
			m_nHoverAreaIndex = -1;
		}
	}
/*
	if (m_bShowWorldMap)
	{
		if (m_nHoverAreaIndex != -1)
		{
			vector<DOMAIN_INFO> *info = globaldata_getdomaininfos5();
			ShowMap((*info)[m_nHoverAreaIndex].id);
		}
	}*/
	else
	{
		size_t count = m_vecMapMark.size();
		for(size_t i = 0; i < count; ++ i)
		{
			const MAPMARK& markPt = m_vecMapMark[i];
			if( markPt.rect.PtInRect(x, y) )
			{
				if (!markPt.active)
					return;

				const UI_TRANSFER_CONFIG::TRANS_POINT& transPt = m_vecTransPoints[markPt.index];
				if (transPt.trans_id == m_currentTransId) // 当前所在点, 不需要传送
					return;

				if( transPt.fee > GetHostPlayer()->GetMoneyAmount() )
				{
					GetGameUIMan()->MessageBox("", GetStringFromTable(807),
						MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
					return;
				}

				ACString szText, szFee;
				int fee1 = transPt.fee % 100;
				int fee2 = (transPt.fee / 100) % 100;
				int fee3 = (transPt.fee / 10000);
				if (fee3 > 0)
					szFee += szText.Format(GetStringFromTable(16001), fee3);
				if (fee2 > 0)
					szFee += szText.Format(GetStringFromTable(16002), fee2);
				if (fee1 > 0)
					szFee += szText.Format(GetStringFromTable(16003), fee1);

				szText.Format(GetStringFromTable(16000), transPt.name, szFee);
				
				PAUIDIALOG pMsgBox = NULL;
				GetGameUIMan()->MessageBox("Game_MapTransfer", szText,
							MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
				pMsgBox->SetData(transPt.trans_id);
				
				return;
			}
		}
	}
}

void CDlgMapTransfer::OnEventRButtonUp_Map(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	OnCommand_Return("");
}

void CDlgMapTransfer::OnTick()
{
	GetDlgItem("Btn_Worldmaps")->Show(m_iMapType!=MAP_MAIN);
}

bool CDlgMapTransfer::Render()
{
	if (m_iIdMap==0)
	{
		m_pImg_Map->SetRenderCallback(NULL, NULL);
		CDlgBase::Render();
		if( m_nHoverAreaIndex != -1 )
			RenderArea(m_nHoverAreaIndex, A3DCOLORRGBA(255, 255, 255, 128));
		return true;
	}
	else
	{
		m_pImg_Map->SetRenderCallback(MapRenderCallback, (DWORD)this);
		return CDlgBase::Render();
	}
	return true;
}

void CDlgMapTransfer::MapRenderCallback(const A3DRECT &rc, DWORD param1, DWORD param2, DWORD param3)
{
	CDlgMapTransfer* pDlgMapTransfer = (CDlgMapTransfer*)param1;
	pDlgMapTransfer->RenderMapMark();
}

void CDlgMapTransfer::RenderMapMark()
{	
	//int instanceId = GetWorld()->GetInstanceID();
	CECInstance* pInstance = GetGameRun()->GetInstance(m_iIdMap);

	AString szSubPath = pInstance->GetPath();
	A3DVECTOR3 vecHpPos = GetHostPlayer()->GetPos();
	szSubPath += pInstance->GetSubPath(vecHpPos.x, vecHpPos.y);
	m_pLable_Title->SetText(pInstance->GetNameByCoord(vecHpPos.x, vecHpPos.y));
	
	AString szFile = AString("Surfaces\\MidMaps\\") + szSubPath + ".dds";
	if (szFile != m_szPathName)
	{
		if(!m_pTexMap)
			m_pTexMap = new A3DTexture;
		else
			m_pTexMap->Release();
		m_pTexMap->SetNoDownSample(true);
		m_pTexMap->LoadFromFile(m_pA3DDevice, szFile, szFile);
		m_szPathName = szFile;

		static const float s_cell = 1024.0f;
		m_instanceSize.x = int(pInstance->GetRowNum() * s_cell);
		m_instanceSize.y = int(pInstance->GetColNum() * s_cell);
	}

	m_rcMap = m_pImg_Map->GetRect();	
	if( m_rcMap.Width() % 2 != 0 )
		m_rcMap.right -= 1;
	if( m_rcMap.Height() % 2 != 0  )
		m_rcMap.bottom -= 1;

	m_vertexMidMap[0].x = m_rcMap.left - 0.5f;
	m_vertexMidMap[0].y = m_rcMap.top - 0.5f;
	m_vertexMidMap[1].x = m_rcMap.right - 0.5f;
	m_vertexMidMap[1].y = m_rcMap.top - 0.5f;
	m_vertexMidMap[2].x = m_rcMap.left - 0.5f;
	m_vertexMidMap[2].y = m_rcMap.bottom - 0.5f;
	m_vertexMidMap[3].x = m_rcMap.right - 0.5f;
	m_vertexMidMap[3].y = m_rcMap.bottom - 0.5f;
	
	m_vertexMidMap[0].tu = 0.0f;
	m_vertexMidMap[0].tv = 0.0f;
	m_vertexMidMap[1].tu = 1.0f;
	m_vertexMidMap[1].tv = 0.0f;
	m_vertexMidMap[2].tu = 0.0f;
	m_vertexMidMap[2].tv = 1.0f;
	m_vertexMidMap[3].tu = 1.0f;
	m_vertexMidMap[3].tv = 1.0f;

	m_pA3DDevice->SetTextureAddress(0, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	
	const static WORD a_wIndex[6] = { 0, 1, 2, 2, 1, 3 };
	A3DVECTOR3 vecPos = A3DVECTOR3(0.0f, 0.0f, 0.0f);
	m_pTexMap->Appear(0);

	MAPTLVERTEX * pVerts;
	if( !m_pMapStream->LockVertexBuffer(0, 0, (BYTE**) &pVerts, 0) )
		return;
	
	pVerts[0] = m_vertexMidMap[0];
	pVerts[1] = m_vertexMidMap[1];
	pVerts[2] = m_vertexMidMap[2];
	
	pVerts[3] = m_vertexMidMap[2];
	pVerts[4] = m_vertexMidMap[1];
	pVerts[5] = m_vertexMidMap[3];
	
	m_pMapStream->UnlockVertexBuffer();
	m_pMapStream->Appear();
	
	bool bRet = m_pA3DDevice->DrawPrimitive(A3DPT_TRIANGLELIST, 0, 2);
	if(!bRet) 
	{
		AUI_ReportError(__LINE__, __FILE__);
		return;
	}
	
	m_pTexMap->Disappear(0);
	m_pA3DDevice->SetTextureAddress(0, A3DTADDR_WRAP, A3DTADDR_WRAP);
	m_pA3DDevice->SetLightingEnable(true);

	// render transfer and host player mark
	UpdateMark();
	
	size_t count = m_vecMapMark.size();
	for (size_t i = 0; i < count; ++ i)
	{
		const MAPMARK& point = m_vecMapMark[i];
		A2DSprite* mark = point.active ? m_pA2DMapActiveMark : m_pA2DMapInactiveMark;
		mark->DrawToInternalBuffer(point.rect.left, point.rect.top);
	}

	if (m_iIdMap == GetGameRun()->GetWorld()->GetInstanceID())
	{
		A3DPOINT2 center = m_rcMap.CenterPoint();
		int posX = TO_SCREEN_X(vecHpPos.x);
		int posY = TO_SCREEN_Y(vecHpPos.z);
		GetGameUIMan()->m_pDlgMiniMap->m_pA2DMapMark[CDlgMiniMap::MAPMARK_HOST]->DrawToInternalBuffer(posX, posY);
	}
}

void CDlgMapTransfer::RenderArea(int nIndex, A3DCOLOR cl)
{
	A3DFlatCollector *pFC = GetGame()->GetA3DEngine()->GetA3DFlatCollector();
	vector<DOMAIN_INFO> *info = GetSuitDomainInfo(m_iMapType);
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

void CDlgMapTransfer::FillAreaRects(int nType)
{
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
		if(!(m_iMapType==MAP_MAIN||m_iMapType==MAP_CROSS||m_iMapType==MAP_HONGHUANG))
		{
			rect.left/=2;
			rect.right/=2;
			rect.top/=2;
			rect.bottom/=2;
		}
		m_vecAreaRect.push_back(rect);
	}

}

// 显示指定的地图
void CDlgMapTransfer::ShowMap(unsigned int mapId)
{
	if (!m_pTransService)
		return;

	m_iIdMap = mapId;
	m_iMapType = IsHongHuangMap(m_iIdMap) ? MAP_HONGHUANGXX : MAP_NORMALXX;
	m_nHoverAreaIndex = -1;

	DrawMap();

	m_vecTransPoints.clear();
	elementdataman* pDataMan = g_pGame->GetElementDataMan();

	DATA_TYPE DataType;
	const void* pData = pDataMan->get_data_ptr(ms_uitransferConfigId, ID_SPACE_CONFIG, DataType);
	if (!pData || DataType != DT_UI_TRANSFER_CONFIG)
	{
		ASSERT(false);
		return;
	}
	const UI_TRANSFER_CONFIG* pTransConfig = (const UI_TRANSFER_CONFIG*)pData;
	
	for (int i = 0; i < m_isTransIdCount; ++ i)
	{
		int id = m_pTransService->trans_id[i];
		if (id <= 0 || id > m_isTransPointCount)
			break;
		
		const UI_TRANSFER_CONFIG::TRANS_POINT& transPt = pTransConfig->trans_point_list[id-1];
		if (transPt.map_id != m_iIdMap) // 只显示当前地图信息 
			continue;

		m_vecTransPoints.push_back(transPt);
	}
}

bool CDlgMapTransfer::IsMapWithTransferPoint(int iMapId)
{
	if(iMapId==0)
		return false;

	elementdataman* pDataMan = g_pGame->GetElementDataMan();

	DATA_TYPE DataType;
	const void* pData = pDataMan->get_data_ptr(ms_uitransferConfigId, ID_SPACE_CONFIG, DataType);
	if (!pData || DataType != DT_UI_TRANSFER_CONFIG)
	{
		ASSERT(false);
		return false;
	}
	const UI_TRANSFER_CONFIG* pTransConfig = (const UI_TRANSFER_CONFIG*)pData;
	
	for (int i = 0; i < m_isTransIdCount; ++ i)
	{
		int id = m_pTransService->trans_id[i];
		if (id <= 0 || id > m_isTransPointCount)
			break;
		
		const UI_TRANSFER_CONFIG::TRANS_POINT& transPt = pTransConfig->trans_point_list[id-1];
		if (transPt.map_id != iMapId) // 只考虑当前判断地图信息 
			continue;
		return true;
	}

	return false;
}


// 显示世界地图
void CDlgMapTransfer::OnCommand_Return(const char * szCommand)
{
	if(m_iMapType==MAP_MAIN)
		return;
	if(m_iMapType==MAP_NORMALXX)
		m_iMapType = MAP_NORMAL;
	else if(m_iMapType==MAP_HONGHUANGXX)
		m_iMapType = MAP_HONGHUANG;
	else
		m_iMapType = MAP_MAIN;

	m_iIdMap = 0;
	DrawMap();
}

void CDlgMapTransfer::UpdateMark()
{
	m_vecMapMark.clear();

	A3DPOINT2 center = m_rcMap.CenterPoint();
	int nMarkW = m_pA2DMapActiveMark->GetWidth();
	int nMarkH = m_pA2DMapActiveMark->GetHeight();
	int nMarkCX = nMarkW / 2;
	int nMarkCY = nMarkH / 2;

	size_t count = m_vecTransPoints.size();
	for (size_t i = 0; i < count; ++ i)
	{
		const UI_TRANSFER_CONFIG::TRANS_POINT& transPt = m_vecTransPoints[i];
		MAPMARK markPt;
		markPt.index = (int)i;
		markPt.active = GetHostPlayer()->IsUiTransIdActive(transPt.trans_id);
		markPt.strHint = transPt.name;
		markPt.rect.left = TO_SCREEN_X(transPt.pos[0]) - nMarkCX;
		markPt.rect.top = TO_SCREEN_Y(transPt.pos[2]) - nMarkCY;
		markPt.rect.right = markPt.rect.left + nMarkW;
		markPt.rect.bottom = markPt.rect.top + nMarkH;
		m_vecMapMark.push_back(markPt);
	}
}

void CDlgMapTransfer::PopupDlgMapTransfer(unsigned int currentTransId, const NPC_UI_TRANSFER_SERVICE *pService)
{
	m_pTransService = pService;
	m_currentTransId = currentTransId;

	ShowMap(GetWorld()->GetInstanceID());
	
	Show(!m_vecTransPoints.empty());
}

A2DSprite* CDlgMapTransfer::GetA2DMapActiveMark()
{
	InitTex();
	return m_pA2DMapActiveMark;
}

A2DSprite* CDlgMapTransfer::GetA2DMapInactiveMark()
{
	InitTex();
	return m_pA2DMapInactiveMark;
}

bool CDlgMapTransfer::IsHongHuangMap(int iMapId)
{
	if (iMapId==44 || iMapId==48 || iMapId==53)
		return true;
	else
		return false;	

}

void CDlgMapTransfer::DrawMap()
{
	AUIOBJECT_SETPROPERTY p;
	if(m_iMapType==MAP_MAIN)
		strcpy(p.fn, "WorldMaps\\worldmaps_main.dds");
	else if(m_iMapType==MAP_CROSS)
		strcpy(p.fn, "WorldMaps\\worldmap_heaven.dds");
	else if(m_iMapType==MAP_NORMAL)
		strcpy(p.fn, "WorldMaps\\WorldMap.dds");
	else if(m_iMapType==MAP_HONGHUANG)
		strcpy(p.fn, "WorldMaps\\WorldMap_02.dds");
	else
	{
		CECInstance* pInstance = GetGameRun()->GetInstance(m_iIdMap);

		AString szSubPath = pInstance->GetPath();
		A3DVECTOR3 vecHpPos = GetHostPlayer()->GetPos();
		szSubPath += pInstance->GetSubPath(vecHpPos.x, vecHpPos.y);
		m_pLable_Title->SetText(pInstance->GetNameByCoord(vecHpPos.x, vecHpPos.y));

		AString szFile = AString("MidMaps\\") + szSubPath + ".dds";
		strcpy(p.fn, szFile); 
	}

	m_pImg_Map->SetProperty("Image File", &p);
	FillAreaRects(m_iMapType);
}
