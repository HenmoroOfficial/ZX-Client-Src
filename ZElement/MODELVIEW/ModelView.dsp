# Microsoft Developer Studio Project File - Name="ModelView" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ModelView - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ModelView.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ModelView.mak" CFG="ModelView - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ModelView - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ModelView - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 1
# PROP Scc_ProjName ""$/QElement/ModelView", FPNAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ModelView - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "..\include\gfx" /I "..\ZCommon" /I ".\\" /I "..\include\luawrapper" /I "..\ZCommon\gfx" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_MODEDITOREX" /D "_ELEMENT_EDITOR_" /D "GFX_EDITOR" /D "_SKILLGFXCOMPOSER" /FD /c
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
# ADD LINK32 CHBasedCD.lib Imm32.lib mpg123lib.lib Immwrapper.lib AngelicaCommon.lib AngelicaMedia.lib AngelicaFile.lib Angelica3D.lib zliblib.lib dxguid.lib d3d8.lib d3dx8.lib dsound.lib dinput8.lib shlwapi.lib dxerr8.lib SpeedTreeRT.lib ElementSkill.lib lua5.1.mt.lib libjpeg.lib vorbisfile_static.lib vorbis_static.lib ogg_static.lib asynbase_u.lib gfxeditor_u.lib LuaWrapper_u.lib stlport_vc6_static.lib /nologo /subsystem:windows /machine:I386 /out:"../ZBin/ModelView.exe" /libpath:"..\lib\CD" /libpath:"..\lib" /libpath:"..\lib\lua" /libpath:"..\Lib\stlport"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "ModelView - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\Include\gfx" /I "..\ZCommon" /I ".\\" /I "..\include\luawrapper" /I "..\ZCommon\gfx" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_MODEDITOREX" /D "_ELEMENT_EDITOR_" /D "GFX_EDITOR" /D "_SKILLGFXCOMPOSER" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 CHBasedCD_d.lib Imm32.lib Mpg123Lib_d.lib Immwrapper_d.lib AngelicaCommon_d.lib AngelicaMedia_d.lib AngelicaFile_d.lib Angelica3D_d.lib ZLibLib_d.lib dxguid.lib d3d8.lib d3dx8d.lib dsound.lib dinput8.lib shlwapi.lib dxerr8.lib SpeedTreeRT_d.lib ElementSkill_d.lib lua5.1.mtd.lib libjpeg.lib vorbisfile_static_d.lib vorbis_static_d.lib ogg_static_d.lib asynbase_ud.lib gfxeditor_ud.lib LuaWrapper_du.lib stlport_vc6_stldebug_static.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../ZBin/ModelView_d.exe" /pdbtype:sept /libpath:"..\lib\CD" /libpath:"..\lib" /libpath:"..\lib\lua" /libpath:"..\Lib\stlport"

!ENDIF 

# Begin Target

# Name "ModelView - Win32 Release"
# Name "ModelView - Win32 Debug"
# Begin Group "BaseFiles"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Render"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\base64.cpp
# End Source File
# Begin Source File

SOURCE=.\base64.h
# End Source File
# Begin Source File

SOURCE=.\Box3D.cpp
# End Source File
# Begin Source File

SOURCE=.\Box3D.h
# End Source File
# Begin Source File

SOURCE=.\Player.cpp
# End Source File
# Begin Source File

SOURCE=.\Player.h
# End Source File
# Begin Source File

SOURCE=.\Render.cpp
# End Source File
# Begin Source File

SOURCE=.\Render.h
# End Source File
# Begin Source File

SOURCE=.\RenderableObject.cpp
# End Source File
# Begin Source File

SOURCE=.\RenderableObject.h
# End Source File
# End Group
# Begin Group "Bar"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AnimationBar.cpp
# End Source File
# Begin Source File

SOURCE=.\AnimationBar.h
# End Source File
# Begin Source File

SOURCE=.\CharactorBar.cpp
# End Source File
# Begin Source File

SOURCE=.\CharactorBar.h
# End Source File
# Begin Source File

SOURCE=.\InitDialogBar.cpp
# End Source File
# Begin Source File

SOURCE=.\InitDialogBar.h
# End Source File
# Begin Source File

SOURCE=.\ListBar.cpp
# End Source File
# Begin Source File

SOURCE=.\ListBar.h
# End Source File
# Begin Source File

SOURCE=.\ListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ListDlg.h
# End Source File
# Begin Source File

SOURCE=.\MltiTree.cpp
# End Source File
# Begin Source File

SOURCE=.\MltiTree.h
# End Source File
# Begin Source File

SOURCE=.\PartDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PartDlg.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ChildView.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildView.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\EC_StringTab.cpp
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\EC_StringTab.h
# End Source File
# Begin Source File

SOURCE=.\Global.cpp
# End Source File
# Begin Source File

SOURCE=.\Global.h
# End Source File
# Begin Source File

SOURCE=.\LuaBind.cpp
# End Source File
# Begin Source File

SOURCE=.\LuaBind.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\ModelView.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelView.h
# End Source File
# Begin Source File

SOURCE=.\ModelView.rc

!IF  "$(CFG)" == "ModelView - Win32 Release"

!ELSEIF  "$(CFG)" == "ModelView - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PathViewDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PathViewDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Utility.cpp
# End Source File
# Begin Source File

SOURCE=.\Utility.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\ModelView.ico
# End Source File
# Begin Source File

SOURCE=.\res\ModelView.rc2
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ZElementData\DataPathMan.cpp
# End Source File
# Begin Source File

SOURCE=..\ZElementData\DataPathMan.h
# End Source File
# Begin Source File

SOURCE=..\ZElementData\EC_MD5Hash.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\elementdataman.cpp
# End Source File
# Begin Source File

SOURCE=.\MODELVIEWDataMan.cpp
# End Source File
# Begin Source File

SOURCE=.\MODELVIEWDataMan.h
# End Source File
# Begin Source File

SOURCE=.\MODELVIEWSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\MODELVIEWSkill.h
# End Source File
# Begin Source File

SOURCE=..\ZElementSkill\skill.cpp
# End Source File
# End Group
# Begin Group "GfxCommon"

# PROP Default_Filter ""
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
