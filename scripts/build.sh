#!/usr/bin/env bash
set -euo pipefail

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SRC_DIR="${PROJECT_ROOT}/src"
BUILD_DIR="${PROJECT_ROOT}/build"

mkdir -p "${BUILD_DIR}"

echo "[build] Compiling OpenCL Fractal Renderer (scaffold)..."

g++ -std=c++17 -O2 -Wextra \
    -I"${PROJECT_ROOT}/include" \
    "${SRC_DIR}/main.cpp" \
    "${SRC_DIR}/device_manager.cpp" \
    "${SRC_DIR}/kernel_manager.cpp" \
    "${SRC_DIR}/memory_manager.cpp" \
    "${SRC_DIR}/fractal_strategy.cpp" \
    "${SRC_DIR}/renderer.cpp" \
    "${SRC_DIR}/output_writer.cpp" \
    -framework OpenCL \
    -o "${BUILD_DIR}/fractal_renderer" \
    2>&1 | sed 's/^/[g++] /'

echo "[build] Done. Binary at ${BUILD_DIR}/fractal_renderer"


