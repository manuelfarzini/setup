/// @file libcx/__config/platform.hh

// /*                                                             *
// * Architecture                                                 *
// * Supported: 32 and 64 bit architecture                        *
// *                                                             */
//
// #ifndef MJ_ARCH
//     #define MJ_ARCH 0
//     #define MJ_ARCH_32 0
//     #define MJ_ARCH_64 0
// #endif
//
// #if (defined(_WIN64) || defined(__x86_64__) || defined(_M_X64) ||          \
//      defined(__64BIT__) || defined(__powerpc64__) || defined(__ppc64__) || \
//      defined(__aarch64__) || (defined(__riscv) && __riscv_xlen == 64))
//     #undef MJ_ARCH_64
//     #define MJ_ARCH_64 1
// #else
//     #undef MJ_ARCH_32
//     #define MJ_ARCH_32 1
// #endif
//
// #ifndef MJ_IS_BIG_ENDIAN
//     #define MJ_IS_BIG_ENDIAN (!*(u8*) &(u16) {1})
// #endif

// /*                                                             *
// * Operating system                                             *
// *                                                             */
//
// #ifndef MJ_SYSTEM
//     #define MJ_SYSTEM 0
//     #define MJ_SYSTEM_WIN 0
//     #define MJ_SYSTEM_OSX 0
//     #define MJ_SYSTEM_UNIX 0
//     #define MJ_SYSTEM_LINUX 0
// #endif
//
// #if defined(_WIN32) || defined(_WIN64)
//     #undef MJ_SYSTEM_WIN
//     #define MJ_SYSTEM_WIN 1
// #elif defined(__APPLE__) && defined(__MACH__)
//     #undef MJ_SYSTEM_OSX
//     #define MJ_SYSTEM_OSX 1
// #elif defined(__unix__)
//     #undef MJ_SYSTEM_UNIX
//     #define MJ_SYSTEM_UNIX 1
//     #if defined(__linux__)
//         #undef MJ_SYSTEM_LINUX
//         #define MJ_SYSTEM_LINUX 1
//     #endif
// #else
//     #error "This operating system is not supported"
// #endif
//
//
// #ifndef MJ_WIN64
//     #if MJ_SYSTEM_WIN && MJ_ARCH_64
//         #define MJ_WIN64 1
//     #else
//         #define MJ_WIN64 0
//     #endif
// #endif
//
// #ifndef MJ_WIN32
//     #if MJ_SYSTEM_WIN && MJ_ARCH_32
//         #define MJ_WIN32 1
//     #else
//         #define MJ_WIN32 0
//     #endif
// #endif
//
// /*                                                             *
// * Compiler                                                     *
// * Supported: MSVC, GCC, clang                                  *
// *                                                             */
//
// #ifndef MJ_COMPILER
//     #define MJ_COMPILER 0
//     #define MJ_COMPILER_MSVC 0
//     #define MJ_COMPILER_CLANG 0
//     #define MJ_COMPILER_GCC 0
//     #define MJ_COMPILER_APPLE 0
// #endif
//
// #if defined(_MSC_VER)
//     #undef MJ_COMPILER_MSVC
//     #define MJ_COMPILER_MSVC 1
// #elif defined(__clang__)
//     #undef MJ_COMPILER_CLANG
//     #define MJ_COMPILER_CLANG 1
//     #if defined(__apple_build_version__)  // XXX: missing header with __apple_build_version__ ?
//         #undef MJ_COMPILER_APPLE
//         #define MJ_COMPILER_APPLE 1
//     #endif
// #elif defined(__GNUC__)
//     #define MJ_COMPILER_GCC 1
// #else
//     #error "This compiler is not supported"
// #endif
//
// /*                                                             *
// * CPU, CACHE, SIMD                                             *
// *                                                             */
//
// #ifndef MJ_CPU
//     #define MJ_CPU 1
//     #define MJ_CPU_X86 0
//     #define MJ_CPU_PPC 0
//     #define MJ_CPU_ARM 0
//     #define MJ_SSE2 0
//     #define MJ_NEON 0
//     #define MJ_CACHE_LINE_SIZE 0
// #endif
//
// #if defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
//     #undef MJ_CPU_X86
//     #define MJ_CPU_X86 1
//     #if defined(__SSE2__)
//         #undef MJ_SSE2
//         #define MJ_SSE2 1
//     #endif
//     #undef MJ_CACHE_LINE_SIZE
//     #define MJ_CACHE_LINE_SIZE 64
//
// #elif defined(_M_PPC) || defined(__powerpc__) || defined(__powerpc64__)
//     #undef MJ_CPU_PPC
//     #define MJ_CPU_PPC 1
//     #undef MJ_CACHE_LINE_SIZE
//     #define MJ_CACHE_LINE_SIZE 128
//
// #elif defined(__arm__) || defined(__aarch64__) || defined(_M_ARM) || defined(_M_ARM64)
//     #undef MJ_CPU_ARM
//     #define MJ_CPU_ARM 1
//     #if defined(__ARM_NEON)
//         #undef MJ_NEON
//         #define MJ_NEON 1
//     #endif
//
//     #if defined(__APPLE__)
//         #undef MJ_CACHE_LINE_SIZE
//         #define MJ_CACHE_LINE_SIZE 128
//     #else
//         #undef MJ_CACHE_LINE_SIZE
//         #define MJ_CACHE_LINE_SIZE 64
//     #endif
//
// #else
//     #error "Unknown CPU"
// #endif
//
// /*----------------------------------------------------------------------------*/
//
// /*                                                             *
// * Primitive types                                              *
// *                                                             */
//
// namespace mojo {
//
// #if MJ_COMPILER_MSVC
//     #if _MSC_VER < 1300
//         using u8  = unsigned char;
//         using u16 = unsigned short;
//         using u32 = unsigned int;
//         using i8  = signed char;
//         using i16 = signed short;
//         using i32 = signed int;
//     #else
//         using u8  = unsigned __int8;
//         using u16 = unsigned __int16;
//         using u32 = unsigned __int32;
//         using i8  = signed __int8;
//         using i16 = signed __int16;
//         using i32 = signed __int32;
//     #endif
//     using u64 = unsigned __int64;
//     using i64 = signed __int64;
// #else
//     #include <stdint.h>
//     using u8  = uint8_t;
//     using u16 = uint16_t;
//     using u32 = uint32_t;
//     using u64 = uint64_t;
//     using i8  = int8_t;
//     using i16 = int16_t;
//     using i32 = int32_t;
//     using i64 = int64_t;
//     using u128 = unsigned __int128;
//     using i128 = __int128;
// #endif
//
// using usize = size_t;
// using isize = ptrdiff_t;
//
// #if MJ_SYSTEM_WIN && MJ_ARCH_64
//     using iptr = signed __int64;
//     using uptr = unsigned __int64;
// #elif MJ_SYSTEM_WIN && MJ_ARCH_32
//     #ifndef _W64
//         #if !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && _MSC_VER >= 1300
//             #define _W64 __w64
//         #else
//             #define _W64
//         #endif
//     #endif
//     using iptr = _W64 signed int;
//     using uptr = _W64 unsigned int;
// #else
//     using uptr = uintptr_t;
//     using iptr = intptr_t;
// #endif
//
// using anyptr  = void*;
//
// using f32 = float;
// using f64 = double;
//
// using f32_ptr = f32*;
// using f64_ptr = f64*;
//
// using Rune = i32;
// #define MJ_RUNE_ERR cast(Rune)(0xfffd)      // unicode replacement character �
// #define MJ_RUNE_MAX cast(Rune)(0x0010ffff)  // max valid codepoint
// #define MJ_RUNE_BOM cast(Rune)(0xfeff)      // byte order mark (endianness)
// #define MJ_RUNE_EOF cast(Rune)(-1)          // end of file
//
// using b8 = i8 ;
// using b16 = i16;
// using b32 = i32;
//
//
// /*                                                             *
// * Extended language types                                      *
// *                                                             */
//
// #ifndef MJ_HAS_CHAR8
//     #if defined(__cpp_char8_t)
//         #define MJ_HAS_CHAR8 1
//     #else
//         #define MJ_HAS_CHAR8 0
//     #endif
// #endif
//
// #ifndef MJ_HAS_CHAR16
//     #if defined(__cpp_unicode_characters) || (__cplusplus >= 201103L) || \
//         (defined(_MSVC_LANG) && _MSVC_LANG >= 201103L)
//         #define MJ_HAS_CHAR16 1
//         #define MJ_HAS_CHAR32 1
//     #else
//         #define MJ_HAS_CHAR16 0
//         #define MJ_HAS_CHAR32 0
//     #endif
// #endif
//
// #ifndef MJ_HAS_WCHAR_MAX
//     #if defined(__WCHAR_MAX__)
//         #define MJ_HAS_WCHAR_MAX 1
//         #define MJ_WCHAR_MAX __WCHAR_MAX__
//     #else
//         #define MJ_HAS_WCHAR_MAX 0
//     #endif
// #endif
//
// #ifndef MJ_HAS_INT128
//     #if !defined(MJ_COMPILER_MSVC) && defined(__SIZEOF_INT128__)
//         #define MJ_HAS_INT128 1
//     #else
//         #define MJ_HAS_INT128 0
//     #endif
// #endif
//
// #ifndef MJ_HAS_FLOAT128
//     #if !defined(MJ_COMPILER_MSVC) &&                                                      \
//         (defined(__SIZEOF_FLOAT128__) || defined(__FLT128_MAX__) || defined(__FLOAT128__))
//         #define MJ_HAS_FLOAT128 1
//     #else
//         #define MJ_HAS_FLOAT128 0
//     #endif
// #endif
//
// /*                                                             *
// * Limits                                                       *
// *                                                             */
//
// #ifndef MJ_LIMIT
//     #define MJ_LIMIT 1
//
//     #define U8_MIN 0u           //     0
//     #define U8_MAX 0xffu        //   255
//     #define I8_MIN (-0x7f - 1)  //  -128
//     #define I8_MAX 0x7f         //   127
//
//     #define U16_MIN 0u             //        0
//     #define U16_MAX 0xffffu        //   65_535
//     #define I16_MIN (-0x7fff - 1)  //  -32_768
//     #define I16_MAX 0x7fff         //   32_767
//
//     #define U32_MIN 0u                 //               0
//     #define U32_MAX 0xffffffffu        //   4_294_967_295
//     #define I32_MIN (-0x7fffffff - 1)  //  -2_147_483_648
//     #define I32_MAX 0x7fffffff         //   2_147_483_647
//
//     #define U64_MIN 0ull                         //                           0
//     #define U64_MAX 0xffffffffffffffffull        //  18_446_744_073_709_551_615
//     #define I64_MIN (-0x7fffffffffffffffll - 1)  //  -9_223_372_036_854_775_808
//     #define I64_MAX 0x7fffffffffffffffll         //   9_223_372_036_854_775_807
//
//     #if MJ_ARCH_32
//         #define USZ_MIN U32_MIN
//         #define USZ_MAX U32_MAX
//         #define ISZ_MIN I32_MIN
//         #define ISZ_MAX I32_MAX
//     #endif
//     #if MJ_ARCH_64
//         #define USZ_MIN U64_MIN
//         #define USZ_MAX U64_MAX
//         #define ISZ_MIN I64_MIN
//         #define ISZ_MAX I64_MAX
//     #else
//         #error "Unsupported architecture size"
//     #endif
//
//     inline constexpr u8 u8_min = U8_MIN;
//     inline constexpr u8 u8_max = U8_MAX;
//     inline constexpr i8 i8_min = I8_MIN;
//     inline constexpr i8 i8_max = I8_MAX;
//
//     inline constexpr u16 u16_min = U16_MIN;
//     inline constexpr u16 u16_max = U16_MAX;
//     inline constexpr i16 i16_min = I16_MIN;
//     inline constexpr i16 i16_max = I16_MAX;
//
//     inline constexpr u32 u32_min = U32_MIN;
//     inline constexpr u32 u32_max = U32_MAX;
//     inline constexpr i32 i32_min = I32_MIN;
//     inline constexpr i32 i32_max = I32_MAX;
//
//     inline constexpr u64 u64_min = U64_MIN;
//     inline constexpr u64 u64_max = U64_MAX;
//     inline constexpr i64 i64_min = I64_MIN;
//     inline constexpr i64 i64_max = I64_MAX;
//
//     inline constexpr usize usz_min = USZ_MIN;
//     inline constexpr usize usz_max = USZ_MAX;
//     inline constexpr isize isz_min = ISZ_MIN;
//     inline constexpr isize isz_max = ISZ_MAX;
// #endif
//
// }  // namespace mojo
//

