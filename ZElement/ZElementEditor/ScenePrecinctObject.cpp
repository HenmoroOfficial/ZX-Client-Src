// ScenePrecinctObject.cpp: implementation of the CScenePrecinctObject class.
//
//////////////////////////////////////////////////////////////////////

#include "global.h"
#include "elementeditor.h"
#include "ScenePrecinctObject.h"
#include "SceneObjectManager.h"
#include "Box3D.h"
#include "Render.h"
#include "A3D.h"
#include "MainFrm.h"
#include "SceneAIGenerator.h"
#include "BezierSelDlg.h"
#include "PrecinctNpcSelDlg.h"
#include "MusicSelDlg.h"
#include "PolygonFunc.h"
#include "EditerBezier.h"
#include "globaldataman.h"


//#define new A_DEBUG_NEW

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void init_precinct_data( PRECINCT_DATA& dat)
{
	dat.nPath = -1;
	dat.strPathName = "";
	dat.strPrecinct = "";
	dat.nPriority = 0;
	dat.nSelfCity = 1;
	dat.nCity = 1;
	dat.vCityPoint = A3DVECTOR3(0);
	dat.dwPrecinctID = 0;
	dat.bIsPrecinct = true;
	dat.nLoopType = 1;
	dat.nAlternation = 60;
	dat.m_strSound = "";
	dat.m_strSoundNight = "";
	dat.m_strMiniMap= "";
	dat.nPartOfCity = 0;

	dat.nFeishengCnt = 0;
	dat.nAllowLevelMax = 0;
	dat.nAllowLevelMin = 0;
	dat.nKickCity = 1;
	dat.vKickCityPoint = A3DVECTOR3(0);
}

CScenePrecinctObject::CScenePrecinctObject()
{
	init_precinct_data(m_Data);
	m_nObjectType = SO_TYPE_PRECINCT;
	m_pBIDSelFuncs = new BEZIER_ID_FUNCS;
	m_pFIDSelFuncs = new NPC_ID_FUNCS;
	m_pMusicSelFuncs = new MUSIC_SEL_FUNCS;
	strMusicList = "...";
	m_bIsPlaying = false;
	m_bIsPlayingNight = false;
	m_nCopy = 0;
	
	city_type.AddElement("不属于任何城市", 0);
	abase::vector<DOMAIN_INFO> * pDomain = globaldata_getdomaininfos();
	if(pDomain)
	{	
		int n = pDomain->size();
		for( int i = 0; i < n; ++i)
		{
			char szBuffer[128];
			WideCharToMultiByte(CP_ACP,0,(LPCWSTR)(*pDomain)[i].name,16,szBuffer,128,NULL,FALSE);
			city_type.AddElement(szBuffer,(*pDomain)[i].id);
		}
	}

	music_type.AddElement("不循环",0);
	music_type.AddElement("首尾循环",1);
	music_type.AddElement("从第二首开始循环",2);

	feisheng_cnt.AddElement("未飞升",0);
	feisheng_cnt.AddElement("飞升1次",1);

	BuildProperty();
	
}

CScenePrecinctObject::~CScenePrecinctObject()
{
}

void CScenePrecinctObject::Translate(const A3DVECTOR3& vDelta)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap==NULL) return;
	if(!pMap->GetTerrain()) return;

	m_vPos += vDelta;
	m_vPos.y = pMap->GetTerrain()->GetPosHeight(m_vPos) + 0.1f;

	m_matTrans.SetRow(3,m_vPos);
	m_matAbs = m_matScale * m_matRotate * m_matTrans;
}

void CScenePrecinctObject::Render(A3DViewport* pA3DViewport)
{
	if(m_nCopy != g_Configs.nShowCopyNum) return;
	
	CSceneObject::Render(pA3DViewport);
}

void CScenePrecinctObject::PostRender(A3DViewport* pA3DViewport)
{
	if(m_nCopy != g_Configs.nShowCopyNum) return;
	DrawCenter();
	CSceneObject::PostRender(pA3DViewport);
}


void CScenePrecinctObject::Tick(DWORD timeDelta)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CSceneObjectManager *pSManager = pMap->GetSceneObjectMan();
		CEditerBezier *pBezier = pSManager->FindBezier(m_Data.nPath);
		if(pBezier==NULL) return;
		int plNum = pBezier->GetPointCount() + 1;
		if(plNum<3) return;

		A3DVECTOR3 cpos = g_Render.GetA3DEngine()->GetActiveCamera()->GetPos();
		POINT_FLOAT pt;
		pt.x = cpos.x; pt.y = cpos.z;
		POINT_FLOAT* ptList = new POINT_FLOAT[plNum];	
		
		A3DVECTOR3 ptpos;	
		POSITION pos = pBezier->m_list_points.GetHeadPosition();
		int index = 0;
		while(pos)
		{
			CEditerBezierPoint * pBPt = (CEditerBezierPoint *)pBezier->m_list_points.GetNext(pos);
			ptpos = pBPt->GetPos();
			ptList[index].x = ptpos.x;
			ptList[index++].y = ptpos.z;
		}
		CEditerBezierPoint * phead = (CEditerBezierPoint *)pBezier->m_list_points.GetHead();
		ptpos = phead->GetPos();
		ptList[index].x = ptpos.x;
		ptList[index].y = ptpos.z;
		
		//这儿要做一个优先及的判别，以决定哪个音乐该响起
		if(IsInPolyGon(pt,ptList,plNum))
		{
			CScenePrecinctObject *pObj = pSManager->GetCurrentPrecinct();
			if(pObj)
			{
				PRECINCT_DATA dat = pObj->GetProperty();
				if(dat.nPriority > m_Data.nPriority)
				{
					if(pObj!=this) pSManager->SetCurrentPrecinct(this);
				}
			}else
			{
				pSManager->SetCurrentPrecinct(this);
			}
		}else
		{
			CScenePrecinctObject *pObj = pSManager->GetCurrentPrecinct();
			if(pObj==this) pSManager->SetCurrentPrecinct(NULL);
		}
		if(ptList) delete []ptList;
	}
	
}

