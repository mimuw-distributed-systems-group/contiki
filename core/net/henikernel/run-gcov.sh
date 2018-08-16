#!/bin/sh

printInfo() {
    echo "Usage:" >&2
    echo "  ./run-gcov.sh <path-to-src-file.c>" >&2
    echo -e >&2
}

if [ $# -ne 1 ]; then
    echo "ERROR: No source file given!" >&2
    echo -e >&2
    printInfo
    exit 1
elif [ '!' '(' -f "$1" ')' -o '!' '(' -r "$1" ')' ]; then
    echo "ERROR: Invalid source file \"$1\"!" >&2
    echo -e >&2
    printInfo
    exit 2
fi

gcov -o obj/native-gcc/unittest "$1"

