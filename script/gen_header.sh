#!/bin/sh

FILE_CONFIG="$BLACKHOLE_HOME/config/.config"
FILE_HEADER="$BLACKHOLE_HOME/config/autoconf.h"

echo "/* Automatically generated file; DO NOT EDIT. */" > "$FILE_HEADER"
echo "/* Kconfig configuration */" >> "$FILE_HEADER"

awk '
/^#/ { next }
/^$/ { next }
{
    split($0, parts, "=")
    if (parts[2] == "y") {
        printf("#define CONFIG_%s 1\n", parts[1]) >> "'"$FILE_HEADER"'"
    } else if (parts[2] == "m") {
        printf("#define CONFIG_%s_MODULE 1\n", parts[1]) >> "'"$FILE_HEADER"'"
    } else if (parts[2] ~ /^"/) {
        printf("#define CONFIG_%s %s\n", parts[1], parts[2]) >> "'"$FILE_HEADER"'"
    } else {
        printf("#define CONFIG_%s %s\n", parts[1], parts[2]) >> "'"$FILE_HEADER"'"
    }
}
' "$FILE_CONFIG"

echo "[info] update autoconf.h file!"
