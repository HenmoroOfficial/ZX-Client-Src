# Microsoft Developer Studio Project File - Name="ZAnimationEditor" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ZAnimationEditor - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ZAnimationEditor.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ZAnimationEditor.mak" CFG="ZAnimationEditor - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ZAnimationEditor - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ZAnimationEditor - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/ZElement/ZAnimationEditor", HWCCAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ZAnimationEditor - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GR /GX /Zi /O2 /I ".\TinyXML" /I ".\ClientForEditor" /I ".\ClientForEditor\ShellSDK" /I "..\ZCommon" /I "..\ZCommon\AUTOCA" /I "..\AnimationCore" /I "..\AnimationClient" /I "..\include\AUI" /I "..\include\CD" /I "..\ZElementClient\Network" /I "..\ZElementClient\Network\iolib" /I "..\ZElementClient\Network\rpcdata" /I "..\ZElementClient\Network\inl" /I "..\..\ZElement\include" /I "..\Include\stlport" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "UNICODE" /D "_AFXDLL" /D "_ELEMENTCLIENT" /D "BACK_VERSION" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 AngelicaCommon_u.lib AngelicaMedia_u.lib AngelicaFile_u.lib Angelica3D_u.lib AnimationCore.lib CHBasedCD.lib GfxCommon.lib dxguid.lib d3d8.lib d3dx8.lib dsound.lib shlwapi.lib zliblib.lib lua5.1.mt.lib LuaWrapper_u.lib FTDriver.lib Immwrapper.lib mpg123lib.lib vorbisfile_static.lib vorbis_static.lib ogg_static.lib speedtreert.lib libjpeg.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /out:"../ZBin/ZAnimationEditor.exe" /libpath:"..\lib\AnimationClient" /libpath:"..\lib\AnimationCore" /libpath:"..\lib" /libpath:"..\lib\CD" /libpath:"..\lib\stlport" /libpath:"..\lib\AUI"

!ELSEIF  "$(CFG)" == "ZAnimationEditor - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ZAnimationEditor___Win32_Debug"
# PROP BASE Intermediate_Dir "ZAnimationEditor___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "ZAnimationEditor___Win32_Debug"
# PROP Intermediate_Dir "ZAnimationEditor___Win32_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I ".\TinyXML" /I ".\ClientForEditor" /I ".\ClientForEditor\ShellSDK" /I "..\ZCommon" /I "..\ZCommon\AUTOCA" /I "..\AnimationCore" /I "..\AnimationClient" /I "..\include\AUI" /I "..\include\CD" /I "..\ZElementClient\Network" /I "..\ZElementClient\Network\iolib" /I "..\ZElementClient\Network\rpcdata" /I "..\ZElementClient\Network\inl" /I "..\..\ZElement\include" /I "..\Include\stlport" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "UNICODE" /D "_AFXDLL" /D "_ELEMENTCLIENT" /D "BACK_VERSION" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 AngelicaCommon_ud.lib AngelicaMedia_ud.lib AngelicaFile_ud.lib Angelica3D_ud.lib AnimationCore_d.lib CHBasedCD_d.lib GfxCommon_d.lib dxguid.lib d3d8.lib d3dx8.lib dsound.lib shlwapi.lib zliblib.lib lua5.1.mtd.lib LuaWrapper_du.lib FTDriver_D.lib Immwrapper_d.lib mpg123lib_d.lib vorbisfile_static_d.lib vorbis_static_d.lib ogg_static_d.lib speedtreert_d.lib libjpeg.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /nodefaultlib:"MSVCRT.lib" /out:"../ZBin/ZAnimationEditor_d.exe" /pdbtype:sept /libpath:"..\lib\AnimationClient" /libpath:"..\lib\AnimationCore" /libpath:"..\lib" /libpath:"..\lib\CD" /libpath:"..\lib\stlport" /libpath:"..\lib\AUI"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "ZAnimationEditor - Win32 Release"
# Name "ZAnimationEditor - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ActionBezierMoveDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ActionBezierPointDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ActionCameraBindDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ActionCameraFilterDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ActionCameraOscillatory.cpp
# End Source File
# Begin Source File

SOURCE=.\ActionDialogueDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ActionDisplayHideDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ActionFOVDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ActionHideWorldDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ActionHookDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ActionJumpDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ActionMotionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ActionMoveDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ActionRotateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ActionScaleDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ActionSoundDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ActionSpeedControlDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ActionTransparentDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AnimationEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\AnimationEditorView.cpp
# End Source File
# Begin Source File

