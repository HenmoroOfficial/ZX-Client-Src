// Filename	: DlgPetFlyAttrUp.h
// Creator	: Fu Chonggang
// Date		: 2013.8.16
// 提升宠物飞升品质Pet_Detail_LvupAttri.xml
// 关闭后返回Pet_Detail.xml

#pragma once

#include "DlgBase.h"

class AUIProgress;

class CDlgPetFlyAttrUp : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();

	enum
	{
		PET_FLY_ATTR_NUM = 14,	// 宠物飞升品质提升数量
	};

public:
	CDlgPetFlyAttrUp();
	virtual ~CDlgPetFlyAttrUp();

	void OnCommandMainAttr(const char * szCommand);
	void OnCommandFlyAttrUp(const char * szCommand);
	
protected:
	virtual bool OnInitDialog();
	virtual void OnTick();

	AUILabel	*m_pLabAttr[PET_FLY_ATTR_NUM];
	AUILabel	*m_pLabAttrPrg[PET_FLY_ATTR_NUM];
	AUIProgress *m_pPrgAttr[PET_FLY_ATTR_NUM];
	static int ms_iLvUpNum[14][11];	// 每个属性有11档，提升一档有一定好处
	static DWORD ms_dwColor[10];	// 每一等级进度条颜色
};
