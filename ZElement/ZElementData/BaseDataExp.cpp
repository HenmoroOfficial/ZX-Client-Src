#include "stdafx.h"
#include "Global.h"
#include "ElementData.h"
#include "BaseDataTemplate.h"
#include "ExtendDataTempl.h"
#include "ExpTypes.h"
#include "BaseDataIDMan.h"
#include "elementdataman.h"
#include "WinUser.h"
#include "TalkModifyDlg.h"
#include "EC_MD5Hash.h"
#include "Policy.h"
#include "ElementDataDoc.h"
#include "AVariant.h"

static const char* _replace = "!$";

void ConvertMultilineText(char* szText, bool bToMultiline)
{
	if (bToMultiline)
	{
		char* pNext = strstr(szText, _replace);

		while (pNext)
		{
			pNext[0] = 0xD;
			pNext[1] = 0xA;

			pNext += 2;
			pNext = strstr(szText, _replace);
		}
	}
	else
	{
		char* pNext = strstr(szText, "\r\n");

		while (pNext)
		{
			pNext[0] = _replace[0];
			pNext[1] = _replace[1];

			pNext += 2;
			pNext = strstr(szText, "\r\n");
		}
	}
}

static const int PATH_LEN			= 128;
static const int NAME_LEN			= 32;
static const int TEXT_LEN			= 256;
static const int SINGLE_TEXT_LEN	= 16;
static const int ANSI_TEXT_LEN		= 32;
static const int PAGE_TITLE_LEN		= 8;
static const int SINGLE_DESC_LEN	= 100;	//Added 2012-03-20.
static const int DOUBLE_DESC_LEN	= 200;	//Added 2012-03-20.

#define FILL_BASE(tmpl, pStruct) \
{ \
	pStruct->id = tmpl.GetID(); \
	wcsncpy( \
		(wchar_t*)pStruct->name, \
		CSafeString(tmpl.GetName()), \
		sizeof(pStruct->name) / sizeof(namechar) - 1); \
}

const int _base_size = sizeof(unsigned int) + sizeof(namechar) * NAME_LEN;

inline void Fill_Data(char* pBuf, const BaseDataTempl& tmpl, int nSize, int i = 0)
{
	char* pOrg = pBuf;

	for (; i < tmpl.GetItemNum(); i++)
	{
		if (tmpl.GetItemType(i) == "path")
		{
			strncpy(pBuf, AString(tmpl.GetItemValue(i)), PATH_LEN - 1);
			pBuf += PATH_LEN;
		}
		else if (tmpl.GetItemType(i) == "vector")
		{
			A3DVECTOR3 vec = tmpl.GetItemValue(i);
			*(float*)pBuf = vec.x;
			pBuf += sizeof(float);
			*(float*)pBuf = vec.y;
			pBuf += sizeof(float);
			*(float*)pBuf = vec.z;
			pBuf += sizeof(float);
		}
		else if (tmpl.GetItemType(i) == "text_type")
		{
			CSafeString str(AString(tmpl.GetItemValue(i)));
			ConvertMultilineText(const_cast<char*>(str.GetAnsi()), true);
			wcsncpy((wchar_t*)pBuf, str, TEXT_LEN - 1);
			pBuf += TEXT_LEN * sizeof(namechar);
		}
		else if (tmpl.GetItemType(i) == "single_text")
		{
			wcsncpy((wchar_t*)pBuf, CSafeString(AString(tmpl.GetItemValue(i))), SINGLE_TEXT_LEN - 1);
			pBuf += SINGLE_TEXT_LEN * sizeof(namechar);
		}
		else if (tmpl.GetItemType(i) == "ansi_text")
		{
			strncpy((char*)pBuf, AString(tmpl.GetItemValue(i)), ANSI_TEXT_LEN - 1);
			pBuf += ANSI_TEXT_LEN;
		}
		else if (tmpl.GetItemType(i) == "page_title")
		{
			wcsncpy((wchar_t*)pBuf, CSafeString(AString(tmpl.GetItemValue(i))), PAGE_TITLE_LEN - 1);
			pBuf += PAGE_TITLE_LEN * sizeof(namechar);
		}
		else if (tmpl.GetItemType(i) == "single_desc")	//Added 2012-03-20.处理100长度的字符串的数据类型
		{
			wcsncpy((wchar_t*)pBuf, CSafeString(AString(tmpl.GetItemValue(i))), SINGLE_DESC_LEN - 1);
			pBuf += SINGLE_DESC_LEN * sizeof(namechar);
		}
		else if (tmpl.GetItemType(i) == "double_desc") //Added 2012-03-20.处理200长度的字符串的数据类型
		{
			wcsncpy((wchar_t*)pBuf, CSafeString(AString(tmpl.GetItemValue(i))), DOUBLE_DESC_LEN - 1);
			pBuf += DOUBLE_DESC_LEN * sizeof(namechar);
		}
		else if(tmpl.GetItemType(i) == "char")	//Added 2012-03-22.
		{
			*(char*)pBuf = ((char)tmpl.GetItemValue(i));
			pBuf += sizeof(char);
		}
		else if(tmpl.GetItemType(i) == "float") //Added 2012-03-22.考虑了下，还是觉得之前将float类型作为int处理太暴力了
		{
			*(float*)pBuf = ((float)tmpl.GetItemValue(i));
			pBuf += sizeof(float);
		}
		else if (tmpl.GetItemType(i) == "smallbool")
		{
			*(bool*)pBuf = ((bool)tmpl.GetItemValue(i));
			pBuf += sizeof(bool);
		}
		else if (tmpl.GetItemType(i) == "int64")
		{
			*(__int64*)pBuf = ((__int64)tmpl.GetItemValue(i));
			pBuf += sizeof(__int64);
		}
		else if (tmpl.GetItemType(i) == "double")
		{
			*(double*)pBuf = ((double)tmpl.GetItemValue(i));
			pBuf += sizeof(double);
		}
		else if (tmpl.GetItemType(i) == "uint64")
		{
			*(unsigned __int64*)pBuf = ((unsigned __int64)tmpl.GetItemValue(i));
			pBuf += sizeof(unsigned __int64);
		}
		else if( (tmpl.GetItemType(i) == "character_combo_id") || (tmpl.GetItemType(i) == "character_combo_id2") )
		{
			*(unsigned __int64*)pBuf = ((unsigned __int64)tmpl.GetItemValue(i));
			pBuf += sizeof(unsigned __int64);
 		}
		else
		{
			*(int*)pBuf = tmpl.GetItemValue(i);
			pBuf += sizeof(int);
		}
	}

	if (pBuf - pOrg != nSize - _base_size)
	{
		char buf[1024];
		sprintf(buf, "填充数据大小出错！！！，Name = %s, Path = %s, TemplSize = %d, StructSize = %d",
			tmpl.GetName(),
			tmpl.GetFilePath(),
			pBuf - pOrg,
			nSize - _base_size);
		::MessageBox(NULL, buf, "错误", MB_ICONSTOP);
	}
}

bool Fill_EQUIPMENT_ADDON(const ExtendDataTempl& tmpl, EQUIPMENT_ADDON* pStruct)
{
	FILL_BASE(tmpl, pStruct)

#ifdef _DEBUG
	if (tmpl.GetItemNum() > 4)
	{
		char buf[256];
		sprintf(buf, "Export: addon num > 3, Path = %s\n", tmpl.GetFilePath());
		OutputDebugString(buf);
	}
	ASSERT(tmpl.GetItemNum() >= 1);
#endif

	pStruct->num_params = tmpl.GetItemNum() - 1;
	for (int i = 0; i < tmpl.GetItemNum(); i++)
	{
		// we have the first param as addon type always.
		if (i == 0) pStruct->type = tmpl.GetItemValue(0);
		else if (i == 1) pStruct->param1 = tmpl.GetItemValue(1);
		else if (i == 2) pStruct->param2 = tmpl.GetItemValue(2);
		else if (i == 3) pStruct->param3 = tmpl.GetItemValue(3);
	}

	return true;
}

/*
 *	Weapon
 */
bool Fill_EQUIPMENT_MAJOR_TYPE(const BaseDataTempl& tmpl, EQUIPMENT_MAJOR_TYPE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	return true;
}

bool Fill_EQUIPMENT_SUB_TYPE(const BaseDataTempl& tmpl, EQUIPMENT_SUB_TYPE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(EQUIPMENT_SUB_TYPE));
	return true;
}

bool Fill_EQUIPMENT_ESSENCE(const BaseDataTempl& tmpl, EQUIPMENT_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)

	BaseDataTempl parent;
	if (!parent.LoadHeader(tmpl.ParseParentPath())) return false;
	pStruct->id_sub_type = parent.GetID();
	if (!parent.LoadHeader(parent.ParseParentPath())) return false;
	pStruct->id_major_type = parent.GetID();

	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(EQUIPMENT_ESSENCE) - 8);
	return true;
}

bool Fill_DESTROYING_ESSENCE(const BaseDataTempl& tmpl, DESTROYING_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(DESTROYING_ESSENCE));
	return true;
}

bool Fill_REFINE_TICKET_ESSENCE(const BaseDataTempl& tmpl, REFINE_TICKET_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(REFINE_TICKET_ESSENCE));
	return true;
}

bool Fill_OFFLINE_TRUSTEE_ESSENCE(const BaseDataTempl& tmpl, OFFLINE_TRUSTEE_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(OFFLINE_TRUSTEE_ESSENCE));
	return true;
}

bool Fill_LOTTERY_ESSENCE(const BaseDataTempl& tmpl, LOTTERY_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(LOTTERY_ESSENCE));
	return true;
}

bool Fill_LOTTERY2_ESSENCE(const BaseDataTempl& tmpl, LOTTERY2_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(LOTTERY2_ESSENCE));
	return true;
}

//Added 2011-07-01.
bool Fill_LOTTERY3_ESSENCE(const BaseDataTempl& tmpl, LOTTERY3_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(LOTTERY3_ESSENCE));
	return true;
}


bool Fill_CAMRECORDER_ESSENCE(const BaseDataTempl& tmpl, CAMRECORDER_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(CAMRECORDER_ESSENCE));
	return true;
}

bool Fill_SKILLMATTER_ESSENCE(const BaseDataTempl& tmpl, SKILLMATTER_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(SKILLMATTER_ESSENCE));
	return true;
}

bool Fill_VEHICLE_ESSENCE(const BaseDataTempl& tmpl, VEHICLE_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(VEHICLE_ESSENCE));
	return true;
}

bool Fill_AIRCRAFT_ESSENCE(const BaseDataTempl& tmpl, AIRCRAFT_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(AIRCRAFT_ESSENCE));
	return true;
}

bool Fill_FLY_ENERGYFOOD_ESSENCE(const BaseDataTempl& tmpl, FLY_ENERGYFOOD_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(FLY_ENERGYFOOD_ESSENCE));
	return true;
}

bool Fill_COUPLE_JUMPTO_ESSENCE(const BaseDataTempl& tmpl, COUPLE_JUMPTO_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(COUPLE_JUMPTO_ESSENCE));
	return true;
}

// 器魄
bool Fill_EQUIP_SOUL_ESSENCE(const BaseDataTempl& tmpl, EQUIP_SOUL_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(EQUIP_SOUL_ESSENCE));
	return true;
}

// 器魄融合
bool Fill_EQUIP_SOUL_MELD_SERVICE(const BaseDataTempl& tmpl, EQUIP_SOUL_MELD_SERVICE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(EQUIP_SOUL_MELD_SERVICE));
	return true;
}

bool Fill_SPECIAL_NAME_ITEM_ESSENCE(const BaseDataTempl& tmpl, SPECIAL_NAME_ITEM_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(SPECIAL_NAME_ITEM_ESSENCE));
	return true;
}

