# Microsoft Developer Studio Project File - Name="Knight" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 60000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Knight - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Knight.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Knight.mak" CFG="Knight - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Knight - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Knight - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "Knight"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Knight - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MLd /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ddraw.lib d3d8.lib d3dx8.lib d3dxof.lib dxguid.lib winmm.lib /nologo /subsystem:windows /machine:I386 /out:"..\SDK 2_0\Run\Knight.exe"

!ELSEIF  "$(CFG)" == "Knight - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ddraw.lib d3d8.lib d3dx8.lib d3dxof.lib dxguid.lib winmm.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\SDK 2_0\Run\Knight_D.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Knight - Win32 Release"
# Name "Knight - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Source\C3DCamera.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\C3DEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\C3DFont.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\C3DFrameObject.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\C3DMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\C3DObject.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\C3DText.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\C3DUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Glove.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\KeyboardControl.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\knight.cpp
# End Source File
# Begin Source File

SOURCE="..\SDK 2_0\P5Utils\P5Motion.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Source\C3DCamera.h
# End Source File
# Begin Source File

SOURCE=.\Source\C3DEngine.h
# End Source File
# Begin Source File

SOURCE=.\Source\C3DFont.h
# End Source File
# Begin Source File

SOURCE=.\Source\C3DFrameObject.h
# End Source File
# Begin Source File

SOURCE=.\Source\C3DMesh.h
# End Source File
# Begin Source File

SOURCE=.\Source\C3DObject.h
# End Source File
# Begin Source File

SOURCE=.\Source\C3DText.h
# End Source File
# Begin Source File

SOURCE=.\Source\C3DUtil.h
# End Source File
# Begin Source File

SOURCE=.\Source\Glove.h
# End Source File
# Begin Source File

SOURCE=.\Source\KeyboardControl.h
# End Source File
# Begin Source File

SOURCE="..\SDK 2_0\P5API\P5dll.h"
# End Source File
# Begin Source File

SOURCE="..\SDK 2_0\P5Utils\P5Motion.h"
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Source\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\Source\Knight.rc
# End Source File
# End Group
# Begin Group "Physics Source"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\Source\C3DBody.cpp
# End Source File
# End Group
# Begin Group "Physics Headers"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=.\Source\C3DBody.h
# End Source File
# Begin Source File

SOURCE=.\Source\d3dx8math2.h
# End Source File
# Begin Source File

SOURCE=.\Source\d3dx8math2.inl
# End Source File
# End Group
# Begin Source File

SOURCE="..\SDK 2_0\P5API\P5DLL.lib"
# End Source File
# End Target
# End Project
