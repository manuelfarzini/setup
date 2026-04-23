#!/bin/sh

/opt/homebrew/opt/llvm/bin/clang++ -std=gnu++2b -O3 -o $2 \
  -DCX_TEST \
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
  $1 -lfmt
