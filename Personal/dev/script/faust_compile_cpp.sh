#!/bin/bash

NAME=$1
ARCH=$2
SRC=$3

/Users/manuelfarzini/Personal/dev/repo/faust/build/bin/faust -lang cpp -I dsp -double -i -a $ARCH.cpp $SRC/$NAME.dsp -o $NAME.cpp && nvim $NAME.cpp
