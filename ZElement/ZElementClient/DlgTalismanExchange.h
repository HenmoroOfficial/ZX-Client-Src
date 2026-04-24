// Filename	: DlgTalismanExchange.h
// Creator	: Fu Chonggang
// Date		: 2012.8.18
// Desc		: 采用物物交易功能模拟兑换单个灵媒

#pragma once

#include "DlgBase.h"
#include "hashmap.h"


class CDlgTalismanExchange : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();

	struct MediumInfo
	{
		MediumInfo(int m=0, int pi=0, int pn=0) { iMediumId=m; iPropId=pi; iPropNum=pn; }
		int iMediumId;
		int iPropId;
		int iPropNum;
	};

public:
	CDlgTalismanExchange();
	virtual ~CDlgTalismanExchange();

	void OnCommandMax(const char * szCommand);
	void OnCommandExchange(const char * szCommand);
	void OnCommandConfirm(const char * szCommand);
	
protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	
	void UpdateDisplay();

	int		m_iPropId;				// 道具id
	int		m_iItemRequire;
	int		m_iItemExchangeNum;

	int		m_iShopSet;
	int		m_iSetIndex;

	abase::hash_map<int, MediumInfo> m_mapMediumInfo;
};


class CDlgTalismanCombineSkillSelect : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
	
public:
	CDlgTalismanCombineSkillSelect() : m_iRefineFinishId(0) {}
	virtual ~CDlgTalismanCombineSkillSelect() {}
	
	void OnCommandAccept(const char * szCommand);
	void OnCommandRefuse(const char * szCommand);
	void OnCommandCancel(const char * szCommand);
	
	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
protected:
	virtual void OnShowDialog();
	virtual void OnTick();
	void UpdateDisplay();

	// 技能融合完毕服务器会通知客户端TALISMAN_REFINESKILL
	// 此时有可能物品信息还不全，因此在tick中处理
	int		m_iRefineFinishId;	
};