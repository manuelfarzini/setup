#ifndef  CX_MEM_BASE_HH
#define  CX_MEM_BASE_HH
#include <libcx/config.hh>
#include <libcx/traits.hh>
#include <libcx/concepts.hh>
#include <libcx/__utils/ownership.hh>
#include <libcx/uti/tuple.hh>

namespace cx::mem {

// enum AllocatorError : u8 {
//     Alctor_OutOfMem,
//     Alctor_InvalidPtr,
//     Alctor_InvalidArg,
//     Alctor_InvalidMode,
// };

glob cexpr u8 Operation_Fail = 1;
glob cexpr u8 Invalid_Ptr = 2;
glob cexpr u8 Invalid_Arg = 3;
glob cexpr u8 Bad_Alloc = 4;
glob cexpr u8 Invalid_Mode = 5;
using ErrorCode = u8;
using AllocatorError = u8;
using InitError = u8;

glob cexpr isize PTR_SIZE = isize_of(anyptr);
glob cexpr isize PTR_ALIGN = ialign_of(anyptr);
glob cexpr isize MAX_SIZE = isize_of(max_align_t);
glob cexpr isize MAX_ALIGN = ialign_of(max_align_t);
glob cexpr isize DEF_ALIGN = 2 * MAX_ALIGN;

static_assert(MAX_ALIGN % PTR_ALIGN == 0, "`max_align` must be a multiple of `ptr_align`");
static_assert((MAX_ALIGN & (MAX_ALIGN - 1)) == 0, "`max_align` must be a power of 2");
static_assert(DEF_ALIGN == 8  || DEF_ALIGN == 16, "`def_align` must be 8 or 16"); // XXX:

/** 
    Copies `size` bytes from `src` to `dst`.
  
    @para
    - `dst`: the destination pointer
    - `src`: the source pointer

    @pre
    - `src` and `dst` have `size` bytes.
**/
onedef cexpr proc mem_copy(anyptr dst, ptrcany src, usize size) -> void {
    // NOTE(manu)
    // should i implement mem_copy? libc is already heavily optimized
    ::memcpy(dst, src, size);
}

/**
    Copies `num` elements of type `T` from `src` to `dst`.
  
    @para
    - `dst`: the destination pointer
    - `src`: the source pointer

    @pre
    - `src` and `dst` have `num` elements
**/
template<CpAsble T>
onedef cexpr proc mem_copy_type(T* dst, T const* src, usize num) noexce -> void
{
    // XXX:(manu)#zero_init# custom predicate
    if constexpr (is_triv_dtble<T>) {
        mem_copy(dst, src, num * isize_of(T));
    } else {
        for (usize i = 0; i < num; i++) {
            dst[i] = src[i];
        }
    }
}

/**
    Takes `num` elements of type `T` from `src` to `dst`
    If `T` is not move assignable, then behaves like `memcopy`.
  
    @para
    - `dst`: the destination pointer
    - `src`: the source pointer

    @pre
    - `src` and `dst` have `num` elements
**/
template<CpOrMvAsble T>
onedef cexpr proc mem_take_type(T* dst, T* src, usize num) noexce -> void {
    for (usize i = 0; i < num; i++) {
        dst[i] = uti::take(src[i]);
    }
}

/**
    Sets `n` bytes of memory at `data` to `v`.
  
    @para
    - `data`: pointer to the memory to be set
    - `v`: value to set the memory to
    - `n`: size of the memory to be set [Byte]

    @pre
    - `data` has at least `n` bytes.
**/
onedef cexpr proc mem_set(anyptr data, u8 val, usize size) -> ErrorCode {
    //  NOTE(manu)
    //  > libc wrapper, actually is the fastest
    //  > found that is the fastest on my m1 macbook pro
    //    since it is implemented in arm64 asm
    //  > maybe on other platoforms I should use another version

    // #if defined(CX_SYSTEM_OSX)
        anyptr res = ::memset(data, val, size);
        if (!res) {
            return Operation_Fail;
        }
    // #endif
    return none;
}

/**
    Zeroes `size` bytes of memory at `data`.

    @para
    - `data`: pointer to the memory to be zeroed
    - `size`: size of the memory to be zeroed [Byte]

    @ret
    - `ErrorCode`

    @pre
    - `data` has at least `size` bytes.
    - `data` is not null
**/
onedef cexpr proc mem_zero(anyptr data, usize size) -> ErrorCode
{
    return mem_set(data, 0, size);
}

/**
    Zeroes `size` bytes of memory at `data`.
  
    @para
    - `data`: pointer to the memory to be zeroed
    - `size`: size of the memory to be zeroed [Byte]
  
    @ret
    - The generated `ErrorCode` if any, `none` otherwise

    @pre
    - `data` has at least `size` bytes.

    @nota
    - intended to be used when dealing with virtual memory as it
      would be already mapped to a 0 page
**/
onedef cexpr proc mem_zero_condition(anyptr data, isize size) -> ErrorCode
{
    //  NOTE(manu)
    //  gb.h memset inspired version
    //  it's faster compared to the odin like one and it's much faster than
    //  libc ::memset with 0s if the memory is already mapped to a 0 page
    //  probably I can improve this even more under certain assumptions,
    //  like using a u64 splat directly
    if (data == null) unlike {
        return Invalid_Ptr;
    }
    if (size <= 0)  unlike {
        return Invalid_Arg;
    }

    #define zero_if_not(x)   \
        if (x != 0) unlike { \
                x = 0;       \
            }
    #define ret_if_less(a, b) \
        if (a < b) {          \
            return none;      \
        }

    ptru8 d = ptru8(data);

    zero_if_not(d[0]);
    zero_if_not(d[size - 1]);
    ret_if_less(size, 3);

    zero_if_not(d[1]);
    zero_if_not(d[size - 2]);
    zero_if_not(d[2]);
    zero_if_not(d[size - 3]);
    ret_if_less(size, 7);

    zero_if_not(d[3]);
    zero_if_not(d[size - 4]);
    ret_if_less(size, 9);

    iptr k = -iptr(d) & 3;
    for (isize i = 0; i < k; i += 1) {
        if (d[i] != 0) {
            d[i] = 0;
        }
    }

    d += k;
    size -= k;
    size &= -4;
    u64* d64 = ptru64(d);
    while (size >= 32) {
        if ((d64[0] | d64[1] | d64[2] | d64[3]) != 0) {
            d64[0] = 0;
            d64[1] = 0;
            d64[2] = 0;
            d64[3] = 0;
        }

        d += 32;
        size -= 32;
    }

    while (size >= 4) {
        zero_if_not(*ptru32(d));
        d += 4;
        size -= 4;
    }

    #undef zero_if_not
    #undef ret_if_less
    return none;
}

/**
    Zero intializes `num` contiguous `T` objects at `ptr`.
  
    @para
    - `ptr`: the pointer to the first element
    - `num`: the number of elements

    @ret
    - `[null, err]` if `ptr == null` or `num` == `0`;
    - `[ptr, none]` otherwise.

    @pre
    - sufficient, properly aligned storage at `ptr`.
**/
template<DefInitble T>
nodisc finline onedef cexpr proc init_type(anyptr ptr, usize num) noexce
    -> Res<T*, InitError>
// XXX:(manu)#zero_init# is it the correct concept?
{
    if (num == 0) {
        return {null, Invalid_Arg};
    }
    if (ptr == null) {
        return {null, Invalid_Ptr};
    }

    for (usize i = 0; i < num; i++) {
        ::new (cast(T*, ptr) + i) T();
    }

    return {cast(T*, ptr), none};
}

/**
 

**/
template<typename T, typename... Args>
nodisc onedef cexpr proc init_va(anyptr ptr, isize num, Args const&... args) noexce
    -> Res<T*, InitError>
    where def_initble<T> || ctble<T, Args...> // XXX:(manu)#zero_init#
{
    if (num == 0) {
        return {null, Invalid_Arg};
    }
    if (ptr == null) {
        return {null, Invalid_Ptr};
    }
    for (isize i = 0; i < num; i++) {
        ::new (cast(T*, ptr) + i) T{uti::forward<Args>(args)...};
    }
    return {cast(T*, ptr), none};

}

/**
    Create and place `num` contiguous `T` objects given by `list` at `ptr`.

    @para
    - `ptr`: the pointer to the first element
    - `list`: the initializer list

    @ret
    - `[null, err]` if `ptr` is `null` or `num` is `0`;
    - `[ptr, none]` otherwise.

    @pre
    - sufficient, properly aligned storage at `ptr`.

    @nota
    - The elements are copied
**/
template<typename T, typename U>
nodisc onedef cexpr proc init_ls(anyptr ptr, initls<U> lst) noexce
    -> Res<T*, InitError>
    where ctble<T, U const&>
{
    if (ptr == null) {
        return {null, Invalid_Ptr};
    }

    if (lst.size() == 0) {
        return {null, Invalid_Arg};
    }

    usize i = 0;
    for (auto const& elem : lst) {
        ::new (cast(T*, ptr) + i++) T(elem);
    }
    return {cast(T*, ptr), none};
}

/**
    Uninitializes `num` contiguous `T` objects at `ptr`.

    @para
    - `ptr`: the pointer to the objects
    - `num`: the number of objects

    @ret
    - `[null, err]` if `ptr == null` or `num == 0`
    - `[empty, none]` otherwise

    @pre
    - `ptr` was obtained from `heap_alloc_aligned<T>`
    - `ptr` has `num` elements
**/
template<typename T>
nodisc onedef proc deinit_type(T* ptr, usize num) noexce -> Res<Empty, InitError>
{
    if (ptr == null) {
        return {empty, Invalid_Ptr};
    }
    if (num == 0) {
        return {empty, Invalid_Arg};
    }
    if constexpr (is_triv_dtble<T>) {
    //  XXX:(manu) #zero_init# custom predicate
        return {empty, none};
    } else {
        for (usize i = 0; i < num; i++) {
            ptr[num].~T();
        }
        return {empty, none};
    }
}

}       // namespace cx::mem
#endif  // CX_MEM_BASE_HH