void CScenePrecinctObject::PlayBackMusic()
{
	if(!m_bIsPlaying && g_fNightFactor < 0.5f)
	{
		//if(!m_Data.m_strSound.IsEmpty())
		{
			g_BackMusic.PlayBackSFX("music\\" + m_Data.m_strSound);
			PlayMusic(true);
			m_bIsPlaying = true;
		}
	}else
	{
		if(g_fNightFactor > 0.5f) m_bIsPlaying = false;
	}
				
	if(!m_bIsPlayingNight && g_fNightFactor > 0.5f)
	{
		//if(m_Data.m_strSoundNight)
		{
			g_BackMusic.PlayBackSFX("music\\" + m_Data.m_strSoundNight);
			PlayMusic(true);
			m_bIsPlayingNight = true;
		}
	}else
	{
		if(g_fNightFactor < 0.5f) m_bIsPlayingNight = false;
	}
}

void CScenePrecinctObject::StopBackMusic()
{
	m_bIsPlaying = false;
	m_bIsPlayingNight = false;
}

void CScenePrecinctObject::UpdateProperty(bool bGet)
{
	if(bGet)
	{
		AString temp = AString(m_pProperty->GetPropVal(0));
		CElementMap *pMap = AUX_GetMainFrame()->GetMap();
		if(pMap)
		{
			CSceneObjectManager *pSManager = pMap->GetSceneObjectMan();
			CSceneObject*pObj = pSManager->FindSceneObject(temp);
			if(pObj != NULL && pObj != this)
			{
				AfxMessageBox("该对象的名字已经存在，编辑器将恢复原对象名字！",NULL,MB_OK);
				m_pProperty->SetPropVal(0,m_strName);
				m_pProperty->SetPropVal(1,m_Data.strPrecinct);
				m_pProperty->SetPropVal(2,m_vPos);
				m_pProperty->SetPropVal(3,m_Data.nPriority);
				m_pProperty->SetPropVal(4,m_Data.nSelfCity);
				m_pProperty->SetPropVal(5,m_Data.nCity);
				m_pProperty->SetPropVal(6,m_Data.m_strSound);
				m_pProperty->SetPropVal(7,m_Data.m_strSoundNight);
				m_pProperty->SetPropVal(8,m_Data.vCityPoint);
				m_pProperty->SetPropVal(9,m_Data.bIsPrecinct);
				m_pProperty->SetPropVal(10,m_Data.dwPrecinctID);
				m_pProperty->SetPropVal(14,m_Data.nLoopType);
				m_pProperty->SetPropVal(15,m_Data.nAlternation);
				m_pProperty->SetPropVal(16,m_Data.nPartOfCity);
				m_pProperty->SetPropVal(17,m_nCopy);
				m_pBIDSelFuncs->OnSetValue(m_Data.strPathName);
				m_pMusicSelFuncs->OnSetValue("...");
				m_pProperty->SetPropVal(18,m_nObjectID);
				m_pProperty->SetPropVal(19,m_Data.nFeishengCnt);
				m_pProperty->SetPropVal(20,m_Data.nAllowLevelMax);
				m_pProperty->SetPropVal(21,m_Data.nAllowLevelMin);
				m_pProperty->SetPropVal(22,m_Data.nKickCity);
				m_pProperty->SetPropVal(23,m_Data.vKickCityPoint);
				m_pProperty->SetPropVal(24,m_Data.m_strMiniMap);
				AUX_GetMainFrame()->GetToolTabWnd()->UpdatePropertyData(false);
				return;
			}
		}
		m_strName	        = temp;
		m_Data.strPrecinct  = AString(m_pProperty->GetPropVal(1));
		m_vPos	            = m_pProperty->GetPropVal(2);
		m_Data.nPriority    = m_pProperty->GetPropVal(3);
		m_Data.nSelfCity    = m_pProperty->GetPropVal(4);
		m_Data.nCity        = m_pProperty->GetPropVal(5);
		
		AString sound       	= AString(m_pProperty->GetPropVal(6));
		AString soundNight      = AString(m_pProperty->GetPropVal(7));
		sound.MakeLower(); soundNight.MakeLower();
		if(sound.Find(".wav")!=-1) m_Data.m_strSound = sound;
		else m_Data.m_strSound = "";
		if(soundNight.Find(".wav")!=-1) m_Data.m_strSoundNight  = soundNight;
		else m_Data.m_strSoundNight = "";
		m_Data.vCityPoint   = m_pProperty->GetPropVal(8);
		m_Data.bIsPrecinct  = m_pProperty->GetPropVal(9);
		m_Data.dwPrecinctID = m_pProperty->GetPropVal(10);
		m_Data.nLoopType  = m_pProperty->GetPropVal(14);
		m_Data.nAlternation  = m_pProperty->GetPropVal(15);
		m_Data.nPartOfCity = m_pProperty->GetPropVal(16);
		m_nCopy = m_pProperty->GetPropVal(17);
		m_matTrans.Translate(m_vPos.x,m_vPos.y,m_vPos.z);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
		m_pMusicSelFuncs->OnSetValue("...");

		m_Data.nFeishengCnt = m_pProperty->GetPropVal(19);
		m_Data.nAllowLevelMax = m_pProperty->GetPropVal(20);
		m_Data.nAllowLevelMin = m_pProperty->GetPropVal(21);
		m_Data.nKickCity = m_pProperty->GetPropVal(22);
		m_Data.vKickCityPoint = m_pProperty->GetPropVal(23);
		m_Data.m_strMiniMap = AString(m_pProperty->GetPropVal(24));


		ReBuildSound();
	}else
	{
		m_pProperty->SetPropVal(0,m_strName);
		m_pProperty->SetPropVal(1,m_Data.strPrecinct);
		m_pProperty->SetPropVal(2,m_vPos);
		m_pProperty->SetPropVal(3,m_Data.nPriority);
		m_pProperty->SetPropVal(4,m_Data.nSelfCity);
		m_pProperty->SetPropVal(5,m_Data.nCity);
		m_pProperty->SetPropVal(6,m_Data.m_strSound);
		m_pProperty->SetPropVal(7,m_Data.m_strSoundNight);
		m_pProperty->SetPropVal(8,m_Data.vCityPoint);
		m_pProperty->SetPropVal(9,m_Data.bIsPrecinct);
		m_pProperty->SetPropVal(10,m_Data.dwPrecinctID);
		m_pProperty->SetPropVal(14,m_Data.nLoopType);
		m_pProperty->SetPropVal(15,m_Data.nAlternation);
		m_pProperty->SetPropVal(16,m_Data.nPartOfCity);
		m_pProperty->SetPropVal(17,m_nCopy);
		m_pBIDSelFuncs->OnSetValue(m_Data.strPathName);
		m_pMusicSelFuncs->OnSetValue("...");
		m_pProperty->SetPropVal(18,m_nObjectID);

		m_pProperty->SetPropVal(19,m_Data.nFeishengCnt);
		m_pProperty->SetPropVal(20,m_Data.nAllowLevelMax);
		m_pProperty->SetPropVal(21,m_Data.nAllowLevelMin);
		m_pProperty->SetPropVal(22,m_Data.nKickCity);
		m_pProperty->SetPropVal(23,m_Data.vKickCityPoint);
		m_pProperty->SetPropVal(24,m_Data.m_strMiniMap);
	}
}

