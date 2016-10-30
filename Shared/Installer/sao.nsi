; SAO Install script for Windows, using NSIS
; Author(s):       iFarbod <ifarbod@outlook.com>
;
; Copyright (c) 2015-2016 The San Andreas Online Open Source Project
;
; Distributed under the MIT license (See accompanying file LICENSE or copy at
; https://opensource.org/licenses/MIT)

; ----------------------
; Headers
; ----------------------
!include "MUI2.nsh"
!include "Sections.nsh"
!include "LogicLib.nsh"
!include "Memento.nsh"
!include "WordFunc.nsh"
!include "Version.nsh"

; ----------------------
; Definitions
; ----------------------
!define NAME "San Andreas Online"
!define NAME2 "SAO"
!define NAME3 "SA:Online"
!define PUBLISHER "The ${NAME} Team"
!define WEBSITE_URL "http://sanandreasonline.com"
!define GITHUB_URL "https://github.com/sanandreasonline/sao"
!define REGKEY "SOFTWARE\${NAME}"
!define UNINST_KEY "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}"
!define UNINST_ROOT_KEY "HKLM"

!ifdef VERSION_PRE_RELEASE_TYPE
    !ifdef VERSION_PRE_RELEASE
        !define VERSION "v${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}-${VERSION_PRE_RELEASE_TYPE}.${VERSION_PRE_RELEASE}"
        !define VER_NOV "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}-${VERSION_PRE_RELEASE_TYPE}.${VERSION_PRE_RELEASE}"
    !else
        !define VERSION "v${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}-${VERSION_PRE_RELEASE_TYPE}"
        !define VER_NOV "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}-${VERSION_PRE_RELEASE_TYPE}"
    !endif
!else
    !define VERSION "v${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
    !define VER_NOV "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
!endif

!define OUTFILE "${NAME2}-${VERSION}-Setup.exe"
!define FILES_ROOT "../../InstallFiles"
!define SERVER_FILES_ROOT "${FILES_ROOT}/Server"

!define MEMENTO_REGISTRY_ROOT ${UNINST_ROOT_KEY}
!define MEMENTO_REGISTRY_KEY "${UNINST_KEY}"

; ----------------------
; General
; ----------------------
Name "${NAME}"
OutFile "${OUTFILE}"
Unicode true
SetCompressor /SOLID lzma
BrandingText "${NAME} ${VERSION}"
Caption "${NAME} ${VERSION} Setup"

InstType "Server and client"
InstType "Client only"

InstallDir "$PROGRAMFILES\${NAME}"
InstallDirRegKey HKLM "${REGKEY}" "InstallLocation"

RequestExecutionLevel admin

!macro CHECK_ADMIN
UserInfo::GetAccountType
pop $0
${If} $0 != "admin"
    MessageBox MB_ICONSTOP "Please run this setup using administrator rights."
    SetErrorLevel 740
    Quit
${EndIf}
!macroend

;--------------------------------
; Installer's VersionInfo
;--------------------------------

VIProductVersion                   "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.0"
VIAddVersionKey "CompanyName"      "${PUBLISHER}"
VIAddVersionKey "ProductName"      "${NAME}"
VIAddVersionKey "ProductVersion"   "${VERSION}"
VIAddVersionKey "FileDescription"  "${NAME} Setup/Uninstaller"
VIAddVersionKey "FileVersion"      "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.0"
VIAddVersionKey "LegalCopyright"   "Copyright (c) 2015-${YEAR} The San Andreas Online Team"
VIAddVersionKey "OriginalFilename" "${NAME2}_${VERSION}-Setup.exe"

; ----------------------
; Interface Settings
; ----------------------

!define MUI_ICON "res\sao_installer.ico"
!define MUI_UNICON "res\sao_uninstaller.ico"
!define MUI_ABORTWARNING
!define MUI_COMPONENTSPAGE_SMALLDESC
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "res\header.bmp"
!define MUI_HEADERIMAGE_BITMAP_RTL "res\header-r.bmp"
!define MUI_HEADERIMAGE_UNBITMAP "res\header-uninstall.bmp"
!define MUI_HEADERIMAGE_UNBITMAP_RTL "res\header-uninstall-r.bmp"
!define MUI_WELCOMEFINISHPAGE_BITMAP "res\welcomefinish.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "res\welcomefinish-uninstall.bmp"
!define SHACF_FILESYSTEM 1

; ----------------------
; Pages
; ----------------------

!define MUI_WELCOMEPAGE_TITLE_3LINES ; Extra space for the title area.
!define MUI_WELCOMEPAGE_TITLE "Welcome to the ${NAME} ${VERSION} Setup Wizard"
!define MUI_WELCOMEPAGE_TEXT "$(INST_WELCOMEPAGE_TEXT)"

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "..\..\LICENSE.md"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
Page custom PageGameDirectory PageLeaveGameDirectory
!define MUI_PAGE_CUSTOMFUNCTION_LEAVE PostInstPage
!insertmacro MUI_PAGE_INSTFILES

!define MUI_FINISHPAGE_LINK "Visit the ${NAME}'s wiki for the latest news, FAQs and support"
!define MUI_FINISHPAGE_LINK_LOCATION "${GITHUB_URL}/wiki"

!define MUI_FINISHPAGE_RUN "$INSTDIR\${NAME2}.exe"
!define MUI_FINISHPAGE_RUN_TEXT "&Start ${NAME} now"
!define MUI_FINISHPAGE_NOREBOOTSUPPORT

!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

; ----------------------
; Languages
; ----------------------
!insertmacro MUI_LANGUAGE "English"

; ----------------------
; Installer sections
; ----------------------

${MementoSection} "Client core files (required)" SecCore
    SectionIn 1 2 RO
    
    SetOutPath $INSTDIR
    File "${FILES_ROOT}\SAO.exe"
    
    SetOutPath "$INSTDIR\${NAME2}"
    File "${FILES_ROOT}\${NAME2}\bass.dll"
    File "${FILES_ROOT}\${NAME2}\sde.dll"
    File "${FILES_ROOT}\${NAME2}\sdo.dll"
    File "${FILES_ROOT}\${NAME2}\sdv.dll"
    File "${FILES_ROOT}\${NAME2}\sdvf.dll"
    File "${FILES_ROOT}\${NAME2}\Core.dll"
    
    ; Create uninstaller
    WriteUninstaller "$INSTDIR\Uninstall.exe"
${MementoSectionEnd}

SectionGroup "Server" SecServer

${MementoSection} "Test" SecTest
    SectionIn 1
${MementoSectionEnd}

${MementoSectionDone}

SectionGroupEnd

; ----------------------
; Descriptions
; ----------------------
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecCore} "The core files required to use ${NAME}"
!insertmacro MUI_FUNCTION_DESCRIPTION_END

