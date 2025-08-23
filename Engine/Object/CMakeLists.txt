project(KitaObject)

add_library(KitaObject
        ${Object_GENERATED_FILES}
        PersistentObject.cpp
        PersistentObject.h
        ObjectTable.cpp
        ObjectTable.h
        Actor.cpp
        Actor.h
        Component.cpp
        Component.h
        ObjectPtr.cpp
        ObjectPtr.h
        Object.h
        Object.cpp
)

target_link_libraries(KitaObject PRIVATE KitaCore)

target_include_directories(KitaObject PUBLIC ${Object_GENERATED_DIR})
target_include_directories(KitaObject PUBLIC ../)