void CScenePrecinctObject::ReBuildSound()
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CSceneObjectManager *pSManager = pMap->GetSceneObjectMan();
		if(pSManager->GetCurrentPrecinct()==this)
		{
			StopBackMusic();
			PlayBackMusic();
		}
	}
}

void CScenePrecinctObject::PlayMusic(bool bPlay)
{
 	if(bPlay)
	{
		int size = m_listMusic.GetCount();
		for( int i = 0; i < size; i++)
		{	
			g_BackMusic.PlayMusic(*m_listMusic.GetByIndex(i),true, i == 0 ? true : false);
		}
	}else g_BackMusic.StopMusic(true,true);
}

void CScenePrecinctObject::Release()
{
	if(m_pBIDSelFuncs)
	{
		delete m_pBIDSelFuncs;
		m_pBIDSelFuncs = NULL;
	}

	if(m_pFIDSelFuncs)
	{
		delete m_pFIDSelFuncs;
		m_pFIDSelFuncs = NULL;
	}

	if(m_pMusicSelFuncs)
	{
		delete m_pMusicSelFuncs;
		m_pMusicSelFuncs = NULL;
	}

	int n = m_listNpc.GetCount();
	int i;
	for( i = 0; i < n; i++)
	{
		PRECINCT_NPC *pNpc = m_listNpc.GetByIndex(i);
		if(pNpc) delete pNpc;
	}
	m_listNpc.RemoveAll();

	n = m_listMusic.GetCount();
	for( i = 0; i < n; i++)
	{
		delete m_listMusic.GetByIndex(i);	
	}
	m_listMusic.RemoveAll();
}

void CScenePrecinctObject::EntryScene()
{

}

void CScenePrecinctObject::LeaveScene()
{
}

void  CScenePrecinctObject::AddNpc( PRECINCT_NPC *pNewNpc)
{
	m_listNpc.AddTail(pNewNpc);
}

void CScenePrecinctObject::DeleteNpc( int idx )
{
	PRECINCT_NPC *pNpc = m_listNpc.GetByIndex(idx);
	if(pNpc) delete pNpc;
	m_listNpc.RemoveAt(m_listNpc.FindByIndex(idx));
}

bool  CScenePrecinctObject::MoveUp( int pos )
{
	if(pos <0 || pos > m_listNpc.GetCount() - 1) return false;
	
	PRECINCT_NPC *pCur = m_listNpc.GetByIndex(pos);
	if(pos > 0)
	{
		PRECINCT_NPC *pUp = m_listNpc.GetByIndex(pos-1);
		m_listNpc.SetByIndex(pos,pUp);
		m_listNpc.SetByIndex(pos-1,pCur);
		return true;
	}
	return false;
}

bool  CScenePrecinctObject::MoveDown( int pos )
{
	if(pos <0 || pos > m_listNpc.GetCount() - 1) return false;
	
	PRECINCT_NPC *pCur = m_listNpc.GetByIndex(pos);
	if(pos + 1 < m_listNpc.GetCount())
	{
		PRECINCT_NPC *pUp = m_listNpc.GetByIndex(pos+1);
		m_listNpc.SetByIndex(pos,pUp);
		m_listNpc.SetByIndex(pos+1,pCur);
		return true;
	}
	return false;
}

