# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/d/c/tail/libs/glfw

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/d/c/tail/libs/glfw/build

# Include any dependencies generated for this target.
include tests/CMakeFiles/empty.dir/depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/empty.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/empty.dir/flags.make

tests/CMakeFiles/empty.dir/empty.c.obj: tests/CMakeFiles/empty.dir/flags.make
tests/CMakeFiles/empty.dir/empty.c.obj: tests/CMakeFiles/empty.dir/includes_C.rsp
tests/CMakeFiles/empty.dir/empty.c.obj: ../tests/empty.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/d/c/tail/libs/glfw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object tests/CMakeFiles/empty.dir/empty.c.obj"
	cd /mnt/d/c/tail/libs/glfw/build/tests && /usr/bin/i686-w64-mingw32-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/empty.dir/empty.c.obj   -c /mnt/d/c/tail/libs/glfw/tests/empty.c

tests/CMakeFiles/empty.dir/empty.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/empty.dir/empty.c.i"
	cd /mnt/d/c/tail/libs/glfw/build/tests && /usr/bin/i686-w64-mingw32-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/d/c/tail/libs/glfw/tests/empty.c > CMakeFiles/empty.dir/empty.c.i

tests/CMakeFiles/empty.dir/empty.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/empty.dir/empty.c.s"
	cd /mnt/d/c/tail/libs/glfw/build/tests && /usr/bin/i686-w64-mingw32-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/d/c/tail/libs/glfw/tests/empty.c -o CMakeFiles/empty.dir/empty.c.s

tests/CMakeFiles/empty.dir/__/deps/tinycthread.c.obj: tests/CMakeFiles/empty.dir/flags.make
tests/CMakeFiles/empty.dir/__/deps/tinycthread.c.obj: tests/CMakeFiles/empty.dir/includes_C.rsp
tests/CMakeFiles/empty.dir/__/deps/tinycthread.c.obj: ../deps/tinycthread.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/d/c/tail/libs/glfw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object tests/CMakeFiles/empty.dir/__/deps/tinycthread.c.obj"
	cd /mnt/d/c/tail/libs/glfw/build/tests && /usr/bin/i686-w64-mingw32-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/empty.dir/__/deps/tinycthread.c.obj   -c /mnt/d/c/tail/libs/glfw/deps/tinycthread.c

tests/CMakeFiles/empty.dir/__/deps/tinycthread.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/empty.dir/__/deps/tinycthread.c.i"
	cd /mnt/d/c/tail/libs/glfw/build/tests && /usr/bin/i686-w64-mingw32-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/d/c/tail/libs/glfw/deps/tinycthread.c > CMakeFiles/empty.dir/__/deps/tinycthread.c.i

tests/CMakeFiles/empty.dir/__/deps/tinycthread.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/empty.dir/__/deps/tinycthread.c.s"
	cd /mnt/d/c/tail/libs/glfw/build/tests && /usr/bin/i686-w64-mingw32-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/d/c/tail/libs/glfw/deps/tinycthread.c -o CMakeFiles/empty.dir/__/deps/tinycthread.c.s

tests/CMakeFiles/empty.dir/__/deps/glad_gl.c.obj: tests/CMakeFiles/empty.dir/flags.make
tests/CMakeFiles/empty.dir/__/deps/glad_gl.c.obj: tests/CMakeFiles/empty.dir/includes_C.rsp
tests/CMakeFiles/empty.dir/__/deps/glad_gl.c.obj: ../deps/glad_gl.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/d/c/tail/libs/glfw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object tests/CMakeFiles/empty.dir/__/deps/glad_gl.c.obj"
	cd /mnt/d/c/tail/libs/glfw/build/tests && /usr/bin/i686-w64-mingw32-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/empty.dir/__/deps/glad_gl.c.obj   -c /mnt/d/c/tail/libs/glfw/deps/glad_gl.c

tests/CMakeFiles/empty.dir/__/deps/glad_gl.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/empty.dir/__/deps/glad_gl.c.i"
	cd /mnt/d/c/tail/libs/glfw/build/tests && /usr/bin/i686-w64-mingw32-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/d/c/tail/libs/glfw/deps/glad_gl.c > CMakeFiles/empty.dir/__/deps/glad_gl.c.i

tests/CMakeFiles/empty.dir/__/deps/glad_gl.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/empty.dir/__/deps/glad_gl.c.s"
	cd /mnt/d/c/tail/libs/glfw/build/tests && /usr/bin/i686-w64-mingw32-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/d/c/tail/libs/glfw/deps/glad_gl.c -o CMakeFiles/empty.dir/__/deps/glad_gl.c.s

# Object files for target empty
empty_OBJECTS = \
"CMakeFiles/empty.dir/empty.c.obj" \
"CMakeFiles/empty.dir/__/deps/tinycthread.c.obj" \
"CMakeFiles/empty.dir/__/deps/glad_gl.c.obj"

# External object files for target empty
empty_EXTERNAL_OBJECTS =

tests/empty.exe: tests/CMakeFiles/empty.dir/empty.c.obj
tests/empty.exe: tests/CMakeFiles/empty.dir/__/deps/tinycthread.c.obj
tests/empty.exe: tests/CMakeFiles/empty.dir/__/deps/glad_gl.c.obj
tests/empty.exe: tests/CMakeFiles/empty.dir/build.make
tests/empty.exe: src/libglfw3.a
tests/empty.exe: tests/CMakeFiles/empty.dir/linklibs.rsp
tests/empty.exe: tests/CMakeFiles/empty.dir/objects1.rsp
tests/empty.exe: tests/CMakeFiles/empty.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/d/c/tail/libs/glfw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable empty.exe"
	cd /mnt/d/c/tail/libs/glfw/build/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/empty.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/empty.dir/build: tests/empty.exe

.PHONY : tests/CMakeFiles/empty.dir/build

tests/CMakeFiles/empty.dir/clean:
	cd /mnt/d/c/tail/libs/glfw/build/tests && $(CMAKE_COMMAND) -P CMakeFiles/empty.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/empty.dir/clean

tests/CMakeFiles/empty.dir/depend:
	cd /mnt/d/c/tail/libs/glfw/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/d/c/tail/libs/glfw /mnt/d/c/tail/libs/glfw/tests /mnt/d/c/tail/libs/glfw/build /mnt/d/c/tail/libs/glfw/build/tests /mnt/d/c/tail/libs/glfw/build/tests/CMakeFiles/empty.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/empty.dir/depend

