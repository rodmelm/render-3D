#!/bin/bash
set -Eeuo pipefail
export LD_LIBRARY_PATH="/opt/gcc-14/lib64${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

./out/build/default/aos/Release/render-aos ./archivos/config1.txt ./archivos/scene1.txt ./archivos/out1.ppm

python3 ./scripts/comparar.py ./archivos/out1.ppm ./archivos/s1.ppm
