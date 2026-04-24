#!/bin/bash

# Single cpp backend impulse test.
# Generates a .cmp file with `filesCompare` comparison
# and a .dif file with `diff` commparison.

NAME=$1
FAUST=/Users/manuelfarzini/Personal/dev/repo/faust

rm -f tmp/${NAME}.cpp
rm -f tmp/${NAME}_cpp
rm -f tmp/${NAME}_cpp.ir
rm -f tmp/${NAME}_cpp.cmp
rm -f tmp/${NAME}_cpp.dif

/Users/manuelfarzini/Personal/dev/repo/faust/build/bin/faust \
    -lang cpp -I dsp -double -i -a ${FAUST}/tests/impulse-tests/archs/impulsearch.cpp \
    ${FAUST}/tests/impulse-tests/dsp/${NAME}.dsp -o tmp/${NAME}.cpp && \
    g++ -O3 -fwrapv -std=gnu++17 -I${FAUST}/tests/impulse-tests/archs tmp/${NAME}.cpp -o tmp/${NAME}_cpp && \
    ./tmp/${NAME}_cpp > tmp/${NAME}_cpp.ir 2>&1 && \
    ${FAUST}/tests/impulse-tests/filesCompare \
        ${FAUST}/tests/impulse-tests/reference/${NAME}.ir \
        tmp/${NAME}_cpp.ir \
    > tmp/${NAME}_cpp.cmp 2>&1 && \
    diff --width=500000 ${FAUST}/tests/impulse-tests/reference/${NAME}.ir tmp/${NAME}_cpp.ir > tmp/${NAME}_cpp.dif 2>&1
