#!/bin/bash
set -euxo pipefail

cmake --preset release-ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build --preset release-ninja -- -j 20 
cmake --install build
