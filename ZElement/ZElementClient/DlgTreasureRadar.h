// Filename	: DlgTreasureRadar.h
// Creator	: Wang Dongliang
// Date		: 2012/08/16
// Desc		: 寻宝雷达界面

#pragma once

#include "DlgBase.h"
class AUIImagePicture;
class CECIvtrItem;
class CDlgTreasureRadar : public CDlgBase  
{
	enum
	{
	   ESHOWINTERVAL_SPEED = 10,
       ESHOWINTERVAL_1 = 500,
	   ESHOWINTERVAL_2 = 200,
	   ESHOWINTERVAL_3 = 100,
	};
	AUI_DECLARE_EVENT_MAP()
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgTreasureRadar();
	virtual ~CDlgTreasureRadar();
	virtual bool OnInitDialog();
	virtual void OnTick() ;
	virtual void OnShowDialog() ;
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
public:
	/*
	  功能：使用“矩形任务发生器物品”，该物品只有在指定地图指定区域内才能使用
	  参数：pIvtrSrc 当前物品 pos 物品在包裹中的位置
	  返回值：无
	*/
	void UseTaskRectItem(CECIvtrItem* pIvtrSrc,int pos);
	/*
	  功能：使用“龙族星盘”，打开寻宝雷达界面
	  参数：pIvtrSrc 当前物品 pos 物品在包裹中的位置
	  返回值：无
	*/
	void UpdateRadarState(CECIvtrItem* pIvtrSrc,int pos);
	/*
      功能：检查“龙族星盘”，关闭寻宝雷达界面
	  参数：pIvtrSrc 当前物品 pos 物品在包裹中的位置
	  返回值：无
	*/
	void CheckRadarState(CECIvtrItem* pIvtrSrc,int pos);



	int GetIvtPos(){return m_inventorypos;}
	void SetIvtPos(int pos){m_inventorypos = pos;}
	unsigned int  GetMapId(){return m_mapid;}
	void SetMapId(unsigned int mapid){m_mapid = mapid;}
private:
private:
	
	AUIImagePicture *		m_pImgPoint;
	//上次刷新时间
	DWORD                   m_lastTick;
	//显示标志
	bool                    m_isShow;
	//雷达所使用的地图ID
	unsigned int	        m_mapid;
	//“龙族星盘”在包裹中的位置
	int                     m_inventorypos;

    //目标点和半径
	struct  STrigger
	{
		float x;
		float z;
		float radius;
		STrigger()
		{
		   memset(this,0,sizeof(STrigger));
		}

	} m_trigger;
};