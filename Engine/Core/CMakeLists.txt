project(KitaCore)


add_library(KitaCore
        String/String.h
        String/String.cpp
        String/StringView.cpp
        String/StringView.h
        Logging/Logger.hpp
        Logging/Logger.cpp
        Macros.h
        TypeDefs.h)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(KitaCore PUBLIC spdlog::spdlog)

target_include_directories(KitaCore PUBLIC ../)
