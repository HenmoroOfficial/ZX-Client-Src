#include "EC_RoleDescXMLParser.h"
#include "EC_Global.h"
#include "EC_Game_HintTool.h"
#include "EC_IvtrItem.h"
#include "EC_Skill.h"
#include "EC_IvtrConsume.h"		//Added 2012-03-19.
#include "EC_FixedMsg.h"		//Added 2012-03-19.
#include "elementdataman.h"		//Added 2012-03-12.

#include "GetItemDesc.hpp"
#include "ToolAnnounceAid.hpp"
#include "GetItemDescResponse.hpp"
#include "NewKeepAlive.hpp"
#include "GetItemDescForPlatform.hpp" //Added 2011-11-09.
#include "GetItemDescForPlatformRe.hpp"//Added 2011-11-09.
#include "GetRoleEquip.hpp" //Added 2011-11-23.
#include "GetSNSRolePet.hpp" //Added 2011-11-23.
#include "GetRoleEquipRe.hpp" //Added 2011-11-23.
#include "GetSNSRolePetRe.hpp" //Added 2011-11-23.
#include "GetRoleDesc.hpp"	//Added 2012-02-09.
#include "GetRoleDescSnS.hpp"
#include "GetRoleDescSnSRe.hpp"
#include "AXMLFile.h"	//Added 2012-02-09.
#include "ConfigFromLua.h"	//Added 2012-03-06.
#include "AUI\\AUICTranslate.h" //Added 2012-03-06.
#include "GetRoleDescRe.hpp"	//Added 2012-03-09.
#include "gconsignrole"
#include "gsnsrole"

//=============================================================================


const int CECRoleDescXMLParser::ms_iProfSkillLvCount	= 6;	// 职业重数
const int CECRoleDescXMLParser::ms_iProfTalentLvCount	= 3;	// 天书类型
const int CECRoleDescXMLParser::ms_iProfSkillCountPerLv	= 30;	// 职业技能每一重、天书技能每一类最多数量

const int CECRoleDescXMLParser::BASIC_SKILL_COUNT = 11;
const int CECRoleDescXMLParser::GENERAL_SKILL_COUNT = 8;
const int CECRoleDescXMLParser::SECT_SKILL_COUNT = 5;
const int CECRoleDescXMLParser::PLAYER_CLS_COUNT = 14;	//!!!!!!新加职业，需要扩展!!!!!!
const int CECRoleDescXMLParser::NATURE_TYPE_COUNT = 3;
const int CECRoleDescXMLParser::NATURE_TYPE_SECT = 2;

CECRoleDescXMLParser::CECRoleDescXMLParser()
{
	m_pRootItem	= NULL;
}

CECRoleDescXMLParser::~CECRoleDescXMLParser()
{
	if ( m_pRootItem )
	{
		ReleaseXMLItems( m_pRootItem );
		m_pRootItem = NULL;
	}
}

const ACHAR* CECRoleDescXMLParser::GetXMLText( GNET::GConsignRole* pDetailInfo )
{
	//XML 根节点: <role version="1.0">
	m_pRootItem = new AXMLItem;
	if ( !m_pRootItem )
	{
		a_LogOutput( 1, "Create Root Item Error,Allocate memory failed!\n" );
		return NULL;
	}
	m_pRootItem->SetName( _AL("role") );
	m_pRootItem->SetValue( _AL("version"), _AL("1.0") );
	
	//创建角色属性子节点并插入根节点下
	AXMLItem* pRolePropertyItem = BuildRolePropertyXML( pDetailInfo );
	if ( !pRolePropertyItem )
	{
		return NULL;
	}
	pRolePropertyItem->InsertItem( m_pRootItem );

	//创建角色技能属性子节点并插入到角色属性子节点后面
	AXMLItem* pRoleSkillsItem = BuildRoleSkillsXML( pDetailInfo );
	if ( !pRoleSkillsItem )
	{
		return NULL;
	}
	pRoleSkillsItem->InsertItem( m_pRootItem, pRolePropertyItem );

	//创建角色包裹属性子节点并将其插入到角色技能属性子节点的后面
	AXMLItem* pRoleInventoryItem = BuildRoleInventoryXML( pDetailInfo );
	if ( !pRoleInventoryItem )
	{
		return NULL;
	}
	pRoleInventoryItem->InsertItem( m_pRootItem, pRoleSkillsItem );

	//创建角色宠物属性子节点并将其插入到角色包裹属性子节点的后面
	AXMLItem* pRolePetsItem = BuildRolePetsXML( pDetailInfo );
	if ( !pRolePetsItem )
	{
		return NULL;
	}
	pRolePetsItem->InsertItem( m_pRootItem, pRoleInventoryItem );

	//获得整个以根节点开始的XML的文字描述并返回
	m_strText = BuildItemText( _AL(""), m_pRootItem );
	
	return m_strText;
}

const ACHAR* CECRoleDescXMLParser::GetXMLText( GNET::GSNSRole* pDetailInfo )
{
	//XML 根节点: <role version="1.0">
	m_pRootItem = new AXMLItem;
	if ( !m_pRootItem )
	{
		a_LogOutput( 1, "Create Root Item Error,Allocate memory failed!\n" );
		return NULL;
	}
	m_pRootItem->SetName( _AL("role") );
	m_pRootItem->SetValue( _AL("version"), _AL("1.0") );
	
	//创建角色属性子节点并插入根节点下
	AXMLItem* pRolePropertyItem = BuildRolePropertyXML( pDetailInfo );
	if ( !pRolePropertyItem )
	{
		return NULL;
	}
	pRolePropertyItem->InsertItem( m_pRootItem );

	//创建角色技能属性子节点并插入到角色属性子节点后面
	AXMLItem* pRoleSkillsItem = BuildRoleSkillsXML( pDetailInfo );
	if ( !pRoleSkillsItem )
	{
		return NULL;
	}
	pRoleSkillsItem->InsertItem( m_pRootItem, pRolePropertyItem );

	//创建角色包裹属性子节点并将其插入到角色技能属性子节点的后面
	AXMLItem* pRoleInventoryItem = BuildRoleInventoryXML( pDetailInfo );
	if ( !pRoleInventoryItem )
	{
		return NULL;
	}
	pRoleInventoryItem->InsertItem( m_pRootItem, pRoleSkillsItem );

	//创建角色宠物属性子节点并将其插入到角色包裹属性子节点的后面
	AXMLItem* pRolePetsItem = BuildRolePetsXML( pDetailInfo );
	if ( !pRolePetsItem )
	{
		return NULL;
	}
	pRolePetsItem->InsertItem( m_pRootItem, pRoleInventoryItem );

	//获得整个以根节点开始的XML的文字描述并返回
	m_strText = BuildItemText( _AL(""), m_pRootItem );
	
	return m_strText;
}
ACString CECRoleDescXMLParser::GetCultivationName( int nCultivation )
{
	const int MAX_CULTIVATION = 8;
	ACString strCultivationName[MAX_CULTIVATION] =	{
														_AL(""),
														_AL("仙"),
														_AL("魔"),
														_AL("仙魔"),
														_AL("佛"),
														_AL("仙佛"),
														_AL("魔佛"),
														_AL("仙魔佛"),
													};
	if ( nCultivation < 0 || nCultivation >= MAX_CULTIVATION )
	{
		return _AL("非法阵营");
	}
	return strCultivationName[nCultivation];
}

ACString CECRoleDescXMLParser::GetItemText( AXMLItem* pItem )
{
	ACString strTmp;
	strTmp.Format( _AL("</%s>\r\n"), pItem->GetName() );
	ACString strText = pItem->GetItemText() + strTmp;
	return strText;
}

ACString CECRoleDescXMLParser::BuildItemText( ACString strTab, AXMLItem *pItem )
{
	ACString strItem = _AL("");
	AXMLItem *pChildItem = pItem->GetFirstChildItem();
	while(pChildItem)
	{
		strItem += BuildItemText(strTab + _AL("\t"), pChildItem);
		pChildItem = pChildItem->GetNextItem();
	}

	if( strItem.GetLength() == 0 && pItem->GetKeyNumber() == 0 )
	{
		return _AL("");
	}

	//
	//strItem = strTab + pItem->GetItemText() /*+ _AL("\r\n")*/ + strItem; //GetItemText(pItem)		//Cancel \r\n 
	strItem = strTab + pItem->GetItemText() + _AL("\r\n") + strItem; //GetItemText(pItem)		//Cancel \r\n 
	ACString strText;
	strText.Format(_AL("</%s>"), pItem->GetName());		//Cancel \r\n _AL("</%s>\r\n")
	strText = strTab + strText;
	strItem += strText;
	
	return strItem;	
}

void CECRoleDescXMLParser::ReleaseXMLItems( AXMLItem* pItem )	//对ReleaseItem还包一层，这么搞的原因是，AXMLItem的RemoveItem接口写得太垃圾了
{
	if ( !pItem )
	{
		return;
	}

	AXMLItem *pChildItem = pItem->GetFirstChildItem();
	while( pChildItem )
	{
		AXMLItem *pNextItem = pChildItem->GetNextItem();
		ReleaseItem(pChildItem);
		pChildItem = pNextItem;
	}

	//最后不用调用RemoveItem，而是直接delete掉
	delete pItem;
}

void CECRoleDescXMLParser::ReleaseItem( AXMLItem* pItem )	//删除并释放pItem以及其所有子节点
{
	if ( !pItem )
	{
		return;
	}

	AXMLItem *pChildItem = pItem->GetFirstChildItem();
	while ( pChildItem )
	{
		AXMLItem *pNextItem = pChildItem->GetNextItem();
		ReleaseItem(pChildItem);
		pChildItem = pNextItem;
	}
	pItem->RemoveItem();
	delete pItem;
}

