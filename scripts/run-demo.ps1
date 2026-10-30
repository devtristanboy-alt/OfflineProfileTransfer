$ErrorActionPreference = "Stop"
$exe = Join-Path $PSScriptRoot "..\build\Release\transfer_tool.exe"
$exe = [System.IO.Path]::GetFullPath($exe)
if (!(Test-Path $exe)) { throw "Compile primeiro: .\scripts\build-windows.ps1" }

$demo = Join-Path $PSScriptRoot "..\demo"
$src = Join-Path $demo "input"
$backup = Join-Path $demo "backup"
$out = Join-Path $demo "restored"
Remove-Item $demo -Recurse -Force -ErrorAction SilentlyContinue
New-Item $src -ItemType Directory -Force | Out-Null
Set-Content (Join-Path $src "save-example.bin") "Example user-owned save data"
New-Item (Join-Path $src "subfolder") -ItemType Directory -Force | Out-Null
Set-Content (Join-Path $src "subfolder" "notes.txt") "Demo file"

& $exe backup $src $backup savedata
if ($LASTEXITCODE -ne 0) { throw "Backup demo falhou" }
& $exe verify $backup
if ($LASTEXITCODE -ne 0) { throw "Verificação demo falhou" }
& $exe restore $backup $out
if ($LASTEXITCODE -ne 0) { throw "Restauração demo falhou" }
Write-Host "Demo concluída em $demo"
