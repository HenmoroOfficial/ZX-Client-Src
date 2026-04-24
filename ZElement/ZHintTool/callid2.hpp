#ifndef __GNET_CLIENT_CALLID2__
#define __GNET_CLIENT_CALLID2__

namespace GNET
{
	enum ProtocolType2
	{
		PROTOCOL_GETITEMDESC			=	8050,	//寻宝网小工具协议
		PROTOCOL_GETITEMDESC_RE			=	8051,	//寻宝网小工具协议
		PROTOCOL_GET_ROLE_DESC			=	8062,	//寻宝网小工具协议（角色交易）
		PROTOCOL_GET_ROLE_DESC_RE		=	8063,	//寻宝网小工具协议（角色交易）
		PROTOCOL_GET_ROLE_DESC_SNS		=	8064,	//主题站角色展示
		PROTOCOL_GET_ROLE_DESC_SNS_RE	=	8065,	//主题站角色展示
		PROTOCOL_NEWKEEPALIVE			=	8036,	//小工具公用心跳协议
		PROTOCOL_GETITEMDESC_PLTFM		=	8052,	//系统平台小工具协议
		PROTOCOL_GETITEMDESC_PLTFM_RE	=	8053,	//系统平台小工具协议
		PROTOCOL_TOOL_ANNOUNCE_AID		=	8090,	//SNS平台小工具id通知协议
		PROTOCOL_GET_ROLE_EQUIP			=	98091,	//SNS平台向小工具获得角色装备协议
		PROTOCOL_GET_ROLE_EQUIP_RE		=	98092,	//SNS平台，小工具返回角色装备协议
		PROTOCOL_GET_SNS_ROLE_PET		=	98093,	//SNS平台向小工具获得角色宠物协议
		PROTOCOL_GET_SNS_ROLE_PET_RE	=	98094,	//SNS平台，小工具返回角色宠物协议
	};
};

#endif //__GNET_CLIENT_CALLID2__
