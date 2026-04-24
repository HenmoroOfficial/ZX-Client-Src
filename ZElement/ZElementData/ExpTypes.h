#ifndef _EXP_TYPES_H_
#define _EXP_TYPES_H_

#include <stdio.h>
#include <stdlib.h>

#ifdef _WINDOWS
#include <memory.h>
#endif

#define ELEMENTDATA_VERSION			0x1000009c

#define SPECIAL_ID_CONFIG_TEMPLID	7	// 689

#ifdef LINUX
#define UInt64  unsigned long long
#else
#define UInt64  unsigned __int64
#endif

enum SERVICE_TYPE
{
	// 交谈服务
	NPC_TALK = 0x80000000,
	// 出售商品的服务
	NPC_SELL,
	// 收购商品的服务
	NPC_BUY,
	// 修理商品的服务
	NPC_REPAIR,
	// 镶嵌服务
	NPC_INSTALL,
	// 拆除服务
	NPC_UNINSTALL,
	// 任务相关服务,分发任务和完成任务以及发放任务物品
	NPC_GIVE_TASK,
	NPC_COMPLETE_TASK,
	NPC_GIVE_TASK_MATTER,
	// 教授相关服务
	NPC_SKILL,
	// 治疗服务
	NPC_HEAL,
	// 传送服务
	NPC_TRANSMIT,
	// 运输服务
	NPC_TRANSPORT,
	// 代售服务
	NPC_PROXY,
	// 存储物品、金钱
	NPC_STORAGE,
	// 生产服务
	NPC_MAKE,
	// 分解服务
	NPC_DECOMPOSE,
	// TALK返回
	TALK_RETURN,
	// 结束对话
	TALK_EXIT,
	// 仓库密码
	NPC_STORAGE_PASSWORD,
	// 鉴定服务
	NPC_IDENTIFY,
	// 放弃任务
	TALK_GIVEUP_TASK,
	// 城战炮塔建造服务
	NPC_WAR_TOWERBUILD,
	// 洗点服务
	NPC_RESETPROP,
	// 装备绑定服务
	NPC_EQUIPBIND,
	// 装备销毁服务
	NPC_EQUIPDESTROY,
	// 装备解除销毁服务
	NPC_EQUIPUNDESTROY,
	// 城战购买弓箭手服务
	NPC_WAR_BUY_ARCHER,
	// 物物交易
	ITEM_TRADE_SERVICE,
	// 器魄融合
	EQUIP_SOUL_MELD,
	// 商品寄售服务
	NPC_CONSIGN,
	// 副本服务
	NPC_TRANSCRIPTION,
};

/* 属性index
*/
enum PlayerPropType
{	
	PROP_TYPE_DAMAGE,
	PROP_TYPE_DEF,
	PROP_TYPE_HP,
	PROP_TYPE_MP,
	PROP_TYPE_ATTACK,			// 命中
	PROP_TYPE_ARMOR,			// 躲闪
	PROP_TYPE_CRIT_RATE,		// 暴击 
	PROP_TYPE_CRIT_ANTI,		// 减暴击
	PROP_TYPE_CRIT_DAMAGE,		// 暴伤
	PROP_TYPE_CRIT_DAMAGE_ANTI,	// 减暴伤
	PROP_TYPE_SKILL_ATTACK_RATE,// 技能命中
	PROP_TYPE_SKILL_ARMOR_RATE,	// 技能躲闪
	PROP_TYPE_RESISTANCE_1,		// 抗性
	PROP_TYPE_RESISTANCE_2,
	PROP_TYPE_RESISTANCE_3,
	PROP_TYPE_RESISTANCE_4,
	PROP_TYPE_RESISTANCE_5,
	PROP_TYPE_CULT_DEFANCE_1,
	PROP_TYPE_CULT_DEFANCE_2,	
	PROP_TYPE_CULT_DEFANCE_3,
	PROP_TYPE_CULT_ATTACK_1,
	PROP_TYPE_CULT_ATTACK_2,
	PROP_TYPE_CULT_ATTACK_3,
	PROP_TYPE_SKILLED_1,		// 精通
	PROP_TYPE_SKILLED_2,
	PROP_TYPE_SKILLED_3,
	PROP_TYPE_SKILLED_4,
	PROP_TYPE_SKILLED_5,
	PROP_TYPE_TOUGHNESS_1,		// 韧性
	PROP_TYPE_TOUGHNESS_2,
	PROP_TYPE_TOUGHNESS_3,
	PROP_TYPE_TOUGHNESS_4,
	PROP_TYPE_TOUGHNESS_5,
	PROP_TYPE_MOVE_SPEED,		// 地面移动速度
	PROP_TYPE_FLY_SPEED,		// 飞行速度
	PROP_TYPE_NUM = 40,
};

#pragma pack(push, EXP_TYPES_INC, 4)

#ifdef _ANGELICA22
typedef wchar_t					namechar;
#else
typedef unsigned short			namechar;
#endif // #ifdef _ANGELICA22

///////////////////////////////////////////////////////////////////////////////////////
//
// 装备的模板中要使用的附加属性的数据结构定义
//
//
//
///////////////////////////////////////////////////////////////////////////////////////
struct EQUIPMENT_ADDON
{
	unsigned int	id;							// 附加属性的ID，在生成物品时要求第14、13位表示本
												// 附加属性的参数个数，第12位不能占用
												// 所以我们在产生ID时，不能使用12，13，14这三位

	namechar		name[32];					// 附加属性的名字

	int				type;						// 附加属性的类型

	int				num_params;					// 本附加属性的参数个数
	int				param1;						// 第1个参数，可以是浮点数
	int				param2;						// 第2个参数，可以是浮点数
	int				param3;						// 第3个参数，可以是浮点数

};


///////////////////////////////////////////////////////////////////////////////////////
//
// 装备模板数据结构定义
//
//
///////////////////////////////////////////////////////////////////////////////////////

// 装备大类别
struct EQUIPMENT_MAJOR_TYPE
{
	unsigned int	id;							// 装备大类别ID
	namechar		name[32];					// 装备大类别名称
};

// 装备小类别
struct EQUIPMENT_SUB_TYPE
{
	unsigned int	id;							// 装备小类别ID
	namechar		name[32];					// 装备小类别名称
};

// 装备本体数据
struct EQUIPMENT_ESSENCE
{
	// 基本信息
	unsigned int	id;							// 装备(类型)ID
	unsigned int	id_major_type;				// 装备大类别ID
	unsigned int	id_sub_type;				// 装备小类别ID
	namechar		name[32];					// 装备名称, 最多15个汉字

	unsigned int	equip_type;					// 装备类别，可能的有 0-武器，1-防具，2-饰品，3-时装，4-翅膀，5-法宝辅件
	unsigned int	equip_mask;					// 逻辑上可以装备的部位，由不同部位Mask组成，使用的部位代码为：
												// EQUIP_MASK_WEAPON         = 0x0001,
												// EQUIP_MASK_HEAD           = 0x0002,
												// EQUIP_MASK_BODY           = 0x0004,
												// EQUIP_MASK_FOOT           = 0x0008,
												// EQUIP_MASK_NECK           = 0x0010,
												// EQUIP_MASK_FINGER1        = 0x0020,
												// EQUIP_MASK_FINGER2        = 0x0040,
												// EQUIP_MASK_FASHION_EYE    = 0x0080,
												// EQUIP_MASK_FASHION_NOSE   = 0x0100,
												// EQUIP_MASK_FASHION_LIP    = 0x0200,
												// EQUIP_MASK_FASHION_HEAD   = 0x0400,
												// EQUIP_MASK_FASHION_BODY   = 0x0800,
												// EQUIP_MASK_FASHION_LEG    = 0x1000,
												// EQUIP_MASK_FASHION_FOOT   = 0x2000,
												// EQUIP_MASK_FASHION_BACK   = 0x4000,
												// EQUIP_MASK_WING			 = 0x8000,
												// EQUIP_MASK_MAIN_TALISMAN	 = 0x10000,
												// EQUIP_MASK_SUB_TALISMAN1	 = 0x20000,
												// EQUIP_MASK_SUB_TALISMAN2	 = 0x40000,
												
	
	int				file_model_male;			// 男模型路径
	int				file_model_female;			// 女模型路径
	
	int				file_model_male_armor_xuanyuan;	//轩辕男帽子模型路径，暗系， pathid 占位用，Added 2012-07-24.
	int				file_model_female_armor_xuanyuan;//轩辕 女帽子模型路径，暗系，pathid 占位用，Added 2012-07-24.
	int				file_model_big_male;		// 大体型男模型路径
	int				file_model_big_female;		// 大体型女模型路径
	int				file_model_small_male;		// 小体型男模型路径
	int				file_model_small_female;	// 小体型女模型路径
	int				file_model_tiny_male;		// 微体型男模型路径
	int				file_model_tiny_female;		// 微体型女模型路径
	int				file_model_tetrapod_male;	// 四足体型男模型路径
	int				file_model_big_male_left;	// 大体型男左手边模型路径 主要用于太昊双手不同武器Added 2011-08-09.
	int				file_model_big_female_left;	// 大体型女左手边模型路径 主要用于太昊双手不同武器Added 2011-08-09.
	int				file_model_back_male;		// 背部男模型路径名 主要用于太昊/焚香背部模型效果Added 2011-08-09.
	int				file_model_back_female;		// 背部女模型路径名 主要用于太昊/焚香背部模型效果Added 2011-08-09.
	int				file_model_female_taihao;	// 太昊女时装路径名 专门用于太昊女时装的模型路径 Added 2011-08-09.
	
	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径



	unsigned int	equip_location;				// 模型覆盖部位，可能的有：0-不显示，1-头部，2-上衣，3-下衣，4-上衣带下衣，
												// 5-鞋子，6-眼镜，7-鼻子，8-胡子，9-背饰，10-翅膀
	unsigned int	action_type;				// 武器的动作播放类型，可能的有: 0-空手, 1-短兵, 2-短法, 3-匕首, 4-长兵, 5-长法

	namechar		show_level[16];				// 装备品级描述
	int				level;						// 装备等级

	UInt64			character_combo_id;			// 职业限制, 从低位到高位的含义为：0-新手, 1-术士, 2-武士, 3-战士, 4-剑士, 5-牧师, 6-法师, 7-将军, 8-重骑士, 9-刺客, 10-剑圣, 11-贤者, 12-祭司, 13-秘法师, 14-召唤使
	UInt64			character_combo_id2;		// 职业限制，扩展职业ID后新增加的职业，Added 2011-07-14
	UInt64			char_lev_1;
	UInt64			char_lev_1_2;				// 转职业时的职业限制，扩展职业ID后的新职业限制，Added 2011-07-15. 
	UInt64			char_lev_2;
	UInt64			char_lev_2_2;				// 转职业时的职业限制，扩展职业ID后的新职业限制，Added 2011-07-15. 
	UInt64			char_lev_3;
	UInt64			char_lev_3_2;				// 转职业时的职业限制，扩展职业ID后的新职业限制，Added 2011-07-15.
	unsigned int	require_gender;				// 性别限制，0-男，1-女，2-男女都可
	int				require_level;				// 等级限制
	unsigned int	sect_mask;					// 门派要求
	unsigned int	sect_mask_1;				// 门派要求
	unsigned int	sect_mask_2;				// 门派要求
	unsigned int	sect_mask_3;				// 门派要求
	int				require_race;				// 种族限制，0-人族，1-兽族， 2-都可以
	int				require_level2;				// 修真等级限制
	int				renascence_count;			// 转生次数要求
	unsigned int	god_devil_mask;				// 仙魔
	unsigned int	nation_position_mask;		// 职位要求
												// 0x0001 国王
												// 0x0002 王后
												// 0x0004 将军
												// 0x0008 大臣
												// 0x0010 帮众
												// 0x0020 亲卫
	int				title_require;				// 称号

	struct
	{
		int index;
		int num;
	} require_repu[2];

	
	/*
	0   默认值，表示没有社稷需求
	10	初阵赤子
	20	双全精卒
	40	三山狂士
	70	四海游侠
	100	五陵豪杰
	130	六合飞卫
	160	七贤名家
	190	八荒勇者
	230	九州英雄
	270	十方霸主
	310	百胜骁将
	350	千军王者
	390	万里人皇
	760	天下至尊
	*/
		
	int				require_territory;			// 社稷需求
	// 本体属性
	int				hp;							// 生命
	int				mp;							// 魔力
	int				dp;							// 神力
	int				deity_power;				// 神圣力
	int				min_dmg;					// 最小攻击力
	int				max_dmg;					// 最大攻击力
	int				defence;					// 防御力
	int				attack;						// 命中
	int				armor;						// 闪躲
	
	float			attack_range;				// 攻击距离

	// 价钱
	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	int				id_addon1;					// 第1种附加属性的类型ID
	int				id_addon2;					// 第2种附加属性的类型ID
	int				id_addon3;					// 第3种附加属性的类型ID
	int				id_addon4;					// 第4种附加属性的类型ID
	int				id_addon5;					// 第5种附加属性的类型ID

	int				fee_estone;					// 单次强化手续费
	int				fee_install_pstone;			// 单次追加手续费
	int				fee_uninstall_pstone;		// 单次拆卸追加石手续费
	int				fee_install_sstone;			// 附加技能手续费
	int				fee_uninstall_sstone;		// 单次拆卸技能石手续费

	unsigned int	id_estone;					// 可用的强化石
	int				ehanced_value[20];			// 1~20次强化后的数值

	struct 
	{
		int			level;
		
		struct
		{
			int		id;
			float	probability;
		} prop[5];

	} hidden_prop[2];

	int			blood_sacrifice;			// 是否允许血祭 0表示不允许
	int			can_punching;				// 是否允许打孔 0表示不允许
	int			color;						// 是否允许染色 0表示不允许

	int				soul_level;					// 灵性等级 
	//  精炼附加属性
	struct  
	{
		int level;		// 精练等级
		int addon;		// 附加属性
	}hidden_addon[3];

	int				gem_slot_identify;		//该饰品是否能进行宝石插槽鉴定， 0表示不允许，编辑器的缘故而使用int

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

/////////////////////////////////////////////////////////////////////////////////////////////////
// 法宝主件本体
/////////////////////////////////////////////////////////////////////////////////////////////////
struct TALISMAN_MAINPART_ESSENCE
{
	unsigned int	id;							// 法宝主件ID
	namechar		name[32];					// 法宝主件名称, 最多15个汉字

	int				file_model[7];				// 法宝主件模型路径1 - 4
	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径
	int				color_change;				// 颜色是否随机变化：只针对飞剑 0：无颜色变化， 1：颜色随机变化

	// 法宝主件只能装备在法宝主件位置上，无需指定

	namechar		show_level[16];				// 法宝主件品级描述
	int				level;						// 法宝主件等级

	int				init_level;					// 初始等级

	UInt64			character_combo_id;			// 职业限制, 从低位到高位的含义为：0-新手, 1-术士, 2-武士, 3-战士, 4-剑士, 5-牧师, 6-法师, 7-将军, 8-重骑士, 9-刺客, 10-剑圣, 11-贤者, 12-祭司, 13-秘法师, 14-召唤使
	UInt64			character_combo_id2;		// 职业限制，扩展职业ID后的新职业限制，Added 2011-07-14. 
	UInt64			char_lev_1;
	UInt64			char_lev_1_2;				// 转职业时的职业限制，扩展职业ID后的新职业限制，Added 2011-07-14. 
	UInt64			char_lev_2;
	UInt64			char_lev_2_2;				// 转职业时的职业限制，扩展职业ID后的新职业限制，Added 2011-07-14.
	UInt64			char_lev_3;
	UInt64			char_lev_3_2;				// 转职业时的职业限制，扩展职业ID后的新职业限制，Added 2011-07-14.
	unsigned int	require_gender;				// 性别限制，0-男，1-女，2-男女都可
	int				require_level;				// 等级限制
	unsigned int	sect_mask;					// 门派要求
	unsigned int	sect_mask_1;				// 门派要求
	unsigned int	sect_mask_2;				// 门派要求
	unsigned int	sect_mask_3;				// 门派要求
	int				require_race;				// 种族限制，0-人族，1-兽族， 2-都可以
	int				require_level2;				// 修真等级限制
	int				renascence_count;			// 转生次数要求
	unsigned int	god_devil_mask;				// 仙魔
	int				title_require;				// 称号

	int				max_level;					// 最大成长等级
	int				max_level2;					// 最大成长等级2

	float			energy_recover_speed;		// 法宝精力增长速度
	float			energy_recover_factor;		// 非战斗状态法宝精力增长速度衰减系数
	float			energy_drop_speed;			// 法宝精力下降速度

	int				fee_levelup;				// 升级手续费
	int				fee_expfood;				// 血炼手续费
	int				fee_reset;					// 重置手续费

	unsigned int	is_aircraft;
	float			fly_speed;					// 飞行速度
	int				fly_mode;					// 飞行模式
	float			fly_energy_drop_speed;
	int				fly_exp_added;				// 每30秒增加飞剑经验值

	// 价钱
	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

/////////////////////////////////////////////////////////////////////////////////////////////////
// 法宝血炼材料
/////////////////////////////////////////////////////////////////////////////////////////////////
struct TALISMAN_EXPFOOD_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径

	int				exp_added;					// 增加的经验值
	int				food_type;					// 适用的法宝类型

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

// 法宝精力值增长道具
struct TALISMAN_ENERGYFOOD_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径名

	int				energy_total;				// 总的精力值
	int				energy_used_minimum;		// 精力使用最小值
	
	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 该药品的堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

/////////////////////////////////////////////////////////////////////////////////////////////////
// 法宝熔炼剂
/////////////////////////////////////////////////////////////////////////////////////////////////
struct TALISMAN_MERGEKATALYST_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

/////////////////////////////////////////////////////////////////////////////////////////////////
// 套装，共客户端显示时查询数据用
/////////////////////////////////////////////////////////////////////////////////////////////////

// 套装本体
struct SUITE_ESSENCE
{
	// 基本信息
	unsigned int	id;							// 套装(类型)ID
	namechar		name[32];					// 套装名称, 最多15个汉字

	int				max_equips;					// 套装件数

	struct 
	{
		unsigned int	id;						// 套装组件1~14
	} equipments[14];

	struct 
	{
		unsigned int	id;						// 2件至13件能激活的附加属性
	} addons[13];

	char			file_gfx[128];				// 满套后的效果
	int				hh_type;					// 挂点
	int				equip_soul_suite;			// 是否器魄套装，0为默认值：不是
};

///////////////////////////////////////////////////////////////////////////////////////
//
// 各种物品的模板
//
//		注意：在这里,物品是个狭义的物品概念,包括普通物品,任务物品以及特殊物品三大类
//
//
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// 精炼概率调整道具
///////////////////////////////////////////////////////////////////////////////////////
struct REFINE_TICKET_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径名

	float			ext_reserved_prob;			// 提高保留概率
	float			ext_succeed_prob;			// 提高成功概率

	unsigned int	new_mode;					// 是否新模式
	int				refine_level;				// 适用精练等级
	unsigned int	bind_equip;					// 炼器后绑定装备，默认为0
	float			new_succeed_prob[20];		// 精练成功率

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

/*
	离线托管道具
*/
struct OFFLINE_TRUSTEE_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径名

	int				max_trustee_time;			// 最大托管时间

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

///////////////////////////////////////////////////////////////////////////////////////
// 彩票道具
///////////////////////////////////////////////////////////////////////////////////////
struct LOTTERY_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径名

	int				dice_count;					// 可以抽取的次数(1~6)
	int				type;						// 彩票类型

	struct
	{
		namechar		desc[16];				// 代表文字, 最多16个汉字
		int				icon;					// 图标路径名

	} candidates[32];							// 备选图案
	
	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};


///////////////////////////////////////////////////////////////////////////////////////
// 彩票道具(可堆叠)
///////////////////////////////////////////////////////////////////////////////////////
struct LOTTERY2_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径名

	int				dice_count;					// 可以抽取的次数(1~6)
	int				type;						// 彩票类型

	struct
	{
		namechar		desc[16];				// 代表文字, 最多16个汉字
		int				icon;					// 图标路径名

	} candidates[32];							// 备选图案
	
	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

///////////////////////////////////////////////////////////////////////////////////////
// 彩票道具 (使用物品id)
///////////////////////////////////////////////////////////////////////////////////////
//Added 2011-07-01
struct LOTTERY3_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径名

	int				award_item;					// 奖励的物品id，0表示没有，2012-12-18

	int				dice_count;					// 可以抽取的次数(1~6)
	int				type;						// 彩票类型
	
	struct
	{
		int			item_id;					// 物品id
		int			item_count;					// 该物品的数量
	}candidate_items[32];

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID

};

///////////////////////////////////////////////////////////////////////////////////////
// 录像道具
///////////////////////////////////////////////////////////////////////////////////////
struct CAMRECORDER_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径名

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};


///////////////////////////////////////////////////////////////////////////////////////
//
// 药品模板数据结构定义
//
//
///////////////////////////////////////////////////////////////////////////////////////
// 药品大类别
struct MEDICINE_MAJOR_TYPE
{
	unsigned int	id;							// 药品大类别ID
	namechar		name[32];					// 药品大类别名称
};

// 药品小类别
struct MEDICINE_SUB_TYPE
{
	unsigned int	id;							// 药品小类别ID
	namechar		name[32];					// 药品小类别名称
};

// 药品本体属性
struct MEDICINE_ESSENCE
{
	unsigned int	id;							// 药品(类型)ID
	unsigned int	id_major_type;				// 药品大类别ID
	unsigned int	id_sub_type;				// 药品小类别ID
	namechar		name[32];					// 药品名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径名

