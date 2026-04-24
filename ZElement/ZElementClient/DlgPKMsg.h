// Filename	: DlgPKSetting.h
// Creator	: Fu Chonggang
// Date		: 2009/10/20

#pragma once

#include "DlgBase.h"

#define CDLGPKMSG_NUM	4
class AUILabel;
class CDlgPKMsg : public CDlgBase  
{
public:
	CDlgPKMsg();
	virtual ~CDlgPKMsg();

	void SetInfo(int killer_id, int deader_id, ACHAR* killer_name = NULL, ACHAR* deader_name = NULL);
	void AddMessage(ACString strText);

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();

	AUILabel*	m_pLabMsg[CDLGPKMSG_NUM];
	int			m_iOrigX[CDLGPKMSG_NUM];
	int			m_iOrigY[CDLGPKMSG_NUM];
	DWORD		m_dwShowTime;
};
