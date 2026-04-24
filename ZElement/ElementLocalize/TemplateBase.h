// TemplateBase.h: interface for the CTemplateBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEMPLATEBASE_H__18E3605B_B806_4CE0_9753_0D6CD895AE11__INCLUDED_)
#define AFX_TEMPLATEBASE_H__18E3605B_B806_4CE0_9753_0D6CD895AE11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector.h>
#include "TextBase.h"
#include "DataManager.h"

struct INTER_HINT
{
	TCHAR szIndex[1024];
	TCHAR szText[10000];
};

struct INTER_STRING
{
	int id;
	TCHAR szIndex[1024];
	TCHAR szText[10000];
};

struct FILE_DATA
{
	TCHAR szIndex[1024];
	TCHAR szText[1024*500];//500k 最大支持
};

class CTemplateBase  
{
public:
	CTemplateBase();
	virtual ~CTemplateBase();
	virtual CTemplateBase * Clone() = 0;
public:
	virtual bool Load(FILE *pFile, DWORD dwVersion) = 0;
	virtual bool Save(FILE *pFile);
	
	virtual bool Import(void *pOrgData) = 0;
	virtual bool Export(void *pOrgData) = 0;

	void SetID( unsigned int uid ){ id = uid; }
	unsigned int GetID(){ return id; }

	int GetTemplateType(){ return template_type; }

	int GetTextNum(){ return listText.size(); }
	CTextBase* GetTextByIdx( int idx)
	{
		if(idx >=0 && idx < listText.size() )
			return listText[idx];
		return NULL;
	}

	void AddChild(CTemplateBase *pChild)
	{
		pChild->SetParent(this, listChild.size()); 
		pChild->SetChunkType(iChunkType);
		listChild.push_back(pChild); 
	}

	int GetChildNum(){ return listChild.size(); }
	CTemplateBase *GetChildByIndex(int idx){ return listChild[idx]; }
	CTemplateBase *GetChildById(int uid)
	{
		for( int i = 0; i < listChild.size(); ++i)
		{
			if(listChild[i]->GetID() == uid) return listChild[i];
		}
		return NULL;
	}
	
	void SetChildToNull(int idx)
	{
		listChild[idx] = NULL;
	}

	void SetTextToNull( int idx)
	{
		listText[idx] = NULL;
	}

	virtual TCHAR*GetChildName(){ return NULL; }
	
	
	void AddText( CTextBase * pNewText){pNewText->SetStrIDInfo(this, listText.size()); listText.push_back(pNewText); }

	virtual CString GetStrID();
	void SetParent(CTemplateBase* pParent, int idx){pTempParent = pParent; index = idx;}
	void SetChunkType(int iChunk){iChunkType = iChunk;}

protected:
	abase::vector<CTextBase *> listText;
	abase::vector<CTemplateBase*>listChild;
	unsigned int id;//对应对象的ID
	int          template_type;

	CTemplateBase*	pTempParent;
	int				index;
	int				iChunkType;
};

//(DT_CHANGE_SHAPE_CARD_ESSENCE)
class CTemplate_ElementChangeShapeCardEssence : public CTemplateBase
{
public:
	CTemplate_ElementChangeShapeCardEssence(): CTemplateBase(){};
	CTemplate_ElementChangeShapeCardEssence(int nttype): CTemplateBase(){ template_type = nttype; }
	virtual ~CTemplate_ElementChangeShapeCardEssence();

	virtual CTemplateBase * Clone()
	{
		return new CTemplate_ElementChangeShapeCardEssence(template_type);
	}

public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Save(FILE *pFile);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
};

//(DT_ITEM_TRADE_CONFIG)
class CTemplate_ElementItemTrade : public CTemplateBase
{
public:
	CTemplate_ElementItemTrade(): CTemplateBase(){};
	CTemplate_ElementItemTrade(int nttype): CTemplateBase(){ template_type = nttype; }
	virtual ~CTemplate_ElementItemTrade();