	int				require_level;				// 玩家的级别要求
	int				renascence_count;			// 转生次数要求
	int				cool_time;					// 冷却时间（毫秒）

	unsigned int	only_in_war;				// 是否城战专用

	int				type;						// 药物类别，六种：0-瞬回HP，1-瞬回MP，2-瞬回HP及MP，
												// 3-持续回HP，4-持续回MP，5-持续回HP及MP，6-瞬回DP，7-持续回DP

	int				hp[4];						// HP，瞬回的表示上限，非瞬回的表示连续四次的分别补给量
	int				mp[4];						// MP，瞬回的表示上限，非瞬回的表示连续四次的分别补给量
	int				dp[4];						// DP, 瞬回的表示上限，非瞬回的表示连续四次的分别补给量

	int				map_restrict[5];			// 可用地图
	
	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 该药品的堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

///////////////////////////////////////////////////////////////////////////////////////
//
// 生产原料模板数据结构定义
//
//
///////////////////////////////////////////////////////////////////////////////////////
// 生产原料大类别
struct MATERIAL_MAJOR_TYPE
{
	unsigned int	id;							// 生产原料大类别ID
	namechar		name[32];					// 生产原料大类别名称
};

// 生产原料小类别
struct MATERIAL_SUB_TYPE
{
	unsigned int	id;							// 生产原料小类别ID
	namechar		name[32];					// 生产原料小类别名称
};

// 生产原料本体属性
struct MATERIAL_ESSENCE
{
	unsigned int	id;							// 生产原料(类型)ID
	unsigned int	id_major_type;				// 生产原料大类别ID
	unsigned int	id_sub_type;				// 生产原料小类别ID
	namechar		name[32];					// 生产原料名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 拆分信息
	int				decompose_price;			// 拆分价格
	int				decompose_time;				// 拆分时间
	unsigned int	element_id;					// 拆分成的元石的类型ID
	int				element_num;				// 拆分产生的元石数

	// 堆叠信息
	int				pile_num_max;				// 该原料的堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

///////////////////////////////////////////////////////////////////////////////////////
// 正在销毁中的物品
///////////////////////////////////////////////////////////////////////////////////////
struct DESTROYING_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

///////////////////////////////////////////////////////////////////////////////////////
//
// 技能书模板数据结构定义
//
//		在这里,我们把"书"参照Diabllo中的使用方法,译为"tome",意为册, 卷, 本, 大本书
//
//
///////////////////////////////////////////////////////////////////////////////////////
// 技能书小类别
struct SKILLTOME_SUB_TYPE
{
	unsigned int	id;							// 技能书小类别ID
	namechar		name[32];					// 技能书小类别名称
};
		   
// 技能书本体属性
struct SKILLTOME_ESSENCE
{
	unsigned int	id;							// (类型)ID
	unsigned int	id_sub_type;				// 技能书小类别ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径名
	int				file_icon;					// 图标路径名

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

// 技能物品本体属性
struct SKILLMATTER_ESSENCE
{
	unsigned int	id;							// (类型)ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径名
	int				file_icon;					// 图标路径名

	int				level_required;				// 等级限制
	int				renascence_count;			// 转生次数要求

	int				id_skill;					// 技能ID
	int				level_skill;				// 技能等级

	unsigned int	only_in_war;				// 是否城战专用

	int				cool_type;					// 冷却时间的类别

	unsigned int	permenent;					// 使用后不消失，false-消失，true-不消失
	unsigned int	cast_skill;					// 0只能自身使用，1可对非自身目标使用
	unsigned int	god_devil_mask;				// 仙魔 造化限制仙/佛/魔/备选1/2/3

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

// 交通工具
struct VEHICLE_ESSENCE
{
	unsigned int	id;							// (类型)ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_model;					// 模型文件

	int				file_matter;				// 掉在地上的模型路径名
	int				file_icon;					// 图标路径名

	int				level_required;				// 等级限制
	int				renascence_count;			// 转生次数要求
	unsigned int	nation_position_mask;		// 职位要求
												// 0x0001 国王
												// 0x0002 王后
												// 0x0004 将军
												// 0x0008 大臣
												// 0x0010 帮众
	unsigned int	only_qianji;				// 是否牵机职业专属 
	unsigned int	only_in_war;				// 是否城战专用
	unsigned int    is_multi_ride;              // 是否多人骑乘 0：否，1：是
	unsigned int	multi_ride_mode;			// 多人骑乘的模式 0：龙蛇模式 1：车马模式

	float			speed;						// 移动速度
	float			height;						// 高度

	unsigned int	init_upgrade;
	float			addon_prop[5];

	struct
	{
		int			id;
		float		prop;
	} addon[20];

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

// 飞行器
struct AIRCRAFT_ESSENCE
{
	unsigned int	id;							// (类型)ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径名
	int				file_icon;					// 图标路径名

	int				file_model;					// 模型文件
	int				price;						// 卖店价
	int				shop_price;					// 店卖价
	int				level_required;				// 等级限制
	UInt64			character_combo_id;			// 职业限制
	UInt64			character_combo_id2;		// 职业限制，扩展职业ID后的新职业掩码，Added 2011-07-14.
	float			speed;						// 移动速度
	int				fly_mode;					// 飞行模式

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

// 飞行精力值药品
struct FLY_ENERGYFOOD_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径名

	int				energy_total;				// 总的精力值
	int				energy_used_minimum;		// 精力使用最小值
	
	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 该药品的堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

///////////////////////////////////////////////////////////////////////////////////////
// 传送卷轴
///////////////////////////////////////////////////////////////////////////////////////
struct TRANSMITROLL_ESSENCE
{
	unsigned int	id;							// (类型)ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径
	
	int				renascence_count;			// 飞升次数限制
	int				require_level;				// 等级限制

	int				tag;						// 目标世界id
	float			x;							// 目标X坐标
	float			y;							// 目标Y坐标
	float			z;							// 目标Z坐标

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

///////////////////////////////////////////////////////////////////////////////////////
// 替身娃娃
///////////////////////////////////////////////////////////////////////////////////////
struct LUCKYROLL_ESSENCE
{
	unsigned int	id;							// (类型)ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径

	int				type;						// 类型：0-物品卷，1-经验卷

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

///////////////////////////////////////////////////////////////////////////////////////
// 双倍经验道具
///////////////////////////////////////////////////////////////////////////////////////
struct DOUBLE_EXP_ESSENCE
{
	unsigned int	id;							// (类型)ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径

	int				exp_multiple;				// 经验倍数		
	int				double_exp_time;			// 多倍经验时间(秒)，(已经不只是双倍，2013.4.23)

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

///////////////////////////////////////////////////////////////////////////////////////
//
// 回城卷轴模板数据结构定义
//
//
//
///////////////////////////////////////////////////////////////////////////////////////
struct TOWNSCROLL_ESSENCE
{
	unsigned int	id;							// (类型)ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径

	float			use_time;					// 使用时间（秒）

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	int				blank;						// 是否为空白

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

///////////////////////////////////////////////////////////////////////////////////////
//
// 复活卷轴模板数据结构定义
//
//
//
///////////////////////////////////////////////////////////////////////////////////////
struct REVIVESCROLL_ESSENCE
{
	unsigned int	id;							// (类型)ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径

	float			use_time;					// 使用时间（秒）
	int				cool_time;					// 冷却时间（毫秒）

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

///////////////////////////////////////////////////////////////////////////////////////
//
// 攻城车用卷轴
//
//
//
///////////////////////////////////////////////////////////////////////////////////////

struct SIEGE_ARTILLERY_SCROLL_ESSENCE
{
	unsigned int	id;							// (类型)ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};


///////////////////////////////////////////////////////////////////////////////////////
//
// 任务物品模板数据结构定义
//
//
//
///////////////////////////////////////////////////////////////////////////////////////
struct TASKMATTER_ESSENCE
{
	unsigned int	id;							// (类型)ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_icon;					// 图标路径

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

struct TASKMATTER_ESSENCE_2 : public TASKMATTER_ESSENCE
{
	unsigned int    price;
};



///////////////////////////////////////////////////////////////////////////////////////
//	用于任务的普通物品
///////////////////////////////////////////////////////////////////////////////////////
struct TASKNORMALMATTER_ESSENCE
{
	unsigned int	id;							// (类型)ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

///////////////////////////////////////////////////////////////////////////////////////
//
// 任务随机发生器数据结构定义
//
//
//
///////////////////////////////////////////////////////////////////////////////////////
struct TASKDICE_ESSENCE
{
	unsigned int	id;							// (类型)ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径

	struct
	{
		unsigned int	id;						// 任务id
		float			probability;			// 产生该任务的概率
		
	} task_lists[8];

	int				type;
	
	int			map_trigger;		// 触发地图id
	float		pos_trigger[3];		// 触发点
	float		radius_trigger;		// 触发半径

	struct 
	{
		unsigned int	map_id;			// 地图号
		float			pos_min[3];		// 左下角
		float			pos_max[3];		// 右上角
	} trigger_point[8];

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限
	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

struct TASKDICE_ESSENCE_2 : public TASKDICE_ESSENCE
{
	unsigned int    price;
};


////////////////////////////////////////////////////////////////////////////////////
// GM 物品生成器
////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
// GM 物品生成器类型定义
///////////////////////////////////////////////////////////////////////////////////////
struct GM_GENERATOR_TYPE
{
	unsigned int	id;							// (类型)ID
	namechar		name[32];					// 名称, 最多15个汉字
};

///////////////////////////////////////////////////////////////////////////////////////
// GM 物品生成器数据结构定义
///////////////////////////////////////////////////////////////////////////////////////
struct GM_GENERATOR_ESSENCE
{
	unsigned int	id;							// ID
	unsigned int	id_type;					// 类别ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径

	unsigned int	id_object;					// 待生成的物品（怪物）ID

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

////////////////////////////////////////////////////////////////////////////////////////
// 烟花本体
////////////////////////////////////////////////////////////////////////////////////////
struct FIREWORKS_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径

	int				file_fw;					// 组合效果文件
	int				level;						// 等级

	int				time_to_fire;				// 燃放时间(秒)

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 该烟花的堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

////////////////////////////////////////////////////////////////////////////////////////
// 文字烟花本体
////////////////////////////////////////////////////////////////////////////////////////
struct TEXT_FIREWORKS_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径

	int				file_fw;					// 组合效果文件
	int				level;						// 等级

	namechar		char_can_use[256];			// 可以使用的字符有哪些
	int				max_char;					// 可以输出的最大字符数

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 该烟花的堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

////////////////////////////////////////////////////////////////////////////////////////
// 强化石本体
////////////////////////////////////////////////////////////////////////////////////////
struct ESTONE_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径

	struct  
	{
		unsigned int	equip_mask;				// 可镶嵌的装备类别
		unsigned int	effect_addon_type;		// 效果附加属性类型

	} effects[4];

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

////////////////////////////////////////////////////////////////////////////////////////
// 属性石本体
////////////////////////////////////////////////////////////////////////////////////////
struct PSTONE_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径

	unsigned int	type;						// 类型：0-普通，1-降低装备等级要求1级

	unsigned int	equip_mask;					// 可镶嵌的装备类别
	unsigned int	effect_addon_id;			// 效果附加属性id
	
	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

////////////////////////////////////////////////////////////////////////////////////////
// 技能石本体
////////////////////////////////////////////////////////////////////////////////////////
struct SSTONE_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径

	unsigned int	equip_mask;					// 可镶嵌的装备类别
	unsigned int	skill_id;					// 技能id
	int				skill_level;				// 技能等级

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

////////////////////////////////////////////////////////////////////////////////////////
// 潜能书本体
////////////////////////////////////////////////////////////////////////////////////////
struct POTENTIAL_TOME_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径

	unsigned int	sect_mask;					// 门派要求
	int				require_race;				// 种族限制，0-人族，1-兽族， 2-都可以
	int				require_level;				// 等级限制
	int				potential_id;				// 潜能id

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

////////////////////////////////////////////////////////////////////////////////////////
// 夫妻同心结
////////////////////////////////////////////////////////////////////////////////////////
struct COUPLE_JUMPTO_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

// 喇叭
struct SPEAKER_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径

	int				id_icon_set;				// 表情符号套号

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 该药品的堆叠上限
	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

// 书籍
struct BOOK_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径
	
	int				book_type;					// 排版方式：1卷轴 2竖轴 3-6备用
	int				file_content[32];

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 该药品的堆叠上限
	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

////////////////////////////////////////////////////////////////////////////////////////
// 宠物牌
////////////////////////////////////////////////////////////////////////////////////////
struct PET_BEDGE_ESSENCE
{
	unsigned int	id;							// (类型)ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径
	int				file_head_icon[7];			// 头像图标
	int				file_to_shown[7];			// 不同星级形象

	namechar		desc[16];					// 宠物描述，显示在宠物名字之下，最多16个汉字
	float			size;						// 宠物大小
	int				pet_type;					// 宠物种族
	unsigned int	name_color;
	int				show_level;					// 显示等级
	unsigned int	immune_type;
	int				level;						// 宠物初始等级

	float			sight_range;				// 视野
	float			dmp_pos;					// 伤害点
	float			attack_range;				// 攻击距离

	int				intimacy_inc_speed;			// 亲密度增长速度
	int				hunger_speed1;				// 非战斗下饱食度降低速度
	int				hunger_speed2;				// 战斗下饱食度降低速度
	int				hp_gen1;					// 非战斗回血速度
	int				hp_gen2;					// 战斗回血速度
	int				mp_gen1;					// 非战斗回魔速度
	int				mp_gen2;					// 战斗回魔速度
	
	unsigned int	stand_mode;					// 站立方式，可能的有：四足站立、双足站立、悬空飞行等

	float			walk_speed;					// 慢速移动速度(M/S)
	float			run_speed;					// 快速移动速度(M/S)
	
	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

////////////////////////////////////////////////////////////////////////////////////////
// 宠物食品
////////////////////////////////////////////////////////////////////////////////////////
struct PET_FOOD_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径

	int				pet_level_min;				// 宠物等级限制
	int				pet_level_max;				// 宠物等级限制
	unsigned int	pet_type_mask;				// 宠物种族限制

	int				food_usage;					// 食品用途
	unsigned int	pet_food_type;				// 一次性消耗药剂还是多次消耗型药剂
	int				food_value;					// 食品增加的值

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

////////////////////////////////////////////////////////////////////////////////////////
// 宠物自动补充药
////////////////////////////////////////////////////////////////////////////////////////
struct PET_AUTO_FOOD_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径

	int				food_value;					// 药品的总量
	float			hp_gen;						// 每点可回复气血
	float			mp_gen;						// 每点可回复真气
	float			hunger_gen;					// 每点可回复饱食度

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

////////////////////////////////////////////////////////////////////////////////////////
// 宠物修炼道具
////////////////////////////////////////////////////////////////////////////////////////
struct PET_REFINE_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径

	int				arg1;						// 特征值1
	int				arg2;						// 特征值2
	int				arg3;						// 特征值3
	int				arg4;						// 特征值4

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

////////////////////////////////////////////////////////////////////////////////////////
// 宠物修炼辅助道具
////////////////////////////////////////////////////////////////////////////////////////
struct PET_ASSIST_REFINE_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

////////////////////////////////////////////////////////////////////////////////////////
// 宠物技能卷
////////////////////////////////////////////////////////////////////////////////////////
struct PET_SKILL_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径

	int				level_required;				// 等级限制
	int				id_skill;					// 技能id
	int				skill_level;				// 技能等级
	unsigned int	only_in_war;				// 是否城战专用
	unsigned int	effective_on_pet;			// 使用对象	宠物/人物
	int				cool_type;					// 冷却时间的类别

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

////////////////////////////////////////////////////////////////////////////////////////
// 宠物装备
////////////////////////////////////////////////////////////////////////////////////////
struct PET_ARMOR_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径
	namechar		desc[16];					// 描述, 最多16个汉字

	unsigned int	pet_type_mask;				// 宠物类别限制
	int				lev_required;
	unsigned int	astro_type;					// 主星相限定
	int				astro_level;				// 星等限定;
	int				equip_location;
	int				fee_enhance;				// 宠物装备强化费用
	
	struct {
		int			maximum;					// 上限
		int			minimum;					// 下限
	} int_props[12];

	struct {
		float		maximum;					// 上限
		float		minimum;					// 下限
	} float_props[2];

	struct {
		int			id;
		int			level;
	} addon_skills[5];

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

///////////////////////////////////////////////////////////////////////////////////////
// 配方卷轴
///////////////////////////////////////////////////////////////////////////////////////
struct RECIPEROLL_MAJOR_TYPE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字
};

struct RECIPEROLL_SUB_TYPE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字
};

struct RECIPEROLL_ESSENCE
{
	unsigned int	id;							// ID
	unsigned int	id_major_type;				// 大类别ID
	unsigned int	id_sub_type;				// 小类别ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径

	unsigned int	id_recipe;					// 对应的配方ID

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

///////////////////////////////////////////////////////////////////////////////////////
//
// 配方模板数据结构定义
//
//
///////////////////////////////////////////////////////////////////////////////////////

// 配方大类别
struct RECIPE_MAJOR_TYPE
{
	unsigned int	id;							// 配方大类别ID
	namechar		name[32];					// 配方大类别名称
};

// 配方小类别
struct RECIPE_SUB_TYPE
{
	unsigned int	id;							// 配方小类别ID
	namechar		name[32];					// 配方小类别名称
};

struct RECIPE_ESSENCE
{
	unsigned int	id;							// 配方本体ID
	unsigned int	id_major_type;				// 配方大类别ID
	unsigned int	id_sub_type;				// 配方小类别ID
	namechar		name[32];					// 配方本体名称

	int				level;						// 配方段数，生产时根据当前生产技能段数和此数值判断是否能够生产
	
	struct 
	{
		float			probability;				// 生成概率（无自动归一化）
		unsigned int	id_to_make;					// 生成的东西ID
		int				min_num_make;				// 生成的东西数目下限
		int				max_num_make;				// 生成的东西数目上限

	} products[3];
	
	int				price;						// 生产的费用
	float			duration;					// 生产的时间

	struct
	{
		unsigned int	id;						// 原料id
		int				num;					// 原料所需数目

	}  materials[6];

	int				recipe_class;				// 配方类型
	int				required[7];
	int				cool_time;					// 冷却时间（毫秒）
	int				cool_type;
};

///////////////////////////////////////////////////////////////////////////////////////
//
// 怪物模板数据结构定义
//
//		注: 怪物只进行依据形象的小分类, 怪物的攻击策略则作为一个属性在个体中进行选择
//
//
///////////////////////////////////////////////////////////////////////////////////////

struct DROPTABLE_TYPE
{
	unsigned int	id;							// (类型)ID
	namechar		name[32];					// 名称, 最多15个汉字
};

struct DROPTABLE_ESSENCE
{
	unsigned int	id;							// 怪物(类型)ID
	unsigned int	id_type;					// 类别ID
	namechar		name[32];					// 名称, 最多15个汉字

	struct										// 掉落0~5个物品的概率
	{
		float		probability;

	} num_to_drop[5];

	struct										// 掉落物品列表
	{
		unsigned int	id_obj;					// 掉落物品id
		float			probability;			// 掉落概率

	} drops[64];

};

// 怪物类别
struct MONSTER_TYPE
{
	unsigned int	id;							// (类型)ID
	namechar		name[32];					// 名称, 最多15个汉字

};

// 怪物本体属性
struct MONSTER_ESSENCE
{
	unsigned int	id;							// 怪物(类型)ID
	unsigned int	id_type;					// 类别ID
	namechar		name[32];					// 名称, 最多15个汉字

	namechar		prop[16];					// 怪物属性, 最多16个汉字
	namechar		desc[16];					// 怪物描述, 最多16个汉字
	namechar		monster_desc[16];			// 怪物描述，显示在怪物名字之下，最多16个汉字

	unsigned int	faction;					// 自己所属阵营组合代码
	unsigned int	enemy_faction;				// 敌对阵营组合代码
	unsigned int	monster_faction;			// 怪物的详细派系划分

	int				file_model;					// 模型路径名
	int				head_icon;					// 头像
	int				big_icon;					// 怪物图鉴，大图标，暂用于趴塔副本，Added 2012-04-23.
	
	float			size;						// 大小

	unsigned int	name_color;					// 名字颜色
	
	unsigned int	id_strategy;				// 怪物攻击策略ID
												// 0 表示物理肉搏类怪物
												// 1 表示物理弓箭类怪物
												// 2 表示魔法类怪物
												// 3 表示肉搏＋远程类怪物
												// 4 堡垒类
												// 5 场景活物类
												// 6 树桩类
												// 7 堡垒魔法类

	unsigned int	select_strategy;			// 怪物被选中策略
												// 0 可以正常选中
												// Tab不可选中
												// 任何方式都不能选中
	unsigned int    air_prop;					// 是否是空中怪 0：否 1：是
	unsigned int	show_damage;				// 客户端是否显示伤害 0：否 1: 是

	int				level;						// 怪物等级
	int				show_level;					// 显示等级
	unsigned int	is_boss;					// 是否BOSS
	unsigned int	killed_exp;					// 被其杀死是否损失经验
	unsigned int	killed_drop;				// 被其杀死是否掉落物品
	int				is_drop_adjustby_skill;		// 掉落是否受技能影响 默认值为0：表示掉落不受技能的影响；1：表示掉落会受到技能影响
	
