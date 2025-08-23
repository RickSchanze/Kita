cmake_minimum_required(VERSION 3.28)

project(Kita)

set(CMAKE_TOOLCHAIN_FILE "C:/Users/kita/Documents/Projects/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(VULKAN_PATH "C:/VulkanSDK/1.4.309.0" CACHE PATH "Vulkan SDK path")
set(CMAKE_GENERATOR "Ninja")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(KITA_GLOBAL_BINARY_DIR ${CMAKE_BINARY_DIR} CACHE INTERNAL "全局二进制目录")
set(KITA_GLOBAL_GENERATED_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Engine/Generated CACHE INTERNAL "全局生成文件目录")
set(CMAKE_CXX_STANDARD 26)
if (NOT ${VARIABLE} EQUAL "0")
    message(WARNING ${VARIABLE})
    message(FATAL_ERROR 代码生成失败)
endif ()

add_subdirectory(Engine)

if (MSVC)
    add_compile_options("/utf-8")
endif()

add_executable(Kita Engine/main.cpp)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG_MODE)
endif()

add_compile_definitions(KITA_EDITOR)

target_link_libraries(Kita PRIVATE KitaCore KitaObject KitaMath)

