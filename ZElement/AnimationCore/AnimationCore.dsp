# Microsoft Developer Studio Project File - Name="AnimationCore" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=AnimationCore - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AnimationCore.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AnimationCore.mak" CFG="AnimationCore - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AnimationCore - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "AnimationCore - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "AnimationCore - Win32 DbgRelease" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/ZElement/AnimationCore/AnimationCore", NECCAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AnimationCore - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\include\stlport" /I "..\include\luawrapper" /I "..\include\CD" /I "..\ZElementClient" /I "..\ZCommon" /I "..\ZElementClient\Network" /I "..\ZElementClient\Network\rpcdata" /I "..\ZElementClient\Network\iolib" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "UNICODE" /D "BACK_VERSION" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Lib\AnimationCore.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Updating AnimationCore SDK...
PostBuild_Cmds=xcopy *.h ..\include\AnimationCore\ /R /Y /Q	xcopy Lib\*.lib ..\lib\AnimationCore\ /R /Y /Q
# End Special Build Tool

!ELSEIF  "$(CFG)" == "AnimationCore - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\include\stlport" /I "..\include\luawrapper" /I "..\include\CD" /I "..\ZElementClient" /I "..\ZCommon" /I "..\ZElementClient\Network" /I "..\ZElementClient\Network\rpcdata" /I "..\ZElementClient\Network\iolib" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "UNICODE" /D "BACK_VERSION" /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Lib\AnimationCore_d.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Updating AnimationCore SDK...
PostBuild_Cmds=xcopy *.h ..\include\AnimationCore\ /R /Y /Q	xcopy Lib\*.lib ..\lib\AnimationCore\ /R /Y /Q
# End Special Build Tool

!ELSEIF  "$(CFG)" == "AnimationCore - Win32 DbgRelease"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "AnimationCore___Win32_DbgRelease"
# PROP BASE Intermediate_Dir "AnimationCore___Win32_DbgRelease"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "AnimationCore___Win32_DbgRelease"
# PROP Intermediate_Dir "AnimationCore___Win32_DbgRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GR /GX /O2 /I "..\include\stlport" /I "..\include\luawrapper" /I "..\include\CD" /I "..\ZElementClient" /I "..\ZCommon" /I "..\ZElementClient\Network" /I "..\ZElementClient\Network\rpcdata" /I "..\ZElementClient\Network\iolib" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "UNICODE" /D "BACK_VERSION" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /Zi /O2 /I "..\include\stlport" /I "..\include\luawrapper" /I "..\include\CD" /I "..\ZElementClient" /I "..\ZCommon" /I "..\ZElementClient\Network" /I "..\ZElementClient\Network\rpcdata" /I "..\ZElementClient\Network\iolib" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "UNICODE" /D "BACK_VERSION" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Lib\AnimationCore.lib"
# ADD LIB32 /nologo /out:"Lib\AnimationCore.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Updating AnimationCore SDK...
PostBuild_Cmds=xcopy *.h ..\include\AnimationCore\ /R /Y /Q	xcopy Lib\*.lib ..\lib\AnimationCore\ /R /Y /Q
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "AnimationCore - Win32 Release"
# Name "AnimationCore - Win32 Debug"
# Name "AnimationCore - Win32 DbgRelease"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Animator.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmAction.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmActionChannel.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmActionGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmActionManager.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmAxes.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmBezier.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmBezierManager.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmBezierMove.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmBezierPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmBezierSegment.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmCamera.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmCameraBind.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmCameraFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmCameraOscillatory.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmCommonActionManager.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmDialogue.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmDisplayHide.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmFOV.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmGFX.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmHideWorldAction.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmHook.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmJump.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmMotion.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmMove.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmNPC.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmObject.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmObjectManager.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmRayTrace.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmRotate.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmScale.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmSound.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmSpeedControl.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmTranslator.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmTransparent.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmWorld.cpp
# End Source File
# Begin Source File

SOURCE=.\SmartCamera.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Animator.h
# End Source File
# Begin Source File

SOURCE=.\AnmAction.h
# End Source File
# Begin Source File

SOURCE=.\AnmActionChannel.h
# End Source File
# Begin Source File

SOURCE=.\AnmActionGroup.h
# End Source File
# Begin Source File

SOURCE=.\AnmActionManager.h
# End Source File
# Begin Source File

SOURCE=.\AnmAxes.h
# End Source File
# Begin Source File

SOURCE=.\AnmBezier.h
# End Source File
# Begin Source File

SOURCE=.\AnmBezierManager.h
# End Source File
# Begin Source File

SOURCE=.\AnmBezierMove.h
# End Source File
# Begin Source File

SOURCE=.\AnmBezierPoint.h
# End Source File
# Begin Source File

SOURCE=.\AnmBezierSegment.h
# End Source File
# Begin Source File

SOURCE=.\AnmCamera.h
# End Source File
# Begin Source File

SOURCE=.\AnmCameraBind.h
# End Source File
# Begin Source File

SOURCE=.\AnmCameraFilter.h
# End Source File
# Begin Source File

SOURCE=.\AnmCameraOscillatory.h
# End Source File
# Begin Source File

SOURCE=.\AnmCommonActionManager.h
# End Source File
# Begin Source File

SOURCE=.\AnmDialogue.h
# End Source File
# Begin Source File

SOURCE=.\AnmDisplayHide.h
# End Source File
# Begin Source File

SOURCE=.\AnmFOV.h
# End Source File
# Begin Source File

SOURCE=.\AnmGFX.h
# End Source File
# Begin Source File

SOURCE=.\AnmHideWorldAction.h
# End Source File
# Begin Source File

SOURCE=.\AnmHook.h
# End Source File
# Begin Source File

SOURCE=.\AnmInstance.h
# End Source File
# Begin Source File

SOURCE=.\AnmJump.h
# End Source File
# Begin Source File

SOURCE=.\AnmMotion.h
# End Source File
# Begin Source File

SOURCE=.\AnmMove.h
# End Source File
# Begin Source File

SOURCE=.\AnmNPC.h
# End Source File
# Begin Source File

SOURCE=.\AnmObject.h
# End Source File
# Begin Source File

SOURCE=.\AnmObjectManager.h
# End Source File
# Begin Source File

SOURCE=.\AnmPlayer.h
# End Source File
# Begin Source File

SOURCE=.\AnmRayTrace.h
# End Source File
# Begin Source File

SOURCE=.\AnmRotate.h
# End Source File
# Begin Source File

SOURCE=.\AnmScale.h
# End Source File
# Begin Source File

SOURCE=.\AnmSound.h
# End Source File
# Begin Source File

SOURCE=.\AnmSpeedControl.h
# End Source File
# Begin Source File

SOURCE=.\AnmTranslator.h
# End Source File
# Begin Source File

SOURCE=.\AnmTransparent.h
# End Source File
# Begin Source File

SOURCE=.\AnmWorld.h
# End Source File
# Begin Source File

SOURCE=.\SmartCamera.h
# End Source File
# End Group
# End Target
# End Project
