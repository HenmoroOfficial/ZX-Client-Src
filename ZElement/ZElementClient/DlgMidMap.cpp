// Filename	: DlgMidMap.cpp
// Creator	: Xiao Zhou
// Date		: 2006/5/25

#include "AFI.h"
#include "AWScriptFile.h"
#include "AUI\\CSplit.h"
#include "AUI\\AUISubDialog.h"
#include "AUI\\A3DFTFont.h"
#include "AFileImage.h"
#include "A2DSprite.h"
#include "A3DFlatCollector.h"
#include "A3DFuncs.h"
#include "A2DSprite.h"
#include "A3DStream.h"
#include "A3DCamera.h"
#include "A3DDevice.h"
#include "A3DTexture.h"
#include "DlgMidMap.h"
#include "DlgPKSetting.h"
#include "DlgTask.h"
#include "DlgWorldMap.h"
#include "DlgInputName.h"
#include "DlgTerrWarMap.h"
#include "EC_Global.h"
#include "EC_GameUIMan.h"
#include "EC_Instance.h"
#include "EC_HostPlayer.h"
#include "EC_GameRun.h"
#include "EC_World.h"
#include "EC_Game.h"
#include "EC_ManNPC.h"
#include "EC_NPC.h"
#include "EC_NPCServer.h"
#include "EC_Friend.h"
#include "EC_ManPlayer.h"
#include "EC_Configs.h"
#include "EC_Team.h"
#include "EC_Model.h"
#include "EC_ElsePlayer.h"
#include "EL_Precinct.h"
#include "EC_Resource.h"
#include "EC_GameSession.h"
#include "EC_InputCtrl.h"
#include "DlgFriendList.h"
#include "DlgMiniMap.h"
#include "EC_HelpGraphGuide.h"
#include "elementdataman.h"
#include "aui\\AUIDef.h"
#include "A3DFont.h"

#include "EC_AutoMove.h"

#define new A_DEBUG_NEW

#define SQUARE(x)	((x) * (x))
#define POINTTOSCREEN_X(x) int(int(p->X) + (rcMap.left + rcMap.Width() / 2 - rcMap.Height() / 2) + (x/nRow - rcMidMap.left) * pDlgMap->m_fZoom * rcMap.Height() / ZOOM_MIN /(1024* fScale))
#define POINTTOSCREEN_Y(y) int(int(p->Y) + rcMap.top + (((-y/nRow - rcMidMap.top) * pDlgMap->m_fZoom / ZOOM_MIN)) * rcMap.Height() / (1024* fScale))
#define SCREENTOPOINT_X(x) float((x - int(p->X) - (rcMap.left + rcMap.Width() / 2 - rcMap.Height() / 2)) * (1024 * fScale)* ZOOM_MIN / m_fZoom / rcMap.Height() + rcMidMap.left)
//#define SCREENTOPOINT_Y(y) float(1024 - (y - int(p->Y) - rcMap.top) * 1024 / rcMap.Height() + rcMidMap.top)
#define SCREENTOPOINT_Y(y) float(-(y - int(p->Y) - rcMap.top) * (1024 * fScale)*ZOOM_MIN / m_fZoom / rcMap.Height() - rcMidMap.top)

static const float ZOOM_MIN = 0.5f;
static const float ZOOM_LEVEL = 0.25;
static const float ZOOM_MAX = 1.0f;
static const float g_fCell = 1024.f;

static const int g_nRenderAreaBase = 1024;

abase::hash_map<int, CDlgMidMap::vecMonsterPos> CDlgMidMap::m_mapNpcMonsterPos;
abase::hash_map<int, CDlgMidMap::vecTownNpcMonsterPos> CDlgMidMap::m_mapTownNpcMonsterPos;
abase::vector<CDlgMidMap::MAPMARK> CDlgMidMap::m_vecMidMapMark;
CDlgMidMap::MAPTLVERTEX CDlgMidMap::m_vertexMidMap[4];
A3DStream * CDlgMidMap::m_pMidMapStream;
A3DTexture * CDlgMidMap::m_pTexRadar = NULL;
bool CDlgMidMap::m_bEvent[3];
	
AUI_BEGIN_COMMAND_MAP(CDlgMidMap, CDlgBase)

AUI_ON_COMMAND("Btn_ZoomIn",	OnCommand_ZoomIn)
AUI_ON_COMMAND("Btn_ZoomOut",	OnCommand_ZoomOut)
AUI_ON_COMMAND("Btn_Add",		OnCommand_AddMark)
AUI_ON_COMMAND("Btn_Search",	OnCommand_Search)
AUI_ON_COMMAND("Edt_Input",		OnCommand_Search)
AUI_ON_COMMAND("Btn_Worldmap",	OnCommand_WorldMap)
AUI_ON_COMMAND("Btn_Worldmap_Terri",OnCommand_TerrMap)
AUI_ON_COMMAND("Btn_KfMap",		OnCommand_KfWorldMap)
AUI_ON_COMMAND("Btn_Edit",		OnCommand_edit)
AUI_ON_COMMAND("Btn_AutoMove",	OnCommand_automove)
AUI_ON_COMMAND("Btn_Delete",	OnCommand_delete)
AUI_ON_COMMAND("Btn_DeleteAll",	OnCommand_deleteall)
AUI_ON_COMMAND("Btn_ToMap02",	OnCommand_CityMap)
AUI_ON_COMMAND("Btn_ToMap03",	OnCommand_TownMap)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_CANCEL)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgMidMap, CDlgBase)

