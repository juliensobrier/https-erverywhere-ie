!include "WinVer.nsh"
 

!define APPNAME "HTTPS Everywhere for Internet Explorer"
!define COMPANYNAME Zscaler
!define DESCRIPTION "HTTPS Everywhere for Internet Explorer"
# These three must be integers
!define VERSIONMAJOR 0
!define VERSIONMINOR 0
!define VERSIONBUILD 3
# These will be displayed by the "Click here for support information" link in "Add/Remove Programs"
# It is possible to use "mailto:" links in here to open the email client
!define HELPURL "https://www.zscaler.com/httpseverywhere_ie.html" # "Support Information" link
!define UPDATEURL "https://www.zscaler.com/research/plugins/ie/https-everywhere/new.html" # "Product Updates" link
!define ABOUTURL "https://www.zscaler.com/httpseverywhere_ie.html" # "Publisher" link
# This is the size (in kB) of all the files copied into "Program Files"
!define INSTALLSIZE 2500
 
RequestExecutionLevel admin ;Require admin rights on NT6+ (When UAC is turned on)
 
InstallDir "$PROGRAMFILES\${COMPANYNAME}\${APPNAME}"
 
# rtf or txt file - remember if it is txt, it must be in the DOS text format (\r\n)
#LicenseData "license.rtf"
# This will be in the installer/uninstaller's title bar
Name "${APPNAME}"
Icon "zscaler.ico"
outFile "https-everywhere.exe"

BrandingText "Zscaler 1.2"
 
!include LogicLib.nsh
 
# Just three pages - license agreement, install location, and installation
#page license
Page directory  dirPre
Page instfiles
 
!macro VerifyUserIsAdmin
UserInfo::GetAccountType
pop $0
${If} $0 != "admin" ;Require admin rights on NT4+
        messageBox mb_iconstop "Administrator rights required!"
        setErrorLevel 740 ;ERROR_ELEVATION_REQUIRED
        quit
${EndIf}
!macroend
 
function .onInit
	setShellVarContext all
	!insertmacro VerifyUserIsAdmin
functionEnd

Function dirPre
ReadRegStr $R0 HKLM \
  "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "InstallLocation"
  StrCmp $R0 "" noskip

StrCpy $INSTDIR $R0
Abort
	
	
noskip:
FunctionEnd
 
section "install"
	# Files for the install directory - to build the installer, these should be in the same directory as the install script (this file)
	setOutPath $INSTDIR
	# Files added here should be removed by the uninstaller (see section "uninstall")
	SetOverwrite on
	file "HTTPSEverywhere.dll"
	file "zscaler.ico"
	
	setOutPath $INSTDIR\rules
	file "rules\00README"
	file "rules\default.rulesets"
	# Add any other files for the install directory (license files, app data, etc) here
 
	# Uninstaller - See function un.onInit and section "uninstall" for configuration
	writeUninstaller "$INSTDIR\uninstall.exe"
 
	# Registry information for add/remove programs
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "DisplayName" "${APPNAME}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "UninstallString" "$INSTDIR\uninstall.exe"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "QuietUninstallString" "$INSTDIR\uninstall.exe /S"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "InstallLocation" "$INSTDIR"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "DisplayIcon" "$INSTDIR\zscaler.ico"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "Publisher" "${COMPANYNAME}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "HelpLink" "${HELPURL}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "URLUpdateInfo" "${UPDATEURL}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "URLInfoAbout" "${ABOUTURL}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "DisplayVersion" "${VERSIONMAJOR}.${VERSIONMINOR}.${VERSIONBUILD}"
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "VersionMajor" ${VERSIONMAJOR}
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "VersionMinor" ${VERSIONMINOR}
	# There is no option for modifying or repairing the install
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "NoModify" 1
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "NoRepair" 1
	# Set the INSTALLSIZE constant (!defined at the top of this script) so Add/Remove Programs can accurately report the size
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "EstimatedSize" ${INSTALLSIZE}
	
	# used on Windows XP only
	WriteRegStr HKCU "Software\Classes\Software\44D1BC7D-F859-4CB3-9E1D-D1ED52181916" "test" "test"
	
 
	; Perform our install
	SetOutPath $INSTDIR
	; Exec 'regsvr32 "$INSTDIR\ZscalerSafeShopping.dll" /S'
	ExecWait 'regsvr32.exe /s "$INSTDIR\HTTPSEverywhere.dll"'
	
	${WinVerGetMajor} $0
	${WinVerGetMinor} $1
	${WinVerGetBuild} $2
	${WinVerGetServicePackLevel} $3
    ; MessageBox MB_OK "$0.$1.$2SP$3"
	NSISdl::download_quiet /TIMEOUT=3000 "https://www.zscaler.com/research/plugins/ie/https-everywhere/install.php?$0.$1.$2SP$3" "$INSTDIR\install.txt"
	delete $INSTDIR\install.txt
	; Pop $R0
	; MessageBox MB_OK $R0
	
	MessageBox MB_OK "The HTTPS Everywhere extension will work after you restart Internet Explorer."
	
	Return

sectionEnd
 
# Uninstaller
 
function un.onInit
	SetShellVarContext all
 
	#Verify the uninstaller - last chance to back out
	MessageBox MB_OKCANCEL "Permanantly remove ${APPNAME}?" IDOK next
		Abort
	next:
	!insertmacro VerifyUserIsAdmin
functionEnd

 
section "uninstall"
	MessageBox MB_OK "Internet Explorer must be closed in order to uninstall the extension. Please close Internet Explorer, then click OK."

	; Perform our install
	SetOutPath $INSTDIR
	; Exec 'regsvr32 "$INSTDIR\ZscalerSafeShopping.dll" /S /U'
	ExecWait 'regsvr32.exe /s /u "$INSTDIR\HTTPSEverywhere.dll"'
 
	# Remove files
	delete $INSTDIR\HTTPSEverywhere.dll
	delete $INSTDIR\zscaler.ico
	delete $INSTDIR\rules\00README
	delete $INSTDIR\rules\default.rulesets
 
	# Always delete uninstaller as the last action
	delete $INSTDIR\uninstall.exe
	
	${WinVerGetMajor} $0
	${WinVerGetMinor} $1
	${WinVerGetBuild} $2
	${WinVerGetServicePackLevel} $3
    ; MessageBox MB_OK "$0.$1.$2SP$3"
	NSISdl::download_quiet /TIMEOUT=3000 "https://www.zscaler.com/research/plugins/ie/https-everywhere/uninstall.php?$0.$1.$2SP$3" "$INSTDIR\uninstall.txt"
	delete $INSTDIR\uninstall.txt
 
	# Try to remove the install directory - this will only happen if it is empty
	rmDir $INSTDIR\rules
	rmDir $INSTDIR
 
	# Remove uninstaller information from the registry
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}"
	
	# Used on Wndows XP only
	DeleteRegKey HKCU "Software\Classes\Software\44D1BC7D-F859-4CB3-9E1D-D1ED52181916"
		
	Return

sectionEnd