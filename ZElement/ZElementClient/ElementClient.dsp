# Microsoft Developer Studio Project File - Name="ElementClient" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ElementClient - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ElementClient.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ElementClient.mak" CFG="ElementClient - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ElementClient - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ElementClient - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "ElementClient - Win32 DbgRelease" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Element/ElementClient", AQDAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ElementClient - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I ".\Home" /I "..\ZCommon\Autoca" /I "..\ZCommon\Face" /I ".\\" /I ".\Network\rpcdata" /I ".\Network\inl" /I ".\Network\IOLib" /I ".\Network" /I ".\Task" /I "..\include\CD" /I "..\Include" /I "..\Include\stlport" /I "..\ZCommon" /I "..\..\Angelica2\Output\A3DSDK\Include" /I "..\..\Angelica2\Output\AFSDK\Include" /I "..\..\Angelica2\Output\ACSDK\Include" /I "..\..\Angelica2\Output\AMSDK\Include" /I "..\..\Angelica2\Output\3rdSDK\Include" /I "F:\DirectX 8.1 SDK\Include" /I "..\include\LuaWrapper" /I "../AnimationCore" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "UNICODE" /D "_ELEMENTCLIENT" /D "BACK_VERSION" /YX /FD /Zm200 /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 GfxCommon.lib CHBasedCD.lib ElementSkill.lib UnicoWS.lib kernel32.lib advapi32.lib user32.lib gdi32.lib shell32.lib comdlg32.lib version.lib mpr.lib rasapi32.lib winmm.lib winspool.lib vfw32.lib secur32.lib oleacc.lib oledlg.lib sensapi.lib AUInterface.lib mpg123lib.lib Immwrapper.lib AngelicaCommon_u.lib AngelicaMedia_u.lib AngelicaFile_u.lib Angelica3D_u.lib ws2_32.lib Imm32.lib zliblib.lib dxguid.lib d3d8.lib d3dx8.lib ddraw.lib dsound.lib dinput8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib speedtreert.lib FTDriver.lib FWEditorLib.lib shlwapi.lib wininet.lib lua5.1.mt.lib libjpeg.lib strmbase_u.lib LuaWrapper_u.lib vorbisfile_static.lib vorbis_static.lib ogg_static.lib asynbase_u.lib AutoMoveImp.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /nodefaultlib:"kernel32.lib" /nodefaultlib:"advapi32.lib" /nodefaultlib:"user32.lib" /nodefaultlib:"gdi32.lib" /nodefaultlib:"shell32.lib" /nodefaultlib:"comdlg32.lib" /nodefaultlib:"version.lib" /nodefaultlib:"mpr.lib" /nodefaultlib:"rasapi32.lib" /nodefaultlib:"winmm.lib" /nodefaultlib:"winspool.lib" /nodefaultlib:"vfw32.lib" /nodefaultlib:"secur32.lib" /nodefaultlib:"oleacc.lib" /nodefaultlib:"oledlg.lib" /nodefaultlib:"sensapi.lib" /out:"..\ZBin\ElementClient.exe" /libpath:"..\Lib\CD" /libpath:"..\Lib" /libpath:"..\Lib\IOLib" /libpath:"..\Lib\stlport" /libpath:"..\Lib\AUI" /libpath:"..\..\Angelica2\Output\A3DSDK\Lib" /libpath:"..\..\Angelica2\Output\AFSDK\Lib" /libpath:"..\..\Angelica2\Output\ACSDK\Lib" /libpath:"..\..\Angelica2\Output\AMSDK\Lib" /libpath:"..\..\Angelica2\Output\3rdSDK\Lib" /libpath:"F:\DirectX 8.1 SDK\lib" /libpath:"..\lib\lua" /libpath:"..\lib\AnimationCore" /LARGEADDRESSAWARE
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=getcodeseg                                                                                                                                                                   ..\ZBin\ElementClient.exe                                                                                                                                                                  ..\ZBin\interfaces\ 
# End Special Build Tool

!ELSEIF  "$(CFG)" == "ElementClient - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I ".\Home" /I "..\ZCommon\Autoca" /I "..\ZCommon\Face" /I ".\\" /I ".\Network\rpcdata" /I ".\Network\inl" /I ".\Network\IOLib" /I ".\Network" /I ".\Task" /I "..\include\CD" /I "..\Include" /I "..\Include\stlport" /I "..\ZCommon" /I "..\..\Angelica2\Output\A3DSDK\Include" /I "..\..\Angelica2\Output\AFSDK\Include" /I "..\..\Angelica2\Output\ACSDK\Include" /I "..\..\Angelica2\Output\AMSDK\Include" /I "..\..\Angelica2\Output\3rdSDK\Include" /I "F:\DirectX 8.1 SDK\Include" /I "..\include\LuaWrapper" /I "../AnimationCore" /D "WIN32" /D "_DEBUG" /D "_NOMINIDUMP" /D "_WINDOWS" /D "_MBCS" /D "UNICODE" /D "_ELEMENTCLIENT" /D "BACK_VERSION" /Fr /YX /FD /GZ /Zm200 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 libjpeg.lib GfxCommon_d.lib CHBasedCD_d.lib ElementSkill_d.lib AUInterface_d.lib mpg123lib_d.lib Immwrapper_d.lib AngelicaCommon_ud.lib AngelicaMedia_ud.lib AngelicaFile_ud.lib Angelica3D_ud.lib ws2_32.lib Imm32.lib zliblib.lib dxguid.lib d3d8.lib d3dx8.lib ddraw.lib dsound.lib dinput8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib speedtreert_d.lib FTDriver_D.lib FWEditorLib_d.lib shlwapi.lib wininet.lib glu32.lib lua5.1.mtd.lib strmbase_u.lib LuaWrapper_du.lib vorbisfile_static_d.lib vorbis_static_d.lib ogg_static_d.lib asynbase_u.lib AutoMoveImp_d.lib dbghelp.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /nodefaultlib:"MSVCRT" /out:"..\ZBin\ElementClient_d.exe" /pdbtype:sept /libpath:"..\lib\CD" /libpath:"..\Lib" /libpath:"..\Lib\IOLib" /libpath:"..\Lib\stlport" /libpath:"..\Lib\AUI" /libpath:"..\..\Angelica2\Output\A3DSDK\Lib" /libpath:"..\..\Angelica2\Output\AFSDK\Lib" /libpath:"..\..\Angelica2\Output\ACSDK\Lib" /libpath:"..\..\Angelica2\Output\AMSDK\Lib" /libpath:"..\..\Angelica2\Output\3rdSDK\Lib" /libpath:"F:\DirectX 8.1 SDK\lib" /libpath:"..\lib\lua" /libpath:"..\lib\AnimationCore"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ElementClient - Win32 DbgRelease"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ElementClient___Win32_DbgRelease"
# PROP BASE Intermediate_Dir "ElementClient___Win32_DbgRelease"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "DbgRelease"
# PROP Intermediate_Dir "DbgRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GR /GX /O2 /I "..\Common\Autoca" /I "..\Common\Face" /I ".\\" /I ".\Network\rpcdata" /I ".\Network\inl" /I ".\Network\IOLib" /I ".\Network" /I ".\Task" /I "..\include\CD" /I "..\Include" /I "..\Include\stlport" /I "..\Common" /I "..\Common\Gfx" /I "..\..\Angelica2\Output\A3DSDK\Include" /I "..\..\Angelica2\Output\AFSDK\Include" /I "..\..\Angelica2\Output\ACSDK\Include" /I "..\..\Angelica2\Output\AMSDK\Include" /I "..\..\Angelica2\Output\3rdSDK\Include" /I "F:\DirectX 8.1 SDK\Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "UNICODE" /D "_ELEMENTCLIENT" /D "BACK_VERSION" /YX /FD /Zm200 /c
# ADD CPP /nologo /MD /W3 /GR /GX /Zi /O2 /I ".\Home" /I "..\ZCommon\Autoca" /I "..\ZCommon\Face" /I ".\\" /I ".\Network\rpcdata" /I ".\Network\inl" /I ".\Network\IOLib" /I ".\Network" /I ".\Task" /I "..\include\CD" /I "..\Include" /I "..\Include\stlport" /I "..\ZCommon" /I "..\..\..\Output\A3DSDK\Include" /I "..\..\..\Output\AFSDK\Include" /I "..\..\..\Output\ACSDK\Include" /I "..\..\..\Output\AMSDK\Include" /I "..\..\..\Output\3rdSDK\Include" /I "..\include\LuaWrapper" /I "../AnimationCore" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "UNICODE" /D "_ELEMENTCLIENT" /D "BACK_VERSION" /YX /FD /Zm200 /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 zlibwapi.lib GfxCommon.lib CHBasedCD.lib ElementSkill.lib UnicoWS.lib kernel32.lib advapi32.lib user32.lib gdi32.lib shell32.lib comdlg32.lib version.lib mpr.lib rasapi32.lib winmm.lib winspool.lib vfw32.lib secur32.lib oleacc.lib oledlg.lib sensapi.lib AUInterface.lib mpg123lib.lib Immwrapper.lib AngelicaCommon_u.lib AngelicaMedia_u.lib AngelicaFile_u.lib Angelica3D_u.lib ws2_32.lib Imm32.lib zliblib.lib dxguid.lib d3d8.lib d3dx8.lib dsound.lib dinput8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib speedtreert.lib FTDriver.lib shlwapi.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /nodefaultlib:"kernel32.lib" /nodefaultlib:"advapi32.lib" /nodefaultlib:"user32.lib" /nodefaultlib:"gdi32.lib" /nodefaultlib:"shell32.lib" /nodefaultlib:"comdlg32.lib" /nodefaultlib:"version.lib" /nodefaultlib:"mpr.lib" /nodefaultlib:"rasapi32.lib" /nodefaultlib:"winmm.lib" /nodefaultlib:"winspool.lib" /nodefaultlib:"vfw32.lib" /nodefaultlib:"secur32.lib" /nodefaultlib:"oleacc.lib" /nodefaultlib:"oledlg.lib" /nodefaultlib:"sensapi.lib" /out:"..\Bin\ElementClient.exe" /libpath:"..\Lib\CD" /libpath:"..\Lib" /libpath:"..\Lib\IOLib" /libpath:"..\Lib\stlport" /libpath:"..\Lib\AUI" /libpath:"..\..\Angelica2\Output\A3DSDK\Lib" /libpath:"..\..\Angelica2\Output\AFSDK\Lib" /libpath:"..\..\Angelica2\Output\ACSDK\Lib" /libpath:"..\..\Angelica2\Output\AMSDK\Lib" /libpath:"..\..\Angelica2\Output\3rdSDK\Lib" /libpath:"F:\DirectX 8.1 SDK\lib"
# SUBTRACT BASE LINK32 /debug
# ADD LINK32 GfxCommon_su.lib CHBasedCD_su.lib ElementSkill.lib UnicoWS.lib kernel32.lib advapi32.lib user32.lib gdi32.lib shell32.lib comdlg32.lib version.lib mpr.lib rasapi32.lib winmm.lib winspool.lib vfw32.lib secur32.lib oleacc.lib oledlg.lib sensapi.lib AUInterface_su.lib mpg123lib.lib Immwrapper.lib AngelicaCommon_dr.lib AngelicaMedia_dr.lib AngelicaFile_dr.lib Angelica3D_dr.lib ws2_32.lib Imm32.lib zliblib.lib dxguid.lib d3d8.lib d3dx8.lib ddraw.lib dsound.lib dinput8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib speedtreert.lib FTDriver.lib FWEditorLib.lib shlwapi.lib wininet.lib c_locale.obj c_locale_stub.obj codecvt.obj collate.obj complex.obj complex_exp.obj complex_io.obj lua5.1.mt.lib libjpeg.lib strmbase_u.lib LuaWrapper_u.lib vorbisfile_static.lib vorbis_static.lib ogg_static.lib asynbase_u.lib AutoMoveImp.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /nodefaultlib:"kernel32.lib" /nodefaultlib:"advapi32.lib" /nodefaultlib:"user32.lib" /nodefaultlib:"gdi32.lib" /nodefaultlib:"shell32.lib" /nodefaultlib:"comdlg32.lib" /nodefaultlib:"version.lib" /nodefaultlib:"mpr.lib" /nodefaultlib:"rasapi32.lib" /nodefaultlib:"winmm.lib" /nodefaultlib:"winspool.lib" /nodefaultlib:"vfw32.lib" /nodefaultlib:"secur32.lib" /nodefaultlib:"oleacc.lib" /nodefaultlib:"oledlg.lib" /nodefaultlib:"sensapi.lib" /nodefaultlib:"libc.lib" /out:"..\ZBin\ElementClient.exe" /libpath:".\DbgRelease" /libpath:"..\Lib\CD" /libpath:"..\Lib" /libpath:"..\Lib\IOLib" /libpath:"..\Lib\stlport" /libpath:"..\Lib\stlport\obj" /libpath:"..\Lib\AUI" /libpath:"..\..\..\work\SDK\DbgRelease\Angelica3D\DBGRelease" /libpath:"..\..\..\work\SDK\DbgRelease\AngelicaFile\DBGRelease" /libpath:"..\..\..\work\SDK\DbgRelease\AngelicaCommon\DBGRelease" /libpath:"..\..\..\work\SDK\DbgRelease\AngelicaMedia\DBGRelease" /libpath:"..\..\..\work\SDK\DbgRelease\GfxCommon" /libpath:"..\lib\lua" /libpath:"D:\ZElement\lib\obj" /OPT:REF /LARGEADDRESSAWARE
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=getcodeseg                                                                                                                               ..\ZBin\ElementClient.exe                                                                                                                              ..\ZBin\interfaces\	DebugReleaseCopy
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "ElementClient - Win32 Release"
# Name "ElementClient - Win32 Debug"
# Name "ElementClient - Win32 DbgRelease"
# Begin Group "Main Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Input Control"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\EC_GhostInputFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_GhostInputFilter.h
# End Source File
# Begin Source File

