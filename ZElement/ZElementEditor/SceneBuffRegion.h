// SceneBuffRegion.h: interface for the CSceneBuffRegion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCENEBUFFREGION_H__0A6E742C_1DC6_4054_B012_92B11282EA61__INCLUDED_)
#define AFX_SCENEBUFFREGION_H__0A6E742C_1DC6_4054_B012_92B11282EA61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SceneObject.h"

struct BUFF_REGION_DATA
{
//	AString strBuffRegion;	//Buff区域的别名
	int     nRegionTemplID;	//区域模板ID
	int     nMonsterID;     //关联怪物ID
	int     nPathID;		//路径关联曲线，Buff区域的范围，需要闭合路径
	AString strPathName;	// 关联曲线名字
};


void init_buff_region_data( BUFF_REGION_DATA& dat);

struct BEZIER_ID_FUNCS3 : public CUSTOM_FUNCS
{
	virtual BOOL CALLBACK OnActivate(void);
	virtual LPCTSTR CALLBACK OnGetShowString(void) const;
	virtual AVariant CALLBACK OnGetValue(void) const;
	virtual void CALLBACK OnSetValue(const AVariant& var);

	AVariant	m_var;
	CString		m_strText;
};

class CSceneBuffRegion : public CSceneObject  
{
public:
	CSceneBuffRegion();
	virtual ~CSceneBuffRegion();
	void BuildProperty();

	virtual void UpdateProperty(bool bGet);
	virtual void Translate(A3DVECTOR3 vDelta);

	BUFF_REGION_DATA& GetProperty(){ return m_Data; };
	void			  SetProperty(const BUFF_REGION_DATA & dat) { m_Data = dat; };

	//Load data
	virtual bool Load(CELArchive& ar,DWORD dwVersion,int iLoadFlags);
	//Save data
	virtual bool Save(CELArchive& ar,DWORD dwVersion);

	int  GetCopyFlags(){ return m_nCopy; }
	void SetCopyFlags( int nCopy){ m_nCopy = nCopy; }
protected:
	BUFF_REGION_DATA m_Data;
	BEZIER_ID_FUNCS3* m_pBIDSelFuncs;
	int     m_nCopy;//数据套数，决定是第几套数据(0,1)
//	NPC_ID_FUNCS* m_pFIDSelFuncs;
};

#endif // !defined(AFX_SCENEBUFFREGION_H__0A6E742C_1DC6_4054_B012_92B11282EA61__INCLUDED_)
