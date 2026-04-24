#ifndef _EC_NETPRTCLNPCINFO_H_
#define _EC_NETPRTCLNPCINFO_H_

#include "EC_NetPrtclGame.h"

class CECNetPrtclNPCInfo : public CECNetPrtclGame
{
public:
	CECNetPrtclNPCInfo() {}
	~CECNetPrtclNPCInfo() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclNPCInfo, S2C::NPC_INFO_LIST, true, 1);
	DECL_STUB(CECNetPrtclNPCInfo, S2C::NPC_ENTER_SLICE, true, 2);
	DECL_STUB(CECNetPrtclNPCInfo, S2C::NPC_ENTER_WORLD, true, 3);
	DECL_STUB(CECNetPrtclNPCInfo, S2C::NPC_INFO_00, true, 4);
};

class CECNetPrtclNPCDie : public CECNetPrtclGame
{
public:
	CECNetPrtclNPCDie() {}
	~CECNetPrtclNPCDie() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclNPCDie, S2C::NPC_DIED, true, 1);
};

class CECNetPrtclNPCDie2 : public CECNetPrtclGame
{
public:
	CECNetPrtclNPCDie2() {}
	~CECNetPrtclNPCDie2() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclNPCDie2, S2C::NPC_DIED2, true, 1);
};

#endif
