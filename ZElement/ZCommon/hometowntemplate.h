#ifndef _HOMETOWN_TEMPLATE_H_
#define _HOMETOWN_TEMPLATE_H_

#ifdef WIN32
#pragma warning(disable: 4996)
#pragma warning(disable: 4786)
#endif

#include <vector>
#include <map>
#include <stdio.h>

#ifdef WIN32
typedef wchar_t namechar;
#else
typedef unsigned short namechar;
#endif
namespace HOMETOWN
{


enum ID_SPACE
{
	ID_SPACE_ESSENCE = 0,
	ID_SPACE_FUNC,
	ID_SPACE_CONFIG,
	ID_SPACE_MAX,
};

enum DATA_TYPE
{
	DT_CROP_ESSENCE = 0,
	DT_FRUIT_ESSENCE,
	DT_TOOL_FUNC,
	DT_TOOL_ESSENCE,
	DT_MAX,
	DT_INVALID = DT_MAX,
};

struct CROP_ESSENCE
{
	unsigned int 	id;
	namechar 		name[32];
	char 			type;				//0:普通，1：珍稀
	int 			required_level;		//人物等级
	int 			fruit_max;			//产量上限
	int 			fruit_min;			//产量下限
	int 			fruit_id;			//果实id
	int 			seed_price;			//种子价格
	int 			grow_period;		//生长期
	int 			protect_period;		//防偷保护期
	int 			steal_limit;		//可偷的比例，百分数
	int 			icon_seed;			//种子图标
	int 			icon_sprout;		//幼苗图标
	int 			icon_immature;		//不成熟图标
	int 			icon_premature;		//即将成熟图标
	int 			icon_mature;		//成熟图标
	int 			icon_wilt;			//枯萎图标
	int 			icon_harvest;		//已收获图标
	int 			icon_stolen;		//被偷光图标
};

struct FRUIT_ESSENCE
{
	unsigned int 	id;
	namechar 		name[32];
	int 			price;				//果实出售价格
	int 			icon;				//果实图标
	int				pile_limit;			//堆叠上限
};

enum TOOL_FUNC_TYPE
{
	TFT_FARM_PROTECT = 0,				//对整个农场加保护期
	TFT_FIELD_PROTECT,					//对一块地加保护期
	TFT_FIELD_SPEEDGROW,				//对一块地加快生长
	TFT_FARM_SPEEDGROW,					//对整个农场加快生长
	TFT_FARM_HIDE,						//农场隐身功能

	TFT_MAX,
};

struct TOOL_FUNC						//道具功能模板
{
	unsigned int	id;
	namechar		name[32];
	int 			type;				//功能类型
	int				num_params;			//功能参数个数
	int				param1;				//参数1
	int 			param2;				//参数2
	int 			param3;				//参数3
};

struct TOOL_ESSENCE
{
	unsigned int	id;
	namechar		name[32];
	int 			required_level;		//需要的人物等级
	int				charm_price;		//购买需要的魅力值
	int				money_price;		//购买需要的组件币数
	unsigned int 	func_ids[5];		//每个道具最多5个功能
	int				cool_type;			//道具冷却类型，从1开始编号
	int 			cool_time;			//冷却时间
	int				icon;				//图标
	int				pile_limit;			//堆叠上限
};

struct LEVEL_CONFIG
{
	int level;							//级别
	int normal_field_count;				//该级别拥有普通土地数
	int love_field_count;				//该级别拥有爱心地数
	int plant_love_field;				//该级别可耕种别人的爱心地数
};

struct FARM_CONFIG							//偷菜组件配置结构
{
	unsigned int 	id;
	namechar		name[32];
	int 			max_level;				//最大等级
	int 			max_field_count;		//每个人最大的土地数目
	int				init_pocket_caps;		//初始包裹各数
	std::vector<LEVEL_CONFIG> level_config;	//不同级别的土地数目限制,要求按等级从小到大排序
	std::vector<__int64> upgrade_config;	//各个级别的升级魅力值,数组大小应等于max_level-1
	std::vector<int> crop_list;				//可以种植的作物ID列表,要求从小到大
	std::vector<int> tool_list;				//可购买的道具ID列表, 要求从小到大
	int 			money_local;			//组件货币的数目兑换下面数目的通用货币
	int				money_universal;		//通用货币的数目对应上面数目的组件货币
};

struct HOMETOWN_CONFIG						//整个家园配置结构
{
	unsigned int	id;
	namechar		name[32];
	std::vector<int> compo_list;			//已经开放的组件ID列表
};

class HometownTemplate
{

public:
	static HometownTemplate *GetInstance()
	{
		static HometownTemplate inst;
		return &inst;
	}
	unsigned int get_first_data_id(ID_SPACE idspace, DATA_TYPE & datatype);
	unsigned int get_next_data_id(ID_SPACE idspace, DATA_TYPE & datatype);
	unsigned int get_data_num(ID_SPACE idspace);
	DATA_TYPE get_data_type(unsigned int id, ID_SPACE idspace);

