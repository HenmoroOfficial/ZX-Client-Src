#include "AnmCommonActionManager.h"
#include <AIniFile.h>

const ACString PHYSIQUE_NORMAL_KEY = _AL("PHYSIQUE_NORMAL");
const ACString PHYSIQUE_SMALL_KEY = _AL("PHYSIQUE_SMALL");
const ACString PHYSIQUE_MIDDLE_KEY = _AL("PHYSIQUE_MIDDLE");
const ACString PHYSIQUE_BIG_MALE_KEY = _AL("PHYSIQUE_BIG");
const AString COMMON_ACTION_INI_PATH = "configs\\animations\\anm.ini";

enum PHYSIQUE_TYPE
{
	PHYSIQUE_NORMAL = 0,	// 正常体型
	PHYSIQUE_SMALL,			// 小体型
	PHYSIQUE_MIDDLE,		// 中体型
	PHYSIQUE_BIG,           // 大体型

	PHYSIQUE_COUNT,
};


int GetPhysiqueType(const ACString& physiqueName) 
{
	if (physiqueName.Compare(PHYSIQUE_SMALL_KEY) == 0)
		return PHYSIQUE_SMALL;
	else if (physiqueName.Compare(PHYSIQUE_MIDDLE_KEY) == 0)
		return PHYSIQUE_MIDDLE;
	else if (physiqueName.Compare(PHYSIQUE_BIG_MALE_KEY) == 0)
		return PHYSIQUE_BIG;
	return PHYSIQUE_NORMAL;
}

const ACString& GetPhysiqueName(int type)
{
	switch(type)
	{
	case PHYSIQUE_SMALL:  return PHYSIQUE_SMALL_KEY;
	case PHYSIQUE_MIDDLE: return PHYSIQUE_MIDDLE_KEY;
	case PHYSIQUE_BIG:	  return PHYSIQUE_BIG_MALE_KEY;
	default: return PHYSIQUE_NORMAL_KEY;
	}
}

//////////////////////////////////////////////////////////////////////////

CAnmCommonActionManager::CAnmCommonActionManager()
: m_bEnable(false), m_iHostPlayerSect(-1)
{
	Init();
}

CAnmCommonActionManager::~CAnmCommonActionManager()
{
	Release();
}

// 初始化, 加载ini
void CAnmCommonActionManager::Init()
{
	AIniFile iniFile;
	if (!iniFile.Open(COMMON_ACTION_INI_PATH))
		return;

	int nSectCount = iniFile.GetValueAsInt("SECT_TYPE", "Count", 0);
	if (nSectCount <= 0)
		return;

	int i;
	AString szKey, szValue;
	for (i = 1; i <= nSectCount; ++ i)
	{
		szKey.Format("SECT%d", i);
		szValue = iniFile.GetValueAsString("SECT_TYPE", szKey);
		m_vecSectNames.push_back(AS2WC(szValue));

		szValue = iniFile.GetValueAsString("SECT_PHYSIQUE", szKey);
		ACString szPhysique = AS2WC(szValue);
		m_sectToMalePhysiqueMap[i-1] = GetPhysiqueType(szPhysique);
		m_sectToFemalePhysiqueMap[i-1] = GetPhysiqueType(szPhysique);
	}

	for (i = PHYSIQUE_NORMAL; i < PHYSIQUE_COUNT; ++ i)
	{
		szKey = WC2AS(GetPhysiqueName(i));
		szValue = iniFile.GetValueAsString("MALE_PHYSIQUE_ECM", szKey);
		m_malePhysiqueECMs.push_back(AS2WC(szValue));

		szValue = iniFile.GetValueAsString("FEMALE_PHYSIQUE_ECM", szKey);
		m_femalePhysiqueECMs.push_back(AS2WC(szValue));
	}

	abase::vector<AString> tempActionNames;
	int nActionCount = iniFile.GetValueAsInt("COMMON_ACTION", "COUNT", 0);
	for (i = 1; i <= nActionCount; ++ i)
	{
		szKey.Format("ACTION%d", i);
		szValue = iniFile.GetValueAsString("COMMON_ACTION", szKey);
		tempActionNames.push_back(szValue);
		m_vecCommonActionNames.push_back(AS2WC(szValue));
	}

	AString szSection;
	int j;
	for (i = 1; i <= nSectCount; ++ i)
	{
		CommonToRealActionMap* pActionMap = new CommonToRealActionMap();
		CommonToRealActionMap& actionMap = *pActionMap;
		szSection.Format("SECT%d_MALE_ACTION", i);
		for (j = 0; j < nActionCount; ++ j)
		{
			const AString& commonName = tempActionNames[j];
			szValue = iniFile.GetValueAsString(szSection, commonName);
			actionMap[AS2WC(commonName)] = AS2WC(szValue);
		}
		m_vecMaleSectActionMap.push_back(pActionMap);

		pActionMap = new CommonToRealActionMap();
		CommonToRealActionMap& actionMap2 = *pActionMap;
		szSection.Format("SECT%d_FEMALE_ACTION", i);
		for (j = 0; j < nActionCount; ++ j)
		{
			const AString& commonName = tempActionNames[j];
			szValue = iniFile.GetValueAsString(szSection, commonName);
			actionMap2[AS2WC(commonName)] = AS2WC(szValue);
		}
		m_vecFemaleSectActionMap.push_back(pActionMap);
	}

	if (m_iHostPlayerSect < 0)
	{
		m_iHostPlayerSect = iniFile.GetValueAsInt("EDITOR", "HostPlayerSect", 0);
		m_bHostPlayerMale = iniFile.GetValueAsBoolean("EDITOR", "HostPlayerMale", true);
	}

	m_bEnable = true;

	iniFile.Close();
}

