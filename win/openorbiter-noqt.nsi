!include "MUI.nsh"

Name "OpenOrbiter"

OutFile "installer-noqt.exe"
InstallDir "$PROGRAMFILES\OpenOrbiter"
InstallDirRegKey HKCU "Software\OpenOrbiter" ""

SetCompressor lzma


!define MUI_ABORTWARNING
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_RIGHT

!insertmacro MUI_PAGE_LICENSE "..\COPYING"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_LANGUAGE "English"


Section "Install"
  CreateDirectory "$INSTDIR"
  DetailPrint "Installing new files"
  
  SetOutPath "$INSTDIR"
  file /r release\bin\openorbiter.exe
  
  SetOutPath "$INSTDIR\maps"
  file /r ..\data\maps\*.xml
  
  SetOutPath "$INSTDIR\gfx"
  file /r ..\data\gfx\*.svg
  
  SetOutPath "$INSTDIR\translations"
  file /r ..\data\translations\*.qm
  
  DetailPrint "Rembering install location (for next time)"
  WriteRegStr HKCU "Software\OpenOrbiter" "" $INSTDIR
  DetailPrint "Creating Uninstaller"
  WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd


Section "Uninstall"
  Delete "$INSTDIR\Uninstall.exe"
  RMDir /r "$INSTDIR"
  DeleteRegKey /ifempty HKCU "Software\OpenOrbiter"
SectionEnd
