#!/bin/sh

input="$1"
basename="${input%.md}"
output="$basename.pdf"

pandoc "$input" -o "$output" \
  --template=/Users/manuelfarzini/Personal/dev/uti/template.tex \
  --pdf-engine=xelatex \
  --toc --toc-depth=2

