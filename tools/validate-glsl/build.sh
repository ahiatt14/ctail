rm -rf obj &&
mkdir obj &&
i686-w64-mingw32-gcc -Wall -c -o obj/main.o main.c \
-I../../libs/GLAD/include/glad -I../../libs/GLFW/include && \
i686-w64-mingw32-gcc -Wall -o bin/validate-glsl.exe \
obj/main.o \
../../libs/GLAD/obj/glad.o \
../../libs/GLFW/obj/*.c.obj \
-lgdi32