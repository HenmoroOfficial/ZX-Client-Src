#ifndef _EC_NETPRTCLOBJECTINFO_H_
#define _EC_NETPRTCLOBJECTINFO_H_

// base class for oject - it may be player, npc or matter

#include "EC_NetPrtclGame.h"

class CECNetPrtclObjectInfo : public CECNetPrtclGame
{
protected:
	CECNetPrtclObjectInfo() {}
	virtual ~CECNetPrtclObjectInfo() {}

public:
	void Process_By_Object_Type(int idObject, CECGameSession* pSession);

	virtual void Process_Player(CECGameSession* pSession, int id) {}
	virtual void Process_NPC(CECGameSession* pSession, int id) {}
	virtual void Process_Matter(CECGameSession* pSession, int id) {}
};

class CECNetPrtclObjectMove : public CECNetPrtclObjectInfo
{
public:
	CECNetPrtclObjectMove() {}
	~CECNetPrtclObjectMove() {}

public:
	virtual bool Process(CECGameSession* pSession);
	virtual void Process_Player(CECGameSession* pSession, int id);
	virtual void Process_NPC(CECGameSession* pSession, int id);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclObjectMove, S2C::OBJECT_MOVE, true, 1);
};

class CECNetPrtclObjectLeaveSlice : public CECNetPrtclObjectInfo
{
public:
	CECNetPrtclObjectLeaveSlice() {}
	~CECNetPrtclObjectLeaveSlice() {}

public:
	virtual bool Process(CECGameSession* pSession);
	virtual void Process_Player(CECGameSession* pSession, int id);
	virtual void Process_NPC(CECGameSession* pSession, int id);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclObjectLeaveSlice, S2C::OBJECT_LEAVE_SLICE, true, 1);
};

class CECNetPrtclObjectDisappear : public CECNetPrtclObjectInfo
{
public:
	CECNetPrtclObjectDisappear() {}
	~CECNetPrtclObjectDisappear() {}

public:
	virtual bool Process(CECGameSession* pSession);
	virtual void Process_Player(CECGameSession* pSession, int id);
	virtual void Process_NPC(CECGameSession* pSession, int id);
	virtual void Process_Matter(CECGameSession* pSession, int id);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclObjectDisappear, S2C::OBJECT_DISAPPEAR, true, 1);
};

class CECNetPrtclObjectOutOfSight : public CECNetPrtclObjectInfo
{
public:
	CECNetPrtclObjectOutOfSight() {}
	~CECNetPrtclObjectOutOfSight() {}

public:
	virtual bool Process(CECGameSession* pSession);
	virtual void Process_Player(CECGameSession* pSession, int id);
	virtual void Process_NPC(CECGameSession* pSession, int id);
	virtual void Process_Matter(CECGameSession* pSession, int id);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclObjectOutOfSight, S2C::OUT_OF_SIGHT_LIST, true, 1);
};

class CECNetPrtclObjectEnterDim : public CECNetPrtclObjectInfo
{
public:
	CECNetPrtclObjectEnterDim() {}
	~CECNetPrtclObjectEnterDim() {}

public:
	virtual bool Process(CECGameSession* pSession);
	virtual void Process_Player(CECGameSession* pSession, int id);
	virtual void Process_NPC(CECGameSession* pSession, int id);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclObjectEnterDim, S2C::ENTER_DIM_STATE, true, 1);
};

class CECNetPrtclRecordStart : public CECNetPrtclGame
{
public:

	CECNetPrtclRecordStart() {}
	~CECNetPrtclRecordStart() {}

public:

	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclRecordStart, S2C::RECORD_START, true, 1);
};

class CECNetPrtcNPCServiceResult : public CECNetPrtclGame
{
protected:

	CECNetPrtcNPCServiceResult() {}
	virtual ~CECNetPrtcNPCServiceResult() {}

public:

	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcNPCServiceResult, S2C::NPC_SERVICE_RESULT, true, 1);
};

class CECNetPrtcBattleFlagChange : public CECNetPrtclGame
{
protected:

	CECNetPrtcBattleFlagChange() {}
	virtual ~CECNetPrtcBattleFlagChange() {}

public:

	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcBattleFlagChange, S2C::BATTLE_FLAG_CHANGE, true, 1);
};

class CECNetPrtcBattleStart : public CECNetPrtclGame
{
protected:

	CECNetPrtcBattleStart() {}
	virtual ~CECNetPrtcBattleStart() {}

public:

	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcBattleStart, S2C::BATTLE_START, true, 1);
};

class CECNetPrtcBattleFlagChangeStatus : public CECNetPrtclGame
{
protected:

	CECNetPrtcBattleFlagChangeStatus() {}
	virtual ~CECNetPrtcBattleFlagChangeStatus() {}

public:

	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcBattleFlagChangeStatus, S2C::BATTLE_FLAG_CHANGE_STATUS, true, 1);
};

/*
class CECNetPrtcPVPMode : public CECNetPrtclGame
{
protected:

	CECNetPrtcPVPMode() {}
	virtual ~CECNetPrtcPVPMode() {}

public:

	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcPVPMode, S2C::PVP_MODE, true, 1);
};
*/

class CECNetPrtcAntiCheatBonus : public CECNetPrtclGame
{
protected:

	CECNetPrtcAntiCheatBonus() {}
	virtual ~CECNetPrtcAntiCheatBonus() {}

public:

	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcAntiCheatBonus, S2C::ANTI_CHEAT_BONUS, true, 1);
};

class CECNetPrtcObjectSpecialState: public CECNetPrtclObjectInfo
{
public:
	CECNetPrtcObjectSpecialState() {}
	~CECNetPrtcObjectSpecialState() {}

public:
	virtual bool Process(CECGameSession* pSession);
	virtual void Process_Player(CECGameSession* pSession, int id);
	virtual void Process_NPC(CECGameSession* pSession, int id);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcObjectSpecialState, S2C::OBJECT_SPECIAL_STATE, true, 1);
};


#endif