AXMLItem* CECRoleDescXMLParser::BuildRolePropertyXML( GNET::GConsignRole* pDetailInfo )
{
	if ( !pDetailInfo )
	{
		return NULL;
	}

	//XML 第一层子节点: <role_property name="属性">
	ACString listValues[3] = { _AL("role_property"), _AL("name"), _AL("属性") };
	AXMLItem* pRolePropertyItem = CreateXMLItem( NULL, NULL, NULL, listValues, 3 );
	if ( !pRolePropertyItem )
	{
		return NULL;
	}

	//XML 第二字节点: <basic name="基础属性">
	ACString basicValues[3] = { _AL("basic"), _AL("name"), _AL("基础属性") };
	AXMLItem* pRoleBasicPropertyItem = CreateXMLItem( pRolePropertyItem, pRolePropertyItem, NULL, basicValues, 3 );
	if ( !pRoleBasicPropertyItem )
	{
		return NULL;
	}

	//XML 第三层子节点: <resists name="抗性">
	ACString resistValues[3] = { _AL("resists"), _AL("name"), _AL("抗性") };
	AXMLItem* pResistsItem = CreateXMLItem( pRolePropertyItem, pRoleBasicPropertyItem, NULL, resistValues, 3 );
	if ( !pResistsItem )
	{
		return NULL;
	}

	//抗性子节点数据项:眩晕抗性 0
	ACString resist1Values[5] = { _AL("resist"), _AL("name"), _AL("眩晕抗性"), _AL("value"), _AL("0") };
	resist1Values[4].Format( _AL("%d"), pDetailInfo->gsroleinfo.rolebasicinfo.resistance1 );
	AXMLItem* pResist1 = CreateXMLItem( pRolePropertyItem, pResistsItem, NULL, resist1Values, 5 );
	if ( !pResist1 )
	{
		return NULL;
	}

	//抗性子节点数据项:昏睡抗性 4
	ACString resist2Values[5] = { _AL("resist"), _AL("name"), _AL("昏睡抗性"), _AL("value"), _AL("0") };
	resist2Values[4].Format( _AL("%d"), pDetailInfo->gsroleinfo.rolebasicinfo.resistance5 );
	AXMLItem* pResist2 = CreateXMLItem( pRolePropertyItem, pResistsItem, pResist1, resist2Values, 5 );
	if ( !pResist2 )
	{
		return NULL;
	}

	//抗性子节点数据项:虚弱抗性 1
	ACString resist3Values[5] = { _AL("resist"), _AL("name"), _AL("虚弱抗性"), _AL("value"), _AL("0") };
	resist3Values[4].Format( _AL("%d"), pDetailInfo->gsroleinfo.rolebasicinfo.resistance2 );
	AXMLItem* pResist3 = CreateXMLItem( pRolePropertyItem, pResistsItem, pResist2, resist3Values, 5 );
	if ( !pResist3 )
	{
		return NULL;
	}

	//抗性子节点数据项：魅惑抗性 3
	ACString resist4Values[5] = { _AL("resist"), _AL("name"), _AL("魅惑抗性"), _AL("value"), _AL("0") };
	resist4Values[4].Format( _AL("%d"), pDetailInfo->gsroleinfo.rolebasicinfo.resistance4 );
	AXMLItem* pResist4 = CreateXMLItem( pRolePropertyItem, pResistsItem, pResist3, resist4Values, 5 );
	if ( !pResist4 )
	{
		return NULL;
	}

	//抗性子节点数据项：定身抗性 2
	ACString resist5Values[5] = { _AL("resist"), _AL("name"), _AL("定身抗性"), _AL("value"), _AL("0") };
	resist5Values[4].Format( _AL("%d"), pDetailInfo->gsroleinfo.rolebasicinfo.resistance3 );
	AXMLItem* pResist5 = CreateXMLItem( pRolePropertyItem, pResistsItem, pResist4, resist5Values, 5 );
	if ( !pResist5 )
	{
		return NULL;
	}

	//特殊信息：第三层结点
	ACString specialValues[3] = { _AL("special_info"), _AL("name"), _AL("特殊信息") };
	AXMLItem* pSpecialInfoItem = CreateXMLItem( pRolePropertyItem, pRoleBasicPropertyItem, pResistsItem, specialValues, 3 );
	if ( !pSpecialInfoItem )
	{
		return NULL;
	}

	//玩家头像信息,第四层子结点
	ACString playerHeadIconValues[3] = { _AL("player_head_icon"), _AL("value"), _AL("head/0.png") };	//!!!!!!头像文件路径？!!!!!!
	playerHeadIconValues[2].Format( _AL("head/%d.png"), pDetailInfo->gsroleinfo.rolebasicinfo.cls );
	AXMLItem* pPlayerHeadIconItem = CreateXMLItem( pRolePropertyItem, pSpecialInfoItem, NULL, playerHeadIconValues, 3 );
	if ( !pPlayerHeadIconItem )
	{
		return NULL;
	}

	//玩家等级信息子节点
	ACString playerLevelValues[5] = { _AL("level"), _AL("name"), _AL("玩家等级"), _AL("value"), _AL("0") };
	playerLevelValues[4].Format( _AL("%d"), pDetailInfo->gsroleinfo.rolebasicinfo.level );
	AXMLItem* pPlayerLevelItem = CreateXMLItem( pRolePropertyItem, pSpecialInfoItem, pPlayerHeadIconItem, playerLevelValues, 5 );
	if ( !pPlayerLevelItem )
	{
		return NULL;
	}

	//门派信息子节点
	ACString playerClsValues[5] = { _AL("cls"), _AL("name"), _AL("门派"), _AL("value"), _AL("") };
	playerClsValues[4] =  GetClsName(pDetailInfo->gsroleinfo.rolebasicinfo.cls);
	AXMLItem* pPlayerClsItem = CreateXMLItem( pRolePropertyItem, pSpecialInfoItem, pPlayerLevelItem, playerClsValues, 5 );
	if ( !pPlayerClsItem )
	{
		return NULL;
	}
	
	//飞升前职业信息节点
	ACString profBeforeRenascenceValues[5] = { _AL("prof_before_renascence"), _AL("name"), _AL("飞升前职业"), _AL("value"), _AL("") };
	ACString strLevelBeforeReborn;
	strLevelBeforeReborn.Format( _AL(" %d"), pDetailInfo->gsroleinfo.rolebasicinfo.level_before_reborn );
	profBeforeRenascenceValues[4] = GetClsName( pDetailInfo->gsroleinfo.rolebasicinfo.cls_before_reborn ) + strLevelBeforeReborn;
	AXMLItem* pProfBeforeRenascenceItem = CreateXMLItem( pRolePropertyItem, pSpecialInfoItem, pPlayerClsItem, profBeforeRenascenceValues, 5 );
	if ( !pProfBeforeRenascenceItem )
	{
		return NULL;
	}

	//血炼值
	ACString combinedValueValues[5] = { _AL("combine_value"), _AL("name"), _AL("血炼值"), _AL("value"), _AL("") };
	combinedValueValues[4].Format( _AL("%d"), pDetailInfo->gsroleinfo.rolebasicinfo.talisman_value );
	AXMLItem* pCombinedValueItem = CreateXMLItem( pRolePropertyItem, pSpecialInfoItem, pProfBeforeRenascenceItem, combinedValueValues, 5 );
	if ( !pCombinedValueItem )
	{
		return NULL;
	}

	//狭义度子结点
	ACString profManaValues[9] = { _AL("prof_mana"), _AL("name"), _AL("侠义度"), _AL("value"), _AL(""), _AL("progress"), _AL(""), _AL("desc"), _AL("") };
	GetProfMana( pDetailInfo->gsroleinfo.rolebasicinfo.reputation, profManaValues[4], profManaValues[6], profManaValues[8] );
	AXMLItem* pProfManaItem = CreateXMLItem( pRolePropertyItem, pSpecialInfoItem, pCombinedValueItem, profManaValues, 9 );
	if ( !pProfManaItem )
	{
		return NULL;
	}

	//善恶值子节点
	ACString pkValueValues[5] = { _AL("pk_value"), _AL("name"), _AL("善恶值"), _AL("value"), _AL("") };
	pkValueValues[4].Format( _AL("%d"), pDetailInfo->gsroleinfo.rolebasicinfo.pk_value );
	AXMLItem* pPKValueItem = CreateXMLItem( pRolePropertyItem, pSpecialInfoItem, pProfManaItem, pkValueValues, 5 );
	if ( !pPKValueItem )
	{
		return NULL;
	}

	//性别子节点	---------New Added 
	ACString genderValues[5] = { _AL("gender"), _AL("name"), _AL("性别"), _AL("value"), _AL("") };
	genderValues[4] = ((pDetailInfo->gsroleinfo.rolebasicinfo.gender == 0) ? _AL("男") : _AL("女"));
	AXMLItem* pGenderValueItem = CreateXMLItem( pRolePropertyItem, pSpecialInfoItem, pPKValueItem, genderValues, 5 );
	if ( !pGenderValueItem )
	{
		return NULL;
	}

	//基本信息 第三层子节点
	ACString basicInfoValues[3] = { _AL("basic_value"), _AL("name"), _AL("基本信息") };
	AXMLItem* pBasicInfoItem = CreateXMLItem( pRolePropertyItem, pRoleBasicPropertyItem, pSpecialInfoItem, basicInfoValues, 3 );
	if ( !pBasicInfoItem )
	{
		return NULL;
	}

	//修为信息 第四层子节点
	ACString expValues[5] = { _AL("exp"), _AL("name"), _AL("修为"), _AL("value"), _AL("") };
	double fMaxExp = GetLevelUpExp( pDetailInfo->gsroleinfo.rolebasicinfo.level + 1, pDetailInfo->gsroleinfo.rolebasicinfo.reborn_count );
	double fExpRatio = 0.0;
	if ( fMaxExp > 0.00 )
	{
		fExpRatio = (double)pDetailInfo->gsroleinfo.rolebasicinfo.exp / fMaxExp;
		if ( fExpRatio > 0.9999 )
		{
			fExpRatio = 0.9999;
		}
	}
	expValues[4].Format( _AL("%.02f%%"), fExpRatio*100.0 );
	AXMLItem* pExpItem = CreateXMLItem( pRolePropertyItem, pBasicInfoItem, NULL, expValues, 5 );
	if ( !pExpItem )
	{
		return NULL;
	}

	//气血信息
	ACString hpValues[5] = { _AL("hp"), _AL("name"), _AL("气血"), _AL("value"), _AL("") };
	hpValues[4].Format( _AL("%d/%d"), pDetailInfo->gsroleinfo.rolebasicinfo.max_hp, pDetailInfo->gsroleinfo.rolebasicinfo.max_hp );
	AXMLItem* pHpItem = CreateXMLItem( pRolePropertyItem, pBasicInfoItem, pExpItem, hpValues, 5 );
	if ( !pHpItem )
	{
		return NULL;
	}

	//真气信息
	ACString mpValues[5] = { _AL("mp"), _AL("name"), _AL("真气"), _AL("value"), _AL("") };
	mpValues[4].Format( _AL("%d/%d"), pDetailInfo->gsroleinfo.rolebasicinfo.max_mp, pDetailInfo->gsroleinfo.rolebasicinfo.max_mp );
	AXMLItem* pMpItem = CreateXMLItem( pRolePropertyItem, pBasicInfoItem, pHpItem, mpValues, 5 );
	if ( !pMpItem )
	{
		return NULL;
	}

	//攻击信息
	ACString attackValues[5] = { _AL("attack"), _AL("name"), _AL("攻击"), _AL("value"), _AL("") };
	attackValues[4].Format( _AL("%d-%d"), pDetailInfo->gsroleinfo.rolebasicinfo.damage_low, pDetailInfo->gsroleinfo.rolebasicinfo.damage_high );
	AXMLItem* pAttackItem = CreateXMLItem( pRolePropertyItem, pBasicInfoItem, pMpItem, attackValues, 5 );
	if ( !pAttackItem )
	{
		return NULL;
	}

	//防御信息
	ACString defenceValues[5] = { _AL("defence"), _AL("name"), _AL("防御"), _AL("value"), _AL("") };
	defenceValues[4].Format( _AL("%d"), pDetailInfo->gsroleinfo.rolebasicinfo.defense );
	AXMLItem* pDefenceItem = CreateXMLItem( pRolePropertyItem, pBasicInfoItem, pAttackItem, defenceValues, 5 );
	if ( !pDefenceItem )
	{
		return NULL;
	}

	//普攻命中信息
	ACString definitionValues[5] = { _AL("definition"), _AL("name"), _AL("普攻命中"), _AL("value"), _AL("") };
	definitionValues[4].Format( _AL("%d"), pDetailInfo->gsroleinfo.rolebasicinfo.attack );
	AXMLItem* pDefinitionItem = CreateXMLItem( pRolePropertyItem, pBasicInfoItem, pDefenceItem, definitionValues, 5 );
	if ( !pDefinitionItem )
	{
		return NULL;
	}

	//普攻躲闪信息
	ACString evadeValues[5] = { _AL("evade"), _AL("name"), _AL("普攻躲闪"), _AL("value"), _AL("") };
	evadeValues[4].Format( _AL("%d"), pDetailInfo->gsroleinfo.rolebasicinfo.armor );
	AXMLItem* pEvadeItem = CreateXMLItem( pRolePropertyItem, pBasicInfoItem, pDefinitionItem, evadeValues, 5 );
	if ( !pEvadeItem )
	{
		return NULL;
	}

	//致命伤害
	ACString criticalDamageValues[5] = { _AL("critical_damage"), _AL("name"), _AL("致命伤害"), _AL("value"), _AL("") };
	criticalDamageValues[4].Format( _AL("%.1f%%"), pDetailInfo->gsroleinfo.rolebasicinfo.crit_damage * 100 + 0.001 );
	AXMLItem* pCriticalDamageItem = CreateXMLItem( pRolePropertyItem, pBasicInfoItem, pEvadeItem, criticalDamageValues, 5 );
	if ( !pCriticalDamageItem )
	{
		return NULL;
	}

	//致命一击率
	ACString criticalRateValues[5] = { _AL("critical_rate"), _AL("name"), _AL("致命一击率"), _AL("value"), _AL("") };
	criticalRateValues[4].Format( _AL("%.1f%%"), pDetailInfo->gsroleinfo.rolebasicinfo.crit_rate / 10.0f );
	AXMLItem* pCriticalRateItem = CreateXMLItem( pRolePropertyItem, pBasicInfoItem, pCriticalDamageItem, criticalRateValues, 5 );
	if ( !pCriticalRateItem )
	{
		return NULL;
	}

	//技能躲闪
	ACString skillEvadeValues[5] = { _AL("skill_evade"), _AL("name"), _AL("技能躲闪"), _AL("value"), _AL("") };
	skillEvadeValues[4].Format( _AL("%.1f"), pDetailInfo->gsroleinfo.rolebasicinfo.skill_armor_rate / 10 );
	AXMLItem* pSkillEvadeItem = CreateXMLItem( pRolePropertyItem, pBasicInfoItem, pCriticalRateItem, skillEvadeValues, 5 );
	if ( !pSkillEvadeItem )
	{
		return NULL;
	}

	//技能命中
	ACString skillDefinitionValues[5] = { _AL("skill_definition"), _AL("name"), _AL("技能命中"), _AL("value"), _AL("") };
	skillDefinitionValues[4].Format( _AL("%.1f"), 100 + pDetailInfo->gsroleinfo.rolebasicinfo.skill_attack_rate / 10.0f );
	AXMLItem* pSkillDefinitionItem = CreateXMLItem( pRolePropertyItem, pBasicInfoItem, pSkillEvadeItem, skillDefinitionValues, 5 );
	if ( !pSkillDefinitionItem )
	{
		return NULL;
	}

	//减免他方致命率
	ACString deCriticalValues[5] = { _AL("de_critical"), _AL("name"), _AL("减免他方致命率"), _AL("value"), _AL("") };
	deCriticalValues[4].Format( _AL("%.1f%%"), pDetailInfo->gsroleinfo.rolebasicinfo.anti_crit / 10.0f );
	AXMLItem* pDeCriticalItem = CreateXMLItem( pRolePropertyItem, pBasicInfoItem, pSkillDefinitionItem, deCriticalValues, 5 );
	if ( !pDeCriticalItem )
	{
		return NULL;
	}

	//减免致命伤害
	ACString deCriticalDamageValues[5] = { _AL("de_critical_damage"), _AL("name"), _AL("减免致命伤害"), _AL("value"), _AL("") };
	deCriticalDamageValues[4].Format( _AL("%.1f%%"), pDetailInfo->gsroleinfo.rolebasicinfo.anti_crit_damage * 100 + 0.001 );
	AXMLItem* pDeCriticalDamageItem = CreateXMLItem( pRolePropertyItem, pBasicInfoItem, pDeCriticalItem, deCriticalDamageValues, 5 );
	if ( !pDeCriticalDamageItem )
	{
		return NULL;
	}

	//御仙
	ACString deDamageTaoismValues[5] = { _AL("de_damage_taoism"), _AL("name"), _AL("御仙"), _AL("value"), _AL("") };
	deDamageTaoismValues[4].Format( _AL("%.1f%%"), pDetailInfo->gsroleinfo.rolebasicinfo.cult_defense_xian / 10.0f );
	AXMLItem* pDeDamageTaoismItem = CreateXMLItem( pRolePropertyItem, pBasicInfoItem, pDeCriticalDamageItem, deDamageTaoismValues, 5 );
	if ( !pDeDamageTaoismItem )
	{
		return NULL;
	}

	//御魔
	ACString deDamageEvilValues[5] = { _AL("de_damage_evil"), _AL("name"), _AL("御魔"), _AL("value"), _AL("") };
	deDamageEvilValues[4].Format( _AL("%.1f%%"), pDetailInfo->gsroleinfo.rolebasicinfo.cult_defense_mo / 10.0f );
	AXMLItem* pDeDamageEvilItem = CreateXMLItem( pRolePropertyItem, pBasicInfoItem, pDeDamageTaoismItem, deDamageEvilValues, 5 );
	if ( !pDeDamageEvilItem )
	{
		return NULL;
	}

	//御佛
	ACString deDamageBuddhismValues[5] = { _AL("de_damage_buddhism"), _AL("name"), _AL("御佛"), _AL("value"), _AL("") };
	deDamageBuddhismValues[4].Format( _AL("%.1f%%"), pDetailInfo->gsroleinfo.rolebasicinfo.cult_defense_fo / 10.0f );
	AXMLItem* pDeDamageBuddhismItem = CreateXMLItem( pRolePropertyItem, pBasicInfoItem, pDeDamageEvilItem, deDamageBuddhismValues, 5 );
	if ( !pDeDamageBuddhismItem )
	{
		return NULL;
	}

	//元神 第二层子节点
	ACString deityInfoValues[3] = { _AL("deity_info"), _AL("name"), _AL("元神") };
	AXMLItem* pDeityInfoItem = CreateXMLItem( pRolePropertyItem, pRolePropertyItem, pRoleBasicPropertyItem, deityInfoValues, 3 );
	if ( !pDeityInfoItem )
	{
		return NULL;
	}

	//元神称号
	ACString deityTitleValues[3] = { _AL("deity_title"), _AL("name"), _AL("未封神") };
	deityTitleValues[2] = GetDeityTitle( pDetailInfo->gsroleinfo.deityinfo.deity_level, pDetailInfo->gsroleinfo.rolebasicinfo.cultivation );
	AXMLItem* pDeityTitleItem = CreateXMLItem( pRolePropertyItem, pDeityInfoItem, NULL, deityTitleValues, 3 );
	if ( !pDeityTitleItem )
	{
		return NULL;
	}

	//克仙
	ACString deTaoismValues[5] = { _AL("de_taoism"), _AL("name"), _AL("克仙"), _AL("value"), _AL("") };
	deTaoismValues[4].Format( _AL("%.1f%%"), pDetailInfo->gsroleinfo.deityinfo.atatck_xian / 10.0f );
	AXMLItem* pDeTaoismItem = CreateXMLItem( pRolePropertyItem, pDeityInfoItem, pDeityTitleItem, deTaoismValues, 5 );
	if ( !pDeTaoismItem )
	{
		return NULL;
	}

	//克佛
	ACString deBuddhismValues[5] = { _AL("de_buddhism"), _AL("name"), _AL("克佛"), _AL("value"), _AL("") };
	deBuddhismValues[4].Format( _AL("%.1f%%"), pDetailInfo->gsroleinfo.deityinfo.atatck_fo / 10.0f );
	AXMLItem* pDeBuddhismItem = CreateXMLItem( pRolePropertyItem, pDeityInfoItem, pDeTaoismItem, deBuddhismValues, 5 );
	if ( !pDeBuddhismItem )
	{
		return NULL;
	}

	//克魔
	ACString deEvilValues[5] = { _AL("de_evil"), _AL("name"), _AL("克魔"), _AL("value"), _AL("") };
	deEvilValues[4].Format( _AL("%.1f%%"), pDetailInfo->gsroleinfo.deityinfo.atatck_mo / 10.0f );
	AXMLItem* pDeEvilItem = CreateXMLItem( pRolePropertyItem, pDeityInfoItem, pDeBuddhismItem, deEvilValues, 5 );
	if ( !pDeEvilItem )
	{
		return NULL;
	}

	//元神等级
	ACString deityLevelValues[5] = { _AL("deity_level"), _AL("name"), _AL("元神等级"), _AL("value"), _AL("") };
	deityLevelValues[4].Format( _AL("%d"), pDetailInfo->gsroleinfo.deityinfo.deity_level );
	AXMLItem* pDeityLevelItem = CreateXMLItem( pRolePropertyItem, pDeityInfoItem, pDeEvilItem, deityLevelValues, 5 );
	if ( !pDeityLevelItem )
	{
		return NULL;
	}

	//元神威能
	ACString deitySpValues[5] = { _AL("deity_sp"), _AL("name"), _AL("元神威能"), _AL("value"), _AL("") };
	deitySpValues[4].Format( _AL("%d"), pDetailInfo->gsroleinfo.deityinfo.deity_power );
	AXMLItem* pDeitySpItem = CreateXMLItem( pRolePropertyItem, pDeityInfoItem, pDeityLevelItem, deitySpValues, 5 );
	if ( !pDeitySpItem )
	{
		return NULL;
	}

	//元力值
	ACString deityNumValues[7] = { _AL("deity_num"), _AL("name"), _AL("元力值"), _AL("value"), _AL(""), _AL("progress"), _AL("0") }; //元力值进度条默认设置为满的=0
	deityNumValues[4].Format( _AL("%d/%d"), pDetailInfo->gsroleinfo.deityinfo.max_dp, pDetailInfo->gsroleinfo.deityinfo.max_dp );
	if ( pDetailInfo->gsroleinfo.deityinfo.max_dp )
	{
		deityNumValues[6] = _AL("100");
	}
	AXMLItem* pDeityNumItem = CreateXMLItem( pRolePropertyItem, pDeityInfoItem, pDeitySpItem, deityNumValues, 7 );
	if ( !pDeityNumItem )
	{
		return NULL;
	}

	//元神修为
	ACString deityExpValues[7] = { _AL("deity_exp"), _AL("name"), _AL("元神修为"), _AL("value"), _AL(""), _AL("progress"), _AL("") };
	double dMaxDpExp = GetDeityLevelUpExp( pDetailInfo->gsroleinfo.deityinfo.deity_level + 1 );
	int nDeityProgress = (int)(((double)(pDetailInfo->gsroleinfo.deityinfo.deity_exp) / dMaxDpExp)*100);
	if ( nDeityProgress > 100 )	//超过100的情况。。。。。
	{
		nDeityProgress = 100;
	}
	deityExpValues[4].Format( _AL("%.0f/%.0f"), (double)(pDetailInfo->gsroleinfo.deityinfo.deity_exp), dMaxDpExp );
	deityExpValues[6].Format( _AL("%d"), nDeityProgress );
	AXMLItem* pDeityExpItem = CreateXMLItem( pRolePropertyItem, pDeityInfoItem, pDeityNumItem, deityExpValues, 7 );
	if ( !pDeityExpItem )
	{
		return NULL;
	}

	//尊号 第二层子节点
	ACString playerTitlesValues[3] = { _AL("player_titles"), _AL("name"), _AL("尊号") };
	AXMLItem* pPlayerTitlesItem = CreateXMLItem( pRolePropertyItem, pRolePropertyItem, pDeityInfoItem, playerTitlesValues, 3 );
	if ( !pPlayerTitlesItem )
	{
		return NULL;
	}

	//加入各类型的尊号
	ACString strDesc, strHint;
	CConfigFromLua * pConfigFromLua = g_pGame->GetConfigFromLua();
	AXMLItem* pTitleCatalogItem = NULL;
	abase::hash_map<int, bool> titlesMap;
	GetPlayerTitles( &pDetailInfo->gsroleinfo.title.titlelist, titlesMap );
	for ( int i = 0; i < pConfigFromLua->m_TitleDir[0].m_ArrVal.size(); ++i )	//LUA 配置表
	{
		if( pConfigFromLua->m_TitleDir[0].m_ArrVal[i].m_Type == CScriptValue::SVT_ARRAY )	//第二层也是数组
		{
			if( pConfigFromLua->m_TitleDir[0].m_ArrVal[i].m_ArrVal[0].m_Type == CScriptValue::SVT_STRING ) //头一个元素是名称
			{
				pConfigFromLua->m_TitleDir[0].m_ArrVal[i].m_ArrVal[0].RetrieveAWString(strDesc);	//获得称号类别
				ACString titleCatalog[3] = { _AL("title_catalog"), _AL("name"), _AL("") };
				titleCatalog[2] = strDesc;
				pTitleCatalogItem = CreateXMLItem( pRolePropertyItem, pPlayerTitlesItem, pTitleCatalogItem, titleCatalog, 3 );
				if ( !pTitleCatalogItem )
				{
					return NULL;
				}

				//处理每个称号类别内的具体称号
				AXMLItem* pRoleTitleItem = NULL;
				for ( int j = 0; j < pConfigFromLua->m_TitleDir[0].m_ArrVal[i].m_ArrVal.size(); ++j )
				{
					if( pConfigFromLua->m_TitleDir[0].m_ArrVal[i].m_ArrVal[j].m_Type == CScriptValue::SVT_NUMBER )
					{
						int idTitle = pConfigFromLua->m_TitleDir[0].m_ArrVal[i].m_ArrVal[j].GetInt();
						if ( titlesMap.find( idTitle ) != titlesMap.end() )	//查找Title ID
						{
							ACString roleTitle[5] = { _AL("role_title"), _AL("name"), _AL(""), _AL("desc"), _AL("") };
							//-------------------------------------------------
							ACString strTmp;
							TranslateStr( GetTitle( idTitle ), strTmp );
							roleTitle[2] = strTmp;	//GetTitle( idTitle ) 转义。。。。
							TranslateStr( GetTitleHint( idTitle ), strTmp );
							roleTitle[4] = strTmp;	//GetTitleHint( idTitle ) 转义。。。
							//-------------------------------------------------
//							roleTitle[2] = GetTitle( idTitle );
//							roleTitle[4] = GetTitleHint( idTitle );
							pRoleTitleItem = CreateXMLItem( pRolePropertyItem, pTitleCatalogItem, pRoleTitleItem, roleTitle, 5 );
							if ( !pRoleTitleItem )
							{
								return NULL;
							}
						}
					}
					else if( pConfigFromLua->m_TitleDir[0].m_ArrVal[i].m_ArrVal[j].m_Type == CScriptValue::SVT_ARRAY )
					{
						if( pConfigFromLua->m_TitleDir[0].m_ArrVal[i].m_ArrVal[j].m_ArrVal[0].m_Type == CScriptValue::SVT_STRING )
						{
							pConfigFromLua->m_TitleDir[0].m_ArrVal[i].m_ArrVal[j].m_ArrVal[0].RetrieveAWString(strHint);
							strDesc = _AL("????????");
							for(int k = 1; k < pConfigFromLua->m_TitleDir[0].m_ArrVal[i].m_ArrVal[j].m_ArrVal.size(); ++k )
							{
								if( pConfigFromLua->m_TitleDir[0].m_ArrVal[i].m_ArrVal[j].m_ArrVal[k].m_Type == CScriptValue::SVT_NUMBER )
								{
									int idTitle = pConfigFromLua->m_TitleDir[0].m_ArrVal[i].m_ArrVal[j].m_ArrVal[k].GetInt();
									if ( titlesMap.find( idTitle ) != titlesMap.end() ) //查找Title ID
									{
										ACString roleTitle[5] = { _AL("role_title"), _AL("name"), _AL(""), _AL("desc"), _AL("") };
										//-------------------------------------------------
										ACString strTmp;
										TranslateStr( GetTitle( idTitle ), strTmp );
										roleTitle[2] = strTmp;	//GetTitle( idTitle ) 转义。。。。
										TranslateStr( GetTitleHint( idTitle ), strTmp );
										roleTitle[4] = strTmp;	//GetTitleHint( idTitle ) 转义。。。
										//-------------------------------------------------
//										roleTitle[2] = GetTitle( idTitle );
//										roleTitle[4] = GetTitleHint( idTitle );
										pRoleTitleItem = CreateXMLItem( pRolePropertyItem, pTitleCatalogItem, pRoleTitleItem, roleTitle, 5 );
										if ( !pRoleTitleItem )
										{
											return NULL;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	//声望 第二层子节点
	ACString strReputations[3] = { _AL("reputations"), _AL("name"), _AL("声望") };
	AXMLItem* pReputationItem = CreateXMLItem( pRolePropertyItem, pRolePropertyItem, pPlayerTitlesItem, strReputations, 3 );
	if ( !pReputationItem )
	{
		return NULL;
	}
	
	//插入各声望的子节点
	const int nReputationMappingID[] = { 1, 2, 3, 4, 0, 19, 14, 13, 15, 16, 17, 18, 22, 5, 6, 7, 8, 12, 9, 10, 11, 20, 21 };	//由于顺序问题，导致当前需要处理的声望id和游戏内UI对应id需要做映射
	const int nReputationTypeSize = 5;
	const int nReputationCount = 23;
	ACString strReputationType[nReputationTypeSize][5] =	{	{ _AL("rep_prof"), _AL("name"), _AL("门派贡献度"), _AL("count"), _AL("12") }, 
																{ _AL("classic"), _AL("name"), _AL("经典"), _AL("count"), _AL("5") },
																{ _AL("special"), _AL("name"), _AL("特殊"), _AL("count"), _AL("1") },
																{ _AL("nature"), _AL("name"), _AL("造化"), _AL("count"), _AL("3") },
																{ _AL("primodial"), _AL("name"), _AL("鸿蒙"), _AL("count"), _AL("2") }
															};
	const int nReputationLoops[nReputationTypeSize] =	{ 12, 5, 1, 3, 2 };
	ACString strReputationValues[nReputationCount][9] =		{
												{ _AL("reputation"), _AL("name"), _AL("青云"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") }, 
												{ _AL("reputation"), _AL("name"), _AL("天音"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("鬼王"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("合欢"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("鬼道"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("焚香"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("烈山"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("九黎"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("辰皇"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("太昊"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("怀光"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("天华"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("仙基"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("忠义"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("情缘"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("文采"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("师德"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("御灵"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("道心"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("魔性"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("佛缘"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("战绩"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("武勋"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
											};

	AXMLItem* pReputationTypeItem = NULL;
	int nCurIndex = 0;
	abase::vector<int> reputationList;
	GetPlayerReputationList( pDetailInfo->creditinfo.credits, reputationList );
	for ( int k = 0; k < nReputationTypeSize; ++k )
	{
		//构建声望类子节点
		pReputationTypeItem = CreateXMLItem( pRolePropertyItem, pReputationItem, pReputationTypeItem, strReputationType[k], 5 );
		if ( !pReputationTypeItem )
		{
			return NULL;
		}
		
		//构建声望类下面的具体声望子节点
		AXMLItem* pReputationValueItem = NULL;
		for ( int j = 0; j < nReputationLoops[k]; ++j )
		{
			//TODO:设置strReputationValues数组中具体属性值
			if ( (nCurIndex != 12) && (reputationList.size() > nReputationMappingID[nCurIndex]) )
			{
				GetReputationStringValues( reputationList[nReputationMappingID[nCurIndex]], 
											nReputationMappingID[nCurIndex], 
											strReputationValues[nCurIndex][4], 
											strReputationValues[nCurIndex][6], 
											strReputationValues[nCurIndex][8]);
													
			}
			if ( 12 == nCurIndex )	//写死 仙基
			{
				GetReputationStringValues(	pDetailInfo->gsroleinfo.rolebasicinfo.battlescore,	//仙基值!!!!
											23, //设定为23!!!!
											strReputationValues[nCurIndex][4], 
											strReputationValues[nCurIndex][6], 
											strReputationValues[nCurIndex][8]);				
			}
			//创建具体声望子节点，并将其插入到合适位置
			if ( nCurIndex < nReputationCount )
			{
				pReputationValueItem = CreateXMLItem( pRolePropertyItem, pReputationTypeItem, pReputationValueItem, strReputationValues[nCurIndex], 9 );
				if ( !pReputationValueItem )
				{
					return NULL;
				}
			}
			else
			{
				a_LogOutput( 1, "Create XML item Error,\"nCurIndex < nReputationCount\"!\n" );
			}

			//
			nCurIndex++;
		}
	}

	//装备属性 第二层子节点
	ACString strEquipments[5] = { _AL("equipments"), _AL("name"), _AL("装备"), _AL("count"), _AL("0") };
	strEquipments[4].Format( _AL("%d"), pDetailInfo->pocketinfo.equipment.size() );
	AXMLItem* pEquipmentsItem = CreateXMLItem( pRolePropertyItem, pRolePropertyItem, pReputationItem, strEquipments, 5 );
	if ( !pEquipmentsItem )
	{
		return NULL;
	}

	//插入各个装备子节点
	AXMLItem* pEquipmentItem = NULL;
	ACString strEquipmentItem[13] = { _AL("item"), _AL("id"), _AL(""), _AL("pos"), _AL(""), _AL("level"), _AL(""), _AL("name"), _AL(""), _AL("icon"), _AL(""), _AL("desc"), _AL("") };
	for ( int nItemIndex = 0; nItemIndex < (int)pDetailInfo->pocketinfo.equipment.size(); ++nItemIndex )
	{
		const GRoleInventory& equipItem = pDetailInfo->pocketinfo.equipment[nItemIndex];
		CECIvtrItem* pItem = CECIvtrItem::CreateItem(equipItem.id, equipItem.expire_date, equipItem.count);
		if ( !pItem )
		{
			a_LogOutput(1, "Failed to create equipment.ID=%d", equipItem.id);
			continue;
		}
		if ( equipItem.data.begin() && equipItem.data.size() )
		{
			pItem->SetItemInfo((BYTE*)equipItem.data.begin(), equipItem.data.size());
		}
		pItem->SetStateFlag(Proctype2State(equipItem.proctype));	//New Added 2013-08-09.
		pItem->UpdateInfoMD5();
		strEquipmentItem[2].Format( _AL("%d"), equipItem.id );
		strEquipmentItem[4].Format( _AL("%d"), equipItem.pos );
		strEquipmentItem[6].Format( _AL("%d"), pItem->GetEstone() );
		//-------------------------------------------------------------------------特殊字符转义
		ACString strItemName;
		TranslateStr( pItem->GetName(), strItemName );
		strEquipmentItem[8] = strItemName;//pItem->GetName()
		//-------------------------------------------------------------------------
		strEquipmentItem[10].Format( _AL("%d.png"), equipItem.id );
		//-------------------------------------------------------------------------特殊字符转义
		ACString strItemDesc;
		TranslateStr( pItem->GetDesc(), strItemDesc );
		strEquipmentItem[12] = strItemDesc;//pItem->GetDesc()
		//-------------------------------------------------------------------------
		pEquipmentItem = CreateXMLItem( pRolePropertyItem, pEquipmentsItem, pEquipmentItem, strEquipmentItem, 13 );
		if ( !pEquipmentItem )
		{
			return NULL;
		}
		
		//!!!!!!注意：清理资源!!!!!!
		delete pItem;
	}

	//最后返回该角色属性根节点
	return pRolePropertyItem;

}

AXMLItem* CECRoleDescXMLParser::BuildRolePropertyXML( GNET::GSNSRole* pDetailInfo )
{
	if ( !pDetailInfo )
	{
		return NULL;
	}

	//XML 第一层子节点: <role_property name="属性">
	ACString listValues[3] = { _AL("role_property"), _AL("name"), _AL("属性") };
	AXMLItem* pRolePropertyItem = CreateXMLItem( NULL, NULL, NULL, listValues, 3 );
	if ( !pRolePropertyItem )
	{
		return NULL;
	}

	//XML 第二字节点: <basic name="基础属性">
	ACString basicValues[3] = { _AL("basic"), _AL("name"), _AL("基础属性") };
	AXMLItem* pRoleBasicPropertyItem = CreateXMLItem( pRolePropertyItem, pRolePropertyItem, NULL, basicValues, 3 );
	if ( !pRoleBasicPropertyItem )
	{
		return NULL;
	}

	//XML 第三层子节点: <resists name="抗性">
	ACString resistValues[3] = { _AL("resists"), _AL("name"), _AL("抗性") };
	AXMLItem* pResistsItem = CreateXMLItem( pRolePropertyItem, pRoleBasicPropertyItem, NULL, resistValues, 3 );
	if ( !pResistsItem )
	{
		return NULL;
	}

	//抗性子节点数据项:眩晕抗性 0
	ACString resist1Values[5] = { _AL("resist"), _AL("name"), _AL("眩晕抗性"), _AL("value"), _AL("0") };
	resist1Values[4].Format( _AL("%d"), pDetailInfo->gsroleinfo.rolebasicinfo.resistance1 );
	AXMLItem* pResist1 = CreateXMLItem( pRolePropertyItem, pResistsItem, NULL, resist1Values, 5 );
	if ( !pResist1 )
	{
		return NULL;
	}

	//抗性子节点数据项:昏睡抗性 4
	ACString resist2Values[5] = { _AL("resist"), _AL("name"), _AL("昏睡抗性"), _AL("value"), _AL("0") };
	resist2Values[4].Format( _AL("%d"), pDetailInfo->gsroleinfo.rolebasicinfo.resistance5 );
	AXMLItem* pResist2 = CreateXMLItem( pRolePropertyItem, pResistsItem, pResist1, resist2Values, 5 );
	if ( !pResist2 )
	{
		return NULL;
	}

	//抗性子节点数据项:虚弱抗性 1
	ACString resist3Values[5] = { _AL("resist"), _AL("name"), _AL("虚弱抗性"), _AL("value"), _AL("0") };
	resist3Values[4].Format( _AL("%d"), pDetailInfo->gsroleinfo.rolebasicinfo.resistance2 );
	AXMLItem* pResist3 = CreateXMLItem( pRolePropertyItem, pResistsItem, pResist2, resist3Values, 5 );
	if ( !pResist3 )
	{
		return NULL;
	}

	//抗性子节点数据项：魅惑抗性 3
	ACString resist4Values[5] = { _AL("resist"), _AL("name"), _AL("魅惑抗性"), _AL("value"), _AL("0") };
	resist4Values[4].Format( _AL("%d"), pDetailInfo->gsroleinfo.rolebasicinfo.resistance4 );
	AXMLItem* pResist4 = CreateXMLItem( pRolePropertyItem, pResistsItem, pResist3, resist4Values, 5 );
	if ( !pResist4 )
	{
		return NULL;
	}

	//抗性子节点数据项：定身抗性 2
	ACString resist5Values[5] = { _AL("resist"), _AL("name"), _AL("定身抗性"), _AL("value"), _AL("0") };
	resist5Values[4].Format( _AL("%d"), pDetailInfo->gsroleinfo.rolebasicinfo.resistance3 );
	AXMLItem* pResist5 = CreateXMLItem( pRolePropertyItem, pResistsItem, pResist4, resist5Values, 5 );
	if ( !pResist5 )
	{
		return NULL;
	}

	//XML 第三层子节点: <renxings name="韧性">
	ACString renxingValues[3] = { _AL("renxings"), _AL("name"), _AL("韧性") };
	AXMLItem* pRenXingsItem = CreateXMLItem( pRolePropertyItem, pRoleBasicPropertyItem, pResistsItem, renxingValues, 3 );
	if ( !pRenXingsItem )
	{
		return NULL;
	}

	//韧性子节点数据项:眩晕韧性0
	ACString renxing1Values[5] = { _AL("renxing"), _AL("name"), _AL("眩晕韧性"), _AL("value"), _AL("0") };
	renxing1Values[4].Format( _AL("%d"), pDetailInfo->gsroleinfo.basic_ext.renxing1 );
	AXMLItem* pRenXing1 = CreateXMLItem( pRolePropertyItem, pRenXingsItem, NULL, renxing1Values, 5 );
	if ( !pRenXing1 )
	{
		return NULL;
	}

	//韧性子节点数据项:昏睡韧性 4
	ACString renxing2Values[5] = { _AL("renxing"), _AL("name"), _AL("昏睡韧性"), _AL("value"), _AL("0") };
	renxing2Values[4].Format( _AL("%d"), pDetailInfo->gsroleinfo.basic_ext.renxing5 );
	AXMLItem* pRenXing2 = CreateXMLItem( pRolePropertyItem, pRenXingsItem, pRenXing1, renxing2Values, 5 );
	if ( !pRenXing2 )
	{
		return NULL;
	}

	//韧性子节点数据项:虚弱韧性 1
	ACString renxing3Values[5] = { _AL("renxing"), _AL("name"), _AL("虚弱韧性"), _AL("value"), _AL("0") };
	renxing3Values[4].Format( _AL("%d"), pDetailInfo->gsroleinfo.basic_ext.renxing2 );
	AXMLItem* pRenXing3 = CreateXMLItem( pRolePropertyItem, pRenXingsItem, pRenXing2, renxing3Values, 5 );
	if ( !pRenXing3 )
	{
		return NULL;
	}

	//韧性子节点数据项：魅惑韧性 3
	ACString renxing4Values[5] = { _AL("renxing"), _AL("name"), _AL("魅惑韧性"), _AL("value"), _AL("0") };
	renxing4Values[4].Format( _AL("%d"), pDetailInfo->gsroleinfo.basic_ext.renxing4 );
	AXMLItem* pRenXing4 = CreateXMLItem( pRolePropertyItem, pRenXingsItem, pRenXing3, renxing4Values, 5 );
	if ( !pRenXing4 )
	{
		return NULL;
	}

	//韧性子节点数据项：定身韧性2
	ACString renxing5Values[5] = { _AL("renxing"), _AL("name"), _AL("定身韧性"), _AL("value"), _AL("0") };
	renxing5Values[4].Format( _AL("%d"), pDetailInfo->gsroleinfo.basic_ext.renxing3 );
	AXMLItem* pRenXing5 = CreateXMLItem( pRolePropertyItem, pRenXingsItem, pRenXing4, renxing5Values, 5 );
	if ( !pRenXing5 )
	{
		return NULL;
	}

	//XML 第三层子节点: <jingtongs name="精通">
	ACString jintongValues[3] = { _AL("jingtongs"), _AL("name"), _AL("精通") };
	AXMLItem* pJingTongsItem = CreateXMLItem( pRolePropertyItem, pRoleBasicPropertyItem, pRenXingsItem, jintongValues, 3 );
	if ( !pJingTongsItem )
	{
		return NULL;
	}
	//子节点数据项:眩晕精通0
	ACString jingtong1Values[5] = { _AL("jingtong"), _AL("name"), _AL("眩晕精通"), _AL("value"), _AL("0") };
	jingtong1Values[4].Format( _AL("%d"), pDetailInfo->gsroleinfo.basic_ext.jingtong1 );
	AXMLItem* pJingTong1 = CreateXMLItem( pRolePropertyItem, pJingTongsItem, NULL, jingtong1Values, 5 );
	if ( !pJingTong1 )
	{
		return NULL;
	}

	//子节点数据项:昏睡精通 4
	ACString jingtong2Values[5] = { _AL("jingtong"), _AL("name"), _AL("昏睡精通"), _AL("value"), _AL("0") };
	jingtong2Values[4].Format( _AL("%d"), pDetailInfo->gsroleinfo.basic_ext.jingtong5 );
	AXMLItem* pJingTong2 = CreateXMLItem( pRolePropertyItem, pJingTongsItem, pJingTong1, jingtong2Values, 5 );
	if ( !pJingTong2 )
	{
		return NULL;
	}

	//子节点数据项:虚弱精通 1
	ACString jingtong3Values[5] = { _AL("jingtong"), _AL("name"), _AL("虚弱精通"), _AL("value"), _AL("0") };
	jingtong3Values[4].Format( _AL("%d"), pDetailInfo->gsroleinfo.basic_ext.jingtong2 );
	AXMLItem* pJingTong3 = CreateXMLItem( pRolePropertyItem, pJingTongsItem, pJingTong2, jingtong3Values, 5 );
	if ( !pJingTong3 )
	{
		return NULL;
	}

	//精通子节点数据项：魅惑精通 3
	ACString jingtong4Values[5] = { _AL("jingtong"), _AL("name"), _AL("魅惑精通"), _AL("value"), _AL("0") };
	jingtong4Values[4].Format( _AL("%d"), pDetailInfo->gsroleinfo.basic_ext.jingtong4 );
	AXMLItem* pJingTong4 = CreateXMLItem( pRolePropertyItem, pJingTongsItem, pJingTong3, jingtong4Values, 5 );
	if ( !pJingTong4 )
	{
		return NULL;
	}

	//精通子节点数据项：定身精通 2
	ACString jingtong5Values[5] = { _AL("jingtong"), _AL("name"), _AL("定身精通"), _AL("value"), _AL("0") };
	jingtong5Values[4].Format( _AL("%d"), pDetailInfo->gsroleinfo.basic_ext.jingtong3 );
	AXMLItem* pJingTong5 = CreateXMLItem( pRolePropertyItem, pJingTongsItem, pJingTong4, jingtong5Values, 5 );
	if ( !pJingTong5 )
	{
		return NULL;
	}

	//特殊信息：第三层结点
	ACString specialValues[3] = { _AL("special_info"), _AL("name"), _AL("特殊信息") };
	AXMLItem* pSpecialInfoItem = CreateXMLItem( pRolePropertyItem, pRoleBasicPropertyItem, pJingTongsItem, specialValues, 3 );
	if ( !pSpecialInfoItem )
	{
		return NULL;
	}

	//玩家头像信息,第四层子结点
	ACString playerHeadIconValues[3] = { _AL("player_head_icon"), _AL("value"), _AL("head/0.png") };	//!!!!!!头像文件路径？!!!!!!
	playerHeadIconValues[2].Format( _AL("head/%d.png"), pDetailInfo->gsroleinfo.rolebasicinfo.cls );
	AXMLItem* pPlayerHeadIconItem = CreateXMLItem( pRolePropertyItem, pSpecialInfoItem, NULL, playerHeadIconValues, 3 );
	if ( !pPlayerHeadIconItem )
	{
		return NULL;
	}

	//玩家等级信息子节点
	ACString playerLevelValues[5] = { _AL("level"), _AL("name"), _AL("玩家等级"), _AL("value"), _AL("0") };
	playerLevelValues[4].Format( _AL("%d"), pDetailInfo->gsroleinfo.rolebasicinfo.level );
	AXMLItem* pPlayerLevelItem = CreateXMLItem( pRolePropertyItem, pSpecialInfoItem, pPlayerHeadIconItem, playerLevelValues, 5 );
	if ( !pPlayerLevelItem )
	{
		return NULL;
	}

	//门派信息子节点
	ACString playerClsValues[5] = { _AL("cls"), _AL("name"), _AL("门派"), _AL("value"), _AL("") };
	playerClsValues[4] =  GetClsName(pDetailInfo->gsroleinfo.rolebasicinfo.cls);
	AXMLItem* pPlayerClsItem = CreateXMLItem( pRolePropertyItem, pSpecialInfoItem, pPlayerLevelItem, playerClsValues, 5 );
	if ( !pPlayerClsItem )
	{
		return NULL;
	}
	
	//飞升前职业信息节点
	ACString profBeforeRenascenceValues[5] = { _AL("prof_before_renascence"), _AL("name"), _AL("飞升前职业"), _AL("value"), _AL("") };
	ACString strLevelBeforeReborn;
	strLevelBeforeReborn.Format( _AL(" %d"), pDetailInfo->gsroleinfo.rolebasicinfo.level_before_reborn );
	profBeforeRenascenceValues[4] = GetClsName( pDetailInfo->gsroleinfo.rolebasicinfo.cls_before_reborn ) + strLevelBeforeReborn;
	AXMLItem* pProfBeforeRenascenceItem = CreateXMLItem( pRolePropertyItem, pSpecialInfoItem, pPlayerClsItem, profBeforeRenascenceValues, 5 );
	if ( !pProfBeforeRenascenceItem )
	{
		return NULL;
	}

	//血炼值
	ACString combinedValueValues[5] = { _AL("combine_value"), _AL("name"), _AL("血炼值"), _AL("value"), _AL("") };
	combinedValueValues[4].Format( _AL("%d"), pDetailInfo->gsroleinfo.rolebasicinfo.talisman_value );
	AXMLItem* pCombinedValueItem = CreateXMLItem( pRolePropertyItem, pSpecialInfoItem, pProfBeforeRenascenceItem, combinedValueValues, 5 );
	if ( !pCombinedValueItem )
	{
		return NULL;
	}

	//狭义度子结点
	ACString profManaValues[9] = { _AL("prof_mana"), _AL("name"), _AL("侠义度"), _AL("value"), _AL(""), _AL("progress"), _AL(""), _AL("desc"), _AL("") };
	GetProfMana( pDetailInfo->gsroleinfo.rolebasicinfo.reputation, profManaValues[4], profManaValues[6], profManaValues[8] );
	AXMLItem* pProfManaItem = CreateXMLItem( pRolePropertyItem, pSpecialInfoItem, pCombinedValueItem, profManaValues, 9 );
	if ( !pProfManaItem )
	{
		return NULL;
	}

	//善恶值子节点
	ACString pkValueValues[5] = { _AL("pk_value"), _AL("name"), _AL("善恶值"), _AL("value"), _AL("") };
	pkValueValues[4].Format( _AL("%d"), pDetailInfo->gsroleinfo.rolebasicinfo.pk_value );
	AXMLItem* pPKValueItem = CreateXMLItem( pRolePropertyItem, pSpecialInfoItem, pProfManaItem, pkValueValues, 5 );
	if ( !pPKValueItem )
	{
		return NULL;
	}

	//性别子节点	---------New Added 
	ACString genderValues[5] = { _AL("gender"), _AL("name"), _AL("性别"), _AL("value"), _AL("") };
	genderValues[4] = ((pDetailInfo->gsroleinfo.rolebasicinfo.gender == 0) ? _AL("男") : _AL("女"));
	AXMLItem* pGenderValueItem = CreateXMLItem( pRolePropertyItem, pSpecialInfoItem, pPKValueItem, genderValues, 5 );
	if ( !pGenderValueItem )
	{
		return NULL;
	}

	//基本信息 第三层子节点
	ACString basicInfoValues[3] = { _AL("basic_value"), _AL("name"), _AL("基本信息") };
	AXMLItem* pBasicInfoItem = CreateXMLItem( pRolePropertyItem, pRoleBasicPropertyItem, pSpecialInfoItem, basicInfoValues, 3 );
	if ( !pBasicInfoItem )
	{
		return NULL;
	}

	//修为信息 第四层子节点
	ACString expValues[5] = { _AL("exp"), _AL("name"), _AL("修为"), _AL("value"), _AL("") };
	double fMaxExp = GetLevelUpExp( pDetailInfo->gsroleinfo.rolebasicinfo.level + 1, pDetailInfo->gsroleinfo.rolebasicinfo.reborn_count );
	double fExpRatio = 0.0;
	if ( fMaxExp > 0.00 )
	{
		fExpRatio = (double)pDetailInfo->gsroleinfo.rolebasicinfo.exp / fMaxExp;
		if ( fExpRatio > 0.9999 )
		{
			fExpRatio = 0.9999;
		}
	}
	expValues[4].Format( _AL("%.02f%%"), fExpRatio*100.0 );
	AXMLItem* pExpItem = CreateXMLItem( pRolePropertyItem, pBasicInfoItem, NULL, expValues, 5 );
	if ( !pExpItem )
	{
		return NULL;
	}

	//气血信息
	ACString hpValues[5] = { _AL("hp"), _AL("name"), _AL("气血"), _AL("value"), _AL("") };
	hpValues[4].Format( _AL("%d/%d"), pDetailInfo->gsroleinfo.rolebasicinfo.max_hp, pDetailInfo->gsroleinfo.rolebasicinfo.max_hp );
	AXMLItem* pHpItem = CreateXMLItem( pRolePropertyItem, pBasicInfoItem, pExpItem, hpValues, 5 );
	if ( !pHpItem )
	{
		return NULL;
	}

	//真气信息
	ACString mpValues[5] = { _AL("mp"), _AL("name"), _AL("真气"), _AL("value"), _AL("") };
	mpValues[4].Format( _AL("%d/%d"), pDetailInfo->gsroleinfo.rolebasicinfo.max_mp, pDetailInfo->gsroleinfo.rolebasicinfo.max_mp );
	AXMLItem* pMpItem = CreateXMLItem( pRolePropertyItem, pBasicInfoItem, pHpItem, mpValues, 5 );
	if ( !pMpItem )
	{
		return NULL;
	}

	//攻击信息
	ACString attackValues[5] = { _AL("attack"), _AL("name"), _AL("攻击"), _AL("value"), _AL("") };
	attackValues[4].Format( _AL("%d-%d"), pDetailInfo->gsroleinfo.rolebasicinfo.damage_low, pDetailInfo->gsroleinfo.rolebasicinfo.damage_high );
	AXMLItem* pAttackItem = CreateXMLItem( pRolePropertyItem, pBasicInfoItem, pMpItem, attackValues, 5 );
	if ( !pAttackItem )
	{
		return NULL;
	}

	//防御信息
	ACString defenceValues[5] = { _AL("defence"), _AL("name"), _AL("防御"), _AL("value"), _AL("") };
	defenceValues[4].Format( _AL("%d"), pDetailInfo->gsroleinfo.rolebasicinfo.defense );
	AXMLItem* pDefenceItem = CreateXMLItem( pRolePropertyItem, pBasicInfoItem, pAttackItem, defenceValues, 5 );
	if ( !pDefenceItem )
	{
		return NULL;
	}

	//普攻命中信息
	ACString definitionValues[5] = { _AL("definition"), _AL("name"), _AL("普攻命中"), _AL("value"), _AL("") };
	definitionValues[4].Format( _AL("%d"), pDetailInfo->gsroleinfo.rolebasicinfo.attack );
	AXMLItem* pDefinitionItem = CreateXMLItem( pRolePropertyItem, pBasicInfoItem, pDefenceItem, definitionValues, 5 );
	if ( !pDefinitionItem )
	{
		return NULL;
	}

	//普攻躲闪信息
	ACString evadeValues[5] = { _AL("evade"), _AL("name"), _AL("普攻躲闪"), _AL("value"), _AL("") };
	evadeValues[4].Format( _AL("%d"), pDetailInfo->gsroleinfo.rolebasicinfo.armor );
	AXMLItem* pEvadeItem = CreateXMLItem( pRolePropertyItem, pBasicInfoItem, pDefinitionItem, evadeValues, 5 );
	if ( !pEvadeItem )
	{
		return NULL;
	}

	//致命伤害
	ACString criticalDamageValues[5] = { _AL("critical_damage"), _AL("name"), _AL("致命伤害"), _AL("value"), _AL("") };
	criticalDamageValues[4].Format( _AL("%.1f%%"), pDetailInfo->gsroleinfo.rolebasicinfo.crit_damage * 100 + 0.001 );
	AXMLItem* pCriticalDamageItem = CreateXMLItem( pRolePropertyItem, pBasicInfoItem, pEvadeItem, criticalDamageValues, 5 );
	if ( !pCriticalDamageItem )
	{
		return NULL;
	}

	//致命一击率
	ACString criticalRateValues[5] = { _AL("critical_rate"), _AL("name"), _AL("致命一击率"), _AL("value"), _AL("") };
	criticalRateValues[4].Format( _AL("%.1f%%"), pDetailInfo->gsroleinfo.rolebasicinfo.crit_rate / 10.0f );
	AXMLItem* pCriticalRateItem = CreateXMLItem( pRolePropertyItem, pBasicInfoItem, pCriticalDamageItem, criticalRateValues, 5 );
	if ( !pCriticalRateItem )
	{
		return NULL;
	}

	//技能躲闪
	ACString skillEvadeValues[5] = { _AL("skill_evade"), _AL("name"), _AL("技能躲闪"), _AL("value"), _AL("") };
	skillEvadeValues[4].Format( _AL("%.1f"), pDetailInfo->gsroleinfo.rolebasicinfo.skill_armor_rate / 10 );
	AXMLItem* pSkillEvadeItem = CreateXMLItem( pRolePropertyItem, pBasicInfoItem, pCriticalRateItem, skillEvadeValues, 5 );
	if ( !pSkillEvadeItem )
	{
		return NULL;
	}

	//技能命中
	ACString skillDefinitionValues[5] = { _AL("skill_definition"), _AL("name"), _AL("技能命中"), _AL("value"), _AL("") };
	skillDefinitionValues[4].Format( _AL("%.1f"), 100 + pDetailInfo->gsroleinfo.rolebasicinfo.skill_attack_rate / 10.0f );
	AXMLItem* pSkillDefinitionItem = CreateXMLItem( pRolePropertyItem, pBasicInfoItem, pSkillEvadeItem, skillDefinitionValues, 5 );
	if ( !pSkillDefinitionItem )
	{
		return NULL;
	}

	//减免他方致命率
	ACString deCriticalValues[5] = { _AL("de_critical"), _AL("name"), _AL("减免他方致命率"), _AL("value"), _AL("") };
	deCriticalValues[4].Format( _AL("%.1f%%"), pDetailInfo->gsroleinfo.rolebasicinfo.anti_crit / 10.0f );
	AXMLItem* pDeCriticalItem = CreateXMLItem( pRolePropertyItem, pBasicInfoItem, pSkillDefinitionItem, deCriticalValues, 5 );
	if ( !pDeCriticalItem )
	{
		return NULL;
	}

	//减免致命伤害
	ACString deCriticalDamageValues[5] = { _AL("de_critical_damage"), _AL("name"), _AL("减免致命伤害"), _AL("value"), _AL("") };
	deCriticalDamageValues[4].Format( _AL("%.1f%%"), pDetailInfo->gsroleinfo.rolebasicinfo.anti_crit_damage * 100 + 0.001 );
	AXMLItem* pDeCriticalDamageItem = CreateXMLItem( pRolePropertyItem, pBasicInfoItem, pDeCriticalItem, deCriticalDamageValues, 5 );
	if ( !pDeCriticalDamageItem )
	{
		return NULL;
	}

	//御仙
	ACString deDamageTaoismValues[5] = { _AL("de_damage_taoism"), _AL("name"), _AL("御仙"), _AL("value"), _AL("") };
	deDamageTaoismValues[4].Format( _AL("%.1f%%"), pDetailInfo->gsroleinfo.rolebasicinfo.cult_defense_xian / 10.0f );
	AXMLItem* pDeDamageTaoismItem = CreateXMLItem( pRolePropertyItem, pBasicInfoItem, pDeCriticalDamageItem, deDamageTaoismValues, 5 );
	if ( !pDeDamageTaoismItem )
	{
		return NULL;
	}

	//御魔
	ACString deDamageEvilValues[5] = { _AL("de_damage_evil"), _AL("name"), _AL("御魔"), _AL("value"), _AL("") };
	deDamageEvilValues[4].Format( _AL("%.1f%%"), pDetailInfo->gsroleinfo.rolebasicinfo.cult_defense_mo / 10.0f );
	AXMLItem* pDeDamageEvilItem = CreateXMLItem( pRolePropertyItem, pBasicInfoItem, pDeDamageTaoismItem, deDamageEvilValues, 5 );
	if ( !pDeDamageEvilItem )
	{
		return NULL;
	}

	//御佛
	ACString deDamageBuddhismValues[5] = { _AL("de_damage_buddhism"), _AL("name"), _AL("御佛"), _AL("value"), _AL("") };
	deDamageBuddhismValues[4].Format( _AL("%.1f%%"), pDetailInfo->gsroleinfo.rolebasicinfo.cult_defense_fo / 10.0f );
	AXMLItem* pDeDamageBuddhismItem = CreateXMLItem( pRolePropertyItem, pBasicInfoItem, pDeDamageEvilItem, deDamageBuddhismValues, 5 );
	if ( !pDeDamageBuddhismItem )
	{
		return NULL;
	}

	//元神 第二层子节点
	ACString deityInfoValues[3] = { _AL("deity_info"), _AL("name"), _AL("元神") };
	AXMLItem* pDeityInfoItem = CreateXMLItem( pRolePropertyItem, pRolePropertyItem, pRoleBasicPropertyItem, deityInfoValues, 3 );
	if ( !pDeityInfoItem )
	{
		return NULL;
	}

	//元神称号
	ACString deityTitleValues[3] = { _AL("deity_title"), _AL("name"), _AL("未封神") };
	deityTitleValues[2] = GetDeityTitle( pDetailInfo->gsroleinfo.deityinfo.deity_level, pDetailInfo->gsroleinfo.rolebasicinfo.cultivation );
	AXMLItem* pDeityTitleItem = CreateXMLItem( pRolePropertyItem, pDeityInfoItem, NULL, deityTitleValues, 3 );
	if ( !pDeityTitleItem )
	{
		return NULL;
	}

	//克仙
	ACString deTaoismValues[5] = { _AL("de_taoism"), _AL("name"), _AL("克仙"), _AL("value"), _AL("") };
	deTaoismValues[4].Format( _AL("%.1f%%"), pDetailInfo->gsroleinfo.deityinfo.atatck_xian / 10.0f );
	AXMLItem* pDeTaoismItem = CreateXMLItem( pRolePropertyItem, pDeityInfoItem, pDeityTitleItem, deTaoismValues, 5 );
	if ( !pDeTaoismItem )
	{
		return NULL;
	}

	//克佛
	ACString deBuddhismValues[5] = { _AL("de_buddhism"), _AL("name"), _AL("克佛"), _AL("value"), _AL("") };
	deBuddhismValues[4].Format( _AL("%.1f%%"), pDetailInfo->gsroleinfo.deityinfo.atatck_fo / 10.0f );
	AXMLItem* pDeBuddhismItem = CreateXMLItem( pRolePropertyItem, pDeityInfoItem, pDeTaoismItem, deBuddhismValues, 5 );
	if ( !pDeBuddhismItem )
	{
		return NULL;
	}

	//克魔
	ACString deEvilValues[5] = { _AL("de_evil"), _AL("name"), _AL("克魔"), _AL("value"), _AL("") };
	deEvilValues[4].Format( _AL("%.1f%%"), pDetailInfo->gsroleinfo.deityinfo.atatck_mo / 10.0f );
	AXMLItem* pDeEvilItem = CreateXMLItem( pRolePropertyItem, pDeityInfoItem, pDeBuddhismItem, deEvilValues, 5 );
	if ( !pDeEvilItem )
	{
		return NULL;
	}

	//元神等级
	ACString deityLevelValues[5] = { _AL("deity_level"), _AL("name"), _AL("元神等级"), _AL("value"), _AL("") };
	deityLevelValues[4].Format( _AL("%d"), pDetailInfo->gsroleinfo.deityinfo.deity_level );
	AXMLItem* pDeityLevelItem = CreateXMLItem( pRolePropertyItem, pDeityInfoItem, pDeEvilItem, deityLevelValues, 5 );
	if ( !pDeityLevelItem )
	{
		return NULL;
	}

	//元神威能
	ACString deitySpValues[5] = { _AL("deity_sp"), _AL("name"), _AL("元神威能"), _AL("value"), _AL("") };
	deitySpValues[4].Format( _AL("%d"), pDetailInfo->gsroleinfo.deityinfo.deity_power );
	AXMLItem* pDeitySpItem = CreateXMLItem( pRolePropertyItem, pDeityInfoItem, pDeityLevelItem, deitySpValues, 5 );
	if ( !pDeitySpItem )
	{
		return NULL;
	}

	//元力值
	ACString deityNumValues[7] = { _AL("deity_num"), _AL("name"), _AL("元力值"), _AL("value"), _AL(""), _AL("progress"), _AL("0") }; //元力值进度条默认设置为满的=0
	deityNumValues[4].Format( _AL("%d/%d"), pDetailInfo->gsroleinfo.deityinfo.max_dp, pDetailInfo->gsroleinfo.deityinfo.max_dp );
	if ( pDetailInfo->gsroleinfo.deityinfo.max_dp )
	{
		deityNumValues[6] = _AL("100");
	}
	AXMLItem* pDeityNumItem = CreateXMLItem( pRolePropertyItem, pDeityInfoItem, pDeitySpItem, deityNumValues, 7 );
	if ( !pDeityNumItem )
	{
		return NULL;
	}

	//元神修为
	ACString deityExpValues[7] = { _AL("deity_exp"), _AL("name"), _AL("元神修为"), _AL("value"), _AL(""), _AL("progress"), _AL("") };
	double dMaxDpExp = GetDeityLevelUpExp( pDetailInfo->gsroleinfo.deityinfo.deity_level + 1 );
	int nDeityProgress = (int)(((double)(pDetailInfo->gsroleinfo.deityinfo.deity_exp) / dMaxDpExp)*100);
	if ( nDeityProgress > 100 )	//超过100的情况。。。。。
	{
		nDeityProgress = 100;
	}
	deityExpValues[4].Format( _AL("%.0f/%.0f"), (double)(pDetailInfo->gsroleinfo.deityinfo.deity_exp), dMaxDpExp );
	deityExpValues[6].Format( _AL("%d"), nDeityProgress );
	AXMLItem* pDeityExpItem = CreateXMLItem( pRolePropertyItem, pDeityInfoItem, pDeityNumItem, deityExpValues, 7 );
	if ( !pDeityExpItem )
	{
		return NULL;
	}

	//尊号 第二层子节点
	ACString playerTitlesValues[3] = { _AL("player_titles"), _AL("name"), _AL("尊号") };
	AXMLItem* pPlayerTitlesItem = CreateXMLItem( pRolePropertyItem, pRolePropertyItem, pDeityInfoItem, playerTitlesValues, 3 );
	if ( !pPlayerTitlesItem )
	{
		return NULL;
	}

	//加入各类型的尊号
	ACString strDesc, strHint;
	CConfigFromLua * pConfigFromLua = g_pGame->GetConfigFromLua();
	AXMLItem* pTitleCatalogItem = NULL;
	abase::hash_map<int, bool> titlesMap;
	GetPlayerTitles( &pDetailInfo->gsroleinfo.title.titlelist, titlesMap );
	for ( int i = 0; i < pConfigFromLua->m_TitleDir[0].m_ArrVal.size(); ++i )	//LUA 配置表
	{
		if( pConfigFromLua->m_TitleDir[0].m_ArrVal[i].m_Type == CScriptValue::SVT_ARRAY )	//第二层也是数组
		{
			if( pConfigFromLua->m_TitleDir[0].m_ArrVal[i].m_ArrVal[0].m_Type == CScriptValue::SVT_STRING ) //头一个元素是名称
			{
				pConfigFromLua->m_TitleDir[0].m_ArrVal[i].m_ArrVal[0].RetrieveAWString(strDesc);	//获得称号类别
				ACString titleCatalog[3] = { _AL("title_catalog"), _AL("name"), _AL("") };
				titleCatalog[2] = strDesc;
				pTitleCatalogItem = CreateXMLItem( pRolePropertyItem, pPlayerTitlesItem, pTitleCatalogItem, titleCatalog, 3 );
				if ( !pTitleCatalogItem )
				{
					return NULL;
				}

				//处理每个称号类别内的具体称号
				AXMLItem* pRoleTitleItem = NULL;
				for ( int j = 0; j < pConfigFromLua->m_TitleDir[0].m_ArrVal[i].m_ArrVal.size(); ++j )
				{
					if( pConfigFromLua->m_TitleDir[0].m_ArrVal[i].m_ArrVal[j].m_Type == CScriptValue::SVT_NUMBER )
					{
						int idTitle = pConfigFromLua->m_TitleDir[0].m_ArrVal[i].m_ArrVal[j].GetInt();
						if ( titlesMap.find( idTitle ) != titlesMap.end() )	//查找Title ID
						{
							ACString roleTitle[5] = { _AL("role_title"), _AL("name"), _AL(""), _AL("desc"), _AL("") };
							//-------------------------------------------------
							ACString strTmp;
							TranslateStr( GetTitle( idTitle ), strTmp );
							roleTitle[2] = strTmp;	//GetTitle( idTitle ) 转义。。。。
							TranslateStr( GetTitleHint( idTitle ), strTmp );
							roleTitle[4] = strTmp;	//GetTitleHint( idTitle ) 转义。。。
							//-------------------------------------------------
//							roleTitle[2] = GetTitle( idTitle );
//							roleTitle[4] = GetTitleHint( idTitle );
							pRoleTitleItem = CreateXMLItem( pRolePropertyItem, pTitleCatalogItem, pRoleTitleItem, roleTitle, 5 );
							if ( !pRoleTitleItem )
							{
								return NULL;
							}
						}
					}
					else if( pConfigFromLua->m_TitleDir[0].m_ArrVal[i].m_ArrVal[j].m_Type == CScriptValue::SVT_ARRAY )
					{
						if( pConfigFromLua->m_TitleDir[0].m_ArrVal[i].m_ArrVal[j].m_ArrVal[0].m_Type == CScriptValue::SVT_STRING )
						{
							pConfigFromLua->m_TitleDir[0].m_ArrVal[i].m_ArrVal[j].m_ArrVal[0].RetrieveAWString(strHint);
							strDesc = _AL("????????");
							for(int k = 1; k < pConfigFromLua->m_TitleDir[0].m_ArrVal[i].m_ArrVal[j].m_ArrVal.size(); ++k )
							{
								if( pConfigFromLua->m_TitleDir[0].m_ArrVal[i].m_ArrVal[j].m_ArrVal[k].m_Type == CScriptValue::SVT_NUMBER )
								{
									int idTitle = pConfigFromLua->m_TitleDir[0].m_ArrVal[i].m_ArrVal[j].m_ArrVal[k].GetInt();
									if ( titlesMap.find( idTitle ) != titlesMap.end() ) //查找Title ID
									{
										ACString roleTitle[5] = { _AL("role_title"), _AL("name"), _AL(""), _AL("desc"), _AL("") };
										//-------------------------------------------------
										ACString strTmp;
										TranslateStr( GetTitle( idTitle ), strTmp );
										roleTitle[2] = strTmp;	//GetTitle( idTitle ) 转义。。。。
										TranslateStr( GetTitleHint( idTitle ), strTmp );
										roleTitle[4] = strTmp;	//GetTitleHint( idTitle ) 转义。。。
										//-------------------------------------------------
//										roleTitle[2] = GetTitle( idTitle );
//										roleTitle[4] = GetTitleHint( idTitle );
										pRoleTitleItem = CreateXMLItem( pRolePropertyItem, pTitleCatalogItem, pRoleTitleItem, roleTitle, 5 );
										if ( !pRoleTitleItem )
										{
											return NULL;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	//声望 第二层子节点
	ACString strReputations[3] = { _AL("reputations"), _AL("name"), _AL("声望") };
	AXMLItem* pReputationItem = CreateXMLItem( pRolePropertyItem, pRolePropertyItem, pPlayerTitlesItem, strReputations, 3 );
	if ( !pReputationItem )
	{
		return NULL;
	}
	
	//插入各声望的子节点
	const int nReputationMappingID[] = { 1, 2, 3, 4, 0, 19, 14, 13, 15, 16, 17, 18, 22, 5, 6, 7, 8, 12, 9, 10, 11, 20, 21, 22, 23, 24 };	//由于顺序问题，导致当前需要处理的声望id和游戏内UI对应id需要做映射
	const int nReputationTypeSize = 6;
	const int nReputationCount = 26;
	ACString strReputationType[nReputationTypeSize][6] =	{	{ _AL("rep_prof"), _AL("name"), _AL("门派贡献度"), _AL("count"), _AL("12") }, 
																{ _AL("classic"), _AL("name"), _AL("经典"), _AL("count"), _AL("5") },
																{ _AL("special"), _AL("name"), _AL("特殊"), _AL("count"), _AL("1") },
																{ _AL("nature"), _AL("name"), _AL("造化"), _AL("count"), _AL("3") },
																{ _AL("primodial"), _AL("name"), _AL("鸿蒙"), _AL("count"), _AL("2") },
																{ _AL("zong"), _AL("name"), _AL("天地人宗"), _AL("count"), _AL("3") }
															};
	const int nReputationLoops[nReputationTypeSize] =	{ 12, 5, 1, 3, 2, 3 };
	ACString strReputationValues[nReputationCount][9] =		{
												{ _AL("reputation"), _AL("name"), _AL("青云"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") }, 
												{ _AL("reputation"), _AL("name"), _AL("天音"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("鬼王"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("合欢"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("鬼道"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("焚香"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("烈山"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("九黎"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("辰皇"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("太昊"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("怀光"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("天华"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("仙基"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("忠义"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("情缘"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("文采"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("师德"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("御灵"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("道心"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("魔性"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("佛缘"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("战绩"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("武勋"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("天宗"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("地宗"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
												{ _AL("reputation"), _AL("name"), _AL("人宗"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
											};
	AXMLItem* pReputationTypeItem = NULL;
	int nCurIndex = 0;
	abase::vector<int> reputationList;
	GetPlayerReputationList( pDetailInfo->creditinfo.credits, reputationList );
	for ( int k = 0; k < nReputationTypeSize; ++k )
	{
		//构建声望类子节点
		pReputationTypeItem = CreateXMLItem( pRolePropertyItem, pReputationItem, pReputationTypeItem, strReputationType[k], 5 );
		if ( !pReputationTypeItem )
		{
			return NULL;
		}
		
		//构建声望类下面的具体声望子节点
		AXMLItem* pReputationValueItem = NULL;
		for ( int j = 0; j < nReputationLoops[k]; ++j )
		{
			//TODO:设置strReputationValues数组中具体属性值
			if ( (nCurIndex != 12) && (reputationList.size() > nReputationMappingID[nCurIndex]) )
			{
				GetReputationStringValues( reputationList[nReputationMappingID[nCurIndex]], 
											nReputationMappingID[nCurIndex], 
											strReputationValues[nCurIndex][4], 
											strReputationValues[nCurIndex][6], 
											strReputationValues[nCurIndex][8]);
													
			}
			if ( 12 == nCurIndex )	//写死 仙基
			{
				GetReputationStringValues(	pDetailInfo->gsroleinfo.rolebasicinfo.battlescore,	//仙基值!!!!
											23, //设定为23!!!!
											strReputationValues[nCurIndex][4], 
											strReputationValues[nCurIndex][6], 
											strReputationValues[nCurIndex][8]);				
			}
			//创建具体声望子节点，并将其插入到合适位置
			if ( nCurIndex < nReputationCount )
			{
				pReputationValueItem = CreateXMLItem( pRolePropertyItem, pReputationTypeItem, pReputationValueItem, strReputationValues[nCurIndex], 9 );
				if ( !pReputationValueItem )
				{
					return NULL;
				}
			}
			else
			{
				a_LogOutput( 1, "Create XML item Error,\"nCurIndex < nReputationCount\"!\n" );
			}

			//
			nCurIndex++;
		}
	}

	//装备属性 第二层子节点
	ACString strEquipments[5] = { _AL("equipments"), _AL("name"), _AL("装备"), _AL("count"), _AL("0") };
	strEquipments[4].Format( _AL("%d"), pDetailInfo->pocketinfo.equipment.size() );
	AXMLItem* pEquipmentsItem = CreateXMLItem( pRolePropertyItem, pRolePropertyItem, pReputationItem, strEquipments, 5 );
	if ( !pEquipmentsItem )
	{
		return NULL;
	}

	//插入各个装备子节点
	AXMLItem* pEquipmentItem = NULL;
	ACString strEquipmentItem[13] = { _AL("item"), _AL("id"), _AL(""), _AL("pos"), _AL(""), _AL("level"), _AL(""), _AL("name"), _AL(""), _AL("icon"), _AL(""), _AL("desc"), _AL("") };
	for ( int nItemIndex = 0; nItemIndex < (int)pDetailInfo->pocketinfo.equipment.size(); ++nItemIndex )
	{
		const GRoleInventory& equipItem = pDetailInfo->pocketinfo.equipment[nItemIndex];
		CECIvtrItem* pItem = CECIvtrItem::CreateItem(equipItem.id, equipItem.expire_date, equipItem.count);
		if ( !pItem )
		{
			a_LogOutput(1, "Failed to create equipment.ID=%d", equipItem.id);
			continue;
		}
		if ( equipItem.data.begin() && equipItem.data.size() )
		{
			pItem->SetItemInfo((BYTE*)equipItem.data.begin(), equipItem.data.size());
		}
		pItem->SetStateFlag(Proctype2State(equipItem.proctype));	//New Added 2013-08-09.
		pItem->UpdateInfoMD5();
		strEquipmentItem[2].Format( _AL("%d"), equipItem.id );
		strEquipmentItem[4].Format( _AL("%d"), equipItem.pos );
		strEquipmentItem[6].Format( _AL("%d"), pItem->GetEstone() );
		//-------------------------------------------------------------------------特殊字符转义
		ACString strItemName;
		TranslateStr( pItem->GetName(), strItemName );
		strEquipmentItem[8] = strItemName;//pItem->GetName()
		//-------------------------------------------------------------------------
		strEquipmentItem[10].Format( _AL("%d.png"), equipItem.id );
		//-------------------------------------------------------------------------特殊字符转义
		ACString strItemDesc;
		TranslateStr( pItem->GetDesc(), strItemDesc );
		strEquipmentItem[12] = strItemDesc;//pItem->GetDesc()
		//-------------------------------------------------------------------------
		pEquipmentItem = CreateXMLItem( pRolePropertyItem, pEquipmentsItem, pEquipmentItem, strEquipmentItem, 13 );
		if ( !pEquipmentItem )
		{
			return NULL;
		}
		
		//!!!!!!注意：清理资源!!!!!!
		delete pItem;
	}

	//最后返回该角色属性根节点
	return pRolePropertyItem;

}

AXMLItem* CECRoleDescXMLParser::BuildRoleSkillsXML( GNET::GConsignRole* pDetailInfo )
{
	//判断参数合法性
	if ( !pDetailInfo )
	{
		a_LogOutput(1, "BuildRoleSkillsXML: Invalid parameter!");
		return NULL;
	}
	
	//技能 第一层子节点
	ACString strRoleSkillsItem[3] = { _AL("skills"), _AL("name"), _AL("技能") };
	AXMLItem* pRoleSkillsItem = CreateXMLItem( NULL, NULL, NULL, strRoleSkillsItem, 3 );
	if ( !pRoleSkillsItem )
	{
		return NULL;
	}

	//造化技能 第二层子节点
	ACString strNaturePropertyItem[3] = { _AL("nature_property"), _AL("name"), _AL("造化") };
	AXMLItem* pNaturePropertyItem = CreateXMLItem( pRoleSkillsItem, pRoleSkillsItem, NULL, strNaturePropertyItem, 3 );
	if ( !pNaturePropertyItem )
	{
		return NULL;
	}

	//造化技能具体子节点
	const int NATURE_TYPE_COUNT = 3;	// 造化子类： 道术，魔劫，佛法
	const int NATURE_SKILL_TYPE_COUNT = 3; //基本技能, 通用技能, 门派技能
	ACString strNatureType[NATURE_TYPE_COUNT][3] =	{
														{ _AL("taoism"), _AL("name"), _AL("道术") },
														{ _AL("evil"), _AL("name"), _AL("魔劫") },
														{ _AL("buddhism"), _AL("name"), _AL("佛法") },
													};

	ACString strNatureValue[NATURE_TYPE_COUNT][9] =	{
														{ _AL("taoism_value"), _AL("name"), _AL("道心值"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
														{ _AL("evil_value"), _AL("name"), _AL("魔性值"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
														{ _AL("buddhism_value"), _AL("name"), _AL("佛缘值"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
													};

	ACString strNatureSkillType[NATURE_SKILL_TYPE_COUNT][5] =	{
																	{ _AL("basic_skill"), _AL("name"), _AL("基本技能"), _AL("count"), _AL("0") },
																	{ _AL("generic_skill"), _AL("name"), _AL("通用技能"), _AL("count"), _AL("0") },
																	{ _AL("sect_skill"), _AL("name"), _AL("门派技能"), _AL("count"), _AL("0") },
																};

	const int nReputationIndex[NATURE_TYPE_COUNT] = { 9, 10, 11 };

	//
	AXMLItem* pNatureTypeItem = NULL;
	abase::hash_map<int, int> skillsIDList;
	skillsIDList.clear();
	GetSkillsInfo( pDetailInfo->gsroleinfo.skillinfo.skills, skillsIDList );
	abase::vector<int> reputationList;
	reputationList.clear();
	GetPlayerReputationList( pDetailInfo->creditinfo.credits, reputationList );
	for ( int i = 0; i < NATURE_TYPE_COUNT; ++i )	//处理造化子类
	{
		pNatureTypeItem = CreateXMLItem( pRoleSkillsItem, pNaturePropertyItem, pNatureTypeItem, strNatureType[i], 3 );
		if ( !pNatureTypeItem )
		{
			return NULL;
		}

		//插入造化子类属性子节点
		//TODO:修改相关值
		GetReputationStringValues( reputationList[nReputationIndex[i]], nReputationIndex[i], strNatureValue[i][4], strNatureValue[i][6], strNatureValue[i][8] );
		AXMLItem* pNatureValueItem = CreateXMLItem( pRoleSkillsItem, pNatureTypeItem, NULL, strNatureValue[i], 9 );
		if ( !pNatureValueItem )
		{
			return NULL;
		}

		//再插入造化技能子节点
		AXMLItem* pNatureSkillTypeItem = pNatureValueItem;	//!!!!!!注意处理方式:为了循环方便!!!!!!
		for ( int j = 0; j < NATURE_SKILL_TYPE_COUNT; ++j )
		{
			//TODO:数量值？
			abase::vector<int> natureSkillIDList;
			natureSkillIDList.clear();
			int nCLSType = GetClsTypeByProfID( pDetailInfo->gsroleinfo.rolebasicinfo.cls );
			if ( nCLSType < 0 )
			{
				a_LogOutput( 1, "BuildRoleSkills consign role Profession Error = %d\n", pDetailInfo->gsroleinfo.rolebasicinfo.cls );
			}
			GetNatureSkillInfo( skillsIDList, i, j, nCLSType, natureSkillIDList );
			strNatureSkillType[j][4].Format( _AL("%d"), natureSkillIDList.size() );
			pNatureSkillTypeItem = CreateXMLItem( pRoleSkillsItem, pNatureTypeItem, pNatureSkillTypeItem, strNatureSkillType[j], 5 );
			if ( !pNatureSkillTypeItem )
			{
				return NULL;
			}
			
			//创建该类技能的具体造化技能子节点
			AXMLItem* pNatureSkillItem = NULL;
			for ( int k = 0; k < natureSkillIDList.size(); ++k )
			{
				int nLevel = skillsIDList[natureSkillIDList[k]];
				nLevel = ( nLevel ? nLevel : 0 );
				CECSkill* pSkill = new CECSkill(natureSkillIDList[k], nLevel);
				if ( !pSkill )
				{
					a_LogOutput( 1, "Create Skill Failed! SkillID=%d\n", natureSkillIDList[k] );
					ReleaseXMLItems( pRoleSkillsItem );
					return NULL;
				}
				ACString strSkillItemValues[9] = { _AL("skill"), _AL("id"), _AL(""), _AL("name"), _AL(""), _AL("icon"), _AL(""), _AL("desc"), _AL("") };
				strSkillItemValues[2].Format( _AL("%d"), natureSkillIDList[k] );
				//-------------------------------------------------------------------------特殊字符转义
				ACString strSkillName;
				TranslateStr( pSkill->GetName(), strSkillName );
				strSkillItemValues[4] = strSkillName;//pSkill->GetName()
				//-------------------------------------------------------------------------
				strSkillItemValues[6].Format( _AL("skill/%d.png"), natureSkillIDList[k] );
				//-------------------------------------------------------------------------特殊字符转义
				ACString strSkillDesc;
				//AUICTranslate trans;
				//strSkillDesc = trans.Translate( pSkill->GetDesc() );
				TranslateStr( pSkill->GetDesc(), strSkillDesc );
				//-------------------------------------------------------------------------
				strSkillItemValues[8] = strSkillDesc;//pSkill->GetDesc()
				pNatureSkillItem = CreateXMLItem( pRoleSkillsItem, pNatureSkillTypeItem, pNatureSkillItem, strSkillItemValues, 9 );

				//!!!!!!!!!!注意：最后清除资源!!!!!!!!!!
				delete pSkill;
				if ( !pNatureSkillItem )
				{
					a_LogOutput( 1, "error in line %d", __LINE__);
					a_LogOutput( 1, "Create nature skill item Failed! SkillID=%d\n", natureSkillIDList[k] );
					ReleaseXMLItems( pRoleSkillsItem );
					return NULL;
				}
			}
		}

	}

	//神通技能 第二层子节点
	ACString strGodSkillsItem[3] = { _AL("gods_property"), _AL("name"), _AL("神通") };
	AXMLItem* pGodSkillsItem = CreateXMLItem( pRoleSkillsItem, pRoleSkillsItem, pNaturePropertyItem, strGodSkillsItem, 3 );
	if ( !pGodSkillsItem )
	{
		return NULL;
	}

	//神通技能子节点， 第三层
	ACString strDeityValues[5] = { _AL("deity_value"), _AL("name"), _AL("元神修为"), _AL("value"), _AL("0") };
	strDeityValues[4].Format( _AL("%d"), pDetailInfo->gsroleinfo.deityinfo.deity_exp );
	AXMLItem* pDeityValueItem = CreateXMLItem( pRoleSkillsItem, pGodSkillsItem, NULL, strDeityValues, 5 );
	if ( !pDeityValueItem )
	{
		return NULL;
	}

	//处理具体各个神通技能
	const int nDeitySkillTypeCount = 3;
	ACString strDeitySkillTypeValues[nDeitySkillTypeCount][5] =	{
																	{ _AL("taoism"), _AL("name"), _AL("天道"), _AL("count"), _AL("0") },
																	{ _AL("evil"), _AL("name"), _AL("血煞"), _AL("count"), _AL("0") },
																	{ _AL("buddhism"), _AL("name"), _AL("禅定"), _AL("count"), _AL("0") },
																};


	AXMLItem* pDeitySkillTypeItem = pDeityValueItem;	//!!!!!!!!!!!!!!!!!!!!
	for ( int nDeityTypeIndex = 0; nDeityTypeIndex < nDeitySkillTypeCount; ++nDeityTypeIndex )
	{
		//获得神通技能ID列表
		abase::vector<int> deitySkillIDList;
		deitySkillIDList.clear();
		int nCLSType = GetClsTypeByProfID( pDetailInfo->gsroleinfo.rolebasicinfo.cls );
		if ( nCLSType < 0 )
		{
			a_LogOutput( 1, "BuildRoleSkills consign role Profession Error = %d\n", pDetailInfo->gsroleinfo.rolebasicinfo.cls );
		}
		GetDeitySkillInfo( skillsIDList, nDeityTypeIndex, nCLSType, deitySkillIDList );
		strDeitySkillTypeValues[nDeityTypeIndex][4].Format( _AL("%d"), deitySkillIDList.size() );
		pDeitySkillTypeItem = CreateXMLItem( pRoleSkillsItem, pGodSkillsItem, pDeitySkillTypeItem, strDeitySkillTypeValues[nDeityTypeIndex], 5 );
		{
			if ( !pDeitySkillTypeItem )
			{
				return NULL;
			}
		}

		//添加具体神通技能子节点
		AXMLItem* pDeitySkillItem = NULL;
		for ( int i = 0; i < deitySkillIDList.size(); ++i )
		{
			int nLevel = skillsIDList[deitySkillIDList[i]];
			nLevel = ( nLevel ? nLevel : 0 );
			CECSkill* pSkill = new CECSkill( deitySkillIDList[i], nLevel );
			if ( !pSkill )
			{
				a_LogOutput( 1, "Create Skill Failed! SkillID=%d\n", deitySkillIDList[i] );
				ReleaseXMLItems( pRoleSkillsItem );
				return NULL;
			}

			ACString strSkillItemValues[9] = { _AL("skill"), _AL("id"), _AL(""), _AL("name"), _AL(""), _AL("icon"), _AL(""), _AL("desc"), _AL("") };
			strSkillItemValues[2].Format( _AL("%d"), deitySkillIDList[i] );
			//-------------------------------------------------------------------------特殊字符转义
			ACString strSkillName;
			TranslateStr( pSkill->GetName(), strSkillName );
			strSkillItemValues[4] = strSkillName;//pSkill->GetName()
			//-------------------------------------------------------------------------
			strSkillItemValues[6].Format( _AL("skill/%d.png"), deitySkillIDList[i] );
			//-------------------------------------------------------------------------特殊字符转义
			ACString strSkillDesc;
			//AUICTranslate trans;
			//strSkillDesc = trans.Translate( pSkill->GetDesc() );
			TranslateStr( pSkill->GetDesc(), strSkillDesc );
			//-------------------------------------------------------------------------
			strSkillItemValues[8] = strSkillDesc; //pSkill->GetDesc()

			//!!!!!!!!!!注意清理资源，防止内存泄露!!!!!!!!!!
			delete pSkill;
			pDeitySkillItem = CreateXMLItem( pRoleSkillsItem, pDeitySkillTypeItem, pDeitySkillItem, strSkillItemValues, 9 );
			if ( !pDeitySkillItem )
			{ 
				a_LogOutput( 1, "error in line %d", __LINE__);
				a_LogOutput( 1, "Create deity Skill item Failed! SkillID=%d\n", deitySkillIDList[i] );
				ReleaseXMLItems( pRoleSkillsItem );
				return NULL;
			}
		}
	}

	//师徒技能 第二层子节点
	ACString strMasterApprenticeSkills[3] = { _AL("master_apprentice_property"), _AL("name"), _AL("师徒") };
	AXMLItem* pMasterApprenticeSkillsItem = CreateXMLItem( pRoleSkillsItem, pRoleSkillsItem, pGodSkillsItem, strMasterApprenticeSkills, 3 );
	if ( !pMasterApprenticeSkillsItem )
	{
		return NULL;
	}

	//师德 第三层子节点
	ACString strMasterReputation[9] = { _AL("master_reputation"), _AL("name"), _AL("师德"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0/0"), _AL("progress"), _AL("0") };
//	abase::vector<int> reputationList;
//	GetPlayerReputationList( pDetailInfo->creditinfo.credits, reputationList );
	if ( reputationList.size() > 8 )	//判断合法性
	{
		GetReputationStringValues(
									reputationList[8],	//8:师德
									8,
									strMasterReputation[4],
									strMasterReputation[6],
									strMasterReputation[8]
								);
	}
	AXMLItem* pMasterReputationItem = CreateXMLItem( pRoleSkillsItem, pMasterApprenticeSkillsItem, NULL, strMasterReputation, 9 );
	if ( !pMasterReputationItem )
	{
		return NULL;
	}

	//师徒技能 标签
	ACString strMasterApprenticeSkillTag[3] = { _AL("master_apprentice_skill"), _AL("count"), _AL("0") };
	abase::vector<int> masterApprenticeSkillIDList;
	GetMasterApprenticeSkillInfo( skillsIDList, masterApprenticeSkillIDList );
	strMasterApprenticeSkillTag[2].Format( _AL("%d"), masterApprenticeSkillIDList.size() );
	AXMLItem* pMasterApprenticeSkillTagItem = CreateXMLItem( pRoleSkillsItem, pMasterApprenticeSkillsItem, pMasterReputationItem, strMasterApprenticeSkillTag, 3 );
	if ( !pMasterApprenticeSkillTagItem )
	{
		return NULL;
	}

	//师徒技能 具体技能子节点
	AXMLItem* pMasterApprenticeSkillItem = NULL;
	for ( int nMasterApprenticeSkillIndex = 0; nMasterApprenticeSkillIndex < masterApprenticeSkillIDList.size(); ++nMasterApprenticeSkillIndex )
	{
		int nLevel = skillsIDList[masterApprenticeSkillIDList[nMasterApprenticeSkillIndex]];
		nLevel = ( nLevel ? nLevel : 0 );
		CECSkill* pSkill = new CECSkill( masterApprenticeSkillIDList[nMasterApprenticeSkillIndex], nLevel );
		if ( !pSkill )
		{
			a_LogOutput( 1, "Create Skill Failed! SkillID=%d\n", masterApprenticeSkillIDList[nMasterApprenticeSkillIndex] );
			ReleaseXMLItems( pRoleSkillsItem );
			return NULL;
		}

		ACString strSkillItemValues[9] = { _AL("skill"), _AL("id"), _AL(""), _AL("name"), _AL(""), _AL("icon"), _AL(""), _AL("desc"), _AL("") };
		strSkillItemValues[2].Format( _AL("%d"), masterApprenticeSkillIDList[nMasterApprenticeSkillIndex] );
		//-------------------------------------------------------------------------特殊字符转义
		ACString strSkillName;
		TranslateStr( pSkill->GetName(), strSkillName );
		strSkillItemValues[4] = strSkillName;//pSkill->GetName()
		//-------------------------------------------------------------------------
		strSkillItemValues[6].Format( _AL("skill/%d.png"), masterApprenticeSkillIDList[nMasterApprenticeSkillIndex] );
		//-------------------------------------------------------------------------特殊字符转义
		ACString strSkillDesc;
		//AUICTranslate trans;
		//strSkillDesc = trans.Translate( pSkill->GetDesc() );
		TranslateStr( pSkill->GetDesc(), strSkillDesc );
		//-------------------------------------------------------------------------
		strSkillItemValues[8] = strSkillDesc;	//pSkill->GetDesc()
		pMasterApprenticeSkillItem = CreateXMLItem( pRoleSkillsItem, pMasterApprenticeSkillTagItem, pMasterApprenticeSkillItem, strSkillItemValues, 9 );

		//!!!!!!!!!!注意清理资源，防止内存泄露!!!!!!!!!!
		delete pSkill;
		if ( !pMasterApprenticeSkillItem )
		{
			a_LogOutput( 1, "error in line %d", __LINE__);
			a_LogOutput( 1, "Create Skill item Failed! SkillID=%d\n", masterApprenticeSkillIDList[nMasterApprenticeSkillIndex] );
			ReleaseXMLItems( pRoleSkillsItem );
			return NULL;
		}
	}

	//其他技能 第二层子节点
	ACString strOtherSkills[3] = { _AL("other_skills"), _AL("name"), _AL("其他") };
	AXMLItem* pOtherSkillsItem = CreateXMLItem( pRoleSkillsItem, pRoleSkillsItem, pMasterApprenticeSkillsItem, strOtherSkills, 3 );
	if ( !pOtherSkillsItem )
	{
		return NULL;
	}

	const int MAX_OTHER_SKILLS_TYPE = 3;
	ACString strOtherSkillsType[MAX_OTHER_SKILLS_TYPE][5] =	{
																{ _AL("catch_pet"), _AL("name"), _AL("捕捉宠物"), _AL("count"), _AL("0") },
																{ _AL("product_profession"), _AL("name"), _AL("生产专精"), _AL("count"), _AL("0") },
																{ _AL("special_skill"), _AL("name"), _AL("特技"), _AL("count"), _AL("0") },
															};
	AXMLItem* pOtherSkillTypeItem = NULL;
	for ( int nOtherSkillTypeIndex = 0; nOtherSkillTypeIndex < MAX_OTHER_SKILLS_TYPE; ++nOtherSkillTypeIndex )
	{
		//获得技能个数
		abase::vector<int> otherSkillIDList;
		otherSkillIDList.clear();
		GetOtherSkillInfo( skillsIDList, nOtherSkillTypeIndex, otherSkillIDList );
		strOtherSkillsType[nOtherSkillTypeIndex][4].Format( _AL("%d"), otherSkillIDList.size() );
		pOtherSkillTypeItem = CreateXMLItem( pRoleSkillsItem, pOtherSkillsItem, pOtherSkillTypeItem, strOtherSkillsType[nOtherSkillTypeIndex], 5 );
		if ( !pOtherSkillTypeItem )
		{
			return NULL;
		}

		//创建具体技能子节点
		AXMLItem* pOtherSkillItem = NULL;
		for ( int i = 0; i < otherSkillIDList.size(); ++i )
		{
			int nLevel = skillsIDList[otherSkillIDList[i]];
			nLevel = ( nLevel ? nLevel : 0 );
			CECSkill* pSkill = new CECSkill( otherSkillIDList[i], nLevel );
			if ( !pSkill )
			{
				a_LogOutput( 1, "Create Skill Failed! SkillID=%d\n", otherSkillIDList[i] );
				ReleaseXMLItems( pRoleSkillsItem );
				return NULL;
			}

			ACString strSkillItemValues[9] = { _AL("skill"), _AL("id"), _AL(""), _AL("name"), _AL(""), _AL("icon"), _AL(""), _AL("desc"), _AL("") };
			strSkillItemValues[2].Format( _AL("%d"), otherSkillIDList[i] );
			//-------------------------------------------------------------------------特殊字符转义
			ACString strSkillName;
			TranslateStr( pSkill->GetName(), strSkillName );
			strSkillItemValues[4] = strSkillName;//pSkill->GetName()
			//-------------------------------------------------------------------------
			strSkillItemValues[6].Format( _AL("skill/%d.png"), otherSkillIDList[i] );
			//-------------------------------------------------------------------------特殊字符转义
			ACString strSkillDesc;
			//AUICTranslate trans;
			//strSkillDesc = trans.Translate( pSkill->GetDesc() );
			TranslateStr( pSkill->GetDesc(), strSkillDesc );
			//-------------------------------------------------------------------------
			strSkillItemValues[8] = strSkillDesc;	//pSkill->GetDesc()

			//!!!!!!!!!!注意清理资源，防止内存泄露!!!!!!!!!!
			delete pSkill;
			pOtherSkillItem = CreateXMLItem( pRoleSkillsItem, pOtherSkillTypeItem, pOtherSkillItem, strSkillItemValues, 9 );
			if ( !pOtherSkillItem )
			{
				a_LogOutput( 1, "error in line %d", __LINE__);
				a_LogOutput( 1, "Create Skill Failed! SkillID=%d\n", otherSkillIDList[i] );
				ReleaseXMLItems( pRoleSkillsItem );
				return NULL;
			}
		}
	}

	//最后返回技能根节点
	return pRoleSkillsItem;
}

AXMLItem* CECRoleDescXMLParser::BuildRoleSkillsXML( GNET::GSNSRole* pDetailInfo )
{
	//判断参数合法性
	if ( !pDetailInfo )
	{
		a_LogOutput(1, "BuildRoleSkillsXML: Invalid parameter!");
		return NULL;
	}
	
	//技能 第一层子节点
	ACString strRoleSkillsItem[3] = { _AL("skills"), _AL("name"), _AL("技能") };
	AXMLItem* pRoleSkillsItem = CreateXMLItem( NULL, NULL, NULL, strRoleSkillsItem, 3 );
	if ( !pRoleSkillsItem )
	{
		return NULL;
	}

	//获取相应属性值
	abase::hash_map<int, int> skillsIDList;
	skillsIDList.clear();
	GetSkillsInfo( pDetailInfo->gsroleinfo.skillinfo.skills, skillsIDList );
	abase::vector<int> reputationList;
	reputationList.clear();
	GetPlayerReputationList( pDetailInfo->creditinfo.credits, reputationList );
	int nCLSType = GetClsTypeByProfID( pDetailInfo->gsroleinfo.rolebasicinfo.cls );
	if ( nCLSType < 0 )
	{
		a_LogOutput( 1, "BuildRoleSkills GSNSRole Profession Error = %d\n", pDetailInfo->gsroleinfo.rolebasicinfo.cls );
	}

	//门派技能天书 第二层子节点
	ACString strProfPropertyItem[3] = { _AL("prof_property"), _AL("name"), _AL("门派技能天书") };
	AXMLItem* pProfPropertyItem = CreateXMLItem( pRoleSkillsItem, pRoleSkillsItem, NULL, strProfPropertyItem, 3 );
	if ( !pProfPropertyItem )
	{
		return NULL;
	}
	if(!BuildSkillProfItem(pRoleSkillsItem, pProfPropertyItem, nCLSType, skillsIDList))
	{
		ReleaseXMLItems(pRoleSkillsItem);
		return NULL;
	}

	//夫妻技能 第二层子节点
	ACString strSpousePropertyItem[3] = { _AL("spouse_property"), _AL("name"), _AL("夫妻") };
	AXMLItem* pSpousePropertyItem = CreateXMLItem( pRoleSkillsItem, pRoleSkillsItem, pProfPropertyItem, strSpousePropertyItem, 3);
	if ( !pSpousePropertyItem )
	{
		return NULL;
	}
	if(!BuildSkillSpouseItem(pRoleSkillsItem, pSpousePropertyItem, skillsIDList))
	{
		ReleaseXMLItems(pRoleSkillsItem);
		return NULL;
	}




	// add sns end



	//造化技能 第二层子节点
	ACString strNaturePropertyItem[3] = { _AL("nature_property"), _AL("name"), _AL("造化") };
	AXMLItem* pNaturePropertyItem = CreateXMLItem( pRoleSkillsItem, pRoleSkillsItem, pSpousePropertyItem, strNaturePropertyItem, 3 );
	if ( !pNaturePropertyItem )
	{
		return NULL;
	}
	if(!BuildSkillNatureTypeItem(pRoleSkillsItem, pNaturePropertyItem, nCLSType, skillsIDList, reputationList))
	{
		ReleaseXMLItems(pRoleSkillsItem);
		return NULL;
	}

	//神通技能 第二层子节点
	ACString strGodSkillsItem[3] = { _AL("gods_property"), _AL("name"), _AL("神通") };
	AXMLItem* pGodSkillsItem = CreateXMLItem( pRoleSkillsItem, pRoleSkillsItem, pNaturePropertyItem, strGodSkillsItem, 3 );
	if ( !pGodSkillsItem )
	{
		return NULL;
	}
	if(!BuildSkillGodSkillItem(pRoleSkillsItem, pGodSkillsItem, nCLSType, skillsIDList, pDetailInfo->gsroleinfo.deityinfo.deity_exp))
	{
		ReleaseXMLItems(pRoleSkillsItem);
		return NULL;
	}


	//师徒技能 第二层子节点
	ACString strMasterApprenticeSkills[3] = { _AL("master_apprentice_property"), _AL("name"), _AL("师徒") };
	AXMLItem* pMasterApprenticeSkillsItem = CreateXMLItem( pRoleSkillsItem, pRoleSkillsItem, pGodSkillsItem, strMasterApprenticeSkills, 3 );
	if ( !pMasterApprenticeSkillsItem )
	{
		return NULL;
	}
	if(!BuildSkillMaster(pRoleSkillsItem, pMasterApprenticeSkillsItem, nCLSType, skillsIDList, reputationList))
	{
		ReleaseXMLItems(pRoleSkillsItem);
		return NULL;
	}

	//其他技能 第二层子节点
	ACString strOtherSkills[3] = { _AL("other_skills"), _AL("name"), _AL("其他") };
	AXMLItem* pOtherSkillsItem = CreateXMLItem( pRoleSkillsItem, pRoleSkillsItem, pMasterApprenticeSkillsItem, strOtherSkills, 3 );
	if ( !pOtherSkillsItem )
	{
		return NULL;
	}
	if(!BuildSkillOther(pRoleSkillsItem, pOtherSkillsItem, nCLSType, skillsIDList))
	{
		ReleaseXMLItems(pRoleSkillsItem);
		return NULL;
	}

	//最后返回技能根节点
	return pRoleSkillsItem;
}

bool CECRoleDescXMLParser::BuildSkillNatureTypeItem( AXMLItem* pRootItem, AXMLItem* pParentItem, int iCLS, abase::hash_map<int, int>& skillsIDList, abase::vector<int> reputationList)
{
	//造化技能具体子节点
	const int NATURE_TYPE_COUNT = 3;	// 造化子类： 道术，魔劫，佛法
	const int NATURE_SKILL_TYPE_COUNT = 3; //基本技能, 通用技能, 门派技能
	ACString strNatureType[NATURE_TYPE_COUNT][3] =	{
														{ _AL("taoism"), _AL("name"), _AL("道术") },
														{ _AL("evil"), _AL("name"), _AL("魔劫") },
														{ _AL("buddhism"), _AL("name"), _AL("佛法") },
													};

	ACString strNatureValue[NATURE_TYPE_COUNT][9] =	{
														{ _AL("taoism_value"), _AL("name"), _AL("道心值"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
														{ _AL("evil_value"), _AL("name"), _AL("魔性值"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
														{ _AL("buddhism_value"), _AL("name"), _AL("佛缘值"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0"), _AL("progress"), _AL("0") },
													};

	ACString strNatureSkillType[NATURE_SKILL_TYPE_COUNT][5] =	{
																	{ _AL("basic_skill"), _AL("name"), _AL("基本技能"), _AL("count"), _AL("0") },
																	{ _AL("generic_skill"), _AL("name"), _AL("通用技能"), _AL("count"), _AL("0") },
																	{ _AL("sect_skill"), _AL("name"), _AL("门派技能"), _AL("count"), _AL("0") },
																};

	const int nReputationIndex[NATURE_TYPE_COUNT] = { 9, 10, 11 };

	//
	AXMLItem* pNatureTypeItem = NULL;
	for ( int i = 0; i < NATURE_TYPE_COUNT; ++i )	//处理造化子类
	{
		pNatureTypeItem = CreateXMLItem( pRootItem, pParentItem, pNatureTypeItem, strNatureType[i], 3 );
		if ( !pNatureTypeItem )
		{
			return false;
		}

		//插入造化子类属性子节点
		//TODO:修改相关值
		GetReputationStringValues( reputationList[nReputationIndex[i]], nReputationIndex[i], strNatureValue[i][4], strNatureValue[i][6], strNatureValue[i][8] );
		AXMLItem* pNatureValueItem = CreateXMLItem( pRootItem, pNatureTypeItem, NULL, strNatureValue[i], 9 );
		if ( !pNatureValueItem )
		{
			return false;
		}

		//再插入造化技能子节点
		AXMLItem* pNatureSkillTypeItem = pNatureValueItem;	//!!!!!!注意处理方式:为了循环方便!!!!!!
		for ( int j = 0; j < NATURE_SKILL_TYPE_COUNT; ++j )
		{
			//TODO:数量值？
			abase::vector<int> natureSkillIDList;
			natureSkillIDList.clear();
			GetNatureSkillInfo( skillsIDList, i, j, iCLS, natureSkillIDList );
			strNatureSkillType[j][4].Format( _AL("%d"), natureSkillIDList.size() );
			pNatureSkillTypeItem = CreateXMLItem( pRootItem, pNatureTypeItem, pNatureSkillTypeItem, strNatureSkillType[j], 5 );
			if ( !pNatureSkillTypeItem )
			{
				return false;
			}
			
			//创建该类技能的具体造化技能子节点
			AXMLItem* pNatureSkillItem = NULL;
			for ( int k = 0; k < natureSkillIDList.size(); ++k )
			{
				int nLevel = skillsIDList[natureSkillIDList[k]];
				nLevel = ( nLevel ? nLevel : 0 );
				CECSkill* pSkill = new CECSkill(natureSkillIDList[k], nLevel);
				if ( !pSkill )
				{
					a_LogOutput( 1, "Create Skill Failed! SkillID=%d\n", natureSkillIDList[k] );
					return false;
				}
				ACString strSkillItemValues[9] = { _AL("skill"), _AL("id"), _AL(""), _AL("name"), _AL(""), _AL("icon"), _AL(""), _AL("desc"), _AL("") };
				strSkillItemValues[2].Format( _AL("%d"), natureSkillIDList[k] );
				//-------------------------------------------------------------------------特殊字符转义
				ACString strSkillName;
				TranslateStr( pSkill->GetName(), strSkillName );
				strSkillItemValues[4] = strSkillName;//pSkill->GetName()
				//-------------------------------------------------------------------------
				strSkillItemValues[6].Format( _AL("skill/%d.png"), natureSkillIDList[k] );
				//-------------------------------------------------------------------------特殊字符转义
				ACString strSkillDesc;
				//AUICTranslate trans;
				//strSkillDesc = trans.Translate( pSkill->GetDesc() );
				TranslateStr( pSkill->GetDesc(), strSkillDesc );
				//-------------------------------------------------------------------------
				strSkillItemValues[8] = strSkillDesc;//pSkill->GetDesc()

				//!!!!!!!!!!注意：最后清除资源!!!!!!!!!!
				delete pSkill;
				pNatureSkillItem = CreateXMLItem( pRootItem, pNatureSkillTypeItem, pNatureSkillItem, strSkillItemValues, 9 );
				if ( !pNatureSkillItem )
				{
					a_LogOutput( 1, "error in line %d", __LINE__);
					a_LogOutput( 1, "Create Skill Failed! SkillID=%d\n", natureSkillIDList[k] );
					return false;
				}
			}
		}

	}
	return true;
}

bool CECRoleDescXMLParser::BuildSkillGodSkillItem( AXMLItem* pRootItem, AXMLItem* pParentItem, int iCLS, abase::hash_map<int, int>& skillsIDList, int iDeityExp)
{
	//神通技能子节点， 第三层
	ACString strDeityValues[5] = { _AL("deity_value"), _AL("name"), _AL("元神修为"), _AL("value"), _AL("0") };
	strDeityValues[4].Format( _AL("%d"), iDeityExp );
	AXMLItem* pDeityValueItem = CreateXMLItem( pRootItem, pParentItem, NULL, strDeityValues, 5 );
	if ( !pDeityValueItem )
	{
		return false;
	}

	//处理具体各个神通技能
	const int nDeitySkillTypeCount = 3;
	ACString strDeitySkillTypeValues[nDeitySkillTypeCount][5] =	{
																	{ _AL("taoism"), _AL("name"), _AL("天道"), _AL("count"), _AL("0") },
																	{ _AL("evil"), _AL("name"), _AL("血煞"), _AL("count"), _AL("0") },
																	{ _AL("buddhism"), _AL("name"), _AL("禅定"), _AL("count"), _AL("0") },
																};


	AXMLItem* pDeitySkillTypeItem = pDeityValueItem;	//!!!!!!!!!!!!!!!!!!!!
	for ( int nDeityTypeIndex = 0; nDeityTypeIndex < nDeitySkillTypeCount; ++nDeityTypeIndex )
	{
		//获得神通技能ID列表
		abase::vector<int> deitySkillIDList;
		deitySkillIDList.clear();
		GetDeitySkillInfo( skillsIDList, nDeityTypeIndex, iCLS, deitySkillIDList );
		strDeitySkillTypeValues[nDeityTypeIndex][4].Format( _AL("%d"), deitySkillIDList.size() );
		pDeitySkillTypeItem = CreateXMLItem( pRootItem, pParentItem, pDeitySkillTypeItem, strDeitySkillTypeValues[nDeityTypeIndex], 5 );
		{
			if ( !pDeitySkillTypeItem )
			{
				return false;
			}
		}

		//添加具体神通技能子节点
		AXMLItem* pDeitySkillItem = NULL;
		for ( int i = 0; i < deitySkillIDList.size(); ++i )
		{
			int nLevel = skillsIDList[deitySkillIDList[i]];
			nLevel = ( nLevel ? nLevel : 0 );
			CECSkill* pSkill = new CECSkill( deitySkillIDList[i], nLevel );
			if ( !pSkill )
			{
				a_LogOutput( 1, "Create Skill Failed! SkillID=%d\n", deitySkillIDList[i] );
				return false;
			}

			ACString strSkillItemValues[9] = { _AL("skill"), _AL("id"), _AL(""), _AL("name"), _AL(""), _AL("icon"), _AL(""), _AL("desc"), _AL("") };
			strSkillItemValues[2].Format( _AL("%d"), deitySkillIDList[i] );
			//-------------------------------------------------------------------------特殊字符转义
			ACString strSkillName;
			TranslateStr( pSkill->GetName(), strSkillName );
			strSkillItemValues[4] = strSkillName;//pSkill->GetName()
			//-------------------------------------------------------------------------
			strSkillItemValues[6].Format( _AL("skill/%d.png"), deitySkillIDList[i] );
			//-------------------------------------------------------------------------特殊字符转义
			ACString strSkillDesc;
			//AUICTranslate trans;
			//strSkillDesc = trans.Translate( pSkill->GetDesc() );
			TranslateStr( pSkill->GetDesc(), strSkillDesc );
			//-------------------------------------------------------------------------
			strSkillItemValues[8] = strSkillDesc; //pSkill->GetDesc()

			//!!!!!!!!!!注意清理资源，防止内存泄露!!!!!!!!!!
			delete pSkill;
			pDeitySkillItem = CreateXMLItem( pRootItem, pDeitySkillTypeItem, pDeitySkillItem, strSkillItemValues, 9 );
			if ( !pDeitySkillItem )
			{
				a_LogOutput( 1, "error in line %d", __LINE__);
				a_LogOutput( 1, "Create Skill Failed! SkillID=%d\n", deitySkillIDList[i] );
				return false;
			}
		}
	}
	return true;
}

bool CECRoleDescXMLParser::BuildSkillMaster( AXMLItem* pRootItem, AXMLItem* pParentItem, int iCLS, abase::hash_map<int, int>& skillsIDList, abase::vector<int> reputationList)
{
	//师德 第三层子节点
	ACString strMasterReputation[9] = { _AL("master_reputation"), _AL("name"), _AL("师德"), _AL("level"), _AL("碌碌无名"), _AL("value"), _AL("0/0"), _AL("progress"), _AL("0") };
//	abase::vector<int> reputationList;
//	GetPlayerReputationList( pDetailInfo->creditinfo.credits, reputationList );
	if ( reputationList.size() > 8 )	//判断合法性
	{
		GetReputationStringValues(
									reputationList[8],	//8:师德
									8,
									strMasterReputation[4],
									strMasterReputation[6],
									strMasterReputation[8]
								);
	}
	AXMLItem* pMasterReputationItem = CreateXMLItem( pRootItem, pParentItem, NULL, strMasterReputation, 9 );
	if ( !pMasterReputationItem )
	{
		return false;
	}

	//师徒技能 标签
	ACString strMasterApprenticeSkillTag[3] = { _AL("master_apprentice_skill"), _AL("count"), _AL("0") };
	abase::vector<int> masterApprenticeSkillIDList;
	GetMasterApprenticeSkillInfo( skillsIDList, masterApprenticeSkillIDList );
	strMasterApprenticeSkillTag[2].Format( _AL("%d"), masterApprenticeSkillIDList.size() );
	AXMLItem* pMasterApprenticeSkillTagItem = CreateXMLItem( pRootItem, pParentItem, pMasterReputationItem, strMasterApprenticeSkillTag, 3 );
	if ( !pMasterApprenticeSkillTagItem )
	{
		return false;
	}

	//师徒技能 具体技能子节点
	AXMLItem* pMasterApprenticeSkillItem = NULL;
	for ( int nMasterApprenticeSkillIndex = 0; nMasterApprenticeSkillIndex < masterApprenticeSkillIDList.size(); ++nMasterApprenticeSkillIndex )
	{
		int nLevel = skillsIDList[masterApprenticeSkillIDList[nMasterApprenticeSkillIndex]];
		nLevel = ( nLevel ? nLevel : 0 );
		CECSkill* pSkill = new CECSkill( masterApprenticeSkillIDList[nMasterApprenticeSkillIndex], nLevel );
		if ( !pSkill )
		{
			a_LogOutput( 1, "Create Skill Failed! SkillID=%d\n", masterApprenticeSkillIDList[nMasterApprenticeSkillIndex] );
			return false;
		}

		ACString strSkillItemValues[9] = { _AL("skill"), _AL("id"), _AL(""), _AL("name"), _AL(""), _AL("icon"), _AL(""), _AL("desc"), _AL("") };
		strSkillItemValues[2].Format( _AL("%d"), masterApprenticeSkillIDList[nMasterApprenticeSkillIndex] );
		//-------------------------------------------------------------------------特殊字符转义
		ACString strSkillName;
		TranslateStr( pSkill->GetName(), strSkillName );
		strSkillItemValues[4] = strSkillName;//pSkill->GetName()
		//-------------------------------------------------------------------------
		strSkillItemValues[6].Format( _AL("skill/%d.png"), masterApprenticeSkillIDList[nMasterApprenticeSkillIndex] );
		//-------------------------------------------------------------------------特殊字符转义
		ACString strSkillDesc;
		//AUICTranslate trans;
		//strSkillDesc = trans.Translate( pSkill->GetDesc() );
		TranslateStr( pSkill->GetDesc(), strSkillDesc );
		//-------------------------------------------------------------------------
		strSkillItemValues[8] = strSkillDesc;	//pSkill->GetDesc()

		//!!!!!!!!!!注意清理资源，防止内存泄露!!!!!!!!!!
		delete pSkill;
		pMasterApprenticeSkillItem = CreateXMLItem( pRootItem, pParentItem, pMasterApprenticeSkillItem, strSkillItemValues, 9 );
		if ( !pMasterApprenticeSkillItem )
		{
			a_LogOutput( 1, "error in line %d", __LINE__);
			a_LogOutput( 1, "Create Skill Failed! SkillID=%d\n", masterApprenticeSkillIDList[nMasterApprenticeSkillIndex] );
			return false;
		}
	}

	return true;
}

bool CECRoleDescXMLParser::BuildSkillOther( AXMLItem* pRootItem, AXMLItem* pParentItem, int iCLS, abase::hash_map<int, int>& skillsIDList)
{
	const int MAX_OTHER_SKILLS_TYPE = 3;
	ACString strOtherSkillsType[MAX_OTHER_SKILLS_TYPE][5] =	{
																{ _AL("catch_pet"), _AL("name"), _AL("捕捉宠物"), _AL("count"), _AL("0") },
																{ _AL("product_profession"), _AL("name"), _AL("生产专精"), _AL("count"), _AL("0") },
																{ _AL("special_skill"), _AL("name"), _AL("特技"), _AL("count"), _AL("0") },
															};
	AXMLItem* pOtherSkillTypeItem = NULL;
	for ( int nOtherSkillTypeIndex = 0; nOtherSkillTypeIndex < MAX_OTHER_SKILLS_TYPE; ++nOtherSkillTypeIndex )
	{
		//获得技能个数
		abase::vector<int> otherSkillIDList;
		otherSkillIDList.clear();
		GetOtherSkillInfo( skillsIDList, nOtherSkillTypeIndex, otherSkillIDList );
		strOtherSkillsType[nOtherSkillTypeIndex][4].Format( _AL("%d"), otherSkillIDList.size() );
		pOtherSkillTypeItem = CreateXMLItem( pRootItem, pParentItem, pOtherSkillTypeItem, strOtherSkillsType[nOtherSkillTypeIndex], 5 );
		if ( !pOtherSkillTypeItem )
		{
			return false;
		}

		//创建具体技能子节点
		AXMLItem* pOtherSkillItem = NULL;
		for ( int i = 0; i < otherSkillIDList.size(); ++i )
		{
			int nLevel = skillsIDList[otherSkillIDList[i]];
			nLevel = ( nLevel ? nLevel : 0 );
			CECSkill* pSkill = new CECSkill( otherSkillIDList[i], nLevel );
			if ( !pSkill )
			{
				a_LogOutput( 1, "Create Skill Failed! SkillID=%d\n", otherSkillIDList[i] );
				return false;
			}

			ACString strSkillItemValues[9] = { _AL("skill"), _AL("id"), _AL(""), _AL("name"), _AL(""), _AL("icon"), _AL(""), _AL("desc"), _AL("") };
			strSkillItemValues[2].Format( _AL("%d"), otherSkillIDList[i] );
			//-------------------------------------------------------------------------特殊字符转义
			ACString strSkillName;
			TranslateStr( pSkill->GetName(), strSkillName );
			strSkillItemValues[4] = strSkillName;//pSkill->GetName()
			//-------------------------------------------------------------------------
			strSkillItemValues[6].Format( _AL("skill/%d.png"), otherSkillIDList[i] );
			//-------------------------------------------------------------------------特殊字符转义
			ACString strSkillDesc;
			//AUICTranslate trans;
			//strSkillDesc = trans.Translate( pSkill->GetDesc() );
			TranslateStr( pSkill->GetDesc(), strSkillDesc );
			//-------------------------------------------------------------------------
			strSkillItemValues[8] = strSkillDesc;	//pSkill->GetDesc()

			//!!!!!!!!!!注意清理资源，防止内存泄露!!!!!!!!!!
			delete pSkill;
			pOtherSkillItem = CreateXMLItem( pRootItem, pOtherSkillTypeItem, pOtherSkillItem, strSkillItemValues, 9 );
			if ( !pOtherSkillItem )
			{
				a_LogOutput( 1, "error in line %d", __LINE__);
				a_LogOutput( 1, "Create Skill Failed! SkillID=%d\n", otherSkillIDList[i] );
				return false;
			}
		}
	}

	return true;
}

bool CECRoleDescXMLParser::BuildSkillProfItem( AXMLItem* pRootItem, AXMLItem* pParentItem, int iCLS, abase::hash_map<int, int>& skillsIDList)
{
	//门派技能
	const int PROF_LEVEL_COUNT = 5;	// 职业重数
	//const int 
	const int PROF_SKILL_COUNT = 20; //基本技能, 通用技能, 门派技能
	ACString strProfType[3] = { _AL("prof"), _AL("name"), _AL("门派技能") };
	ACString strProfSubInfo[3] = { _AL("第n重"), _AL("重数"), _AL("0") };
	ACString strProfTalentSubInfo[3] = { _AL("第n类"), _AL("类型"), _AL("0") };
	AXMLItem* pProfItem = CreateXMLItem( pRootItem, pParentItem, NULL, strProfType, 3 );
	abase::vector<int> profSkillIDList;
	profSkillIDList.clear();
	AXMLItem* pProfSubItem = NULL;
	int i;
	for(i=0; i<ms_iProfSkillLvCount; ++i)
	{
		strProfSubInfo[2].Format(_AL("%d"), i);
		pProfSubItem = CreateXMLItem( pRootItem, pParentItem, pProfSubItem, strProfSubInfo, 3 );
		GetProfSkillInfo(skillsIDList, iCLS, i, profSkillIDList);
		AXMLItem* pProfSkillItem = NULL;
		int j;
		for(j=0; j<profSkillIDList.size(); ++j)
		{
			int nLevel = skillsIDList[profSkillIDList[j]];
			nLevel = ( nLevel ? nLevel : 0 );
			pProfSkillItem = CreateSkillXMLItem(pRootItem, pProfSubItem, pProfSkillItem, profSkillIDList[j], nLevel);
			if(!pProfSkillItem)
				return false;
		}
	}
	
	//天书
	ACString strTalentType[3] = { _AL("talent"), _AL("name"), _AL("天书") };
	AXMLItem* pTalentItem = CreateXMLItem( pRootItem, pParentItem, NULL, strTalentType, 3 );
	abase::vector<int> profTalentIDList;
	profTalentIDList.clear();
	AXMLItem* pTalentSubItem = NULL;
	for(i=0; i<ms_iProfTalentLvCount; ++i)
	{
		strProfTalentSubInfo[2].Format(_AL("%d"), i+1);
		pTalentSubItem = CreateXMLItem( pRootItem, pParentItem, pTalentSubItem, strProfTalentSubInfo, 3 );
		GetTalentSkillInfo(skillsIDList, iCLS, i, profTalentIDList);
		AXMLItem* pProfSkillItem = NULL;
		int j;
		for(j=0; j<profTalentIDList.size(); ++j)
		{
			int nLevel = skillsIDList[profTalentIDList[j]];
			nLevel = ( nLevel ? nLevel : 0 );
			pProfSkillItem = CreateSkillXMLItem(pRootItem, pProfSubItem, pProfSkillItem, profTalentIDList[j], nLevel);
			if(!pProfSkillItem)
				return false;
		}
	}

	return true;
}

bool CECRoleDescXMLParser::BuildSkillSpouseItem( AXMLItem* pRootItem, AXMLItem* pParentItem, abase::hash_map<int, int>& skillsIDList)
{
	ACString strSpouseSkill[5] = { _AL("spouse_skill"), _AL("name"), _AL("夫妻技能"), _AL("count"), _AL("0") };
	AXMLItem *pSpouseSkillRoot = CreateXMLItem(pRootItem, pParentItem, NULL, strSpouseSkill, 5);
	if(!pSpouseSkillRoot)
		return false;
	static const int siSpouseSkillsMax = 5;
	static const int iSpouseSkills[siSpouseSkillsMax] = {4877,4878,4879,4880,5227};
	AXMLItem* pSpouseSkillItem = NULL;
	int i;
	for(i=0; i<siSpouseSkillsMax; ++i)
	{
		if(skillsIDList[iSpouseSkills[i]]==0)
			continue;

		pSpouseSkillItem = CreateSkillXMLItem(pRootItem, pSpouseSkillRoot, pSpouseSkillItem, iSpouseSkills[i], skillsIDList[iSpouseSkills[i]]);
		if(!pSpouseSkillItem)
			return false;
	}
	return true;
}

AXMLItem* CECRoleDescXMLParser::CreateSkillXMLItem( AXMLItem* pRootItem, AXMLItem* pParentItem, AXMLItem* pPrevSiblingItem, int iSkillId, int iSkillLv)
{
	CECSkill* pSkill = new CECSkill(iSkillId, iSkillLv);
	if ( !pSkill )
	{
		a_LogOutput( 1, "Create Skill Failed! SkillID=%d\n", iSkillId );
		return NULL;
	}
	ACString strSkillItemValues[9] = { _AL("skill"), _AL("id"), _AL(""), _AL("name"), _AL(""), _AL("icon"), _AL(""), _AL("desc"), _AL("") };
	strSkillItemValues[2].Format( _AL("%d"), iSkillId);
	//-------------------------------------------------------------------------特殊字符转义
	ACString strSkillName;
	TranslateStr( pSkill->GetName(), strSkillName );
	strSkillItemValues[4] = strSkillName;//pSkill->GetName()
	//-------------------------------------------------------------------------
	strSkillItemValues[6].Format( _AL("skill/%d.png"), iSkillId);
	//-------------------------------------------------------------------------特殊字符转义
	ACString strSkillDesc;
	//AUICTranslate trans;
	//strSkillDesc = trans.Translate( pSkill->GetDesc() );
	TranslateStr( pSkill->GetDesc(), strSkillDesc );
	//-------------------------------------------------------------------------
	strSkillItemValues[8] = strSkillDesc;//pSkill->GetDesc()
	AXMLItem* pSkillItem = CreateXMLItem( pRootItem, pParentItem, pPrevSiblingItem, strSkillItemValues, 9 );
	delete pSkill;
	return pSkillItem;
}


AXMLItem* CECRoleDescXMLParser::BuildRoleInventoryXML( GNET::GConsignRole* pDetailInfo )
{
	//判断参数合法性
	if ( !pDetailInfo )
	{
		a_LogOutput( 1, "Invalid parameter int Function BuildRoleInventoryXML()!" );
		return NULL;
	}

	//创建Inventory相关根节点
	ACString strRoleInventory[3] = { _AL("inventory"), _AL("name"), _AL("包裹仓库") };
	AXMLItem* pRoleInventoryItem = CreateXMLItem( NULL, NULL, NULL, strRoleInventory, 3 );
	if ( !pRoleInventoryItem )
	{
		return NULL;
	}

	const int ROLE_BAG_STRING_COUNT = 15;
	const int PET_BAG_STRING_COUNT = 9;
	const int FASHION_BAG_STRING_COUNT = 7;
	const int ROLE_STORAGE_STRING_COUNT = 15;
	const int MOUNT_WING_BAG_STRING_COUNT = 9;
	const int MAX_IVENTORY_TYPE = 5;//Modified from 4 -- 添加坐骑/飞剑包裹2012-08-09.
	ACString strRoleBag[ROLE_BAG_STRING_COUNT] =	{
													_AL("role_bag"), _AL("name"), _AL("角色包裹"), _AL("count"), _AL("0"), 
													_AL("capacity"), _AL("0"), _AL("max_count"), _AL("144"), _AL("gold"),
													_AL("0"), _AL("silver"), _AL("0"), _AL("copper"), _AL("0")
												};

	//-------------------------------------------------------------------------
/*	int nEmptySlotCount = pDetailInfo->pocketinfo.role_pocket_capacity - pDetailInfo->pocketinfo.role_pocket.size();
	if ( nEmptySlotCount < 0 )
	{
		a_LogOutput( 1, "Invalid Empty Inventory Size! nEmptySlotCount = %d\n", nEmptySlotCount );
		nEmptySlotCount = 0;
	}
*/	//-------------------------------------------------------------------------
	strRoleBag[4].Format( _AL("%d"), pDetailInfo->pocketinfo.role_pocket.size() );
	strRoleBag[6].Format( _AL("%d"), pDetailInfo->pocketinfo.role_pocket_capacity );
	strRoleBag[10].Format( _AL("%d"), pDetailInfo->pocketinfo.money / 10000 );
	strRoleBag[12].Format( _AL("%d"), (pDetailInfo->pocketinfo.money % 10000 ) / 100 );
	strRoleBag[14].Format( _AL("%d"), pDetailInfo->pocketinfo.money % 100 );
	
	ACString strPetBag[PET_BAG_STRING_COUNT] =	{
													_AL("pet_bag"), _AL("name"), _AL("宠物包裹"), _AL("count"), _AL("0"), 
													_AL("capacity"), _AL("0"), _AL("max_count"), _AL("144")
												};
	strPetBag[4].Format( _AL("%d"), pDetailInfo->pocketinfo.pet_pocket.size() );
	strPetBag[6].Format( _AL("%d"), pDetailInfo->pocketinfo.pet_pocket_capacity );

	ACString strFashionBag[FASHION_BAG_STRING_COUNT] =	{ _AL("fashion_bag"), _AL("name"), _AL("时装包裹"), _AL("max_count"), _AL("120"), _AL("count"), _AL("0") };
	strFashionBag[6].Format( _AL("%d"), pDetailInfo->pocketinfo.fashion.size() );

	ACString strRoleStorage[ROLE_STORAGE_STRING_COUNT] =	{
																_AL("role_storage"), _AL("name"), _AL("角色仓库"), _AL("count"), _AL("0"), 
																_AL("capacity"), _AL(""), _AL("max_count"), _AL("144"), _AL("gold"),
																_AL(""), _AL("silver"), _AL(""), _AL("copper"), _AL("")																
															};
	strRoleStorage[4].Format( _AL("%d"), pDetailInfo->pocketinfo.storehouse.items.size() );
	strRoleStorage[6].Format( _AL("%d"), pDetailInfo->pocketinfo.storehouse.capacity );
	strRoleStorage[10].Format( _AL("%d"), pDetailInfo->pocketinfo.storehouse.money / 10000 );
	strRoleStorage[12].Format( _AL("%d"), (pDetailInfo->pocketinfo.storehouse.money % 10000) / 100 );
	strRoleStorage[14].Format( _AL("%d"), pDetailInfo->pocketinfo.storehouse.money % 100 );

	//-------------------------------------------------------------------------
	//添加坐骑/飞剑包裹
	ACString strMountWingBag[MOUNT_WING_BAG_STRING_COUNT] = { _AL("mountwing_bag"), _AL("name"), _AL("坐骑/飞剑包裹"), _AL("max_count"), _AL("18"), _AL("capacity"), _AL("0"), _AL("count"), _AL("0") };
	strMountWingBag[6].Format( _AL("%d"), pDetailInfo->pocketinfo.mountwing_capacity );
	strMountWingBag[8].Format( _AL("%d"), pDetailInfo->pocketinfo.mountwing.size() );
	//-------------------------------------------------------------------------

	struct TAG_ITEM
	{
		int			m_nItemCount;
		ACString*	m_pStrItemValues;
	};

	TAG_ITEM tagItem[MAX_IVENTORY_TYPE] =	{
												{ ROLE_BAG_STRING_COUNT, strRoleBag }, { PET_BAG_STRING_COUNT, strPetBag },
												{ FASHION_BAG_STRING_COUNT, strFashionBag }, { ROLE_STORAGE_STRING_COUNT, strRoleStorage },
												{ MOUNT_WING_BAG_STRING_COUNT, strMountWingBag } //新增坐骑飞剑包裹
											};
	
	//单独处理宠物包裹
	GRoleInventoryVector petInventory;
	for ( int i = 0; i < pDetailInfo->pocketinfo.pet_pocket.size(); ++i )
	{
		GRoleInventory roleInventory;
		roleInventory.id = pDetailInfo->pocketinfo.pet_pocket[i].id;
		roleInventory.pos = pDetailInfo->pocketinfo.pet_pocket[i].pos;
		roleInventory.count = pDetailInfo->pocketinfo.pet_pocket[i].count;
		petInventory.add( roleInventory );
	}

	//
	GRoleInventoryVector* pRoleInventoryVector[MAX_IVENTORY_TYPE] =			{ 
																				&(pDetailInfo->pocketinfo.role_pocket), &petInventory, 
																				&(pDetailInfo->pocketinfo.fashion), &(pDetailInfo->pocketinfo.storehouse.items),
																				&(pDetailInfo->pocketinfo.mountwing) //新增坐骑飞剑包裹
																			};

	
	//处理所有Inventory子节点
	AXMLItem* pRoleBagTypeItem = NULL;
	ACString strInventoryItem[15] = { _AL("item"), _AL("id"), _AL(""), _AL("pos"), _AL(""), _AL("level"), _AL(""), _AL("count"), _AL(""), _AL("name"), _AL(""), _AL("icon"), _AL(""), _AL("desc"), _AL("") };
	for ( int j = 0; j < MAX_IVENTORY_TYPE; ++j )
	{
		//创建Inventory类别Tag子节点
		pRoleBagTypeItem = CreateXMLItem( pRoleInventoryItem, pRoleInventoryItem, pRoleBagTypeItem, tagItem[j].m_pStrItemValues, tagItem[j].m_nItemCount );
		if ( !pRoleBagTypeItem )
		{
			return NULL;
		}
		
		//创建具体物品子节点
		GRoleInventoryVector& roleInventoryVector = *(pRoleInventoryVector[j]);
		AXMLItem* pInventoryItem = NULL;
		for ( int k = 0; k < roleInventoryVector.size(); ++k )
		{
			const GRoleInventory& itemInfo = roleInventoryVector[k];
			CECIvtrItem* pItem = CECIvtrItem::CreateItem(itemInfo.id, itemInfo.expire_date, itemInfo.count);
			if ( !pItem )
			{
				a_LogOutput(1, "Failed to create Inventory Item. ID=%d", itemInfo.id);
				continue;
			}
			if ( itemInfo.data.begin() && itemInfo.data.size() )
			{
				pItem->SetItemInfo((BYTE*)itemInfo.data.begin(), itemInfo.data.size());
			}
			else
			{
				pItem->SetItemInfo(NULL, 0);
			}
			pItem->SetStateFlag(Proctype2State(itemInfo.proctype));	//New Added 2012-05-08.
			pItem->UpdateInfoMD5();
			strInventoryItem[2].Format( _AL("%d"), itemInfo.id );
			strInventoryItem[4].Format( _AL("%d"), itemInfo.pos );
			strInventoryItem[6].Format( _AL("%d"), pItem->GetEstone() );
			strInventoryItem[8].Format( _AL("%d"), itemInfo.count );
			//-------------------------------------------------------------------------特殊字符转义
			ACString strItemName;
			TranslateStr( pItem->GetName(), strItemName );
			strInventoryItem[10] = strItemName;//pItem->GetName()
			//-------------------------------------------------------------------------
			strInventoryItem[12].Format( _AL("%d.png"), itemInfo.id );
			//-------------------------------------------------------------------------特殊字符转义
			ACString strItemDesc;
			TranslateStr( pItem->GetDesc(), strItemDesc );
			strInventoryItem[14] = strItemDesc;//pItem->GetDesc()
			
			//!!!!!!注意：清理资源!!!!!!
			delete pItem;
			//-------------------------------------------------------------------------
			pInventoryItem = CreateXMLItem( pRoleInventoryItem, pRoleBagTypeItem, pInventoryItem, strInventoryItem, 15 );
			if ( !pInventoryItem )
			{
				a_LogOutput( 1, "error in line %d", __LINE__);
				return NULL;
			}
		}
	}
	
	//最后，返回创建该类型标签的根节点
	return pRoleInventoryItem;
}

AXMLItem* CECRoleDescXMLParser::BuildRoleInventoryXML( GNET::GSNSRole* pDetailInfo )
{
	//判断参数合法性
	if ( !pDetailInfo)
	{
		a_LogOutput( 1, "Invalid parameter int Function BuildRoleInventoryXML()!" );
		return NULL;
	}

	//创建Inventory相关根节点
	ACString strRoleInventory[3] = { _AL("inventory"), _AL("name"), _AL("包裹仓库") };
	AXMLItem* pRoleInventoryItem = CreateXMLItem( NULL, NULL, NULL, strRoleInventory, 3 );
	if ( !pRoleInventoryItem )
	{
		return NULL;
	}

	const int ROLE_BAG_STRING_COUNT = 15;
	const int PET_BAG_STRING_COUNT = 9;
	const int FASHION_BAG_STRING_COUNT = 7;
	const int ROLE_STORAGE_STRING_COUNT = 15;
	const int MOUNT_WING_BAG_STRING_COUNT = 9;
	const int FUWEN_STRING_COUNT = 7;
	const int MAX_IVENTORY_TYPE = 6;//0:角色包裹 1:宠物包裹 2:时装包裹 3:仓库 4:坐骑/飞剑包裹 5:命符包裹(或者叫符)
	ACString strRoleBag[ROLE_BAG_STRING_COUNT] =	{
													_AL("role_bag"), _AL("name"), _AL("角色包裹"), _AL("count"), _AL("0"), 
													_AL("capacity"), _AL("0"), _AL("max_count"), _AL("144"), _AL("gold"),
													_AL("0"), _AL("silver"), _AL("0"), _AL("copper"), _AL("0")
												};

	strRoleBag[4].Format( _AL("%d"), pDetailInfo->pocketinfo.role_pocket.size() );
	strRoleBag[6].Format( _AL("%d"), pDetailInfo->pocketinfo.role_pocket_capacity );
	strRoleBag[10].Format( _AL("%d"), pDetailInfo->pocketinfo.money / 10000 );
	strRoleBag[12].Format( _AL("%d"), (pDetailInfo->pocketinfo.money % 10000 ) / 100 );
	strRoleBag[14].Format( _AL("%d"), pDetailInfo->pocketinfo.money % 100 );
	
	ACString strPetBag[PET_BAG_STRING_COUNT] =	{
													_AL("pet_bag"), _AL("name"), _AL("宠物包裹"), _AL("count"), _AL("0"), 
													_AL("capacity"), _AL("0"), _AL("max_count"), _AL("144")
												};
	strPetBag[4].Format( _AL("%d"), pDetailInfo->pocketinfo.pet_pocket.size() );
	strPetBag[6].Format( _AL("%d"), pDetailInfo->pocketinfo.pet_pocket_capacity );

	ACString strFashionBag[FASHION_BAG_STRING_COUNT] =	{ _AL("fashion_bag"), _AL("name"), _AL("时装包裹"), _AL("max_count"), _AL("120"), _AL("count"), _AL("0") };
	strFashionBag[6].Format( _AL("%d"), pDetailInfo->pocketinfo.fashion.size() );

	ACString strRoleStorage[ROLE_STORAGE_STRING_COUNT] =	{
																_AL("role_storage"), _AL("name"), _AL("角色仓库"), _AL("count"), _AL("0"), 
																_AL("capacity"), _AL(""), _AL("max_count"), _AL("144"), _AL("gold"),
																_AL(""), _AL("silver"), _AL(""), _AL("copper"), _AL("")																
															};
	strRoleStorage[4].Format( _AL("%d"), pDetailInfo->pocketinfo.storehouse.items.size() );
	strRoleStorage[6].Format( _AL("%d"), pDetailInfo->pocketinfo.storehouse.capacity );
	strRoleStorage[10].Format( _AL("%d"), pDetailInfo->pocketinfo.storehouse.money / 10000 );
	strRoleStorage[12].Format( _AL("%d"), (pDetailInfo->pocketinfo.storehouse.money % 10000) / 100 );
	strRoleStorage[14].Format( _AL("%d"), pDetailInfo->pocketinfo.storehouse.money % 100 );

	//-------------------------------------------------------------------------
	//添加坐骑/飞剑包裹
	ACString strMountWingBag[MOUNT_WING_BAG_STRING_COUNT] = { _AL("mountwing_bag"), _AL("name"), _AL("坐骑/飞剑包裹"), _AL("max_count"), _AL("18"), _AL("capacity"), _AL("0"), _AL("count"), _AL("0") };
	strMountWingBag[6].Format( _AL("%d"), pDetailInfo->pocketinfo.mountwing_capacity );
	strMountWingBag[8].Format( _AL("%d"), pDetailInfo->pocketinfo.mountwing.size() );
	//-------------------------------------------------------------------------
	
	ACString strFuWenBag[FUWEN_STRING_COUNT] = { _AL("fuwen_bag"), _AL("name"), _AL("命符"), _AL("max_count"), _AL("6"), _AL("count"), _AL("0") };
	strFuWenBag[6].Format( _AL("%d"), pDetailInfo->pocketinfo.storehouse.fuwen.size());

	struct TAG_ITEM
	{
		int			m_nItemCount;
		ACString*	m_pStrItemValues;
	};

	TAG_ITEM tagItem[MAX_IVENTORY_TYPE] =	{
												{ ROLE_BAG_STRING_COUNT, strRoleBag }, { PET_BAG_STRING_COUNT, strPetBag },
												{ FASHION_BAG_STRING_COUNT, strFashionBag }, { ROLE_STORAGE_STRING_COUNT, strRoleStorage },
												{ MOUNT_WING_BAG_STRING_COUNT, strMountWingBag }, //新增坐骑飞剑包裹
												{ FUWEN_STRING_COUNT, strFuWenBag}, 
											};
	
	//单独处理宠物包裹
	GRoleInventoryVector petInventory;
	for ( int i = 0; i < pDetailInfo->pocketinfo.pet_pocket.size(); ++i )
	{
		GRoleInventory roleInventory;
		roleInventory.id = pDetailInfo->pocketinfo.pet_pocket[i].id;
		roleInventory.pos = pDetailInfo->pocketinfo.pet_pocket[i].pos;
		roleInventory.count = pDetailInfo->pocketinfo.pet_pocket[i].count;
		petInventory.add( roleInventory );
	}

	//
	GRoleInventoryVector* pRoleInventoryVector[MAX_IVENTORY_TYPE] =			{ 
																				&(pDetailInfo->pocketinfo.role_pocket), 
																				&petInventory, 
																				&(pDetailInfo->pocketinfo.fashion), 
																				&(pDetailInfo->pocketinfo.storehouse.items),
																				&(pDetailInfo->pocketinfo.mountwing), 
																				&(pDetailInfo->pocketinfo.storehouse.items2) //新增坐骑飞剑包裹
																			};

	
	//处理所有Inventory子节点
	AXMLItem* pRoleBagTypeItem = NULL;
	ACString strInventoryItem[15] = { _AL("item"), _AL("id"), _AL(""), _AL("pos"), _AL(""), _AL("level"), _AL(""), _AL("count"), _AL(""), _AL("name"), _AL(""), _AL("icon"), _AL(""), _AL("desc"), _AL("") };
	for ( int j = 0; j < MAX_IVENTORY_TYPE; ++j )
	{
		//创建Inventory类别Tag子节点
		pRoleBagTypeItem = CreateXMLItem( pRoleInventoryItem, pRoleInventoryItem, pRoleBagTypeItem, tagItem[j].m_pStrItemValues, tagItem[j].m_nItemCount );
		if ( !pRoleBagTypeItem )
		{
			return NULL;
		}
		
		//创建具体物品子节点
		GRoleInventoryVector& roleInventoryVector = *(pRoleInventoryVector[j]);
		AXMLItem* pInventoryItem = NULL;
		for ( int k = 0; k < roleInventoryVector.size(); ++k )
		{
			const GRoleInventory& itemInfo = roleInventoryVector[k];
			CECIvtrItem* pItem = CECIvtrItem::CreateItem(itemInfo.id, itemInfo.expire_date, itemInfo.count);
			if ( !pItem )
			{
				a_LogOutput(1, "Failed to create Inventory Item. ID=%d", itemInfo.id);
				continue;
			}
			if ( itemInfo.data.begin() && itemInfo.data.size() )
			{
				pItem->SetItemInfo((BYTE*)itemInfo.data.begin(), itemInfo.data.size());
			}
			else
			{
				pItem->SetItemInfo(NULL, 0);
			}
			pItem->SetStateFlag(Proctype2State(itemInfo.proctype));	//New Added 2012-05-08.
			pItem->UpdateInfoMD5();
			strInventoryItem[2].Format( _AL("%d"), itemInfo.id );
			strInventoryItem[4].Format( _AL("%d"), itemInfo.pos );
			strInventoryItem[6].Format( _AL("%d"), pItem->GetEstone() );
			strInventoryItem[8].Format( _AL("%d"), itemInfo.count );
			//-------------------------------------------------------------------------特殊字符转义
			ACString strItemName;
			TranslateStr( pItem->GetName(), strItemName );
			strInventoryItem[10] = strItemName;//pItem->GetName()
			//-------------------------------------------------------------------------
			strInventoryItem[12].Format( _AL("%d.png"), itemInfo.id );
			//-------------------------------------------------------------------------特殊字符转义
			ACString strItemDesc;
			TranslateStr( pItem->GetDesc(), strItemDesc );
			strInventoryItem[14] = strItemDesc;//pItem->GetDesc()
			
			//!!!!!!注意：清理资源!!!!!!
			delete pItem;
			//-------------------------------------------------------------------------
			pInventoryItem = CreateXMLItem( pRoleInventoryItem, pRoleBagTypeItem, pInventoryItem, strInventoryItem, 15 );
			if ( !pInventoryItem )
			{
				a_LogOutput( 1, "error in line %d", __LINE__);
				return NULL;
			}
		}
	}
	
	//最后，返回创建该类型标签的根节点
	return pRoleInventoryItem;
}

AXMLItem* CECRoleDescXMLParser::BuildRolePetsXML( GNET::GConsignRole* pDetailInfo )
{
	//判断参数合法性
	if ( !pDetailInfo )
	{
		a_LogOutput( 1, "Invalid Parameter!\n" );
		return NULL;
	}

	int nPetCount = pDetailInfo->petinfo.petbadge.size();
	ACString strPetsInfo[7] = { _AL("pets"), _AL("name"), _AL("宠物"), _AL("capacity"), _AL("32"), _AL("count"), _AL("") };
	strPetsInfo[6].Format( _AL("%d"), nPetCount );

	//创建宠物标签根节点
	AXMLItem* pPetsInfoItem = CreateXMLItem( NULL, NULL, NULL, strPetsInfo, 7 );
	if ( !pPetsInfoItem )
	{
		return NULL;
	}

	//创建每个具体的宠物信息节点，并插入到根节点下面
	AXMLItem* pPetItem = NULL;
	AXMLItem* pPetPrevItem = NULL;
	for ( int i = 0; i < nPetCount; ++i )
	{
		pPetItem = CreateRolePetXML( pDetailInfo, i );
		if ( !pPetItem )
		{
			ReleaseXMLItems( pPetsInfoItem );
			a_LogOutput( 1, "Create Pet item Failed!\n" );
			return NULL;
		}
		pPetItem->InsertItem( pPetsInfoItem, pPetPrevItem );
		pPetPrevItem = pPetItem;
	}

	//返回根节点
	return pPetsInfoItem;
}

AXMLItem* CECRoleDescXMLParser::BuildRolePetsXML( GNET::GSNSRole* pDetailInfo )
{
	//判断参数合法性
	if ( !pDetailInfo )
	{
		a_LogOutput( 1, "Invalid Parameter!\n" );
		return NULL;
	}

	int nPetCount = pDetailInfo->petinfo.petbadge.size();
	ACString strPetsInfo[7] = { _AL("pets"), _AL("name"), _AL("宠物"), _AL("capacity"), _AL("32"), _AL("count"), _AL("") };
	strPetsInfo[6].Format( _AL("%d"), nPetCount );

	//创建宠物标签根节点
	AXMLItem* pPetsInfoItem = CreateXMLItem( NULL, NULL, NULL, strPetsInfo, 7 );
	if ( !pPetsInfoItem )
	{
		return NULL;
	}

	//创建每个具体的宠物信息节点，并插入到根节点下面
	AXMLItem* pPetItem = NULL;
	AXMLItem* pPetPrevItem = NULL;
	for ( int i = 0; i < nPetCount; ++i )
	{
		pPetItem = CreateRolePetXML( pDetailInfo, i );
		if ( !pPetItem )
		{
			ReleaseXMLItems( pPetsInfoItem );
			a_LogOutput( 1, "Create Pet item Failed!\n" );
			return NULL;
		}
		pPetItem->InsertItem( pPetsInfoItem, pPetPrevItem );
		pPetPrevItem = pPetItem;
	}

	//返回根节点
	return pPetsInfoItem;
}

int CECRoleDescXMLParser::GetPetAddInfoIndex( GNET::GConsignRole* pDetailInfo, int nPetIndex )
{
	if ( !pDetailInfo )
	{
		a_LogOutput( 1, "Input parameter invalid!" );
		return -1;
	}

	for ( int i = 0; i < pDetailInfo->gsroleinfo.pet_addinfo.size(); ++i )
	{
		if ( (int)(pDetailInfo->gsroleinfo.pet_addinfo[i].pet_index) == nPetIndex )
		{
			return i;
		}
	}

	//
	return -1;
}

int CECRoleDescXMLParser::GetPetAddInfoIndex( GNET::GSNSRole* pDetailInfo, int nPetIndex )
{
	if ( !pDetailInfo )
	{
		a_LogOutput( 1, "Input parameter invalid!" );
		return -1;
	}

	for ( int i = 0; i < pDetailInfo->gsroleinfo.pet_addinfo.size(); ++i )
	{
		if ( (int)(pDetailInfo->gsroleinfo.pet_addinfo[i].pet_index) == nPetIndex )
		{
			return i;
		}
	}

	//
	return -1;
}

AXMLItem* CECRoleDescXMLParser::CreateRolePetXML( GNET::GSNSRole* pDetailInfo, int nPetIndex )
{
	if ( !pDetailInfo || nPetIndex < 0 || nPetIndex >= pDetailInfo->petinfo.petbadge.size() )
	{
		a_LogOutput( 1, "Input Parameters Invalid!" );
		return NULL;
	}

	//根据信息创建宠物
	const GRoleInventory& petInfo = pDetailInfo->petinfo.petbadge[nPetIndex];
	CECIvtrItem* pItem = CECIvtrItem::CreateItem(petInfo.id, petInfo.expire_date, petInfo.count);
	if ( !pItem )
	{
		a_LogOutput( 1, "Create Pet Badge Failed! PetID=%d", petInfo.id );
		return NULL;
	}
	
	//设置具体信息
	if ( petInfo.data.begin() && petInfo.data.size() )
	{
		pItem->SetItemInfo((BYTE*)petInfo.data.begin(), petInfo.data.size());
	}
	pItem->UpdateInfoMD5();

	CECPetBedge* pPet = (CECPetBedge*)pItem;

	//
	if ( !pPet || !pPet->GetEssence().identify )
	{
		a_LogOutput( 1, "Create Pet Failed! Pet Identify = %d\n", pPet->GetEssence().identify );
		return NULL;
	}

	ACString strPetInfo[5] = { _AL("pet"), _AL("name"), _AL(""), _AL("icon"), _AL("") };
	strPetInfo[4].Format( _AL("%d.png"), petInfo.id );	//不需要在head目录下？直接使用宠物牌物品下的图标？head/
	ACString strPetTransName;
	TranslateStr( pPet->GetName(), strPetTransName );
	strPetInfo[2] = strPetTransName;	//宠物名称可能需要转义pPet->GetName()	Modified 2012-05-03.
	//==========宠物头像路径如何定义？-------放到头像下面以宠物ID为检索

	//创建宠物信息根节点
	AXMLItem* pPetInfoItem = CreateXMLItem( NULL, NULL, NULL, strPetInfo, 5 );
	if ( !pPetInfoItem )
	{
		return NULL;
	}

	//宠物基本属性 第一层子节点
	ACString strPetBasicProperty[3] = { _AL("basic_property"), _AL("name"), _AL("基本属性") };
	AXMLItem* pPetBasicPropertyItem = CreateXMLItem( pPetInfoItem, pPetInfoItem, NULL, strPetBasicProperty, 3 );
	if ( !pPetBasicPropertyItem )
	{
		return NULL;
	}

	//宠物名称， 第二层子节点
	ACString strPetName[3] = { _AL("pet_name"), _AL("name"), _AL("") };
	strPetName[2] = strPetTransName;	//宠物名称可能需要转义pPet->GetName()
	AXMLItem* pPetNameItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, NULL, strPetName, 3 );
	if ( !pPetNameItem )
	{
		return NULL;
	}

	//宠物等级， 第二层子节点
	ACString strPetLevel[5] = { _AL("level"), _AL("name"), _AL("等级"), _AL("value"), _AL("") };
	strPetLevel[4].Format( _AL("%d"), pPet->GetEssence().level );
	AXMLItem* pPetLevelItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetNameItem, strPetLevel, 5 );
	if ( !pPetLevelItem )
	{
		return NULL;
	}
	
	//宠物经验，第二层子节点
	ACString strPetExp[5] = { _AL("exp"), _AL("name"), _AL("经验"), _AL("value"), _AL("--") };
	double fMaxExp = GetPetLevelUpExp( pPet->GetEssence().level + 1 );
	if ( fMaxExp > 0.0 )
	{
		strPetExp[4].Format( _AL("%.2f%%"), pPet->GetEssence().exp * 100.0 / fMaxExp );
	}
	AXMLItem* pPetExpItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetLevelItem, strPetExp, 5 );
	if ( !pPetExpItem )
	{
		return NULL;
	}

	//宠物性格 第二层子节点
	ACString strPetPersonality[5] = { _AL("personality"), _AL("name"), _AL("性格"), _AL("value"), _AL("--") };
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	if ( pDescTab )
	{
		strPetPersonality[4] = pDescTab->GetWideString( ITEMDESC_PET_ELEMENT0 + pPet->GetEssence().element );
	}
	AXMLItem* pPetPersonalityItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetExpItem, strPetPersonality, 5 );
	if ( !pPetPersonalityItem )
	{
		return NULL;
	}

	//宠物种族， 第二层子节点
	ACString strPetRace[5] = { _AL("race"), _AL("name"), _AL("种族"), _AL("value"), _AL("--") };
	if ( pDescTab )
	{
		strPetRace[4] = pDescTab->GetWideString(ITEMDESC_PET_RACE0 + pPet->GetEssence().race);
	}
	AXMLItem* pPetRaceItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetPersonalityItem, strPetRace, 5 );
	if ( !pPetRaceItem )
	{
		return NULL;
	}
	
	//宠物星象， 第二层子节点
	ACString strPetHoroscope[7] = { _AL("horoscope"), _AL("name"), _AL("星象"), _AL("value"), _AL("--"), _AL("level"), _AL("") };
	if ( pDescTab )
	{
		strPetHoroscope[4] = pDescTab->GetWideString(ITEMDESC_PET_HOROSCOPE0 + pPet->GetEssence().horoscope);
	}
	strPetHoroscope[6].Format( _AL("%d星"), pPet->GetEssence().star	 );
	AXMLItem* pPetHoroscopeItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetRaceItem, strPetHoroscope, 7 );
	if ( !pPetHoroscopeItem )
	{
		return NULL;
	}

	//宠物生命，第二层子节点
	ACString strPetHp[9] = { _AL("hp"), _AL("name"), _AL("生命"), _AL("value"), _AL("--"), _AL("attitude_name"), _AL("资质"), _AL("attitude_value"), _AL("") };
	strPetHp[4].Format( _AL("%d/%d"), pPet->GetEssence().cur_hp, pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MAX_HP].cur_value );
	//如果有附加属性，则添加
	int nPetAddonListIndex = GetPetAddInfoIndex(pDetailInfo, nPetIndex);
	if ( nPetAddonListIndex >= 0 &&
		pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].max_hp)
	{
		ACString strAddonHp;
		strAddonHp.Format( _AL("^00ff00%+d"), pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].max_hp );
		strPetHp[4] = strPetHp[4] + strAddonHp;
	}
	strPetHp[8].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MAX_HP].potential );
	AXMLItem* pPetHpItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetHoroscopeItem, strPetHp, 9 );
	if ( !pPetHpItem )
	{
		return NULL;
	}

	//宠物精力， 第二层子节点
	ACString strPetMp[9] = { _AL("mp"), _AL("name"), _AL("精力"), _AL("value"), _AL("--"), _AL("attitude_name"), _AL("资质"), _AL("attitude_value"), _AL("") };
	strPetMp[4].Format( _AL("%d/%d"), pPet->GetEssence().cur_vigor, pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MAX_VIGOR].cur_value );
	//如果有附加属性，则添加
	if ( nPetAddonListIndex >= 0 &&
		pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].max_mp)
	{
		ACString strAddonMp;
		strAddonMp.Format( _AL("^00ff00%+d"), pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].max_mp );
		strPetMp[4] = strPetMp[4] + strAddonMp;
	}
	strPetMp[8].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MAX_VIGOR].potential );
	AXMLItem* pPetMpItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetHpItem, strPetMp, 9 );
	if ( !pPetMpItem )
	{
		return NULL;
	}

	//宠物最小攻击 第二层子节点
	ACString strPetMinAttack[9] = { _AL("min_attack"), _AL("name"), _AL("最小攻击"), _AL("value"), _AL("--"), _AL("attitude_name"), _AL("资质"), _AL("attitude_value"), _AL("") };
	strPetMinAttack[4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MIN_ATTACK].cur_value );
	//如果有附加属性，则添加
	if ( nPetAddonListIndex >= 0 &&
		pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].attack)
	{
		ACString strAddonMinAttack;
		strAddonMinAttack.Format( _AL("^00ff00%+d"), pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].attack );
		strPetMinAttack[4] = strPetMinAttack[4] + strAddonMinAttack;
	}
	strPetMinAttack[8].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MIN_ATTACK].potential );
	AXMLItem* pPetMinAttackItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetMpItem, strPetMinAttack, 9 );
	if ( !pPetMinAttackItem )
	{
		return NULL;
	}

	//宠物最大攻击 第二层子节点
	ACString strPetMaxAttack[9] = { _AL("max_attack"), _AL("name"), _AL("最大攻击"), _AL("value"), _AL("--"), _AL("attitude_name"), _AL("资质"), _AL("attitude_value"), _AL("") };
	strPetMaxAttack[4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MAX_ATTACK].cur_value );
	//如果有附加属性，则添加
	if ( nPetAddonListIndex >= 0 &&
		pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].attack)
	{
		ACString strAddonMaxAttack;
		strAddonMaxAttack.Format( _AL("^00ff00%+d"), pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].attack );
		strPetMaxAttack[4] = strPetMaxAttack[4] + strAddonMaxAttack;
	}
	strPetMaxAttack[8].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MAX_ATTACK].potential );
	AXMLItem* pPetMaxAttackItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetMinAttackItem, strPetMaxAttack, 9 );
	if ( !pPetMaxAttackItem )
	{
		return NULL;
	}
	
	//宠物防御 第二层子节点
	ACString strPetDefence[9] = { _AL("defense"), _AL("name"), _AL("防御"), _AL("value"), _AL("--"), _AL("attitude_name"), _AL("资质"), _AL("attitude_value"), _AL("") };
	strPetDefence[4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::DEFENCE].cur_value );
	//如果有附加属性，则添加
	if ( nPetAddonListIndex >= 0 &&
		pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].defence)
	{
		ACString strAddonDefence;
		strAddonDefence.Format( _AL("^00ff00%+d"), pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].defence );
		strPetDefence[4] = strPetDefence[4] + strAddonDefence;
	}
	strPetDefence[8].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::DEFENCE].potential );
	AXMLItem* pPetDefenceItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetMaxAttackItem, strPetDefence, 9 );
	if ( !pPetDefenceItem )
	{
		return NULL;
	}

	//宠物命中 第二层子节点
	ACString strPetHit[9] = { _AL("hit"), _AL("name"), _AL("命中"), _AL("value"), _AL("--"), _AL("attitude_name"), _AL("资质"), _AL("attitude_value"), _AL("") };
	strPetHit[4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::HIT].cur_value );
	//如果有附加属性，则添加
	if ( nPetAddonListIndex >= 0 &&
		pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].hit)
	{
		ACString strAddonHit;
		strAddonHit.Format( _AL("^00ff00%+d"), pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].hit );
		strPetHit[4] = strPetHit[4] + strAddonHit;
	}
	strPetHit[8].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::HIT].potential );
	AXMLItem* pPetHitItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetDefenceItem, strPetHit, 9 );
	if ( !pPetHitItem )
	{
		return NULL;
	}

	//宠物闪避， 第二层子节点
	ACString strPetDodge[9] = { _AL("dodge"), _AL("name"), _AL("闪避"), _AL("value"), _AL("--"), _AL("attitude_name"), _AL("资质"), _AL("attitude_value"), _AL("") };
	strPetDodge[4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::JOUK].cur_value );
	//如果有附加属性，则添加
	if ( nPetAddonListIndex >= 0 &&
		pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].jouk)
	{
		ACString strAddonDodge;
		strAddonDodge.Format( _AL("^00ff00%+d"), pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].jouk );
		strPetDodge[4] = strPetDodge[4] + strAddonDodge;
	}
	strPetDodge[8].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::JOUK].potential );
	AXMLItem* pPetDodgeItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetHitItem, strPetDodge, 9 );
	if ( !pPetDodgeItem )
	{
		return NULL;
	}

	//宠物眩晕抗性 第二层子节点
	ACString strPetResist1[9] = { _AL("resist"), _AL("name"), _AL("眩晕抗性"), _AL("value"), _AL("--"), _AL("attitude_name"), _AL("资质"), _AL("attitude_value"), _AL("") };
	strPetResist1[4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE1].cur_value );
	//如果有附加属性，则添加
	if ( nPetAddonListIndex >= 0 &&
		pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].resistance1 )
	{
		ACString strAddonResist1;
		strAddonResist1.Format( _AL("^00ff00%+d"), pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].resistance1 );
		strPetResist1[4] = strPetResist1[4] + strAddonResist1;
	}
	strPetResist1[8].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE1].potential );
	AXMLItem* pPetResistItem1 = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetDodgeItem, strPetResist1, 9 );
	if ( !pPetResistItem1 )
	{
		return NULL;
	}

	//宠物虚弱抗性 第二层子节点
	ACString strPetResist2[9] = { _AL("resist"), _AL("name"), _AL("虚弱抗性"), _AL("value"), _AL("--"), _AL("attitude_name"), _AL("资质"), _AL("attitude_value"), _AL("") };
	strPetResist2[4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE2].cur_value );
	//如果有附加属性，则添加
	if ( nPetAddonListIndex >= 0 &&
		pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].resistance2 )
	{
		ACString strAddonResist2;
		strAddonResist2.Format( _AL("^00ff00%+d"), pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].resistance2 );
		strPetResist2[4] = strPetResist2[4] + strAddonResist2;
	}
	strPetResist2[8].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE2].potential );
	AXMLItem* pPetResistItem2 = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetResistItem1, strPetResist2, 9 );
	if ( !pPetResistItem2 )
	{
		return NULL;
	}

	//宠物定身抗性 第二层子节点
	ACString strPetResist3[9] = { _AL("resist"), _AL("name"), _AL("定身抗性"), _AL("value"), _AL("--"), _AL("attitude_name"), _AL("资质"), _AL("attitude_value"), _AL("") };
	strPetResist3[4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE3].cur_value );
	//如果有附加属性，则添加
	if ( nPetAddonListIndex >= 0 &&
		pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].resistance3 )
	{
		ACString strAddonResist3;
		strAddonResist3.Format( _AL("^00ff00%+d"), pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].resistance3 );
		strPetResist3[4] = strPetResist3[4] + strAddonResist3;
	}
	strPetResist3[8].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE3].potential );
	AXMLItem* pPetResistItem3 = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetResistItem2, strPetResist3, 9 );
	if ( !pPetResistItem3 )
	{
		return NULL;
	}

	//宠物魅惑抗性 第二层子节点
	ACString strPetResist4[9] = { _AL("resist"), _AL("name"), _AL("魅惑抗性"), _AL("value"), _AL("--"), _AL("attitude_name"), _AL("资质"), _AL("attitude_value"), _AL("") };
	strPetResist4[4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE4].cur_value );
	//如果有附加属性，则添加
	if ( nPetAddonListIndex >= 0 &&
		pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].resistance4 )
	{
		ACString strAddonResist4;
		strAddonResist4.Format( _AL("^00ff00%+d"), pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].resistance4 );
		strPetResist4[4] = strPetResist4[4] + strAddonResist4;
	}
	strPetResist4[8].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE4].potential );
	AXMLItem* pPetResistItem4 = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetResistItem3, strPetResist4, 9 );
	if ( !pPetResistItem4 )
	{
		return NULL;
	}

	//宠物昏睡抗性 第二层子节点
	ACString strPetResist5[9] = { _AL("resist"), _AL("name"), _AL("昏睡抗性"), _AL("value"), _AL("--"), _AL("attitude_name"), _AL("资质"), _AL("attitude_value"), _AL("") };
	strPetResist5[4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE5].cur_value );
	//如果有附加属性，则添加
	if ( nPetAddonListIndex >= 0 &&
		pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].resistance5 )
	{
		ACString strAddonResist5;
		strAddonResist5.Format( _AL("^00ff00%+d"), pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].resistance5 );
		strPetResist5[4] = strPetResist5[4] + strAddonResist5;
	}
	strPetResist5[8].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE5].potential );
	AXMLItem* pPetResistItem5 = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetResistItem4, strPetResist5, 9 );
	if ( !pPetResistItem5 )
	{
		return NULL;
	}

	//宠物爆击率 第二层子节点
	ACString strPetCriticalRate[5] = { _AL("critical_rate"), _AL("name"), _AL("暴击率"), _AL("value"), _AL("") };
	strPetCriticalRate[4].Format( _AL("%.1f%%"), pPet->GetEssence().crit_rate * 100 );
	//如果有附加属性，则添加
	if ( nPetAddonListIndex >= 0 &&
		fabs(pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].crit_rate) > 0.00001f )	//浮点数单独判断
	{
		ACString strAddonCriticalRate;
		strAddonCriticalRate.Format( _AL("^00ff00%+.1f%%"), pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].crit_rate * 100.0f );
		strPetCriticalRate[4] = strPetCriticalRate[4] + strAddonCriticalRate;
	}
	AXMLItem* pPetCriticalRateItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetResistItem5, strPetCriticalRate, 5 );
	if ( !pPetCriticalRateItem )
	{
		return NULL;
	}

	//宠物暴击伤害 第二层子节点
	ACString strPetCriticalDamage[5] = { _AL("critical_damage"), _AL("name"), _AL("暴击伤害"), _AL("value"), _AL("") };
	strPetCriticalDamage[4].Format( _AL("%d%%"), int(pPet->GetEssence().crit_damage * 100 + 0.001) );
	//如果有附加属性，则添加
	if ( nPetAddonListIndex >= 0 &&
		fabs(pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].crit_damage) > 0.00001f )
	{
		ACString strAddonCriticalDamage;
		strAddonCriticalDamage.Format( _AL("^00ff00%+d%%"), int(pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].crit_damage * 100 + 0.001f) );
		strPetCriticalDamage[4] = strPetCriticalDamage[4] + strAddonCriticalDamage;
	}
	AXMLItem* pPetCriticalDamageItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetCriticalRateItem, strPetCriticalDamage, 5 );
	if ( !pPetCriticalDamageItem )
	{
		return NULL;
	}

	//宠物寿命 第二层子节点
	ACString strPetLife[5] = { _AL("life"), _AL("name"), _AL("寿命"), _AL("value"), _AL("") };
	strPetLife[4].Format( _AL("%d/%d"), pPet->GetEssence().age, pPet->GetEssence().life );
	AXMLItem* pPetLifeItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetCriticalDamageItem, strPetLife, 5 );
	if ( !pPetLifeItem )
	{
		return NULL;
	}

	//宠物亲密度 第二层子节点
	ACString strPetIntimate[5] = { _AL("intimate"), _AL("name"), _AL("亲密度"), _AL("value"), _AL("") };
	strPetIntimate[4].Format( _AL("%d/%d"), pPet->GetEssence().cur_honor_point, pPet->GetEssence().max_honor_point );
	AXMLItem* pPetIntimateItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetLifeItem, strPetIntimate, 5 );
	if ( !pPetIntimateItem )
	{
		return NULL;
	}

	//宠物饱食度， 第二层子节点
	ACString strPetHungry[5] = { _AL("hungry"), _AL("name"), _AL("饱食度"), _AL("value"), _AL("") };
	strPetHungry[4].Format( _AL("%d/%d"), pPet->GetEssence().cur_hunger_point, pPet->GetEssence().max_hunger_point );
	AXMLItem* pPetHungryItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetIntimateItem, strPetHungry, 5 );
	if ( !pPetHungryItem )
	{
		return NULL;
	}

	//宠物主人，第二层子节点 New Added 2012-04-23.
	ACString strPetMaster[5] = { _AL("master"), _AL("name"), _AL("宠物主人"), _AL("value"), _AL("") };
	strPetMaster[4] = (ACHAR*)pPet->GetEssence().owner_name;
	AXMLItem* pPetMasterItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetHungryItem, strPetMaster, 5 );
	if ( !pPetMasterItem )
	{
		return NULL;
	}
	
	//宠物限界， 第二层子节点 New Added 2012-04-23.
	ACString strPetMainType[5] = { _AL("main_type"), _AL("name"), _AL("宠物限界"), _AL("value"), _AL("") };
	strPetMainType[4] = pDescTab->GetWideString(ITEMDESC_PET_MAINTYPE0 + pPet->GetEssence().main_type);
	AXMLItem* pPetMainTypeItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetMasterItem, strPetMainType, 5 );
	if ( !pPetMainTypeItem )
	{
		return NULL;
	}

	//宠物特质，第二层子节点 New Added 2012-04-23.
	ACString strPetTalent[5] = { _AL("talent"), _AL("name"), _AL("宠物特质"), _AL("value"), _AL("") };
	//---------------------------------------------------------------
	int nTalentCount = 0;
	const int MAX_TALENT_COUNT = 13;
	ACString strTalent;
	int nTalentIndex;
	for ( nTalentIndex = 0; nTalentIndex < MAX_TALENT_COUNT; ++nTalentIndex )
	{
		if (pPet->GetEssence().talents[nTalentIndex] == 10)
		{
			CECSkill* pSkill = new CECSkill(1210 + nTalentIndex, 1);
			if ( !pSkill )
			{
				a_LogOutput( 1, "Deal with Pet Talent: Create Skill Failed!, SkillID = %d\n", 1210 + nTalentIndex );
				continue;
			}

			if ( nTalentCount == 0 )
			{
				strTalent = g_pGame->GetSkillDesc()->GetWideString(pSkill->GetSkillID() * 10);
			}
			else
			{
				strTalent += _AL("/");
				strTalent += g_pGame->GetSkillDesc()->GetWideString(pSkill->GetSkillID() * 10);
			}

			delete pSkill;

			if (++nTalentCount == 4)
			{
				break;
			}
		}
	}

	if ( nTalentCount < 4 )
	{
		for (int i = 0; i < MAX_TALENT_COUNT; i++)
		{
			if (pPet->GetEssence().talents[i] == 5)
			{
				CECSkill* pSkill = new CECSkill(1210 + i, 1);
				if ( !pSkill )
				{
					a_LogOutput( 1, "Deal with Pet Talent: Create Skill Failed!, SkillID = %d\n", 1210 + nTalentIndex );
					continue;
				}

				if ( nTalentCount == 0 )
				{
					strTalent = g_pGame->GetSkillDesc()->GetWideString(pSkill->GetSkillID() * 10);
				}
				else
				{
					strTalent += _AL("/");
					strTalent += g_pGame->GetSkillDesc()->GetWideString(pSkill->GetSkillID() * 10);
				}

				delete pSkill;

				if ( ++nTalentCount == 4 )
				{
					break;
				}
			}
		}
	}
	strPetTalent[4] = strTalent;
	//---------------------------------------------------------------
	AXMLItem* pPetTalentItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetMainTypeItem, strPetTalent, 5 );
	if ( !pPetTalentItem )
	{
		return NULL;
	}

	//-------------------------------------------------------------------------
	//添加宠物时限 Added 2012-06-21.
	if ( pPet->GetExpireDate() > 0 )	//有时限
	{
		time_t curTime = time(NULL);
		tm tm1 = *gmtime(&curTime);
		tm tm2 = *localtime(&curTime);
		int nZone = tm2.tm_hour - tm1.tm_hour;
		time_t expTime = pPet->GetExpireDate();
		expTime -= nZone * 3600;
		tm* newTime = localtime(&expTime);
		const int MAX_BUFF_SIZE = 128;
		ACHAR tmpBuff[MAX_BUFF_SIZE];
		wcsftime( tmpBuff, MAX_BUFF_SIZE, _AL("%#x"), newTime );

		ACString strPetExpireTime[5] = { _AL("expire_time"), _AL("name"), _AL("到期时间"), _AL("value"), _AL("") };
		strPetExpireTime[4] = tmpBuff;
		AXMLItem* pPetExpireTimeItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetTalentItem, strPetExpireTime, 5 );
		if ( !pPetExpireTimeItem )
		{
			return NULL;
		}
	}
	//-------------------------------------------------------------------------

	//宠物生产力，第一层子节点
	ACString strPetProduction[3] = { _AL("product_profession"), _AL("name"), _AL("生产能力") };
	AXMLItem* pPetProductionItem = CreateXMLItem( pPetInfoItem, pPetInfoItem, pPetBasicPropertyItem, strPetProduction, 3 );
	if ( !pPetProductionItem )
	{
		return NULL;
	}

	//宠物生产能力	第二层子节点
	const int MAX_LIVE_ABILITY = 6;
	int nLiveValue[MAX_LIVE_ABILITY] =	{
		pPet->GetEssence().make_attr[itemdataman::_pet_bedge_essence::ACUITY],	//农艺
											pPet->GetEssence().make_attr[itemdataman::_pet_bedge_essence::DOWRY],	//渔猎
											pPet->GetEssence().make_attr[itemdataman::_pet_bedge_essence::STAMINA],	//采伐
											pPet->GetEssence().make_attr[itemdataman::_pet_bedge_essence::WISDOM],	//矿产
											pPet->GetEssence().make_attr[itemdataman::_pet_bedge_essence::SAWY],	//捕兽
											pPet->GetEssence().make_attr[itemdataman::_pet_bedge_essence::CNR]		//宝物
										};
	ACString strLiveAbility[MAX_LIVE_ABILITY][5] =	{
														{ _AL("product"), _AL("name"), _AL("农艺精通"), _AL("value"), _AL("") },
														{ _AL("product"), _AL("name"), _AL("渔猎精通"), _AL("value"), _AL("") },
														{ _AL("product"), _AL("name"), _AL("采伐精通"), _AL("value"), _AL("") },
														{ _AL("product"), _AL("name"), _AL("矿产精通"), _AL("value"), _AL("") },
														{ _AL("product"), _AL("name"), _AL("捕兽精通"), _AL("value"), _AL("") },
														{ _AL("product"), _AL("name"), _AL("宝物精通"), _AL("value"), _AL("") },
													};
	
	AXMLItem* pLiveAbilityItem = NULL;
	for ( int i = 0; i < MAX_LIVE_ABILITY; ++i )
	{
		strLiveAbility[i][4].Format( _AL("%d"), nLiveValue[i] );
		pLiveAbilityItem = CreateXMLItem( pPetInfoItem, pPetProductionItem, pLiveAbilityItem, strLiveAbility[i], 5 );
		if ( !pLiveAbilityItem )
		{
			return NULL;
		}
	}

	//宠物初始属性 第一层子节点
	ACString strPetInitValue[3] = { _AL("property_basic"), _AL("name"), _AL("初始值") };
	AXMLItem* pPetInitValueItem = CreateXMLItem( pPetInfoItem, pPetInfoItem, pPetProductionItem, strPetInitValue, 3 );
	if ( !pPetInitValueItem )
	{
		return NULL;
	}
	
	//具体初始值项 第二层子节点
	const int MAX_INIT_PROPERTY_COUNT = 12;
	ACString strInitProperty[MAX_INIT_PROPERTY_COUNT][5] =	{
																{ _AL("life"), _AL("name"), _AL("生命"), _AL("value"), _AL("") },
																{ _AL("defense"), _AL("name"), _AL("防御"), _AL("value"), _AL("") },
																{ _AL("mp"), _AL("name"), _AL("精力"), _AL("value"), _AL("") },
																{ _AL("hit"), _AL("name"), _AL("命中"), _AL("value"), _AL("") },
																{ _AL("dodge"), _AL("name"), _AL("闪避"), _AL("value"), _AL("") },
																{ _AL("min_attack"), _AL("name"), _AL("最小攻击"), _AL("value"), _AL("") },
																{ _AL("max_attack"), _AL("name"), _AL("最大攻击"), _AL("value"), _AL("") },
																{ _AL("resist"), _AL("name"), _AL("眩晕抗性"), _AL("value"), _AL("") },
																{ _AL("resist"), _AL("name"), _AL("虚弱抗性"), _AL("value"), _AL("") },
																{ _AL("resist"), _AL("name"), _AL("定身抗性"), _AL("value"), _AL("") },
																{ _AL("resist"), _AL("name"), _AL("魅惑抗性"), _AL("value"), _AL("") },
																{ _AL("resist"), _AL("name"), _AL("昏睡抗性"), _AL("value"), _AL("") }
															};

	strInitProperty[0][4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MAX_HP].init );
	strInitProperty[1][4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::DEFENCE].init );
	strInitProperty[2][4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MAX_VIGOR].init );
	strInitProperty[3][4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::HIT].init );
	strInitProperty[4][4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::JOUK].init );
	strInitProperty[5][4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MIN_ATTACK].init );
	strInitProperty[6][4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MAX_ATTACK].init );
	strInitProperty[7][4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE1].init );
	strInitProperty[8][4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE2].init );
	strInitProperty[9][4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE3].init );
	strInitProperty[10][4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE4].init );
	strInitProperty[11][4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE5].init );

	//创建具体基本初始属性子节点
	AXMLItem* pPetInitPropertyItem = NULL;
	for ( int j = 0; j < MAX_INIT_PROPERTY_COUNT; ++j )
	{
		pPetInitPropertyItem = CreateXMLItem( pPetInfoItem, pPetInitValueItem, pPetInitPropertyItem, strInitProperty[j], 5 );
		if ( !pPetInitPropertyItem )
		{
			return NULL;
		}
	}

	//宠物技能 第一层子节点
	ACString strPetSkills[5] = { _AL("pet_skill"), _AL("name"), _AL("宠物技能"), _AL("count"), _AL("") };
	AXMLItem* pPetSkillsItem = CreateXMLItem( pPetInfoItem, pPetInfoItem, pPetInitValueItem, strPetSkills, 5 );
	if ( !pPetSkillsItem )
	{
		return NULL;
	}

	//具体宠物技能 第二层子节点
	ACString strSkillItem[9] = { _AL("skill"), _AL("id"), _AL(""), _AL("name"), _AL(""), _AL("icon"), _AL(""), _AL("desc"), _AL("") };
	AXMLItem* pPetSkillItem = NULL;
	int nSkillCount = 0;
	for ( int k = 0; k < itemdataman::_pet_bedge_essence::MAX_PET_SKILL_COUNT; ++k )
	{
		int idSkill = pPet->GetEssence().skills[k].id;
		if ( !idSkill )
		{
			continue;
		}
		int iLevel =  pPet->GetEssence().skills[k].level;
		CECSkill *pSkill = new CECSkill(idSkill, iLevel);
		if( pSkill )
		{
			strSkillItem[2].Format( _AL("%d"), idSkill );
			//-------------------------------------------------------------------------特殊字符转义
			ACString strSkillName;
			TranslateStr( pSkill->GetName(), strSkillName );
			strSkillItem[4] = strSkillName;//pSkill->GetName()
			//-------------------------------------------------------------------------
			strSkillItem[6].Format( _AL("skill/%d.png"), idSkill );
			//-------------------------------------------------------------------------特殊字符转义
			ACString strSkillDesc;
			//AUICTranslate trans;
			//strSkillDesc = trans.Translate( pSkill->GetDesc() );
			TranslateStr( pSkill->GetDesc(), strSkillDesc );
			//-------------------------------------------------------------------------
			strSkillItem[8] = strSkillDesc;	//pSkill->GetDesc()
			nSkillCount++;
		}
		else
		{
			continue;
		}
		

		//!!!!!!!!!!清理资源!!!!!!!!!!
		delete pSkill;
		//创建具体技能表述信息子节点
		pPetSkillItem = CreateXMLItem( pPetInfoItem, pPetSkillsItem, pPetSkillItem, strSkillItem, 9 );
		if ( !pPetSkillItem )
		{
			a_LogOutput( 1, "error in line %d", __LINE__);
			return NULL;
		}
	}
	
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!注意，设置pPetSkillsItem中的count属性!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	ACString strPetSkillCount;
	strPetSkillCount.Format( _AL("%d"), nSkillCount );
	pPetSkillsItem->SetValue( _AL("count"), strPetSkillCount );

	//宠物装备 第一层子节点
	ACString strPetEquipments[5] = { _AL("pet_equipment"), _AL("name"), _AL("宠物装备"), _AL("count"), _AL("") };
	int nEquipCount = pDetailInfo->petinfo.petequip.size();
	strPetEquipments[4].Format( _AL("%d"), nEquipCount );
	AXMLItem* pPetEquipmentsItem = CreateXMLItem( pPetInfoItem, pPetInfoItem, pPetSkillsItem, strPetEquipments, 5 );
	if ( !pPetEquipmentsItem )
	{
		return NULL;
	}

	//具体宠物装备信息子节点
	ACString strInventoryItem[13] = { _AL("item"), _AL("id"), _AL(""), _AL("pos"), _AL(""), _AL("level"), _AL(""), _AL("name"), _AL(""), _AL("icon"), _AL(""), _AL("desc"), _AL("") };
	AXMLItem* pInventoryItem = NULL;
	for ( int nEquipIndex = 0; nEquipIndex < nEquipCount; ++nEquipIndex )
	{
		const GRoleInventory& itemInfo = pDetailInfo->petinfo.petequip[nEquipIndex];
		CECIvtrItem* pItem = CECIvtrItem::CreateItem(itemInfo.id, itemInfo.expire_date, itemInfo.count);
		if ( !pItem )
		{
			a_LogOutput(1, "Failed to create Inventory Item. ID=%d", itemInfo.id);
			continue;
		}
		if ( itemInfo.data.begin() && itemInfo.data.size() )
		{
			pItem->SetItemInfo((BYTE*)itemInfo.data.begin(), itemInfo.data.size());
		}
		pItem->UpdateInfoMD5();
		strInventoryItem[2].Format( _AL("%d"), itemInfo.id );
		strInventoryItem[4].Format( _AL("%d"), itemInfo.pos );
		strInventoryItem[6].Format( _AL("%d"), pItem->GetEstone() );
		//-------------------------------------------------------------------------特殊字符转义
		ACString strItemName;
		TranslateStr( pItem->GetName(), strItemName );
		strInventoryItem[8] = strItemName;//pItem->GetName()
		//-------------------------------------------------------------------------
		strInventoryItem[10].Format( _AL("%d.png"), itemInfo.id );
		//-------------------------------------------------------------------------特殊字符转义
		ACString strItemDesc;
		TranslateStr( pItem->GetDesc(), strItemDesc );
		strInventoryItem[12] = strItemDesc;//pItem->GetDesc()
		//-------------------------------------------------------------------------
		pInventoryItem = CreateXMLItem( pPetInfoItem, pPetEquipmentsItem, pInventoryItem, strInventoryItem, 13 );
		if ( !pInventoryItem )
		{
			return NULL;
		}
		
		//!!!!!!注意：清理资源!!!!!!
		delete pItem;
	}

	//最后返回宠物信息根节点
	return pPetInfoItem;
}
AXMLItem* CECRoleDescXMLParser::CreateRolePetXML( GNET::GConsignRole* pDetailInfo, int nPetIndex )
{
	if ( !pDetailInfo || nPetIndex < 0 || nPetIndex >= pDetailInfo->petinfo.petbadge.size() )
	{
		a_LogOutput( 1, "Input Parameters Invalid!" );
		return NULL;
	}

	//根据信息创建宠物
	const GRoleInventory& petInfo = pDetailInfo->petinfo.petbadge[nPetIndex];
	CECIvtrItem* pItem = CECIvtrItem::CreateItem(petInfo.id, petInfo.expire_date, petInfo.count);
	if ( !pItem )
	{
		a_LogOutput( 1, "Create Pet Badge Failed! PetID=%d", petInfo.id );
		return NULL;
	}
	
	//设置具体信息
	if ( petInfo.data.begin() && petInfo.data.size() )
	{
		pItem->SetItemInfo((BYTE*)petInfo.data.begin(), petInfo.data.size());
	}
	pItem->UpdateInfoMD5();

	CECPetBedge* pPet = (CECPetBedge*)pItem;

	//
	if ( !pPet || !pPet->GetEssence().identify )
	{
		a_LogOutput( 1, "Create Pet Failed! Pet Identify = %d\n", pPet->GetEssence().identify );
		return NULL;
	}

	ACString strPetInfo[5] = { _AL("pet"), _AL("name"), _AL(""), _AL("icon"), _AL("") };
	strPetInfo[4].Format( _AL("%d.png"), petInfo.id );	//不需要在head目录下？直接使用宠物牌物品下的图标？head/
	ACString strPetTransName;
	TranslateStr( pPet->GetName(), strPetTransName );
	strPetInfo[2] = strPetTransName;	//宠物名称可能需要转义pPet->GetName()	Modified 2012-05-03.
	//==========宠物头像路径如何定义？-------放到头像下面以宠物ID为检索

	//创建宠物信息根节点
	AXMLItem* pPetInfoItem = CreateXMLItem( NULL, NULL, NULL, strPetInfo, 5 );
	if ( !pPetInfoItem )
	{
		return NULL;
	}

	//宠物基本属性 第一层子节点
	ACString strPetBasicProperty[3] = { _AL("basic_property"), _AL("name"), _AL("基本属性") };
	AXMLItem* pPetBasicPropertyItem = CreateXMLItem( pPetInfoItem, pPetInfoItem, NULL, strPetBasicProperty, 3 );
	if ( !pPetBasicPropertyItem )
	{
		return NULL;
	}

	//宠物名称， 第二层子节点
	ACString strPetName[3] = { _AL("pet_name"), _AL("name"), _AL("") };
	strPetName[2] = strPetTransName;	//宠物名称可能需要转义pPet->GetName()
	AXMLItem* pPetNameItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, NULL, strPetName, 3 );
	if ( !pPetNameItem )
	{
		return NULL;
	}

	//宠物等级， 第二层子节点
	ACString strPetLevel[5] = { _AL("level"), _AL("name"), _AL("等级"), _AL("value"), _AL("") };
	strPetLevel[4].Format( _AL("%d"), pPet->GetEssence().level );
	AXMLItem* pPetLevelItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetNameItem, strPetLevel, 5 );
	if ( !pPetLevelItem )
	{
		return NULL;
	}
	
	//宠物经验，第二层子节点
	ACString strPetExp[5] = { _AL("exp"), _AL("name"), _AL("经验"), _AL("value"), _AL("--") };
	double fMaxExp = GetPetLevelUpExp( pPet->GetEssence().level + 1 );
	if ( fMaxExp > 0.0 )
	{
		strPetExp[4].Format( _AL("%.2f%%"), pPet->GetEssence().exp * 100.0 / fMaxExp );
	}
	AXMLItem* pPetExpItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetLevelItem, strPetExp, 5 );
	if ( !pPetExpItem )
	{
		return NULL;
	}

	//宠物性格 第二层子节点
	ACString strPetPersonality[5] = { _AL("personality"), _AL("name"), _AL("性格"), _AL("value"), _AL("--") };
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	if ( pDescTab )
	{
		strPetPersonality[4] = pDescTab->GetWideString( ITEMDESC_PET_ELEMENT0 + pPet->GetEssence().element );
	}
	AXMLItem* pPetPersonalityItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetExpItem, strPetPersonality, 5 );
	if ( !pPetPersonalityItem )
	{
		return NULL;
	}

	//宠物种族， 第二层子节点
	ACString strPetRace[5] = { _AL("race"), _AL("name"), _AL("种族"), _AL("value"), _AL("--") };
	if ( pDescTab )
	{
		strPetRace[4] = pDescTab->GetWideString(ITEMDESC_PET_RACE0 + pPet->GetEssence().race);
	}
	AXMLItem* pPetRaceItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetPersonalityItem, strPetRace, 5 );
	if ( !pPetRaceItem )
	{
		return NULL;
	}
	
	//宠物星象， 第二层子节点
	ACString strPetHoroscope[7] = { _AL("horoscope"), _AL("name"), _AL("星象"), _AL("value"), _AL("--"), _AL("level"), _AL("") };
	if ( pDescTab )
	{
		strPetHoroscope[4] = pDescTab->GetWideString(ITEMDESC_PET_HOROSCOPE0 + pPet->GetEssence().horoscope);
	}
	strPetHoroscope[6].Format( _AL("%d星"), pPet->GetEssence().star	 );
	AXMLItem* pPetHoroscopeItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetRaceItem, strPetHoroscope, 7 );
	if ( !pPetHoroscopeItem )
	{
		return NULL;
	}

	//宠物生命，第二层子节点
	ACString strPetHp[9] = { _AL("hp"), _AL("name"), _AL("生命"), _AL("value"), _AL("--"), _AL("attitude_name"), _AL("资质"), _AL("attitude_value"), _AL("") };
	strPetHp[4].Format( _AL("%d/%d"), pPet->GetEssence().cur_hp, pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MAX_HP].cur_value );
	//如果有附加属性，则添加
	int nPetAddonListIndex = GetPetAddInfoIndex(pDetailInfo, nPetIndex);
	if ( nPetAddonListIndex >= 0 &&
		pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].max_hp)
	{
		ACString strAddonHp;
		strAddonHp.Format( _AL("^00ff00%+d"), pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].max_hp );
		strPetHp[4] = strPetHp[4] + strAddonHp;
	}
	strPetHp[8].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MAX_HP].potential );
	AXMLItem* pPetHpItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetHoroscopeItem, strPetHp, 9 );
	if ( !pPetHpItem )
	{
		return NULL;
	}

	//宠物精力， 第二层子节点
	ACString strPetMp[9] = { _AL("mp"), _AL("name"), _AL("精力"), _AL("value"), _AL("--"), _AL("attitude_name"), _AL("资质"), _AL("attitude_value"), _AL("") };
	strPetMp[4].Format( _AL("%d/%d"), pPet->GetEssence().cur_vigor, pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MAX_VIGOR].cur_value );
	//如果有附加属性，则添加
	if ( nPetAddonListIndex >= 0 &&
		pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].max_mp)
	{
		ACString strAddonMp;
		strAddonMp.Format( _AL("^00ff00%+d"), pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].max_mp );
		strPetMp[4] = strPetMp[4] + strAddonMp;
	}
	strPetMp[8].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MAX_VIGOR].potential );
	AXMLItem* pPetMpItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetHpItem, strPetMp, 9 );
	if ( !pPetMpItem )
	{
		return NULL;
	}

	//宠物最小攻击 第二层子节点
	ACString strPetMinAttack[9] = { _AL("min_attack"), _AL("name"), _AL("最小攻击"), _AL("value"), _AL("--"), _AL("attitude_name"), _AL("资质"), _AL("attitude_value"), _AL("") };
	strPetMinAttack[4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MIN_ATTACK].cur_value );
	//如果有附加属性，则添加
	if ( nPetAddonListIndex >= 0 &&
		pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].attack)
	{
		ACString strAddonMinAttack;
		strAddonMinAttack.Format( _AL("^00ff00%+d"), pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].attack );
		strPetMinAttack[4] = strPetMinAttack[4] + strAddonMinAttack;
	}
	strPetMinAttack[8].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MIN_ATTACK].potential );
	AXMLItem* pPetMinAttackItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetMpItem, strPetMinAttack, 9 );
	if ( !pPetMinAttackItem )
	{
		return NULL;
	}

	//宠物最大攻击 第二层子节点
	ACString strPetMaxAttack[9] = { _AL("max_attack"), _AL("name"), _AL("最大攻击"), _AL("value"), _AL("--"), _AL("attitude_name"), _AL("资质"), _AL("attitude_value"), _AL("") };
	strPetMaxAttack[4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MAX_ATTACK].cur_value );
	//如果有附加属性，则添加
	if ( nPetAddonListIndex >= 0 &&
		pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].attack)
	{
		ACString strAddonMaxAttack;
		strAddonMaxAttack.Format( _AL("^00ff00%+d"), pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].attack );
		strPetMaxAttack[4] = strPetMaxAttack[4] + strAddonMaxAttack;
	}
	strPetMaxAttack[8].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MAX_ATTACK].potential );
	AXMLItem* pPetMaxAttackItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetMinAttackItem, strPetMaxAttack, 9 );
	if ( !pPetMaxAttackItem )
	{
		return NULL;
	}
	
	//宠物防御 第二层子节点
	ACString strPetDefence[9] = { _AL("defense"), _AL("name"), _AL("防御"), _AL("value"), _AL("--"), _AL("attitude_name"), _AL("资质"), _AL("attitude_value"), _AL("") };
	strPetDefence[4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::DEFENCE].cur_value );
	//如果有附加属性，则添加
	if ( nPetAddonListIndex >= 0 &&
		pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].defence)
	{
		ACString strAddonDefence;
		strAddonDefence.Format( _AL("^00ff00%+d"), pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].defence );
		strPetDefence[4] = strPetDefence[4] + strAddonDefence;
	}
	strPetDefence[8].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::DEFENCE].potential );
	AXMLItem* pPetDefenceItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetMaxAttackItem, strPetDefence, 9 );
	if ( !pPetDefenceItem )
	{
		return NULL;
	}

	//宠物命中 第二层子节点
	ACString strPetHit[9] = { _AL("hit"), _AL("name"), _AL("命中"), _AL("value"), _AL("--"), _AL("attitude_name"), _AL("资质"), _AL("attitude_value"), _AL("") };
	strPetHit[4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::HIT].cur_value );
	//如果有附加属性，则添加
	if ( nPetAddonListIndex >= 0 &&
		pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].hit)
	{
		ACString strAddonHit;
		strAddonHit.Format( _AL("^00ff00%+d"), pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].hit );
		strPetHit[4] = strPetHit[4] + strAddonHit;
	}
	strPetHit[8].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::HIT].potential );
	AXMLItem* pPetHitItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetDefenceItem, strPetHit, 9 );
	if ( !pPetHitItem )
	{
		return NULL;
	}

	//宠物闪避， 第二层子节点
	ACString strPetDodge[9] = { _AL("dodge"), _AL("name"), _AL("闪避"), _AL("value"), _AL("--"), _AL("attitude_name"), _AL("资质"), _AL("attitude_value"), _AL("") };
	strPetDodge[4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::JOUK].cur_value );
	//如果有附加属性，则添加
	if ( nPetAddonListIndex >= 0 &&
		pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].jouk)
	{
		ACString strAddonDodge;
		strAddonDodge.Format( _AL("^00ff00%+d"), pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].jouk );
		strPetDodge[4] = strPetDodge[4] + strAddonDodge;
	}
	strPetDodge[8].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::JOUK].potential );
	AXMLItem* pPetDodgeItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetHitItem, strPetDodge, 9 );
	if ( !pPetDodgeItem )
	{
		return NULL;
	}

	//宠物眩晕抗性 第二层子节点
	ACString strPetResist1[9] = { _AL("resist"), _AL("name"), _AL("眩晕抗性"), _AL("value"), _AL("--"), _AL("attitude_name"), _AL("资质"), _AL("attitude_value"), _AL("") };
	strPetResist1[4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE1].cur_value );
	//如果有附加属性，则添加
	if ( nPetAddonListIndex >= 0 &&
		pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].resistance1 )
	{
		ACString strAddonResist1;
		strAddonResist1.Format( _AL("^00ff00%+d"), pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].resistance1 );
		strPetResist1[4] = strPetResist1[4] + strAddonResist1;
	}
	strPetResist1[8].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE1].potential );
	AXMLItem* pPetResistItem1 = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetDodgeItem, strPetResist1, 9 );
	if ( !pPetResistItem1 )
	{
		return NULL;
	}

	//宠物虚弱抗性 第二层子节点
	ACString strPetResist2[9] = { _AL("resist"), _AL("name"), _AL("虚弱抗性"), _AL("value"), _AL("--"), _AL("attitude_name"), _AL("资质"), _AL("attitude_value"), _AL("") };
	strPetResist2[4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE2].cur_value );
	//如果有附加属性，则添加
	if ( nPetAddonListIndex >= 0 &&
		pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].resistance2 )
	{
		ACString strAddonResist2;
		strAddonResist2.Format( _AL("^00ff00%+d"), pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].resistance2 );
		strPetResist2[4] = strPetResist2[4] + strAddonResist2;
	}
	strPetResist2[8].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE2].potential );
	AXMLItem* pPetResistItem2 = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetResistItem1, strPetResist2, 9 );
	if ( !pPetResistItem2 )
	{
		return NULL;
	}

	//宠物定身抗性 第二层子节点
	ACString strPetResist3[9] = { _AL("resist"), _AL("name"), _AL("定身抗性"), _AL("value"), _AL("--"), _AL("attitude_name"), _AL("资质"), _AL("attitude_value"), _AL("") };
	strPetResist3[4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE3].cur_value );
	//如果有附加属性，则添加
	if ( nPetAddonListIndex >= 0 &&
		pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].resistance3 )
	{
		ACString strAddonResist3;
		strAddonResist3.Format( _AL("^00ff00%+d"), pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].resistance3 );
		strPetResist3[4] = strPetResist3[4] + strAddonResist3;
	}
	strPetResist3[8].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE3].potential );
	AXMLItem* pPetResistItem3 = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetResistItem2, strPetResist3, 9 );
	if ( !pPetResistItem3 )
	{
		return NULL;
	}

	//宠物魅惑抗性 第二层子节点
	ACString strPetResist4[9] = { _AL("resist"), _AL("name"), _AL("魅惑抗性"), _AL("value"), _AL("--"), _AL("attitude_name"), _AL("资质"), _AL("attitude_value"), _AL("") };
	strPetResist4[4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE4].cur_value );
	//如果有附加属性，则添加
	if ( nPetAddonListIndex >= 0 &&
		pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].resistance4 )
	{
		ACString strAddonResist4;
		strAddonResist4.Format( _AL("^00ff00%+d"), pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].resistance4 );
		strPetResist4[4] = strPetResist4[4] + strAddonResist4;
	}
	strPetResist4[8].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE4].potential );
	AXMLItem* pPetResistItem4 = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetResistItem3, strPetResist4, 9 );
	if ( !pPetResistItem4 )
	{
		return NULL;
	}

	//宠物昏睡抗性 第二层子节点
	ACString strPetResist5[9] = { _AL("resist"), _AL("name"), _AL("昏睡抗性"), _AL("value"), _AL("--"), _AL("attitude_name"), _AL("资质"), _AL("attitude_value"), _AL("") };
	strPetResist5[4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE5].cur_value );
	//如果有附加属性，则添加
	if ( nPetAddonListIndex >= 0 &&
		pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].resistance5 )
	{
		ACString strAddonResist5;
		strAddonResist5.Format( _AL("^00ff00%+d"), pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].resistance5 );
		strPetResist5[4] = strPetResist5[4] + strAddonResist5;
	}
	strPetResist5[8].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE5].potential );
	AXMLItem* pPetResistItem5 = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetResistItem4, strPetResist5, 9 );
	if ( !pPetResistItem5 )
	{
		return NULL;
	}

	//宠物爆击率 第二层子节点
	ACString strPetCriticalRate[5] = { _AL("critical_rate"), _AL("name"), _AL("暴击率"), _AL("value"), _AL("") };
	strPetCriticalRate[4].Format( _AL("%.1f%%"), pPet->GetEssence().crit_rate * 100 );
	//如果有附加属性，则添加
	if ( nPetAddonListIndex >= 0 &&
		fabs(pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].crit_rate) > 0.00001f )	//浮点数单独判断
	{
		ACString strAddonCriticalRate;
		strAddonCriticalRate.Format( _AL("^00ff00%+.1f%%"), pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].crit_rate * 100.0f );
		strPetCriticalRate[4] = strPetCriticalRate[4] + strAddonCriticalRate;
	}
	AXMLItem* pPetCriticalRateItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetResistItem5, strPetCriticalRate, 5 );
	if ( !pPetCriticalRateItem )
	{
		return NULL;
	}

	//宠物暴击伤害 第二层子节点
	ACString strPetCriticalDamage[5] = { _AL("critical_damage"), _AL("name"), _AL("暴击伤害"), _AL("value"), _AL("") };
	strPetCriticalDamage[4].Format( _AL("%d%%"), int(pPet->GetEssence().crit_damage * 100 + 0.001) );
	//如果有附加属性，则添加
	if ( nPetAddonListIndex >= 0 &&
		fabs(pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].crit_damage) > 0.00001f )
	{
		ACString strAddonCriticalDamage;
		strAddonCriticalDamage.Format( _AL("^00ff00%+d%%"), int(pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].crit_damage * 100 + 0.001f) );
		strPetCriticalDamage[4] = strPetCriticalDamage[4] + strAddonCriticalDamage;
	}
	AXMLItem* pPetCriticalDamageItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetCriticalRateItem, strPetCriticalDamage, 5 );
	if ( !pPetCriticalDamageItem )
	{
		return NULL;
	}


	//宠物减免暴击率 第二层子节点
	ACString strPetDeCriticalRate[5] = { _AL("de_critical_rate"), _AL("name"), _AL("减免暴击率"), _AL("value"), _AL("") };
	strPetDeCriticalRate[4].Format( _AL("%d%%"), int(pPet->GetEssence().anti_crit_rate * 100 + 0.001) );
	//如果有附加属性，则添加
	/*if ( nPetAddonListIndex >= 0 &&
		fabs(pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].crit_damage) > 0.00001f )
	{
		ACString strAddonCriticalDamage;
		strAddonCriticalDamage.Format( _AL("^00ff00%+d%%"), int(pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].crit_damage * 100 + 0.001f) );
		strPetCriticalDamage[4] = strPetCriticalDamage[4] + strAddonCriticalDamage;
	}*/
	AXMLItem* pPetDeCriticalRateItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetCriticalDamageItem, strPetDeCriticalRate, 5 );
	if ( !pPetDeCriticalRateItem )
	{
		return NULL;
	}

	//宠物减免暴击伤害 第二层子节点
	ACString strPetDeCriticalDamage[5] = { _AL("de_critical_damage"), _AL("name"), _AL("减免暴击伤害"), _AL("value"), _AL("") };
	strPetDeCriticalDamage[4].Format( _AL("%d%%"), int(pPet->GetEssence().anti_crit_damage * 100 + 0.001) );
	//如果有附加属性，则添加
	/*if ( nPetAddonListIndex >= 0 &&
		fabs(pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].crit_damage) > 0.00001f )
	{
		ACString strAddonCriticalDamage;
		strAddonCriticalDamage.Format( _AL("^00ff00%+d%%"), int(pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].crit_damage * 100 + 0.001f) );
		strPetCriticalDamage[4] = strPetCriticalDamage[4] + strAddonCriticalDamage;
	}*/
	AXMLItem* pPetDeCriticalDamageItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetDeCriticalRateItem, strPetDeCriticalDamage, 5 );
	if ( !pPetDeCriticalDamageItem )
	{
		return NULL;
	}

	//宠物技能命中 第二层子节点
	ACString strPetSkillAttackRate[5] = { _AL("skill_attack_rate"), _AL("name"), _AL("技能命中"), _AL("value"), _AL("") };
	strPetSkillAttackRate[4].Format( _AL("%d%%"), int(pPet->GetEssence().skill_attack_rate * 100 + 0.001) );
	//如果有附加属性，则添加
	/*if ( nPetAddonListIndex >= 0 &&
		fabs(pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].crit_damage) > 0.00001f )
	{
		ACString strAddonCriticalDamage;
		strAddonCriticalDamage.Format( _AL("^00ff00%+d%%"), int(pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].crit_damage * 100 + 0.001f) );
		strPetCriticalDamage[4] = strPetCriticalDamage[4] + strAddonCriticalDamage;
	}*/
	AXMLItem* pPetSkillAttackRateItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetDeCriticalDamageItem, strPetSkillAttackRate, 5 );
	if ( !pPetSkillAttackRateItem )
	{
		return NULL;
	}

	//宠物技能躲闪 第二层子节点
	ACString strPetSkillArmorRate[5] = { _AL("skill_armor_rate"), _AL("name"), _AL("技能躲闪"), _AL("value"), _AL("") };
	strPetSkillArmorRate[4].Format( _AL("%d%%"), int(pPet->GetEssence().skill_armor_rate * 100 + 0.001) );
	//如果有附加属性，则添加
	/*if ( nPetAddonListIndex >= 0 &&
		fabs(pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].crit_damage) > 0.00001f )
	{
		ACString strAddonCriticalDamage;
		strAddonCriticalDamage.Format( _AL("^00ff00%+d%%"), int(pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].crit_damage * 100 + 0.001f) );
		strPetCriticalDamage[4] = strPetCriticalDamage[4] + strAddonCriticalDamage;
	}*/
	AXMLItem* pPetSkillArmorRateItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetSkillAttackRateItem, strPetSkillArmorRate, 5 );
	if ( !pPetSkillArmorRateItem )
	{
		return NULL;
	}

	//宠物伤害减免 第二层子节点
	ACString strPetDamageReduce[5] = { _AL("damage_reduce"), _AL("name"), _AL("伤害减免"), _AL("value"), _AL("") };
	strPetDamageReduce[4].Format( _AL("%d"), pPet->GetEssence().damage_reduce );
	//如果有附加属性，则添加
	/*if ( nPetAddonListIndex >= 0 &&
		fabs(pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].crit_damage) > 0.00001f )
	{
		ACString strAddonCriticalDamage;
		strAddonCriticalDamage.Format( _AL("^00ff00%+d%%"), int(pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].crit_damage * 100 + 0.001f) );
		strPetCriticalDamage[4] = strPetCriticalDamage[4] + strAddonCriticalDamage;
	}*/
	AXMLItem* pPetDamageReduceItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetSkillArmorRateItem, strPetDamageReduce, 5 );
	if ( !pPetDamageReduceItem )
	{
		return NULL;
	}

	//宠物伤害减免百分比 第二层子节点
	ACString strPetDeDamageRate[5] = { _AL("de_damage_rate"), _AL("name"), _AL("伤害减免百分比"), _AL("value"), _AL("") };
	strPetDeDamageRate[4].Format( _AL("%d%%"), int(pPet->GetEssence().damage_reduce_percent * 100 + 0.001) );
	//如果有附加属性，则添加
	if ( nPetAddonListIndex >= 0 &&
		fabs(pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].crit_damage) > 0.00001f )
	{
		ACString strAddonCriticalDamage;
		strAddonCriticalDamage.Format( _AL("^00ff00%+d%%"), int(pDetailInfo->gsroleinfo.pet_addinfo[nPetAddonListIndex].crit_damage * 100 + 0.001f) );
		strPetDeDamageRate[4] = strPetDeDamageRate[4] + strAddonCriticalDamage;
	}
	AXMLItem* pPetDeDamageRateItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetDamageReduceItem, strPetDeDamageRate, 5 );
	if ( !pPetDeDamageRateItem )
	{
		return NULL;
	}





















	//宠物寿命 第二层子节点
	ACString strPetLife[5] = { _AL("life"), _AL("name"), _AL("寿命"), _AL("value"), _AL("") };
	strPetLife[4].Format( _AL("%d/%d"), pPet->GetEssence().age, pPet->GetEssence().life );
	AXMLItem* pPetLifeItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetDeDamageRateItem, strPetLife, 5 );
	if ( !pPetLifeItem )
	{
		return NULL;
	}

	//宠物亲密度 第二层子节点
	ACString strPetIntimate[5] = { _AL("intimate"), _AL("name"), _AL("亲密度"), _AL("value"), _AL("") };
	strPetIntimate[4].Format( _AL("%d/%d"), pPet->GetEssence().cur_honor_point, pPet->GetEssence().max_honor_point );
	AXMLItem* pPetIntimateItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetLifeItem, strPetIntimate, 5 );
	if ( !pPetIntimateItem )
	{
		return NULL;
	}

	//宠物饱食度， 第二层子节点
	ACString strPetHungry[5] = { _AL("hungry"), _AL("name"), _AL("饱食度"), _AL("value"), _AL("") };
	strPetHungry[4].Format( _AL("%d/%d"), pPet->GetEssence().cur_hunger_point, pPet->GetEssence().max_hunger_point );
	AXMLItem* pPetHungryItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetIntimateItem, strPetHungry, 5 );
	if ( !pPetHungryItem )
	{
		return NULL;
	}

	//宠物主人，第二层子节点 New Added 2012-04-23.
	ACString strPetMaster[5] = { _AL("master"), _AL("name"), _AL("宠物主人"), _AL("value"), _AL("") };
	strPetMaster[4] = (ACHAR*)pPet->GetEssence().owner_name;
	AXMLItem* pPetMasterItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetHungryItem, strPetMaster, 5 );
	if ( !pPetMasterItem )
	{
		return NULL;
	}
	
	//宠物限界， 第二层子节点 New Added 2012-04-23.
	ACString strPetMainType[5] = { _AL("main_type"), _AL("name"), _AL("宠物限界"), _AL("value"), _AL("") };
	strPetMainType[4] = pDescTab->GetWideString(ITEMDESC_PET_MAINTYPE0 + pPet->GetEssence().main_type);
	AXMLItem* pPetMainTypeItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetMasterItem, strPetMainType, 5 );
	if ( !pPetMainTypeItem )
	{
		return NULL;
	}

	//宠物特质，第二层子节点 New Added 2012-04-23.
	ACString strPetTalent[5] = { _AL("talent"), _AL("name"), _AL("宠物特质"), _AL("value"), _AL("") };
	//---------------------------------------------------------------
	int nTalentCount = 0;
	const int MAX_TALENT_COUNT = 13;
	ACString strTalent;
	int nTalentIndex;
	for ( nTalentIndex = 0; nTalentIndex < MAX_TALENT_COUNT; ++nTalentIndex )
	{
		if (pPet->GetEssence().talents[nTalentIndex] == 10)
		{
			CECSkill* pSkill = new CECSkill(1210 + nTalentIndex, 1);
			if ( !pSkill )
			{
				a_LogOutput( 1, "Deal with Pet Talent: Create Skill Failed!, SkillID = %d\n", 1210 + nTalentIndex );
				continue;
			}

			if ( nTalentCount == 0 )
			{
				strTalent = g_pGame->GetSkillDesc()->GetWideString(pSkill->GetSkillID() * 10);
			}
			else
			{
				strTalent += _AL("/");
				strTalent += g_pGame->GetSkillDesc()->GetWideString(pSkill->GetSkillID() * 10);
			}

			delete pSkill;

			if (++nTalentCount == 4)
			{
				break;
			}
		}
	}

	if ( nTalentCount < 4 )
	{
		for (int i = 0; i < MAX_TALENT_COUNT; i++)
		{
			if (pPet->GetEssence().talents[i] == 5)
			{
				CECSkill* pSkill = new CECSkill(1210 + i, 1);
				if ( !pSkill )
				{
					a_LogOutput( 1, "Deal with Pet Talent: Create Skill Failed!, SkillID = %d\n", 1210 + nTalentIndex );
					continue;
				}

				if ( nTalentCount == 0 )
				{
					strTalent = g_pGame->GetSkillDesc()->GetWideString(pSkill->GetSkillID() * 10);
				}
				else
				{
					strTalent += _AL("/");
					strTalent += g_pGame->GetSkillDesc()->GetWideString(pSkill->GetSkillID() * 10);
				}

				delete pSkill;

				if ( ++nTalentCount == 4 )
				{
					break;
				}
			}
		}
	}
	strPetTalent[4] = strTalent;
	//---------------------------------------------------------------
	AXMLItem* pPetTalentItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetMainTypeItem, strPetTalent, 5 );
	if ( !pPetTalentItem )
	{
		return NULL;
	}

	//-------------------------------------------------------------------------
	//添加宠物时限 Added 2012-06-21.
	if ( pPet->GetExpireDate() > 0 )	//有时限
	{
		time_t curTime = time(NULL);
		tm tm1 = *gmtime(&curTime);
		tm tm2 = *localtime(&curTime);
		int nZone = tm2.tm_hour - tm1.tm_hour;
		time_t expTime = pPet->GetExpireDate();
		expTime -= nZone * 3600;
		tm* newTime = localtime(&expTime);
		const int MAX_BUFF_SIZE = 128;
		ACHAR tmpBuff[MAX_BUFF_SIZE];
		wcsftime( tmpBuff, MAX_BUFF_SIZE, _AL("%#x"), newTime );

		ACString strPetExpireTime[5] = { _AL("expire_time"), _AL("name"), _AL("到期时间"), _AL("value"), _AL("") };
		strPetExpireTime[4] = tmpBuff;
		AXMLItem* pPetExpireTimeItem = CreateXMLItem( pPetInfoItem, pPetBasicPropertyItem, pPetTalentItem, strPetExpireTime, 5 );
		if ( !pPetExpireTimeItem )
		{
			return NULL;
		}
	}
	//-------------------------------------------------------------------------

	//宠物生产力，第一层子节点
	ACString strPetProduction[3] = { _AL("product_profession"), _AL("name"), _AL("生产能力") };
	AXMLItem* pPetProductionItem = CreateXMLItem( pPetInfoItem, pPetInfoItem, pPetBasicPropertyItem, strPetProduction, 3 );
	if ( !pPetProductionItem )
	{
		return NULL;
	}

	//宠物生产能力	第二层子节点
	const int MAX_LIVE_ABILITY = 6;
	int nLiveValue[MAX_LIVE_ABILITY] =	{
		pPet->GetEssence().make_attr[itemdataman::_pet_bedge_essence::ACUITY],	//农艺
											pPet->GetEssence().make_attr[itemdataman::_pet_bedge_essence::DOWRY],	//渔猎
											pPet->GetEssence().make_attr[itemdataman::_pet_bedge_essence::STAMINA],	//采伐
											pPet->GetEssence().make_attr[itemdataman::_pet_bedge_essence::WISDOM],	//矿产
											pPet->GetEssence().make_attr[itemdataman::_pet_bedge_essence::SAWY],	//捕兽
											pPet->GetEssence().make_attr[itemdataman::_pet_bedge_essence::CNR]		//宝物
										};
	ACString strLiveAbility[MAX_LIVE_ABILITY][5] =	{
														{ _AL("product"), _AL("name"), _AL("农艺精通"), _AL("value"), _AL("") },
														{ _AL("product"), _AL("name"), _AL("渔猎精通"), _AL("value"), _AL("") },
														{ _AL("product"), _AL("name"), _AL("采伐精通"), _AL("value"), _AL("") },
														{ _AL("product"), _AL("name"), _AL("矿产精通"), _AL("value"), _AL("") },
														{ _AL("product"), _AL("name"), _AL("捕兽精通"), _AL("value"), _AL("") },
														{ _AL("product"), _AL("name"), _AL("宝物精通"), _AL("value"), _AL("") },
													};
	
	AXMLItem* pLiveAbilityItem = NULL;
	for ( int i = 0; i < MAX_LIVE_ABILITY; ++i )
	{
		strLiveAbility[i][4].Format( _AL("%d"), nLiveValue[i] );
		pLiveAbilityItem = CreateXMLItem( pPetInfoItem, pPetProductionItem, pLiveAbilityItem, strLiveAbility[i], 5 );
		if ( !pLiveAbilityItem )
		{
			return NULL;
		}
	}

	//宠物初始属性 第一层子节点
	ACString strPetInitValue[3] = { _AL("property_basic"), _AL("name"), _AL("初始值") };
	AXMLItem* pPetInitValueItem = CreateXMLItem( pPetInfoItem, pPetInfoItem, pPetProductionItem, strPetInitValue, 3 );
	if ( !pPetInitValueItem )
	{
		return NULL;
	}
	
	//具体初始值项 第二层子节点
	const int MAX_INIT_PROPERTY_COUNT = 12;
	ACString strInitProperty[MAX_INIT_PROPERTY_COUNT][5] =	{
																{ _AL("life"), _AL("name"), _AL("生命"), _AL("value"), _AL("") },
																{ _AL("defense"), _AL("name"), _AL("防御"), _AL("value"), _AL("") },
																{ _AL("mp"), _AL("name"), _AL("精力"), _AL("value"), _AL("") },
																{ _AL("hit"), _AL("name"), _AL("命中"), _AL("value"), _AL("") },
																{ _AL("dodge"), _AL("name"), _AL("闪避"), _AL("value"), _AL("") },
																{ _AL("min_attack"), _AL("name"), _AL("最小攻击"), _AL("value"), _AL("") },
																{ _AL("max_attack"), _AL("name"), _AL("最大攻击"), _AL("value"), _AL("") },
																{ _AL("resist"), _AL("name"), _AL("眩晕抗性"), _AL("value"), _AL("") },
																{ _AL("resist"), _AL("name"), _AL("虚弱抗性"), _AL("value"), _AL("") },
																{ _AL("resist"), _AL("name"), _AL("定身抗性"), _AL("value"), _AL("") },
																{ _AL("resist"), _AL("name"), _AL("魅惑抗性"), _AL("value"), _AL("") },
																{ _AL("resist"), _AL("name"), _AL("昏睡抗性"), _AL("value"), _AL("") }
															};

	strInitProperty[0][4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MAX_HP].init );
	strInitProperty[1][4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::DEFENCE].init );
	strInitProperty[2][4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MAX_VIGOR].init );
	strInitProperty[3][4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::HIT].init );
	strInitProperty[4][4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::JOUK].init );
	strInitProperty[5][4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MIN_ATTACK].init );
	strInitProperty[6][4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MAX_ATTACK].init );
	strInitProperty[7][4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE1].init );
	strInitProperty[8][4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE2].init );
	strInitProperty[9][4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE3].init );
	strInitProperty[10][4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE4].init );
	strInitProperty[11][4].Format( _AL("%d"), pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::RESISTANCE5].init );

	//创建具体基本初始属性子节点
	AXMLItem* pPetInitPropertyItem = NULL;
	for ( int j = 0; j < MAX_INIT_PROPERTY_COUNT; ++j )
	{
		pPetInitPropertyItem = CreateXMLItem( pPetInfoItem, pPetInitValueItem, pPetInitPropertyItem, strInitProperty[j], 5 );
		if ( !pPetInitPropertyItem )
		{
			return NULL;
		}
	}

	//宠物技能 第一层子节点
	ACString strPetSkills[5] = { _AL("pet_skill"), _AL("name"), _AL("宠物技能"), _AL("count"), _AL("") };
	AXMLItem* pPetSkillsItem = CreateXMLItem( pPetInfoItem, pPetInfoItem, pPetInitValueItem, strPetSkills, 5 );
	if ( !pPetSkillsItem )
	{
		return NULL;
	}

	//具体宠物技能 第二层子节点
	ACString strSkillItem[9] = { _AL("skill"), _AL("id"), _AL(""), _AL("name"), _AL(""), _AL("icon"), _AL(""), _AL("desc"), _AL("") };
	AXMLItem* pPetSkillItem = NULL;
	int nSkillCount = 0;
	for ( int k = 0; k < itemdataman::_pet_bedge_essence::MAX_PET_SKILL_COUNT; ++k )
	{
		int idSkill = pPet->GetEssence().skills[k].id;
		if ( !idSkill )
		{
			continue;
		}
		int iLevel =  pPet->GetEssence().skills[k].level;
		CECSkill *pSkill = new CECSkill(idSkill, iLevel);
		if( pSkill )
		{
			strSkillItem[2].Format( _AL("%d"), idSkill );
			//-------------------------------------------------------------------------特殊字符转义
			ACString strSkillName;
			TranslateStr( pSkill->GetName(), strSkillName );
			strSkillItem[4] = strSkillName;//pSkill->GetName()
			//-------------------------------------------------------------------------
			strSkillItem[6].Format( _AL("skill/%d.png"), idSkill );
			//-------------------------------------------------------------------------特殊字符转义
			ACString strSkillDesc;
			//AUICTranslate trans;
			//strSkillDesc = trans.Translate( pSkill->GetDesc() );
			TranslateStr( pSkill->GetDesc(), strSkillDesc );
			//-------------------------------------------------------------------------
			strSkillItem[8] = strSkillDesc;	//pSkill->GetDesc()
			nSkillCount++;
		}
		else
		{
			continue;
		}
		

		//!!!!!!!!!!清理资源!!!!!!!!!!
		delete pSkill;
		//创建具体技能表述信息子节点
		pPetSkillItem = CreateXMLItem( pPetInfoItem, pPetSkillsItem, pPetSkillItem, strSkillItem, 9 );
		if ( !pPetSkillItem )
		{
			a_LogOutput( 1, "error in line %d", __LINE__);
			return NULL;
		}
	}
	
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!注意，设置pPetSkillsItem中的count属性!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	ACString strPetSkillCount;
	strPetSkillCount.Format( _AL("%d"), nSkillCount );
	pPetSkillsItem->SetValue( _AL("count"), strPetSkillCount );

	//宠物装备 第一层子节点
	ACString strPetEquipments[5] = { _AL("pet_equipment"), _AL("name"), _AL("宠物装备"), _AL("count"), _AL("") };
	int nEquipCount = pDetailInfo->petinfo.petequip.size();
	strPetEquipments[4].Format( _AL("%d"), nEquipCount );
	AXMLItem* pPetEquipmentsItem = CreateXMLItem( pPetInfoItem, pPetInfoItem, pPetSkillsItem, strPetEquipments, 5 );
	if ( !pPetEquipmentsItem )
	{
		return NULL;
	}

	//具体宠物装备信息子节点
	ACString strInventoryItem[13] = { _AL("item"), _AL("id"), _AL(""), _AL("pos"), _AL(""), _AL("level"), _AL(""), _AL("name"), _AL(""), _AL("icon"), _AL(""), _AL("desc"), _AL("") };
	AXMLItem* pInventoryItem = NULL;
	for ( int nEquipIndex = 0; nEquipIndex < nEquipCount; ++nEquipIndex )
	{
		const GRoleInventory& itemInfo = pDetailInfo->petinfo.petequip[nEquipIndex];
		CECIvtrItem* pItem = CECIvtrItem::CreateItem(itemInfo.id, itemInfo.expire_date, itemInfo.count);
		if ( !pItem )
		{
			a_LogOutput(1, "Failed to create Inventory Item. ID=%d", itemInfo.id);
			continue;
		}
		if ( itemInfo.data.begin() && itemInfo.data.size() )
		{
			pItem->SetItemInfo((BYTE*)itemInfo.data.begin(), itemInfo.data.size());
		}
		pItem->UpdateInfoMD5();
		strInventoryItem[2].Format( _AL("%d"), itemInfo.id );
		strInventoryItem[4].Format( _AL("%d"), itemInfo.pos );
		strInventoryItem[6].Format( _AL("%d"), pItem->GetEstone() );
		//-------------------------------------------------------------------------特殊字符转义
		ACString strItemName;
		TranslateStr( pItem->GetName(), strItemName );
		strInventoryItem[8] = strItemName;//pItem->GetName()
		//-------------------------------------------------------------------------
		strInventoryItem[10].Format( _AL("%d.png"), itemInfo.id );
		//-------------------------------------------------------------------------特殊字符转义
		ACString strItemDesc;
		TranslateStr( pItem->GetDesc(), strItemDesc );
		strInventoryItem[12] = strItemDesc;//pItem->GetDesc()
		//-------------------------------------------------------------------------
		pInventoryItem = CreateXMLItem( pPetInfoItem, pPetEquipmentsItem, pInventoryItem, strInventoryItem, 13 );
		if ( !pInventoryItem )
		{
			return NULL;
		}
		
		//!!!!!!注意：清理资源!!!!!!
		delete pItem;
	}

	//最后返回宠物信息根节点
	return pPetInfoItem;
}

