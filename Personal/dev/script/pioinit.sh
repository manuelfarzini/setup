#!/bin/bash

mkdir -p $1
cd $1
pio init --board $2

echo 'upload_protocol = teensy-cli' >>platformio.ini
echo 'extrs_scripts = pre:gen_compile_commands.py' >>platformio.ini
echo 'monitor_speed = 115200' >>platformio.ini

create_file() {
  echo "$2" >"$1"
  echo "Created $1"
}

create_file ".clangd" "# clangd controls options for the LSP *server*
CompileFlags:
  Add: [
    -DSSIZE_MAX,
    -DLWIP_NO_UNISTD_H=1,
    -Dssize_t=long,
    -D_SSIZE_T_DECLARED,
    -Wno-unknown-warning-option
  ]
  Remove: [
    -mlong-calls,
    -fno-tree-switch-conversion,
    -mtext-section-literals,
    -mlongcalls,
    -fstrict-volatile-bitfields,
    -free,
    -fipa-pta,
    -march=*,
    -mabi=*,
    -mcpu=*
  ]
Diagnostics:
  Suppress: \"pp_including_mainfile_in_preamble\""

create_file ".clang-tidy" "# Formatter options
Checks: \"*,
        -abseil-*,
        -altera-*,
        -android-*,
        -fuchsia-*,
        -google-*,
        -llvm*,
        -modernize-use-trailing-return-type,
        -zircon-*,
        -readability-else-after-return,
        -readability-static-accessed-through-instance,
        -readability-avoid-const-params-in-decls,
        -cppcoreguidelines-non-private-member-variables-in-classes,
        -misc-non-private-member-variables-in-classes,
\"
WarningsAsErrors: ''
HeaderFilterRegex: ''
FormatStyle:     none"

create_file "gen_compile_commands.py" "import os
Import(\"env\") # platformio specific stuff

# include toolchain paths (i.e. to have stuff like the Arduino framework headers present in the compile commands)
env.Replace(COMPILATIONDB_INCLUDE_TOOLCHAIN=True)

# override compilation DB path
env.Replace(COMPILATIONDB_PATH=\"compile_commands.json\")"

create_file "src/main.cpp" "#include <Arduino.h>

void setup() {
  Serial.begin(115200);

}

void loop() {
  Serial.println(\"Hello, world!\");
  delay(1000);
}"

pio run -t compiledb

cp /Users/manuelfarzini/Personal/dev/utils/piobuild.sh .