// /*                                                             *
// * Miscellaneous                                                *
// *                                                             */
//
// #if MJ_COMPILER_GCC || MJ_COMPILER_CLANG
//     #ifndef MJ_PRINTF_ARGS
//         #define MJ_PRINTF_ARGS(fmt) __attribute__((format(printf, fmt, (fmt + 1))))
//     #endif
// #else // MJ_COMPILER_MSVC
//     #define MJ_PRINTF_ARGS(fmt)
// #endif
//
// #if MJ_COMPILER_MSVC
//     #ifndef mj_restrict
//         #define mj_restrict __restrict
//     #endif
// #else  // MJ_COMPILER_GCC || MJ_COMPILER_CLANG
//     #define mj_restrict __restrict__
// #endif
//
// #endif  // MOJO_HAL_HH

#ifndef CX___CONFIG_PLATFORM_HH
#define CX___CONFIG_PLATFORM_HH

/*                                         *
* Architecture                             *
* Supported: 32 bit and 64 bit             *
*                                         */

#if defined(_WIN64) || defined(__x86_64__) || defined(_M_X64) ||          \
    defined(__64BIT__) || defined(__powerpc64__) || defined(__ppc64__) || \
    defined(__aarch64__) || (defined(__riscv) && __riscv_xlen == 64)
    #ifndef CX_ARCH_64
        #define CX_ARCH_64 1
    #endif
