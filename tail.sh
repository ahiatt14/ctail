target="i686-w64-mingw32-gcc"
options="-Isrc/headers -O2 -Wall"

build() {
  rm -rf obj
  mkdir obj
  ${target} -c src/math/m4x4.c -o obj/m4x4.o ${options}
  ${target} -c src/math/vec4.c -o obj/vec4.o ${options}
  ${target} -c src/math/tail_math.c -o obj/tail_math.o ${options}
  ${target} -c src/camera.c -o obj/camera.o ${options}
  ${target} -c src/space.c -o obj/space.o ${options}
  ${target} -c src/precision.c -o obj/precision.o ${options}
  ${target} -c src/viewport.c -o obj/viewport.o ${options}
}
static() {
  rm -rf static
  mkdir static
  ar -crs static/tail.a \
  obj/m4x4.o \
  obj/vec4.o \
  obj/tail_math.o \
  obj/camera.o \
  obj/precision.o \
  obj/space.o \
  obj/viewport.o
}
run_and_log_tests() {
  ./tests.exe &> test_report.txt
}
build_tests() {
  rm -rf tests.o && \
  ${target} -c tests.c -o tests.o -Iinclude -Wall && \
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
fi