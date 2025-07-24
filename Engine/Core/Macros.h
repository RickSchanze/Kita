#pragma once

#ifdef DEBUG
#define KITA_DEBUG 1
#else
#define KITA_DEBUG 0
#endif

#ifdef KITA_DEBUG
#define KITA_FORCE_INLINE inline
#else
#define KITA_FORCE_INLINE __forceinline
#endif

#define KITA_INLINE_NODISCARD [[nodiscard]] KITA_FORCE_INLINE
