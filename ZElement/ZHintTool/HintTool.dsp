# Microsoft Developer Studio Project File - Name="HintTool" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=HintTool - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "HintTool.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "HintTool.mak" CFG="HintTool - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "HintTool - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "HintTool - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/ZElement/ZHintTool", SKFBAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "HintTool - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "..\ZElementClient\Network\rpcdata" /I "..\ZElementClient\Network\inl" /I "..\ZElementClient\Network\IOLib" /I "..\ZElementClient\Network" /I "..\include\stlport" /I "..\ZCommon" /I "..\include" /I "..\include\LuaWrapper" /I "..\include\CD" /I "..\ZElementClient" /I "..\ZElementClient\Task" /I "..\ZHintTool" /I "..\include\AUI" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "UNICODE" /D "_ELEMENTCLIENT" /D "_INVENTORY_DESC_MAN" /D "BACK_VERSION" /FR /YX /FD /Zm200 /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ElementSkill.lib AngelicaCommon_u.lib AngelicaFile_u.lib ws2_32.lib zliblib.lib kernel32.lib user32.lib lua5.1.mt.lib LuaWrapper_u.lib AUInterface.lib Advapi32.lib /nologo /subsystem:console /incremental:yes /map /debug /machine:I386 /out:"..\ZBin\ZhuXian_HintTool.exe" /libpath:"..\Lib\stlport" /libpath:"..\lib\CD" /libpath:"..\Lib" /libpath:"..\lib\AUI"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "HintTool - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\ZElementClient\Network\rpcdata" /I "..\ZElementClient\Network\inl" /I "..\ZElementClient\Network\IOLib" /I "..\ZElementClient\Network" /I "..\include\stlport" /I "..\ZCommon" /I "..\include" /I "..\include\LuaWrapper" /I "..\include\CD" /I "..\ZElementClient" /I "..\ZElementClient\Task" /I "..\ZHintTool" /I "..\include\AUI" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "UNICODE" /D "_ELEMENTCLIENT" /D "_INVENTORY_DESC_MAN" /D "BACK_VERSION" /FR /YX /FD /GZ /Zm200 /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ElementSkill_d.lib AngelicaCommon_ud.lib AngelicaFile_ud.lib ws2_32.lib zliblib.lib kernel32.lib user32.lib lua5.1.mtd.lib LuaWrapper_du.lib AUInterface_d.lib dbghelp.lib shlwapi.lib Advapi32.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"MSVCRT" /out:".\ElementHintMan\ZhuXian_HintTool_d.exe" /pdbtype:sept /libpath:"..\Lib\stlport" /libpath:"..\lib\CD" /libpath:"..\Lib" /libpath:"..\lib\AUI"

!ENDIF 

# Begin Target

# Name "HintTool - Win32 Release"
# Name "HintTool - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\HintTool.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Inventory"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ZElementClient\EC_DealInventory.cpp
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\EC_DealInventory.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\EC_Inventory.cpp
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\EC_Inventory.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\EC_IvtrConsume.cpp
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\EC_IvtrConsume.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\EC_IvtrDestroyingEssence.cpp
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\EC_IvtrDestroyingEssence.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\EC_IvtrEquip.cpp
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\EC_IvtrEquip.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\EC_IvtrHTItem.cpp
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\EC_IvtrHTItem.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\EC_IvtrItem.cpp
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\EC_IvtrItem.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\EC_IvtrMaterial.cpp
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\EC_IvtrMaterial.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\EC_IvtrRune.cpp
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\EC_IvtrRune.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\EC_IvtrScroll.cpp
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\EC_IvtrScroll.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\EC_IvtrStone.cpp
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\EC_IvtrStone.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\EC_IvtrTalismanMain.cpp
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\EC_IvtrTalismanMain.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\EC_IvtrTaskItem.cpp
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\EC_IvtrTaskItem.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\EC_IvtrTypes.h
# End Source File
# End Group
# Begin Group "NetWork"

# PROP Default_Filter ""
# Begin Group "IOLib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ZElementClient\Network\IOLib\gnactiveio.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\Network\IOLib\gnbyteorder.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\Network\IOLib\gncompress.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\Network\IOLib\gnconf.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\Network\IOLib\gncoordinate.cpp
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\Network\IOLib\gncoordinate.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\Network\IOLib\gnerrno.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\Network\IOLib\gnmarshal.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\Network\IOLib\gnmppc.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\Network\IOLib\gnnetio.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\Network\IOLib\gnoctets.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\Network\IOLib\gnpassiveio.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\Network\IOLib\gnpollio.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\Network\IOLib\gnproto.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\Network\IOLib\gnrpc.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\Network\IOLib\gnsecure.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\Network\IOLib\gnstatic.cpp
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\Network\IOLib\gnthread.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\Network\IOLib\gntimer.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\Network\IOLib\iolib.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\Network\IOLib\streamcompress.h
# End Source File
# End Group
# Begin Group "RPCData"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\gconsigngsroleinfo
# End Source File
# Begin Source File

SOURCE=.\gconsignrole
# End Source File
# Begin Source File

SOURCE=.\gcreditinfo
# End Source File
# Begin Source File

