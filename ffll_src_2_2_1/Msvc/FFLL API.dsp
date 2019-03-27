# Microsoft Developer Studio Project File - Name="ffllapi" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ffllapi - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FFLL API.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FFLL API.mak" CFG="ffllapi - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ffllapi - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ffllapi - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ffllapi - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\bin"
# PROP Intermediate_Dir "..\..\intermediate files\ffllapi\release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SPARKDLL_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W4 /GR /GX /Zi /O2 /Oy- /D "NDEBUG" /D "_STATIC_LIB" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 /nologo /dll /pdb:"c:\bin/SparkBase.pdb" /map /debug /debugtype:both /machine:I386 /out:"..\..\bin\ffllapi.dll"
# SUBTRACT LINK32 /pdb:none /incremental:yes

!ELSEIF  "$(CFG)" == "ffllapi - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\bin"
# PROP Intermediate_Dir "..\..\intermediate files\ffllapi\debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SPARKDLL_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /Zi /Od /D "_DEBUG" /D "_STATIC_LIB" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /dll /profile /map /debug /debugtype:both /machine:I386 /out:"..\..\bin\ffllapiD.dll"

!ENDIF 

# Begin Target

# Name "ffllapi - Win32 Release"
# Name "ffllapi - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\COGDefuzzSetObj.cpp
# End Source File
# Begin Source File

SOURCE=..\COGDefuzzVarObj.cpp
# End Source File
# Begin Source File

SOURCE=..\DefuzzSetObj.cpp
# End Source File
# Begin Source File

SOURCE=..\DefuzzVarObj.cpp
# End Source File
# Begin Source File

SOURCE=..\FFLLAPI.cpp
# End Source File
# Begin Source File

SOURCE=..\FFLLAPI.def
# End Source File
# Begin Source File

SOURCE=..\FFLLBase.cpp
# End Source File
# Begin Source File

SOURCE=..\FuzzyModelBase.cpp
# End Source File
# Begin Source File

SOURCE=..\FuzzyOutSet.cpp
# End Source File
# Begin Source File

SOURCE=..\FuzzyOutVariable.cpp
# End Source File
# Begin Source File

SOURCE=..\FuzzySetBase.cpp
# End Source File
# Begin Source File

SOURCE=..\FuzzyVariableBase.cpp
# End Source File
# Begin Source File

SOURCE=..\MemberFuncBase.cpp
# End Source File
# Begin Source File

SOURCE=..\MemberFuncSCurve.cpp
# End Source File
# Begin Source File

SOURCE=..\MemberFuncSingle.cpp
# End Source File
# Begin Source File

SOURCE=..\MemberFuncTrap.cpp
# End Source File
# Begin Source File

SOURCE=..\MemberFuncTri.cpp
# End Source File
# Begin Source File

SOURCE=..\MOMDefuzzSetObj.cpp
# End Source File
# Begin Source File

SOURCE=..\MOMDefuzzVarObj.cpp
# End Source File
# Begin Source File

SOURCE=..\RuleArray.cpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\FFLLAPI.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\COGDefuzzSetObj.h
# End Source File
# Begin Source File

SOURCE=..\COGDefuzzVarObj.h
# End Source File
# Begin Source File

SOURCE=..\DefuzzSetObj.h
# End Source File
# Begin Source File

SOURCE=..\DefuzzVarObj.h
# End Source File
# Begin Source File

SOURCE=..\FFLLAPI.h
# End Source File
# Begin Source File

SOURCE=..\FFLLBase.h
# End Source File
# Begin Source File

SOURCE=..\FuzzyModelBase.h
# End Source File
# Begin Source File

SOURCE=..\FuzzyOutSet.h
# End Source File
# Begin Source File

SOURCE=..\FuzzyOutVariable.h
# End Source File
# Begin Source File

SOURCE=..\FuzzySetBase.h
# End Source File
# Begin Source File

SOURCE=..\FuzzyVariableBase.h
# End Source File
# Begin Source File

SOURCE=..\MemberFuncBase.h
# End Source File
# Begin Source File

SOURCE=..\MemberFuncSCurve.h
# End Source File
# Begin Source File

SOURCE=..\MemberFuncSingle.h
# End Source File
# Begin Source File

SOURCE=..\MemberFuncTrap.h
# End Source File
# Begin Source File

SOURCE=..\MemberFuncTri.h
# End Source File
# Begin Source File

SOURCE=..\MOMDefuzzSetObj.h
# End Source File
# Begin Source File

SOURCE=..\MOMDefuzzVarObj.h
# End Source File
# Begin Source File

SOURCE=..\RuleArray.h
# End Source File
# End Group
# End Target
# End Project