; ----------------------
; Installer sections
; ----------------------
Function .onInit
    !insertmacro CHECK_ADMIN
    ${MementoSectionRestore}
FunctionEnd

Function .onInstSuccess
    ${MementoSectionSave}
    
    WriteRegStr HKLM "${REGKEY}" "InstallLocation" $INSTDIR
FunctionEnd

Function un.onInit
    !insertmacro CHECK_ADMIN
FunctionEnd

; ----------------------
; Language strings
; ----------------------
LangString INST_WELCOMEPAGE_TEXT ${LANG_ENGLISH} "This wizard will guide you through the installation of ${NAME}, a modification for Grand Theft Auto: San Andreas which allows you to play over Internet or LAN.$\r$\n$\r$\n${NAME} is highly customizable, ranging from custom maps, weapons, vehicles to Lua scripts that change the entire game logic.$\r$\n$\r$\n$_CLICK"
LangString INST_GAMEDIRPAGE_HEADER_TEXT ${LANG_ENGLISH} "Game directory"
LangString INST_GAMEDIRPAGE_HEADER_SUBTITLE_TEXT ${LANG_ENGLISH} "Where is your game located?"
LangString INST_GAMEDIRPAGE_INSTRUCTIONS ${LANG_ENGLISH} "Locate the directory in which Grand Theft Auto: San Andreas is installed.$\r$\nYou need GTA:SA v1.00 in order to play San Andreas Online.$\r$\nAny other versions will not work and may need to be downgraded.$\r$\nNOTE: Please use a clean install."
LangString INST_GAMEDIRPAGE_GROUPBOX_TEXT ${LANG_ENGLISH} "Game Install Folder"
LangString INST_GAMEDIRPAGE_BROWSEBTN_TEXT ${LANG_ENGLISH} "B&rowse..."

Var GameDirPage_Dialog
Var GameDirPage_lblInstructions
Var GameDirPage_gbDirSelect
Var GameDirPage_drText
Var GameDirPage_drButton