SOURCE=.\EC_HostInputFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_HostInputFilter.h
# End Source File
# Begin Source File

SOURCE=.\EC_InputCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_InputCtrl.h
# End Source File
# Begin Source File

SOURCE=.\EC_InputFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_InputFilter.h
# End Source File
# End Group
# Begin Group "Utilities"

# PROP Default_Filter ""
# Begin Group "Gt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\gt_overlay.cpp
# End Source File
# Begin Source File

SOURCE=.\gt_overlay.h
# End Source File
# Begin Source File

SOURCE=.\gt_overlay_implement.cpp
# End Source File
# Begin Source File

SOURCE=.\hook_game.h
# End Source File
# Begin Source File

SOURCE=.\overlay.h
# End Source File
# Begin Source File

SOURCE=.\overlay_export.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\AMiniDump.cpp
# End Source File
# Begin Source File

SOURCE=.\AMiniDump.h
# End Source File
# Begin Source File

SOURCE=.\base64.cpp
# End Source File
# Begin Source File

SOURCE=.\base64.h
# End Source File
# Begin Source File

SOURCE=.\BaseFunc.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseFunc.h
# End Source File
# Begin Source File

SOURCE=.\ClosedArea.cpp
# End Source File
# Begin Source File

SOURCE=.\ClosedArea.h
# End Source File
# Begin Source File

SOURCE=.\EC_Achievement.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Achievement.h
# End Source File
# Begin Source File

SOURCE=.\EC_AssureMove.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_AssureMove.h
# End Source File
# Begin Source File

SOURCE=.\EC_BossKey.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_BossKey.h
# End Source File
# Begin Source File

SOURCE=.\EC_CameraCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_CameraCtrl.h
# End Source File
# Begin Source File

SOURCE=.\EC_CameraPath.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_CameraPath.h
# End Source File
# Begin Source File

SOURCE=.\EC_CDR.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_CDR.h
# End Source File
# Begin Source File

SOURCE=.\EC_CDS.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_CDS.h
# End Source File
# Begin Source File

SOURCE=.\EC_Clipboard.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Clipboard.h
# End Source File
# Begin Source File

SOURCE=.\EC_ClockIcon.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_ClockIcon.h
# End Source File
# Begin Source File

SOURCE=.\EC_Configs.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Configs.h
# End Source File
# Begin Source File

SOURCE=.\EC_ConfigsData.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_ConfigsData.h
# End Source File
# Begin Source File

SOURCE=.\EC_Counter.h
# End Source File
# Begin Source File

SOURCE=.\EC_DataExpectation.h
# End Source File
# Begin Source File

SOURCE=.\EC_FullGlowRender.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_FullGlowRender.h
# End Source File
# Begin Source File

SOURCE=.\EC_GFXCaster.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_GFXCaster.h
# End Source File
# Begin Source File

SOURCE=.\EC_ImageRes.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_ImageRes.h
# End Source File
# Begin Source File

SOURCE=.\EC_LoadProgress.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_LoadProgress.h
# End Source File
# Begin Source File

SOURCE=.\EC_PateText.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_PateText.h
# End Source File
# Begin Source File

SOURCE=.\EC_PortraitRender.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_PortraitRender.h
# End Source File
# Begin Source File

SOURCE=.\EC_Resource.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Resource.h
# End Source File
# Begin Source File

SOURCE=.\EC_RTDebug.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_RTDebug.h
# End Source File
# Begin Source File

SOURCE=.\EC_SceneCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_SceneCheck.h
# End Source File
# Begin Source File

SOURCE=.\EC_ScreenRender.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_ScreenRender.h
# End Source File
# Begin Source File

SOURCE=.\EC_ShadowRender.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_ShadowRender.h
# End Source File
# Begin Source File

SOURCE=.\EC_SimpleBloomRender.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_SimpleBloomRender.h
# End Source File
# Begin Source File

SOURCE=.\EC_SoundCache.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_SoundCache.h
# End Source File
# Begin Source File

SOURCE=.\EC_StringTab.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_StringTab.h
# End Source File
# Begin Source File

SOURCE=.\EC_Utility.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Utility.h
# End Source File
# Begin Source File

SOURCE=.\ExplorerMan.cpp
# End Source File
# Begin Source File

SOURCE=.\ExplorerMan.h
# End Source File
# Begin Source File

SOURCE=.\MsgTransFun.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgTransFun.h
# End Source File
# Begin Source File

SOURCE=.\PWAtl.h
# End Source File
# Begin Source File

SOURCE=.\PWBrowser.cpp
# End Source File
# Begin Source File

SOURCE=.\PWBrowser.h
# End Source File
# Begin Source File

SOURCE=.\rdbmp.c
# End Source File
# Begin Source File

SOURCE=.\SafeWinImpl.h
# End Source File
# End Group
# Begin Group "World"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\EC_Instance.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Instance.h
# End Source File
# Begin Source File

SOURCE=.\EC_Scene.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Scene.h
# End Source File
# Begin Source File

SOURCE=.\EC_SceneBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_SceneBlock.h
# End Source File
# Begin Source File

SOURCE=.\EC_SceneLights.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_SceneLights.h
# End Source File
# Begin Source File

SOURCE=.\EC_SceneLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_SceneLoader.h
# End Source File
# Begin Source File

SOURCE=.\EC_World.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_World.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EC_WorldFile.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\AnimationCore\AnmObjectManager.cpp
# End Source File
# Begin Source File

SOURCE=.\DataPathMan.cpp
# End Source File
# Begin Source File

SOURCE=.\DataPathMan.h
# End Source File
# Begin Source File

SOURCE=.\EC_AutoMove.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_AutoMove.h
# End Source File
# Begin Source File

SOURCE=.\EC_Cutscene.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Cutscene.h
# End Source File
# Begin Source File

SOURCE=.\EC_FixedMsg.h
# End Source File
# Begin Source File

SOURCE=.\EC_Game.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Game.h
# End Source File
# Begin Source File

SOURCE=.\EC_GameRecord.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_GameRecord.h
# End Source File
# Begin Source File

SOURCE=.\EC_GameRun.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_GameRun.h
# End Source File
# Begin Source File

SOURCE=.\EC_Global.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Global.h
# End Source File
# Begin Source File

SOURCE=.\EC_MsgDef.h
# End Source File
# Begin Source File

SOURCE=.\EC_Viewport.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Viewport.h
# End Source File
# Begin Source File

SOURCE=.\ElementClient.cpp
# End Source File
# Begin Source File

SOURCE=.\ElementClient.rc
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# End Group
# Begin Group "Object Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Players"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\EC_BattleDungeonTower.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_BattleDungeonTower.h
# End Source File
# Begin Source File

SOURCE=.\EC_ChangePill.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_ChangePill.h
# End Source File
# Begin Source File

SOURCE=.\EC_Circle.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Circle.h
# End Source File
# Begin Source File

SOURCE=.\EC_CollisionTest.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_CollisionTest.h
# End Source File
# Begin Source File

SOURCE=.\EC_ComboSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_ComboSkill.h
# End Source File
# Begin Source File

SOURCE=.\EC_ElsePlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_ElsePlayer.h
# End Source File
# Begin Source File

SOURCE=.\EC_Faction.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Faction.h
# End Source File
# Begin Source File

SOURCE=.\EC_Friend.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Friend.h
# End Source File
# Begin Source File

SOURCE=.\EC_HostCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_HostMove.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_HostMove.h
# End Source File
# Begin Source File

SOURCE=.\EC_HostMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_HostPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_HostPlayer.h
# End Source File
# Begin Source File

SOURCE=.\EC_HPWork.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_HPWork.h
# End Source File
# Begin Source File

SOURCE=.\EC_HPWorkAutoMove.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_HPWorkAutoMove.h
# End Source File
# Begin Source File

SOURCE=.\EC_HPWorkDead.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_HPWorkDead.h
# End Source File
# Begin Source File

SOURCE=.\EC_HPWorkFall.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_HPWorkFall.h
# End Source File
# Begin Source File

SOURCE=.\EC_HPWorkFly.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_HPWorkFly.h
# End Source File
# Begin Source File

SOURCE=.\EC_HPWorkFollow.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_HPWorkFollow.h
# End Source File
# Begin Source File

SOURCE=.\EC_HPWorkMelee.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_HPWorkMelee.h
# End Source File
# Begin Source File

SOURCE=.\EC_HPWorkMove.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_HPWorkMove.h
# End Source File
# Begin Source File

SOURCE=.\EC_HPWorkSit.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_HPWorkSit.h
# End Source File
# Begin Source File

SOURCE=.\EC_HPWorkSpell.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_HPWorkSpell.h
# End Source File
# Begin Source File

SOURCE=.\EC_HPWorkStand.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_HPWorkStand.h
# End Source File
# Begin Source File

SOURCE=.\EC_HPWorkTrace.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_HPWorkTrace.h
# End Source File
# Begin Source File

SOURCE=.\EC_HPWorkUse.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_HPWorkUse.h
# End Source File
# Begin Source File

SOURCE=.\EC_LoginPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_LoginPlayer.h
# End Source File
# Begin Source File

SOURCE=.\EC_OnlineAward.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_OnlineAward.h
# End Source File
# Begin Source File

SOURCE=.\EC_PetCorral.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_PetCorral.h
# End Source File
# Begin Source File

SOURCE=.\EC_Player.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Player.h
# End Source File
# Begin Source File

SOURCE=.\EC_RoleTypes.h
# End Source File
# Begin Source File

SOURCE=.\EC_Team.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Team.h
# End Source File
# Begin Source File

SOURCE=.\EC_TeamMan.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_TeamMan.h
# End Source File
# End Group
# Begin Group "NPCs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\EC_Monster.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Monster.h
# End Source File
# Begin Source File

SOURCE=.\EC_NPC.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_NPC.h
# End Source File
# Begin Source File

SOURCE=.\EC_NPCCarrier.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_NPCCarrier.h
# End Source File
# Begin Source File

SOURCE=.\EC_NPCServer.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_NPCServer.h
# End Source File
# Begin Source File

SOURCE=.\EC_Pet.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Pet.h
# End Source File
# End Group
# Begin Group "Scene Objects"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ZCommon\EC_Bezier.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EC_Bezier.h
# End Source File
# Begin Source File

SOURCE=.\EC_CritterGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_CritterGroup.h
# End Source File
# Begin Source File

SOURCE=.\EC_GrassArea.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_GrassArea.h
# End Source File
# Begin Source File

SOURCE=.\EC_Ornament.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Ornament.h
# End Source File
# Begin Source File

SOURCE=.\EC_SceneObject.h
# End Source File
# Begin Source File

SOURCE=.\EC_SceneThreadObj.h
# End Source File
# Begin Source File

SOURCE=.\EC_ScnBoxArea.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_ScnBoxArea.h
# End Source File
# End Group
# Begin Group "Inventory"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\EC_DealInventory.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_DealInventory.h
# End Source File
# Begin Source File

SOURCE=.\EC_Inventory.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Inventory.h
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrConsume.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrConsume.h
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrDestroyingEssence.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrDestroyingEssence.h
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrEquip.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrEquip.h
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrHTItem.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrHTItem.h
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrItem.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrItem.h
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrMaterial.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrMaterial.h
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrRune.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrRune.h
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrScroll.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrScroll.h
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrStone.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrStone.h
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrTalismanMain.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrTalismanMain.h
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrTaskItem.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrTaskItem.h
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrTypes.h
# End Source File
# End Group
# Begin Group "Shortcut"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\EC_Shortcut.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Shortcut.h
# End Source File
# Begin Source File

SOURCE=.\EC_ShortcutSet.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_ShortcutSet.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\EC_Decal.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Decal.h
# End Source File
# Begin Source File

SOURCE=.\EC_Matter.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Matter.h
# End Source File
# Begin Source File

SOURCE=.\EC_Object.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Object.h
# End Source File
# Begin Source File

SOURCE=.\EC_ObjectWork.h
# End Source File
# Begin Source File

SOURCE=.\EC_Skill.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Skill.h
# End Source File
# Begin Source File

SOURCE=.\EC_Sprite.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Sprite.h
# End Source File
# Begin Source File

SOURCE=.\EC_SunMoon.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_SunMoon.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\element.ico
# End Source File
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# End Group
# Begin Group "Interface Files"

# PROP Default_Filter ""
# Begin Group "Dialogs"

# PROP Default_Filter ""
# Begin Group "Battle"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DlgBattleDungeonApply.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBattleDungeonApply.h
# End Source File
# Begin Source File

SOURCE=.\DlgBattleDungeonApplyList.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBattleDungeonApplyList.h
# End Source File
# Begin Source File

SOURCE=.\DlgBattleDungeonHeyang.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBattleDungeonHeyang.h
# End Source File
# Begin Source File

SOURCE=.\DlgBattleDungeonMemberList.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBattleDungeonMemberList.h
# End Source File
# Begin Source File

SOURCE=.\DlgBattleDungeonNameInput.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBattleDungeonNameInput.h
# End Source File
# Begin Source File

SOURCE=.\DlgBattleDungeonTime.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBattleDungeonTime.h
# End Source File
# Begin Source File

SOURCE=.\DlgBattleDungeonTower.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBattleDungeonTower.h
# End Source File
# Begin Source File

SOURCE=.\DlgBattleDungeonTowerIngame.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBattleDungeonTowerIngame.h
# End Source File
# Begin Source File

SOURCE=.\DlgBattleDungeonTowerScore.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBattleDungeonTowerScore.h
# End Source File
# Begin Source File

SOURCE=.\DlgBattleFactionWarApply.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBattleFactionWarApply.h
# End Source File
# Begin Source File

SOURCE=.\DlgBattleFactionWarControl.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBattleFactionWarControl.h
# End Source File
# Begin Source File

