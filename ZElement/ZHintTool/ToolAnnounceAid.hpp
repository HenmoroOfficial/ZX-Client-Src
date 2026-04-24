#ifndef __TOOL_ANNOUNCE_AID_HPP__
#define __TOOL_ANNOUNCE_AID_HPP__

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "callid2.hpp"

namespace GNET
{

	class ToolAnnounceAid : public GNET::Protocol
	{
		public:
			int aid;
			enum { PROTOCOL_TYPE = PROTOCOL_TOOL_ANNOUNCE_AID };
		public:
			ToolAnnounceAid() :aid(0) { type = PROTOCOL_TOOL_ANNOUNCE_AID; }
			ToolAnnounceAid(void*) : Protocol(PROTOCOL_TOOL_ANNOUNCE_AID), aid(0) { }
			ToolAnnounceAid (int nAid)
				 : aid(nAid)
			{
				type = PROTOCOL_TOOL_ANNOUNCE_AID;
			}

			ToolAnnounceAid(const ToolAnnounceAid &rhs)
				: Protocol(rhs),aid(rhs.aid) { }

			GNET::Protocol *Clone() const { return new ToolAnnounceAid(*this); }

			OctetsStream& marshal(OctetsStream & os) const
			{
				os << aid;
				return os;
			}

			const OctetsStream& unmarshal(const OctetsStream &os)
			{
				os >> aid;
				return os;
			}

			int PriorPolicy( ) const { return 1; }

			bool SizePolicy(size_t size) const { return size <= 64; }//be careful.

			void Process(Manager *manager, Manager::Session::ID sid)
			{
				// TODO
			}
	};

};

#endif //__TOOL_ANNOUNCE_AID_HPP__