//Load data
bool CScenePrecinctObject::Load(CELArchive& ar,DWORD dwVersion,int iLoadFlags)
{
	init_precinct_data(m_Data);
	if(dwVersion>0xe && dwVersion<=0xf)
	{
	
		m_strName = ar.ReadString();
		CString strDummyName = ar.ReadString();
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		DWORD r,n;
		if(!ar.Read(&r,sizeof(DWORD)))
			return false;
		if(!ar.Read(&r,sizeof(DWORD)))
			return false;
		if(!ar.Read(&n,sizeof(int)))
			return false;
		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
	}else if( dwVersion > 0xf && dwVersion<=0x10)
	{
		m_strName = ar.ReadString();
		m_Data.strPathName = ar.ReadString();
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.nPath,sizeof(int)))
			return false;
		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
	}else if( dwVersion>0x10 && dwVersion<=0x15)
	{
		m_strName = ar.ReadString();
		m_Data.strPathName = ar.ReadString();
		m_Data.strPrecinct = ar.ReadString();
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.nPath,sizeof(int)))
			return false;
		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
	}else if( dwVersion>0x15 && dwVersion <= 0x16)
	{
		m_strName = ar.ReadString();
		m_Data.strPathName = ar.ReadString();
		m_Data.strPrecinct = ar.ReadString();
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.nPath,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nPriority,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nCity,sizeof(int)))
			return false;

		int size;
		if(!ar.Read(&size,sizeof(int)))
			return false;
		for( int i= 0; i < size; i++)
		{
			PRECINCT_NPC *pNpc = new PRECINCT_NPC;
			ASSERT(pNpc);
			if(!ar.Read(&pNpc->vPos,sizeof(A3DVECTOR3)))
				return false;
			pNpc->strName = ar.ReadString();
			m_listNpc.AddTail(pNpc);
		}
		
		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
	}else if(dwVersion > 0x16 && dwVersion <= 0x17)
	{
		m_strName = ar.ReadString();
		m_Data.strPathName = ar.ReadString();
		m_Data.strPrecinct = ar.ReadString();
		m_Data.m_strSound  = ar.ReadString();
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.nPath,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nPriority,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nCity,sizeof(int)))
			return false;

		int size;
		if(!ar.Read(&size,sizeof(int)))
			return false;
		int i;
		for( i= 0; i < size; i++)
		{
			PRECINCT_NPC *pNpc = new PRECINCT_NPC;
			ASSERT(pNpc);
			if(!ar.Read(&pNpc->vPos,sizeof(A3DVECTOR3)))
				return false;
			pNpc->strName = ar.ReadString();
			m_listNpc.AddTail(pNpc);
		}
		if(!ar.Read(&size,sizeof(int)))
			return false;
		for( i= 0; i < size; i++)
		{
			AString* pNew = new AString;
			*pNew = ar.ReadString();
			m_listMusic.AddTail(pNew);
		}
		
		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
	}else if(dwVersion > 0x17 && dwVersion <= 0x1b)
	{
		m_strName = ar.ReadString();
		m_Data.strPathName = ar.ReadString();
		m_Data.strPrecinct = ar.ReadString();
		m_Data.m_strSound  = ar.ReadString();
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.nPath,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nPriority,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nCity,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.vCityPoint,sizeof(A3DVECTOR3)))
			return false;

		int size;
		if(!ar.Read(&size,sizeof(int)))
			return false;
		int i;
		for( i= 0; i < size; i++)
		{
			PRECINCT_NPC *pNpc = new PRECINCT_NPC;
			ASSERT(pNpc);
			if(!ar.Read(&pNpc->vPos,sizeof(A3DVECTOR3)))
				return false;
			pNpc->strName = ar.ReadString();
			m_listNpc.AddTail(pNpc);
		}
		if(!ar.Read(&size,sizeof(int)))
			return false;
		for( i= 0; i < size; i++)
		{
			AString* pNew = new AString;
			*pNew = ar.ReadString();
			m_listMusic.AddTail(pNew);
		}
		
		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
	}else if(dwVersion > 0x1b && dwVersion <= 0x24)
	{
		m_strName = ar.ReadString();
		m_Data.strPathName = ar.ReadString();
		m_Data.strPrecinct = ar.ReadString();
		m_Data.m_strSound  = ar.ReadString();
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.nPath,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nPriority,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nCity,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.vCityPoint,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.bIsPrecinct,sizeof(bool)))
		return false;
	    if(!ar.Read(&m_Data.dwPrecinctID,sizeof(DWORD)))

		return false;	
		int size;
		if(!ar.Read(&size,sizeof(int)))
			return false;
		int i;
		for( i= 0; i < size; i++)
		{
			PRECINCT_NPC *pNpc = new PRECINCT_NPC;
			ASSERT(pNpc);
			if(!ar.Read(&pNpc->vPos,sizeof(A3DVECTOR3)))
				return false;
			pNpc->strName = ar.ReadString();
			m_listNpc.AddTail(pNpc);
		}
		if(!ar.Read(&size,sizeof(int)))
			return false;
		for( i= 0; i < size; i++)
		{
			AString* pNew = new AString;
			*pNew = ar.ReadString();
			m_listMusic.AddTail(pNew);
		}
		
		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
	}
	else if( dwVersion > 0x24 && dwVersion <=0x2a)
	{
		m_strName = ar.ReadString();
		m_Data.strPathName = ar.ReadString();
		m_Data.strPrecinct = ar.ReadString();
		m_Data.m_strSound  = ar.ReadString();
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.nPath,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nPriority,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nCity,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.vCityPoint,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.bIsPrecinct,sizeof(bool)))
			return false;
	    if(!ar.Read(&m_Data.dwPrecinctID,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.nLoopType,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nAlternation,sizeof(int)))
			return false;	
		int size;
		if(!ar.Read(&size,sizeof(int)))
			return false;
		int i;
		for( i= 0; i < size; i++)
		{
			PRECINCT_NPC *pNpc = new PRECINCT_NPC;
			ASSERT(pNpc);
			if(!ar.Read(&pNpc->vPos,sizeof(A3DVECTOR3)))
				return false;
			pNpc->strName = ar.ReadString();
			m_listNpc.AddTail(pNpc);
		}
		if(!ar.Read(&size,sizeof(int)))
			return false;
		for( i= 0; i < size; i++)
		{
			AString* pNew = new AString;
			*pNew = ar.ReadString();
			m_listMusic.AddTail(pNew);
		}
		
		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
	}
	else if( dwVersion > 0x2a && dwVersion<=0x2c)
	{
		m_strName = ar.ReadString();
		m_Data.strPathName = ar.ReadString();
		m_Data.strPrecinct = ar.ReadString();
		m_Data.m_strSound  = ar.ReadString();
		m_Data.m_strSoundNight  = ar.ReadString();
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.nPath,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nPriority,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nCity,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.vCityPoint,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.bIsPrecinct,sizeof(bool)))
			return false;
	    if(!ar.Read(&m_Data.dwPrecinctID,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.nLoopType,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nAlternation,sizeof(int)))
			return false;	
		int size;
		if(!ar.Read(&size,sizeof(int)))
			return false;
		int i;
		for( i= 0; i < size; i++)
		{
			PRECINCT_NPC *pNpc = new PRECINCT_NPC;
			ASSERT(pNpc);
			if(!ar.Read(&pNpc->vPos,sizeof(A3DVECTOR3)))
				return false;
			pNpc->strName = ar.ReadString();
			m_listNpc.AddTail(pNpc);
		}
		if(!ar.Read(&size,sizeof(int)))
			return false;
		for( i= 0; i < size; i++)
		{
			AString* pNew = new AString;
			*pNew = ar.ReadString();
			m_listMusic.AddTail(pNew);
		}
		
		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
	}
	else if( dwVersion > 0x2c && dwVersion <=0x35)
	{
		m_strName = ar.ReadString();
		m_Data.strPathName = ar.ReadString();
		m_Data.strPrecinct = ar.ReadString();
		m_Data.m_strSound  = ar.ReadString();
		m_Data.m_strSoundNight  = ar.ReadString();
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.nPath,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nPriority,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nCity,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.vCityPoint,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.bIsPrecinct,sizeof(bool)))
			return false;
	    if(!ar.Read(&m_Data.dwPrecinctID,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.nLoopType,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nAlternation,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nSelfCity,sizeof(int)))
			return false;	
		int size;
		if(!ar.Read(&size,sizeof(int)))
			return false;
		int i;
		for( i= 0; i < size; i++)
		{
			PRECINCT_NPC *pNpc = new PRECINCT_NPC;
			ASSERT(pNpc);
			if(!ar.Read(&pNpc->vPos,sizeof(A3DVECTOR3)))
				return false;
			pNpc->strName = ar.ReadString();
			m_listNpc.AddTail(pNpc);
		}
		if(!ar.Read(&size,sizeof(int)))
			return false;
		for( i= 0; i < size; i++)
		{
			AString* pNew = new AString;
			*pNew = ar.ReadString();
			m_listMusic.AddTail(pNew);
		}
		
		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
	}
	else if( dwVersion > 0x35 && dwVersion <= 0x3e)
	{
		m_strName = ar.ReadString();
		m_Data.strPathName = ar.ReadString();
		m_Data.strPrecinct = ar.ReadString();
		m_Data.m_strSound  = ar.ReadString();
		m_Data.m_strSoundNight  = ar.ReadString();
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.nPath,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nPriority,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nCity,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.vCityPoint,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.bIsPrecinct,sizeof(bool)))
			return false;
	    if(!ar.Read(&m_Data.dwPrecinctID,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.nLoopType,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nAlternation,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nSelfCity,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nPartOfCity,sizeof(int)))
			return false;	
		int size;
		if(!ar.Read(&size,sizeof(int)))
			return false;
		int i;
		for( i= 0; i < size; i++)
		{
			PRECINCT_NPC *pNpc = new PRECINCT_NPC;
			ASSERT(pNpc);
			if(!ar.Read(&pNpc->vPos,sizeof(A3DVECTOR3)))
				return false;
			pNpc->strName = ar.ReadString();
			m_listNpc.AddTail(pNpc);
		}
		if(!ar.Read(&size,sizeof(int)))
			return false;
		for( i= 0; i < size; i++)
		{
			AString* pNew = new AString;
			*pNew = ar.ReadString();
			m_listMusic.AddTail(pNew);
		}
		
		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
	}
	else if( dwVersion > 0x3e && dwVersion <= 0x4b)
	{
		m_strName = ar.ReadString();
		m_Data.strPathName = ar.ReadString();
		m_Data.strPrecinct = ar.ReadString();
		m_Data.m_strSound  = ar.ReadString();
		m_Data.m_strSoundNight  = ar.ReadString();
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.nPath,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nPriority,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nCity,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.vCityPoint,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.bIsPrecinct,sizeof(bool)))
			return false;
	    if(!ar.Read(&m_Data.dwPrecinctID,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.nLoopType,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nAlternation,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nSelfCity,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nPartOfCity,sizeof(int)))
			return false;
		if(!ar.Read(&m_nCopy,sizeof(int)))
			return false;

		int size;
		if(!ar.Read(&size,sizeof(int)))
			return false;
		int i;
		for( i= 0; i < size; i++)
		{
			PRECINCT_NPC *pNpc = new PRECINCT_NPC;
			ASSERT(pNpc);
			if(!ar.Read(&pNpc->vPos,sizeof(A3DVECTOR3)))
				return false;
			pNpc->strName = ar.ReadString();
			m_listNpc.AddTail(pNpc);
		}
		if(!ar.Read(&size,sizeof(int)))
			return false;
		for( i= 0; i < size; i++)
		{
			AString* pNew = new AString;
			*pNew = ar.ReadString();
			m_listMusic.AddTail(pNew);
		}
		
		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
	}
	else if( dwVersion > 0x4b && dwVersion < 0x52 )
	{
		m_strName = ar.ReadString();
		m_Data.strPathName = ar.ReadString();
		m_Data.strPrecinct = ar.ReadString();
		m_Data.m_strSound  = ar.ReadString();
		m_Data.m_strSoundNight  = ar.ReadString();
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.nPath,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nPriority,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nCity,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.vCityPoint,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.bIsPrecinct,sizeof(bool)))
			return false;
	    if(!ar.Read(&m_Data.dwPrecinctID,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.nLoopType,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nAlternation,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nSelfCity,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nPartOfCity,sizeof(int)))
			return false;
		if(!ar.Read(&m_nCopy,sizeof(int)))
			return false;

		// new added
		if(!ar.Read(&m_Data.nFeishengCnt,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nAllowLevelMax,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nAllowLevelMin,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nKickCity,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.vKickCityPoint,sizeof(A3DVECTOR3)))
			return false;

		int size;
		if(!ar.Read(&size,sizeof(int)))
			return false;
		int i;
		for( i= 0; i < size; i++)
		{
			PRECINCT_NPC *pNpc = new PRECINCT_NPC;
			ASSERT(pNpc);
			if(!ar.Read(&pNpc->vPos,sizeof(A3DVECTOR3)))
				return false;
			pNpc->strName = ar.ReadString();
			m_listNpc.AddTail(pNpc);
		}
		if(!ar.Read(&size,sizeof(int)))
			return false;
		for( i= 0; i < size; i++)
		{
			AString* pNew = new AString;
			*pNew = ar.ReadString();
			m_listMusic.AddTail(pNew);
		}
		
		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
	}
	else
	{
		m_strName = ar.ReadString();
		m_Data.strPathName = ar.ReadString();
		m_Data.strPrecinct = ar.ReadString();
		m_Data.m_strSound  = ar.ReadString();
		m_Data.m_strSoundNight  = ar.ReadString();
		m_Data.m_strMiniMap = ar.ReadString();
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.nPath,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nPriority,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nCity,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.vCityPoint,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_Data.bIsPrecinct,sizeof(bool)))
			return false;
	    if(!ar.Read(&m_Data.dwPrecinctID,sizeof(DWORD)))
			return false;
		if(!ar.Read(&m_Data.nLoopType,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nAlternation,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nSelfCity,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nPartOfCity,sizeof(int)))
			return false;
		if(!ar.Read(&m_nCopy,sizeof(int)))
			return false;

		// new added
		if(!ar.Read(&m_Data.nFeishengCnt,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nAllowLevelMax,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nAllowLevelMin,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.nKickCity,sizeof(int)))
			return false;
		if(!ar.Read(&m_Data.vKickCityPoint,sizeof(A3DVECTOR3)))
			return false;

		int size;
		if(!ar.Read(&size,sizeof(int)))
			return false;
		int i;
		for( i= 0; i < size; i++)
		{
			PRECINCT_NPC *pNpc = new PRECINCT_NPC;
			ASSERT(pNpc);
			if(!ar.Read(&pNpc->vPos,sizeof(A3DVECTOR3)))
				return false;
			pNpc->strName = ar.ReadString();
			m_listNpc.AddTail(pNpc);
		}
		if(!ar.Read(&size,sizeof(int)))
			return false;
		for( i= 0; i < size; i++)
		{
			AString* pNew = new AString;
			*pNew = ar.ReadString();
			m_listMusic.AddTail(pNew);
		}
		
		m_matTrans.SetRow(3,m_vPos);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
	}
	if(m_Data.nSelfCity < 1 || m_Data.nSelfCity > 10000)  m_Data.nSelfCity = 1;
	if(m_Data.nCity < 1 || m_Data.nCity > 10000 ) m_Data.nCity = 1;
	if(iLoadFlags!= LOAD_EXPLIGHTMAP) ReBuildSound();
	return true;
}

