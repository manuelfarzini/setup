#include <libcx/config.hh>
#include <libcx/uti/iterator.hh>

onedef cexpr proc cx_has_zero_byte_u64(u64 x) -> b32 {
    // Hacker's Delight: detect any zero byte
    return ((x - 0x0101010101010101ULL) & ~x & 0x8080808080808080ULL) != 0;
}

finline onedef cexpr proc cx_first_zero_byte_u64(u64 x) -> u32 {
    u64 m = (x - 0x0101010101010101ULL) & ~x & 0x8080808080808080ULL;

    #if defined(CX_COMPILER_GCC) || defined(CX_COMPILER_CLANG)
        return u32(__builtin_ctzll(m) >> 3);

    #elif defined(CX_COMPILER_MSVC) && defined(CX_ARCH_64)
        u32 idx;
        _BitScanForward64(&idx, m);
        return u32(idx >> 3);

    #else
        u8 const* b = ptrcu8(&x);
        for (u32 i = 0; i < 8; ++i) 
            if (b[i] == 0) 
                return i;
        return 8;
    #endif
}

finline onedef cexpr proc cx_strlen_u8(u8 const* s) -> isize {
    const u8* p = s;

    // align to 8 (optional; helps some CPUs)
    while ((uptr(p) & 7u) != 0u) {
        if (*p == 0) return (isize)(p - s);
        ++p;
    }

    for (;;) {
        u64 x;
        // memcpy avoids UB from strict-aliasing and unaligned loads
        ::memcpy(&x, p, sizeof(x));

        if (cx_has_zero_byte_u64(x)) {
            return isize(p - s) + cx_first_zero_byte_u64(x);
        }
        p += 8;
    }
}

finline onedef cexpr proc cx_strlen(char const* txt) noexce -> isize
{
    return isize(::strlen(txt));
}

/**
    @pre
    - `txt` must be null-terminated
**/
finline onedef cexpr proc cx_strlen(u8 const* txt) noexce -> isize
{
    return isize(::strlen(ptrcch8(txt)));
}

#ifndef CX_STRING_HH
#define CX_STRING_HH

namespace cx {

struct StringView {
    CX_MEMBER_ALIASES(u8, isize);
    using Self = StringView;

    u8* ptr;
    Size len;

    finline cexpr proc operator[](Size const idx) noexce -> u8 const& { return ptr[idx]; }

    finline cexpr proc begin() noexce -> Iter { return ptr; }
    finline cexpr proc end() noexce -> Iter { return ptr + len; }

    finline cexpr proc begin() const noexce -> Kter { return ptr; }
    finline cexpr proc end() const noexce -> Kter { return ptr + len; }
};

template<typename T>
finline onedef cexpr proc string_view(T const& src) noexce -> StringView;

template<typename T>
finline onedef cexpr proc string_view(T const* src) noexce -> StringView;

template<typename T, typename S>
finline onedef cexpr proc string_view(T const* src, S len = -1) noexce -> StringView;

template<typename T>
predicate is_string_like = requires(T t) { []{ cx::string_view(t); }(); };

template<> predicate is_string_like<StringView> = true;

static_assert(is_string_like<StringView>);

struct StringIter {
    using Size = isize;
    using Self = StringIter;

    StringView const& str;
    isize pos = 0;
};

template<>
finline onedef cexpr proc string_view(u8 const* txt, isize len) noexce -> StringView
{
    if (len < 0) unlike {
        len = cx_strlen(txt);
    }
    return {ptru8(txt), len};
}

template<>
finline onedef cexpr proc string_view(char const* txt) noexce -> StringView
{
    return string_view(ptrcu8(txt), cx_strlen(txt));
}

}       // namespace cx
#endif  // CX_STRING_HH