SOURCE=..\AnimationCore\AnmCommonActionManager.cpp
# End Source File
# Begin Source File

SOURCE=.\BezierDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BezierList.cpp
# End Source File
# Begin Source File

SOURCE=.\BindCameraDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomActionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CWButton.cpp
# End Source File
# Begin Source File

SOURCE=.\CWControlBar.cpp
# End Source File
# Begin Source File

SOURCE=.\CWTabctrlBar.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MoveActionsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NewActionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NewObjectDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjectList.cpp
# End Source File
# Begin Source File

SOURCE=.\OperationRecorder.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SectPlayerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectedObjectDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetTimeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\TrackChannel.cpp
# End Source File
# Begin Source File

SOURCE=.\TrackChannelDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TrackGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\TrackGroupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TrackNode.cpp
# End Source File
# Begin Source File

SOURCE=.\TrackPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\TrackRuler.cpp
# End Source File
# Begin Source File

SOURCE=.\WorldDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ActionBezierMoveDlg.h
# End Source File
# Begin Source File

SOURCE=.\ActionBezierPointDlg.h
# End Source File
# Begin Source File

SOURCE=.\ActionCameraBindDlg.h
# End Source File
# Begin Source File

SOURCE=.\ActionCameraFilterDlg.h
# End Source File
# Begin Source File

SOURCE=.\ActionCameraOscillatory.h
# End Source File
# Begin Source File

SOURCE=.\ActionDialogueDlg.h
# End Source File
# Begin Source File

SOURCE=.\ActionDisplayHideDlg.h
# End Source File
# Begin Source File

SOURCE=.\ActionFOVDlg.h
# End Source File
# Begin Source File

SOURCE=.\ActionHideWorldDlg.h
# End Source File
# Begin Source File

SOURCE=.\ActionHookDlg.h
# End Source File
# Begin Source File

SOURCE=.\ActionJumpDlg.h
# End Source File
# Begin Source File

SOURCE=.\ActionMotionDlg.h
# End Source File
# Begin Source File

SOURCE=.\ActionMoveDlg.h
# End Source File
# Begin Source File

SOURCE=.\ActionRotateDlg.h
# End Source File
# Begin Source File

SOURCE=.\ActionScaleDlg.h
# End Source File
# Begin Source File

SOURCE=.\ActionSoundDlg.h
# End Source File
# Begin Source File

SOURCE=.\ActionSpeedControlDlg.h
# End Source File
# Begin Source File

SOURCE=.\ActionTransparentDlg.h
# End Source File
# Begin Source File

SOURCE=.\AnimationEditor.h
# End Source File
# Begin Source File

SOURCE=.\AnimationEditorView.h
# End Source File
# Begin Source File

SOURCE=.\BezierDlg.h
# End Source File
# Begin Source File

SOURCE=.\BezierList.h
# End Source File
# Begin Source File

SOURCE=.\BindCameraDlg.h
# End Source File
# Begin Source File

SOURCE=.\CustomActionDlg.h
# End Source File
# Begin Source File

SOURCE=.\CWButton.h
# End Source File
# Begin Source File

SOURCE=.\CWControlBar.h
# End Source File
# Begin Source File

SOURCE=.\CWTabCtrlBar.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MoveActionsDlg.h
# End Source File
# Begin Source File

SOURCE=.\NewActionDlg.h
# End Source File
# Begin Source File

SOURCE=.\NewObjectDlg.h
# End Source File
# Begin Source File

SOURCE=.\ObjectList.h
# End Source File
# Begin Source File

SOURCE=.\OperationRecorder.h
# End Source File
# Begin Source File

SOURCE=.\PlayerDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SectPlayerDlg.h
# End Source File
# Begin Source File

SOURCE=.\SelectedObjectDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetTimeDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TrackChannel.h
# End Source File
# Begin Source File

SOURCE=.\TrackChannelDlg.h
# End Source File
# Begin Source File

SOURCE=.\TrackGroup.h
# End Source File
# Begin Source File

SOURCE=.\TrackGroupDlg.h
# End Source File
# Begin Source File

SOURCE=.\TrackNode.h
# End Source File
# Begin Source File

SOURCE=.\TrackPanel.h
# End Source File
# Begin Source File

SOURCE=.\TrackRuler.h
# End Source File
# Begin Source File

