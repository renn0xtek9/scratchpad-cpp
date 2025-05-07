#!/bin/bash
# PATH="$PATH:/opt/cmake-4.0.1-linux-x86_64/bin/"
# export PATH
set -euxo pipefail
# rm -rf build

cmake --preset release-ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build --preset release-ninja -- -j 20 
cmake --install build