double CECRoleDescXMLParser::GetPetLevelUpExp( int nLevel )
{
	DATA_TYPE dt;
	int idEssence = g_pGame->GetElementDataMan()->get_first_data_id(ID_SPACE_CONFIG, dt);
	PLAYER_LEVELEXP_CONFIG* pPlayerLevelExp = NULL;
	while ( idEssence )
	{
		if ( dt == DT_PLAYER_LEVELEXP_CONFIG )
		{
			pPlayerLevelExp = (PLAYER_LEVELEXP_CONFIG*)g_pGame->GetElementDataMan()->get_data_ptr(idEssence, ID_SPACE_CONFIG, dt);
			if ( !pPlayerLevelExp )
			{
				a_LogOutput( 1, "Read DT_PLAYER_LEVELEXP_CONFIG (id = %d)Failed!\n", idEssence );
				return 0.0;
			}
		}
		idEssence = g_pGame->GetElementDataMan()->get_next_data_id(ID_SPACE_CONFIG, dt);
	}

	if ( pPlayerLevelExp )
	{
		return pPlayerLevelExp->pet_exp[nLevel-1];
	}

	//
	return 0.0;	
}

void CECRoleDescXMLParser::GetOtherSkillInfo( const abase::hash_map<int, int>& skillsIDList, int nOtherSkillType, abase::vector<int>& deitySkillIDList )
{
	const int MAX_OTHER_SKILL_TYPE = 3;
	const int nOtherSkillIDBeginIndex[MAX_OTHER_SKILL_TYPE] = { 1970, 1979, 1988 };
	const int nOtherSkillNum[MAX_OTHER_SKILL_TYPE] = { 5, 6, 4 };
	if ( nOtherSkillType >= MAX_OTHER_SKILL_TYPE )
	{
		a_LogOutput( 1, "Invalid OtherSkillTypeSize! size=%d\n", nOtherSkillType );
		return;
	}
	for ( int i = 0; i < nOtherSkillNum[nOtherSkillType]; ++i )
	{
		int nSkillID = nOtherSkillIDBeginIndex[nOtherSkillType] + i;
		if ( skillsIDList.find( nSkillID ) != skillsIDList.end() )
		{
			deitySkillIDList.push_back( nSkillID );
		}
	}
}

