Name "San Andreas Online"

AutoCloseWindow true

#!addplugindir nsis_plugins
#!include "nsis_includes/nsProcess.nsh"

# Use the modern ui
!include MUI.nsh
!define MUI_ICON sao_icon.ico

;Languages
#!insertmacro MUI_LANGUAGE "Farsi"
!insertmacro MUI_LANGUAGE "English"
#!insertmacro MUI_LANGUAGE "Czech"
#!insertmacro MUI_LANGUAGE "Dutch"
#!insertmacro MUI_LANGUAGE "French"
#!insertmacro MUI_LANGUAGE "German"
#!insertmacro MUI_LANGUAGE "Korean"
#!insertmacro MUI_LANGUAGE "Russian"
#!insertmacro MUI_LANGUAGE "Spanish"
#!insertmacro MUI_LANGUAGE "Swedish"
#!insertmacro MUI_LANGUAGE "TradChinese"
#!insertmacro MUI_LANGUAGE "SimpChinese"
#!insertmacro MUI_LANGUAGE "Slovak"

# define name of installer
OutFile "sao-installer.exe"

# define installation directory
InstallDir $APPDATA\San Andreas Online

# Request user permissions so that auto-updates will work with no prompt
RequestExecutionLevel user

# start default section
Section
    # Uninstall the previous version. This will also kill the process.
    Call UninstallPrevious
    ClearErrors
    IfErrors 0 +2
        Abort "Error stopping previous SAO version. Please stop it from the system tray and install again."

    # set the installation directory as the destination for the following actions
    SetOutPath $INSTDIR
    SetOverwrite on

    File sao.exe

    # Store installation folder
    WriteRegStr HKCU "Software\San Andreas Online" "" $INSTDIR

    WriteUninstaller "$INSTDIR\uninstall.exe"

    # Support uninstalling via Add/Remove programs
    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\San Andreas Online" \
                     "DisplayName" "San Andreas Online"

    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\San Andreas Online" \
                     "DisplayIcon" "$INSTDIR\sao.ico"

    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\San Andreas Online" \
                     "Publisher" "Brave New Software Project, Inc."

    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\San Andreas Online" \
                     "URLInfoAbout" "http://www.sanandreasonline.com"

    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\San Andreas Online" \
                     "DisplayVersion" "${VERSION}"

    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\San Andreas Online" \
                     "UninstallString" "$\"$INSTDIR\uninstall.exe$\""

    CreateDirectory "$SMPROGRAMS\San Andreas Online"
    CreateShortCut "$SMPROGRAMS\San Andreas Online\San Andreas Online.lnk" "$INSTDIR\SAO.exe" "" "$INSTDIR\sao.ico" 0
    CreateShortCut "$SMPROGRAMS\San Andreas Online\Uninstall San Andreas Online.lnk" "$INSTDIR\uninstall.exe"
    CreateShortCut "$DESKTOP\San Andreas Online.lnk" "$INSTDIR\SAO.exe" "" "$INSTDIR\sao.ico" 0

    # Launch San Andreas Online
    ShellExecAsUser::ShellExecAsUser "" "$INSTDIR\SAO.exe"

SectionEnd
# end default section

# Uninstall previous versions before installing the new one
Function UninstallPrevious
    DetailPrint "Uninstalling previous version"
    ReadRegStr $R0 HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\San Andreas Online" \
						"UninstallString"
    StrCmp $R0 "" noprevious

    DetailPrint "Uninstalling $R0"
    ClearErrors
    ExecWait '$R0 /S _?=$INSTDIR' ;Do not copy the uninstaller to a temp file

    IfErrors erroruninstalling done
noprevious:
	DetailPrint "No previous version to uninstall"
        Goto end
erroruninstalling:
	DetailPrint "Error uninstalling previous at $R0"
        Goto end
done:
	DetailPrint "Successfully uninstalled $R0"
end:
FunctionEnd

# start uninstaller section
Section "uninstall"
    # Stop SAO if necessary
    ${nsProcess::CloseProcess} "sao.exe" $R0
    # Sleep for 1 second to process a chance to die and file to become writable
    Sleep 1000

    RMDir /r "$SMPROGRAMS\San Andreas Online"

    Delete "$DESKTOP\San Andreas Online.lnk"

    Delete "$INSTDIR\sao.exe"
    Delete "$INSTDIR\uninstall.exe"
    
	# Remove uninstaller from Add/Remove programs
    DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\San Andreas Online"

    # Don't run San Andreas Online on startup.
    DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "San Andreas Online"

    ${nsProcess::Unload}
SectionEnd
# end uninstaller section
