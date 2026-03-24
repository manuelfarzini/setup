/** @file libcx/__container/string.hh **/
#ifndef CX___CONTAINER_STRING_HH
#define CX___CONTAINER_STRING_HH
#include <libcx/config.hh>
#include <libcx/traits.hh>
#include <libcx/mem/allocator.hh>
#include <libcx/__container/string_view.hh>

namespace cx {

struct ___LongStr  { u8* ptr; isize len; };
struct ___ShortStr { u8 buf[16]; };

/**
    A string …
**/
struct String {
    CX_MEMBER_ALIASES(u8, isize);
    using Self = String;

    union {
        ___LongStr l;
        ___ShortStr s;
    };
};

/**
    Creates a new string from the given `txt` of size `len`.
    If `len` is less than 0 then the length of `txt` will be used.
    If `0 <= len <= 16` then the buffer is copied into the string.

    @para
    - `txt`: the pointer to the string
    - `len`: the length of the string

    @ret
    - the new string

    @pre
    - if `len < 0` then `txt` must be null-terminated
    - if `len > 16` then `txt` must be properly allocated (1.)

    @nota
    1. this procedure does not allocate memory
**/
onedef cexpr proc make_string(
    u8 const*         txt,
    isize             len       = 0, 
    mem::Allocator    alctor    = mem::heap_allocator()
) noexce -> String {
    if (len < 0) /* unlike */ {
        len = cx_strlen(txt);
    }
    if (len <= 16) {
        // is this fast or should I use memcpy? how it relates with RVO
        return String{.s{cx_extract16(txt)}};
    }

    auto [ptr, err] = mem::cx_mem_alloc(alctor, len);
    return String{.l{.ptr = ptru8(ptr), .len = len}};
}

/**
    Creates a new string from the given `txt`.

    @para
    - `text`: the source character buffer

    @pre
    - If `len < 0` then `txt` must be null-terminated

    @see
    -  `make_string(u8 const*, isize)`
**/
onedef cexpr proc make_string(
    char const*       txt,
    mem::Allocator    alctor    = mem::heap_allocator()
) noexce -> String {
    return make_string(ptrcu8(txt), cx_strlen(txt), alctor);
}

/**
    Creates a new string from the given `str`.
**/
template<>
onedef cexpr proc string_view(String const& str) noexce -> StringView 
{
    return {str.l.ptr, str.l.len};
}

static_assert(is_string_like<String>);

proc test() -> void {

    cexpr char const* txt = "Hello";

    isize len = cx_strlen(txt);

    String str1{.l{.ptr = ptru8("Hello"), .len = len}};

    String str2{.s{"Hello"}};

    // str2.s.buf[0] = u8('H');
    str2.l.ptr[0] = u8('H');

    String str3{.l{.ptr = ptru8(txt), .len = 5}};

    cx_unused(str1);
    cx_unused(str3);
    // auto s3 = make_string(txt); 
}

}       // namespace cx
#endif  // CX___CONTAINER_STRING_HH
