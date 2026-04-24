#ifndef __GNET_GET_SNS_ROLE_PET_HPP__
#define __GNET_GET_SNS_ROLE_PET_HPP__

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "callid2.hpp"


namespace GNET
{

	class GetSNSRolePet : public GNET::Protocol
	{
	public:
		int		zone_id;
		INT64	role_id;
		INT64	timestamp;
		Octets	detailInfo;
		enum { PROTOCOL_TYPE = PROTOCOL_GET_SNS_ROLE_PET };//the type of the protocol.
	public:
		GetSNSRolePet() : zone_id(0), role_id(0), timestamp(0){ type = PROTOCOL_GET_SNS_ROLE_PET; }
		GetSNSRolePet(void*) : Protocol(PROTOCOL_GET_SNS_ROLE_PET), zone_id(0), role_id(0), timestamp(0) { }
		GetSNSRolePet( int nZoneId, INT64 lRoleId, INT64 lTimeStamp,const Octets& data )
			 : zone_id(nZoneId),role_id(lRoleId), timestamp(lTimeStamp),detailInfo(data)
		{
			type = PROTOCOL_GET_SNS_ROLE_PET;
		}

		GetSNSRolePet(const GetSNSRolePet &rhs)
			: Protocol(rhs),zone_id(rhs.zone_id), role_id(rhs.role_id), timestamp(rhs.timestamp),detailInfo(rhs.detailInfo) { }

		GNET::Protocol *Clone() const { return new GetSNSRolePet(*this); }

		OctetsStream& marshal(OctetsStream & os) const
		{
			os << zone_id;
			os << role_id;
			os << timestamp;
			os << detailInfo;
			return os;
		}

		const OctetsStream& unmarshal(const OctetsStream &os)
		{
			os >> zone_id;
			os >> role_id;
			os >> timestamp;
			os >> detailInfo;
			return os;
		}

		int PriorPolicy() const { return 1; }

		bool SizePolicy(size_t size) const { return size <= 1024; }	

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
	};

};

#endif //__GNET_GET_SNS_ROLE_PET_HPP__