#else
    #ifndef CX_ARCH_32
        #define CX_ARCH_32 1
    #endif
#endif

/*                                         *
* Operationg system                        *
*                                         */

#if defined(_WIN32) || defined(_WIN64)
    #ifndef CX_SYSTEM_WIN
        #define CX_SYSTEM_WIN 1
    #endif
#elif defined(__APPLE__) && defined(__MACH__)
    #ifndef CX_SYSTEM_OSX
        #define CX_SYSTEM_OSX 1
    #endif
#elif defined(__unix__)
    #ifndef CX_SYSTEM_UNIX
        #define CX_SYSTEM_UNIX 1
        #include <sys/wait.h>
    #endif
    #if defined(__linux__)
        #ifndef CX_SYSTEM_LINUX
            #define CX_SYSTEM_LINUX 1
        #endif
    #elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
        #ifndef CX_SYSTEM_FREEBSD
            #define CX_SYSTEM_FREEBSD 1
        #endif
    #elif defined(__OpenBSD__)
        #ifndef CX_SYSTEM_OPENBSD
            #define CX_SYSTEM_OPENBSD 1
        #endif
    #elif defined(__NetBSD__)
        #ifndef CX_SYSTEM_NETBSD
            #define CX_SYSTEM_NETBSD 1
        #endif
    #elif defined(__HAIKU__) || defined(__haiku__)
        #ifndef CX_SYSTEM_HAIKU
            #define CX_SYSTEM_HAIKU 1
        #endif
    #endif