void CECRoleDescXMLParser::GetMasterApprenticeSkillInfo( const abase::hash_map<int, int>& skillsIDList, abase::vector<int>& deitySkillIDList )
{
	const int MASTER_APPRENTICE_SKILL_BEGIN_ID = 1505;
	const int MASTER_APPRENTICE_SKILL_COUNT = 7;
	for ( int i = 0; i < MASTER_APPRENTICE_SKILL_COUNT; ++i )
	{
		if ( skillsIDList.find( MASTER_APPRENTICE_SKILL_BEGIN_ID + i ) != skillsIDList.end() )
		{
			deitySkillIDList.push_back( MASTER_APPRENTICE_SKILL_BEGIN_ID + i );
		}
	}
}

void CECRoleDescXMLParser::GetDeitySkillInfo( const abase::hash_map<int, int>& skillsIDList, int nDeityType, int nClsType, abase::vector<int>& deitySkillIDList )
{
	const int DEITY_BASIC_SKILL_COUNT = 12;
	const int MAX_DEITY_TYPE = 3;
	const int DEITY_SECT_SKILL_COUNT = 4;

	int nDeityBasicSkillBeginIndex[MAX_DEITY_TYPE] = { 3005, 3053, 3101 };
	int nDeitySectSkillBeginIndex[MAX_DEITY_TYPE][PLAYER_CLS_COUNT] =	{ 
																				{ 3021, 3029, 3017, 3025, 3033, 3569, 3037, 3041, 3045, 3049, 0, 4585, 5266, 0 },	//仙
																				{ 3069, 3077, 3065, 3073, 3081, 3575, 3085, 3089, 3093, 3097, 0, 4589, 5270, 0 },	//魔
																				{ 3117, 3125, 3113, 3121, 3129, 3579, 3133, 3137, 3141, 3145, 0, 4593, 5274, 0 }, //佛
																			};

	int nDeitySectSkillID[MAX_DEITY_TYPE][DEITY_SECT_SKILL_COUNT] =	{
																		{ 3573, 3584, 3574, 3585 },
																		{ 3587, 3588, 3590, 3591 },
																		{ 3583, 3594, 3592, 3595 },
																	};

	if ( nDeityType >= MAX_DEITY_TYPE )
	{
		a_LogOutput( 1, "Deity Type ERROR! current deity Type:%d\n", nDeityType );
		return;
	}
	for ( int i = 0; i < DEITY_BASIC_SKILL_COUNT; ++i )
	{
		int nSkillID = nDeityBasicSkillBeginIndex[nDeityType]+i;
		if ( skillsIDList.find( nSkillID ) != skillsIDList.end() )
		{
			deitySkillIDList.push_back( nSkillID );
		}
	}

	//
	if ( nClsType >= PLAYER_CLS_COUNT )
	{
		a_LogOutput( 1, "CLS Type ERROR! Cls Type:%d\n", nClsType );
		return;
	}

	if ( nClsType < (PLAYER_CLS_COUNT-1) )
	{
		for ( int j = 0; j < DEITY_SECT_SKILL_COUNT; ++j )
		{
			int nSkillID = nDeitySectSkillBeginIndex[nDeityType][nClsType] + j;
			if ( skillsIDList.find( nSkillID ) != skillsIDList.end() )
			{
				deitySkillIDList.push_back( nSkillID );
			}
		}
	}
	else if ( nClsType == (PLAYER_CLS_COUNT - 1) )	//太昊单独处理
	{
		for ( int i = 0; i < DEITY_SECT_SKILL_COUNT; ++i )
		{
			if ( skillsIDList.find( nDeitySectSkillID[nDeityType][i] ) != skillsIDList.end() )
			{
				deitySkillIDList.push_back( nDeitySectSkillID[nDeityType][i] );
			}
		}
	}

}

