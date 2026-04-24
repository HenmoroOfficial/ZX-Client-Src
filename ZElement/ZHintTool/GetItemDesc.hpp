#ifndef __GNET_GET_ITEM_DESC_HPP
#define __GNET_GET_ITEM_DESC_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

//Added 2010-08-09.
#include "callid2.hpp"


namespace GNET
{

	class GetItemDesc : public GNET::Protocol
	{
	public:
		INT64 sn;
		INT64 timestamp;
		Octets detailInfo;
		enum { PROTOCOL_TYPE = PROTOCOL_GETITEMDESC };//the type of the protocol.
	public:
		GetItemDesc() : sn(0), timestamp(0){ type = PROTOCOL_GETITEMDESC; }
		GetItemDesc(void*) : Protocol(PROTOCOL_GETITEMDESC), sn(0), timestamp(0) { }
		GetItemDesc( INT64 serialNum, INT64 timeStamp,const Octets& data )
			 : sn(serialNum),timestamp(timeStamp),detailInfo(data)
		{
			type = PROTOCOL_GETITEMDESC;
		}

		GetItemDesc(const GetItemDesc &rhs)
			: Protocol(rhs),sn(rhs.sn),timestamp(rhs.timestamp),detailInfo(rhs.detailInfo) { }

		GNET::Protocol *Clone() const { return new GetItemDesc(*this); }

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

		bool SizePolicy(size_t size) const { return size <= 1024; }	

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
	};

};

#endif //__GNET_GET_ITEM_DESC_HPP
