cmake_minimum_required(VERSION 2.8)
project(Lisp)
option(GC_DEBUG "debug output for GC" OFF)
if (GC_DEBUG)
  add_definitions(-DGC_DEBUG)
endif()
list(APPEND CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Wconversion")
add_executable(lisp function.cpp functions.cpp object.cpp parser.cpp
  special_forms.cpp scope.cpp main.cpp numeric_functions.cpp list_functions.cpp gc.cpp)

target_link_libraries(lisp gmp gmpxx)
