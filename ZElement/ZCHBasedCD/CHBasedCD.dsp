# Microsoft Developer Studio Project File - Name="CHBasedCD" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=CHBasedCD - Win32 Mem_Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CHBasedCD.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CHBasedCD.mak" CFG="CHBasedCD - Win32 Mem_Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CHBasedCD - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "CHBasedCD - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "CHBasedCD - Win32 DbgRelease" (based on "Win32 (x86) Static Library")
!MESSAGE "CHBasedCD - Win32 SDebug" (based on "Win32 (x86) Static Library")
!MESSAGE "CHBasedCD - Win32 SRelease" (based on "Win32 (x86) Static Library")
!MESSAGE "CHBasedCD - Win32 Mem_Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/CHBasedCD", RKDAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CHBasedCD - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "BACK_VERSION" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy release\*.lib ..\lib\cd\ /R /Y /Q	xcopy *.h ..\include\cd\ /R /Y /Q
# End Special Build Tool

!ELSEIF  "$(CFG)" == "CHBasedCD - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "BACK_VERSION" /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\CHBasedCD_d.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=copy files to the lib
PostBuild_Cmds=xcopy debug\*.lib ..\lib\cd\ /R /Y /Q	xcopy *.h ..\include\cd\ /R /Y /Q
# End Special Build Tool

!ELSEIF  "$(CFG)" == "CHBasedCD - Win32 DbgRelease"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "CHBasedCD___Win32_DbgRelease"
# PROP BASE Intermediate_Dir "CHBasedCD___Win32_DbgRelease"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "DbgRelease"
# PROP Intermediate_Dir "DbgRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "BACK_VERSION" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"DbgRelease\CHBasedCD_su.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy dbgrelease\*.lib ..\lib\cd\ /R /Y /Q	xcopy *.h ..\include\cd\ /R /Y /Q
# End Special Build Tool

!ELSEIF  "$(CFG)" == "CHBasedCD - Win32 SDebug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CHBasedCD___Win32_SDebug"
# PROP BASE Intermediate_Dir "CHBasedCD___Win32_SDebug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "SDebug"
# PROP Intermediate_Dir "SDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_A_FORBID_NEWDELETE" /D "_A_FORBID_MALLOC" /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Debug\CHBasedCD_d.lib"
# ADD LIB32 /nologo /out:"sDebug\CHBasedCD_sd.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=copy files to the lib
PostBuild_Cmds=xcopy sdebug\*.lib ..\lib\cd\ /R /Y /Q	xcopy *.h ..\include\cd\ /R /Y /Q
# End Special Build Tool

!ELSEIF  "$(CFG)" == "CHBasedCD - Win32 SRelease"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "CHBasedCD___Win32_SRelease"
# PROP BASE Intermediate_Dir "CHBasedCD___Win32_SRelease"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "CHBasedCD___Win32_SRelease"
# PROP Intermediate_Dir "CHBasedCD___Win32_SRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_A_FORBID_NEWDELETE" /D "_A_FORBID_MALLOC" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"CHBasedCD___Win32_SRelease\CHBasedCD_s.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy CHBasedCD___Win32_SRelease\*.lib ..\lib\cd\ /R /Y /Q	xcopy *.h ..\include\cd\ /R /Y /Q
# End Special Build Tool

!ELSEIF  "$(CFG)" == "CHBasedCD - Win32 Mem_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CHBasedCD___Win32_Mem_Debug"
# PROP BASE Intermediate_Dir "CHBasedCD___Win32_Mem_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Mem_Debug"
# PROP Intermediate_Dir "Mem_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_A_FORBID_NEWDELETE" /D "_A_FORBID_MALLOC" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_A_FORBID_NEWDELETE" /D "_A_FORBID_MALLOC" /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"sDebug\CHBasedCD_sd.lib"
# ADD LIB32 /nologo /out:"Mem_Debug\CHBasedCD_smd.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=copy files to the lib
PostBuild_Cmds=xcopy mem_debug\*.lib ..\lib\cd\ /R /Y /Q	xcopy *.h ..\include\cd\ /R /Y /Q
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "CHBasedCD - Win32 Release"
# Name "CHBasedCD - Win32 Debug"
# Name "CHBasedCD - Win32 DbgRelease"
# Name "CHBasedCD - Win32 SDebug"
# Name "CHBasedCD - Win32 SRelease"
# Name "CHBasedCD - Win32 Mem_Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\2DGiftWrap.cpp
# End Source File
# Begin Source File

SOURCE=.\2DObstruct.cpp
# End Source File
# Begin Source File

SOURCE=.\aabbcd.cpp
# End Source File
# Begin Source File

SOURCE=.\CDWithCH.cpp
# End Source File
# Begin Source File

SOURCE=.\CombGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\ConvexHullAlgorithm.cpp
# End Source File
# Begin Source File

SOURCE=.\ConvexHullData.cpp
# End Source File
# Begin Source File

SOURCE=.\ConvexPolytope.cpp
# End Source File
# Begin Source File

SOURCE=.\EdgeStack.cpp
# End Source File
# Begin Source File

SOURCE=.\Face.cpp
# End Source File
# Begin Source File

SOURCE=.\GiftWrap.cpp
# End Source File
# Begin Source File

SOURCE=.\HalfSpace.cpp
# End Source File
# Begin Source File

SOURCE=.\LEquations.cpp
# End Source File
# Begin Source File

SOURCE=.\Matrix.cpp
# End Source File
# Begin Source File

SOURCE=.\Patch.cpp
# End Source File
# Begin Source File

SOURCE=.\RightPrism.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\2DGiftWrap.h
# End Source File
# Begin Source File

SOURCE=.\2DObstruct.h
# End Source File
# Begin Source File

SOURCE=.\aabbcd.h
# End Source File
# Begin Source File

SOURCE=.\CDWithCH.h
# End Source File
# Begin Source File

SOURCE=.\CombGenerator.h
# End Source File
# Begin Source File

SOURCE=.\ConvexHullAlgorithm.h
# End Source File
# Begin Source File

SOURCE=.\ConvexHullData.h
# End Source File
# Begin Source File

SOURCE=.\ConvexPolytope.h
# End Source File
# Begin Source File

SOURCE=.\EdgeStack.h
# End Source File
# Begin Source File

SOURCE=.\Face.h
# End Source File
# Begin Source File

SOURCE=.\GiftWrap.h
# End Source File
# Begin Source File

SOURCE=.\HalfSpace.h
# End Source File
# Begin Source File

SOURCE=.\LEquations.h
# End Source File
# Begin Source File

SOURCE=.\Matrix.h
# End Source File
# Begin Source File

SOURCE=.\Patch.h
# End Source File
# Begin Source File

SOURCE=.\RightPrism.h
# End Source File
# End Group
# End Target
# End Project
