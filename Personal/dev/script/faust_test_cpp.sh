#!/bin/bash

../../build/bin/faust -lang cpp -I dsp -double -i -a archs/impulsearch.cpp dsp/$1.dsp -o tmp/$1.cpp && nvim tmp/$1.cpp

