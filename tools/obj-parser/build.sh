if [ "$1" == "build" ]
then
  rm -rf bin/parser && \
  gcc main.c parser.c \
  ../../static/tail.a \
  -I../../include \
  -o bin/parser
elif [ "$1" == "test" ]
then
  rm -rf test_report.txt && \
  gcc tests.c parser.c \
  -I../../include \
  ../../static/tail.a \
  -o bin/tests && \
  ./bin/tests > test_report.txt && \
  rm -rf bin/tests
fi