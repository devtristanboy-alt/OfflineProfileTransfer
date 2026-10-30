param([string]$Exe = ".\build\Release\transfer_tool.exe")
$ErrorActionPreference = "Stop"
& $Exe --help
if ($LASTEXITCODE -ne 0) { throw "Help test failed" }
