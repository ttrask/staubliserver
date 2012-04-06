# Microsoft Developer Studio Project File - Name="P5UtilBeta" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=P5UtilBeta - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "P5UtilBeta.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "P5UtilBeta.mak" CFG="P5UtilBeta - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "P5UtilBeta - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "P5UtilBeta - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "P5UtilBeta"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "P5UtilBeta - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"../Run/P5MouseUtil.exe"

!ELSEIF  "$(CFG)" == "P5UtilBeta - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"../Run/P5MouseUtil_D.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "P5UtilBeta - Win32 Release"
# Name "P5UtilBeta - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BendsPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ButtonsPage.cpp
# End Source File
# Begin Source File

SOURCE=.\LedButton.cpp
# End Source File
# Begin Source File

SOURCE=.\MouseExPage.cpp
# End Source File
# Begin Source File

SOURCE=.\MousePage.cpp
# End Source File
# Begin Source File

SOURCE=.\P5UtilBeta.cpp
# End Source File
# Begin Source File

SOURCE=.\P5UtilBeta.rc
# End Source File
# Begin Source File

SOURCE=.\P5UtilBetaDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\XyzPage.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BendsPage.h
# End Source File
# Begin Source File

SOURCE=.\ButtonsPage.h
# End Source File
# Begin Source File

SOURCE=.\LedButton.h
# End Source File
# Begin Source File

SOURCE=.\MouseExPage.h
# End Source File
# Begin Source File

SOURCE=.\MousePage.h
# End Source File
# Begin Source File

SOURCE=..\P5API\P5dll.h
# End Source File
# Begin Source File

SOURCE=.\P5UtilBeta.h
# End Source File
# Begin Source File

SOURCE=.\P5UtilBetaDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\XyzPage.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\dclick1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\dclick2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\dclick3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\led_red.bmp
# End Source File
# Begin Source File

SOURCE=.\res\logo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\P5UtilBeta.ico
# End Source File
# Begin Source File

SOURCE=.\res\P5UtilBeta.rc2
# End Source File
# Begin Source File

SOURCE=.\res\sclick1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sclick2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sclickLeft1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sclickLeft2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sclickMid1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sclickMid2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sclickRight1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sclickRight2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\testarea1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\testarea2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\testarea3.bmp
# End Source File
# End Group
# Begin Group "Libs"

# PROP Default_Filter "lib"
# Begin Source File

SOURCE=..\P5API\P5DLL.lib
# End Source File
# End Group
# Begin Source File

SOURCE=".\res\jack-in-the-box.avi"
# End Source File
# End Target
# End Project