#else
    #error "This operating system is not supported."
#endif

/*-----------------------------------------+
| Compiler                                 |
| Supported: MSVC, GCC, clang              |
+-----------------------------------------*/

#if defined(_MSC_VER)
    #define CX_COMPILER_MSVC 1
#elif defined(__clang__)
    #define CX_COMPILER_CLANG 1
    #if defined(__apple_build_version__)
        #define CX_COMPILER_APPLE 1
    #endif
#elif defined(__GNUC__)
    #define CX_COMPILER_GCC 1
#else
    #error "This compiler is not supported."
#endif

/*                                         *
* Endianness                               *
*                                         */

#ifndef CX_IS_BIG_ENDIAN
    #if CX_COMPILER_MSVC
        #define CX_IS_BIG_ENDIAN 0
    #elif defined(__BYTE_ORDER__)
        #define CX_IS_BIG_ENDIAN (__BYTE_ORDER__ == 4321)
    #else
        #error "Cannot determine endianness at compile time."
    #endif
#endif

/*-----------------------------------------+
| CPU, CACHE, SIMD                         |
+-----------------------------------------*/

#if defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
    #ifndef CX_CPU_X86
        #define CX_CPU_X86 1
    #endif
    #if defined(__SSE2__)
        #ifndef CX_SSE2
            #define CX_SSE2 1
        #endif
    #endif
    #ifndef CX_CACHE_LINE_SIZE
        #define CX_CACHE_LINE_SIZE 64
    #endif

