#pragma once

#if ENABLE_PROFILING
#include "tracy/Tracy.hpp"
#define CPU_PROFILING_SCOPE ZoneScoped
#define CPU_NAMED_PROFILING_SCOPE(Name) ZoneScopedN(Name)
#define DECL_TRACKABLE_MUREX(Type, Name) TracyLockable(Type, Name)
#define DECL_TRACKABLE_MUREX_D(Type, Name, Desc) TracyLockableN(Type, Name, Desc)
#define MARK_PROFILER_FRAME FrameMark
#else
#define CPU_PROFILING_SCOPE
#define CPU_NAMED_PROFILING_SCOPE(Name)
#define DECL_TRACKABLE_MUREX(Type, Name) Type Name
#define DECL_TRACKABLE_MUREX_D(Type, Name, Desc) Type Name
#define MARK_PROFILER_FRAME
#endif
