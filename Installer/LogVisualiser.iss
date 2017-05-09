;InnoSetup installer-builder script for LogVisualiser.
;V.Heinitz, 2017-05-10


[Setup]
AppName=LOG_VISIALISER
AppVerName=LOG_VISIALISER 1.0.0
OutputBaseFilename=LogVisualiserSetup_1.0.0
AppPublisher=VHEINITZ
AppPublisherURL=http://www.heinitz-it.de
AppSupportURL=http://www.heinitz-it.de
AppUpdatesURL=http://www.heinitz-it.de
AppCopyright=Copyright © 2015 VHEINITZ
DefaultDirName={pf}\\VHEINITZ\\LOG_VISIALISER
DefaultGroupName=VHEINITZ\LOG_VISIALISER
DisableDirPage=no
DisableProgramGroupPage=yes
AllowNoIcons=yes
OutputDir=..\..\Releases
BackColor=clGreen
SetupIconFile=..\LogVisualiser\icon.ico
Compression=lzma
WizardImageFile=.\res\LogVisualiser.bmp
WizardSmallImageFile=.\res\LogVisualiser_s.bmp
SolidCompression=yes


[Languages]
Name: "en"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "..\output\LogVisualiserApp.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\4.8.4\bin\QtCore4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\4.8.4\bin\QtGui4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\4.8.4\bin\QtNetwork4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\4.8.4\bin\QtScript4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\4.8.4\bin\QtXml4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\4.8.4\bin\QtSql4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\build\cl2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\build\LogVisualiser.exe"; DestDir: "{app}"; Flags: ignoreversion


[Icons]
Name: "{group}\LOG_VISIALISER"; Filename: "{app}\LogVisualiser.exe"
Name: "{group}\{cm:UninstallProgram,LOG_VISIALISER}"; Filename: "{uninstallexe}"
Name: "{userdesktop}\LOG_VISIALISER"; Filename: "{app}\LogVisualiser.exe"; Tasks: desktopicon