SOURCE=.\DlgBattleFactionWarWatch.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBattleFactionWarWatch.h
# End Source File
# Begin Source File

SOURCE=.\DlgBattleTeam6v6.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBattleTeam6v6.h
# End Source File
# Begin Source File

SOURCE=.\DlgBattleTeam6v6Award.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBattleTeam6v6Award.h
# End Source File
# Begin Source File

SOURCE=.\DlgBattleTeam6v6Score.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBattleTeam6v6Score.h
# End Source File
# Begin Source File

SOURCE=.\DlgBattleTeamCustom.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBattleTeamCustom.h
# End Source File
# Begin Source File

SOURCE=.\EC_BattleDungeon.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_BattleDungeon.h
# End Source File
# End Group
# Begin Group "Equip"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DlgEquipBijou.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEquipBijou.h
# End Source File
# Begin Source File

SOURCE=.\DlgEquipBind.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEquipBind.h
# End Source File
# Begin Source File

SOURCE=.\DlgEquipDestroy.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEquipDestroy.h
# End Source File
# Begin Source File

SOURCE=.\DlgEquipJack.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEquipJack.h
# End Source File
# Begin Source File

SOURCE=.\DlgEquipLvup.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEquipLvup.h
# End Source File
# Begin Source File

SOURCE=.\DlgEquipUndestroy.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEquipUndestroy.h
# End Source File
# Begin Source File

SOURCE=.\DlgFengshenAttach.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFengshenAttach.h
# End Source File
# Begin Source File

SOURCE=.\DlgFengshenConvert.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFengshenConvert.h
# End Source File
# Begin Source File

SOURCE=.\DlgFengshenDrill.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFengshenDrill.h
# End Source File
# Begin Source File

SOURCE=.\DlgFengshenErase.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFengshenErase.h
# End Source File
# Begin Source File

SOURCE=.\DlgFengshenIdentify.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFengshenIdentify.h
# End Source File
# Begin Source File

SOURCE=.\DlgFengshenUpgrade.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFengshenUpgrade.h
# End Source File
# Begin Source File

SOURCE=.\DlgItemSvrBase.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgItemSvrBase.h
# End Source File
# Begin Source File

SOURCE=.\DlgRuneMain.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRuneMain.h
# End Source File
# Begin Source File

SOURCE=.\DlgRuneOperate.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRuneOperate.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\DlgAchievement.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAchievement.h
# End Source File
# Begin Source File

SOURCE=.\DlgAchivItem.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAchivItem.h
# End Source File
# Begin Source File

SOURCE=.\DlgAchivItemMan.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAchivItemMan.h
# End Source File
# Begin Source File

SOURCE=.\DlgAchivMain.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAchivMain.h
# End Source File
# Begin Source File

SOURCE=.\DlgAction.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAction.h
# End Source File
# Begin Source File

SOURCE=.\DlgAgentCombined.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAgentCombined.h
# End Source File
# Begin Source File

SOURCE=.\DlgAniversaryPet5.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAniversaryPet5.h
# End Source File
# Begin Source File

SOURCE=.\DlgArrangeTeam.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgArrangeTeam.h
# End Source File
# Begin Source File

SOURCE=.\DlgArrangeTeamSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgArrangeTeamSetting.h
# End Source File
# Begin Source File

SOURCE=.\DlgAskHelpToGM.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAskHelpToGM.h
# End Source File
# Begin Source File

SOURCE=.\DlgAttriUp.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAttriUp.h
# End Source File
# Begin Source File

SOURCE=.\DlgAuction.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAuction.h
# End Source File
# Begin Source File

SOURCE=.\DlgAutoLock.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAutoLock.h
# End Source File
# Begin Source File

SOURCE=.\DlgAward.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAward.h
# End Source File
# Begin Source File

SOURCE=.\DlgAwardAccountInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAwardAccountInfo.h
# End Source File
# Begin Source File

SOURCE=.\DlgAwardOnline.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAwardOnline.h
# End Source File
# Begin Source File

SOURCE=.\DlgBase.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBase.h
# End Source File
# Begin Source File

SOURCE=.\DlgBaseExplorer.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBaseExplorer.h
# End Source File
# Begin Source File

SOURCE=.\DlgBattleFlow.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBattleFlow.h
# End Source File
# Begin Source File

SOURCE=.\DlgBattleInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBattleInfo.h
# End Source File
# Begin Source File

SOURCE=.\DlgBattleInstance.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBattleInstance.h
# End Source File
# Begin Source File

SOURCE=.\DlgBattlePk.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBattlePk.h
# End Source File
# Begin Source File

SOURCE=.\DlgBattleSong.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBattleSong.h
# End Source File
# Begin Source File

SOURCE=.\DlgBattleTerr.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBattleTerr.h
# End Source File
# Begin Source File

SOURCE=.\DlgBBS.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBBS.h
# End Source File
# Begin Source File

SOURCE=.\DlgBodyChange.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBodyChange.h
# End Source File
# Begin Source File

SOURCE=.\DlgBook.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBook.h
# End Source File
# Begin Source File

SOURCE=.\DlgBooth.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBooth.h
# End Source File
# Begin Source File

SOURCE=.\DlgBroadCast.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBroadCast.h
# End Source File
# Begin Source File

SOURCE=.\DlgBroadCastInGame.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBroadCastInGame.h
# End Source File
# Begin Source File

SOURCE=.\DlgBuddyState.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBuddyState.h
# End Source File
# Begin Source File

SOURCE=.\DlgBuff.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBuff.h
# End Source File
# Begin Source File

SOURCE=.\DlgCalendar.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCalendar.h
# End Source File
# Begin Source File

SOURCE=.\DlgCalendarPresent.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCalendarPresent.h
# End Source File
# Begin Source File

SOURCE=.\DlgCamera.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCamera.h
# End Source File
# Begin Source File

SOURCE=.\DlgCameraPath.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCameraPath.h
# End Source File
# Begin Source File

SOURCE=.\DlgChannelChat.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgChannelChat.h
# End Source File
# Begin Source File

SOURCE=.\DlgChannelCreate.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgChannelCreate.h
# End Source File
# Begin Source File

SOURCE=.\DlgChannelJoin.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgChannelJoin.h
# End Source File
# Begin Source File

SOURCE=.\DlgChannelOption.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgChannelOption.h
# End Source File
# Begin Source File

SOURCE=.\DlgChannelPW.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgChannelPW.h
# End Source File
# Begin Source File

SOURCE=.\DlgCharacter.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCharacter.h
# End Source File
# Begin Source File

SOURCE=.\DlgCharacterDungeonAward.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCharacterDungeonAward.h
# End Source File
# Begin Source File

SOURCE=.\DlgCharacterDungeonBook.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCharacterDungeonBook.h
# End Source File
# Begin Source File

SOURCE=.\DlgChat.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgChat.h
# End Source File
# Begin Source File

SOURCE=.\DlgCityBuilding.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCityBuilding.h
# End Source File
# Begin Source File

SOURCE=.\DlgClassmateManage.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgClassmateManage.h
# End Source File
# Begin Source File

SOURCE=.\DlgCommonDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCommonDlg.h
# End Source File
# Begin Source File

SOURCE=.\DlgConsole.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgConsole.h
# End Source File
# Begin Source File

SOURCE=.\DlgContest.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgContest.h
# End Source File
# Begin Source File

SOURCE=.\DlgDailyPromp.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDailyPromp.h
# End Source File
# Begin Source File

SOURCE=.\DlgDailyPrompLoginHint.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDailyPrompLoginHint.h
# End Source File
# Begin Source File

SOURCE=.\DlgDelMsgReason.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDigTreas.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDigTreas.h
# End Source File
# Begin Source File

SOURCE=.\DlgDoubleExp.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDoubleExp.h
# End Source File
# Begin Source File

SOURCE=.\DlgDoubleExpS.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDoubleExpS.h
# End Source File
# Begin Source File

SOURCE=.\DlgDragDrop.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDragDrop.h
# End Source File
# Begin Source File

SOURCE=.\DlgDungeonAncient.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDungeonAncient.h
# End Source File
# Begin Source File

SOURCE=.\DlgDynamicSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDynamicSystem.h
# End Source File
# Begin Source File

SOURCE=.\DlgEnemyOptionName.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEnemyOptionName.h
# End Source File
# Begin Source File

SOURCE=.\DlgEPEquip.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEPEquip.h
# End Source File
# Begin Source File

SOURCE=.\DlgExit.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgExit.h
# End Source File
# Begin Source File

SOURCE=.\DlgFaceLift.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFaceLift.h
# End Source File
# Begin Source File

SOURCE=.\DlgFaceOff.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFaceOff.h
# End Source File
# Begin Source File

SOURCE=.\DlgFastPay.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFastPay.h
# End Source File
# Begin Source File

SOURCE=.\DlgFastPayCode.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFastPayCode.h
# End Source File
# Begin Source File

SOURCE=.\DlgFengshenSoul.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFengshenSoul.h
# End Source File
# Begin Source File

SOURCE=.\DlgFlower.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFlower.h
# End Source File
# Begin Source File

SOURCE=.\DlgFlyProgress.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFlyProgress.h
# End Source File
# Begin Source File

SOURCE=.\DlgFrdCallBck.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFrdCallBck.h
# End Source File
# Begin Source File

SOURCE=.\DlgFriendChat.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFriendChat.h
# End Source File
# Begin Source File

SOURCE=.\DlgFriendColor.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFriendColor.h
# End Source File
# Begin Source File

SOURCE=.\DlgFriendGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFriendGroup.h
# End Source File
# Begin Source File

SOURCE=.\DlgFriendHistory.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFriendHistory.h
# End Source File
# Begin Source File

SOURCE=.\DlgFriendList.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFriendList.h
# End Source File
# Begin Source File

SOURCE=.\DlgFriendOptionGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFriendOptionGroup.h
# End Source File
# Begin Source File

SOURCE=.\DlgFriendOptionName.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFriendOptionName.h
# End Source File
# Begin Source File

SOURCE=.\DlgFriendOptionNormal.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFriendOptionNormal.h
# End Source File
# Begin Source File

SOURCE=.\DlgFriendRequest.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFriendRequest.h
# End Source File
# Begin Source File

SOURCE=.\DlgFriendSeeking.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFriendSeeking.h
# End Source File
# Begin Source File

SOURCE=.\DlgGameCasual.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGameCasual.h
# End Source File
# Begin Source File

SOURCE=.\DlgGameHelp.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGameHelp.h
# End Source File
# Begin Source File

SOURCE=.\DlgGamePlaza.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGamePlaza.h
# End Source File
# Begin Source File

SOURCE=.\DlgGamePoker.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGamePoker.h
# End Source File
# Begin Source File

SOURCE=.\DlgGameSnake.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGameSnake.h
# End Source File
# Begin Source File

SOURCE=.\DlgGardenInventory.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGardenInventory.h
# End Source File
# Begin Source File

SOURCE=.\DlgGardenMan.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGardenMan.h
# End Source File
# Begin Source File

SOURCE=.\DlgGardenMoney.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGardenMoney.h
# End Source File
# Begin Source File

SOURCE=.\DlgGardenQShop.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGardenQShop.h
# End Source File
# Begin Source File

SOURCE=.\DlgGMCallMultiPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGMCallMultiPlayer.h
# End Source File
# Begin Source File

SOURCE=.\DlgGMConsole.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGMConsole.h
# End Source File
# Begin Source File

SOURCE=.\DlgGMDelMsgReason.h
# End Source File
# Begin Source File

SOURCE=.\DlgGMFinishMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGMFinishMsg.h
# End Source File
# Begin Source File

SOURCE=.\DlgGMParam.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGMParam.h
# End Source File
# Begin Source File

SOURCE=.\DlgGMTransferMap.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGMTransferMap.h
# End Source File
# Begin Source File

SOURCE=.\DlgGoldAccount.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGoldAccount.h
# End Source File
# Begin Source File

SOURCE=.\DlgGoldInquire.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGoldInquire.h
# End Source File
# Begin Source File

SOURCE=.\DlgGoldPassword.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGoldPassword.h
# End Source File
# Begin Source File

SOURCE=.\DlgGoldTrade.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGoldTrade.h
# End Source File
# Begin Source File

SOURCE=.\DlgGuildCreate.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGuildCreate.h
# End Source File
# Begin Source File

SOURCE=.\DlgGuildMan.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGuildMan.h
# End Source File
# Begin Source File

SOURCE=.\DlgGuildMap.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGuildMap.h
# End Source File
# Begin Source File

SOURCE=.\DlgGuildMapStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGuildMapStatus.h
# End Source File
# Begin Source File

SOURCE=.\DlgGuildMapTravel.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGuildMapTravel.h
# End Source File
# Begin Source File

SOURCE=.\DlgHelp.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgHelp.h
# End Source File
# Begin Source File

SOURCE=.\DlgHelpFariy.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgHelpFariy.h
# End Source File
# Begin Source File

SOURCE=.\DlgHelpLvUp.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgHelpLvUp.h
# End Source File
# Begin Source File

SOURCE=.\DlgHelpMemo.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgHelpMemo.h
# End Source File
# Begin Source File

SOURCE=.\DlgHorseTame.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgHorseTame.h
# End Source File
# Begin Source File

SOURCE=.\DlgHost.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgHost.h
# End Source File
# Begin Source File

SOURCE=.\DlgHostAction.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgHostAction.h
# End Source File
# Begin Source File

SOURCE=.\DlgHostPet.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgHostPet.h
# End Source File
# Begin Source File

SOURCE=.\DlgIcons.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgIcons.h
# End Source File
# Begin Source File

SOURCE=.\DlgIdentify.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgIdentify.h
# End Source File
# Begin Source File

SOURCE=.\DlgInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgInfo.h
# End Source File
# Begin Source File

