// ElementDataDoc.cpp : implementation of the CElementDataDoc class
//

#include "stdafx.h"
#include "A3D.h"
#include "ElementData.h"
#include "EnumTypes.h"
#include "Global.h"
#include "ElementDataDoc.h"
#include "TemplIDSelDlg.h"
#include "FactionsSel.h"
#include "PathDataSel.h"
#include "MonsterFaction.h"
#include "EquipMaskDlg.h"
#include "TaskIDSelDlg.h"
#include "ProcTypeMaskDlg.h"
#include "ElementDataView.h"
#include "TalkModifyDlg.h"
#include "TextInputDlg.h"
#include "SingleTextDlg.h"
#include "AVariant.h"
#include "EC_MD5Hash.h"
#include "ExtTemplateIDSelDlg.h"
#include "OccupationMaskDlg.h"
#include "TimeSettingDlg.h"

#define new A_DEBUG_NEW

static LPCTSTR _mask_equip_name[] = {
	_T("武器"),
	_T("头盔"),
	_T("衣服"),
	_T("鞋子"),
	_T("项链"),
	_T("戒指1"),
	_T("戒指2"),
	_T("时装眼镜"),
	_T("时装鼻子"),
	_T("时装胡子"),
	_T("时装头饰"),
	_T("时装上衣"),
	_T("时装下衣"),
	_T("时装鞋子"),
	_T("时装背饰"),
	_T("翅膀"),
	_T("法宝主件"),
	_T("法宝附加1"),
	_T("法宝附加2"),
	_T("喇叭"),
	_T("饰品1"),
	_T("饰品2"),
	_T("饰品3"),
	_T("饰品4"),
	_T("饰品5"),
	_T("饰品6"),
	_T("时装武器"),
};

static LPCTSTR _mask_visualize_id[11] =  {
	_T("性感"),
	_T("骨感"),
	_T("可爱"),
	_T("冷艳"),
	_T("清秀"),
	_T("俊朗"),
	_T("妖媚"),
	_T("野性"),
	_T("硬朗"),
	_T("阴险"),
	_T("猥琐")
};

static LPCTSTR _mask_patroll_mode[1] = {
	_T("随机走动")
};

static LPCTSTR _mask_character_combo_id[] =
{
	_T("c0:人族少侠"),
	_T("c1:鬼王宗1"),
	_T("c2:鬼王宗2"),
	_T("c3:鬼王宗3"),
	_T("c4:合欢派1"),
	_T("c5:合欢派2"),
	_T("c6:合欢派3"),
	_T("c7:青云门1"),
	_T("c8:青云门2"),
	_T("c9:青云门3"),
	_T("c10:天音寺1"),
	_T("c11:天音寺2"),
	_T("c12:天音寺3"),
	_T("c13:鬼王宗4"),
	_T("c14:鬼王宗5"),
	_T("c15:"),
	_T("c16:合欢派4"),
	_T("c17:合欢派5"),
	_T("c18:"),
	_T("c19:青云门4"),
	_T("c20:青云门5"),
	_T("c21:"),
	_T("c22:天音寺4"),
	_T("c23:天音寺5"),
	_T("c24:"),
	_T("c25:鬼道1"),
	_T("c26:鬼道2"),
	_T("c27:鬼道3"),
	_T("c28:鬼道4"),
	_T("c29:鬼道5"),
	_T("c30:"),
	_T("c31:"),
	_T("c32:神裔少侠"),
	_T("c33:九黎1"),
	_T("c34:九黎2"),
	_T("c35:九黎3"),
	_T("c36:九黎4"),
	_T("c37:九黎5"),
	_T("c38:"),
	_T("c39:烈山1"),
	_T("c40:烈山2"),
	_T("c41:烈山3"),
	_T("c42:烈山4"),
	_T("c43:烈山5"),
	_T("c44:"),
	_T("c45:怀光1"),
	_T("c46:怀光2"),
	_T("c47:怀光3"),
	_T("c48:怀光4"),
	_T("c49:怀光5"),
	_T("c50:"),
	_T("c51:天华1"),
	_T("c52:天华2"),
	_T("c53:天华3"),
	_T("c54:天华4"),
	_T("c55:天华5"),
	_T("c56:"),
	_T("c57:"),
	_T("c58:"),
	_T("c59:"),
	_T("c60:"),
	_T("c61:"),
	_T("c62:"),
	_T("c63:"),
};

static LPCTSTR _mask_character_combo_id2[] =
{
	_T("c64:焚香1"),
	_T("c65:焚香2"),
	_T("c66:焚香3"),
	_T("c67:焚香4"),
	_T("c68:焚香5"),
	_T("c69:"),
	_T("c70:"),
	_T("c71:"),
	_T("c72:"),
	_T("c73:"),
	_T("c74:"),
	_T("c75:"),
	_T("c76:"),
	_T("c77:"),
	_T("c78:"),
	_T("c79:"),
	_T("c80:"),
	_T("c81:"),
	_T("c82:"),
	_T("c83:"),
	_T("c84:"),
	_T("c85:"),
	_T("c86:"),
	_T("c87:"),
	_T("c88:"),
	_T("c89:"),
	_T("c90:"),
	_T("c91:"),
	_T("c92:"),
	_T("c93:"),
	_T("c94:"),
	_T("c95:"),
	_T("c96:太昊1"),
	_T("c97:太昊2"),
	_T("c98:太昊3"),
	_T("c99:太昊4"),
	_T("c100:太昊5"),
	_T("c101:"),
	_T("c102:"),
	_T("c103:"),
	_T("c104:"),
	_T("c105:"),
	_T("c106:"),
	_T("c107:"),
	_T("c108:"),
	_T("c109:"),
	_T("c110:"),
	_T("c111:"),
	_T("c112:"),
	_T("c113:"),
	_T("c114:"),
	_T("c115:"),
	_T("c116:"),
	_T("c117:"),
	_T("c118:"),
	_T("c119:"),
	_T("c120:"),
	_T("c121:"),
	_T("c122:"),
	_T("c123:"),
	_T("c124:"),
	_T("c125:"),
	_T("c126:"),
	_T("c127:"),
};

