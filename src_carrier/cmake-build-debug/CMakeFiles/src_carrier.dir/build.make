# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

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
CMAKE_COMMAND = /home/atmelfan/intellij/clion-2017.1/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/atmelfan/intellij/clion-2017.1/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/atmelfan/documents/ASH-MKII/src_carrier

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/atmelfan/documents/ASH-MKII/src_carrier/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/src_carrier.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/src_carrier.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/src_carrier.dir/flags.make

CMakeFiles/src_carrier.dir/main.c.o: CMakeFiles/src_carrier.dir/flags.make
CMakeFiles/src_carrier.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/atmelfan/documents/ASH-MKII/src_carrier/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/src_carrier.dir/main.c.o"
	/usr/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/src_carrier.dir/main.c.o   -c /home/atmelfan/documents/ASH-MKII/src_carrier/main.c

CMakeFiles/src_carrier.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/src_carrier.dir/main.c.i"
	/usr/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/atmelfan/documents/ASH-MKII/src_carrier/main.c > CMakeFiles/src_carrier.dir/main.c.i

CMakeFiles/src_carrier.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/src_carrier.dir/main.c.s"
	/usr/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/atmelfan/documents/ASH-MKII/src_carrier/main.c -o CMakeFiles/src_carrier.dir/main.c.s

CMakeFiles/src_carrier.dir/main.c.o.requires:

.PHONY : CMakeFiles/src_carrier.dir/main.c.o.requires

CMakeFiles/src_carrier.dir/main.c.o.provides: CMakeFiles/src_carrier.dir/main.c.o.requires
	$(MAKE) -f CMakeFiles/src_carrier.dir/build.make CMakeFiles/src_carrier.dir/main.c.o.provides.build
.PHONY : CMakeFiles/src_carrier.dir/main.c.o.provides

CMakeFiles/src_carrier.dir/main.c.o.provides.build: CMakeFiles/src_carrier.dir/main.c.o


CMakeFiles/src_carrier.dir/leg.c.o: CMakeFiles/src_carrier.dir/flags.make
CMakeFiles/src_carrier.dir/leg.c.o: ../leg.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/atmelfan/documents/ASH-MKII/src_carrier/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/src_carrier.dir/leg.c.o"
	/usr/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/src_carrier.dir/leg.c.o   -c /home/atmelfan/documents/ASH-MKII/src_carrier/leg.c

CMakeFiles/src_carrier.dir/leg.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/src_carrier.dir/leg.c.i"
	/usr/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/atmelfan/documents/ASH-MKII/src_carrier/leg.c > CMakeFiles/src_carrier.dir/leg.c.i

CMakeFiles/src_carrier.dir/leg.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/src_carrier.dir/leg.c.s"
	/usr/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/atmelfan/documents/ASH-MKII/src_carrier/leg.c -o CMakeFiles/src_carrier.dir/leg.c.s

CMakeFiles/src_carrier.dir/leg.c.o.requires:

.PHONY : CMakeFiles/src_carrier.dir/leg.c.o.requires

CMakeFiles/src_carrier.dir/leg.c.o.provides: CMakeFiles/src_carrier.dir/leg.c.o.requires
	$(MAKE) -f CMakeFiles/src_carrier.dir/build.make CMakeFiles/src_carrier.dir/leg.c.o.provides.build
.PHONY : CMakeFiles/src_carrier.dir/leg.c.o.provides

CMakeFiles/src_carrier.dir/leg.c.o.provides.build: CMakeFiles/src_carrier.dir/leg.c.o


CMakeFiles/src_carrier.dir/math/linalg_vec.c.o: CMakeFiles/src_carrier.dir/flags.make
CMakeFiles/src_carrier.dir/math/linalg_vec.c.o: ../math/linalg_vec.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/atmelfan/documents/ASH-MKII/src_carrier/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/src_carrier.dir/math/linalg_vec.c.o"
	/usr/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/src_carrier.dir/math/linalg_vec.c.o   -c /home/atmelfan/documents/ASH-MKII/src_carrier/math/linalg_vec.c

