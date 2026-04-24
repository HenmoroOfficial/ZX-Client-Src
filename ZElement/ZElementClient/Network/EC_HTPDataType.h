/*
 * FILE: EC_HTPDataType.h
 *
 * DESCRIPTION: home town command types
 *
 * CREATED BY: 2009/8/19
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */
#include "vector.h"
#include "gnoctets.h"
#pragma once

namespace GNET
{
	class Octets;
}
///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////


//	Check fun game data macro for variable length command structure
#define HT_S2CCMD_FILL_CHECKDATA(s) \
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

namespace HOMETOWN 
{
	enum FARM_TARGET_TYPE						//农场组件中USE_ITEM的目标类型
	{
		FARM_TARGET_SELF = 0,					//自身
		FARM_TARGET_PLAYER	,					//其他玩家
		FARM_TARGET_FARM,						//整个菜园
		FARM_TARGET_FIELD,						//单独一块菜地
	};

	struct crop_info							//庄稼信息
	{
		int id;									//庄稼模板id
		int seed_time;							//播种时间
		int ripe_time;							//还有多少时间成熟,只有在未成熟之前才有效,但都会发
        int protect_period;                     //剩余防偷保护期
		int state;								//状态
		int fruit_left;							//当前剩余果实数 只在成熟状态下才有效,但都会发
		int fruit_total;						//被偷的果实数, 只在成熟或被偷光状态下才有效,但都会发
	};
	struct field_info							//地块信息
	{
		int field_id;
		char field_type;						//0:未开垦, 1:普通地，2：爱心地
		int state;								//地块当前的状态
		int friend_id;							//爱心地播种者角色id,只有爱心地才会发这个和下个字段
		char friend_name_size;
		char friend_name[20];
		struct crop_info crop;					//庄稼信息,只在耕种状态下才会发该字段
	};
	struct farm_player
	{
		int id;
		__int64 charm; 
		__int64 money; 
		int level; 
		int state;								//人当前的状态
	};
	struct farm_info
	{
        int state;                              //整个农场的状态列表
        int protect_period;                     //整个农场的剩余保护时间

		struct farm_player owner;
		abase::vector<struct field_info> fields;
		abase::vector<GNET::Octets> farm_dynamic;
		abase::vector<GNET::Octets> action_dynamic;
	};
	struct item_data
	{
		unsigned int id;
		int pos;
		int count;
		int max_count;
		GNET::Octets content;
	};
	struct pocket_info
	{
		int capacity;					//包裹大小
		abase::vector<item_data> items;	//物品列表
	};

	namespace DYNAMIC					//我的行动和菜园动态结构
	{
		struct dynamic_header
		{
			int time;
			int action;
		};

		//只提示自己
		struct exchg_money
		{
			char type;				//参见hometownprotocol.h
			__int64 local_money;	//兑换的本币数
			__int64 foreign_money;	//兑换的外币数
		};

		//只提示自己
		struct level_up
		{
			int level;				//升级后等级
		};

		//只提示自己
		struct buy_item
		{
			int itemid;
			int count;
			__int64 cost;
		};

		//只提示自己
		struct sell_item
		{
			int itemid;
			int count;
			__int64 earning;
		};

		//对好友使用了物品,提示自己和提示好友
		struct use_item
		{
			int sponsor;				//使用者角色id
			char sponsor_name_size;	
			char sponsor_name[20];
			int target;					//目标角色id
			char target_name_size;	
			char target_name[20];
			int itemid;
			int target_type;
		};

		//在好友的爱心地犁地，提示自己和好友
		struct plow_field
		{
			int sponsor;				//犁地人
			char sponsor_name_size;	
			char sponsor_name[20];
			int target;					//被犁地的人
			char target_name_size;	
			char target_name[20];
			int emotion;
		};

		//在好友的爱心地播种，提示自己和好友
		struct sow_field
		{
			int sponsor;				//播种人
			char sponsor_name_size;	
			char sponsor_name[20];
			int target;					//被播种的人
			char target_name_size;	
			char target_name[20];
			int seed_id;
		};

		//在好友的地里浇水，提示自己和好友
		struct water_field
		{
			int sponsor;				//浇水人
			char sponsor_name_size;	
			char sponsor_name[20];
			int target;					//被浇水的人
			char target_name_size;	
			char target_name[20];
			int emotion;
		};

		//在好友的地里捉虫，提示自己和好友
		struct pest_field
		{
			int sponsor;				//捉虫人
			char sponsor_name_size;	
			char sponsor_name[20];
			int target;					//被捉虫的人
			char target_name_size;	
			char target_name[20];
			int emotion;
		};

		//在好友的地里除草，提示自己和好友
		struct weed_field
		{
			int sponsor;				//除草人
			char sponsor_name_size;	
			char sponsor_name[20];
			int target;					//被除草的人
			char target_name_size;	
			char target_name[20];
			int emotion;
		};

