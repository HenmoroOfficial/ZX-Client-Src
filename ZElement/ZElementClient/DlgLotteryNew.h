/*
 Filename	: DlgLottery.h
 Creator	: Xiao Zhou
 Date		: 2006/10/18

  彩票共3种
  CECIvtrLottery 不可堆叠彩票，抽完以后变成“抽完彩票”，点领奖获得物品
  界面表现：1.777则显示777界面 2.转盘则旋转显示物品
  CECIvtrLottery2 可堆叠彩票，抽完直接获得物品，没有“抽完彩票”这种状态
  CECIvtrLottery3 彩票描述采用物品id，而不是写在物品模板里，抽完直接获得物品，没有“抽完彩票”这种状态
  界面表现：1.间隔显示物品 2.葫芦界面

  彩票处理方式3种
  CDlgLottery	777彩票
  CDlgLotteryNew 转盘彩票
  CDlgLottery3  葫芦彩票
  CDlgLotteryTangYuan 2013汤圆彩票

  上面2中分类没关联，互相独立
*/

#pragma once

#include "DlgBase.h"

class AUIStillImageButton;
class AUILabel;
class AUIImagePicture;
class AUICheckBox;
class A3DGFXEx;
class AUIObject;
struct AUITEXTAREA_EDITBOX_ITEM;

#define CDLGLOTTERYITEM_MAX 16

class CLuaState;
class CECIvtrItem;
class CECIvtrLottery2;
class CECIvtrLottery3;

class CDlgLotteryBase : public CDlgBase
{
public:
	CDlgLotteryBase():m_pLottery2(NULL), m_pLottery3(NULL) {}
	virtual ~CDlgLotteryBase() {}

protected:
	void SetItemCoverByIcon2(PAUIIMAGEPICTURE pItemImage, int index);
	void SetItemCoverByIcon3(PAUIIMAGEPICTURE pItemImage, int index);
	void SetItemCoverByIconNew(PAUIIMAGEPICTURE pItemImage, int index);

	virtual	bool Release();

	CECIvtrLottery2*	m_pLottery2;
	CECIvtrLottery3*	m_pLottery3;
	int				m_iItemId;
	int				m_iInterval;

	const static int		m_siInterval;
};

class CDlgLotteryNew : public CDlgLotteryBase  
{
	AUI_DECLARE_COMMAND_MAP();
	
public:
	CDlgLotteryNew();
	virtual ~CDlgLotteryNew();
	
	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	void OnCommandCancel(const char* szCommand);
	void OnCommandStartStop(const char* szCommand);
	void OnCommandGetAward(const char* szCommand);
	
protected:
	virtual void OnTick();
	virtual bool Render();
	virtual	bool OnInitDialog();
	virtual void OnShowDialog();
	virtual	bool Release();
	
	PAUICHECKBOX			m_pBtn_StartStop;
	PAUISTILLIMAGEBUTTON	m_pBtn_GetAward;
	PAUILABEL				m_pTxt_AwardLevel;
	PAUILABEL				m_pTxt_Award;
	PAUILABEL				m_pTxt_AwardMoney;
	PAUILABEL				m_pTxt_Title;
	PAUIIMAGEPICTURE		m_pImg_Congrat;
	PAUIIMAGEPICTURE		m_pImg_Award;
	PAUIIMAGEPICTURE		m_pImg_Word[16];
	
	CLuaState * m_pState;
	static A2DSprite* m_pA2DSpriteLotteryCur;
	
	bool		m_bBeginSlow;
	bool		m_bStart;
	bool		m_bFinish;
	int			m_iCurPos;	// 1 - 16
	int			m_iRange;
	int			m_iResult;
	int			m_iLevel;
	
	bool		m_bNewLottery;
	
	int			m_dwTime;
	A3DGFXEx*	m_pGfx;
	int			m_iGfxIndex;
	
protected:
	void SetItemCoverByIcon(PAUIIMAGEPICTURE pItemImage, int count, unsigned int id, int index);
	