	template <typename T>
	const T *get_data_ptr(unsigned int id, ID_SPACE idspace, DATA_TYPE & datatype, const T *&data)
	{
		if (idspace >= ID_SPACE_MAX) 
		{
			data = NULL;
			return NULL;
		}
		IDToLOCATIONMap &id_loc_map = _id_loc_map[idspace];
		IDToLOCATIONMap::iterator it = id_loc_map.find(id);
		if (it == id_loc_map.end()) 
		{
			data = NULL;
			return NULL;
		}
		datatype = it->second.type;
		std::vector<T> &v = *(std::vector<T> *)_vec_start_ptr[datatype];
		data = &v[it->second.pos];
		return data;
	}
	const HOMETOWN_CONFIG *get_hometown_config() { return &_HOMETOWN_CONFIG; }
	const FARM_CONFIG *get_farm_config() { return &_FARM_CONFIG; }

	template <typename T>
	void add_structure(int data_type, const T &data)
	{
		if (data_type>=0 && data_type<DT_MAX)
		{
			std::vector<T> &v = *(std::vector<T> *)_vec_start_ptr[data_type];
			v.push_back(data);
		}
	}
	void add_structure(const HOMETOWN_CONFIG &cfg) { _HOMETOWN_CONFIG = cfg; }
	void add_structure(const FARM_CONFIG &cfg) { _FARM_CONFIG = cfg; }
	void finish_add_structure() { _setup_location(); }		//调用了一系列add_structure后要调用finish_add_structure重新计算_id_loc_map

	int save_data(const char * pathname);
	int load_data(const char * pathname);

private:
	HometownTemplate() { _setup_start(); }
	HometownTemplate(const HometownTemplate &){}
	HometownTemplate & operator = (const HometownTemplate &){ return *this; }

	int _save_data(FILE *file);
	int _load_data(FILE *file);
	void _setup_location(); 		//填充_id_loc_map
	void _setup_start();			//填充_vec_start_ptr

	struct LOCATION
	{
		DATA_TYPE type;
		size_t pos;
	};
	typedef std::map<unsigned int, LOCATION> IDToLOCATIONMap;

	std::vector<CROP_ESSENCE>		_CROP_ESSENCE_vector;
	std::vector<FRUIT_ESSENCE>		_FRUIT_ESSENCE_vector;
	std::vector<TOOL_FUNC>			_TOOL_FUNC_vector;
	std::vector<TOOL_ESSENCE>		_TOOL_ESSENCE_vector;
	HOMETOWN_CONFIG					_HOMETOWN_CONFIG;
	FARM_CONFIG						_FARM_CONFIG;

	std::vector<void *>				_vec_start_ptr;

	IDToLOCATIONMap					_id_loc_map[ID_SPACE_MAX];
	IDToLOCATIONMap::iterator 		_id_loc_itr[ID_SPACE_MAX];
};

};
#endif
