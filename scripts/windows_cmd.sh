#!/bin/bash

# 1. Reconstruct arguments correctly preserving quotes for PowerShell

#    This ensures that commands like 'cmake -DVAR="Value with spaces"' work correctly.
CMD_LINE=""
for arg in "$@"; do
  # Escape single quotes inside the argument (replace ' with '')
  # and wrap the whole argument in single quotes
  ESCAPED_ARG="${arg//\'/\'\'}"
  CMD_LINE="$CMD_LINE '$ESCAPED_ARG'"

done

if [ -z "$CMD_LINE" ]; then
  echo "Usage: $0 <command> [args...]"
  echo "Example: $0 cmake --preset windows-debug"
  exit 1
fi

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
# 1. Load VS Dev Shell (piped to Out-Null to silence the welcome banner)
# 2. cd to the working directory
# 3. Use '&' operator to execute the reconstructed command line
# 4. Propagate the exit code
powershell.exe -ExecutionPolicy Bypass -NoProfile -Command "
  & '$VS_DEV_SHELL' -Arch amd64 -HostArch amd64 | Out-Null; 
  cd '$WIN_CWD'; 
  & $CMD_LINE; 
  if (\$LastExitCode -ne 0) { exit \$LastExitCode }

"

check_success
