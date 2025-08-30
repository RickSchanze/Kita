project(KitaWorld)

find_package(Vulkan REQUIRED)

add_library(KitaWorld
        ${World_GENERATED_FILES}
        EngineLoop.h
        EngineLoop.cpp
        ITick.cpp
        ITick.h
        TickManager.cpp
        TickManager.h
)

target_link_libraries(KitaWorld PRIVATE KitaCore KitaMath KitaRHI KitaInput KitaRender)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(KitaWorld PRIVATE glfw)

target_include_directories(KitaWorld PUBLIC ${World_GENERATED_DIR})
target_include_directories(KitaWorld PUBLIC ../)