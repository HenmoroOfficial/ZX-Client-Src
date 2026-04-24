#ifndef __GNET__GAMECLIENT
#define __GNET__GAMECLIENT

#include "gnet.h"
#include "netclient.h"
#include "gnconf.h"

#ifdef _INVENTORY_DESC_MAN //Added by Ford.W 2010-09-10.
#include "state2.hpp"
#endif

namespace GNET
{
	union Attr{
		UINT64 _attr;
		struct {
			unsigned char load;
			unsigned char lambda;
			unsigned char anything;
			unsigned char multipleExp;
			unsigned char multipleMoney;
			unsigned char doubleObject;
			unsigned char doubleSP;
			unsigned char freeZone;
		};
	};

	typedef struct _ServerStatus
	{
		DWORD ping;
		union Attr attr;
	}ServerStatus;

	class GameClient: public NetClient, public Timer::Observer
	{
		static GameClient instance;
		char m_zoneid;
		Session::ID m_sid;
		Thread::Mutex locker;
		std::map<Session::ID, DWORD> pingmap;
		std::string hostaddr;

		int m_state;
		unsigned int m_ping;
		DWORD m_lastsend;
		DWORD m_lastrecv;

		bool m_bIsUseUsbKey;

		enum STATE{
			STATE_CLOSED,
			STATE_CONNECTING,
			STATE_KEEPING,
		};

	public:
		typedef int(* callback_t)(void*, Session::ID, int);

		GameClient() : m_callback(NULL), m_state(STATE_CLOSED), m_ping(100)	
		{
			Timer::Attach(this);
		}	

		static GameClient& GetInstance() 
		{ 
			instance.blCompress = (atoi(Conf::GetInstance()->find("GameClient","compress").c_str())==1);
			return instance;
		}

		bool InputPolicy(Protocol::Type type, size_t len) const { return true; }
#ifndef _INVENTORY_DESC_MAN //Added by Ford.W 2010-09-10.
		const Session::State *GetInitState() const { return &state_GLoginClient;}
#else
		const Session::State *GetInitState() const { return &state_ElementHint;}
#endif
		std::string Identification() const { return "GameClient"; }
		const std::string& GetPeer() const { return hostaddr; }

		void Attach(callback_t funptr) { m_callback = funptr; }
		void Detach() { m_callback = NULL; }
		void SetZoneID(char zoneid) { m_zoneid = zoneid; }
		char GetZoneID() const { return m_zoneid; }

		void OnAddSession(Session::ID sid);
		void OnDelSession(Session::ID sid);
		void OnAbortSession(Session::ID sid);
		void OnRecvProtocol(Session::ID sid, Protocol* pdata);
		int Connect(const char* host, unsigned short port);
		void Disconnect();
		void Update();

		bool ChangeState(Session::ID id, Session::State *state)
		{
			Thread::Mutex::Scoped l(locker);
#ifndef _INVENTORY_DESC_MAN //Added by Ford.W 2010-09-10.
			if(state==&state_GSelectRoleClient)
				m_state = STATE_KEEPING;
#else
			if(state==&state_ElementHint)
			{
				m_state = STATE_KEEPING;
			}
#endif
			return Manager::ChangeState(id,state);
		}

		unsigned int GetPing()
		{
			if(m_ping > 9999)
				return 9999;
			return m_ping>5?m_ping:5;
		}

		const char* GetLastError();
		void SetupResponse(Response&, const char* name,const char* rawname,const char* passwd, const char* state, Octets& challenge, char algo, bool bUseToken);
		bool IsUseUsbKey() { return m_bIsUseUsbKey; }
		bool IsUsbKeyExisting();
		bool IsInStateKeeping() const { return m_state == STATE_KEEPING; }

		void RestoreISecurity();
		void RestoreOSecurity();
		
	protected:
		int ConnectTo(const char* host, unsigned short port);
		int ConnectTo(struct in_addr *, unsigned short port);
		callback_t m_callback;
	};
};
#endif
