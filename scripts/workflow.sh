#!/bin/bash

# SYNOPSIS
#   Runs CMake workflows for Windows (CL) and Linux (Clang/GCC) presets from WSL.
#
# USAGE
#   ./build_workflows.sh <preset_base>
#   Example: ./build_workflows.sh debug

PRESET_BASE=$1

if [ -z "$PRESET_BASE" ]; then
  echo "Usage: $0 <preset-base>"

  exit 1
fi

# Function to print colored status messages
function print_status {
  # Color 36 is Cyan
  echo -e "\033[1;36m[$(date +'%H:%M:%S')] $1\033[0m"
}

# Function to check exit code and stop if failed
function check_success {
  if [ $? -ne 0 ]; then
    # Color 31 is Red
    echo -e "\033[1;31mBuild failed! Stopping workflow.\033[0m"

    exit 1
  fi
}

# --- 1. Windows CL Build (Calling Windows Binary with VS Environment) ---
print_status "Starting Windows (CL) Workflow for: $PRESET_BASE"

# Path to the VS Dev Shell script provided by user
VS_DEV_SHELL="C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\Launch-VsDevShell.ps1"

# We explicitly invoke powershell.exe to:
# 1. Source the VS environment variables using the script.
# 2. Run cmake.exe.
# 3. Ensure the exit code is propagated back to Bash.
powershell.exe -ExecutionPolicy Bypass -NoProfile -Command "& '$VS_DEV_SHELL' -Arch amd64 -HostArch amd64; cd Z:\home\chope\dev\MonoGames; cmake.exe --workflow --preset '${PRESET_BASE}-windows-cl'; if (-not \$?) { exit 1 }"

check_success

# --- 2. Linux Clang Build (Native Linux) ---
print_status "Starting Linux (Clang) Workflow for: $PRESET_BASE"
cmake --workflow --preset "$PRESET_BASE-linux-clang"

check_success

# --- 3. Linux GCC Build (Native Linux) ---
print_status "Starting Linux (GCC) Workflow for: $PRESET_BASE"
cmake --workflow --preset "$PRESET_BASE-linux-gcc"

check_success

print_status "All workflows completed successfully!"
