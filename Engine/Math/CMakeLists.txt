project(KitaMath)

add_library(KitaMath
#        ${Math_GENERATED_FILES}
        Vector.h
        Vector.cpp
        Color.cpp
        Color.h
)

target_link_libraries(KitaMath PRIVATE KitaCore)

#target_include_directories(KitaMath PUBLIC ${Object_GENERATED_DIR})

find_package(glm CONFIG REQUIRED)
target_link_libraries(KitaMath PUBLIC glm::glm)
