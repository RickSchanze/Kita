#pragma once

#if DEBUG_MODE
#define KITA_DEBUG 1
#else
#define KITA_DEBUG 0
#endif

#if KITA_DEBUG
#define FORCE_INLINE inline

bool CheckDebuggerPresent();

#ifdef _MSC_VER
#define DEBUG_BREAK()         \
  if (CheckDebuggerPresent()) \
  __debugbreak()
#endif
#else
#define FORCE_INLINE __forceinline
#define DEBUG_BREAK()
#endif

#define INLINE_NODISCARD [[nodiscard]] FORCE_INLINE
#define LIKELY(x) (x) [[likely]]
#define UNLIKELY(x) (x) [[unlikely]]

#ifdef KITA_EDITOR
#define KITA_EDITOR 1
#endif