AUI_ON_EVENT("Img_MidMap",	WM_MOUSEMOVE,		OnEventMouseMove_MidMap)
AUI_ON_EVENT("Img_MidMap",	WM_LBUTTONUP,		OnEventLButtonUp_MidMap)
AUI_ON_EVENT("Img_MidMap",	WM_LBUTTONDOWN,		OnEventLButtonDown_MidMap)
AUI_ON_EVENT("Img_MidMap",	WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_ON_EVENT("Lst_Given",	WM_RBUTTONUP,		OnEventRButtonDown_ListRegion)
AUI_ON_EVENT("Lst_Given",	WM_LBUTTONDBLCLK,	OnEventRButtonDown_ListRegion)
AUI_ON_EVENT("List_Choose",	WM_RBUTTONUP,		OnEventRButtonDown_ListRegion)
AUI_ON_EVENT("List_Choose",	WM_LBUTTONDBLCLK,	OnEventRButtonDown_ListRegion)
AUI_ON_EVENT("*",			WM_RBUTTONDOWN,		OnEventRButtonDown)
AUI_ON_EVENT("*",			WM_RBUTTONUP,		OnEventRButtonUp)

AUI_END_EVENT_MAP()

CDlgMidMap::CDlgMidMap()
{
	m_fZoom = ZOOM_MIN;
	m_nDownPosX = m_nDownPosY = -1;
	
	for(int j = 0; j < 4; j++ )
	{
		m_vertexMidMap[j].z = 0.0f;
		m_vertexMidMap[j].rhw = 1.0f;
		m_vertexMidMap[j].diffuse = 0xFFFFFFFF;
		m_vertexMidMap[j].specular = 0xFF000000;
	}
}

CDlgMidMap::~CDlgMidMap()
{
	if( m_pMidMapStream )
	{
		m_pMidMapStream->Release();
		delete m_pMidMapStream;
		m_pMidMapStream = NULL;
	}
	A3DRELEASE(m_pTexRadar);

}

void CDlgMidMap::OnCommand_ZoomIn(const char * szCommand)
{
	GetGameUIMan()->m_nCursorState = CECGameUIMan::CURSOR_STATE_ZOOM;
	GetGame()->ChangeCursor(RES_CUR_ZOOMIN);
}

void CDlgMidMap::OnCommand_ZoomOut(const char * szCommand)
{
	GetGameUIMan()->m_nCursorState = CECGameUIMan::CURSOR_STATE_ZOOM;
	GetGame()->ChangeCursor(RES_CUR_ZOOMOUT);
}

void CDlgMidMap::OnCommand_AddMark(const char * szCommand)
{
	if( GetGameUIMan()->m_pDlgMiniMap->m_vecMark.size() >= CECGAMEUIMAN_MAX_MARKS )
	{
		GetGameUIMan()->AddChatMessage(GetStringFromTable(507), GP_CHAT_MISC);
		return;
	}

	ACString strText = GetDlgItem("Txt_Mark")->GetText();
	if( strText.GetLength() <= 0 ) return;
	
	int i;
	CSplit s(strText);
	CSPLIT_STRING_VECTOR vec;
	
	for( i = 266; i <= 269; i++ )
	{
		vec = s.Split(GetStringFromTable(i));
		if( vec.size() >= 2 ) break;
	}
	
	if( vec.size() < 2 )
	{
		GetGameUIMan()->AddChatMessage(GetStringFromTable(237), GP_CHAT_MISC);
		return;
	}
	
	GetGameUIMan()->m_vecTargetPos.x = (float)a_atoi(vec[0]);
	GetGameUIMan()->m_vecTargetPos.z = (float)a_atoi(vec[1]);
	GetGameUIMan()->m_vecTargetPos.y = g_pGame->GetGameRun()->GetWorld()->GetTerrainHeight(GetGameUIMan()->m_vecTargetPos);
	
	GetGameUIMan()->m_pDlgInputName->Show(true, true);
	GetGameUIMan()->m_pDlgInputName->m_pTxtInput->SetText(_AL(""));
	GetGameUIMan()->m_pDlgInputName->SetData(CDlgInputName::INPUTNAME_MAKE_MARK);
}

void CDlgMidMap::OnCommand_Search(const char * szCommand)
{
	PAUILABEL pLabel = (PAUILABEL)GetDlgItem("Title");
	PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("Lst_Given");
	pList->ResetContent();
	
	int idInstance = GetWorld()->GetInstanceID();
	CECInstance *pInstance = GetGameRun()->GetInstance(idInstance);
	A3DVECTOR3 pos = GetHostPlayer()->GetPos();

	pLabel->SetText(pInstance->GetNameByCoord(pos.x, pos.z));
	
	vecMonsterPos vecPos = m_mapNpcMonsterPos[GetWorld()->GetInstanceID()];
	vecMonsterPos::iterator it = vecPos.begin();
	ACString strText;
	ACString strTextSearch = m_pEdt_Search->GetText();
	while (it!=vecPos.end())
	{
		NpcMonsterPos pos = *it;
		if (strTextSearch.IsEmpty() || IsStringMatch(pos.strName, strTextSearch) )
		{
			if (CheckNpcArea(pos.x, pos.z))
			{
				pList->AddString(pos.strName);
				strText.Format(_AL("%d,%d"), pos.x, pos.z);
				pList->SetItemHint(pList->GetCount()-1, strText);
				pList->SetItemData(pList->GetCount()-1, pos.x, 0);
				pList->SetItemData(pList->GetCount()-1, pos.y, 1);
				pList->SetItemData(pList->GetCount()-1, pos.z, 2);
			}
		}
		it++;
	}
}

bool CDlgMidMap::IsStringMatch(ACString strName, ACString strSearch)
{
	bool bRet = a_strstr(strName, strSearch) != NULL;
	const char *szCountry = GetGame()->GetConfigs()->GetCountryRegion();
	if ( !bRet && 0 == stricmp(szCountry, "cn") )
	{
		ACHAR ch = NULL;
		int i;
		for (i=0; i<strSearch.GetLength(); ++i)
		{
			if (i>=strName.GetLength())
				break;
			
			if (strSearch[i]>=_AL('a')&&strSearch[i]<=_AL('z'))
				ch = strSearch[i];
			else if (strSearch[i]>=_AL('A')&&strSearch[i]<=_AL('Z'))
				ch = strSearch[i] - _AL('A') + _AL('a');
			if (!_ChinaPinYin(ch, strName[i]))
			{
				break;
			}
		}
		// 没有不匹配字符
		if (i==strName.GetLength()||i==strSearch.GetLength())
		{
			bRet = true;
		}
	}
	return bRet;
}
bool CDlgMidMap::GenerateTownArea()
{
	//强制为方形地图
    if(IsInTownPrecinct())
	{
	    float fLeft,fRight,fTop,fBottom;
	    float fMapLeft,fMapRight,fMapTop,fMapBottom;
		int idInstance = GetWorld()->GetInstanceID();
		//新河阳暂定
		if(idInstance == 401)
		{
		   if (stricmp(GetWorld()->GetCurPrecinct()->GetMiniMap(),"Surfaces\\thirdmaps\\x1.dds") == 0)
		   {
				m_TownArea.left   = 0.0f;
				m_TownArea.right  = 1024.f;
				m_TownArea.top    = 0.0f;
				m_TownArea.bottom = 1024.0f;
				return true;
		   }
		}
		CECInstance *pInstance = GetGameRun()->GetInstance(idInstance);
        fMapLeft   = -pInstance->GetColNum()*512.f;
		fMapRight  = pInstance->GetColNum()*512.f;
		fMapTop    = -pInstance->GetRowNum()*512.f;
		fMapBottom = pInstance->GetRowNum()*512.f; 
        GetWorld()->GetCurPrecinct()->GetBoundBox(fLeft,fTop,fRight,fBottom);
		float fWidth = fRight-fLeft;
		float fHeight = fBottom-fTop;
		if(fWidth > fHeight)
		{
		   float fHalfDiff = (fWidth-fHeight)/2.0f;
		   fTop    -= fHalfDiff;
		   fBottom += fHalfDiff;

		   if(fTop < fMapTop)
		   {
		      fTop = fMapTop;
			  fBottom = fTop + fWidth;
		   }
		   else if(fBottom > fMapBottom)
		   {
		      fBottom = fMapBottom;
			  fTop = fBottom - fWidth;
		   }
		   assert(fTop >= fMapTop && fBottom<= fMapBottom);
		}
		else if(fHeight > fWidth)
		{
		   float fHalfDiff = (fHeight-fWidth)/2.0f;
		   fLeft  -= fHalfDiff;
		   fRight += fHalfDiff;

		   if(fLeft < fMapLeft)
		   {
		     fLeft  = fMapLeft;
			 fRight = fLeft + fHeight; 
		   }
		   else if(fRight > fMapRight)
		   {
		     fRight = fMapRight;
			 fLeft = fRight - fHeight;
		   }
		   assert(fLeft>=fMapLeft && fRight<=fMapRight);
		}
		m_TownArea.left   = fLeft;
		m_TownArea.right  = fRight;
		m_TownArea.top    = fTop;
		m_TownArea.bottom = fBottom;
		return true;
	}
    return false;
}
void CDlgMidMap::GetMidMapOffsetParam(float& fScale,float& fOffsetX,float& fOffsetZ)
{
    fScale = 1.0f;
	fOffsetX = 0.0f;
	fOffsetZ = 0.0f;
	if(m_iCurMapType == MAP_TYPE_TOWN && GenerateTownArea())
	{	
		int idInstance = GetWorld()->GetInstanceID();
		CECInstance *pInstance = GetGameRun()->GetInstance(idInstance);
		fScale = (m_TownArea.right-m_TownArea.left)/(pInstance->GetColNum()*1024.f);
		fOffsetX = (m_TownArea.left-(-pInstance->GetColNum()*512.f))/pInstance->GetColNum();
		fOffsetZ = -(m_TownArea.bottom-(pInstance->GetRowNum()*512.f))/pInstance->GetRowNum();

	}
}
bool CDlgMidMap::IsInTownPrecinct()
{
   if(NULL != GetWorld()->GetCurPrecinct())
   {
      if(stricmp(GetWorld()->GetCurPrecinct()->GetMiniMap(),"") != 0 &&
		  stricmp(GetWorld()->GetCurPrecinct()->GetMiniMap(),"Surfaces\\") != 0)
		  return true;
   }
   return false;
}
bool CDlgMidMap::_ChinaPinYin(ACHAR chSearch, ACHAR chName)
{
	AString strName = AC2AS(ACString(chName, 1));
	int chWideChar = (unsigned char)strName[0];
	chWideChar <<= 8;
	chWideChar += (unsigned char)strName[1];
	return (chSearch==_AL('a') && chWideChar>=0xB0A1 && chWideChar<=0xB0C4) ||
		(chSearch==_AL('b') && chWideChar>=0XB0C5 && chWideChar<=0XB2C0) ||
		(chSearch==_AL('c') && chWideChar>=0xB2C1 && chWideChar<=0xB4ED) ||
		(chSearch==_AL('d') && chWideChar>=0xB4EE && chWideChar<=0xB6E9) ||
		(chSearch==_AL('e') && chWideChar>=0xB6EA && chWideChar<=0xB7A1) ||
		(chSearch==_AL('f') && chWideChar>=0xB7A2 && chWideChar<=0xB8c0) ||
		(chSearch==_AL('g') && chWideChar>=0xB8C1 && chWideChar<=0xB9FD) ||
		(chSearch==_AL('h') && chWideChar>=0xB9FE && chWideChar<=0xBBF6) ||
		(chSearch==_AL('j') && chWideChar>=0xBBF7 && chWideChar<=0xBFA5) ||
		(chSearch==_AL('k') && chWideChar>=0xBFA6 && chWideChar<=0xC0AB) ||
		(chSearch==_AL('l') && chWideChar>=0xC0AC && chWideChar<=0xC2E7) ||
		(chSearch==_AL('m') && chWideChar>=0xC2E8 && chWideChar<=0xC4C2) ||
		(chSearch==_AL('n') && chWideChar>=0xC4C3 && chWideChar<=0xC5B5) ||
		(chSearch==_AL('o') && chWideChar>=0xC5B6 && chWideChar<=0xC5BD) ||
		(chSearch==_AL('p') && chWideChar>=0xC5BE && chWideChar<=0xC6D9) ||
		(chSearch==_AL('q') && chWideChar>=0xC6DA && chWideChar<=0xC8BA) ||
		(chSearch==_AL('r') && chWideChar>=0xC8BB && chWideChar<=0xC8F5) ||
		(chSearch==_AL('s') && chWideChar>=0xC8F6 && chWideChar<=0xCBF0) ||
		(chSearch==_AL('t') && chWideChar>=0xCBFA && chWideChar<=0xCDD9) ||
		(chSearch==_AL('w') && chWideChar>=0xCDDA && chWideChar<=0xCEF3) ||
		(chSearch==_AL('x') && chWideChar>=0xCEF4 && chWideChar<=0xD188) ||
		(chSearch==_AL('y') && chWideChar>=0xD1B9 && chWideChar<=0xD4D0) ||
		(chSearch==_AL('z') && chWideChar>=0xD4D1 && chWideChar<=0xD7F9);
}

bool CDlgMidMap::CheckNpcArea(int x, int z)
{	
	A3DVECTOR3 pos = GetHostPlayer()->GetPos();
	int idInstance = GetWorld()->GetInstanceID();
	CECInstance *pInstance = GetGameRun()->GetInstance(idInstance);
	if( pInstance->GetID() == 19 )
	{
		if( pos.x > 130 && x > 130)
			return true;
		else if( pos.z < -31 && z < -31)
			return true;
		else if( pos.x < 130 && pos.z > -31 && x < 130 && z > -31)
			return true;
	}
	else if( pInstance->GetID() == 319 )
	{
		if( pos.x > 130 && x > 130)
			return true;
		else if( pos.z < -31 && z < -31)
			return true;
		else if( pos.x > 130 && pos.z > -31 &&x < 130 && z > -31)
			return true;
	}
	else if ( pInstance->GetID() == 13)
	{
		if ( pos.x<0 && pos.x>-290 && pos.z>194 && x<0 && x>-290 && z>194)
			return true;
		else if ( pos.x<-230 && pos.z>-194 && pos.z<194 && x<-230 && z>-194 && z<194)
			return true;
		else if ( pos.x<-230 && pos.z<-194 && x<-230 && z<-194)
			return true;
		else if ( pos.x>0 && x>0)
			return true;
	}
	else if( pInstance->GetID() == 18)
	{
		if ( pos.x<-75 && pos.z<-119 && x<-75 && z<-119)
			return true;
		else if ( pos.x>-75 && pos.z<-119 && x>-75 && z<-119)
			return true;
	}
	else if( pInstance->GetID() == 12)
	{
		if ( pos.z < 0 && z<0)
			return true;
		else if (pos.x > 0 && x>0)
			return true;
	}
	
	else if( pInstance->GetID() == 14 )
	{
		if( pos.x > 0 && x>0)
			return true;
		else if( pos.z < -252 && z<-252)
			return true;
	}
	else
		return true;
	return false;
}

void CDlgMidMap::OnCommand_WorldMap(const char * szCommand)
{
	if( GetWorld()->GetInstanceID() == 15 && 
		GetHostPlayer()->GetPos().x > -473 && GetHostPlayer()->GetPos().x < -381 &&
		GetHostPlayer()->GetPos().z > -444 && GetHostPlayer()->GetPos().z < -194)
		return;
	
	PAUIDIALOG pShow = GetGameUIMan()->m_pDlgWorldMap;
	
	bool bShow = !pShow->IsShow();
	pShow->Show(bShow);
	PAUISUBDIALOG pSub = (PAUISUBDIALOG)GetGameUIMan()->GetDialog("Win_WorldMap")->GetDlgItem("Sub_Worldmap");
	pSub->SetDialog(GetGameUIMan()->GetDialog("WorldMap_Sub"));
}


void CDlgMidMap::OnCommand_TerrMap(const char * szCommand)
{
	PAUIDIALOG pShow = GetGameUIMan()->m_pDlgWorldMap;
	
	bool bShow = !pShow->IsShow();
	pShow->Show(bShow);
	PAUISUBDIALOG pSub = (PAUISUBDIALOG)GetGameUIMan()->GetDialog("Win_WorldMap")->GetDlgItem("Sub_Worldmap");
	pSub->SetDialog(GetGameUIMan()->GetDialog("WorldMap_TerriSub"));
	CDlgTerrWarMap::SetMOpen(true);
	if (bShow)
		GetGameSession()->territory_get_map();
}

void CDlgMidMap::OnCommand_KfWorldMap(const char * szCommand)
{
	PAUIDIALOG pShow = GetGameUIMan()->m_pDlgWorldMap;
	bool bShow = !pShow->IsShow();
	pShow->Show(bShow);

	PAUISUBDIALOG pSub = (PAUISUBDIALOG)GetGameUIMan()->GetDialog("Win_WorldMap")->GetDlgItem("Sub_Worldmap");
	pSub->SetDialog(GetGameUIMan()->GetDialog("WorldMap_Sub"));
}

void CDlgMidMap::OnCommand_edit(const char * szCommand)
{
	PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("List_Choose");
	int nCurSel = pList->GetCurSel();
	if( nCurSel < 0 || nCurSel >= pList->GetCount() ) return;
	
	if( GetGameUIMan()->m_pDlgMiniMap->m_vecMark[nCurSel].nNPC >= 0 )
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(511),
			MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	}
	else
	{
		ACString strName = pList->GetText(nCurSel);
		PAUIDIALOG pDlgInuptString11 = GetGameUIMan()->GetDialog("Win_InputString11");
		PAUITEXTAREA pTxt = (PAUITEXTAREA)pDlgInuptString11->GetDlgItem("DEFAULT_Txt_Input");
		pTxt->SetText(strName);
		pTxt = (PAUITEXTAREA)pDlgInuptString11->GetDlgItem("Txt_NewLocation");
		strName.Format(_AL("%.0f %.0f"), GetGameUIMan()->m_pDlgMiniMap->m_vecMark[nCurSel].vecPos.x, GetGameUIMan()->m_pDlgMiniMap->m_vecMark[nCurSel].vecPos.z);
		pTxt->SetText(strName);
		
		pDlgInuptString11->Show(true, true);
		pDlgInuptString11->SetData(CDlgInputName::INPUTNAME_RENAME_MARK);
		pDlgInuptString11->SetDataPtr((void*)nCurSel);
	}
}


