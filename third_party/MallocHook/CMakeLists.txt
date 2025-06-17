cmake_minimum_required(VERSION 3.12)
project(MallocHook)

# 设置 C++20 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 创建静态库
add_library(MallocHook STATIC
    MallocHook.cpp
)

# 设置库的输出目录
set_target_properties(MallocHook PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)