project(KitaObject)

add_library(KitaObject
        Object.h
        Object.cpp
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
)

target_link_libraries(KitaObject PRIVATE KitaCore)

target_include_directories(KitaObject PUBLIC ${Object_GENERATED_DIR})