	virtual CTemplateBase * Clone()
	{
		return new CTemplate_ElementItemTrade(template_type);
	}

public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Save(FILE *pFile);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
};

//(DT_INSTANCING_BATTLE_CONFIG)
class CTemplate_ElementInstancingBattleConfig : public CTemplateBase
{
public:
	CTemplate_ElementInstancingBattleConfig(): CTemplateBase(){};
	CTemplate_ElementInstancingBattleConfig(int nttype): CTemplateBase(){ template_type = nttype; }
	virtual ~CTemplate_ElementInstancingBattleConfig();

	virtual CTemplateBase * Clone()
	{
		return new CTemplate_ElementInstancingBattleConfig(template_type);
	}

public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Save(FILE *pFile);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
};


//这个类针对护身符(TALISMAN_MAINPART_ESSENCE)
class CTemplate_ElementTalisman : public CTemplateBase
{
public:
	CTemplate_ElementTalisman(): CTemplateBase(){};
	CTemplate_ElementTalisman(int nttype): CTemplateBase(){ template_type = nttype; }
	virtual ~CTemplate_ElementTalisman();

	virtual CTemplateBase * Clone()
	{
		return new CTemplate_ElementTalisman(template_type);
	}

public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Save(FILE *pFile);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
};

//全服产出限量配置表
class CTemplate_ElementDropInternalConfig : public CTemplateBase
{
public:
	CTemplate_ElementDropInternalConfig() : CTemplateBase() {};
	CTemplate_ElementDropInternalConfig(int nttype) : CTemplateBase() { template_type = nttype; }
	virtual ~CTemplate_ElementDropInternalConfig();

	virtual CTemplateBase * Clone()
	{
		return new CTemplate_ElementDropInternalConfig(template_type);
	}
	
public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Save(FILE *pFile);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
};

//碰撞副本变身配置表
class CTemplate_ElementCollRaidTransfigConfig : public CTemplateBase
{
public:
	CTemplate_ElementCollRaidTransfigConfig() : CTemplateBase() {};
	CTemplate_ElementCollRaidTransfigConfig(int nttype) : CTemplateBase() { template_type = nttype; }
	virtual ~CTemplate_ElementCollRaidTransfigConfig();
	
	virtual CTemplateBase * Clone()
	{
		return new CTemplate_ElementCollRaidTransfigConfig(template_type);
	}
	
public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Save(FILE *pFile);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
};

// 声望商店服务
class CTemplate_ElementNpcRepShopSvc : public CTemplateBase
{
public:
	CTemplate_ElementNpcRepShopSvc() : CTemplateBase() {};
	CTemplate_ElementNpcRepShopSvc(int nttype) : CTemplateBase() { template_type = nttype; }
	
	virtual CTemplateBase * Clone()
	{
		return new CTemplate_ElementNpcRepShopSvc(template_type);
	}
	
public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
};

//上古世界传送服务配置
class CTemplate_ElementUITransferCfg : public CTemplateBase
{
public:
	CTemplate_ElementUITransferCfg() : CTemplateBase() {};
	CTemplate_ElementUITransferCfg(int nttype) : CTemplateBase() { template_type = nttype; }
	
	virtual CTemplateBase * Clone()
	{
		return new CTemplate_ElementUITransferCfg(template_type);
	}
	
public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
};

class CTemplate_ElementLivenessCfg : public CTemplateBase
{
public:
	CTemplate_ElementLivenessCfg() : CTemplateBase() {};
	CTemplate_ElementLivenessCfg(int nttype) : CTemplateBase() { template_type = nttype; }
	
	virtual CTemplateBase * Clone()
	{
		return new CTemplate_ElementLivenessCfg(template_type);
	}
	
public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
};

