#!/usr/bin/env bash
dp0=$(dirname "$(readlink -m "$BASH_SOURCE")")

SRCFILE="$dp0/codelist.csv"
if [ -f "$SRCFILE" ]; then
    {
        read
        while IFS=, read -r id revision name author || [ -n "$author" ]
        do
            "$dp0/compile.sh" "$id" "$revision" "$name" "$author" $@
        done
    } < "$SRCFILE"
else
    echo "\"$SRCFILE\" does not exist"
fi
