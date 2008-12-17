!ifndef version
  !define version "0.0"
!endif

!include "MUI2.nsh"

Name "OpenOrbiter ${version}"

OutFile "openorbiter-${version}-setup.exe"
InstallDir "$PROGRAMFILES\OpenOrbiter"
InstallDirRegKey HKCU "Software\OpenOrbiter" ""

SetCompressor /SOLID lzma

Var StartMenuFolder

!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\OpenOrbiter"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "StartMenuFolder"

!define MUI_LANGDLL_ALWAYSSHOW
!define MUI_LANGDLL_REGISTRY_ROOT "HKCU" 
!define MUI_LANGDLL_REGISTRY_KEY "Software\OpenOrbiter" 
!define MUI_LANGDLL_REGISTRY_VALUENAME "Installer Language"

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

!insertmacro MUI_RESERVEFILE_LANGDLL

Section "Install"
  CreateDirectory "$INSTDIR"
  DetailPrint "Installing new files"
  
  SetOutPath "$INSTDIR"
  file /oname=OpenOrbiter.exe release\bin\openorbiter.exe
  file ChangeLog.txt
  file ReadMe.txt
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
  WriteUninstaller "$INSTDIR\Uninstall.exe"
  
  WriteRegStr   HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\OpenOrbiter" "DisplayName" "OpenOrbiter"
  WriteRegStr   HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\OpenOrbiter" "DisplayVersion" "${version}"
  WriteRegStr   HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\OpenOrbiter" "HelpLink" "http://openorbiter.sf.net/"
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\OpenOrbiter" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\OpenOrbiter" "NoRepair" 1
  WriteRegStr   HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\OpenOrbiter" "Readme" "$INSTDIR\Readme.txt"
  WriteRegStr   HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\OpenOrbiter" "UninstallString" "$INSTDIR\Uninstall.exe"
  
  DetailPrint "Creating shortcuts"
  
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" "$INSTDIR\Uninstall.exe" "" "$INSTDIR\Uninstall.exe" 0
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\OpenOrbiter.lnk" "$INSTDIR\OpenOrbiter.exe" "" "$INSTDIR\OpenOrbiter.exe" 0
  !insertmacro MUI_STARTMENU_WRITE_END
  
  CreateShortCut "$DESKTOP\OpenOrbiter.lnk" "$INSTDIR\OpenOrbiter.exe" "" "$INSTDIR\OpenOrbiter.exe" 0
SectionEnd

Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

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

Function un.onInit
  !insertmacro MUI_UNGETLANGUAGE
FunctionEnd