SOURCE=.\DlgInputName.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgInputName.h
# End Source File
# Begin Source File

SOURCE=.\DlgInputNO.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgInputNO.h
# End Source File
# Begin Source File

SOURCE=.\DlgInstall.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgInstall.h
# End Source File
# Begin Source File

SOURCE=.\DlgInventory.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgInventory.h
# End Source File
# Begin Source File

SOURCE=.\DlgInventoryCostume.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgInventoryCostume.h
# End Source File
# Begin Source File

SOURCE=.\DlgInventoryExpand.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgInventoryExpand.h
# End Source File
# Begin Source File

SOURCE=.\DlgInventoryRide.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgInventoryRide.h
# End Source File
# Begin Source File

SOURCE=.\DlgItemLock.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgItemLock.h
# End Source File
# Begin Source File

SOURCE=.\DlgKf2012Guess.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgKf2012Guess.h
# End Source File
# Begin Source File

SOURCE=.\DlgKfSelectLine.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgKfSelectLine.h
# End Source File
# Begin Source File

SOURCE=.\DlgKingWar.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgKingWar.h
# End Source File
# Begin Source File

SOURCE=.\DlgLevelUp.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLevelUp.h
# End Source File
# Begin Source File

SOURCE=.\DlgLottery.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLottery.h
# End Source File
# Begin Source File

SOURCE=.\DlgLotteryNew.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLotteryNew.h
# End Source File
# Begin Source File

SOURCE=.\DlgMailList.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMailList.h
# End Source File
# Begin Source File

SOURCE=.\DlgMailOption.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMailOption.h
# End Source File
# Begin Source File

SOURCE=.\DlgMailRead.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMailRead.h
# End Source File
# Begin Source File

SOURCE=.\DlgMailWrite.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMailWrite.h
# End Source File
# Begin Source File

SOURCE=.\DlgMapTransfer.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMapTransfer.h
# End Source File
# Begin Source File

SOURCE=.\DlgMarkAstro.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMarkAstro.h
# End Source File
# Begin Source File

SOURCE=.\DlgMarkAstroList.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMarkAstroList.h
# End Source File
# Begin Source File

SOURCE=.\DlgMend.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMend.h
# End Source File
# Begin Source File

SOURCE=.\DlgMenology.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMenology.h
# End Source File
# Begin Source File

SOURCE=.\DlgMessageBox.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMessageBox.h
# End Source File
# Begin Source File

SOURCE=.\DlgMidMap.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMidMap.h
# End Source File
# Begin Source File

SOURCE=.\DlgMiniMap.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMiniMap.h
# End Source File
# Begin Source File

SOURCE=.\DlgMinimizeBar.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMinimizeBar.h
# End Source File
# Begin Source File

SOURCE=.\DlgModule.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgModule.h
# End Source File
# Begin Source File

SOURCE=.\DlgNPC.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgNPC.h
# End Source File
# Begin Source File

SOURCE=.\DlgPetAction.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPetAction.h
# End Source File
# Begin Source File

SOURCE=.\DlgPetCombine.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPetCombine.h
# End Source File
# Begin Source File

SOURCE=.\DlgPetDetail.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPetDetail.h
# End Source File
# Begin Source File

SOURCE=.\DlgPetEquip.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPetEquip.h
# End Source File
# Begin Source File

SOURCE=.\DlgPetFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPetFactory.h
# End Source File
# Begin Source File

SOURCE=.\DlgPetMain.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPetMain.h
# End Source File
# Begin Source File

SOURCE=.\DlgPetOperate.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPetOperate.h
# End Source File
# Begin Source File

SOURCE=.\DlgPetPackage.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPetPackage.h
# End Source File
# Begin Source File

SOURCE=.\DlgPetService.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPetService.h
# End Source File
# Begin Source File

SOURCE=.\DlgPetSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPetSetting.h
# End Source File
# Begin Source File

SOURCE=.\DlgPKMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPKMsg.h
# End Source File
# Begin Source File

SOURCE=.\DlgPKSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPKSetting.h
# End Source File
# Begin Source File

SOURCE=.\DlgPopmsgSafeArea.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPopmsgSafeArea.h
# End Source File
# Begin Source File

SOURCE=.\DlgProclaim.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgProclaim.h
# End Source File
# Begin Source File

SOURCE=.\DlgProduce.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgProduce.h
# End Source File
# Begin Source File

SOURCE=.\DlgQProduceTool.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgQProduceTool.h
# End Source File
# Begin Source File

SOURCE=.\DlgQShop.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgQShop.h
# End Source File
# Begin Source File

SOURCE=.\DlgQShopItem.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgQShopItem.h
# End Source File
# Begin Source File

SOURCE=.\DlgQuestion.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgQuestion.h
# End Source File
# Begin Source File

SOURCE=.\DlgQuickAction.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgQuickAction.h
# End Source File
# Begin Source File

SOURCE=.\DlgQuickBar.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgQuickBar.h
# End Source File
# Begin Source File

SOURCE=.\DlgQuitReason.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgQuitReason.h
# End Source File
# Begin Source File

SOURCE=.\DlgRankList.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRankList.h
# End Source File
# Begin Source File

SOURCE=.\DlgRecord.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRecord.h
# End Source File
# Begin Source File

SOURCE=.\DlgRemedyMetempsychosis.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRemedyMetempsychosis.h
# End Source File
# Begin Source File

SOURCE=.\DlgReName.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgReName.h
# End Source File
# Begin Source File

SOURCE=.\DlgReplay.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgReplay.h
# End Source File
# Begin Source File

SOURCE=.\DlgReplayEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgReplayEdit.h
# End Source File
# Begin Source File

SOURCE=.\DlgReportToGM.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgReportToGM.h
# End Source File
# Begin Source File

SOURCE=.\DlgResetProp.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgResetProp.h
# End Source File
# Begin Source File

SOURCE=.\DlgRevive.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRevive.h
# End Source File
# Begin Source File

SOURCE=.\DlgRoleRune.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRoleRune.h
# End Source File
# Begin Source File

SOURCE=.\DlgSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSetting.h
# End Source File
# Begin Source File

SOURCE=.\DlgSettingGame.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSettingGame.h
# End Source File
# Begin Source File

SOURCE=.\DlgSettingPostEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSettingPostEffect.h
# End Source File
# Begin Source File

SOURCE=.\DlgSettingSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSettingSystem.h
# End Source File
# Begin Source File

SOURCE=.\DlgSettingVideo.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSettingVideo.h
# End Source File
# Begin Source File

SOURCE=.\DlgShop.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgShop.h
# End Source File
# Begin Source File

SOURCE=.\DlgShopBuyBack.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgShopBuyBack.h
# End Source File
# Begin Source File

SOURCE=.\DlgShopConsign.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgShopConsign.h
# End Source File
# Begin Source File

SOURCE=.\DlgShopConsignAction.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgShopConsignAction.h
# End Source File
# Begin Source File

SOURCE=.\DlgShopConsignView.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgShopConsignView.h
# End Source File
# Begin Source File

SOURCE=.\DlgShopExchange.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgShopExchange.h
# End Source File
# Begin Source File

SOURCE=.\DlgShopReputation.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgShopReputation.h
# End Source File
# Begin Source File

SOURCE=.\DlgSiege.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSiege.h
# End Source File
# Begin Source File

SOURCE=.\DlgSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSkill.h
# End Source File
# Begin Source File

SOURCE=.\DlgSkillEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSkillEdit.h
# End Source File
# Begin Source File

SOURCE=.\DlgSkillEdit1.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSkillEdit1.h
# End Source File
# Begin Source File

SOURCE=.\DlgSkillMan.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSkillMan.h
# End Source File
# Begin Source File

SOURCE=.\DlgSkillTree.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSkillTree.h
# End Source File
# Begin Source File

SOURCE=.\DlgSkillXP.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSkillXP.h
# End Source File
# Begin Source File

SOURCE=.\DlgSnsInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSnsInfo.h
# End Source File
# Begin Source File

SOURCE=.\DlgSoftKB.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSoftKB.h
# End Source File
# Begin Source File

SOURCE=.\DlgSoul.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSoul.h
# End Source File
# Begin Source File

SOURCE=.\DlgSoulCombine.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSoulCombine.h
# End Source File
# Begin Source File

SOURCE=.\DlgSpeakerSpecial.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSpeakerSpecial.h
# End Source File
# Begin Source File

SOURCE=.\DlgSplit.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSplit.h
# End Source File
# Begin Source File

SOURCE=.\DlgSpread.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSpread.h
# End Source File
# Begin Source File

SOURCE=.\DlgStorage.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgStorage.h
# End Source File
# Begin Source File

SOURCE=.\DlgStorageChangePW.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgStorageChangePW.h
# End Source File
# Begin Source File

SOURCE=.\DlgStoragePW.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgStoragePW.h
# End Source File
# Begin Source File

SOURCE=.\DlgStorageTaskCross.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgStorageTaskCross.h
# End Source File
# Begin Source File

SOURCE=.\DlgSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSystem.h
# End Source File
# Begin Source File

SOURCE=.\DlgSystem2.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSystem2.h
# End Source File
# Begin Source File

SOURCE=.\DlgSystem3.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSystem3.h
# End Source File
# Begin Source File

SOURCE=.\DlgSystem4.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSystem4.h
# End Source File
# Begin Source File

SOURCE=.\DlgTab3D.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTab3D.h
# End Source File
# Begin Source File

SOURCE=.\DlgTalisman.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTalisman.h
# End Source File
# Begin Source File

SOURCE=.\DlgTalismanExchange.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTalismanExchange.h
# End Source File
# Begin Source File

SOURCE=.\DlgTalismanService.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTalismanService.h
# End Source File
# Begin Source File

SOURCE=.\DlgTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTarget.h
# End Source File
# Begin Source File

SOURCE=.\DlgTask.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTask.h
# End Source File
# Begin Source File

SOURCE=.\DlgTaskAncient.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTaskAncient.h
# End Source File
# Begin Source File

SOURCE=.\DlgTaskBase.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTaskBase.h
# End Source File
# Begin Source File

SOURCE=.\DlgTaskDaily.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTaskDaily.h
# End Source File
# Begin Source File

SOURCE=.\DlgTaskList.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTaskList.h
# End Source File
# Begin Source File

SOURCE=.\DlgTaskZhuXianList.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTaskZhuXianList.h
# End Source File
# Begin Source File

SOURCE=.\DlgTeach.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTeach.h
# End Source File
# Begin Source File

SOURCE=.\DlgTeacherMan.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTeacherMan.h
# End Source File
# Begin Source File

SOURCE=.\DlgTeamContext.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTeamContext.h
# End Source File
# Begin Source File

SOURCE=.\DlgTeamMain.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTeamMain.h
# End Source File
# Begin Source File

SOURCE=.\DlgTeamMate.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTeamMate.h
# End Source File
# Begin Source File

SOURCE=.\DlgTerrWarMap.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTerrWarMap.h
# End Source File
# Begin Source File

SOURCE=.\DlgTextFirework.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTextFirework.h
# End Source File
# Begin Source File

SOURCE=.\DlgTopUPReward.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTopUPReward.h
# End Source File
# Begin Source File

SOURCE=.\DlgTouch.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTouch.h
# End Source File
# Begin Source File

SOURCE=.\DlgTrade.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTrade.h
# End Source File
# Begin Source File

SOURCE=.\DlgTreasureRadar.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTreasureRadar.h
# End Source File
# Begin Source File

SOURCE=.\DlgUpgrade.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgUpgrade.h
# End Source File
# Begin Source File

SOURCE=.\DlgVipAward.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVipAward.h
# End Source File
# Begin Source File

SOURCE=.\DlgWebShop.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgWebShop.h
# End Source File
# Begin Source File

SOURCE=.\DlgWorldMap.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgWorldMap.h
# End Source File
# Begin Source File

SOURCE=.\DlgZhuxian5Year.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgZhuxian5Year.h
# End Source File
# Begin Source File

SOURCE=.\EC_UIGameSnake.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_UIGameSnake.h
# End Source File
# End Group
# Begin Group "GMTool Files"

# PROP Default_Filter ""
# Begin Group "SimpleDB"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SDBArchive.h
# End Source File
# Begin Source File

SOURCE=.\SDBDatabase.h
# End Source File
# Begin Source File

SOURCE=.\SDBDateTime.h
# End Source File
# Begin Source File

SOURCE=.\SDBField.h
# End Source File
# Begin Source File

SOURCE=.\SDBMisc.cpp
# End Source File
# Begin Source File

SOURCE=.\SDBMisc.h
# End Source File
# Begin Source File

SOURCE=.\SDBRecord.h
# End Source File
# Begin Source File

SOURCE=.\SDBTable.h
# End Source File
# Begin Source File

SOURCE=.\TPBase.h
# End Source File
# Begin Source File

SOURCE=.\TypeList.h
# End Source File
# Begin Source File

SOURCE=.\ValueList.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\GMCDatabase.cpp
# End Source File
# Begin Source File

SOURCE=.\GMCDatabase.h
# End Source File
# Begin Source File

SOURCE=.\GMCDBMan.cpp
# End Source File
# Begin Source File

SOURCE=.\GMCDBMan.h
# End Source File
# Begin Source File

SOURCE=.\GMCommand.h
# End Source File
# Begin Source File

SOURCE=.\GMCommandFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\GMCommandFactory.h
# End Source File
# Begin Source File

SOURCE=.\GMCommandInGame.cpp
# End Source File
# Begin Source File

SOURCE=.\GMCommandInGame.h
# End Source File
# Begin Source File

SOURCE=.\GMCommandListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\GMCommandListBox.h
# End Source File
# Begin Source File

SOURCE=.\Param.cpp
# End Source File
# Begin Source File

SOURCE=.\Param.h
# End Source File
# Begin Source File