int CECRoleDescXMLParser::GetClsTypeByProfID( int nClsType )
{

	const int nClsTypeList[128] =	{ 
										CLS_INVALID,	//0
										CLS_GUIWANG, 
										CLS_GUIWANG, 
										CLS_GUIWANG,
										CLS_HEHUAN,
										CLS_HEHUAN,
										CLS_HEHUAN,
										CLS_QINGYUN,
										CLS_QINGYUN,
										CLS_QINGYUN,
										CLS_TIANYIN,	//10
										CLS_TIANYIN,
										CLS_TIANYIN,
										CLS_GUIWANG,
										CLS_GUIWANG,
										CLS_INVALID, 
										CLS_HEHUAN,
										CLS_HEHUAN,
										CLS_INVALID, 
										CLS_QINGYUN,
										CLS_QINGYUN,	//20
										CLS_INVALID,
										CLS_TIANYIN,
										CLS_TIANYIN,
										CLS_INVALID,
										CLS_GUIDAO,
										CLS_GUIDAO,
										CLS_GUIDAO,
										CLS_GUIDAO,
										CLS_GUIDAO,
										CLS_INVALID,	//30
										CLS_INVALID,
										CLS_INVALID,
										CLS_JIULI,
										CLS_JIULI,
										CLS_JIULI,
										CLS_JIULI,
										CLS_JIULI,
										CLS_INVALID,
										CLS_LIESHAN,
										CLS_LIESHAN,	//40
										CLS_LIESHAN,
										CLS_LIESHAN,
										CLS_LIESHAN,
										CLS_INVALID,
										CLS_HUAIGUANG,
										CLS_HUAIGUANG,
										CLS_HUAIGUANG,
										CLS_HUAIGUANG,
										CLS_HUAIGUANG,
										CLS_INVALID,	//50
										CLS_TIANHUA,
										CLS_TIANHUA,
										CLS_TIANHUA,
										CLS_TIANHUA,
										CLS_TIANHUA,
										CLS_CHENHUANG,
										CLS_CHENHUANG,
										CLS_CHENHUANG,
										CLS_CHENHUANG,
										CLS_CHENHUANG,	//60
										CLS_INVALID,
										CLS_INVALID,
										CLS_INVALID,
										CLS_FENXIANG,
										CLS_FENXIANG,
										CLS_FENXIANG,
										CLS_FENXIANG,
										CLS_FENXIANG,
										CLS_INVALID,
										CLS_INVALID,	//70
										CLS_INVALID,
										CLS_INVALID,
										CLS_INVALID,
										CLS_INVALID,
										CLS_INVALID,
										CLS_INVALID,
										CLS_INVALID,
										CLS_INVALID,
										CLS_INVALID,
										CLS_INVALID,	//80
										CLS_INVALID,
										CLS_INVALID,
										CLS_INVALID,
										CLS_INVALID,
										CLS_INVALID,
										CLS_INVALID,
										CLS_INVALID,
										CLS_INVALID,
										CLS_INVALID,
										CLS_INVALID,	//90
										CLS_INVALID,
										CLS_INVALID,
										CLS_INVALID,
										CLS_INVALID,
										CLS_INVALID,
										CLS_TAIHAO,
										CLS_TAIHAO,
										CLS_TAIHAO,
										CLS_TAIHAO,
										CLS_TAIHAO,		//100
										CLS_INVALID,
										CLS_LUOLI,			//萝莉
										CLS_LUOLI,			//萝莉
										CLS_LUOLI,			//萝莉
										CLS_LUOLI,			//萝莉
										CLS_LUOLI,			//萝莉
										CLS_INVALID,
										CLS_RENMA,			//人马
										CLS_RENMA,			//人马
										CLS_RENMA,			//人马110
										CLS_RENMA,			//人马
										CLS_RENMA,			//人马
										CLS_INVALID,	
										CLS_INVALID,
										CLS_INVALID,
										CLS_INVALID,
										CLS_INVALID,
										CLS_INVALID,
										CLS_INVALID,
										CLS_INVALID,	//120
										CLS_INVALID,
										CLS_INVALID,
										CLS_INVALID,
										CLS_INVALID,
										CLS_INVALID,
										CLS_INVALID,
										CLS_INVALID,	//127
									};
	
	if ( nClsType >= 128 )
	{
		return CLS_INVALID;
	}

	return nClsTypeList[nClsType];
}

