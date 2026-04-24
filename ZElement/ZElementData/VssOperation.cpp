// VssOperation.cpp: implementation of the CVssOperation class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "VssOperation.h"
#include "ACHAR.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVssOperation::CVssOperation()
{
	m_bInit = false;
	m_pLogFunc = NULL;
}

CVssOperation::~CVssOperation()
{

}

CString CVssOperation::GetVssPath()
{
	static TCHAR strNull[2] = _T("");
    static TCHAR strPath[MAX_PATH];
    DWORD dwType;
    DWORD dwSize = MAX_PATH;
    HKEY  hKey;

	const TCHAR * vss_path[] = 
	{
		_T("Software\\Microsoft\\VisualStudio\\6.0\\setup\\Microsoft Visual SourceSafe\\"),
		_T("Software\\Microsoft\\VisualStudio\\8.0\\setup\\vs\\vss\\"),
		NULL
	};

	int i = 0;
	LONG lResult;
	while (vss_path[i])
	{
		lResult = RegOpenKeyEx( HKEY_LOCAL_MACHINE, vss_path[i],0, KEY_READ, &hKey );
		if (lResult == ERROR_SUCCESS)
		{
			break;
		}
		++i;
	}

	if (lResult != ERROR_SUCCESS)
	{
		return strNull;
	}
	
	lResult = RegQueryValueEx( hKey, _T("ProductDir"), NULL,
                              &dwType, (BYTE*)strPath, &dwSize );
    RegCloseKey( hKey );

    if( ERROR_SUCCESS != lResult )
        return strNull;
	CString path(strPath);
	
	if (i== 0)
	{//version 6.0
		path += "\\win32\\";
	}

	return path;
}

void CVssOperation::InitVss(CMDLOGFUNC* pLogFunc, bool bClose)
{
	if(bClose)
	{
		m_bInit = false;
		return;
	}

	CString path = GetVssPath();
	if(path.IsEmpty())
	{
		AfxMessageBox(_T("没有安装 Microsoft Visual SourceSafe！"));
		m_bInit = false;
		exit(0);
	}
	m_szVssPath = path + "ss";
	m_bInit = true;
	m_pLogFunc = pLogFunc;
}

void CVssOperation::RunStartCode()
{
	/*
	CString CommandLine;
	CommandLine.Format("set ssuser=%s",m_szUserName);
	CommandLine = CommandLine;
	::WinExec(CommandLine,SW_SHOW);
	CommandLine.Format("set sspwd=%s",m_szPassword);	
	CommandLine = CommandLine;
	::WinExec(CommandLine,SW_SHOW);
	CommandLine.Format("set ssdir=%s",m_szServerPath);
	CommandLine = CommandLine;
	::WinExec(CommandLine,SW_SHOW);*/
}

void CVssOperation::GetAllFiles(CString szPath)
{
	if(!m_bInit) return;

	CString CommandLine;
	CommandLine.Format(_T(" get -GL%s $/%s -R -I-Y -GTM"),m_szWorkingFolder,szPath);
	ProcessCommand(CommandLine);
}

bool CVssOperation::GetFile(CString szPath)
{
	if(!m_bInit) return false;
	
	CString CommandLine;
	CommandLine.Format(_T(" get -GL%s $/%s -I-Y -GTM"),m_szWorkingFolder,szPath);
	ProcessCommand(CommandLine);
	if(m_szResult.Find(szPath,0)==-1)
		return false;
	return true;
}

void CVssOperation::AddFile(CString szFileName)
{
	if(!m_bInit) return;
	CString CommandLine;
	CommandLine.Format(_T(" Add %s -B -I-Y"),szFileName);//-B Assume the file is binary
	ProcessCommand(CommandLine);
}

void CVssOperation::AddAllFile(CString szPath)
{
	if(!m_bInit) return;
	CString CommandLine;
	CommandLine.Format(_T(" Add %s -R* -B -I-Y"),szPath);//-B Assume the file is binary
	ProcessCommand(CommandLine);
}

void CVssOperation::DeleteFile(CString szFileName)
{
	if(!m_bInit) return;

	CString CommandLine;
	CommandLine.Format(_T(" Destroy %s -I-Y"),szFileName);
	ProcessCommand(CommandLine);
}

void CVssOperation::CreateProject(CString szPath)
{
	if(!m_bInit) return;
	CString CommandLine;
	CommandLine.Format(_T(" Create %s -I-Y"),szPath);
	ProcessCommand(CommandLine);
}

void CVssOperation::SetProjectPath(CString szPath)
{
	if(!m_bInit) return;
	m_szWorkingFolder = szPath;
	CString CommandLine;
	CommandLine.Format(_T(" Cp $/%s -I-Y"),szPath);
	ProcessCommand(CommandLine);
}

