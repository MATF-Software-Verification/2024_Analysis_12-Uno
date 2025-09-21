#!/usr/bin/env bash

#QT_PATH="/home/marija/Qt/6.5.3/gcc_64"
#QT6_DIR="$QT_PATH/Qt6"

PROJEKAT=../12-UNO
CODECHECKER_DIR="/home/marija/Desktop/2024_Analysis_12-Uno/codeChecker"
rm -rf "$CODECHECKER_DIR/results" "$CODECHECKER_DIR/reports_html"

source ~/codechecker/venv/bin/activate
export PATH="/home/marija/codechecker/build/CodeChecker/bin:$PATH"
BUILD_DIR="$PROJEKAT/UNO/build"

rm -rf "$BUILD_DIR"
mkdir "$BUILD_DIR"

cd "$BUILD_DIR"
cmake .. #-DCMAKE_PREFIX_PATH="$QT_PATH"
#cmake -DQt6_DIR=/home/marija/Qt/6.5.3/gcc_64/lib/cmake/Qt6 ../
make clean

CodeChecker check -b "make" -o "$CODECHECKER_DIR/results"
echo "Trenutna putanja: $(pwd)"
echo "Pokrećem CodeChecker parse..."

cd "$CODECHECKER_DIR"
CodeChecker parse -e html "$CODECHECKER_DIR/results" -o "$CODECHECKER_DIR/report_html"
deactivate
echo "Zavrsena staticka analiza pomocu codeChecker alata"