void CECRoleDescXMLParser::GetProfSkillInfo( const abase::hash_map<int, int>& skillsIDList, int nClsType, int iLevel, abase::vector<int>& profSkillIDList)
{
	profSkillIDList.clear();
	int iProfSkills[CLS_MAX][ms_iProfSkillLvCount][ms_iProfSkillCountPerLv] =	
	{
		{ 
			{218,219,220,221},
			{243,244,245,248,251,252,260,261,462, } ,
			{246,247,250,253,254,257,258,259,393,},
			{249,255,256,262,263,264,314,315,463,},
			{519,520,521,522,523,524,525,526,527,},
			{775,776,777,778,},
		}, //鬼王
		{ 
			{218,219,220,221},
			{287,289,290,291,297,300,305,308,466, },
			{288,294,295,296,298,299,302,307,394,},
			{292,293,301,303,304,306,309,324,467,},
			{528,529,530,531,532,533,534,535,536,},
			{779,780,781,782,},
		},//合欢
		{ 
			{218,219,220,221},
			{222,223,225,226,231,232,233,380,460, },
			{224,228,229,230,234,235,239,312,395,},
			{236,237,238,240,241,242,313,381,461,},
			{537,538,539,540,541,542,543,544,545,},
			{783,784,785,786,},
		},//青云
		{ 
			{218,219,220,221},
			{266,267,268,269,270,271,276,283,464, },
			{273,274,278,279,280,382,383,386,396,},
			{275,277,282,284,285,319,384,385,465,},
			{546,547,548,549,550,551,552,553,554,},
			{787,788,789,790,},
		},//天音
		{ 
			{218,219,220,221},
			{1093,1094,1095,1096,1097,1098,1099,1100,1101,1102, },
			{1103,1104,1105,1106,1107,1108,1109,1110,1111,1112,},
			{1113,1114,1115,1116,1117,1118,1119,1120,1121,},
			{1122,1123,1124,1125,1126,1127,1128,1129,1130,},
			{1131,1132,1133,1134,},
		},//鬼道
		{ 
			{218,219,220,221},
			{3364,3365,3366,3367,3368,3369,3370,3371,3372, },
			{3424,3425,3426,3427,3428,3429,3430,3431,3432,},
			{3433,3434,3435,3436,3437,3438,3439,3440,3441,},
			{3442,3443,3444,3445,3446,3447,3448,3449,3450,},
			{3451,3452,3453,3454,},
		},//焚香
		{ 
			{218,219,220,221},
			{1843,1844,1845,1846,1847,1848,1849,1850,1851, 2183,2184,2185,},
			{1852,1853,1854,1855,1856,1857,1858,1859,1860,},
			{1861,1862,1863,1864,1865,1866,1867,1868,1869,},
			{1870,1871,1872,1873,1874,1875,1876,1877,1878,},
			{1879,1880,1881,1882,},
		},//九黎
		{
			{},
			{1883,1884,1885,1886,1887,1888,1889,1890,1891, 2186,2187,2188, },
			{1892,1893,1894,1895,1896,1897,1898,1899,1900,},
			{1901,1902,1903,1904,1905,1906,1907,1908,1909,},
			{1910,1911,1912,1913,1914,1915,1916,1917,1918,},
			{1919,1920,1921,1922,},
		},//烈山
		{ 
			{},
			{2411,2412,2413,2414,2415,2416,2417,2418,2419,2420,2421,2422, },
			{2423,2424,2425,2426,2427,2428,2429,2430,2431,},
			{2432,2433,2434,2435,2436,2437,2438,2439,2440,},
			{2441,2442,2443,2444,2445,2446,2447,2448,2449,},
			{2450,2451,2452,2453,},
		},//怀光
		{ 
			{},
			{2459,2460,2461,2462,2463,2464,2465,2466,2467,2468,2469,2470, },
			{2471,2472,2473,2474,2475,2476,2477,2478,2479,},
			{2480,2481,2482,2483,2484,2485,2486,2487,2488,},
			{2489,2490,2491,2492,2493,2494,2495,2496,2497,},
			{2498,2499,2500,2501,},
		},//天华
		{ 
			{},
			{3455,3456,3457,3458, 3460,3461,3462,3463,3464,3465,3466,3467, },
			{3468,3469,3470,3472,3473,3474,3475,3476,3477, },
			{3478,3479,3480, 3482,3483,3484,3485,3486,3487,},
			{3488,3489,3490, 3492,3493,3494,3495,3496,3497, },
			{3498,3499,3500,3501,3502,},
		},//太昊
		{ 
			{},
			{3997,3998,3999,4000,4001,4002,4003,4004,4005,4006,4007,4008,},
			{4009,4010,4011,4012,4013,4014,4015,4016,4017,},
			{4018,4019,4020,4021,4022,4023,4024,4025,4026, },
			{4027,4028,4029,4030,4031,4032,4033,4034,4035,},
			{4036,4037,4038,4039,4352,4353,4354,4355,4356,},
		},//辰皇
		{ 
			{},
			{4771,4772,4773,4774,4775,4776,4777,4778,4779,4780,4781,4782,4783,4784,4785,4786,4787,4788,4985,4986,4987,4988, },
			{4789,4790,4791,4792,4793,4794,4795,4796,4797,},
			{4798,4799,4800,4801,4802,4803,4804,4805,4806,},
			{4807,4808,4809,4810,4811,4812,4813,4814,4815,},
			{4816,4817,4818,4819,4820,4821,4822,4823,4824,},
		},//牵机
		{ 
			{},
			{4838,4839,4840,4841,4842,4843,4844,4845,4846,4847,4848,4849, },	
			{4850,4851,4852,4853,4854,4855,4856,4857,4858,5240,},
			{4859,4860,4861,4862,4863,4864,4865,4866,4867,5115, },
			{4868,4869,4870,4871,4872,4873,4874,4875,4876,5116,},
			{4877,4878,4879,4880,5227,},
		},//英招



	};
	int i;
	for(i=0; i<ms_iProfSkillLvCount; ++i)
	{
		if(iProfSkills[nClsType][iLevel][i]==0)
			break;
		profSkillIDList.push_back(iProfSkills[nClsType][iLevel][i]);
	}
}

