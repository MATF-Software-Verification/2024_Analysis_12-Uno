#!/bin/bash
set -euo pipefail

ROOT_DIR="$(pwd)"
PROJEKAT="$ROOT_DIR/../../12-UNO"
BUILD_DIR="$PROJEKAT/UNO/build"
EXECUTABLE="$BUILD_DIR/run_tests"

rm -rf "$ROOT_DIR/memcheck_output/memcheck_report_05.txt"
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
  --suppressions="$ROOT_DIR/qt.supp" \
  --log-file="$ROOT_DIR/memcheck_output/memcheck_report_05.txt" \
  "$EXECUTABLE"

echo "Valgrind analiza završena. Izveštaj: memcheck_report_05.txt"