SOURCE=.\PredefinedGMHelpMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\PredefinedGMHelpMsg.h
# End Source File
# End Group
# Begin Group "LoginDialogs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DlgLoginPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLoginPage.h
# End Source File
# Begin Source File

SOURCE=.\DlgLoginPwdProtect.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLoginPwdProtect.h
# End Source File
# Begin Source File

SOURCE=.\DlgLoginRefId.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLoginRefId.h
# End Source File
# Begin Source File

SOURCE=.\DlgLoginServerList.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLoginServerList.h
# End Source File
# Begin Source File

SOURCE=.\DlgLoginServerSearch.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLoginServerSearch.h
# End Source File
# Begin Source File

SOURCE=.\EC_ThreadPing.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_ThreadPing.h
# End Source File
# End Group
# Begin Group "Data"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DlgDataConsignViewChar.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDataConsignViewChar.h
# End Source File
# Begin Source File

SOURCE=.\EC_Calendar.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Calendar.h
# End Source File
# Begin Source File

SOURCE=.\EC_UIData.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_UIData.h
# End Source File
# End Group
# Begin Group "UICommon"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DlgBaseFadeOut.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBaseFadeOut.h
# End Source File
# Begin Source File

SOURCE=.\DlgTabContainer.h
# End Source File
# Begin Source File

SOURCE=.\EC_BBSFileDownloader.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_BBSFileDownloader.h
# End Source File
# Begin Source File

SOURCE=.\EC_BBSPage.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_BBSPage.h
# End Source File
# Begin Source File

SOURCE=.\EC_BBSPageFile.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_BBSPageFile.h
# End Source File
# Begin Source File

SOURCE=.\EC_DlgCmdConsole.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_GameUIMisc.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_GameUIMisc.h
# End Source File
# Begin Source File

SOURCE=.\EC_ModelRender.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_ModelRender.h
# End Source File
# Begin Source File

SOURCE=.\EC_PageDownloaderMan.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_PageDownloaderMan.h
# End Source File
# Begin Source File

SOURCE=.\EC_PlayerImage.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_PlayerImage.h
# End Source File
# Begin Source File

SOURCE=.\EC_UIPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_UIPlayer.h
# End Source File
# Begin Source File

SOURCE=..\ZElementData\vector_string.cpp
# End Source File
# Begin Source File

SOURCE=..\ZElementData\vector_string.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\EC_GameUIMan.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_GameUIMan.h
# End Source File
# Begin Source File

SOURCE=.\EC_GameUIMan1.inl
# End Source File
# Begin Source File

SOURCE=.\EC_GameUIMan2.inl
# End Source File
# Begin Source File

SOURCE=.\EC_GameUIManDlgs.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_LoginUIMan.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_LoginUIMan.h
# End Source File
# Begin Source File

SOURCE=.\EC_UIManager.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_UIManager.h
# End Source File
# End Group
# Begin Group "Manager Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\EC_FunGame.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_FunGame.h
# End Source File
# Begin Source File

SOURCE=.\EC_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Manager.h
# End Source File
# Begin Source File

SOURCE=.\EC_ManDecal.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_ManDecal.h
# End Source File
# Begin Source File

SOURCE=.\EC_ManMatter.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_ManMatter.h
# End Source File
# Begin Source File

SOURCE=.\EC_ManMessage.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_ManMessage.h
# End Source File
# Begin Source File

SOURCE=.\EC_ManNPC.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_ManNPC.h
# End Source File
# Begin Source File

SOURCE=.\EC_ManOrnament.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_ManOrnament.h
# End Source File
# Begin Source File

SOURCE=.\EC_ManPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_ManPlayer.h
# End Source File
# Begin Source File

SOURCE=.\EC_ManSkillGfx.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_ManSkillGfx.h
# End Source File
# End Group
# Begin Group "Network"

# PROP Default_Filter ""
# Begin Group "Protocol Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Network\acanswer.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\achievementmessage.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\acquestion.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\acremotecode.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\acreport.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\addfriend.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\addfriend_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\addfriendrqst.hrp
# End Source File
# Begin Source File

SOURCE=.\Network\announcedistrictid.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\announceforbidinfo.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\announcenewmail.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\announcesellresult.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\auctionattendlist.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\auctionattendlist_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\auctionbid.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\auctionbid_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\auctionclose.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\auctionclose_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\auctionexitbid.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\auctionexitbid_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\auctionget.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\auctionget_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\auctiongetitem.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\auctiongetitem_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\auctionlist.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\auctionlist_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\auctionopen.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\auctionopen_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\battlechallenge.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\battlechallenge_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\battlechallengemap.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\battlechallengemap_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\battleenter.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\battleenter_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\battlegetmap.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\battlegetmap_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\battlestatus.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\battlestatus_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\buypoint.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\buypoint_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\challenge.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\changeds_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\chatmessage.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\chatroomcreate.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\chatroomcreate_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\chatroomexpel.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\chatroominvite.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\chatroominvite_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\chatroomjoin.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\chatroomjoin_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\chatroomleave.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\chatroomlist.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\chatroomlist_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\chatroomspeak.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\checknewmail.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\circlelist.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\circlelist_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\circlenotify.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\claimcirclegradbonus.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\complain2gm.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\complain2gm_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\consigngetitem.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\consigngetitem_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\consignlistall.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\consignlistall_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\consignlistlargecategory.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\consignlistlargecategory_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\createrole.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\createrole_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\deletemail.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\deletemail_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\deleterole.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\deleterole_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\delfriend.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\delfriend_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\deliverrolestatus.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\discountannounce.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\dochangegs.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\dochangegs_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\enterworld.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\errorinfo.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\factionacceptjoin.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\factionacceptjoin_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\factionapplyjoin_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\factionappoint_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\factionbroadcastnotice_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\factionchangproclaim_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\factionchat.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\factioncreate_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\factiondegrade_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\factiondismiss_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\factionexpel_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\factioninvitejoin.hrp
# End Source File
# Begin Source File

SOURCE=.\Network\factionleave_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\factionlist_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\factionlistmember_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\factionmasterresign_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\factionoprequest.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\factionoprequest_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\factionpkraidbegin.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\factionpkraidchallenge.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\factionpkraidchallenge_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\factionpkraidchallengecancel.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\factionpkraidchallengecancel_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\factionpkraidgetlist.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\factionpkraidgetlist_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\factionpkraidgetscore.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\factionpkraidgetscore_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\factionpkraidinvite.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\factionpkraidinvite_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\factionpkraidstopfight.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\factionrename_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\factionresign_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\factionupgrade_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\factionwithdrawbonus.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\factionwithdrawbonus_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\fastpay.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\fastpay_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\findrole.hrp
# End Source File
# Begin Source File

SOURCE=.\Network\findrolebyname.hrp
# End Source File
# Begin Source File

SOURCE=.\Network\findsellpointinfo.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\findsellpointinfo_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\friendalllist.hrp
# End Source File
# Begin Source File

SOURCE=.\Network\friendcallbackaward.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\friendcallbackaward_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\friendcallbackinfo.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\friendcallbackinfo_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\friendcallbackmail.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\friendcallbackmail_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\friendcallbacksubscribe.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\friendcallbacksubscribe_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\frienddel.hrp
# End Source File
# Begin Source File

SOURCE=.\Network\friendmove.hrp
# End Source File
# Begin Source File

SOURCE=.\Network\friendstatus.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\fungamedatasend.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\gameclient.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\gameclient.h
# End Source File
# Begin Source File

SOURCE=.\Network\gamedatasend.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\gamezonelist.hrp
# End Source File
# Begin Source File

SOURCE=.\Network\gcirclechat.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\getcirclebaseinfo.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\getcirclebaseinfo_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\getcustomdata.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\getcustomdata_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\getfactionbaseinfo.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\getfactionbaseinfo_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\getfriends.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\getfriends_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\gethelpstates.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\gethelpstates_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\getmail.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\getmail_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\getmailattachobj.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\getmailattachobj_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\getmaillist.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\getmaillist_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\getplayerbriefinfo.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\getplayerbriefinfo_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\getplayerfactioninfo.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\getplayerfactioninfo_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\getplayeridbyname.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\getplayeridbyname_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\getsavedmsg.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\getsavedmsg_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\getselllist.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\getselllist_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\getserverrtt.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\getuiconfig.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\getuiconfig_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\gmforbidrole.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\gmforbidrole_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\gmkickoutrole.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\gmkickoutrole_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\gmkickoutuser.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\gmkickoutuser_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\gmlistonlineuser.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\gmlistonlineuser_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\gmonlinenum.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\gmonlinenum_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\gmrestartserver.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\gmrestartserver_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\gmshutup.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\gmshutup_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\gmshutuprole.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\gmshutuprole_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\gmtogglechat.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\gmtogglechat_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\groupgetinfo.hrp
# End Source File
# Begin Source File

SOURCE=.\Network\groupsetinfo.hrp
# End Source File
# Begin Source File

SOURCE=.\Network\gshopnotifyscheme.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\hometowndatasend.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\hometownexchgmoney_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\instancingaccept.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\instancingaccept_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\instancingappoint.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\instancingappoint_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\instancingappointnotify.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\instancingavailablelist.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\instancingavailablelist_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\instancingclosenotify.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\instancingenter.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\instancingenter_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\instancinggetfield.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\instancinggetfield_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\instancinggetlist.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\instancinggetlist_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\instancingjoin.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\instancingjoin_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\instancingjoinreq.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\instancingkick.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\instancingkick_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\instancingkicknotify.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\instancingleave.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\instancingleave_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\instancingstart.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\instancingstart_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\instancingstartnotify.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\keepalive.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\keyexchange.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\keyreestablish.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\matrixchallenge.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\matrixresponse.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\msgretrieveoffline.hrp
# End Source File
# Begin Source File

SOURCE=.\Network\onlineannounce.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\pkmessage.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\playerbaseinfo.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\playerbaseinfo2.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\playerbaseinfo2_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\playerbaseinfo_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\playerbaseinfocrc.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\playerbaseinfocrc_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\playerchangeds_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\playerlogout.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\playersendmail.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\playersendmail_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\preservemail.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\preservemail_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\privatechat.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\publicchat.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\qqaddfriend.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\qqaddfriend_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\qqaddfriendrqst.hrp
# End Source File
# Begin Source File

SOURCE=.\Network\qqclient.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\qqclient.h
# End Source File
# Begin Source File

SOURCE=.\Network\queryuserprivilege_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\raidappoint.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\raidappoint_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\raidappointnotify.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\raidenter.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\raidenter_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\raidgetlist.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\raidgetlist_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\raidgetroom.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\raidgetroom_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\raidjoin_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\raidjoinaccept.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\raidjoinaccept_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\raidjoinapplylist.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\raidjoinreq.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\raidkick.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\raidkick_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\raidkicknotify.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\raidopen_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\raidquit.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\raidquit_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\raidstart.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\raidstart_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\raidstartnotify.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\refgetreferencecode.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\refgetreferencecode_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\refinemessage.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\reflistreferrals.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\reflistreferrals_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\refwithdrawbonus.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\refwithdrawbonus_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\refwithdrawexp.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\refwithdrawexp_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\report2gm.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\report2gm_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\response.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\rolealllist.hrp
# End Source File
# Begin Source File

SOURCE=.\Network\rolelist.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\rolelist_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\rolestatusannounce.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\selectrole.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\selectrole_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\sellcancel.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\sellcancel_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\sellpoint.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\sellpoint_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\sendmsg.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\setcustomdata.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\setcustomdata_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\setfriendgroup.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\setfriendgroup_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\setgroupname.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\setgroupname_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\sethelpstates.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\sethelpstates_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\setuiconfig.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\setuiconfig_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\siegechallenge.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\siegechallenge_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\siegeenter.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\siegeenter_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\siegeinfoget.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\siegeinfoget_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\siegesetassistant.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\siegesetassistant_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\snsacceptapply.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\snsacceptapply_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\snsapply_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\snsapplynotify.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\snscancelmessage.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\snscancelmessage_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\snsgetmessage.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\snsgetmessage_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\snsgetplayerinfo.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\snsgetplayerinfo_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\snslistmessage.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\snslistmessage_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\snspressmessage_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\snssetplayerinfo.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\snssetplayerinfo_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\snsvote_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\statusannounce.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\territorychallenge_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\territoryenter_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\territoryenterremind.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\territoryleave_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\territorymapget.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\territorymapget_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\territoryscoreupdate.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\tradeaddgoods.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\tradeaddgoods_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\tradeconfirm.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\tradeconfirm_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\tradediscard.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\tradediscard_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\tradeend.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\trademoveobj.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\trademoveobj_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\traderemovegoods.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\traderemovegoods_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\tradestart.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\tradestart_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\tradestartrqst.hrp
# End Source File
# Begin Source File

SOURCE=.\Network\tradesubmit.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\tradesubmit_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\trychangegs.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\trychangegs_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\undodeleterole.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\undodeleterole_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\uniquebid_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\uniquebidhistory.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\uniquebidhistory_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\uniquegetitem_re.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\updateremaintime.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\usergetinfo.hrp
# End Source File
# Begin Source File

SOURCE=.\Network\usersetinfo.hrp
# End Source File
# Begin Source File

SOURCE=.\Network\worldchat.hpp
# End Source File
# End Group
# Begin Group "inl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Network\inl\achievementmessage
# End Source File
# Begin Source File

SOURCE=.\Network\inl\acremotecode
# End Source File
# Begin Source File

SOURCE=.\Network\inl\acreport
# End Source File
# Begin Source File

SOURCE=.\Network\inl\addfriend
# End Source File
# Begin Source File

SOURCE=.\Network\inl\addfriend_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\addfriendrqst
# End Source File
# Begin Source File

SOURCE=.\Network\inl\announcedistrictid
# End Source File
# Begin Source File

SOURCE=.\Network\inl\announceforbidinfo
# End Source File
# Begin Source File

SOURCE=.\Network\inl\announcenewmail
# End Source File
# Begin Source File

