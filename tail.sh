#!/bin/bash

target="win32"
wsl_abs_path_to_root="/mnt/d/c/tail/"
options="-O2 -Wall"
includes="-Isrc/headers -Ilibs/GLAD/include -Ilibs/GLFW/include -Iinclude"

declare -A targets
targets[win32]=i686-w64-mingw32-gcc
targets[gcc]=gcc

usage() {
  echo "
    usage: tail [-t|-o|-f|-s|-m] <command>

    Commands
    clean               delete tail build artifacts
    build-glad          compile glad opengl loader into obj
    build-glfw          compile glfw into local static library
    build               compile tail src into obj
    static              compile tail into static lib
    create-slim         create dir for lib and header to easily copy into game
    template            bootstrap a new game project at output path
    test                build tail, run tests, print results to console
    testlog             build tail, run tests, print results to file
    validate-glsl       check a glsl file for errors
    parse-obj           process obj file into c src and print to stdout

    Options
    -t                  compile target, e.g. win32, gcc
    -o                  output directory for new project
    -f                  filepath to input file for glsl validator and obj parser
    -s                  shader type: \"frag\" | \"vert\"
    -m                  mesh shading style: \"flat\" | \"smooth\"
  "
}
clean() {
  rm -rf obj static copy test_report.txt
}
build() {

  if [[ "$target" != "win32" && "$target" != "gcc" ]]; then
  echo "Invalid compilation target."
  exit 1
  fi

  rm -rf obj
  mkdir obj
  ${targets[${target}]} -c src/math/m2x2.c -o obj/m2x2.o ${includes} ${options}
  ${targets[${target}]} -c src/math/m3x3.c -o obj/m3x3.o ${includes} ${options}
  ${targets[${target}]} -c src/math/m4x4.c -o obj/m4x4.o ${includes} ${options}
  ${targets[${target}]} -c src/math/quaternion.c -o obj/quaternion.o ${includes} ${options}
  ${targets[${target}]} -c src/math/vec3.c -o obj/vec3.o ${includes} ${options}
  ${targets[${target}]} -c src/opengl_gpu.c -o obj/opengl_gpu.o ${includes} ${options}
  ${targets[${target}]} -c src/debugger.c -o obj/debugger.o ${includes} ${options}
  ${targets[${target}]} -c src/camera.c -o obj/camera.o ${includes} ${options}
  ${targets[${target}]} -c src/glfw_window.c -o obj/glfw_window.o ${includes} ${options}
  ${targets[${target}]} -c src/space.c -o obj/space.o ${includes} ${options}
  ${targets[${target}]} -c src/precision.c -o obj/precision.o ${includes} ${options}
  ${targets[${target}]} -c src/math/tail_math.c -o obj/tail_math.o ${includes} ${options}
  ${targets[${target}]} -c src/viewport.c -o obj/viewport.o ${includes} ${options}
}
# TODO: below, extracting gdi32 not gonna hold up for multiple targets as is
static() {
  rm -rf static
  mkdir static
  mkdir gdi32obj
  ar x --output gdi32obj /usr/i686-w64-mingw32/lib/libgdi32.a \
  && \
  ar -crs static/tail.a \
  obj/*.o \
  gdi32obj/*.o \
  libs/GLAD/obj/glad.o \
  libs/GLFW/obj/*.c.obj \
  && \
  rm -rf gdi32obj
}
create_slim() {
  rm -rf copy
  mkdir copy
  mkdir copy/tail
  mkdir copy/tail/static
  mkdir copy/tail/src
  mkdir copy/tail/include
  mkdir copy/tail/tools
  cp static/tail.a copy/tail/static/tail.a \
  && \
  cp include/tail.h copy/tail/include/tail.h \
  && \
  cp -r src/headers copy/tail/src \
  && \
  cp tools/obj-parser/bin/obj-parser.exe copy/tail/tools \
  && \
  cp tools/validate-glsl/bin/validate-glsl.exe copy/tail/tools \
  && \
  cp tools/sourcify-glsl/bin/sourcify-glsl.exe copy/tail/tools
}
template() {
  
  if [ -d "$output_path" ]; then
    echo "Directory already exists."
    exit 1
  fi

  mkdir -p "$output_path/src" && \
  mkdir -p "$output_path/libs/tail" && \
  mkdir -p "$output_path/tools" && \
  cp -R copy/tail "$output_path/libs/" && \
  cp -R copy/tail/tools "$output_path/" && \
  cp template_src/main.c "$output_path/src/main.c"

  # TODO: add case for gcc
  if [ $target == "win32" ]; then
    cp template_src/win32-build.sh "$output_path/build.sh"
  fi
}
run_and_log_tests() {
  ./tests.exe &> test_report.txt
}
build_glfw() {
  rm -rf libs/glfw/obj libs/glfw/build
  mkdir libs/glfw/obj libs/glfw/build
  cmake -S libs/glfw -B libs/glfw/build \
  -DCMAKE_TOOLCHAIN_FILE=${wsl_abs_path_to_root}libs/glfw/CMake/i686-w64-mingw32.cmake \
  && \
  cmake --build libs/glfw/build \
  && \
  ar x --output libs/GLFW/obj libs/GLFW/build/src/libglfw3.a
}
build_glad() {
  rm -rf libs/GLAD/obj
  mkdir libs/GLAD/obj
  ${targets[${target}]} -c libs/GLAD/src/glad.c \
  -Ilibs/GLAD/include -o libs/GLAD/obj/glad.o \
  -Wall
}
build_tests() {
  rm -rf tests.o && \
  ${targets[${target}]} -c tests.c -o tests.o -Itest_data ${options} ${includes} && \
  ${targets[${target}]} -o tests.exe \
  tests.o \
  static/tail.a
}
build_tools() {
  # creates windows executables (doesn't matter for wsl but target should be
  # parameter here)
  cd tools/obj-parser && ./build.sh build
  cd ../validate-glsl && ./build.sh && \
  cd ../sourcify-glsl && ./build.sh && \
  cd ../..
}
validate_glsl() {

  if [ -f "$input_filepath" ]; then
    echo "Validating $input_filepath"
  else
    echo "Shader file does not exist."
    exit 1
  fi

  if [[ "$shader_type" != "vert" && "$shader_type" != "frag" ]]; then
  echo "To validate glsl, shader type must be vert or frag."
  exit 1
  fi

  $wsl_abs_path_to_root/tools/validate-glsl/validate-glsl.exe \
  "$input_filepath" "$shader_type"
}
parse_obj() {
  if [ -f "$input_filepath" ]; then
    echo "Parsting $input_filepath"
  else
    echo "Obj file not found."
    exit 1
  fi
  if [ -f "$input_filepath" ]; then
    echo "Parsting $input_filepath"
  else
    echo "Obj file not found."
    exit 1
  fi

  $wsl_abs_path_to_root/tools/obj-parser/bin/parser.exe \
  "$input_filepath"
}

while getopts ":t:o:f:s:m:" option; do
  case "$option" in
    "t") target=$OPTARG;;
    "o") output_path=$OPTARG;;
    "f") input_filepath=$OPTARG;;
    "s") shader_type=$OPTARG;;
    "m") shading_style=$OPTARG;;
    ":")
      echo "    A value must be provided for the -$OPTARG option"
      usage
      exit;;
  esac
done

ARG1=${@:$OPTIND:1}

if [ "$ARG1" == "build" ]; then
  clean && build
elif [ "$ARG1" == "clean" ]; then
  clean
elif [ "$ARG1" == "static" ]; then
  clean && build && static
elif [ "$ARG1" == "create-slim" ]; then
  clean && build && static && build_tools && create_slim
elif [ "$ARG1" == "template" ]; then
  clean && build && static && build_tools && create_slim && template
  clean
elif [ "$ARG1" == "test" ]; then
  clean && build && static && build_tests && ./tests.exe
  rm -f tests.exe tests.o
elif [ "$ARG1" == "testlog" ]; then
  clean && build && static && build_tests && run_and_log_tests
  rm -f tests.exe tests.o
elif [ "$ARG1" == "build-glfw" ]; then
  build_glfw
elif [ "$ARG1" == "build-glad" ]; then
  build_glad
elif [ "$ARG1" == "validate-glsl" ]; then
  validate_glsl
elif [ "$ARG1" == "parse-obj" ]; then
  parse_obj
else
  echo "You must specify a provided command."
  usage
fi