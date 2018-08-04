# Microsoft Developer Studio Project File - Name="Football" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Football - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Football.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Football.mak" CFG="Football - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Football - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Football - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Football - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Football - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "header" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ddraw.lib dinput.lib dsound.lib dxguid.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Football - Win32 Release"
# Name "Football - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Source\Ball.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\DepthChart.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Football.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Formation.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\GameInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\MathUtils.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Play.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Playbook.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\PlayerInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\PlayerObj.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerObj_Actions.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerObj_States.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Roster.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Side.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\StrUtils.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Team.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\WinMain.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Header\Ball.h
# End Source File
# Begin Source File

SOURCE=.\Header\Constants.h
# End Source File
# Begin Source File

SOURCE=.\Header\DepthChart.h
# End Source File
# Begin Source File

SOURCE=.\Header\DownInfo.h
# End Source File
# Begin Source File

SOURCE=.\Header\Football.h
# End Source File
# Begin Source File

SOURCE=.\Header\Formation.h
# End Source File
# Begin Source File

SOURCE=.\Header\GameInfo.h
# End Source File
# Begin Source File

SOURCE=.\Header\MathUtils.h
# End Source File
# Begin Source File

SOURCE=.\Header\Play.h
# End Source File
# Begin Source File

SOURCE=.\Header\Playbook.h
# End Source File
# Begin Source File

SOURCE=.\Header\PlayerInfo.h
# End Source File
# Begin Source File

SOURCE=.\Header\PlayerObj.h
# End Source File
# Begin Source File

SOURCE=.\Header\Roster.h
# End Source File
# Begin Source File

SOURCE=.\Header\Side.h
# End Source File
# Begin Source File

SOURCE=.\Header\StrUtils.h
# End Source File
# Begin Source File

SOURCE=.\Header\Team.h
# End Source File
# Begin Source File

SOURCE=.\Header\WinMain.h
# End Source File
# End Group
# Begin Group "Stages"

# PROP Default_Filter ""
# Begin Group "Touchdown"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\TouchdownStage.cpp
# End Source File
# Begin Source File

SOURCE=.\Header\TouchdownStage.h
# End Source File
# End Group
# Begin Group "Title Screen"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\TitleScreenStage.cpp
# End Source File
# Begin Source File

SOURCE=.\Header\TitleScreenStage.h
# End Source File
# End Group
# Begin Group "Select Game Mode"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\SelectGameModeStage.cpp
# End Source File
# Begin Source File

SOURCE=.\Header\SelectGameModeStage.h
# End Source File
# End Group
# Begin Group "Select Team"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\SelectTeamStage.cpp
# End Source File
# Begin Source File

SOURCE=.\Header\SelectTeamStage.h
# End Source File
# End Group
# Begin Group "Coin Toss"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\CoinTossStage.cpp
# End Source File
# Begin Source File

SOURCE=.\Header\CoinTossStage.h
# End Source File
# End Group
# Begin Group "Select Play"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\SelectPlayStage.cpp
# End Source File
# Begin Source File

SOURCE=.\Header\SelectPlayStage.h
# End Source File
# End Group
# Begin Group "Play"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PlayStage.cpp
# End Source File
# Begin Source File

SOURCE=.\Header\PlayStage.h
# End Source File
# End Group
# Begin Group "First Down"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\FirstDownStage.cpp
# End Source File
# Begin Source File

SOURCE=.\Header\FirstDownStage.h
# End Source File
# End Group
# Begin Group "Turn Over"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\TurnOverStage.cpp
# End Source File
# Begin Source File

SOURCE=.\Header\TurnOverStage.h
# End Source File
# End Group
# Begin Group "Check Time"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\CheckTimeStage.cpp
# End Source File
# Begin Source File

SOURCE=.\Header\CheckTimeStage.h
# End Source File
# End Group
# Begin Group "Half Time"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\HalfTimeStage.cpp
# End Source File
# Begin Source File

SOURCE=.\Header\HalfTimeStage.h
# End Source File
# End Group
# Begin Group "End Game"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\EndGameStage.cpp
# End Source File
# Begin Source File

SOURCE=.\Header\EndGameStage.h
# End Source File
# End Group
# Begin Group "Begin Game"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\BeginGameStage.cpp
# End Source File
# Begin Source File

SOURCE=.\Header\BeginGameStage.h
# End Source File
# End Group
# Begin Group "Select Input"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\SelectInputStage.cpp
# End Source File
# Begin Source File

SOURCE=.\Header\SelectInputStage.h
# End Source File
# End Group
# End Group
# Begin Group "Gamelib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\Gamelib Source\Actions.cpp"
# End Source File
# Begin Source File

SOURCE=".\Gamelib Source\DirMusicMgr.cpp"
# End Source File
# Begin Source File

SOURCE=".\Gamelib Source\DisplayList.cpp"
# End Source File
# Begin Source File

SOURCE=".\Gamelib Source\Game.cpp"
# End Source File
# Begin Source File

SOURCE=".\Gamelib Source\GameFile.cpp"
# End Source File
# Begin Source File

SOURCE=.\Header\Gamelib.h
# End Source File
# Begin Source File

SOURCE=".\Gamelib Source\GameTimer.cpp"
# End Source File
# Begin Source File

SOURCE=".\Gamelib Source\Input.cpp"
# End Source File
# Begin Source File

SOURCE=".\Gamelib Source\Map.cpp"
# End Source File
# Begin Source File

SOURCE=".\Gamelib Source\MessageMgr.cpp"
# End Source File
# Begin Source File

SOURCE=".\Gamelib Source\Midi.cpp"
# End Source File
# Begin Source File

SOURCE=".\Gamelib Source\Objects.cpp"
# End Source File
# Begin Source File

SOURCE=".\Gamelib Source\Screen.cpp"
# End Source File
# Begin Source File

SOURCE=".\Gamelib Source\Sprite.cpp"
# End Source File
# Begin Source File

SOURCE=".\Gamelib Source\String.cpp"
# End Source File
# Begin Source File

SOURCE=".\Gamelib Source\Surface.cpp"
# End Source File
# Begin Source File

SOURCE=".\Gamelib Source\SurfacePixelFormat.cpp"
# End Source File
# Begin Source File

SOURCE=".\Gamelib Source\VPad.cpp"
# End Source File
# End Group
# End Target
# End Project
