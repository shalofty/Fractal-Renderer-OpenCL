#!/usr/bin/env bash
set -euo pipefail

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${PROJECT_ROOT}/build"
BINARY="${BUILD_DIR}/fractal_renderer"

if [[ ! -x "${BINARY}" ]]; then
  echo "[run] Binary not found. Building first..."
  "${PROJECT_ROOT}/scripts/build.sh"
fi

exec "${BINARY}" "$@"


