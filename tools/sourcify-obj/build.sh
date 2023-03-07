# TODO: need to parameterize build target in here
# TODO: the way this build is quite brittle, paricularly the tests

if [ "$1" == "build" ]
then
  rm -rf bin && \
  mkdir bin && \
  i686-w64-mingw32-gcc ../../src/math/tail_math.c ../../src/math/vector.c \
  ../../src/precision.c main.c parser.c normals.c \
  -o bin/sourcify-obj.exe \
  -I../../src/headers \
  -Wall
elif [ "$1" == "test" ]
then
  rm -rf test_report.txt bin && \
  mkdir bin && \
  i686-w64-mingw32-gcc tests.c parser.c normals.c \
  ../../src/math/vector.c ../../src/math/m4x4.c ../../src/math/m3x3.c \
  ../../src/opengl_gpu.c ../../src/math/m2x2.c ../../src/precision.c \
  ../../src/math/tail_math.c \
  ../../libs/GLAD/obj/glad.o \
  -o bin/tests.exe \
  -I../../include -I../../src/headers -Itest_data -I../../libs/GLAD/include/glad \
  -Wall && \
  ./bin/tests.exe && \
  rm -rf bin/tests.exe
fi