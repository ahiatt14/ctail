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
include examples/CMakeFiles/offscreen.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/offscreen.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/offscreen.dir/flags.make

examples/CMakeFiles/offscreen.dir/offscreen.c.obj: examples/CMakeFiles/offscreen.dir/flags.make
examples/CMakeFiles/offscreen.dir/offscreen.c.obj: examples/CMakeFiles/offscreen.dir/includes_C.rsp
examples/CMakeFiles/offscreen.dir/offscreen.c.obj: ../examples/offscreen.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/d/c/tail/libs/glfw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object examples/CMakeFiles/offscreen.dir/offscreen.c.obj"
	cd /mnt/d/c/tail/libs/glfw/build/examples && /usr/bin/i686-w64-mingw32-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/offscreen.dir/offscreen.c.obj   -c /mnt/d/c/tail/libs/glfw/examples/offscreen.c

examples/CMakeFiles/offscreen.dir/offscreen.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/offscreen.dir/offscreen.c.i"
	cd /mnt/d/c/tail/libs/glfw/build/examples && /usr/bin/i686-w64-mingw32-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/d/c/tail/libs/glfw/examples/offscreen.c > CMakeFiles/offscreen.dir/offscreen.c.i

examples/CMakeFiles/offscreen.dir/offscreen.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/offscreen.dir/offscreen.c.s"
	cd /mnt/d/c/tail/libs/glfw/build/examples && /usr/bin/i686-w64-mingw32-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/d/c/tail/libs/glfw/examples/offscreen.c -o CMakeFiles/offscreen.dir/offscreen.c.s

examples/CMakeFiles/offscreen.dir/glfw.rc.res: examples/CMakeFiles/offscreen.dir/flags.make
examples/CMakeFiles/offscreen.dir/glfw.rc.res: ../examples/glfw.rc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/d/c/tail/libs/glfw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building RC object examples/CMakeFiles/offscreen.dir/glfw.rc.res"
	cd /mnt/d/c/tail/libs/glfw/build/examples && i686-w64-mingw32-windres -O coff $(RC_DEFINES) $(RC_INCLUDES) $(RC_FLAGS) /mnt/d/c/tail/libs/glfw/examples/glfw.rc CMakeFiles/offscreen.dir/glfw.rc.res

examples/CMakeFiles/offscreen.dir/__/deps/glad_gl.c.obj: examples/CMakeFiles/offscreen.dir/flags.make
examples/CMakeFiles/offscreen.dir/__/deps/glad_gl.c.obj: examples/CMakeFiles/offscreen.dir/includes_C.rsp
examples/CMakeFiles/offscreen.dir/__/deps/glad_gl.c.obj: ../deps/glad_gl.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/d/c/tail/libs/glfw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object examples/CMakeFiles/offscreen.dir/__/deps/glad_gl.c.obj"
	cd /mnt/d/c/tail/libs/glfw/build/examples && /usr/bin/i686-w64-mingw32-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/offscreen.dir/__/deps/glad_gl.c.obj   -c /mnt/d/c/tail/libs/glfw/deps/glad_gl.c

examples/CMakeFiles/offscreen.dir/__/deps/glad_gl.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/offscreen.dir/__/deps/glad_gl.c.i"
	cd /mnt/d/c/tail/libs/glfw/build/examples && /usr/bin/i686-w64-mingw32-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/d/c/tail/libs/glfw/deps/glad_gl.c > CMakeFiles/offscreen.dir/__/deps/glad_gl.c.i

examples/CMakeFiles/offscreen.dir/__/deps/glad_gl.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/offscreen.dir/__/deps/glad_gl.c.s"
	cd /mnt/d/c/tail/libs/glfw/build/examples && /usr/bin/i686-w64-mingw32-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/d/c/tail/libs/glfw/deps/glad_gl.c -o CMakeFiles/offscreen.dir/__/deps/glad_gl.c.s

# Object files for target offscreen
offscreen_OBJECTS = \
"CMakeFiles/offscreen.dir/offscreen.c.obj" \
"CMakeFiles/offscreen.dir/glfw.rc.res" \
"CMakeFiles/offscreen.dir/__/deps/glad_gl.c.obj"

# External object files for target offscreen
offscreen_EXTERNAL_OBJECTS =

examples/offscreen.exe: examples/CMakeFiles/offscreen.dir/offscreen.c.obj
examples/offscreen.exe: examples/CMakeFiles/offscreen.dir/glfw.rc.res
examples/offscreen.exe: examples/CMakeFiles/offscreen.dir/__/deps/glad_gl.c.obj
examples/offscreen.exe: examples/CMakeFiles/offscreen.dir/build.make
examples/offscreen.exe: src/libglfw3.a
examples/offscreen.exe: examples/CMakeFiles/offscreen.dir/linklibs.rsp
examples/offscreen.exe: examples/CMakeFiles/offscreen.dir/objects1.rsp
examples/offscreen.exe: examples/CMakeFiles/offscreen.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/d/c/tail/libs/glfw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable offscreen.exe"
	cd /mnt/d/c/tail/libs/glfw/build/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/offscreen.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/offscreen.dir/build: examples/offscreen.exe

.PHONY : examples/CMakeFiles/offscreen.dir/build

examples/CMakeFiles/offscreen.dir/clean:
	cd /mnt/d/c/tail/libs/glfw/build/examples && $(CMAKE_COMMAND) -P CMakeFiles/offscreen.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/offscreen.dir/clean

examples/CMakeFiles/offscreen.dir/depend:
	cd /mnt/d/c/tail/libs/glfw/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/d/c/tail/libs/glfw /mnt/d/c/tail/libs/glfw/examples /mnt/d/c/tail/libs/glfw/build /mnt/d/c/tail/libs/glfw/build/examples /mnt/d/c/tail/libs/glfw/build/examples/CMakeFiles/offscreen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/offscreen.dir/depend