#elif defined(_M_PPC) || defined(__powerpc__) || defined(__powerpc64__)
    #ifndef CX_CPU_PPC
        #define CX_CPU_PPC 1
    #endif
    #ifndef CX_CACHE_LINE_SIZE
        #define CX_CACHE_LINE_SIZE 128
    #endif

#elif defined(__arm__) || defined(__aarch64__) || defined(_M_ARM) || defined(_M_ARM64)
    #ifndef CX_CPU_ARM
        #define CX_CPU_ARM 1
        #if defined(__ARM_NEON)
            #ifndef CX_NEON
                #define CX_NEON 1
            #endif
        #endif
    #endif

    #if defined(__APPLE__)
        #ifndef CX_CACHE_LINE_SIZE
            #define CX_CACHE_LINE_SIZE 128
        #endif
    #else
        #ifndef CX_CACHE_LINE_SIZE
            #define CX_CACHE_LINE_SIZE 64
        #endif
    #endif

#elif defined(__MIPSEL__) || defined(__mips_isa_rev)
    #ifndef CX_CPU_MIPS
        #define CX_CPU_MIPS 1
    #endif
    #ifndef CX_CACHE_LINE_SIZE
        #define CX_CACHE_LINE_SIZE 64
    #endif

#elif defined(__riscv)
    #ifndef CX_CPU_RISCV
        #define CX_CPU_RISCV 1
    #endif
    #ifndef CX_CACHE_LINE_SIZE
        #define CX_CACHE_LINE_SIZE 64
    #endif
#else

    #error Unknown CPU
#endif

/*-----------------------------------------+
| Headers                                  |
+-----------------------------------------*/

#if !defined(CX_SYSTEM_WIN)
    #include <stddef.h>
    #include <stdarg.h>  // XXX:
#endif

#if defined(CX_SYSTEM_WIN)  // windows

    #if !(defined(CX_DONT_INCLUDE_WINDOWS_H))
        // avoid windows own min and max macros
        #ifndef NOMINMAX
            #define NOMINMAX 1
            #define NOMINMAX_FLAG 1
        #endif

        #if !defined(CX_ALREADY_INCLUDED_WINDOWS_H)
            // avoid some other windows own macros
            #define WIN32_LEAN_AND_MEAN 1
            #define WIN32_MEAN_AND_LEAN 1
            #define VC_EXTRALEN 1
            #define ALREADY_INCLUDED_FLAG
        #endif

        #include <windows.h>

        // cleanup
        #ifdef NOMINMAX_FLAG
            #undef NOMINMAX
            #undef NOMINMAX_FLAG
        #endif
        #ifdef ALREADY_INCLUDED_FLAG
            #undef WIN32_LEAN_AND_MEAN
            #undef WIN32_MEAN_AND_LEAN
            #undef VC_EXTRALEN
            #undef ALREADY_INCLUDED_FLAG
        #endif

    #endif
    #include <intrin.h>

