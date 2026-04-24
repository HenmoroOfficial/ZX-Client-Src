#ifdef LINUX
#include <ASSERT.h>
#else
#include <assert.h>
#define ASSERT assert
#endif
#include "elementdataman.h"
#include "itemdataman.h"

itemdataman::itemdataman()
{
	_edm = new elementdataman;
}

itemdataman::~itemdataman()
{
	delete _edm;
	for(size_t i=0; i<sale_item_ptr_array.size(); i++)
	{
		abase::fastfree(sale_item_ptr_array[i], sale_item_size_array[i]);
	}
}

int itemdataman::load_data(const char * pathname) 
{ 
	if(_edm->load_data(pathname) == 0)
	{
		generate_item_for_sell();
		return 0;
	}
	else
		return -1;
}

unsigned int itemdataman::get_data_id(ID_SPACE idspace, unsigned int index, DATA_TYPE& datatype)
{	
	return _edm->get_data_id(idspace, index, datatype);
}

unsigned int itemdataman::get_first_data_id(ID_SPACE idspace, DATA_TYPE& datatype)
{
	return _edm->get_first_data_id(idspace, datatype);
}

unsigned int itemdataman::get_next_data_id(ID_SPACE idspace, DATA_TYPE& datatype)
{
	return _edm->get_next_data_id(idspace, datatype);
}

unsigned int itemdataman::get_data_num(ID_SPACE idspace)
{
	return _edm->get_data_num(idspace);
}

DATA_TYPE itemdataman::get_data_type(unsigned int id, ID_SPACE idspace)
{
	return _edm->get_data_type(id, idspace);
}

const void * itemdataman::get_data_ptr(unsigned int id, ID_SPACE idspace, DATA_TYPE& datatype)
{
	return _edm->get_data_ptr(id, idspace, datatype);
}

#define CASE_GET_ITEM_PRICE(x)  case DT_##x:  \
				return ((x*)dataptr)->price

int itemdataman::get_item_sell_price(unsigned int id)
{
	DATA_TYPE datatype;
	const void * dataptr = get_data_ptr(id, ID_SPACE_ESSENCE, datatype);
	if(dataptr == NULL || datatype == DT_INVALID)	return 0;
	
	switch(datatype) {
		CASE_GET_ITEM_PRICE(EQUIPMENT_ESSENCE);
		CASE_GET_ITEM_PRICE(ESTONE_ESSENCE);
		CASE_GET_ITEM_PRICE(PSTONE_ESSENCE);
		CASE_GET_ITEM_PRICE(SSTONE_ESSENCE);
		CASE_GET_ITEM_PRICE(MATERIAL_ESSENCE);
		CASE_GET_ITEM_PRICE(REFINE_TICKET_ESSENCE);
		CASE_GET_ITEM_PRICE(RECIPEROLL_ESSENCE);
		CASE_GET_ITEM_PRICE(MEDICINE_ESSENCE);
		CASE_GET_ITEM_PRICE(TRANSMITROLL_ESSENCE);
		CASE_GET_ITEM_PRICE(LUCKYROLL_ESSENCE);
		CASE_GET_ITEM_PRICE(TASKNORMALMATTER_ESSENCE);
		CASE_GET_ITEM_PRICE(DOUBLE_EXP_ESSENCE);
		CASE_GET_ITEM_PRICE(SKILLMATTER_ESSENCE);
		CASE_GET_ITEM_PRICE(VEHICLE_ESSENCE);
		CASE_GET_ITEM_PRICE(COUPLE_JUMPTO_ESSENCE);
		CASE_GET_ITEM_PRICE(LOTTERY_ESSENCE);
		CASE_GET_ITEM_PRICE(FIREWORKS_ESSENCE);
		CASE_GET_ITEM_PRICE(CAMRECORDER_ESSENCE);
		CASE_GET_ITEM_PRICE(TEXT_FIREWORKS_ESSENCE);
		CASE_GET_ITEM_PRICE(TALISMAN_MAINPART_ESSENCE);
		CASE_GET_ITEM_PRICE(TALISMAN_EXPFOOD_ESSENCE);
		CASE_GET_ITEM_PRICE(TALISMAN_MERGEKATALYST_ESSENCE);
		CASE_GET_ITEM_PRICE(TALISMAN_ENERGYFOOD_ESSENCE);
		CASE_GET_ITEM_PRICE(SPEAKER_ESSENCE);
		CASE_GET_ITEM_PRICE(TOWNSCROLL_ESSENCE);
		CASE_GET_ITEM_PRICE(SIEGE_ARTILLERY_SCROLL_ESSENCE);
		CASE_GET_ITEM_PRICE(PET_BEDGE_ESSENCE);
		CASE_GET_ITEM_PRICE(PET_FOOD_ESSENCE);
		CASE_GET_ITEM_PRICE(PET_SKILL_ESSENCE);
		CASE_GET_ITEM_PRICE(PET_ARMOR_ESSENCE);
		CASE_GET_ITEM_PRICE(PET_AUTO_FOOD_ESSENCE);
		CASE_GET_ITEM_PRICE(PET_REFINE_ESSENCE);
		CASE_GET_ITEM_PRICE(PET_ASSIST_REFINE_ESSENCE);
		CASE_GET_ITEM_PRICE(AIRCRAFT_ESSENCE);
		CASE_GET_ITEM_PRICE(FLY_ENERGYFOOD_ESSENCE);
		CASE_GET_ITEM_PRICE(BOOK_ESSENCE);
		CASE_GET_ITEM_PRICE(OFFLINE_TRUSTEE_ESSENCE);
		CASE_GET_ITEM_PRICE(EQUIP_SOUL_ESSENCE);
		CASE_GET_ITEM_PRICE(SPECIAL_NAME_ITEM_ESSENCE);
		CASE_GET_ITEM_PRICE(GIFT_BAG_ESSENCE);
		CASE_GET_ITEM_PRICE(VIP_CARD_ESSENCE);
		CASE_GET_ITEM_PRICE(CHANGE_SHAPE_CARD_ESSENCE);
		CASE_GET_ITEM_PRICE(CHANGE_SHAPE_STONE_ESSENCE);
		CASE_GET_ITEM_PRICE(MERCENARY_CREDENTIAL_ESSENCE);
		CASE_GET_ITEM_PRICE(TELEPORTATION_ESSENCE);
		CASE_GET_ITEM_PRICE(TELEPORTATION_STONE_ESSENCE);


		


	default:
		return 0;
	}
	return 0;
}