	unsigned int	immune_type;				// 免疫类型，为Mask组合方式，各位的含义从低到高依次为：
												// 免疫恐惧、免疫致盲、免疫击退、免疫被打掉mp、免疫减总防御％、免疫减固定值防御
												// 免疫眩晕、免疫虚弱、免疫缓慢、免疫沉默、免疫睡眠、免疫缠绕、免疫禁食、免疫空手
												// 免疫持续伤害、免疫嘲讽、免疫元神耗散、免疫流魔、免疫变身、免疫灼烧、免疫血咒
												// 免疫降低防御%

	float			sight_range;				// 视野
	float			attack_range;				// 攻击距离
	
	unsigned int	aggressive_mode;			// 主被动选择：0 - 被动；1 - 主动
	unsigned int	monster_faction_ask_help;	// 向哪些阵营求助，从怪物详细派系中选择
	unsigned int	monster_faction_can_help;	// 接受哪些阵营求助，从怪物详细派系中选择

	float			aggro_range;				// 仇恨距离
	float			aggro_time;					// 仇恨时间
	float			dead_aggro_time;			// 死亡仇恨清空时间

	unsigned int	patroll_mode;				// 巡逻方式，可能的有：随机走动，...
	unsigned int	stand_mode;					// 站立方式，可能的有：四足站立、双足站立、悬空飞行等

	float			walk_speed;					// 慢速移动速度(M/S)
	float			run_speed;					// 快速移动速度(M/S)
	
	unsigned int	common_strategy;			// 怪物策略

	unsigned int	after_death;				// 死亡后续：无-0、自爆-0x1、重生-0x2
	unsigned int	adjust_exp;					// 经验是否不受惩罚系数影响, 默认为0，不受影响，1为受影响
	unsigned int	can_catch;					// 是否可以捕捉，默认0表示不可捕捉，1-8表示捕捉的难度:
												// 从非常容易、容易、比较容易、中等、难、比较难、非常难、难上加难
	unsigned int	player_cannot_attack;		// 玩家攻击是否无效，:有效-0， 无效-1								
	int				exp;						// 初始经验
	int 			money_average;				// 初始钱：标准值
	int				money_var;					// 初始钱：浮动值
	int				hp;							// 初始生命力
	int				hp_disp_lv;					// 血量层数
	int				mp;							// 初始魔力
	int				dmg;						// 初始攻击力
	int				defense;					// 初始防御力
	int				extra_damage;				// 附加伤害
	int				extra_defence;				// 减免伤害
	int				attack;						// 初始命中
	int				armor;						// 初始闪躲
	int				crit_rate;					// 初始致命一击率(%)
	int				crit_damage;				// 初始致命一击伤害(%)
	int				anti_stunt;					// 初始眩晕抗性
	int				anti_weak;					// 初始虚弱抗性
	int				anti_slow;					// 初始缓慢抗性
	int				anti_silence;				// 初始沉默抗性
	int				anti_sleep;					// 初始睡眠抗性
	int				anti_twist;					// 初始缠绕抗性

	float			lvlup_exp;					// 每升一级所增长的经验
	float 			lvlup_money_average;		// 每升一级所增长的钱：标准值
	float			lvlup_money_var;			// 每升一级所增长的钱：浮动值
	float			lvlup_hp;					// 每升一级所增长的生命力
	float			lvlup_mp;					// 每升一级所增长的魔力
	float			lvlup_dmg;					// 每升一级所增长的攻击力
	float			lvlup_defense;				// 每升一级所增长的防御力
	float			lvlup_attack;				// 每升一级所增长的命中
	float			lvlup_armor;				// 每升一级所增长的闪躲
	float			lvlup_crit_rate;			// 每升一级所增长的致命一击率(%)
	float			lvlup_crit_damage;			// 每升一级所增长的致命一击伤害(%)
	float			lvlup_anti_stunt;			// 每升一级所增长的眩晕抗性
	float			lvlup_anti_weak;			// 每升一级所增长的虚弱抗性
	float			lvlup_anti_slow;			// 每升一级所增长的缓慢抗性
	float			lvlup_anti_silence;			// 每升一级所增长的沉默抗性
	float			lvlup_anti_sleep;			// 每升一级所增长的睡眠抗性
	float			lvlup_anti_twist;			// 每升一级所增长的缠绕抗性

	int				hp_gen1;					// 慢速回血速度
	int				hp_gen2;					// 快速回血速度
	int				mp_gen1;					// 慢速回魔速度
	int				mp_gen2;					// 快速回魔速度

	unsigned int	role_in_war;				// 城战中所扮演的角色
												// 0 无；1 中心建筑；2 炮塔；3 箭塔；4 投石车；5 传送点；6 复活点；7 服务NPC；8 占领标志物

	int				drop_times;					// 掉落次数(1-10)

	struct  
	{
		int		id_drop_table;						// 掉落表 id
		float	prop_drop_table;					// 掉落表的概率
	}drop_table[3];

	// 技能列表
	struct
	{
		unsigned int	id_skill;				// 怪物的技能类型ID
		int				level;					// 技能的级别
	} skills[32];

	float			dec_crit_rate;
	float			dec_crit_dmg;
	float			armor_prop;
	float			attack_prop;

	unsigned int	tick_stratege;				// 怪物策略是否始终随心跳执行
	unsigned int	task_share;					// 该怪物是否可以为任务分享所杀怪，开启后，非组队情况下可共享杀怪 Added 2012-04-09.
	unsigned int	item_drop_share;			// 该怪物被杀死后，是否共享怪物掉落 Added 2012-04-09.
	unsigned int	buff_area_id;				// 怪物相关的buff区域配置id
};

///////////////////////////////////////////////////////////////////////////////////////
//
// NPC模板数据结构定义
//
//		注: NPC本体具有相同的数据结构，而NPC能够提供的服务则作为一个属性加入到NPC本体中，
//			具体服务相关的数据，如：卖东西时的物品列表等，则作为服务的属性存在服务模板中
//			
//		根据目前的了解，服务一共分以下几类：
//			0、基本交谈功能，并可由基本交谈中引出其他的服务，需要的数据：对话数据
//			1、出售物品，需要的数据：对话数据、可提供的物品列表
//			2、收购物品，需要的数据：对话数据
//			3、修理物品和装备，需要的数据：对话数据
//			4、给有眼的装备镶嵌饰品，需要的数据：对话数据、可以提供的饰品列表等
//			5、拆除已镶嵌在装备上的饰品，需要的数据：对话数据
//			6、发任务、验证任务、进行完成任务奖励等同任务相关的服务，需要的数据：对话数据、可以提供和处理的任务列表
//			7、教授一些技能，需要的数据：对话数据、可以教授的技能列表
//			8、对玩家进行治疗，需要的数据：对话数据
//			9、将玩家进行传送，需要的数据：对话数据，瞬移目标点，费用列表
//			10、为玩家提供交通工具运输服务，需要的数据：对话数据，交通工具及路线，价格
//			11、物品的代售，需要的数据：对话数据
//			12、为玩家存储物品、金钱，需要的数据：对话数据，手续费
//			13、生产、分解特定物品，需要的数据：对话数据、可生产，分解的物品列表，生产及分解物品的时间、价钱影响比例
//			14、向导功能，需要的数据：特定等级对应的对话，特殊区域对应的对话
//			15、猎杀红名/杀死玩家
//			16、杀死某几个帮派玩家
//			17、进入帮派基地：帮派功能NPC
//			18、道路通行验证人：检票员NPC
//
///////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////
//
// NPC服务模板数据结构定义
//
///////////////////////////////////////////////////////////////////////////////////////

// 交谈服务
struct NPC_TALK_SERVICE
{
	unsigned int	id;							// 服务(类型)ID
	namechar		name[32];					// 名称, 最多15个汉字	

	unsigned int	id_dialog;					// 对话ID
};

// 出售商品的服务
struct NPC_SELL_SERVICE
{
	unsigned int	id;							// 服务(类型)ID
	namechar		name[32];					// 名称, 最多15个汉字

	struct
	{
		namechar		page_title[8];			// 页的名字，最多7个汉字
		float			discount;
		unsigned int	id_goods[48];			// 可出售的商品列表

	} pages[8];									// 按页方式存储的商品列表

	unsigned int	id_dialog;					// 对话ID
};

// 收购商品的服务
struct NPC_BUY_SERVICE
{
	unsigned int	id;							// 服务(类型)ID
	namechar		name[32];					// 名称, 最多15个汉字	

	unsigned int	id_dialog;					// 对话ID
};

// 发放任务服务
struct NPC_TASK_OUT_SERVICE
{
	unsigned int	id;							// 服务(类型)ID
	namechar		name[32];					// 名称, 最多15个汉字	

	unsigned int	id_task_set;				// 任务库号
	int				task_storage_type;			//库任务属性, Added 2011-02-21
	unsigned int	id_tasks[256];				// 可以处理的任务列表
};

// 验证完成任务服务
struct NPC_TASK_IN_SERVICE
{
	unsigned int	id;							// 服务(类型)ID
	namechar		name[32];					// 名称, 最多15个汉字	

	unsigned int	id_tasks[256];				// 可以处理的任务列表
};

// 发放任务物品服务
struct NPC_TASK_MATTER_SERVICE
{
	unsigned int	id;							// 服务(类型)ID
	namechar		name[32];					// 名称, 最多15个汉字	

	struct
	{
		unsigned int	id_task;				// 可以处理的任务列表
		struct
		{
		unsigned int	id_matter;				// 任务相关物品id
		int				num_matter;				// 任务相关物品数目
		} taks_matters[4];

	} tasks[16];
};

// 治疗服务
struct NPC_HEAL_SERVICE
{
	unsigned int	id;							// 服务(类型)ID
	namechar		name[32];					// 名称, 最多15个汉字

	unsigned int	id_dialog;					// 对话ID
};

// 传送服务
struct NPC_TRANSMIT_SERVICE
{
	unsigned int	id;							// 服务(类型)ID
	namechar		name[32];					// 名称, 最多15个汉字

	struct {
		int			id_world;					// 目标点世界ID
		namechar	name[16];					// 目标点名称1
		float		x;							// 目标点x坐标
		float		y;							// 目标点y坐标
		float		z;							// 目标点z坐标
		int			fee;						// 费用
		int			required_level;				// 等级要求
	} targets[16];

	unsigned int	id_dialog;					// 对话ID
};

// 代售服务
struct NPC_PROXY_SERVICE
{
	unsigned int	id;							// 服务(类型)ID
	namechar		name[32];					// 名称, 最多15个汉字

	unsigned int	id_dialog;					// 对话ID
};

// 存储物品、金钱
struct NPC_STORAGE_SERVICE
{
	unsigned int	id;							// 服务(类型)ID
	namechar		name[32];					// 名称, 最多15个汉字
};

// 城战炮塔建造服务
struct NPC_WAR_TOWERBUILD_SERVICE
{
	unsigned int	id;							// 服务(类型)ID
	namechar		name[32];					// 名称, 最多15个汉字

	struct 
	{
		int				id_in_build;				// 建造中的对象id
		int				id_buildup;					// 建造完成后的对象id
		int				id_object_need;				// 所需要的物品
		int				time_use;					// 建造所需的时间
		int				fee;						// 建造所需的费用

	} build_info[4];
};

// 洗点服务
struct NPC_RESETPROP_SERVICE
{
	unsigned int	id;							// 服务(类型)ID
	namechar		name[32];					// 名称, 最多15个汉字

	struct 
	{
		int				id_object_need;			// 所需要的物品
		int				resetprop_type;			// 洗点方式
	} prop_entry[6];
};

// 装备绑定服务
struct NPC_EQUIPBIND_SERVICE
{
	unsigned int	id;							// 服务(类型)ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				id_object_need;				// 所需要的物品
	int				price;						// 所需要的钱
};

// 装备销毁服务
struct NPC_EQUIPDESTROY_SERVICE
{
	unsigned int	id;							// 服务(类型)ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				id_object_need;				// 所需要的物品
	int				price;						// 所需要的钱
};

// 装备解除销毁服务
struct NPC_EQUIPUNDESTROY_SERVICE
{
	unsigned int	id;							// 服务(类型)ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				id_object_need;				// 所需要的物品
	int				price;						// 所需要的钱
};

// 城战购买弓箭手服务
struct NPC_WAR_BUY_ARCHER_SERVICE
{
	unsigned int	id;							// 服务(类型)ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				price;						// 消耗金钱
	int				war_material_id;			// 消耗资源种类
	int				war_material_count;			// 消耗资源数量
	unsigned int	item_wanted;				// 需要包裹中有某物品
	unsigned int	remove_one_item;			// 是否扣掉1个该物品
	unsigned int	controller_id[20];			// 控制器id
};

// 商品寄售服务
struct NPC_CONSIGN_SERVICE
{
	unsigned int	id;						// id
	namechar		name[32];				// 名称，最多15个汉字
	unsigned int	deposit;				// 保证金
};

// 声望商店服务
struct NPC_REPUTATION_SHOP_SERVICE
{
	unsigned int	id;							// 服务(类型)ID
	namechar		name[32];					// 名称, 最多15个汉字
	
	struct
	{
		namechar		page_title[8];			// 页的名字，最多7个汉字

		struct  
		{
			unsigned int	id_goods;
			
			struct
			{
				unsigned int	repu_id_req;
				unsigned int	repu_req_value;
				unsigned int	repu_id_consume;
				unsigned int	repu_consume_value;
			} repu_required_pre[3];
			
		}goods[48];	// 可出售的商品列表
		
	} pages[4];									// 按页方式存储的商品列表
};

// UI传送服务
struct NPC_UI_TRANSFER_SERVICE
{
	unsigned int	id;							// 服务(类型)ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				trans_id[256];				// 传送模板id
};

// NPC类型定义
struct NPC_TYPE
{
	unsigned int	id;							// (类型)ID
	namechar		name[32];					// 名称, 最多15个汉字	

};

///////////////////////////////////////////////////////////////////////////////////////
//
// NPC本体模板数据结构定义
//
///////////////////////////////////////////////////////////////////////////////////////
struct NPC_ESSENCE
{
	unsigned int	id;							// NPC(类型)ID
	namechar		name[32];					// 名称, 最多15个汉字

	unsigned int	id_type;					// NPC 类型, 从NPC_TYPE中选取
	
	namechar		desc[16];					// NPC描述，显示在NPC名字之下，最多16个汉字

	float			refresh_time;				// 刷新时间

	unsigned int	attack_rule;				// 是否被攻击 0-不可被攻击，1-可被攻击攻击变粉名，2-可被攻击攻击变红名
	
	int				file_model;					// 模型路径名
	int				head_icon;					// 头像

	unsigned int	id_src_monster;				// 基本属性来自的怪物ID，非零时表示使用该怪物属性替换本NPC的属性

	namechar		hello_msg[256];				// 开场白，是一个多行文本

	unsigned int	id_to_discover;				// 发现和传送服务的驿站ID

	unsigned int	domain_related;				// 是否领土相关 0-否，1-是
	unsigned int	guard_npc;					// 是否卫兵NPC  0-否，1-是
	unsigned int	is_vehicle;					// 是否交通工具 0-否，1-是

	unsigned int	war_role_config;			// 城战角色属性表id

	// 服务列表
	unsigned int	id_talk_service;			// 交谈的服务ID
	unsigned int	id_sell_service;			// 出售商品的服务ID
	unsigned int	id_gshop_majortype;			// 百宝阁出售的大类号ID
	unsigned int	id_buy_service;				// 收购商品的服务ID
	unsigned int	id_task_out_service;		// 任务相关的服务ID: 发放任务服务
	unsigned int	id_task_in_service;			// 任务相关的服务ID: 验证完成任务服务
	unsigned int	id_task_matter_service;		// 任务相关的服务ID: 发放任务物品服务
	unsigned int	id_heal_service;			// 治疗的服务ID
	unsigned int	id_transmit_service;		// 传送的服务ID
	unsigned int	id_proxy_service;			// 代售的服务ID
	unsigned int	id_storage_service;			// 仓库的服务ID
	unsigned int	id_war_towerbuild_service;	// 城战炮塔建造的服务ID
	unsigned int	id_resetprop_service;		// 洗点服务ID
	unsigned int	id_equipbind_service;		// 装备绑定服务
	unsigned int	id_equipdestroy_service;	// 装备销毁服务
	unsigned int	id_equipundestroy_service;	// 装备解除销毁服务
	unsigned int	id_war_archer_service;		// 城战购买弓箭手服务
	unsigned int	id_item_trade_service[4];		// 物物交易服务
	unsigned int	id_equip_soul_service;		// 器魄融合服务ID：0表示不开启此服务
	unsigned int	id_consign_service;			// 商品寄售服务ID
	unsigned int	id_reputation_shop_service;	// 声望商店ID
	int				id_trans_dest_service;		// 界面传送服务
	int				id_open_trans;				// 打开某个传送区域
	unsigned int	combined_services;			// 简单服务组合：其中的每一位代表一个不需要参数的服务，可能的有：
												//		0：驯化服务；1：跨服pk隐仙阁报名；2：物品锁定；3：驿站发现服务；4：帮派服务；5：修复破损物品；6：邮寄服务；7：拍卖服务；8：双倍经验打卡服务；
												//		9：孵化宠物蛋服务；10：还原宠物蛋服务；11：城战管理服务；12：离开战场服务；13：点卡寄售；14：帮派仓库服务；
												//		15：法宝相关服务；16：城战报名服务；17：战场入场服务；18：城战日常建设服务；19：宠物驯养；20：宠物放生；
												//		21：宠物合成；22：宠物命名; 23: 滴血认主；24：器魄镶嵌；25：器魄拆除；26：恢复魄力；27：装备拆解； 28：飞升战场管理服务
												//		29：宋金战场报名服务 30：竞技场报名服务 31：线上推广服务

	unsigned int	combined_services2;			// 简单服务组合：其中的每一位代表一个不需要参数的服务：
												// 0：形象修改服务；1：宠物装备强化; 2:跨服pk逸龙轩报名 3：跨服pk观战者报名 4:剧情战场报名服务 5：变身道具相关服务
												// 6：变身卡灵力恢复服务；7：领土战宣战；8:进入领土战战场 9：领土战奖励（包括各种奖励）10:星盘充能服务 11:修复破碎的物品（特殊） 
												// 12: 装备升级服务; 13: 传送到跨服服务; 14: 从跨服传回服务; 15: 进入挑战空间服务; 16: 宝石插槽鉴定服务; 17: 宝石插槽重铸服务;
												// 18: 宝石插槽定制服务; 19: 宝石镶嵌服务; 20: 宝石拆除服务; 21: 宝石升品服务; 22: 宝石精炼服务; 23: 宝石萃取服务; 24: 宝石熔炼服务;
												// 25: 跨服小队pk红队报名; 26: 跨服小队pk蓝队报名; 27: 跨服小队pk记者报名; 28: 副本相关服务; 29: 爬塔副本属性显示; 30: 改名服务
												// 31: 家族改名

	unsigned int	combined_services3;			// 简单服务组合：其中的每一位代表一个不需要参数的服务：
												// 0：帮派改名；1：法宝飞升；2：法宝技能融合；3：法宝技能镶嵌；4：无用别勾；5：装备星座打孔；6：星座镶嵌；7：星座摘除；8：无用别勾
												// 9：无用别勾；10：跨服报名传回 11:跨服报名传送 12:国战报名 13:飞升进度查询 14:首饰升级 15:生产技能快速生产 16:金身法身快速制作
												// 17:流水席战场报名 18:帮派基地 19：宠物飞升 20: 灾变专用 21:帮派元宝商城

	// 洗PK值服务
	unsigned int	has_pkvalue_service;		// 是否提供洗PK值服务
	int				fee_per_pkvalue;			// 每洗一点PK值所需的花费

	bool			service_install;			// 是否提供镶嵌服务
	bool			service_uninstall;			// 是否提供拆卸服务
	bool			service_temp1;				// 占位
	bool			service_temp2;				// 占位

	unsigned int	id_mine;					// 附带的矿
	unsigned int	id_interaction_object;		// 可交互物体
	unsigned int	collision_in_server;		// 是否参与碰撞
	float			vehicle_min_length;			// 交通工具的最小长度
	float			vehicle_min_width;			// 交通工具的最小宽度
	float			vehicle_min_height;			// 交通工具的最小高度
	float			vehicle_max_length;
	float			vehicle_max_width;
	float			vehicle_max_height;
	int				id_territory;				// 所属区域id，领土战专用，默认为0表示非领土战区域npc
	int				id_transcription;			// 副本模板ID，默认情况下为：0表示没有副本发放服务 Added 2011-07-20.
};

///////////////////////////////////////////////////////////////////////////////////////
//
// 城战角色属性表
//
///////////////////////////////////////////////////////////////////////////////////////

struct WAR_ROLE_CONFIG
{
	unsigned int	id;							// (类型)ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				attack_max_lev;				// 攻击力等级上限

	struct
	{
		int			attack_value;				// 攻击力等级1-20的攻击力
		int			attack_extra;				// 攻击力等级1-20的附加伤害
	} attack_level[20];

	int				attack_war_material;		// 建设攻击力等级消耗城防物资种类
	float			attack_co[3];				// 建设攻击力等级消耗城防物资值系数a,b,c
	
	int				defence_max_lev;			// 防御力等级上限

	struct
	{
		int			defence_value;				// 防御力等级1-20的防御力
		int			defence_extra;				// 防御力等级1-20的减免附加伤害
	} defence_level[20];

	int				defence_war_material;		// 建设防御力等级消耗城防物资种类
	float			defence_co[3];				// 建设防御力等级消耗城防物资值系数a,b,c