static LPCTSTR _mask_pet_food[] =
{
	_T("草料"),
	_T("肉类"),
	_T("野菜"),
	_T("水果"),
	_T("清水"),
};

static LPCTSTR _mask_pet[] =
{
	_T("pet_1"),
	_T("pet_2"),
	_T("pet_3"),
	_T("pet_4"),
	_T("pet_5"),
	_T("pet_6"),
	_T("pet_7"),
	_T("pet_8"),
	_T("pet_9"),
	_T("pet_10"),
	_T("pet_11"),
	_T("pet_12"),
	_T("pet_13"),
	_T("pet_14"),
	_T("pet_15"),
	_T("pet_16"),
};

static LPCTSTR _combined_services[] =
{
	_T("驯化服务"),
	_T("跨服PK隐仙阁报名"),
	_T("物品锁定"),
	_T("驿站发现服务"),
	_T("帮派服务"),
	_T("修复破损物品"),
	_T("邮寄服务"),
	_T("拍卖服务"),
	_T("双倍经验打卡服务"),
	_T("孵化宠物蛋服务"),
	_T("还原宠物蛋服务"),
	_T("城战管理服务"),
	_T("离开战场服务"),
	_T("点卡寄售服务"),
	_T("帮派仓库服务"),
	_T("法宝相关服务"),
	_T("城战报名服务"),
	_T("战场入场服务"),
	_T("城战日常建设服务"),
	_T("宠物驯养"),
	_T("宠物放生"),
	_T("宠物修炼"),
	_T("宠物改名"),
	_T("滴血认主"),
	_T("器魄镶嵌"),
	_T("器魄拆除"),
	_T("恢复魄力"),
	_T("装备拆解"),
	_T("飞升战场管理服务"),
	_T("宋金战场报名服务"),
	_T("竞技场报名服务"),
	_T("线上推广服务"),
};

static LPCTSTR _combined_services2[] =
{
	_T("形象修改服务"),
	_T("宠物装备强化"),
	_T("跨服PK逸龙轩报名"),
	_T("跨服PK观战"),
	_T("剧情战场报名服务"),
	_T("变身道具相关服务"),
	_T("变身卡灵力恢复服务"),
	_T("领土战宣战服务"),
	_T("进入领土战战场"),
	_T("领土战领取奖励"),
	_T("星盘充能服务"),
	_T("修复破损物品（特殊）"),
	_T("装备升级服务"),
	_T("传送到跨服服务"),
	_T("从跨服传回服务"),
	_T("进入挑战空间服务"),
	_T("宝石插槽鉴定服务"),
	_T("宝石插槽重铸服务"),
	_T("宝石插槽定制服务"),
	_T("宝石镶嵌服务"),
	_T("宝石拆除服务"),
	_T("宝石升品服务"),
	_T("宝石精炼服务"),
	_T("宝石萃取服务"),
	_T("宝石熔炼服务"),
	_T("跨服小队pk红队报名"),//25
	_T("跨服小队pk蓝队报名"),
	_T("跨服小队pk记者报名"),
	_T("副本相关服务"),	//28
	_T("神武镇元塔个人信息"),	
	_T("改名服务"),	
	_T("家族改名"),
};

static LPCTSTR _combined_services3[] =
{
	_T("帮派改名"),
	_T("法宝飞升"),
	_T("法宝技能融合"),
	_T("法宝技能镶嵌"),
	_T("无用别勾"),
	_T("装备星座打孔"),
	_T("星座镶嵌"),
	_T("星座摘除"),
	_T("无用别勾"),
	_T("无用别勾"),
	_T("跨服报名传回"),
	_T("跨服报名传送"),
	_T("国战报名"),
	_T("飞升进度"),
	_T("首饰升级"),
	_T("生产技能快速生产"),
	_T("金身法身快速制作"),
	_T("流水席战场报名"),
	_T("帮派基地"),
	_T("宠物飞升"),
	_T("灾变专用"),
	_T("帮派元宝商城"),
};

static LPCTSTR _immune_type[] =
{
	_T("免疫恐惧"),
	_T("免疫致盲"),
	_T("免疫击退"),
	_T("免疫被打掉mp"),
	_T("免疫减总防御％"),
	_T("免疫减固定值防御"),
	_T("免疫眩晕"),
	_T("免疫虚弱"),
	_T("免疫缓慢"),
	_T("免疫沉默"),
	_T("免疫睡眠"),
	_T("免疫缠绕"),
	_T("免疫禁食"),
	_T("免疫空手"),
	_T("免疫持续伤害"),
	_T("免疫嘲讽"),
	_T("免疫元神耗散"),
	_T("免疫流魔"),	//配合服务器的掩码设置而添加，暂时还没使用需求
	_T("免疫变身"), //配合服务器的掩码设置而添加，暂时还没使用需求
	_T("免疫灼烧"),
	_T("免疫血咒"),	//0x00100000 免疫血咒		
	_T("免疫降低防御%"),
};

static LPCTSTR _mask_sect[] =
{
	_T("少侠"),
	_T("鬼王宗"),
	_T("合欢派"),
	_T("青云门"),
	_T("天音寺"),
	_T("鬼道"),
	_T("焚香"),
	_T("保留3"),
	_T("神裔少侠"),
	_T("九黎"),
	_T("烈山"),
	_T("太昊"),
	_T("怀光"),
	_T("东夷"),
	_T("天华"),
	_T("萝莉"),
	_T("四足"),

};

static LPCTSTR _astro_type[] =
{
	"horoscope_1",
	"horoscope_2",
	"horoscope_3",
	"horoscope_4",
	"horoscope_5",
	"horoscope_6",
	"horoscope_7",
	"horoscope_8",
	"horoscope_9",
};

static LPCTSTR _god_devil_mask[] =
{
	"仙",
	"魔",
	"佛",
	"备选1",
	"备选2",
	"备选3",
};