#else  // non windows

    #include <dlfcn.h>
    #include <errno.h>
    #include <fcntl.h>
    #include <pthread.h>
    #ifndef _ISOC11_SOURCE
        #define _ISOC11_SOURCE 1
    #endif
    #include <stdlib.h>
    #include <sys/mman.h>
    #if !defined(CX_SYSTEM_OSX) && !defined(__FreeBSD__) && !defined(__OpenBSD__) && \
        !defined(__NetBSD__) && !defined(__HAIKU__)
        #include <sys/sendfile.h>
    #endif
    #include <sys/stat.h>
    #include <sys/time.h>
    #include <sys/types.h>
    #include <time.h>
    #include <unistd.h>

    #if defined(CX_CPU_X86)
        #include <xmmintrin.h>
    #endif
#endif

#if defined(CX_SYSTEM_OSX)
    #include <copyfile.h>
    #include <mach/clock.h>
    #include <mach/mach.h>
    #include <mach/mach_init.h>
    #include <mach/mach_time.h>
    #include <mach/thread_act.h>
    #include <mach/thread_policy.h>
    #include <sys/sysctl.h>
#endif

#if defined(CX_SYSTEM_FREEBSD)
    #include <pthread_np.h>
    #include <sys/cpuset.h>
    #include <sys/socket.h>
    #include <sys/sysctl.h>
    #include <sys/types.h>
    #include <sys/uio.h>
    #define lseek64 lseek
    #define sendfile(out, in, offset, count)            \
        sendfile(out, in, offset, count, NULL, NULL, 0)
#endif

#if defined(CX_SYSTEM_OPENBSD)
    #include <pthread_np.h>
    #include <stdio.h>
    #define lseek64 lseek
#endif

#if defined(CX_SYSTEM_NETBSD)
    #include <lwp.h>
    #include <stdio.h>
    #define lseek64 lseek
#endif

#if defined(CX_SYSTEM_HAIKU)
    #include <kernel/OS.h>
    #include <pthread.h>
    #include <stdio.h>
    #define lseek64 lseek
#endif

#if defined(CX_SYSTEM_UNIX)
    #include <semaphore.h>
#endif

#if defined(CX_SSE2)
    #include <emmintrin.h>
#endif

#if defined(CX_NEON)
    #include <arm_neon.h>
#endif

// #include <inttypes.h>  // XXX:

/*-----------------------------------------+
| Miscellaneous                            |
+-----------------------------------------*/

#ifndef CX_COMPILER_CLANG
    #define _Nonnull
    #define _Nullable
    #define _Null_unspecified
#endif
#ifndef cx_non_null
    #define cx_non_null _Nonnull
#endif
#ifndef cx_nullable
    #define cx_nullable _Nullable
#endif
#ifndef cx_null_unspec
    #define cx_null_unspec _Null_unspecified
#endif

#if defined(CX_COMPILER_GCC) || defined(CX_COMPILER_CLANG)
    #ifndef CX_PRINTF_ARGS
        #define CX_PRINTF_ARGS(fmt) __attribute__((format(printf, fmt, (fmt + 1))))
    #endif
#else // CX_COMPILER_MSVC
    #define CX_PRINTF_ARGS(fmt)
#endif

#if defined(CX_COMPILER_MSVC)
    #ifndef cx_restrict
        #define cx_restrict __restrict
    #endif
#else  // CX_COMPILER_GCC || CX_COMPILER_CLANG
    #define cx_restrict __restrict__
