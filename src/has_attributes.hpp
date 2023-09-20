#pragma once
#include <vcruntime.h>

#if _HAS_NODISCARD
#define NODISCARD [[nodiscard]]
#else
#define NODISCARD
#endif // !_HAD_NODISCARD

#if _HAS_CXX11
#define CONSTEXPR11 constexpr
#else 
#endif


#if _HAS_CXX14
#define CONSTEXPR14 constexpr
#else 
#define CONSTEXPR14
#endif

#if _HAS_CXX17
#define CONSTEXPR17 constexpr
#define INLINE_NAMESPACE inline
#else 
#define INLINE_NAMESPACE
#define CONSTEXPR17
#endif
#if _HAS_CXX20
#define CONSTEXPR20 constexpr
#else 
#define CONSTEXPR20
#endif
#if _HAS_CXX23
#define CONSTEXPR23 constexpr
#else 
#define CONSTEXPR23
#endif