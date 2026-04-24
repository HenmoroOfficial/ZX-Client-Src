#ifndef __GNET__PINGCLIENT
#define __GNET__PINGCLIENT

#include "gnet.h"
#include "netclient.h"
#include "gameclient.h"
#include "gnconf.h"
namespace GNET
{
	class PingClient: public NetClient
	{
		static PingClient instance;
		Thread::Mutex locker;
		std::map<Session::ID, DWORD> pingmap;

	public:
		typedef int(* callback_t)(void*, Session::ID, int);

		PingClient() : m_callback(NULL)
		{
		}	

		static PingClient& GetInstance() 
		{ 
			return instance;
		}

		bool InputPolicy(Protocol::Type type, size_t len) const { return true; }
		const Session::State *GetInitState() const { return &state_GLoginClient;}
		std::string Identification() const { return "GameClient"; }

		void Attach(callback_t funptr) { m_callback = funptr; }
		void Detach() { m_callback = NULL; }

		void OnAddSession(Session::ID sid) 
		{
			Thread::Mutex::Scoped l(locker);
			pingmap[sid] = GetTickCount();
		}

		void OnDelSession(Session::ID sid) 
		{
		}

		void OnAbortSession(Session::ID sid) 
		{ 
		}

		int Ping(struct in_addr *host, unsigned short port)
		{
			return ConnectTo(host, port);
		}

		void OnRecvProtocol(Session::ID sid, Protocol* pdata);

	protected:
		callback_t m_callback;
		int ConnectTo(struct in_addr *host, unsigned short port);
		int ConnectTo(const char* hostname, unsigned short port);
	};
};
#endif