	int				range_max_lev;				// 射程等级上限
	int				range_values[5];			// 射程等级1-5
	int				range_war_material;			// 建设射程等级消耗城防物资种类
	float			range_co[3];				// 建设射程等级消耗城防物资值系数a,b,c

	int				strategy_max_lev;			// 策略等级上限
	int				strategy_id[5];				// 策略等级1-5
	int				strategy_war_material;		// 建设策略等级消耗城防物资种类
	float			strategy_co[3];				// 建设策略等级消耗城防物资值系数a,b,c

	int				init_hp;					// 初始HP
	int				hp_war_material;			// 建设HP消耗城防物资种类
	int				lvlup_hp;					// HP每次建设提高的点数
	int				hp_material_num;			// 每次建设HP所需城防物资值
};

///////////////////////////////////////////////////////////////////////////////////////
//
// 矿类别模板数据结构定义
//
///////////////////////////////////////////////////////////////////////////////////////
struct MINE_TYPE
{
	unsigned int	id;							// (类型)ID
	namechar		name[32];					// 名称, 最多15个汉字
};


///////////////////////////////////////////////////////////////////////////////////////
//
// 矿本体模板数据结构定义
//
///////////////////////////////////////////////////////////////////////////////////////
struct MINE_ESSENCE
{
	unsigned int	id;							// 矿(类型)ID
	unsigned int	id_type;					// 类别ID
	namechar		name[32];					// 名称, 最多15个汉字
						  
	unsigned int	level;						// 等级
	unsigned int	level_required;				// 等级限制
	unsigned int	id_equipment_required;		// 所需要的工具类型
	unsigned int	eliminate_tool;				// 消耗采掘用具(true or false)
	unsigned int	time_min;					// 采矿时间下限（秒）
	unsigned int	time_max;					// 采矿时间上限（秒）


	int				exp;						// 每次采集所获的经验
	int				skillpoint;					// 每次采集所获的技能点

	int				cursor_icon;				// 采矿时显示的鼠标的图标（如果为0，则用默认的图标）	
	int				file_model;					// 模型路径名

	struct 
	{
		unsigned int	id;						// 原料ID 
		float			probability;			// 挖出概率 (总和为1)
		
	} materials[16];							// 挖掘出的原料表

	int				num1;						// 原料数目1
	float			probability1;				// 概率1
	int				num2;						// 原料数目2
	float			probability2;				// 概率2

	unsigned int	task_in;					// 需要的任务id
	unsigned int	task_out;					// 采集后相关任务id

	unsigned int	uninterruptable;			// 采集过程不可中断, false-可以中断, true-不可中断

	struct 
	{
		unsigned int	id_monster;				// 产生怪物id
		int				num;					// 产生怪物数目
		float			radius;					// 产生半径（米）
	} npcgen[4];

	struct 
	{
		unsigned int	monster_faction;		// 仇恨的怪物详细派系
		float			radius;					// 仇恨半径（米）
		int				num;					// 仇恨数值
	} aggros[1];

	unsigned int	role_in_war;				// 城战中所扮演的角色
												// 0 无；1 中心建筑；2 炮塔；3 箭塔；4 投石车；5 传送点；6 复活点；7 服务NPC；8 占领标志物

	unsigned int	permenent;					// 采集后矿物不消失，false-消失，true-不消失

	unsigned int	activate_controller_mode;	// 开启怪物控制器方式
	unsigned int	activate_controller_id[4];	// 开启怪物控制器id

	unsigned int	deactivate_controller_mode;	// 关闭怪物控制器方式
	unsigned int	deactivate_controller_id[4];// 关闭怪物控制器id
	unsigned int	close_collision_detection;	// 是否关闭碰撞检测, 0-不关闭, 1-关闭
};

// 对话过程对象
struct talk_proc
{
	namechar			text[64];					// 对话的第一个窗口的提示文字，最多63个汉字

	struct option
	{
		unsigned int	id;							// 选项链接的子窗口或功能ID, 如果最高位为1表示是一个预定义的功能, 为0表示一个子窗口
		namechar		text[64];					// 选项链接的提示文字，最多19个汉字
		unsigned int	param;						// 选项相关的参数
	};

	struct window
	{
		unsigned int	id;							// 窗口ID, 最高位不能为1
		unsigned int	id_parent;					// 父窗口ID, 为-1表示根窗口

		int				talk_text_len;				// 对话文字的长度
		namechar * 		talk_text;					// 对话文字

		int				num_option;					// 选项数目
		option *		options;					// 选项列表

		window() { options = 0; }
		~window() { delete [] options; options = 0; delete [] talk_text; talk_text = 0; }

		int save(FILE * fp)
		{
			fwrite(&id, sizeof(id), 1, fp);
			fwrite(&id_parent, sizeof(id_parent), 1, fp);

			fwrite(&talk_text_len, sizeof(talk_text_len), 1, fp);
			fwrite(talk_text, sizeof(namechar), talk_text_len, fp);

			fwrite(&num_option, sizeof(num_option), 1, fp);
			fwrite(options, sizeof(option), num_option, fp);
			return 0;
		}

		int load(FILE * fp)
		{
			fread(&id, sizeof(id), 1, fp);
			fread(&id_parent, sizeof(id_parent), 1, fp);

			fread(&talk_text_len, sizeof(talk_text_len), 1, fp);
			talk_text = new namechar[talk_text_len];
			fread(talk_text, sizeof(namechar), talk_text_len, fp);

			fread(&num_option, sizeof(num_option), 1, fp);
			options = new option[num_option];
			fread(options, sizeof(option), num_option, fp);
			return 0;
		}

		//Added by Ford.W 2010-06-21.
		window& operator = ( const window& src )
		{
			//copy the basic variables.
			id = src.id;
			id_parent = src.id_parent;
			talk_text_len = src.talk_text_len;
			num_option = src.num_option;

			//copy the talk text.
			if( !talk_text_len )
			{
				talk_text = NULL;
			}
			else
			{
				talk_text = new namechar[talk_text_len+1];
//				wcscpy(talk_text, src.talk_text);
				memcpy(talk_text, src.talk_text, (talk_text_len+1)*sizeof(namechar));
			}

			//copy the option.
			if( !num_option )
			{
				options = NULL;
			}
			else
			{
				//if the options is not NULL, then delete it.
				if( options )
				{
					delete []options;
					options = 0;
				}

				options = new option[num_option];
				if( !options )
				{
					//write log: failed to allocate memory.
					return *this;
				}

				//copy the options.
				for(int i = 0; i < num_option; ++i )
				{
					options[i] = src.options[i];
				}

			}

			return *this;
		}
		//Added end.
	};

	unsigned int		id_talk;				// 对话对象的ID

	int					num_window;				// 带对话文字的窗口个数
	window *			windows;				// 带对话文字的窗口
	
	talk_proc()
	{
		id_talk		= 0;
		num_window	= 0;
		windows		= 0;
	}

	~talk_proc()
	{
		delete [] windows;
		windows = 0;
		num_window = 0;
	}

	// 存盘读盘
	int save(FILE * fp)
	{
		fwrite(&id_talk, sizeof(id_talk), 1, fp);
		fwrite(text, sizeof(text), 1, fp);

		fwrite(&num_window, sizeof(num_window), 1, fp);
		for(int i=0; i<num_window; i++)
			windows[i].save(fp);
		
		return 0;
	}

	int load(FILE * fp)
	{
		fread(&id_talk, sizeof(id_talk), 1, fp);
		fread(text, sizeof(text), 1, fp);

		fread(&num_window, sizeof(num_window), 1, fp);
		windows = new window[num_window]; 
		for(int i=0; i<num_window; i++)
			windows[i].load(fp);

		return 0;
	}

	//Added by Ford.W 2010-06-21.
	talk_proc& operator = (const talk_proc& src)
	{
		//copy the basic variables.
//		wcscpy(text, src.text);
		memcpy(text, src.text, sizeof(src.text));
		id_talk = src.id_talk;
		num_window = src.num_window;

		//create num_window window, and then set the value.
		if( !num_window )
		{
			windows = NULL;
		}
		else
		{
			//create the windows, and if the windows isnot NULL, then delete it.
			if( windows )
			{
				delete []windows;
				windows = NULL;
			}

			windows = new window[num_window];
			if( !windows )
			{
				//write log: failed to allocate memory!
				return *this;//?something unreasonable?
			}

			//copy the data from the src talk_proc.
			for(int i = 0; i < num_window; ++i )
			{
				windows[i] = src.windows[i];
			}
		}

		//return the talk_proc instance.
		return *this;
	}

	//Added end.
};

///////////////////////////////////////////////////////////////////////////////////////
// 系统配置文件类模板
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
// 阵营敌对列表
///////////////////////////////////////////////////////////////////////////////////////
struct ENEMY_FACTION_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// 名称，最多15个汉字

	unsigned int	enemy_factions[32];		// 0 - 普通玩家的敌对阵营列表
											// 1 - 白名状态的敌对阵营列表
											// 2 - 红名状态的敌对阵营列表
											// 3 - 蓝名状态的敌对阵营列表
											// 4 - 城战攻方的敌对阵营列表
											// 5 - 城战守方的敌对阵营列表
											// 6 - 城战攻方友军的敌对阵营列表
											// 7 - 城战守方友军的敌对阵营列表
											// 8 - 普通怪物的敌对阵营列表
											// 9 - 普通NPC(功能、任务、守卫)的敌对阵营列表
											// 10 - 怪物派系1的敌对阵营列表
											// 11 - 怪物派系2的敌对阵营列表
											// 12 - 32 空缺的敌对阵营列表
};

///////////////////////////////////////////////////////////////////////////////////////
// 职业基础属性列表
///////////////////////////////////////////////////////////////////////////////////////
struct CHARACTER_CLASS_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// 名称，最多15个汉字

	unsigned int	character_class_id;		// 职业id, 可能的有:0-新手, 1-术士, 2-武士, 3-战士, 4-剑士, 5-牧师, 6-法师, 7-将军, 8-重骑士, 9-刺客, 10-剑圣, 11-贤者, 12-祭司, 13-秘法师, 14-召唤使
	
	unsigned int	faction;				// 该职业的所属阵营
	unsigned int	enemy_faction;			// 该职业的敌对阵营

	int				level_required;			// 转至本职业所需要的最小等级
	unsigned int	sect_mask;				// 门派归属
	int				level2;					// 修真等级

	int				hp;						// 初始生命力
	int				mp;						// 初始魔力
	int				dmg;					// 初始攻击力
	int				defense;				// 初始防御力
	int				attack;					// 初始命中
	int				armor;					// 初始闪躲
	float			crit_rate;				// 初始致命一击率(%)
	float			crit_damage;			// 初始致命一击伤害(%)
	int				anti_stunt;				// 初始眩晕抗性
	int				anti_weak;				// 初始虚弱抗性
	int				anti_slow;				// 初始缓慢抗性
	int				anti_silence;			// 初始沉默抗性
	int				anti_sleep;				// 初始睡眠抗性
	int				anti_twist;				// 初始缠绕抗性

	float			lvlup_hp;				// 每升一级所增长的生命力
	float			lvlup_mp;				// 每升一级所增长的魔力
	float			lvlup_dmg;				// 每升一级所增长的攻击力
	float			lvlup_defense;			// 每升一级所增长的防御力
	float			lvlup_attack;			// 每升一级所增长的命中
	float			lvlup_armor;			// 每升一级所增长的闪躲
	float			lvlup_crit_rate;		// 每升一级所增长的致命一击率(%)
	float			lvlup_crit_damage;		// 每升一级所增长的致命一击伤害(%)
	float			lvlup_anti_stunt;		// 每升一级所增长的眩晕抗性
	float			lvlup_anti_weak;		// 每升一级所增长的虚弱抗性
	float			lvlup_anti_slow;		// 每升一级所增长的缓慢抗性
	float			lvlup_anti_silence;		// 每升一级所增长的沉默抗性
	float			lvlup_anti_sleep;		// 每升一级所增长的睡眠抗性
	float			lvlup_anti_twist;		// 每升一级所增长的缠绕抗性

	float			walk_speed;				// 行走速度(M/S)
	float			run_speed;				// 奔跑速度(M/S)

	int				hp_gen1;				// 非战斗状态中HP恢复速度
	int				hp_gen2;				// 非战斗状态中打坐HP恢复速度
	int				hp_gen3;				// 战斗状态中HP恢复速度
	int				hp_gen4;				// 战斗状态中打坐HP恢复速度
	int				mp_gen1;				// 非战斗状态中MP恢复速度
	int				mp_gen2;				// 非战斗状态中打坐MP恢复速度
	int				mp_gen3;				// 战斗状态中MP恢复速度
	int				mp_gen4;				// 战斗状态中打坐MP恢复速度

	UInt64			character_combo_id;		// 可以转至的职业组合, 从低位到高位的含义为：0-新手, 1-术士, 2-武士, 3-战士, 4-剑士, 5-牧师, 6-法师, 7-将军, 8-重骑士, 9-刺客, 10-剑圣, 11-贤者, 12-祭司, 13-秘法师, 14-召唤使
	UInt64			character_combo_id2;	// 可以转至的职业组合, 扩展职业ID后的新职业掩码， Added 2011-07-14.
	
	int				xp_skill;				// 角色群怪技能
};

///////////////////////////////////////////////////////////////////////////////////////
// 封神人物属性表
///////////////////////////////////////////////////////////////////////////////////////
struct DEITY_CHARACTER_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// 名称，最多15个汉字

	struct  
	{
		int				deity_point;			// 封神等级达到x时的神力值
		int				hp;						// 封神等级达到x时增加的生命力
		int				mp;						// 封神等级达到x时增加的魔力
		int				dmg;					// 封神等级达到x时增加的攻击力
		int				defense;				// 封神等级达到x时增加的防御力
		int				deity_power;			// 封神等级达到x时增加的神圣力（可以认为初始为0）
		int				anti_stunt;				// 封神等级达到x时增加的眩晕抗性
		int				anti_weak;				// 封神等级达到x时增加的虚弱抗性
		int				anti_slow;				// 封神等级达到x时增加的缓慢抗性
		int				anti_silence;			// 封神等级达到x时增加的沉默抗性
		int				anti_sleep;				// 封神等级达到x时增加的睡眠抗性
		int				anti_twist;				// 封神等级达到x时增加的缠绕抗性

		int				dp_gen1;				// 非战斗状态中DP恢复速度
		int				dp_gen2;				// 非战斗状态中打坐DP恢复速度
		int				dp_gen3;				// 战斗状态中DP恢复速度
		int				dp_gen4;				// 战斗状态中打坐DP恢复速度
		int				talent_point;			// 封神等级达到x获得的天书点数
		int				skill_point;			// 封神等级达到x获得的技能点数
	}deity_config[100];
};

///////////////////////////////////////////////////////////////////////////////////////
// 数据修正表，对游戏中计算公式中某些参数的修正
///////////////////////////////////////////////////////////////////////////////////////
struct PARAM_ADJUST_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// 名称，最多15个汉字

	unsigned int    adjust_config_id;		//修正表id，为了使飞升前和飞升后的玩家使用不同的修正表
	struct
	{
		int			level_diff;				// Player和怪物的级别差(Player-怪物)(当级别差为0时，表示此条失效)
		float		adjust_exp;				// 对杀怪获得经验的修正系数(0.0~1.0)
		float		adjust_money;			// 对杀怪获得金钱掉落数目的修正系数(0.0~1.0)
		float		adjust_matter;			// 对杀怪掉落物品的概率的修正(0.0~1.0)
		float		adjust_attack;			// 对攻击力的等级惩罚系数(0.0~1.0)
	} level_diff_adjust[16];				// 级别差对杀怪计算修正			

	struct
	{
		float		adjust_exp;				// 对杀怪获得经验的修正系数(0.0~1.0)
	} team_adjust[11];						// 组队杀怪时的计算修正
	
	struct
	{
		float		adjust_exp;				// 对杀怪获得经验的修正系数(0.0~1.0)
	} team_profession_adjust[9];			// 队伍中职业个数计算修正

	struct			
	{
		int			level_diff;				// 生产等级和生产装备的级差
		float		adjust_exp;				// 对生产获得生产经验的系数修正（0.0～1.0）
	} level_diff_produce[9];
};

///////////////////////////////////////////////////////////////////////////////////////
// 交友平台配置表
///////////////////////////////////////////////////////////////////////////////////////
struct PIE_LOVE_CONFIG
{
	unsigned int	id;								// id
	namechar		name[32];						// 名称，最多15个汉字

	struct  
	{
		int				fee_press_msg;				//发布信息消耗金钱
		int             id_press_msg_item;			//发布信息消耗物品id
		int				id2_press_msg_item;			//发布消息消耗物品id2
		int				id_press_msg_task;			//发布信息触发任务id
		int				id_support_msg_item;		//投票支持信息消耗物品id
		int				id_support_msg_task;		//投票支持信息触发任务id
		int				id_against_msg_item;		//投票反对信息消耗物品ID
		int				id_against_msg_task;		//投票反对信息触发任务ID
		int				fee_apply_msg;				//申请信息消耗金钱
		int				id_apply_msg_item;			//申请信息消耗物品ID
		int				id_apply_msg_task;			//申请信息触发任务ID
		int				fee_response_msg;			//留言板回复信息消耗金钱
		int				id_response_msg_item;		//留言板回复信息消耗物品ID
		int				id_accept_apply_task1;		//接受申请触发任务ID1
		int				id_accept_apply_task2;		//接受申请触发任务ID2
		int				id_accept_apply_task3;		//接受申请触发任务ID3
	} sns_config[5];
	// 4个分别是 征婚、征友、收徒、帮派招人、知道提问	
};

///////////////////////////////////////////////////////////////////////////////////////
// 玩家升级曲线表
///////////////////////////////////////////////////////////////////////////////////////
struct PLAYER_LEVELEXP_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// 名称，最多15个汉字

	double			exp[200];				// 升级至第1~200级每级所需要的经验值
	double			exp_lev_1[200];			// 1转升级至第1~200级每级所需要的经验值
	double			exp_lev_2[200];			// 2转升级至第1~200级每级所需要的经验值
	double			exp_lev_3[200];			// 3转升级至第1~200级每级所需要的经验值
	int				talisman_exp[100];		// 法宝升级至第1~100级每级所需要的经验值

	int				prod_exp_need[10];			// 生产等级升至1~10级需要经验
	int				prod_exp_gained[10][10];	// 生产XX级装备获得的经验, 可通过 prod_exp_gained[当前段数][配方段数]获取生产经验数值
	double			pet_exp[200];				// 宠物升级至第1~200级每级所需要的经验值
	
	struct  
	{
		double	deity_exp;						// 元神升级需要的封神修为
		float	levelup_prob;					// 元神升级的概率
		double	failure_exp;					// 元神升级失败扣除的封神修为
	}deity[100];	
};

struct PLAYER_SECONDLEVEL_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// 名称，最多15个汉字

	float			exp_lost[256];			// 经验损失表
};

struct PLAYER_TALENT_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// 名称，最多15个汉字

	int				talent_point[200];
	int				talent_point_1[200];
	int				talent_point_2[200];
	int				talent_point_3[200];
};

struct PLAYER_SKILL_POINT_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// 名称，最多15个汉字

	int				skill_point[200];
	int				skill_point_1[200];
	int				skill_point_2[200];
	int				skill_point_3[200];
};

struct TITLE_PROP_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// 名称，最多15个汉字

	unsigned int	id_title;				// 称谓id
	unsigned int	id_addons[3];			// 称谓附加属性
};


/////////////////////////////////////////////////////////////////////////////////////////
// 特殊ID定义表
/////////////////////////////////////////////////////////////////////////////////////////
struct SPECIAL_ID_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// 名称，最多15个汉字
	
	unsigned int	version;					// elementdata version used by game designers	
	int			id_tmpl_jinfashen_config;	// 金身法身逆向兑换配置表id
	float		monster_drop_prob;			// 怪物掉落金钱概率
	int			id_money_matter;			// 金钱物品的id
	int			id_speaker;					// 千里传音的id
	int			id_destroying_matter;		// 摧毁物品的id
	int			id_townscroll;				// 传送遁地符的id
	int			id_attacker_droptable;		// 攻方玩家死亡掉落表
	int			id_defender_droptable;		// 守方玩家死亡掉落表
	int			id_talisman_reset_matter;	// 法宝洗练符
	int			fee_talisman_merge;			// 法宝熔炼手续费
	int			fee_talisman_enchant;		// 法宝灌魔手续费
	int			id_war_material1;			// 城战物资1需要ID
	int			id_war_material2;			// 城战物资2需要ID
	int			fee_pet_gained;				// 宠物驯养手续费
	int			fee_pet_free;				// 宠物放生手续费
	int			fee_pet_refine;				// 宠物修炼手续费
	int			fee_pet_rename;				// 宠物改名手续费
	int			id_male_war_statue;			// 男城战雕像id
	int			id_female_war_statue;		// 女城战雕像id
	int			id_vehicle_upgrade_item;	// 驯化物品
	int			fee_lock;					// 加锁费用
	int			fee_unlock;					// 解锁费用
	int			unlock_item_id;				// 解锁道具id
	int			unlocking_item_id;
	int			id_damaged_item;			// 破损的物品
	int			id_repair_damaged_item;		// 修复破损的物品
	int			id_bleed_identity_host_item;// 滴血认主道具id
	int			fee_restore_soul;			// 恢复每点魄力的手续费
	int			id_lowgrade_soul_stone;		// 低级元魂珠
	int			id_highgrade_soul_stone;	// 高级元魂珠
	int			id_enter_arena_item;		// 普通竞技场报名物品id
	int			id_enter_arena_reborn_item;	// 飞升竞技场报名物品id
	int			id_change_face_item;		// 更换形象消耗道具的id
	int			id_speaker2;				// 千里传音id2 
	int			id_unique_bid_item;			// 唯一最低价拍卖的出价符

