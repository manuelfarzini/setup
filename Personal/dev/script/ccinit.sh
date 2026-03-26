#!/bin/zsh

# Check arguments
if [ $# -lt 1 ]; then
  echo "Usage: ccinit <project_name>"
  exit 1
fi

# snake_case to PascalCase
snake_to_pascal() {
  local input="$1"
  echo "$input" | tr '_' ' ' | awk '{for(i=1;i<=NF;i++) $i=toupper(substr($i,1,1)) tolower(substr($i,2))}1' | tr -d ' '
}

# Setup naming
PROJECT_NAME="$1"
PROJECT_NAME_UPPER=$(echo "${PROJECT_NAME}" | tr 'a-z' 'A-Z')
PROJECT_NAME_PASCAL=$(snake_to_pascal "${PROJECT_NAME}")

# Project structure
mkdir -p "${PROJECT_NAME}"
cd "${PROJECT_NAME}"
mkdir -p build build/bin build/lib doc external include lib script src test

# Config files
cp ~/.clangd .
cp ~/.clang-tidy .
cp ~/.clang-format .
cp ~/Personal/dev/utils/CMakeLists.txt .
cp ~/Personal/dev/utils/script/ccbuild.sh script
cp ~/Personal/dev/utils/Doxyfile .

# Includes
cp ~/Personal/dev/opt/fmlib/include/o_io.hh include
cp ~/Personal/dev/opt/fmlib/include/o_error.hh include
cp ~/Personal/dev/opt/fmlib/include/o_utilities.hh include
cp -r ~/Personal/dev/opt/fast_io external/fast_io
cp -r ~/Personal/dev/opt/catch test/catch

# src/main.cc
cat <<EOF >"src/main.cc"
/// @file main.cc
#include <o_io.hh>

int main() {
  io::printfn("Hello {} {}", "World", 42);
}
EOF

# test/test_hello.cc
cat <<EOF >"test/test_hello.cc"
/// @file test_hello.cc

#include <o_io.hh>
#include <o_utilities.hh>

#include <catch/catch_amalgamated.hh>


void test_hello_1() {
  io::printfn("{} {}", "Hello Test", 42);
}


TEST_CASE("test_hello") {
  o_test("test_hello_1", test_hello_1);
}
EOF

# First build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --target all -- -j$(sysctl -n hw.ncpu)
./build/bin/project