CMakeFiles/src_carrier.dir/math/linalg_vec.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/src_carrier.dir/math/linalg_vec.c.i"
	/usr/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/atmelfan/documents/ASH-MKII/src_carrier/math/linalg_vec.c > CMakeFiles/src_carrier.dir/math/linalg_vec.c.i

CMakeFiles/src_carrier.dir/math/linalg_vec.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/src_carrier.dir/math/linalg_vec.c.s"
	/usr/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/atmelfan/documents/ASH-MKII/src_carrier/math/linalg_vec.c -o CMakeFiles/src_carrier.dir/math/linalg_vec.c.s

CMakeFiles/src_carrier.dir/math/linalg_vec.c.o.requires:

.PHONY : CMakeFiles/src_carrier.dir/math/linalg_vec.c.o.requires

CMakeFiles/src_carrier.dir/math/linalg_vec.c.o.provides: CMakeFiles/src_carrier.dir/math/linalg_vec.c.o.requires
	$(MAKE) -f CMakeFiles/src_carrier.dir/build.make CMakeFiles/src_carrier.dir/math/linalg_vec.c.o.provides.build
.PHONY : CMakeFiles/src_carrier.dir/math/linalg_vec.c.o.provides

CMakeFiles/src_carrier.dir/math/linalg_vec.c.o.provides.build: CMakeFiles/src_carrier.dir/math/linalg_vec.c.o


CMakeFiles/src_carrier.dir/math/linalg_mat.c.o: CMakeFiles/src_carrier.dir/flags.make
CMakeFiles/src_carrier.dir/math/linalg_mat.c.o: ../math/linalg_mat.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/atmelfan/documents/ASH-MKII/src_carrier/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/src_carrier.dir/math/linalg_mat.c.o"
	/usr/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/src_carrier.dir/math/linalg_mat.c.o   -c /home/atmelfan/documents/ASH-MKII/src_carrier/math/linalg_mat.c

CMakeFiles/src_carrier.dir/math/linalg_mat.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/src_carrier.dir/math/linalg_mat.c.i"
	/usr/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/atmelfan/documents/ASH-MKII/src_carrier/math/linalg_mat.c > CMakeFiles/src_carrier.dir/math/linalg_mat.c.i

CMakeFiles/src_carrier.dir/math/linalg_mat.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/src_carrier.dir/math/linalg_mat.c.s"
	/usr/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/atmelfan/documents/ASH-MKII/src_carrier/math/linalg_mat.c -o CMakeFiles/src_carrier.dir/math/linalg_mat.c.s

CMakeFiles/src_carrier.dir/math/linalg_mat.c.o.requires:

.PHONY : CMakeFiles/src_carrier.dir/math/linalg_mat.c.o.requires

CMakeFiles/src_carrier.dir/math/linalg_mat.c.o.provides: CMakeFiles/src_carrier.dir/math/linalg_mat.c.o.requires
	$(MAKE) -f CMakeFiles/src_carrier.dir/build.make CMakeFiles/src_carrier.dir/math/linalg_mat.c.o.provides.build
.PHONY : CMakeFiles/src_carrier.dir/math/linalg_mat.c.o.provides

CMakeFiles/src_carrier.dir/math/linalg_mat.c.o.provides.build: CMakeFiles/src_carrier.dir/math/linalg_mat.c.o


CMakeFiles/src_carrier.dir/math/linalg_vecmat.c.o: CMakeFiles/src_carrier.dir/flags.make
CMakeFiles/src_carrier.dir/math/linalg_vecmat.c.o: ../math/linalg_vecmat.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/atmelfan/documents/ASH-MKII/src_carrier/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/src_carrier.dir/math/linalg_vecmat.c.o"
	/usr/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/src_carrier.dir/math/linalg_vecmat.c.o   -c /home/atmelfan/documents/ASH-MKII/src_carrier/math/linalg_vecmat.c

CMakeFiles/src_carrier.dir/math/linalg_vecmat.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/src_carrier.dir/math/linalg_vecmat.c.i"
	/usr/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/atmelfan/documents/ASH-MKII/src_carrier/math/linalg_vecmat.c > CMakeFiles/src_carrier.dir/math/linalg_vecmat.c.i

