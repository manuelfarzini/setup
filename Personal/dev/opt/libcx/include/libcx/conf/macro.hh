/** @type conf/macro.hh **/

#ifndef CX_CONF_MACRO
#define CX_CONF_MACRO

#include "libcx/conf/hal.hh"

#ifndef CX_HIDE_FROM_ABI
    #if CX_COMPILER_MSVC
        #define CX_HIDE_FROM_ABI
    #else
        #define CX_HIDE_FROM_ABI __attribute__((__visibility__("hidden")))
    #endif
#endif

// =========================================
// Syntax
// =========================================

#ifndef or_return
    #define or_return ; if (err) return
#endif
#ifndef or_else
    #define or_else ; if (err)
#endif
#ifndef or_continue
    #define or_continue ; if (err) continue
#endif
#ifndef or_break
    #define or_break ; if (err) break
#endif
#ifndef or_goto
    #define or_goto ; if (err) goto
#endif
  
#ifndef nodisc
    #define nodisc [[nodiscard]]
#endif
#ifndef noret
    #define noret [[noreturn]]
#endif
#ifndef unlike
    #define unlike [[unlikely]]
#endif
#ifndef cx_no_uniq_addr
    #if CX_COMPILER_MSVC
        #define cx_no_uniq_addr [[msvc::no_unique_address]]
    #else
        #define cx_no_uniq_addr [[no_unique_address]]
    #endif
#endif

#ifndef priv  // internal linkage procedures/variables
    #define priv static
#endif
#ifndef glob
    #define glob static
#endif
#ifndef persist  // persistent local variables
    #define persist static
#endif
#ifndef onedef  // single procedure/variable across translation units
    #define onedef inline
#endif

#ifndef fn
    #define fn inline auto
#endif
#ifndef noexce
    #define noexce noexcept
#endif
#ifndef where
    #define where requires
#endif
#ifndef proc
    #define proc(_name_, ...) inline auto _name_(__VA_ARGS__) noexcept ->
#endif
#ifndef func
    #define func auto
#endif
#ifndef clos
    #define clos auto
#endif
#ifndef comp
    #define comp consteval
#endif
#ifndef cons
    #define cons constexpr
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

#ifndef declt
    #define declt(x) decltype(x)
#endif

#ifndef Requires
    #define Requires(...)                                                                \
        typename = cx::uti::enable_if<true> > requires (__VA_ARGS__) && cx::kpred < true
#endif
#ifndef req
    #define req(...) , typename = cx::uti::enable_if<(__VA_ARGS__)>
#endif

#ifndef inln
    #if CX_COMPILER_MSVC
        #if _MSC_VER < 1300
            #define inln
        #else
            #define inln __forceinline
        #endif
    #elif CX_COMPILER_CLANG
        #define inln [[clang::always_inline]] __attribute__((always_inline))
    #elif CX_COMPILER_GCC
        #define inln [[gnu::always_inline]] __attribute__((always_inline))
    #else
        #define inln
    #endif
#endif
#ifndef noinln
    #if CX_COMPILER_MSVC
        #define noinln __declspec(noinline)
    #elif CX_COMPILER_CLANG || CX_COMPILER_GCC
        #define noinln __attribute__((noinline))
    #else
        #define noinln
    #endif
#endif
#ifndef inln_clos
    #if CX_COMPILER_GCC || CX_COMPILER_CLANG
        #define inln_clos __attribute__((always_inline, hot))
    #else
        #define inln_clos
    #endif
#endif
#ifndef noinln_clos
    #if CX_COMPILER_GCC || CX_COMPILER_CLANG
        #define noinln_clos __attribute__((noinline, cold))
    #else
        #define noinln_clos
    #endif
#endif

#ifndef CX_DEFINE
    #define CX_DEFINE inline
#endif

// =========================================
// Text generation
// =========================================

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
    #if CX_COMPILER_CLANG || CX_COMPILER_GCC
        #define CX_FUNCTION __PRETTY_FUNCTION__
    #elif CX_COMPILER_MSVC
        #define CX_FUNCTION __FUNCSIG__
    #else
        #define CX_FUNCTION __func__
    #endif
#endif

#ifndef NS
    #define NS_(name) name::
#endif

#ifndef STR_
    #define CX__STR_(x) #x
    #define STR_(x) CX__STR_(x)
#endif

#ifndef VA_
    #define VA_(...) __VA_ARGS__
#endif

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
    #define CX_BEG {
#endif
#ifndef CX_END
    #define CX_END }
#endif
#ifndef CX_HASH
    #define CX_HASH #
#endif
#ifndef CX_EMPTY
    #define CX_EMPTY
#endif

// =========================================
// Concepts generators
// =========================================

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

// =========================================
// Member aliases
// =========================================

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

// =========================================
// Type assertions
// =========================================

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

// =========================================
// Unreachable
// =========================================

#ifndef CX__UNREACHABLE
    #if CX_COMPILER_MSVC
        #define CX__UNREACHABLE \
            __assume(0);        \
            __debugbreak();     \
            for (;;) {}
    #else // GCC || CLANG
        #define CX__UNREACHABLE      \
            __builtin_unreachable(); \
            __builtin_trap();        \
            for (;;) {}
    #endif
#endif
#ifndef cx_unreachable
    #define cx_unreachable() CX__UNREACHABLE
#endif

// =========================================
// Function like
// =========================================

#ifndef size_of
    #define size_of(x) (isize(sizeof(x)))
#endif
#ifndef offset_of
    #define offset_of(Type, elem) (cast(isize) & ((cast(Type*) 0)->elem))
#endif
#ifndef align_of
    #define align_of(Type) alignof(Type)
#endif
#ifndef align_of
    #define align_of(Type) isize(alignof(Type))
#endif
#ifndef count_of
    #define count_of(x) \
        ((size_of(x) / size_of(0 [x])) / (cast(isize, !(size_of(x) % size_of(0 [x])))))
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

// =========================================
// Array operations
// =========================================

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

// =========================================
// Probability branch
// =========================================

#ifndef CX_PROBABILITY_BRANCH
    #define CX_PROBABILITY_BRANCH
    #define cx_rare(_BLK_) \
        [[unlikely]] { [&] noinln_clos { _BLK_; }(); }

    #define cx_rare_if(b, _BLK_) \
        if (b) [[unlikely]] { [&] noinln_clos { _BLK_; }(); }

    #define cx_often(_BLK_) \
        [[likely]] { [&] inln_clos { _BLK_; }(); }

    #define cx_often_if(b, _BLK_) \
        if (b) [[likely]] { [&] inln_clos { _BLK_; }(); }
#endif

// =========================================
// Undefine macros
// =========================================

#ifndef CX_UNDEF_KEYWORDS
    #define CX_UNDEF_KEYWORDS \
        #undef cons           \
        #undef comp           \
        #undef glob           \
        #undef implicit       \
        #undef priv           \
        #undef noexce         \
        #undef persist        \
        #undef predicate      \
        #undef fn             \
        #undef glob           \
        #undef onedef
#endif
      
#endif  // CX_CONF_MACRO
