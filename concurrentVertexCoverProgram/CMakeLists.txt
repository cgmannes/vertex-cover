cmake_minimum_required(VERSION 2.8)
project(ece650Project)

if (EXISTS "${CMAKE_SOURCE_DIR}/minisat")
  add_subdirectory(minisat)
else()
  message (FATAL_ERROR "MiniSat not found. Use `git clone https://github.com/agurfinkel/minisat` to check it out.")
endif()

## ECE650 Specific configuration start here
## Project specific configurations go here

# enable c++11 support
set (CMAKE_CXX_FLAGS "-std=c++11 -pthread -Wall ${CMAKE_CXX_FLAGS}")

set(PROJECT_SRC
    ece650Project.cpp
    graph.cpp)

set(PROJECT_HDR
    graph.hpp)

# create the main executable
# expose minisat header files
include_directories(${CMAKE_SOURCE_DIR}/minisat)
## add additional .cpp files if needed
add_executable(ece650Project ${PROJECT_HDR} ${PROJECT_SRC})
# link minisat library with the main executable
target_link_libraries(ece650Project minisat-lib-static)

############################################################

# Compile and link for testing.
set(TEST_SRC
    test.cpp
    graph.cpp
)

set(TEST_HDR
    graph.hpp
)

############################################################

# create the executable for tests.
add_executable(test ${TEST_HDR} ${TEST_SRC})

############################################################

# link MiniSAT libraries.
target_link_libraries(test minisat-lib-static)