#endif

/*-----------------------------------------+
| Primitive types                          |
+-----------------------------------------*/

#if defined(CX_COMPILER_MSVC)
    #if _MSC_VER < 1300
        typedef unsigned char u8;
        typedef unsigned short u16;
        typedef unsigned int u32;
        typedef signed char i8;
        typedef signed short i16;
        typedef signed int i32;
    #else
        typedef unsigned __int8 u8;
        typedef unsigned __int16 u16;
        typedef unsigned __int32 u32;
        typedef signed __int8 i8;
        typedef signed __int16 i16;
        typedef signed __int32 i32;
    #endif
    typedef unsigned __int64 u64;
    typedef signed __int64 i64;
#else
    #include <stdint.h>
    typedef uint8_t u8;
    typedef uint16_t u16;
    typedef uint32_t u32;
    typedef uint64_t u64;
    typedef int8_t i8;
    typedef int16_t i16;
    typedef int32_t i32;
    typedef int64_t i64;
    typedef unsigned __int128 u128;
    typedef __int128 i128;
#endif

typedef size_t usize;
typedef ptrdiff_t isize;

#if defined(_WIN64)
    typedef signed __int64 iptr;
    typedef unsigned __int64 uptr;
#elif defined(_WIN32)
    #ifndef _W64
        #if !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && _MSC_VER >= 1300
            #define _W64 __w64
        #else
            #define _W64
        #endif
    #endif
    typedef _W64 signed int iptr;
    typedef _W64 unsigned int uptr;
#else
    typedef uintptr_t uptr;
    typedef intptr_t iptr;
#endif

typedef void* anyptr;
typedef void const* ptrcany;
typedef void** viewany;

typedef u8* ptru8;
typedef u8* const cptru8;
typedef u8 const* ptrcu8;
typedef u8** viewu8;

typedef u16* ptru16;
typedef u16 const* ptrcu16;

typedef u32* ptru32;
typedef u32 const* ptrcu32;
typedef u32** viewu32;

typedef u64* ptru64;
typedef u64 const* ptrcu64;

typedef uptr* viewuptr;
typedef iptr* viewiptr;

typedef float f32;
typedef double f64;

typedef i32 Rune;
#define CX_RUNE_ERR cast(Rune)(0xfffd)      // unicode replacement character �
#define CX_RUNE_MAX cast(Rune)(0x0010ffff)  // max valid codepoint
#define CX_RUNE_BOM cast(Rune)(0xfeff)      // byte order mark (endianness)
#define CX_RUNE_EOF cast(Rune)(-1)          // end of file

typedef i8 b8;
typedef i16 b16;
typedef i32 b32;

typedef char const* ptrcch8;

#if !defined(__cplusplus)
    #if (defined(CX_COMPILER_MSVC) && _MSC_VER < 1800) ||  \
        (!defined(_MSC_VER) && !defined(__STDC_VERSION__))
        #ifndef true
            #define true (0 == 0)
        #endif
        #ifndef false
            #define false (0 != 0)
        #endif
        typedef b8 bool;
    #else
        #include <stdbool.h>
    #endif
#endif

// inline constexpr decltype(nullptr) null = nullptr;

/*-----------------------------------------+
| Extended language types                  |
+-----------------------------------------*/

#ifndef CX_HAS_CHAR8
    #if defined(__cpp_char8_t)
        #define CX_HAS_CHAR8 1
    #else
        #define CX_HAS_CHAR8 0
    #endif
#endif

#ifndef CX_HAS_CHAR16
    #if defined(__cpp_unicode_characters) || (__cplusplus >= 201103L) || \
        (defined(_MSVC_LANG) && _MSVC_LANG >= 201103L)
        #define CX_HAS_CHAR16 1
        #define CX_HAS_CHAR32 1
    #else
        #define CX_HAS_CHAR16 0
        #define CX_HAS_CHAR32 0
    #endif
#endif

