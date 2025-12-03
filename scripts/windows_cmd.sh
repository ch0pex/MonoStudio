#!/bin/bash

# 1. Join arguments into a single string.
#    We do not wrap them in single quotes individually because we want
#    Invoke-Expression to parse the spaces as arguments, not as a filename.
CMD_LINE="$*"

if [ -z "$CMD_LINE" ]; then
  echo "Usage: $0 <command> [args...]"
  echo "Example: $0 cmake --preset windows-debug"

  exit 1
fi

# 2. Escape single quotes for the PowerShell string wrapper.
#    Since we are passing this inside a PowerShell string enclosed in ',
#    we need to replace any ' in the command with '' (PowerShell escape).
ESCAPED_CMD_LINE="${CMD_LINE//\'/\'\'}"

function check_success {
  if [ $? -ne 0 ]; then
    echo -e "\033[1;31mCommand failed in Windows environment.\033[0m"
    exit 1
  fi
}

WIN_CWD=$(wslpath -w "$(pwd)")

# Path to the Visual Studio Developer Shell script
VS_DEV_SHELL="C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\Launch-VsDevShell.ps1"
VS_DEV_SHELL_WSL=$(wslpath -u "${VS_DEV_SHELL//\\/\/}")

if [ ! -f "$VS_DEV_SHELL_WSL" ]; then
  VS_DEV_SHELL="C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\Launch-VsDevShell.ps1"
fi

echo -e "\033[1;36m[VS-Exec] Running: $CMD_LINE\033[0m"

# Execute logic:
# CHANGED: Replaced '&' with 'Invoke-Expression' (iex).
# This allows PowerShell to interpret the string "cmake -arg" as command + arg,
# instead of looking for a file named "cmake -arg".
powershell.exe -ExecutionPolicy Bypass -NoProfile -Command "
  & '$VS_DEV_SHELL' -Arch amd64 -HostArch amd64 | Out-Null; 
  cd '$WIN_CWD'; 
  Invoke-Expression '$ESCAPED_CMD_LINE'; 
  if (\$LastExitCode -ne 0) { exit \$LastExitCode }
"

check_success
