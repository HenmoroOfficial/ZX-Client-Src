/*
 * FILE: globaldataman.h
 *
 * DESCRIPTION: global data loader and manager
 *
 * CREATED BY: Hedi, 2005/7/18
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#ifndef _GLOBALDATAMAN_H_
#define _GLOBALDATAMAN_H_

#include "A3DTypes.h"
#include "vector.h"
#include "AAssist.h"

using namespace abase;

// data and methods for domain
enum DOMAIN_TYPE
{
	DOMAIN_TYPE_NULL = 0,
	DOMAIN_TYPE_1ST_CLASS, 
	DOMAIN_TYPE_2ND_CLASS,
	DOMAIN_TYPE_3RD_CLASS,	
};

typedef struct _DOMAIN_INFO
{
	int						id;				// id of the domain
	DOMAIN_TYPE				type;			// type of the domain
	int						reward;			// money rewarded per week
	ACHAR					name[16];		// name of the domain in Unicode

	vector<A3DPOINT2>		verts;			// verts of the surrounding polygon
	vector<int>				faces;			// faces of the surrounding polygon

	A3DPOINT2				center;			// center of the domain, it is the place we put the icon of the faction
	vector<int>				neighbours;		// neighbours of this domain
	
	int						challenge_itemid;	// 宣战物品id
	int						item_lowerlimit;	// 宣战物品下限
	int						item_upperlimit;	// 宣战物品上限

	// these members will be set at run time
	int						id_owner;			// 领土的占领方，可能是以前的防守方，也可能是攻打成功的进攻方
	int						id_attacker;		// 宣战者 也是 进攻方
	int						id_defender;		// 防守方
	int						flag;				// -1:战斗未开启 0：战斗中1：守方胜2：攻方胜
	int						battle_time_start;
	int						battle_time_end;
	int						deposit;
	// 神秘奖励
	int						random_award;		// 神秘大奖的id		
	int						random_award_num;	// 神秘大奖的数量
	char					random_award_drawn;	// 神秘大奖已经被领取 0 未领取 1 已领取
	// 佣兵凭证
	int						assis_num;			// 可领取的佣兵凭证的数量
	int						assis_drawn_num;	// 已领取的佣兵凭着的数量
	// 战斗中，攻方和守方的人数对比
	int						attacker_num;		//攻方人数
	int						defender_num; 		//守方人数
	int						player_limit;		//攻方和守方的人数上限
	char					color;
	char					challenged_byself;

} DOMAIN_INFO;

typedef struct _DOMAIN_INFO_SERV
{
	int						id;				// id of the domain
	DOMAIN_TYPE				type;			// type of the domain
	int						challenge_itemid;
	int						item_upperlimit;
	int						item_lowerlimit;
	vector<int>				neighbours;		// neighbours of this domain

} DOMAIN_INFO_SERV;

vector<DOMAIN_INFO> * globaldata_getdomaininfos();
vector<DOMAIN_INFO> * globaldata_getdomaininfos2();
vector<DOMAIN_INFO> * globaldata_getdomaininfos3();
vector<DOMAIN_INFO> * globaldata_getdomaininfos4();
vector<DOMAIN_INFO> * globaldata_getdomaininfos5();

vector<DOMAIN_INFO_SERV> * globaldata_getdomaininfosserver();

#pragma pack(push, GSHOP_ITEM_PACK, 1)
typedef struct _GSHOP_ITEM
{
	unsigned int			id;				// object id of this item
	unsigned int			num;			// number of objects in this item
	char					icon[128];		// icon file path

	struct {
		unsigned int			price;			// price of this item
		unsigned int			time;			// time of duration, 0 means forever
	} buy[1];
	
	int						discount;		//打折比例
	int						bonus;			//返还红利的比例					
	unsigned int			props;			// mask of all props, currently from low bit to high bit: 新品、推荐品、促销品
											// 最高8位分别表示打折方案，从低到高分别是1-8种方案
											// 次高8位分别表示出售方案，从低到高分别是1-8种方案

	int						main_type;		// index into the main type array
	int						sub_type;		// index into the sub type arrray
	int						local_id;		// id of this shop item, used only for localization purpose

	unsigned short			desc[512];		// simple description
	unsigned short			szName[32];		// name of this item to show

	//新加赠品
	bool		 			bHasPresent;				//是否有赠品	
	unsigned short 			szPresentName[32];			//赠品名字
	unsigned int 			Presentid;					//赠品ID
	unsigned int 			Presentcount;				//赠品个数
	unsigned int 			Presenttime;				//赠品存在的时限(秒)
	char		 			Present_file_icon[128];		//赠品图标路径
	bool		 			bPresentBind;				//赠品是否绑定
	unsigned short         			Presentdesc[512];			//赠品文字描述

	//商城物品的上架有效时间Added 2011-11-07.
	struct
	{
		int		type;						//有效时间的类别 0:永久有效，默认类别；1：按时间段；2：每周；3：每月
		int		start_time;					//开始时间：按时间段时：存储UTC的秒数，按每月和每周时：存储（小时+分+秒）的秒数
		int		end_time;					//终止时间：存储方案同上
		int		param;						//按时间段时：1：表示开启开始时间；2：表示开启终止时间
											//按每周时：（掩码）0-6位分别表示：周日 - 周六
											//按每月时：（掩码）1-31位分别表示：1-31日
	}valid_time;
	//Added end.
	
	// 关键字搜索商城物品Added 2013-04-17.
	unsigned short         SearchKey[64];			//关键字，","分割
	//Added end.
	
	_GSHOP_ITEM()
	{
		bHasPresent = false;
		memset(szPresentName,0,128);
		Presentid = 0;
		Presentcount = 0;
		Presenttime = 0;
		memset(Present_file_icon,0,128);
		bPresentBind = false;
		memset(Presentdesc,0,sizeof(unsigned short)*512);
		memset(&valid_time, 0, sizeof(valid_time)); //Added 2011-11-08.
		memset(&SearchKey, 0, sizeof(SearchKey));
	}

} GSHOP_ITEM;
typedef struct _GSHOP_MAIN_TYPE
{
	WORD					szName[64];		// name of this main type
	int						id;				// id of this main type
	vector<AWString>		sub_types;		// sub type name of this main type
} GSHOP_MAIN_TYPE;
#pragma pack(pop, GSHOP_ITEM_PACK)

//=============================================================================
//VIP奖励相关数据和接口定义 Added 2011-12-29.
#pragma pack(push, VIP_AWARD_ITEM_PACK, 1)
typedef struct _VIP_AWARD_ITEM
{
	unsigned int	tid;					//模板ID
	unsigned short	szName[64];				//所含物品的名称
	unsigned int	item_id;				//所含物品的模板id
	unsigned int	count;					//所含物品的个数

	unsigned int	award_type;				//奖励类型 0：普通奖励， 1：VIP奖励
	unsigned int	award_level;			//奖励级别 普通奖励的奖励级别根据玩家等级设定，VIP等级根据充值额度设定
	unsigned int	award_obtain_type;		//获取奖励的方式：0：每日领取，1：特殊领取
	int				expire_time;			//期限，奖品有单独的时限
}VIP_AWARD_ITEM;
#pragma pack(pop, VIP_AWARD_ITEM_PACK)

DWORD globaldata_get_vipaward_timestamp();
vector<VIP_AWARD_ITEM>* globaldata_get_vipaward_items();
//Added end.
//=============================================================================

DWORD globaldata_getgshop_timestamp();
vector<GSHOP_ITEM> * globaldata_getgshopitems();
vector<GSHOP_MAIN_TYPE> * globaldata_getgshopmaintypes();
GSHOP_MAIN_TYPE * globaldata_getgshopmaintype_by_id(int id);
int globaldata_getgshopitemindex_by_id(unsigned int id, unsigned int num = 1);

DWORD globaldata_getgshop_timestamp2();
vector<GSHOP_ITEM> * globaldata_getgshopitems2();
vector<GSHOP_MAIN_TYPE> * globaldata_getgshopmaintypes2();
GSHOP_MAIN_TYPE * globaldata_getgshopmaintype_by_id2(int id);
int globaldata_getgshopitemindex_by_id2(unsigned int id, unsigned int num = 1);

// 大跨服商城
DWORD globaldata_getgshop_timestamp3();
vector<GSHOP_ITEM> * globaldata_getgshopitems3();
vector<GSHOP_MAIN_TYPE> * globaldata_getgshopmaintypes3();
GSHOP_MAIN_TYPE * globaldata_getgshopmaintype_by_id3(int id);
int globaldata_getgshopitemindex_by_id3(unsigned int id, unsigned int num = 1);

// common interface 
bool globaldata_load();
bool globaldata_save_to(const char * path);
bool globaldata_release();

bool globaldata_loadserver();
bool globaldata_releaseserver();
// for localization
static void SplitString(vector<AString>& vec, AString& s, const char* split);

#endif//_GLOBALDATAMAN_H_