SOURCE=.\WorldDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\AnimationEditor.ico
# End Source File
# Begin Source File

SOURCE=.\AnimationEditor.rc
# End Source File
# Begin Source File

SOURCE=.\res\AnimationEditor.rc2
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\imagelist.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Group "ZCommon"

# PROP Default_Filter ""
# Begin Group "ClientForEditor"

# PROP Default_Filter ""
# Begin Group "ShellSDK"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ClientForEditor\ShellSDK\EncStr.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\ShellSDK\EncStr.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ClientForEditor\DataPathMan.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\DataPathMan.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_AssureMove.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_AssureMove.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_CameraCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_CameraCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_CDR.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_CDR.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_CDS.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_CDS.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_Clipboard.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_Clipboard.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_Configs.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_Configs.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_ConfigsData.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_ConfigsData.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_Counter.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_CritterGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_CritterGroup.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_Decal.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_Decal.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_ElsePlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_ElsePlayer.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_FixedMsg.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_FullGlowRender.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_FullGlowRender.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_Game.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_Game.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_GameRun.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_GameRun.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_GFXCaster.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_GFXCaster.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_Global.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_Global.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_GrassArea.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_GrassArea.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_HostMove.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_HostMove.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_HostPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_HostPlayer.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_ImageRes.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_ImageRes.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_Instance.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_Instance.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_IvtrTypes.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_Manager.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_ManDecal.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_ManDecal.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_ManMatter.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_ManMatter.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_ManMessage.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_ManMessage.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_ManNPC.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_ManNPC.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_ManOrnament.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_ManOrnament.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_ManPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_ManPlayer.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_ManSkillGfx.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_ManSkillGfx.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_Matter.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_Matter.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_Monster.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_Monster.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_MsgDef.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_NPC.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_NPC.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_Object.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_Object.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_Ornament.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_Ornament.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_Pet.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_Pet.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_Player.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_Player.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_PortraitRender.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_PortraitRender.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_Resource.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_Resource.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_RoleTypes.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_RTDebug.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_RTDebug.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_Scene.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_Scene.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_SceneBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_SceneBlock.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_SceneCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_SceneCheck.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_SceneLights.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_SceneLights.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_SceneLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_SceneLoader.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_ScnBoxArea.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_ScnBoxArea.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_ScreenRender.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_ScreenRender.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_ShadowRender.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_ShadowRender.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_SimpleBloomRender.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_SimpleBloomRender.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_SkinCache.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_SkinCache.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_SoundCache.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_SoundCache.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_Sprite.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_Sprite.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_StringTab.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_StringTab.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_SunMoon.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_SunMoon.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_Utility.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_Utility.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_Viewport.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_Viewport.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_World.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\EC_World.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\LuaBind.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\LuaBind.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\LuaFuncCommon.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\LuaFuncCommon.h
# End Source File
# Begin Source File

SOURCE=.\MiniDump.cpp
# End Source File
# Begin Source File

SOURCE=.\MiniDump.h
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\rdbmp.c
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\terrain.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientForEditor\terrain.h
# End Source File
# End Group
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Group "AUTOCA"

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
# Begin Source File

SOURCE=..\ZCommon\EC_Bezier.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EC_Bezier.h
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

SOURCE=..\ZCommon\EC_WorldFile.h
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

SOURCE=..\ZCommon\EL_TerrainOutline.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EL_TerrainOutline.h
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
# Begin Source File

SOURCE=..\ZCommon\PolygonFunc.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\PolygonFunc.h
# End Source File
# End Group
# Begin Group "TinyXML"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TinyXML\tinyxml.cpp
# End Source File
# Begin Source File

SOURCE=.\TinyXML\tinyxml.h
# End Source File
# Begin Source File

SOURCE=.\TinyXML\tinyxmlerror.cpp
# End Source File
# Begin Source File

SOURCE=.\TinyXML\tinyxmlparser.cpp
# End Source File
# End Group
# End Group
# Begin Group "AnimationClient"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AnimationClient\AnmObjLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\AnimationClient\AnmObjLoader.h
# End Source File
# Begin Source File

SOURCE=.\AnimationClient\AnmSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\AnimationClient\AnmSystem.h
# End Source File
# Begin Source File

SOURCE=.\AnimationClient\AnmUtility.cpp
# End Source File
# Begin Source File

SOURCE=.\AnimationClient\AnmUtility.h
# End Source File
# End Group
# End Target
# End Project