// 礼包
bool Fill_GIFT_BAG_ESSENCE(const BaseDataTempl& tmpl, GIFT_BAG_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(GIFT_BAG_ESSENCE));
	UInt64 testAdd = pStruct->normalize_group[0]+pStruct->normalize_group[1]+pStruct->normalize_group[2]+pStruct->normalize_group[3];
	if ((pStruct->normalize_group[0]|pStruct->normalize_group[1]|pStruct->normalize_group[2]|pStruct->normalize_group[3]) != testAdd)
	{
		char buf[1024];
		sprintf(buf, "礼包归一掩码错误！！！，Name = %s, Path = %s，请通知相关策划修改",
			tmpl.GetName(),
			tmpl.GetFilePath());
		::MessageBox(NULL, buf, "错误", MB_ICONSTOP);
	}
	int i;
	for (i=0; i<4; ++i)
	{
		float prob = .0f;
		int j;
		if (pStruct->normalize_group[i]==0)
			continue;

		for (j=0; j<16; ++j)
		{
			if (pStruct->normalize_group[i]&(1<<j))
			{
				prob += pStruct->gifts[j].probability;
			}			
		}
		if (abs(prob-1.0f)>0.001)
		{
			char buf[1024];
			sprintf(buf, "礼包归一%d概率归一错误：总概率：%.2f！！！，Name = %s, Path = %s，请通知相关策划修改",
				i+1,
				prob,
				tmpl.GetName(),
				tmpl.GetFilePath());
			::MessageBox(NULL, buf, "错误", MB_ICONSTOP);
		}
	}
	return true;
}

// VIP卡
bool Fill_VIP_CARD_ESSENCE(const BaseDataTempl& tmpl, VIP_CARD_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(VIP_CARD_ESSENCE));
	return true;
}

bool Fill_CHANGE_SHAPE_CARD_ESSENCE(const BaseDataTempl& tmpl, CHANGE_SHAPE_CARD_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(CHANGE_SHAPE_CARD_ESSENCE));
	return true;
}

bool Fill_CHANGE_SHAPE_STONE_ESSENCE(const BaseDataTempl& tmpl, CHANGE_SHAPE_STONE_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(CHANGE_SHAPE_STONE_ESSENCE));
	return true;
}
// 佣兵凭证
bool Fill_MERCENARY_CREDENTIAL_ESSENCE(const BaseDataTempl& tmpl, MERCENARY_CREDENTIAL_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(MERCENARY_CREDENTIAL_ESSENCE));
	return true;
}

bool Fill_TELEPORTATION_ESSENCE(const BaseDataTempl& tmpl, TELEPORTATION_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(TELEPORTATION_ESSENCE));
	return true;
}

bool Fill_TELEPORTATION_STONE_ESSENCE(const BaseDataTempl& tmpl, TELEPORTATION_STONE_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(TELEPORTATION_STONE_ESSENCE));
	return true;
}

bool Fill_UPGRADE_EQUIP_STONE_ESSENCE(const BaseDataTempl& tmpl, UPGRADE_EQUIP_STONE_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(UPGRADE_EQUIP_STONE_ESSENCE));
	return true;
}

/*
 *	Medicine
 */
bool Fill_MEDICINE_MAJOR_TYPE(const BaseDataTempl& tmpl, MEDICINE_MAJOR_TYPE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	return true;
}

bool Fill_MEDICINE_SUB_TYPE(const BaseDataTempl& tmpl, MEDICINE_SUB_TYPE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	return true;
}

bool Fill_MEDICINE_ESSENCE(const BaseDataTempl& tmpl, MEDICINE_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)

	BaseDataTempl parent;
	if (!parent.LoadHeader(tmpl.ParseParentPath())) return false;
	pStruct->id_sub_type = parent.GetID();
	if (!parent.LoadHeader(parent.ParseParentPath())) return false;
	pStruct->id_major_type = parent.GetID();

	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(MEDICINE_ESSENCE) - 8);
	return true;
}

/*
 *	Material
 */
bool Fill_MATERIAL_MAJOR_TYPE(const BaseDataTempl& tmpl, MATERIAL_MAJOR_TYPE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	return true;
}

bool Fill_MATERIAL_SUB_TYPE(const BaseDataTempl& tmpl, MATERIAL_SUB_TYPE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	return true;
}

bool Fill_MATERIAL_ESSENCE(const BaseDataTempl& tmpl, MATERIAL_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)

	BaseDataTempl parent;
	if (!parent.LoadHeader(tmpl.ParseParentPath())) return false;
	pStruct->id_sub_type = parent.GetID();
	if (!parent.LoadHeader(parent.ParseParentPath())) return false;
	pStruct->id_major_type = parent.GetID();

	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(MATERIAL_ESSENCE) - 8);
	return true;
}

/*
 *	Skill_Tome
 */
bool Fill_SKILLTOME_SUB_TYPE(const BaseDataTempl& tmpl, SKILLTOME_SUB_TYPE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	return true;
}

bool Fill_SKILLTOME_ESSENCE(const BaseDataTempl& tmpl, SKILLTOME_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)

	BaseDataTempl parent;
	if (!parent.LoadHeader(tmpl.ParseParentPath())) return false;
	pStruct->id_sub_type = parent.GetID();

	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(SKILLTOME_ESSENCE) - 4);
	return true;
}

/*
 *	Transmit_Scroll
 */
bool Fill_TRANSMITROLL_ESSENCE(const BaseDataTempl& tmpl, TRANSMITROLL_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(TRANSMITROLL_ESSENCE));
	return true;
}


/*
 *	Town_Scroll
 */
bool Fill_LUCKYROLL_ESSENCE(const BaseDataTempl& tmpl, LUCKYROLL_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(LUCKYROLL_ESSENCE));
	return true;
}


/*
 *	Town_Scroll
 */
bool Fill_TOWNSCROLL_ESSENCE(const BaseDataTempl& tmpl, TOWNSCROLL_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(TOWNSCROLL_ESSENCE));
	return true;
}

/*
 *	Revive_Scroll
 */
bool Fill_REVIVESCROLL_ESSENCE(const BaseDataTempl& tmpl, REVIVESCROLL_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(REVIVESCROLL_ESSENCE));
	return true;
}

/*
 *	SIEGE_ARTILLERY_SCROLL_ESSENCE
 */
bool Fill_SIEGE_ARTILLERY_SCROLL_ESSENCE(const BaseDataTempl& tmpl, SIEGE_ARTILLERY_SCROLL_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(SIEGE_ARTILLERY_SCROLL_ESSENCE));
	return true;
}

/*
 * pet bedge
 */
bool Fill_PET_BEDGE_ESSENCE(const BaseDataTempl& tmpl, PET_BEDGE_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(PET_BEDGE_ESSENCE));
	return true;
}

/*
 * pet food
 */
bool Fill_PET_FOOD_ESSENCE(const BaseDataTempl& tmpl, PET_FOOD_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(PET_FOOD_ESSENCE));
	return true;
}

bool Fill_PET_AUTO_FOOD_ESSENCE(const BaseDataTempl& tmpl, PET_AUTO_FOOD_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(PET_AUTO_FOOD_ESSENCE));
	return true;
}

bool Fill_PET_REFINE_ESSENCE(const BaseDataTempl& tmpl, PET_REFINE_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(PET_REFINE_ESSENCE));
	return true;
}

bool Fill_PET_ASSIST_REFINE_ESSENCE(const BaseDataTempl& tmpl, PET_ASSIST_REFINE_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(PET_ASSIST_REFINE_ESSENCE));
	return true;
}

/*
 * pet skill
 */
bool Fill_PET_SKILL_ESSENCE(const BaseDataTempl& tmpl, PET_SKILL_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(PET_SKILL_ESSENCE));
	return true;
}

/*
 * pet armor
 */
bool Fill_PET_ARMOR_ESSENCE(const BaseDataTempl& tmpl, PET_ARMOR_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(PET_ARMOR_ESSENCE));
	return true;
}

/*
 *	Task Matter
 */
bool Fill_TASKMATTER_ESSENCE(const BaseDataTempl& tmpl, TASKMATTER_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(TASKMATTER_ESSENCE));
	return true;
}

bool Fill_TASKNORMALMATTER_ESSENCE(const BaseDataTempl& tmpl, TASKNORMALMATTER_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(TASKNORMALMATTER_ESSENCE));
	return true;
}

bool Fill_TASKDICE_ESSENCE(const BaseDataTempl& tmpl, TASKDICE_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(TASKDICE_ESSENCE));
	return true;
}

/*
 * GM generator
*/
bool  Fill_GM_GENERATOR_TYPE(const BaseDataTempl& tmpl, GM_GENERATOR_TYPE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	return true;
}

bool Fill_GM_GENERATOR_ESSENCE(const BaseDataTempl& tmpl, GM_GENERATOR_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)

	BaseDataTempl parent;
	if (!parent.LoadHeader(tmpl.ParseParentPath())) return false;
	pStruct->id_type = parent.GetID();

	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(GM_GENERATOR_ESSENCE) - 4);
	return true;
}

/*
 * Fire works
 */
bool Fill_FIREWORKS_ESSENCE(const BaseDataTempl& tmpl, FIREWORKS_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(FIREWORKS_ESSENCE));
	return true;
}

/*
 * Fire works than can input text
 */
bool Fill_TEXT_FIREWORKS_ESSENCE(const BaseDataTempl& tmpl, TEXT_FIREWORKS_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(TEXT_FIREWORKS_ESSENCE));
	return true;
}

/*
 * Talisman mainpart
 */
bool Fill_TALISMAN_MAINPART_ESSENCE(const BaseDataTempl& tmpl, TALISMAN_MAINPART_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(TALISMAN_MAINPART_ESSENCE));
	return true;
}

/*
 * Talisman exp food
 */
bool Fill_TALISMAN_EXPFOOD_ESSENCE(const BaseDataTempl& tmpl, TALISMAN_EXPFOOD_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(TALISMAN_EXPFOOD_ESSENCE));
	return true;
}

/*
 * Talisman merge katalyst
 */
bool Fill_TALISMAN_MERGEKATALYST_ESSENCE(const BaseDataTempl& tmpl, TALISMAN_MERGEKATALYST_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(TALISMAN_MERGEKATALYST_ESSENCE));
	return true;
}

/*
 * Talisman energy food
 */
bool Fill_TALISMAN_ENERGYFOOD_ESSENCE(const BaseDataTempl& tmpl, TALISMAN_ENERGYFOOD_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(TALISMAN_ENERGYFOOD_ESSENCE));
	return true;
}

/*
 * Speaker
 */
bool Fill_SPEAKER_ESSENCE(const BaseDataTempl& tmpl, SPEAKER_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(SPEAKER_ESSENCE));
	return true;
}

/*
 * Book
 */
bool Fill_BOOK_ESSENCE(const BaseDataTempl& tmpl, BOOK_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(BOOK_ESSENCE));
	return true;
}

/*
 *	Monster
 */
bool Fill_NPC_TYPE(const BaseDataTempl& tmpl, NPC_TYPE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	return true;
}

bool Fill_DROPTABLE_TYPE(const BaseDataTempl& tmpl, DROPTABLE_TYPE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(DROPTABLE_TYPE));
	return true;
}

bool Fill_DROPTABLE_ESSENCE(const BaseDataTempl& tmpl, DROPTABLE_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	BaseDataTempl parent;
	if (!parent.LoadHeader(tmpl.ParseParentPath())) return false;
	pStruct->id_type = parent.GetID();
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(DROPTABLE_ESSENCE) - 4);
	return true;
}

bool Fill_MONSTER_TYPE(const BaseDataTempl& tmpl, MONSTER_TYPE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(MONSTER_TYPE));
	return true;
}

bool Fill_MONSTER_ESSENCE(const BaseDataTempl& tmpl, MONSTER_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	BaseDataTempl parent;
	if (!parent.LoadHeader(tmpl.ParseParentPath())) return false;
	pStruct->id_type = parent.GetID();
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(MONSTER_ESSENCE) - 4);
	return true;
}

/*
 *	NPC Talk Service
 */
bool Fill_NPC_TALK_SERVICE(const BaseDataTempl& tmpl, NPC_TALK_SERVICE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(NPC_TALK_SERVICE));
	return true;
}

/*
 *	NPC Sell Service
 */
bool Fill_NPC_SELL_SERVICE(const BaseDataTempl& tmpl, NPC_SELL_SERVICE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(NPC_SELL_SERVICE));
	return true;
}

/*
 *	NPC Buy Service
 */
bool Fill_NPC_BUY_SERVICE(const BaseDataTempl& tmpl, NPC_BUY_SERVICE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(NPC_BUY_SERVICE));
	return true;
}

/*
 *	NPC Task Service
 */
