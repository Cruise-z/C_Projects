# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.20

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "E:\E_drive\clion\CLion 2021.2.2\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "E:\E_drive\clion\CLion 2021.2.2\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "E:\E_drive\clion\Project List\cryptography\My_BigNum_GF_cal"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "E:\E_drive\clion\Project List\cryptography\My_BigNum_GF_cal\cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/My_BigNum_GF_cal.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/My_BigNum_GF_cal.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/My_BigNum_GF_cal.dir/flags.make

CMakeFiles/My_BigNum_GF_cal.dir/library.c.obj: CMakeFiles/My_BigNum_GF_cal.dir/flags.make
CMakeFiles/My_BigNum_GF_cal.dir/library.c.obj: ../library.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="E:\E_drive\clion\Project List\cryptography\My_BigNum_GF_cal\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/My_BigNum_GF_cal.dir/library.c.obj"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\My_BigNum_GF_cal.dir\library.c.obj -c "E:\E_drive\clion\Project List\cryptography\My_BigNum_GF_cal\library.c"

CMakeFiles/My_BigNum_GF_cal.dir/library.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/My_BigNum_GF_cal.dir/library.c.i"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "E:\E_drive\clion\Project List\cryptography\My_BigNum_GF_cal\library.c" > CMakeFiles\My_BigNum_GF_cal.dir\library.c.i

CMakeFiles/My_BigNum_GF_cal.dir/library.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/My_BigNum_GF_cal.dir/library.c.s"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "E:\E_drive\clion\Project List\cryptography\My_BigNum_GF_cal\library.c" -o CMakeFiles\My_BigNum_GF_cal.dir\library.c.s

# Object files for target My_BigNum_GF_cal
My_BigNum_GF_cal_OBJECTS = \
"CMakeFiles/My_BigNum_GF_cal.dir/library.c.obj"

# External object files for target My_BigNum_GF_cal
My_BigNum_GF_cal_EXTERNAL_OBJECTS =

libMy_BigNum_GF_cal.dll: CMakeFiles/My_BigNum_GF_cal.dir/library.c.obj
libMy_BigNum_GF_cal.dll: CMakeFiles/My_BigNum_GF_cal.dir/build.make
libMy_BigNum_GF_cal.dll: CMakeFiles/My_BigNum_GF_cal.dir/linklibs.rsp
libMy_BigNum_GF_cal.dll: CMakeFiles/My_BigNum_GF_cal.dir/objects1.rsp
libMy_BigNum_GF_cal.dll: CMakeFiles/My_BigNum_GF_cal.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="E:\E_drive\clion\Project List\cryptography\My_BigNum_GF_cal\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking C shared library libMy_BigNum_GF_cal.dll"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\My_BigNum_GF_cal.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/My_BigNum_GF_cal.dir/build: libMy_BigNum_GF_cal.dll
.PHONY : CMakeFiles/My_BigNum_GF_cal.dir/build

CMakeFiles/My_BigNum_GF_cal.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\My_BigNum_GF_cal.dir\cmake_clean.cmake
.PHONY : CMakeFiles/My_BigNum_GF_cal.dir/clean

CMakeFiles/My_BigNum_GF_cal.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "E:\E_drive\clion\Project List\cryptography\My_BigNum_GF_cal" "E:\E_drive\clion\Project List\cryptography\My_BigNum_GF_cal" "E:\E_drive\clion\Project List\cryptography\My_BigNum_GF_cal\cmake-build-debug" "E:\E_drive\clion\Project List\cryptography\My_BigNum_GF_cal\cmake-build-debug" "E:\E_drive\clion\Project List\cryptography\My_BigNum_GF_cal\cmake-build-debug\CMakeFiles\My_BigNum_GF_cal.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/My_BigNum_GF_cal.dir/depend

