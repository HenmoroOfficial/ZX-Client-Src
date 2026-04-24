/********************************************************************
	created:	2013/07/16
	created:	16:7:2013   10:05
	filename: 	e:\Works\ZElementClient\DlgIndividualityColor.h
	file path:	e:\Works\ZElementClient
	file base:	DlgIndividualityColor
	file ext:	h
	author:		zhuyan
	
	purpose:	个性颜色
*********************************************************************/
#pragma once

#include "DlgBase.h"
#include "AUI\\AUIScroll.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIListBox.h"

class DlgIndividualityColor  : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();
public:
	DlgIndividualityColor();
	virtual ~DlgIndividualityColor();
	virtual void OnShowDialog();
	virtual void OnHideDialog();
	virtual bool OnInitDialog();

	void OnEventRButtonDblclk(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnCommand_Compose(const char * szCommand);	// 合成
	void OnCommand_Page(const char * szCommand);
	void OnCommand_CANCEL(const char * szCommand);
	void UpdateFreezeItem();

	void SetIndividualityItem(CECIvtrItem* pIvtrSrc,int nPos);
	void SetNameToPosItem(const char* szName,CECIvtrItem* pIvtrSrc,int nPos);

	void UpDateItemImg(PAUIIMAGEPICTURE pDlg, CECIvtrItem* pIvtrSrc ) ;

	void UpDateResult(int nPos);

private:
	PAUIIMAGEPICTURE m_pPicture1;
	PAUIIMAGEPICTURE m_pPicture2;
	PAUIIMAGEPICTURE m_pPicture3;
	PAUIIMAGEPICTURE m_pPicture4;

	int m_nPos1;
	int m_nPos2;
	int m_nPos3;
	int m_nItemID1;
	int m_nItemID2;
	int m_nItemID3;
};


class DlgIndividualityReveal  : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();
public:
	DlgIndividualityReveal();
	virtual ~DlgIndividualityReveal();
	virtual void OnShowDialog();
	virtual bool OnInitDialog();
	virtual void OnHideDialog();
	virtual void OnTick();
	void OnCommand_Page(const char * szCommand);

	void OnCommand_OK(const char * szCommand);
	void OnCommand_Close(const char * szCommand);
	void OnCommand_Min_Ceccamera(const char * szCommand);
	void OnCommand_Max_Ceccamera(const char * szCommand);
	void UpdateFreezeItem();
	void ReLoadModel();
	void SetNameToPosItem(const char* szName,CECIvtrItem* pIvtrSrc,int nPos,int nPackType,int nPackPos);

	void UpDateEquipColor();

	void OnEventRButtonDblclk(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
private:
	PAUIIMAGEPICTURE		m_pImg_Char;
	PAUIIMAGEPICTURE m_pPicture1;
	PAUIIMAGEPICTURE m_pPicture2;
	PAUISTILLIMAGEBUTTON	m_pBtn_LeftTurn;
	PAUISTILLIMAGEBUTTON	m_pBtn_RightTurn;

	CECLoginPlayer*		m_pPlayer;
	int						m_nAngle;
	int						m_aEquips[SIZE_EQUIPIVTR];	//	Equipment item ID array
	int			m_nPackType;
	int			m_nPackFashionPos;
	int			m_nPackPos;
};