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
  -o bin/tests.exe \
  -I../../include -Itest_data \
  ../../static/tail.a -Wall && \
  ./bin/tests.exe > test_report.txt && \
  rm -rf bin/tests.exe
fi