#!/bin/bash
set -Eeuo pipefail
export LD_LIBRARY_PATH="/opt/gcc-14/lib64${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

./out/build/default/aos/Release/render-aos ./archivos/config4.txt ./archivos/scene4.txt ./archivos/out4.ppm

python3 ./scripts/comparar.py ./archivos/out4.ppm ./archivos/s4.ppm