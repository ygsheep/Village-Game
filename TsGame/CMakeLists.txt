# 项目名称与版本
project(TsGame
        VERSION 1.0
        LANGUAGES CXX
)

# --------------------------
# 编译选项与全局配置
# --------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 启用编译警告（可选）
if(MSVC)
    add_compile_options(/std:c++20)  # MSVC严格警告
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)  # GCC/Clang严格警告
endif()


# --------------------------
# 源文件列表（自动收集）
# --------------------------
# 自动收集src下所有.cpp文件（按模块分类）
file(GLOB_RECURSE SRC_FILES
        "sources/*.cpp"
)

add_executable(${PROJECT_NAME}
        ${SRC_FILES}
        sources/main.cpp # 显式添加主文件（避免被GLOB遗漏）
)
# --------------------------
# 头文件目录包含
# --------------------------
target_include_directories(${PROJECT_NAME} PUBLIC
        include/
)

# 链接SDL2库
target_link_libraries(${PROJECT_NAME} PRIVATE
        ${SDL2_LIB_DIR}/SDL2.lib
        ${SDL2_LIB_DIR}/SDL2main.lib
        ${SDL2_TTF_LIB_DIR}/SDL2_ttf.lib
        ${SDL2_GFX_LIB_DIR}/SDL2_gfx.lib
        ${SDL2_IMAGE_LIB_DIR}/SDL2_image.lib
        ${SDL2_MIXER_LIB_DIR}/SDL2_mixer.lib
        ${CMAKE_BINARY_DIR}/third_party/cJSON/cjson.lib
        imgui
)

# Windows平台：复制SDL2.dll到输出目录
if(WIN32)
    configure_file(
            ${SDL2_LIB_DIR}/SDL2.dll
            ${CMAKE_CURRENT_BINARY_DIR}/SDL2.dll
            COPYONLY
    )
    configure_file(
            ${SDL2_TTF_LIB_DIR}/SDL2_ttf.dll
            ${CMAKE_CURRENT_BINARY_DIR}/SDL2_ttf.dll
            COPYONLY
    )
    configure_file(
            ${SDL2_IMAGE_LIB_DIR}/SDL2_image.dll
            ${CMAKE_CURRENT_BINARY_DIR}/SDL2_image.dll
            COPYONLY
    )
    configure_file(
            ${SDL2_MIXER_LIB_DIR}/SDL2_mixer.dll
            ${CMAKE_CURRENT_BINARY_DIR}/SDL2_mixer.dll
            COPYONLY
    )

endif()

# --------------------------
# 资源文件复制（可选，用于运行时访问）
# --------------------------
# 将config目录下的配置文件复制到构建输出目录
add_custom_command(
        TARGET TsGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/config"
        "$<TARGET_FILE_DIR:TsGame>/config"
        COMMENT "Copying config files to build directory"
)

# 将resources目录下的资源文件复制到构建输出目录
add_custom_command(
        TARGET TsGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:TsGame>/resources"
        COMMENT "Copying resources to build directory"
)

#file(COPY resources DESTINATION ${CMAKE_BINARY_DIR})
