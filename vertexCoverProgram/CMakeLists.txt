cmake_minimum_required(VERSION 2.8)
project(a4ece650)

if (EXISTS "${CMAKE_SOURCE_DIR}/minisat")
  add_subdirectory(minisat)
else()
  message (FATAL_ERROR "MiniSat not found. Use `git clone https://github.com/agurfinkel/minisat` to check it out.")
endif()

## ECE650 Specific configuration start here
## Project specific configurations go here

# enable c++11 support
set (CMAKE_CXX_FLAGS "-std=c++11 -Wall ${CMAKE_CXX_FLAGS}")

set(PROJECT_SRC
    a4ece650.cpp
    graph.cpp)

set(PROJECT_HDR
    graph.hpp)

# create the main executable
# expose minisat header files
include_directories(${CMAKE_SOURCE_DIR}/minisat)
## add additional .cpp files if needed
#add_executable(a4ece650 a4ece650.cpp)
add_executable(a4ece650 ${PROJECT_HDR} ${PROJECT_SRC})
# link minisat library with the main executable
target_link_libraries(a4ece650 minisat-lib-static)

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
