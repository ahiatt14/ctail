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
include examples/CMakeFiles/gears.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/gears.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/gears.dir/flags.make

examples/CMakeFiles/gears.dir/gears.c.obj: examples/CMakeFiles/gears.dir/flags.make
examples/CMakeFiles/gears.dir/gears.c.obj: examples/CMakeFiles/gears.dir/includes_C.rsp
examples/CMakeFiles/gears.dir/gears.c.obj: ../examples/gears.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/d/c/tail/libs/glfw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object examples/CMakeFiles/gears.dir/gears.c.obj"
	cd /mnt/d/c/tail/libs/glfw/build/examples && /usr/bin/i686-w64-mingw32-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/gears.dir/gears.c.obj   -c /mnt/d/c/tail/libs/glfw/examples/gears.c

examples/CMakeFiles/gears.dir/gears.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/gears.dir/gears.c.i"
	cd /mnt/d/c/tail/libs/glfw/build/examples && /usr/bin/i686-w64-mingw32-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/d/c/tail/libs/glfw/examples/gears.c > CMakeFiles/gears.dir/gears.c.i

examples/CMakeFiles/gears.dir/gears.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/gears.dir/gears.c.s"
	cd /mnt/d/c/tail/libs/glfw/build/examples && /usr/bin/i686-w64-mingw32-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/d/c/tail/libs/glfw/examples/gears.c -o CMakeFiles/gears.dir/gears.c.s

examples/CMakeFiles/gears.dir/glfw.rc.res: examples/CMakeFiles/gears.dir/flags.make
examples/CMakeFiles/gears.dir/glfw.rc.res: ../examples/glfw.rc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/d/c/tail/libs/glfw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building RC object examples/CMakeFiles/gears.dir/glfw.rc.res"
	cd /mnt/d/c/tail/libs/glfw/build/examples && i686-w64-mingw32-windres -O coff $(RC_DEFINES) $(RC_INCLUDES) $(RC_FLAGS) /mnt/d/c/tail/libs/glfw/examples/glfw.rc CMakeFiles/gears.dir/glfw.rc.res

examples/CMakeFiles/gears.dir/__/deps/glad_gl.c.obj: examples/CMakeFiles/gears.dir/flags.make
examples/CMakeFiles/gears.dir/__/deps/glad_gl.c.obj: examples/CMakeFiles/gears.dir/includes_C.rsp
examples/CMakeFiles/gears.dir/__/deps/glad_gl.c.obj: ../deps/glad_gl.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/d/c/tail/libs/glfw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object examples/CMakeFiles/gears.dir/__/deps/glad_gl.c.obj"
	cd /mnt/d/c/tail/libs/glfw/build/examples && /usr/bin/i686-w64-mingw32-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/gears.dir/__/deps/glad_gl.c.obj   -c /mnt/d/c/tail/libs/glfw/deps/glad_gl.c

examples/CMakeFiles/gears.dir/__/deps/glad_gl.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/gears.dir/__/deps/glad_gl.c.i"
	cd /mnt/d/c/tail/libs/glfw/build/examples && /usr/bin/i686-w64-mingw32-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/d/c/tail/libs/glfw/deps/glad_gl.c > CMakeFiles/gears.dir/__/deps/glad_gl.c.i

examples/CMakeFiles/gears.dir/__/deps/glad_gl.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/gears.dir/__/deps/glad_gl.c.s"
	cd /mnt/d/c/tail/libs/glfw/build/examples && /usr/bin/i686-w64-mingw32-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/d/c/tail/libs/glfw/deps/glad_gl.c -o CMakeFiles/gears.dir/__/deps/glad_gl.c.s

# Object files for target gears
gears_OBJECTS = \
"CMakeFiles/gears.dir/gears.c.obj" \
"CMakeFiles/gears.dir/glfw.rc.res" \
"CMakeFiles/gears.dir/__/deps/glad_gl.c.obj"

# External object files for target gears
gears_EXTERNAL_OBJECTS =

examples/gears.exe: examples/CMakeFiles/gears.dir/gears.c.obj
examples/gears.exe: examples/CMakeFiles/gears.dir/glfw.rc.res
examples/gears.exe: examples/CMakeFiles/gears.dir/__/deps/glad_gl.c.obj
examples/gears.exe: examples/CMakeFiles/gears.dir/build.make
examples/gears.exe: src/libglfw3.a
examples/gears.exe: examples/CMakeFiles/gears.dir/linklibs.rsp
examples/gears.exe: examples/CMakeFiles/gears.dir/objects1.rsp
examples/gears.exe: examples/CMakeFiles/gears.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/d/c/tail/libs/glfw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable gears.exe"
	cd /mnt/d/c/tail/libs/glfw/build/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gears.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/gears.dir/build: examples/gears.exe

.PHONY : examples/CMakeFiles/gears.dir/build

examples/CMakeFiles/gears.dir/clean:
	cd /mnt/d/c/tail/libs/glfw/build/examples && $(CMAKE_COMMAND) -P CMakeFiles/gears.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/gears.dir/clean

examples/CMakeFiles/gears.dir/depend:
	cd /mnt/d/c/tail/libs/glfw/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/d/c/tail/libs/glfw /mnt/d/c/tail/libs/glfw/examples /mnt/d/c/tail/libs/glfw/build /mnt/d/c/tail/libs/glfw/build/examples /mnt/d/c/tail/libs/glfw/build/examples/CMakeFiles/gears.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/gears.dir/depend