bool Fill_NPC_TASK_OUT_SERVICE(const BaseDataTempl& tmpl, NPC_TASK_OUT_SERVICE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(NPC_TASK_OUT_SERVICE));
	return true;
}

bool Fill_NPC_TASK_IN_SERVICE(const BaseDataTempl& tmpl, NPC_TASK_IN_SERVICE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(NPC_TASK_IN_SERVICE));
	return true;
}

bool Fill_NPC_TASK_MATTER_SERVICE(const BaseDataTempl& tmpl, NPC_TASK_MATTER_SERVICE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(NPC_TASK_MATTER_SERVICE));
	return true;
}

/*
 *	NPC Heal Service
 */
bool Fill_NPC_HEAL_SERVICE(const BaseDataTempl& tmpl, NPC_HEAL_SERVICE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(NPC_HEAL_SERVICE));
	return true;
}

/*
 *	NPC Transmit Service
 */
bool Fill_NPC_TRANSMIT_SERVICE(const BaseDataTempl& tmpl, NPC_TRANSMIT_SERVICE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(NPC_TRANSMIT_SERVICE));
	return true;
}

/*
 *	NPC Proxy Service
 */
bool Fill_NPC_PROXY_SERVICE(const BaseDataTempl& tmpl, NPC_PROXY_SERVICE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(NPC_PROXY_SERVICE));
	return true;
}

/*
 *	NPC Storage Service
 */
bool Fill_NPC_STORAGE_SERVICE(const BaseDataTempl& tmpl, NPC_STORAGE_SERVICE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(NPC_STORAGE_SERVICE));
	return true;
}

/*
 *	NPC tower build Service
 */
bool Fill_NPC_WAR_TOWERBUILD_SERVICE(const BaseDataTempl& tmpl, NPC_WAR_TOWERBUILD_SERVICE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(NPC_WAR_TOWERBUILD_SERVICE));
	return true;
}

/*
 *	NPC reset prop Service
 */
bool Fill_NPC_RESETPROP_SERVICE(const BaseDataTempl& tmpl, NPC_RESETPROP_SERVICE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(NPC_RESETPROP_SERVICE));
	return true;
}

/*
 *	NPC equipment bind Service
 */
bool Fill_NPC_EQUIPBIND_SERVICE(const BaseDataTempl& tmpl, NPC_EQUIPBIND_SERVICE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(NPC_EQUIPBIND_SERVICE));
	return true;
}

/*
 *	NPC equipment destroy Service
 */
bool Fill_NPC_EQUIPDESTROY_SERVICE(const BaseDataTempl& tmpl, NPC_EQUIPDESTROY_SERVICE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(NPC_EQUIPDESTROY_SERVICE));
	return true;
}

/*
 *	NPC equipment undestroy Service
 */
bool Fill_NPC_EQUIPUNDESTROY_SERVICE(const BaseDataTempl& tmpl, NPC_EQUIPUNDESTROY_SERVICE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(NPC_EQUIPUNDESTROY_SERVICE));
	return true;
}

/*
 *	NPC war buy archer Service
*/
bool Fill_NPC_WAR_BUY_ARCHER_SERVICE(const BaseDataTempl& tmpl, NPC_WAR_BUY_ARCHER_SERVICE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(NPC_WAR_BUY_ARCHER_SERVICE));
	return true;
}

/*
 *	NPC consign Service
*/
bool Fill_NPC_CONSIGN_SERVICE(const BaseDataTempl& tmpl, NPC_CONSIGN_SERVICE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(NPC_CONSIGN_SERVICE));
	return true;
}

bool Fill_NPC_REPUTATION_SHOP_SERVICE(const BaseDataTempl& tmpl, NPC_REPUTATION_SHOP_SERVICE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(NPC_REPUTATION_SHOP_SERVICE));
	return true;
}

bool Fill_NPC_UI_TRANSFER_SERVICE(const BaseDataTempl& tmpl, NPC_UI_TRANSFER_SERVICE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(NPC_UI_TRANSFER_SERVICE));
	return true;
}

/*
 *	NPC_Essence
 */
bool Fill_NPC_ESSENCE(const BaseDataTempl& tmpl, NPC_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(NPC_ESSENCE));
	return true;
}

bool  Fill_MINE_TYPE(const BaseDataTempl& tmpl, MINE_TYPE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	return true;
}

bool Fill_MINE_ESSENCE(const BaseDataTempl& tmpl, MINE_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)

	BaseDataTempl parent;
	if (!parent.LoadHeader(tmpl.ParseParentPath())) return false;
	pStruct->id_type = parent.GetID();

	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(MINE_ESSENCE) - 4);
	return true;
}

bool Fill_WAR_ROLE_CONFIG(const BaseDataTempl& tmpl, WAR_ROLE_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(WAR_ROLE_CONFIG));
	return true;
}

bool Fill_RECIPE_MAJOR_TYPE(const BaseDataTempl& tmpl, RECIPE_MAJOR_TYPE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	return true;
}

bool Fill_RECIPE_SUB_TYPE(const BaseDataTempl& tmpl, RECIPE_SUB_TYPE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	return true;
}

bool Fill_RECIPE_ESSENCE(const BaseDataTempl& tmpl, RECIPE_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)

	BaseDataTempl parent;
	if (!parent.LoadHeader(tmpl.ParseParentPath())) return false;
	pStruct->id_sub_type = parent.GetID();
	if (!parent.LoadHeader(parent.ParseParentPath())) return false;
	pStruct->id_major_type = parent.GetID();

	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(RECIPE_ESSENCE) - 8);
	return true;
}

bool Fill_ENEMY_FACTION_CONFIG(const BaseDataTempl& tmpl, ENEMY_FACTION_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(ENEMY_FACTION_CONFIG));
	return true;
}

bool Fill_CHARACTER_CLASS_CONFIG(const BaseDataTempl& tmpl, CHARACTER_CLASS_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(CHARACTER_CLASS_CONFIG));
	return true;
}

bool Fill_PARAM_ADJUST_CONFIG(const BaseDataTempl& tmpl, PARAM_ADJUST_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(PARAM_ADJUST_CONFIG));
	return true;
}

bool Fill_PIE_LOVE_CONFIG(const BaseDataTempl& tmpl, PIE_LOVE_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(PIE_LOVE_CONFIG));
	return true;
}

bool Fill_PLAYER_LEVELEXP_CONFIG(const BaseDataTempl& tmpl, PLAYER_LEVELEXP_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(PLAYER_LEVELEXP_CONFIG));
	return true;
}

bool Fill_PLAYER_SECONDLEVEL_CONFIG(const BaseDataTempl& tmpl, PLAYER_SECONDLEVEL_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(PLAYER_SECONDLEVEL_CONFIG));
	return true;
}

bool Fill_PLAYER_TALENT_CONFIG(const BaseDataTempl& tmpl, PLAYER_TALENT_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(PLAYER_TALENT_CONFIG));
	return true;
}

bool Fill_PLAYER_SKILL_POINT_CONFIG(const BaseDataTempl& tmpl, PLAYER_SKILL_POINT_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(PLAYER_SKILL_POINT_CONFIG));
	return true;
}

bool Fill_TITLE_PROP_CONFIG(const BaseDataTempl& tmpl, TITLE_PROP_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(TITLE_PROP_CONFIG));
	return true;
}

bool Fill_SPECIAL_ID_CONFIG(const BaseDataTempl& tmpl, SPECIAL_ID_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(SPECIAL_ID_CONFIG));
	return true;
}

bool Fill_RENASCENCE_SKILL_CONFIG(const BaseDataTempl& tmpl, RENASCENCE_SKILL_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(RENASCENCE_SKILL_CONFIG));
	return true;
}

bool Fill_RENASCENCE_PROP_CONFIG(const BaseDataTempl& tmpl, RENASCENCE_PROP_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(RENASCENCE_PROP_CONFIG));
	return true;
}

bool Fill_ITEM_TRADE_CONFIG(const BaseDataTempl& tmpl, ITEM_TRADE_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(ITEM_TRADE_CONFIG));
	return true;
}

bool Fill_RECYCLE_ITEM_CONFIG(const BaseDataTempl& tmpl, RECYCLE_ITEM_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(RECYCLE_ITEM_CONFIG));
	return true;
}

bool Fill_SCORE_TO_RANK_CONFIG(const BaseDataTempl& tmpl, SCORE_TO_RANK_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(SCORE_TO_RANK_CONFIG));
	return true;
}

bool Fill_BATTLE_DROP_CONFIG(const BaseDataTempl& tmpl, BATTLE_DROP_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(BATTLE_DROP_CONFIG));
	return true;
}

bool Fill_BATTLE_DEPRIVE_CONFIG(const BaseDataTempl& tmpl, BATTLE_DEPRIVE_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(BATTLE_DEPRIVE_CONFIG));
	return true;
}

bool Fill_BATTLE_SCORE_CONFIG(const BaseDataTempl& tmpl, BATTLE_SCORE_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(BATTLE_SCORE_CONFIG));
	return true;
}


bool Fill_INSTANCING_BATTLE_CONFIG(const BaseDataTempl& tmpl, INSTANCING_BATTLE_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(INSTANCING_BATTLE_CONFIG));
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//将编辑器中的模板数据存入到模板列表
//Added 2011-07-13. 
///////////////////////////////////////////////////////////////////////////////
bool Fill_RUNE_DATA_CONFIG(const BaseDataTempl& tmpl, RUNE_DATA_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(RUNE_DATA_CONFIG));
	return true;
}

bool Fill_RUNE_COMB_PROPERTY(const BaseDataTempl& tmpl, RUNE_COMB_PROPERTY* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(RUNE_COMB_PROPERTY));
	return true;
}

bool Fill_RUNE_EQUIP_ESSENCE(const BaseDataTempl& tmpl, RUNE_EQUIP_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(RUNE_EQUIP_ESSENCE));
	return true;
}

bool Fill_RUNE_SLOT_ESSENCE(const BaseDataTempl& tmpl, RUNE_SLOT_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(RUNE_SLOT_ESSENCE));
	return true;
}

bool Fill_BOOTH_FIGURE_ITEM_ESSENCE(const BaseDataTempl& tmpl, BOOTH_FIGURE_ITEM_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(BOOTH_FIGURE_ITEM_ESSENCE));
	return true;
}

bool Fill_FLAG_BUFF_ITEM_ESSENCE(const BaseDataTempl& tmpl, FLAG_BUFF_ITEM_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(FLAG_BUFF_ITEM_ESSENCE));
	return true;
}

bool Fill_XINGZUO_ITEM_ESSENCE(const BaseDataTempl& tmpl, XINGZUO_ITEM_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(XINGZUO_ITEM_ESSENCE));
	return true;
}

bool Fill_XINGZUO_ENERGY_ITEM_ESSENCE(const BaseDataTempl& tmpl, XINGZUO_ENERGY_ITEM_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(XINGZUO_ENERGY_ITEM_ESSENCE));
	return true;
}

bool Fill_CASH_MEDIC_MERGE_ITEM_ESSENCE(const BaseDataTempl& tmpl, CASH_MEDIC_MERGE_ITEM_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(CASH_MEDIC_MERGE_ITEM_ESSENCE));
	return true;
}

bool Fill_TRANSCRIPTION_CONFIG(const BaseDataTempl& tmpl, TRANSCRIPTION_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(TRANSCRIPTION_CONFIG));
	return true;
}

bool Fill_TOWER_TRANSCRIPTION_CONFIG(const BaseDataTempl& tmpl, TOWER_TRANSCRIPTION_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(TOWER_TRANSCRIPTION_CONFIG));
	return true;
}

bool Fill_TOWER_TRANSCRIPTION_PROPERTY_CONFIG(const BaseDataTempl& tmpl, TOWER_TRANSCRIPTION_PROPERTY_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(TOWER_TRANSCRIPTION_PROPERTY_CONFIG));
	return true;
}

bool Fill_LITTLE_PET_UPGRADE_CONFIG(const BaseDataTempl& tmpl, LITTLE_PET_UPGRADE_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(LITTLE_PET_UPGRADE_CONFIG));
	return true;
}

bool Fill_COLLISION_RAID_TRANSFIGURE_CONFIG(const BaseDataTempl& tmpl, COLLISION_RAID_TRANSFIGURE_CONFIG* pStruct)	//Added 2012-08-02.
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(COLLISION_RAID_TRANSFIGURE_CONFIG));
	return true;
}