void CDlgMidMap::OnCommand_automove(const char * szCommand)
{
	PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("List_Choose");
	int nCurSel = pList->GetCurSel();
	
	if( nCurSel >= 0 && nCurSel < pList->GetCount() )
	{
		g_ECAutoMove.SearchPath(GetHostPlayer()->GetPos(), GetGameUIMan()->m_pDlgMiniMap->m_vecMark[nCurSel].vecPos, GetGameUIMan()->m_pDlgMiniMap->m_vecMark[nCurSel].idInstance);
	}
}

void CDlgMidMap::OnCommand_delete(const char * szCommand)
{
	PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("List_Choose");
	
	if( pList->GetCurSel() >= 0 && pList->GetCurSel() < pList->GetCount() )
	{
		PAUIDIALOG pMsgBox = NULL;
		GetGameUIMan()->MessageBox("Game_DelMark", GetStringFromTable(509),
			MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		pMsgBox->SetData(pList->GetCurSel());
	}
}

void CDlgMidMap::OnCommand_deleteall(const char * szCommand)
{
	PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("List_Choose");
	
	if( pList->GetCount() > 0 )
	{
		GetGameUIMan()->MessageBox("Game_DelAllMarks", GetStringFromTable(510),
			MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160));
	}
}

void CDlgMidMap::OnCommand_CANCEL(const char * szCommand)
{
	m_nDownPosX = m_nDownPosY = -1;
	GetGameUIMan()->m_nCursorState = CECGameUIMan::CURSOR_STATE_NORMAL;
	GetGame()->ChangeCursor(RES_CUR_NORMAL);
	Show(false);
}

void CDlgMidMap::OnEventMouseMove_MidMap(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	GetGameUIMan()->RespawnMessage();
	if (m_nDownPosX < 0)
	{
		int i;
		A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);
		
		int idInstance = GetWorld()->GetInstanceID();
		CECInstance *pInstance = GetGameRun()->GetInstance(idInstance);
		int nRow = pInstance->GetRowNum();
		int nCol = pInstance->GetColNum();
		A3DRECT rcMidMap, rcMap;
		A3DVECTOR3 vecPosHost = GetHostPlayer()->GetPos();
		float fScale = 1.0;
		float fOffsetX = 0.0f;
		float fOffsetZ = 0.0f;
		GetMidMapOffsetParam(fScale,fOffsetX,fOffsetZ);
		rcMap = m_pImg_MidMap->GetRect();
		rcMidMap.left = int((vecPosHost.x/nRow - 512) / 1024) * 1024 - 512;
		rcMidMap.top = int((vecPosHost.z/nRow - 512) / 1024) * 1024 - 512;
		rcMidMap.right = rcMidMap.left + 1024;
		rcMidMap.bottom = rcMidMap.top + 1024;
		rcMidMap.left = m_rcRenderArea.left*fScale - 512 + fOffsetX;
		rcMidMap.top = m_rcRenderArea.top*fScale - 512 + fOffsetZ;
		rcMidMap.right = m_rcRenderArea.right*fScale - 512 + fOffsetX;
		rcMidMap.bottom = m_rcRenderArea.bottom*fScale - 512 + fOffsetZ;
		GetGameUIMan()->m_pDlgMiniMap->m_vecNowTarget.x = nRow*SCREENTOPOINT_X(x);
		GetGameUIMan()->m_pDlgMiniMap->m_vecNowTarget.y = vecPosHost.y;
		GetGameUIMan()->m_pDlgMiniMap->m_vecNowTarget.z = nRow*SCREENTOPOINT_Y(y);
		pObj->SetHint(_AL(""));
		for( i = 0; i < (int)m_vecMidMapMark.size(); i++ )
		{
			if( m_vecMidMapMark[i].rect.PtInRect(x, y) )
			{
				pObj->SetHint(m_vecMidMapMark[i].strHint);
				GetGameUIMan()->m_pDlgMiniMap->m_vecNowTarget = m_vecMidMapMark[i].vecPos;
				return;
			}
		}

		for ( i = 0; i < (int)m_vecTransferMapMarks.size(); ++ i)
		{
			if (m_vecTransferMapMarks[i].transRect.PtInRect(x, y) )
			{
				pObj->SetHint(m_vecTransferMapMarks[i].strHint);
				GetGameUIMan()->m_pDlgMiniMap->m_vecNowTarget.x = m_vecTransferMapMarks[i].transRect.CenterPoint().x;
				GetGameUIMan()->m_pDlgMiniMap->m_vecNowTarget.z = m_vecTransferMapMarks[i].transRect.CenterPoint().y;
				return;
			}
		}
	}
	else
	{
		if (fabs(m_fZoom - ZOOM_MIN) < 0.001)
			return;

		A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);
		
		if( m_nStartX != -1 )
		{
			int nOffX = x - m_nStartX;
			int nOffY = y - m_nStartY;
			m_rcRenderArea.left -= int(nOffX/m_fZoom);
			m_rcRenderArea.right -= int(nOffX/m_fZoom);
			m_rcRenderArea.top -= int(nOffY/m_fZoom);
			m_rcRenderArea.bottom -= int(nOffY/m_fZoom);
			CheckRenderArea();
			
			m_nStartX = x;
			m_nStartY = y;
		}
	}
}

void CDlgMidMap::OnEventLButtonDown_MidMap(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	m_nDownPosX = GET_X_LPARAM(lParam);
	m_nDownPosY = GET_Y_LPARAM(lParam);
	
	m_nStartX = m_nDownPosX;
	m_nStartY = m_nDownPosY;
	m_dwLBtnDownTime = GetTickCount();
}

void CDlgMidMap::OnEventLButtonUp_MidMap(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	m_dwLBtnDownTime = 0;
	if (GetGame()->GetCurCursor()==RES_CUR_NORMAL)
	{
		A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
		int x = GET_X_LPARAM(lParam) - p->X;
		int y = GET_Y_LPARAM(lParam) - p->Y;
		
		
		if (abs(x - m_nDownPosX) > 3 || abs(y - m_nDownPosY) > 3)
		{
			m_nStartX = -1;
			m_nStartY = -1;
			m_nDownPosX = m_nDownPosY = -1;
			return;
		}
		
		m_nStartX = -1;
		m_nStartY = -1;
		m_nDownPosX = m_nDownPosY = -1;
		int idInstance = GetWorld()->GetInstanceID();
		CECInstance *pInstance = GetGameRun()->GetInstance(idInstance);
		int nRow = pInstance->GetRowNum();
		int nCol = pInstance->GetColNum();
		A3DRECT rcMidMap, rcMap;
		A3DVECTOR3 vecPosHost = GetHostPlayer()->GetPos();
		float fScale = 1.0;
		float fOffsetX = 0.0f;
		float fOffsetZ = 0.0f;
		GetMidMapOffsetParam(fScale,fOffsetX,fOffsetZ);
		rcMap = pObj->GetRect();
		rcMidMap.left = int((vecPosHost.x/nRow - 512) / 1024) * 1024 - 512;
		rcMidMap.top = int((vecPosHost.z/nRow - 512) / 1024) * 1024 - 512;
		rcMidMap.right = rcMidMap.left + 1024;
		rcMidMap.bottom = rcMidMap.top + 1024;
		rcMidMap.left = m_rcRenderArea.left*fScale - 512 + fOffsetX;
		rcMidMap.top = m_rcRenderArea.top*fScale - 512  + fOffsetZ;
		rcMidMap.right = m_rcRenderArea.right*fScale - 512 + fOffsetX;
		rcMidMap.bottom = m_rcRenderArea.bottom*fScale - 512 + fOffsetZ;
		GetGameUIMan()->m_pDlgMiniMap->m_dwTempTargetTime = GetTickCount();
		float posX = float((x - int(p->X) - (rcMap.left + rcMap.Width() / 2 - rcMap.Height() / 2)) * 1024 * ZOOM_MIN / m_fZoom / rcMap.Height() + rcMidMap.left);
		float posY = float(-(y - int(p->Y) - rcMap.top) * 1024 * ZOOM_MIN / m_fZoom / rcMap.Height() - rcMidMap.top);
		GetGameUIMan()->m_pDlgMiniMap->m_vecTempTarget.x = nRow*SCREENTOPOINT_X(x);
		GetGameUIMan()->m_pDlgMiniMap->m_vecTempTarget.y = vecPosHost.y;
		GetGameUIMan()->m_pDlgMiniMap->m_vecTempTarget.z = nRow*SCREENTOPOINT_Y(y);
		GetGameUIMan()->m_pDlgMiniMap->m_idTempTargetInstance = GetWorld()->GetInstanceID();
		
		g_ECAutoMove.SearchPath(g_pGame->GetGameRun()->GetWorld()->GetHostPlayer()->GetPos(), 
			GetGameUIMan()->m_pDlgMiniMap->m_vecTempTarget,
			g_pGame->GetGameRun()->GetWorld()->GetInstanceID(),
			false);
		
		if( GetHostPlayer()->GetModelTempArrow() )
			GetHostPlayer()->GetModelTempArrow()->PlayActionByName("动画", 1.0f, true);
	}
	else
	{
		m_nStartX = -1;
		m_nStartY = -1;
		m_nDownPosX = m_nDownPosY = -1;
		float z;
		if (GetGame()->GetCurCursor()==RES_CUR_ZOOMIN)
		{
			z = ZOOM_LEVEL;
		}
		else if (GetGame()->GetCurCursor()==RES_CUR_ZOOMOUT)
		{
			z = -ZOOM_LEVEL;
		}
		else if (GetGame()->GetCurCursor()==RES_CUR_MOVE)
		{
			GetGameUIMan()->m_nCursorState = CECGameUIMan::CURSOR_STATE_NORMAL;
			GetGame()->ChangeCursor(RES_CUR_NORMAL);
			return;
		}

		if (m_fZoom+z < ZOOM_MIN)
			z = ZOOM_MIN - m_fZoom;
		else if (m_fZoom+z >ZOOM_MAX)
			z = ZOOM_MAX - m_fZoom;
		
// 		A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
// 		int x = GET_X_LPARAM(lParam);
// 		int y = GET_Y_LPARAM(lParam);
		int x, y;
		g_pGame->GetGameRun()->GetInputCtrl()->GetMousePos(&x, &y);
		
		int xLocal = x - m_pImg_MidMap->GetPos().x;
		int yLocal = y - m_pImg_MidMap->GetPos().y;

		int xMap = m_rcRenderArea.left + xLocal*(m_rcRenderArea.Width())/m_pImg_MidMap->GetRect().Width();
		int yMap = m_rcRenderArea.top + yLocal*(m_rcRenderArea.Height())/m_pImg_MidMap->GetRect().Height();

		m_rcRenderArea.left = int((m_rcRenderArea.left*m_fZoom + xMap*z)/(m_fZoom+z));
		m_rcRenderArea.right = int((m_rcRenderArea.right*m_fZoom + xMap*z)/(m_fZoom+z));
		m_rcRenderArea.top = int((m_rcRenderArea.top*m_fZoom + yMap*z)/(m_fZoom+z));
		m_rcRenderArea.bottom = int((m_rcRenderArea.bottom*m_fZoom + yMap*z)/(m_fZoom+z));
		m_fZoom += z;
		CheckRenderArea();
	}
}

