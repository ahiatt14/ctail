target="i686-w64-mingw32-gcc"
options="-Isrc/headers -Wall"

build() {
  rm -rf obj tests.exe
  mkdir obj
  ${target} -c src/math/m4x4.c -o obj/m4x4.o ${options}
  ${target} -c src/math/vec4.c -o obj/vec4.o ${options}
  ${target} -c src/math/tail_math.c -o obj/tail_math.o ${options}
  ${target} -c src/camera.c -o obj/camera.o ${options}
  ${target} -c src/worldspace.c -o obj/worldspace.o ${options}
}
test() {
  build && \
  rm -rf tests.o && \
  ${target} -c tests.c -o tests.o -Iinclude -Wall && \
  ${target} -o tests.exe \
  tests.o \
  obj/m4x4.o \
  obj/vec4.o \
  obj/tail_math.o \
  obj/camera.o \
  obj/worldspace.o -Wall -Iinclude && \
  ./tests.exe &> test_report.txt
}

if [ "$1" == "build" ]
then
  build
elif [ "$1" == "test" ]
then
  build && test
  rm -rf tests.exe tests.o
fi