project(KitaAssets)

find_package(Vulkan REQUIRED)

add_library(KitaAssets
        ${Assets_GENERATED_FILES}
        Asset.h
        Mesh.cpp
        Mesh.h
)

target_link_libraries(KitaAssets PRIVATE KitaCore KitaObject KitaRHI)

target_include_directories(KitaAssets PUBLIC ${Assets_GENERATED_DIR})
target_include_directories(KitaAssets PUBLIC ../)
