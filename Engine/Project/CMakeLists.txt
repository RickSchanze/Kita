project(KitaProject)

add_library(KitaProject
        ${Project_GENERATED_FILES}
        Project.h
        Project.cpp
)

target_link_libraries(KitaProject PRIVATE KitaCore)

target_include_directories(KitaProject PUBLIC ${Project_GENERATED_DIR})
target_include_directories(KitaProject PUBLIC ../)
