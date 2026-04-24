#ifndef __GNET_GET_ITEM_DESC_FOR_PLTFM_HPP
#define __GNET_GET_ITEM_DESC_FOR_PLTFM_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


#include "callid2.hpp"


namespace GNET
{

	class GetItemDescForPlatform : public GNET::Protocol
	{
	public:
		INT64 sn;
		INT64 timestamp;
		Octets detailInfo;
		Octets md5Info;
		enum { PROTOCOL_TYPE = PROTOCOL_GETITEMDESC_PLTFM };//the type of the protocol.
	public:
		GetItemDescForPlatform() : sn(0), timestamp(0){ type = PROTOCOL_GETITEMDESC_PLTFM; }
		GetItemDescForPlatform(void*) : Protocol(PROTOCOL_GETITEMDESC_PLTFM), sn(0), timestamp(0) { }
		GetItemDescForPlatform( INT64 serialNum, INT64 timeStamp,const Octets& data, const Octets& md5Info )
			 : sn(serialNum),timestamp(timeStamp),detailInfo(data), md5Info(md5Info)
		{
			type = PROTOCOL_GETITEMDESC_PLTFM;
		}

		GetItemDescForPlatform(const GetItemDescForPlatform &rhs)
			: Protocol(rhs),sn(rhs.sn),timestamp(rhs.timestamp),detailInfo(rhs.detailInfo),md5Info(rhs.md5Info) { }

		GNET::Protocol *Clone() const { return new GetItemDescForPlatform(*this); }

		OctetsStream& marshal(OctetsStream & os) const
		{
			os << sn;
			os << timestamp;
			os << detailInfo;
			os << md5Info;
			return os;
		}

		const OctetsStream& unmarshal(const OctetsStream &os)
		{
			os >> sn;
			os >> timestamp;
			os >> detailInfo;
			os >> md5Info;
			return os;
		}

		int PriorPolicy( ) const { return 2; }

		bool SizePolicy(size_t size) const { return size <= 1024; }	

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
	};

};

#endif //__GNET_GET_ITEM_DESC_FOR_PLTFM_HPP