//Save data
bool CScenePrecinctObject::Save(CELArchive& ar,DWORD dwVersion)
{
	ar.WriteString(m_strName);	
	ar.WriteString(m_Data.strPathName);
	ar.WriteString(m_Data.strPrecinct);
	ar.WriteString(m_Data.m_strSound);
	ar.WriteString(m_Data.m_strSoundNight);
	ar.WriteString(m_Data.m_strMiniMap);
	if(!ar.Write(&m_nObjectID,sizeof(int)))
		return false;
	if(!ar.Write(&m_vPos,sizeof(A3DVECTOR3)))
		return false;
	if(!ar.Write(&m_Data.nPath,sizeof(int)))
		return false;
	if(!ar.Write(&m_Data.nPriority,sizeof(int)))
		return false;
	if(!ar.Write(&m_Data.nCity,sizeof(int)))
		return false;
	if(!ar.Write(&m_Data.vCityPoint,sizeof(A3DVECTOR3)))
		return false;
	if(!ar.Write(&m_Data.bIsPrecinct,sizeof(bool)))
		return false;
	if(!ar.Write(&m_Data.dwPrecinctID,sizeof(DWORD)))
		return false;
	if(!ar.Write(&m_Data.nLoopType,sizeof(int)))
		return false;
	if(!ar.Write(&m_Data.nAlternation,sizeof(int)))
		return false;
	if(!ar.Write(&m_Data.nSelfCity,sizeof(int)))
		return false;
	if(!ar.Write(&m_Data.nPartOfCity,sizeof(int)))
		return false;	
	if(!ar.Write(&m_nCopy,sizeof(int)))
		return false;	

	if(!ar.Write(&m_Data.nFeishengCnt,sizeof(int)))
		return false;
	if(!ar.Write(&m_Data.nAllowLevelMax,sizeof(int)))
		return false;
	if(!ar.Write(&m_Data.nAllowLevelMin,sizeof(int)))
		return false;
	if(!ar.Write(&m_Data.nKickCity,sizeof(int)))
		return false;
	if(!ar.Write(&m_Data.vKickCityPoint,sizeof(A3DVECTOR3)))
		return false;
	
	int size = m_listNpc.GetCount();
	if(!ar.Write(&size,sizeof(int)))
		return false;
	int i;
	for( i = 0; i < size; i++)
	{	
		PRECINCT_NPC *pNpc = GetNpc(i);
		if(!ar.Write(&pNpc->vPos,sizeof(A3DVECTOR3)))
			return false;
		ar.WriteString(pNpc->strName);	
	}

	size = m_listMusic.GetCount();
	if(!ar.Write(&size,sizeof(int)))
		return false;
	for( i = 0; i < size; i++)
	{	
		ar.WriteString(*m_listMusic.GetByIndex(i));	
	}

	return true;
}