void CVssOperation::CheckInFile(CString szName)
{
	if(!m_bInit) return;

	CString CommandLine;
	CommandLine.Format(_T(" checkin %s -GL%s -I-Y"),szName,m_szWorkingFolder);
	ProcessCommand(CommandLine);
}

void CVssOperation::CheckInAllFile(CString szPath)
{
	if(!m_bInit) return;

	CString CommandLine;
	CommandLine.Format(_T(" checkin %s -P -GL%s -I-Y"),_T("*"),szPath);
	ProcessCommand(CommandLine);
}

void CVssOperation::CheckOutFile(CString szPathName)
{
	if(!m_bInit) return;
	CString CommandLine;
	CommandLine.Format(_T(" checkout -GL%s $/%s -I-Y -GTM"),m_szWorkingFolder,szPathName);
	ProcessCommand(CommandLine);
}

void CVssOperation::CheckOutAllFile(CString szPath)
{
	if(!m_bInit) return;
	CString CommandLine;
	CommandLine.Format(_T(" checkout -R-GL%s $/%s -I-Y -GTM"),m_szWorkingFolder,szPath);
	ProcessCommand(CommandLine);
}

void CVssOperation::UndoCheckOut(CString szName)
{
	if(!m_bInit) return;
	CString CommandLine;
	CommandLine.Format(_T(" undocheckout -GL%s $/%s -I-Y -GTM"),m_szWorkingFolder,szName);
	ProcessCommand(CommandLine);	
}

bool CVssOperation::IsCheckOutStatus(CString szFileName)
{
	CString CommandLine;
	CommandLine.Format(_T(" Status $/%s -U%s -I-Y"),szFileName,m_szUserName);
	ProcessCommand(CommandLine);
	if(m_szResult.Find(_T("No files found checked out"),0)!=-1)
		return false;
	return true;
}

void CVssOperation::Property(CString szFileName)
{
	CString CommandLine;
	CommandLine.Format(_T(" Properties $/%s -I-Y"),szFileName);
	ProcessCommand(CommandLine);
}

void CVssOperation::UndoCheckOutFiles()
{
	if(!m_bInit) return;
	CString CommandLine;
	CommandLine.Format(_T(" Undocheckout * -P -I-Y -GTM"));
	ProcessCommand(CommandLine);
}

CString CVssOperation::GetFileTypes(CString szPathName)
{

	if(!m_bInit) return _T("");
	CString CommandLine;
	CommandLine.Format(_T(" filetype %s -I-Y"), szPathName);
	ProcessCommand(CommandLine);
	return m_szResult;
}

void CVssOperation::GetFileLastWriteInfo(CString szPathName, CString &strUser, int &iTime)
{
	strUser.Empty();
	iTime = 0;

	int index;
	if ((index=szPathName.ReverseFind('\\'))>=0)
	{
		SetProjectPath(szPathName.Left(index));
		szPathName = szPathName.Mid(index+1, szPathName.GetLength()-index-1);
	}

	if(!m_bInit) return ;
	CString CommandLine;
	CommandLine.Format(_T(" History %s -I-Y. -#1"), szPathName);
	ProcessCommand(CommandLine);
	/* 正常格式：
	History of $/BaseData/...
	************ Version xx *****************
	User: Pw_***_*******    Data: 13-01-25 Time:10:15
	Checked in $BaseData/...
	Comment: 
	*/
	if ((index=m_szResult.Find(_T("User: ")))>0)
	{
		int index1;
		if ((index1=m_szResult.Find(_T("Date:")))>0)
		{
			strUser = m_szResult.Mid(index+6, index1-index-6); // strlen("User: ")=6
			strUser.TrimRight(_T(" "));

			CString strInfo;
			strInfo = m_szResult.Mid(index1+6, 8); // 1 = strlen("Date: ") 14 = strlen("13-03-06)
			int y, mon, d, h, min;
			a_sscanf(strInfo.GetBuffer(0), _T("%d-%d-%d"), &y, &mon, &d);

			if ((index1=m_szResult.Find(_T("Time:")))>0)
			{
				strInfo = m_szResult.Mid(index1+6, 5); // 1 = strlen("Time: ") 14 = strlen("13:17")
				a_sscanf(strInfo.GetBuffer(0), _T("%d:%d"), &h, &min);
				iTime = GetTime(y, mon, d, h, min);
			}
		}

	}
}

CString CVssOperation::GetUserCheckout(CString szPathName)
{
	CheckOutFile(szPathName);

	int index;
	CString strRet;
	if ((index=m_szResult.Find(_T("checked out by ")))>0)
	{
		int index1;
		if ((index1=m_szResult.Find(_T("."), index))>0)
		{
			strRet = m_szResult.Mid(index+15, index1-index-15); // strlen("checked out by ")=15
			strRet.TrimRight(_T(" "));
		}
	}
	return strRet;
}

