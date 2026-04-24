/*
 * FILE: DlgHomeBase.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Liyi, 2005/12/2
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "DlgHomeBase.h"
#include "EC_HomeDlgsMgr.h"

#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_World.h"


AUI_BEGIN_COMMAND_MAP(CDlgHomeBase, AUIDialog)

AUI_ON_COMMAND("ChooseTerrain", OnCommandSwitchDialog)
AUI_ON_COMMAND("ChooseMountain", OnCommandSwitchDialog)
AUI_ON_COMMAND("ChooseWater", OnCommandSwitchDialog)
AUI_ON_COMMAND("ChooseRoad", OnCommandSwitchDialog)
AUI_ON_COMMAND("ChooseLocalTexture", OnCommandSwitchDialog)
AUI_ON_COMMAND("ChoosePlant", OnCommandSwitchDialog)
AUI_ON_COMMAND("ChooseBuilding", OnCommandSwitchDialog)
AUI_ON_COMMAND("ChooseLight", OnCommandSwitchDialog)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgHomeBase, AUIDialog)

AUI_END_EVENT_MAP()

CDlgHomeBase::CDlgHomeBase() : m_pECHomeDlgsMgr(0)
{
}

CDlgHomeBase::~CDlgHomeBase()
{
}

bool CDlgHomeBase::InitContext(CECHomeDlgsMgr *pECHomeDlgsMgr)
{
	m_pECHomeDlgsMgr = pECHomeDlgsMgr;

	if( !m_pECHomeDlgsMgr)
		return false;

	if (!OnInitContext())
		return false;

	return true;
}

void CDlgHomeBase::OnShowDialog()
{
	AUIDialog::OnShowDialog();
}

void CDlgHomeBase::OnCommandSwitchDialog(const char *szCommand)
{
	GetHomeDlgsMgr()->SwitchDialog(szCommand);
}

int CDlgHomeBase::GetOperationType() const
{
	CAutoScene* pAutoScene = GetAutoScene();

	return pAutoScene->GetOperationType();
}

CAutoScene* CDlgHomeBase::GetAutoScene() const
{
	return m_pECHomeDlgsMgr->GetAutoScene();
}

void CDlgHomeBase::SetOperationType(int nType)
{
	CAutoScene* pAutoScene = GetAutoScene();

	pAutoScene->SetOperationType(nType);
	return;
}