void CScenePrecinctObject::BuildProperty()
{
	if(m_pProperty)
	{
		m_pProperty->DynAddProperty(AVariant(m_strName), "名字");
		m_pProperty->DynAddProperty(AVariant(m_Data.strPrecinct), "管辖区域别名");
		m_pProperty->DynAddProperty(AVariant(m_vPos), "位置坐标");
		m_pProperty->DynAddProperty(AVariant(m_Data.nPriority), "区域优先及别");
		m_pProperty->DynAddProperty(AVariant(m_Data.nSelfCity), "自己的副本世界ID");
		m_pProperty->DynAddProperty(AVariant(m_Data.nCity), "回城副本世界ID");
		m_pProperty->DynAddProperty(AVariant(m_Data.m_strSound), "(白天)背景音效",NULL,NULL,WAY_FILENAME,"music");
		m_pProperty->DynAddProperty(AVariant(m_Data.m_strSoundNight), "(晚上)背景音效",NULL,NULL,WAY_FILENAME,"music");
		m_pProperty->DynAddProperty(AVariant(m_Data.vCityPoint), "回城点");
		m_pProperty->DynAddProperty(AVariant(m_Data.bIsPrecinct), "是否是管辖区域");
		m_pProperty->DynAddProperty(AVariant(m_Data.dwPrecinctID), "区域编号");

		m_pProperty->DynAddProperty(AVariant(m_strDummyName), "关联曲线", (ASet*)m_pBIDSelFuncs, NULL, WAY_CUSTOM);
		m_pProperty->DynAddProperty(AVariant(m_strDummyName), "关联位置表", (ASet*)m_pFIDSelFuncs, NULL, WAY_CUSTOM);
		m_pProperty->DynAddProperty(AVariant(strMusicList), "背景音乐列表", (ASet*)m_pMusicSelFuncs, NULL, WAY_CUSTOM);
		m_pProperty->DynAddProperty(AVariant(m_Data.nLoopType), "背景音乐循环方式", &music_type);
		m_pProperty->DynAddProperty(AVariant(m_Data.nAlternation), "背景音乐间隔(s)");
		m_pProperty->DynAddProperty(AVariant(m_Data.nLoopType), "所属城市", &city_type);
		m_pProperty->DynAddProperty(AVariant(m_nCopy), "第几套数据");
		m_pProperty->DynAddProperty(AVariant(m_nObjectID), "场景对象ID（海外版专用）", NULL,NULL,WAY_READONLY);

		m_pProperty->DynAddProperty(AVariant(m_Data.nFeishengCnt), "地图限制人物飞升次数", &feisheng_cnt);
		m_pProperty->DynAddProperty(AVariant(m_Data.nAllowLevelMax), "高于此等级传出地图");
		m_pProperty->DynAddProperty(AVariant(m_Data.nAllowLevelMin), "低于此等级传出地图");
		m_pProperty->DynAddProperty(AVariant(m_Data.nKickCity), "等级不符合时限制回城地图");
		m_pProperty->DynAddProperty(AVariant(m_Data.vKickCityPoint), "等级不符合时限制回城坐标");
		m_pProperty->DynAddProperty(AVariant(m_Data.m_strMiniMap), "单独小地图",NULL,NULL,WAY_FILENAME,"Surfaces");
	}
}

