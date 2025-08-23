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
        Reflection/ArrayProxy.h
        Reflection/MapProxy.h
        Reflection/Any.h
        Reflection/Any.cpp
        Serialization/OutputArchive.h
        Serialization/OutputArchive.cpp
        Serialization/InputArchive.h
        Serialization/InputArchive.cpp
        Serialization/YAML/YAMLInputArchive.h
        Serialization/YAML/YAMLInputArchive.cpp
        Serialization/YAML/YAMLOutputArchive.cpp
        Serialization/YAML/YAMLOutputArchive.h
        Assert.h
        Trace.cpp
        Trace.h
        FileSystem/FileSystemError.h
        FileSystem/Path.cpp
        FileSystem/Path.h
        Serialization/TOML/TOMLOutputArchive.cpp
        Serialization/TOML/TOMLOutputArchive.h
        Serialization/TOML/TOMLInputArchive.cpp
        Serialization/TOML/TOMLInputArchive.h
        Container/Stack.h
        FileSystem/File.cpp
        FileSystem/File.h
        Reflection/MetaMark.h
        TaskGraph/TaskHandle.cpp
        TaskGraph/TaskHandle.h
        TaskGraph/TaskNode.cpp
        TaskGraph/TaskNode.h
        TaskGraph/NamedThreads.h
        TaskGraph/TaskGraph.cpp
        TaskGraph/TaskGraph.h
        TaskGraph/TaskGraphBoard.cpp
        TaskGraph/TaskGraphBoard.h
        Singleton/Singleton.h
        Concurrency/ConcurrentQueue.h
        Container/Set.h
        Concurrency/ConcurrentSet.h
        TaskGraph/QueuedThread.cpp
        TaskGraph/QueuedThread.h
        TaskGraph/QueuedThreadPool.cpp
        TaskGraph/QueuedThreadPool.h
        TaskGraph/ThreadUtils.cpp
        TaskGraph/ThreadUtils.h
        Reflection/TemplateAdapter.h
        Reflection/EnumString.h
)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(KitaCore PUBLIC spdlog::spdlog)

find_package(eventpp CONFIG REQUIRED)
target_link_libraries(KitaCore PUBLIC eventpp::eventpp)

find_package(yaml-cpp CONFIG REQUIRED)
target_link_libraries(KitaCore PRIVATE yaml-cpp::yaml-cpp)

find_package(cpptrace CONFIG REQUIRED)
target_link_libraries(KitaCore PRIVATE cpptrace::cpptrace)

find_package(mimalloc CONFIG REQUIRED)
target_link_libraries(KitaCore PRIVATE $<IF:$<TARGET_EXISTS:mimalloc-static>,mimalloc-static,mimalloc>)

find_package(absl CONFIG REQUIRED)
# note: 189 additional targets are not displayed.
target_link_libraries(KitaCore PUBLIC absl::any absl::log absl::base absl::bits)

target_include_directories(KitaCore PUBLIC ../)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG_MODE)
    message("CMAKE_BUILD_TYPE is Debug")
endif()
