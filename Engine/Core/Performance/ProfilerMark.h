#pragma once

#include "tracy/Tracy.hpp"

#ifdef ENABLE_PROFILING
#include "tracy/Tracy.hpp"
#define CPU_PROFILING_SCOPE ZoneScoped
#define CPU_PROFILING_SCOPE_N(Name) ZoneScopedN(Name)
#define DECL_TRACKABLE_MUREX(Type, Name) TracyLockable(Type, Name)
#define DECL_TRACKABLE_MUREX_D(Type, Name, Desc) TracyLockableN(Type, Name, Desc)
#else
#define CPU_PROFILING_SCOPE
#define CPU_PROFILING_SCOPE_N(Name)
#define DECL_TRACKABLE_MUREX(Type, Name) Type Name
#define DECL_TRACKABLE_MUREX_D(Type, Name, Desc) Type Name
#endif
