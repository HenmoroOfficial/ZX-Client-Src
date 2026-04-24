// Filename	: DlgVipAward.h
// Creator	: Fu Chonggang
// Date		: 2012/02/01
// Desc		: VIP 奖励发放界面，类似商城

#pragma once

#include "DlgBase.h"
#include "EC_IvtrTypes.h"
#include "hashmap.h"

struct _VIP_AWARD_ITEM;
struct EQUIPMENT_ESSENCE;
class AUILabel;
class AUIImagePicture;
class AUIStillImageButton;
class AUIScroll;
class AUISubDialog;
class AUIEditBox;

class CECLoginPlayer;
class A2DSprite;


// 每个奖励显示界面，作为CDlgVipAward的子界面
class CDlgVipAwardItem : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();
	
public:
	CDlgVipAwardItem() {}
	virtual ~CDlgVipAwardItem() {}
	
	virtual bool OnInitDialog();
	virtual bool Release();
	void OnCommand_GetAward(const char * szCommand);
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);

private:
	_VIP_AWARD_ITEM	*m_pItem;

	int				m_iCanGetInfo;
	static A2DSprite	*m_pA2DSpriteDaily;
	static A2DSprite	*m_pA2DSpriteOnce;

private:
	void UpdateDisplay();
};

#define VIP_AWARD_PERPAGE	16
#define VIP_SUBBUTTON_MAX	8

// vip奖励管理界面
class CDlgVipAward : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();

public:
	CDlgVipAward();
	virtual ~CDlgVipAward();

	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	
	void OnCommand_MainType(const char * szCommand);
	void OnCommand_SubType(const char * szCommand);
	void OnCommand_Help(const char * szCommand);
	void OnCommand_HelpLvUp(const char * szCommand);
	void OnCommand_ResumeEquip(const char * szCommand);

	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);

	void ChangeEquip(const EQUIPMENT_ESSENCE* pEquip);

protected:
	virtual void OnTick();

private:
	AUILabel *m_pLv;
	AUILabel *m_pVipLv;
	AUILabel *m_pEndTime;
	AUILabel *m_pLabDateLeave;

	AUIImagePicture		*m_pImg_Char;
	AUIStillImageButton	*m_pBtn_LeftTurn;
	AUIStillImageButton	*m_pBtn_RightTurn;
	int					m_nAngle;
	
	AUIStillImageButton	*m_pBtn_SubType[VIP_SUBBUTTON_MAX];
	AUISubDialog		*m_pSubItems[VIP_AWARD_PERPAGE];
	AUIEditBox			*m_pEdtShape[VIP_AWARD_PERPAGE];
	AUIImagePicture		*m_pImgGfxMain[2];
	AUIImagePicture		*m_pImgGfxSub[VIP_SUBBUTTON_MAX];

	int					m_nAwardType;	// 对应m_nMainType;
	int					m_nAwardLevel;	// 对应m_nSubType;
	
	int					m_aEquips[SIZE_EQUIPIVTR];
	CECLoginPlayer*		m_pPlayer;

	// data	
	abase::vector<int>		m_vecItemIndex;		// 当前奖励类型在globaldata_get_vipaward_items中的index
	int						m_iExpireTime;
	abase::hash_map<int, int> m_mapAwardId;	// 是否有这个id的奖励，0表示未领取，1表示已领取

	int					m_iLastC2STime;
	static const int	ms_C2SCoolTime;

	void UpdateDisplay();
};