bool Fill_UI_TRANSFER_CONFIG(const BaseDataTempl& tmpl, UI_TRANSFER_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(UI_TRANSFER_CONFIG));
	return true;
}

bool Fill_COLLISION_RAID_AWARD_CONFIG(const BaseDataTempl& tmpl, COLLISION_RAID_AWARD_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(COLLISION_RAID_AWARD_CONFIG));
	return true;
}

bool Fill_BUFF_AREA_CONFIG(const BaseDataTempl& tmpl, BUFF_AREA_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(BUFF_AREA_CONFIG));
	return true;
}

bool Fill_LIVENESS_CONFIG(const BaseDataTempl& tmpl, LIVENESS_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(LIVENESS_CONFIG));
	return true;
}

bool Fill_CHALLENGE_2012_CONFIG(const BaseDataTempl& tmpl, CHALLENGE_2012_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(CHALLENGE_2012_CONFIG));
	return true;
}

bool Fill_SALE_PROMOTION_ITEM_ESSENCE(const BaseDataTempl& tmpl, SALE_PROMOTION_ITEM_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(SALE_PROMOTION_ITEM_ESSENCE));
	return true;
}

bool Fill_GIFT_BAG_LOTTERY_DELIVER_ESSENCE(const BaseDataTempl& tmpl, GIFT_BAG_LOTTERY_DELIVER_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(GIFT_BAG_LOTTERY_DELIVER_ESSENCE));
	return true;
}

bool Fill_LOTTERY_TANGYUAN_ITEM_ESSENCE(const BaseDataTempl& tmpl, LOTTERY_TANGYUAN_ITEM_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(LOTTERY_TANGYUAN_ITEM_ESSENCE));
	return true;
}

bool Fill_GIFT_PACK_ITEM_ESSENCE(const BaseDataTempl& tmpl, GIFT_PACK_ITEM_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(GIFT_PACK_ITEM_ESSENCE));
	return true;
}

bool Fill_PROP_ADD_MATERIAL_ITEM_ESSENCE(const BaseDataTempl& tmpl, PROP_ADD_MATERIAL_ITEM_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(PROP_ADD_MATERIAL_ITEM_ESSENCE));
	return true;
}

bool Fill_PROP_ADD_ITEM_ESSENCE(const BaseDataTempl& tmpl, PROP_ADD_ITEM_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(PROP_ADD_ITEM_ESSENCE));
	return true;
}

bool Fill_TASK_SPECIAL_AWARD_CONFIG(const BaseDataTempl& tmpl, TASK_SPECIAL_AWARD_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(TASK_SPECIAL_AWARD_CONFIG));
	return true;
}

bool Fill_PROP_ADD_CONFIG(const BaseDataTempl& tmpl, PROP_ADD_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(PROP_ADD_CONFIG));
	return true;
}

bool Fill_KING_WAR_CONFIG(const BaseDataTempl& tmpl, KING_WAR_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(KING_WAR_CONFIG));
	return true;
}

bool Fill_JINFASHEN_TO_MONEY_CONFIG(const BaseDataTempl& tmpl, JINFASHEN_TO_MONEY_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(JINFASHEN_TO_MONEY_CONFIG));
	return true;
}

bool Fill_BATTLE_201304_CONFIG(const BaseDataTempl& tmpl, BATTLE_201304_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(BATTLE_201304_CONFIG));
	return true;
}

bool Fill_RUNE2013_ITEM_ESSENCE(const BaseDataTempl& tmpl, RUNE2013_ITEM_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(RUNE2013_ITEM_ESSENCE));
	return true;
}

bool Fill_COLORANT_ITEM_ESSENCE(const BaseDataTempl& tmpl, COLORANT_ITEM_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(COLORANT_ITEM_ESSENCE));
	return true;
}

bool Fill_INTERACTION_OBJECT_ESSENCE(const BaseDataTempl& tmpl, INTERACTION_OBJECT_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(INTERACTION_OBJECT_ESSENCE));
	return true;
}


bool Fill_COMPOUND_MINE_ESSENCE(const BaseDataTempl& tmpl, COMPOUND_MINE_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(COMPOUND_MINE_ESSENCE));
	return true;
}

bool Fill_BUILDING_ESSENCE(const BaseDataTempl& tmpl, BUILDING_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(BUILDING_ESSENCE));
	return true;
}
bool Fill_REGION_BLOCK_ESSENCE(const BaseDataTempl& tmpl, REGION_BLOCK_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(REGION_BLOCK_ESSENCE));
	return true;
}
bool Fill_RUNE2013_CONFIG(const BaseDataTempl& tmpl, RUNE2013_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(RUNE2013_CONFIG));
	return true;
}

bool Fill_BASHE_AWARD_CONFIG(const BaseDataTempl& tmpl, BASHE_AWARD_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(BASHE_AWARD_CONFIG));
	return true;
}

bool Fill_ARENA_SEASON_TIME_CONFIG(const BaseDataTempl& tmpl, ARENA_SEASON_TIME_CONFIG* pStruct)
{	
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(ARENA_SEASON_TIME_CONFIG));
	return true;
}

bool Fill_PERSONAL_LEVEL_AWARD_CONFIG(const BaseDataTempl& tmpl, PERSONAL_LEVEL_AWARD_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(PERSONAL_LEVEL_AWARD_CONFIG));
	return true;
}

bool Fill_TEAM_SEASON_AWARD_CONFIG(const BaseDataTempl& tmpl, TEAM_SEASON_AWARD_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(TEAM_SEASON_AWARD_CONFIG));
	return true;
}

bool Fill_WEEK_CURRNCY_AWARD_CONFIG(const BaseDataTempl& tmpl, WEEK_CURRNCY_AWARD_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(WEEK_CURRNCY_AWARD_CONFIG));
	return true;
}

bool Fill_INTERACTION_OBJECT_HOOK_CONFIG(const BaseDataTempl& tmpl, INTERACTION_OBJECT_HOOK_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(INTERACTION_OBJECT_HOOK_CONFIG));
	return true;
}

bool Fill_COLORANT_CONFIG(const BaseDataTempl& tmpl, COLORANT_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(COLORANT_CONFIG));
	return true;	
}

bool Fill_FACTION_TRANSFER_CONFIG(const BaseDataTempl& tmpl, FACTION_TRANSFER_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(FACTION_TRANSFER_CONFIG));
	return true;	
}

bool Fill_BUILDING_REGION_CONFIG(const BaseDataTempl& tmpl, BUILDING_REGION_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(BUILDING_REGION_CONFIG));
	return true;
}

bool Fill_PHASE_CONFIG(const BaseDataTempl& tmpl, PHASE_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(PHASE_CONFIG));
	return true;
}

bool Fill_TRANSCRIPT_STRATEGY_CONFIG(const BaseDataTempl& tmpl, TRANSCRIPT_STRATEGY_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(TRANSCRIPT_STRATEGY_CONFIG));
	return true;
}


bool Fill_FACTION_MONEY_SHOP_CONFIG(const BaseDataTempl& tmpl, FACTION_MONEY_SHOP_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(FACTION_MONEY_SHOP_CONFIG));
	return true;
}


bool Fill_COLORANT_DEFAULT_CONFIG(const BaseDataTempl& tmpl, COLORANT_DEFAULT_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(COLORANT_DEFAULT_CONFIG));
	return true;
}

bool Fill_FACTION_AUCTION_CONFIG(const BaseDataTempl& tmpl, FACTION_AUCTION_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(FACTION_AUCTION_CONFIG));
	return true;
}

bool Fill_FACTION_SHOP_CONFIG(const BaseDataTempl& tmpl, FACTION_SHOP_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(FACTION_SHOP_CONFIG));
	return true;
}

bool Fill_CROSS6V6_ITEM_EXCHANGE_CONFIG(const BaseDataTempl& tmpl, CROSS6V6_ITEM_EXCHANGE_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(CROSS6V6_ITEM_EXCHANGE_CONFIG));
	return true;
}

bool Fill_DROP_INTERNAL_CONFIG(const BaseDataTempl& tmpl, DROP_INTERNAL_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(DROP_INTERNAL_CONFIG));
	return true;
}

bool Fill_PK2012_GUESS_CONFIG(const BaseDataTempl& tmpl, PK2012_GUESS_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(PK2012_GUESS_CONFIG));
	return true;
}

bool Fill_CHANGE_SHAPE_PROP_CONFIG(const BaseDataTempl& tmpl, CHANGE_SHAPE_PROP_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(CHANGE_SHAPE_PROP_CONFIG));
	return true;
}

bool Fill_ORIGINAL_SHAPE_CONFIG(const BaseDataTempl& tmpl, ORIGINAL_SHAPE_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(ORIGINAL_SHAPE_CONFIG));
	return true;
}

bool Fill_LIFE_SKILL_CONFIG(const BaseDataTempl& tmpl, LIFE_SKILL_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(LIFE_SKILL_CONFIG));
	return true;
}

bool Fill_ARENA_DROPTABLE_CONFIG(const BaseDataTempl& tmpl, ARENA_DROPTABLE_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(ARENA_DROPTABLE_CONFIG));
	return true;
}

bool Fill_COMBINE_SKILL_EDIT_CONFIG(const BaseDataTempl& tmpl, COMBINE_SKILL_EDIT_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(COMBINE_SKILL_EDIT_CONFIG));
	return true;
}

bool Fill_UPGRADE_EQUIP_CONFIG(const BaseDataTempl& tmpl, UPGRADE_EQUIP_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(UPGRADE_EQUIP_CONFIG));
	return true;
}

bool Fill_UPGRADE_EQUIP_CONFIG_1(const BaseDataTempl& tmpl, UPGRADE_EQUIP_CONFIG_1* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(UPGRADE_EQUIP_CONFIG_1));
	return true;
}

bool Fill_DEITY_CHARACTER_CONFIG(const BaseDataTempl& tmpl, DEITY_CHARACTER_CONFIG* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(DEITY_CHARACTER_CONFIG));
	return true;
}
/*
	ESTONE_ESSENCE
*/
bool Fill_ESTONE_ESSENCE(const BaseDataTempl& tmpl, ESTONE_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(ESTONE_ESSENCE));
	return true;
}

/*
	PSTONE_ESSENCE
*/
bool Fill_PSTONE_ESSENCE(const BaseDataTempl& tmpl, PSTONE_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(PSTONE_ESSENCE));
	return true;
}

/*
	SSTONE_ESSENCE
*/
bool Fill_SSTONE_ESSENCE(const BaseDataTempl& tmpl, SSTONE_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(SSTONE_ESSENCE));
	return true;
}

/*
	POTENTIAL_TOME_ESSENCE
*/
bool Fill_POTENTIAL_TOME_ESSENCE(const BaseDataTempl& tmpl, POTENTIAL_TOME_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(POTENTIAL_TOME_ESSENCE));
	return true;
}

/*
	RECIPEROLL
*/
bool Fill_RECIPEROLL_MAJOR_TYPE(const BaseDataTempl& tmpl, RECIPEROLL_MAJOR_TYPE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	return true;
}

bool Fill_RECIPEROLL_SUB_TYPE(const BaseDataTempl& tmpl, RECIPEROLL_SUB_TYPE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	return true;
}

bool Fill_RECIPEROLL_ESSENCE(const BaseDataTempl& tmpl, RECIPEROLL_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)

	BaseDataTempl parent;
	if (!parent.LoadHeader(tmpl.ParseParentPath())) return false;
	pStruct->id_sub_type = parent.GetID();
	if (!parent.LoadHeader(parent.ParseParentPath())) return false;
	pStruct->id_major_type = parent.GetID();

	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(RECIPEROLL_ESSENCE) - 8);
	return true;
}

bool Fill_SUITE_ESSENCE(const BaseDataTempl& tmpl, SUITE_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(SUITE_ESSENCE));
	return true;
}

/*
 *	Double exp data
 */
