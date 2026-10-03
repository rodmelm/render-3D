#!/bin/bash
set -Eeuo pipefail
export LD_LIBRARY_PATH="/opt/gcc-14/lib64${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"


python3 ./scripts/comparar.py ./archivos/out1.ppm ./archivos/s2.ppm
python3 ./scripts/comparar.py ./archivos/out1.ppm ./archivos/s3.ppm
python3 ./scripts/comparar.py ./archivos/out1.ppm ./archivos/s4.ppm

python3 ./scripts/comparar.py ./archivos/out2.ppm ./archivos/s1.ppm
python3 ./scripts/comparar.py ./archivos/out2.ppm ./archivos/s3.ppm
python3 ./scripts/comparar.py ./archivos/out2.ppm ./archivos/s4.ppm

python3 ./scripts/comparar.py ./archivos/out3.ppm ./archivos/s2.ppm
python3 ./scripts/comparar.py ./archivos/out3.ppm ./archivos/s1.ppm
python3 ./scripts/comparar.py ./archivos/out3.ppm ./archivos/s4.ppm

python3 ./scripts/comparar.py ./archivos/out4.ppm ./archivos/s1.ppm
python3 ./scripts/comparar.py ./archivos/out4.ppm ./archivos/s2.ppm
python3 ./scripts/comparar.py ./archivos/out4.ppm ./archivos/s3.ppm