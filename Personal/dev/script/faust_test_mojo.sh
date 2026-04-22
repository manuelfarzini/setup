#!/bin/bash

../../build/bin/faust -lang mojo -I dsp -double -i -a archs/impulsemojo.mojo dsp/$1.dsp -o tmp/$1.mojo && nvim tmp/$1.mojo