bool Fill_DOUBLE_EXP_ESSENCE(const BaseDataTempl& tmpl, DOUBLE_EXP_ESSENCE* pStruct)
{
	FILL_BASE(tmpl, pStruct)
	Fill_Data((char*)(pStruct->name + NAME_LEN), tmpl, sizeof(DOUBLE_EXP_ESSENCE));
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//导出：宝石品级品质配置表模板
///////////////////////////////////////////////////////////////////////////////
bool Fill_GEM_CONFIG( const BaseDataTempl& tmpl, GEM_CONFIG* pStruct )
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data( (char*)(pStruct->name + NAME_LEN), tmpl, sizeof(GEM_CONFIG) );
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//导出：宝石本体模板
///////////////////////////////////////////////////////////////////////////////
bool Fill_GEM_ESSENCE( const BaseDataTempl& tmpl, GEM_ESSENCE* pStruct )
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data( (char*)(pStruct->name + NAME_LEN), tmpl, sizeof(GEM_ESSENCE) );
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//导出：宝石魔印本体模板
///////////////////////////////////////////////////////////////////////////////
bool Fill_GEM_SEAL_ESSENCE( const BaseDataTempl& tmpl, GEM_SEAL_ESSENCE* pStruct )
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data( (char*)(pStruct->name + NAME_LEN), tmpl, sizeof(GEM_SEAL_ESSENCE) );
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//导出：宝石粉尘本体模板
///////////////////////////////////////////////////////////////////////////////
bool Fill_GEM_DUST_ESSENCE( const BaseDataTempl& tmpl, GEM_DUST_ESSENCE* pStruct )
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data( (char*)(pStruct->name + NAME_LEN), tmpl, sizeof(GEM_DUST_ESSENCE) );
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//导出：萃取产物的配置模板
///////////////////////////////////////////////////////////////////////////////
bool Fill_GEM_EXTRACT_CONFIG( const BaseDataTempl& tmpl, GEM_EXTRACT_CONFIG* pStruct )
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data( (char*)(pStruct->name + NAME_LEN), tmpl, sizeof(GEM_EXTRACT_CONFIG) );
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//导出：通用道具的本体模板
///////////////////////////////////////////////////////////////////////////////
bool Fill_GENERAL_ARTICLE_ESSENCE( const BaseDataTempl& tmpl, GENERAL_ARTICLE_ESSENCE* pStruct )
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data( (char*)(pStruct->name + NAME_LEN), tmpl, sizeof(GENERAL_ARTICLE_ESSENCE) );
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//导出：在线奖励大礼包本体模板
///////////////////////////////////////////////////////////////////////////////
bool Fill_ONLINE_GIFT_BAG_CONFIG( const BaseDataTempl& tmpl, ONLINE_GIFT_BAG_CONFIG* pStruct )
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data( (char*)(pStruct->name + NAME_LEN), tmpl, sizeof(ONLINE_GIFT_BAG_CONFIG) );
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//导出：在线奖励小礼包本体模板
///////////////////////////////////////////////////////////////////////////////
bool Fill_SMALL_ONLINE_GIFT_BAG_ESSENCE( const BaseDataTempl& tmpl, SMALL_ONLINE_GIFT_BAG_ESSENCE* pStruct )
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data( (char*)(pStruct->name + NAME_LEN), tmpl, sizeof(SMALL_ONLINE_GIFT_BAG_ESSENCE) );
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//导出：诛仙古卷区域配置表模板 2012-03-20.
///////////////////////////////////////////////////////////////////////////////
bool Fill_SCROLL_REGION_CONFIG( const BaseDataTempl& tmpl, SCROLL_REGION_CONFIG* pStruct )
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data( (char*)(pStruct->name + NAME_LEN), tmpl, sizeof(SCROLL_REGION_CONFIG) );
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//导出：诛仙古卷解锁道具本体模板 2012-03-20.
///////////////////////////////////////////////////////////////////////////////
bool Fill_SCROLL_UNLOCK_ESSENCE( const BaseDataTempl& tmpl, SCROLL_UNLOCK_ESSENCE* pStruct )
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data( (char*)(pStruct->name + NAME_LEN), tmpl, sizeof(SCROLL_UNLOCK_ESSENCE) );
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//导出：诛仙古卷增加挖宝次数道具本体模板 2012-03-22.
///////////////////////////////////////////////////////////////////////////////
bool Fill_SCROLL_DIG_COUNT_INC_ESSENCE( const BaseDataTempl& tmpl, SCROLL_DIG_COUNT_INC_ESSENCE* pStruct )
{
	FILL_BASE(tmpl, pStruct);
	Fill_Data( (char*)(pStruct->name + NAME_LEN), tmpl, sizeof(SCROLL_DIG_COUNT_INC_ESSENCE) );
	return true;
}

/*
 *	Export all data
 */

#define EXPORT_DATA(type) \
{ \
	type data; \
	memset(&data, 0, sizeof(data)); \
	Fill_##type(tmpl, &data); \
	DataMan.add_structure(data.id, data); \
}

bool CheckMd5CodeForTemplate(char *szPathName)
{
	// turned off until we begin public test.
	return true;

	AFile file;
	PATCH::MD5Hash md;
	
	if (!file.Open(szPathName, AFILE_OPENEXIST | AFILE_TEXT))
		return true;
	
	char	szLine[AFILE_LINEMAXLEN];
	DWORD	dwRead;
	DWORD	dwVersion = 0;
	int		nItemNum = 0;

	file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
	//Skip previous version
	sscanf(szLine, "Version: %u", &dwVersion);
	if(dwVersion < 3)
	{
		// early version can not be used now.
		file.Close();
		return false;
	}
	md.Update(szLine,dwRead);

	file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
	md.Update(szLine,dwRead);

	file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
	md.Update(szLine,dwRead);

	file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
	md.Update(szLine,dwRead);
	
	file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
	md.Update(szLine,dwRead);
	
	sscanf(szLine, "ItemNum: %d", &nItemNum);
	
	for (int i = 0; i < nItemNum; i++)
	{
		file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
		md.Update(szLine,dwRead);
		file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
		md.Update(szLine,dwRead);
		
		//----------------------------------------
		file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
		md.Update(szLine,dwRead);
		int iType;
		sscanf(szLine,"Type: %d", &iType);
		switch (iType)
		{
		case AVariant::AVT_INVALIDTYPE:
			break;
		case AVariant::AVT_BOOL:
			file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
			md.Update(szLine,dwRead);
			break;
		case AVariant::AVT_INT:
		case AVariant::AVT_LONG:
			file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
			md.Update(szLine,dwRead);
			break;
		case AVariant::AVT_UINT:
		case AVariant::AVT_ULONG:
			file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
			md.Update(szLine,dwRead);
			break;
		case AVariant::AVT_FLOAT:
		case AVariant::AVT_DOUBLE:
			file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
			md.Update(szLine,dwRead);
			break;
		case AVariant::AVT_STRING:
			file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
			md.Update(szLine,dwRead);
			break;
		case AVariant::AVT_A3DVECTOR3: 
			file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
			md.Update(szLine,dwRead);
			break;
		case AVariant::AVT_INT64:
			file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
			md.Update(szLine, dwRead);
			break;
		case AVariant::AVT_UINT64:
			file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
			md.Update(szLine, dwRead);
			break;		
		default:
			ASSERT(FALSE);
		}
	}
	//Md5 finished
	md.Final();
	//Campare md5 code 
	while(file.GetPos() < file.GetFileLength())
	{
		file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
		if(dwRead==0) break;
		unsigned int nLenght = 64; 
		char szBuffer[64],szCode[64];
		md.GetString(szBuffer,nLenght);
		sscanf(szLine,"Md: %s",szCode);
		if(strcmp(szCode,szBuffer)==0) 
		{
			file.Close();
			return true;
		}
	}
	file.Close();
	return false;
}

bool ExportAllBaseData(const char* szPath)
{
	elementdataman DataMan;
	abase::vector<AString> PathArray;
	g_BaseIDMan.GeneratePathArray(PathArray);

	size_t i;
	for ( i = 0; i < PathArray.size(); i++)
	{
		BaseDataTempl tmpl;
		
		//Checking template by md5 code
#ifdef _MD5_CHECK
		if(!CheckMd5CodeForTemplate(PathArray[i].GetBuffer(0)))
		{
			CString str;
			str.Format("模板经MD5校验发现不合法! %s", PathArray[i]);
			AfxMessageBox(str);
			PathArray[i].ReleaseBuffer();
			PathArray.clear();
			return false;
		}
		PathArray[i].ReleaseBuffer();
#endif
		AString strPath = PathArray[i];
		int iVersion = TemplVersionState(strPath);		
		if (iVersion==VS_1)
		{
			AString strPrefix;
			strPrefix.Format("%x", g_ulVersion);
			if (FileIsExist(strPrefix+strPath))
			{
				strPath = strPrefix+strPath;
			}
		}
		if (tmpl.Load(strPath) != 0)
		{/*
#ifdef _DEBUG
			CString str;
			str.Format("不能加载 %s", PathArray[i]);
			AfxMessageBox(str);
#endif*/
			continue;
		}

		if (tmpl.GetID() != g_BaseIDMan.GetIDByPath(PathArray[i]))
		{
			CString str;
			str.Format("ID对应错误 %s", PathArray[i]);
			AfxMessageBox(str);
		}

		if (PathArray[i].Find("BaseData\\装备\\") != -1)
		{
			if (tmpl.GetLevel() == 1)
				EXPORT_DATA(EQUIPMENT_MAJOR_TYPE)
			else if (tmpl.GetLevel() == 2)
				EXPORT_DATA(EQUIPMENT_SUB_TYPE)
			else if (tmpl.GetLevel() == 3)
				EXPORT_DATA(EQUIPMENT_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\套装\\") != -1)
		{
			EXPORT_DATA(SUITE_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\精炼概率调整道具\\") != -1)
		{
			EXPORT_DATA(REFINE_TICKET_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\离线托管道具\\") != -1)
		{
			EXPORT_DATA(OFFLINE_TRUSTEE_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\彩票\\") != -1)
		{
			EXPORT_DATA(LOTTERY_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\彩票2\\") != -1)
		{
			EXPORT_DATA(LOTTERY2_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\彩票3\\") != -1)
		{
			EXPORT_DATA(LOTTERY3_ESSENCE);
		}
		else if (PathArray[i].Find("BaseData\\物品\\录像道具\\") != -1)
		{
			EXPORT_DATA(CAMRECORDER_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\药品\\") != -1)
		{
			if (tmpl.GetLevel() == 1)
				EXPORT_DATA(MEDICINE_MAJOR_TYPE)
			else if (tmpl.GetLevel() == 2)
				EXPORT_DATA(MEDICINE_SUB_TYPE)
			else if (tmpl.GetLevel() == 3)
				EXPORT_DATA(MEDICINE_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\正在被销毁的物品\\") != -1)
		{
			EXPORT_DATA(DESTROYING_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\生产原料\\") != -1)
		{
			if (tmpl.GetLevel() == 1)
				EXPORT_DATA(MATERIAL_MAJOR_TYPE)
			else if (tmpl.GetLevel() == 2)
				EXPORT_DATA(MATERIAL_SUB_TYPE)
			else if (tmpl.GetLevel() == 3)
				EXPORT_DATA(MATERIAL_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\技能书\\") != -1)
		{
			if (tmpl.GetLevel() == 1)
				EXPORT_DATA(SKILLTOME_SUB_TYPE)
			else if (tmpl.GetLevel() == 2)
				EXPORT_DATA(SKILLTOME_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\技能物品\\") != -1)
		{
			EXPORT_DATA(SKILLMATTER_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\交通工具\\") != -1)
		{
			EXPORT_DATA(VEHICLE_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\飞行器\\") != -1)
		{
			EXPORT_DATA(AIRCRAFT_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\飞行精力值药品\\") != -1)
		{
			EXPORT_DATA(FLY_ENERGYFOOD_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\复活卷轴\\") != -1)
		{
			EXPORT_DATA(REVIVESCROLL_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\攻城车用卷轴\\") != -1)
		{
			EXPORT_DATA(SIEGE_ARTILLERY_SCROLL_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\宠物牌\\") != -1)
		{
			EXPORT_DATA(PET_BEDGE_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\宠物食品\\") != -1)
		{
			EXPORT_DATA(PET_FOOD_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\宠物自动补充药\\") != -1)
		{
			EXPORT_DATA(PET_AUTO_FOOD_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\宠物修炼道具\\") != -1)
		{
			if (tmpl.GetLevel() == 2)
				EXPORT_DATA(PET_REFINE_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\宠物修炼辅助道具\\") != -1)
		{
			EXPORT_DATA(PET_ASSIST_REFINE_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\宠物技能卷轴\\") != -1)
		{
			EXPORT_DATA(PET_SKILL_ESSENCE)
		}
		else if	(PathArray[i].Find("BaseData\\物品\\宠物装备物品\\") != -1)
		{
			if (tmpl.GetLevel() == 2)
				EXPORT_DATA(PET_ARMOR_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\回城卷轴\\") != -1)
		{
			EXPORT_DATA(TOWNSCROLL_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\传送卷轴\\") != -1)
		{
			EXPORT_DATA(TRANSMITROLL_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\替身娃娃\\") != -1)
		{
			EXPORT_DATA(LUCKYROLL_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\任务物品\\") != -1)
		{
			if (tmpl.GetLevel() == 3)
				EXPORT_DATA(TASKMATTER_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\用于任务的普通物品\\") != -1)
		{
			if (tmpl.GetLevel() == 2)
				EXPORT_DATA(TASKNORMALMATTER_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\任务随机发生器\\") != -1)
		{
			EXPORT_DATA(TASKDICE_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\GM物品生成器\\") != -1)
		{
			if (tmpl.GetLevel() == 1)
				EXPORT_DATA(GM_GENERATOR_TYPE)
			else if (tmpl.GetLevel() == 2)
				EXPORT_DATA(GM_GENERATOR_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\烟花\\") != -1)
		{
			EXPORT_DATA(FIREWORKS_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\文字烟花\\") != -1)
		{
			EXPORT_DATA(TEXT_FIREWORKS_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\强化石\\") != -1)
		{
			EXPORT_DATA(ESTONE_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\属性石\\") != -1)
		{
			EXPORT_DATA(PSTONE_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\技能石\\") != -1)
		{
			EXPORT_DATA(SSTONE_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\潜能书\\") != -1)
		{
			EXPORT_DATA(POTENTIAL_TOME_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\夫妻同心结\\") != -1)
		{
			EXPORT_DATA(COUPLE_JUMPTO_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\配方卷轴\\") != -1)
		{
			if (tmpl.GetLevel() == 1)
				EXPORT_DATA(RECIPEROLL_MAJOR_TYPE)
			else if (tmpl.GetLevel() == 2)
				EXPORT_DATA(RECIPEROLL_SUB_TYPE)
			else if (tmpl.GetLevel() == 3)
				EXPORT_DATA(RECIPEROLL_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\双倍经验道具\\") != -1)
		{
			EXPORT_DATA(DOUBLE_EXP_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\法宝血炼材料\\") != -1)
		{
			EXPORT_DATA(TALISMAN_EXPFOOD_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\法宝精力增长道具\\") != -1)
		{
			EXPORT_DATA(TALISMAN_ENERGYFOOD_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\喇叭\\") != -1)
		{
			EXPORT_DATA(SPEAKER_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\书籍\\") != -1)
		{
			EXPORT_DATA(BOOK_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\法宝熔炼剂\\") != -1)
		{
			EXPORT_DATA(TALISMAN_MERGEKATALYST_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\NPC\\功能\\器魄融合\\") != -1)
		{
			EXPORT_DATA(EQUIP_SOUL_MELD_SERVICE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\器魄\\") != -1)
		{
			EXPORT_DATA(EQUIP_SOUL_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\特殊命名逻辑的物品\\") != -1)
		{
			EXPORT_DATA(SPECIAL_NAME_ITEM_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\礼包\\") != -1)
		{
			EXPORT_DATA(GIFT_BAG_ESSENCE);
		}
		else if (PathArray[i].Find("BaseData\\物品\\VIP卡\\") != -1)
		{
			EXPORT_DATA(VIP_CARD_ESSENCE);
		}
		else if (PathArray[i].Find("BaseData\\物品\\变身道具\\") != -1)
		{
			EXPORT_DATA(CHANGE_SHAPE_CARD_ESSENCE);
		}
		else if(PathArray[i].Find("BaseData\\物品\\变身炼化道具\\") != -1)
		{
			EXPORT_DATA(CHANGE_SHAPE_STONE_ESSENCE);
		}
		else if(PathArray[i].Find("BaseData\\物品\\佣兵凭证\\") != -1)
		{
			EXPORT_DATA(MERCENARY_CREDENTIAL_ESSENCE);
		}
		else if(PathArray[i].Find("BaseData\\物品\\星盘\\") != -1)
		{
			EXPORT_DATA(TELEPORTATION_ESSENCE);
		}
		else if(PathArray[i].Find("BaseData\\物品\\星盘充能石\\") != -1)
		{
			EXPORT_DATA(TELEPORTATION_STONE_ESSENCE);
		}
		else if(PathArray[i].Find("BaseData\\物品\\装备升级石\\") != -1)		
		{
			EXPORT_DATA(UPGRADE_EQUIP_STONE_ESSENCE);
		}
		else if( PathArray[i].Find("BaseData\\物品\\宝石\\") != -1 )//Added 2011-03-15.
		{
			EXPORT_DATA(GEM_ESSENCE);
		}
		else if( PathArray[i].Find("BaseData\\物品\\宝石魔印\\") != -1 )//Added 2011-03-15.
		{
			EXPORT_DATA(GEM_SEAL_ESSENCE);
		}
		else if( PathArray[i].Find("BaseData\\物品\\宝石粉尘\\") != -1 )//Added 2011-03-15.
		{
			EXPORT_DATA(GEM_DUST_ESSENCE);
		}
		else if( PathArray[i].Find("BaseData\\物品\\通用道具\\") != -1 )//Added 2011-03-17.
		{
			EXPORT_DATA(GENERAL_ARTICLE_ESSENCE);
		}
		else if( PathArray[i].Find("BaseData\\物品\\在线奖励小礼包\\") != -1 )//Added 2011-12-06.
		{
			EXPORT_DATA(SMALL_ONLINE_GIFT_BAG_ESSENCE);
		}
		else if( PathArray[i].Find("BaseData\\物品\\诛仙古卷解锁道具\\") != -1 ) //Added 2012-03-20.
		{
			EXPORT_DATA(SCROLL_UNLOCK_ESSENCE);
		}
		else if( PathArray[i].Find("BaseData\\物品\\诛仙古卷挖掘次数道具\\") != -1 ) //Added 2012-03-22.
		{
			EXPORT_DATA(SCROLL_DIG_COUNT_INC_ESSENCE);
		}
		else if (PathArray[i].Find("BaseData\\法宝主件\\") != -1)
		{
			EXPORT_DATA(TALISMAN_MAINPART_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\怪物\\") != -1)
		{
			if (tmpl.GetLevel() == 1)
				EXPORT_DATA(MONSTER_TYPE)
			else if (tmpl.GetLevel() == 2)
				EXPORT_DATA(MONSTER_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\掉落表\\") != -1)
		{
			if (tmpl.GetLevel() == 1)
				EXPORT_DATA(DROPTABLE_TYPE)
			else if (tmpl.GetLevel() == 2)
				EXPORT_DATA(DROPTABLE_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\NPC\\NPC类型\\") != -1)
		{
			EXPORT_DATA(NPC_TYPE)
		}
		else if (PathArray[i].Find("BaseData\\NPC\\功能\\出售商品\\") != -1)
		{
			EXPORT_DATA(NPC_SELL_SERVICE)
		}
		else if (PathArray[i].Find("BaseData\\NPC\\功能\\传送\\") != -1)
		{
			EXPORT_DATA(NPC_TRANSMIT_SERVICE)
		}
		else if (PathArray[i].Find("BaseData\\NPC\\功能\\存储\\") != -1)
		{
			EXPORT_DATA(NPC_STORAGE_SERVICE)
		}
		else if (PathArray[i].Find("BaseData\\NPC\\功能\\代售\\") != -1)
		{
			EXPORT_DATA(NPC_PROXY_SERVICE)
		}
		else if (PathArray[i].Find("BaseData\\NPC\\功能\\交谈\\") != -1)
		{
			EXPORT_DATA(NPC_TALK_SERVICE)
		}
		else if (PathArray[i].Find("BaseData\\NPC\\功能\\发放任务\\") != -1)
		{
			EXPORT_DATA(NPC_TASK_OUT_SERVICE)
		}
		else if (PathArray[i].Find("BaseData\\NPC\\功能\\验证完成任务\\") != -1)
		{
			EXPORT_DATA(NPC_TASK_IN_SERVICE)
		}
		else if (PathArray[i].Find("BaseData\\NPC\\功能\\发放任务物品\\") != -1)
		{
			EXPORT_DATA(NPC_TASK_MATTER_SERVICE)
		}
		else if (PathArray[i].Find("BaseData\\NPC\\功能\\收购商品\\") != -1)
		{
			EXPORT_DATA(NPC_BUY_SERVICE)
		}
		else if (PathArray[i].Find("BaseData\\NPC\\功能\\治疗\\") != -1)
		{
			EXPORT_DATA(NPC_HEAL_SERVICE)
		}
		else if (PathArray[i].Find("BaseData\\NPC\\功能\\城战炮塔建造\\") != -1)
		{
			EXPORT_DATA(NPC_WAR_TOWERBUILD_SERVICE)
		}
		else if (PathArray[i].Find("BaseData\\NPC\\功能\\洗点服务\\") != -1)
		{
			EXPORT_DATA(NPC_RESETPROP_SERVICE)
		}
		else if (PathArray[i].Find("BaseData\\NPC\\功能\\装备绑定服务\\") != -1)
		{
			EXPORT_DATA(NPC_EQUIPBIND_SERVICE)
		}
		else if (PathArray[i].Find("BaseData\\NPC\\功能\\装备销毁服务\\") != -1)
		{
			EXPORT_DATA(NPC_EQUIPDESTROY_SERVICE)
		}
		else if (PathArray[i].Find("BaseData\\NPC\\功能\\装备解除销毁服务\\") != -1)
		{
			EXPORT_DATA(NPC_EQUIPUNDESTROY_SERVICE)
		}
		else if (PathArray[i].Find("BaseData\\NPC\\功能\\城战购买弓箭手\\") != -1)
		{
			EXPORT_DATA(NPC_WAR_BUY_ARCHER_SERVICE)
		}
		else if (PathArray[i].Find("BaseData\\NPC\\功能\\商品寄售\\") != -1)
		{
			EXPORT_DATA(NPC_CONSIGN_SERVICE)
		}
		else if (PathArray[i].Find("BaseData\\NPC\\功能\\声望商店\\") != -1)
		{
			EXPORT_DATA(NPC_REPUTATION_SHOP_SERVICE)
		}
		else if (PathArray[i].Find("BaseData\\NPC\\功能\\UI传输\\") != -1)
		{
			EXPORT_DATA(NPC_UI_TRANSFER_SERVICE)
		}
		else if (PathArray[i].Find("BaseData\\NPC\\功能NPC\\") != -1)
		{
			EXPORT_DATA(NPC_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\NPC\\矿\\") != -1)
		{
			if (tmpl.GetLevel() == 1)
				EXPORT_DATA(MINE_TYPE)
			else if (tmpl.GetLevel() == 2)
				EXPORT_DATA(MINE_ESSENCE)
		}
		else if(PathArray[i].Find("BaseData\\NPC\\互动物体\\") != -1)
		{
			EXPORT_DATA(INTERACTION_OBJECT_ESSENCE);
		}
		else if(PathArray[i].Find("BaseData\\NPC\\复合矿\\") != -1)
		{
			EXPORT_DATA(COMPOUND_MINE_ESSENCE);
		}
		else if (PathArray[i].Find("BaseData\\NPC\\城战角色属性表\\") != -1)
		{
			EXPORT_DATA(WAR_ROLE_CONFIG)
		}
		else if (PathArray[i].Find("BaseData\\NPC\\功能\\物物交易\\") != -1)
		{
			EXPORT_DATA(ITEM_TRADE_CONFIG);
		}
		else if (PathArray[i].Find("BaseData\\元魂符语\\") != -1)
		{
			EXPORT_DATA(RUNE_EQUIP_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\符语\\") != -1)
		{
			EXPORT_DATA(RUNE_SLOT_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\摆摊道具\\") != -1)
		{
			EXPORT_DATA(BOOTH_FIGURE_ITEM_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\战旗\\") != -1)
		{
			EXPORT_DATA(FLAG_BUFF_ITEM_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\星座\\") != -1)
		{
			EXPORT_DATA(XINGZUO_ITEM_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\星座精力石\\") != -1)
		{
			EXPORT_DATA(XINGZUO_ENERGY_ITEM_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\大药合并道具\\") != -1)
		{
			EXPORT_DATA(CASH_MEDIC_MERGE_ITEM_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\台历\\") != -1)
		{
			EXPORT_DATA(SALE_PROMOTION_ITEM_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\礼包彩票赠送\\") != -1)
		{
			EXPORT_DATA(GIFT_BAG_LOTTERY_DELIVER_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\汤圆彩票\\") != -1)
		{
			EXPORT_DATA(LOTTERY_TANGYUAN_ITEM_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\礼包盒子\\") != -1)
		{
			EXPORT_DATA(GIFT_PACK_ITEM_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\基础属性增益原材料道具\\") != -1)
		{
			EXPORT_DATA(PROP_ADD_MATERIAL_ITEM_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\基础属性增益道具\\") != -1)
		{
			EXPORT_DATA(PROP_ADD_ITEM_ESSENCE)
		}
		else if (PathArray[i].Find("BaseData\\物品\\符文2013\\") != -1)
		{
			EXPORT_DATA(RUNE2013_ITEM_ESSENCE)
		}
		else if(PathArray[i].Find("BaseData\\物品\\染色剂\\") != -1)
		{		
			EXPORT_DATA(COLORANT_ITEM_ESSENCE);
		}
		else if(PathArray[i].Find("BaseData\\建筑\\") != -1)
		{
			EXPORT_DATA(BUILDING_ESSENCE);
		}
		else if(PathArray[i].Find("BaseData\\地块\\") != -1)
		{
			EXPORT_DATA(REGION_BLOCK_ESSENCE);
		}
		else
		{
#ifdef _DEBUG
			char buf[256];
			sprintf(buf, "Export: Unknown Templ %s\n", PathArray[i]);
			OutputDebugString(buf);
#endif
		}

		tmpl.Release();
	}

	PathArray.clear();
	g_ExtBaseIDMan.GeneratePathArray(PathArray);

	for (i = 0; i < PathArray.size(); i++)
	{
		ExtendDataTempl tmpl;
		
		//Checking template by md5 code
#ifdef _MD5_CHECK
		if(!CheckMd5CodeForTemplate(PathArray[i].GetBuffer(0)))
		{
			CString str;
			str.Format("模板经MD5校验发现不合法! %s", PathArray[i]);
			AfxMessageBox(str);
			PathArray[i].ReleaseBuffer();
			PathArray.clear();
			return false;
		}
		PathArray[i].ReleaseBuffer();
#endif
		if (tmpl.Load(PathArray[i], false) != 0)
		{
#ifdef _DEBUG
			char buf[256];
			sprintf(buf, "Export: Cant Load %s\n", PathArray[i]);
			AfxMessageBox(buf);
#endif
			continue;
		}

		EXPORT_DATA(EQUIPMENT_ADDON);

		tmpl.Release();
	}

	PathArray.clear();
	g_TalkIDMan.GeneratePathArray(PathArray);

	for (i = 0; i < PathArray.size(); i++)
	{
		CTalkModifyDlg talk;
		talk_proc* tp = new talk_proc;
		memset(tp, 0, sizeof(talk_proc));

		if (!talk.ExportData(tp, PathArray[i], g_TalkIDMan.GetIDByPath(PathArray[i])))
		{
#ifdef _DEBUG
			char buf[256];
			sprintf(buf, "Export: Load Talk Data Error %s\n", PathArray[i]);
			OutputDebugString(buf);
#endif
			delete tp;
			continue;
		}

		DataMan.add_structure(tp->id_talk, tp);
	}

	PathArray.clear();
	g_RecipeIDMan.GeneratePathArray(PathArray);

	for (i = 0; i < PathArray.size(); i++)
	{
		//Checking template by md5 code
#ifdef _MD5_CHECK
		if(!CheckMd5CodeForTemplate(PathArray[i].GetBuffer(0)))
		{
			CString str;
			str.Format("模板经MD5校验发现不合法! %s", PathArray[i]);
			AfxMessageBox(str);
			PathArray[i].ReleaseBuffer();
			PathArray.clear();
			return false;
		}
		PathArray[i].ReleaseBuffer();
#endif
		BaseDataTempl tmpl;
		if (tmpl.Load(PathArray[i]) != 0)
		{
#ifdef _DEBUG
			char buf[256];
			sprintf(buf, "Export: Cant Load %s\n", PathArray[i]);
			OutputDebugString(buf);
#endif
			continue;
		}

		if (PathArray[i].Find("BaseData\\配方\\") != -1)
		{
			if (tmpl.GetLevel() == 1)
				EXPORT_DATA(RECIPE_MAJOR_TYPE)
			else if (tmpl.GetLevel() == 2)
				EXPORT_DATA(RECIPE_SUB_TYPE)
			else if (tmpl.GetLevel() == 3)
				EXPORT_DATA(RECIPE_ESSENCE)
		}
		else
		{
#ifdef _DEBUG
			char buf[256];
			sprintf(buf, "Export: Unknown Templ %s\n", PathArray[i]);
			OutputDebugString(buf);
#endif
		}

		tmpl.Release();
	}

	PathArray.clear();
	g_ConfigIDMan.GeneratePathArray(PathArray);

	for (i = 0; i < PathArray.size(); i++)
	{
		//Checking template by md5 code
#ifdef _MD5_CHECK
		if(!CheckMd5CodeForTemplate(PathArray[i].GetBuffer(0)))
		{
			CString str;
			str.Format("模板经MD5校验发现不合法! %s", PathArray[i]);
			AfxMessageBox(str);
			PathArray[i].ReleaseBuffer();
			PathArray.clear();
			return false;
		}
		PathArray[i].ReleaseBuffer();
#endif	
		
		BaseDataTempl tmpl;
		AString strPath = PathArray[i];
		int iVersion = TemplVersionState(strPath);		
		if (iVersion==VS_1)
		{
			AString strPrefix;
			strPrefix.Format("%x", g_ulVersion);
			if (FileIsExist(strPrefix+strPath))
			{
				strPath = strPrefix+strPath;
			}
		}
		if (tmpl.Load(strPath) != 0)
		{
#ifdef _DEBUG
			char buf[256];
			sprintf(buf, "Export: Cant Load %s\n", PathArray[i]);
			OutputDebugString(buf);
#endif
			continue;
		}

		if (PathArray[i].Find("BaseData\\Config\\敌对阵营列表\\") != -1)
		{
			EXPORT_DATA(ENEMY_FACTION_CONFIG)
		}
		else if (PathArray[i].Find("BaseData\\Config\\职业属性列表\\") != -1)
		{
			EXPORT_DATA(CHARACTER_CLASS_CONFIG)
		}
		else if (PathArray[i].Find("BaseData\\Config\\数据修正表\\") != -1)
		{
			EXPORT_DATA(PARAM_ADJUST_CONFIG)
		}
		else if (PathArray[i].Find("BaseData\\Config\\交友平台配置表\\") != -1)
		{
			EXPORT_DATA(PIE_LOVE_CONFIG)
		}
		else if (PathArray[i].Find("BaseData\\Config\\玩家升级曲线表\\") != -1)
		{
			EXPORT_DATA(PLAYER_LEVELEXP_CONFIG);
		}
		else if (PathArray[i].Find("BaseData\\Config\\玩家修真级别表\\") != -1)
		{
			EXPORT_DATA(PLAYER_SECONDLEVEL_CONFIG);
		}
		else if (PathArray[i].Find("BaseData\\Config\\天赋点数表\\") != -1)
		{
			EXPORT_DATA(PLAYER_TALENT_CONFIG);
		}
		else if (PathArray[i].Find("BaseData\\Config\\技能点数表\\") != -1)
		{
			EXPORT_DATA(PLAYER_SKILL_POINT_CONFIG);
		}
		else if (PathArray[i].Find("BaseData\\Config\\称谓属性表\\") != -1)
		{
			EXPORT_DATA(TITLE_PROP_CONFIG);
		}
		else if (PathArray[i].Find("BaseData\\Config\\特殊ID定义表\\") != -1)
		{
			EXPORT_DATA(SPECIAL_ID_CONFIG);
		}
		else if (PathArray[i].Find("BaseData\\Config\\转生技能\\") != -1)
		{
			EXPORT_DATA(RENASCENCE_SKILL_CONFIG);
		}
		else if (PathArray[i].Find("BaseData\\Config\\转生属性\\") != -1)
		{
			EXPORT_DATA(RENASCENCE_PROP_CONFIG);
		}
		else if (PathArray[i].Find("BaseData\\Config\\回收物品表\\") != -1)
		{
			EXPORT_DATA(RECYCLE_ITEM_CONFIG);
		}
		else if (PathArray[i].Find("BaseData\\Config\\积分与军衔对应表\\") != -1)
		{
			EXPORT_DATA(SCORE_TO_RANK_CONFIG);
		}
		else if (PathArray[i].Find("BaseData\\Config\\宋金战场死亡掉落表\\") != -1)
		{
			EXPORT_DATA(BATTLE_DROP_CONFIG);
		}
		else if (PathArray[i].Find("BaseData\\Config\\宋金战场死亡掠夺表\\") != -1)
		{
			EXPORT_DATA(BATTLE_DEPRIVE_CONFIG);
		}
		else if (PathArray[i].Find("BaseData\\Config\\战场积分获取配置表\\") != -1)
		{
			EXPORT_DATA(BATTLE_SCORE_CONFIG);
		}
		else if(PathArray[i].Find("BaseData\\Config\\剧情战场配置表\\") != -1)
		{
			EXPORT_DATA(INSTANCING_BATTLE_CONFIG);
		}
		else if(PathArray[i].Find("BaseData\\Config\\变身属性配置表\\") != -1)
		{
			EXPORT_DATA(CHANGE_SHAPE_PROP_CONFIG);
		}
		else if(PathArray[i].Find("BaseData\\Config\\显形规则表\\") != -1)
		{
			EXPORT_DATA(ORIGINAL_SHAPE_CONFIG);
		}
		else if(PathArray[i].Find("BaseData\\Config\\生活技能配置表\\") != -1)
		{
			EXPORT_DATA(LIFE_SKILL_CONFIG);
		}
		else if(PathArray[i].Find("BaseData\\Config\\竞技场死亡掉落配置表\\") != -1)
		{
			EXPORT_DATA(ARENA_DROPTABLE_CONFIG);
		}
		else if(PathArray[i].Find("BaseData\\Config\\组合技编辑冷却配置表\\") != -1)
		{
			EXPORT_DATA(COMBINE_SKILL_EDIT_CONFIG);
		}
		else if(PathArray[i].Find("BaseData\\Config\\装备升级配置表1\\") != -1)
		{
			EXPORT_DATA(UPGRADE_EQUIP_CONFIG_1);
		}
		else if(PathArray[i].Find("BaseData\\Config\\装备升级配置表\\") != -1)
		{
			EXPORT_DATA(UPGRADE_EQUIP_CONFIG);
		}
		else if(PathArray[i].Find("BaseData\\Config\\封神人物属性表\\") != -1)	
		{
			EXPORT_DATA(DEITY_CHARACTER_CONFIG);
		}
		else if( PathArray[i].Find("BaseData\\Config\\宝石品级品质配置表\\") != -1 )//Added 2011-03-15.
		{
			EXPORT_DATA(GEM_CONFIG);	
		}
		else if( PathArray[i].Find("BaseData\\Config\\宝石萃取产物配置表\\") != -1 )//Added 2011-03-15.
		{
			EXPORT_DATA(GEM_EXTRACT_CONFIG);	
		}
		else if( PathArray[i].Find("BaseData\\Config\\副本配置表\\") != -1 )//Added 2011-07-13.
		{
			EXPORT_DATA(TRANSCRIPTION_CONFIG);
		}
		else if( PathArray[i].Find("BaseData\\Config\\在线奖励大礼包配置表\\") != -1 )//Added 2011-12-06.
		{
			EXPORT_DATA(ONLINE_GIFT_BAG_CONFIG);
		}
		else if( PathArray[i].Find("BaseData\\Config\\诛仙古卷区域配置表\\") != -1 ) //Added 2012-03-20.
		{
			EXPORT_DATA(SCROLL_REGION_CONFIG);
		}
		else if( PathArray[i].Find("BaseData\\Config\\趴塔副本配置表\\") != -1 ) //Added 2012-03-31.
		{
			EXPORT_DATA(TOWER_TRANSCRIPTION_CONFIG);
		}
		else if( PathArray[i].Find("BaseData\\Config\\趴塔副本每层属性配置表\\") != -1 ) //Added 2012-03-31.
		{
			EXPORT_DATA(TOWER_TRANSCRIPTION_PROPERTY_CONFIG);
		}
		else if( PathArray[i].Find("BaseData\\Config\\元魂系统参数配置\\") != -1 ) //Added 2012.4.17
		{
			EXPORT_DATA(RUNE_DATA_CONFIG);
		}
		else if( PathArray[i].Find("BaseData\\Config\\符文之语配置表\\") != -1 ) //Added 2012.4.17
		{
			EXPORT_DATA(RUNE_COMB_PROPERTY);
		}
		else if( PathArray[i].Find("BaseData\\Config\\诛小仙升级配置表\\") != -1 ) //Added 2012-04-19.
		{
			EXPORT_DATA(LITTLE_PET_UPGRADE_CONFIG);
		}
		else if( PathArray[i].Find("BaseData\\Config\\全服限量产出配置表\\") != -1 )
		{
			EXPORT_DATA(DROP_INTERNAL_CONFIG);
		}
		else if( PathArray[i].Find("BaseData\\Config\\跨服PK竞猜配置表\\") != -1 )
		{
			EXPORT_DATA(PK2012_GUESS_CONFIG);
		}
		else if( PathArray[i].Find("BaseData\\Config\\碰撞副本变身配置表\\") != -1 ) //Added 2012-08-02.
		{
			EXPORT_DATA(COLLISION_RAID_TRANSFIGURE_CONFIG);
		}
		else if( PathArray[i].Find("BaseData\\Config\\上古世界传送服务配置\\") != -1 ) //Added 2012-08-15.
		{
			EXPORT_DATA(UI_TRANSFER_CONFIG);
		}
		else if( PathArray[i].Find("BaseData\\Config\\碰撞副本奖励配置表\\") != -1 ) //Added 2012-08-15.
		{
			EXPORT_DATA(COLLISION_RAID_AWARD_CONFIG);
		}
		else if( PathArray[i].Find("BaseData\\Config\\buff区域\\") != -1 ) //Added 2012-08-15.
		{
			EXPORT_DATA(BUFF_AREA_CONFIG);
		}
		else if( PathArray[i].Find("BaseData\\Config\\活跃度配置\\") != -1 ) //Added 2012-08-15.
		{
			EXPORT_DATA(LIVENESS_CONFIG);
		}
		else if( PathArray[i].Find("BaseData\\Config\\上古新世界副本配置\\") != -1 ) //Added 2012-08-15.
		{
			EXPORT_DATA(CHALLENGE_2012_CONFIG);
		}
		else if( PathArray[i].Find("BaseData\\Config\\特殊任务奖励配置\\") != -1 ) //Added 2012-08-15.
		{
			EXPORT_DATA(TASK_SPECIAL_AWARD_CONFIG);
		}
		else if( PathArray[i].Find("BaseData\\Config\\基础属性增益配置\\") != -1 ) //Added 2012-08-15.
		{
			EXPORT_DATA(PROP_ADD_CONFIG);
		}
		else if( PathArray[i].Find("BaseData\\Config\\国战配置表\\") != -1 ) //Added 2012-08-15.
		{
			EXPORT_DATA(KING_WAR_CONFIG);
		}
		else if( PathArray[i].Find("BaseData\\Config\\金身法身换金币造化配置表\\") != -1 ) //Added 2012-08-15.
		{
			EXPORT_DATA(JINFASHEN_TO_MONEY_CONFIG);
		}
		else if( PathArray[i].Find("BaseData\\Config\\流水席副本配置\\") != -1 )
		{
			EXPORT_DATA(BATTLE_201304_CONFIG);
		}
		else if( PathArray[i].Find("BaseData\\Config\\符文2013配置表\\") != -1 )
		{
			EXPORT_DATA(RUNE2013_CONFIG);
		}
		else if( PathArray[i].Find("BaseData\\Config\\泡澡奖励配置表\\") != -1 )
		{
			EXPORT_DATA(BASHE_AWARD_CONFIG);
		}
		else if(PathArray[i].Find("BaseData\\Config\\竞技场赛季时间配置表\\") != -1)
		{
			EXPORT_DATA(ARENA_SEASON_TIME_CONFIG);
		}
		else if(PathArray[i].Find("BaseData\\Config\\每周个人分档奖励配置表\\") != -1)
		{
			EXPORT_DATA(PERSONAL_LEVEL_AWARD_CONFIG);
		}
		else if(PathArray[i].Find("BaseData\\Config\\战队赛季奖励配置表\\") != -1)
		{
			EXPORT_DATA(TEAM_SEASON_AWARD_CONFIG);
		}
		else if(PathArray[i].Find("BaseData\\Config\\每周兑换币奖励配置表\\") != -1)
		{
			EXPORT_DATA(WEEK_CURRNCY_AWARD_CONFIG);
		}
		else if(PathArray[i].Find("BaseData\\Config\\互动物体挂点配置表\\") != -1)
		{
			EXPORT_DATA(INTERACTION_OBJECT_HOOK_CONFIG);
		}
		else if(PathArray[i].Find("BaseData\\Config\\染色剂配置表\\") != -1)
		{
			EXPORT_DATA(COLORANT_CONFIG);
		}
		else if(PathArray[i].Find("BaseData\\Config\\帮派传送配置表\\") != -1)
		{
			EXPORT_DATA(FACTION_TRANSFER_CONFIG);
		}
		else if(PathArray[i].Find("BaseData\\Config\\可建筑区域配置表\\") != -1)
		{
			EXPORT_DATA(BUILDING_REGION_CONFIG);
		}
		else if(PathArray[i].Find("BaseData\\Config\\相位配置表\\") != -1)
		{
			EXPORT_DATA(PHASE_CONFIG);
		}
		else if(PathArray[i].Find("BaseData\\Config\\跨服6v6物品兑换表\\") != -1)
		{
			EXPORT_DATA(CROSS6V6_ITEM_EXCHANGE_CONFIG);
		}
		else if(PathArray[i].Find("BaseData\\Config\\副本攻略配置表\\") != -1)
		{
			EXPORT_DATA(TRANSCRIPT_STRATEGY_CONFIG);
		}
		else if(PathArray[i].Find("BaseData\\Config\\帮派商城配置表\\") != -1)
		{
			EXPORT_DATA(FACTION_SHOP_CONFIG);
		}
		else if(PathArray[i].Find("BaseData\\Config\\帮派竞拍品配置表\\") != -1)
		{
			EXPORT_DATA(FACTION_AUCTION_CONFIG);
		}
		else if(PathArray[i].Find("BaseData\\Config\\帮派元宝商城配置表\\") != -1)
		{
			EXPORT_DATA(FACTION_MONEY_SHOP_CONFIG);
		}
		else if(PathArray[i].Find("BaseData\\Config\\染色剂初始配置表\\") != -1)
		{
			EXPORT_DATA(COLORANT_DEFAULT_CONFIG);
		}
		else
		{
#ifdef _DEBUG
			char buf[256];
			sprintf(buf, "Export: Unknown Templ %s\n", PathArray[i]);
			OutputDebugString(buf);
#endif
		}

		tmpl.Release();
	}

	return (DataMan.save_data(szPath) == 0);
}

bool ExportPolicyData( const char* szPath)
{
	elementdataman DataMan;
	abase::vector<AString> PathArray;
	CString error_msg;
	g_PolicyIDMan.Release();
	if (g_PolicyIDMan.Load("PolicyData\\PolicyID.dat") != 0)
	{
		AfxMessageBox("ExportPolicyData(), 打开TPolicyData\\PolicyID.dat文件失败", MB_ICONSTOP);
		return false;
	}
	g_PolicyIDMan.GeneratePathArray(PathArray);
	
	FILE *pFile = fopen(szPath,"wb");
	if(pFile==NULL) return false;
	int num = 0;
	unsigned int uVersion = F_POLICY_EXP_VERSION;
	fwrite(&uVersion,sizeof(unsigned int),1,pFile);
	fwrite(&num,sizeof(int),1,pFile);
	for (size_t i = 0; i < PathArray.size(); i++)
	{
		CPolicyData dat;
		if(!dat.Load(PathArray[i].GetBuffer(0)))
		{
			error_msg.Format("警告: 不能打开 %s 策略,输出忽略该策略!",PathArray[i]);
			AfxMessageBox(error_msg);
			continue;
		}
		if(!dat.Save(pFile))
		{
			AfxMessageBox("保存数据失败!");
			goto faild;
		}
		++num;
	}
	if(0==fseek(pFile,sizeof(unsigned int),SEEK_SET))
		fwrite(&num,sizeof(int),1,pFile);
	else {
		AfxMessageBox("保存数据失败!");
		goto faild;
	}
	fclose(pFile);
	g_PolicyIDMan.Release();
	PathArray.clear();
	return true;
faild:
	fclose(pFile);
	g_PolicyIDMan.Release();
	PathArray.clear();
	return false;
}

//Use only for programer
void TempTransVersion()
{
#ifdef _MD5_CHECK
	abase::vector<AString> PathArray;
	g_BaseIDMan.GeneratePathArray(PathArray);

	size_t i;
	for (i = 0; i < PathArray.size(); i++)
	{
		BaseDataTempl tmpl;
		if (tmpl.Load(PathArray[i]) != 0)
		{
			g_Log.Log("不能加载 %s", PathArray[i]);
			continue;
		}
		if(!tmpl.Save(PathArray[i]))
		{
			g_Log.Log("不能保存 %s", PathArray[i]);
			continue;
		}

		if(!WriteMd5CodeToTemplate((LPCSTR)PathArray[i]))
			g_Log.Log("WriteMd5CodeToTemplate() %s" ,PathArray[i]);
	}

	PathArray.clear();
	g_ExtBaseIDMan.GeneratePathArray(PathArray);

	for (i = 0; i < PathArray.size(); i++)
	{
		ExtendDataTempl tmpl;
		if (tmpl.Load(PathArray[i], false) != 0)
		{
			g_Log.Log("不能加载 %s", PathArray[i]);
			continue;
		}
		if(!tmpl.Save(PathArray[i]))
		{
			g_Log.Log("不能保存 %s", PathArray[i]);
			continue;
		}
		if(!WriteMd5CodeToTemplate((LPCSTR)PathArray[i]))
			g_Log.Log("WriteMd5CodeToTemplate() %s" ,PathArray[i]);
	}

	PathArray.clear();
	g_RecipeIDMan.GeneratePathArray(PathArray);

	for (i = 0; i < PathArray.size(); i++)
	{
		BaseDataTempl tmpl;
		if (tmpl.Load(PathArray[i]) != 0)
		{
			g_Log.Log("不能加载 %s", PathArray[i]);
			continue;
		}
		if(!tmpl.Save(PathArray[i]))
		{
			g_Log.Log("不能保存 %s", PathArray[i]);
			continue;
		}
		if(!WriteMd5CodeToTemplate((LPCSTR)PathArray[i]))
			g_Log.Log("WriteMd5CodeToTemplate() %s" ,PathArray[i]);
	}

	PathArray.clear();
	g_ConfigIDMan.GeneratePathArray(PathArray);

	for (i = 0; i < PathArray.size(); i++)
	{
		BaseDataTempl tmpl;
		if (tmpl.Load(PathArray[i]) != 0)
		{
			g_Log.Log("不能加载 %s", PathArray[i]);
			continue;
		}
		if(!tmpl.Save(PathArray[i]))
		{
			g_Log.Log("不能保存 %s", PathArray[i]);
			continue;
		}
		if(!WriteMd5CodeToTemplate((LPCSTR)PathArray[i]))
			g_Log.Log("WriteMd5CodeToTemplate() %s" ,PathArray[i]);
	}
#endif
}