#ifndef CX_HAS_WCHAR_MAX
    #if defined(__WCHAR_MAX__)
        #define CX_HAS_WCHAR_MAX 1
        #define CX_WCHAR_MAX __WCHAR_MAX__
    #else
        #define CX_HAS_WCHAR_MAX 0
    #endif
#endif

#ifndef CX_HAS_INT128
    #if !defined(CX_COMPILER_MSVC) && defined(__SIZEOF_INT128__)
        #define CX_HAS_INT128 1
    #else
        #define CX_HAS_INT128 0
    #endif
#endif

#ifndef CX_HAS_FLOAT128
    #if !defined(CX_COMPILER_MSVC) &&                                                      \
        (defined(__SIZEOF_FLOAT128__) || defined(__FLT128_MAX__) || defined(__FLOAT128__))
        #define CX_HAS_FLOAT128 1
    #else
        #define CX_HAS_FLOAT128 0
    #endif
#endif

/*-----------------------------------------+
| Limits                                   |
+-----------------------------------------*/

#ifndef CX_LIMIT
    #define CX_LIMIT 1

    #define U8_MIN 0u           //     0
    #define U8_MAX 0xffu        //   255
    #define I8_MIN (-0x7f - 1)  //  -128
    #define I8_MAX 0x7f         //   127

    #define U16_MIN 0u             //        0
    #define U16_MAX 0xffffu        //   65_535
    #define I16_MIN (-0x7fff - 1)  //  -32_768
    #define I16_MAX 0x7fff         //   32_767

    #define U32_MIN 0u                 //               0
    #define U32_MAX 0xffffffffu        //   4_294_967_295
    #define I32_MIN (-0x7fffffff - 1)  //  -2_147_483_648
    #define I32_MAX 0x7fffffff         //   2_147_483_647

    #define U64_MIN 0ull                         //                           0
    #define U64_MAX 0xffffffffffffffffull        //  18_446_744_073_709_551_615
    #define I64_MIN (-0x7fffffffffffffffll - 1)  //  -9_223_372_036_854_775_808
    #define I64_MAX 0x7fffffffffffffffll         //   9_223_372_036_854_775_807

    #if defined(CX_ARCH_32)
        #define USZ_MIN U32_MIN
        #define USZ_MAX U32_MAX
        #define ISZ_MIN I32_MIN
        #define ISZ_MAX I32_MAX
    #endif
    #if defined(CX_ARCH_64)
        #define USZ_MIN U64_MIN
        #define USZ_MAX U64_MAX
        #define ISZ_MIN I64_MIN
        #define ISZ_MAX I64_MAX
    #else
        #error "Unsupported architecture size"
    #endif

    #if defined(__cplusplus)
    inline constexpr u8 u8_min = U8_MIN;
    inline constexpr u8 u8_max = U8_MAX;
    inline constexpr i8 i8_min = I8_MIN;
    inline constexpr i8 i8_max = I8_MAX;

    inline constexpr u16 u16_min = U16_MIN;
    inline constexpr u16 u16_max = U16_MAX;
    inline constexpr i16 i16_min = I16_MIN;
    inline constexpr i16 i16_max = I16_MAX;

    inline constexpr u32 u32_min = U32_MIN;
    inline constexpr u32 u32_max = U32_MAX;
    inline constexpr i32 i32_min = I32_MIN;
    inline constexpr i32 i32_max = I32_MAX;

    inline constexpr u64 u64_min = U64_MIN;
    inline constexpr u64 u64_max = U64_MAX;
    inline constexpr i64 i64_min = I64_MIN;
    inline constexpr i64 i64_max = I64_MAX;

    inline constexpr usize usz_min = USZ_MIN;
    inline constexpr usize usz_max = USZ_MAX;
    inline constexpr isize isz_min = ISZ_MIN;
    inline constexpr isize isz_max = ISZ_MAX;
    #endif

#endif

#endif  // CX___CONFIG_PLATFORM_HH
