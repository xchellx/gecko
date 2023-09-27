#!/usr/bin/env bash

# dp0 = current directory
dp0=$(dirname "$(readlink -m "$BASH_SOURCE")")

# check for existence of python
if test -x "$(which python)" ; then
    # run compile script with python
    python "$dp0/compile.py" $@
else
    # notify user that python does not exist
    printf "ERROR: python not found, please install it (for MINGW: \"mingw-w64-x86_64-python\"" \
        "(or install \"requirements_linux.txt\"/\"requirements_mingw.txt\")\n"
fi