CMakeFiles/src_carrier.dir/math/linalg_vecmat.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/src_carrier.dir/math/linalg_vecmat.c.s"
	/usr/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/atmelfan/documents/ASH-MKII/src_carrier/math/linalg_vecmat.c -o CMakeFiles/src_carrier.dir/math/linalg_vecmat.c.s

CMakeFiles/src_carrier.dir/math/linalg_vecmat.c.o.requires:

.PHONY : CMakeFiles/src_carrier.dir/math/linalg_vecmat.c.o.requires

CMakeFiles/src_carrier.dir/math/linalg_vecmat.c.o.provides: CMakeFiles/src_carrier.dir/math/linalg_vecmat.c.o.requires
	$(MAKE) -f CMakeFiles/src_carrier.dir/build.make CMakeFiles/src_carrier.dir/math/linalg_vecmat.c.o.provides.build
.PHONY : CMakeFiles/src_carrier.dir/math/linalg_vecmat.c.o.provides

CMakeFiles/src_carrier.dir/math/linalg_vecmat.c.o.provides.build: CMakeFiles/src_carrier.dir/math/linalg_vecmat.c.o


CMakeFiles/src_carrier.dir/drivers/pca9685.c.o: CMakeFiles/src_carrier.dir/flags.make
CMakeFiles/src_carrier.dir/drivers/pca9685.c.o: ../drivers/pca9685.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/atmelfan/documents/ASH-MKII/src_carrier/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/src_carrier.dir/drivers/pca9685.c.o"
	/usr/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/src_carrier.dir/drivers/pca9685.c.o   -c /home/atmelfan/documents/ASH-MKII/src_carrier/drivers/pca9685.c

CMakeFiles/src_carrier.dir/drivers/pca9685.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/src_carrier.dir/drivers/pca9685.c.i"
	/usr/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/atmelfan/documents/ASH-MKII/src_carrier/drivers/pca9685.c > CMakeFiles/src_carrier.dir/drivers/pca9685.c.i

CMakeFiles/src_carrier.dir/drivers/pca9685.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/src_carrier.dir/drivers/pca9685.c.s"
	/usr/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/atmelfan/documents/ASH-MKII/src_carrier/drivers/pca9685.c -o CMakeFiles/src_carrier.dir/drivers/pca9685.c.s

CMakeFiles/src_carrier.dir/drivers/pca9685.c.o.requires:

.PHONY : CMakeFiles/src_carrier.dir/drivers/pca9685.c.o.requires

CMakeFiles/src_carrier.dir/drivers/pca9685.c.o.provides: CMakeFiles/src_carrier.dir/drivers/pca9685.c.o.requires
	$(MAKE) -f CMakeFiles/src_carrier.dir/build.make CMakeFiles/src_carrier.dir/drivers/pca9685.c.o.provides.build
.PHONY : CMakeFiles/src_carrier.dir/drivers/pca9685.c.o.provides

CMakeFiles/src_carrier.dir/drivers/pca9685.c.o.provides.build: CMakeFiles/src_carrier.dir/drivers/pca9685.c.o


CMakeFiles/src_carrier.dir/drivers/mpu6000.c.o: CMakeFiles/src_carrier.dir/flags.make
CMakeFiles/src_carrier.dir/drivers/mpu6000.c.o: ../drivers/mpu6000.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/atmelfan/documents/ASH-MKII/src_carrier/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/src_carrier.dir/drivers/mpu6000.c.o"
	/usr/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/src_carrier.dir/drivers/mpu6000.c.o   -c /home/atmelfan/documents/ASH-MKII/src_carrier/drivers/mpu6000.c

CMakeFiles/src_carrier.dir/drivers/mpu6000.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/src_carrier.dir/drivers/mpu6000.c.i"
	/usr/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/atmelfan/documents/ASH-MKII/src_carrier/drivers/mpu6000.c > CMakeFiles/src_carrier.dir/drivers/mpu6000.c.i

CMakeFiles/src_carrier.dir/drivers/mpu6000.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/src_carrier.dir/drivers/mpu6000.c.s"
	/usr/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/atmelfan/documents/ASH-MKII/src_carrier/drivers/mpu6000.c -o CMakeFiles/src_carrier.dir/drivers/mpu6000.c.s

