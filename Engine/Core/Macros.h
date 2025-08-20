#pragma once

#ifdef DEBUG_MODE
#define KITA_DEBUG 1
#else
#define KITA_DEBUG 0
#endif

#ifdef KITA_DEBUG
#define FORCE_INLINE inline
#define DEBUG_BREAK() __debugbreak()
#else
#define FORCE_INLINE __forceinline
#define DEBUG_BREAK()
#endif

#define INLINE_NODISCARD [[nodiscard]] FORCE_INLINE
#define LIKELY(x) (x) [[likely]]
#define UNLIKELY(x) (x) [[unlikely]]
