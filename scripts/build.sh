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
  echo -e "\033[1;36m[$(date +'%H:%M:%S')] $1\033[0m"
}

# Function to check exit code and stop if failed
function check_success {
  if [ $? -ne 0 ]; then
    echo -e "\033[1;31mBuild failed! Stopping workflow.\033[0m"
    exit 1
  fi
}

# --- 0. Fix Git Permissions ---
print_status "Configuring Windows Git to trust WSL paths..."
powershell.exe -NoProfile -Command "git config --global --replace-all safe.directory '*'"
check_success

# --- PATH VARIABLES (Adjust according to CMakePresets.json) ---
# Assumes folder structure: build/<preset-name>

# If paths are fixed in JSON (e.g., build/windows), update these variables.
WIN_DIR="build/windows"
LIN_CLANG_DIR="build/linux/clang"
LIN_GCC_DIR="build/linux/gcc"

# --- 1. Windows CL Build ---
print_status "Starting Windows (CL) Workflow for: $PRESET_BASE"

VS_DEV_SHELL="C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\Launch-VsDevShell.ps1"
PROJECT_ROOT="Z:\home\chope\dev\MonoGames"

# 1.A: Configure (Generate) if folder does not exist

if [ ! -d "$WIN_DIR" ]; then
  print_status "Build folder not found ($WIN_DIR). Running Configure step..."
  powershell.exe -ExecutionPolicy Bypass -NoProfile -Command "& '$VS_DEV_SHELL' -Arch amd64 -HostArch amd64; cd $PROJECT_ROOT; cmake.exe --preset 'Ninja-windows-cl'; if (-not \$?) { exit 1 }"
  check_success
fi

# 1.B: Build and Test
powershell.exe -ExecutionPolicy Bypass -NoProfile -Command "& '$VS_DEV_SHELL' -Arch amd64 -HostArch amd64; cd $PROJECT_ROOT; cmake.exe --build --preset '${PRESET_BASE}-windows-cl'; if (-not \$?) { exit 1 }"

check_success

# --- 2. Linux Clang Build ---
print_status "Starting Linux (Clang) Workflow for: $PRESET_BASE"

# 2.A: Configure if folder does not exist
if [ ! -d "$LIN_CLANG_DIR" ]; then
  print_status "Build folder not found ($LIN_CLANG_DIR). Running Configure step..."
  cmake --preset "Ninja-linux-clang"
  check_success
fi

# 2.B: Build and Test
cmake --build --preset "$PRESET_BASE-linux-clang"
check_success

# --- 3. Linux GCC Build ---

print_status "Starting Linux (GCC) Workflow for: $PRESET_BASE"

# 3.A: Configure if folder does not exist
if [ ! -d "$LIN_GCC_DIR" ]; then
  print_status "Build folder not found ($LIN_GCC_DIR). Running Configure step..."
  cmake --preset "Ninja-linux-gcc"

  check_success
fi

# 3.B: Build and Test
cmake --build --preset "$PRESET_BASE-linux-gcc"

check_success

print_status "All workflows completed successfully!"
