#!/bin/bash

target="i686-w64-mingw32-gcc"
declare -a options=("-O2" "-Wall" "-mwindows")

tools="libs/tail/tools/"
artifact_dir="src/temp_asset_src/"
tail_static="libs/tail/static/tail.a"

declare -a includes=("-Ilibs/tail/include" -I$artifact_dir)

declare -a src_directories=($artifact_dir "src/")

clean() {
  rm -rf "$artifact_dir" obj bin
  return 0
}

build_assets() {
  ./${tools}sourcify-obj.exe assets/cube.obj flat $artifact_dir
  ./${tools}sourcify-glsl.exe assets/cube_frag.glsl $artifact_dir
  ./${tools}sourcify-glsl.exe assets/cube_vert.glsl $artifact_dir
  ./${tools}sourcify-png.exe assets/fur.png 3 $artifact_dir
}

compile_src() {
  for src_dir in ${src_directories[@]}; do
    for filepath in ${src_dir}*.c; do
      src_file=$(basename $filepath .c)
      $target -c $src_dir$src_file.c -o obj/$src_file.o ${includes[@]} ${options[@]}
    done
  done
  return 0
}

link_objs() {
  $target -o bin/game.exe $(echo obj/*.o) $tail_static
}

# ENTRY
clean && \
mkdir $artifact_dir obj bin && \
build_assets && \
compile_src && \
link_objs