# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.15.5/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.15.5/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/Chris/Desktop/ece650/ece650Project

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/Chris/Desktop/ece650/ece650Project/build

# Include any dependencies generated for this target.
include CMakeFiles/ece650Project.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ece650Project.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ece650Project.dir/flags.make

CMakeFiles/ece650Project.dir/ece650Project.cpp.o: CMakeFiles/ece650Project.dir/flags.make
CMakeFiles/ece650Project.dir/ece650Project.cpp.o: ../ece650Project.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Chris/Desktop/ece650/ece650Project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ece650Project.dir/ece650Project.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ece650Project.dir/ece650Project.cpp.o -c /Users/Chris/Desktop/ece650/ece650Project/ece650Project.cpp

CMakeFiles/ece650Project.dir/ece650Project.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ece650Project.dir/ece650Project.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Chris/Desktop/ece650/ece650Project/ece650Project.cpp > CMakeFiles/ece650Project.dir/ece650Project.cpp.i

CMakeFiles/ece650Project.dir/ece650Project.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ece650Project.dir/ece650Project.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Chris/Desktop/ece650/ece650Project/ece650Project.cpp -o CMakeFiles/ece650Project.dir/ece650Project.cpp.s

CMakeFiles/ece650Project.dir/graph.cpp.o: CMakeFiles/ece650Project.dir/flags.make
CMakeFiles/ece650Project.dir/graph.cpp.o: ../graph.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Chris/Desktop/ece650/ece650Project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/ece650Project.dir/graph.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ece650Project.dir/graph.cpp.o -c /Users/Chris/Desktop/ece650/ece650Project/graph.cpp

CMakeFiles/ece650Project.dir/graph.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ece650Project.dir/graph.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Chris/Desktop/ece650/ece650Project/graph.cpp > CMakeFiles/ece650Project.dir/graph.cpp.i

CMakeFiles/ece650Project.dir/graph.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ece650Project.dir/graph.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Chris/Desktop/ece650/ece650Project/graph.cpp -o CMakeFiles/ece650Project.dir/graph.cpp.s

# Object files for target ece650Project
ece650Project_OBJECTS = \
"CMakeFiles/ece650Project.dir/ece650Project.cpp.o" \
"CMakeFiles/ece650Project.dir/graph.cpp.o"

# External object files for target ece650Project
ece650Project_EXTERNAL_OBJECTS =

ece650Project: CMakeFiles/ece650Project.dir/ece650Project.cpp.o
ece650Project: CMakeFiles/ece650Project.dir/graph.cpp.o
ece650Project: CMakeFiles/ece650Project.dir/build.make
ece650Project: minisat/libminisat.a
ece650Project: /usr/lib/libz.dylib
ece650Project: CMakeFiles/ece650Project.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/Chris/Desktop/ece650/ece650Project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ece650Project"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ece650Project.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ece650Project.dir/build: ece650Project

.PHONY : CMakeFiles/ece650Project.dir/build

CMakeFiles/ece650Project.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ece650Project.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ece650Project.dir/clean

CMakeFiles/ece650Project.dir/depend:
	cd /Users/Chris/Desktop/ece650/ece650Project/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/Chris/Desktop/ece650/ece650Project /Users/Chris/Desktop/ece650/ece650Project /Users/Chris/Desktop/ece650/ece650Project/build /Users/Chris/Desktop/ece650/ece650Project/build /Users/Chris/Desktop/ece650/ece650Project/build/CMakeFiles/ece650Project.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ece650Project.dir/depend

