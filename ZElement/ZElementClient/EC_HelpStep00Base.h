/********************************************************************
	created:	2012/8/17
	author:		fuchonggang
	
	purpose:	图形帮助系统-每一步操作基类
	Copyright (C) 2012 - All Rights Reserved
*********************************************************************/

#pragma  once

#include "A3DTypes.h"

class A3DGfxEx;

class CECHelpStepBase
{
public:	

public:		//	Constructor and Destructor

	CECHelpStepBase() : m_iGfxId(0), m_iStartIndex(0), m_iEndIndex(0), m_iCurIndex(-1) {}
	virtual ~CECHelpStepBase() {}

public:		//	Attributes

public:		//	Operations

	virtual void StartHelp(int index);		// 弹出帮助提示
	virtual void FinishHelp(int index);		// 玩家作出适当操作，该帮助结束

	/** 
	* @brief UseItem 
	* 
	* Detailed description.
	* @param[in] id item id
	* @param[in] index current help index. Defaults to -1.
	*/
	virtual void UseItem(int id, int index=-1) {}			// 使用物品，特定帮助完成
	
	virtual void Help0(bool bStart=true) {}
	virtual void Help1(bool bStart=true) {}
	virtual void Help2(bool bStart=true) {}
	virtual void Help3(bool bStart=true) {}
	virtual void Help4(bool bStart=true) {}
	virtual void Help5(bool bStart=true) {}
	virtual void Help6(bool bStart=true) {}
	virtual void Help7(bool bStart=true) {}
	virtual void Help8(bool bStart=true) {}
	virtual void Help9(bool bStart=true) {}

	A3DRECT GetValidArea() { return m_rcArea; }
	bool DealKeyMsg() { return m_bDealKeyMsg; }

protected:
	int		m_iGfxId;
	int		m_iStartIndex;
	int		m_iEndIndex;

	int		m_iCurIndex;

	A3DRECT	m_rcArea;
	bool	m_bDealKeyMsg;
};