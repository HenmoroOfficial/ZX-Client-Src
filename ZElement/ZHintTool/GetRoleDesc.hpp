#ifndef __GNET_GET_ROLE_DESC_HPP__
#define __GNET_GET_ROLE_DESC_HPP__

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "callid2.hpp"


namespace GNET
{

	class GetRoleDesc : public GNET::Protocol
	{
	public:
		INT64 sn;
		INT64 timestamp;
		Octets detailInfo;
		enum { PROTOCOL_TYPE = PROTOCOL_GET_ROLE_DESC };//the type of the protocol.
	public:
		GetRoleDesc() : sn(0), timestamp(0){ type = PROTOCOL_GET_ROLE_DESC; }
		GetRoleDesc(void*) : Protocol(PROTOCOL_GET_ROLE_DESC), sn(0), timestamp(0) { }
		GetRoleDesc( INT64 serialNum, INT64 timeStamp,const Octets& data )
			 : sn(serialNum),timestamp(timeStamp),detailInfo(data)
		{
			type = PROTOCOL_GET_ROLE_DESC;
		}

		GetRoleDesc(const GetRoleDesc &rhs)
			: Protocol(rhs),sn(rhs.sn),timestamp(rhs.timestamp),detailInfo(rhs.detailInfo) { }

		GNET::Protocol *Clone() const { return new GetRoleDesc(*this); }

		OctetsStream& marshal(OctetsStream & os) const
		{
			os << sn;
			os << timestamp;
			os << detailInfo;
			return os;
		}

		const OctetsStream& unmarshal(const OctetsStream &os)
		{
			os >> sn;
			os >> timestamp;
			os >> detailInfo;
			return os;
		}

		int PriorPolicy( ) const { return 2; }

		bool SizePolicy(size_t size) const { return size <= 131072; }	

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
	};

};

#endif //__GNET_GET_ROLE_DESC_HPP__