class CTemplate_ElementSalePromotionItem : public CTemplateBase
{
public:
	CTemplate_ElementSalePromotionItem() : CTemplateBase() {};
	CTemplate_ElementSalePromotionItem(int nttype) : CTemplateBase() { template_type = nttype; }
	
	virtual CTemplateBase * Clone()
	{
		return new CTemplate_ElementSalePromotionItem(template_type);
	}
	
public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
};

class CTemplate_ElementTaskSpecialAwardCfg : public CTemplateBase
{
public:
	CTemplate_ElementTaskSpecialAwardCfg() : CTemplateBase() {};
	CTemplate_ElementTaskSpecialAwardCfg(int nttype) : CTemplateBase() { template_type = nttype; }
	
	virtual CTemplateBase * Clone()
	{
		return new CTemplate_ElementTaskSpecialAwardCfg(template_type);
	}
	
public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
};

//这个类是针对很难提取文字的文本文件而单独处理特殊文本文件，比如逻辑脚本文件
//这个类针对文字索引的特殊情况，没有ID，主要用于界面接口
class CTemplate_ElementTextFile : public CTemplateBase
{
public:
	CTemplate_ElementTextFile(): CTemplateBase()
	{
		template_type = 303030; 
		m_nIndexLen = 0;
		m_pIndex = NULL;
	}
    virtual ~CTemplate_ElementTextFile();

	virtual CTemplateBase * Clone()
	{
		return new CTemplate_ElementTextFile();
	}

public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Save(FILE *pFile);

	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
	
	TCHAR *GetIndexText(){ return m_pIndex; }

	virtual CString GetStrID()
	{
		CString str;
		str.Format(_T("%d%s%s"), iChunkType, STRID_TOK, m_pIndex);
		str.MakeLower();
		return str;
	}

private:
	TCHAR *m_pIndex;
	int    m_nIndexLen;
};

//这个类针对武林NPC传送(NPC_TRANSMIT_SERVICE)
class CTemplate_ElementNpcTransmit : public CTemplateBase
{
public:
	CTemplate_ElementNpcTransmit(): CTemplateBase(){};
	CTemplate_ElementNpcTransmit(int nttype): CTemplateBase(){ template_type = nttype; }
	virtual ~CTemplate_ElementNpcTransmit();

	virtual CTemplateBase * Clone()
	{
		return new CTemplate_ElementNpcTransmit(template_type);
	}

public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Save(FILE *pFile);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
};

//这个类针对武林文字烟花(FIREWORKS)
class CTemplate_ElementFireworks : public CTemplateBase
{
public:
	CTemplate_ElementFireworks(): CTemplateBase(){};
	CTemplate_ElementFireworks(int nttype): CTemplateBase(){ template_type = nttype; }
	virtual ~CTemplate_ElementFireworks();

	virtual CTemplateBase * Clone()
	{
		return new CTemplate_ElementFireworks(template_type);
	}

public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Save(FILE *pFile);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
};

//这个类针对彩票(LOTTERY)
class CTemplate_ElementLottery : public CTemplateBase
{
public:
	CTemplate_ElementLottery(): CTemplateBase(){};
	CTemplate_ElementLottery(int nttype): CTemplateBase(){ template_type = nttype; }
	virtual ~CTemplate_ElementLottery();

	virtual CTemplateBase * Clone()
	{
		return new CTemplate_ElementLottery(template_type);
	}

public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Save(FILE *pFile);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
};

//这个类针对彩票(LOTTERY2)
class CTemplate_ElementLottery2 : public CTemplateBase
{
public:
	CTemplate_ElementLottery2(): CTemplateBase(){};
	CTemplate_ElementLottery2(int nttype): CTemplateBase(){ template_type = nttype; }
	virtual ~CTemplate_ElementLottery2();

	virtual CTemplateBase * Clone()
	{
		return new CTemplate_ElementLottery2(template_type);
	}

public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Save(FILE *pFile);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
};

