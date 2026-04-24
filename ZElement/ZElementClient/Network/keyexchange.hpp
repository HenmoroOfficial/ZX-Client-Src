
#ifndef __GNET_KEYEXCHANGE_HPP
#define __GNET_KEYEXCHANGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "netclient.h"

namespace GNET
{

class KeyExchange : public GNET::Protocol
{
#include "keyexchange"
	void Process(Manager *manager, Manager::Session::ID sid)
	{
	}
	
	Octets& GenerateKey(Octets &identity, Octets &password, Octets &nonce, Octets& key )
	{   
		HMAC_MD5Hash hash;
		hash.SetParameter(identity);
		hash.Update(password);
		hash.Update(nonce);
		return hash.Final(key);
	}
	
	void Setup(Manager* mgr, Manager::Session::ID sid, const char* name, int kick)
	{
		Octets key;
		NetClient* m = (NetClient*)mgr;
		blkickuser = kick;
		GNET::Octets id = GNET::Octets(name, strlen(name));
		m->keyCopy = GenerateKey(id, m->nonce, nonce, key);
		mgr->SetOSecurity(sid,ARCFOURSECURITY,GenerateKey(id, m->nonce, nonce, key));
		
		Random r;
		r.Update(nonce.resize(16));
		m->keyCopy2 = GenerateKey(id, m->nonce, nonce, key);
		//mgr->SetISecurity(sid,ARCFOURSECURITY,GenerateKey(id, m->nonce, nonce, key));
		mgr->SetISecurity(sid,DECOMPRESSARCFOURSECURITY,GenerateKey(id, m->nonce, nonce, key));
	}
};

};

#endif
