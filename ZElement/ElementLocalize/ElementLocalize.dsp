# Microsoft Developer Studio Project File - Name="ElementLocalize" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ElementLocalize - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ElementLocalize.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ElementLocalize.mak" CFG="ElementLocalize - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ElementLocalize - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ElementLocalize - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/ZElement/ElementLocalize", UOLAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ElementLocalize - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../ZElementClient/task" /I "../ZCommon" /I "../ZElementData" /I "../include/luawrapper" /I "../include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "UNICODE" /D "TASK_TEMPL_EDITOR" /D "_A_FORBID_NEWDELETE" /D "_A_FORBID_MALLOC" /D "_MD5_CHECK" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 AngelicaCommon_su.lib AngelicaFile_su.lib zliblib.lib luawrapper_s.lib lua5.1.mt.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /out:"Release/ZElementLocalize.exe" /libpath:"../lib" /libpath:"../lib/aui"

!ELSEIF  "$(CFG)" == "ElementLocalize - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../ZElementClient/task" /I "../ZCommon" /I "../ZElementData" /I "../include/luawrapper" /I "../include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "UNICODE" /D "TASK_TEMPL_EDITOR" /D "_A_FORBID_NEWDELETE" /D "_A_FORBID_MALLOC" /D "_MD5_CHECK" /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 AngelicaCommon_sud.lib AngelicaFile_sud.lib zliblib.lib lua5.1.mtd.lib luawrapper_sd.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /incremental:no /debug /machine:I386 /out:"Debug/ZElementLocalize.exe" /pdbtype:sept /libpath:"../lib" /libpath:"../lib/aui"

!ENDIF 

# Begin Target

# Name "ElementLocalize - Win32 Release"
# Name "ElementLocalize - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\common.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgProgress.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSelectItems.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgToxmlOption.cpp
# End Source File
# Begin Source File

SOURCE=.\ElementDataView.cpp
# End Source File
# Begin Source File

SOURCE=.\ElementLocalize.cpp
# End Source File
# Begin Source File

SOURCE=.\ElementLocalize.rc
# End Source File
# Begin Source File

SOURCE=.\ElementLocalizeDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ElementLocalizeView.cpp
# End Source File
# Begin Source File

SOURCE=.\FinderDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\global.cpp
# End Source File
# Begin Source File

SOURCE=.\ImportProgressDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectTransDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\XmlLocalize.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\common.h
# End Source File
# Begin Source File

SOURCE=.\DlgProgress.h
# End Source File
# Begin Source File

SOURCE=.\DlgSelectItems.h
# End Source File
# Begin Source File

SOURCE=.\DlgToxmlOption.h
# End Source File
# Begin Source File

SOURCE=.\ElementDataView.h
# End Source File
# Begin Source File

SOURCE=.\ElementLocalize.h
# End Source File
# Begin Source File

SOURCE=.\ElementLocalizeDoc.h
# End Source File
# Begin Source File

SOURCE=.\ElementLocalizeView.h
# End Source File
# Begin Source File

SOURCE=.\FinderDlg.h
# End Source File
# Begin Source File

SOURCE=.\ImportProgressDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SelectTransDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\XmlLocalize.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\ElementLocalize.ico
# End Source File
# Begin Source File

SOURCE=.\res\ElementLocalize.rc2
# End Source File
# Begin Source File

SOURCE=.\res\ElementLocalizeDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\listtag.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Group "Template"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DataManager.cpp
# End Source File
# Begin Source File

SOURCE=.\DataManager.h
# End Source File
# Begin Source File

SOURCE=.\EC_StringTab.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_StringTab.h
# End Source File
# Begin Source File

SOURCE=.\ElementInterfaceMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\ElementInterfaceMgr.h
# End Source File
# Begin Source File

SOURCE=..\ZElementData\ExpTypes.cpp
# End Source File
# Begin Source File

SOURCE=.\GlobalDataTemplate.cpp
# End Source File
# Begin Source File

SOURCE=.\GlobalDataTemplate.h
# End Source File
# Begin Source File

SOURCE=.\LuaBind.cpp
# End Source File
# Begin Source File

SOURCE=.\LuaBind.h
# End Source File
# Begin Source File

SOURCE=.\PolicyTemplate.cpp
# End Source File
# Begin Source File

SOURCE=.\PolicyTemplate.h
# End Source File
# Begin Source File

SOURCE=.\PrecinctData.cpp
# End Source File
# Begin Source File

SOURCE=.\PrecinctData.h
# End Source File
# Begin Source File

SOURCE=.\PrecinctTemplate.cpp
# End Source File
# Begin Source File

SOURCE=.\PrecinctTemplate.h
# End Source File
# Begin Source File

SOURCE=.\TaskTemplate.cpp
# End Source File
# Begin Source File

SOURCE=.\TaskTemplate.h
# End Source File
# Begin Source File

SOURCE=.\TemplateBase.cpp
# End Source File
# Begin Source File

SOURCE=.\TemplateBase.h
# End Source File
# Begin Source File

SOURCE=.\TemplateManager.cpp
# End Source File
# Begin Source File

SOURCE=.\TemplateManager.h
# End Source File
# Begin Source File

SOURCE=.\TextBase.cpp
# End Source File
# Begin Source File

SOURCE=.\TextBase.h
# End Source File
# Begin Source File

SOURCE=.\TextFileMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\TextFileMgr.h
# End Source File
# End Group
# Begin Group "Command"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ZAUInterface\AXMLFile.cpp
# End Source File
# Begin Source File

SOURCE=..\ZAUInterface\AXMLFile.h
# End Source File
# Begin Source File

SOURCE=..\ZElementData\BaseDataIDMan.cpp
# End Source File
# Begin Source File

SOURCE=..\ZElementData\DataPathMan.cpp
# End Source File
# Begin Source File

SOURCE=..\ZElementData\DataPathMan.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EC_MD5Hash.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EC_MD5Hash.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\elementdataman.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\elementdataman.h
# End Source File
# Begin Source File

SOURCE=..\ZElementData\EnumTypes.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\globaldataman.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\globaldataman.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\hometowntemplate.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\hometowntemplate.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\Task\TaskExpAnalyser.cpp
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\Task\TaskExpAnalyser.h
# End Source File
# Begin Source File

SOURCE=.\TaskInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\Task\TaskInterface.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\Task\TaskProcess.cpp
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\Task\TaskProcess.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\Task\TaskTempl.cpp
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\Task\TaskTempl.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\Task\TaskTempl.inl
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\Task\TaskTemplMan.cpp
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\Task\TaskTemplMan.h
# End Source File
# Begin Source File

SOURCE=..\ZElementData\VssOperation.cpp
# End Source File
# Begin Source File

SOURCE=..\ZElementData\VssOperation.h
# End Source File
# End Group
# Begin Group "policy"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ZElementData\Policy.cpp
# End Source File
# End Group
# Begin Group "Treasure House"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ZElementData\Treasure.cpp
# End Source File
# End Group
# Begin Group "Big5"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Jb.cpp
# End Source File
# Begin Source File

SOURCE=.\Jb.h
# End Source File
# End Group
# Begin Group "Excel"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CSpreadSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\CSpreadSheet.h
# End Source File
# Begin Source File

SOURCE=.\Excel.cpp
# End Source File
# Begin Source File

SOURCE=.\Excel.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
