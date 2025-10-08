#!/bin/bash
ROOT_DIR="$(pwd)"
INPUT="$ROOT_DIR/memcheck_output/memcheck_report_03.txt"
OUTPUT="$ROOT_DIR/indirectly_lost.txt"

rm -f "$OUTPUT"
#echo "Rezultat grepa: $INPUT"
#grep "^==89998== [0-9]" "$INPUT" | cat -A
awk '
/^==[0-9]*== [0-9]/ {
    if ($0 ~ /indirectly lost/){inside = 1;block = $0 "\n";next}
}

inside {
    block = block $0 "\n"

    if ($0 ~ /^==[0-9]*== $/) {
        inside = 0
        if (block ~ /test_pile\.cpp/) {
            print block >> "'"$OUTPUT"'"
        }
        block = ""
    }

    next
}
' "$INPUT"


echo "Qt/Wayland suppressions sacuvani u: $OUTPUT"
