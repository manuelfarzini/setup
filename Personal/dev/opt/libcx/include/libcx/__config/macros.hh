/// \file libcx/__config/macros.hh
#ifndef CX___CONFIG_MACRO_HH
#define CX___CONFIG_MACRO_HH
#include <libcx/__config/platform.hh>

// TODO:
// #define def(name, ...) \
//     inline constexpr auto name(__VA_ARGS__) noexce
// def(do_something, ...) -> i32
// {
//     return 42;
// }

/*----------------------------------------**
    Syntax
**----------------------------------------*/

#ifndef nodisc
    #define nodisc [[nodiscard]]
#endif
#ifndef noret
    #define noret [[noreturn]]
#endif
#ifndef unlike
    #define unlike [[unlikely]]
#endif

#ifndef intern  // internal linkage procedures/variables
    #define intern static
#endif
#ifndef onedef  // single procedure across translation units
    #define onedef inline
#endif
#ifndef proc
    #define proc auto
#endif
#ifndef noexce
    #define noexce noexcept
#endif
#ifndef implicit
    #define implicit
#endif
#ifndef persist  // persistent local variables
    #define persist static
#endif
#ifndef ceval
    #define ceval consteval
#endif
#ifndef cexpr
    #define cexpr constexpr
#endif
#ifndef twide  // type level member variables (type wide)
    #define twide static
#endif
#ifndef glob
    #define glob inline
#endif
#ifndef predicate
    #define predicate inline constexpr bool
#endif

#ifndef cast
    #define cast(Type, v) ((Type) (v))
#endif
#ifndef nil
    #define nil (void*) nullptr
#endif

#ifndef cx_unused
    #define cx_unused(x) (void) x
#endif

#ifndef decT
    #define decT(x) decltype(x)
#endif
#ifndef where
    #define where requires
#endif

#ifndef Requires
    #define Requires(...)                                                                \
        typename = cx::uti::enable_if<true> > requires (__VA_ARGS__) && cx::kpred < true
#endif
#ifndef EnableIf
    #define EnableIf(...) typename = cx::uti::enable_if<(__VA_ARGS__)>
#endif

#ifndef cx_no_uniq_addr
    #if defined(CX_COMPILER_MSVC)
        #define cx_no_uniq_addr [[msvc::no_unique_address]]
    #else
        #define cx_no_uniq_addr [[no_unique_address]]
    #endif
#endif

#ifndef finline
    #if defined(CX_COMPILER_MSVC)
        #if _MSC_VER < 1300
            #define finline
        #else
            #define finline __forceinline
        #endif
    #elif defined(CX_COMPILER_CLANG)
        #define finline [[clang::always_inline]] __attribute__((always_inline))
    #elif defined(CX_COMPILER_GCC)
        #define finline [[gnu::always_inline]] __attribute__((always_inline))
    #else
        #define finline
    #endif
#endif

#ifndef noinline
    #if defined(CX_COMPILER_MSVC)
        #define noinline __declspec(noinline)
    #elif defined(CX_COMPILER_CLANG) || defined(CX_COMPILER_GCC)
        #define noinline __attribute__((noinline))
    #else
        #define noinline
    #endif
#endif

#ifndef finline_clos
    #if defined(CX_COMPILER_GCC) || defined(CX_COMPILER_CLANG)
        #define finline_clos __attribute__((always_inline, hot))
    #else
        #define finline_clos
    #endif
#endif

/*----------------------------------------**
    Text generation
**----------------------------------------*/

#ifndef CX
    #define CX namespace cx
#endif

#ifndef CX_LINE
    #define CX_LINE __LINE__
#endif
#ifndef CX_FILE
    #define CX_FILE __FILE__
#endif
#ifndef CX_FUNCTION
    #if defined(CX_COMPILER_CLANG) || defined(CX_COMPILER_GCC)
        #define CX_FUNCTION __PRETTY_FUNCTION__
    #elif defined(CX_COMPILER_MSVC)
        #define CX_FUNCTION __FUNCSIG__
    #else
        #define CX_FUNCTION __func__
    #endif
#endif

#define NS_(name) name::
#define CX__STR_(x) #x
#define STR_(x) CX__STR_(x)
#define VA_(...) __VA_ARGS__

#ifndef CX_JOIN_MACROS
    #define CX_JOIN_MACROS 1
    #define CX__JOIN2(a, b) a##b
    #define CX_JOIN2(a, b) CX__JOIN2(a, b)
    #define CX_JOIN3(a, b, c) CX_JOIN2(CX_JOIN2(a, b), c)
    #define CX_JOIN4(a, b, c, d) CX_JOIN2(CX_JOIN2(CX_JOIN2(a, b), c), d)
#endif

#ifndef CX_UNIQ
    #define CX__UNIQ(a, b) CX_JOIN2(a, b)
    #define CX_UNIQ(base) CX__UNIQ(base, __COUNTER__)
