/*
 * FILE: EC_IvtrTypes.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/11/19
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "AAssist.h"
#include "base64.h"
#include <vector.h>

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

#ifndef NUM_MAGICCLASS
#define NUM_MAGICCLASS		6
#endif	//	NUM_MAGICCLASS

//	Equipment endurance scale
#define ENDURANCE_SCALE		100
//	Player item price scale
#define PLAYER_PRICE_SCALE	1.0f
//	Weapon endurance reduce speed
#define WEAPON_RUIN_SPEED	-2
//	Armor endurance reduce speed
#define ARMOR_RUIN_SPEED	-25

//装备-饰品的宝石插槽个数，Added 2011-03-11.
#define MAX_GEM_SLOT		3

//	Inventory type
enum
{
	IVTRTYPE_PACK = 0,		//	Normal pack
	IVTRTYPE_EQUIPPACK,		//	Equipment
	IVTRTYPE_TASKPACK,		//	Task pack
	IVTRTYPE_TRASHBOX,		//	Trash box
	IVTRTYPE_FACTIONTRASHBOX,//	Faction trash box
	IVTRTYPE_PET,			//	Pet pack
	IVTRTYPE_PET_EQUIP,		//	Pet equipment
	IVTRTYPE_POCKETPACK,		//  bag pack, 随身包裹，存放宠物生产的物品
	IVTRTYPE_FASHION,		//  Fashion pack
#ifdef LUA_UI
	//define here not elsewhere to avoid name conflict.   By kuiwu. [18/5/2006]
	IVTRTYPE_EPEQUIPPACK,	//	Else player's equipment
#endif
	IVTRTYPE_EPPETPACK,		//	Else player's pet pack
	IVTRTYPE_EPPETEQUIPPACK,//	Else player's pet equipment
};

//	Index of item in equipment inventory
enum
{
	EQUIPIVTR_WEAPON = 0,
	EQUIPIVTR_HEAD,
	EQUIPIVTR_BODY,
	EQUIPIVTR_FOOT,
	EQUIPIVTR_NECK,
	EQUIPIVTR_FINGER1,
	EQUIPIVTR_FINGER2,
	EQUIPIVTR_FASHION_EYE,
	EQUIPIVTR_FASHION_NOSE,
	EQUIPIVTR_FASHION_LIP,
	EQUIPIVTR_FASHION_HEAD,
	EQUIPIVTR_FASHION_BODY,
	EQUIPIVTR_FASHION_LEG,
	EQUIPIVTR_FASHION_FOOT,
	EQUIPIVTR_FASHION_BACK,
	EQUIPIVTR_WING,
	EQUIPIVTR_MAIN_TALISMAN,
	EQUIPIVTR_SUB_TALISMAN1,
	EQUIPIVTR_SUB_TALISMAN2,
	EQUIPIVTR_SPEAKER,
	EQUIPIVTR_AUX1,				// 锦囊
	EQUIPIVTR_AUX2,				// 玺绶
	EQUIPIVTR_AUX3,				// 罡气
	EQUIPIVTR_RUNE,				// 元魂
	EQUIPIVTR_AUX5,
	EQUIPIVTR_AUX6,
	SIZE_EQUIPIVTR,
};

//	Equip mask
enum
{
	EQUIP_MASK_WEAPON			= 0x0001,
	EQUIP_MASK_HEAD				= 0x0002,
	EQUIP_MASK_BODY				= 0x0004,
	EQUIP_MASK_FOOT				= 0x0008,
	EQUIP_MASK_NECK				= 0x0010,
	EQUIP_MASK_FINGER1			= 0x0020,
	EQUIP_MASK_FINGER2			= 0x0040,
	EQUIP_MASK_FASHION_EYE		= 0x0080,
	EQUIP_MASK_FASHION_NOSE		= 0x0100,
	EQUIP_MASK_FASHION_LIP		= 0x0200,
	EQUIP_MASK_FASHION_HEAD		= 0x0400,
	EQUIP_MASK_FASHION_BODY		= 0x0800,
	EQUIP_MASK_FASHION_LEG		= 0x1000,
	EQUIP_MASK_FASHION_FOOT		= 0x2000,
	EQUIP_MASK_FASHION_BACK		= 0x4000,
	EQUIP_MASK_WING				= 0x8000,
	EQUIP_MASK_MAIN_TALISMAN	= 0x10000,
	EQUIP_MASK_SUB_TALISMAN1	= 0x20000,
	EQUIP_MASK_SUB_TALISMAN2	= 0x40000,
	EQUIP_MASK_SPEAKER			= 0x80000,
	EQUIP_MASK_AUX1				= 0x100000,			// 锦囊
	EQUIP_MASK_AUX2				= 0x200000,			// 玺绶
	EQUIP_MASK_AUX3				= 0x400000,			// 罡气
	EQUIP_MASK_AUX4				= 0x800000,			// 元魂
	EQUIP_MASK_AUX5				= 0x1000000,
	EQUIP_MASK_AUX6				= 0x2000000,
	EQUIP_MASK_ALL				= 0x3ffffff,
};

//	Inventory size
enum
{
	IVTRSIZE_EQUIPPACK	= SIZE_EQUIPIVTR,	//	Equipment
	IVTRSIZE_TASKPACK	= 20,	//	Task pack
	IVTRSIZE_PETPACK	= 32,	//	Pet pack
	IVTRSIZE_PETEQUIP	= 12,
	IVTRSIZE_DEALPACK	= 36,	//	Deal pack
	IVTRSIZE_NPCPACK	= 48,	//	NPC pack
	IVTRSIZE_BUYPACK	= 18,	//	Buy pack
	IVTRSIZE_SELLPACK	= 18,	//	Sell pack
	IVTRSIZE_BOOTHSPACK	= 18,	//	Booth pack for selling
	IVTRSIZE_BOOTHBPACK = 18,	//	Booth pack for buying
	IVTRSIZE_FASHIONPACK= 90,   //  fashion pack
	IVTRSIZE_HTFARMPACK = 12,	//  home town farm pack
	
	NUM_NPCIVTR			= 8,	//	NPC inventory number
};

//	Weapon type
enum
{
    WEAPONTYPE_MELEE = 0,
    WEAPONTYPE_RANGE = 1,
};

//	Shortcut
enum
{
	NUM_HOSTSCSETS1 = 3,
	NUM_HOSTSCSETS2 = 3,
	SIZE_HOSTSCSET1 = 12,
	SIZE_HOSTSCSET2 = 12,
};

//	Pet food type
enum
{
	PET_FOOD_GRASS = 0,
	PET_FOOD_MEAT,
	PET_FOOD_VEGETABLE,
	PET_FOOD_FRUIT,
	PET_FOOD_WATER,
	MAX_PET_FOOD,
};

///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////

#pragma pack(1)

struct IVTR_ESSENCE_PETEGG
{
	int		req_level;          //	需求玩家级别
	int		req_class;          //	需求玩家职业
	int		honor_point;        //	好感度
	int		pet_tid;            //	宠物的模板ID
	int		pet_vis_tid;        //	宠物的可见ID（如果为0，则表示无特殊可见ID）
	int		pet_egg_tid;        //	宠物蛋的ID
	int		pet_class;          //	宠物类型 战宠，骑宠，观赏宠
	short	level;				//	宠物级别
	short	sec_level;			//	第二级别
	int		exp;                //	宠物当前经验
	int		skill_point;        //	剩余技能点
	unsigned short name_len;	//	名字长度 
	unsigned short skill_count;	//	技能数量		
	char	name[16];			//	名字内容
	//	这里跟随技能表
	/*
	struct
	{	
		int skill
		int level;

	} skills[]
	 */
};

