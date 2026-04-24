#ifndef __GNET_GET_ITEM_DESC_FOR_PLTFM_RE_HPP
#define __GNET_GET_ITEM_DESC_FOR_PLTFM_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


#include "callid2.hpp"

namespace GNET
{

	class GetItemDescForPlatformRe : public GNET::Protocol
	{
	public:
		byte		retCode;
		INT64		sn;
		INT64		timestamp;
		int			templateID;
		int			level;
		Octets		name;
		Octets		icon;
		Octets		detailInfo;
		enum { PROTOCOL_TYPE = PROTOCOL_GETITEMDESC_PLTFM_RE };//the type of the protocol.
	public:
		GetItemDescForPlatformRe() : retCode(0), sn(0), 
		timestamp(0), templateID(-1), level(-1)
		{
			type = PROTOCOL_GETITEMDESC_PLTFM_RE; 
		}
		GetItemDescForPlatformRe(void*) : Protocol(PROTOCOL_GETITEMDESC_PLTFM_RE),
		retCode(1), sn(0), timestamp(0), templateID(-1), level(-1){ }
		GetItemDescForPlatformRe( byte retcode, INT64 serialNum, INT64 timeStamp, int templateid, 
			int level, const Octets& name, const Octets& icon, const Octets& data )
			 : retCode(retcode), sn(serialNum),timestamp(timeStamp),templateID(templateid), 
			 level(level), name(name), icon(icon), detailInfo(data)
		{
			type = PROTOCOL_GETITEMDESC_PLTFM_RE;
		}

		GetItemDescForPlatformRe(const GetItemDescForPlatformRe &rhs)
			: Protocol(rhs),retCode(rhs.retCode), sn(rhs.sn),timestamp(rhs.timestamp),
			templateID(rhs.templateID), level(rhs.level), name(rhs.name), icon(rhs.icon), 
			detailInfo(rhs.detailInfo) { }

		GNET::Protocol *Clone() const { return new GetItemDescForPlatformRe(*this); }

		OctetsStream& marshal(OctetsStream & os) const
		{
			os << retCode;
			os << sn;
			os << timestamp;
			os << templateID;
			os << level;
			os << name;
			os << icon;
			os << detailInfo;
			return os;
		}

		const OctetsStream& unmarshal(const OctetsStream &os)
		{
			os >> retCode;
			os >> sn;
			os >> timestamp;
			os >> templateID;
			os >> level;
			os >> name;
			os >> icon;
			os >> detailInfo;
			return os;
		}

		int PriorPolicy( ) const { return 3; }

		bool SizePolicy(size_t size) const { return size <= 10240; }//be careful the size.	

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
	};

};

#endif //__GNET_GET_ITEM_DESC_FOR_PLTFM_RE_HPP
