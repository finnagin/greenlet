/* -*- indent-tabs-mode: nil; tab-width: 4; -*- */
#ifndef GREENLET_COMPILER_COMPAT_HPP
#define GREENLET_COMPILER_COMPAT_HPP

/**
 * Definitions to aid with compatibility with different compilers.
 */


/* The compiler used for Python 2.7 on Windows doesn't include
   either stdint.h or cstdint.h. Nor does it understand nullptr or have
   std::shared_ptr. = delete, etc Sigh. */
#if defined(_MSC_VER) &&  _MSC_VER <= 1500
typedef unsigned long long uint64_t;
typedef signed long long int64_t;
typedef unsigned int uint32_t;
// C++ defines NULL to be 0, which is ambiguous
// with an integer in certain cases, and won't autoconvert to a
// pointer in other cases.
#define nullptr NULL
#define G_HAS_METHOD_DELETE 0
// Use G_EXPLICIT_OP as the prefix for operator methods
// that should be explicit. Old MSVC doesn't support explicit operator
// methods.
#define G_EXPLICIT_OP
#define G_NOEXCEPT
#else
#include <cstdint>
#define G_HAS_METHOD_DELETE 1
#define G_EXPLICIT_OP explicit
#define G_NOEXCEPT noexcept
#endif

#if G_HAS_METHOD_DELETE == 1
#    define G_NO_COPIES_OF_CLS(Cls) public:     \
    Cls(const Cls& other) = delete; \
    Cls& operator=(const Cls& other) = delete
#    define G_NO_ASSIGNMENT_OF_CLS(Cls) public:  \
    Cls& operator=(const Cls& other) = delete
#else
#    define G_NO_COPIES_OF_CLS(Cls) private: \
    Cls(const Cls& other); \
    Cls& operator=(const Cls& other)
#    define G_NO_ASSIGNMENT_OF_CLS(Cls) private: \
        Cls& operator=(const Cls& other)
#endif

// CAUTION: MSVC is stupidly picky:
//
// "The compiler ignores, without warning, any __declspec keywords
// placed after * or & and in front of the variable identifier in a
// declaration."
// (https://docs.microsoft.com/en-us/cpp/cpp/declspec?view=msvc-160)
//
// So pointer return types must be handled differently (because of the
// trailing *), or you get inscrutable compiler warnings like "error
// C2059: syntax error: ''"

#if defined(__GNUC__) || defined(__clang__)
/* We used to check for GCC 4+ or 3.4+, but those compilers are
   laughably out of date. Just assume they support it. */
#    define GREENLET_NOINLINE_SUPPORTED
#    define GREENLET_NOINLINE(name) __attribute__((noinline)) name
#    define GREENLET_NOINLINE_P(rtype, name) rtype __attribute__((noinline)) name
#elif defined(_MSC_VER)
/* We used to check for  && (_MSC_VER >= 1300) but that's also out of date. */
#    define GREENLET_NOINLINE_SUPPORTED
#    define GREENLET_NOINLINE(name) __declspec(noinline) name
#    define GREENLET_NOINLINE_P(rtype, name) __declspec(noinline) rtype name
#endif


#endif
