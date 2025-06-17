# YourProject/CMakeLists.txt
cmake_minimum_required(VERSION 3.18)
project(Demo VERSION 1.0 LANGUAGES CXX)

# --------------------------
# 编译选项（可选）
# --------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --------------------------
# 主程序目标
# --------------------------
add_executable(${PROJECT_NAME} 
	main.cpp
)

# 链接 MallocHook 静态库
#target_link_libraries(Demo PRIVATE MallocHook)