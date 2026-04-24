
#include "hashmap.h"
#include "AString.h"
#include "AAssist.h"

class AXMLItem;

namespace GNET
{
//	class RoleInfo;
	class Protocol;
	class GameClient;
	class Octets;
	class GConsignRole;
	class GSNSRole;
	class GPocketInfo;
}

//=============================================================================
enum
{
	ITEM_PROC_TYPE_BIND2		=	0x0040,		//装备即绑定的物品
	ITEM_PROC_TYPE_BIND			=	0x0080,		//是已经绑定的物品
	ITEM_PROC_TYPE_LOCK			=	0x80000000,	//锁定的物品
};

class CECRoleDescXMLParser
{
public:
	enum CLS_TYPE
	{
		CLS_GUIWANG = 0,	//鬼王
		CLS_HEHUAN,			//合欢
		CLS_QINGYUN,		//青云
		CLS_TIANYIN,		//天音
		CLS_GUIDAO,			//鬼道
		CLS_FENXIANG,		//焚香
		CLS_JIULI,			//九黎
		CLS_LIESHAN,		//烈山
		CLS_HUAIGUANG,		//怀光
		CLS_TIANHUA,		//天华
		CLS_TAIHAO,			//太昊
		CLS_CHENHUANG,		//辰皇
		CLS_LUOLI,			//萝莉
		CLS_RENMA,			//人马
		CLS_MAX,
		CLS_INVALID = -1,
	};

public:
	CECRoleDescXMLParser();
	~CECRoleDescXMLParser();
	const ACHAR* GetXMLText( GNET::GConsignRole* pDetailInfo );
	const ACHAR* GetXMLText( GNET::GSNSRole* pDetailInfo );

public:
	ACString GetClsName( int nClsID );
	ACString GetCultivationName( int nCultivation );
	void TranslateStr( const wchar_t* str, ACString& strDest );	//转换字符串中的特殊字符转义

protected:
	void ReleaseXMLItems( AXMLItem* pItem );
	void ReleaseItem( AXMLItem* pItem );
	AXMLItem* BuildRolePropertyXML( GNET::GConsignRole* pDetailInfo );
	AXMLItem* BuildRolePropertyXML( GNET::GSNSRole* pDetailInfo );
	AXMLItem* CreateXMLItem( AXMLItem* pRootItem, AXMLItem* pParentItem, AXMLItem* pPrevSiblingItem, ACString* pListValues, int nListValuesSize );
	ACString GetTitle( int nTitleID );
	ACString GetTitleHint( int nTitleID );
//	ACString GetClsName( int nClsID );
	void GetProfMana( int nReputation, ACString& strValue, ACString& strProgress, ACString& strDesc );
	double GetDeityLevelUpExp( int nDeityLevel );
	ACString GetDeityTitle( int nDeityLevel, char cCultivation );
	void GetPlayerTitles( const GNET::Octets* TitlesInfo, abase::hash_map<int, bool>& titlesMap );
	void GetPlayerReputationList( const GNET::Octets& reputationInfo, abase::vector<int>& reputationList );
	void GetReputationStringValues( int nReputation, int nIndex, ACString& strReputationName, ACString& strReputationValue, ACString& strReputationProgress );
	double GetLevelUpExp( int nLevel, int nRebornCount );
	
	// Build Skill items
	AXMLItem* BuildRoleSkillsXML( GNET::GConsignRole* pDetailInfo );
	AXMLItem* BuildRoleSkillsXML( GNET::GSNSRole* pDetailInfo );

	// iCLS:职业（不考虑重数）
	bool BuildSkillNatureTypeItem( AXMLItem* pRootItem, AXMLItem* pParentItem, int iCLS, abase::hash_map<int, int>& skillsIDList, abase::vector<int> reputationList);
	bool BuildSkillGodSkillItem( AXMLItem* pRootItem, AXMLItem* pParentItem, int iCLS, abase::hash_map<int, int>& skillsIDList, int iDeityExp);
	bool BuildSkillMaster( AXMLItem* pRootItem, AXMLItem* pParentItem, int iCLS, abase::hash_map<int, int>& skillsIDList, abase::vector<int> reputationList);
	bool BuildSkillOther( AXMLItem* pRootItem, AXMLItem* pParentItem, int iCLS, abase::hash_map<int, int>& skillsIDList);
	bool BuildSkillProfItem( AXMLItem* pRootItem, AXMLItem* pParentItem, int iCLS, abase::hash_map<int, int>& skillsIDList);
	bool BuildSkillSpouseItem( AXMLItem* pRootItem, AXMLItem* pParentItem, abase::hash_map<int, int>& skillsIDList);
	AXMLItem* CreateSkillXMLItem( AXMLItem* pRootItem, AXMLItem* pParentItem, AXMLItem* pPrevSiblingItem, int iSkillId, int iSkillLv);


