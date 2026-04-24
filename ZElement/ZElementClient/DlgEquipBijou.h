// Filename	:	DlgEquipBijou.h
// Creator	:	Fu Chonggang
// Date		:	Mar 15, 2011
// 宝石相关功能

#pragma once

#include "DlgItemSvrBase.h"

class AUIImagePicture;

//////////////////////////////////////////////////////////////////////////
// CDlgBijouUpgrade
//////////////////////////////////////////////////////////////////////////

class CDlgBijouUpgrade : public CDlgItemSvrBase
{
	AUI_DECLARE_EVENT_MAP()
public:
	enum
	{
		PROP_NUM_MAX = 12,
	};
	CDlgBijouUpgrade();
	virtual ~CDlgBijouUpgrade();
	
	virtual void SetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg=NULL);
	virtual void ServerStart();
	virtual void UpdateDisplay();
	virtual void Reset();

	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	void OnEventLButtonDownDust(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	
protected:
	virtual bool OnInitDialog();

	AUIImagePicture*	m_pImgDestNormal;
	AUIImagePicture*	m_pImgDestGood;
	AUIImagePicture*	m_pImgProp[PROP_NUM_MAX];
	AUILabel*			m_pLabProbNormal;
	AUILabel*			m_pLabProbGood;

	int m_iArticleSlots[12];
	int m_iArticleTemplId[12];
	bool m_bPutProp;
};


//////////////////////////////////////////////////////////////////////////
// CDlgBijouRefine
//////////////////////////////////////////////////////////////////////////

class CDlgBijouRefine : public CDlgItemSvrBase
{
public:
	CDlgBijouRefine();
	virtual ~CDlgBijouRefine();
	
	virtual void SetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg=NULL);
	virtual void ServerStart();
	virtual void UpdateDisplay();
	virtual void Reset();
	
protected:
	virtual bool OnInitDialog();

	AUIImagePicture*	m_pImgProp;
	AUIImagePicture*	m_pImgDest;
};


//////////////////////////////////////////////////////////////////////////
// CDlgBijouExtract
//////////////////////////////////////////////////////////////////////////

class CDlgBijouExtract : public CDlgItemSvrBase
{
public:
	enum
	{
		POWDER_NUM = 3,
	};
	CDlgBijouExtract();
	virtual ~CDlgBijouExtract();
	
	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	virtual void SetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg=NULL);
	virtual void ServerStart();
	virtual void UpdateDisplay();
	
protected:
	virtual bool OnInitDialog();
	
//	AUIImagePicture*	m_pImgPowder[POWDER_NUM];
//	AUILabel*			m_pLabProb[POWDER_NUM];
};


//////////////////////////////////////////////////////////////////////////
// CDlgBijouTransfer
//////////////////////////////////////////////////////////////////////////

class CDlgBijouTransfer : public CDlgItemSvrBase
{
public:
	CDlgBijouTransfer();
	virtual ~CDlgBijouTransfer();
	
	virtual void SetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg=NULL);
	virtual void ServerStart();
	virtual void UpdateDisplay();
	virtual void Reset();
	
protected:
	virtual bool OnInitDialog();

	AUIImagePicture*	m_pImgProp;
	AUIImagePicture*	m_pImgDest;
};
