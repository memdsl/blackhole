#!/bin/sh

FILE_CONFIG="$BLACKHOLE_HOME/config/.config"
FILE_HEADER="$BLACKHOLE_HOME/config/autoconf.h"
FILE_MAKEFL="$BLACKHOLE_HOME/config/autoconf.m"

echo "// Automatically generated file; DO NOT EDIT." > "$FILE_HEADER"
echo "// Kconfig configuration" >> "$FILE_HEADER"
echo "# Automatically generated file; DO NOT EDIT." > "$FILE_MAKEFL"
echo "# Kconfig configuration" >> "$FILE_MAKEFL"

awk '
/^#/ { next }
/^$/ { next }
{
    split($0, parts, "=")
    if (parts[2] == "y") {
        printf("#define %s 1\n", parts[1]) >> "'"$FILE_HEADER"'"
    } else if (parts[2] == "m") {
        printf("#define %s_MODULE 1\n", parts[1]) >> "'"$FILE_HEADER"'"
    } else if (parts[2] ~ /^"/) {
        printf("#define %s %s\n", parts[1], parts[2]) >> "'"$FILE_HEADER"'"
    } else {
        printf("#define %s %s\n", parts[1], parts[2]) >> "'"$FILE_HEADER"'"
    }
}
' "$FILE_CONFIG"

awk '
/^#/ { next }
/^$/ { next }
{
    print >> "'"$FILE_MAKEFL"'"
}
' "$FILE_CONFIG"

echo "[info] update autoconf.h file!"
echo "[info] update autoconf.m file!"
