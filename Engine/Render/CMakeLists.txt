project(KitaRender)

find_package(Vulkan REQUIRED)

add_library(KitaRender
        ${Render_GENERATED_FILES}
        RenderTicker.h
        RenderTicker.cpp
        RenderContext.cpp
        RenderContext.h
        RenderPipeline.cpp
        RenderPipeline.h
)

target_link_libraries(KitaRender PRIVATE KitaCore KitaMath KitaRHI KitaWorld )

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(KitaRender PRIVATE glfw)

target_include_directories(KitaRender PUBLIC ${Render_GENERATED_DIR})
target_include_directories(KitaRender PUBLIC ../)