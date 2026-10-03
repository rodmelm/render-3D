#!/bin/bash
set -Eeuo pipefail
export LD_LIBRARY_PATH="/opt/gcc-14/lib64${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

./out/build/default/aos/Release/render-aos ./archivos/config2.txt ./archivos/scene2.txt ./archivos/out2.ppm

python3 ./scripts/comparar.py ./archivos/out2.ppm ./archivos/s2.ppm
