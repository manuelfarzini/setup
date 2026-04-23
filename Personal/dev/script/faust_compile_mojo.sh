#!/bin/bash

NAME=$1
ARCH=$2
SRC=$3

/Users/manuelfarzini/Personal/dev/repo/faust/build/bin/faust -lang mojo -I dsp -double -i -a $ARCH.mojo $SRC/$NAME.dsp -o $NAME.mojo && nvim $NAME.mojo