void CDlgMidMap::OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
	int zDelta = (short)HIWORD(wParam);
	if( zDelta > 0 )
		OnCommand_ZoomIn("");
	else
		OnCommand_ZoomOut("");
	
	OnEventLButtonUp_MidMap(wParam, lParam, pObj);
	GetGameUIMan()->m_nCursorState = CECGameUIMan::CURSOR_STATE_NORMAL;
	GetGame()->ChangeCursor(RES_CUR_NORMAL);
}

void CDlgMidMap::OnEventRButtonDown_ListRegion(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	PAUILISTBOX	pList = (PAUILISTBOX)pObj;
	int nSel = pList->GetCurSel();
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	POINT ptPos = pObj->GetPos();
	int x = GET_X_LPARAM(lParam) - ptPos.x - p->X;
	int y = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;
	if( nSel >= 0 && nSel < pList->GetCount()  &&
		pList->GetHitArea(x, y) == AUILISTBOX_RECT_TEXT )
	{
		if( stricmp(pList->GetName(), "List_Choose") == 0 )
			g_ECAutoMove.SearchPath(GetHostPlayer()->GetPos(), GetGameUIMan()->m_pDlgMiniMap->m_vecMark[nSel].vecPos, GetGameUIMan()->m_pDlgMiniMap->m_vecMark[nSel].idInstance);
		else
		{
			A3DVECTOR3 vec;
			vec.x = (float)(int)(pList->GetItemData(nSel, 0));
			vec.y = (float)(int)(pList->GetItemData(nSel, 1));
			vec.z = (float)(int)(pList->GetItemData(nSel, 2));
			
			GetGameUIMan()->m_pDlgMiniMap->m_dwTempTargetTime = GetTickCount();
			GetGameUIMan()->m_pDlgMiniMap->m_vecTempTarget = vec;
			GetGameUIMan()->m_pDlgMiniMap->m_idTempTargetInstance = GetWorld()->GetInstanceID();
			
			g_ECAutoMove.SearchPath(GetHostPlayer()->GetPos(), vec, GetWorld()->GetInstanceID());
		}
		
		if( GetHostPlayer()->GetModelTempArrow() )
				GetHostPlayer()->GetModelTempArrow()->PlayActionByName("动画", 1.0f, true);
	}
	GetGameRun()->GetHelpGraphGuide()->FinishHelp(CECHelpGraphGuide::HELP2012_8_2_MAP_NPCMINE_LIST);
}

void CDlgMidMap::CheckRenderArea()
{
// 	int idInstance = GetWorld()->GetInstanceID();
// 	CECInstance *pInstance = GetGameRun()->GetInstance(idInstance);
// 	int nRow = pInstance->GetRowNum();
// 	int nCol = pInstance->GetColNum();
// 	int fTotalW = nCol * (int)g_fCell;
// 	int fTotalH = nRow * (int)g_fCell;
	if( m_rcRenderArea.left < 0 ) {
		int offX = m_rcRenderArea.left;
		
		m_rcRenderArea.left -= offX;
		m_rcRenderArea.right -= offX;
	}
	if( m_rcRenderArea.right > g_nRenderAreaBase ) {
		int offX = (int)(m_rcRenderArea.right - g_nRenderAreaBase);
		
		m_rcRenderArea.left -= offX;
		m_rcRenderArea.right -= offX;
	}
	if( m_rcRenderArea.top < 0 ) {
		int offY = m_rcRenderArea.top;
		
		m_rcRenderArea.top -= offY;
		m_rcRenderArea.bottom -= offY;
	}
	if( m_rcRenderArea.bottom > g_nRenderAreaBase ) {
		int offY = (int)(m_rcRenderArea.bottom - g_nRenderAreaBase);
		m_rcRenderArea.top -= offY;
		m_rcRenderArea.bottom -= offY;
	}
	m_rcRenderArea &= A3DRECT(0, 0, g_nRenderAreaBase, g_nRenderAreaBase);
}

void CDlgMidMap::OnEventRButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	GetGameUIMan()->RespawnMessage();
}

void CDlgMidMap::OnEventRButtonUp(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
#if 0
	if (AUI_PRESS(VK_CONTROL))
	{
		A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
		int x = GET_X_LPARAM(lParam) - p->X;
		int y = GET_Y_LPARAM(lParam) - p->Y;

		A3DRECT rcMidMap, rcMap;
		A3DVECTOR3 vecPosHost = GetHostPlayer()->GetPos();
		float fScale = 1.0;
		float fOffsetX = 0.0f;
		float fOffsetZ = 0.0f;
		GetMidMapOffsetParam(fScale,fOffsetX,fOffsetZ);
		rcMap = pObj->GetRect();
		rcMidMap.left = int((vecPosHost.x - 512) / 1024) * 1024 - 512;
		rcMidMap.top = int((vecPosHost.z - 512) / 1024) * 1024 - 512;
		rcMidMap.right = rcMidMap.left + 1024;
		rcMidMap.bottom = rcMidMap.top + 1024;
		A3DVECTOR3 des;
		des.Set(SCREENTOPOINT_X(x), vecPosHost.y, SCREENTOPOINT_Y(y));
		g_AutoMoveAgent.Init(vecPosHost, des, 3.0f);
		m_nSearchTime = GetTickCount();
		m_nSearchRet = g_AutoMoveAgent.Search();
		m_nSearchTime = GetTickCount() - m_nSearchTime;
	}
#else
	GetGameUIMan()->RespawnMessage();
#endif
}

