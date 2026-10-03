#!/bin/bash
set -Eeuo pipefail
export LD_LIBRARY_PATH="/opt/gcc-14/lib64${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

perf stat -r 5 ./out/build/default/soa/Release/render-soa ./archivos/config3.txt ./archivos/scene3.txt ./archivos/out3.ppm
