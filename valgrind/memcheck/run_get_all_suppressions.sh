#!/bin/bash
set -euo pipefail

ROOT_DIR="$(pwd)"
PROJEKAT="$ROOT_DIR/../../12-UNO"
BUILD_DIR="$PROJEKAT/UNO/build"
EXECUTABLE="$BUILD_DIR/run_tests"


rm -rf "$ROOT_DIR/memcheck_all_suppressions.txt"
rm -rf "$BUILD_DIR"
mkdir "$BUILD_DIR"
cd "$BUILD_DIR"

cmake .. -DCMAKE_BUILD_TYPE=Debug
make clean
make

valgrind \
  --tool=memcheck \
  --leak-check=full \
  --track-origins=yes \
  --show-leak-kinds=all \
  --gen-suppressions=all \
  "$EXECUTABLE" \
  > "$ROOT_DIR/memcheck_all_suppressions.txt" 2>&1

echo "Valgrind analiza završena. memcheck_all_suppressions.txt"
