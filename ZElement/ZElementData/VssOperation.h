// VssOperation.h: interface for the CVssOperation class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _VSS_OPERATION_
#define _VSS_OPERATION_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef void CALLBACK CMDLOGFUNC(const TCHAR*szLog);

class CVssOperation  
{

public:
	CVssOperation();
	virtual ~CVssOperation();
	CString GetVssPath();
	
	void SetServerPath(CString szServerPath){ m_szServerPath = szServerPath; };
	void SetUserName(CString szUser){ m_szUserName = szUser; };
	void SetPassword(CString szPassWord) { m_szPassword = szPassWord; };
	void SetWorkFolder(CString szWorkingFolder){ m_szWorkingFolder = szWorkingFolder; };
	void InitVss( CMDLOGFUNC* pLogFunc, bool bClose = false);
	
	void RunStartCode();
	bool GetFile(CString szPath);
	void AddFile(CString szFileName);
	void AddAllFile(CString szPath);
	void CreateProject(CString szPath);
	void SetProjectPath(CString szPath);
	void GetAllFiles(CString szPath);
	void CheckInFile(CString szName);
	void CheckInAllFile(CString szPath);
	void CheckOutFile(CString szPathName);
	void CheckOutAllFile(CString szPath);
	void DeleteFile(CString szFileName);
	void UndoCheckOut(CString szName);
	bool IsCheckOutStatus(CString szFileName);
	void Property(CString szFileName);
	void UndoCheckOutFiles();
	CString GetFileTypes(CString szPathName);
	void GetFileLastWriteInfo(CString szPathName, CString &strUser, int &iTime);
	CString GetUserCheckout(CString szPathName);
	bool TestVssConnected(CString strMsg);
	
	inline CString GetLastCommandResult(){ return m_szResult; }
	inline bool IsInited(){ return m_bInit; }
	
	int GetTime(int y, int mon, int d, int h, int min);
	CString GetTimeString(int t);

protected:

	static DWORD WINAPI ReadAndHandleOutput(LPVOID param);
	void ProcessCommand(CString command);
	CString m_szResult;
	CString m_szUserName;
	CString m_szPassword;
	CString m_szServerPath;
	CString m_szWorkingFolder;
	CString m_szVssPath;
	CMDLOGFUNC *m_pLogFunc;
	bool m_bInit;

};

#endif //_VSS_OPERATION_
