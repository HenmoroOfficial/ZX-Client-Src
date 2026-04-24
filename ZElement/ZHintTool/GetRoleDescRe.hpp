#ifndef __GNET_GET_ROLE_DESC_RE_HPP__
#define __GNET_GET_ROLE_DESC_RE_HPP__

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "callid2.hpp"

namespace GNET
{

	class GetRoleDescRe : public GNET::Protocol
	{
	public:
		byte		retCode;
		INT64		sn;
		INT64		timestamp;
		int			cls;
		byte		gender;
		int			level;
		Octets		name;
		Octets		icon;
		Octets		basicDetailInfo;
		Octets		detailDesc;
		enum { PROTOCOL_TYPE = PROTOCOL_GET_ROLE_DESC_RE };//the type of the protocol.
	public:
		GetRoleDescRe() : retCode(0), sn(0), 
		timestamp(0), cls(-1), gender(0), level(-1)
		{
			type = PROTOCOL_GET_ROLE_DESC_RE; 
		}
		GetRoleDescRe(void*) : Protocol(PROTOCOL_GET_ROLE_DESC_RE),
		retCode(0), sn(0), timestamp(0), cls(-1), gender(0), level(-1){ }
		GetRoleDescRe( byte retcode, INT64 serialNum, INT64 timeStamp, int cls, byte gender,
			int level, const Octets& name, const Octets& icon, const Octets& basicinfo, const Octets& data )
			 : retCode(retcode), sn(serialNum),timestamp(timeStamp),cls(cls), gender(gender),
			 level(level), name(name), icon(icon), basicDetailInfo(basicinfo), detailDesc(data)
		{
			type = PROTOCOL_GET_ROLE_DESC_RE;
		}

		GetRoleDescRe(const GetRoleDescRe &rhs)
			: Protocol(rhs),retCode(rhs.retCode), sn(rhs.sn),timestamp(rhs.timestamp),
			cls(rhs.cls), gender(rhs.gender), level(rhs.level), name(rhs.name), icon(rhs.icon), 
			basicDetailInfo(rhs.basicDetailInfo), detailDesc(rhs.detailDesc) { }

		GNET::Protocol *Clone() const { return new GetRoleDescRe(*this); }

		OctetsStream& marshal(OctetsStream & os) const
		{
			os << retCode;
			os << sn;
			os << timestamp;
			os << cls;
			os << gender;
			os << level;
			os << name;
			os << icon;
			os << basicDetailInfo;
			os << detailDesc;
			return os;
		}

		const OctetsStream& unmarshal(const OctetsStream &os)
		{
			os >> retCode;
			os >> sn;
			os >> timestamp;
			os >> cls;
			os >> gender;
			os >> level;
			os >> name;
			os >> icon;
			os >> basicDetailInfo;
			os >> detailDesc;
			return os;
		}

		int PriorPolicy( ) const { return 3; }

		bool SizePolicy(size_t size) const { return size <= 1310720; }//be careful the size.	

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
	};

};

#endif //__GNET_GET_ROLE_DESC_RE_HPP__
