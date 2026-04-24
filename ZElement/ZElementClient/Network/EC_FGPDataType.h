/*
 * FILE: EC_FGPDataType.h
 *
 * DESCRIPTION: Fun game command types
 *
 * CREATED BY: Wangrui, 2009/3/11
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once


///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////


//	Check fun game data macro for variable length command structure
#define FG_S2CCMD_FILL_CHECKDATA(s) \
	static bool CheckValid(const void* pData, int iLen) \
	{ \
		s data; \
		return data.FillData(pData, iLen); \
	} \
	bool FillData(const void* pData, int iBufLen, int* piRealLen=NULL); \


///////////////////////////////////////////////////////////////////////////
//	
//	Reference to External variables and functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Local Types and Variables and Global variables
//	
///////////////////////////////////////////////////////////////////////////

#pragma pack(1)

namespace FG_S2C
{
	struct fgs2c_cmd_header 
	{
		unsigned char game_type;
		unsigned char cmd;
	};

	namespace INFO
	{
		struct player_base_info
		{
			int roleid;
			char direction;
			char state;
			char gender;
			INT64 score;
			int win_count;
			int lose_count;
			int draw_count;
			int escape_count;
			char name_size;
			char* name;			//	最多不超过20字节

			FG_S2CCMD_FILL_CHECKDATA(player_base_info)
		};

		struct room_info
		{
			short roomid;
			char player_size;
			player_base_info* players;

			FG_S2CCMD_FILL_CHECKDATA(room_info)
			~room_info() { if (players) delete [] players; }
		};

		struct player_state
		{
			int roleid;
			char direction;
			char state;
		};

		struct normal_card_info
		{
			char card_size;
			char* cards;

			FG_S2CCMD_FILL_CHECKDATA(normal_card_info)
			~normal_card_info() { if (cards) delete [] cards; }
		};

		struct landlord_result_info
		{
			int roleid;	
			int score;
		};
		struct landlord_card_info
		{
			char direct;	
			char card_size;
		};

		struct landlord_reonline_info
		{
			room_info cur_room_info;
			int score;
			char lord_dir;					// 地主位置
			char desk_dir;					// 桌面牌所有者
			char next_dir;					// 当前出牌人 
			landlord_card_info cards[2];	// 其他玩家手中牌信息
			char self_card_size;			// 手中牌数目
			char* self_cards;				// 手中牌
			char desk_card_size;			// 桌面牌数目
			char* desk_cards;				// 桌面牌

			FG_S2CCMD_FILL_CHECKDATA(landlord_reonline_info)
			~landlord_reonline_info() { if (self_cards) delete [] self_cards; if (desk_cards) delete [] desk_cards;	 }
		};

	}; 

	enum  //CMD
	{
		GET_ROOM_LIST = 0,				// 玩家获取房间列表
		NOTIFY_ROOM_INFO,				// 通知房间信息
		PLAYER_INFO,					// 通知玩家信息
		PLAYER_ENTER_ROOM,				// 玩家进入房间
		PLAYER_LEAVE_ROOM, 				// 玩家离开房间
		
		PLAYER_STATE_CHANGE = 5,		// 玩家状态变化通知 
		PLAYER_REENTER_INVITE,			// 玩家断线后 邀请玩家继续游戏 
		ROOM_GAME_IS_READY, 			// 游戏人数已够， 等待玩家开始
		ROOM_DISPATCH_NORMAL_CARD,		// 游戏开始 发牌
		PLAYER_PICKUP_NORMAL_CARD,		// 玩家拿底牌 
	
		PLAYER_INTURN_NORMAL_CARD = 10,	// 轮到由该玩家出牌 
		PLAYER_THROW_NORMAL_CARD,		// 玩家出牌
		PLAYER_GIVEUP_NORMAL_CARD,		// 玩家跳过出牌
		PLAYER_AUCTION,					// 玩家叫牌
		ROOM_GAME_END,					// 房间内游戏结束
		
		LANDLORD_RESULT_BROADCAST = 15,	// 广播游戏结果
		LANDLORD_REONLINE_DATASEND,		// 重上线后发送当前数据
		PLAYER_ENTER_HALL,				// 玩家进入大厅
		PLAYER_LEAVE_HALL,				// 玩家离开大厅
		NOTIFY_HALL_MEMBERS,			// 提示大厅中玩家新进入玩家信息

	};

	namespace CMD
	{
		using namespace INFO;

		struct fgcmd_get_room_list 
		{
			char room_size;
			room_info* data;

			FG_S2CCMD_FILL_CHECKDATA(fgcmd_get_room_list)
			~fgcmd_get_room_list() { if (data) delete [] data; }
		};

		struct fgcmd_notify_room_info
		{
			room_info data;

			FG_S2CCMD_FILL_CHECKDATA(fgcmd_notify_room_info)
		};

		struct fgcmd_player_info
		{
			player_base_info data;
			
			FG_S2CCMD_FILL_CHECKDATA(fgcmd_player_info)
		};

		struct fgcmd_player_enter_room
		{
			short roomid;
			player_base_info data;

			FG_S2CCMD_FILL_CHECKDATA(fgcmd_player_enter_room)
		};

		struct fgcmd_player_leave_room
		{
			int roleid;
			short roomid;
		};

		struct fgcmd_player_state_change
		{
			short roomid;
			player_state data;
		};

		struct fgcmd_room_game_is_ready
		{
			short roomid;
		};

		struct player_reenter_invite
		{
			short roomid;
		};

		struct fgcmd_room_dispatch_normal_card
		{
			short roomid;
			int roleid;
			int next_roleid;
			normal_card_info cards;

			FG_S2CCMD_FILL_CHECKDATA(fgcmd_room_dispatch_normal_card)
		};

		struct fgcmd_player_inturn_normal_card 
		{
			short roomid;
			int roleid;
		};

		struct fgcmd_player_throw_normal_card 
		{
			short roomid;
			int roleid;
			int next_roleid;
			normal_card_info cards;

			FG_S2CCMD_FILL_CHECKDATA(fgcmd_player_throw_normal_card)
		};

		struct fgcmd_player_pickup_normal_card 
		{
			short roomid;
			int roleid;
			normal_card_info cards;

			FG_S2CCMD_FILL_CHECKDATA(fgcmd_player_pickup_normal_card)
		};

		struct fgcmd_player_giveup_normal_card
		{
			short roomid;
			int roleid;
			int next_roleid;			// 下一个出牌人 
		};
		
		struct fgcmd_player_auction
		{
			short roomid;
			int roleid;
			int next_roleid;
			char score;					// 分值
		};

		struct fgcmd_room_game_end
		{
			short roomid;
			int roleid;
			char reason;				//结束原因
		};

		struct fgcmd_landlord_result_broadcast
		{
			short roomid;
			char size;
			landlord_result_info* results;

			FG_S2CCMD_FILL_CHECKDATA(fgcmd_landlord_result_broadcast)
		};

		struct fgcmd_landlord_reonline_datasend
		{
			short roomid;
			landlord_reonline_info info;

			FG_S2CCMD_FILL_CHECKDATA(fgcmd_landlord_reonline_datasend)
		};

		struct fgcmd_player_enter_hall
		{
			player_base_info data;
			FG_S2CCMD_FILL_CHECKDATA(fgcmd_player_enter_hall)
		};
		
		struct fgcmd_player_leave_hall
		{
			int roleid;
		};
		
		struct fgcmd_notify_hall_members
		{
			int member_num;
			player_base_info *players;

			FG_S2CCMD_FILL_CHECKDATA(fgcmd_notify_hall_members)
		};

	};

}; // end namespace S2C



namespace FG_C2S
{
	struct fgc2s_cmd_header
	{
		unsigned char game_type;
		unsigned char cmd_type;
	};

	namespace INFO
	{
		struct normal_card_info
		{
			char card_size;
			char card[1];
		};
	}; 

	enum  //CMD
	{
		GET_ROOM_LIST = 0,				// 玩家获取房间列表
		PLAYER_ENTER_ROOM,				// 玩家进入房间 
		PLAYER_LEAVE_ROOM, 				// 玩家离开房间
		PLAYER_BEGIN_GAME,				// 玩家开始游戏
		PLAYER_REENTER_ROOM,			// 玩家断线后 重新进入房间
		
		PLAYER_PICKUP_NORMAL_CARD = 5,	// 玩家拿底牌
		PLAYER_THROW_NORMAL_CARD,		// 玩家出牌
		PLAYER_GIVEUP_NORMAL_CARD,		// 玩家跳过出牌
		PLAYER_AUCTION,					// 叫牌
		PLAYER_ENTER_HALL,

		PLAYER_LEAVE_HALL = 10,

	};

	namespace CMD
	{
		using namespace INFO;

		struct fgcmd_get_room_list 
		{
			char index;
		};

		struct fgcmd_player_enter_room
		{
			short roomid;
			char direction;
		};

		struct fgcmd_player_begin_game
		{
			short roomid;
		};

		struct fgcmd_player_reenter_room
		{
			short roomid;
		};

		struct fgcmd_player_leave_room
		{
			short roomid;
		};

		struct fgcmd_player_throw_normal_card
		{
			short roomid;
			normal_card_info cards;
		};

		struct fgcmd_player_pickup_normal_card
		{
			short roomid;
		};

		struct fgcmd_player_giveup_normal_card
		{
			short roomid;
		};

		struct fgcmd_player_auction
		{
			short roomid;
			int roleid;
			char score;					// 分值
		};

		struct player_enter_hall
		{
		};

		struct player_leave_hall
		{
		};
	};

};		// end namespace C2S

#pragma pack()  


