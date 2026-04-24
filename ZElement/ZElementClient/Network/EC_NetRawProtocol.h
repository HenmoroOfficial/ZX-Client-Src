#ifndef _EC_NETRAWPROTOCOL_H_
#define _EC_NETRAWPROTOCOL_H_

#include "EC_NetProtocolBase.h"

class CECNetRawProtocol : public CECNetProtocolBase
{
public:
	CECNetRawProtocol() {}
	~CECNetRawProtocol() {}

public:
	virtual bool Filter(CECGameSession* pSession);
	virtual bool Process(CECGameSession* pSession);

public:
	static CECNetProtocolBase* CreateRaw(GNET::Protocol* pSrc)
	{
		_CREATE_PRTCL_FROM(CECNetRawProtocol)
	}
};

#endif
