project(KitaRHI)

find_package(Vulkan REQUIRED)

add_library(KitaRHI
        ${RHI_GENERATED_FILES}
        RHIEnums.h
        IRHIResource.h
        Image.h
        ImageView.h
        RenderPass.h
        ShaderModule.h
        SurfaceWindow.h
        Sync.h
        RHIConfig.h
        FrameBuffer.h
        DescriptorSet.h
        Commands.h
        CommandBuffer.h
        CommandBuffer.cpp
        GfxContext.cpp
        GfxContext.h
        Pipeline.h
        Vulkan/GfxContext_Vulkan.cpp
        Vulkan/GfxContext_Vulkan.h
        Vulkan/RHIEnums_Vulkan.h
        Vulkan/RHIEnums_Vulkan.cpp
        Vulkan/FrameBuffer_Vulkan.cpp
        Vulkan/FrameBuffer_Vulkan.h
        Vulkan/Image_Vulkan.cpp
        Vulkan/Image_Vulkan.h
        Vulkan/ImageView_Vulkan.cpp
        Vulkan/ImageView_Vulkan.h
)

target_link_libraries(KitaRHI PRIVATE KitaCore KitaMath)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(KitaRHI PRIVATE glfw)

target_include_directories(KitaRHI PUBLIC ${RHI_GENERATED_DIR})
target_include_directories(KitaRHI PUBLIC ../)

target_include_directories(KitaRHI PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(KitaRHI PRIVATE ${Vulkan_LIBRARIES})