SOURCE=.\Network\inl\announcesellresult
# End Source File
# Begin Source File

SOURCE=.\Network\inl\auctionattendlist
# End Source File
# Begin Source File

SOURCE=.\Network\inl\auctionattendlist_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\auctionbid
# End Source File
# Begin Source File

SOURCE=.\Network\inl\auctionbid_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\auctionclose
# End Source File
# Begin Source File

SOURCE=.\Network\inl\auctionclose_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\auctionexitbid
# End Source File
# Begin Source File

SOURCE=.\Network\inl\auctionexitbid_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\auctionget
# End Source File
# Begin Source File

SOURCE=.\Network\inl\auctionget_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\auctiongetitem
# End Source File
# Begin Source File

SOURCE=.\Network\inl\auctiongetitem_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\auctionlist
# End Source File
# Begin Source File

SOURCE=.\Network\inl\auctionlist_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\auctionopen
# End Source File
# Begin Source File

SOURCE=.\Network\inl\auctionopen_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\battlechallenge
# End Source File
# Begin Source File

SOURCE=.\Network\inl\battlechallenge_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\battlechallengemap
# End Source File
# Begin Source File

SOURCE=.\Network\inl\battlechallengemap_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\battleenter
# End Source File
# Begin Source File

SOURCE=.\Network\inl\battleenter_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\battlegetmap
# End Source File
# Begin Source File

SOURCE=.\Network\inl\battlegetmap_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\battlestatus
# End Source File
# Begin Source File

SOURCE=.\Network\inl\battlestatus_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\buypoint
# End Source File
# Begin Source File

SOURCE=.\Network\inl\buypoint_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\challenge
# End Source File
# Begin Source File

SOURCE=.\Network\inl\changeds_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\chatmessage
# End Source File
# Begin Source File

SOURCE=.\Network\inl\chatroomcreate
# End Source File
# Begin Source File

SOURCE=.\Network\inl\chatroomcreate_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\chatroomexpel
# End Source File
# Begin Source File

SOURCE=.\Network\inl\chatroominvite
# End Source File
# Begin Source File

SOURCE=.\Network\inl\chatroominvite_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\chatroomjoin
# End Source File
# Begin Source File

SOURCE=.\Network\inl\chatroomjoin_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\chatroomleave
# End Source File
# Begin Source File

SOURCE=.\Network\inl\chatroomlist
# End Source File
# Begin Source File

SOURCE=.\Network\inl\chatroomlist_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\chatroomspeak
# End Source File
# Begin Source File

SOURCE=.\Network\inl\checknewmail
# End Source File
# Begin Source File

SOURCE=.\Network\inl\circlelist
# End Source File
# Begin Source File

SOURCE=.\Network\inl\circlelist_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\circlenotify
# End Source File
# Begin Source File

SOURCE=.\Network\inl\claimcirclegradbonus
# End Source File
# Begin Source File

SOURCE=.\Network\inl\complain2gm
# End Source File
# Begin Source File

SOURCE=.\Network\inl\complain2gm_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\consigngetitem
# End Source File
# Begin Source File

SOURCE=.\Network\inl\consigngetitem_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\consignlistall
# End Source File
# Begin Source File

SOURCE=.\Network\inl\consignlistall_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\consignlistlargecategory
# End Source File
# Begin Source File

SOURCE=.\Network\inl\consignlistlargecategory_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\createrole
# End Source File
# Begin Source File

SOURCE=.\Network\inl\createrole_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\deletemail
# End Source File
# Begin Source File

SOURCE=.\Network\inl\deletemail_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\deleterole
# End Source File
# Begin Source File

SOURCE=.\Network\inl\deleterole_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\delfriend
# End Source File
# Begin Source File

SOURCE=.\Network\inl\delfriend_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\deliverrolestatus
# End Source File
# Begin Source File

SOURCE=.\Network\inl\dochangegs
# End Source File
# Begin Source File

SOURCE=.\Network\inl\dochangegs_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\enterworld
# End Source File
# Begin Source File

SOURCE=.\Network\inl\errorinfo
# End Source File
# Begin Source File

SOURCE=.\Network\inl\factionacceptjoin
# End Source File
# Begin Source File

SOURCE=.\Network\inl\factionacceptjoin_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\factionapplyjoin_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\factionappoint_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\factionbroadcastnotice_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\factionchangproclaim_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\factionchat
# End Source File
# Begin Source File

SOURCE=.\Network\inl\factioncreate_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\factiondegrade_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\factiondismiss_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\factionexpel_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\factioninvitejoin
# End Source File
# Begin Source File

SOURCE=.\Network\inl\factionleave_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\factionlistmember_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\factionmasterresign_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\factionoprequest
# End Source File
# Begin Source File

SOURCE=.\Network\inl\factionoprequest_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\factionpkraidbegin
# End Source File
# Begin Source File

SOURCE=.\Network\inl\factionpkraidchallenge
# End Source File
# Begin Source File

SOURCE=.\Network\inl\factionpkraidchallenge_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\factionpkraidchallengecancel
# End Source File
# Begin Source File

SOURCE=.\Network\inl\factionpkraidchallengecancel_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\factionpkraidgetlist
# End Source File
# Begin Source File

SOURCE=.\Network\inl\factionpkraidgetlist_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\factionpkraidgetscore
# End Source File
# Begin Source File

SOURCE=.\Network\inl\factionpkraidgetscore_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\factionpkraidinvite
# End Source File
# Begin Source File

SOURCE=.\Network\inl\factionpkraidinvite_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\factionpkraidstopfight
# End Source File
# Begin Source File

SOURCE=.\Network\inl\factionrecruit
# End Source File
# Begin Source File

SOURCE=.\Network\inl\factionrename_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\factionresign_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\factionupgrade_re
# End Source File
# Begin Source File

SOURCE=.\Network\fastpaybindinfo.hpp
# End Source File
# Begin Source File

SOURCE=.\Network\inl\findrole
# End Source File
# Begin Source File

SOURCE=.\Network\inl\findrolebyname
# End Source File
# Begin Source File

SOURCE=.\Network\inl\findsellpointinfo
# End Source File
# Begin Source File

SOURCE=.\Network\inl\findsellpointinfo_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\friendalllist
# End Source File
# Begin Source File

SOURCE=.\Network\inl\friendcallbackaward
# End Source File
# Begin Source File

SOURCE=.\Network\inl\friendcallbackaward_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\friendcallbackinfo
# End Source File
# Begin Source File

SOURCE=.\Network\inl\friendcallbackinfo_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\friendcallbackmail
# End Source File
# Begin Source File

SOURCE=.\Network\inl\friendcallbackmail_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\friendcallbacksubscribe
# End Source File
# Begin Source File

SOURCE=.\Network\inl\friendcallbacksubscribe_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\frienddel
# End Source File
# Begin Source File

SOURCE=.\Network\inl\friendmove
# End Source File
# Begin Source File

SOURCE=.\Network\inl\friendstatus
# End Source File
# Begin Source File

SOURCE=.\Network\inl\fungamedatasend
# End Source File
# Begin Source File

SOURCE=.\Network\inl\gamedatasend
# End Source File
# Begin Source File

SOURCE=.\Network\inl\gamezonelist
# End Source File
# Begin Source File

SOURCE=.\Network\inl\gcirclechat
# End Source File
# Begin Source File

SOURCE=.\Network\inl\getcirclebaseinfo
# End Source File
# Begin Source File

SOURCE=.\Network\inl\getcirclebaseinfo_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\getcustomdata
# End Source File
# Begin Source File

SOURCE=.\Network\inl\getcustomdata_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\getfactionbaseinfo
# End Source File
# Begin Source File

SOURCE=.\Network\inl\getfactionbaseinfo_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\getfriends
# End Source File
# Begin Source File

SOURCE=.\Network\inl\getfriends_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\gethelpstates
# End Source File
# Begin Source File

SOURCE=.\Network\inl\gethelpstates_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\getmail
# End Source File
# Begin Source File

SOURCE=.\Network\inl\getmail_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\getmailattachobj
# End Source File
# Begin Source File

SOURCE=.\Network\inl\getmailattachobj_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\getmaillist
# End Source File
# Begin Source File

SOURCE=.\Network\inl\getmaillist_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\getplayerbriefinfo
# End Source File
# Begin Source File

SOURCE=.\Network\inl\getplayerbriefinfo_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\getplayerfactioninfo
# End Source File
# Begin Source File

SOURCE=.\Network\inl\getplayerfactioninfo_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\getplayeridbyname
# End Source File
# Begin Source File

SOURCE=.\Network\inl\getplayeridbyname_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\getsavedmsg
# End Source File
# Begin Source File

SOURCE=.\Network\inl\getsavedmsg_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\getselllist
# End Source File
# Begin Source File

SOURCE=.\Network\inl\getselllist_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\getserverrtt
# End Source File
# Begin Source File

SOURCE=.\Network\inl\getuiconfig
# End Source File
# Begin Source File

SOURCE=.\Network\inl\getuiconfig_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\gmforbidrole
# End Source File
# Begin Source File

SOURCE=.\Network\inl\gmforbidrole_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\gmkickoutrole
# End Source File
# Begin Source File

SOURCE=.\Network\inl\gmkickoutrole_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\gmkickoutuser
# End Source File
# Begin Source File

SOURCE=.\Network\inl\gmkickoutuser_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\gmlistonlineuser
# End Source File
# Begin Source File

SOURCE=.\Network\inl\gmlistonlineuser_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\gmonlinenum
# End Source File
# Begin Source File

SOURCE=.\Network\inl\gmonlinenum_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\gmrestartserver
# End Source File
# Begin Source File

SOURCE=.\Network\inl\gmrestartserver_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\gmshutup
# End Source File
# Begin Source File

SOURCE=.\Network\inl\gmshutup_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\gmshutuprole
# End Source File
# Begin Source File

SOURCE=.\Network\inl\gmshutuprole_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\gmtogglechat
# End Source File
# Begin Source File

SOURCE=.\Network\inl\gmtogglechat_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\groupgetinfo
# End Source File
# Begin Source File

SOURCE=.\Network\inl\groupsetinfo
# End Source File
# Begin Source File

SOURCE=.\Network\inl\gshopnotifyscheme
# End Source File
# Begin Source File

SOURCE=.\Network\inl\hometowndatasend
# End Source File
# Begin Source File

SOURCE=.\Network\inl\hometownexchgmoney_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\instancingaccept
# End Source File
# Begin Source File

SOURCE=.\Network\inl\instancingaccept_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\instancingappoint
# End Source File
# Begin Source File

SOURCE=.\Network\inl\instancingappoint_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\instancingappointnotify
# End Source File
# Begin Source File

SOURCE=.\Network\inl\instancingavailablelist
# End Source File
# Begin Source File

SOURCE=.\Network\inl\instancingavailablelist_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\instancingclosenotify
# End Source File
# Begin Source File

SOURCE=.\Network\inl\instancingenter
# End Source File
# Begin Source File

SOURCE=.\Network\inl\instancingenter_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\instancinggetfield
# End Source File
# Begin Source File

SOURCE=.\Network\inl\instancinggetfield_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\instancinggetlist
# End Source File
# Begin Source File

SOURCE=.\Network\inl\instancinggetlist_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\instancingjoin_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\instancingjoinreq
# End Source File
# Begin Source File

SOURCE=.\Network\inl\instancingkick
# End Source File
# Begin Source File

SOURCE=.\Network\inl\instancingkick_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\instancingkicknotify
# End Source File
# Begin Source File

SOURCE=.\Network\inl\instancingleave
# End Source File
# Begin Source File

SOURCE=.\Network\inl\instancingleave_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\instancingstart
# End Source File
# Begin Source File

SOURCE=.\Network\inl\instancingstart_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\instancingstartnotify
# End Source File
# Begin Source File

SOURCE=.\Network\inl\keepalive
# End Source File
# Begin Source File

SOURCE=.\Network\inl\keyexchange
# End Source File
# Begin Source File

SOURCE=.\Network\inl\keyreestablish
# End Source File
# Begin Source File

SOURCE=.\Network\inl\matrixchallenge
# End Source File
# Begin Source File

SOURCE=.\Network\inl\matrixresponse
# End Source File
# Begin Source File

SOURCE=.\Network\inl\msgretrieveoffline
# End Source File
# Begin Source File

SOURCE=.\Network\inl\onlineannounce
# End Source File
# Begin Source File

SOURCE=.\Network\inl\pkmessage
# End Source File
# Begin Source File

SOURCE=.\Network\inl\playerbaseinfo
# End Source File
# Begin Source File

SOURCE=.\Network\inl\playerbaseinfo2
# End Source File
# Begin Source File

SOURCE=.\Network\inl\playerbaseinfo2_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\playerbaseinfo_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\playerbaseinfocrc
# End Source File
# Begin Source File

SOURCE=.\Network\inl\playerbaseinfocrc_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\playerchangeds_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\playerchangegs
# End Source File
# Begin Source File

SOURCE=.\Network\inl\playerchangegs_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\playerlogout
# End Source File
# Begin Source File

SOURCE=.\Network\inl\playersendmail
# End Source File
# Begin Source File

SOURCE=.\Network\inl\playersendmail_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\preservemail
# End Source File
# Begin Source File

SOURCE=.\Network\inl\preservemail_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\privatechat
# End Source File
# Begin Source File

SOURCE=.\Network\inl\publicchat
# End Source File
# Begin Source File

SOURCE=.\Network\inl\qqaddfriend
# End Source File
# Begin Source File

SOURCE=.\Network\inl\qqaddfriend_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\qqaddfriendrqst
# End Source File
# Begin Source File

SOURCE=.\Network\inl\queryuserprivilege_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\raidappoint
# End Source File
# Begin Source File

SOURCE=.\Network\inl\raidappoint_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\raidappointnotify
# End Source File
# Begin Source File

