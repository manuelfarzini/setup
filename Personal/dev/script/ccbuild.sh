#!/bin/bash

BUILD_DIR="build"
BUILD_TYPE="Debug"

CPU_CORES=$(getconf _NPROCESSORS_ONLN 2>/dev/null || sysctl -n hw.ncpu)

PROJECT_NAME=$(grep -iE '^project\(' CMakeLists.txt | cut -d'(' -f2 | cut -d')' -f1 | awk '{print $1}')

check_cmake() {
  if ! command -v cmake &>/dev/null; then
    echo "Error: cmake is not installed. Please install it first."
    return 1
  fi
}

xmode() {
  echo "Select build mode:"
  select choice in "Debug" "Release"; do
    case $choice in
    Debug | Release)
      BUILD_TYPE=$choice
      echo "Build mode set to: $BUILD_TYPE"
      return 0
      ;;
    *)
      echo "Invalid selection. Please choose a valid option."
      ;;
    esac
  done
}

xclean() {
  check_cmake || return 1
  echo "Cleaning build directory..."
  cmake --build "$BUILD_DIR" --target clean
}

xcleanall() {
  check_cmake || return 1
  echo "Performing full cleanup..."
  cmake --build "$BUILD_DIR" --target clean
  rm -rf "$BUILD_DIR" .cache
}

xbuild() {
  check_cmake || return 1
  echo "Building main project: $PROJECT_NAME ($BUILD_TYPE mode)..."
  cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
  cmake --build "$BUILD_DIR" --target "$PROJECT_NAME" -- -j"$CPU_CORES"
}

xrun() {
  xbuild || return 1
  EXECUTABLE="./$BUILD_DIR/bin/$PROJECT_NAME"
  if [[ -x "$EXECUTABLE" ]]; then
    echo "Running $PROJECT_NAME..."
    "$EXECUTABLE"
  else
    echo "Error: Executable not found or not built correctly."
    return 1
  fi
}

xrebuild() {
  xcleanall
  xrun
}

xtest() {
  check_cmake || return 1
  echo "Building tests..."
  cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
  cmake --build "$BUILD_DIR" --target tests -- -j"$CPU_CORES"

  echo
  echo "Running tests..."
  find "$BUILD_DIR/bin" -maxdepth 1 -type f -perm +111 -name 'test_*' | while read -r test_exec; do
    echo ">>> Running $(basename "$test_exec")"
    "$test_exec" --reporter console --success --colour-mode ansi || return 1
    echo
  done
}

# XXX: actually too much verbose and maybe obsolete
xctest() {
  check_cmake || return 1
  echo "Building and running tests..."

  cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
  cmake --build "$BUILD_DIR" --target tests -- -j"$CPU_CORES"

  echo
  echo "Running test executable via CTest..."
  ctest --test-dir "$BUILD_DIR" --output-on-failure -V
}
