#!/usr/bin/env bash
dp0=$(dirname "$(readlink -m "$BASH_SOURCE")")

if [ "$#" -gt 3 ]; then
    # Game ID
    GMENAME="$1"
    # Game Revision
    GMEREVI="$2"
    # Code Name (source file name without .c)
    SRCNAME="$3"
    # Code Author (name of folder holding source file)
    FDRNAME="$4"

    SRCFILE="$dp0/$FDRNAME/${SRCNAME}_${GMENAME}_${GMEREVI}.c"
    if [ -f "$SRCFILE" ]; then
        OUTNAME="${SRCNAME}_${GMENAME}_${GMEREVI}"
        OUTCMD=(-std=gnu99 )
        ISDEBUG=""
        if [ $# -gt 4 ] && [ "$(echo "$5" |  tr '[:upper:]' '[:lower:]' )" == "asan" ]; then ISDEBUG="1" ; fi
        if [ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ] || [ "$(expr substr $(uname -s) 1 10)" == "MINGW64_NT" ]; then
            OUTNAME+=".exe"
            if [ -n "$ISDEBUG" ]; then
                OUTCMD+=(-o "$dp0/$FDRNAME/bin/$OUTNAME" -O0 -g3 -ggdb -Wall )
            else
                OUTCMD+=(-o "$dp0/$FDRNAME/bin/$OUTNAME" -O1 )
            fi
        else
            if [ -n "$ISDEBUG" ]; then
                OUTCMD+=(-o "$dp0/$FDRNAME/bin/$OUTNAME" -O0 -g3 -ggdb -fsanitize=address -fsanitize=undefined -fno-sanitize-recover -fstack-protector-strong -Wall )
            else
                OUTCMD+=(-o "$dp0/$FDRNAME/bin/$OUTNAME" -O1 )
            fi
        fi
        OUTCMD+=(-Werror=vla -Wno-unused-but-set-variable -Wno-unused-variable )
        
        OUTCMD+=(-I"$dp0/include" )
        OUTCMD+=("$SRCFILE" )
        
        [ ! -d "$dp0/$FDRNAME/bin" ] && mkdir "$dp0/$FDRNAME/bin"
        
        echo "gcc --version" > "$dp0/$FDRNAME/bin/$OUTNAME.log"
        gcc --version >> "$dp0/$FDRNAME/bin/$OUTNAME.log" 2>&1
        echo "gcc ${OUTCMD[@]}" >> "$dp0/$FDRNAME/bin/$OUTNAME.log"
        gcc ${OUTCMD[@]} >> "$dp0/$FDRNAME/bin/$OUTNAME.log" 2>&1
        
        if [ -f "$dp0/$FDRNAME/bin/$OUTNAME" ]; then
            md5sum -b "$dp0/$FDRNAME/bin/$OUTNAME" > "$dp0/$FDRNAME/bin/$OUTNAME.md5" 2>&1
            sha1sum -b "$dp0/$FDRNAME/bin/$OUTNAME" > "$dp0/$FDRNAME/bin/$OUTNAME.sha1" 2>&1
            sha256sum -b "$dp0/$FDRNAME/bin/$OUTNAME" > "$dp0/$FDRNAME/bin/$OUTNAME.sha256" 2>&1
        fi
    else
        echo "\"$SRCFILE\" does not exist"
    fi
else
    echo "USAGE: ./compile.sh <gameid> <gamerevision> <codename> <codeauthor> [asan]"
fi