	void GetSkillsInfo( const GNET::Octets& skillsData, abase::hash_map<int, int>& skillsIDList );
	void GetProfSkillInfo( const abase::hash_map<int, int>& skillsIDList, int nClsType, int iLevel, abase::vector<int>& profSkillIDList);
	void GetTalentSkillInfo( const abase::hash_map<int, int>& skillsIDList, int nClsType, int iLevel, abase::vector<int>& profTalentIDList);
	void GetNatureSkillInfo( const abase::hash_map<int, int>& skillsIDList, int nNatureType, int nNatureSkillType, int nClsType, abase::vector<int>& natureSkillIDList );
	int GetClsTypeByProfID( int nClsType );
	void GetDeitySkillInfo( const abase::hash_map<int, int>& skillsIDList, int nDeityType, int nClsType, abase::vector<int>& deitySkillIDList );
	void GetMasterApprenticeSkillInfo( const abase::hash_map<int, int>& skillsIDList, abase::vector<int>& deitySkillIDList );
	void GetOtherSkillInfo( const abase::hash_map<int, int>& skillsIDList, int nOtherSkillType, abase::vector<int>& deitySkillIDList );
	AXMLItem* BuildRoleInventoryXML( GNET::GConsignRole* pDetailInfo );
	AXMLItem* BuildRoleInventoryXML( GNET::GSNSRole* pDetailInfo );
	AXMLItem* BuildRolePetsXML( GNET::GConsignRole* pDetailInfo );
	AXMLItem* BuildRolePetsXML( GNET::GSNSRole* pDetailInfo );
	AXMLItem* CreateRolePetXML( GNET::GConsignRole* pDetailInfo, int nPetIndex );
	AXMLItem* CreateRolePetXML( GNET::GSNSRole* pDetailInfo, int nPetIndex );
	double GetPetLevelUpExp( int nLevel );
	ACString GetItemText( AXMLItem* pItem );
	ACString BuildItemText( ACString strTab, AXMLItem *pItem );
//	ACString GetCultivationName( int nCultivation );
	int GetPetAddInfoIndex( GNET::GConsignRole* pDetailInfo, int nPetIndex );	//不满足时返回-1
	int GetPetAddInfoIndex( GNET::GSNSRole* pDetailInfo, int nPetIndex );	//不满足时返回-1
	void FormatTitle(ACString & title); //处理称号的特殊信息
	inline int Proctype2State( int nProcType )
	{
		int nState = 0;
		if ( nProcType & ITEM_PROC_TYPE_BIND )
		{
			nState = 0x001;
		}
		if ( nProcType & ITEM_PROC_TYPE_BIND2 )
		{
			nState |= 0x002;
		}
		if ( nProcType & ITEM_PROC_TYPE_LOCK )
		{
			nState |= 0x004;
		}

		return nState;
	}

protected:
	AXMLItem* m_pRootItem;
	ACString  m_strText;

private:
	const static int ms_iProfSkillLvCount;		// 职业重数
	const static int ms_iProfTalentLvCount;		// 天书类型
	const static int ms_iProfSkillCountPerLv;	// 职业技能每一重、天书技能每一类最多数量

	const static int BASIC_SKILL_COUNT;
	const static int GENERAL_SKILL_COUNT;
	const static int SECT_SKILL_COUNT;
	const static int PLAYER_CLS_COUNT;	//!!!!!!新加职业，需要扩展!!!!!!
	const static int NATURE_TYPE_COUNT;
	const static int NATURE_TYPE_SECT;
};