inline ACString BufferMakeString(const void* p, size_t sz)
{
	char* buf = new char[(sz+2)/3*4+32];
	int len = base64_encode((unsigned char*)p, sz, buf);
	buf[len] = 0;
	buf[len + 1] = 0;
	ACString str((ACHAR*)buf);
	delete[] buf;
	return str;
}

inline unsigned char* BufferFromString(const ACHAR* szText, size_t& sz)
{
	int len = wcslen(szText) * sizeof(ACHAR);
	unsigned char* p = new unsigned char[len+2];
	sz = base64_decode((char*)szText, len, p);
	return p;
}

template<class T>
inline ACString ItemMakeString(T* p)
{
	char buf[(sizeof(T)+2)/3*4+32];
	int len = base64_encode((unsigned char*)p, sizeof(T), buf);
	buf[len] = 0;
	buf[len + 1] = 0;
	return (ACHAR*)buf;
}

template<class T>
inline void ItemFromString(const ACHAR* szText, T* p)
{
	memset(p, 0, sizeof(T));
	int len = wcslen(szText) * sizeof(ACHAR);

	if (len != ((sizeof(T)+2)/3*4))
	{
		assert(false);
		return;
	}

	unsigned char buf[sizeof(T)+2];

	if (base64_decode((char*)szText, len, buf) != sizeof(T))
	{
		assert(false);
		return;
	}

	memcpy(p, buf, sizeof(T));
}

