#pragma once

#include <A3D.h>

class CAnmCommonActionManager
{
public:
	CAnmCommonActionManager();

	~CAnmCommonActionManager();

	bool IsEnable() const { return m_bEnable; }
	
	int GetSectCount() const { return m_vecSectNames.size(); }
	
	const ACString* GetSectName(int iSect) const; // sect取值: [0, count-1]

	const ACString* GetSectPhysique(int iSect, bool bMale) const;

	int GetCommonActionCount() const { return m_vecCommonActionNames.size(); }

	const ACString* GetCommonActionName(int id) const; // id取值: [0, count-1]

	const ACString* GetRealActionName(int iSect, bool bMale, const ACString& szCommonActionName) const;

	int AddCommonAction(const ACString& szActionName); // 返回添加后的通用动作数

	void MapCommonActionToRealAction(int iSect,  bool bMale, const ACString& szCommonAction,const ACString& szRealAction);

	bool SaveToFile();

	// 放弃当前添加的内容, 重新读取ini配置
	void Reset();

	// 设置当前主角职业
	void SetHostPlayerSect(int iSect) { m_iHostPlayerSect = iSect; }

	// 设置当前主角性别
	void SetHostPlayerGender(bool bMale) { m_bHostPlayerMale = bMale; }

	// 获取当前主角的职业
	int GetHostPlayerSect() const { return m_iHostPlayerSect; }

	// 判断当前主角性别
	bool IsHostPlayerMale() const { return m_bHostPlayerMale; }

	// 获取当前主角模型
	const ACString& GetHostPlayerModel() const;

	// 获取当前主角某个通用动作对应的真实动作
	const ACString* GetHostPlayerRealAction(const ACString& szCommonActionName) const;

private:
	// 初始化, 加载ini
	void Init();

	void Release();

private:
	typedef abase::hash_map<int, int>					SectToPhysiqueMap;		// Sect id -> PHYSIQUE_TYPE
	typedef abase::hash_map<ACString, ACString>			CommonToRealActionMap;  // common name -> real name

	int  m_iHostPlayerSect;
	bool m_bHostPlayerMale;

	bool					m_bEnable;		// 只有commom_action.ini文件存在且配置有效时为true, 表示启用主角通用动作
	abase::vector<ACString>	m_vecSectNames;
	abase::vector<ACString>	m_vecCommonActionNames;
	abase::vector<ACString>	m_malePhysiqueECMs;
	abase::vector<ACString>	m_femalePhysiqueECMs;
	SectToPhysiqueMap		m_sectToMalePhysiqueMap;
	SectToPhysiqueMap		m_sectToFemalePhysiqueMap;
	abase::vector<CommonToRealActionMap*> m_vecMaleSectActionMap;
	abase::vector<CommonToRealActionMap*> m_vecFemaleSectActionMap;
};