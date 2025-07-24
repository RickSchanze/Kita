cmake_minimum_required(VERSION 3.28)

project(Kita)

set(CMAKE_CXX_STANDARD 26)

add_subdirectory(Engine)

add_executable(Kita Engine/main.cpp)

target_compile_definitions(Kita
        PRIVATE
        # If the debug configuration pass the DEBUG define to the compiler
        $<$<CONFIG:Debug>:DEBUG>
)

