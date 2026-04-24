#ifndef __GNET_GET_ROLE_EQUIP_RE_HPP__
#define __GNET_GET_ROLE_EQUIP_RE_HPP__

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "callid2.hpp"


namespace GNET
{

	class GetRoleEquipRe : public GNET::Protocol
	{
	public:
		byte	retCode;
		int		zone_id;
		INT64	role_id;
		INT64	timestamp;
		Octets	detailInfo;
		enum { PROTOCOL_TYPE = PROTOCOL_GET_ROLE_EQUIP_RE };//the type of the protocol.
	public:
		GetRoleEquipRe() : retCode(0), zone_id(0), role_id(0), timestamp(0){ type = PROTOCOL_GET_ROLE_EQUIP_RE; }
		GetRoleEquipRe(void*) : Protocol(PROTOCOL_GET_ROLE_EQUIP_RE), retCode(0), zone_id(0), role_id(0), timestamp(0) { }
		GetRoleEquipRe( byte ret, int nZoneId, INT64 lRoleId, INT64 lTimeStamp,const Octets& data )
			 : retCode(ret), zone_id(nZoneId),role_id(lRoleId), timestamp(lTimeStamp),detailInfo(data)
		{
			type = PROTOCOL_GET_ROLE_EQUIP_RE;
		}

		GetRoleEquipRe(const GetRoleEquipRe &rhs)
			: Protocol(rhs),retCode(rhs.retCode), zone_id(rhs.zone_id), role_id(rhs.role_id), timestamp(rhs.timestamp),detailInfo(rhs.detailInfo) { }

		GNET::Protocol *Clone() const { return new GetRoleEquipRe(*this); }

		OctetsStream& marshal(OctetsStream & os) const
		{
			os << retCode;
			os << zone_id;
			os << role_id;
			os << timestamp;
			os << detailInfo;
			return os;
		}

		const OctetsStream& unmarshal(const OctetsStream &os)
		{
			os >> retCode;
			os >> zone_id;
			os >> role_id;
			os >> timestamp;
			os >> detailInfo;
			return os;
		}

		int PriorPolicy() const { return 1; }

		bool SizePolicy(size_t size) const { return size <= 10240; }	

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
	};

};

#endif //__GNET_GET_ROLE_EQUIP_RE_HPP__