AString CScenePrecinctObject::GetMusic( int index)
{
	return *m_listMusic.GetByIndex(index);
}
void CScenePrecinctObject::AddMusic( AString *pMusic)
{
	m_listMusic.AddTail(pMusic);
}
void CScenePrecinctObject::DelMusic( AString *pMusic)
{
	ALISTPOSITION oldPos;
	ALISTPOSITION pos = m_listMusic.GetHeadPosition();
	while(pos)
	{
		oldPos = pos;
		AString *ptemp = m_listMusic.GetNext(pos);
		if(stricmp(*pMusic,*ptemp)==0)
		{
			delete ptemp;
			m_listMusic.RemoveAt(oldPos);
			return;
		}
	}
}

BOOL CALLBACK BEZIER_ID_FUNCS::OnActivate(void)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
		if(pManager->m_listSelectedObject.GetCount()==1)
		{
			CSceneObject *pObj = (CSceneObject*)pManager->m_listSelectedObject.GetHead();
			if(pObj->GetObjectType()==CSceneObject::SO_TYPE_PRECINCT)
			{
				CBezierSelDlg dlg;
				PRECINCT_DATA data = ((CScenePrecinctObject*)pObj)->GetProperty();
				dlg.m_nBezierID = data.nPath;
				dlg.m_nObjectType = CSceneObject::SO_TYPE_BEZIER;
				ASSERT(pObj);
				if(dlg.DoModal()==IDOK)
				{
					m_strText = dlg.m_strBezierName;
					data.nPath = dlg.m_nBezierID;
					data.strPathName = dlg.m_strBezierName;
					((CScenePrecinctObject*)pObj)->SetProperty(data);
				}
			}
		}
	}
	
	return TRUE;
}

LPCTSTR CALLBACK BEZIER_ID_FUNCS::OnGetShowString(void) const
{
	return m_strText;
}

AVariant CALLBACK BEZIER_ID_FUNCS::OnGetValue(void) const
{
	return m_var;
}

void CALLBACK BEZIER_ID_FUNCS::OnSetValue(const AVariant& var)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
		if(pManager->m_listSelectedObject.GetCount()==1)
		{
			CSceneObject* pObj = (CSceneObject*)pManager->m_listSelectedObject.GetHead();
			if(pObj->GetObjectType()==CSceneObject::SO_TYPE_PRECINCT)
			{
				PRECINCT_DATA data = ((CScenePrecinctObject*)pObj)->GetProperty();
				m_strText = data.strPathName;
			}
		}
	}
}

BOOL CALLBACK BEZIER_ID_FUNCS2::OnActivate(void)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
		if(pManager->m_listSelectedObject.GetCount()==1)
		{
			CSceneObject *pObj = (CSceneObject*)pManager->m_listSelectedObject.GetHead();
			if(pObj->GetObjectType()==CSceneObject::SO_TYPE_RANGE)
			{
				CBezierSelDlg dlg;
			
				dlg.m_nBezierID = ((CSceneRangeObject*)pObj)->GetBezierID();
				dlg.m_nObjectType = CSceneObject::SO_TYPE_BEZIER;
				ASSERT(pObj);
				if(dlg.DoModal()==IDOK)
				{
					((CSceneRangeObject*)pObj)->SetBezierName(AString(dlg.m_strBezierName));
					((CSceneRangeObject*)pObj)->SetBezierID(dlg.m_nBezierID);
					m_strText = dlg.m_strBezierName;
				}
			}
		}
	}
	return TRUE;
}

LPCTSTR CALLBACK BEZIER_ID_FUNCS2::OnGetShowString(void) const
{
	return m_strText;
}

AVariant CALLBACK BEZIER_ID_FUNCS2::OnGetValue(void) const
{
	return m_var;
}

void CALLBACK BEZIER_ID_FUNCS2::OnSetValue(const AVariant& var)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
		if(pManager->m_listSelectedObject.GetCount()==1)
		{
			CSceneObject* pObj = (CSceneObject*)pManager->m_listSelectedObject.GetHead();
			if(pObj->GetObjectType()==CSceneObject::SO_TYPE_RANGE)
			{
				m_strText = ((CSceneRangeObject *)pObj)->GetBezierName();
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////


BOOL CALLBACK NPC_ID_FUNCS::OnActivate(void)
{
	m_strText = "...";
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
		if(pManager->m_listSelectedObject.GetCount()==1)
		{
			CSceneObject *pObj = (CSceneObject*)pManager->m_listSelectedObject.GetHead();
			if(pObj->GetObjectType()==CSceneObject::SO_TYPE_PRECINCT)
			{
				CPrecinctNpcSelDlg dlg;
				PRECINCT_DATA data = ((CScenePrecinctObject*)pObj)->GetProperty();
			    dlg.SetPrecinctObject((CScenePrecinctObject*)pObj);
				ASSERT(pObj);
				if(dlg.DoModal()==IDOK)
				{
					pMap->SetModifiedFlag(true);
				}
			}
		}
	}
	
	return TRUE;
}

LPCTSTR CALLBACK NPC_ID_FUNCS::OnGetShowString(void) const
{
	return m_strText;
}

AVariant CALLBACK NPC_ID_FUNCS::OnGetValue(void) const
{
	return m_var;
}

void CALLBACK NPC_ID_FUNCS::OnSetValue(const AVariant& var)
{
	m_strText = "...";
}

////////////////////////////////////////////

BOOL CALLBACK MUSIC_SEL_FUNCS::OnActivate(void)
{
	m_strText = "...";
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
		if(pManager->m_listSelectedObject.GetCount()==1)
		{
			CSceneObject *pObj = (CSceneObject*)pManager->m_listSelectedObject.GetHead();
			if(pObj->GetObjectType()==CSceneObject::SO_TYPE_PRECINCT)
			{
				CMusicSelDlg dlg;
				dlg.Init((CScenePrecinctObject*)pObj);
				if(dlg.DoModal()==IDOK)
				{
				}
			}
		}
	}
	
	return TRUE;
}

LPCTSTR CALLBACK MUSIC_SEL_FUNCS::OnGetShowString(void) const
{
	return m_strText;
}

AVariant CALLBACK MUSIC_SEL_FUNCS::OnGetValue(void) const
{
	return m_var;
}

void CALLBACK MUSIC_SEL_FUNCS::OnSetValue(const AVariant& var)
{
	m_strText = "...";
}



// class range

CSceneRangeObject::CSceneRangeObject()
{
	m_nBezierID = -1;
	m_nCopy = 0;
	m_nType = RANGE_PK;
	m_nObjectType = SO_TYPE_RANGE;
	m_pBIDSelFuncs = new BEZIER_ID_FUNCS2;
	range_type.AddElement("PK区域",0);
	range_type.AddElement("安全区域",1);
	BuildProperty();
}

