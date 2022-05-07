default_target="win32"
wsl_abs_path_to_root="/mnt/d/c/tail/"
options="-O2 -Wall"
includes="-Isrc/headers -Ilibs/GLAD/include -Ilibes/GLFW/include"

declare -A targets
targets[win32]=i686-w64-mingw32-gcc

usage() {
  echo "
    usage: tail [--target|-t] <command>

    Commands
    clean          delete tail build artifacts
    build-glad     compile glad opengl loader into obj
    build-glfw     compile glfw into local static library
    build          compile tail src into obj
    static         compile tail into static lib
    test           build tail, run and log tests to file
  "
}
clean() {
  rm -rf obj static test_report.txt
}
build() {
  rm -rf obj
  mkdir obj
  ${targets[${default_target}]} -c src/math/m2x2.c -o obj/m2x2.o ${options}
  ${targets[${default_target}]} -c src/math/m3x3.c -o obj/m3x3.o ${options}
  ${targets[${default_target}]} -c src/math/m4x4.c -o obj/m4x4.o ${options}
  ${targets[${default_target}]} -c src/math/vec3.c -o obj/vec3.o ${options}
  ${targets[${default_target}]} -c src/opengl_gpu.c -o obj/opengl_gpu.o ${options}
  ${targets[${default_target}]} -c src/gl_util.c -o obj/gl_util.o ${options}
  ${targets[${default_target}]} -c src/debugger.c -o obj/debugger.o ${options}
  ${targets[${default_target}]} -c src/camera.c -o obj/camera.o ${options}
  ${targets[${default_target}]} -c src/glfw_window.c -o obj/glfw_window.o ${options}
  ${targets[${default_target}]} -c src/space.c -o obj/space.o ${options}
  ${targets[${default_target}]} -c src/precision.c -o obj/precision.o ${options}
  ${targets[${default_target}]} -c src/math/tail_math.c -o obj/tail_math.o ${options}
  ${targets[${default_target}]} -c src/viewport.c -o obj/viewport.o ${options}
}
static() {
  rm -rf static
  mkdir static
  ar -crs static/tail.a \
  obj/*.o
}
run_and_log_tests() {
  ./tests.exe &> test_report.txt
}
build_glfw() {
  rm -rf libs/glfw/obj libs/glfw/build
  mkdir libs/glfw/obj libs/glfw/build
  cmake -S libs/glfw -B libs/glfw/build \
  -DCMAKE_TOOLCHAIN_FILE=${wsl_abs_path_to_root}libs/glfw/CMake/i686-w64-mingw32.cmake
  cmake --build libs/glfw/build
}
build_glad() {
  rm -rf libs/GLAD/obj
  mkdir libs/GLAD/obj
  ${targets[${default_target}]} -c libs/GLAD/src/glad.c \
  -Ilibs/GLAD/include -o libs/GLAD/obj/glad.o \
  -Wall
}
build_tests() {
  rm -rf tests.o && \
  ${target} -c tests.c -o tests.o -Iinclude -Itest_data -Wall && \
  ${target} -o tests.exe \
  tests.o \
  static/tail.a
}

if [ "$1" == "build" ]
then
  build
elif [ "$1" == "static" ]
then
  build && static
elif [ "$1" == "test" ]
then
  build && static && build_tests && run_and_log_tests
  rm -rf tests.exe tests.o
elif [ "$1" == "build-glfw" ]
then
  build_glfw
elif [ "$1" == "build-glad" ]
then
  build_glad
else
  echo "You must specify a command."
  usage
fi