void CECRoleDescXMLParser::GetTalentSkillInfo( const abase::hash_map<int, int>& skillsIDList, int nClsType, int iLevel, abase::vector<int>& profTalentIDList)
{
	profTalentIDList.clear();
	int iProfTalents[CLS_MAX][ms_iProfTalentLvCount][ms_iProfSkillCountPerLv] =	
	{
		{ 
			{618,619,620,621,622,623,624,625,626,627,628,629,630,631,632,633,634, },
			{635,636,637,638,639,640,641,642,643,644,645,646,647,648,649,650,651, },
			{652,653,654,655,656,657,658,659,660,661,662,663,664,665,666,667,668, },
		},//鬼王
		{ 
			{720,721,722,723,724,725,726,727,728,729,730,731,732,733,734,735,736, },
			{737,738,739,740,741,742,743,744,745,746,747,748,749,750,751,752,753, },
			{754,755,756,757,758,759,760,761,762,763,764,765,766,767,768,769,770, },
		},//合欢
		{ 
			{567,568,569,570,571,572,573,574,575,576,577,578,579,580,581,582,583,},
			{584,585,586,587,588,589,590,591,592,593,594,595,596,597,598,599,600,},
			{601,602,603,604,605,606,607,608,609,610,611,612,613,614,615,616,617,},
		},//青云
		{ 
			{669,670,671,672,673,674,675,676,677,678,679,680,681,682,683,684,685,},
			{686,687,688,689,690,691,692,693,694,695,696,697,698,699,700,701,702,},
			{703,704,705,706,707,708,709,710,711,712,713,714,715,716,717,718,719, },
		},//天音
		{ 
			{1139,1140,1141,1142,1143,1144,1145,1146,1147,1148,1149,1150,1151,1152,1153,1154,1155, },
			{1156,1157,1158,1159,1160,1161,1162,1163,1164,1165,1166,1167,1168,1169,1170,1171,1172, },
			{1173,1174,1175,1176,1177,1178,1179,1180,1181,1182,1183,1184,1185,1186,1187,1188,1189,},
		},//鬼道
		{ 
			{3373,3374,3375,3376,3377,3378,3379,3380,3381,3382,3383,3384,3385,3386,3387,3388,3389, },
			{3390,3391,3392,3393,3394,3395,3396,3397,3398,3399,3400,3401,3402,3403,3404,3405,3406, },
			{3407,3408,3409,3410,3411,3412,3413,3414,3415,3416,3417,3418,3419,3420,3421,3422,3423, },
		},//焚香
		{ 
			{2074,2075,2076,2077,2078,2079,2080,2081,2082,2083,2084,2085,2086,2087,2088,2089,2090, },
			{2091,2092,2093,2094,2095,2096,2097,2098,2099,2100,2101,2102,2103,2104,2105,2106,2107, },
			{2108,2109,2110,2111,2112,2113,2114,2115,2116,2117,2118,2119,2120,2121,2122,2123,2124,},
		},//九黎
		{ 
			{2125,2126,2127,2128,2129,2130,2131,2132,2133,2134,2135,2136,2137,2138,2139,2140,2141, },
			{2142,2143,2144,2145,2146,2147,2148,2149,2150,2151,2152,2153,2154,2155,2156,2157,2158, },
			{2159,2160,2161,2162,2163,2164,2165,2166,2167,2168,2169,2170,2171,2172,2173,2174,2175, },
		},//烈山
		{ 
			{2523,2524,2525,2526,2527,2528,2529,2530,2531,2532,2533,2534,2535,2536,2537,2538,2539, },
			{2540,2541,2542,2543,2544,2545,2546,2547,2548,2549,2550,2551,2552,2553,2554,2555,2556, },
			{2557,2558,2559,2560,2561,2562,2563,2564,2565,2566,2567,2568,2569,2570,2571,2572,2573,},
		},//怀光
		{ 
			{2584,2585,2586,2587,2588,2589,2590,2591,2592,2593,2594,2595,2596,2597,2598,2599,2600, },
			{2603,2604,2605,2606,2608,2609,2610,2611,2612,2613,2614,2615,2616,2617,2618,2619,2620, },
			{2622,2623,2624,2625,2626,2627,2628,2629,2630,2631,2632,2633,2634,2635,2636,2637,2638, },
		},//天华
		{ 
			{3503,3504,3505,3506,3507,3508,3509,3510,3511,3512,3513,3514,3515,3516,3517,3518,3519, },
			{3520,3521,3522,3523,3524,3525,3526,3527,3528,3529,3530,3531,3532,3533,3534,3535,3536, },
			{3537,3538,3539,3540,3541,3542,3543,3544,3545,3546,3547,3548,3549,3550,3551,3552,3553, },
		},//太昊
		{ 
			{4115,4116,4117,4118,4119,4120,4121,4122,4123,4124,4125,4126,4127,4128,4129,4130,4131, },
			{4132,4133,4134,4135,4136,4137,4138,4139,4140,4141,4142,4143,4144,4145,4146,4147,4148, },
			{4149,4150,4151,4152,4153,4154,4155,4156,4157,4158,4159,4160,4161,4162,4163,4164,4165,},
		},//辰皇
		{ 
			{5125,5126,5127,5128,5129,5130,5131,5132,5133,5134,5135,5136,5137,5138,5139,5140,5141, },
			{5142,5143,5144,5145,5146,5147,5148,5149,5150,5151,5152,5153,5154,5155,5156,5157,5158, },
			{5159,5160,5161,5162,5163,5164,5165,5166,5167,5168,5169,5170,5171,5172,5173,5174,5175,},
		},//牵机
		{ 
			{5176,5177,5178,5179,5180,5181,5182,5183,5184,5185,5186,5187,5188,5189,5190,5191,5192, },
			{5193,5194,5195,5196,5197,5198,5199,5200,5201,5202,5203,5204,5205,5206,5207,5208,5209, },
			{5210,5211,5212,5213,5214,5215,5216,5217,5218,5219,5220,5221,5222,5223,5224,5225,5226,},
		},//英招

	};
	int i;
	for(i=0; i<ms_iProfTalentLvCount; ++i)
	{
		if(iProfTalents[nClsType][iLevel][i]==0)
			break;
		profTalentIDList.push_back(iProfTalents[nClsType][iLevel][i]);
	}
}

