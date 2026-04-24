// Filename	: DlgTreasureRadar.cpp
// Creator	: Wang Dongliang
// Date		: 2012/08/16
// Desc		: 寻宝雷达界面
#include "A3DFuncs.h"
#include "AUI\\AUIImagePicture.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_HostPlayer.h"
#include "EC_World.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrTaskItem.h"
#include "DlgTreasureRadar.h"
#include "EC_GameUIMan.h"

AUI_BEGIN_COMMAND_MAP(CDlgTreasureRadar, CDlgBase)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(CDlgTreasureRadar,CDlgBase)
AUI_ON_EVENT("*",		WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_END_EVENT_MAP()
//根据当前距离，得到应当显示的图片索引,min当前策划填的值：10
static int GetStateByDistance_1(float d,float min)
{
    if(d >= 850)
	{
	   return 0;
	}
	else if(d >= 700)
	{
	   return 1;
	}
	else if(d >= 600)
	{
	   return 2;
	}
	else if(d >= 500)
	{
	  return 3;
	}
	else if(d >= 400)
	{
	  return 4;
	}
	else if(d >= 300)
	{
	  return 5;
	}
	else if(d >= 200)
	{
	  return 6;
	}
	else if(d >= 100)
	{
	  return 7;
	}
	else if(d >= (100+min)/2)
	{
	  return 8;
	}
	else if(d >= min)
	{
	  return 9;
	}
	else
	{
	  return 10;
	}

}
static int GetStateByDistance_2(float d,float min)
{
	if(d >= 520)
	{
		return 0;
	}
	else if(d >= 460)
	{
		return 1;
	}
	else if(d >= 400)
	{
		return 2;
	}
	else if(d >= 340)
	{
		return 3;
	}
	else if(d >= 280)
	{
		return 4;
	}
	else if(d >= 220)
	{
		return 5;
	}
	else if(d >= 160)
	{
		return 6;
	}
	else if(d >= 100)
	{
		return 7;
	}
	else if(d >= 50)
	{
		return 8;
	}
	else if(d >= 10)
	{
		return 9;
	}
	else
	{
		return 10;
	}
}
static int GetStateByDistance(float d,float min,const char* strName)
{
	 if(0 == stricmp(strName,"Win_DragonCampass"))
	 {
	     return GetStateByDistance_1(d,min);
	 }
	 else if(0 == stricmp(strName,"Win_ZongziCampass"))
	 {
	     return GetStateByDistance_2(d,min);  
	 }
	 return 0;
}

static int GetMaxVisibleDis(const char* strName)
{
    if(0 == stricmp(strName,"Win_DragonCampass"))
	{
		return 1000;
	}
	else if(0 == stricmp(strName,"Win_ZongziCampass"))
	{
		return 580;
	}
	return 0;
}
CDlgTreasureRadar::CDlgTreasureRadar()
:m_pImgPoint(NULL)
,m_inventorypos(-1)
,m_mapid(-1)
,m_lastTick(0)
,m_isShow(true)
{
}
CDlgTreasureRadar::~CDlgTreasureRadar()
{
 
}
bool CDlgTreasureRadar::OnInitDialog()
{
   m_pImgPoint = (PAUIIMAGEPICTURE)GetDlgItem("Img_Symbol");
   return true;
}
void CDlgTreasureRadar::CheckRadarState(CECIvtrItem* pIvtrSrc,int pos)
{
	if (this->IsShow() && this->GetIvtPos() == pos)
	{
		 CECIvtrTaskDice*  pTaskDice = (CECIvtrTaskDice*)pIvtrSrc;
		 if(pTaskDice->IsUsing())  
		 {
			 pTaskDice->Use(false);
			 pTaskDice->Freeze(false);
			 this->SetIvtPos(-1);
			 this->Show(false);
		 }
	}
}
void CDlgTreasureRadar::UseTaskRectItem(CECIvtrItem* pIvtrSrc,int pos)
{
    CECIvtrTaskDice*  pTaskDice = (CECIvtrTaskDice*)pIvtrSrc;
	bool bCanUse = false;
	A3DVECTOR3 vecPosHost = GetHostPlayer()->GetPos();
	for (int i=0;i<8;++i)
	{
		if(pTaskDice->GetDBEssence()->trigger_point[i].map_id == (unsigned int)(g_pGame->GetGameRun()->GetWorld()->GetInstanceID()))
		{

		      if(vecPosHost.x > pTaskDice->GetDBEssence()->trigger_point[i].pos_min[0] && vecPosHost.x < pTaskDice->GetDBEssence()->trigger_point[i].pos_max[0])
			  {
			      if(vecPosHost.y > pTaskDice->GetDBEssence()->trigger_point[i].pos_min[1] && vecPosHost.y < pTaskDice->GetDBEssence()->trigger_point[i].pos_max[1])
				  {
					  if(vecPosHost.z > pTaskDice->GetDBEssence()->trigger_point[i].pos_min[2] && vecPosHost.z < pTaskDice->GetDBEssence()->trigger_point[i].pos_max[2])
					  {
						   bCanUse = true;
						   break;
					  }
				  }
			  }
		}
	}
	if(!bCanUse)
	{
		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(15403));
	}
	else
	{
	    GetHostPlayer()->UseItemInPack(IVTRTYPE_PACK, pos);
	}

}
void CDlgTreasureRadar::UpdateRadarState(CECIvtrItem* pIvtrSrc,int pos)
{
	    CECIvtrTaskDice*  pTaskDice = (CECIvtrTaskDice*)pIvtrSrc;
		if(this->IsShow())
		{
			if(0 == stricmp(GetName(),"Win_DragonCampass"))
			{
			  GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(15400));
			}
			else if(0 == stricmp(GetName(),"Win_ZongziCampass"))
			{
			   GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(15404));
			}
		  
		    return;
		}
		if(pTaskDice->GetDBEssence()->map_trigger != g_pGame->GetGameRun()->GetWorld()->GetInstanceID()/*-1 == index*/)
		{
			GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(15401));
		}
		else
		{
			m_trigger.x = pTaskDice->GetDBEssence()->pos_trigger[0];
			m_trigger.z = pTaskDice->GetDBEssence()->pos_trigger[2];
			m_trigger.radius = pTaskDice->GetDBEssence()->radius_trigger;
			this->SetIvtPos(pos);
			this->SetMapId(pTaskDice->GetDBEssence()->map_trigger);
			pTaskDice->Freeze(true);
			pTaskDice->Use(true);
			this->Show(true);	
		}	
}
void CDlgTreasureRadar::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if(10 == m_pImgPoint->GetFixFrame())
	{
	   CECIvtrItem* pItem = GetHostPlayer()->GetPack()->GetItem(m_inventorypos);
	   CECIvtrTaskDice*  pTaskDice = (CECIvtrTaskDice*)pItem;
	   pTaskDice->Use(false);
	   pTaskDice->Freeze(false);
	   this->Show(false);
	   GetHostPlayer()->UseItemInPack(IVTRTYPE_PACK, m_inventorypos);
	   this->SetIvtPos(-1);
	   
	}
	else
	{
	   GetGameUIMan()->MessageBox("",GetGameUIMan()->GetStringFromTable(15402),MB_OK,A3DCOLORRGBA(255, 255, 255, 160));
	}

}
void CDlgTreasureRadar::OnTick()
{

   //切换地图时关掉界面
   if(m_mapid != (unsigned int)(g_pGame->GetGameRun()->GetWorld()->GetInstanceID()))
   {
	   CECIvtrItem* pItem = GetHostPlayer()->GetPack()->GetItem(m_inventorypos);
	   CECIvtrTaskDice*  pTaskDice = (CECIvtrTaskDice*)pItem;
	   pTaskDice->Use(false);
	   pTaskDice->Freeze(false);
	   this->SetIvtPos(-1);
	   this->Show(false);
	   return;
   }

    A3DVECTOR3 vecDst = A3DVECTOR3(m_trigger.x,0,m_trigger.z);
    A3DVECTOR3 vecPosHost = GetHostPlayer()->GetPos();
	A3DVECTOR3 vecDir = vecDst-vecPosHost;
	float distance = vecDir.MagnitudeH();

    DWORD interval = 0;
    if(distance >= 100)
	{
        interval = (DWORD)(distance*ESHOWINTERVAL_SPEED);
	}
	else if(distance >= 50)
	{
	    interval = (DWORD)(ESHOWINTERVAL_1);
	}
	else if(distance >= m_trigger.radius)
	{
	    interval = (DWORD)(ESHOWINTERVAL_2);
	}
	else
	{
	    interval = (DWORD)(ESHOWINTERVAL_3);
	}

	m_pImgPoint->FixFrame(GetStateByDistance(distance,m_trigger.radius,GetName()));

	DWORD tick = GetTickCount();
	if(tick - m_lastTick > interval)
	{
		m_isShow = !m_isShow;
		m_lastTick = tick;
	}


	if(distance >= GetMaxVisibleDis(GetName()))
	{
	   m_pImgPoint->Show(false);
	}
	else
	{
	   m_pImgPoint->Show(m_isShow);
	}


}
void CDlgTreasureRadar::OnShowDialog()
{
  m_lastTick = GetTickCount();
}