void CAnmCommonActionManager::Release()
{
	m_vecSectNames.clear();
	m_vecCommonActionNames.clear();
	m_malePhysiqueECMs.clear();
	m_femalePhysiqueECMs.clear();
	m_sectToMalePhysiqueMap.clear();
	m_sectToFemalePhysiqueMap.clear();

	abase::vector<CommonToRealActionMap*>::iterator it = m_vecMaleSectActionMap.begin();
	abase::vector<CommonToRealActionMap*>::iterator ite = m_vecMaleSectActionMap.end();
	for (; it != ite; ++ it)
	{
		delete *it;
	}
	m_vecMaleSectActionMap.clear();

	for (it = m_vecFemaleSectActionMap.begin(), ite = m_vecFemaleSectActionMap.end(); it != ite; ++ it)
	{
		delete *it;
	}
	m_vecFemaleSectActionMap.clear();

	m_bEnable = false;
}
	
const ACString* CAnmCommonActionManager::GetSectName(int iSect) const
{
	if (iSect >= 0 && iSect < (int)m_vecSectNames.size())
		return &m_vecSectNames[iSect];
	return NULL;
}

const ACString* CAnmCommonActionManager::GetSectPhysique(int iSect, bool bMale) const
{
	if (iSect >= 0 && iSect < (int)m_vecSectNames.size())
	{
		const SectToPhysiqueMap& sect2PhysiqueMap = bMale ? m_sectToMalePhysiqueMap : m_sectToFemalePhysiqueMap;
		const abase::vector<ACString>& physiqueECMs = bMale ? m_malePhysiqueECMs : m_femalePhysiqueECMs;
		SectToPhysiqueMap::const_iterator it = sect2PhysiqueMap.find(iSect);
		if (it != sect2PhysiqueMap.end())
			return &physiqueECMs[it->second];
	}
	return NULL;
}

const ACString* CAnmCommonActionManager::GetCommonActionName(int id) const
{
	if (id >= 0 && id < (int)m_vecCommonActionNames.size())
		return &m_vecCommonActionNames[id];
	return NULL;
}

const ACString* CAnmCommonActionManager::GetRealActionName(int iSect, bool bMale, const ACString& szCommonActionName) const
{
	const abase::vector<CommonToRealActionMap*>& vecCommonToRealMap = bMale ? m_vecMaleSectActionMap : m_vecFemaleSectActionMap;
	if (iSect >= 0 && iSect < (int)vecCommonToRealMap.size())
	{
		const CommonToRealActionMap& commonToRealMap = *vecCommonToRealMap[iSect];
		CommonToRealActionMap::const_iterator it = commonToRealMap.find(szCommonActionName);
		if (it != commonToRealMap.end())
			return &(it->second);
	}
	return NULL;
}

