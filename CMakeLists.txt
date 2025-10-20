cmake_minimum_required(VERSION 3.28)
project(mallard)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-fno-rtti -fno-exceptions")

add_library(lib INTERFACE)
target_include_directories(lib INTERFACE ${CMAKE_SOURCE_DIR}/include)

add_executable(template src/template.cc)
target_link_libraries(template PRIVATE lib)