#!/bin/bash

NAME=$1

/Users/manuelfarzini/Personal/dev/repo/faust/build/bin/faust \
    -lang mojo -I dsp -double -i -a /Users/manuelfarzini/Personal/dev/repo/faust/tests/impulse-tests/archs/impulsemojo.mojo \
    /Users/manuelfarzini/Personal/dev/repo/faust/tests/impulse-tests/dsp/$NAME.dsp -o tmp/$NAME.mojo && \
    mojo build tmp/$NAME.mojo -o tmp/$NAME && \
    ./tmp/$NAME >> tmp/$NAME.ir && \
    diff --width=500000 /Users/manuelfarzini/Personal/dev/repo/faust/tests/impulse-tests/reference/$NAME.ir tmp/$NAME.ir > tmp/$NAME.dif 2>&1
