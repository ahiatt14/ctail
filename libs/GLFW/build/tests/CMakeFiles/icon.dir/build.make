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
include tests/CMakeFiles/icon.dir/depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/icon.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/icon.dir/flags.make

tests/CMakeFiles/icon.dir/icon.c.obj: tests/CMakeFiles/icon.dir/flags.make
tests/CMakeFiles/icon.dir/icon.c.obj: tests/CMakeFiles/icon.dir/includes_C.rsp
tests/CMakeFiles/icon.dir/icon.c.obj: ../tests/icon.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/d/c/tail/libs/glfw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object tests/CMakeFiles/icon.dir/icon.c.obj"
	cd /mnt/d/c/tail/libs/glfw/build/tests && /usr/bin/i686-w64-mingw32-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/icon.dir/icon.c.obj   -c /mnt/d/c/tail/libs/glfw/tests/icon.c

tests/CMakeFiles/icon.dir/icon.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/icon.dir/icon.c.i"
	cd /mnt/d/c/tail/libs/glfw/build/tests && /usr/bin/i686-w64-mingw32-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/d/c/tail/libs/glfw/tests/icon.c > CMakeFiles/icon.dir/icon.c.i

tests/CMakeFiles/icon.dir/icon.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/icon.dir/icon.c.s"
	cd /mnt/d/c/tail/libs/glfw/build/tests && /usr/bin/i686-w64-mingw32-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/d/c/tail/libs/glfw/tests/icon.c -o CMakeFiles/icon.dir/icon.c.s

tests/CMakeFiles/icon.dir/__/deps/glad_gl.c.obj: tests/CMakeFiles/icon.dir/flags.make
tests/CMakeFiles/icon.dir/__/deps/glad_gl.c.obj: tests/CMakeFiles/icon.dir/includes_C.rsp
tests/CMakeFiles/icon.dir/__/deps/glad_gl.c.obj: ../deps/glad_gl.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/d/c/tail/libs/glfw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object tests/CMakeFiles/icon.dir/__/deps/glad_gl.c.obj"
	cd /mnt/d/c/tail/libs/glfw/build/tests && /usr/bin/i686-w64-mingw32-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/icon.dir/__/deps/glad_gl.c.obj   -c /mnt/d/c/tail/libs/glfw/deps/glad_gl.c

tests/CMakeFiles/icon.dir/__/deps/glad_gl.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/icon.dir/__/deps/glad_gl.c.i"
	cd /mnt/d/c/tail/libs/glfw/build/tests && /usr/bin/i686-w64-mingw32-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/d/c/tail/libs/glfw/deps/glad_gl.c > CMakeFiles/icon.dir/__/deps/glad_gl.c.i

tests/CMakeFiles/icon.dir/__/deps/glad_gl.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/icon.dir/__/deps/glad_gl.c.s"
	cd /mnt/d/c/tail/libs/glfw/build/tests && /usr/bin/i686-w64-mingw32-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/d/c/tail/libs/glfw/deps/glad_gl.c -o CMakeFiles/icon.dir/__/deps/glad_gl.c.s

# Object files for target icon
icon_OBJECTS = \
"CMakeFiles/icon.dir/icon.c.obj" \
"CMakeFiles/icon.dir/__/deps/glad_gl.c.obj"

# External object files for target icon
icon_EXTERNAL_OBJECTS =

tests/icon.exe: tests/CMakeFiles/icon.dir/icon.c.obj
tests/icon.exe: tests/CMakeFiles/icon.dir/__/deps/glad_gl.c.obj
tests/icon.exe: tests/CMakeFiles/icon.dir/build.make
tests/icon.exe: src/libglfw3.a
tests/icon.exe: tests/CMakeFiles/icon.dir/linklibs.rsp
tests/icon.exe: tests/CMakeFiles/icon.dir/objects1.rsp
tests/icon.exe: tests/CMakeFiles/icon.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/d/c/tail/libs/glfw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable icon.exe"
	cd /mnt/d/c/tail/libs/glfw/build/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/icon.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/icon.dir/build: tests/icon.exe

.PHONY : tests/CMakeFiles/icon.dir/build

tests/CMakeFiles/icon.dir/clean:
	cd /mnt/d/c/tail/libs/glfw/build/tests && $(CMAKE_COMMAND) -P CMakeFiles/icon.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/icon.dir/clean

tests/CMakeFiles/icon.dir/depend:
	cd /mnt/d/c/tail/libs/glfw/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/d/c/tail/libs/glfw /mnt/d/c/tail/libs/glfw/tests /mnt/d/c/tail/libs/glfw/build /mnt/d/c/tail/libs/glfw/build/tests /mnt/d/c/tail/libs/glfw/build/tests/CMakeFiles/icon.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/icon.dir/depend
