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
        TypeDefs.h
        Memory/UniquePtr.h
        Memory/SharedPtr.h
        Memory/Malloc.cpp
        Memory/Malloc.h
        Assert.h
        Trace.cpp
        Trace.h)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(KitaCore PUBLIC spdlog::spdlog)

find_package(eventpp CONFIG REQUIRED)
target_link_libraries(KitaCore PUBLIC eventpp::eventpp)

find_package(cpptrace CONFIG REQUIRED)
target_link_libraries(KitaCore PRIVATE cpptrace::cpptrace)

find_package(mimalloc CONFIG REQUIRED)
target_link_libraries(KitaCore PRIVATE $<IF:$<TARGET_EXISTS:mimalloc-static>,mimalloc-static,mimalloc>)


target_include_directories(KitaCore PUBLIC ../)