struct IVTR_ESSENCE_EQUIPMENT
{
	unsigned short	id_estone;
	unsigned short	num_estone;
	unsigned short	id_pstone;
	unsigned short	num_pstone;
	unsigned short	id_addon1;
	unsigned short	id_addon2;
}; 

struct IVTR_ESSENCE_EQUIPMENT_NEW
{
	IVTR_ESSENCE_EQUIPMENT old_data;

    int master_id;                  //主人ID
    char master_name[20];           //主人名称
    int reserved1;
    int reserved2;
    int cur_spirit_power;           //当前魄力
    int max_spirit_power;           //魄力上限
	int soul_used_spirit;           //器魄已经占用的魂力
    int soul[5];					//金木水火土5个器魄
    int soul_rune_word;             //器魄组合属性的CRC，只有非0才有效
};

//Added 2011-03-11.对装备-饰品增加宝石插槽等相关属性
struct IVTR_ESSENCE_EQUIPMENT_NEW_FOR_GEM : public IVTR_ESSENCE_EQUIPMENT_NEW
{
//	IVTR_ESSENCE_EQUIPMENT_NEW old_data;	//根据客户端的原有代码状况，决定不通过组合方式增加字段，而通过继承
	char	gem_slot_shape[MAX_GEM_SLOT];	//装备-饰品的宝石插槽形状属性，-1表示该槽还没有被鉴定
	int		gem_embeded[MAX_GEM_SLOT];		//已经镶嵌的宝石id，-1表示该槽还没有镶嵌宝石
	int		reserved1;						//保留位
	int		reserved2;						//保留位
	int		reserved3;						//保留位
};

struct IVTR_ESSENCE_MEDICINE
{
	int		hp;
	int		mp;	
	int		dp;
	
	ACString MakeString() const
	{
		ACHAR buf[64];
		wsprintf(buf, _AL("%d,%d,%d"),	hp,	mp, dp);
		return buf;
	}

	void FromString(const ACHAR* szText)
	{
		memset(this, 0, sizeof(*this));
		swscanf(szText,	_AL("%d,%d,d%"), &hp, &mp, &dp);
	}
};

struct IVTR_ESSENCE_LOTTERY
{
	BYTE		dice_data[6];	// dice generated data, first 0 means end

	ACString MakeString() const
	{
		ACHAR buf[64];
		wsprintf(buf, _AL("%d,%d,%d,%d,%d,%d"),	dice_data[0], dice_data[1], dice_data[2], 
			dice_data[3], dice_data[4], dice_data[5]);
		return buf;
	}