bool CDlgMidMap::OnInitDialog()
{
	m_pImg_MidMap = (PAUIIMAGEPICTURE)GetDlgItem("Img_MidMap");
	m_pSld_MidMap = (PAUISLIDER)GetDlgItem("Sld_MidMap");
	m_pSld_MidMap->SetLevel(255);
	m_pBtn_ZoomIn = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_ZoomIn");
	m_pBtn_ZoomOut = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_ZoomOut");
	m_pEdt_Search = (PAUIEDITBOX)GetDlgItem("Edt_Input");
	m_pLab_SearchHint = (PAUILABEL)GetDlgItem("Lab_SearchHint");

	// now create a dynamic stream for map render
	if( !m_pMidMapStream )
	{
		m_pMidMapStream = new A3DStream();
		if( !m_pMidMapStream->Init(m_pA3DDevice, sizeof(MAPTLVERTEX),
			FVF_MAPTLVERTEX, 6, 0, A3DSTRM_REFERENCEPTR, 0) )
		{
			return AUI_ReportError(__LINE__, __FILE__);
		}
	}
	
	m_mapNpcMonsterPos.clear();
	AWScriptFile ScriptFile;
	if (ScriptFile.Open("surfaces\\NpcMonsterPos.txt"))
	{
		while(ScriptFile.GetNextToken(true))
		{
			if (ScriptFile.m_szToken[0]==_AL('-') && ScriptFile.m_szToken[1] == _AL('-'))
			{
				ScriptFile.SkipLine();
				continue;
			}
			int mapid;
			NpcMonsterPos pos;
			pos.strName = ScriptFile.m_szToken;
			if(!ScriptFile.GetNextToken(false))
				continue;
			pos.x = a_atoi(ScriptFile.m_szToken);
			if(!ScriptFile.GetNextToken(false))
				continue;
			pos.y = a_atoi(ScriptFile.m_szToken);
			if(!ScriptFile.GetNextToken(false))
				continue;
			pos.z = a_atoi(ScriptFile.m_szToken);
			if(!ScriptFile.GetNextToken(false))
				continue;
			mapid = a_atoi(ScriptFile.m_szToken);
			
			if( m_mapNpcMonsterPos.find(mapid) != m_mapNpcMonsterPos.end() )
				m_mapNpcMonsterPos[mapid].push_back(pos);
			else
			{
				vecMonsterPos vecPos;
				vecPos.clear();
				vecPos.push_back(pos);
				m_mapNpcMonsterPos[mapid] = vecPos;
			}
		}

		ScriptFile.Close();
	}
	else
	{
		assert(0);
		AUI_ReportError(__LINE__, "surfaces\\NpcMonsterPos.txt open error");
	}

	InitTownNpcMonsterPos();
    
	
	m_pFont = GetGameUIMan()->GetA3DFTFontMan()->CreatePointFont(14, L"方正隶变简体", STYLE_NORMAL);
	if( !m_pFont ) 
	{
		AUI_ReportError(__LINE__, 1, "CDlgMidMap::OnInitDialog(), failed to call m_pFontMan->CreatePointFont()");
	}

	m_fZoom = 1.f;
	m_dwLBtnDownTime = 0;
	
	GetDlgItem("Shape_1")->Show(false);
	GetDlgItem("Img_1")->Show(false);
	GetDlgItem("Txt_1")->Show(false);

	for(int i = 0; i < 3; i++)
		m_bEvent[i] = false;

	m_iCurMapType = MAP_TYPE_CITY;

	return true;
}
void CDlgMidMap::InitTownNpcMonsterPos()
{
	m_mapTownNpcMonsterPos.clear();
	AWScriptFile ScriptFile;
	if (ScriptFile.Open("surfaces\\TownNpcMonsterpos.txt"))
	{
		while(ScriptFile.GetNextToken(true))
		{
			if (ScriptFile.m_szToken[0]==_AL('-') && ScriptFile.m_szToken[1] == _AL('-'))
			{
				ScriptFile.SkipLine();
				continue;
			}
			int mapid;
			TownNpcMonsterPos pos;
			pos.strName = ScriptFile.m_szToken;
			if(!ScriptFile.GetNextToken(false))
				continue;
			pos.x = a_atoi(ScriptFile.m_szToken);
			if(!ScriptFile.GetNextToken(false))
				continue;
			pos.y = a_atoi(ScriptFile.m_szToken);
			if(!ScriptFile.GetNextToken(false))
				continue;
			pos.z = a_atoi(ScriptFile.m_szToken);
			if(!ScriptFile.GetNextToken(false))
				continue;
            pos.strIconPath = WC2AS(ScriptFile.m_szToken);
			if(!ScriptFile.GetNextToken(false))
				continue;
			mapid = a_atoi(ScriptFile.m_szToken);
			

			if( m_mapTownNpcMonsterPos.find(mapid) != m_mapTownNpcMonsterPos.end() )
				m_mapTownNpcMonsterPos[mapid].push_back(pos);
			else
			{
				vecTownNpcMonsterPos vecPos;
				vecPos.clear();
				vecPos.push_back(pos);
				m_mapTownNpcMonsterPos[mapid] = vecPos;
			}
		}

		ScriptFile.Close();
	}
	else
	{
		assert(0);
		AUI_ReportError(__LINE__, "surfaces\\TownNpcMonsterpos.txt open error");
	}
}
void CDlgMidMap::OnShowDialog()
{
	GetMapTransferMark(GetWorld()->GetInstanceID());
	OnCommand_Search("");
	if (GetHostPlayer()->GetSrcServerID()==0)
	{
		GetDlgItem("Btn_Worldmap")->Show(true);
		GetDlgItem("Btn_Worldmap_Terri")->Show(true);
		GetDlgItem("Btn_KfMap")->Show(false);
	}
	else
	{
		GetDlgItem("Btn_Worldmap")->Show(false);
		GetDlgItem("Btn_Worldmap_Terri")->Show(false);
		GetDlgItem("Btn_KfMap")->Show(true);
	}

	GetDlgItem("Btn_ToMap02")->Show(false);
	GetDlgItem("Btn_ToMap03")->Show(false);
	m_iCurMapType = MAP_TYPE_CITY;
    if(IsInTownPrecinct())
	{
        m_iCurMapType = MAP_TYPE_TOWN;
		GetDlgItem("Btn_ToMap02")->Show(true);
	}
}

void CDlgMidMap::OnTick()
{
	int iCurCursor = GetGame()->GetCurCursor();
	m_pBtn_ZoomIn->SetPushed(iCurCursor == RES_CUR_ZOOMIN);
	m_pBtn_ZoomOut->SetPushed(iCurCursor == RES_CUR_ZOOMOUT);
	ACString strText = m_pEdt_Search->GetText();
	if (m_strSearch != strText)
	{
		m_strSearch = strText;
		OnCommand_Search("");
	}
	m_pLab_SearchHint->Show(strText.IsEmpty() && GetFocus()!=m_pEdt_Search);
	if (m_dwLBtnDownTime>0 && GetTickCount()>(m_dwLBtnDownTime+200))
	{		
		GetGameUIMan()->m_nCursorState = CECGameUIMan::CURSOR_STATE_ZOOM;
		GetGame()->ChangeCursor(RES_CUR_MOVE);
		m_dwLBtnDownTime = 0;
	}

	if(!IsInTownPrecinct())
	{
		GetDlgItem("Btn_ToMap02")->Show(false);
		GetDlgItem("Btn_ToMap03")->Show(false);
		m_iCurMapType = MAP_TYPE_CITY;
	}
	else
	{
	    if(m_iCurMapType == MAP_TYPE_CITY)
		{
		   GetDlgItem("Btn_ToMap03")->Show(true);
		   GetDlgItem("Btn_ToMap02")->Show(false);
		}
		else
		{
		   GetDlgItem("Btn_ToMap02")->Show(true);
		   GetDlgItem("Btn_ToMap03")->Show(false);
		}
	}
	

}

bool CDlgMidMap::Render()
{
	if( GetWorld()->GetInstanceID() == 15 && 
		GetHostPlayer()->GetPos().x > -473 && GetHostPlayer()->GetPos().x < -381 &&
		GetHostPlayer()->GetPos().z > -444 && GetHostPlayer()->GetPos().z < -194)
	{
		Show(false);
		return true;
	}
	// Alpha
	int alpha = max(m_pSld_MidMap->GetLevel(), 1);
	int j;
	for(j = 0; j < 4; j++ )
		m_vertexMidMap[j].diffuse = A3DCOLORRGBA(255, 255, 255, alpha);


	int idInstance = GetWorld()->GetInstanceID();
	SetData(idInstance);
	CECInstance *pInstance = GetGameRun()->GetInstance(idInstance);

	AString strSubPath = pInstance->GetPath();
	strSubPath += pInstance->GetSubPath(GetHostPlayer()->GetPos().x, GetHostPlayer()->GetPos().z);
	
	AString strFile = AString("Surfaces\\MidMaps\\") + strSubPath + ".dds";

    if(m_iCurMapType == MAP_TYPE_TOWN)
	{
	    strFile = GetWorld()->GetCurPrecinct()->GetMiniMap();
	}
	if( strFile != m_szPathName )
	{
		if( !m_pTexRadar )
			m_pTexRadar = new A3DTexture;
		else
			m_pTexRadar->Release();
		m_pTexRadar->SetNoDownSample(true);
		m_pTexRadar->LoadFromFile(m_pA3DDevice, strFile, strFile);
		m_szPathName = strFile;
		m_fZoom = ZOOM_MIN;
		m_rcRenderArea.SetRect(0, 0, 99999, 99999);
		CheckRenderArea();
	}

	ACString strText;
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	float fWindowScale = m_pAUIManager->GetWindowScale();
	int nRow = pInstance->GetRowNum();
	int nCol = pInstance->GetColNum();
	float fTotalW = nCol * g_fCell;
	float fTotalH = nRow * g_fCell;
	float fOffsetX = fTotalW * 0.5f;
	float fOffsetY = fTotalH * 0.5f;
	
	A3DRECT rcMidMap = m_pImg_MidMap->GetRect();
	if( rcMidMap.Width() < rcMidMap.Height())
		rcMidMap.bottom = rcMidMap.top + rcMidMap.Width();
	else if( rcMidMap.Width() > rcMidMap.Height())
		rcMidMap.right= rcMidMap.left + rcMidMap.Height();
	
	if( rcMidMap.Width() % 2 != 0 )
		rcMidMap.right -= 1;
	if( rcMidMap.Height() % 2 != 0  )
		rcMidMap.bottom -= 1;
	int W = rcMidMap.Width();
	int H = rcMidMap.Height();
	A3DPOINT2 C = rcMidMap.CenterPoint();
	
	ARectF rcRenderArea;
	rcRenderArea.SetRect(-W / 2.0f, -H / 2.0f, W / 2.0f, H / 2.0f);
	m_vertexMidMap[0].x = rcRenderArea.left;
	m_vertexMidMap[0].y = rcRenderArea.top;
	m_vertexMidMap[1].x = rcRenderArea.right;
	m_vertexMidMap[1].y = rcRenderArea.top;
	m_vertexMidMap[2].x = rcRenderArea.left;
	m_vertexMidMap[2].y = rcRenderArea.bottom;
	m_vertexMidMap[3].x = rcRenderArea.right;
	m_vertexMidMap[3].y = rcRenderArea.bottom;
	
	m_vertexMidMap[0].tu = m_rcRenderArea.left	/ g_fCell;
	m_vertexMidMap[0].tv = m_rcRenderArea.top	/ g_fCell;
	m_vertexMidMap[1].tu = m_rcRenderArea.right	/ g_fCell;
	m_vertexMidMap[1].tv = m_rcRenderArea.top	/ g_fCell;
	m_vertexMidMap[2].tu = m_rcRenderArea.left	/ g_fCell;
	m_vertexMidMap[2].tv = m_rcRenderArea.bottom/ g_fCell;
	m_vertexMidMap[3].tu = m_rcRenderArea.right	/ g_fCell;
	m_vertexMidMap[3].tv = m_rcRenderArea.bottom/ g_fCell;
	
	m_pA3DDevice->SetTextureAddress(0, A3DTADDR_CLAMP, A3DTADDR_CLAMP);

	static WORD a_wIndex[6] = { 0, 1, 2, 2, 1, 3 };
	A3DVECTOR3 vecPos = A3DVECTOR3(0.0f, 0.0f, 0.0f);
	m_pTexRadar->Appear(0);
	
	bool bval;
	for( j = 0; j < 4; j++ )
	{
		m_vertexMidMap[j].tu2 = float(m_vertexMidMap[j].x + C.x - rcMidMap.left) / float(W);
		m_vertexMidMap[j].tv2 = float(m_vertexMidMap[j].y + C.y - rcMidMap.top) / float(H);
		
		m_vertexMidMap[j].x += C.x + p->X;
		m_vertexMidMap[j].y += C.y + p->Y;
		
		m_vertexMidMap[j].x -= 0.5f;
		m_vertexMidMap[j].y -= 0.5f;
	}
	
	MAPTLVERTEX * pVerts;
	if( !m_pMidMapStream->LockVertexBuffer(0, 0, (BYTE**) &pVerts, 0) )
		return false;
	
	pVerts[0] = m_vertexMidMap[0];
	pVerts[1] = m_vertexMidMap[1];
	pVerts[2] = m_vertexMidMap[2];
	
	pVerts[3] = m_vertexMidMap[2];
	pVerts[4] = m_vertexMidMap[1];
	pVerts[5] = m_vertexMidMap[3];
	
	m_pMidMapStream->UnlockVertexBuffer();
	m_pMidMapStream->Appear();
	
	bval = m_pA3DDevice->DrawPrimitive(A3DPT_TRIANGLELIST, 0, 2);
	if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
	
	m_pTexRadar->Disappear(0);
	
	m_pA3DDevice->SetTextureAddress(0, A3DTADDR_WRAP, A3DTADDR_WRAP);
	
	m_pA3DDevice->SetLightingEnable(true);


	m_pImg_MidMap->SetAlpha(m_pSld_MidMap->GetLevel());
	SetAlpha(m_pSld_MidMap->GetLevel());
	
    m_pImg_MidMap->SetRenderCallback(MidMapRenderCallback, (DWORD)this);
	

	A3DVECTOR3 vecDir = GetHostPlayer()->GetDir();
	int nDegree = 90 - (int)RAD2DEG((float)atan2(vecDir.z, vecDir.x));
	GetGameUIMan()->m_pDlgMiniMap->m_pA2DMapMark[CDlgMiniMap::MAPMARK_HOST]->SetDegree(nDegree);

	bool bRet = CDlgBase::Render();

#if 0

	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	A3DRECT rcMidMap, rcMap;
	A3DVECTOR3 vecPosHost = GetHostPlayer()->GetPos();
	float fScale = 1.0;
	float fOffsetX = 0.0f;
	float fOffsetZ = 0.0f;
	GetMidMapOffsetParam(fScale,fOffsetX,fOffsetZ);
	rcMap = GetDlgItem("Img_MidMap")->GetRect();
	rcMidMap.left = int((vecPosHost.x - 512) / 1024) * 1024 - 512;
	rcMidMap.top = int((vecPosHost.z - 512) / 1024) * 1024 - 512;
	rcMidMap.right = rcMidMap.left + 1024;
	rcMidMap.bottom = rcMidMap.top + 1024;
	vector<A3DPOINT2> *path = g_AutoMoveAgent.GetPath();
	if( path )
	{
		DWORD i;
		for(i = 1; i < path->size(); i++)
		{
			A3DPOINT2 p1, p2;
			p1.x = POINTTOSCREEN_X((*path)[i - 1].x - 512);
			p1.y = POINTTOSCREEN_Y((*path)[i - 1].y - 512);
			p2.x = POINTTOSCREEN_X((*path)[i].x - 512);
			p2.y = POINTTOSCREEN_Y((*path)[i].y - 512);
			g_pA3DGDI->Draw2DLine(p1, p2, A3DCOLORRGB(255, 255, 255));
		}
		ACString strRet, strTime;
		switch(m_nSearchRet)
		{
		case AutoMove::PF2D_UNKNOWN:
			strRet = _AL("PF2D_UNKNOWN");
			break;
		case AutoMove::PF2D_SEARCHING:
			strRet = _AL("PF2D_SEARCHING");
			break;
		case AutoMove::PF2D_FOUND:
			strRet = _AL("PF2D_FOUND");
			break;
		case AutoMove::PF2D_NOPATH:
			strRet = _AL("PF2D_NOPATH");
			break;
		case AutoMove::PF2D_INVALIDSTART:
			strRet = _AL("PF2D_INVALIDSTART");
			break;
		case AutoMove::PF2D_INVALIDEND:
			strRet = _AL("PF2D_INVALIDEND");
			break;
		case AutoMove::PF2D_EXCEED:
			strRet = _AL("PF2D_EXCEED");
			break;
		}
		strTime.Format(_AL("%dms   "), m_nSearchTime);
		g_pGame->GetA3DEngine()->GetSystemFont()->TextOut(0, 300, strTime + strRet, 0xFFFFFFFF);
	}
#endif

	return bRet;
}


