# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /home/utnso/tp-2019-1c-Gank-mid

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/utnso/tp-2019-1c-Gank-mid/cmake-build-debug

# Include any dependencies generated for this target.
include Kernel/CMakeFiles/Kernel.dir/depend.make

# Include the progress variables for this target.
include Kernel/CMakeFiles/Kernel.dir/progress.make

# Include the compile flags for this target's objects.
include Kernel/CMakeFiles/Kernel.dir/flags.make

Kernel/CMakeFiles/Kernel.dir/src/Kernel.c.o: Kernel/CMakeFiles/Kernel.dir/flags.make
Kernel/CMakeFiles/Kernel.dir/src/Kernel.c.o: ../Kernel/src/Kernel.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/utnso/tp-2019-1c-Gank-mid/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object Kernel/CMakeFiles/Kernel.dir/src/Kernel.c.o"
	cd /home/utnso/tp-2019-1c-Gank-mid/cmake-build-debug/Kernel && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Kernel.dir/src/Kernel.c.o   -c /home/utnso/tp-2019-1c-Gank-mid/Kernel/src/Kernel.c

Kernel/CMakeFiles/Kernel.dir/src/Kernel.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Kernel.dir/src/Kernel.c.i"
	cd /home/utnso/tp-2019-1c-Gank-mid/cmake-build-debug/Kernel && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/utnso/tp-2019-1c-Gank-mid/Kernel/src/Kernel.c > CMakeFiles/Kernel.dir/src/Kernel.c.i

Kernel/CMakeFiles/Kernel.dir/src/Kernel.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Kernel.dir/src/Kernel.c.s"
	cd /home/utnso/tp-2019-1c-Gank-mid/cmake-build-debug/Kernel && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/utnso/tp-2019-1c-Gank-mid/Kernel/src/Kernel.c -o CMakeFiles/Kernel.dir/src/Kernel.c.s

Kernel/CMakeFiles/Kernel.dir/src/Kernel.c.o.requires:

.PHONY : Kernel/CMakeFiles/Kernel.dir/src/Kernel.c.o.requires

Kernel/CMakeFiles/Kernel.dir/src/Kernel.c.o.provides: Kernel/CMakeFiles/Kernel.dir/src/Kernel.c.o.requires
	$(MAKE) -f Kernel/CMakeFiles/Kernel.dir/build.make Kernel/CMakeFiles/Kernel.dir/src/Kernel.c.o.provides.build
.PHONY : Kernel/CMakeFiles/Kernel.dir/src/Kernel.c.o.provides

Kernel/CMakeFiles/Kernel.dir/src/Kernel.c.o.provides.build: Kernel/CMakeFiles/Kernel.dir/src/Kernel.c.o


Kernel/CMakeFiles/Kernel.dir/src/config/Config_kernel.c.o: Kernel/CMakeFiles/Kernel.dir/flags.make
Kernel/CMakeFiles/Kernel.dir/src/config/Config_kernel.c.o: ../Kernel/src/config/Config_kernel.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/utnso/tp-2019-1c-Gank-mid/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object Kernel/CMakeFiles/Kernel.dir/src/config/Config_kernel.c.o"
	cd /home/utnso/tp-2019-1c-Gank-mid/cmake-build-debug/Kernel && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Kernel.dir/src/config/Config_kernel.c.o   -c /home/utnso/tp-2019-1c-Gank-mid/Kernel/src/config/Config_kernel.c

Kernel/CMakeFiles/Kernel.dir/src/config/Config_kernel.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Kernel.dir/src/config/Config_kernel.c.i"
	cd /home/utnso/tp-2019-1c-Gank-mid/cmake-build-debug/Kernel && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/utnso/tp-2019-1c-Gank-mid/Kernel/src/config/Config_kernel.c > CMakeFiles/Kernel.dir/src/config/Config_kernel.c.i

Kernel/CMakeFiles/Kernel.dir/src/config/Config_kernel.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Kernel.dir/src/config/Config_kernel.c.s"
	cd /home/utnso/tp-2019-1c-Gank-mid/cmake-build-debug/Kernel && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/utnso/tp-2019-1c-Gank-mid/Kernel/src/config/Config_kernel.c -o CMakeFiles/Kernel.dir/src/config/Config_kernel.c.s

Kernel/CMakeFiles/Kernel.dir/src/config/Config_kernel.c.o.requires:

.PHONY : Kernel/CMakeFiles/Kernel.dir/src/config/Config_kernel.c.o.requires

Kernel/CMakeFiles/Kernel.dir/src/config/Config_kernel.c.o.provides: Kernel/CMakeFiles/Kernel.dir/src/config/Config_kernel.c.o.requires
	$(MAKE) -f Kernel/CMakeFiles/Kernel.dir/build.make Kernel/CMakeFiles/Kernel.dir/src/config/Config_kernel.c.o.provides.build
.PHONY : Kernel/CMakeFiles/Kernel.dir/src/config/Config_kernel.c.o.provides

Kernel/CMakeFiles/Kernel.dir/src/config/Config_kernel.c.o.provides.build: Kernel/CMakeFiles/Kernel.dir/src/config/Config_kernel.c.o


