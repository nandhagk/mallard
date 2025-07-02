cmake_minimum_required(VERSION 3.28)
project(mallard)

# find_package(Clang REQUIRED CONFIG)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-fno-rtti -fno-exceptions")

add_subdirectory(abseil-cpp)

# include_directories(${CLANG_INCLUDE_DIRS})
# separate_arguments(CLANG_DEFINITIONS_LIST NATIVE_COMMAND ${CLANG_DEFINITIONS})
# add_definitions(${CLANG_DEFINITIONS_LIST})

add_executable(mallard main.cpp
        expander.h
        expander.cpp)
# target_link_libraries(mallard clangAST clangBasic clangFrontend clangLex clangRewrite clangSema clangTooling)

target_link_libraries(mallard 
        absl::flags absl::flags_parse absl::log absl::log_initialize absl::log_severity absl::flat_hash_set)