#endif

#ifndef CX_DEF
    #define CX_DEF
#endif
#ifndef CX_UNDEF
    #define CX_UNDEF
#endif
#ifndef CX_BEG
    #define CX_BEG
#endif
#ifndef CX_END
    #define CX_END
#endif
#ifndef CX_HASH
    #define CX_HASH #
#endif
#ifndef CX_SPACE
    #define CX_SPACE()
#endif
#ifndef CX_NOTHING
    #define CX_NOTHING
#endif

/*----------------------------------------**
    Concept generators
**----------------------------------------*/

#ifndef CX_CONCEPT_GEN
    #define CX_CONCEPT_GEN(TypeName, is_type_name, CConceptName)                  \
        template<typename T> predicate is_type_name = false;                      \
        template<> predicate is_type_name<TypeName> = true;                       \
        template<typename T> concept CConceptName = is_type_name<cx::rm_cvref<T>>
#endif

#ifndef CX_CONCEPT_GEN_TEMPL
    #define CX_CONCEPT_GEN_TEMPL(                                                 \
        TypeName, is_type_name, CConceptName, _TPARAMS_, _TARGS_                  \
    )                                                                             \
        template<typename T> predicate is_type_name = false;                      \
        template<_TPARAMS_> predicate is_type_name<TypeName<_TARGS_>> = true;     \
        template<typename T> concept CConceptName = is_type_name<cx::rm_cvref<T>>
#endif

/*----------------------------------------**
    Member aliases
**----------------------------------------*/

#define CX_MEMBER_ALIASES(E, S)          \
    using Elem = E;                      \
    using Size = S;                      \
    using Iter = Elem*;                  \
    using Kter = Elem const*;            \
    using Rter = uti::RevIterator<Iter>; \
    using KRter = uti::RevIterator<Kter>

#define CX_EXTRACT_ALIASES(cont)                              \
    using Elem = typename cx::rm_cvref<decltype(cont)>::Elem; \
    using Size = typename cx::rm_cvref<decltype(cont)>::Size; \
    using Iter = Elem*;                                       \
    using Kter = Elem const*;                                 \
    using Rter = uti::RevIterator<Iter>;                      \
    using KRter = uti::RevIterator<Kter>

/*----------------------------------------**
    Type assertions
**----------------------------------------*/

#ifndef CX_TYPE_ASSERT
    #define CX_BASIC_TYPE_ASSERT(_BODY_)         \
        static_assert(cx::is_def_ctble<_BODY_>); \
        static_assert(cx::is_cp_ctble<_BODY_>);  \
        static_assert(cx::is_mv_ctble<_BODY_>);  \
        static_assert(cx::is_cp_asble<_BODY_>);  \
        static_assert(cx::is_mv_asble<_BODY_>);  \
        static_assert(cx::is_dtble<_BODY_>)
#endif
#ifndef CX_TRIVIAL_TYPE_ASSERT
    #define CX_TRIVIAL_TYPE_ASSERT(_BODY_)           \
        static_assert(cx::is_triv_cp_ctble<_BODY_>); \
        static_assert(cx::is_triv_mv_ctble<_BODY_>); \
        static_assert(cx::is_triv_cp_asble<_BODY_>); \
        static_assert(cx::is_triv_mv_asble<_BODY_>); \
        static_assert(cx::is_triv_dtble<_BODY_>)
#endif

/*----------------------------------------**
    Uncreachable
**----------------------------------------*/

#ifndef CX__UNREACHABLE
    #if defined(CX_COMPILER_MSVC)
        #define CX__UNREACHABLE \
            __assume(0);        \
            __debugbreak();     \
            for (;;) {}
    #else
        #define CX__UNREACHABLE      \
            __builtin_unreachable(); \
            __builtin_trap();        \
            for (;;) {}
    #endif
#endif
#ifndef cx_unreachable
    #define cx_unreachable() CX__UNREACHABLE
#endif

/*----------------------------------------**
    Function like 
**----------------------------------------*/

#ifndef usize_of
    #define usize_of(x) sizeof(x)
#endif
#ifndef isize_of
    #define isize_of(x) ((isize) (sizeof(x)))
#endif
#ifndef ioffset_of
    #define ioffset_of(Type, elem) (cast(isize) & ((cast(Type*) 0)->elem))
#endif
#ifndef ualign_of
    #define ualign_of(Type) alignof(Type)
#endif
#ifndef ialign_of
    #define ialign_of(Type) isize(alignof(Type))
#endif
#ifndef icount_of
    #define icount_of(x) \
        ((isize_of(x) / isize_of(0 [x])) / (cast(isize) !(isize_of(x) % isize_of(0 [x]))))
#endif

#ifndef cx_forward
    #define cx_forward(val) cx::uti::forward<decltype(val)>(val)
