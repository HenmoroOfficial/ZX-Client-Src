/*
 * FILE: DlgAutoChooseSky.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Liyi, 2005/12/19
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "DlgHomeBase.h"

#ifndef _DLG_AUTO_CHOOSE_SKY_H_
#define _DLG_AUTO_CHOOSE_SKY_H_

class AUIListBox;

class CDlgAutoChooseSky : public CDlgHomeBase
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
public:
	CDlgAutoChooseSky();
	virtual ~CDlgAutoChooseSky();

	enum SkyType_e
	{
		ST_NULL,
		ST_DAY,
		ST_NIGHT,
	};

	void InitChooseSky(SkyType_e st);
protected:
	AUIListBox* m_pListBoxSkys;
	int m_nIdxSky;

	CAutoSky::SKYINFO* m_pSkyInfos;
	int m_nNumSkyInfos;
	
	SkyType_e m_SkyType;

protected:
	virtual bool OnInitContext();
	virtual void OnShowDialog();
	virtual void DoDataExchange(bool bSave);
	
	bool InitResource();
	void ClearListBox();
	void FillListBox(SkyType_e st);

	void OnCommandOK(const char* szCommand);
	void OnCommandCancel(const char* szCommand);
};

#endif

