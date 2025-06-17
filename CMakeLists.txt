cmake_minimum_required(VERSION 3.12)
project(DearTs)

# 设置第三方库路径
set(THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/third_party)

# 设置根目录（根据实际路径修改）
set(IMGUI_DIR ${THIRD_PARTY_DIR}/imgui)
set(SDL2_ROOT ${THIRD_PARTY_DIR}/SDL2)
set(SDL2_TTF_ROOT ${THIRD_PARTY_DIR}/SDL2_ttf)
set(SDL2_GFX_ROOT ${THIRD_PARTY_DIR}/SDL2_gfx)
set(SDL2_IMAGE_ROOT ${THIRD_PARTY_DIR}/SDL2_image)
set(SDL2_MIXER_ROOT ${THIRD_PARTY_DIR}/SDL2_mixer)
set(CJSON_DIR ${THIRD_PARTY_DIR}/cJSON)
set(MYMALLOCHOOK ${THIRD_PARTY_DIR}/MallocHook)

# 根据平台选择库路径
if(CMAKE_SIZEOF_VOID_P EQUAL 8)  # 64位系统
    set(SDL2_LIB_DIR ${SDL2_ROOT}/lib/x64)
    set(SDL2_TTF_LIB_DIR ${SDL2_TTF_ROOT}/lib/x64)
    set(SDL2_GFX_LIB_DIR ${SDL2_GFX_ROOT}/lib/x64)
    set(SDL2_IMAGE_LIB_DIR ${SDL2_IMAGE_ROOT}/lib/x64)
    set(SDL2_MIXER_LIB_DIR ${SDL2_MIXER_ROOT}/lib/x64)
    message("-- Using SDL2 x64 libraries")
else()  # 32位系统
    set(SDL2_LIB_DIR ${SDL2_ROOT}/lib/x86)
    set(SDL2_TTF_LIB_DIR ${SDL2_TTF_ROOT}/lib/x86)
    set(SDL2_GFX_LIB_DIR ${SDL2_GFX_ROOT}/lib/x86)
    set(SDL2_IMAGE_LIB_DIR ${SDL2_IMAGE_ROOT}/lib/x86)
    set(SDL2_MIXER_LIB_DIR ${SDL2_MIXER_ROOT}/lib/x86)
    message("-- Using SDL2 x86 libraries")
endif()

# 配置ImGui
file(GLOB IMGUI_SOURCES
    ${IMGUI_DIR}/*.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdlrenderer2.cpp
)
add_subdirectory(${CJSON_DIR})

add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC 
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

# 主程序配置
include_directories(
        ${SDL2_ROOT}/include
        ${SDL2_TTF_ROOT}/include
        ${SDL2_GFX_ROOT}/include
        ${SDL2_IMAGE_ROOT}/include
        ${SDL2_MIXER_ROOT}/include
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
        ${CJSON_DIR}
        ${MYMALLOCHOOK}
)
add_subdirectory(TsGame)
add_subdirectory(Demo)
