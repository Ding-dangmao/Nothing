# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.28

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
CMAKE_COMMAND = "F:\CLion 2023.3.4\bin\cmake\win\x64\bin\cmake.exe"

# The command to remove a file.
RM = "F:\CLion 2023.3.4\bin\cmake\win\x64\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = F:\Nothing\TCP-IP

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = F:\Nothing\TCP-IP\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/tcp.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/tcp.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/tcp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/tcp.dir/flags.make

CMakeFiles/tcp.dir/main.cpp.obj: CMakeFiles/tcp.dir/flags.make
CMakeFiles/tcp.dir/main.cpp.obj: CMakeFiles/tcp.dir/includes_CXX.rsp
CMakeFiles/tcp.dir/main.cpp.obj: F:/Nothing/TCP-IP/main.cpp
CMakeFiles/tcp.dir/main.cpp.obj: CMakeFiles/tcp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=F:\Nothing\TCP-IP\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/tcp.dir/main.cpp.obj"
	"F:\CLion 2023.3.4\bin\mingw\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/tcp.dir/main.cpp.obj -MF CMakeFiles\tcp.dir\main.cpp.obj.d -o CMakeFiles\tcp.dir\main.cpp.obj -c F:\Nothing\TCP-IP\main.cpp

CMakeFiles/tcp.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/tcp.dir/main.cpp.i"
	"F:\CLion 2023.3.4\bin\mingw\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E F:\Nothing\TCP-IP\main.cpp > CMakeFiles\tcp.dir\main.cpp.i

CMakeFiles/tcp.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/tcp.dir/main.cpp.s"
	"F:\CLion 2023.3.4\bin\mingw\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S F:\Nothing\TCP-IP\main.cpp -o CMakeFiles\tcp.dir\main.cpp.s

CMakeFiles/tcp.dir/socket/socket_util.cpp.obj: CMakeFiles/tcp.dir/flags.make
CMakeFiles/tcp.dir/socket/socket_util.cpp.obj: CMakeFiles/tcp.dir/includes_CXX.rsp
CMakeFiles/tcp.dir/socket/socket_util.cpp.obj: F:/Nothing/TCP-IP/socket/socket_util.cpp
CMakeFiles/tcp.dir/socket/socket_util.cpp.obj: CMakeFiles/tcp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=F:\Nothing\TCP-IP\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/tcp.dir/socket/socket_util.cpp.obj"
	"F:\CLion 2023.3.4\bin\mingw\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/tcp.dir/socket/socket_util.cpp.obj -MF CMakeFiles\tcp.dir\socket\socket_util.cpp.obj.d -o CMakeFiles\tcp.dir\socket\socket_util.cpp.obj -c F:\Nothing\TCP-IP\socket\socket_util.cpp

CMakeFiles/tcp.dir/socket/socket_util.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/tcp.dir/socket/socket_util.cpp.i"
	"F:\CLion 2023.3.4\bin\mingw\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E F:\Nothing\TCP-IP\socket\socket_util.cpp > CMakeFiles\tcp.dir\socket\socket_util.cpp.i

CMakeFiles/tcp.dir/socket/socket_util.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/tcp.dir/socket/socket_util.cpp.s"
	"F:\CLion 2023.3.4\bin\mingw\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S F:\Nothing\TCP-IP\socket\socket_util.cpp -o CMakeFiles\tcp.dir\socket\socket_util.cpp.s

# Object files for target tcp
tcp_OBJECTS = \
"CMakeFiles/tcp.dir/main.cpp.obj" \
"CMakeFiles/tcp.dir/socket/socket_util.cpp.obj"

# External object files for target tcp
tcp_EXTERNAL_OBJECTS =

tcp.exe: CMakeFiles/tcp.dir/main.cpp.obj
tcp.exe: CMakeFiles/tcp.dir/socket/socket_util.cpp.obj
tcp.exe: CMakeFiles/tcp.dir/build.make
tcp.exe: CMakeFiles/tcp.dir/linkLibs.rsp
tcp.exe: CMakeFiles/tcp.dir/objects1.rsp
tcp.exe: CMakeFiles/tcp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=F:\Nothing\TCP-IP\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable tcp.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\tcp.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/tcp.dir/build: tcp.exe
.PHONY : CMakeFiles/tcp.dir/build

CMakeFiles/tcp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\tcp.dir\cmake_clean.cmake
.PHONY : CMakeFiles/tcp.dir/clean

CMakeFiles/tcp.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" F:\Nothing\TCP-IP F:\Nothing\TCP-IP F:\Nothing\TCP-IP\cmake-build-debug F:\Nothing\TCP-IP\cmake-build-debug F:\Nothing\TCP-IP\cmake-build-debug\CMakeFiles\tcp.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/tcp.dir/depend