#endif
#ifndef cx_forward_args
    #define cx_forward_args(args) cx::uti::forward<Args>(args)
#endif

#ifndef cx_dprint
    #define cx_dprint(A) cx::io::printfn("DEBUG ---- {}", A)
#endif

#ifndef cx_between
    #define cx_between(lower, val, upper) lower < val&& val < upper
#endif
#ifndef cx_between_enum
    #define cx_between_enum(EnumName, val)                                 \
        cx_between(usize(EnumName##Beg), usize(val), usize(EnumName##End))
#endif
#ifndef cx_between_enum_ns
    #define cx_between_enum_ns(EnumName, val)                                      \
        cx_between(usize(NS_(EnumName) Beg), usize(val), usize(NS_(EnumName) End))
#endif

#ifndef cx_bit
    #define cx_bit(x) (1 << (x))
#endif

#ifndef cx_max2
    #define cx_max2(a, b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef cx_max3
    #define cx_max3(a, b, c) cx_max2(cx_max2(a, b), c)
#endif
#ifndef cx_max4
    #define cx_max4(a, b, c, d) cx_max2(cx_max2(a, b), cx_max2(c, d))
#endif

#ifndef cx_min2
    #define cx_min2(a, b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef cx_min3
    #define cx_min3(a, b, c) cx_min2(cx_min2(a, b), c)
#endif
#ifndef cx_min4
    #define cx_min4(a, b, c, d) cx_min2(cx_min2(a, b), cx_min2(c, d))
#endif

#ifndef cx_splat_8_16
    #define cx_splat_8_32(v8) ((u32(-1) / 255) * u8(v8))
#endif
#ifndef cx_splat_8_64
    #define cx_splat_8_64(v8) ((u64(-1) / 255) * u8(v8))
#endif
#ifndef cx_splat_32_64
    #define cx_splat_32_64(v32) ((u64(v32) << 32) | u32(v32))
#endif

#ifndef cx_mask_set
    #define cx_mask_set(var, set, mask) \
        if (set) {                      \
            (var) |= (mask);            \
        } else {                        \
            (var) &= ~(mask);           \
        }
#endif

/*----------------------------------------**
    Array operations
**----------------------------------------*/

#ifndef cx_extract4
    #define cx_extract4(src) {src[0], src[1], src[2], src[3]}
#endif
#ifndef cx_extract4_rev
    #define cx_extract4_rev(src) {src[3], src[2], src[1], src[0]}
#endif
#ifndef cx_extract4_z
    #define cx_extract4_z(src) {src[0], src[1], src[2], src[3], 0}
#endif
#ifndef cx_extract4_revz
    #define cx_extract4_revz(src) {src[3], src[2], src[1], src[0], 0}
#endif
#ifndef cx_print4_ul
    #define cx_print4_ul(src) printf("%ul %ul %ul %ul\n", src[0], src[1], src[2], src[3])
#endif
#ifndef cx_extract16
#define cx_extract16(arr)                                                  \
    {arr[0], arr[1], arr[2],  arr[3],  arr[4],  arr[5],  arr[6],  arr[7],  \
     arr[8], arr[9], arr[10], arr[11], arr[12], arr[13], arr[14], arr[15]}
#endif


/*----------------------------------------**
    Probability branches
**----------------------------------------*/

#if defined(CX_COMPILER_CLANG) || defined(CX_COMPILER_GCC)
    #ifndef CX_PROBABILITY_BRANCH
        #define CX_PROBABILITY_BRANCH
        #define cx_rare(_BLK_)                        \
            [[unlikely]] {                            \
                [&] __attribute__((noinline, cold)) { \
                    _BLK_;                            \
                }();                                  \
            }

        #define cx_rare_if(b, _BLK_)                  \
            if (b) [[unlikely]] {                     \
                [&] __attribute__((noinline, cold)) { \
                    _BLK_;                            \
                }();                                  \
            }

        #define cx_often(_BLK_)                               \
            [[likely]] {                                      \
                [&] __attribute__((__always_inline__, hot)) { \
                    _BLK_;                                    \
                }();                                          \
            }

        #define cx_often_if(b, _BLK_)                         \
            if (b) [[likely]] {                               \
                [&] __attribute__((__always_inline__, hot)) { \
                    _BLK_;                                    \
                }();                                          \
            }
    #endif
#endif

/*----------------------------------------**
    Undefine macros
**----------------------------------------*/

#ifndef CX_UNDEF_KEYWORDS
    #define CX_UNDEF_KEYWORDS \
        #undef cexpr          \
        #undef ceval          \
        #undef glob           \
        #undef implicit       \
        #undef intern         \
        #undef noexce         \
        #undef persist        \
        #undef predicate      \
        #undef proc           \
        #undef twide          \
        #undef onedef
#endif
      
#endif  // CX___CONFIG_MACRO_HH
