#ifndef _NPC_CONTOROLLER_MAN_DLG_
#define _NPC_CONTOROLLER_MAN_DLG_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DynamicObjectManDlg.h : header file
//

#include "vector.h"
#include <hashmap.h>

/////////////////////////////////////////////////////////////////////////////
// CDynamicObjectManDlg dialog

struct CONTROLLER_TIME 
{
	CONTROLLER_TIME()
	{
		nYear = -1;
		nMouth = -1;
		nWeek = -1;
		nDay = -1;
		nHours = -1;
		nMinutes = -1;
	}
	int nYear;//正常区间 [1900-2100] -1 代表任意
	int nMouth;//正常取值区间[0,11]  -1 代表任意
	int nWeek;//正常取值区间[0,6]  0代表星期日 -1 代表任意
	int nDay; //正常取值区间[1,31]  -1 代表任意
	int nHours; //正常取值区间[0,23]  -1 代表任意
	int nMinutes;//正常取值区间[0,59]  -1代表任意
};

struct CONTROLLER_OBJECT
{
	unsigned int id;
	int       nControllerID;// 	控制器的ID，这个ID由策划填数，和以前布置区的ID一致，用于对控制器进行一些操作
	char      szName[128];
	bool      bActived;				//是否初始时就激活
	int       nWaitTime;			//激活后等待多少时间才开始生成对象
	int       nStopTime;			//激活后等待多少时间就自动停止激活 int  0代表此项无效
	
	bool      bActiveTimeInvalid;   
	bool      bStopTimeInvalid;
	CONTROLLER_TIME mActiveTime;	//是否存在定时激活的时间,以及时间值　 特殊结构 
	CONTROLLER_TIME mStopTime;    //是否存在定时停止的时间, 以及时间值   特殊结构
	int       nActiveTimeRange;   //激活的时间范围值,单位是秒
	bool      bRepeatActived;     //是否可重复激活

	int		  nTimeSegNum;			// 低16位为时间段个数，高16位为时间段逻辑关系，0为并集，1为交集
	abase::vector<CONTROLLER_TIME>	aTimeSeg;	// 多个时间段，size大小为nTimeSegNum*2, 奇数个元素为起始时间，偶数个为结束时间


	int GetTimeSegNum()  const { return (nTimeSegNum & 0xffff); }
	int GetTimeSegLogic() const	{ return (nTimeSegNum/0xffff); }	// 0:并集  1:交集
	void SetTimeSegLogic(int nUnion) { nTimeSegNum = (nUnion == 1)?(nTimeSegNum|0x00010000):(nTimeSegNum&0x0000ffff);}
};

struct CONTROLLER_OBJECT_VERSION3
{
	unsigned int id;
	int       nControllerID;
	char      szName[128];
	bool      bActived;	
	int       nWaitTime;
	int       nStopTime;
	
	bool      bActiveTimeInvalid;   
	bool      bStopTimeInvalid;
	CONTROLLER_TIME mActiveTime;
	CONTROLLER_TIME mStopTime; 
	int       nActiveTimeRange;
	bool      bRepeatActived;
};

struct CONTROLLER_OBJECT_VERSION2
{
	unsigned int id;
	int       nControllerID;
	char      szName[128];
	bool      bActived;	
	int       nWaitTime;
	int       nStopTime;
	
	bool      bActiveTimeInvalid;   
	bool      bStopTimeInvalid;
	CONTROLLER_TIME mActiveTime;
	CONTROLLER_TIME mStopTime; 
	int       nActiveTimeRange;
};

struct CONTROLLER_OBJECT_VERSION1
{
	unsigned int id;
	int       nControllerID;
	char      szName[128];
	bool      bActived;			
	int       nWaitTime;		
	int       nStopTime;		
	
	bool      bActiveTimeInvalid;   
	bool      bStopTimeInvalid;
	CONTROLLER_TIME mActiveTime;
	CONTROLLER_TIME mStopTime;    
};


void InitControllerObj(CONTROLLER_OBJECT * pObj);
void InitControllerTime(CONTROLLER_TIME* pObj);

class CNpcControllerManDlg : public CDialog
{
// Construction
	enum
	{
		CONTROLLER_VERSION = 4,
	};
public:
	CNpcControllerManDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDynamicObjectManDlg)
	enum { IDD = IDD_DIALOG_CONTROLLER_OBJECT_MAN };
	CStatic	m_FlagCtrl;
	CListCtrl	m_listObject;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDynamicObjectManDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_VIRTUAL

