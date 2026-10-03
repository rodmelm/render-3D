#!/bin/bash
set -Eeuo pipefail
export LD_LIBRARY_PATH="/opt/gcc-14/lib64${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

perf stat -r 5 ./out/build/default/aos/Release/render-aos ./archivos/config4.txt ./archivos/scene4.txt ./archivos/out4.ppm
