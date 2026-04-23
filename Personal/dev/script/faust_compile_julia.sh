#!/bin/bash

NAME=$1
ARCH=$2
SRC=$3

/Users/manuelfarzini/Personal/dev/repo/faust/build/bin/faust -lang julia -I dsp -double -i -a $ARCH.jl $SRC/$NAME.dsp -o $NAME.jl && nvim $NAME.jl
