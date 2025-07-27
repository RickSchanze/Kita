project(KitaCore)


add_library(KitaCore
        String/String.h
        String/String.cpp
        String/StringView.cpp
        String/StringView.h
        String/StringTraits.h
        Container/Optional.h
        Container/Function.h
        Container/Array.h
        Container/Map.h
        Logging/Logger.hpp
        Logging/Logger.cpp
        Event/Callback.h
        Macros.h
        TypeDefs.h
        Memory/UniquePtr.h
        Memory/SharedPtr.h
        Memory/Malloc.cpp
        Memory/Malloc.h
        Reflection/Type.h
        Reflection/Type.cpp
        Reflection/Field.h
        Reflection/Field.cpp
        Reflection/TypeRegistry.h
        Reflection/TypeRegistry.cpp
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

find_package(absl CONFIG REQUIRED)
# note: 189 additional targets are not displayed.
target_link_libraries(KitaCore PUBLIC absl::any absl::log absl::base absl::bits)

target_include_directories(KitaCore PUBLIC ../)
