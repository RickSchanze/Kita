project(KitaEditor)

find_package(Vulkan REQUIRED)

add_library(KitaEditor
        ${Editor_GENERATED_FILES}
        EditorWindow.h
        EditorWindow.cpp
        MenuAction.cpp
        MenuAction.h
        EditorUI.cpp
        EditorUI.h
        MenuActionManager.cpp
        MenuActionManager.h
        Windows/LoggingWindow.cpp
        Windows/LoggingWindow.h
        EditorWindowManager.cpp
        EditorWindowManager.h
)

target_link_libraries(KitaEditor PRIVATE KitaCore KitaObject KitaRender)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(KitaEditor PRIVATE glfw)

target_include_directories(KitaEditor PUBLIC ${Editor_GENERATED_DIR})
target_include_directories(KitaEditor PUBLIC ../)

find_package(argparse CONFIG REQUIRED)
target_link_libraries(KitaEditor PRIVATE argparse::argparse)