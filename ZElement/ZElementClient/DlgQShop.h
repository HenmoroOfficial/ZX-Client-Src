// Filename	: DlgQShop.h
// Creator	: Xiao Zhou
// Date		: 2006/5/22

#pragma once

#include "DlgBase.h"
#include "AUI\\AUIScroll.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIListBox.h"
#include "EC_IvtrTypes.h"
#include "hashmap.h"

class CECLoginPlayer;
struct EQUIPMENT_ESSENCE;
struct _GSHOP_ITEM;
#define CDLGQSHOP_BUTTONMAX 10


class CDlgQShop : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();

public:
	enum
	{
		SHOP_TYPE_Q,			
		SHOP_TYPE_POP,			//ºèÀû
		SHOP_TYPE_CROSSSERVER,
	};
	CDlgQShop();
	virtual ~CDlgQShop();

	void OnCommand_Top(const char * szCommand);
	void OnCommand_Bottom(const char * szCommand);
	void OnCommand_Next(const char * szCommand);
	void OnCommand_Prec(const char * szCommand);
	void OnCommand_MainType(const char * szCommand);
	void OnCommand_SubType(const char * szCommand);
	void OnCommand_ResumeEquip(const char * szCommand);
	void OnCommand_Payment(const char * szCommand);
	void OnCommand_FastPay(const char * szCommand);
	void OnCommand_ChangeShop(const char * szCommand);
	void OnCommand_Help(const char * szCommand);
	void OnCommand_PresentNext(const char * szCommand);
	void OnCommand_PresentPrev(const char * szCommand);
	void OnCommand_GetGift(const char * szCommand);
	void OnCommand_CANCEL(const char * szCommand);
	void OnCommand_Search(const char * szCommand);
	void OnCommand_Edit(const char * szCommand);
	void OnCommand_WebShop(const char* szCommand);

	void OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	void ChangeEquip(const EQUIPMENT_ESSENCE* pEquip);

	ACString GetCashText(int nCash, bool bFullText = false, bool bPoint = false);
	static void SetShopType(int type);
	static int GetShopType() {return m_iQshopType;}
	static int GetCrossServerItemId() { return ms_iCrossServerItemId; }
	void Refresh();
	void ReLoadModel();
	int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	static int GetRemainTime(int index) { if(m_mapIndexRemainTime.find(index)!=m_mapIndexRemainTime.end()) return m_mapIndexRemainTime[index]; else return 0;}
	static void TestServer() { ms_bTestServer = !ms_bTestServer; }
	
	void OpenQShop(ACString strSearch);
	
	static DWORD WINAPI OpenUrl(void * ptr);
	bool _ChinaPinYin(ACHAR chSearch, ACHAR chName);
	bool IsStringMatch(ACString strName, ACString strSearch);

protected:
	virtual void OnShowDialog();
	virtual bool OnInitDialog();
	virtual bool Release();
	virtual void OnTick();
	void BuildPresentItems();
	void UpdatePresentItemInfo();

	PAUISCROLL				m_pScl_Item;
	PAUILABEL				m_pTxt_Cash;
	PAUILABEL				m_pLab_Title;
	PAUILABEL				m_pLab_ConsumeScore;
	int						m_nStartLine;
	int						m_nAngle;
	int						m_nMainType;
	int						m_nSubType;
	PAUIIMAGEPICTURE		m_pImg_Char;
	PAUISTILLIMAGEBUTTON	m_pBtn_LeftTurn;
	PAUISTILLIMAGEBUTTON	m_pBtn_RightTurn;
	PAUISTILLIMAGEBUTTON	m_pBtn_MainType[CDLGQSHOP_BUTTONMAX];
	PAUISTILLIMAGEBUTTON	m_pBtn_SubType[CDLGQSHOP_BUTTONMAX];
	abase::vector<int>		m_vecIdItem;
	int						m_aEquips[SIZE_EQUIPIVTR];	//	Equipment item ID array
	static int					m_iQshopType;
	const static int			ms_iCrossServerItemId;
	abase::vector<_GSHOP_ITEM*>	m_vecPresentItems;
	UINT						m_nCurPresentIndex;
	DWORD						m_dwPresentTime;
	static abase::hash_map<int, int>	m_mapIndexRemainTime;
	bool					m_bGetSaleTime;
	static				bool ms_bTestServer;
	abase::vector<ACString>	m_vecSearchWord;
	bool					m_bGetName;
	ACString				m_strSearch;
	bool					m_bKeepSearchResultOnce;

	CECLoginPlayer*		m_pPlayer;
public:	
	AString					m_strUrl;

};
