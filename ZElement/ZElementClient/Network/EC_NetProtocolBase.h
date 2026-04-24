#ifndef _EC_NETPROTOCOLBASE_H_
#define	_EC_NETPROTOCOLBASE_H_

#include "EC_GPDataType.h"

namespace GNET
{
	class Protocol;
}

class CECGameSession;

template<class T, int id, bool game_prtcl>
class NetPrtclRegisterStub
{
public:
	NetPrtclRegisterStub();
	~NetPrtclRegisterStub() {}
};

#define DECL_STUB(class_name, prtcl_id, game_prtcl, order) \
static NetPrtclRegisterStub<class_name, prtcl_id, game_prtcl> _stub##order

#define IMPL_STUB(class_name, prtcl_id, game_prtcl, order) \
NetPrtclRegisterStub<class_name, prtcl_id, game_prtcl> class_name::_stub##order

// 派生的类必须在CECNetProtocolStub类中注册

class CECNetProtocolBase
{
protected:
	CECNetProtocolBase() : m_pRaw(0) {}
	virtual ~CECNetProtocolBase() {}

protected:

	GNET::Protocol* m_pRaw;

protected:

	virtual void Attach(GNET::Protocol* p) { m_pRaw = p; }

public:

	GNET::Protocol* GetRawProtocol() { return m_pRaw; }
	void Detach() { m_pRaw = 0; }

	virtual void Destroy()
	{
		if (m_pRaw)
			m_pRaw->Destroy();

		delete this;
	}

	virtual bool Filter(CECGameSession* pSession) { return false; }

	// pure virtual function
	virtual bool Process(CECGameSession* pSession) = 0;

	// common create function
	static CECNetProtocolBase* CreateFrom(GNET::Protocol*);
};

#define _CREATE_PRTCL_FROM(type) \
	type* p = new type;\
	p->Attach(pSrc);\
	return p;

#endif
