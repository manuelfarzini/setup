clang++ $1                         \
  -std=c++2c -stdlib=libc++        \
  -I/opt/homebrew/opt/llvm/include \
  -L/opt/homebrew/opt/llvm/lib     \
  -Wl,-rpath,/opt/homebrew/opt/llvm/lib