CSceneRangeObject::~CSceneRangeObject()
{
	if(m_pBIDSelFuncs) delete m_pBIDSelFuncs;
}

void CSceneRangeObject::Render(A3DViewport* pA3DViewport)
{
	if(m_nCopy != g_Configs.nShowCopyNum) return;
	if(!g_Configs.bShowRange) return;

	CSceneObject::Render(pA3DViewport);
}

void CSceneRangeObject::PostRender(A3DViewport* pA3DViewport)
{
	if(m_nCopy != g_Configs.nShowCopyNum) return;
	if(!g_Configs.bShowRange) return;
	DrawCenter();
	CSceneObject::PostRender(pA3DViewport);
}

void CSceneRangeObject::Tick(DWORD timeDelta)
{

}

void CSceneRangeObject::UpdateProperty(bool bGet)
{
	if(bGet)
	{
		AString temp = AString(m_pProperty->GetPropVal(0));
		CElementMap *pMap = AUX_GetMainFrame()->GetMap();
		if(pMap)
		{
			CSceneObjectManager *pSManager = pMap->GetSceneObjectMan();
			CSceneObject*pObj = pSManager->FindSceneObject(temp);
			if(pObj != NULL && pObj != this)
			{
				AfxMessageBox("该对象的名字已经存在，编辑器将恢复原对象名字！",NULL,MB_OK);
				m_pProperty->SetPropVal(0,m_strName);
				m_pProperty->SetPropVal(1,m_strRange);
				m_pProperty->SetPropVal(2,m_vPos);
				m_pProperty->SetPropVal(3,m_nType);
				m_pProperty->SetPropVal(5,m_nCopy);
				m_pBIDSelFuncs->OnSetValue(m_strBezier);
				m_pProperty->SetPropVal(6,m_nObjectID);
				AUX_GetMainFrame()->GetToolTabWnd()->UpdatePropertyData(false);
				return;
			}
		}
		m_strName	        = temp;
		m_strRange          = AString(m_pProperty->GetPropVal(1));
		m_vPos	            = m_pProperty->GetPropVal(2);
		m_nType	            = m_pProperty->GetPropVal(3);
		m_nCopy             = m_pProperty->GetPropVal(5);
		m_matTrans.Translate(m_vPos.x,m_vPos.y,m_vPos.z);
		m_matAbs = m_matScale * m_matRotate * m_matTrans;
	}else
	{
		m_pProperty->SetPropVal(0,m_strName);
		m_pProperty->SetPropVal(1,m_strRange);
		m_pProperty->SetPropVal(2,m_vPos);
		m_pProperty->SetPropVal(3,m_nType);
		m_pProperty->SetPropVal(5,m_nCopy);
		m_pBIDSelFuncs->OnSetValue(m_strBezier);
		m_pProperty->SetPropVal(6,m_nObjectID);
	}
}

void CSceneRangeObject::Release()
{

}

void CSceneRangeObject::EntryScene()
{

}

void CSceneRangeObject::LeaveScene()
{
}

//Load data
bool CSceneRangeObject::Load(CELArchive& ar,DWORD dwVersion,int iLoadFlags)
{
	if(dwVersion > 0x20 && dwVersion <= 0x21)
	{
		m_strName = ar.ReadString();
		m_strBezier = ar.ReadString();
		m_strRange = ar.ReadString();
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_nBezierID,sizeof(int)))
			return false;
	}if(dwVersion > 0x21 && dwVersion < 0x40)
	{
		m_strName = ar.ReadString();
		m_strBezier = ar.ReadString();
		m_strRange = ar.ReadString();
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_nBezierID,sizeof(int)))
			return false;
		if(!ar.Read(&m_nType,sizeof(int)))
			return false;
	}else if(dwVersion >= 0x40)
	{
		m_strName = ar.ReadString();
		m_strBezier = ar.ReadString();
		m_strRange = ar.ReadString();
		if(!ar.Read(&m_nObjectID,sizeof(int)))
			return false;
		if(!ar.Read(&m_vPos,sizeof(A3DVECTOR3)))
			return false;
		if(!ar.Read(&m_nBezierID,sizeof(int)))
			return false;
		if(!ar.Read(&m_nType,sizeof(int)))
			return false;
		if(!ar.Read(&m_nCopy,sizeof(int)))
			return false;
	}
	return true;
}

//Save data
bool CSceneRangeObject::Save(CELArchive& ar,DWORD dwVersion)
{
	ar.WriteString(m_strName);	
	ar.WriteString(m_strBezier);
	ar.WriteString(m_strRange);
	
	if(!ar.Write(&m_nObjectID,sizeof(int)))
		return false;
	if(!ar.Write(&m_vPos,sizeof(A3DVECTOR3)))
		return false;
	if(!ar.Write(&m_nBezierID,sizeof(int)))
		return false;
	if(!ar.Write(&m_nType,sizeof(int)))
		return false;
	if(!ar.Write(&m_nCopy,sizeof(int)))
		return false;
	return true;
}

void CSceneRangeObject::BuildProperty()
{
	if(m_pProperty)
	{
		m_pProperty->DynAddProperty(AVariant(m_strName), "名字");
		m_pProperty->DynAddProperty(AVariant(m_strRange), "范围名字");
		m_pProperty->DynAddProperty(AVariant(m_vPos), "位置坐标");
		m_pProperty->DynAddProperty(AVariant(m_nType), "区域类型",&range_type);
		m_pProperty->DynAddProperty(AVariant(m_strBezier), "关联曲线", (ASet*)m_pBIDSelFuncs, NULL, WAY_CUSTOM);
		m_pProperty->DynAddProperty(AVariant(m_nCopy), "第几套数据");
		m_pProperty->DynAddProperty(AVariant(m_nObjectID), "场景对象ID（海外版专用）", NULL,NULL,WAY_READONLY);
	}
}

void CSceneRangeObject::Translate(A3DVECTOR3 vDelta)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap==NULL) return;
	if(!pMap->GetTerrain()) return;

	m_vPos += vDelta;
	m_vPos.y = pMap->GetTerrain()->GetPosHeight(m_vPos) + 0.1f;

	m_matTrans.SetRow(3,m_vPos);
	m_matAbs = m_matScale * m_matRotate * m_matTrans;
}