void CDlgMidMap::SetEvent(int id, bool bevent)
{
	if(id >= 0 && id <= 2)
		m_bEvent[id] = bevent;
}

void CDlgMidMap::MidMapRenderCallback(const A3DRECT &rc, DWORD param1, DWORD param2, DWORD param3)
{
	CDlgMidMap *pDlgMap = (CDlgMidMap *)param1;
	CDlgMiniMap *pDlgMiniMap = pDlgMap->GetGameUIMan()->m_pDlgMiniMap;
	A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();

	int idInstance = pDlgMap->GetWorld()->GetInstanceID();
	CECInstance *pInstance = pDlgMap->GetGameRun()->GetInstance(idInstance);
	CECHostPlayer *pHost = pDlgMap->GetHostPlayer();
	A3DVECTOR3 vecPosHost = pHost->GetPos();
	float fScale = 1.0;
	float fOffsetX = 0.0f;
	float fOffsetZ = 0.0f;
	pDlgMap->GetMidMapOffsetParam(fScale,fOffsetX,fOffsetZ);
	float fWindowScale = pDlgMap->GetGameUIMan()->GetWindowScale();
	float fSize = fWindowScale / 2;
	A3DRECT rcMidMap, rcMap, rcMidMapReal;
	int nRow = pInstance->GetRowNum();
	int nCol = pInstance->GetColNum();
	rcMap = pDlgMap->m_pImg_MidMap->GetRect();
	rcMidMap.left = int((vecPosHost.x/nRow - 512) / 1024) * 1024 - 512;
	rcMidMap.top = int((vecPosHost.z/nRow - 512) / 1024) * 1024 - 512;
	rcMidMap.right = rcMidMap.left + 1024;
	rcMidMap.bottom = rcMidMap.top + 1024;
	rcMidMap.left = pDlgMap->m_rcRenderArea.left*fScale - 512 + fOffsetX;
	rcMidMap.top = pDlgMap->m_rcRenderArea.top*fScale - 512 + fOffsetZ;
	rcMidMap.right = pDlgMap->m_rcRenderArea.right*fScale - 512 + fOffsetX;
	rcMidMap.bottom = pDlgMap->m_rcRenderArea.bottom*fScale - 512 + fOffsetZ;
	rcMidMapReal.left = rcMidMap.left * nCol;
	rcMidMapReal.right = rcMidMap.right * nCol;
	rcMidMapReal.top = rcMidMap.top * nRow;
	rcMidMapReal.bottom = rcMidMap.bottom * nRow;

	int i;
	A3DPOINT2 ptPos;
	int idTarget = pHost->GetSelectedTarget();
	bool bTarget = false;

	MAPMARK Mark;
	CECNPCServer *pServer;
	CECNPCMan *pNPCMan = pDlgMap->GetWorld()->GetNPCMan();
	const APtrArray<CECNPC *> &NPCs = pNPCMan->GetNPCsInMiniMap();
	EC_VIDEO_SETTING vs = g_pGame->GetConfigs()->GetVideoSettings();

	CECFriendMan::GROUP *pGroup;
	CECFriendMan::FRIEND *pFriend;
	CECTeam *pTeam = pHost->GetTeam();
	CECFriendMan *pMan = pHost->GetFriendMan();
	CECPlayerMan *pPlayerMan = pDlgMap->GetWorld()->GetPlayerMan();
	const APtrArray<CECElsePlayer *> &Players = pPlayerMan->GetPlayersInMiniMap();

	int iTeamMenber = pHost->GetTeam() ? pHost->GetTeam()->GetMemberNum() : 0;
	CDlgMiniMap::RadarPoint *rp = new CDlgMiniMap::RadarPoint
		[iTeamMenber + Players.GetSize() + NPCs.GetSize() + pDlgMiniMap->m_vecMark.size() + 3];
	int n = 0;

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

	pDlgMap->m_vecMidMapMark.clear();

	// player
	if( vs.bMapPlayer )
	{
		for( i = 0; i < Players.GetSize(); i++ )
		{
			if( pTeam && pTeam->GetMemberByID(Players[i]->GetCharacterID()) )
				continue;

			ptPos.x = int(Players[i]->GetPos().x);
			ptPos.y = int(Players[i]->GetPos().z);
			if( !rcMidMapReal.PtInRect(ptPos.x, -ptPos.y) )
				continue;

			rp[n].pos = ptPos;
			if( Players[i]->GetCharacterID() == idTarget )
			{
				rp[n].type = CDlgMiniMap::MAPMARK_TARGET;
				n++;
				rp[n].pos = ptPos;
			}
			int battleType = pDlgMap->GetHostPlayer()->GetBattleType();
			if ( battleType==BATTLE_TYPE_ARENA || battleType==BATTLE_TYPE_ARENAREBORN || battleType==BATTLE_TYPE_CRSSVR )
			{
				rp[n].type = CDlgMiniMap::MAPMARK_PLAYER;
			}
			else if( pHost->IsInBattle() )
			{
				if( Players[i]->GetBattleCamp() == pHost->GetBattleCamp() )
					rp[n].type = CDlgMiniMap::MAPMARK_TEAMMATE;
				else if( Players[i]->GetBattleCamp() == GP_BATTLE_CAMP_OBSERVER ) // 不显示观战方
					continue;
				else
					rp[n].type = CDlgMiniMap::MAPMARK_ENEMY;
			}
			else if( pHost->GetCombatFaction() != 0 && Players[i]->GetFactionID() == pHost->GetCombatFaction() )
				rp[n].type = CDlgMiniMap::MAPMARK_ENEMYGUILD;
			else if( pDlgMap->GetGameUIMan()->m_pDlgFriendList->IsEnemy(Players[i]->GetCharacterID()) )
			{
				rp[n].type = CDlgMiniMap::MAPMARK_ENEMY;
				int W = int(pDlgMiniMap->m_pA2DMapMark[CDlgMiniMap::MAPMARK_ENEMY]->GetWidth() * fSize);
				int H = int(pDlgMiniMap->m_pA2DMapMark[CDlgMiniMap::MAPMARK_ENEMY]->GetHeight() * fSize);
				Mark.strHint = Players[i]->GetName();
				Mark.vecPos = Players[i]->GetPos();
				Mark.rect.SetRect(POINTTOSCREEN_X(ptPos.x) - W, POINTTOSCREEN_Y(ptPos.y) - H,
						POINTTOSCREEN_X(ptPos.x) + W, POINTTOSCREEN_Y(ptPos.y) + H);
				m_vecMidMapMark.push_back(Mark);
			}
			else if( pFriend = pMan->GetFriendByID(Players[i]->GetCharacterID()) )
			{
				rp[n].type = CDlgMiniMap::MAPMARK_FRIEND;
				pGroup = pMan->GetGroupByID(pFriend->iGroup);
				if( pGroup )
				{
					int W = int(pDlgMiniMap->m_pA2DMapMark[CDlgMiniMap::MAPMARK_FRIEND]->GetWidth() * fSize);
					int H = int(pDlgMiniMap->m_pA2DMapMark[CDlgMiniMap::MAPMARK_FRIEND]->GetHeight() * fSize);
					Mark.strHint = Players[i]->GetName();
					Mark.vecPos = Players[i]->GetPos();
					Mark.rect.SetRect(POINTTOSCREEN_X(ptPos.x) - W, POINTTOSCREEN_Y(ptPos.y) - H,
							POINTTOSCREEN_X(ptPos.x) + W, POINTTOSCREEN_Y(ptPos.y) + H);
					m_vecMidMapMark.push_back(Mark);
				}
			}
			else if( pHost->GetFactionID() != 0 && Players[i]->GetFactionID() == pHost->GetFactionID() )
			{
				rp[n].type = CDlgMiniMap::MAPMARK_GUILD;
				int W = int(pDlgMiniMap->m_pA2DMapMark[CDlgMiniMap::MAPMARK_GUILD]->GetWidth() * fSize);
				int H = int(pDlgMiniMap->m_pA2DMapMark[CDlgMiniMap::MAPMARK_GUILD]->GetHeight() * fSize);
				Mark.strHint = Players[i]->GetName();
				Mark.vecPos = Players[i]->GetPos();
				Mark.rect.SetRect(POINTTOSCREEN_X(ptPos.x) - W, POINTTOSCREEN_Y(ptPos.y) - H,
						POINTTOSCREEN_X(ptPos.x) + W, POINTTOSCREEN_Y(ptPos.y) + H);
				m_vecMidMapMark.push_back(Mark);
			}
			else if( pHost->GetFamilyID() != 0 && Players[i]->GetFamilyID() == pHost->GetFamilyID() )
			{
				rp[n].type = CDlgMiniMap::MAPMARK_FAMILY;
				int W = int(pDlgMiniMap->m_pA2DMapMark[CDlgMiniMap::MAPMARK_FAMILY]->GetWidth() * fSize);
				int H = int(pDlgMiniMap->m_pA2DMapMark[CDlgMiniMap::MAPMARK_FAMILY]->GetHeight() * fSize);
				Mark.strHint = Players[i]->GetName();
				Mark.vecPos = Players[i]->GetPos();
				Mark.rect.SetRect(POINTTOSCREEN_X(ptPos.x) - W, POINTTOSCREEN_Y(ptPos.y) - H,
						POINTTOSCREEN_X(ptPos.x) + W, POINTTOSCREEN_Y(ptPos.y) + H);
				m_vecMidMapMark.push_back(Mark);
			}
			else
				rp[n].type = CDlgMiniMap::MAPMARK_PLAYER;
			n++;
		}
	}

	// NPC
	for( i = 0; i < NPCs.GetSize(); i++ )
	{
		ptPos.x = int(NPCs[i]->GetPos().x);
		ptPos.y = int(NPCs[i]->GetPos().z);
		
		if( !rcMidMapReal.PtInRect(ptPos.x, -ptPos.y) )
			continue;
		if(!NPCs[i]->IsPhaseShow())
			continue;

		if( NPCs[i]->GetNPCID() == idTarget )
		{
			rp[n].pos = ptPos;
			rp[n].type = CDlgMiniMap::MAPMARK_TARGET;
			n++;
		}
		
		if( pHost->IsInBattle() )
		{
			elementdataman* pElementDataMan = g_pGame->GetElementDataMan();
			DATA_TYPE dt;
			NPC_ESSENCE* pNPCEssence = (NPC_ESSENCE*)pElementDataMan->get_data_ptr(
				NPCs[i]->GetTemplateID(), ID_SPACE_ESSENCE,dt);

			if( pNPCEssence && pNPCEssence->id_src_monster )
			{
				MONSTER_ESSENCE* pEssence = (MONSTER_ESSENCE*)pElementDataMan->get_data_ptr(
					pNPCEssence->id_src_monster, ID_SPACE_ESSENCE,dt); 
				if( pEssence )
				{
					if( pEssence->faction & 16 )
					{
						rp[n].type = CDlgMiniMap::MAPMARK_BUILDINGB;
						rp[n].pos = ptPos;
						n++;
						continue;
					}
					else if( pEssence->faction & 32 )
					{
						rp[n].type = CDlgMiniMap::MAPMARK_BUILDINGA;
						rp[n].pos = ptPos;
						n++;
						continue;
					}
				}
			}
		}

		if( (NPCs[i]->IsServerNPC() && !vs.bMapNPC) ||
			(!NPCs[i]->IsServerNPC() && !vs.bMapMonster) )
		{
			continue;
		}
		
		rp[n].pos = ptPos;

		if( NPCs[i]->IsServerNPC() )
		{
			pServer = (CECNPCServer *)NPCs[i];
			int nDraw;
			if( pServer->GetCurQuestIcon() == CECNPCServer::QI_IN ||
				pServer->GetCurQuestIcon() == CECNPCServer::QI_IN_K )
			{
				nDraw = CDlgMiniMap::MAPMARK_NPCFINISHTASK;
			}
			else if( pServer->GetCurQuestIcon() == CECNPCServer::QI_OUT ||
				pServer->GetCurQuestIcon() == CECNPCServer::QI_OUT_K )
				nDraw = CDlgMiniMap::MAPMARK_NPCACCEPTTASK;
			else
				nDraw = CDlgMiniMap::MAPMARK_NPC;

			rp[n].type = nDraw;
			int W = int(pDlgMiniMap->m_pA2DMapMark[nDraw]->GetWidth() * fSize);
			int H = int(pDlgMiniMap->m_pA2DMapMark[nDraw]->GetHeight() * fSize);
			Mark.strHint = NPCs[i]->GetName();
			Mark.vecPos = NPCs[i]->GetPos();
			Mark.rect.SetRect(POINTTOSCREEN_X(ptPos.x) - W, POINTTOSCREEN_Y(ptPos.y) - H,
					POINTTOSCREEN_X(ptPos.x) + W, POINTTOSCREEN_Y(ptPos.y) + H);
			m_vecMidMapMark.push_back(Mark);
		}
		else
		{
			rp[n].type = CDlgMiniMap::MAPMARK_MONSTER;
			if( NPCs[i]->IsMonsterNPC() )
			{

				if( CDlgTask::m_mapTaskMonster.find(NPCs[i]->GetTemplateID()) != CDlgTask::m_mapTaskMonster.end() )
					rp[n].type = CDlgMiniMap::MAPMARK_MONSTERTASK;
				else
				{
					int nShowLevel = 0;
					elementdataman* pElementDataMan = g_pGame->GetElementDataMan();
					DATA_TYPE dt;
					MONSTER_ESSENCE* pEssence = (MONSTER_ESSENCE*)pElementDataMan->get_data_ptr(
						NPCs[i]->GetTemplateID(), ID_SPACE_ESSENCE,dt); 
					if (pEssence && dt == DT_MONSTER_ESSENCE )
						nShowLevel = pEssence->show_level;
					if( nShowLevel == 4 )
						rp[n].type = CDlgMiniMap::MAPMARK_MONSTERBOSS;
				}
			}
		}
		n++;
	}

	// team member
	if( vs.bMapPlayer )
	{
		CECTeamMember *pMember;
		CECTeam *pTeam = pHost->GetTeam();
		CECFriendMan *pMan = pHost->GetFriendMan();
		for( i = 0; pTeam && i < pTeam->GetMemberNum(); i++ )
		{
			pMember = pTeam->GetMemberByIndex(i);
			if( pMember->GetCharacterID() == pHost->GetCharacterID() ||
				pMember->GetInstanceID() != pDlgMap->GetWorld()->GetInstanceID() )
			{
				continue;
			}

			ptPos.x = int(pMember->GetPos().x);
			ptPos.y = int(pMember->GetPos().z);
			if( !rcMidMapReal.PtInRect(ptPos.x, -ptPos.y) )
				continue;

			int nDraw;
			if( pMember->GetCharacterID() == pTeam->GetLeaderID() )
				nDraw = CDlgMiniMap::MAPMARK_CAPTAIN;
			else
				nDraw = CDlgMiniMap::MAPMARK_TEAMMATE;
			rp[n].pos = ptPos;
			rp[n].type = nDraw;
			pDlgMiniMap->m_pA2DMapMark[nDraw]->DrawToInternalBuffer(POINTTOSCREEN_X(ptPos.x), POINTTOSCREEN_Y(ptPos.y));

			int W = int(pDlgMiniMap->m_pA2DMapMark[nDraw]->GetWidth() * fSize);
			int H = int(pDlgMiniMap->m_pA2DMapMark[nDraw]->GetHeight() * fSize);
			Mark.strHint = pMember->GetName();
			Mark.vecPos = pMember->GetPos();
			Mark.rect.SetRect(POINTTOSCREEN_X(ptPos.x) - W, POINTTOSCREEN_Y(ptPos.y) - H,
					POINTTOSCREEN_X(ptPos.x) + W, POINTTOSCREEN_Y(ptPos.y) + H);
			pDlgMap->m_vecMidMapMark.push_back(Mark);

			n++;
		}
	}

	// customize marks
	for( i = 0; i < (int)pDlgMiniMap->m_vecMark.size(); i++ )
		if( pDlgMiniMap->m_vecMark[i].idInstance == g_pGame->GetGameRun()->GetWorld()->GetInstanceID() )
		{
			ptPos.x = (int)pDlgMiniMap->m_vecMark[i].vecPos.x;
			ptPos.y = (int)pDlgMiniMap->m_vecMark[i].vecPos.z;

			rp[n].pos = ptPos;
			rp[n].type = i + CDlgMiniMap::MAPMARK_TAG0;

			int W = int(pDlgMiniMap->m_pA2DMapMark[i + CDlgMiniMap::MAPMARK_TAG0]->GetWidth() * fSize);
			int H = int(pDlgMiniMap->m_pA2DMapMark[i + CDlgMiniMap::MAPMARK_TAG0]->GetHeight() * fSize);
			if( pDlgMiniMap->m_vecMark[i].nNPC >= 0 )
				Mark.strHint = _AL("^FFFF00") + pDlgMiniMap->m_vecMark[i].strName;
			else
				Mark.strHint = pDlgMiniMap->m_vecMark[i].strName;
			Mark.vecPos = pDlgMiniMap->m_vecMark[i].vecPos;
			Mark.rect.SetRect(POINTTOSCREEN_X(ptPos.x) - W, POINTTOSCREEN_Y(ptPos.y) - H,
					POINTTOSCREEN_X(ptPos.x) + W, POINTTOSCREEN_Y(ptPos.y) + H);
			m_vecMidMapMark.push_back(Mark);
			n++;
		}

    
	// mouse click target
	DWORD dwTimePassed = GetTickCount() - pDlgMiniMap->m_dwTempTargetTime;
	if( pDlgMiniMap->m_idTempTargetInstance == g_pGame->GetGameRun()->GetWorld()->GetInstanceID() &&
		pDlgMiniMap->m_dwTempTargetTime > 0 && dwTimePassed <= 3000 && (dwTimePassed / 500) % 2 == 0 )
	{
		ptPos.x = (int)pDlgMiniMap->m_vecTempTarget.x;
		ptPos.y = (int)pDlgMiniMap->m_vecTempTarget.z;
		if( rcMidMapReal.PtInRect((int)ptPos.x, -(int)ptPos.y))
		{
		    rp[n].pos = ptPos;
		    rp[n].type = CDlgMiniMap::MAPMARK_TAGCLICK;
		    n++;
		}
	}
	
	 
	if( rcMidMapReal.PtInRect((int)vecPosHost.x, -(int)vecPosHost.z) )
	{
		rp[n].pos.x = (int)vecPosHost.x;
		rp[n].pos.y = (int)vecPosHost.z;
		rp[n].type = CDlgMiniMap::MAPMARK_HOST;
		n++;
	}

	// 地图号  、坐标数量和最标值写死了，  
	//策划以后要频繁增加、改变坐标和地图好，最好写个配置
	
	int coord[][2] =
	{
		-64,   -3,
		148,   -210,
		385,   137,
	};

	if(idInstance == 1)
	{
		for(i = 0; i < 3; i++)
			if(m_bEvent[i])
			{
				if( rcMidMapReal.PtInRect(coord[i][0], -coord[i][1]))
				{
					rp[n].pos.x = coord[i][0];
					rp[n].pos.y = coord[i][1];
					rp[n].type = CDlgMiniMap::MAPMARK_EVENT;
					n++;
				}				
			}
	}
	
	qsort((void*)rp, n, sizeof(CDlgMiniMap::RadarPoint), compare);

	int  iMouseClick = -1;
	int  iPlayerHost = -1;
	for(i = 0; i < n; i++)
	{
		if(rp[i].type == CDlgMiniMap::MAPMARK_TAGCLICK)
		{
		  iMouseClick = i;
		  continue;
		}
		else if(rp[i].type == CDlgMiniMap::MAPMARK_HOST)
		{
		  iPlayerHost = i;
		  continue;
		}
		int posX = POINTTOSCREEN_X(rp[i].pos.x);
		int posY = POINTTOSCREEN_Y(rp[i].pos.y);
		pDlgMiniMap->m_pA2DMapMark[rp[i].type]->DrawToInternalBuffer(posX, posY);
	}

	//delete []rp;

	// 传送阵图标
	A2DSprite* pActiveMark = pDlgMap->GetGameUIMan()->m_pDlgMapTransfer->GetA2DMapActiveMark();
	A2DSprite* pInactiveMark = pDlgMap->GetGameUIMan()->m_pDlgMapTransfer->GetA2DMapInactiveMark();
	for (i = 0; i < (int)pDlgMap->m_vecTransferMapMarks.size(); ++ i)
	{
		CDlgMapTransfer::MAPMARK& point = pDlgMap->m_vecTransferMapMarks[i];
		A2DSprite* mark = point.active ? pActiveMark : pInactiveMark;
		int nMarkW = pDlgMap->GetGameUIMan()->m_pDlgMapTransfer->GetA2DMapActiveMark()->GetWidth();
		int nMarkH = pDlgMap->GetGameUIMan()->m_pDlgMapTransfer->GetA2DMapActiveMark()->GetHeight();
		int nMarkCX = nMarkW / 2;
		int nMarkCY = nMarkH / 2;

		point.transRect.SetRect(POINTTOSCREEN_X(point.rect.left)-nMarkCX, POINTTOSCREEN_Y(point.rect.top)-nMarkCY,
				                POINTTOSCREEN_X(point.rect.left)+nMarkW,  POINTTOSCREEN_Y(point.rect.top)+nMarkH);
		if( point.transRect.left   <  rcMap.left  ||
		    point.transRect.right  >  rcMap.right ||
			point.transRect.top    <  rcMap.top   ||
			point.transRect.bottom >  rcMap.bottom)
			continue;
		mark->DrawToInternalBuffer(POINTTOSCREEN_X(point.rect.left)-nMarkCX, POINTTOSCREEN_Y(point.rect.top)-nMarkCY);
	}

	//if(pDlgMap->m_iCurMapType == CDlgMidMap::MAP_TYPE_TOWN)
	//	pDlgMap->RenderTownPrecinct();

	{
	    if(iMouseClick != -1)
		{
			int posX = POINTTOSCREEN_X(rp[iMouseClick].pos.x);
			int posY = POINTTOSCREEN_Y(rp[iMouseClick].pos.y);
			pDlgMiniMap->m_pA2DMapMark[rp[iMouseClick].type]->DrawToInternalBuffer(posX, posY);
		}
		if(iPlayerHost != -1)
		{
			int posX = POINTTOSCREEN_X(rp[iPlayerHost].pos.x);
			int posY = POINTTOSCREEN_Y(rp[iPlayerHost].pos.y);
			pDlgMiniMap->m_pA2DMapMark[rp[iPlayerHost].type]->DrawToInternalBuffer(posX, posY);
		}
		delete []rp;
	}

	
	if( g_ECAutoMove.GetState() == CECAutoMove::enumStateSuccess )
	{
		for(int i = g_ECAutoMove.GetWayPointCount() - 1; i >= 0; i -= AUTOMOVE_POINTDEC)
		{
			ptPos.x = POINTTOSCREEN_X(g_ECAutoMove.GetPosByIndex(i).x);
			ptPos.y = POINTTOSCREEN_Y(g_ECAutoMove.GetPosByIndex(i).z);
			if(!rcMap.PtInRect(ptPos.x,ptPos.y))
				continue;
			pDlgMiniMap->m_pA2DMapMark[CDlgMiniMap::MAPMARK_AUTOMOVE]->DrawToInternalBuffer(ptPos.x, ptPos.y);
		}
	}

	
}