	//Added 2011-03-10.
	int			fee_gem_refine;				// 宝石精炼手续费
	int			fee_gem_extract;			// 宝石萃取手续费
	int			fee_gem_tessellation;		// 宝石镶嵌手续费
	int			fee_gem_single_dismantle;	// 宝石单次拆除手续费
	int			fee_gem_smelt;				// 宝石熔炼手续费
	int			fee_gem_slot_identify;		// 宝石插槽鉴定手续费
	int			fee_gem_slot_customize;		// 宝石插槽定制手续费
	int			fee_gem_slot_rebuild;		// 宝石插槽重铸手续费
	int			gem_upgrade_upper_limit;	// 宝石升品上限
	int			id_gem_smelt_article;		// 宝石熔炼道具ID
	int			id_gem_smelt_article_1;		// 宝石熔炼道具ID，可为非绑定道具等 Added 2011-04-01
	int			id_gem_smelt_article_2;		// 宝石熔炼道具ID，可作为其他特殊用处 Added 2011-04-01
	int			id_gem_refine_article;		// 宝石精炼道具ID
	int			id_gem_refine_article_1;	// 宝石精炼道具ID, 可作为非绑定道具等 Added 2011-04-01
	int			id_gem_refine_article_2;	// 宝石精炼道具ID, 可作为其他特殊用处道具 Added 2011-04-01
	int			id_gem_slot_lock_article;	// 宝石插槽重铸时锁定道具ID
	int			id_gem_slot_lock_article_1;	// 宝石插槽重铸时锁定道具ID, 可作为非绑定道具等 Added 2011-04-01
	int			id_gem_slot_lock_article_2;	// 宝石插槽重铸时锁定道具ID, 其他特殊用处道具 Added 2011-04-01
	int			id_gem_slot_rebuild_article[3];//宝石插槽重铸所需道具ID,有三项，可作普通，非绑定，特殊用途等 Added 2011-05-03
	int			id_consign_role_item;		//角色寄售所需消耗的物品ID	Added 2012-03-23.
	int			consign_role_item_count;	//角色寄售所需消耗物品的数量	Added 2012-03-23.
	int			consign_role_type;			//角色寄售所需要使用类别 Added 2012-03-23.
	int			id_treasure_region_upgrade_item;	//诛仙古卷挖宝区域升级所需要消耗的物品的ID Added 2012-03-23.
	
	int			id_speaker_special[3];		// 大喇叭id
	int			id_speaker_special_anony[3];	// 匿名大喇叭id
	int			id_change_name[3];			// 改名道具id
	int			id_change_name_family[3];	// 家族改名道具id
	int			id_change_name_guild[3];	// 帮派改名道具id

	// 星座相关
	int			id_equip_hole[3];			// 装备打孔道具（镶嵌星座）
	int			id_xingzuo_levelup[3];		// 星座升级道具
	int			fee_xingzuo_add;			// 星座镶嵌手续费
	int			fee_xingzuo_remove;			// 星座摘除手续费
	int			fee_xingzuo_identify;		// 星座鉴定手续费

	int			id_fix_prop_lose;			// 修复天残号(未满150飞升)所需道具id
	int			id_rose_free;				// 免费玫瑰花id
	int			id_rose_money;				// 元宝玫瑰花id
	
	int			id_rename_equip_Props1;		// +12装备重命名道具1id
	int			id_rename_equip_Props2;		// +12装备重命名道具2id
	
	int			id_rune2013_fragment[2];	// 符文碎片id
	int			id_rune2013_erase[2];		// 移除符文id
	int			id_rune2013_merge_extra_num[2];	// 额外符文融合次数id
};

/////////////////////////////////////////////////////////////////////////////////////////
// 转生技能模板
/////////////////////////////////////////////////////////////////////////////////////////
struct RENASCENCE_SKILL_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// 名称，最多15个汉字

	UInt64			occup_lev_0;			// shit！敢加上注释不？以后别人要修改代码都找不到！！！
	UInt64			occup_lev_0_2;			// 0 转职业所限制的职业掩码，扩展职业ID后的新职业掩码，Added 2011-07-14.
	UInt64			occup_lev_1;
	UInt64			occup_lev_1_2;			// 1 转职业所限制的职业ID掩码，扩展职业ID后的新职业掩码，Added 2011-07-14.
	UInt64			occup_lev_2;
	UInt64			occup_lev_2_2;			// 2 转职业所限制的职业ID掩码，扩展职业ID后的新职业掩码，Added 2011-07-14.
	UInt64			occup_lev_3;
	UInt64			occup_lev_3_2;			// 3 转职业所限制的职业ID掩码，扩展职业ID后的新职业掩码，Added 2011-07-14.

	struct
	{
		int id;
		int lev;
	} skills[12];
};

struct RENASCENCE_PROP_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// 名称，最多15个汉字

	UInt64			occup_lev_0;			// 1: 这样的变量命名尽量避免，不知其意， 2：不加注释很不好。。。
	UInt64			occup_lev_0_2;			// 0 转职业所限制的职业掩码，扩展职业ID后的新职业掩码，Added 2011-07-14
	UInt64			occup_lev_1;
	UInt64			occup_lev_1_2;			// 1 转职业所限制的职业掩码，扩展职业ID后的新职业掩码，Added 2011-07-14
	UInt64			occup_lev_2;
	UInt64			occup_lev_2_2;			// 2 转职业所限制的职业掩码，扩展职业ID后的新职业掩码，Added 2011-07-14
	UInt64			occup_lev_3;
	UInt64			occup_lev_3_2;			// 3 转职业所限制的职业掩码，扩展职业ID后的新职业掩码，Added 2011-07-14

	float			hp[15];
	float			mp[15];
	float			dmg[15];
	float			def[15];
	float			attack[15];
	float			armor[15];
	float			crit_rate[15];
	float			crit_damage[15];
	float			anti[6][15];
};

/*
	物物交易
*/

struct ITEM_TRADE_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// 名称，最多15个汉字

	struct
	{
		namechar		page_title[8];			// 页的名字，最多7个汉字

		struct
		{
			unsigned int	id_goods;
			unsigned int	goods_num;	// 对应商品的打包数量（一组包含n个）

			struct
			{
				unsigned int	id;
				int				count;
			} item_required[2];

			struct
			{
				unsigned int	id;
				int				count;
			} repu_required[2];

			struct 
			{
				unsigned int  type;  // 耗费：仙基值 or ...
				int			  count;
			}special_required[1];

		} goods[48];
	} pages[4];

	unsigned int	id_dialog;					// 对话ID
};

//器魄融合配方
struct EQUIP_SOUL_MELD_SERVICE
{
	unsigned int	id;						// id
	namechar		name[32];				// 名称，最多15个汉字
	unsigned int	prescription[500];		// 配方
};

//器魄
struct EQUIP_SOUL_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径

	unsigned int	equip_mask;					// 可镶嵌部位	
	int				level_required;				// 等级限制
	int				soul_type;					// 器魄种类，（0-9）
	int				soul_level;					// 器魄本身的级别，（0-16），默认为0，表示器魄的档次
	UInt64			character_combo_id;			// 职业限制, 从低位到高位的含义为：0-新手, 1-术士, 2-武士, 3-战士, 4-剑士, 5-牧师, 6-法师, 7-将军, 8-重骑士, 9-刺客, 10-剑圣, 11-贤者, 12-祭司, 13-秘法师, 14-召唤使
	UInt64			character_combo_id2;		// 职业限制, 扩展职业ID后的新职业掩码，Added 2011-07-14.
	unsigned int	sect_mask;					// 可镶嵌的门派 客户端显示
	int				require_race;				// 种族限制，0-人族，1-兽族， 2-都可以
	int				hole_type;					// 孔位		0:全部；1：金  2：木 3：水 4：火 5：土
	int				id_addon1;					// 效果附加属性
	int				cost_soul;					// 占用的魂力

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	float			fee_install;				// 铸魂手续费
	float			fee_uninstall;				// 洗魂手续费
	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

// 特殊命名的物品
struct SPECIAL_NAME_ITEM_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径名

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

// 宋金战场回收物品配置表
struct RECYCLE_ITEM_CONFIG
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				map_id;						// 对应的地图号

	struct
	{
		int id_obj;
	}recycle_item_list[200];
};


// 宋金战场积分与军衔的对应表
struct SCORE_TO_RANK_CONFIG
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				map_id;						// 对应的地图号
	
	int				controller;					// 战场开启激活的控制器
	struct
	{
		int score;								//  该军衔所需积分
		int task_id;							//  战场结束后，该军衔触发的任务id
		namechar rank_name[16];					//  军衔名称，最多15个汉字
	}score2rank[10];
};

// 宋金战场死亡掉落配置表
struct BATTLE_DROP_CONFIG
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				map_id;						// 对应的地图号

	struct										// 掉落物品列表
	{
		unsigned int	id_obj;					// 掉落物品id
		float			probability;			// 掉落概率

	} drops[10][10];                            // 前面参数是军衔序号：军衔1-军衔10，后面是掉落物品id序号id1-id10
};

// 宋金战场死亡掠夺配置表
struct BATTLE_DEPRIVE_CONFIG
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				map_id;						// 对应的地图号
	
	struct 
	{
		unsigned int id_obj;					//  掠夺物品的id
		unsigned int max_num;					//	掠夺该物品的数目上限
	}deprive[10];

};

// 宋金战场积分获取配置表
struct BATTLE_SCORE_CONFIG
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				map_id;						// 对应的地图号

	struct 
	{
		unsigned int score;						// 获取的积分
	}scores[10];								// 杀死军衔1-10级获取的积分

	struct 
	{
		unsigned int ext_score;
	}ext_score[8];								// 连续杀死3-10人获取的额外积分						

	struct 
	{
		unsigned int monster_id;
		unsigned int monster_score;				
	}monster_scores[50];						// 杀死特定id的怪物获取的积分
	
	unsigned int   victory_score;				// 获得最终胜利获取的积分
};

// 礼包
struct GIFT_BAG_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径

	int				level_everLimit;			// 曾经的等级的下限（适合飞升之后的判断）
	int				level_required;				// 当前等级下限
	int				level_upperLimit;			// 当前等级上限
	unsigned int	require_gender;				// 性别限制，0-男，1-女，2-男女都可
	int				renascence_count;			// 转生次数要求
	UInt64			character_combo_id;			// 职业限制
	UInt64			character_combo_id2;		// 职业限制, 扩展职业ID后的新职业掩码，Added 2011-07-14.
	unsigned int	god_devil_mask;				// 仙魔 造化限制仙/佛/魔/备选1/2/3
	unsigned int    open_time;					// 开启需要的时间
	float			probability;				// 开启成功的概率
	unsigned int	success_disappear;			// 成功开启后消失(1为是，0为否)
	unsigned int	faliure_disappear;			// 开启失败后消失(1为是，0为否)
	unsigned int	id1_object_need;			// 开启需要物品1
	unsigned int	id1_object_num;				// 开启需要物品1的数量
	unsigned int	id2_object_need;			// 开启需要物品2
	unsigned int	id2_object_num;				// 开启需要物品2的数量
	int				money_need;					// 开启需要金钱
	unsigned int	num_object;					// 内含物品的总数

	struct 
	{
		unsigned int	id_object;				// 物品id
		float			probability;			// 物品产出概率
		unsigned int	num_min;				// 物品的数量下限
		unsigned int	num_max;				// 物品的数量上限
		unsigned int	is_bind;				// 物品是否绑定
		unsigned int	effect_time;			// 物品的时效（单位为秒，0为永久）
	}gifts[16];
	
	unsigned int	normalize_group[4];

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
	
};

// 特殊命名的物品
struct VIP_CARD_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径名

	int				vip_type;					// vip 卡的类型：白银卡（1），黄金卡（2），白金卡（3）……
	int				expires;					// 有效期
	
	float			multiple_exp;				// 经验加成

	unsigned int	free_helper;				// 免费挂机
	unsigned int	no_exp_drop;				// 死亡后不掉经验

	struct 
	{
		int skill_id;							// 技能物品id
		int skill_level;						// 技能物品等级
	}buff[8];

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

struct INSTANCING_BATTLE_CONFIG
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名字，最多15个汉字

	int				map_id;						// 战场id
	namechar		desc[16];					// 战场说明
	int				file_map;					// 地图存储路径
	int				active_fee;					// 激活花费金钱
	int				active_item;				// 激活战场的道具
	int				apply_time;					// 报名时间
	int				run_time;					// 进行时间
	int				renascence_count;			// 转生次数要求
	int				level_lower_limit;			// 等级下限
	int				level_upper_limit;			// 等级上限
	int				max_player;					// 战场人数上限
	int				open_player_num;			// 提前开启人数下限
	int				open_player_num2;			// 自动开启人数下限
	int				victory_controller;			// 战场胜利条件，控制器被激活
	int				reopen_time;				// 重新开启时间间隔
	int				relation_limit;				// 进入战场限制
	int				sect_limit;					// 门派限制
	int				forbid_food;				// 禁食限制
	int				valid_tollgate_num;			// 有效关卡的数量

	struct
	{
		namechar	tollgate_name[16];			// 关卡名称
		int			score;						// 积分，过关时奖励的积分
		int			time_limit;					// 时间	
		int			score_per_sec;				// 每分钟对应分数
		namechar	introduce[16];				// 简介
		int			open_controller;			// 开启条件，控制器被开启
		int			finish_controller;			// 完成控制器
		int			death_penlty;				// 死亡惩罚的分数
	}tollgate[20];

	struct 
	{
		int			monster_id;					// 怪物id
		int			score;						// 怪物积分
	}monster[20];

	struct 
	{
		int			victory_score_level;
		int			victory_task_id;
		int			failure_score_level;
		int			failure_task_id;
	}award_level[5];
};

///////////////////////////////////////////////////////////////////////////////
//基本类型副本配置表
//Added 2011-07-12.
///////////////////////////////////////////////////////////////////////////////
struct TRANSCRIPTION_CONFIG
{
	unsigned int		id;						// ID
	namechar			name[32];				// 副本类名称

	int					max_finish_count;		// 副本每天完成次数限制
	int					map_id;					// 副本地图编号id
	int					open_room_item_id;		// 开启副本房间所需要的物品id
	int					open_room_item_count;	// 开启副本房间所需要的物品数量
	int					room_active_time;		// 开启房间时限，时间到并满足条件将开启副本
	int					player_min_level;		// 进入副本玩家的等级下限
	int					player_max_level;		// 进入副本玩家的等级上限
	UInt64				character_combo_id;		// 进入副本玩家的职业限制，profession_id:0 -- 63
	UInt64				character_combo_id2;	// 进入副本玩家的职业限制，profession_id:64 -- 127
	unsigned int		god_devil_mask;			// 进入副本玩家的造化限制，仙/佛/魔：1/2/3
	int					renascence_count;		// 进入副本玩家的转生次数限制，0表示没有限制
	int					required_race;			// 种族限制，0-人族，1-兽族， 2-都可以
	int					required_money;			// 进入副本所需要收取的金钱
	
	struct
	{
		int		reputation_type;				// 所需要的声望类型id
		int		reputation_value;				// 所需要该类型的声望值
	}required_reputation[4];					// 进入副本所需要的声望类型和声望值
	
	int					required_item_id;		// 进入副本所需要的物品id
	int					required_item_count;	// 进入副本所需要的物品数量
	int					is_item_need_consumed;	// 所需要的物品是否消耗， 0：不消耗；1：消耗
	int					min_player_num;			// 开启副本的人数下限
	int					max_player_num;			// 开启副本的人数上限
	int					invincible_time;		// 传入副本后无敌等待时间，从副本正式开启时开始计时
	int					wait_time_before_leave;	// 死亡传出副本的等待时间
	int					total_exist_time;		// 副本从开启到自动关闭的总时间，0：表示没有时间限制
	int					controller_id;			// 副本胜利条件控制器id，控制器激活，副本结束
	int					award_task_id;			// 副本胜利奖励任务id
	int					forbiddon_items_id[10];	// 禁用物品id列表
	int					forbiddon_skill_id[10];	// 禁用技能id列表

	int					map_variable_id[20];	// 副本地图全局变量id列表 Added 2012-04-09.
	
	int					level_controller_id[10];// 不同难度对应的控制器ID Added 2012-08-20.
	
	int					strategy[10];			// 副本攻略配置表
};


struct CHANGE_SHAPE_CARD_ESSENCE 
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径名
						
	int				require_level;				// 等级限制
	int				require_sect;				// 门派限制
	
	UInt64			character_combo_id;			// 职业限制
	UInt64			character_combo_id2;		// 职业限制, 扩展职业ID后的新职业掩码，Added 2011-07-14.
	int				renascence_count;			// 转生次数要求		
	int				require_gender;
	int				repu_index;					// 区域声望的序号
	int				require_repu;				// 区域声望值限制
	int				ref_temp_id;				// 引用的属性模板的id
	namechar		preface[16];				// 变身开场白
	namechar		epilogue[16];				// 变身结语
	float			energy_decrease_speed;		// 精力下降速度
	int				fee_restore;				// 归元
	int				fee_refine;					// 强化
	int				fee_contract;				// 契合
	int				fee_recover_wakan;			// 恢复每点灵力费用	

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};


struct CHANGE_SHAPE_STONE_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径名
	
	int				refine_type;				// 炼化方式
	float			ext_succeed_prob;			// 提高成功概率
	float			ext_match_value;			// 提高契合值

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};


struct CHANGE_SHAPE_PROP_CONFIG
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				monster_id;					// 对应的变身怪物id
	int				action_type;				// 动作方案（精英怪or普通怪）
	int				is_replace_name;			// 是否替换怪物名字
	int				move_type;					// 控制移动的方式（1-变身后可以控制移动；2-无法控制移动；3-随机走动）
	float			recover_prop;				// 受到攻击的复原概率 
	int				change_faction;				// 变身后是否改变阵营 0：不改变 1：改变（改变结果取下面的两个值）
	unsigned int	faction;					// 变身后的所属阵营
	unsigned int	enemy_faction;				// 变身后的敌对阵营	
	int				shield_skill;				// 变身后屏蔽技能
	int				can_use_medicine;			// 可否加红蓝药
	int				can_run_helper;				// 可否天人合一	
	int				can_fight;					// 是否可战斗
	int				is_fly;						// 是否飞行类变身
	int				hp_cost;					// 消耗气血
	int				mp_cost;					// 消耗真气
	int				is_equip_invalid;			// 装备是否失效 0：不失效 1：失效
	int				is_replace_prop;			// 是否替换人物本体属性 0：不替换，1：替换
	int				is_buff_positive;			// 变身buff的效果，0：负面效果 1：正面效果

};

struct ORIGINAL_SHAPE_CONFIG
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字
	int				table_id;					// 显形规则表id
	struct  
	{
		int monster_id;
		struct  
		{
			int		original_type;				// 原形的类型(0:npc, 1:monster)
			int		original_id;				// 怪物原形的id
			float	probability;				// 怪物显形的概率
		}original[3];

	}shape_map[20];
};

// 生活技能配置表
struct LIFE_SKILL_CONFIG
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	struct  
	{
		UInt64	character_combo_id;				// 职业限制
		UInt64	character_combo_id2;			// 职业限制, 扩展职业ID后的新职业掩码，Added 2011-07-14.
		int		require_gender;					// 性别限制
		int		renascence_count;				// 飞升次数限制
		int		require_level;					// 等级限制
		int		remove_renascence;				// 失去技能飞升次数要求
		int		remove_level;					// 失去技能人物等级
		int		skill_id;						// 技能id
		int		skill_level;					// 技能等级
	}skill_config[32];
};

struct ARENA_DROPTABLE_CONFIG 
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字
	
	int				mapid;						// 地图号

	struct  
	{
		int		itemid;							// 掉落物品id
	}droptable[10];
};

struct MERCENARY_CREDENTIAL_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径名
	
	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

// 星盘，传送阵
struct TELEPORTATION_ESSENCE
{
	unsigned int	id;							// (类型)ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径

	int				max_tele_point;				// 所能记录的传送点个数的上限
	int				default_day;				// 初始默认的有效天数

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

// 星盘充能石
struct TELEPORTATION_STONE_ESSENCE
{
	unsigned int	id;							// (类型)ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径

	int				day;						// 充能石能量（天数）			

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

// 组合技能编辑冷却的配置
struct COMBINE_SKILL_EDIT_CONFIG
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字
	
	struct  
	{
		int		skill_id;
		int		cool_index;
		int		cool_time;
	}edit_config[30];
};

// 特定装备通过服务进行升级的配置表
struct UPGRADE_EQUIP_CONFIG
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字
	
	struct  
	{
		int		equip_id;						// 装备id
		int		stone_id;						// 模具id
		int		num;							// 所需模具数量
		int		out_equip_id;					// 产出的武器id
	}upgrade_config[200];
};

// 特定装备通过服务进行升级的配置表（新）
// 与老的区别：新的升级精炼保留道具12级封顶，老的不封顶
struct UPGRADE_EQUIP_CONFIG_1
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字
	
	struct  
	{
		int		equip_id;						// 装备id
		int		stone_id;						// 模具id
		int		out_equip_id;					// 产出的武器id
	}upgrade_config[200];
};

// 特定装备升级中使用的精炼道具
struct UPGRADE_EQUIP_STONE_ESSENCE
{
	unsigned int	id;							// (类型)ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径

