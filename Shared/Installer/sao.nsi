; SAO Install script for Windows, using NSIS
; Author(s):       iFarbod <ifarbod@outlook.com>
;
; Copyright (c) 2015-2016 San Andreas Online
;
; Distributed under the MIT license (See accompanying file LICENSE or copy at
; https://opensource.org/licenses/MIT)

!ifndef OUTFILE
  !define OUTFILE "sao-${VERSION}-setup.exe"
!endif

Name "San Andreas Online"
OutFile "${OUTFILE}"
Unicode true
SetCompressor /SOLID lzma
BrandingText "San Andreas Online"
Caption "San Andreas Online ${VERSION}${NAMESUFFIX} Setup"

InstType "Full"
InstType "Lite"
InstType "Minimal"

InstallDir "$PROGRAMFILES\San Andreas Online"
InstallDirRegKey HKLM "Software\San Andreas Online" ""

RequestExecutionLevel admin

; ----------------------
; Headers
; ----------------------
!include "MUI2.nsh"
!include "Sections.nsh"
!include "LogicLib.nsh"
!include "Memento.nsh"
!include "WordFunc.nsh"

;--------------------------------
; Installer's VersionInfo
;--------------------------------

; VIProductVersion                   "${VERSION_FULL}"
; VIAddVersionKey "CompanyName"      "${PUBLISHER}"
; VIAddVersionKey "ProductName"      "FileZilla"
; VIAddVersionKey "ProductVersion"   "${VERSION}"
; VIAddVersionKey "FileDescription"  "FileZilla FTP Client"
; VIAddVersionKey "FileVersion"      "${VERSION}"
; VIAddVersionKey "LegalCopyright"   "${PUBLISHER}"
; VIAddVersionKey "OriginalFilename" "FileZilla_${VERSION}_win32-setup.exe"

; ----------------------
; Configuration
; ----------------------

!define REG_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\San Andreas Online"

; Memento Settings
!define MEMENTO_REGISTRY_ROOT HKLM
!define MEMENTO_REGISTRY_KEY "${REG_UNINST_KEY}"

; ----------------------
; Interface Settings
; ----------------------

!define MUI_ICON "sao_installer.ico"
!define MUI_UNICON "sao_uninstaller.ico"
!define MUI_ABORTWARNING
!define MUI_COMPONENTSPAGE_SMALLDESC
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "header.bmp"
!define MUI_HEADERIMAGE_BITMAP_RTL "header-r.bmp"
!define MUI_HEADERIMAGE_UNBITMAP "header-uninstall.bmp"
!define MUI_HEADERIMAGE_UNBITMAP_RTL "header-uninstall.bmp"
!define MUI_WELCOMEFINISHPAGE_BITMAP "welcomefinish.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "welcomefinish-uninstall.bmp"

; ----------------------
; Pages
; ----------------------
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "..\..\LICENSE.md"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES

!define MUI_FINISHPAGE_LINK "Visit the San Andreas Online's wiki for the latest news, FAQs and support"
!define MUI_FINISHPAGE_LINK_LOCATION "http://github.com/sanandreasonline/sao/wiki"

!define MUI_FINISHPAGE_RUN "$INSTDIR\SAO.exe"
!define MUI_FINISHPAGE_NOREBOOTSUPPORT

!define MUI_FINISHPAGE_SHOWREADME
!define MUI_FINISHPAGE_SHOWREADME_TEXT "Show release notes"
;!define MUI_FINISHPAGE_SHOWREADME_FUNCTION ShowReleaseNotes

!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

; ----------------------
; Languages
; ----------------------
!insertmacro MUI_LANGUAGE "English"

Section

;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd

Function ShowReleaseNotes
    ExecShell "" "http://nsis.sourceforge.net/Docs/AppendixF.html#F.1"
FunctionEnd

;--------------------------------
;Uninstaller Section

Section Uninstall

SectionEnd
