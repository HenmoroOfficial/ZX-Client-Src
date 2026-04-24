/********************************************************************
	created:	2013/08/15
	created:	15:8:2013   21:10
	filename: 	e:\Works\ZElementClient\DlgGuildLandBuild.h
	file path:	e:\Works\ZElementClient
	file base:	DlgGuildLandBuild
	file ext:	h
	author:		zhuyan
	
	purpose:	帮派地图,帮派基地任务
*********************************************************************/

#pragma once

#include "DlgBase.h"
#include "AUI\\AUIScroll.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUIProgress.h"

class DlgGuildCreateLandBuild: public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP()
public:
	DlgGuildCreateLandBuild();
	~DlgGuildCreateLandBuild();
	virtual bool OnInitDialog();
	void CreateBuild(int nTempIndex, int nCreateId,bool isUpDrade = false);			// 根据ID 显示可以建筑的建筑物

	ACString GetBuildHint( BUILDING_ESSENCE* pBuildEssence,int nLevel  );
	void UpDradeBuild(int nTempIndex,int nIndex, int nLevel);

	void ClearBuildTemp();

	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	bool CheckBuild( BUILDING_ESSENCE * m_pEssence, int nLevel ) ;
	
	void SendCreateBuild();
	void SendUpDradeBuild();
private:
	int m_nFId;			// 地块ID
	int m_nTempID;		// 模板ID
	int m_nCurLevel;	// 当前等级
	bool m_bIsCreate;
	AUILabel			*m_pTxt[12];		// 可建筑物名称
	AUIImagePicture		*m_pImg_Icon[12];		// 可建筑物图标
};

class DlgGuildLandBuild: public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP()
public:
	DlgGuildLandBuild();
	~DlgGuildLandBuild();
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	void OnCommand_CreateBuild(const char * szCommand);
	void OnCommand_Build(const char * szCommand);
	void OnCommand_DestroyBuild(const char * szCommand);
	void OnCommand_Quest(const char * szCommand);
	void OnEventButtonDBCLK(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void ResultBuilding(int nTempId, int nIndex);
	void ResultBuildUpgrade(int nTempId, int nIndex , int nLevel);
	void ResultBuildRemove(int nIndex);
	void ResultBuildComplete(int nTempId, int nIndex , int nLevel);
	void ChangeBuildGrid(bool isChange);
	void RemoveBuild();
	void MessageBoxBuild();
	void SendTransferBuild();
private:
	PAUISTILLIMAGEBUTTON	m_pBtn[20];
	PAUIPROGRESS			m_pPrg;			// 建设度进度条
	PAUITREEVIEW				m_pTreeTask;	// 帮派基地任务列表
	int m_nCurSelect;
	int m_nState;
};