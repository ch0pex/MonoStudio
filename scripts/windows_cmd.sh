#!/bin/bash

CMD_LINE="$*"

if [ -z "$CMD_LINE" ]; then
  echo "Uso: $0 <comando> [argumentos...]"
  echo "Ejemplo: $0 cmake --build build/windows/cl"
  exit 1

fi

WSL_UNC_PATH=$(wslpath -w "$(pwd)")
TEMP_DRIVE="B:"
PS_SCRIPT="temp_exec_worker.ps1"

cat <<EOF >"$PS_SCRIPT"
\$ErrorActionPreference = "Stop"

# --- CONFIGURACIÓN ---
\$VsDevCmd_Community = "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\Launch-VsDevShell.ps1"
\$VsDevCmd_BuildTools = "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\Launch-VsDevShell.ps1"

if (Test-Path \$VsDevCmd_BuildTools) {
    \$VsDevCmd = \$VsDevCmd_BuildTools
} elseif (Test-Path \$VsDevCmd_Community) {
    \$VsDevCmd = \$VsDevCmd_Community
} else {
    Write-Error "No se encontró Launch-VsDevShell.ps1"
    exit 1
}

# --- MONTAJE DE UNIDAD (SUBST) ---
\$Drive = "$TEMP_DRIVE"
\$Target = "$WSL_UNC_PATH"


# Limpieza preventiva
if (Test-Path \$Drive) { cmd /c "subst \$Drive /d" > \$null }


# Montar unidad
cmd /c "subst \$Drive ""\$Target"""

if (-not (Test-Path \$Drive)) {
    Write-Host "FATAL: No se pudo montar la unidad \$Drive" -ForegroundColor Red

    exit 1
}

# --- EJECUCIÓN ---
try {
    # Cargar entorno VS (Silencioso para no ensuciar la salida del comando)
    & \$VsDevCmd -Arch amd64 -HostArch amd64 | Out-Null

    # Cambiar a la unidad virtual

    Set-Location "\$Drive\\"

    Write-Host "[VS-Exec] Running on $TEMP_DRIVE : $CMD_LINE" -ForegroundColor Cyan

    # Ejecutar el comando arbitrario
    # Invoke-Expression permite pasar argumentos complejos
    Invoke-Expression "$CMD_LINE"

    if (\$LASTEXITCODE -ne 0) { exit \$LASTEXITCODE }

} catch {
    Write-Host "Error executing command: \$_.Exception.Message" -ForegroundColor Red
    exit 1
} finally {
    # --- LIMPIEZA (Siempre desmontar) ---
    # Volvemos a C: para no bloquear el desmontaje

    Set-Location "C:\" 

    cmd /c "subst \$Drive /d" > \$null
}
EOF

powershell.exe -ExecutionPolicy Bypass -File "$PS_SCRIPT"
EXIT_CODE=$?

rm "$PS_SCRIPT"

exit $EXIT_CODE
