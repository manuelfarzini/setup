#!/bin/bash

../../build/bin/faust -lang julia -I dsp -double -i -a archs/impulsejulia.jl dsp/$1.dsp -o tmp/$1.jl && nvim tmp/$1.jl
