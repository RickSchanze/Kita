project(KitaCore)


add_library(KitaCore
        String/String.h
        String/String.cpp
        String/StringView.cpp
        String/StringView.h
        String/StringTraits.h
        Logging/Logger.hpp
        Logging/Logger.cpp
        Event/Callback.h
        Macros.h
        TypeDefs.h)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(KitaCore PUBLIC spdlog::spdlog)

find_package(eventpp CONFIG REQUIRED)
target_link_libraries(KitaCore PUBLIC eventpp::eventpp)

target_include_directories(KitaCore PUBLIC ../)
