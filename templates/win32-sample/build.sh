#!/bin/bash

target="i686-w64-mingw32-gcc"
options="-O2 -Wall -mwindows"
includes="-Ilibs/tail/include -Isrc/headers -Isrc/"

artifact_dir="src/temp_asset_src/"
tail_static="libs/tail/static/tail.a"

declare -a src_directories=("${artifact_dir}" "src/")

clean() {
  rm -rf "${artifact_dir}"
  rm -rf obj
  return 0
}

# $1 = path to file (w/o filename or extension)
# $2 = filename (w/o extension)
compile_src_to_obj() {
  return "${${target} -c "$1/$2.c" -o "obj/$2.o" ${incudes} ${options}}"
}

# ENTRY
clean && \
mkdir ${artifact_dir} && \
mkdir obj && \
for src_dir in "${src_directories}"; do
  for src_file in "${src_dir}/*"; do
    compile_src_to_obj ${src_dir} ${src_file}
  done
done