	int				type;						// 类型：0普通1特殊			
	int				level;						// 等级：只对特殊的有效，+X必成
	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID

};

///////////////////////////////////////////////////////////////////////////////
//宝石品级品质配置表
//作用：对宝石的上一级/下一级品级和品质的宝石id的配置表
///////////////////////////////////////////////////////////////////////////////
struct GEM_CONFIG
{
	unsigned int			id;				//模板ID
	namechar				name[32];		//名称，最多15个汉字
	struct
	{
		unsigned int	ordinary_gem_id;	//普通品质宝石id
		unsigned int	perfect_gem_id;		//完美品质宝石id
	}gem_id[10];							//分别表示各品级宝石所对应的id

};

///////////////////////////////////////////////////////////////////////////////
//宝石模板
///////////////////////////////////////////////////////////////////////////////
struct GEM_ESSENCE
{
	unsigned int			id;				//模板ID
	namechar				name[32];		//名称，最多15个汉字

	int						file_matter;	//掉在地上的模型路径
	int						file_icon;		//图标路径
	int						file_shape_icon;//宝石形状图标路径

	int						shape_type;		//宝石的形状类型，目前为0 - 11，共12类
	int						grade;			//宝石品级，升品上限在特殊ID模板中
	int						quality;		//宝石品质，同一品级有不同品质，当前0：普通品质，1：完美品质
	unsigned int			gem_config_id;	//宝石上/下品级和品质的配置模板id

	//宝石附加属性，包括基本属性和镶嵌属性,共16个
	struct
	{
		int		addon_type;		//附加属性的类型，0:基本属性，1:镶嵌属性
		int		addon_id;		//附加属性模板id
	}addon_property[6];			//目前暂定最多6*3=18个附加属性,需要注意的是:要和策划约定好附加属性的填写顺序？

	//升品手续费
	int						fee_upgrade;	//指定进行宝石升品操作时扣除的游戏币数量
	unsigned int			gem_extract_config_id;	//宝石萃取后分解物配置模板ID

	int						price;			// 卖店价
	int						shop_price;		// 店卖价

	// 堆叠信息
	int						pile_num_max;	// 堆叠上限

	// 处理方式
	unsigned int			proc_type;		// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID

};

///////////////////////////////////////////////////////////////////////////////
//宝石魔印
///////////////////////////////////////////////////////////////////////////////
struct GEM_SEAL_ESSENCE
{
	unsigned int			id;				//模板ID
	namechar				name[32];		//名称，最多15个汉字

	int						file_matter;	//掉在地上的模型路径
	int						file_icon;		//图标路径	

	int						shape_type;		//魔印的形状属性,和宝石插槽属性相对应，目前共13种类型

	int						price;			// 卖店价
	int						shop_price;		// 店卖价

	// 堆叠信息
	int						pile_num_max;	// 堆叠上限

	// 处理方式
	unsigned int			proc_type;		// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID

};

///////////////////////////////////////////////////////////////////////////////
//宝石粉尘
///////////////////////////////////////////////////////////////////////////////
struct GEM_DUST_ESSENCE
{
	unsigned int			id;				//模板ID
	namechar				name[32];		//名称，最多15个汉字

	int						file_matter;	//掉在地上的模型路径
	int						file_icon;		//图标路径
	
	int						grade;			//当前宝石粉尘的品级

	//当前宝石粉尘对各品级宝石的升品概率
	struct
	{
		float	ordinary_upgrade_prob;		//对普通宝石的升品概率
		float	perfect_upgrade_prob;		//对完美宝石的升品概率
	}upgrade_prob[9];						//分别对应10级的升品概率，如1-2， 2-3， 3-4，……

	int						price;			// 卖店价
	int						shop_price;		// 店卖价

	// 堆叠信息
	int						pile_num_max;	// 堆叠上限

	// 处理方式
	unsigned int			proc_type;		// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID

};

///////////////////////////////////////////////////////////////////////////////
//宝石萃取产物的配置模板
///////////////////////////////////////////////////////////////////////////////
struct GEM_EXTRACT_CONFIG
{
	unsigned int			id;				//模板ID
	namechar				name[32];		//名称，最多15个汉字
	
	struct
	{
		unsigned int		gem_dust_id;	//生成的宝石粉尘ID
		int					gem_dust_num;	//生成的宝石粉尘的数量
		float				gem_dust_prob;	//生成该宝石粉尘的概率
	}gem_dust_config[3];

};

///////////////////////////////////////////////////////////////////////////////
//通用道具模板
//主要提供给各种无特殊需求（物品中无特殊属性字段）的通用道具使用
///////////////////////////////////////////////////////////////////////////////
struct GENERAL_ARTICLE_ESSENCE
{
	unsigned int			id;				//模板ID
	namechar				name[32];		//名称，最多15个汉字

	int						file_matter;	//掉在地上的模型路径
	int						file_icon;		//图标路径

	int						price;			// 卖店价
	int						shop_price;		// 店卖价

	// 堆叠信息
	int						pile_num_max;	// 堆叠上限

	// 处理方式
	unsigned int			proc_type;		// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID

};

//=============================================================================
//Added 2011-12-05.
///////////////////////////////////////////////////////////////////////////////
//在线奖励大礼包配置模板
//主要设置大礼包的发放条件以及每一个小礼包的发放时间段。
///////////////////////////////////////////////////////////////////////////////
struct ONLINE_GIFT_BAG_CONFIG
{
	unsigned int			id;							//模板ID
	namechar				name[32];					//名称，最多15个汉字

	int						file_matter;				//掉落在地上的模型路径
	int						file_icon;					//图标路径

	int						award_type;					//奖励的类型：0：活动奖励；1：每日奖励
	int						start_time_per_day;			//每日奖励时的开启时间
	int						end_time_per_day;			//每日奖励时的结束时间
	int						start_time_per_interval;	//活动奖励时的开启时间
	int						end_time_per_interval;		//活动奖励时的结束时间
	int						expire_time;				//奖励领取的截止时间，主要用于活动奖励类型，每日奖励每日0点清除

	int						renascence_count;			//转生次数要求
	int						require_min_level;			//等级下限
	int						require_max_level;			//等级上限
	unsigned int			require_gender;				//性别限制，0-男，1-女，2-男女都可
	UInt64					character_combo_id;			//职业限制，通过掩码实现
	UInt64					character_combo_id2;		//职业限制，扩展需求
	int						require_race;				//种族限制，0-人族，1-兽族， 2-都可以
	
	struct
	{
		int		gift_bag_id;							//小礼包id
		int		deliver_interval;						//小礼包发放间隔，都是以上一个小礼包发放后开始计算，时间单位：秒
	}small_gift_bags[16];								//最多16个小礼包
};

///////////////////////////////////////////////////////////////////////////////
//在线奖励小礼包配置模板
//主要包含相关发放物品的id，数量，绑定属性和时间限制等属性.
///////////////////////////////////////////////////////////////////////////////
struct SMALL_ONLINE_GIFT_BAG_ESSENCE
{
	unsigned int			id;							//模板ID
	namechar				name[32];					//名称，最多15个汉字

	int						file_matter;				//掉落在地上的模型路径
	int						file_icon;					//图标路径

	unsigned int			open_time;					//开启礼包需要的时间
	unsigned int			item_num;					//礼包内的物品个数

	struct
	{
		int				item_id;						//物品ID
		int				item_count;						//物品数量
		unsigned int	is_bind;						//物品是否绑定
		unsigned int	effect_time;					//物品的时效（单位为秒，0为永久）
	}item_configs[16];									//物品属性，最多16种物品
	
	int						price;						// 卖店价
	int						shop_price;					// 店卖价

	// 堆叠信息
	int						pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int			proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

//Added end.
//=============================================================================
//Added 2012-03-20.
///////////////////////////////////////////////////////////////////////////////
//诛仙古卷挖宝区域配置模板
//主要包含相关区域升级，区域概率，事件列表等属性
///////////////////////////////////////////////////////////////////////////////
struct SCROLL_REGION_CONFIG
{
	unsigned int			id;							//模板ID
	namechar				name[32];					//名称，最多15个汉字

	int						file_matter;				//区域图标
	int						file_icon;					//区域图片

	namechar				region_desc[200];			//区域描述 最多200汉字
	float					region_level_prob[6];		//区域等级初始出现概率，分别对应0-5等级时的概率，注意：概率总和为1
	float					region_level_up_prob[5];	//区域等级升级概率，分别对应1-5等级的升级概率

	struct
	{
		unsigned int	item_id;						//奖励物品ID
		unsigned int	item_count;						//奖励该ID的物品数量
		float			deliver_prob;					//发放该物品的概率
		int				event_id;						//对应奖励事件ID：0-普通奖励事件，1-9：特殊奖励事件
	}region_award_list[6][10];							//对应0-5(6个)等级的奖励列表，每个等级最多10种奖励

	int					region_pos;					//区域位置，一共12个，1-9为普通区域位置，10-12为隐藏区域位置
	struct
	{
		int		active_start_time;						//活动开启时间，年月日时分秒，--->最终以秒为单位，注意跨时区的问题
		int		active_stop_time;						//活动结束时间，年月日时分秒，--->最终以秒为单位，注意跨时区的问题
	}hide_region_active_time[3];						//对于隐藏区域，分别表示该区域的活动时间段，[活动开启时间，活动结束时间]，可以填写多段，最大为3段

	namechar				hide_region_active_desc[100];	//隐藏区域活动开启介绍
	unsigned int			hide_region_unlock_item_id;		//隐藏区域解锁道具ID

	struct
	{
		int			event_id;							//事件ID，0-普通， 1-9：特殊，策划填入，随意可重复
		int			event_icon;							//时间图片
		namechar	event_desc[100];					//事件描述
	}event_list[10];									//事件列表，最大10个事件信息
};

///////////////////////////////////////////////////////////////////////////////
//诛仙古卷挖宝解锁道具模板
//主要包含相关解锁区域的ID， 解锁区域位置等信息
///////////////////////////////////////////////////////////////////////////////
struct SCROLL_UNLOCK_ESSENCE
{
	unsigned int			id;							//模板ID
	namechar				name[32];					//名称，最多15个汉字

	int						file_matter;				//掉落在地上的模型路径
	int						file_icon;					//图标路径

	unsigned int			region_id;					//对应需要解锁的区域ID
	int						region_pos;					//对应需要解锁的区域POS，位置：1-9，普通 10-12：隐藏区域

	int						price;						// 卖店价
	int						shop_price;					// 店卖价

	// 堆叠信息
	int						pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int			proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

///////////////////////////////////////////////////////////////////////////////
//诛仙古卷挖宝增加探索次数道具模板
//主要包含相关可以增加探索次数等信息
///////////////////////////////////////////////////////////////////////////////
struct SCROLL_DIG_COUNT_INC_ESSENCE
{
	unsigned int			id;							//模板ID
	namechar				name[32];					//名称，最多15个汉字

	int						file_matter;				//掉落在地上的模型路径
	int						file_icon;					//图标路径

	int						dig_count;					//此道具可增加挖宝次数

	int						price;						// 卖店价
	int						shop_price;					// 店卖价

	// 堆叠信息
	int						pile_num_max;				// 堆叠上限

	// 处理方式
	unsigned int			proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

///////////////////////////////////////////////////////////////////////////////
//趴塔副本配置表
//主要包含一般副本的基本信息以及趴塔所独有的属性 Added 2012-03-29.
///////////////////////////////////////////////////////////////////////////////
struct TOWER_TRANSCRIPTION_CONFIG
{
	unsigned int		id;						// ID
	namechar			name[32];				// 副本类名称

	int					max_finish_count;		// 副本每天完成次数限制
	int					map_id;					// 副本地图编号id
	int					open_room_item_id;		// 开启副本房间所需要的物品id
	int					open_room_item_count;	// 开启副本房间所需要的物品数量
	int					room_active_time;		// 开启房间时限，时间到并满足条件将开启副本
	int					player_min_level;		// 进入副本玩家的等级下限
	int					player_max_level;		// 进入副本玩家的等级上限
	UInt64				character_combo_id;		// 进入副本玩家的职业限制，profession_id:0 -- 63
	UInt64				character_combo_id2;	// 进入副本玩家的职业限制，profession_id:64 -- 127
	unsigned int		god_devil_mask;			// 进入副本玩家的造化限制，仙/佛/魔：1/2/3
	int					renascence_count;		// 进入副本玩家的转生次数限制，0表示没有限制
	int					required_race;			// 种族限制，0-人族，1-兽族， 2-都可以
	int					required_money;			// 进入副本所需要收取的金钱
	
	struct REPUTATION_ITEM
	{
		int		reputation_type;				// 所需要的声望类型id
		int		reputation_value;				// 所需要该类型的声望值
	}required_reputation[4];					// 进入副本所需要的声望类型和声望值
	
	int					required_item_id;		// 进入副本所需要的物品id
	int					required_item_count;	// 进入副本所需要的物品数量
	int					is_item_need_consumed;	// 所需要的物品是否消耗， 0：不消耗；1：消耗
	int					min_player_num;			// 开启副本的人数下限
	int					max_player_num;			// 开启副本的人数上限
	int					invincible_time;		// 传入副本后无敌等待时间，从副本正式开启时开始计时
	int					wait_time_before_leave;	// 死亡传出副本的等待时间
	int					total_exist_time;		// 副本从开启到自动关闭的总时间，0：表示没有时间限制
	int					controller_id;			// 副本胜利条件控制器id，控制器激活，副本结束
	int					award_task_id;			// 副本胜利奖励任务id
	int					forbiddon_items_id[10];	// 禁用物品id列表
	int					forbiddon_skill_id[10];	// 禁用技能id列表

	int					tower_layer_count;		// 趴塔副本总层数
	int					opened_tower_layer_num;	// 趴塔副本开启层数
	int					tower_pass_count;		// 趴塔副本管卡数
	int					enable_skill_id[20];	// 趴塔副本内额外可用技能ID列表 最多20个
	int					property_config_id[100];// 趴塔副本每层属性配置表ID 最多100层
};


///////////////////////////////////////////////////////////////////////////////
//趴塔副本每层属性配置表
//主要包含趴塔副本中该层塔所配置的基础属性 Added 2012-03-29.
///////////////////////////////////////////////////////////////////////////////
struct TOWER_TRANSCRIPTION_PROPERTY_CONFIG
{
	unsigned int		id;						// ID
	namechar			name[32];				// 副本类名称

	int					tower_layer;			// 该配置表生效的层数，对应第几层
	float				monster_gen_prob[4];	// 老虎机选择怪物数量时各自出现的概率，依次：1，2，3，4个
	float				monster_range_center[3];// 刷怪范围为圆心，圆心坐标位置
	int					monster_range_radius;	// 刷怪范围为圆心，圆半径
	float				renascence_pos[3];		// 趴塔副本该层的复活点
	int					success_controller_id;	// 达到胜利条件，开启的控制器ID

	struct MONSTER_ADDON_PROP
	{
		int		hp;								// 血
		int		mp;								// 魔
		int		attack;							// 攻击
		int		defence;						// 防御
		int		addon_damage;					// 附加伤害
		int		damage_resistance;				// 伤害减免
		int		hit;							// 初始命中
		int		evade;							// 初始躲闪
		int		critical_rate;					// 初始致命一击率 整形千分数
		float	critical_damage;				// 初始致命一击伤害
		int		resist[6];						// 初始抗性，分别对应：眩晕，虚弱，缓慢，沉默，睡眠，缠绕抗性
		int		anti_critical_rate;				// 减免致命一击率 整形千分数?
		float	anti_critical_damage;			// 减免致命一击伤害
		int		skill_armor_rate;				// 技能躲闪		整形千分数?
		int		skill_attack_rate;				// 技能命中		整形千分数?
	}monster_addon_property[3];					// 老虎机选择怪物出现数量后，怪物增加的附加基础属性，分别对应怪物数：1，2，3.注意：怪物数为4时，没有增加属性
	
	int					monster_id_list[32];	// 怪物库ID列表
	
	struct LIFE_TIME_AWARD_ITEM
	{
		int		item_id;						// 奖励物品ID
		int		item_count;						// 奖励物品数量
	}life_time_award[5];						// 终生奖励物品列表

	struct SINGLE_TIME_AWARD_ITEM
	{
		int		item_id;						// 奖励物品ID
		int		item_count;						// 奖励物品数量
	}single_time_award[5];						// 单次奖励物品列表
};


///////////////////////////////////////////////////////////////////////////////////////
// 元魂系统相关数值属性
///////////////////////////////////////////////////////////////////////////////////////
struct RUNE_DATA_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// 名称，最多15个汉字
	
	unsigned int	id_rune_reserve1;		// 
	unsigned int	id_rune_reserve2;		// 
	unsigned int	id_rune_reserve3;		// 
	unsigned int	id_rune_reserve4;		// 
	unsigned int	exp_identify;			// 鉴定服务增加的经验
	unsigned int	exp_return;				// 归元服务增加的经验
	unsigned int	exp_refresh;			// 洗练服务增加的经验
	unsigned int	exp[100];				// 元魂1-100级升级至下一级所需要的经验值
	
	struct SINGLE_TIME_AWARD_ITEM
	{
		unsigned int	lv_req;			// 元魂1-100级佩戴所需等级
		unsigned int	rebount_count_req;	// 元魂1-100级佩戴所需飞升次数
	}equip_req[100]; // 元魂升级到2-101级佩戴所需飞升次数
	unsigned int	decomposeo_exp[100];	// 分解平均档位1-100档获得的经验
	
	struct FIXED_PROPERTY
	{
		unsigned int	level;		// 等级
		unsigned int	attack;		// 攻击增加
		unsigned int	hp;			// 气血增加
		unsigned int	mp;			// 真气增加
		unsigned int	dp;			// 元力增加
	}fixed_property[60];			// 元魂系统固定增益
	
	
	unsigned int	id_rune_identify[5];		// 元魂鉴定所需物品id
	unsigned int	id_rune_refine[5];			// 元魂洗练所需物品id
	unsigned int	id_rune_reset[5];			// 元魂归元所需物品id
	unsigned int	id_rune_change_slot[5];		// 元魂改变孔位所需物品id
};

///////////////////////////////////////////////////////////////////////////////////////
// 元魂系统符文之语模板
///////////////////////////////////////////////////////////////////////////////////////
struct RUNE_COMB_PROPERTY
{
	// 基本信息
	unsigned int	id;							// id
	namechar		name[32];					// 名称，最多15个汉字
	
	int				max_ids;					// 符文数
	unsigned int	ids[10];					// 符文id1~10
	unsigned int	addons[9];					// 跟套装属性不同，小于max_ids个id不激活任何属性
};


///////////////////////////////////////////////////////////////////////////////////////
// 元魂装备模板
///////////////////////////////////////////////////////////////////////////////////////
struct RUNE_EQUIP_ESSENCE
{
	unsigned int	id;							// id
	namechar		name[32];					// 名称，最多15个汉字

	int				file_model;					// 元魂在角色周围显示的模型路径
	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径

	int				quality;					// 法宝品质 0-100
	int				hole_max;					// 最大孔位数 2-5
	int				require_level;				// 等级限制
	int				renascence_count;			// 转生次数要求
	
	// 价钱
	int				price;						// 卖店价
	int				shop_price;					// 店卖价
	
	// 堆叠信息
	int				pile_num_max;				// 堆叠上限
	
	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

///////////////////////////////////////////////////////////////////////////////
//诛小仙功能配置表
//主要包含诛小仙升级所需配置属性 Added 2012-04-19.
///////////////////////////////////////////////////////////////////////////////
struct LITTLE_PET_UPGRADE_CONFIG
{
	unsigned int			id;							//模板ID
	namechar				name[32];					//名称，最多15个汉字

	struct FEED_PET
	{
		int		feed_pet_item_id;						//饲养诛小仙所需道具
		int		gain_exp;								//饲养一次诛小仙所得经验
	}feed_pet[2];										//饲养宠物所需道具ID和一次饲养所得经验值

	int						default_file_model;			//诛小仙默认形象模型路径
	
	struct PET_UPGRADE_INFO
	{
		int		file_model;								//该等级所对应的形象路径
		int		required_exp;							//从上一级升级到该级所需要的经验：如pet_upgrade_info_list[0].required_exp -->表示从0级升级1级所需经验值
		int		award_item;								//达到该等级可以领取的奖励物品ID
	}pet_upgrade_info_list[5];							//诛小仙升级配置信息列表，最多5个等级
};

///////////////////////////////////////////////////////////////////////////////
// 元魂符文物品模板
///////////////////////////////////////////////////////////////////////////////
struct RUNE_SLOT_ESSENCE
{
	unsigned int			id;				//模板ID
	namechar				name[32];		//名称，最多15个汉字
	
	int						file_matter;	//掉在地上的模型路径
	int						file_icon;		//图标路径	
	
	int						price;			// 卖店价
	int						shop_price;		// 店卖价
	
	// 堆叠信息
	int						pile_num_max;	// 堆叠上限
	
	// 处理方式
	unsigned int			proc_type;		// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
	
};

///////////////////////////////////////////////////////////////////////////////
//全服产出限量配置表 Added 2012-06-19.
///////////////////////////////////////////////////////////////////////////////
struct DROP_INTERNAL_CONFIG
{
	unsigned int			id;							//模板ID
	namechar				name[32];					//名称，最多15个汉字
	