int CAnmCommonActionManager::AddCommonAction(const ACString& szActionName)
{
	for (abase::vector<ACString>::iterator it = m_vecCommonActionNames.begin(),
		ite = m_vecCommonActionNames.end(); it != ite; ++ it)
	{
		if (*it == szActionName)
			return 0;
	}
	m_vecCommonActionNames.push_back(szActionName);
	return (int)m_vecCommonActionNames.size();
}

void CAnmCommonActionManager::MapCommonActionToRealAction(int iSect,  bool bMale, const ACString& szCommonAction, const ACString& szRealAction)
{
	abase::vector<CommonToRealActionMap*>& vecCommonToRealMap = bMale ? m_vecMaleSectActionMap : m_vecFemaleSectActionMap;
	if (iSect >= 0 && iSect < (int)vecCommonToRealMap.size())
	{
		CommonToRealActionMap& commonToRealMap = *vecCommonToRealMap[iSect];
		commonToRealMap[szCommonAction] = szRealAction;
	}
}

// 获取当前主角模型
const ACString& CAnmCommonActionManager::GetHostPlayerModel() const
{
	const static ACString defaultECM = _AL("models\\Players\\形象\\男\\躯干\\男.ecm");
	if (!m_bEnable || m_iHostPlayerSect < 0 || m_iHostPlayerSect >= (int)m_vecSectNames.size())
		return defaultECM;
	
	if (const ACString* pECM = GetSectPhysique(m_iHostPlayerSect, m_bHostPlayerMale))
		return *pECM;

	return defaultECM;
}

// 获取当前主角某个通用动作对应的真实动作
const ACString* CAnmCommonActionManager::GetHostPlayerRealAction(const ACString& szCommonActionName) const
{
	if (!m_bEnable || m_iHostPlayerSect < 0 || m_iHostPlayerSect >= (int)m_vecSectNames.size())
		return NULL;

	return GetRealActionName(m_iHostPlayerSect, m_bHostPlayerMale, szCommonActionName);
}

bool CAnmCommonActionManager::SaveToFile()
{
	AIniFile iniFile;
	if (!iniFile.Open(COMMON_ACTION_INI_PATH))
		return false;

	AString szSection, szKey;
	int count = (int)m_vecCommonActionNames.size();
	szSection = "COMMON_ACTION";
	iniFile.WriteIntValue(szSection, "COUNT", (int)m_vecCommonActionNames.size());
	int i;
	for (i = 0; i < count; ++ i)
	{
		szKey.Format("ACTION%d", i+1);
		iniFile.WriteStringValue(szSection, szKey, WC2AS(m_vecCommonActionNames[i]));
	}

	count = (int)m_vecMaleSectActionMap.size();
	for (i = 0; i < count; ++ i)
	{
		const CommonToRealActionMap& commonToRealMap = *m_vecMaleSectActionMap[i];
		szSection.Format("SECT%d_MALE_ACTION", i+1);
		for (CommonToRealActionMap::const_iterator it = commonToRealMap.begin(),
			ite = commonToRealMap.end(); it != ite; ++ it)
		{
			iniFile.WriteStringValue(szSection, WC2AS(it->first), WC2AS(it->second));
		}
	}

	count = (int)m_vecFemaleSectActionMap.size();
	for (i = 0; i < count; ++ i)
	{
		const CommonToRealActionMap& commonToRealMap = *m_vecFemaleSectActionMap[i];
		szSection.Format("SECT%d_FEMALE_ACTION", i+1);
		for (CommonToRealActionMap::const_iterator it = commonToRealMap.begin(),
			ite = commonToRealMap.end(); it != ite; ++ it)
		{
			iniFile.WriteStringValue(szSection, WC2AS(it->first), WC2AS(it->second));
		}
	}

	iniFile.WriteIntValue("EDITOR", "HostPlayerSect", m_iHostPlayerSect);
	iniFile.WriteIntValue("EDITOR", "HostPlayerMale", m_bHostPlayerMale);

	bool r = iniFile.Save(COMMON_ACTION_INI_PATH);
	iniFile.Close();

	return r;
}

void CAnmCommonActionManager::Reset()
{
	Release();
	Init();
}