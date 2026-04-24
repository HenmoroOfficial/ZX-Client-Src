/*
 * FILE: EC_Shortcut.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/1/5
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "AString.h"
#include "AWString.h"

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////

class CECIvtrItem;
class CECSkill;
struct PET_ESSENCE;
class CECPlayer;

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECShortcut
//	
///////////////////////////////////////////////////////////////////////////

class CECShortcut
{
public:		//	Types

	//	Shortcut type
	enum
	{
		SCT_UNKNOWN = -1,
		SCT_SKILL = 0,
		SCT_ITEM,
		SCT_COMMAND,
		SCT_SKILLGRP,
		SCT_SKILLDUMB,
		SCT_TARGET,
	};

public:		//	Constructor and Destructor

	CECShortcut() { m_iSCType = SCT_UNKNOWN; }
	CECShortcut(const CECShortcut& src);
	virtual ~CECShortcut() {}

public:		//	Attributes

public:		//	Operations

	//	Clone shortcut
	virtual CECShortcut* Clone();
	//	Execute shortcut
	virtual bool Execute(unsigned int keycode = 0) { return true; }
	virtual bool OnKeyUp() { return true; }
	//	Get icon file
	virtual const char* GetIconFile();
	//	Get shortcut description text
	virtual const wchar_t* GetDesc() { return NULL; }
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL) { return 0; }

	//	Get shortcut type
	int GetType() { return m_iSCType; }

protected:	//	Attributes

	int		m_iSCType;	//	Shortcut type

protected:	//	Operations

};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECSCSkill
//  CATEGORY_TRIGGERED 类型的技能需要单独管理，其它技能为hostplayer技能的指针
//	
///////////////////////////////////////////////////////////////////////////

class CECSCSkill : public CECShortcut
{
public:		//	Types

public:		//	Constructor and Destructor

	CECSCSkill();
	CECSCSkill(const CECSCSkill& src);
	virtual ~CECSCSkill();

public:		//	Attributes

public:		//	Operations

	//	Initialize object
	bool Init(CECSkill* pSkill);

	//	Clone shortcut
	virtual CECShortcut* Clone();
	//	Execute shortcut
	virtual bool Execute(unsigned int keycode = 0);
	//  On Key up 
	//  蓄力技能的蓄力处理中使用
	virtual bool OnKeyUp();
	//	Get icon file
	virtual const char* GetIconFile();
	//	Get shortcut description text
	virtual const wchar_t* GetDesc();
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);

	//	Get skill object
	CECSkill* GetSkill() const { return m_pSkill; }
	//	Replace skill object
	void SetSkill(CECSkill* pSkill);

protected:	//	Attributes

	CECSkill*	m_pSkill;	//	Skill object

protected:	//	Operations

};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECSCItem
//	
///////////////////////////////////////////////////////////////////////////

class CECSCItem : public CECShortcut
{
public:		//	Types

public:		//	Constructor and Destructor

	CECSCItem();
	CECSCItem(const CECSCItem& src);
	virtual ~CECSCItem();

public:		//	Attributes

public:		//	Operations

	//	Initialize object
	bool Init(int iIvtr, int iSlot, CECIvtrItem* pItem);

	//	Clone shortcut
	virtual CECShortcut* Clone();
	//	Execute shortcut
	virtual bool Execute(unsigned int keycode = 0);
	//	Get icon file
	virtual const char* GetIconFile() { return m_strIconFile; }
	//	Get shortcut description text
	virtual const wchar_t* GetDesc();
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);

	//	Get inventory index
	int GetInventory() { return m_iIvtr; }
	//	Get slot index
	int GetIvtrSlot() { return m_iSlot; }
	//	Get item's template ID
	int GetItemTID() { return m_tidItem; }
	//	Set new position of item referenced by this shortcut
	void MoveItem(int iIvtr, int iSlot);
	//	Get auto find flag
	bool GetAutoFindFlag() { return m_bAutoFind; }

protected:	//	Attributes

	int			m_iIvtr;		//	Inventory index
	int			m_iSlot;		//	Slot in inventory
	int			m_tidItem;		//	Item's template ID
	AString		m_strIconFile;	//	Icon file name
	bool		m_bAutoFind;	//	Auto find flag

protected:	//	Operations

	//	Get item object
	CECIvtrItem* GetItem();
	//	Update item associated data after m_iIvtr or m_iSlot changed
	void UpdateItemData();
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECSCCommand
//	
///////////////////////////////////////////////////////////////////////////

class CECSCCommand : public CECShortcut
{
public:		//	Types

	//	Command ID
	enum
	{
		CMD_UNKNOWN = -1,
		CMD_SITDOWN = 0,
		CMD_WALKRUN,
		CMD_NORMALATTACK,
		CMD_FINDTARGET,
		CMD_ASSISTATTACK,
		CMD_INVITETOTEAM,
		CMD_LEAVETEAM,	
		CMD_KICKTEAMMEM,
		CMD_FINDTEAM,
		CMD_STARTTRADE,
		CMD_SELLBOOTH,
		CMD_BUYBOOTH,
		CMD_PLAYPOSE,
		CMD_PLAYACTIVEPOSE,
		CMD_INVITETOFACTION,
		CMD_FLY,
		CMD_PICKUP,
		CMD_GATHER,
		CMD_RUSHFLY,
		CMD_BINDBUDDY,
		CMD_PET_CALL,
		CMD_PET_COMBINE1,
		CMD_PET_COMBINE2,
	};

public:		//	Constructor and Destructor

	CECSCCommand(int iCommand);
	CECSCCommand(const CECSCCommand& src);
	virtual ~CECSCCommand();

public:		//	Attributes

public:		//	Operations

	//	Clone shortcut
	virtual CECShortcut* Clone();
	//	Execute shortcut
	virtual bool Execute(unsigned int keycode = 0);
	//	Get icon file
	virtual const char* GetIconFile();
	//	Get shortcut description text
	virtual const wchar_t* GetDesc();
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);

	//	Set / Get command parameter
	void SetParam(DWORD dwParam) { m_dwParam = dwParam; }
	DWORD GetParam() { return m_dwParam; }
	//	Get command ID
	int	GetCommandID() { return m_iCommand; }

protected:	//	Attributes

	int		m_iCommand;		//	Command ID
	DWORD	m_dwParam;		//	Parameter

protected:	//	Operations

};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECSCSkillGrp
//	
///////////////////////////////////////////////////////////////////////////

class CECSCSkillGrp : public CECShortcut
{
public:		//	Types

public:		//	Constructor and Destructor

	CECSCSkillGrp();
	CECSCSkillGrp(const CECSCSkillGrp& src);
	virtual ~CECSCSkillGrp() {}

public:		//	Attributes

public:		//	Operations

	//	Initialize object
	bool Init(int iGroupIdx);

	//	Clone shortcut
	virtual CECShortcut* Clone();
	//	Execute shortcut
	virtual bool Execute(unsigned int keycode = 0);
	//	Get icon file
	virtual const char* GetIconFile();
	//	Get shortcut description text
	virtual const wchar_t* GetDesc();
	//	Get item cool time
	virtual int GetCoolTime(int* piMax=NULL);

	//	Get group index
	int GetGroupIndex() { return m_iGroupIdx; }

protected:	//	Attributes

	int			m_iGroupIdx;	//	Group index
	AWString	m_strDesc;		//	Description

protected:	//	Operations

};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECSCSkillDumb
//  天界没有家族技能等技能，保存在快捷栏的信息会丢失，特此添加
//	
///////////////////////////////////////////////////////////////////////////

class CECSCSkillDumb : public CECShortcut
{
public:		//	Types
	
public:		//	Constructor and Destructor
	
	CECSCSkillDumb();
	CECSCSkillDumb(const CECSCSkillDumb& src);
	virtual ~CECSCSkillDumb();
	
public:		//	Attributes
	
public:		//	Operations
	
	//	Initialize object
	bool Init(int id);
	
	//	Clone shortcut
	virtual CECShortcut* Clone();
	
	//	Get skill object
	int GetSkillId() { return m_iSkillId; }
	//	Replace skill object
	void SetSkillId(int id) { m_iSkillId = id; }
	
protected:	//	Attributes
	
	int		m_iSkillId;	//	Skill object
	
protected:	//	Operations
	
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECSCTarget
//	可选择的敌对目标：Player，Monster
///////////////////////////////////////////////////////////////////////////

class CECSCTarget : public CECShortcut
{
public:		//	Types

	enum
	{
		TT_UNKNOWN = 0,
		TT_PLAYER,
		TT_MONSTER,

		NUM_TT,
	};
public:		//	Constructor and Destructor

	CECSCTarget();
	CECSCTarget(const CECSCTarget& src);
	virtual ~CECSCTarget();

public:		//	Attributes

public:		//	Operations

	//	Initialize object
	bool Init(int nTargetType, int id, int level,int prof, int feishengcnt, int cultivation);

	//	Clone shortcut
	virtual CECShortcut* Clone();
	//	Execute shortcut
	virtual bool Execute(unsigned int keycode = 0);
	
	//  Check whether can execute 
	bool CanExecute();

	//	Get icon file
	virtual const char* GetIconFile() { return m_strIconFile; }
	//	Get shortcut description text
	virtual const wchar_t* GetDesc() { return m_strDesc; }

	//  Get target type
	int GetTargetType() { return m_nTargetType; }

	int GetTargetID() { return m_nTargetID; }
	int GetTargetProfession() { return m_nTargetProf; }

	int GetTargetLevel()	{ return m_nTargetLevel; }

	int GetFeishengCnt()	{ return m_nFeishengCnt; }
	
	int GetCultivation()	{ return m_nCultivation; }

	void BuildDesc();

	AWString	GetTargetName() { return m_strName; }
protected:	//	Attributes

	int			m_nTargetType;	// 0:unknown, 1:player, 2:monster

	int			m_nTargetID;	

	AWString	m_strDesc;		//	Description

	AWString	m_strName;		//	Name
	int			m_nTargetLevel;

	// for player
	int			m_nTargetProf;
	int			m_nFeishengCnt;
	int			m_nCultivation;
	

	AString		m_strIconFile;	//	Icon file name
protected:	//	Operations

	//	Get target object
	CECPlayer* GetPlayer();
	//	Update target associated data after player level changed
	void UpdateTargetData();

	//  Select a target
	void SelectTarget();
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////



