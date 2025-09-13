cmake_minimum_required(VERSION 3.28)
project(Kita)

set(CMAKE_GENERATOR "Ninja")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(KITA_GLOBAL_BINARY_DIR ${CMAKE_BINARY_DIR} CACHE INTERNAL "全局二进制目录")
set(KITA_GLOBAL_GENERATED_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Engine/Generated CACHE INTERNAL "全局生成文件目录")
set_property(GLOBAL PROPERTY CMAKE_CXX_SCAN_FOR_MODULES OFF)
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

# 方法1：使用函数（推荐）
function(link_whole_archive target)
    foreach(lib ${ARGN})
        if(MSVC)
            # 获取库文件的完整路径
            target_link_options(${target} PRIVATE
                    "/WHOLEARCHIVE:$<TARGET_FILE:${lib}>"
            )
        elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
            target_link_libraries(${target} PRIVATE
                    -Wl,--whole-archive ${lib} -Wl,--no-whole-archive
            )
        endif()

        # 确保依赖关系正确
        target_link_libraries(${target} PRIVATE ${lib})
    endforeach()
endfunction()

link_whole_archive(Kita KitaCore)
link_whole_archive(Kita KitaObject)
link_whole_archive(Kita KitaMath)
link_whole_archive(Kita KitaRHI)
link_whole_archive(Kita KitaWorld)
link_whole_archive(Kita KitaAssets)
link_whole_archive(Kita KitaEditor)
