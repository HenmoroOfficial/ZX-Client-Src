# Microsoft Developer Studio Project File - Name="ElementSkill" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ElementSkill - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ElementSkill.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ElementSkill.mak" CFG="ElementSkill - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ElementSkill - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ElementSkill - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ElementSkill - Win32 DbgRelease" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Element/ElementSkill", CBHAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ElementSkill - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELEMENTSKILL_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELEMENTSKILL_EXPORTS" /D "_SKILL_CLIENT" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy Release\ElementSkill.lib ..\lib	copy Release\ElementSkill.dll ..\zbin	copy ElementSkill.h ..\include	xcopy skillstr.txt ..\zbin\configs /R/Y/Q
# End Special Build Tool

!ELSEIF  "$(CFG)" == "ElementSkill - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELEMENTSKILL_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm- /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELEMENTSKILL_EXPORTS" /D "_SKILL_CLIENT" /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"Debug/ElementSkill_d.dll" /pdbtype:sept
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy debug\elementskill_d.lib ..\lib	copy debug\elementskill_d.dll ..\zbin	copy ElementSkill.h ..\include	copy skillstr.txt ..\zbin\configs
# End Special Build Tool

!ELSEIF  "$(CFG)" == "ElementSkill - Win32 DbgRelease"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ElementSkill___Win32_DbgRelease"
# PROP BASE Intermediate_Dir "ElementSkill___Win32_DbgRelease"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "DbgRelease"
# PROP Intermediate_Dir "DbgRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "../include/iolib" /I "../include/stlport" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELEMENTSKILL_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../include/iolib" /I "../include/stlport" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELEMENTSKILL_EXPORTS" /D "_SKILL_CLIENT" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /libpath:"../lib/stlport"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /libpath:"../lib/stlport"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy DbgRelease\ElementSkill.lib ..\lib	copy DbgRelease\ElementSkill.dll ..\zbin	copy ElementSkill.h ..\include	xcopy skillstr.txt ..\zbin\configs /R/Y/Q
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "ElementSkill - Win32 Release"
# Name "ElementSkill - Win32 Debug"
# Name "ElementSkill - Win32 DbgRelease"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ElementSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\gnstring.cpp
# End Source File
# Begin Source File

SOURCE=.\skill.cpp
# End Source File
# Begin Source File

SOURCE=.\skillwrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs1.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs10.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs11.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs12.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs13.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs14.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs15.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs16.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs17.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs18.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs19.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs2.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs20.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs21.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs22.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs23.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs24.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs25.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs26.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs27.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs28.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs29.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs3.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs30.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs31.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs32.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs33.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs34.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs35.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs36.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs37.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs38.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs39.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs4.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs40.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs41.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs42.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs43.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs44.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs45.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs46.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs47.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs48.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs49.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs5.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs50.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs51.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs52.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs53.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs54.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs55.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs56.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs57.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs58.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs59.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs6.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs60.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs61.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs62.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs63.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs64.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs65.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs66.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs67.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs68.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs69.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs7.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs70.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs71.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs72.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs73.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs74.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs75.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs76.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs77.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs78.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs79.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs8.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs80.cpp
# End Source File
# Begin Source File

SOURCE=.\stubs9.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ElementSkill.h
# End Source File
# Begin Source File

SOURCE=.\gnstring.h
# End Source File
# Begin Source File

SOURCE=.\playerwrapper.h
# End Source File
# Begin Source File

SOURCE=.\skill.h
# End Source File
# Begin Source File

SOURCE=.\skillwrapper.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
