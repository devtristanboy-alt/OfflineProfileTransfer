$ErrorActionPreference = "Stop"
cmake -S . -B build -A x64
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
Write-Host "`nBuild concluído: build\Release\transfer_tool.exe"
