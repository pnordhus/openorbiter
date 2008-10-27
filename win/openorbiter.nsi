!include "MUI.nsh"

Name "OpenOrbiter"

OutFile "installer.exe"
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
  
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\OpenOrbiter "DisplayName" "OpenOrbiter (remove only)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\OpenOrbiter" "UninstallString" "$INSTDIR\Uninstall.exe"
  
  DetailPrint "Creating shortcuts"
  CreateDirectory "$SMPROGRAMS\OpenOrbiter"
  CreateShortCut "$SMPROGRAMS\OpenOrbiter\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\OpenOrbiter\OpenOrbiter.lnk" "$INSTDIR\openorbiter.exe" "" "$INSTDIR\openorbiter.exe" 0
  CreateShortCut "$DESKTOP\OpenOrbiter.lnk" "$INSTDIR\openorbiter.exe" "" "$INSTDIR\openorbiter.exe" 0
SectionEnd


Section "Uninstall"
  Delete "$INSTDIR\Uninstall.exe"
  RMDir /r "$INSTDIR"
  DeleteRegKey /ifempty HKCU "Software\OpenOrbiter"
  DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\OpenOrbiter"
  
  Delete "$DESKTOP\OpenOrbiter.lnk"
  Delete "$SMPROGRAMS\OpenOrbiter\Uninstall.lnk"
  Delete "$SMPROGRAMS\OpenOrbiter\OpenOrbiter.lnk"
  RMDir "$SMPROGRAMS\OpenOrbiter"
SectionEnd
