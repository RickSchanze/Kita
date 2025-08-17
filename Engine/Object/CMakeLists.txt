project(KitaObject)

add_library(KitaObject
        Object.h
        Object.cpp
        ${Object_GENERATED_FILES}
)

target_link_libraries(KitaObject PRIVATE KitaCore)

target_include_directories(KitaObject PUBLIC ${Object_GENERATED_DIR})