	int						time_of_duration;			// 持续时间
	int						start_time;					// 开始时间
	int						item_id;					// 掉落物品id
	int						item_num;					// 产出上限
	namechar				speak_words[100];			// 产出喊话
	namechar				remain_words[100];			// 余量喊话
	int						remain_speak_interval;		// 余量喊话间隔(秒)
};

///////////////////////////////////////////////////////////////////////////////
//跨服PK竞猜配置表 Added 2012-06-25.
///////////////////////////////////////////////////////////////////////////////
struct PK2012_GUESS_CONFIG
{
	unsigned int			id;						//模板ID
	namechar				name[32];				//名称，最多15个汉字
	
	int						first_id;				// 冠军ID
	int						second_id;				// 亚军ID
	int						third_id;				// 季军ID
	int						guess_start_time;		// 竞猜开始时间
	int						guess_end_time;			// 竞猜截止时间
	int						accept_award_start_time;// 领奖开始时间
	int						accept_award_end_time;	// 领奖截止时间
	int						champion_guess_item;	// 冠军投注道具
	int						champion_guess_itemNum;	// 每投1注消耗道具数量
	int						champion_guess_award;	// 冠军竞猜礼包
	float					award_back_ratio;		// 返奖率
	int						guess_item;				// 竞猜道具
	int						guess_item_num;			// 竞猜道具消耗数量
	int						guess_award_item;		// 三强竞猜礼包
	int						guess_award_item3;		// 三强全中礼包
	int						guess_award_item2;		// 猜中两强礼包
	int						guess_award_item1;		// 猜中一强礼包
};


///////////////////////////////////////////////////////////////////////////////
//碰撞副本变身配置表 Added 2012-08-02.
///////////////////////////////////////////////////////////////////////////////
struct COLLISION_RAID_TRANSFIGURE_CONFIG
{
	unsigned int			id;						//模板ID
	namechar				name[32];				//名称，最多15个汉字

	struct TRANSFIGURE_ITEM
	{
		unsigned int		transfigure_id;			//变身ID，index
		int					file_matter;			//变身模型路径
		int					file_icon;				//选体型图标路径
		unsigned int		mass;					//变身后角色质量
		namechar			description[100];		//改体型的描述信息最多100个汉字
	}transfigure_info_list[20];						//变身信息列表
};

//////////////////////////////////////////////////////////////////////////
//摆摊道具物品，显示不同的形象 2012-8-13
//////////////////////////////////////////////////////////////////////////
struct BOOTH_FIGURE_ITEM_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字
	
	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径名
	int				file_model;					// 摆摊形象
	
	int				price;						// 卖店价
	int				shop_price;					// 店卖价
	
	// 堆叠信息
	int				pile_num_max;				// 堆叠上限
	
	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

//////////////////////////////////////////////////////////////////////////
// 战旗buff道具 2012.8.20
//////////////////////////////////////////////////////////////////////////
struct FLAG_BUFF_ITEM_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字
	
	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径名

	int				npc_id;						// 召唤出来npcid
	int				require_level;				// 等级限制
	int				exit_time;					// 存在时间(s)
	int				cool_time;					// 冷却时间(s)

	int				dmg;						// 攻击
	int				defence;					// 防御
	int				hp;							// 气血
	int				mp;							// 真气
	int				extra_defence;				// 减免伤害
	int				crit_rate;					// 致命一击率(千分数)
	int				crit_damage;				// 致命一击伤害(千分数)
	int				anti_stunt;					// 眩晕抗性
	int				anti_weak;					// 虚弱抗性
	int				anti_slow;					// 定身抗性
	int				anti_silence;				// 沉默抗性
	int				anti_sleep;					// 睡眠抗性
	int				anti_twist;					// 缠绕抗性
	int				skill_attack_rate;			// 技能命中(千分数)
	int				skill_armor_rate;			// 技能躲闪(千分数)
	int				cult_defense[3];			// 御仙魔佛(千分数)
	int				cult_attack[3];				// 克仙魔佛(千分数)
	
	int				price;						// 卖店价
	int				shop_price;					// 店卖价
	
	// 堆叠信息
	int				pile_num_max;				// 堆叠上限
	
	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

///////////////////////////////////////////////////////////////////////////////
//上古世界传送服务配置 Added 2012-08-15.
///////////////////////////////////////////////////////////////////////////////
struct UI_TRANSFER_CONFIG
{
	unsigned int			id;						//模板ID
	namechar				name[32];				//名称，最多15个汉字
	
	struct TRANS_POINT
	{
		unsigned int		trans_id;			// ID，index
		namechar			name[16];			// 名字
		unsigned int		map_id;				// 目标地图号
		float				pos[3];				// 目标坐标
		int					fee;				// 费用
	}trans_point_list[256];						// 传送目标点列表
};

///////////////////////////////////////////////////////////////////////////////////////
// 星座物品模板数据结构定义
// 镶嵌到装备上
///////////////////////////////////////////////////////////////////////////////////////
struct XINGZUO_ITEM_ESSENCE
{
	unsigned int	id;							// (类型)ID
	namechar		name[32];					// 名称, 最多15个汉字
	
	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径
	int				file_model;					// 预留的，暂时不用，美术表现，可能像法宝那样飘在人物周围的ecm模型
	
	unsigned int	equip_mask;					// 可镶嵌部位	
	int				reborn_count;				// 飞升次数

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	// 堆叠信息
	int				pile_num_max;				// 堆叠上限
	
	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

// 星座精力石
struct XINGZUO_ENERGY_ITEM_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字
	
	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径名
	
	int				energy;						// 精力值
	
	int				price;						// 卖店价
	int				shop_price;					// 店卖价
	
	// 堆叠信息
	int				pile_num_max;				// 该药品的堆叠上限
	
	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

// 大药合并道具
struct CASH_MEDIC_MERGE_ITEM_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字
	
	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径名

	int				require_level;				// 等级限制
	int				renascence_count;			// 转生次数要求
	int				type;						// 灌注类型
	int				cool_time;					// 冷却时间（毫秒）

	int				item_ids[20];				// 有效药品id表
	
	int				price;						// 卖店价
	int				shop_price;					// 店卖价
	
	// 堆叠信息
	int				pile_num_max;				// 该药品的堆叠上限
	
	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

///////////////////////////////////////////////////////////////////////////////
//碰撞副本奖励配置 Added 2012-08-25.
///////////////////////////////////////////////////////////////////////////////
struct COLLISION_RAID_AWARD_CONFIG
{
	unsigned int			id;						//模板ID
	namechar				name[32];				//名称，最多15个汉字
	
	int				daily_award_item;			// 每日奖励物品
	struct TRANS_POINT
	{
		unsigned int		item_id;			// 奖励物品id
		unsigned int		item_num;			// 奖励物品数量
		unsigned int		score1_num;			// 所需A积分
		unsigned int		score2_num;			// 所需B积分
		unsigned int		win_num;			// 所需获胜场次
	}award_list[20];							// 奖励
};

///////////////////////////////////////////////////////////////////////////////
//buff区域配置 Added 2012-10-29.
///////////////////////////////////////////////////////////////////////////////
struct BUFF_AREA_CONFIG
{
	unsigned int			id;						//模板ID
	namechar				name[32];				//名称，最多15个汉字
	
	int						skill_ids[10];			// 进入区域被击中技能id，应该是一些上buff技能
	int						transfigure_id;			// 变身属性id
};

///////////////////////////////////////////////////////////////////////////////
//活跃度配置 Added 2012-11-05.
///////////////////////////////////////////////////////////////////////////////
struct LIVENESS_CONFIG
{
	unsigned int			id;						//模板ID
	namechar				name[32];				//名称，最多31个汉字
	
	int						scores[15];				// 每个活动分值
	int						gifts[4];				// 四个礼包 0-25、26-50、51-75、76-100

	struct ELEMENT 
	{
		namechar			name[16];				// 名称，最多15个汉字，专用于策划查找
		int					type_activity;			// 活动类别	
		int					require_level;			// 修真等级限制
		int					renascence_count;		// 转生次数要求
		int					type_judge;				// 判定类型，决定require_id解释方式：0-任务id，1-进入地图id
		int					require_id;				// 判断完成的任务id/进入地图id
	}element_list[120];
};

///////////////////////////////////////////////////////////////////////////////
// 上古新副本配置表(挑战副本)
// 2012.11.12
///////////////////////////////////////////////////////////////////////////////
struct CHALLENGE_2012_CONFIG
{
	unsigned int	id;						// ID
	namechar		name[32];				// 名称
	
	struct CONFIG_TMPL
	{
		int				controler_id;			// 副本开启关联控制器
		int				monster_id;				// 副本挑战目标击杀怪物ID	
		int				monster_count_min;		// 副本挑战目标怪物数量下限	
		int				monster_count_max;		// 副本挑战目标怪物数量上限	
		int				mine_id;				// 副本挑战目标开矿ID		
		int				mine_count_min;			// 副本挑战目标开矿数量下限	
		int				mine_count_max;			// 副本挑战目标开矿数量上限	
		int				dead_count;				// 副本挑战目标总死亡次数	
		int				time_limit;				// 副本挑战目标时间时限		
		int				medic_limit_hp;			// 副本挑战大红药品使用量	
		int				medic_limit_mp;			// 副本挑战大蓝药品使用量	
		int				controller_id_challenge;// 副本挑战目标相关变量ID
		int				controller_id_min;		// 副本挑战目标相关变量下限	
		int				controller_id_max;		// 副本挑战目标相关变量上限	
		int				controller_id_win;		// 副本挑战目标对应的控制器ID
		int				award_item_id;			// 副本挑战目标完成奖励物品ID
		int				win_control_id;			// 副本挑战目标完成开启控制器ID
	};

	CONFIG_TMPL main_config;					// 副本总配置
	CONFIG_TMPL lv_config[10];					// 每一关卡配置
};

// 台历道具 2012-12-14
struct SALE_PROMOTION_ITEM_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字
	
	namechar		ui_name[16];				// 界面文字, 2013-04-17
	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径名

	struct ELEMENT
	{
		int		valid_time_start;			// 有效期起始时间
		int		valid_time_end;				// 有效期结束时间
		int		get_count;					// 可重复领取次数
		int		condition_type;				// 前提或消耗条件类型，稍后策划给具体表示
		int		condition_arg1;				// 前提或消耗条件参数1
		int		condition_arg2;				// 前提或消耗条件参数2
		int		award_item1;				// 奖励物品1
		int		award_item1_count;			// 奖励物品数量1
		int		award_item1_valid_time;		// 奖励物品有效期1
		int		award_item1_is_bind;		// 奖励物品是否绑定1
		int		award_item2;				// 奖励物品2
		int		award_item2_count;			// 奖励物品数量2
		int		award_item2_valid_time;		// 奖励物品有效期2
		int		award_item2_is_bind;		// 奖励物品是否绑定2
		namechar desc[100];					// 策划定义描述，可能包含图片、链接等信息
	} element_list[20];

	int				extra_award_need_count;	// 获取额外奖励所需领取次数
	int				extra_award_id;			// 额外奖励
	int				extra_award_num;		// 额外奖励
	int				extra_award_valid_time;	// 额外奖励
	int				extra_award_is_bind;	// 额外奖励

	int				price;						// 卖店价
	int				shop_price;					// 店卖价
	
	// 堆叠信息
	int				pile_num_max;				// 该药品的堆叠上限
	
	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

// 任务特殊奖励，包括界面gfx、小游戏等 2012-12-14
struct TASK_SPECIAL_AWARD_CONFIG
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	// gfx
	int				gfx_path[20];				// 路径
	
	// 小游戏
	namechar		mini_game[10][8];			// 名字，只给策划用
};

// 彩票赠送的礼包，服务器需要一个新的模板，结构跟GIFT_BAG_ESSENCE完全相同
struct GIFT_BAG_LOTTERY_DELIVER_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字
	
	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径	
	
	int				level_everLimit;			// 曾经的等级的下限（适合飞升之后的判断）
	int				level_required;				// 当前等级下限
	int				level_upperLimit;			// 当前等级上限
	unsigned int	require_gender;				// 性别限制，0-男，1-女，2-男女都可
	int				renascence_count;			// 转生次数要求
	UInt64			character_combo_id;			// 职业限制
	UInt64			character_combo_id2;		// 职业限制, 扩展职业ID后的新职业掩码，Added 2011-07-14.
	unsigned int	god_devil_mask;				// 仙魔 造化限制仙/佛/魔/备选1/2/3
	unsigned int    open_time;					// 开启需要的时间
	float			probability;				// 开启成功的概率
	unsigned int	success_disappear;			// 成功开启后消失(1为是，0为否)
	unsigned int	faliure_disappear;			// 开启失败后消失(1为是，0为否)
	unsigned int	id1_object_need;			// 开启需要物品1
	unsigned int	id1_object_num;				// 开启需要物品1的数量
	unsigned int	id2_object_need;			// 开启需要物品2
	unsigned int	id2_object_num;				// 开启需要物品2的数量
	int				money_need;					// 开启需要金钱
	unsigned int	num_object;					// 内含物品的总数
	
	struct 
	{
		unsigned int	id_object;				// 物品id
		float			probability;			// 物品产出概率
		unsigned int	num_min;				// 物品的数量下限
		unsigned int	num_max;				// 物品的数量上限
		unsigned int	is_bind;				// 物品是否绑定
		unsigned int	effect_time;			// 物品的时效（单位为秒，0为永久）
	}gifts[16];
	
	unsigned int	normalize_group[4];
	
	int				price;						// 卖店价
	int				shop_price;					// 店卖价
	
	// 堆叠信息
	int				pile_num_max;				// 堆叠上限
	
	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
	
};

// 汤圆类型的彩票
struct LOTTERY_TANGYUAN_ITEM_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字
	
	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径

	int				open_item;					// 开启消耗物品id
	int				open_item_num;				// 开启消耗物品数量
	
	int				exp_get;					// 开启一次获得经验
	int				exp_level[7];				// 升级到1-7级所需经验

	struct 
	{
		struct 
		{
			unsigned int id;
			unsigned int num;
		}gifts[4];
	}group_gifts[8];
	
	int				price;						// 卖店价
	int				shop_price;					// 店卖价
	
	// 堆叠信息
	int				pile_num_max;				// 堆叠上限
	
	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
	
};

// 消费大礼包
struct GIFT_PACK_ITEM_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字
	
	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径
	
	unsigned int    open_time;					// 开启需要的时间

	struct 
	{
		unsigned int id;						// 礼包id
		unsigned int score;						// 礼包消耗积分数量
	}item_gifts[4];
	
	int				price;						// 卖店价
	int				shop_price;					// 店卖价
	
	// 堆叠信息
	int				pile_num_max;				// 堆叠上限
	
	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
	
};

enum
{
	PROP_ADD_DAMAGE,
	PROP_ADD_DEF,
	PROP_ADD_HP,
	PROP_ADD_MP,
	PROP_ADD_ATTACK,		// 命中
	PROP_ADD_ARMOR,			// 躲闪
	PROP_ADD_CRIT_RATE,		// 暴击 
	PROP_ADD_CRIT_ANTI,		// 减暴击
	PROP_ADD_CRIT_DAMAGE,	// 暴伤
	PROP_ADD_CRIT_DAMAGE_ANTI,	// 减暴伤
	PROP_ADD_SKILL_ATTACK_RATE,	// 技能命中
	PROP_ADD_SKILL_ARMOR_RATE,	// 技能躲闪
	PROP_ADD_RESISTANCE_1,
	PROP_ADD_RESISTANCE_2,
	PROP_ADD_RESISTANCE_3,
	PROP_ADD_RESISTANCE_4,
	PROP_ADD_RESISTANCE_5,
	PROP_ADD_CULT_DEFANCE_1,
	PROP_ADD_CULT_DEFANCE_2,	
	PROP_ADD_CULT_DEFANCE_3,
	PROP_ADD_CULT_ATTACK_1,
	PROP_ADD_CULT_ATTACK_2,
	PROP_ADD_CULT_ATTACK_3,
	PROP_ADD_NUM = 30,
};

// 基础属性增益原材料道具
struct PROP_ADD_MATERIAL_ITEM_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字
	
	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径

	int				require_num;				// 需求数量
	int				forge_item[3];				// 重练道具

	struct 
	{
		unsigned int id;						// 合成道具
		float		prop;						// 概率
	}forge_result[10];	

	int				price;						// 卖店价
	int				shop_price;					// 店卖价
	
	// 堆叠信息
	int				pile_num_max;				// 堆叠上限
	
	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

// 基础属性增益道具
struct PROP_ADD_ITEM_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字
	
	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径

	int				material_id;				// 原材料id，用来重练	
	int				require_level;				// 等级限制
	int				renascence_count;			// 转生次数要求
	
	int				prop_add[PROP_ADD_NUM];		// 增益
	
	int				price;						// 卖店价
	int				shop_price;					// 店卖价
	
	// 堆叠信息
	int				pile_num_max;				// 堆叠上限
	
	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

// 基础属性增益上限配置表
struct PROP_ADD_CONFIG
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				prop_add_limit[PROP_ADD_NUM];	// 增益上限
};

// 国战配置表
struct KING_WAR_CONFIG
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				king_continue_item;			// 国王连任奖励
	int				king_continue_item_num;		// 奖励数量

	struct  
	{
		int				monser_id;				// 副战场怪物id
		int				hp_delta;				// 生命改变量
	} monster[2];
	
	struct
	{
		int id;
		int num;
		int score;
	} king_shop[20];							// 国王积分购买模板

	struct
	{
		int id;
		int num;
		int score;
	} normal_shop[20];							// 个人积分购买模板

};

struct JINFASHEN_TO_MONEY_CONFIG
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	struct Config
	{
		int id;					//
		int money;				//
		int reputation;			// 仙魔佛造化之通用版本，需要服务器处理
	}jinshen[25], fashen[25];
};

struct BATTLE_201304_CONFIG
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	unsigned int	map_id;						// 地图号
	bool			add_time_if_full;			// 人满是否延时
	int				minite_add;					// 延时长度
	int				require_level;				// 等级限制
	int				renascence_count;			// 转生次数要求

	struct Monster 
	{
		int id;
		int score;
	} monster_info[5];

	int			score;				// 人头基础分值
	int			score_target;		// 目标分数
	int			taskid_success_human;		// 人族完成发奖任务
	int			taskid_success_wild;		// 神族完成发奖任务
	int			taskid_everyone;	// 参与奖发放任务
};

// 飞升150-160符文模板，想不到更好名字了
struct RUNE2013_ITEM_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字
	
	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径

	int				quality;					// 品质0:初等 1:中等 2:高等 3:极品
	int				prop_type;					// 增益类型：enum PlayerPropType
	float			add_value;					// 每级增益数值
	int				self_exp;					// 本体经验

	int				price;						// 卖店价
	int				shop_price;					// 店卖价
	
	// 堆叠信息
	int				pile_num_max;				// 堆叠上限
	
	// 处理方式
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

// 150-160升级符文配置表
struct RUNE2013_CONFIG
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				lv_limit;					// 等级上限
	int				lvup_exp[4][10];			// 符文升级所需经验，初等、中等、高等、极品 1-10
};

// 泡澡奖励配置
struct BASHE_AWARD_CONFIG
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字
	
	int				period;						// 间隔时间(s)
	
	struct Award
	{
		int id;
		int count;
		float prob;
	} award_info[10];
};

//竞技场赛季时间配置
struct ARENA_SEASON_TIME_CONFIG
{
	unsigned int	id;							//ID
	namechar		name[32];					//名称，最多15个汉字

	struct Seanson_Time
	{
		int start_time;
		int end_time;
	}season_info[4];
};

//每周个人分档奖励配置
struct PERSONAL_LEVEL_AWARD_CONFIG
{
	unsigned int	id;							//ID
	namechar		name[32];					//名称，最多15个汉字
		
	struct personal_level_award
	{
		int				level;						//档位
		int				inception_points;			//初始分
		int				cutoff_points;				//截止分
		int				week_receive_num;			//每周领取次数

		struct award_item
		{
			int item_id;							//奖励物品ID
			int item_num;							//奖励物品数量
			int time_limit;							//时间限制
		}item_info[5];
	}personal_level_award_info[15];
};

//战队赛季奖励配置
struct TEAM_SEASON_AWARD_CONFIG
{
	unsigned int	id;							//ID
	namechar		name[32];					//名称，最多15个汉字

	struct team_season_award
	{
		int				inception_rank;				//竞技场排名起始
		int				cutoff_rank;				//竞技场排名截止

		struct award_item
		{
			int item_id;							//奖励物品ID
			int item_num;							//奖励物品数量
			int time_limit;							//时间限制
		}item_info[6];
	}team_season_awrd_info[15];

};

//每周兑换币奖励配置
struct WEEK_CURRNCY_AWARD_CONFIG
{
	unsigned int	id;							//ID
	namechar		name[32];					//名称，最多15个汉字
	
	struct week_currncy_award
	{
		int				inception_rank;				//竞技场排名起始
		int				cutoff_rank;				//竞技场排名截止
		int				receive_currency_id;		//可领取的兑换币ID
		int				receive_currnecy_num;		//可领取的兑换币数量
		int				week_receive_num;			//每周可领取次数
		int				time_limit;					//时间限制
	}week_currncy_awrd_info[15];

};


//染色剂
struct COLORANT_ITEM_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 掉在地上的模型路径
	int				file_icon;					// 图标路径

	int				color;						// 色系 0:红 1:橙 2:黄 3:绿 4:青 5:蓝 6:紫 7:黑 8:白 9:褐 10:粉 11:灰
	int				quality;					// 品级

	int				index;

	int				price;						// 卖店价
	int				shop_price;					// 店卖价

	int				pile_num_max;				// 堆叠上限
	unsigned int	proc_type;					// 是以下几种方式的组合: 死亡时是否掉落，是否可以扔在地上，是否可以卖给NPC，是人民币物品，是否可以玩家间交易，是否任务相关物品，是否装备后绑定，保留勿选中，产生GUID
};

