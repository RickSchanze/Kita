cmake_minimum_required(VERSION 3.28)
project(Kita)

set(CMAKE_GENERATOR "Ninja")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(KITA_GLOBAL_BINARY_DIR ${CMAKE_BINARY_DIR} CACHE INTERNAL "全局二进制目录")
set(KITA_GLOBAL_GENERATED_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Engine/Generated CACHE INTERNAL "全局生成文件目录")
set(CMAKE_CXX_STANDARD 26)
if (NOT ${VARIABLE} EQUAL "0")
    message(WARNING ${VARIABLE})
    message(FATAL_ERROR 代码生成失败)
endif ()

# =========================== 编译选项 ===============================
add_compile_definitions($<$<CONFIG:Debug>:DEBUG_MODE=1>)
add_compile_definitions(KITA_EDITOR=1) # 是否启用Editor
add_compile_definitions(ENABLE_PROFILING=1) # 是否启用Profile模式

add_subdirectory(Engine)

if (MSVC)
    add_compile_options("/utf-8")
endif ()

add_executable(Kita Engine/main.cpp)
target_link_libraries(Kita PRIVATE KitaCore KitaObject KitaMath KitaRHI KitaWorld KitaAssets)


