#!/usr/bin/env bash
set -euo pipefail

PROJEKAT=../12-UNO
OUTPUT_DIR=clang-format-output

rm -rf "$OUTPUT_DIR"
mkdir -p "$OUTPUT_DIR"

FILES=$(find "$PROJEKAT" -type f \( -name "*.cpp" -o -name "*.hpp" -o -name "*.h" \))
for FILE in $FILES;do
    REL_PATH="${FILE#$PROJEKAT/}"
    DEST="$OUTPUT_DIR/$REL_PATH"

    mkdir -p "$(dirname "$DEST")"
    #echo "Obradjuje se fajl $FILE"
    clang-format -style=file:.clang-format "$FILE" > "$DEST"

    if ! cmp -s "$FILE" "$DEST";then
        echo "Izmenjen fajl $FILE"
    else
        rm "$DEST"
    fi
done

echo "Zavrseno formatiranje pomocu clang-format alata"
