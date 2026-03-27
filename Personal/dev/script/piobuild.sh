function xrebuild() {
  rm compile_commands.json
  rm -rf .pio
  pio run -t clean
  pio run -t compiledb
}

function xclean() {
  rm compile_commands.json
  rm -rf .pio
  pio run -t clean
}

function xrun() {
  pio run -t compiledb
}