	void FromString(const ACHAR* szText)
	{
		memset(this, 0, sizeof(*this));
		swscanf(szText,	_AL("%d,%d,%d,%d,%d,%d"), &dice_data[0], &dice_data[1], &dice_data[2], 
			&dice_data[3], &dice_data[4], &dice_data[5]);
	}

};

struct IVTR_ESSENCE_DESTROYING
{
	int tid;
};

struct IVTR_ESSENCE_TALISMAN_MAIN
{
	struct talisman_enhanced_essence
	{
		int master_id;			// 主人ID
		char master_name[20];	// 主人名字
		int exp;				// 法宝经验
		int level;				// 法宝等级

		//后面是脚本调整的 上面是脚本不能调整的
		//原来的三个参数attack、armor、defense一直没有用，现在改为法宝二次强化对应的attack_enhance、mp_enhance、 hp_enhance.
		int stamina;
		unsigned int quality;
		int hp;
		int mp;
		int attack_enhance;		// 法宝二次强化，攻击加成
		int damage_low;
		int damage_high;
		int mp_enhance;			// 法宝二次强化，真气加成
		int hp_enhance;			// 法宝二次强化，气血加成
		int resistance[6];
	} ess;

	abase::vector<int>	addons;
};

struct IVTR_ESSENCE_TALISMAN_ENERGYFOOD
{
	int		energy_left;
	
	ACString MakeString() const
	{
		ACHAR buf[64];
		wsprintf(buf, _AL("%d"), energy_left);
		return buf;
	}

	void FromString(const ACHAR* szText)
	{
		memset(this, 0, sizeof(*this));
		swscanf(szText,	_AL("%d"), &energy_left);
	}
};

struct RUNE_PROP
{
	int id;
	int value;
	int grade;			// 大档位
	float grade_extra;	// 小档位
};

struct RUNE_ENHANCED_ESSENCE
{
	int exp;
	int level;
	int quality;			// 品质
	int hole;				// 孔数
	bool refine_active;		// 是否有洗练属性
	int refine_quality;
	RUNE_PROP prop[8];
	RUNE_PROP refine_prop[8];// 洗练属性
	int rune_stone[5];
	int reserver[8];		// 预留字段
	int cash_refine_total;	// 使用收费道具洗练次数
	int refine_remain;		// 不消耗道具剩余洗练次数
	int refine_total;		// 总洗练次数
	int reset_cnt;			// 归元次数
	int avg_grade;			// 平均档位
};

struct IVTR_ESSENCE_SPECIALNAME
{
	int owner_id;
};

//需要存盘的数据
struct IVTR_ESSENCE_CHANGE_SAHPE_CARD
{
	struct change_shape_card_enhanced_essence
	{	
		int master_id;
		char master_name[20];	//主人名称
		int level;				//变身石等级
		int exp_level;			//熟练度等级
		int stamina;			//精力
		int max_stamina;		//最大精力
		int quality;			//品级
		int exp;				//熟练度
		int max_exp;			//当前等级最大熟练度
		int exp_speed;			//熟练度成长速度
		int hp;
		int mp;
		int damage_low;			//最低攻击	
		int damage_high;		//最高攻击
		int attack;				//普攻命中
		int defense;			//防御
		int armor;				//普攻闪躲
		int resistance[6];
		int reserver1;
		int reserver2;
		int reserver3;
		int reserver4;
		int reserver5;
	}ess;

	abase::vector<int>	addons;
};

struct IVTR_ESSENCE_MERCENARY_CREDENTIAL
{
	int faction_id;
	int territory_id;
};

struct IVTR_TELEPORTATION_ESSENCE
{
	struct enhanced_teleportation_essence
	{
		int time;
		int num;
	}ess;
	struct coordinate_point
	{
		int inst;
		float x;
		float y;
		float z;
	};
	abase::vector<coordinate_point> points;
};

#pragma pack()

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////