SOURCE=.\Network\inl\raidenter
# End Source File
# Begin Source File

SOURCE=.\Network\inl\raidenter_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\raidgetlist
# End Source File
# Begin Source File

SOURCE=.\Network\inl\raidgetlist_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\raidgetroom
# End Source File
# Begin Source File

SOURCE=.\Network\inl\raidgetroom_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\raidjoin_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\raidjoinaccept
# End Source File
# Begin Source File

SOURCE=.\Network\inl\raidjoinaccept_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\raidjoinapplylist
# End Source File
# Begin Source File

SOURCE=.\Network\inl\raidjoinreq
# End Source File
# Begin Source File

SOURCE=.\Network\inl\raidkick
# End Source File
# Begin Source File

SOURCE=.\Network\inl\raidkick_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\raidkicknotify
# End Source File
# Begin Source File

SOURCE=.\Network\inl\raidopen_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\raidquit
# End Source File
# Begin Source File

SOURCE=.\Network\inl\raidquit_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\raidstart
# End Source File
# Begin Source File

SOURCE=.\Network\inl\raidstart_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\raidstartnotify
# End Source File
# Begin Source File

SOURCE=.\Network\inl\refgetreferencecode
# End Source File
# Begin Source File

SOURCE=.\Network\inl\refgetreferencecode_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\refinemessage
# End Source File
# Begin Source File

SOURCE=.\Network\inl\reflistreferrals
# End Source File
# Begin Source File

SOURCE=.\Network\inl\reflistreferrals_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\refwithdrawbonus
# End Source File
# Begin Source File

SOURCE=.\Network\inl\refwithdrawbonus_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\refwithdrawexp
# End Source File
# Begin Source File

SOURCE=.\Network\inl\refwithdrawexp_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\report2gm
# End Source File
# Begin Source File

SOURCE=.\Network\inl\report2gm_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\response
# End Source File
# Begin Source File

SOURCE=.\Network\inl\rolealllist
# End Source File
# Begin Source File

SOURCE=.\Network\inl\rolelist
# End Source File
# Begin Source File

SOURCE=.\Network\inl\rolelist_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\rolestatusannounce
# End Source File
# Begin Source File

SOURCE=.\Network\inl\selectrole
# End Source File
# Begin Source File

SOURCE=.\Network\inl\selectrole_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\sellcancel
# End Source File
# Begin Source File

SOURCE=.\Network\inl\sellcancel_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\sellpoint
# End Source File
# Begin Source File

SOURCE=.\Network\inl\sellpoint_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\sendmsg
# End Source File
# Begin Source File

SOURCE=.\Network\inl\setcustomdata
# End Source File
# Begin Source File

SOURCE=.\Network\inl\setcustomdata_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\setfriendgroup
# End Source File
# Begin Source File

SOURCE=.\Network\inl\setfriendgroup_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\setgroupname
# End Source File
# Begin Source File

SOURCE=.\Network\inl\setgroupname_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\sethelpstates
# End Source File
# Begin Source File

SOURCE=.\Network\inl\sethelpstates_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\setuiconfig
# End Source File
# Begin Source File

SOURCE=.\Network\inl\setuiconfig_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\snsacceptapply
# End Source File
# Begin Source File

SOURCE=.\Network\inl\snsacceptapply_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\snsapply_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\snsapplynotify
# End Source File
# Begin Source File

SOURCE=.\Network\inl\snscancelmessage
# End Source File
# Begin Source File

SOURCE=.\Network\inl\snscancelmessage_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\snsgetmessage
# End Source File
# Begin Source File

SOURCE=.\Network\inl\snsgetmessage_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\snsgetplayerinfo
# End Source File
# Begin Source File

SOURCE=.\Network\inl\snsgetplayerinfo_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\snslistmessage
# End Source File
# Begin Source File

SOURCE=.\Network\inl\snslistmessage_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\snspressmessage_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\snssetplayerinfo
# End Source File
# Begin Source File

SOURCE=.\Network\inl\snssetplayerinfo_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\snsvote_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\statusannounce
# End Source File
# Begin Source File

SOURCE=.\Network\inl\territorychallenge_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\territoryenter_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\territoryenterremind
# End Source File
# Begin Source File

SOURCE=.\Network\inl\territoryleave_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\territorymapget
# End Source File
# Begin Source File

SOURCE=.\Network\inl\territorymapget_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\territoryscoreupdate
# End Source File
# Begin Source File

SOURCE=.\Network\inl\topflower_err
# End Source File
# Begin Source File

SOURCE=.\Network\inl\topflower_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\topflowerreq
# End Source File
# Begin Source File

SOURCE=.\Network\inl\tradeaddgoods
# End Source File
# Begin Source File

SOURCE=.\Network\inl\tradeaddgoods_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\tradeconfirm
# End Source File
# Begin Source File

SOURCE=.\Network\inl\tradeconfirm_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\tradediscard
# End Source File
# Begin Source File

SOURCE=.\Network\inl\tradediscard_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\tradeend
# End Source File
# Begin Source File

SOURCE=.\Network\inl\tradeinventory
# End Source File
# Begin Source File

SOURCE=.\Network\inl\trademoveobj
# End Source File
# Begin Source File

SOURCE=.\Network\inl\trademoveobj_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\traderemovegoods
# End Source File
# Begin Source File

SOURCE=.\Network\inl\traderemovegoods_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\tradesave
# End Source File
# Begin Source File

SOURCE=.\Network\inl\tradestart
# End Source File
# Begin Source File

SOURCE=.\Network\inl\tradestart_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\tradestartrqst
# End Source File
# Begin Source File

SOURCE=.\Network\inl\tradesubmit
# End Source File
# Begin Source File

SOURCE=.\Network\inl\tradesubmit_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\trychangegs
# End Source File
# Begin Source File

SOURCE=.\Network\inl\trychangegs_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\undodeleterole
# End Source File
# Begin Source File

SOURCE=.\Network\inl\undodeleterole_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\uniquebid_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\uniquebidhistory
# End Source File
# Begin Source File

SOURCE=.\Network\inl\uniquebidhistory_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\uniquegetitem_re
# End Source File
# Begin Source File

SOURCE=.\Network\inl\updateremaintime
# End Source File
# Begin Source File

SOURCE=.\Network\inl\usergetinfo
# End Source File
# Begin Source File

SOURCE=.\Network\inl\usersetinfo
# End Source File
# Begin Source File

SOURCE=.\Network\inl\worldchat
# End Source File
# End Group
# Begin Group "rpcdata"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Network\rpcdata\addfriendrqstarg
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\addfriendrqstres
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\auctionid
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\circlememberinfo
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\factioninvitearg
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\factioninviteres
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\findrolearg
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\findrolebynamearg
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\findrolebynameres
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\findroleres
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\fmemberinfo
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\friendalllistarg
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\friendalllistres
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\friendbrief
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\frienddelarg
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\frienddelres
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\friendmovearg
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\friendmoveres
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\gamezonelistarg
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\gamezonelistres
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\gauctionbrief
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\gauctiondetail
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\gauctionitem
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\gauctionlist
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\gbattlechallenge
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\gbattlerole
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\gchatmember
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\gchatroom
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\gcirclebaseinfo
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\gcirclehistory
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\gconsignlistnode
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\gfactionbaseinfo
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\gfactionpkraidrecord
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\gfactionpkroominfo
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\gfieldinfo
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\gfriendinfo
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\ggroupinfo
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\ginstancingfieldinfo
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\gmail
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\gmailheader
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\gmailid
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\gmailsyncdata
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\gmplayerinfo
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\graidroominfo
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\grolebase
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\groleforbid
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\groleinventory
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\groomdetail
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\groupgetinfoarg
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\groupgetinfores
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\groupsetinfoarg
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\groupsetinfores
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\gterritory
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\guserfaction
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\message
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\msg
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\msgretrieveofflinearg
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\msgretrieveofflineres
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\playerbriefinfo
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\qqaddfriendrqstarg
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\qqaddfriendrqstres
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\raidfieldinfo
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\raidgroupinfo
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\raidgrouplimit
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\referralbrief
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\role
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\rolealllistarg
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\rolealllistres
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\roleinfo
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\sellid
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\sellpointinfo
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\sendflowerreq
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\snsmessagebrief
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\snsplayerinfo
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\snsplayerinfocache
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\snsroleinfo
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\territorybrief
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\tradestartrqstarg
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\tradestartrqstres
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\transid
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\uniquewinner
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\usergetinfoarg
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\usergetinfores
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\userinfodetail
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\usersetinfoarg
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\usersetinfores
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdata\zone
# End Source File
# End Group
# Begin Group "IOLib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Network\IOLib\gnactiveio.h
# End Source File
# Begin Source File

SOURCE=.\Network\IOLib\gnbyteorder.h
# End Source File
# Begin Source File

SOURCE=.\Network\IOLib\gnconf.h
# End Source File
# Begin Source File

SOURCE=.\Network\IOLib\gnerrno.h
# End Source File
# Begin Source File

SOURCE=.\Network\IOLib\gnmarshal.h
# End Source File
# Begin Source File

SOURCE=.\Network\IOLib\gnnetio.h
# End Source File
# Begin Source File

SOURCE=.\Network\IOLib\gnoctets.h
# End Source File
# Begin Source File

SOURCE=.\Network\IOLib\gnpassiveio.h
# End Source File
# Begin Source File

SOURCE=.\Network\IOLib\gnpollio.h
# End Source File
# Begin Source File

SOURCE=.\Network\IOLib\gnproto.h
# End Source File
# Begin Source File

SOURCE=.\Network\IOLib\gnrpc.h
# End Source File
# Begin Source File

SOURCE=.\Network\IOLib\gnsecure.h
# End Source File
# Begin Source File

SOURCE=.\Network\IOLib\gnstatic.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\IOLib\gnthread.h
# End Source File
# Begin Source File

SOURCE=.\Network\IOLib\gntimer.h
# End Source File
# Begin Source File

SOURCE=.\Network\IOLib\iolib.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Network\callid.hxx
# End Source File
# Begin Source File

SOURCE=.\EC_C2SCmdCache.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_C2SCmdCache.h
# End Source File
# Begin Source File

SOURCE=.\Network\EC_FGC2SRawCmds.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\EC_FGC2SRawCmds.h
# End Source File
# Begin Source File

SOURCE=.\Network\EC_FGCHMisc.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\EC_FGPDataType.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\EC_FGPDataType.h
# End Source File
# Begin Source File

SOURCE=.\Network\EC_FunGamePrtcHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\EC_FunGamePrtcHandler.h
# End Source File
# Begin Source File

SOURCE=.\Network\EC_GameDataPrtc.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\EC_GameSession.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\EC_GameSession.h
# End Source File
# Begin Source File

SOURCE=.\Network\EC_GPDataType.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\EC_GPDataType.h
# End Source File
# Begin Source File

SOURCE=.\Network\EC_HomeTownPrtcHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\EC_HomeTownPrtcHandler.h
# End Source File
# Begin Source File

SOURCE=.\Network\EC_HTC2SRawCmds.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\EC_HTC2SRawCmds.h
# End Source File
# Begin Source File

SOURCE=.\Network\EC_HTCHMisc.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\EC_HTPDataType.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\EC_HTPDataType.h
# End Source File
# Begin Source File

SOURCE=.\Network\EC_NetDef.h
# End Source File
# Begin Source File

SOURCE=.\Network\EC_NetProtocol.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\EC_NetProtocol.h
# End Source File
# Begin Source File

SOURCE=.\Network\EC_NetProtocolBase.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\EC_NetProtocolBase.h
# End Source File
# Begin Source File

SOURCE=.\Network\EC_NetPrtclGame.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\EC_NetPrtclGame.h
# End Source File
# Begin Source File

SOURCE=.\Network\EC_NetPrtclNPCInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\EC_NetPrtclNPCInfo.h
# End Source File
# Begin Source File

SOURCE=.\Network\EC_NetPrtclObjectInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\EC_NetPrtclObjectInfo.h
# End Source File
# Begin Source File

SOURCE=.\Network\EC_NetPrtclPlayerInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\EC_NetPrtclPlayerInfo.h
# End Source File
# Begin Source File

SOURCE=.\Network\EC_NetRawProtocol.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\EC_NetRawProtocol.h
# End Source File
# Begin Source File

SOURCE=.\Network\EC_PrtcPBIProc.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\EC_PrtcPBIProc.h
# End Source File
# Begin Source File

SOURCE=.\Network\EC_PrtcProc.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_SendC2SCmds.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_SendC2SCmds.h
# End Source File
# Begin Source File

SOURCE=.\Network\factiondata.hxx
# End Source File
# Begin Source File

SOURCE=.\Network\forbid.hxx
# End Source File
# Begin Source File

SOURCE=.\Network\gnet.h
# End Source File
# Begin Source File

SOURCE=.\Network\gnetdef.h
# End Source File
# Begin Source File

SOURCE=.\Network\ids.hxx
# End Source File
# Begin Source File

SOURCE=.\Network\netclient.h
# End Source File
# Begin Source File

SOURCE=.\Network\pingclient.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\pingclient.h
# End Source File
# Begin Source File

SOURCE=.\Network\privilege.hxx
# End Source File
# Begin Source File

SOURCE=.\Network\rpcdefs.h
# End Source File
# Begin Source File

SOURCE=.\Network\state.cxx
# End Source File
# Begin Source File

SOURCE=.\Network\state.hxx
# End Source File
# Begin Source File

SOURCE=.\Network\stubs.cxx
# End Source File
# Begin Source File

SOURCE=.\Network\stubs1.cxx
# End Source File
# Begin Source File

