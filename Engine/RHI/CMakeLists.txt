project(KitaRHI)

add_library(KitaRHI
        ${RHI_GENERATED_FILES}
        RHIFormat.h
        IRHIResource.h
        Image.h
        ImageView.h
        RenderPass.h
        ShaderModule.h
        SurfaceWindow.h
        Sync.h
        RHIConfig.h
        FrameBuffer.h
        DescripotrSet.h
        Commands.h
        CommandBuffer.h
        CommandBuffer.cpp
        GfxContext.cpp
        GfxContext.h
        Pipeline.h
)

target_link_libraries(KitaRHI PRIVATE KitaCore KitaMath)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(KitaRHI PRIVATE glfw)

target_include_directories(KitaRHI PUBLIC ${RHI_GENERATED_DIR})
target_include_directories(KitaRHI PUBLIC ../)