static LPCTSTR _nation_position_mask[] =
{
	"国王",
	"王后",
	"将军",
	"大臣",
	"帮众",
	"亲卫",
	"备选2",
};

static LPCTSTR _forbid_food_mask[] =
{
	"瞬回HP",
	"瞬回MP",
	"瞬回HP及MP",
	"持续回HP",
	"持续回MP",
	"持续回HP及MP",	
};

static LPCTSTR _gift_bag_mask[] =
{
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"10",
	"11",
	"12",
	"13",
	"14",
	"15",
	"16"
};

static LPCTSTR _accident_action_mask[] =
{
	"受攻击结束",
	"主动移动结束",
};

/////////////////////////////////////////////////////////////////////////////
// CElementDataDoc

IMPLEMENT_DYNCREATE(CElementDataDoc, CDocument)

BEGIN_MESSAGE_MAP(CElementDataDoc, CDocument)
	//{{AFX_MSG_MAP(CElementDataDoc)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_WINDOW_NEW, OnWindowNew)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CElementDataDoc construction/destruction

CElementDataDoc::CElementDataDoc()
{
	// TODO: add one-time construction code here
	m_pProperty = NULL;
	m_bExt = false;
	bInited = false;
}

CElementDataDoc::~CElementDataDoc()
{
	if(m_pProperty) delete m_pProperty;
	POSITION pos = m_listTypes.GetHeadPosition();
	while(pos)
	{
		ASetTemplate<int>*ptemp = (ASetTemplate<int>*)m_listTypes.GetNext(pos);
		delete ptemp;
	}

	pos = m_listCustoms.GetHeadPosition();
	while(pos) delete (CUSTOM_FUNCS*)m_listCustoms.GetNext(pos);

	m_Tmpl.Release();
	m_ExtTmpl.Release();
}

BOOL CElementDataDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CElementDataDoc serialization

void CElementDataDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CElementDataDoc diagnostics

#ifdef _DEBUG
void CElementDataDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CElementDataDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CElementDataDoc commands

void CElementDataDoc::OnFileOpen() 
{
	// TODO: Add your command handler code here
	AfxMessageBox("doc");
}

void CElementDataDoc::OnWindowNew() 
{
	// TODO: Add your command handler code here
	
}

BOOL CElementDataDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
 
#ifdef _DEBUG 
	DWORD lastTime = a_GetTime();
	CString strDebug;
