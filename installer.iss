[Setup]
AppName=MicroSIP Custom
AppVersion=3.22.16
AppPublisher=August
DefaultDirName={autopf}\MicroSIP
DefaultGroupName=MicroSIP
UninstallDisplayIcon={app}\microsip.exe
Compression=lzma2/ultra64
SolidCompression=yes
OutputDir=.\Dist
OutputBaseFilename=MicroSIP-3.22.16-Custom-Setup
DisableProgramGroupPage=yes
PrivilegesRequired=lowest

[Languages]
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"

[Files]
Source: "Release\microsip.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "langpack_russian.txt"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\MicroSIP"; Filename: "{app}\microsip.exe"
Name: "{group}\{cm:UninstallProgram,MicroSIP}"; Filename: "{uninstallexe}"
Name: "{autodesktop}\MicroSIP"; Filename: "{app}\microsip.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\microsip.exe"; Description: "{cm:LaunchProgram,MicroSIP}"; Flags: nowait postinstall skipifsilent
