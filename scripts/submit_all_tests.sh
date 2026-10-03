#!/bin/bash

set -Eeuo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
JOB_SCRIPT="${SCRIPT_DIR}/render_job.sh"

# Estrategias de partición
PARTITIONERS=("auto")

# Configuración por escenario: "granos|threads_parte1|threads_parte2|..."
declare -A SCENARIO_CONFIG
SCENARIO_CONFIG[5]="16,32,64,128|16 32 64 128 256"

JOB_COUNT=0

submit_job() {
    local scenario=$1
    local partitioner=$2
    local grain=$3
    local threads=$4
    
    sbatch -p stan "$JOB_SCRIPT" "$scenario" "$partitioner" "$grain" "$threads"
    JOB_COUNT=$((JOB_COUNT + 1))
}

process_scenario() {
    local scenario=$1
    local config="${SCENARIO_CONFIG[$scenario]}"
    
    local grains_str="${config%%|*}"
    local threads_parts="${config#*|}"
    
    IFS=',' read -ra grains <<< "$grains_str"
    IFS='|' read -ra thread_ranges <<< "$threads_parts"
    
    echo "--- Escenario $scenario ---"
    echo "Granos: ${grains[*]}"
    echo "Partes: ${#thread_ranges[@]}"
    
    for partitioner in "${PARTITIONERS[@]}"; do
        for grain in "${grains[@]}"; do
            for threads in "${thread_ranges[@]}"; do
                submit_job "$scenario" "$partitioner" "$grain" "$threads"
            done
        done
    done
    echo ""
}

# Verificar que existe el script de trabajo
if [[ ! -x "$JOB_SCRIPT" ]]; then
    echo "Error: No se encuentra $JOB_SCRIPT"
    exit 1
fi

echo " Lanzador de pruebas TBB -------------------------------------"
echo ""
echo "Script de trabajo: $JOB_SCRIPT"
echo ""

for scenario in 5; do
    process_scenario "$scenario"
done

echo " Resumen -----------------------------------------"
echo ""
echo "Total de trabajos enviados: $JOB_COUNT"
echo "Para ver estado: squeue -u \$USER"
echo ""
