// Filename	: DlgInventoryRide.h
// Creator	: Fu Chonggang
// Date		: 2012.7.31

#pragma once

#include "DlgBase.h"
#include "DlgUpgrade.h"

class CECLoginPlayer;
class AUIScroll;
class AUIImagePicture;
class AUIStillImageButton;

class CDlgInventoryRide : public CDlgFormatText  
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();

public:
	CDlgInventoryRide();
	virtual ~CDlgInventoryRide();

	void OnCommand_Resume(const char * szCommand);
	
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventRButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	
	void UpdateInventory();
	void ReLoadModel();
	
	void OnFlyEventLButtonDownItem(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnFlyEventRButtonDownItem(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnMountEventLButtonDownItem(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnMountEventRButtonDownItem(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void UpdataMountsOrFly();
	void GetGroupItem(int nID,int nType);
	void SendMsgUseItem(int nType);
	void SendMsgMoveItem(int nType);
protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();

	int						m_nAngle;
	CECLoginPlayer			*m_pPlayer;
	AUIScroll				*m_pScl_Item;
	AUIImagePicture			*m_pImg_Char;
	AUIStillImageButton		*m_pBtn_LeftTurn;
	AUIStillImageButton		*m_pBtn_RightTurn;
	
	AUIImagePicture			*m_pImg_Mounts;
	AUIImagePicture			*m_pImg_Fly;

	int				m_nFirstLine;

	static const int	ms_PackLineMax;
	int m_nOldFlyPackageType;	// ¾É·É½£±³°üÀàÐÍ
	int m_nOldFlyPackageID;		// ¾É·É½£Î»ÖÃ
	int m_OldFlyItemID;			// ¾É·É½£ITEM ID
	int m_nOldMountType;		// ¾É×øÆï±³°üÀàÐÍ
	int m_nOldMountID;			// ¾É×øÆïÎ»ÖÃ
	int m_nOldMountItemID;		// ¾É×øÆïitem iD
};
