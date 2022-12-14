#!/bin/bash

target="i686-w64-mingw32-gcc"
options="-O2 -Wall -mwindows"
includes="-Ilibs/tail/include -Isrc/headers -Isrc/"

artifact_dir="src/temp_asset_src/"
tail_static="libs/tail/static/tail.a"

declare -a src_directories=("src/")

clean() {
  rm -rf "$artifact_dir" obj bin
  return 0
}

# $1 = path to file (w/o filename or extension)
# $2 = filename (w/o extension)
compile_src_to_obj() {
  "$target" -c "$1$2.c" -o "obj/$2.o" ${includes} ${options} && \
  return 0
}

# ENTRY
clean && \
mkdir "$artifact_dir" && \
mkdir obj bin && \
for src_dir in ${src_directories}; do
  for filepath in "${src_dir}*.c"; do
    src_file=$(basename ${filepath} .c)
    compile_src_to_obj "$src_dir" "$src_file"
  done
done