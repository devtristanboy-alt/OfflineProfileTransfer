# Build on Windows 11

## Requirements

- Visual Studio 2022 with **Desktop development with C++**
- CMake 3.20+
- Git
- PowerShell 7 or Windows PowerShell 5.1

## Build

From the repository root:

```powershell
cmake -S . -B build -A x64
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

Executable:

```text
build\Release\transfer_tool.exe
```

Or run:

```powershell
.\scripts\build-windows.ps1
```

## Quick demo

```powershell
.\scripts\run-demo.ps1
```

The demo creates sample input files, makes a backup, verifies it, and restores into a fresh output directory.

**Important:** this builds the Windows test harness only. It does not produce PS4/PS5 PKGs.
