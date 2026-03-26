g++ -std=c++2b -o $2 \
  -I/opt/homebrew/include \
  -I/usr/local/include \
  -I/Users/manuelfarzini/Personal/dev/opt \
  -L/opt/homebrew/lib \
  -L/usr/local/lib \
  $1 -lfmt

