cmake_minimum_required(VERSION 2.8)
project(Lisp)
list(APPEND CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Wconversion")
add_executable(main function.cpp functions.cpp object.cpp parser.cpp
                    special_forms.cpp scope.cpp main.cpp numeric_functions.cpp list_functions.cpp)

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED OFF)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost COMPONENTS regex) 
include_directories(${Boost_INCLUDE_DIRS}) 
target_link_libraries(main ${Boost_LIBRARIES} gmp gmpxx)
