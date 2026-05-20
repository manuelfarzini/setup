#!/bin/sh

src="$1"
out="$2"
shift 2

/opt/homebrew/opt/llvm/bin/clang++ -std=gnu++2b -O3 -o "$out" \
  "$@" \
  -Iinclude \
  -I/opt/homebrew/include \
  -I/opt/homebrew/include/fmt \
  -I/usr/local/include \
  -I/Users/manuelfarzini/Personal/dev/include \
  -I/opt/homebrew/opt/llvm/include \
  -fno-exceptions \
  -fno-rtti \
  -L/opt/homebrew/lib \
  -L/usr/local/lib \
  -L/opt/homebrew/opt/llvm/lib \
  "$src" -lfmt
