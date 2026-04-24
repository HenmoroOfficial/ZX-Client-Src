# Microsoft Developer Studio Project File - Name="TaskDataParserEx" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=TaskDataParserEx - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TaskDataParserEx.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TaskDataParserEx.mak" CFG="TaskDataParserEx - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TaskDataParserEx - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "TaskDataParserEx - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TaskDataParserEx - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../../ZElement/ZElementClient/Task" /I "../../../ZElement/ZElementData" /I "../../ZElement/ZElementClient/task" /I "../../../ZElement/ZElementClient/Network/IOLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "TASK_TEMPL_EDITOR" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "TaskDataParserEx - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../ZElement/ZElementClient/Task" /I "../../../ZElement/ZElementData" /I "../../ZElement/ZElementClient/task" /I "../../../ZElement/ZElementClient/Network/IOLib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "TASK_TEMPL_EDITOR" /D "_AFXDLL" /FD /GZ /c
# SUBTRACT CPP /Fr /YX /Yc /Yu
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "TaskDataParserEx - Win32 Release"
# Name "TaskDataParserEx - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\ZElement\ZElementData\BaseDataIDMan.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\ZElement\ZElementData\BaseDataOrg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\ZElement\ZElementData\BaseDataTemplate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\ZElement\ZElementData\EC_MD5Hash.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\ZElement\ZElementData\elementdataman.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\ZElement\ZElementData\VssOperation.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "TaskTemplCommon"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\ZElement\ZElementClient\Task\TaskAsyncData.h
# End Source File
# Begin Source File

SOURCE=.\TaskDummy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\ZElement\ZElementClient\Task\TaskExpAnalyser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\ZElement\ZElementClient\Task\TaskExpAnalyser.h
# End Source File
# Begin Source File

SOURCE=..\..\..\ZElement\ZElementClient\Task\TaskInterface.h
# End Source File
# Begin Source File

SOURCE=..\..\..\ZElement\ZElementClient\Task\TaskProcess.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\ZElement\ZElementClient\Task\TaskProcess.h
# End Source File
# Begin Source File

SOURCE=..\..\..\ZElement\ZElementClient\Task\TaskTempl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\ZElement\ZElementClient\Task\TaskTempl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\ZElement\ZElementClient\Task\TaskTempl.inl
# End Source File
# Begin Source File

SOURCE=..\..\..\ZElement\ZElementClient\Task\TaskTemplMan.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\ZElement\ZElementClient\Task\TaskTemplMan.h
# End Source File
# End Group
# End Target
# End Project