Var GTA_DIR

Function PageGameDirectory
    !insertmacro MUI_HEADER_TEXT "$(INST_GAMEDIRPAGE_HEADER_TEXT)" "$(INST_GAMEDIRPAGE_HEADER_SUBTITLE_TEXT)"
    
    ; Attempt to grab SA installation path from registry
    ; Try to get the path from a previous SAO installation
    ReadRegStr $0 HKLM "${REGKEY}" "GTASAPath"
    ${If} $0 == ""
        ; Try Steam
        ReadRegStr $0 HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Steam App 12120" "InstallLocation"
    ${EndIf}
    ${If} $0 == ""
        ; Try for retail SA
        ReadRegStr $0 HKLM "SOFTWARE\Rockstar Games\GTA San Andreas\Installation" "ExePath"
    ${EndIf}
    
    ; Remove exe name from path
    ;!insertmacro ReplaceSubStr $0 "gta_sa.exe" ""
    ; Conform slash types
    ;!insertmacro ReplaceSubStr $MODIFIED_STR "/" "\"
    ; Remove quotes
    ;StrCpy $3 '"'
    ;!insertmacro ReplaceSubStr $MODIFIED_STR $3 ""
    ; Store result 
    ;StrCpy $GTA_DIR $MODIFIED_STR
    strcpy $GTA_DIR $0

    ; Default to standard path if nothing defined
    ${If} $GTA_DIR == "" 
        StrCpy $GTA_DIR "$PROGRAMFILES\Rockstar Games\GTA San Andreas\"
    ${EndIf}
    
    nsDialogs::Create 1018
    Pop $GameDirPage_Dialog
    ; Did we encounter an error?
    ${If} $GameDirPage_Dialog == error
        Abort
    ${EndIf}
    
    ${NSD_CreateLabel} 0u 0u 297.52u 58.46u "$(INST_GAMEDIRPAGE_INSTRUCTIONS)"
    Pop $GameDirPage_lblInstructions

    ${NSD_CreateGroupBox} 0u 60.31u 297.52u 33.85u "$(INST_GAMEDIRPAGE_GROUPBOX_TEXT)"
    Pop $GameDirPage_gbDirSelect

    ; DirRequest Text box
    ${NSD_CreateText} 7.9u 73.23u 233.01u 12.31u $GTA_DIR
    Pop $GameDirPage_drText
    System::Call shlwapi::SHAutoComplete(p$GameDirPage_drText,i${SHACF_FILESYSTEM})

    ; DirRequest Button
    ${NSD_CreateButton} 242.23u 72u 46.08u 15.38u "$(INST_GAMEDIRPAGE_BROWSEBTN_TEXT)"
    Pop $GameDirPage_drButton
    ${NSD_OnClick} $GameDirPage_drButton PageGameDirectoryDirRequestBtnClick
    
    nsDialogs::Show
FunctionEnd

Function PageLeaveGameDirectory

FunctionEnd

LangString INST_GAMEDIRPAGE_DIRSELECTDLG_TEXT ${LANG_ENGLISH} "Select your GTA:SA installation directory:"

Function PageGameDirectoryDirRequestBtnClick
	Pop $R0
	${If} $R0 == $GameDirPage_drButton
		${NSD_GetText} $GameDirPage_drText $R0
		nsDialogs::SelectFolderDialog /NOUNLOAD "$(INST_GAMEDIRPAGE_DIRSELECTDLG_TEXT)" "$R0"
		Pop $R0
		${If} "$R0" != "error"
			${NSD_SetText} $GameDirPage_drText "$R0"
		${EndIf}
	${EndIf}
FunctionEnd

Function PostInstPage
    ; Don't advance automatically if details expanded
    FindWindow $R0 "#32770" "" $HWNDPARENT
    GetDlgItem $R0 $R0 1016
    System::Call user32::IsWindowVisible(i$R0)i.s
    Pop $R0

    StrCmp $R0 0 +2
    SetAutoClose false
FunctionEnd

; ----------------------
; Uninstaller section
; ----------------------

Section "Uninstall"
    DeleteRegKey ${UNINST_ROOT_KEY} "${UNINST_KEY}"
    DeleteRegKey HKLM "${REGKEY}"
    Delete "$INSTDIR\uninstall.exe"
    RMDir $INSTDIR
SectionEnd
