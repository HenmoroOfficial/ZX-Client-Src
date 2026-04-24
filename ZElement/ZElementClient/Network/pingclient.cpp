#include <winsock2.h>
#include "pingclient.h"
#include "challenge.hpp"

namespace GNET
{
	PingClient PingClient::instance;

	void PingClient::OnRecvProtocol(Session::ID sid, Protocol* pdata) 
	{
		if(pdata->GetType()==PROTOCOL_CHALLENGE)
		{
			Challenge* p = (Challenge*)pdata;
			ServerStatus status;
			status.attr._attr = p->server_attr;
			{
				Thread::Mutex::Scoped l(locker);
				status.ping = (GetTickCount()-pingmap[sid]);
			}
			if(status.ping<=5)
				status.ping = 5;
			status.ping = status.ping>9999?9999:status.ping;
			m_callback(&status, sid, EVENT_PINGRETURN);
			Close(sid);
			p->Destroy();
		}
	} 
	int PingClient::ConnectTo(struct in_addr *host, unsigned short port)
	{
		std::string hostaddr = inet_ntoa(*(struct in_addr *)(host));
		return Protocol::Client(this, hostaddr.c_str(), port);
	}
	int PingClient::ConnectTo(const char* hostname, unsigned short port)
	{
		if(!hostname)
			return Protocol::Client(this);

		struct hostent * addr = gethostbyname(hostname);
	
		if(!addr)
			return Protocol::Client(this, hostname, port);
		
		char ** p;
		int n = 0;
		for(p=addr->h_addr_list;*p!=NULL;p++) 
			n++;

		if(n<=0)
			return 0;
		n = rand() % n;
		std::string hostaddr = inet_ntoa(*(struct in_addr *)(addr->h_addr_list[n]));
	
		return Protocol::Client(this, hostaddr.c_str(), port);
	}
}

