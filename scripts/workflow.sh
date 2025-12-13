#!/bin/bash

# ==============================================================================
# CONFIGURATION
# ==============================================================================

PRESET_BASE=$1

if [ -z "$PRESET_BASE" ]; then
  echo "Usage: $0 <preset-base>"

  exit 1
fi

# 1. Get the actual UNC path of the current directory

# This returns something like: \\wsl.localhost\Fedora\home\chope\dev\MonoGames
WSL_UNC_PATH=$(wslpath -w $(pwd))

# Use drive B: to avoid conflicts with Z: or C:
TEMP_DRIVE="B:"

function print_status {
  echo -e "\033[1;36m[$(date +'%H:%M:%S')] $1\033[0m"

}

function check_success {
  if [ $? -ne 0 ]; then
    echo -e "\033[1;31mBuild failed! Stopping workflow.\033[0m"

    exit 1
  fi
}

# Relative folders (inside the project)
WIN_BUILD_DIR="build/windows/cl"
LIN_CLANG_DIR="build/linux/clang"
LIN_GCC_DIR="build/linux/gcc"

# ==============================================================================
# 1. WINDOWS CL BUILD
# ==============================================================================
print_status "Starting Windows (CL) Workflow for: $PRESET_BASE"
print_status "Mapping WSL path to virtual drive $TEMP_DRIVE..."
print_status "Source: $WSL_UNC_PATH"

PS_SCRIPT="temp_build_worker.ps1"

# Generate PowerShell script
cat <<EOF >"$PS_SCRIPT"
\$ErrorActionPreference = "Stop"

# VS Dev Shell Path
\$VsDevCmd = "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\Launch-VsDevShell.ps1"
# Uncomment if using Community:
# \$VsDevCmd = "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\Launch-VsDevShell.ps1"

# ---------------------------------------------------------
# VIRTUAL DRIVE MOUNT (SUBST)
# ---------------------------------------------------------
\$Drive = "$TEMP_DRIVE"
\$Target = "$WSL_UNC_PATH"

# Pre-emptive cleanup in case of a previous stuck state
if (Test-Path \$Drive) {
    cmd /c "subst \$Drive /d" > \$null
}

# Create virtual drive B: pointing to the WSL folder
cmd /c "subst \$Drive ""\$Target"""

if (-not (Test-Path \$Drive)) {
    Write-Host "FATAL: Could not create virtual drive \$Drive" -ForegroundColor Red
    exit 1
}

# ---------------------------------------------------------
# COMPILATION
# ---------------------------------------------------------
try {
    # Load VS Environment
    Write-Host "Loading Visual Studio Environment..." -ForegroundColor Cyan
    & \$VsDevCmd -Arch amd64 -HostArch amd64 | Out-Null

    # Switch to drive B: (Current project root)
    Set-Location "\$Drive\\"

    \$PresetConfigure = "Ninja-windows-cl"
    \$PresetBuild     = "${PRESET_BASE}-windows-cl"
    \$BuildDir        = "$WIN_BUILD_DIR"

    # Configure
    if (-not (Test-Path \$BuildDir)) {
        Write-Host "Configuring CMake preset: \$PresetConfigure" -ForegroundColor Yellow
        cmake.exe --preset \$PresetConfigure
        if (\$LASTEXITCODE -ne 0) { throw "CMake Configuration Failed" }
    }

    # Build
    Write-Host "Building preset: \$PresetBuild" -ForegroundColor Yellow
    cmake.exe --build --preset \$PresetBuild
    if (\$LASTEXITCODE -ne 0) { throw "CMake Build Failed" }

    # Test
    Write-Host "Running Tests..." -ForegroundColor Yellow
    ctest.exe --preset \$PresetBuild
    if (\$LASTEXITCODE -ne 0) { throw "CTest Failed" }

} catch {
    Write-Host "Error: \$_.Exception.Message" -ForegroundColor Red
    # Ensure drive unmount even on failure
    cmd /c "subst \$Drive /d"
    exit 1
}

# Unmount drive upon success
cmd /c "subst \$Drive /d"
EOF

# Execute PowerShell
powershell.exe -ExecutionPolicy Bypass -File "$PS_SCRIPT"
EXIT_CODE=$?
rm "$PS_SCRIPT"

if [ $EXIT_CODE -ne 0 ]; then
  echo -e "\033[1;31mWindows Build failed!\033[0m"
  exit 1
fi

# ==============================================================================
# 2. LINUX WORKFLOWS
# ==============================================================================
print_status "Starting Linux Workflows..."

if [ ! -d "$LIN_CLANG_DIR" ]; then
  cmake --preset "Ninja-linux-clang"
  check_success
fi

cmake --build --preset "${PRESET_BASE}-linux-clang"
check_success
ctest --preset "${PRESET_BASE}-linux-clang"
check_success

if [ ! -d "$LIN_GCC_DIR" ]; then
  cmake --preset "Ninja-linux-gcc"
  check_success
fi

cmake --build --preset "${PRESET_BASE}-linux-gcc"
check_success
ctest --preset "${PRESET_BASE}-linux-gcc"
check_success

print_status "All workflows completed successfully!"