//可互动物体配置表
struct INTERACTION_OBJECT_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// 名称, 最多15个汉字

	int				file_matter;				// 模型路径
	int				file_icon;					// 图标路径

	unsigned int	main_body;					//以谁为主体操作
	unsigned int	client_main_body;			//客户端挂点主体
	unsigned int	hook[6];					//挂点关联的挂点模板
	int				max_interaction_num;		//最多可互动次数
	unsigned int	disappear;                  //到达互动上限次数是否消失
	int				require_item_id;			//互动需要的物品
	int				require_task_id;			//互动需要的任务
	int				require_level;				//互动需要的人物等级
	int				require_gender;				//互动性别限制，0-男，1-女，2-男女都可
	UInt64			character_combo_id;			//互动职业限制
	UInt64			character_combo_id2;		//互动职业限制
	int				consum_item_id;				//互动消耗的物品
	int				open_time;					//开启互动的准备时间
	int				interaction_time;			//互动时间
	int				finish_time;				//结束互动准备时间
	unsigned int	monster_id;					//套用的怪物模板
	int				path_id;					//关联路径
	unsigned int    move_enable;				//移动可用
	int				velocity;					//移动速度
	unsigned int    auto_end;					//到达地点是否自动结束
	int				new_item_id;				//完成互动后转变为新互动物体
};

//可互动物体挂点配置
struct INTERACTION_OBJECT_HOOK_CONFIG
{
	unsigned int	id;								//ID
	namechar		name[32];						//名称，最多15个汉字
	
	namechar		object_hook_name[100];			//物品挂点名称
	namechar		character_hook_name[100];		//人物挂点名称
	int				start_task_id;					//互动开始触发的任务
	int				start_open_controller_id;		//互动开始开启的控制器
	int				start_close_controller_id;		//互动开开始关闭的控制器

	int				prepare_time;					//准备时间
	namechar		start_object_action[100];		//互动开始时物品播放的动作
	namechar		start_character_action[100];	//互动开始时人物播放的动作
	namechar		static_object_action[100];		//互动静止时物品播放的动作
	namechar		static_character_action[100];	//互动静止时人物播放的动作
	namechar		move_object_action[100];		//互动移动时物品播放的动作
	namechar		move_character_action[100];		//互动激动时人物播放的动作
	namechar		end_object_action[100];			//互动结束物品播放的动作
	namechar		end_character_action[100];		//互动结束人物播放的动作

	int				end_task_id;					//互动结束触发的任务
	int				end_open_controller_id;			//互动结束开启的控制器
	int				end_close_controller_id;		//互动结束关闭的控制器
	
	struct award_item
	{
		int		item_id;							//互动结束获得的物品
		float	prop;								//获得物品的概率
	}item_info[5];	

	unsigned int	accident;							//意外终止行为
														// 0x0001 受攻击结束
														// 0x0002 主动移动结束
	unsigned int	showbutton;							//互动结束是否显示按钮
	namechar		buttontext[100];						//结束按钮的文字

	int				absx;								//结束时玩家去往的绝对坐标x
	int				absy;								//结束时玩家去往的绝对坐标y	
	int				absz;								//结束时玩家去往的绝对坐标z
	int				relativex;							//结束时玩家去往的相对坐标x
	int				relativey;							//结束时玩家去往的相对坐标y 
	int				relativez;							//结束时玩家去往的相对坐标z
};

struct COLORANT_CONFIG
{
	unsigned int	id;								//ID
	namechar		name[32];						//名称，最多15个汉字

	struct colorant_prop
	{
		int				colorant_item_id;				//染色剂ID
		float			probability;					//概率
	}colorant_info[100];
};

//帮派传送配置表
struct FACTION_TRANSFER_CONFIG
{
	unsigned int	id;								//ID
	namechar		name[32];						//名称，最多15个汉字
	
	struct location
	{
		int			region_index;					//区域索引
		namechar	name[16];						//区域名称
		int			x;								//X坐标
		int			y;								//Y坐标
		int			z;								//Z坐标
	}transfer_location[30];
};

//可建筑区域配置表
struct BUILDING_REGION_CONFIG
{
	unsigned int	id;								//ID
	namechar		name[32];						//名称，最多15个汉字

	struct region
	{
		namechar		name[16];					//区域名称
		int				centerx;					//区域中心点X
		int				centery;					//区域中心点Y
		int				length;						//区域长度
		int				width;						//区域宽度
		int				region_index;				//区域索引
		unsigned int	region_id;					//区域id
	}build_region[30];
};

struct BUILDING_ESSENCE
{
	unsigned int	id;								//ID
	namechar		name[32];						//名称，最多15个汉字

	int				constructing_model;				//正在建筑中的等级模型
	namechar		building_desc[256];				//建筑描述

	struct update_building
	{
		int				ordinary;						//普通建筑图像
		int				hightlight;						//高亮建筑图像	
		int				grass_num;						//升级等级需要的灵草数量
		int				mine_num;						//升级等级需要的矿石数量
		int				monster_food_num;				//升级等级需要的兽粮数量
		int				monster_core_num;				//升级等级需要的怪核数量
		int				money_num;						//升级等级需要的元宝数量

		struct require_building 
		{
			unsigned int building_id;					//升级等级需要的建筑
			int			 level;							//升级等级需要的建筑等级
		}req_building[10];

		int				building_model;					//建筑模型
		int				task_id;						//升级时可接任务
		int				finish_num;						//升级任务完成次数

		struct add_item
		{
			int item_id;								//增加物品ID
			int item_num;								//增加物品数量
			int time;									//增加物品时间
		}item[5];	
		
		int				added_grass_num;				//增加灵草数量
		int				added_grass_time;				//增加灵草时间
		int				add_mine_num;					//增加矿石数量
		int				add_mine_time;					//增加矿石时间
		int				add_monster_core_num;			//增加兽核数量
		int				add_mosnter_core_tiem;			//增加兽核时间
		int				add_mosnter_food_num;			//增加兽粮数量
		int				add_monster_food_time;			//增加兽粮时间
		int				add_money_num;					//增加元宝数量
		int				add_money_time;					//增加元宝时间

		int controller_open[15];								//开启控制器的ID
		int controller_close[15];								//关闭控制器的ID
	}up_building[5];
};

//地块
struct REGION_BLOCK_ESSENCE
{
	unsigned int	id;								//ID
	namechar		name[32];						//名称，最多15个汉字

	unsigned int   building[10];					//建筑模板ID
};

//相位配置表
struct PHASE_CONFIG
{
	unsigned int	id;								//ID
	namechar		name[32];						//名称，最多15个汉字

	unsigned int	phaselist[200];					//默认信息
};

//跨服6v6物品兑换表
struct CROSS6V6_ITEM_EXCHANGE_CONFIG
{
	unsigned int	id;								//ID
	namechar		name[32];						//名称，最多15个汉字	
	
	struct cross6v6_item_exchange
	{
		int				item_id;					//物品ID
		int				item_num;					//物品数量
		int				require_currency_id;		//所需兑换币ID
		int				require_currnecy_num;		//所需兑换币数量
		int				time_limit;					//时间限制
	}cross6v6_item_exchange_info[15];
};

//副本攻略配置表
struct TRANSCRIPT_STRATEGY_CONFIG
{
	unsigned int	id;								//ID
	namechar		name[32];						//名称，最多15个汉字

	struct level_info
	{
		int level_type;						
		int monster[10];
	}level_list[10];
};

//帮配商城配置表
struct FACTION_SHOP_CONFIG
{
	unsigned int	id;								//ID
	namechar		name[32];						//名称，最多15个汉字

	struct faction_item
	{
		int item_index;			//物品编号
		int item_id;			//物品id
		int build_id;			//建筑id
		int build_level;		//建筑等级
		int contribution;		//贡献度价格
	}faction_item_list[100];
};


//帮派竞拍品配置表
struct FACTION_AUCTION_CONFIG
{
	unsigned int	id;								//ID
	namechar		name[32];						//名称，最多15个汉字

	struct faction_auction_item
	{
		int		item_id;				//物品id
		int		baseprice;				//底价
		int		file_icon;				//物品大图标
		int		aucitontime;			//拍卖时间
		namechar words[100];			//产出喊话
	}faction_auction_list[100];
};

//帮派元宝商城配置表
struct FACTION_MONEY_SHOP_CONFIG
{
	unsigned int	id;								//ID
	namechar		name[32];						//名称，最多15个汉字

	struct faction_money_item
	{
		int		 item_index;			//物品索引
		int		 build_id;				//建筑id
		int		 build_leve;			//建筑等级
		namechar item_name[16];			//商品名称
		int		 price;					//价格
		int      file_icon;				//图标
		namechar itemdes[100];			//描述
		int	     cooltime;				//冷却时间
		namechar words[100];			//喊话
		int		 type;					//类型
		int		 auition_item_id;		//产出拍卖品id
		int      pen_controller_id;		//开启控制器id
		int		 exp_multi;				// 经验倍数		
		int		 multi_exp_time;		// 多倍经验时间	
	}faction_money_item_list[140];
};

//复合矿
struct COMPOUND_MINE_ESSENCE
{
	unsigned int	id;								//ID
	namechar		name[32];						//名称，最多15个汉字

	int				file_mine[3];					//矿路径
	int				count;							//可采集次数
	int				file_mine_spe;					//矿4路径
};

//初始染色剂配置表
struct COLORANT_DEFAULT_CONFIG
{
	unsigned int	id;								//ID
	namechar		name[32];						//名称，最多15个汉字

	int				colorant[10];					//染色剂ID			
};

//=============================================================================

enum ID_SPACE
{
	ID_SPACE_ESSENCE	= 0,
	ID_SPACE_ADDON		= 1,
	ID_SPACE_TALK		= 2,
	ID_SPACE_RECIPE		= 3,
	ID_SPACE_CONFIG		= 4,
};

//新数据类型必须从最后依次加
enum DATA_TYPE
{
	DT_INVALID = 0,
	DT_EQUIPMENT_ADDON,
	DT_EQUIPMENT_MAJOR_TYPE,
	DT_EQUIPMENT_SUB_TYPE,
	DT_EQUIPMENT_ESSENCE,
	DT_REFINE_TICKET_ESSENCE,
	DT_MEDICINE_MAJOR_TYPE,
	DT_MEDICINE_SUB_TYPE,
	DT_MEDICINE_ESSENCE,
	DT_MATERIAL_MAJOR_TYPE,
	DT_MATERIAL_SUB_TYPE,
	DT_MATERIAL_ESSENCE,
	DT_SKILLTOME_SUB_TYPE,
	DT_SKILLTOME_ESSENCE,		
	DT_TRANSMITROLL_ESSENCE,
	DT_LUCKYROLL_ESSENCE,
	DT_TOWNSCROLL_ESSENCE,
	DT_REVIVESCROLL_ESSENCE,
	DT_TASKMATTER_ESSENCE,
	DT_DROPTABLE_TYPE,
	DT_DROPTABLE_ESSENCE,
	DT_MONSTER_TYPE,
	DT_MONSTER_ESSENCE,	
	DT_NPC_TALK_SERVICE,
	DT_NPC_SELL_SERVICE,
	DT_NPC_BUY_SERVICE,
	DT_NPC_TASK_IN_SERVICE,
	DT_NPC_TASK_OUT_SERVICE,
	DT_NPC_TASK_MATTER_SERVICE,
	DT_NPC_HEAL_SERVICE,
	DT_NPC_TRANSMIT_SERVICE,
	DT_NPC_PROXY_SERVICE,
	DT_NPC_STORAGE_SERVICE,
	DT_NPC_TYPE,
	DT_NPC_ESSENCE,
	DT_TALK_PROC,
	DT_RECIPE_MAJOR_TYPE,
	DT_RECIPE_SUB_TYPE,
	DT_RECIPE_ESSENCE,	
	DT_ENEMY_FACTION_CONFIG,
	DT_CHARACTER_CLASS_CONFIG,
	DT_PARAM_ADJUST_CONFIG,
	DT_PIE_LOVE_CONFIG,
	DT_TASKDICE_ESSENCE,
	DT_TASKNORMALMATTER_ESSENCE,
	DT_PLAYER_LEVELEXP_CONFIG,
	DT_MINE_TYPE,
	DT_MINE_ESSENCE,
	DT_GM_GENERATOR_TYPE,
	DT_GM_GENERATOR_ESSENCE,
	DT_FIREWORKS_ESSENCE,
	DT_NPC_WAR_TOWERBUILD_SERVICE,
	DT_PLAYER_SECONDLEVEL_CONFIG,
	DT_NPC_RESETPROP_SERVICE,
	DT_ESTONE_ESSENCE,
	DT_PSTONE_ESSENCE,
	DT_SSTONE_ESSENCE,
	DT_RECIPEROLL_MAJOR_TYPE,
	DT_RECIPEROLL_SUB_TYPE,
	DT_RECIPEROLL_ESSENCE,
	DT_SUITE_ESSENCE,
	DT_DOUBLE_EXP_ESSENCE,
	DT_DESTROYING_ESSENCE,
	DT_NPC_EQUIPBIND_SERVICE,
	DT_NPC_EQUIPDESTROY_SERVICE,
	DT_NPC_EQUIPUNDESTROY_SERVICE,
	DT_SKILLMATTER_ESSENCE,
	DT_VEHICLE_ESSENCE,
	DT_COUPLE_JUMPTO_ESSENCE,
	DT_LOTTERY_ESSENCE,
	DT_CAMRECORDER_ESSENCE,
	DT_TITLE_PROP_CONFIG,
	DT_SPECIAL_ID_CONFIG,
	DT_TEXT_FIREWORKS_ESSENCE,
	DT_TALISMAN_MAINPART_ESSENCE,
	DT_TALISMAN_EXPFOOD_ESSENCE,
	DT_TALISMAN_MERGEKATALYST_ESSENCE,
	DT_TALISMAN_ENERGYFOOD_ESSENCE,
	DT_SPEAKER_ESSENCE,
	DT_PLAYER_TALENT_CONFIG,
	DT_POTENTIAL_TOME_ESSENCE,
	DT_WAR_ROLE_CONFIG,
	DT_NPC_WAR_BUY_ARCHER_SERVICE,
	DT_SIEGE_ARTILLERY_SCROLL_ESSENCE,
	DT_PET_BEDGE_ESSENCE,
	DT_PET_FOOD_ESSENCE,
	DT_PET_SKILL_ESSENCE,
	DT_PET_ARMOR_ESSENCE,
	DT_PET_AUTO_FOOD_ESSENCE,
	DT_PET_REFINE_ESSENCE,
	DT_PET_ASSIST_REFINE_ESSENCE,
	DT_RENASCENCE_SKILL_CONFIG,
	DT_RENASCENCE_PROP_CONFIG,
	DT_AIRCRAFT_ESSENCE,
	DT_FLY_ENERGYFOOD_ESSENCE,
	DT_ITEM_TRADE_CONFIG,
	DT_BOOK_ESSENCE,
	DT_PLAYER_SKILL_POINT_CONFIG,
	DT_OFFLINE_TRUSTEE_ESSENCE,
	DT_EQUIP_SOUL_ESSENCE,
	DT_EQUIP_SOUL_MELD_SERVICE,
	DT_SPECIAL_NAME_ITEM_ESSENCE,
	DT_RECYCLE_ITEM_CONFIG,
	DT_SCORE_TO_RANK_CONFIG,
	DT_BATTLE_DROP_CONFIG,
	DT_BATTLE_DEPRIVE_CONFIG,
	DT_BATTLE_SCORE_CONFIG,
	DT_GIFT_BAG_ESSENCE,
	DT_VIP_CARD_ESSENCE,
	DT_INSTANCING_BATTLE_CONFIG,
	DT_CHANGE_SHAPE_CARD_ESSENCE,
	DT_CHANGE_SHAPE_STONE_ESSENCE,
	DT_CHANGE_SHAPE_PROP_CONFIG,
	DT_ORIGINAL_SHAPE_CONFIG,
	DT_LIFE_SKILL_CONFIG,
	DT_ARENA_DROPTABLE_CONFIG,
	DT_MERCENARY_CREDENTIAL_ESSENCE,
	DT_TELEPORTATION_ESSENCE,
	DT_TELEPORTATION_STONE_ESSENCE,
	DT_COMBINE_SKILL_EDIT_CONFIG,
	DT_UPGRADE_EQUIP_CONFIG,
	DT_UPGRADE_EQUIP_CONFIG_1,
	DT_UPGRADE_EQUIP_STONE_ESSENCE,
	DT_NPC_CONSIGN_SERVICE,
	DT_DEITY_CHARACTER_CONFIG,
	DT_LOTTERY2_ESSENCE,
	DT_GEM_CONFIG,
	DT_GEM_ESSENCE,
	DT_GEM_SEAL_ESSENCE,
	DT_GEM_DUST_ESSENCE,
	DT_GEM_EXTRACT_CONFIG,
	DT_GENERAL_ARTICLE_ESSENCE,
	DT_LOTTERY3_ESSENCE,		//Added 2011-07-01.
	DT_TRANSCRIPTION_CONFIG,	//Added 2011-07-13.
	DT_ONLINE_GIFT_BAG_CONFIG,	//Added 2011-12-06.
	DT_SMALL_ONLINE_GIFT_BAG_ESSENCE, //Added 2011-12-06.
	DT_SCROLL_REGION_CONFIG,	//Added 2012-03-20.
	DT_SCROLL_UNLOCK_ESSENCE,	//Added 2012-03-20.
	DT_SCROLL_DIG_COUNT_INC_ESSENCE,	//Added 2012-03-22.
	DT_TOWER_TRANSCRIPTION_CONFIG,		//Added 2012-03-31.
	DT_TOWER_TRANSCRIPTION_PROPERTY_CONFIG,	//Added 2012-03-31.
	DT_RUNE_DATA_CONFIG,				// 元魂系统相关数值属性 2012.4.16
	DT_RUNE_COMB_PROPERTY,				// 元魂系统符文之语模板 2012.4.16
	DT_RUNE_EQUIP_ESSENCE,				// 元魂这个装备			2012.4.16
	DT_LITTLE_PET_UPGRADE_CONFIG,		//诛小仙升级属性配置表 Added 2012-04-19.
	DT_RUNE_SLOT_ESSENCE,
	DT_DROP_INTERNAL_CONFIG,			// 限时限量掉落配置表 2012.6.19
	DT_PK2012_GUESS_CONFIG,				// 跨服PK竞猜配置表
	DT_COLLISION_RAID_TRANSFIGURE_CONFIG,//碰撞副本变身配置表 Added 2012-08-02.
	DT_BOOTH_FIGURE_ITEM_ESSENCE,
	DT_FLAG_BUFF_ITEM_ESSENCE,
	DT_NPC_REPUTATION_SHOP_SERVICE,
	DT_NPC_UI_TRANSFER_SERVICE,
	DT_UI_TRANSFER_CONFIG,
	DT_XINGZUO_ITEM_ESSENCE,
	DT_XINGZUO_ENERGY_ITEM_ESSENCE,
	DT_COLLISION_RAID_AWARD_CONFIG,
	DT_CASH_MEDIC_MERGE_ITEM_ESSENCE,
	DT_BUFF_AREA_CONFIG,
	DT_LIVENESS_CONFIG,
	DT_CHALLENGE_2012_CONFIG,
	DT_SALE_PROMOTION_ITEM_ESSENCE,
	DT_GIFT_BAG_LOTTERY_DELIVER_ESSENCE,
	DT_LOTTERY_TANGYUAN_ITEM_ESSENCE,
	DT_TASK_SPECIAL_AWARD_CONFIG,
	DT_GIFT_PACK_ITEM_ESSENCE,	
	DT_PROP_ADD_MATERIAL_ITEM_ESSENCE,
	DT_PROP_ADD_ITEM_ESSENCE,
	DT_PROP_ADD_CONFIG,
	DT_KING_WAR_CONFIG,
	DT_JINFASHEN_TO_MONEY_CONFIG,
	DT_BATTLE_201304_CONFIG,
	DT_RUNE2013_ITEM_ESSENCE,
	DT_RUNE2013_CONFIG,
	DT_BASHE_AWARD_CONFIG,
	DT_ARENA_SEASON_TIME_CONFIG,
	DT_PERSONAL_LEVEL_AWARD_CONFIG,
	DT_TEAM_SEASON_AWARD_CONFIG,
	DT_WEEK_CURRNCY_AWARD_CONFIG,
	DT_COLORANT_ITEM_ESSENCE,
	DT_INTERACTION_OBJECT_ESSENCE,
	DT_INTERACTION_OBJECT_HOOK_CONFIG,
	DT_COLORANT_CONFIG,
	DT_FACTION_TRANSFER_CONFIG,
	DT_BUILDING_REGION_CONFIG,
	DT_BUILDING_ESSENCE,
	DT_REGION_BLOCK_ESSENCE,
	DT_PHASE_CONFIG,
	DT_CROSS6V6_ITEM_EXCHANGE_CONFIG,
	DT_TRANSCRIPT_STRATEGY_CONFIG,
	DT_FACTION_SHOP_CONFIG,
	DT_FACTION_AUCTION_CONFIG,
	DT_FACTION_MONEY_SHOP_CONFIG,
	DT_COMPOUND_MINE_ESSENCE,
	DT_COLORANT_DEFAULT_CONFIG,
	DT_MAX,              //新数据类型必须从最后依次加
};

#pragma pack(pop, EXP_TYPES_INC)
#endif//_EXP_TYPES_H_


