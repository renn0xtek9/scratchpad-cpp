#!/bin/bash
# PATH="$PATH:/opt/cmake-4.0.1-linux-x86_64/bin/"
# export PATH
set -euxo pipefail
rm -rf build

cmake --preset release
cmake --build --preset release -- -j 20
cd build/release 
ninja cmake-dependency-diagrams
# cmake --install build/release
