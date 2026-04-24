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

#ifndef _DLG_HOME_BASE_H_
#define _DLG_HOME_BASE_H_

#include "aui/AUIDialog.h"
#include "AutoScene.h"
#include "EC_HomeDlgsMgr.h"


class CECHomeDlgsMgr;
class AUIStillImageButton;
class AutoScene;

class CDlgHomeBase: public AUIDialog
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
	
public:
	CDlgHomeBase();
	virtual ~CDlgHomeBase();
	
	bool InitContext(CECHomeDlgsMgr *pECHomeDlgsMgr);
	
	virtual AUIStillImageButton * GetSwithButton() { ASSERT(false);  return NULL; }

protected:
	CECHomeDlgsMgr* GetHomeDlgsMgr() { return m_pECHomeDlgsMgr;}

	virtual bool OnInitContext() { return true; }
	virtual void OnShowDialog();

	void OnCommandSwitchDialog(const char * szCommand);

	inline AUIManager* GetAUIManager() const{return m_pECHomeDlgsMgr->GetAUIManager();}

	CAutoScene* GetAutoScene() const;
	int GetOperationType() const;
	void SetOperationType(int nType);
	

protected:
	CECHomeDlgsMgr* m_pECHomeDlgsMgr;
	
};

#endif 