#endif

	m_pProperty = new ADynPropertyObject;
	m_strDocPath = lpszPathName;
	CString str(lpszPathName);
	str.Replace(g_szWorkDir,"");
	if(!FileIsReadOnly(m_strDocPath))
	{
		//SetModifiedFlag(true);
		nFlag = WAY_DEFAULT;
	}else
	{
		nFlag = WAY_READONLY;	
	}
	m_Tmpl.Release();
	m_ExtTmpl.Release();
	if(str.Find(".tmpl") != -1)
	{
		if(m_Tmpl.Load(str)==0)
		{//读一个模板
			
#ifdef _DEBUG
			strDebug.Format("Load template time: %d\n", a_GetTime() - lastTime);
			lastTime = a_GetTime();
			TRACE(strDebug);
#endif
			
			m_bExt = false;
			int id = m_Tmpl.GetID();
			AString tname = m_Tmpl.GetName();
			m_pProperty->DynAddProperty(AVariant(id), "模板ID",NULL,NULL,WAY_READONLY);
			m_pProperty->DynAddProperty(AVariant(tname), "模板名字",NULL,NULL,nFlag);
			int itemNum = m_Tmpl.GetItemNum();
			for(int i=0; i<itemNum; i++)
			{
				AString itemType = m_Tmpl.GetItemType(i);
				AString itemName = m_Tmpl.GetItemName(i);
				AVariant var = m_Tmpl.GetItemValue(i);
				if(AVariant::AVT_INVALIDTYPE == var.GetType())
				{
					if(stricmp(itemType,"int")==0)
					{
						int x = 0;
						m_Tmpl.SetItemValue(i,AVariant(x));
						var = AVariant(x);
					}else
					if(stricmp(itemType,"int64")==0)
					{
						__int64 x = 0;
						m_Tmpl.SetItemValue(i,AVariant(x));
						var = AVariant(x);
					}else
					if(stricmp(itemType,"float")==0)
					{
						float x = 0.0f;
						m_Tmpl.SetItemValue(i,AVariant(x));
						var = AVariant(x);
					}else
					if(stricmp(itemType,"double")==0)
					{
						double x = 0.0;
						m_Tmpl.SetItemValue(i,AVariant(x));
						var = AVariant(x);
					}else
					if(stricmp(itemType,"path")==0)
					{
						AString x("");
						m_Tmpl.SetItemValue(i,AVariant(x));
						var = AVariant(x);
					}else
					if(stricmp(itemType,"vector")==0)
					{
						var = A3DVECTOR3(0);
						m_Tmpl.SetItemValue(i,var);
					}else
					if(stricmp(itemType,"color")==0)
					{
						var = 0xff000000;
						m_Tmpl.SetItemValue(i,var);
					}else
					if(g_EnumTypes.GetType(AString(itemType)))
					{//enum
						int x = 0;
						m_Tmpl.SetItemValue(i,AVariant(x));
						var = AVariant(x);
					}else
					if(stricmp(itemType,"smallbool")==0)
					{
						bool x = false;
						m_Tmpl.SetItemValue(i,AVariant(x));
						var = AVariant(x);
					}else
					if(	stricmp(itemType, "character_combo_id")==0 || 
						stricmp(itemType, "character_combo_id2")==0) //需要正确初始化Added 2011-07-19.
					{
						UINT64 x = 0;
						m_Tmpl.SetItemValue(i,AVariant(x));
						var = AVariant(x);						
					}else
					if( stricmp(itemType, "char")==0 )	//Added 2012-03-22.
					{
						char x = '\0';
						m_Tmpl.SetItemValue(i,AVariant(x));
						var = AVariant(x);
					}
					else
					{//uint,and other user template
						unsigned int x = 0;
						m_Tmpl.SetItemValue(i,AVariant(x));
						var = AVariant(x);
					}
								
				}
				Explain(CString(itemType),var,CString(itemName));
				
			}
		}

#ifdef _DEBUG
			strDebug.Format("Explain time: %d\n", a_GetTime() - lastTime);
			lastTime = a_GetTime();
			TRACE(strDebug);
#endif

	}else
	if(str.Find(".ext") != -1)
	{
		if(m_ExtTmpl.Load(str, false)==0)
		{//读一个模板

#ifdef _DEBUG
			strDebug.Format("Load template time: %d\n", a_GetTime() - lastTime);
			lastTime = a_GetTime();
			TRACE(strDebug);
#endif

			m_bExt = true;
			int id = m_ExtTmpl.GetID();
			AString tname = m_ExtTmpl.GetName();
			m_pProperty->DynAddProperty(AVariant(id), "模板ID",NULL,NULL,WAY_READONLY);
			m_pProperty->DynAddProperty(AVariant(tname), "模板名字",NULL,NULL,nFlag);
			int itemNum = m_ExtTmpl.GetItemNum();
			for(int i=0; i<itemNum; i++)
			{
				AString itemType = m_ExtTmpl.GetItemType(i);
				AString itemName = m_ExtTmpl.GetItemName(i);
				AVariant var = m_ExtTmpl.GetItemValue(i);
				if(AVariant::AVT_INVALIDTYPE == var.GetType())
				{
					if(stricmp(itemType,"int")==0)
					{
						int x = 0;
						m_ExtTmpl.SetItemValue(i,AVariant(x));
						var = AVariant(x);
					}else
					if(stricmp(itemType,"int64")==0)
					{
						__int64 x = 0;
						m_ExtTmpl.SetItemValue(i,AVariant(x));
						var = AVariant(x);
					}else
					if(stricmp(itemType,"float")==0)
					{
						float x = 0.0f;
						m_ExtTmpl.SetItemValue(i,AVariant(x));
						var = AVariant(x);
					}else
					if(stricmp(itemType,"double")==0)
					{
						double x = 0.0;
						m_ExtTmpl.SetItemValue(i,AVariant(x));
						var = AVariant(x);
					}else
					if(stricmp(itemType,"path")==0)
					{
						AString x("");
						m_ExtTmpl.SetItemValue(i,AVariant(x));
						var = AVariant(x);
					}else
					if(stricmp(itemType,"vector")==0)
					{
						var = A3DVECTOR3(0);
						m_ExtTmpl.SetItemValue(i,var);
					}else
					if(stricmp(itemType,"color")==0)
					{
						var = 0xff000000;
						m_Tmpl.SetItemValue(i,var);
					}else
					if(g_EnumTypes.GetType(AString(itemType)))
					{//enum
						int x = 0;
						m_ExtTmpl.SetItemValue(i,AVariant(x));
						var = AVariant(x);
					}else if(stricmp(itemType,"uint")==0)
					{
						unsigned int x = 0;
						m_ExtTmpl.SetItemValue(i,AVariant(x));
						var = AVariant(x);
					}
				}
				Explain(CString(itemType),var,CString(itemName));
			}
		}
#ifdef _DEBUG
			strDebug.Format("Explain time: %d\n", a_GetTime() - lastTime);
			lastTime = a_GetTime();
			TRACE(strDebug);
#endif
	}
	return TRUE;
}

BOOL CElementDataDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(!FileIsReadOnly(m_strDocPath))
	{
		POSITION pos = GetFirstViewPosition();
		while (pos != NULL)
		{
			CElementDataView* pView = (CElementDataView*)GetNextView(pos);
			pView->UpdateData();
		}  
		
		AVariant var = m_pProperty->GetPropVal(0);
		var = m_pProperty->GetPropVal(1);
		if(!m_bExt)
		{
			m_Tmpl.SetName((AString)var);
			int itemNum = m_Tmpl.GetItemNum();
			for(int i=2; i<itemNum + 2; i++)
			{
				AVariant var = m_pProperty->GetPropVal(i);
				m_Tmpl.SetItemValue(i-2,var);
			}
			m_Tmpl.Save(m_strDocPath);
#ifdef _MD5_CHECK
			if(!WriteMd5CodeToTemplate(m_strDocPath.GetBuffer(0)))
			{
				AfxMessageBox("CElementDataDoc::OnSaveDocument(), failed to write md5 code to template!");
				return false;
			}
#endif 
			m_strDocPath.ReleaseBuffer();
		}else
		{
			m_ExtTmpl.SetName((AString)var);
			int itemNum = m_ExtTmpl.GetItemNum();
			for(int i=2; i<itemNum + 2; i++)
			{
				AVariant var = m_pProperty->GetPropVal(i);
				m_ExtTmpl.SetItemValue(i-2,var);
			}
			m_ExtTmpl.Save(m_strDocPath);
#ifdef _MD5_CHECK
			if(!WriteMd5CodeToTemplate(m_strDocPath.GetBuffer(0)))
			{
				AfxMessageBox("CElementDataDoc::OnSaveDocument(), failed to write md5 code to template!");
				return false;
			}
#endif
			m_strDocPath.ReleaseBuffer();
		}
	}
	return true;
}

