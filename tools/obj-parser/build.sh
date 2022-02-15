if [ "$1" == "build" ]
then
  rm -rf bin/parser bin && \
  mkdir bin && \
  gcc main.c parser.c \
  -o bin/parser \
  ../../static/tail.a \
  -I../../include \
  -Wall
elif [ "$1" == "test" ]
then
  rm -rf test_report.txt bin && \
  mkdir bin && \
  gcc tests.c parser.c \
  -o bin/tests \
  -I../../include \
  ../../static/tail.a -Wall && \
  ./bin/tests > test_report.txt && \
  rm -rf bin/tests
fi