SOURCE=.\gdeityinfo
# End Source File
# Begin Source File

SOURCE=.\gpetaddedinfo
# End Source File
# Begin Source File

SOURCE=.\gpetsinfo
# End Source File
# Begin Source File

SOURCE=.\gpocketinfo
# End Source File
# Begin Source File

SOURCE=.\gpocketinventory
# End Source File
# Begin Source File

SOURCE=.\grolebasicattribute
# End Source File
# Begin Source File

SOURCE=.\grolestorehouse
# End Source File
# Begin Source File

SOURCE=.\gskillinfo
# End Source File
# Begin Source File

SOURCE=.\gtitleinfo
# End Source File
# End Group
# Begin Source File

SOURCE=.\callid2.hpp
# End Source File
# Begin Source File

SOURCE=.\EC_GameRecord_HintTool.h
# End Source File
# Begin Source File

SOURCE=.\EC_GameSession_HintTool.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_GameSession_HintTool.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\Network\EC_GPDataType.cpp
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\Network\EC_GPDataType.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\Network\gameclient.cpp
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\Network\gameclient.h
# End Source File
# Begin Source File

SOURCE=.\GetItemDesc.hpp
# End Source File
# Begin Source File

SOURCE=.\GetItemDescForPlatform.hpp
# End Source File
# Begin Source File

SOURCE=.\GetItemDescForPlatformRe.hpp
# End Source File
# Begin Source File

SOURCE=.\GetItemDescResponse.hpp
# End Source File
# Begin Source File

SOURCE=.\GetRoleDesc.hpp
# End Source File
# Begin Source File

SOURCE=.\GetRoleDescRe.hpp
# End Source File
# Begin Source File

SOURCE=.\GetRoleEquip.hpp
# End Source File
# Begin Source File

SOURCE=.\GetRoleEquipRe.hpp
# End Source File
# Begin Source File

SOURCE=.\GetSNSRolePet.hpp
# End Source File
# Begin Source File

SOURCE=.\GetSNSRolePetRe.hpp
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\Network\gnet.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\Network\netclient.h
# End Source File
# Begin Source File

SOURCE=.\NewKeepAlive.hpp
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\Network\state.cxx
# End Source File
# Begin Source File

SOURCE=.\state2.cpp
# End Source File
# Begin Source File

SOURCE=.\state2.hpp
# End Source File
# Begin Source File

SOURCE=.\stubs2.cxx
# End Source File
# Begin Source File

SOURCE=.\ToolAnnounceAid.hpp
# End Source File
# End Group
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ZElementClient\AMiniDump.cpp
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\AMiniDump.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\ConfigFromLua.cpp
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\ConfigFromLua.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EC_MD5Hash.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EC_MD5Hash.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EL_Precinct.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\EL_Precinct.h
# End Source File
# Begin Source File

SOURCE=..\ZCommon\elementdataman.cpp
# End Source File
# Begin Source File

SOURCE=..\ZCommon\elementdataman.h
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
# Begin Group "Main"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ZElementClient\DataPathMan.cpp
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\DataPathMan.h
# End Source File
# Begin Source File

SOURCE=.\EC_Faction_HintTool.h
# End Source File
# Begin Source File

SOURCE=.\EC_Game_HintTool.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Game_HintTool.h
# End Source File
# Begin Source File

SOURCE=.\EC_GameRun_HintTool.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_GameRun_HintTool.h
# End Source File
# Begin Source File

SOURCE=.\EC_GameUIMan_HintTool.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_GameUIMan_HintTool.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\EC_Global.cpp
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\EC_Global.h
# End Source File
# Begin Source File

SOURCE=.\EC_HostPlayer_HintTool.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_HostPlayer_HintTool.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\EC_Instance.cpp
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\EC_Instance.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\EC_PetCorral.cpp
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\EC_PetCorral.h
# End Source File
# Begin Source File

SOURCE=.\EC_Player_HintTool.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Player_HintTool.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\EC_Skill.cpp
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\EC_Skill.h
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\EC_StringTab.cpp
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\EC_StringTab.h
# End Source File
# Begin Source File

SOURCE=.\EC_UIManager_HintTool.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_UIManager_HintTool.h
# End Source File
# End Group
# Begin Group "Utilities"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ZElementClient\base64.cpp
# End Source File
# Begin Source File

SOURCE=..\ZElementClient\base64.h
# End Source File
# Begin Source File

SOURCE=.\EC_Utility_HintTool.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Utility_HintTool.h
# End Source File
# End Group
# Begin Group "AUIInterface"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ZAUInterface\AUI.cpp
# End Source File
# Begin Source File

SOURCE=..\ZAUInterface\AUI.h
# End Source File
# Begin Source File

SOURCE=.\AUICommon_HintTool.h
# End Source File
# Begin Source File

SOURCE=..\ZAUInterface\AUICTranslate.cpp
# End Source File
# Begin Source File

SOURCE=..\ZAUInterface\AUICTranslate.h
# End Source File
# Begin Source File

SOURCE=..\ZAUInterface\AXMLFile.cpp
# End Source File
# Begin Source File

SOURCE=..\ZAUInterface\AXMLFile.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