// Implementation
protected:
	abase::vector<CONTROLLER_OBJECT> m_listControllerObject;
	void FreshObjectList();
	void DelDynamicObj( unsigned int id);
	void GetDynamicObj( unsigned int id, CONTROLLER_OBJECT &obj);
	void SetDynamicObj( unsigned int id, CONTROLLER_OBJECT &obj);

	unsigned int m_uMaxID;
	bool Save();
	bool Load();
	bool Export(const char* szPath);

	bool m_bReadOnly;
	bool m_bChanged;
	HICON m_IconReadOnly;
	HICON m_IconWriteable;
	// Generated message map functions
	//{{AFX_MSG(CDynamicObjectManDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckOut();
	afx_msg void OnCheckIn();
	afx_msg void OnUndoCheckOut();
	afx_msg void OnAddNewobject();
	afx_msg void OnDelObject();
	afx_msg void OnExport();
	afx_msg void OnDblclkListControllerObject(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnColumnClickControllerObject(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class CNpcControllerMan
{

public:
	CNpcControllerMan(){}
	virtual ~CNpcControllerMan(){Release(); }

protected:
	abase::hash_map<unsigned int , CONTROLLER_OBJECT> m_ObjectIDMap;
public:

	void GetObjByID(unsigned int ulID, CONTROLLER_OBJECT& obj)
	{
		abase::hash_map<unsigned int, CONTROLLER_OBJECT>::iterator it = m_ObjectIDMap.find(ulID);
		if(it == m_ObjectIDMap.end())
			return;
		obj = it->second;
	}

	unsigned long GetIdNum()
	{
		return m_ObjectIDMap.size();
	}

	void GetObjByIndex( int idx, CONTROLLER_OBJECT& obj)
	{
		int index = 0;
		abase::hash_map<unsigned int, CONTROLLER_OBJECT>::iterator it = m_ObjectIDMap.begin();
		for( it; it != m_ObjectIDMap.end(); ++it)
		{
			if(index==idx)
			{
				obj = (it->second);
			}
			index++;
		}
	}
	
	void Release()
	{
		m_ObjectIDMap.clear();
	}

	//Return 0 if failed
	bool Load()
	{
		Release();
		
		FILE *pFile = fopen("ResTable\\ControllerObject.dat","rb");
		if(pFile==NULL) return false;
		DWORD dwVersion;
		fread(&dwVersion,sizeof(DWORD),1,pFile);
		if(dwVersion==1)
		{
			int size = 0;
			fread(&size,sizeof(DWORD),1,pFile);
			for( int i = 0; i < size; ++i)
			{
				CONTROLLER_OBJECT_VERSION1 obj1;
				fread(&obj1,sizeof(CONTROLLER_OBJECT_VERSION1),1,pFile);
				
				CONTROLLER_OBJECT obj;
				memcpy(&obj,&obj1,sizeof(CONTROLLER_OBJECT_VERSION1));
				obj.nActiveTimeRange = 60;
				
				obj.bRepeatActived = false;
				obj.nTimeSegNum = 0;

				m_ObjectIDMap[obj.id] = obj;
			}	
		}else if(dwVersion==2)
		{
			int size = 0;
			fread(&size,sizeof(DWORD),1,pFile);
			for( int i = 0; i < size; ++i)
			{
				CONTROLLER_OBJECT_VERSION2 obj2;
				fread(&obj2,sizeof(CONTROLLER_OBJECT_VERSION2),1,pFile);
				
				CONTROLLER_OBJECT obj;
				memcpy(&obj,&obj2,sizeof(CONTROLLER_OBJECT_VERSION2));
				obj.bRepeatActived = false;
				obj.nTimeSegNum = 0;

				m_ObjectIDMap[obj.id] = obj;
			}	
		}else if(dwVersion==3)
		{
			int size = 0;
			fread(&size,sizeof(DWORD),1,pFile);
			for( int i = 0; i < size; ++i)
			{
				CONTROLLER_OBJECT_VERSION3 obj3;
				fread(&obj3,sizeof(CONTROLLER_OBJECT_VERSION3),1,pFile);
				
				CONTROLLER_OBJECT obj;
				memcpy(&obj,&obj3,sizeof(CONTROLLER_OBJECT_VERSION3));
				obj.nTimeSegNum = 0;

				m_ObjectIDMap[obj.id] = obj;
			}
		} else if( dwVersion == 4)
		{
			int size = 0;
			fread(&size,sizeof(DWORD),1,pFile);
			for( int i = 0; i < size; ++i)
			{
				CONTROLLER_OBJECT_VERSION3 obj3;
				fread(&obj3,sizeof(CONTROLLER_OBJECT_VERSION3),1,pFile);
				
				CONTROLLER_OBJECT obj;
				memcpy(&obj,&obj3,sizeof(CONTROLLER_OBJECT_VERSION3));

				int num;
				fread(&num, sizeof(int), 1, pFile);
				obj.nTimeSegNum = num;

				for(int j=0; j<obj.GetTimeSegNum(); j++)
				{
					CONTROLLER_TIME _time;
					fread(&_time, sizeof(CONTROLLER_TIME), 1, pFile);
					obj.aTimeSeg.push_back(_time);

					fread(&_time, sizeof(CONTROLLER_TIME), 1, pFile);
					obj.aTimeSeg.push_back(_time);
				}

				m_ObjectIDMap[obj.id] = obj;
			}
		}

		fclose(pFile);
		return true;
	}
	
};

extern CNpcControllerMan g_ControllerObjectMan;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DYNAMICOBJECTMANDLG_H__6AB2C6FC_7BE0_4C68_85BE_B6FE6AA2F6CB__INCLUDED_)
