project(KitaAssets)

find_package(Vulkan REQUIRED)

add_library(KitaAssets
        ${Assets_GENERATED_FILES}
        Asset.h
        Mesh.cpp
        Mesh.h
        AssetsManager.cpp
        AssetsManager.h
)

target_link_libraries(KitaAssets PRIVATE KitaCore KitaObject KitaRHI)

target_include_directories(KitaAssets PUBLIC ${Assets_GENERATED_DIR})
target_include_directories(KitaAssets PUBLIC ../)

find_package(SqliteOrm CONFIG REQUIRED)
target_link_libraries(KitaAssets PRIVATE sqlite_orm::sqlite_orm)

