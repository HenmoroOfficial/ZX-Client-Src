// Filename	:	DlgEquipLvup.h
// Creator	:	Fu Chonggang
// Date		:	Jul 5, 2010

#pragma once


#include "DlgBase.h"
#include "vector.h"
class AUILabel;
class AUIStillImageButton;
class AUIImagePicture;

class CDlgEquipLvup : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	struct  EquipLvUpConfig
	{
		int		equip_id;						// 装备id
		int		stone_id;						// 模具id
		int		num;							// 所需模具数量
		int		out_equip_id;					// 产出的武器id
	};
	CDlgEquipLvup();
	virtual ~CDlgEquipLvup();

	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	virtual bool Release();

    void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	void OnCommand_CANCEL(const char * szCommand);
	void OnCommand_depart(const char * szCommand);
	void OnCommand_JewelryStart(const char * szCommand);

	// 确定升级, 发送升级协议
	void SendLvupCmd();
protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	virtual bool Render();
	void LoadConfigData();

	AUILabel *m_pTitle;
	AUIImagePicture *m_pItem_1;
	AUIStillImageButton *m_pBtn_Confirm;
	AUILabel *m_pLab_1;
	AUILabel *m_pLab_2;
	AUIImagePicture *m_pItem_2;
	AUILabel *m_pLab_3;
	AUIImagePicture *m_pItem_3;

	abase::vector<EquipLvUpConfig> m_vecInfo;
	int m_nIdProps;
	int m_nIdHelper;

};
