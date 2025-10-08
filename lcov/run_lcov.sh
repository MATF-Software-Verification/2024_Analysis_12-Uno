#!/bin/bash
set -euo pipefail

PROJEKAT=../12-UNO

ROOT_DIR="$(pwd)"
COVERAGE_DIR="$ROOT_DIR/coverage"
mkdir -p $COVERAGE_DIR

BUILD_DIR="$PROJEKAT/UNO/build"

rm -rf "$BUILD_DIR"
mkdir "$BUILD_DIR"
cd "$BUILD_DIR"

cmake .. -DCMAKE_BUILD_TYPE=Coverage
make clean
make

ctest --output-on-failure
export GCOV=/usr/bin/gcov-10

# Kreiraj pokrivenost sa `lcov` komandom
lcov --gcov-tool "$GCOV" --capture --directory . --output-file "$COVERAGE_DIR/coverage.info"

# Opcionalno: Uključi samo relevantne fajlove (ako želiš da filtriraš nebitne fajlove)
lcov --remove "$COVERAGE_DIR/coverage.info" '/usr/include/*' "$HOME/Qt/*" '*/tests/*' '*/external/*' '*/build/*' --output-file "$COVERAGE_DIR/coverage_filtered.info"

# Prezentuj pokrivenost u HTML formatu
genhtml "$COVERAGE_DIR/coverage_filtered.info" --output-directory "$COVERAGE_DIR/html"

echo "Pokrivenost je generisana u $COVERAGE_DIR/html"