Kernel/CMakeFiles/Kernel.dir/src/comandos/comandos.c.o: Kernel/CMakeFiles/Kernel.dir/flags.make
Kernel/CMakeFiles/Kernel.dir/src/comandos/comandos.c.o: ../Kernel/src/comandos/comandos.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/utnso/tp-2019-1c-Gank-mid/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object Kernel/CMakeFiles/Kernel.dir/src/comandos/comandos.c.o"
	cd /home/utnso/tp-2019-1c-Gank-mid/cmake-build-debug/Kernel && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Kernel.dir/src/comandos/comandos.c.o   -c /home/utnso/tp-2019-1c-Gank-mid/Kernel/src/comandos/comandos.c

Kernel/CMakeFiles/Kernel.dir/src/comandos/comandos.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Kernel.dir/src/comandos/comandos.c.i"
	cd /home/utnso/tp-2019-1c-Gank-mid/cmake-build-debug/Kernel && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/utnso/tp-2019-1c-Gank-mid/Kernel/src/comandos/comandos.c > CMakeFiles/Kernel.dir/src/comandos/comandos.c.i

Kernel/CMakeFiles/Kernel.dir/src/comandos/comandos.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Kernel.dir/src/comandos/comandos.c.s"
	cd /home/utnso/tp-2019-1c-Gank-mid/cmake-build-debug/Kernel && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/utnso/tp-2019-1c-Gank-mid/Kernel/src/comandos/comandos.c -o CMakeFiles/Kernel.dir/src/comandos/comandos.c.s

Kernel/CMakeFiles/Kernel.dir/src/comandos/comandos.c.o.requires:

.PHONY : Kernel/CMakeFiles/Kernel.dir/src/comandos/comandos.c.o.requires

Kernel/CMakeFiles/Kernel.dir/src/comandos/comandos.c.o.provides: Kernel/CMakeFiles/Kernel.dir/src/comandos/comandos.c.o.requires
	$(MAKE) -f Kernel/CMakeFiles/Kernel.dir/build.make Kernel/CMakeFiles/Kernel.dir/src/comandos/comandos.c.o.provides.build
.PHONY : Kernel/CMakeFiles/Kernel.dir/src/comandos/comandos.c.o.provides

Kernel/CMakeFiles/Kernel.dir/src/comandos/comandos.c.o.provides.build: Kernel/CMakeFiles/Kernel.dir/src/comandos/comandos.c.o


# Object files for target Kernel
Kernel_OBJECTS = \
"CMakeFiles/Kernel.dir/src/Kernel.c.o" \
"CMakeFiles/Kernel.dir/src/config/Config_kernel.c.o" \
"CMakeFiles/Kernel.dir/src/comandos/comandos.c.o"

# External object files for target Kernel
Kernel_EXTERNAL_OBJECTS =

Kernel/Kernel: Kernel/CMakeFiles/Kernel.dir/src/Kernel.c.o
Kernel/Kernel: Kernel/CMakeFiles/Kernel.dir/src/config/Config_kernel.c.o
Kernel/Kernel: Kernel/CMakeFiles/Kernel.dir/src/comandos/comandos.c.o
Kernel/Kernel: Kernel/CMakeFiles/Kernel.dir/build.make
Kernel/Kernel: Librerias/libLibrerias.a
Kernel/Kernel: Kernel/CMakeFiles/Kernel.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/utnso/tp-2019-1c-Gank-mid/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable Kernel"
	cd /home/utnso/tp-2019-1c-Gank-mid/cmake-build-debug/Kernel && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Kernel.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Kernel/CMakeFiles/Kernel.dir/build: Kernel/Kernel

.PHONY : Kernel/CMakeFiles/Kernel.dir/build

Kernel/CMakeFiles/Kernel.dir/requires: Kernel/CMakeFiles/Kernel.dir/src/Kernel.c.o.requires
Kernel/CMakeFiles/Kernel.dir/requires: Kernel/CMakeFiles/Kernel.dir/src/config/Config_kernel.c.o.requires
Kernel/CMakeFiles/Kernel.dir/requires: Kernel/CMakeFiles/Kernel.dir/src/comandos/comandos.c.o.requires

.PHONY : Kernel/CMakeFiles/Kernel.dir/requires

Kernel/CMakeFiles/Kernel.dir/clean:
	cd /home/utnso/tp-2019-1c-Gank-mid/cmake-build-debug/Kernel && $(CMAKE_COMMAND) -P CMakeFiles/Kernel.dir/cmake_clean.cmake
.PHONY : Kernel/CMakeFiles/Kernel.dir/clean

Kernel/CMakeFiles/Kernel.dir/depend:
	cd /home/utnso/tp-2019-1c-Gank-mid/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/utnso/tp-2019-1c-Gank-mid /home/utnso/tp-2019-1c-Gank-mid/Kernel /home/utnso/tp-2019-1c-Gank-mid/cmake-build-debug /home/utnso/tp-2019-1c-Gank-mid/cmake-build-debug/Kernel /home/utnso/tp-2019-1c-Gank-mid/cmake-build-debug/Kernel/CMakeFiles/Kernel.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Kernel/CMakeFiles/Kernel.dir/depend
