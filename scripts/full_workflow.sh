#!/bin/bash
set -e # Salir inmediatamente si algo falla

./scripts/workflow.sh Debug
./scripts/workflow.sh RelWithDebInfo </dev/null
./scripts/workflow.sh Release </dev/null