CMakeFiles/src_carrier.dir/drivers/mpu6000.c.o.requires:

.PHONY : CMakeFiles/src_carrier.dir/drivers/mpu6000.c.o.requires

CMakeFiles/src_carrier.dir/drivers/mpu6000.c.o.provides: CMakeFiles/src_carrier.dir/drivers/mpu6000.c.o.requires
	$(MAKE) -f CMakeFiles/src_carrier.dir/build.make CMakeFiles/src_carrier.dir/drivers/mpu6000.c.o.provides.build
.PHONY : CMakeFiles/src_carrier.dir/drivers/mpu6000.c.o.provides

CMakeFiles/src_carrier.dir/drivers/mpu6000.c.o.provides.build: CMakeFiles/src_carrier.dir/drivers/mpu6000.c.o


# Object files for target src_carrier
src_carrier_OBJECTS = \
"CMakeFiles/src_carrier.dir/main.c.o" \
"CMakeFiles/src_carrier.dir/leg.c.o" \
"CMakeFiles/src_carrier.dir/math/linalg_vec.c.o" \
"CMakeFiles/src_carrier.dir/math/linalg_mat.c.o" \
"CMakeFiles/src_carrier.dir/math/linalg_vecmat.c.o" \
"CMakeFiles/src_carrier.dir/drivers/pca9685.c.o" \
"CMakeFiles/src_carrier.dir/drivers/mpu6000.c.o"

# External object files for target src_carrier
src_carrier_EXTERNAL_OBJECTS =

carrier.elf: CMakeFiles/src_carrier.dir/main.c.o
carrier.elf: CMakeFiles/src_carrier.dir/leg.c.o
carrier.elf: CMakeFiles/src_carrier.dir/math/linalg_vec.c.o
carrier.elf: CMakeFiles/src_carrier.dir/math/linalg_mat.c.o
carrier.elf: CMakeFiles/src_carrier.dir/math/linalg_vecmat.c.o
carrier.elf: CMakeFiles/src_carrier.dir/drivers/pca9685.c.o
carrier.elf: CMakeFiles/src_carrier.dir/drivers/mpu6000.c.o
carrier.elf: CMakeFiles/src_carrier.dir/build.make
carrier.elf: CMakeFiles/src_carrier.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/atmelfan/documents/ASH-MKII/src_carrier/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking C executable carrier.elf"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/src_carrier.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/src_carrier.dir/build: carrier.elf

.PHONY : CMakeFiles/src_carrier.dir/build

CMakeFiles/src_carrier.dir/requires: CMakeFiles/src_carrier.dir/main.c.o.requires
CMakeFiles/src_carrier.dir/requires: CMakeFiles/src_carrier.dir/leg.c.o.requires
CMakeFiles/src_carrier.dir/requires: CMakeFiles/src_carrier.dir/math/linalg_vec.c.o.requires
CMakeFiles/src_carrier.dir/requires: CMakeFiles/src_carrier.dir/math/linalg_mat.c.o.requires
CMakeFiles/src_carrier.dir/requires: CMakeFiles/src_carrier.dir/math/linalg_vecmat.c.o.requires
CMakeFiles/src_carrier.dir/requires: CMakeFiles/src_carrier.dir/drivers/pca9685.c.o.requires
CMakeFiles/src_carrier.dir/requires: CMakeFiles/src_carrier.dir/drivers/mpu6000.c.o.requires

.PHONY : CMakeFiles/src_carrier.dir/requires

CMakeFiles/src_carrier.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/src_carrier.dir/cmake_clean.cmake
.PHONY : CMakeFiles/src_carrier.dir/clean

CMakeFiles/src_carrier.dir/depend:
	cd /home/atmelfan/documents/ASH-MKII/src_carrier/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/atmelfan/documents/ASH-MKII/src_carrier /home/atmelfan/documents/ASH-MKII/src_carrier /home/atmelfan/documents/ASH-MKII/src_carrier/cmake-build-debug /home/atmelfan/documents/ASH-MKII/src_carrier/cmake-build-debug /home/atmelfan/documents/ASH-MKII/src_carrier/cmake-build-debug/CMakeFiles/src_carrier.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/src_carrier.dir/depend

