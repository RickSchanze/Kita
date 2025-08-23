project(KitaRHI)

add_library(KitaRHI
        ${RHI_GENERATED_FILES}
        RHIFormat.h
        RHIFormat.cpp
)

target_link_libraries(KitaRHI PRIVATE KitaCore)

target_include_directories(KitaRHI PUBLIC ${RHI_GENERATED_DIR})