// 取得指定地图的传送阵图标
void CDlgMidMap::GetMapTransferMark(unsigned int instanceId)
{
	m_vecTransferMapMarks.clear();

	elementdataman* pDataMan = g_pGame->GetElementDataMan();

	const int TRANS_POINT_COUNT = 96;

	DATA_TYPE DataType;
	const void* pData = pDataMan->get_data_ptr(CDlgMapTransfer::ms_uitransferConfigId, ID_SPACE_CONFIG, DataType);
	if (!pData || DataType != DT_UI_TRANSFER_CONFIG)
		return;
	
	const UI_TRANSFER_CONFIG* pTransConfig = (const UI_TRANSFER_CONFIG*)pData;

	for (int i = 0; i < TRANS_POINT_COUNT; ++ i)
	{
		
		const UI_TRANSFER_CONFIG::TRANS_POINT& transPt = pTransConfig->trans_point_list[i];
		if (transPt.map_id != instanceId)
			continue;

		CDlgMapTransfer::MAPMARK markPt;
		markPt.index = (int)i;
		markPt.active = GetHostPlayer()->IsUiTransIdActive(transPt.trans_id);
		markPt.strHint = transPt.name;
		markPt.rect.left = transPt.pos[0];
		markPt.rect.top = transPt.pos[2];
		markPt.rect.right = 0;
		markPt.rect.bottom = 0;
		m_vecTransferMapMarks.push_back(markPt);
	}	
}