bool CElementDataDoc::IsModifed()
{
	if(FileIsReadOnly(m_strDocPath)) return false;
	
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CElementDataView* pView = (CElementDataView*)GetNextView(pos);
		pView->UpdateData();
	}  
	
	AVariant var = m_pProperty->GetPropVal(0);
	var = m_pProperty->GetPropVal(1);
	if(!m_bExt)
	{
		if(0!=strcmp(m_Tmpl.GetName(),((AString)var)))
			return true;
		
		int itemNum = m_Tmpl.GetItemNum();
		for(int i=2; i<itemNum + 2; i++)
		{
			AVariant var = m_pProperty->GetPropVal(i);
			AVariant old = m_Tmpl.GetItemValue(i-2);
			AString type = m_Tmpl.GetItemType(i-2);

			if(!IsEqual(type,var,old)) return true;
		}
	}else
	{
		if(0!=strcmp(m_ExtTmpl.GetName(),((AString)var)))
			return true;

		int itemNum = m_ExtTmpl.GetItemNum();
		for(int i=2; i<itemNum + 2; i++)
		{
			AVariant var = m_pProperty->GetPropVal(i);
			AVariant old = m_ExtTmpl.GetItemValue(i-2);
			AString type = m_ExtTmpl.GetItemType(i-2);
			if(!IsEqual(type,var,old)) return true;
		}
	}
	
	return false;
}

bool CElementDataDoc::IsEqual(AString type,AVariant var1,AVariant var2)
{
	if(stricmp(type,"int")==0)
	{
		int value1 = (int)var1;
		int value2 = (int)var2;
		if(value1 == value2) return true;
	}else
	if(stricmp(type,"int64")==0)
	{
		__int64 value1 = (__int64)var1;
		__int64 value2 = (__int64)var2;
		if(value1 == value2) return true;
	}else
	if(stricmp(type,"float")==0)
	{
		float value1 = (float)var1;
		float value2 = (float)var2;
		if(value1 == value2) return true;
	}else
	if(stricmp(type,"double")==0)
	{
		double value1 = (double)var1;
		double value2 = (double)var2;
		if(value1 == value2) return true;
	}else
	if(stricmp(type,"uint")==0)
	{
		unsigned int value1 = (unsigned int)var1;
		unsigned int value2 = (unsigned int)var2;
		if(value1 == value2) return true;
	}else
	if(stricmp(type,"path")==0)
	{
		AString value1 = (AString)var1;
		AString value2 = (AString)var2;
		if(value1 == value2) return true;	
	}else
	if(stricmp(type,"vector")==0)
	{
		A3DVECTOR3 value1 = (A3DVECTOR3)var1;
		A3DVECTOR3 value2 = (A3DVECTOR3)var2;
		if(value1 == value2) return true;	
	}else
	if(stricmp(type,"color")==0)
	{
		A3DCOLOR value1 = (A3DCOLOR)var1;
		A3DCOLOR value2 = (A3DCOLOR)var2;
		if(value1 == value2) return true;	
	}else
	if(stricmp(type,"char")==0)	//Added 2012-03-23.
	{
		char value1 = (char)var1;
		char value2 = (char)var2;
		if (value1 == value2) return true;
	}else
	if(stricmp(type,"smallbool")==0)
	{
		bool value1 = (bool)var1;
		bool value2 = (bool)var2;
		if(value1 == value2) return true;	
	}else
	{
		unsigned int value1 = (unsigned int)var1;
		unsigned int value2 = (unsigned int)var2;
		if(value1 == value2) return true;	
	}
	return false;
}

void CElementDataDoc::Explain(CString type,AVariant var,CString name)
{
	if(stricmp(type,"int")==0)
	{
		m_pProperty->DynAddProperty(AVariant((int)var), name,NULL,NULL,nFlag);
	}else
	if(stricmp(type,"int64")==0)
	{
		m_pProperty->DynAddProperty(AVariant((__int64)var), name,NULL,NULL,nFlag);
	}else
	if(stricmp(type,"float")==0)
	{
		m_pProperty->DynAddProperty(AVariant((float)var), name,NULL,NULL,nFlag);
	}else
	if(stricmp(type,"double")==0)
	{
		m_pProperty->DynAddProperty(AVariant((double)var), name,NULL,NULL,nFlag);
	}else
	if(stricmp(type,"uint")==0)
	{
		m_pProperty->DynAddProperty(AVariant((unsigned int)var), name,NULL,NULL,nFlag);
	}else
	if(stricmp(type,"path")==0)
	{
		m_pProperty->DynAddProperty(AVariant((AString)var),name,NULL,NULL,WAY_FILENAME|nFlag,"");
	}else
	if(stricmp(type,"vector")==0)
	{
		m_pProperty->DynAddProperty(AVariant((A3DVECTOR3)var),name,NULL,NULL,WAY_VECTOR|nFlag,"");
	}else
	if(stricmp(type,"color")==0)
	{
		m_pProperty->DynAddProperty(AVariant((A3DCOLOR)var),name,NULL,NULL,WAY_COLOR|nFlag,"");
	}else
	if(stricmp(type,"smallbool")==0)
	{
		m_pProperty->DynAddProperty(AVariant((bool)var),name,NULL,NULL,nFlag);
	}else
	if( stricmp(type,"char")==0 )	//Added 2012-03-22.
	{
		m_pProperty->DynAddProperty(AVariant((char)var),name,NULL,NULL,nFlag);
	}
	else
	{//enum
		ENUM_TYPE* pEType = g_EnumTypes.GetType(AString(type));
		if(pEType)
		{//看是否在枚举类型中有
			ASetTemplate<int> *pNew_type = new ASetTemplate<int>;
			m_listTypes.AddTail(pNew_type);
			for(int i=0; i<pEType->dwItemNum; i++)
			{
				pNew_type->AddElement(pEType->listItemName[i],i);
			}
			m_pProperty->DynAddProperty(AVariant(var), name, pNew_type,NULL,nFlag);
			return;
		}else
		{//看生成的模板中有没有
			ExplainEx(type,var,name);
		}
	}
}

