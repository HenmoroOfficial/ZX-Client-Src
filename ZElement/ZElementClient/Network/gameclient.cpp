#include <winsock2.h>
#include "gameclient.h"
#include "challenge.hpp"
#include "UsbHidKey.h"

#ifdef _INVENTORY_DESC_MAN //Added by Ford.W 2010-09-10.
#include "NewKeepAlive.hpp"
#include "ALog.h"
#endif

extern void VerifyDefenceThread();

namespace GNET
{
	GameClient GameClient::instance;
	const char *GameClient::GetLastError()
	{
		Thread::Mutex::Scoped l(locker);
		int n = strlen(errormsg);
		DWORD now = GetTickCount();
		if(n<ERRSIZE-20)
			_snprintf(errormsg+n, ERRSIZE-n, " %d:%d", now-m_lastsend,now-m_lastrecv);
		return errormsg;
	}

	void GameClient::OnAddSession(Session::ID sid) 
	{
		Thread::Mutex::Scoped l(locker);
		if(m_callback&&m_state==STATE_CONNECTING&&m_sid==sid)
			m_callback(NULL, sid, EVENT_ADDSESSION);
	}

	void GameClient::OnDelSession(Session::ID sid) 
	{
		Thread::Mutex::Scoped l(locker);
		if(m_callback && sid==m_sid)
		{
			if(m_state==STATE_KEEPING)
			{
				m_callback(NULL, sid, EVENT_DELSESSION);
				m_state = STATE_CLOSED;
			}
			m_callback(NULL, sid, EVENT_DISCONNECT);
		}
	}

	void GameClient::OnAbortSession(Session::ID sid) 
	{ 
		Thread::Mutex::Scoped l(locker);
		if(m_callback && m_state==STATE_CONNECTING && m_sid==sid)
			m_callback(NULL, sid, EVENT_ABORTSESSION);
	}

	int GameClient::Connect(const char* host, unsigned short port)
	{
		Thread::Mutex::Scoped l(locker);
		if(m_state!=STATE_CLOSED)
			Disconnect();
		m_state = STATE_CONNECTING;
		m_sid = -1;
		ConnectTo(host, port);
		return m_sid;
	}
	void GameClient::Disconnect()
	{
		Close(m_sid);
		m_sid = -1;
		m_state = STATE_CLOSED;
	}

	void GameClient::OnRecvProtocol(Session::ID sid, Protocol* pdata) 
	{
		Thread::Mutex::Scoped l(locker);
		if(sid==m_sid)
		{
			if(pdata->GetType()==PROTOCOL_KEEPALIVE)
			{
				m_lastrecv = GetTickCount();
				m_ping = m_lastrecv - m_lastsend;
				pdata->Destroy();
			}else{
				m_callback(pdata, sid, WM_IOPROTOCOL);
			}
		}
	} 

	void GameClient::Update()
	{
		Thread::Mutex::Scoped l(locker);
		static unsigned long _counter = 0;
		if(m_state==STATE_KEEPING && (_counter%15)==0){
			m_lastsend = GetTickCount();
#ifndef _INVENTORY_DESC_MAN //Added by Ford.W 2010-09-10.
			Send(m_sid, KeepAlive((unsigned int)PROTOCOL_KEEPALIVE));
#else
			Send(m_sid, NewKeepAlive() );
//			a_LogOutput( 1,  "Send New KeepAlive....\n" );
#endif
		}
		if ((_counter % 10) == 0)
			VerifyDefenceThread();
		_counter++;
	}

	int GameClient::ConnectTo(struct in_addr *host, unsigned short port)
	{
		std::string hostaddr = inet_ntoa(*(struct in_addr *)(host));
		m_sid = Protocol::Client(this, hostaddr.c_str(), port);
		return m_sid;
	}
	int GameClient::ConnectTo(const char* hostname, unsigned short port)
	{
		if(!hostname)
		{
			m_sid = Protocol::Client(this);
			return m_sid;
		}
		unsigned long   rst;

		rst = inet_addr(hostname);
		if(rst == INADDR_NONE)
		{
			struct hostent * addr = gethostbyname(hostname);
			if(!addr)
			{
				m_sid = -1;
				hostaddr.clear();
				return m_sid; 
			}

			char ** p;
			int n = 0;
			for(p=addr->h_addr_list;*p!=NULL;p++) 
				n++;

			if(n<=0)
				return 0;
			n = rand() % n;
			hostaddr = inet_ntoa(*(struct in_addr *)(addr->h_addr_list[n]));
		}
		else
			hostaddr = hostname;

		m_sid = Protocol::Client(this, hostaddr.c_str(), port);
		return m_sid;
	}

	void GameClient::SetupResponse(Response& data, const char* name,const char* rawname,const char* passwd, const char* state,  Octets& challenge, char algo, bool bUseToken)
	{
		if (bUseToken)
		{
			MD5Hash md5;
			md5.Update(Octets(passwd,strlen(passwd)));
			md5.Update(challenge);
			md5.Final(data.response);			
		}
		else if(!algo)
		{
			m_bIsUseUsbKey = HIDUSBKEY::TryDigestByUsbKey( passwd, (const BYTE*)(challenge.begin()), challenge.size(), (BYTE*)data.response.resize( 16).begin());
			if(!m_bIsUseUsbKey)
			{
				HMAC_MD5Hash hash;
				MD5Hash md5;
				Octets digest;
				md5.Update(Octets(rawname,strlen(rawname)));
				md5.Update(Octets(passwd,strlen(passwd)));
				md5.Final(digest);
				hash.SetParameter(digest);
				hash.Update(challenge);
				hash.Final(data.response);
			}
		}
		else
		{
			data.response.replace(passwd, strlen(passwd));
			char* ptr1 = (char*)data.response.begin();
			char* ptr2 = (char*)challenge.begin();
			size_t len = strlen(passwd);
			for(size_t i=0,j=0;i<len;i++,j++)
			{
				if(j>=challenge.size())
				{
					j = 0;
					ptr2 = (char*)challenge.begin();
				}
				*ptr1 ^= *ptr2;
				ptr1++;
				ptr2++;
			}
		}
		data.identity.replace(name,strlen(name));
		data.state.replace(state, strlen(state));
		nonce = data.response;
	}
	bool GameClient::IsUsbKeyExisting()
	{
		return HIDUSBKEY::IsHasUsbKey();
	}

	void GameClient::RestoreOSecurity()
	{
		SetOSecurity(m_sid, ARCFOURSECURITY, keyCopy);
//		keyCopy.clear();	
	}
	void GameClient::RestoreISecurity()
	{
		SetISecurity(m_sid, DECOMPRESSARCFOURSECURITY, keyCopy2);
//		keyCopy2.clear();
	}

}