//这个类针对宋金战场积分与军衔的对应表(SCORE_TO_RANK_CONFIG)
class CTemplate_ElementScoreToRank : public CTemplateBase
{
public:
	CTemplate_ElementScoreToRank(): CTemplateBase(){};
	CTemplate_ElementScoreToRank(int nttype): CTemplateBase(){ template_type = nttype; }
	virtual ~CTemplate_ElementScoreToRank();

	virtual CTemplateBase * Clone()
	{
		return new CTemplate_ElementScoreToRank(template_type);
	}

public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Save(FILE *pFile);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
};

//这个类针武林装备(EQUIPMENT)
class CTemplate_ElementEquipment : public CTemplateBase
{
public:
	CTemplate_ElementEquipment(): CTemplateBase(){};
	CTemplate_ElementEquipment(int nttype): CTemplateBase(){ template_type = nttype; }
	virtual ~CTemplate_ElementEquipment();

	virtual CTemplateBase * Clone()
	{
		return new CTemplate_ElementEquipment(template_type);
	}

public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Save(FILE *pFile);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
};

//这个类针对的是百宝格子
class CTreasureTemplate : public CTemplateBase
{
public:
	CTreasureTemplate(): CTemplateBase(){};
	virtual ~CTreasureTemplate();

	virtual CTemplateBase * Clone()
	{
		return new CTreasureTemplate();
	}

public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Save(FILE *pFile);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
};

//这个类针怪物本体
class CTemplate_ElementMonster : public CTemplateBase
{
public:
	CTemplate_ElementMonster(): CTemplateBase(){};
	CTemplate_ElementMonster(int nttype): CTemplateBase(){ template_type = nttype; }
	virtual ~CTemplate_ElementMonster();

	virtual CTemplateBase * Clone()
	{
		return new CTemplate_ElementMonster(template_type);
	}

public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Save(FILE *pFile);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
};


//这个类针对NPC出售商品的服务
class CTemplate_ElementNpcSell : public CTemplateBase
{
public:
	CTemplate_ElementNpcSell(): CTemplateBase(){};
	CTemplate_ElementNpcSell(int nttype): CTemplateBase(){ template_type = nttype; }
	virtual ~CTemplate_ElementNpcSell();

	virtual CTemplateBase * Clone()
	{
		return new CTemplate_ElementNpcSell(template_type);
	}

public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Save(FILE *pFile);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
};


//这个类针NPC本体
class CTemplate_ElementNpc : public CTemplateBase
{
public:
	CTemplate_ElementNpc(): CTemplateBase(){};
	CTemplate_ElementNpc(int nttype): CTemplateBase(){ template_type = nttype; }
	virtual ~CTemplate_ElementNpc();

	virtual CTemplateBase * Clone()
	{
		return new CTemplate_ElementNpc(template_type);
	}

public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Save(FILE *pFile);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
};

class CTemplate_ElementPetArmor : public CTemplateBase
{
public:
	CTemplate_ElementPetArmor(): CTemplateBase(){};
	CTemplate_ElementPetArmor(int nttype): CTemplateBase(){ template_type = nttype; }
	virtual ~CTemplate_ElementPetArmor();

	virtual CTemplateBase * Clone()
	{
		return new CTemplate_ElementPetArmor(template_type);
	}

public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Save(FILE *pFile);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
};

class CTemplate_ElementPetBedge : public CTemplateBase
{
public:
	CTemplate_ElementPetBedge(): CTemplateBase(){};
	CTemplate_ElementPetBedge(int nttype): CTemplateBase(){ template_type = nttype; }
	virtual ~CTemplate_ElementPetBedge();

	virtual CTemplateBase * Clone()
	{
		return new CTemplate_ElementPetBedge(template_type);
	}

public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Save(FILE *pFile);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
};

