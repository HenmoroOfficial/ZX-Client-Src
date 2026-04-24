#ifndef _EC_NETPRTCLGAME_H_
#define _EC_NETPRTCLGAME_H_

#include "EC_NetProtocolBase.h"

class CECGameSession;

class CECNetPrtclGame : public CECNetProtocolBase
{
protected:
	CECNetPrtclGame() {}
	virtual ~CECNetPrtclGame() {}

protected:

	unsigned short m_Cmd;
	BYTE* m_pData;
	int m_nDataSize;

protected:

	void PreProcess(CECGameSession* pSession);

public:
	virtual bool Filter(CECGameSession* pSession);
};

#endif