		//收获了自己的土地或好友的爱心地,只出现在我的行动中
		struct harvest_field
		{
			int sponsor;				//收获的人
			char sponsor_name_size;	
			char sponsor_name[20];
			int target;					//被收获的人
			char target_name_size;	
			char target_name[20];
			int fruit_id;
			int count;
		};

		//自己的爱心地或者自己种的爱心地被好友收获，得到果实，只出现在菜园动态中
		struct harvest_lovefield
		{
			int sponsor;
			char sponsor_name_size;	
			char sponsor_name[20];
			int target;
			char target_name_size;	
			char target_name[20];
			int fruit_id;
			int gain_count;				//实际获得的果实数
			int drop_count;				//因包裹满丢失的数目
		};

		//偷好友庄园，提示自己和好友
		struct steal_field
		{
			int sponsor;				//偷窃的人
			char sponsor_name_size;	
			char sponsor_name[20];
			int target;					//被偷窃的人
			char target_name_size;	
			char target_name[20];
			int emotion;
			int fruit_id;
			int count;
		};
	};

	// home town return code 
	enum
	{
		RET_SUCCESS  = 0,

	};
};

namespace HT_S2C
{
	using namespace HOMETOWN;
	enum										//CMD type
	{
		COMPO_LIST = 0          ,               //列举自己已添加的组件,该命令忽略c2s_hometown_cmd_header中的compo_id字段
        COMPO_ADD               ,               //添加组件
        EXCHG_MONEY             ,               //通用币和组件货币互换, compo_id指明了组件货币
        LEVEL_UP                ,               //升级  
        BUY_ITEM                ,               //购买物品
        USE_ITEM                ,               //使用物品
        SELL_ITEM               ,               //卖物品
        PACKAGE_LIST            ,               //包裹查看
        FARM_VIEW			    ,               //查看  
        FARM_VIEW_FIELD         ,               //查看一块地
        FARM_PLOW_FIELD         ,               //犁地  
        FARM_SOW_FIELD          ,               //播种  
        FARM_WATER_FIELD        ,               //浇水  
        FARM_PEST_FIELD         ,               //杀虫  
        FARM_WEED_FIELD         ,               //除草  
        FARM_HARVEST_FIELD      ,               //收获  
        FARM_STEAL_FIELD        ,               //偷窃  
        FARM_LIST_LOVE_FIELD    ,               //列举哪些好友家中有我耕种的爱心地
        FARM_LIST_MATURE_FIELD  ,               //列举哪些好友家中有成熟的地
	};

	struct hometown_s2c_cmd_header
	{
		int retcode;			
		int cmd_type;
		int compo_id;						//组件id
	};

	namespace CMD
	{
		struct compo_list
		{
			int roleid;
			INT64 money;
			int count;
			int compos[1];		//玩家添加的组件ID列表，只在retcode==HT_ERR_SUCCESS才存在
		};

		struct compo_add
		{
			//只在HT_ERR_SUCCESS才存在
			//对于不同的组件有不同的值
			//struct farm_info farm;		添加农场组件后的结果
		};
		struct exchg_money
		{
			char type;						//同C2S
			__int64 money;					//同C2S
			//下面两项只在HT_ERR_SUCCESS才有
			__int64 hometown_money;			//兑换完成后通用币数目
			__int64 compo_money;			//兑换完成后组件币数目
		};
		struct level_up
		{
			//只在HT_ERR_SUCCESS才存在
			//对于不同的组件有不同的值
			//struct farm_info farm;		农场组件升级后的结果
		};
		struct buy_item
		{       
			int itemid;        			//购买物品ID
			int count;          			//购买数量

			//下面部分只有success才存在
			//农场组件中cost1表示消耗的魅力值, cost2表示消耗的金钱
			__int64 cost1;
			__int64 cost2;
		};      
		struct use_item
		{
			int pos;			//使用1个位于包裹栏pos的物品
			int itemid;
			int target_type;	//目标类型
								//对于农场组件如下：
								//FARM_TARGET_SELF = 0,	自身
								//FARM_TARGET_PLAYER,其他玩家
								//FARM_TARGET_FARM,整个菜园
								//FARM_TARGET_FIELD,单独一块菜地
								//FARM_TARGET_CROP,单独一棵庄稼

			//下面的内容根据不同的target_type变化

