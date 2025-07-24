project(KitaCore)


add_library(KitaCore
        String/String.h
        String/String.cpp
        String/StringView.cpp
        String/StringView.h
        Macros.h
        TypeDefs.h)

target_include_directories(KitaCore PUBLIC ../)
find_package(spdlog CONFIG REQUIRED)
target_link_libraries(KitaCore PRIVATE spdlog::spdlog)