void CECRoleDescXMLParser::GetNatureSkillInfo( const abase::hash_map<int, int>& skillsIDList, int nNatureType, int nNatureSkillType, int nClsType, abase::vector<int>& natureSkillIDList )
{
	int nGeneralSkillTaoism[GENERAL_SKILL_COUNT] = { 1598, 1599, 1600, 1601, 1602, 1603, 2035, 2036 };
	int nGeneralSkillEvil[GENERAL_SKILL_COUNT] = { 1604, 1605, 1606, 1607, 1608, 1609, 2037, 2038 };
	int nGeneralSkillBuddihsm[GENERAL_SKILL_COUNT] = { 1610, 1611, 1612, 1613, 1614, 1615, 2039, 2040 };
	int nBasicSkillTaoism[BASIC_SKILL_COUNT] = { 1520, 1521, 1522, 1523, 1524, 1525, 1526, 1527, 1528, 1529, 1530 };
	int nBasicSkillEvil[BASIC_SKILL_COUNT] = { 1546, 1547, 1548, 1549, 1550, 1551, 1552, 1553, 1554, 1555, 1556 };
	int nBasicSkillBuddihsm[BASIC_SKILL_COUNT] = { 1572, 1573, 1574, 1575, 1576, 1577, 1578, 1579, 1580, 1581, 1582 };
/*	int nSectSkillTaoism[PLAYER_CLS_COUNT][SECT_SKILL_COUNT] =	{ 
																	{ 1531, 1532, 1533, 2048, 2049 },	//鬼王
																	{ 1534, 1535, 1536, 2046, 2047 },	//合欢
																	{ 1537, 1538, 1539, 2044, 2045 },	//青云
																	{ 1540, 1541, 1542, 2050, 2051 },	//天音
																	{ 1543, 1544, 1545, 2052, 2053 },	//鬼道
																	{ 3554, 3555, 3556, 3557, 3558 },	//焚香
																	{ 2215, 2216, 2217, 2218, 2219 },	//九黎
																	{ 2220, 2221, 2222, 2223, 2224 },	//烈山
																	{ 2689, 2690, 2691, 2692, 2693 },	//怀光
																	{ 2694, 2695, 2696, 2697, 2698 },	//天华
																	{ 3628, 3629, 3630, 3631, 3632 },	//太昊
																};
	int nSectSkillEvil[PLAYER_CLS_COUNT][SECT_SKILL_COUNT]	=	{
																	{ 1557, 1558, 1559, 2058, 2059 },	//鬼王
																	{ 1560, 1561, 1562, 2056, 2057 },	//合欢
																	{ 1563, 1564, 1565, 2054, 2055 },	//青云
																	{ 1566, 1567, 1568, 2060, 2061 },	//天音
																	{ 1569, 1570, 1571, 2062, 2063 },	//鬼道
																	{ 3559, 3560, 3561, 3562, 3563 },	//焚香
																	{ 2225, 2226, 2227, 2228, 2229 },	//九黎
																	{ 2230, 2231, 2232, 2233, 2234 },	//烈山
																	{ 2699, 2700, 2701, 2702, 2703 },	//怀光
																	{ 2704, 2705, 2706, 2707, 2708 },	//天华
																	{ 3623, 3624, 3625, 3626, 3627 },	//太昊																	
																};

	int nSectSkillBuddihsm[PLAYER_CLS_COUNT][SECT_SKILL_COUNT]	=	{
																	{ 1583, 1584, 1585, 2068, 2069 },	//鬼王
																	{ 1586, 1587, 1588, 2066, 2067 },	//合欢
																	{ 1589, 1590, 1591, 2064, 2065 },	//青云
																	{ 1592, 1593, 1594, 2070, 2071 },	//天音
																	{ 1595, 1596, 1597, 2072, 2073 },	//鬼道
																	{ 3564, 3565, 3566, 3567, 3568 },	//焚香
																	{ 2235, 2236, 2237, 2238, 2239 },	//九黎
																	{ 2240, 2241, 2242, 2243, 2244 },	//烈山
																	{ 2709, 2710, 2711, 2712, 2713 },	//怀光
																	{ 2714, 2715, 2716, 2717, 2718 },	//天华
																	{ 3618, 3619, 3620, 3621, 3622 },	//太昊																	
																};
	*/

	int nSectSkills[NATURE_TYPE_COUNT][PLAYER_CLS_COUNT][SECT_SKILL_COUNT] =	{ 
																					{	{ 1531, 1532, 1533, 2048, 2049 },	//鬼王
																						{ 1534, 1535, 1536, 2046, 2047 },	//合欢
																						{ 1537, 1538, 1539, 2044, 2045 },	//青云
																						{ 1540, 1541, 1542, 2050, 2051 },	//天音
																						{ 1543, 1544, 1545, 2052, 2053 },	//鬼道
																						{ 3746, 3747, 3748, 3749, 3750 },	//焚香  //Modified 3554, 3555, 3556, 3557, 3558
																						{ 2215, 2216, 2217, 2218, 2219 },	//九黎
																						{ 2220, 2221, 2222, 2223, 2224 },	//烈山
																						{ 2689, 2690, 2691, 2692, 2693 },	//怀光
																						{ 2694, 2695, 2696, 2697, 2698 },	//天华
																						{ 3741, 3742, 3743, 3744, 3745 },	//太昊	//Modified 3628, 3629, 3630, 3631, 3632
																						{ 4570, 4571, 4572, 4573, 4574 },	//辰皇
																						{ 5251, 5252, 5253,5354, 5355 },	//英招
																						{ 4570, 4571, 4572, 4573, 4574 },	//辰皇
																					},
																					{
																						{ 1557, 1558, 1559, 2058, 2059 },	//鬼王
																						{ 1560, 1561, 1562, 2056, 2057 },	//合欢
																						{ 1563, 1564, 1565, 2054, 2055 },	//青云
																						{ 1566, 1567, 1568, 2060, 2061 },	//天音
																						{ 1569, 1570, 1571, 2062, 2063 },	//鬼道
																						{ 3751, 3752, 3753, 3754, 3755 },	//焚香	//Modified 
																						{ 2225, 2226, 2227, 2228, 2229 },	//九黎
																						{ 2230, 2231, 2232, 2233, 2234 },	//烈山
																						{ 2699, 2700, 2701, 2702, 2703 },	//怀光
																						{ 2704, 2705, 2706, 2707, 2708 },	//天华
																						{ 3810, 3811, 3812, 3813, 3814 },	//太昊	//Modified 	3623, 3624, 3625, 3626, 3627															
																						{ 4575, 4576, 4577, 4578, 4579 },	//辰皇
																						{ 5256, 5257, 5258,5359, 5360 },	//英招
																						{ 4575, 4576, 4577, 4578, 4579 },	//辰皇
																					},
																					{
																						{ 1583, 1584, 1585, 2068, 2069 },	//鬼王
																						{ 1586, 1587, 1588, 2066, 2067 },	//合欢
																						{ 1589, 1590, 1591, 2064, 2065 },	//青云
																						{ 1592, 1593, 1594, 2070, 2071 },	//天音
																						{ 1595, 1596, 1597, 2072, 2073 },	//鬼道
																						{ 3756, 3757, 3758, 3759, 3760 },	//焚香	//Modified 3564, 3565, 3566, 3567, 3568
																						{ 2235, 2236, 2237, 2238, 2239 },	//九黎
																						{ 2240, 2241, 2242, 2243, 2244 },	//烈山
																						{ 2709, 2710, 2711, 2712, 2713 },	//怀光
																						{ 2714, 2715, 2716, 2717, 2718 },	//天华
																						{ 3805, 3806, 3807, 3808, 3809 },	//太昊	//Modified 3618, 3619, 3620, 3621, 3622															
																						{ 4580, 4581, 4582, 4583, 4584 },	//辰皇
																						{ 5261, 5262, 5263,5364, 5365 },	//英招
																						{ 4580, 4581, 4582, 4583, 4584 },	//辰皇
																					}
																				};

	//
	struct ARRAY_ITEM
	{
		int		m_nArraySize;
		int*	m_pArray;
	};

	ARRAY_ITEM arrayItem[NATURE_TYPE_COUNT][NATURE_TYPE_COUNT-1] =	{
																		{{BASIC_SKILL_COUNT, nBasicSkillTaoism}, {GENERAL_SKILL_COUNT, nGeneralSkillTaoism}},
																		{{BASIC_SKILL_COUNT, nBasicSkillEvil},{GENERAL_SKILL_COUNT, nGeneralSkillEvil}},
																		{{BASIC_SKILL_COUNT, nBasicSkillBuddihsm},{GENERAL_SKILL_COUNT, nGeneralSkillBuddihsm}},
																	};

//	int** pSectSkillList[NATURE_TYPE_COUNT] = { (int**)(&nSectSkillTaoism[0][0]), (int**)(&nSectSkillEvil[0][0]), (int**)(&nSectSkillBuddihsm) };

	//
	if ( nNatureType >= NATURE_TYPE_COUNT || nNatureSkillType >= NATURE_TYPE_COUNT || nClsType >= PLAYER_CLS_COUNT || nClsType < 0 )
	{
		a_LogOutput( 1, "Nature type or Nature skill type size invalid! NatureTypeSize=%d, NatureSkillTypeSize=%d, nClsType = %d\n", nNatureType, nNatureSkillType, nClsType );
		return;
	}

	natureSkillIDList.clear();	//先清空
	if ( nNatureSkillType != NATURE_TYPE_SECT )	//写死了。。。2就是指门派技能
	{
		for ( int i = 0; i < arrayItem[nNatureType][nNatureSkillType].m_nArraySize; ++i )
		{
			if ( skillsIDList.find( arrayItem[nNatureType][nNatureSkillType].m_pArray[i] ) != skillsIDList.end() )
			{
				natureSkillIDList.push_back( arrayItem[nNatureType][nNatureSkillType].m_pArray[i] );
			}
		}
	}
	else
	{
/*		int** sectSkillList = NULL;
		if ( 0 == nNatureType )
		{
			sectSkillList = (int**)(&nSectSkillTaoism[0][0]);
		}
		else if ( 1 == nNatureType )
		{
			sectSkillList = (int**)(&nSectSkillEvil[0][0]);
		}
		else if ( 2 == nNatureType )
		{
			sectSkillList = (int**)(&nSectSkillBuddihsm[0][0]);
		}*/
		for ( int i = 0; i < SECT_SKILL_COUNT; ++i )
		{
			if ( skillsIDList.find( nSectSkills[nNatureType][nClsType][i] ) != skillsIDList.end() ) //pSectSkillList[nNatureType][nClsType][i]
			{
				natureSkillIDList.push_back( nSectSkills[nNatureType][nClsType][i] ); //pSectSkillList[nNatureType][nClsType][i]
			}
		}
	}

}

void CECRoleDescXMLParser::GetSkillsInfo( const GNET::Octets& skillsData, abase::hash_map<int, int>& skillsIDList )
{
	//判断输入参数的合法性
	if ( skillsData.size() < sizeof(size_t) )
	{
		a_LogOutput( 1, "Skill data size Error! size=%d\n", skillsData.size() );
		return;
	}

#pragma pack(1)
	struct SKILL
	{
		short			id_skill;
		unsigned char	level;
	};
#pragma pack()
	char* pSkillData = (char*)skillsData.begin();
	int nSkillsSize = *((size_t*)pSkillData);
	pSkillData += sizeof(size_t);
	int nDataSize = (char*)skillsData.end() - pSkillData;
	if ( nDataSize < nSkillsSize*sizeof(SKILL) )
	{
		a_LogOutput( 1, "Skill data size Error! DataSize=%d, RealDataSize=%d\n", nSkillsSize*sizeof(SKILL), nDataSize );
		return;
	}

	for ( int i = 0; i < nSkillsSize; ++i )
	{
		const SKILL& skillInfo = *((SKILL*)pSkillData);
		skillsIDList[skillInfo.id_skill] = skillInfo.level;
		pSkillData += sizeof(SKILL);
	}
}

double CECRoleDescXMLParser::GetLevelUpExp( int nLevel, int nRebornCount )
{
	DATA_TYPE dt;
	int idEssence = g_pGame->GetElementDataMan()->get_first_data_id(ID_SPACE_CONFIG, dt);
	PLAYER_LEVELEXP_CONFIG* pPlayerLevelExp = NULL;
	while ( idEssence )
	{
		if ( dt == DT_PLAYER_LEVELEXP_CONFIG )
		{
			pPlayerLevelExp = (PLAYER_LEVELEXP_CONFIG*)g_pGame->GetElementDataMan()->get_data_ptr(idEssence, ID_SPACE_CONFIG, dt);
			if ( !pPlayerLevelExp )
			{
				a_LogOutput( 1, "Read DT_PLAYER_LEVELEXP_CONFIG (id = %d)Failed!\n", idEssence );
				return 0.0;
			}
		}
		idEssence = g_pGame->GetElementDataMan()->get_next_data_id(ID_SPACE_CONFIG, dt);
	}

	if ( pPlayerLevelExp )
	{
		switch ( nRebornCount )
		{
			case 0:
				return pPlayerLevelExp->exp[nLevel-1];
			case 1:
				return pPlayerLevelExp->exp_lev_1[nLevel-1];
			case 2:
				return pPlayerLevelExp->exp_lev_2[nLevel-1];
			default:
				return pPlayerLevelExp->exp_lev_3[nLevel-1];
		}
	}

	//
	return 0.0;
}

void CECRoleDescXMLParser::GetReputationStringValues( int nReputation, int nIndex, ACString& strReputationName, ACString& strReputationValue, ACString& strReputationProgress )
{
	const int regionreputationlevel[] = {
											0,
											2000,
											10000,
											50000,
											130000,
											290000,
											530000,
											930000,
											1570000,
											2850000,
											2147483647	
										};

	const int affectionlevel[] =	{
										0,
										1000,
										3000,
										5000,
										7000,
										9000,
										11000,
										13000,
										15000,
										17000,
										2147483647
									};

	const int literarytalentlevel[] =	{
											0,
											300,
											1200,
											3900,
											12000,
											36300,
											109200,
											327900,
											984000,
											2952300,
											2147483647
										};

	const int teacherlevel[] =	{
									0,
									350,
									1500,
									4500,
									12000,
									30000,
									72000,
									168000,
									384000,
									864000,
									2147483647
								};

	const int battlescorelevel[] =	{
									0,
									400,
									800,
									1200,
									2000,
									4800,
									8800,
									14400,
									27200,
									44000,
									2147483647
								};

	const ACString strRegionReputationName[] =	{
													_AL("碌碌无名"),
													_AL("泯然众人"),
													_AL("脱颖而出"),
													_AL("锋芒毕露"),
													_AL("后生典范"),
													_AL("劳苦功高"),
													_AL("群雄咸服"),
													_AL("众望所归"),
													_AL("师门擎柱"),
													_AL("寂寞英雄")
												};

	const ACString strLoyaltyRepuationName[] =	{
													_AL("山野草民"),
													_AL("平民百姓"),
													_AL("懵懂孺子"),
													_AL("拳拳赤子"),
													_AL("慷慨志士"),
													_AL("磊落君子"),
													_AL("慷慨志士"),
													_AL("爱国义士"),
													_AL("精忠报国"),
													_AL("忠义英雄")													
												};

	const ACString strAffectionReputationName[] =	{
														_AL("情窦初开"),
														_AL("情心萌动"),
														_AL("情牵一线"),
														_AL("情真意切"),
														_AL("情深一往"),
														_AL("山盟海誓"),
														_AL("相濡以沫"),
														_AL("白头偕老"),
														_AL("生死相许"),
														_AL("海枯石烂")														
													};

	const ACString strLiteraryReputationName[] =	{
														_AL("目不识丁"),
														_AL("粗通文墨"),
														_AL("开蒙启智"),
														_AL("致学修文"),
														_AL("文不加点"),
														_AL("妙笔生花"),
														_AL("才惊四座"),
														_AL("学贯古今"),
														_AL("雅量高致"),
														_AL("文心雕龙")		
													};

	const ACString strBattleScoreReputationName[] =	{
														_AL("闻道仙童"),
														_AL("凡尘地仙"),
														_AL("逍遥散仙"),
														_AL("仙界天丁"),
														_AL("巡天星将"),
														_AL("妙法上仙"),
														_AL("八极龙君"),
														_AL("无量圣王"),
														_AL("元通祖师"),
														_AL("混沌天尊")	
													};

	int nLevel = 0;
	const int* currentLevels = regionreputationlevel;
	const int MAX_PROGRESS = 100;
	const ACString* currentReputationName = strRegionReputationName;
	const int MAX_REPUTATION_COUNT = 11;
	if ( 5 == nIndex )
	{
		currentReputationName = strLoyaltyRepuationName;
	}
	else if ( 6 == nIndex )
	{
		currentReputationName = strAffectionReputationName;
		currentLevels = affectionlevel;
	}
	else if ( 7 == nIndex )
	{
		currentReputationName = strLiteraryReputationName;
		currentLevels = literarytalentlevel;
	}
	else if ( 8 == nIndex )
	{
		currentLevels = teacherlevel;
	}
	else if ( 23 == nIndex )
	{
		currentLevels = battlescorelevel;
		currentReputationName = strBattleScoreReputationName;
	}
	
	while ( nReputation >= currentLevels[nLevel+1] && (nLevel < MAX_REPUTATION_COUNT-1) )
	{
		nLevel++;
	}

	strReputationName = currentReputationName[nLevel];
	int nNowReputation = nReputation - currentLevels[nLevel];
	int nMaxReputation = currentLevels[nLevel + 1] - currentLevels[nLevel];
	int nReputationProgress = (double(nNowReputation) / nMaxReputation)*MAX_PROGRESS;
	strReputationProgress.Format( _AL("%d"), nReputationProgress );
	strReputationValue.Format( _AL("%d/%d"), nReputation, currentLevels[nLevel+1] );
}

void CECRoleDescXMLParser::GetPlayerReputationList( const GNET::Octets& reputationInfo, abase::vector<int>& reputationList )
{
	using namespace GNET;
	//判断Octets的长度合法性
/*	if ( reputationInfo.size() < sizeof(int) )		//==================================Canceled!服务器那边传过来的结果发生变化了
	{
		a_LogOutput( 1, "Reputation list size Error! size=%d\n", reputationInfo.size() );
		return;
	}
	char* pDataInfo = (char*)(reputationInfo.begin());
	int nReputationListSize = *((int*)pDataInfo);	
	pDataInfo += sizeof(int);
	//检测所存的size和后面存的元素个数是否一致
	int nRealCount = (int*)reputationInfo.end() - (int*)pDataInfo;
	if ( nReputationListSize > nRealCount )
	{
		a_LogOutput( 1, "Reputation list size Error! ReputationListSize=%d, RealListSize=%d\n", nReputationListSize, nRealCount );
		return;
	}

	for ( int i = 0; i < nReputationListSize; ++i )
	{
		int nReputation = *((int*)pDataInfo);
		reputationList.push_back( nReputation );
		pDataInfo += sizeof(int);
	}
*/	//==================================================================================
	//整个数据块存储的是reputation数据项，因此直接算出元素个数
	int nRealCount = reputationInfo.size() / sizeof(int);
	int* pDataInfo = (int*)(reputationInfo.begin());
	for ( int i = 0; i < nRealCount; ++i )
	{
		int nReputation = pDataInfo[i];
		reputationList.push_back( nReputation );
	}
}

void CECRoleDescXMLParser::GetPlayerTitles( const GNET::Octets* TitlesInfo, abase::hash_map<int, bool>& titlesMap )
{
	using namespace GNET;
	//判断Octets的长度合法性
	//TitlesInfo 的结构是：size_t titlesinfosize, short* list
	if ( TitlesInfo->size() < sizeof(size_t) ) //至少有一个长度信息
	{
		a_LogOutput( 1, "Title list size Error! size=%d\n", TitlesInfo->size() );
		return;
	}
	char* dataInfo = (char*)(TitlesInfo->begin());
	size_t titlesListSize = *((size_t*)dataInfo);
	dataInfo += sizeof(size_t);
	//检测所存的size和后面存的元素个数是否一致
	short* pListEnd = (short*)(TitlesInfo->end());
	int nCount = pListEnd - (short*)dataInfo;
	if ( titlesListSize > nCount )
	{
		a_LogOutput( 1, "Title list size Error! titlesListSize=%d, RealTitlesListSize=%d\n", titlesListSize, nCount );
		return;
	}

	for ( size_t i = 0; i < titlesListSize; ++i )
	{
		int nTitleID = *((short*)dataInfo);
		titlesMap[nTitleID] = true;
		dataInfo += sizeof(short);
	}
	
}

ACString CECRoleDescXMLParser::GetTitle( int nTitleID )
{
	ACString strTitle = _AL("......");
	CConfigFromLua * pConfigFromLua = g_pGame->GetConfigFromLua();
	if( pConfigFromLua )
	{
		CConfigFromLua::TitleDefTab::iterator it = pConfigFromLua->m_TitleDef.find(nTitleID);
		if( it != pConfigFromLua->m_TitleDef.end() )
		{
			strTitle = *it.value();
			FormatTitle(strTitle);
			//GetHostPlayer()->FormatTitle(strTitle);
		}
	}
	//
	return strTitle;
}

void CECRoleDescXMLParser::FormatTitle(ACString & title)
{
	int nStart =0;
	int nVar = title.Find(_AL("$"), nStart);
	while( nVar >= 0 )
	{
		ACString strLeft = title.Left(nVar);
		ACString strRight = title.Right(title.GetLength() - nVar - 2);
		title.CutLeft(nVar + 1);
		wchar_t control = title.GetLength() ? title[0] : ' ';
		switch(control)
		{
		case L'M':
//			title = strLeft + m_strName + strRight;
//			nStart = strLeft.GetLength() + m_strName.GetLength();
			break;

		case L'S':
/*			if( GetSpouse() )
			{
				const wchar_t * szSpouseName = g_pGame->GetGameRun()->GetPlayerName(GetSpouse(), false);
				if( !szSpouseName )
				{
					static DWORD timeLast = 0;
					if( timeLast + 1000 < timeGetTime() )
					{
						int idPlayer = GetSpouse();
						g_pGame->GetGameSession()->GetPlayerBriefInfo(1, &idPlayer, 2);
						timeLast = timeGetTime();
					}
					title = strLeft + _AL("XXXX") + strRight;
					nStart = strLeft.GetLength() + 4;
					return false;
				}
				title = strLeft + szSpouseName + strRight;
				nStart = strLeft.GetLength() + a_strlen(szSpouseName);
			}
			else*/
			{
				title = strLeft + _AL("XXXX") + strRight; 
				nStart = strLeft.GetLength() + 4;
			}
			break;

		case L'T':
/*			if( GetMaster() )
			{
				const wchar_t * szMasterName = g_pGame->GetGameRun()->GetPlayerName(GetMaster(), false);
				if( !szMasterName )
				{
					static DWORD timeLast = 0;
					if( timeLast + 1000 < timeGetTime() )
					{
						int idPlayer = GetMaster();
						g_pGame->GetGameSession()->GetPlayerBriefInfo(1, &idPlayer, 2);
						timeLast = timeGetTime();
					}
					title = strLeft + _AL("XXXX") + strRight;
					nStart = strLeft.GetLength() + 4;
					return false;
				}
				title = strLeft + szMasterName + strRight;
				nStart = strLeft.GetLength() + a_strlen(szMasterName);
			}
			else*/
			{
				title = strLeft + _AL("XXXX") + strRight;
				nStart = strLeft.GetLength() + 4;
			}
			break;

		default:
			title = strLeft + strRight;
			nStart = strLeft.GetLength();
			break;
		}
		nVar = title.Find(_AL("$"), nStart);
	}	
}

ACString CECRoleDescXMLParser::GetDeityTitle( int nDeityLevel, char cCultivation )
{
	const ACString strTitleLevels[] =	{
										_AL("一阶"),
										_AL("二阶"),
										_AL("三阶"),
										_AL("四阶"),
										_AL("五阶"),
										_AL("六阶"),
										_AL("七阶"),
										_AL("八阶"),
										_AL("九阶"),
									};

	const ACString strTitleNames[] =	{
										_AL("散仙"),	//0
										_AL("地仙"),
										_AL("灵仙"),
										_AL("天仙"),
										_AL("玄仙"),
										_AL("金仙"),	//5
										_AL("真仙"),
										_AL("仙帝"),
										_AL("天尊"),
										_AL("散魔"),
										_AL("地魔"),	//10
										_AL("心魔"),
										_AL("天魔"),
										_AL("玄魔"),
										_AL("煞魔"),
										_AL("魔将"),	//15
										_AL("魔君"),
										_AL("魔皇"),
										_AL("散佛"),
										_AL("地佛"),
										_AL("灵佛"),	//20
										_AL("天佛"),
										_AL("玄佛"),
										_AL("光佛"),
										_AL("真佛"),
										_AL("佛主"),	//25
										_AL("佛祖"),
									};

	const int MAX_CULTIVATION = 8;		//!!!!!!!!目前最大值为8
	const int nMapedValue[MAX_CULTIVATION] =	{		//cCultivation值对应在strTitleNames数组中的部分Index
													-1,	//cCultivation = 0	无阵营
													0,	//cCultivation = 1	仙阵营
													1,	//cCultivation = 2	魔阵营
													-1,	//cCultivation = 3	仙魔阵营
													2,	//cCultivation = 4	佛阵营
													-1,	//cCultivation = 5	仙佛阵营
													-1,	//cCultivation = 6	魔佛阵营
													-1,	//cCultivation = 7	仙魔佛阵营
												};
	const int MAXLEVEL = 9;
	ACString strDeityTitle;
	if ( nDeityLevel <= 0 )
	{
		return _AL("未封神");
	}
	int nLevelIndex = (nDeityLevel-1)%MAXLEVEL;
	//LOG-------
	if ( cCultivation < 0 || cCultivation >= MAX_CULTIVATION || nMapedValue[cCultivation] == -1 )
	{
		a_LogOutput( 1, "Invalid Cultivation! cCultivation = %d\n", cCultivation );
		return _AL("非法值");
	}
	//LOG end---
	int nNameIndex = MAXLEVEL*nMapedValue[cCultivation]+(nDeityLevel - 1)/MAXLEVEL;	//10*cCultivation+(nDeityLevel+MAXLEVEL-1)/MAXLEVEL; Modified!!!
	if ( nLevelIndex < 0 || nLevelIndex >= MAXLEVEL || nNameIndex < 0 || nNameIndex >= MAXLEVEL*3 )
	{
		a_LogOutput( 1, "Error Deity array index nLevelIndex=%d, nNameIndex=%d!\n", nLevelIndex, nNameIndex );
		return _AL("");
	}
	strDeityTitle.Format( _AL("元神 %s%s"), strTitleLevels[nLevelIndex], strTitleNames[nNameIndex] );

	//
	return strDeityTitle;
}

ACString CECRoleDescXMLParser::GetTitleHint( int nTitleID )
{
	ACString strDesc = _AL("");

	CConfigFromLua * pConfigFromLua = g_pGame->GetConfigFromLua();
	if( pConfigFromLua )
	{
		CConfigFromLua::TitleDescTab::iterator it = pConfigFromLua->m_TitleDesc.find(nTitleID);
		if( it != pConfigFromLua->m_TitleDesc.end() )
		{
/*			if( !bOnlyDesc )
				strDesc += _AL("\r");*/
			AUICTranslate trans;
			strDesc += trans.Translate(*it.value());
		}
	}
	return strDesc;
}

AXMLItem* CECRoleDescXMLParser::CreateXMLItem( AXMLItem* pRootItem, AXMLItem* pParentItem, AXMLItem* pPrevSiblingItem, ACString* pListValues, int nListValuesSize )
{
	// 判断输入的合法性
	if ( !pListValues || !nListValuesSize || 0 != ((nListValuesSize - 1)%2) )	//传入的nListValuesSize个数必须是2*n+1个
	{
		a_LogOutput( 1, "Create XML item Error,Invalid parameters!\n" );
		if ( pRootItem )	//从传入的根节点开始删除整个XML结构树
		{
			ReleaseXMLItems( pRootItem );
		}
		return NULL;
	}

	//!!!!!!约定：pListValues数组的第一个元素为AXMLItem的name，之后的全部为<Key,Value>顺序!!!!!!
	AXMLItem* pCurrentItem = new AXMLItem;
	if ( !pCurrentItem )
	{
		a_LogOutput( 1, "Create <%s> Error,Allocate memory failed!\n", pListValues[0] );
		if ( pRootItem )	//从传入的根节点开始删除整个XML结构树
		{
			ReleaseXMLItems( pRootItem );
		}
		return NULL;
	}
	
	//将相应的属性信息值存入到AXMLItem中
	pCurrentItem->SetName( pListValues[0] );
	for ( int i = 1; i < nListValuesSize; i += 2 )
	{
		pCurrentItem->SetValue( pListValues[i],  pListValues[i+1]);
	}

	//将该AXMLItem插入到相应的位置
	if ( pParentItem || pPrevSiblingItem )
	{
		pCurrentItem->InsertItem( pParentItem, pPrevSiblingItem );
	}

	//
	return pCurrentItem;
}

void CECRoleDescXMLParser::GetProfMana( int nReputation, ACString& strValue, ACString& strProgress, ACString& strDesc )
{
	if ( nReputation < 0 )
	{
		return;
	}
	const int ReputationLevel[] =	{
										0,
										5000,
										25000,
										50000,
										100000,
										200000,
										500000,
										1000000,
										2000000,
										5000000,
										2147483647										
									};
	const ACString ReputationDesc[] =	{
											_AL("独善其身"),
											_AL("见义勇为"),
											_AL("行侠仗义"),
											_AL("义不容辞"),
											_AL("义薄云天"),
											_AL("舍生取义"),
											_AL("侠名远播"),
											_AL("威加四海"),
											_AL("兼济天下"),
											_AL("万世师表"),
										};
	const int MAX_VALUE = 100;
	const int MAX_REPUTATION_COUNT = 11;					

	//
	int nLevel = 0;
	while ( nReputation >= ReputationLevel[nLevel+1] && (nLevel < MAX_REPUTATION_COUNT-1) )
	{
		nLevel++;
	}

	int nNowReputation = nReputation - ReputationLevel[nLevel];
	int nMaxReputation = ReputationLevel[nLevel + 1] - ReputationLevel[nLevel];
	strValue.Format( _AL("%d/%d"), nReputation, ReputationLevel[nLevel + 1] );

	int nProgress = (double(nNowReputation) / nMaxReputation ) * MAX_VALUE;
	strProgress.Format( _AL("%d"), nProgress );

	strDesc = ReputationDesc[nLevel];

}

double CECRoleDescXMLParser::GetDeityLevelUpExp( int nDeityLevel )
{
	if ( nDeityLevel < 0 )
	{
		return 0.0;
	}

	DATA_TYPE dt;
	int idEssence = g_pGame->GetElementDataMan()->get_first_data_id(ID_SPACE_CONFIG, dt);
	while ( idEssence )
	{
		if ( dt == DT_PLAYER_LEVELEXP_CONFIG )
		{
			PLAYER_LEVELEXP_CONFIG* pPlayerLevelUpExp = (PLAYER_LEVELEXP_CONFIG*)g_pGame->GetElementDataMan()->get_data_ptr(idEssence, ID_SPACE_CONFIG, dt);
			if ( !pPlayerLevelUpExp )
			{
				a_LogOutput( 1, "Read DT_PLAYER_LEVELEXP_CONFIG (id = %d)Failed!\n", idEssence );
				return 0.0;
			}
			return pPlayerLevelUpExp->deity[nDeityLevel-1].deity_exp;
		}
		idEssence = g_pGame->GetElementDataMan()->get_next_data_id(ID_SPACE_CONFIG, dt);
	}

	//
	return 0.0;
}

ACString CECRoleDescXMLParser::GetClsName( int nClsID )
{
	const int nMaxClsSize = 128;
	static const ACString strClsName[nMaxClsSize] =	{
											_AL("人族少侠"),		//0
											_AL("鬼王宗 一重"), 
											_AL("鬼王宗 二重"), 
											_AL("鬼王宗 三重"), 
											_AL("合欢派 一重"), 
											_AL("合欢派 二重"),		//5
											_AL("合欢派 三重"),
											_AL("青云门 一重"),
											_AL("青云门 二重"),
											_AL("青云门 三重"),
											_AL("天音寺 一重"),		//10
											_AL("天音寺 二重"),
											_AL("天音寺 三重"),
											_AL("鬼王宗 四重"),
											_AL("鬼王宗 五重"),
											_AL(""),				//15
											_AL("合欢派 四重"),
											_AL("合欢派 五重"),
											_AL(""),
											_AL("青云门 四重"),
											_AL("青云门 五重"),		//20
											_AL(""),
											_AL("天音寺 四重"),
											_AL("天音寺 五重"),
											_AL(""),
											_AL("鬼道 一重"),		//25
											_AL("鬼道 二重"),
											_AL("鬼道 三重"),
											_AL("鬼道 四重"),
											_AL("鬼道 五重"),
											_AL(""),				//30
											_AL(""),
											_AL("神裔少侠"),
											_AL("九黎 一重"),
											_AL("九黎 二重"),
											_AL("九黎 三重"),		//35
											_AL("九黎 四重"),
											_AL("九黎 五重"),
											_AL(""),
											_AL("烈山 一重"),
											_AL("烈山 二重"),		//40
											_AL("烈山 三重"),
											_AL("烈山 四重"),
											_AL("烈山 五重"),
											_AL(""),
											_AL("怀光 一重"),		//45
											_AL("怀光 二重"),
											_AL("怀光 三重"),
											_AL("怀光 四重"),
											_AL("怀光 五重"),
											_AL(""),				//50
											_AL("天华 一重"),
											_AL("天华 二重"),
											_AL("天华 三重"),
											_AL("天华 四重"),
											_AL("天华 五重"),		//55
											_AL("辰皇 一重"),
											_AL("辰皇 二重"),
											_AL("辰皇 三重"),
											_AL("辰皇 四重"),
											_AL("辰皇 五重"),		//60
											_AL(""),
											_AL(""),
											_AL(""),
											_AL("焚香 一重"),
											_AL("焚香 二重"),		//65
											_AL("焚香 三重"),
											_AL("焚香 四重"),
											_AL("焚香 五重"),
											_AL(""),
											_AL(""),				//70
											_AL(""),
											_AL(""),
											_AL(""),
											_AL(""),
											_AL(""),				//75
											_AL(""),
											_AL(""),
											_AL(""),
											_AL(""),
											_AL(""),				//80
											_AL(""),
											_AL(""),
											_AL(""),
											_AL(""),
											_AL(""),				//85
											_AL(""),
											_AL(""),
											_AL(""),
											_AL(""),
											_AL(""),				//90
											_AL(""),
											_AL(""),
											_AL(""),
											_AL(""),
											_AL(""),				//95
											_AL("太昊 一重"),
											_AL("太昊 二重"),
											_AL("太昊 三重"),
											_AL("太昊 四重"),
											_AL("太昊 五重"),		//100
											_AL(""),
											_AL("牵机 一重"),
											_AL("牵机 二重"),
											_AL("牵机 三重"),
											_AL("牵机 四重"),				//105
											_AL("牵机 五重"),
											_AL(""),
											_AL("英招 一重"),
											_AL("英招 二重"),
											_AL("英招 三重"),				//110
											_AL("英招 四重"),
											_AL("英招 五重"),
											_AL(""),
											_AL(""),
											_AL(""),				//115
											_AL(""),
											_AL(""),
											_AL(""),
											_AL(""),
											_AL(""),				//120
											_AL(""),
											_AL(""),
											_AL(""),
											_AL(""),
											_AL(""),				//125
											_AL(""),
											_AL(""),
											};
	
	if ( nClsID >= 0 && nClsID < nMaxClsSize )
	{
		return strClsName[nClsID];
	}

	//
	return _AL("");
}

void CECRoleDescXMLParser::TranslateStr( const wchar_t* str, ACString& strDest )
{
	if( !str )
	{
		return;
	}

	strDest = str;
	const wchar_t * tmp = str; 
	wchar_t * dest = new wchar_t[wcslen(tmp) * 6 + 1];	//申请最大可能的字符串长度
	wchar_t * target = dest;
	while(*tmp) //一个字符串的形式为： "内容"  
	{  
		if ( *tmp == '\\' )
		{
			const wchar_t* tmpNext = tmp;
			tmpNext++;
			if ( *tmpNext && (*tmpNext == '"' || *tmpNext == '\'' ) )
			{
				tmp++;
				continue;
			}
		}
		switch(*tmp)
		{
			case '"': 
			case 8220:
			case 8221:
				*target ++ = '&';
				*target ++ = 'q';
				*target ++ = 'u';
				*target ++ = 'o';
				*target ++ = 't';
				*target ++ = ';';
				break;
			case '\'':
//			case '\\':				//需要么？？？？？
				*target ++ = '&';
				*target ++ = 'a';
				*target ++ = 'p';
				*target ++ = 'o';
				*target ++ = 's';
				*target ++ = ';';
				break;
			case '\n':
				*target ++ = '\\';
				*target ++ = 'n';
				break;
			case '\r':
				*target ++= '\\';
				*target ++ = 'r';
				break;
			case '\t':
				*target ++= '\\';
				*target ++ = 't';
				break;
			case '\v':
				*target ++= '\\';
				*target ++ = 'v';
				break;
			case '<':				//XML中特殊字符
				*target ++= '&';
				*target ++= 'l';
				*target ++= 't';
				*target ++= ';';
				break;
			case '>':				//XML中特殊字符
				*target ++= '&';
				*target ++= 'g';
				*target ++= 't';
				*target ++= ';';
				break;
			case '&':				//XML中特殊字符
				*target ++= '&';
				*target ++= 'a';
				*target ++= 'm';
				*target ++= 'p';
				*target ++= ';';
				break;
			default:
				*target ++ = *tmp;
				break;
		}
		tmp++;
	}
	*target = 0;
	strDest = dest;
	delete []dest;
}

