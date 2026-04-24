// VssOperation.cpp: implementation of the CVssOperation class.
//
//////////////////////////////////////////////////////////////////////
#include "Global.h"
#include "VssOperation.h"

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
	DWORD dwAccessMask = KEY_READ;

	if (IsWow64())
	{
		//dwAccessMask |= KEY_ALL_ACCESS;
		dwAccessMask |= KEY_WOW64_64KEY;
	}

	while (vss_path[i])
	{
		lResult = RegOpenKeyEx( HKEY_LOCAL_MACHINE, vss_path[i],0, dwAccessMask, &hKey );
		if (lResult == ERROR_SUCCESS)
		{
			break;
		}
		++i;
	}

	if (lResult != ERROR_SUCCESS)
	{
		AfxMessageBox("open key error！");
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

void CVssOperation::InitVss(CMDLOGFUNC* pLogFunc)
{
	CString path = GetVssPath();
	if(path.IsEmpty())
	{
		AfxMessageBox("没有安装 Microsoft Visual SourceSafe！");
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
	CommandLine.Format(" get -GL%s $/%s -R -I-Y",m_szWorkingFolder,szPath);
	ProcessCommand(CommandLine);
}

bool CVssOperation::GetFile(CString szPath)
{
	if(!m_bInit) return false;
	
	CString CommandLine;
	CommandLine.Format(" get -GL%s $/%s -I-Y",m_szWorkingFolder,szPath);
	ProcessCommand(CommandLine);
	if(m_szResult.Find(szPath,0)==-1)
		return false;
	return true;
}

void CVssOperation::AddFile(CString szFileName)
{
	if(!m_bInit) return;
	CString CommandLine;
	CommandLine.Format(" Add %s -B -I-Y",szFileName);//-B Assume the file is binary
	ProcessCommand(CommandLine);
}

void CVssOperation::AddAllFile(CString szPath)
{
	if(!m_bInit) return;
	CString CommandLine;
	CommandLine.Format(" Add %s -R* -B -I-Y",szPath);//-B Assume the file is binary
	ProcessCommand(CommandLine);
}

void CVssOperation::DeleteFile(CString szFileName)
{
	if(!m_bInit) return;

	CString CommandLine;
	CommandLine.Format(" Destroy %s -I-Y",szFileName);
	ProcessCommand(CommandLine);
}

void CVssOperation::CreateProject(CString szPath)
{
	if(!m_bInit) return;
	CString CommandLine;
	CommandLine.Format(" Create %s -I-Y",szPath);
	ProcessCommand(CommandLine);
}

void CVssOperation::SetProjectPath(CString szPath)
{
	if(!m_bInit) return;
	m_szWorkingFolder = szPath;
	CString CommandLine;
	CommandLine.Format(" Cp $/%s -I-Y",szPath);
	ProcessCommand(CommandLine);
}

void CVssOperation::CheckInFile(CString szName)
{
	if(!m_bInit) return;

	CString CommandLine;
	CommandLine.Format(" checkin %s -GL%s -I-Y",szName,m_szWorkingFolder);
	ProcessCommand(CommandLine);
}

void CVssOperation::CheckInAllFile(CString szPath)
{
	if(!m_bInit) return;

	CString CommandLine;
	CommandLine.Format(" checkin %s -P -GL%s -I-Y","*",szPath);
	ProcessCommand(CommandLine);
}

void CVssOperation::CheckOutFile(CString szPathName)
{
	if(!m_bInit) return;
	CString CommandLine;
	CommandLine.Format(" checkout -GL%s $/%s -I-Y",m_szWorkingFolder,szPathName);
	ProcessCommand(CommandLine);
}

void CVssOperation::CheckOutAllFile(CString szPath)
{
	if(!m_bInit) return;
	CString CommandLine;
	CommandLine.Format(" checkout -R-GL%s $/%s -I-Y",m_szWorkingFolder,szPath);
	ProcessCommand(CommandLine);
}

void CVssOperation::UndoCheckOut(CString szName)
{
	if(!m_bInit) return;
	CString CommandLine;
	CommandLine.Format(" undocheckout -GL%s $/%s -I-Y",m_szWorkingFolder,szName);
	ProcessCommand(CommandLine);	
}

int CVssOperation::IsCheckOutStatus(CString szFileName)
{
	CString CommandLine;
	CommandLine.Format(" Status $/%s -U%s -I-Y",szFileName,m_szUserName);
	ProcessCommand(CommandLine);
	if(m_szResult.Find("No files found checked out",0)!=-1)
		return 0;
	else if(m_szResult.Find("is not an existing filename or project",0)!=-1)
		return 2;
	else
		return 1;
}

void CVssOperation::Property(CString szFileName)
{
	CString CommandLine;
	CommandLine.Format(" Properties $/%s -I-Y",szFileName);
	ProcessCommand(CommandLine);
}

void CVssOperation::UndoCheckOutFiles()
{
	if(!m_bInit) return;
	CString CommandLine;
	CommandLine.Format(" Undocheckout * -P -I-Y");
	ProcessCommand(CommandLine);
}

CString CVssOperation::GetFileTypes(CString szPathName)
{

	if(!m_bInit) return "";
	CString CommandLine;
	CommandLine.Format(" filetype %s -I-Y", szPathName);
	ProcessCommand(CommandLine);
	return m_szResult;
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
	m_szResult = "";
	
	//create pipe
	SECURITY_ATTRIBUTES saAttr;
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
	saAttr.bInheritHandle = TRUE; 
	saAttr.lpSecurityDescriptor = NULL; 
	char szBuffer[257];
	HRESULT ret;
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
	char buf[256];
	strcpy(buf, command);

	VERIFY(CreateProcess(
		m_szVssPath + ".exe",
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
		strTemp.Format( "Vss command: %s", buf);
		m_pLogFunc(strTemp);
		if(m_szResult.GetLength() < 1024) m_pLogFunc(m_szResult);
		else m_pLogFunc("......OK");
	}
	
	CloseHandle(hRead1);
	CloseHandle(hWrite1);
	CloseHandle(hRead2);
	CloseHandle(hWrite2);
}

bool CVssOperation::IsWow64()
{
	typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS)(HANDLE, PBOOL);
	LPFN_ISWOW64PROCESS fnlsWow64Process;
	BOOL bIsWow64 = FALSE;
	fnlsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle("kernel32"), "IsWow64Process");
	if (fnlsWow64Process)
	{
		fnlsWow64Process(GetCurrentProcess(), &bIsWow64);
	}
	return bIsWow64;
}