			int roleid;
			int field_id;
			char consumed;      //0:不消耗，1:消耗

		};
		struct sell_item
		{       
			int pos;            //卖位于包裹栏pos的物品
			int itemid;
			int count;          //卖的数量
			__int64 earning;	//收入，不同组件可能获得不同的收入
								//对于农场组件是农场币
		};  
		struct package_list
		{
			struct pocket_info pktinfo;		//玩家在组件里的包裹信息，只有HT_ERR_SUCCESS才存在
		};
		struct farm_view
		{
			int roleid;
			struct farm_info farm;			//只在HT_ERR_SUCCESS才存在
		};
		struct farm_view_field
		{
			int roleid;
			int field_id;
			//下面只在HT_ERR_SUCCESS才存在
			struct field_info field;
		};
		struct farm_plow_field
		{
			int roleid;
			int field_id;
			int emotion;
		};
		struct farm_sow_field
		{
			int roleid;
			int field_id;
			int seed_id;
			int cost;						//种子耗费的金钱数
		};
		struct farm_water_field
		{
			int roleid;
			int field_id;
			int emotion;
			int charm_change;
		};
		struct farm_pest_field
		{
			int roleid;
			int field_id;
			int emotion;
			int charm_change;
		};
		struct farm_weed_field
		{
			int roleid;
			int field_id;
			int emotion;
			int charm_change;
		};
		struct farm_harvest_field
		{
			int roleid;
			int field_id;
			int charm_add;
			int fruit_id;
			int fruit_count;
		};
		struct farm_steal_field
		{
			int roleid;
			int field_id;
			int emotion;
			int charm_change;
			int fruit_id;
			int fruit_count;
		};
		struct farm_list_love_field
		{
			int count;
			int friends[1];
		};
		struct farm_list_mature_field
		{
			int  count;
			int  friends[1];
		};
	};

}; // end namespace S2C



namespace HT_C2S
{
	using namespace HOMETOWN;
	
	enum  //CMD
	{
		COMPO_LIST = 0			,				//列举自己已添加的组件,该命令忽略c2s_hometown_cmd_header中的compo_id字段
		COMPO_ADD   			,				//添加组件
		EXCHG_MONEY				,				//通用币和组件货币互换, compo_id指明了组件货币
		LEVEL_UP				,				//升级
		BUY_ITEM				,				//购买物品
		USE_ITEM				,				//使用物品
		SELL_ITEM				,				//卖物品
		PACKAGE_LIST			,				//包裹查看
		FARM_VIEW     			,				//查看
        FARM_VIEW_FIELD         ,               //查看一块地
		FARM_PLOW_FIELD 		,				//犁地
		FARM_SOW_FIELD			,				//播种
		FARM_WATER_FIELD		,				//浇水
		FARM_PEST_FIELD			,				//杀虫
		FARM_WEED_FIELD			,				//除草
		FARM_HARVEST_FIELD		,				//收获
		FARM_STEAL_FIELD		,				//偷窃
		FARM_LIST_LOVE_FIELD	,				//列举哪些好友家中有我耕种的爱心地
		FARM_LIST_MATURE_FIELD	,				//列举哪些好友家中有成熟的地
	};

	// 组件id
	enum
	{
		FARM_COMPONENT = 1,

	};

	struct hometown_c2s_cmd_header
	{
		int cmd_type;
		int compo_id;
	};

	namespace CMD
	{
		struct compo_list
		{
			int roleid;
		};
		struct compo_add
		{
			//没有数据
		};
		struct exchg_money
		{
			char type;		//0表示通用币换成组件币，1:相反
			__int64 money;	//兑换的本币数，type==0:通用货币数，type==1:组件币数
		};
		struct level_up
		{
		};
		struct buy_item
		{
			int itemid;		//购买物品ID
			int count;			//购买数量
		};
		struct use_item
		{
			int pos;			//使用1个位于包裹栏pos的物品
			int itemid;
			int target_type;	//目标类型
								//对于农场组件如下：
								//FARM_TARGET_SELF = 0,	自身
								//FARM_TARGET_PLAYER,其他玩家
								//FARM_TARGET_FARM,整个菜园
								//FARM_TARGET_FIELD,单独一块菜地
								//FARM_TARGET_CROP,单独一棵庄稼
			int roleid;
			int field_id;
		};
		struct sell_item
		{
			int pos;			//卖位于包裹栏pos的物品
			int itemid;
			int count;			//卖的数量
		};
		struct package_list
		{
		};
		struct farm_view
		{
			int roleid;
		};
		struct farm_view_field              
		{
			int roleid;
			int field_id;
		};
		struct farm_plow_field
		{
			int roleid;
			int field_id;
			int emotion;
		};
		struct farm_sow_field
		{
			int roleid;
			int field_id;
			int seed_id;
		};
		struct farm_water_field
		{
			int roleid;
			int field_id;
			int emotion;
		};
		struct farm_pest_field
		{
			int roleid;
			int field_id;
			int emotion;
		};
		struct farm_weed_field
		{
			int roleid;
			int field_id;
			int emotion;
		};
		struct farm_harvest_field
		{
			int roleid;
			int field_id;
		};
		struct farm_steal_field
		{
			int roleid;
			int field_id;
			int emotion;
		};
		struct farm_list_love_field
		{
		};
		struct farm_list_mature_field
		{
		};
	};
	
};		// end namespace C2S

#pragma pack()  