	void InitLottery();
	void InitLotteryNew(bool bFirst = true);
	void TickLottery(CECIvtrItem* pItem);
	void TickLotteryNew();
};

class CDlgLottery3 : public CDlgLotteryBase  
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();
	
public:
	CDlgLottery3();
	virtual ~CDlgLottery3();
	
	void OnCommandCancel(const char* szCommand);
	void OnCommandStart(const char* szCommand);			// 开始客户端动画显示
	void OnCommandGetAward(const char* szCommand);		// 实际抽奖
	void OnCommandStartAuto(const char* szCommand);
	void OnCommandStopAuto(const char* szCommand);
	
	void OnEventLButtonDown_Desc(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventMouseMove_Desc(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	
protected:
	virtual void OnTick();
	virtual	bool OnInitDialog();
	virtual void OnShowDialog();

	void InitLottery();
	bool GetItemLinkItemOn(int x, int y, AUIObject* pObj, AUITEXTAREA_EDITBOX_ITEM * pLink);
	inline int GetItemColor(int id);
	
	PAUILABEL		m_pItemNum;

	const static int	ms_iAward1Color;
	static abase::hash_map<int, int> ms_mapItem2Color;

	// 连续抽奖功能
	bool			m_bContinue;
	int				m_iState;		// 0:空闲	1:开始	2:抽奖
	int				m_iLastStart;
};

// 类似礼包的可以重复开包的彩票赠送物品，2012.12.21，2013.1版本没出
class CDlgLotteryGift : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();
	
public:
	CDlgLotteryGift() {}
	virtual ~CDlgLotteryGift() {}
	
	void OnCommandCancel(const char* szCommand);
	void OnCommandStart(const char* szCommand);			// 祭炼
	void OnCommandGetAward(const char* szCommand);		// 领取
	
	void OnEventLButtonDown_Desc(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventMouseMove_Desc(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	
protected:
	virtual void OnTick();
	virtual	bool OnInitDialog();
	virtual void OnShowDialog();
	
	bool GetItemLinkItemOn(int x, int y, AUIObject* pObj, AUITEXTAREA_EDITBOX_ITEM * pLink);
	
	PAUIIMAGEPICTURE	m_pImgLottery;
	
	const static int	ms_iAward1Color;
	static abase::hash_map<int, int> ms_mapItem2Color;
	
	// 连续抽奖功能
	bool			m_bContinue;
	int				m_iState;		// 0:空闲	1:开始	2:抽奖
	int				m_iLastStart;
};

class CDlgLotteryTangYuan : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
	
public:
	CDlgLotteryTangYuan() {}
	virtual ~CDlgLotteryTangYuan() {}
	
	void OnCommandCancel(const char* szCommand);
	void OnCommandStart(const char* szCommand);			// 祭炼
	void OnCommandGetAward(const char* szCommand);		// 领取
	
	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	
protected:
	virtual void OnTick();
	virtual	bool OnInitDialog();
	virtual void OnShowDialog();
	
	PAUIIMAGEPICTURE	m_pImgLottery;
	PAUIPROGRESS		m_pPrgExp;
	PAUILABEL			m_pLabCash;
	PAUILABEL			m_pLabLevel;
	PAUIOBJECT			m_pBtnStart;
	PAUIOBJECT			m_pBtnGet;
	PAUIIMAGEPICTURE	m_pImgExpShow[4];
	PAUIIMAGEPICTURE	m_pImgAward;
	PAUICHECKBOX		m_pChkGfx;
	
	const static int	ms_iAward1Color;
	static abase::hash_map<int, int> ms_mapItem2Color;
	
	// 连续抽奖功能
	int				m_iLastStart;	// 0-2000 表示一次抽取结束
	int				m_iGetAward;	// 0-2000 表示领奖gfx展示，9999表示其它情况
	int				m_iShowExpTime;	// 上次显示经验暴击时间记录
};