SOURCE=.\Network\topflower_err.hpp
# End Source File
# End Group
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Group "AutoCa"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ZCommon\AUTOCA\A3DAutoButterfly.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\AUTOCA\A3DAutoButterfly.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\AUTOCA\A3DAutoCrow.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\AUTOCA\A3DAutoCrow.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\AUTOCA\A3DAutoEagle.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\AUTOCA\A3DAutoEagle.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\AUTOCA\A3DAutoFirefly.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\AUTOCA\A3DAutoFirefly.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\AUTOCA\A3DAutoFish.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\AUTOCA\A3DAutoFish.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\AUTOCA\A3DAutonomousAgent.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\AUTOCA\A3DAutonomousAgent.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\AUTOCA\A3DAutonomousBehaviour.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\AUTOCA\A3DAutonomousBehaviour.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\AUTOCA\A3DAutonomousGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\AUTOCA\A3DAutonomousGroup.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\AUTOCA\Utilities.h
# End Source File
# End Group
# Begin Group "Face"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ZCommon\Face\Expression.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\Face\Expression.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\Face\FaceAnimation.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\Face\FaceAnimation.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\Face\FaceBone.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\Face\FaceBone.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\Face\FaceBoneController.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\Face\FaceBoneController.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\Face\FaceCommon.h
# End Source File
# End Group
# Begin Group "FireWorks"

# PROP Default_Filter ""
# End Group
# Begin Group "Cloud"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ZCommon\EL_Cloud.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EL_Cloud.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EL_CloudManager.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EL_CloudManager.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EL_CloudSprite.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EL_CloudSprite.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\EC_Algorithm.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EC_BrushMan.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EC_BrushMan.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EC_BrushManUtil.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EC_MD5Hash.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EC_MD5Hash.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EC_TriangleMan.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EC_TriangleMan.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EL_Archive.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EL_Archive.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EL_BackMusic.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EL_BackMusic.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EL_BrushBuilding.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EL_BrushBuilding.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EL_Building.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EL_Building.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EL_Forest.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EL_Forest.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EL_Grassland.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EL_Grassland.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EL_GrassType.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EL_GrassType.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EL_Precinct.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EL_Precinct.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EL_Region.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EL_Region.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EL_Tree.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EL_Tree.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\elementdataman.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\elementdataman.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\elementpckdir.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\ELOutlineQuadTree.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\ELOutlineQuadTree.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\ELOutlineTexture.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\ELOutlineTexture.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\ELTerrainOutline2.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\ELTerrainOutline2.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\ExpTypes.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\generate_item_temp.h
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

SOURCE=..\ZCommon\itemdataman.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\itemdataman.h
# End Source File
# End Group
# Begin Group "Task"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Task\EC_TaskInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\Task\EC_TaskInterface.h
# End Source File
# Begin Source File

SOURCE=.\Task\TaskAsyncData.h
# End Source File
# Begin Source File

SOURCE=.\Task\TaskClient.cpp
# End Source File
# Begin Source File

SOURCE=.\Task\TaskClient.h
# End Source File
# Begin Source File

SOURCE=.\task\TaskExpAnalyser.cpp
# End Source File
# Begin Source File

SOURCE=.\task\TaskExpAnalyser.h
# End Source File
# Begin Source File

SOURCE=.\Task\TaskInterface.h
# End Source File
# Begin Source File

SOURCE=.\Task\TaskProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\Task\TaskProcess.h
# End Source File
# Begin Source File

SOURCE=.\Task\TaskTempl.cpp
# End Source File
# Begin Source File

SOURCE=.\Task\TaskTempl.h
# End Source File
# Begin Source File

SOURCE=.\Task\TaskTempl.inl
# End Source File
# Begin Source File

SOURCE=.\Task\TaskTemplMan.cpp
# End Source File
# Begin Source File

SOURCE=.\Task\TaskTemplMan.h
# End Source File
# End Group
# Begin Group "HelpSystem Files"

# PROP Default_Filter ""
# Begin Group "oldhelp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CodeTemplate.h
# End Source File
# Begin Source File

SOURCE=.\DlgScriptHelp.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgScriptHelp.h
# End Source File
# Begin Source File

SOURCE=.\DlgScriptHelpHistory.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgScriptHelpHistory.h
# End Source File
# Begin Source File

SOURCE=.\DlgSettingScriptHelp.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSettingScriptHelp.h
# End Source File
# Begin Source File

SOURCE=.\ECParamList.cpp
# End Source File
# Begin Source File

SOURCE=.\ECParamList.h
# End Source File
# Begin Source File

SOURCE=.\ECScript.cpp
# End Source File
# Begin Source File

SOURCE=.\ECScript.h
# End Source File
# Begin Source File

SOURCE=.\ECScriptBoolExp.cpp
# End Source File
# Begin Source File

SOURCE=.\ECScriptBoolExp.h
# End Source File
# Begin Source File

SOURCE=.\ECScriptCheckBase.cpp
# End Source File
# Begin Source File

SOURCE=.\ECScriptCheckBase.h
# End Source File
# Begin Source File

SOURCE=.\ECScriptCheckImp.cpp
# End Source File
# Begin Source File

SOURCE=.\ECScriptCheckImp.h
# End Source File
# Begin Source File

SOURCE=.\ECScriptCheckState.cpp
# End Source File
# Begin Source File

SOURCE=.\ECScriptCheckState.h
# End Source File
# Begin Source File

SOURCE=.\ECScriptCheckStateInGame.cpp
# End Source File
# Begin Source File

SOURCE=.\ECScriptCheckStateInGame.h
# End Source File
# Begin Source File

SOURCE=.\ECScriptContext.cpp
# End Source File
# Begin Source File

SOURCE=.\ECScriptContext.h
# End Source File
# Begin Source File

SOURCE=.\ECScriptController.cpp
# End Source File
# Begin Source File

SOURCE=.\ECScriptController.h
# End Source File
# Begin Source File

SOURCE=.\ECScriptFunctionBase.cpp
# End Source File
# Begin Source File

SOURCE=.\ECScriptFunctionBase.h
# End Source File
# Begin Source File

SOURCE=.\ECScriptFunctionInGame.cpp
# End Source File
# Begin Source File

SOURCE=.\ECScriptFunctionInGame.h
# End Source File
# Begin Source File

SOURCE=.\ECScriptGlobalResource.h
# End Source File
# Begin Source File

SOURCE=.\ECScriptGlobalResourceInGame.cpp
# End Source File
# Begin Source File

SOURCE=.\ECScriptGlobalResourceInGame.h
# End Source File
# Begin Source File

SOURCE=.\ECScriptMan.cpp
# End Source File
# Begin Source File

SOURCE=.\ECScriptMan.h
# End Source File
# Begin Source File

SOURCE=.\ECScriptManInGame.cpp
# End Source File
# Begin Source File

SOURCE=.\ECScriptManInGame.h
# End Source File
# Begin Source File

SOURCE=.\ECScriptOption.cpp
# End Source File
# Begin Source File

SOURCE=.\ECScriptOption.h
# End Source File
# Begin Source File

SOURCE=.\ECScriptOptionInGame.cpp
# End Source File
# Begin Source File

SOURCE=.\ECScriptOptionInGame.h
# End Source File
# Begin Source File

SOURCE=.\ECScriptUI.h
# End Source File
# Begin Source File

SOURCE=.\ECScriptUIInGame.cpp
# End Source File
# Begin Source File

SOURCE=.\ECScriptUIInGame.h
# End Source File
# Begin Source File

SOURCE=.\ECScriptUnit.cpp
# End Source File
# Begin Source File

SOURCE=.\ECScriptUnit.h
# End Source File
# Begin Source File

SOURCE=.\ECScriptUnitExecutor.cpp
# End Source File
# Begin Source File

SOURCE=.\ECScriptUnitExecutor.h
# End Source File
# Begin Source File

SOURCE=.\Factory.h
# End Source File
# Begin Source File

SOURCE=.\SingletonHolder.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\EC_HelpGraphGuide.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_HelpGraphGuide.h
# End Source File
# Begin Source File

SOURCE=.\EC_HelpStep00Base.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_HelpStep00Base.h
# End Source File
# Begin Source File

SOURCE=.\EC_HelpStep00Start.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_HelpStep00Start.h
# End Source File
# Begin Source File

SOURCE=.\EC_HelpStep01Move.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_HelpStep01Move.h
# End Source File
# Begin Source File

SOURCE=.\EC_HelpStep02Equip.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_HelpStep02Equip.h
# End Source File
# Begin Source File

SOURCE=.\EC_HelpStep03Task.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_HelpStep03Task.h
# End Source File
# Begin Source File

SOURCE=.\EC_HelpStep04Trace.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_HelpStep04Trace.h
# End Source File
# Begin Source File

SOURCE=.\EC_HelpStep05Attack.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_HelpStep05Attack.h
# End Source File
# Begin Source File

SOURCE=.\EC_HelpStep06Medic.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_HelpStep06Medic.h
# End Source File
# Begin Source File

SOURCE=.\EC_HelpStep07Talisman.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_HelpStep07Talisman.h
# End Source File
# Begin Source File

SOURCE=.\EC_HelpStep08Map.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_HelpStep08Map.h
# End Source File
# Begin Source File

SOURCE=.\EC_HelpStep09Skill.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_HelpStep09Skill.h
# End Source File
# Begin Source File

SOURCE=.\EC_HelpStep10Transfer.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_HelpStep10Transfer.h
# End Source File
# Begin Source File

SOURCE=.\EC_HelpStep11TaskZhuxian.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_HelpStep11TaskZhuxian.h
# End Source File
# End Group
# Begin Group "Defence"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\defence\APILoader.cpp
# End Source File
# Begin Source File

SOURCE=.\defence\APILoader.h
# End Source File
# Begin Source File

SOURCE=.\defence\cheaterkiller.cpp
# End Source File
# Begin Source File

SOURCE=.\defence\cheaterkiller.h
# End Source File
# Begin Source File

SOURCE=.\defence\defence.cpp
# End Source File
# Begin Source File

SOURCE=.\defence\defence.h
# End Source File
# Begin Source File

SOURCE=.\defence\EnumWin32Pro.cpp
# End Source File
# Begin Source File

SOURCE=.\defence\EnumWin32Pro.h
# End Source File
# Begin Source File

SOURCE=.\defence\hardinfo.h
# End Source File
# Begin Source File

SOURCE=.\defence\imagechecker.cpp
# End Source File
# Begin Source File

SOURCE=.\defence\imagechecker.h
# End Source File
# Begin Source File

SOURCE=.\defence\infocollection.h
# End Source File
# Begin Source File

SOURCE=.\defence\Mouse.cpp
# End Source File
# Begin Source File

SOURCE=.\defence\Mouse.h
# End Source File
# Begin Source File

SOURCE=.\defence\Process.cpp
# End Source File
# Begin Source File

SOURCE=.\defence\Process.h
# End Source File
# Begin Source File

SOURCE=.\defence\replaceapi.cpp
# End Source File
# Begin Source File

SOURCE=.\defence\replaceapi.h
# End Source File
# Begin Source File

SOURCE=.\defence\socket_helper.cpp
# End Source File
# Begin Source File

SOURCE=.\defence\stackchecker.cpp
# End Source File
# Begin Source File

SOURCE=.\defence\stackchecker.h
# End Source File
# Begin Source File

SOURCE=.\defence\stackinfocollection.cpp
# End Source File
# Begin Source File

SOURCE=.\defence\stackinfocollection.h
# End Source File
# Begin Source File

SOURCE=.\defence\tracer.cpp
# End Source File
# End Group
# Begin Group "Lua"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ConfigFromLua.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigFromLua.h
# End Source File
# Begin Source File

SOURCE=.\EC_AutoHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_AutoHelper.h
# End Source File
# Begin Source File

SOURCE=.\EC_Helper.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Helper.h
# End Source File
# Begin Source File

SOURCE=.\EC_Menology.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Menology.h
# End Source File
# Begin Source File

SOURCE=.\EC_Secretary.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Secretary.h
# End Source File
# Begin Source File

SOURCE=.\LuaAchievementAPI.cpp
# End Source File
# Begin Source File

SOURCE=.\LuaAchievementEnv.cpp
# End Source File
# Begin Source File

SOURCE=.\LuaApiEnv.cpp
# End Source File
# Begin Source File

SOURCE=.\LuaApiEnv.h
# End Source File
# Begin Source File

SOURCE=.\LuaBind.cpp
# End Source File
# Begin Source File

SOURCE=.\LuaBind.h
# End Source File
# Begin Source File

SOURCE=.\LuaEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\LuaEvent.h
# End Source File
# Begin Source File

SOURCE=.\LuaFuncCommon.cpp
# End Source File
# Begin Source File

SOURCE=.\LuaFuncCommon.h
# End Source File
# Begin Source File

SOURCE=.\LuaGameAPI.cpp
# End Source File
# Begin Source File

SOURCE=.\LuaGameAPI.h
# End Source File
# Begin Source File

SOURCE=.\LuaRankDisp.cpp
# End Source File
# Begin Source File

SOURCE=.\LuaRankDisp.h
# End Source File
# Begin Source File

SOURCE=.\LuaTriggerTask.cpp
# End Source File
# Begin Source File

SOURCE=.\LuaTriggerTask.h
# End Source File
# Begin Source File

SOURCE=.\TowerDefencePF.cpp
# End Source File
# Begin Source File

SOURCE=.\TowerDefencePF.h
# End Source File
# End Group
# Begin Group "AutoMove"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\terrain.cpp
# End Source File
# Begin Source File

SOURCE=.\terrain.h
# End Source File
# Begin Source File

SOURCE=.\TransferMap.cpp
# End Source File
# Begin Source File

SOURCE=.\TransferMap.h
# End Source File
# End Group
# Begin Group "Anm"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AnmObjLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmObjLoader.h
# End Source File
# Begin Source File

SOURCE=.\AnmSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmSystem.h
# End Source File
# Begin Source File

SOURCE=.\AnmUtility.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmUtility.h
# End Source File
# End Group
# End Target
# End Project
