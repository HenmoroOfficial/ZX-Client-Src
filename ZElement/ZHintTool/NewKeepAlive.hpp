#ifndef __GNET_NEW_KEEPALIVE_HPP__
#define __GNET_NEW_KEEPALIVE_HPP__

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

//Added 2010-08-09.
#include "callid2.hpp"

namespace GNET
{

	class NewKeepAlive : public GNET::Protocol
	{
		public:
			int reserved;
			enum { PROTOCOL_TYPE = PROTOCOL_NEWKEEPALIVE };
		public:
			NewKeepAlive() :reserved(1) { type = PROTOCOL_NEWKEEPALIVE; }
			NewKeepAlive(void*) : Protocol(PROTOCOL_NEWKEEPALIVE), reserved(1) { }
			NewKeepAlive (int l_reserveCode)
				 : reserved(l_reserveCode)
			{
				type = PROTOCOL_NEWKEEPALIVE;
			}

			NewKeepAlive(const NewKeepAlive &rhs)
				: Protocol(rhs),reserved(rhs.reserved) { }

			GNET::Protocol *Clone() const { return new NewKeepAlive(*this); }

			OctetsStream& marshal(OctetsStream & os) const
			{
				os << reserved;
				return os;
			}

			const OctetsStream& unmarshal(const OctetsStream &os)
			{
				os >> reserved;
				return os;
			}

			bool SizePolicy(size_t size) const { return size <= 32; }//be careful.

			void Process(Manager *manager, Manager::Session::ID sid)
			{
				// TODO
			}
	};

};

#endif //__GNET_NEW_KEEPALIVE_HPP__