#define CASE_GET_ITEM_SHOP_PRICE(x)  case DT_##x:  \
				return ((x*)dataptr)->shop_price

int itemdataman::get_item_shop_price(unsigned int id)
{
	DATA_TYPE datatype;
	const void * dataptr = get_data_ptr(id, ID_SPACE_ESSENCE, datatype);
	if(dataptr == NULL || datatype == DT_INVALID)	return 0;
	
	switch(datatype) {
		CASE_GET_ITEM_SHOP_PRICE(EQUIPMENT_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(ESTONE_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(PSTONE_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(SSTONE_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(MATERIAL_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(REFINE_TICKET_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(RECIPEROLL_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(MEDICINE_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(TRANSMITROLL_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(LUCKYROLL_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(TASKNORMALMATTER_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(DOUBLE_EXP_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(SKILLMATTER_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(VEHICLE_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(COUPLE_JUMPTO_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(LOTTERY_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(FIREWORKS_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(CAMRECORDER_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(TEXT_FIREWORKS_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(TALISMAN_MAINPART_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(TALISMAN_EXPFOOD_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(TALISMAN_MERGEKATALYST_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(TALISMAN_ENERGYFOOD_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(SPEAKER_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(TOWNSCROLL_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(SIEGE_ARTILLERY_SCROLL_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(PET_BEDGE_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(PET_FOOD_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(PET_SKILL_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(PET_ARMOR_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(PET_AUTO_FOOD_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(PET_REFINE_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(PET_ASSIST_REFINE_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(AIRCRAFT_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(FLY_ENERGYFOOD_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(BOOK_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(OFFLINE_TRUSTEE_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(EQUIP_SOUL_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(SPECIAL_NAME_ITEM_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(GIFT_BAG_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(VIP_CARD_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(CHANGE_SHAPE_CARD_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(CHANGE_SHAPE_STONE_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(MERCENARY_CREDENTIAL_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(TELEPORTATION_ESSENCE);
		CASE_GET_ITEM_SHOP_PRICE(TELEPORTATION_STONE_ESSENCE);

	default:
		return 0;
	}
	return 0;
}

#define CASE_GET_ITEM_PILE_LIMIT(x)  case DT_##x:  \
				return ((x*)dataptr)->pile_num_max
int itemdataman::get_item_pile_limit(unsigned int id)
{
	DATA_TYPE datatype;
	const void * dataptr = get_data_ptr(id, ID_SPACE_ESSENCE, datatype);
	if(dataptr == NULL || datatype == DT_INVALID)	return 0;
	
	switch(datatype) {
		CASE_GET_ITEM_PILE_LIMIT(EQUIPMENT_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(ESTONE_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(PSTONE_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(SSTONE_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(MATERIAL_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(REFINE_TICKET_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(RECIPEROLL_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(MEDICINE_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(TRANSMITROLL_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(LUCKYROLL_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(TASKMATTER_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(TASKNORMALMATTER_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(TASKDICE_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(DOUBLE_EXP_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(DESTROYING_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(SKILLMATTER_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(VEHICLE_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(COUPLE_JUMPTO_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(LOTTERY_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(FIREWORKS_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(CAMRECORDER_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(TEXT_FIREWORKS_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(TALISMAN_MAINPART_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(TALISMAN_EXPFOOD_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(TALISMAN_MERGEKATALYST_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(TALISMAN_ENERGYFOOD_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(SPEAKER_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(TOWNSCROLL_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(SIEGE_ARTILLERY_SCROLL_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(PET_BEDGE_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(PET_FOOD_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(PET_SKILL_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(PET_ARMOR_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(PET_AUTO_FOOD_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(PET_REFINE_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(PET_ASSIST_REFINE_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(AIRCRAFT_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(FLY_ENERGYFOOD_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(BOOK_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(OFFLINE_TRUSTEE_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(EQUIP_SOUL_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(SPECIAL_NAME_ITEM_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(GIFT_BAG_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(VIP_CARD_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(CHANGE_SHAPE_CARD_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(CHANGE_SHAPE_STONE_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(MERCENARY_CREDENTIAL_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(TELEPORTATION_ESSENCE);
		CASE_GET_ITEM_PILE_LIMIT(TELEPORTATION_STONE_ESSENCE);

	default:
		return 0;
	}
	return 0;
}

#define CASE_GET_ITEM_PROC_TYPE(x)  case DT_##x:  \
return ((x*)dataptr)->proc_type
int itemdataman::get_item_proc_type(unsigned int id)
{
	DATA_TYPE datatype;
	const void * dataptr = get_data_ptr(id, ID_SPACE_ESSENCE, datatype);
	if(dataptr == NULL || datatype == DT_INVALID)	return 0;
	
	switch(datatype) {
		CASE_GET_ITEM_PROC_TYPE(EQUIPMENT_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(ESTONE_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(PSTONE_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(SSTONE_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(MATERIAL_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(REFINE_TICKET_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(RECIPEROLL_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(MEDICINE_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(TRANSMITROLL_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(LUCKYROLL_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(TASKMATTER_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(TASKNORMALMATTER_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(TASKDICE_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(DOUBLE_EXP_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(DESTROYING_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(SKILLMATTER_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(VEHICLE_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(COUPLE_JUMPTO_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(LOTTERY_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(FIREWORKS_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(CAMRECORDER_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(TEXT_FIREWORKS_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(TALISMAN_MAINPART_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(TALISMAN_EXPFOOD_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(TALISMAN_MERGEKATALYST_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(TALISMAN_ENERGYFOOD_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(SPEAKER_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(TOWNSCROLL_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(SIEGE_ARTILLERY_SCROLL_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(PET_BEDGE_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(PET_FOOD_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(PET_SKILL_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(PET_ARMOR_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(PET_AUTO_FOOD_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(PET_REFINE_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(PET_ASSIST_REFINE_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(AIRCRAFT_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(FLY_ENERGYFOOD_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(BOOK_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(OFFLINE_TRUSTEE_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(EQUIP_SOUL_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(SPECIAL_NAME_ITEM_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(GIFT_BAG_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(VIP_CARD_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(CHANGE_SHAPE_CARD_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(CHANGE_SHAPE_STONE_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(MERCENARY_CREDENTIAL_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(TELEPORTATION_ESSENCE);
		CASE_GET_ITEM_PROC_TYPE(TELEPORTATION_STONE_ESSENCE);
		
	default:
		return 0;
	}
	return 0;
}


int itemdataman::get_monster_drop_times(unsigned int id)
{
	DATA_TYPE datatype;
	 const MONSTER_ESSENCE * mon = (const MONSTER_ESSENCE*) get_data_ptr(id, ID_SPACE_ESSENCE, datatype);
	if(mon == NULL || datatype != DT_MONSTER_ESSENCE)	
	{
		return 0;
	}
	return mon->drop_times;
}

void itemdataman::get_monster_drop_money(unsigned int id,int & low, int &high)
{
	DATA_TYPE datatype;
	 const MONSTER_ESSENCE * mon = (const MONSTER_ESSENCE*) get_data_ptr(id, ID_SPACE_ESSENCE, datatype);
	if(mon == NULL || datatype != DT_MONSTER_ESSENCE)	
	{
		low = 0;
		high = 0;
		return ;
	}
	low = mon->money_average-mon->money_var;
	high = mon->money_average+mon->money_var;
	return;
}

int itemdataman::generate_item_from_monster(unsigned int id, int * list,  size_t list_size)
{
	DATA_TYPE datatype;
	const MONSTER_ESSENCE * mon = (const MONSTER_ESSENCE*) get_data_ptr(id, ID_SPACE_ESSENCE, datatype);
	if(datatype == DT_NPC_ESSENCE && mon)
	{
		//如果是NPC，则再取一遍数据
		const NPC_ESSENCE *npc = (const NPC_ESSENCE*)mon;
		if(npc->id_src_monster == 0) return -1;
		
		mon = (const MONSTER_ESSENCE*) get_data_ptr(npc->id_src_monster, ID_SPACE_ESSENCE, datatype);
		if(datatype != DT_MONSTER_ESSENCE) return -1;
	}

	if(mon == NULL || datatype != DT_MONSTER_ESSENCE)	
	{
		return -1;
	}

	size_t mcount = mon->drop_times;
	if(mcount == 0) return 0;

	//查找掉落表
	if(mon->drop_table[0].id_drop_table == 0 && mon->drop_table[1].id_drop_table == 0) return 0;
	int index = abase::RandSelect(&mon->drop_table[0].prop_drop_table, sizeof(int)+sizeof(float), 2);
	int drop_id = mon->drop_table[index].id_drop_table;
	if(drop_id <= 0) return 0;
	DATA_TYPE datatype2;
	const DROPTABLE_ESSENCE * table = (const DROPTABLE_ESSENCE*) get_data_ptr(drop_id, ID_SPACE_ESSENCE, datatype2);
	if(table == NULL || datatype2 != DT_DROPTABLE_ESSENCE)	
	{
		return 0;
	}
	

	size_t drop_count = 0;
	for(size_t j = 0; j < mcount && drop_count < list_size; j ++)
	{
		unsigned int drop_num = abase::RandSelect(table->num_to_drop,sizeof(float), 5);
		for(size_t i=0; i<drop_num; i++)
		{
			int index = abase::RandSelect(&(table->drops[0].probability), sizeof(unsigned int)+sizeof(float), 64);
			if(j && index >= 32) continue;	//如果是多次丢出则只有一次能生成后面的物品
			int item_id = table->drops[index].id_obj;
			if(item_id > 0)
			{
				list[drop_count] = item_id;
				drop_count ++;
				if(drop_count >= list_size) break;
			}
		}
	}

	return drop_count;
}

item_data * itemdataman::generate_item(unsigned int id,const void * tag, size_t tag_size, int name_id)
{
	item_data * item = NULL;
	size_t size;
	if(generate_item(id, &item, size,tag,tag_size, name_id)) return NULL;
	ASSERT(size == sizeof(item_data) + item->content_length);
	return item;
}

int itemdataman::generate_talisman_stamina_potion(unsigned int id, ID_SPACE idspace, char ** data, size_t& size)
{
	DATA_TYPE datatype;
	TALISMAN_ENERGYFOOD_ESSENCE * ess = (TALISMAN_ENERGYFOOD_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if( ess == NULL || datatype != DT_TALISMAN_ENERGYFOOD_ESSENCE)	return -1;
		
	size = sizeof(item_data) + sizeof(int);
	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = *data;
	
	*(unsigned int*)buf = id;			buf += sizeof(unsigned int);		//物品的模板ID
	*(size_t*)buf = 1;				buf += sizeof(size_t);			//物品的数量
	*(size_t*)buf = ess->pile_num_max;		buf += sizeof(size_t);			//物品的堆叠上限
	*(int*)buf = ess->proc_type;			buf += sizeof(int);			//物品的处理方式
	*(int*)buf = 0;					buf += sizeof(int);			//GUID
	*(int*)buf = 0;					buf += sizeof(int);			//GUID	
	*(int*)buf = ess->price;			buf += sizeof(int);			//物品的价格
	*(int*)buf = 0;					buf += sizeof(int);			//物品的到期时间
	size_t* content_length = (size_t*)buf;		buf += sizeof(size_t);			//记住buf的指针，以后再填
	char ** item_content = (char **)buf;		buf += sizeof(char *);			//记住buf的指针，以后再填
	*content_length = (char *)(*data)+size-buf;
	*item_content = buf;

	*(int*)buf = ess->energy_total; buf += sizeof(int);
	ASSERT( (size_t)(buf - *data)== size);

	return 0;
}


#define CASE_GEN_NORMAL_ITEM(x) case DT_##x: \
				ret = generate_normal_item(id,ID_SPACE_ESSENCE,(char **)&item, size,(x*)NULL,(x*)NULL,DT_##x); \
				break
#define CASE_GEN_NORMAL_ITEM2(x) case DT_##x: \
				ret = generate_normal_item(id,ID_SPACE_ESSENCE,(char **)&item, size,(x*)NULL,(x##_2*)NULL,DT_##x); \
				break



				
int itemdataman::generate_item_for_sell()
{
	char* item;
	size_t size;
	int ret;

	DATA_TYPE datatype;
	unsigned int id = _edm->get_first_data_id(ID_SPACE_ESSENCE, datatype);
	element_data::item_tag_t tag = {element_data::IMT_SHOP,0};
	while(id != 0 && datatype != DT_INVALID)
	{

		switch(datatype)
		{
		case DT_EQUIPMENT_ESSENCE:
			ret = generate_equipment(id, ID_SPACE_ESSENCE, (char **)&item, size, &tag,sizeof(tag));
			break;

		case DT_MEDICINE_ESSENCE:
			ret = generate_medicine(id,ID_SPACE_ESSENCE,(char**)&item, size);
			break;

		case DT_TALISMAN_ENERGYFOOD_ESSENCE:
			ret = generate_talisman_stamina_potion(id,ID_SPACE_ESSENCE,(char**)&item, size);
			break;

		case DT_FLY_ENERGYFOOD_ESSENCE:
			///$$$$$$$$$$$$$$$
			break;


		CASE_GEN_NORMAL_ITEM(ESTONE_ESSENCE);
		CASE_GEN_NORMAL_ITEM(PSTONE_ESSENCE);
		CASE_GEN_NORMAL_ITEM(SSTONE_ESSENCE);
		CASE_GEN_NORMAL_ITEM(MATERIAL_ESSENCE);
		CASE_GEN_NORMAL_ITEM(REFINE_TICKET_ESSENCE);
		CASE_GEN_NORMAL_ITEM(RECIPEROLL_ESSENCE);
		CASE_GEN_NORMAL_ITEM(TRANSMITROLL_ESSENCE);
		CASE_GEN_NORMAL_ITEM(LUCKYROLL_ESSENCE);
		CASE_GEN_NORMAL_ITEM2(TASKMATTER_ESSENCE);
		CASE_GEN_NORMAL_ITEM(TASKNORMALMATTER_ESSENCE);
		CASE_GEN_NORMAL_ITEM2(TASKDICE_ESSENCE);
		CASE_GEN_NORMAL_ITEM(DOUBLE_EXP_ESSENCE);
		CASE_GEN_NORMAL_ITEM(SKILLMATTER_ESSENCE);
		CASE_GEN_NORMAL_ITEM(VEHICLE_ESSENCE);
		CASE_GEN_NORMAL_ITEM(COUPLE_JUMPTO_ESSENCE);
		CASE_GEN_NORMAL_ITEM(LOTTERY_ESSENCE);
		CASE_GEN_NORMAL_ITEM(FIREWORKS_ESSENCE);
		CASE_GEN_NORMAL_ITEM(CAMRECORDER_ESSENCE);
		CASE_GEN_NORMAL_ITEM(TEXT_FIREWORKS_ESSENCE);
		CASE_GEN_NORMAL_ITEM(TALISMAN_MAINPART_ESSENCE);
		CASE_GEN_NORMAL_ITEM(TALISMAN_EXPFOOD_ESSENCE);
		CASE_GEN_NORMAL_ITEM(TALISMAN_MERGEKATALYST_ESSENCE);
		CASE_GEN_NORMAL_ITEM(SPEAKER_ESSENCE);
		CASE_GEN_NORMAL_ITEM(TOWNSCROLL_ESSENCE);
		CASE_GEN_NORMAL_ITEM(SIEGE_ARTILLERY_SCROLL_ESSENCE);
		CASE_GEN_NORMAL_ITEM(PET_BEDGE_ESSENCE);
		CASE_GEN_NORMAL_ITEM(PET_FOOD_ESSENCE);
		CASE_GEN_NORMAL_ITEM(PET_SKILL_ESSENCE);
		CASE_GEN_NORMAL_ITEM(PET_ARMOR_ESSENCE);
		CASE_GEN_NORMAL_ITEM(PET_AUTO_FOOD_ESSENCE);
		CASE_GEN_NORMAL_ITEM(PET_REFINE_ESSENCE);
		CASE_GEN_NORMAL_ITEM(PET_ASSIST_REFINE_ESSENCE);
		CASE_GEN_NORMAL_ITEM(AIRCRAFT_ESSENCE);
		CASE_GEN_NORMAL_ITEM(BOOK_ESSENCE);
		CASE_GEN_NORMAL_ITEM(OFFLINE_TRUSTEE_ESSENCE);
		CASE_GEN_NORMAL_ITEM(EQUIP_SOUL_ESSENCE);
		CASE_GEN_NORMAL_ITEM(SPECIAL_NAME_ITEM_ESSENCE);
		CASE_GEN_NORMAL_ITEM(GIFT_BAG_ESSENCE);
		CASE_GEN_NORMAL_ITEM(VIP_CARD_ESSENCE);
		CASE_GEN_NORMAL_ITEM(CHANGE_SHAPE_CARD_ESSENCE);
		CASE_GEN_NORMAL_ITEM(CHANGE_SHAPE_STONE_ESSENCE);
		CASE_GEN_NORMAL_ITEM(MERCENARY_CREDENTIAL_ESSENCE);
		CASE_GEN_NORMAL_ITEM(TELEPORTATION_ESSENCE);
		CASE_GEN_NORMAL_ITEM(TELEPORTATION_STONE_ESSENCE);
		CASE_GEN_NORMAL_ITEM(LOTTERY3_ESSENCE);
		CASE_GEN_NORMAL_ITEM(CASH_MEDIC_MERGE_ITEM_ESSENCE);
		
		case DT_MONSTER_ESSENCE:
		case DT_NPC_ESSENCE:
		default:
			ret = -1;
			size = 0;
		}

		if(ret==0 && size != 0)		
		{
			ASSERT(((item_data*)item)->pile_limit < 32768);
			sale_item_ptr_array.push_back(item);
			sale_item_size_array.push_back(size);
			LOCATION loc;
			loc.type = datatype;
			loc.pos = sale_item_ptr_array.size()-1;
			sale_item_id_index_map[id] = loc;
		}
		id = _edm->get_next_data_id(ID_SPACE_ESSENCE, datatype);
	}
	return 0;
}

int itemdataman::duplicate_static_item(unsigned int id, char ** list,  size_t& size)
{
	DATA_TYPE datatype;
	size_t pos;
	IDToLOCATIONMap::iterator itr;
	itr = sale_item_id_index_map.find(id);

	if(itr != sale_item_id_index_map.end())
	{
		pos = (itr->second).pos;
		datatype = (itr->second).type;
		void * data_static = sale_item_ptr_array[pos];
		size = sale_item_size_array[pos];

		*list = (char*) abase::fastalloc(size);
		memcpy(*list, data_static, size);
		((item_data*)(*list))->item_content = (*list) + sizeof(item_data);
		return 0;
	}
	else
		return -1;	
}


const void * itemdataman::get_item_for_sell(unsigned int id)
{
	size_t pos;
	IDToLOCATIONMap::iterator itr;
	itr = sale_item_id_index_map.find(id);

	if(itr != sale_item_id_index_map.end())
	{
		pos = (itr->second).pos;
		return sale_item_ptr_array[pos];
	}
	else
		return NULL;
}

int itemdataman::generate_item(unsigned int id, item_data ** item, size_t& size, const void * tag, size_t tag_size, int name_id)
{
	int ret = -1;
	DATA_TYPE datatype = get_data_type(id, ID_SPACE_ESSENCE);
	switch(datatype)
	{
	case DT_EQUIPMENT_ESSENCE:
	case DT_ESTONE_ESSENCE:
	case DT_PSTONE_ESSENCE:
	case DT_SSTONE_ESSENCE:
	case DT_MATERIAL_ESSENCE:
	case DT_REFINE_TICKET_ESSENCE:	
	case DT_RECIPEROLL_ESSENCE:
	case DT_MEDICINE_ESSENCE:
	case DT_TRANSMITROLL_ESSENCE:
	case DT_LUCKYROLL_ESSENCE:
	case DT_TASKMATTER_ESSENCE:
	case DT_TASKNORMALMATTER_ESSENCE:
	case DT_TASKDICE_ESSENCE:
	case DT_DOUBLE_EXP_ESSENCE:
	case DT_SKILLMATTER_ESSENCE:
	case DT_COUPLE_JUMPTO_ESSENCE:
	case DT_LOTTERY_ESSENCE:
	case DT_FIREWORKS_ESSENCE:
	case DT_CAMRECORDER_ESSENCE:
	case DT_TEXT_FIREWORKS_ESSENCE:
	case DT_TALISMAN_MAINPART_ESSENCE:
	case DT_TALISMAN_EXPFOOD_ESSENCE:
	case DT_TALISMAN_MERGEKATALYST_ESSENCE:
	case DT_TALISMAN_ENERGYFOOD_ESSENCE:
	case DT_SPEAKER_ESSENCE:
	case DT_TOWNSCROLL_ESSENCE:
	case DT_SIEGE_ARTILLERY_SCROLL_ESSENCE:
	case DT_PET_BEDGE_ESSENCE:
	case DT_PET_FOOD_ESSENCE:
	case DT_PET_SKILL_ESSENCE:
	case DT_PET_ARMOR_ESSENCE:
	case DT_PET_AUTO_FOOD_ESSENCE:
	case DT_PET_REFINE_ESSENCE:
	case DT_PET_ASSIST_REFINE_ESSENCE:
	case DT_AIRCRAFT_ESSENCE:
	case DT_FLY_ENERGYFOOD_ESSENCE:
	case DT_BOOK_ESSENCE:
	case DT_OFFLINE_TRUSTEE_ESSENCE:
	case DT_EQUIP_SOUL_ESSENCE:
	case DT_GIFT_BAG_ESSENCE:
	case DT_VIP_CARD_ESSENCE:
	case DT_CHANGE_SHAPE_CARD_ESSENCE:
	case DT_CHANGE_SHAPE_STONE_ESSENCE:
	case DT_MERCENARY_CREDENTIAL_ESSENCE:
	case DT_TELEPORTATION_ESSENCE:
	case DT_TELEPORTATION_STONE_ESSENCE:
		ret = duplicate_static_item(id, (char **)item, size);
		break;
	case DT_VEHICLE_ESSENCE:
		ret = generate_vehicle(id,ID_SPACE_ESSENCE,(char**)item,size,tag,tag_size);
		break;
	case DT_SPECIAL_NAME_ITEM_ESSENCE:
		ret = generate_special_name_item(id, ID_SPACE_ESSENCE, (char**)item, size, name_id);
		break;
		
	default:
		*item = NULL;
		size = 0;
		return -1;
	}
	
	if(ret == 0)
	{
		item_data * pItem = *item;
		if(pItem->proc_type & 0x0100) generate_item_guid(pItem);
	}
	return ret;
}


int itemdataman::generate_item_from_droptable(unsigned int id, int * list,  size_t list_size)
{
	//查找到掉落表
	int drop_id = id;
	if(drop_id <= 0) return 0;
	DATA_TYPE datatype2;
	const DROPTABLE_ESSENCE * table = (const DROPTABLE_ESSENCE*) get_data_ptr(drop_id, ID_SPACE_ESSENCE, datatype2);
	if(table == NULL || datatype2 != DT_DROPTABLE_ESSENCE)	
	{
		return 0;
	}
	
	size_t drop_count = 0;
	unsigned int drop_num = abase::RandSelect(table->num_to_drop,sizeof(float), 5);
	for(size_t i=0; i<drop_num; i++)
	{
		int index = abase::RandSelect(&(table->drops[0].probability), sizeof(unsigned int)+sizeof(float), 64);
		int item_id = table->drops[index].id_obj;
		if(item_id > 0)
		{
			list[drop_count] = item_id;
			drop_count ++;
			if(drop_count >= list_size) break;
		}
	}
	return drop_count;
}
