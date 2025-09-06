project(KitaAssets)

find_package(Vulkan REQUIRED)

add_library(KitaAssets
        ${Assets_GENERATED_FILES}
        Asset.h
        Mesh.cpp
        Mesh.h
        AssetsManager.cpp
        AssetsManager.h
        Shader.cpp
        Shader.h
        AssetsConfig.cpp
        AssetsConfig.h
)

target_link_libraries(KitaAssets PRIVATE KitaCore KitaObject KitaRHI KitaProject)

target_include_directories(KitaAssets PUBLIC ${Assets_GENERATED_DIR})
target_include_directories(KitaAssets PUBLIC ../)

find_package(SqliteOrm CONFIG REQUIRED)
target_link_libraries(KitaAssets PRIVATE sqlite_orm::sqlite_orm)

# 查找 shader-slang 的头文件和链接库
find_path(SLANG_INCLUDE_DIR slang.h) # 查找 shader-slang 的头文件
find_library(SLANG_LIBRARY NAMES slang) # 查找 shader-slang 的库文件

# 如果找到 shader-slang，设置头文件和链接库
if (SLANG_INCLUDE_DIR AND SLANG_LIBRARY)
    message(STATUS "Found shader-slang")
    add_library(slang STATIC IMPORTED)
    set_target_properties(slang PROPERTIES
            IMPORTED_LOCATION ${SLANG_LIBRARY}
            INTERFACE_INCLUDE_DIRECTORIES ${SLANG_INCLUDE_DIR})
else ()
    message(FATAL_ERROR "shader-slang not found!")
endif ()

# 链接 shader-slang 库
target_link_libraries(KitaAssets PRIVATE slang)

