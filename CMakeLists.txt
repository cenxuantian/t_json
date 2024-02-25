cmake_minimum_required(VERSION 3.0.0)
project(t_json_example VERSION 0.1.0 LANGUAGES C CXX)



set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(value examples/value.cpp)
add_executable(array examples/array.cpp)
add_executable(object examples/object.cpp)