void CElementDataDoc::ExplainEx(CString type,AVariant var,CString name)
{
	ASetTemplate<int> *pNew_type = NULL;
	if(stricmp(type,"ext_equipment_type")==0)
	{
		CExtTemplIDSel* _IDSel = new CExtTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->SetValue(var);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM |nFlag);	
	}else
	if(stricmp(type,"ext_projectile_type")==0)
	{
		CExtTemplIDSel* _IDSel = new CExtTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->SetValue(var);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM |nFlag);
	}else
	if(stricmp(type,"ext_npc_type")==0)
	{
		CExtTemplIDSel* _IDSel = new CExtTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->SetValue(var);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM |nFlag);
	}else
	if(stricmp(type,"ext_stone_type")==0)
	{
		CExtTemplIDSel* _IDSel = new CExtTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->SetValue(var);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM |nFlag);
	}else		
	if(stricmp(type,"all_type")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM |nFlag);
	}else 
	if(stricmp(type,"equipment_type")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\装备",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM |nFlag);
	}else
	if(stricmp(type,"war_role_config")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\NPC\\城战角色属性表",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM |nFlag);
	}else
	if(stricmp(type,"matter_type")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\物品",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}else
	if(stricmp(type,"material_type")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}else		
	if(stricmp(type,"npc_type")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\NPC\\NPC类型",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}else
	if(stricmp(type,"mine_type")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\NPC\\矿",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}else		
	if(stricmp(type,"droptable_type")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\掉落表",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}else		
	if(stricmp(type,"monster_type")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\怪物",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}else		
	if(stricmp(type,"pet_type")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\宠物",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}else		
	if(stricmp(type,"pet_egg_type")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\物品\\宠物蛋",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}else		
	if(stricmp(type,"skill_type")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\技能",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}else
	if(stricmp(type,"projectile_type")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\装备\\弹药类型",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}else
	if(stricmp(type,"id_projectile")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\装备\\弹药",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}else
	if(stricmp(type,"visualize_id")==0)
	{
		EquipMask* fs = new EquipMask;
		m_listCustoms.AddTail(fs);
		fs->m_var = var;
		fs->m_pMaskNames = _mask_visualize_id;
		fs->m_nNameCount = sizeof(_mask_visualize_id) / sizeof(LPCTSTR);
		m_pProperty->DynAddProperty(var, name, (ASet*)fs, NULL, WAY_CUSTOM | nFlag);
	}else
	if(stricmp(type,"emotion_id")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\Face\\表情情绪属性",
			".tmpl",
			var,
			enumFaceID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}else
	if(stricmp(type,"factions_mask")==0)
	{
		FactionSel* fs = new FactionSel;
		m_listCustoms.AddTail(fs);
		fs->m_var = var;
		m_pProperty->DynAddProperty(var, name, (ASet*)fs, NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"monster_faction")==0)
	{
		MonsterFaction* fs = new MonsterFaction;
		m_listCustoms.AddTail(fs);
		fs->m_var = var;
		m_pProperty->DynAddProperty(var, name, (ASet*)fs, NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"equip_mask")==0)
	{
		EquipMask* fs = new EquipMask;
		m_listCustoms.AddTail(fs);
		fs->m_var = var;
		fs->m_pMaskNames = _mask_equip_name;
		fs->m_nNameCount = sizeof(_mask_equip_name) / sizeof(LPCTSTR);
		m_pProperty->DynAddProperty(var, name, (ASet*)fs, NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"combined_services")==0)
	{
		EquipMask* fs = new EquipMask;
		m_listCustoms.AddTail(fs);
		fs->m_var = var;
		fs->m_pMaskNames = _combined_services;
		fs->m_nNameCount = sizeof(_combined_services) / sizeof(LPCTSTR);
		m_pProperty->DynAddProperty(var, name, (ASet*)fs, NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type, "combined_services2")==0)
	{
		EquipMask* fs = new EquipMask;
		m_listCustoms.AddTail(fs);
		fs->m_var = var;
		fs->m_pMaskNames = _combined_services2;
		fs->m_nNameCount = sizeof(_combined_services2) / sizeof(LPCTSTR);
		m_pProperty->DynAddProperty(var, name, (ASet*)fs, NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type, "combined_services3")==0)
	{
		EquipMask* fs = new EquipMask;
		m_listCustoms.AddTail(fs);
		fs->m_var = var;
		fs->m_pMaskNames = _combined_services3;
		fs->m_nNameCount = sizeof(_combined_services3) / sizeof(LPCTSTR);
		m_pProperty->DynAddProperty(var, name, (ASet*)fs, NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"immune_type")==0)
	{
		EquipMask* fs = new EquipMask;
		m_listCustoms.AddTail(fs);
		fs->m_var = var;
		fs->m_pMaskNames = _immune_type;
		fs->m_nNameCount = sizeof(_immune_type) / sizeof(LPCTSTR);
		m_pProperty->DynAddProperty(var, name, (ASet*)fs, NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"forbid_food")==0)
	{
		EquipMask* fs = new EquipMask;
		m_listCustoms.AddTail(fs);
		fs->m_var = var;
		fs->m_pMaskNames = _forbid_food_mask;
		fs->m_nNameCount = sizeof(_forbid_food_mask) / sizeof(LPCTSTR);
		m_pProperty->DynAddProperty(var, name, (ASet*)fs, NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"patroll_mode")==0)
	{
		EquipMask* fs = new EquipMask;
		m_listCustoms.AddTail(fs);
		fs->m_var = var;
		fs->m_pMaskNames = _mask_patroll_mode;
		fs->m_nNameCount = sizeof(_mask_patroll_mode) / sizeof(LPCTSTR);
		m_pProperty->DynAddProperty(var, name, (ASet*)fs, NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"character_combo_id")==0)
	{
		OccupationMask* fs = new OccupationMask;
		m_listCustoms.AddTail(fs);
		fs->m_var = var;
		fs->m_pMaskNames = _mask_character_combo_id;
		fs->m_nNameCount = sizeof(_mask_character_combo_id) / sizeof(LPCTSTR);
		m_pProperty->DynAddProperty(var, name, (ASet*)fs, NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type, "character_combo_id2")==0)	//扩展职业ID，Added 2011-07-14.
	{
		OccupationMask* fs = new OccupationMask;
		m_listCustoms.AddTail(fs);
		fs->m_var = var;
		fs->m_pMaskNames = _mask_character_combo_id2;
		fs->m_nNameCount = sizeof(_mask_character_combo_id2) / sizeof(LPCTSTR);
		m_pProperty->DynAddProperty(var, name, (ASet*)fs, NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type, "time_per_day")==0)	//在线奖励时，每天时间（开启和终止发放时间） Added 2011-12-06.
	{
		TimeSetting* fs = new TimeSetting;
		m_listCustoms.AddTail(fs);
		fs->m_var = var;
		fs->m_nFlag = 2;	//表示每天发放类型
		m_pProperty->DynAddProperty(var, name, (ASet*)fs, NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type, "time_per_interval")==0)	//在线奖励时，时间短时间类型 Added 2011-12-06.
	{
		TimeSetting* fs = new TimeSetting;
		m_listCustoms.AddTail(fs);
		fs->m_var = var;
		fs->m_nFlag = 0;	//表示每时间段发放类型
		m_pProperty->DynAddProperty(var, name, (ASet*)fs, NULL, WAY_CUSTOM | nFlag);		
	}
	else
	if(stricmp(type,"food_type")==0)
	{
		EquipMask* fs = new EquipMask;
		m_listCustoms.AddTail(fs);
		fs->m_var = var;
		fs->m_pMaskNames = _mask_pet_food;
		fs->m_nNameCount = sizeof(_mask_pet_food) / sizeof(LPCTSTR);
		m_pProperty->DynAddProperty(var, name, (ASet*)fs, NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"pet_type_mask")==0)
	{
		EquipMask* fs = new EquipMask;
		m_listCustoms.AddTail(fs);
		fs->m_var = var;
		fs->m_pMaskNames = _mask_pet;
		fs->m_nNameCount = sizeof(_mask_pet) / sizeof(LPCTSTR);
		m_pProperty->DynAddProperty(var, name, (ASet*)fs, NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"sect_type")==0)
	{
		EquipMask* fs = new EquipMask;
		m_listCustoms.AddTail(fs);
		fs->m_var = var;
		fs->m_pMaskNames = _mask_sect;
		fs->m_nNameCount = sizeof(_mask_sect) / sizeof(LPCTSTR);
		m_pProperty->DynAddProperty(var, name, (ASet*)fs, NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"astro_type")==0)
	{
		EquipMask* fs = new EquipMask;
		m_listCustoms.AddTail(fs);
		fs->m_var = var;
		fs->m_pMaskNames = _astro_type;
		fs->m_nNameCount = sizeof(_astro_type) / sizeof(LPCTSTR);
		m_pProperty->DynAddProperty(var, name, (ASet*)fs, NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"god_devil_mask")==0)
	{
		EquipMask* fs = new EquipMask;
		m_listCustoms.AddTail(fs);
		fs->m_var = var;
		fs->m_pMaskNames = _god_devil_mask;
		fs->m_nNameCount = sizeof(_god_devil_mask) / sizeof(LPCTSTR);
		m_pProperty->DynAddProperty(var, name, (ASet*)fs, NULL, WAY_CUSTOM | nFlag);
	}
	else if(stricmp(type,"nation_position_mask")==0)
	{
		EquipMask* fs = new EquipMask;
		m_listCustoms.AddTail(fs);
		fs->m_var = var;
		fs->m_pMaskNames = _nation_position_mask;
		fs->m_nNameCount = sizeof(_nation_position_mask) / sizeof(LPCTSTR);
		m_pProperty->DynAddProperty(var, name, (ASet*)fs, NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"gift_bag_mask")==0)
	{
		EquipMask* fs = new EquipMask;
		m_listCustoms.AddTail(fs);
		fs->m_var = var;
		fs->m_pMaskNames = _gift_bag_mask;
		fs->m_nNameCount = sizeof(_gift_bag_mask) / sizeof(LPCTSTR);
		m_pProperty->DynAddProperty(var, name, (ASet*)fs, NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"accident_action")==0)
	{
		EquipMask* fs = new EquipMask;
		m_listCustoms.AddTail(fs);
		fs->m_var = var;
		fs->m_pMaskNames = _accident_action_mask;
		fs->m_nNameCount = sizeof(_accident_action_mask) / sizeof(LPCTSTR);
		m_pProperty->DynAddProperty(var, name, (ASet*)fs, NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"task_type")==0)
	{
		TASK_ID_FUNCS* fs = new TASK_ID_FUNCS;
		m_listCustoms.AddTail(fs);
		fs->OnSetValue(var);
		m_pProperty->DynAddProperty(var, name, (ASet*)fs, NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"proc_type")==0)
	{
		ProcTypeMask* fs = new ProcTypeMask;
		m_listCustoms.AddTail(fs);
		fs->OnSetValue(var);
		m_pProperty->DynAddProperty(var, name, (ASet*)fs, NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"talk_service")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\NPC\\功能\\交谈",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"sell_service")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\NPC\\功能\\出售商品",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"buy_service")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\NPC\\功能\\收购商品",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"repair_service")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\NPC\\功能\\修理商品",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"install_service")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\NPC\\功能\\镶嵌",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"uninstall_service")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\NPC\\功能\\拆除",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"task_out_service")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\NPC\\功能\\发放任务",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"task_in_service")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\NPC\\功能\\验证完成任务",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"task_item_service")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\NPC\\功能\\发放任务物品",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"skill_service")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\NPC\\功能\\教授",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"heal_service")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\NPC\\功能\\治疗",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"transmit_service")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\NPC\\功能\\传送",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"transport_service")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\NPC\\功能\\运输",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"proxy_service")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\NPC\\功能\\代售",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"storage_service")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\NPC\\功能\\存储",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"make_service")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\NPC\\功能\\生产",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"decompose_service")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\NPC\\功能\\分解",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"identify_service")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\NPC\\功能\\鉴定",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"war_towerbuild_service")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\NPC\\功能\\城战炮塔建造",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"resetprop_service")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\NPC\\功能\\洗点服务",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"equipbind_service")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\NPC\\功能\\装备绑定服务",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"war_archer_service")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\NPC\\功能\\城战购买弓箭手",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}	
	else
	if(stricmp(type,"equipdestroy_service")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\NPC\\功能\\装备销毁服务",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"equipundestroy_service")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\NPC\\功能\\装备解除销毁服务",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"item_trade_service")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\NPC\\功能\\物物交易",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"equip_soul_service")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\NPC\\功能\\器魄融合",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"consign_service")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\NPC\\功能\\商品寄售",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"reputation_shop_service")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\NPC\\功能\\声望商店",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"ui_transfer_service")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
			m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\NPC\\功能\\UI传输",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"transform_id")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\Config\\变身属性配置表",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"buff_area_id")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\Config\\buff区域",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"recipe_type")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\配方",
			".tmpl",
			var,
			enumRecipeID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM |nFlag);
	}
	else
	if(stricmp(type,"talk_type")==0)
	{
		TALK_ID_FUNCS* ts = new TALK_ID_FUNCS;
		m_listCustoms.AddTail(ts);
		ts->OnSetValue(var);
		m_pProperty->DynAddProperty(var, name, (ASet*)ts, NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"text_type")==0)
	{
		TextInputFuncs* ts = new TextInputFuncs;
		m_listCustoms.AddTail(ts);
		ts->OnSetValue(var);
		m_pProperty->DynAddProperty(var, name, (ASet*)ts, NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"single_desc")==0)	//Added 2012-03-22.
	{
		TextInputFuncs* ts = new TextInputFuncs;
		m_listCustoms.AddTail(ts);
		ts->OnSetValue(var);
		m_pProperty->DynAddProperty(var, name, (ASet*)ts, NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"double_desc")==0)	//Added 2012-03-22.
	{
		TextInputFuncs* ts = new TextInputFuncs;
		m_listCustoms.AddTail(ts);
		ts->OnSetValue(var);
		m_pProperty->DynAddProperty(var, name, (ASet*)ts, NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"single_text")==0)
	{
		SingleTextFuncs* ts = new SingleTextFuncs;
		m_listCustoms.AddTail(ts);
		ts->OnSetValue(var);
		m_pProperty->DynAddProperty(var, name, (ASet*)ts, NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"ansi_text")==0)
	{
		SingleTextFuncs* ts = new SingleTextFuncs;
		m_listCustoms.AddTail(ts);
		ts->OnSetValue(var);
		m_pProperty->DynAddProperty(var, name, (ASet*)ts, NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"page_title")==0)
	{
		SingleTextFuncs* ts = new SingleTextFuncs;
		m_listCustoms.AddTail(ts);
		ts->OnSetValue(var);
		m_pProperty->DynAddProperty(var, name, (ASet*)ts, NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"data_path")==0)
	{//这儿是新加的路径数据类型,可以按照不同的扩展名对应不同的类型
		CPathSel * ps = new CPathSel;
		m_listCustoms.AddTail(ps);
		ps->Init("","",var);
		m_pProperty->DynAddProperty(var, name, (ASet*)ps->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type,"recipe_type")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\配方",
			".tmpl",
			var,
			enumRecipeID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type, "hook_config")==0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\Config\\互动物体挂点配置表",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type, "interaction_type") == 0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\NPC\\互动物体",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}
	else
	if(stricmp(type, "region_block") == 0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\地块",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}
	else if(stricmp(type, "building_id") == 0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\建筑",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}
	else if(stricmp(type, "stratety_id") == 0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\Config\\副本攻略配置表",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}	
	else if(stricmp(type, "mine_id") == 0)
	{
		CTemplIDSel* _IDSel = new CTemplIDSel;
		m_listCustoms.AddTail(_IDSel);
		_IDSel->Init(
			"BaseData\\NPC\\矿",
			".tmpl",
			var,
			enumBaseID);
		m_pProperty->DynAddProperty(var, name, (ASet*)_IDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | nFlag);
	}
	else
	{
		CString msg;
		msg.Format("CElementDataDoc::ExplainEx(),解释器不能解释的类型--%s",type);
		AfxMessageBox(msg);
		ASSERT(false);
	}
}

bool CElementDataDoc::EnumFiles(ASetTemplate<int>* pType,AVariant var)
{
	bool bFound = false;
	unsigned int id;
	CString path;
	abase::vector<AString> pathArray;
	g_ExtBaseIDMan.GeneratePathArray(pathArray);
	for( int i = 0; i < pathArray.size(); ++i)
	{
		path = pathArray[i];
		id = g_ExtBaseIDMan.GetIDByPath(AString(path));
		if(id==(int)var) bFound = true;
		int pos = path.ReverseFind('\\');
		CString ObjectName = path.Right(pos);
		pType->AddElement(AString(ObjectName),id);
	}
	
	if(!bFound && (int)var != 0) return false;
	else return true;
}

void CElementDataDoc::SetName()
{
	CString title = GetTitle();
	if(!bInited)
	{
		SetTitle(title + szID);
		bInited = true;
	}
}

void CElementDataDoc::OnFileSave() 
{
	// TODO: Add your command handler code here
	CString pathName = GetPathName();
	if(!FileIsReadOnly(AString(pathName)))
		CDocument::OnFileSave();
}

void CElementDataDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
	CDocument::OnCloseDocument();
}

void CElementDataDoc::Save()
{
	OnFileSave();	
}

void CElementDataDoc::DeleteContents() 
{
	CDocument::DeleteContents();
}

BOOL CElementDataDoc::CanCloseFrame(CFrameWnd* pFrame) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(IsModifed())
	{
		SetModifiedFlag(true);
	}
	return CDocument::CanCloseFrame(pFrame);
}
void CElementDataDoc::Fresh()
{
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CElementDataView* pView = (CElementDataView*)GetNextView(pos);
		pView->UpdateData();
	}  
}

void CElementDataDoc::SetTitle(LPCTSTR lpszTitle) 
{
	CDocument::SetTitle(lpszTitle);
}