class CTemplate_ElementScrollRegionConfig : public CTemplateBase
{
public:
	CTemplate_ElementScrollRegionConfig(): CTemplateBase(){};
	CTemplate_ElementScrollRegionConfig(int nttype): CTemplateBase(){ template_type = nttype; }
	virtual ~CTemplate_ElementScrollRegionConfig();
	
	virtual CTemplateBase * Clone()
	{
		return new CTemplate_ElementScrollRegionConfig(template_type);
	}
	
public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Save(FILE *pFile);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
};

//这个类针对话过程
class CTemplate_ElementTalk : public CTemplateBase
{
public:
	CTemplate_ElementTalk(): CTemplateBase(){};
	CTemplate_ElementTalk(int nttype): CTemplateBase(){ template_type = nttype; }
	virtual ~CTemplate_ElementTalk();

	virtual CTemplateBase * Clone()
	{
		return new CTemplate_ElementTalk(template_type);
	}

public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Save(FILE *pFile);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);

	void SetStatus(int status);
protected:
	CTemplateBase *FindTalkOption(unsigned int wndid);
};

//这个类针对话Options
class CTemplate_ElementTalkOptions : public CTemplateBase
{
public:
	CTemplate_ElementTalkOptions(): CTemplateBase(){};
	CTemplate_ElementTalkOptions(int nttype): CTemplateBase(){ template_type = nttype; }
	virtual ~CTemplate_ElementTalkOptions();

	virtual CTemplateBase * Clone()
	{
		return new CTemplate_ElementTalkOptions(template_type);
	}

public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Save(FILE *pFile);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
	
	void SetStatus(int ns);
};

//这个类针对客户端的描述类文字
class CTemplate_ElementStringTab : public CTemplateBase
{
public:
	CTemplate_ElementStringTab(): CTemplateBase(){};
virtual ~CTemplate_ElementStringTab();

	virtual CTemplateBase * Clone()
	{
		return new CTemplate_ElementStringTab();
	}

public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Save(FILE *pFile);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
};

//这个类针对客户端的instance.txt
class CTemplate_ElementInstance : public CTemplateBase
{
public:
	CTemplate_ElementInstance(): CTemplateBase(){};
virtual ~CTemplate_ElementInstance();

	virtual CTemplateBase * Clone()
	{
		return new CTemplate_ElementInstance();
	}

public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Save(FILE *pFile);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
};

//这个类针对客户端的消息提示提示文字
class CTemplate_ElementMessage : public CTemplateBase
{
public:
	CTemplate_ElementMessage(): CTemplateBase(){};
virtual ~CTemplate_ElementMessage();

	virtual CTemplateBase * Clone()
	{
		return new CTemplate_ElementMessage();
	}

public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Save(FILE *pFile);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
};

//这个类针对文字索引的特殊情况，没有ID，主要用于界面接口
class CTemplate_ElementInterfaceHint : public CTemplateBase
{
public:
	CTemplate_ElementInterfaceHint(): CTemplateBase()
	{
		template_type = 202020; 
		m_nIndexLen = 0;
		m_pIndex = NULL;
	}
    virtual ~CTemplate_ElementInterfaceHint();

	virtual CTemplateBase * Clone()
	{
		return new CTemplate_ElementInterfaceHint();
	}

public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Save(FILE *pFile);

	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
	
	TCHAR *GetIndexText(){ return m_pIndex; }

	virtual CString GetStrID()
	{
		CString str;
		str.Format(_T("%d%s%s"), iChunkType, STRID_TOK, m_pIndex);
		str.MakeLower();
		return str;
	}

private:
	TCHAR *m_pIndex;
	int    m_nIndexLen;
};


//这个类针对客户端的提示文字
class CTemplate_ElementInterfaceString : public CTemplateBase
{
public:
	CTemplate_ElementInterfaceString(): CTemplateBase(){};
virtual ~CTemplate_ElementInterfaceString();

	virtual CTemplateBase * Clone()
	{
		return new CTemplate_ElementInterfaceString();
	}

public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Save(FILE *pFile);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
	TCHAR * GetIndexStr(){ return m_pIndex; }