bool CVssOperation::TestVssConnected(CString strMsg)
{
	CString strNOVSS=_T("No VSS database");
	int len= strNOVSS.GetLength();
	if (strMsg.GetLength()>len) 
	{
		if (strNOVSS == strMsg.Left(len)) 
			return false;
	}
	return true;
}

struct DOUBLEHANDEL
{
	HANDLE hPipe;
	HANDLE hProcess;
	CString strResult;
};

DWORD WINAPI CVssOperation::ReadAndHandleOutput(LPVOID param)
{
	DWORD nBytesRead;
	BYTE szBuffer[256];
	HANDLE hRead = ((DOUBLEHANDEL*)param)->hPipe;
	HANDLE hProcess = ((DOUBLEHANDEL*)param)->hProcess;
	while(TRUE)
	{
		//use PeekNamedPipe to avoid dead lock.
		DWORD dwTotalBytes=0;
		DWORD dwTotalLeft=0;
		BOOL returnValue= PeekNamedPipe(hRead, szBuffer, sizeof(szBuffer),\
			                            &nBytesRead, &dwTotalBytes, &dwTotalLeft);
		if (returnValue) 
		{
			if (dwTotalBytes!=0) 
			{
				memset(szBuffer,0,256);
				if (ReadFile(hRead, szBuffer, sizeof(szBuffer), &nBytesRead, NULL) \
					&& nBytesRead)
				{
					((DOUBLEHANDEL*)param)->strResult += (char*)szBuffer;
					CString msg = szBuffer;
					if(msg.Find(_T("Username"))>=0 && msg.Find(_T("Password"))>=0)
					{
						TerminateProcess(hProcess,1);
						return 1;
					}
				}
			}
		}
		
		DWORD dwExitCode;
		if(GetExitCodeProcess(hProcess,&dwExitCode))
		{
			if(dwExitCode==STILL_ACTIVE)
			{
				Sleep(100);
			}else return 1;
		}
			
	}
	return 1;
}

void CVssOperation::ProcessCommand(CString command)
{
	m_szResult = _T("");
	
	//create pipe
	SECURITY_ATTRIBUTES saAttr;
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
	saAttr.bInheritHandle = TRUE; 
	saAttr.lpSecurityDescriptor = NULL; 
	
	HANDLE hRead1,hWrite1,hRead2,hWrite2;
	if (!CreatePipe(&hRead1, &hWrite1, &saAttr, 1024*1024) || !CreatePipe(&hRead2, &hWrite2, &saAttr, 1024*1024))
		return;
	

	//create process
	STARTUPINFO si;
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESTDHANDLES ;
	si.hStdOutput = hWrite1; 
	si.hStdInput  = hRead2;
	si.hStdError = hWrite1; 
	
	PROCESS_INFORMATION pi;
	TCHAR buf[256];
	_tcscpy(buf, command);

	VERIFY(CreateProcess(
		m_szVssPath + _T(".exe"),
		buf,
		NULL,
		NULL,
		TRUE,
		CREATE_NO_WINDOW,
		NULL,
		NULL,
		&si,
		&pi));

	DWORD ThreadId;
	DOUBLEHANDEL dHandle;
	dHandle.hPipe = hRead1;
	dHandle.hProcess = pi.hProcess;
	HANDLE hThread = CreateThread(NULL, 0, ReadAndHandleOutput, (LPVOID)&dHandle, 0, &ThreadId);
	if (hThread == NULL) 
	{
		TerminateProcess(pi.hProcess,1);
		CloseHandle(hRead1);
		CloseHandle(hWrite1);
		CloseHandle(hRead2);
		CloseHandle(hWrite2);
		return;
	}

	WaitForSingleObject(hThread,INFINITE);
	
	m_szResult = dHandle.strResult;
	if(m_pLogFunc) 
	{
		CString strTemp;
		strTemp.Format( _T("Vss command: %s"), buf);
		m_pLogFunc(strTemp);
		if(m_szResult.GetLength() < 1024) m_pLogFunc(m_szResult);
		else m_pLogFunc(_T("......OK"));
	}
	
	CloseHandle(hRead1);
	CloseHandle(hWrite1);
	CloseHandle(hRead2);
	CloseHandle(hWrite2);
}

int CVssOperation::GetTime(int y, int mon, int d, int h, int min)
{
	return (y%100)*10000000 + (mon*40+d)*10000 + h*100 + min;
}

CString CVssOperation::GetTimeString(int t)
{
	CString strRet;
	strRet.Format(_T("%d-%d-%d %d:%d"), t/10000000, ((t%10000000)/10000)/40, ((t%10000000)/10000)%40, (t%10000)/100, t%100);
	return strRet;
}