!ifndef version
  !define version "0.0"
!endif

!include "MUI.nsh"

Name "OpenOrbiter"

OutFile "openorbiter-${version}-setup.exe"
InstallDir "$PROGRAMFILES\OpenOrbiter"
InstallDirRegKey HKCU "Software\OpenOrbiter" ""

SetCompressor /SOLID lzma

Var StartMenuFolder

!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\OpenOrbiter"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "StartMenuFolder"

!define MUI_ABORTWARNING
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_RIGHT

!insertmacro MUI_PAGE_LICENSE "..\COPYING"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder 
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "German"

Section "Install"
  CreateDirectory "$INSTDIR"
  DetailPrint "Installing new files"
  
  SetOutPath "$INSTDIR"
  file /r release\bin\openorbiter.exe
  file /r libs\*.*
  
  SetOutPath "$INSTDIR\maps"
  file /r ..\data\maps\*.xml
  
  SetOutPath "$INSTDIR\gfx"
  file /r ..\data\gfx\*.svg
  
  SetOutPath "$INSTDIR\translations"
  file /r ..\data\translations\*.qm
  
  SetOutPath "$INSTDIR"
  
  DetailPrint "Rembering install location (for next time)"
  WriteRegStr HKCU "Software\OpenOrbiter" "" $INSTDIR
  DetailPrint "Creating Uninstaller"
  WriteUninstaller "$INSTDIR\uninstall.exe"
  
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\OpenOrbiter" "DisplayName" "OpenOrbiter"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\OpenOrbiter" "UninstallString" "$INSTDIR\Uninstall.exe"
  
  DetailPrint "Creating shortcuts"
  
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\OpenOrbiter.lnk" "$INSTDIR\openorbiter.exe" "" "$INSTDIR\openorbiter.exe" 0
  !insertmacro MUI_STARTMENU_WRITE_END
  
  CreateShortCut "$DESKTOP\OpenOrbiter.lnk" "$INSTDIR\openorbiter.exe" "" "$INSTDIR\openorbiter.exe" 0
SectionEnd


Section "Uninstall"
  Delete "$INSTDIR\Uninstall.exe"
  RMDir /r "$INSTDIR"
  DeleteRegKey /ifempty HKCU "Software\OpenOrbiter"
  DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\OpenOrbiter"
  
  Delete "$DESKTOP\OpenOrbiter.lnk"
  
  !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
  Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\OpenOrbiter.lnk"
  RMDir "$SMPROGRAMS\$StartMenuFolder"
SectionEnd