	virtual CString GetStrID()
	{
		CString str;
		str.Format(_T("%d%s%s"), iChunkType, STRID_TOK, m_pIndex);
		str.MakeLower();
		return str;
	}
private:
	TCHAR *m_pIndex;
	int    m_nIndexLen;
};


//这一类模板对象只有名字需要被译文替换
template <typename T>
class CTemplate_ElementName: public CTemplateBase
{
public:
	CTemplate_ElementName();
	CTemplate_ElementName(int nttype);
	virtual ~CTemplate_ElementName();

	virtual CTemplateBase * Clone()
	{
		return new CTemplate_ElementName<T>(template_type);
	}

public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Save(FILE *pFile);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
};

template <typename T>
CTemplate_ElementName<T>::CTemplate_ElementName() : CTemplateBase()
{
}

template <typename T>
CTemplate_ElementName<T>::CTemplate_ElementName(int nttype) : CTemplateBase()
{
	template_type = nttype;
}

template <typename T>
CTemplate_ElementName<T>::~CTemplate_ElementName()
{
	for( int i = 0; i < listText.size(); ++i)
		if(listText[i]) delete listText[i];
	listText.clear();
}

template <typename T>
bool CTemplate_ElementName<T>::Load(FILE *pFile, DWORD dwVersion)
{
	fread(&id,sizeof(unsigned int),1,pFile);
	
#ifdef _DEBUG
	CString debug;
	debug.Format(_T("id = %d\n"), id);
	TRACE(debug);
#endif
	
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		CTextBase *pNewBase = new CText_Normal;
		pNewBase->Init(32,false,NULL,NULL,_T("Name"));
		bool r = pNewBase->Load(pFile,dwVersion);
		if(!r) return false;
		AddText(pNewBase);
	}
	return true;
}

template <typename T>
bool CTemplate_ElementName<T>::Save(FILE *pFile)
{
	fwrite(&id,sizeof(unsigned int),1,pFile);
	DWORD size = listText.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listText[i]->Save(pFile);	
		if(!r) return false;
	}
	return true;
}
	
template <typename T>
bool CTemplate_ElementName<T>::Import(void *pOrgData)
{
	
	T * pData = (T *) pOrgData;
	id = pData->id;

#ifdef _DEBUG	
	CString debug;
	debug.Format(_T("id = %d\n"), id);
	TRACE(debug);
#endif

	if(listText.size() > 0)
	{// 比较md5，并进行更新，同时判断更新标志
	   char * pOrg = (char*)listText[0]->GetOriginalText();	
	   int len = listText[0]->GetCharUpperLimit();
	   if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->name,sizeof(TCHAR)*len))
	   {
		  listText[0]->ReplaceOriginalText(pData->name); 
		  listText[0]->SetStatus(CTextBase::status_new);
	   }
	   else
		{
		   CTextBase *pText = listText[0];
		   TCHAR* pStr = pText->GetTranslationText();
		   int nTranslationTextLen = pStr?_tcslen(pStr):0;
			if (nTranslationTextLen > 0)
			{
				pText->SetStatus(CTextBase::status_normal);
			}
		}
	}else
	{
		CTextBase * pText = new CText_Normal();
		pText->Init(32,false,pData->name, NULL,_T("Name"));
		AddText(pText);
	}
	return true;
}

template <typename T>
bool CTemplate_ElementName<T>::Export(void *pOrgData)
{
	
	if( listText[0]->GetStatus() == CTextBase::status_normal && listText[0]->GetTranslationText()!=NULL)
	{
		T * pData = (T *) pOrgData;
		lstrcpy(pData->name, listText[0]->GetTranslationText());
	}
	return true;
}

#endif // !defined(AFX_TEMPLATEBASE_H__18E3605B_B806_4CE0_9753_0D6CD895AE11__INCLUDED_)
