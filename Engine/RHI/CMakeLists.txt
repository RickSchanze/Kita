project(KitaRHI)

add_library(KitaRHI
        ${RHI_GENERATED_FILES}
        RHIFormat.h
        RHIFormat.cpp
        IRHIResource.h
        Image.h
        ImageView.h
        RenderPass.h
        ShaderModule.h
        SurfaceWindow.h
        Sync.h
        RHIConfig.cpp
        RHIConfig.h
)

target_link_libraries(KitaRHI PRIVATE KitaCore KitaMath)

target_include_directories(KitaRHI PUBLIC ${RHI_GENERATED_DIR})
target_include_directories(KitaRHI PUBLIC ../)
