project(KitaInput)

find_package(Vulkan REQUIRED)

add_library(KitaInput STATIC
#        ${Input_GENERATED_FILES}
        Input.cpp
        Input.h
        InputTicker.h
        InputTicker.cpp
)

target_link_libraries(KitaInput PRIVATE KitaCore KitaMath KitaRHI KitaWorld)

target_include_directories(KitaInput PUBLIC ../)