#!/bin/bash

set -Eeuo pipefail
export LD_LIBRARY_PATH="/opt/gcc-14/lib64${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

# Parámetros de entrada
SCENARIO=$1
PARTITIONER=$2
GRAIN=$3
THREADS=$4

# Configuración
EXEC="./out/build/default/par/Release/render-par"
CONFIG="archivos/config${SCENARIO}.txt"
SCENE="archivos/scene${SCENARIO}.txt"
OUT_DIR="resultados/scenario${SCENARIO}/${PARTITIONER}/grain${GRAIN}"

mkdir -p "$OUT_DIR"

export RENDER_PARTITIONER=$PARTITIONER
export RENDER_GRAIN_SIZE=$GRAIN

echo "=== Evaluación TBB ==="
echo "Escenario: $SCENARIO"
echo "Particionador: $RENDER_PARTITIONER"
echo "Grain size: $RENDER_GRAIN_SIZE"
echo "Hilos a probar: $THREADS"
echo ""

for threads in $THREADS; do
    echo ">>> Threads: $threads <<<"
    export RENDER_NUM_THREADS=$threads
    perf stat -a -e power/energy-pkg/,power/energy-ram/ \
        $EXEC $CONFIG $SCENE "$OUT_DIR/out_${threads}.ppm"
    echo "Completado threads=$threads"
    echo ""
done

echo "=== Trabajo completado ==="
