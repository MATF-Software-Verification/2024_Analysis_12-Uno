#!/bin/bash

INPUT="memcheck_all_suppressions.txt"
OUTPUT="qt.supp"

rm -f "$OUTPUT"

awk '
/^==/ {if ($0 ~ /\.cpp/) has_cpp=1}
/^{/ { block = ""; inside = 1 }
inside { block = block $0 "\n" }
/^}/ {
    inside = 0
    if (!has_cpp) {
        print block >> "'"$OUTPUT"'"
    }
    block = ""
    has_cpp=0
}
' "$INPUT"

echo "Qt/Wayland suppressions sacuvani u: $OUTPUT"

#/libQt|libQt5Core|libQt5Gui|libQt5Widgets|libQt6Core|libQt6Gui|libQt6Widgets|libwayland|libglib|libgobject|libgtk|libatk|libfontconfig|libpango|libpangocairo|libpangoft2|libharfbuzz|libxkbcommon|libgdk|libqgtk3/

#/lib(Qt|gtk|gdk|atk|wayland|xkbcommon|fontconfig|pango|harfbuzz|dbus|atspi|atk-bridge|glib|gobject)/