void CDlgMidMap::OnCommand_CityMap(const char * szCommand)
{
    m_iCurMapType = MAP_TYPE_CITY;
}
void CDlgMidMap::OnCommand_TownMap(const char * szCommand)
{
    m_iCurMapType = MAP_TYPE_TOWN;
}
void CDlgMidMap::RenderTownPrecinct()
{
	int idInstance = GetWorld()->GetInstanceID();
	if(m_mapTownNpcMonsterPos.find(idInstance) == m_mapTownNpcMonsterPos.end())
		return;
	CDlgMidMap *pDlgMap = (CDlgMidMap *)(this);
	const vecTownNpcMonsterPos& vecTownPos = m_mapTownNpcMonsterPos[idInstance];
	A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
	CECInstance *pInstance = GetGameRun()->GetInstance(idInstance);
	CECHostPlayer *pHost = GetHostPlayer();
	A3DVECTOR3 vecPosHost = pHost->GetPos();
	float fScale = 1.0;
	float fOffsetX = 0.0f;
	float fOffsetZ = 0.0f;
	GetMidMapOffsetParam(fScale,fOffsetX,fOffsetZ);
	float fWindowScale = GetGameUIMan()->GetWindowScale();
	float fSize = fWindowScale / 2;
	A3DRECT rcMidMap, rcMap, rcMidMapReal;
	int nRow = pInstance->GetRowNum();
	int nCol = pInstance->GetColNum();
	rcMap = m_pImg_MidMap->GetRect();
	rcMidMap.left = int((vecPosHost.x/nRow - 512) / 1024) * 1024 - 512;
	rcMidMap.top = int((vecPosHost.z/nRow - 512) / 1024) * 1024 - 512;
	rcMidMap.right = rcMidMap.left + 1024;
	rcMidMap.bottom = rcMidMap.top + 1024;
	rcMidMap.left = m_rcRenderArea.left*fScale - 512 + fOffsetX;
	rcMidMap.top = m_rcRenderArea.top*fScale - 512 + fOffsetZ;
	rcMidMap.right = m_rcRenderArea.right*fScale - 512 + fOffsetX;
	rcMidMap.bottom = m_rcRenderArea.bottom*fScale - 512 + fOffsetZ;
	rcMidMapReal.left = rcMidMap.left * nCol;
	rcMidMapReal.right = rcMidMap.right * nCol;
	rcMidMapReal.top = rcMidMap.top * nRow;
	rcMidMapReal.bottom = rcMidMap.bottom * nRow;
	A2DSprite* pIcon = GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_TOWNNPCMONSTER];
	for (int i=0;i<(int)vecTownPos.size();++i)
	{
		const TownNpcMonsterPos&   townPos = vecTownPos[i];
		if(!rcMidMapReal.PtInRect(townPos.x,-townPos.z))
			continue;
		if(NULL != pIcon)
		{
            int posX = POINTTOSCREEN_X(townPos.x);
			int posY = POINTTOSCREEN_Y(townPos.z);
			pIcon->SetCurrentItem(GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_TOWNNPCMONSTER][townPos.strIconPath]);
			A2DSpriteItem* pIconItem = pIcon->GetItem(GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_TOWNNPCMONSTER][townPos.strIconPath]);
			const A3DRECT& rectIcon = pIconItem->GetRect();
			int width1 = rectIcon.Width();
			int height1 = rectIcon.Height();
			int width2 =0;
			int height2 =0;
			//A3DFTFont* pFont = GetGameUIMan()->GetDefaultFont();
			A3DPOINT2 sz = m_pFont->GetTextExtent(townPos.strName);
			width2 = sz.x;
			height2 = sz.y;
			int iconStartX = (int)(posX-(width1+width2)/2.0f);
			int iconStartY = (int)(posY-(height1)/2.0f);
			int fontStartX = (int)(iconStartX + width1);
			int fontStartY = (int)(posY-(height2)/2.0f);
			pIcon->DrawToInternalBuffer(iconStartX,iconStartY);
			m_pFont->TextOut(fontStartX,fontStartY,townPos.strName,A3DCOLORRGBA(0, 0, 0, 255));
			
		}
	}
    

}
