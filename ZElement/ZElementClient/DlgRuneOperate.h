// Filename	: DlgRuneOperate.h
// Creator	: Fu Chonggang
// Date		: 2012.4.21
// Desc		: 元魂相关操作类

#pragma once

#include "DlgItemSvrBase.h"

class AUIStillImageButton;

//////////////////////////////////////////////////////////////////////////
//  元魂洗练
//////////////////////////////////////////////////////////////////////////
class CDlgRuneRefine : public CDlgItemSvrBase  
{
	AUI_DECLARE_COMMAND_MAP();

public:
	CDlgRuneRefine();
	virtual ~CDlgRuneRefine();
	
	void OnCommandKeep(const char* szCommand);
	void OnCommandChange(const char* szCommand);
	
	virtual void SetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg=NULL);
	virtual void ServerStart();
	virtual void Reset();

	void SetRune();
protected:
	virtual bool OnInitDialog();
	virtual void OnTick();
	int m_iIndexAssist;

	int	m_iAssistId[5];
	int	m_iId[8];
	int m_iValue[8];
	int m_iChangeDelta;
		
	int m_iLastTimeServe;
	static const int ms_InternalTime;
};

//////////////////////////////////////////////////////////////////////////
//  元魂血炼
//////////////////////////////////////////////////////////////////////////
class CDlgRuneLevelUp : public CDlgItemSvrBase  
{
public:
	CDlgRuneLevelUp();
	virtual ~CDlgRuneLevelUp();
	
	virtual void SetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg=NULL);
	virtual void ServerStart();
	virtual void Reset();
	
	void SetRune();
	void ServerStartWithoutCheck();
protected:
	virtual void OnTick();
	int m_iIndexAssist;
};


//////////////////////////////////////////////////////////////////////////
//  元魂归元
//////////////////////////////////////////////////////////////////////////
class CDlgRuneReset : public CDlgItemSvrBase  
{
public:
	CDlgRuneReset();
	virtual ~CDlgRuneReset();
	
	virtual void SetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg=NULL);
	virtual void ServerStart();
	virtual void Reset();
	
	void SetRune();
	void DoServerStart();
	
protected:
	virtual bool OnInitDialog();
	virtual void OnTick();
	int m_iIndexAssist;

	int m_iAssistId[5];
	
	int m_iLastTimeServe;
	static const int ms_InternalTime;
};


//////////////////////////////////////////////////////////////////////////
//  孔位擦写
//////////////////////////////////////////////////////////////////////////
class CDlgSlotErase : public CDlgItemSvrBase  
{
public:
	CDlgSlotErase();
	virtual ~CDlgSlotErase();
	
	virtual void SetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg=NULL);
	virtual void ServerStart();
	virtual void Reset();
	
	void SetRune();
protected:
	virtual bool OnInitDialog();
	virtual void OnTick();
	int m_iIndexAssist;

	int m_iAssistId[5];
	AUIImagePicture *m_pImgShape[5];
};


//////////////////////////////////////////////////////////////////////////
//  元魂鉴定
//////////////////////////////////////////////////////////////////////////
class CDlgRuneIdentify : public CDlgItemSvrBase  
{
public:
	CDlgRuneIdentify();
	virtual ~CDlgRuneIdentify();
	
	virtual void SetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg=NULL);
	virtual void ServerStart();
	virtual void Reset();

	void SetRune(int index);
	
protected:
	virtual bool OnInitDialog();
	virtual void OnTick();
	int m_iIndexAssist;

	int m_iAssistId[5];
	int m_iIndexRune;
};



//////////////////////////////////////////////////////////////////////////
//  符语镶嵌
//////////////////////////////////////////////////////////////////////////
class CDlgRuneFillSlot : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();
	
public:
	CDlgRuneFillSlot();
	virtual ~CDlgRuneFillSlot();
	
	void OnCommandStart(const char* szCommand);
	void OnCommandCancel(const char* szCommand);

	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	void SetRune();
	void SetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg=NULL);
protected:
	virtual bool OnInitDialog();
	virtual void OnTick();
	
	AUIImagePicture *m_pImgStone[5];
	AUIImagePicture *m_pImgShape[5];
	int				m_iIndex[5];		// 符文在包裹中index
};


//////////////////////////////////////////////////////////////////////////
//  符语擦写
//////////////////////////////////////////////////////////////////////////
class CDlgRuneErase : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();
	
public:
	CDlgRuneErase();
	virtual ~CDlgRuneErase();
	
	void OnCommandStart(const char* szCommand);
	void OnCommandCancel(const char* szCommand);

	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	void SetRune();
protected:
	virtual bool OnInitDialog();
	virtual void OnTick();

	int m_iStoneIndex;
	
	AUIImagePicture *m_pImgStone[5];
	AUIImagePicture *m_pImgShape[5];
	AUIImagePicture *m_pImgGfx[5];
};


