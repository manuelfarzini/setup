/**
    @file libcx/mem/multi.hh

    Documents memory operations in terms of the parallel-array abstraction
    used to implement multi-array / soa-style containers.
 **/


// TODO:
// - documentation (especilly requires)

#ifndef CX_MEM_MULTI_HH
#define CX_MEM_MULTI_HH

#include "libcx/uti/typeseq.hh"
#include "libcx/mem/common.hh"
#include "libcx/mem/allocator.hh"
#include "libcx/arr/static_array.hh"

namespace cx {
inline namespace mem {

/** Computes the maximum value in `head` and `rest`. **/
template<typename Head, typename... Rest>
inln cons fn max(Head head, Rest... rest) noexce -> Head
    where (sizeof...(Rest) > 0 && is_homogeneous_va<Head, Rest...> && size_of(Head) <= 8)
{
    Head max = head;
    ((max = max < rest ? rest : max), ...);
    return max;
}

/** Computes the maximum element in `head` and `rest`. **/
template<typename Head, typename... Rest>
inln cons fn max(Head& head, Rest&... rest) noexce -> Head&
    where (sizeof...(Rest) > 0 && is_homogeneous_va<Head, Rest...> && size_of(Head) > 8)
{
    Head const* max = &head;
    ((max = *max < rest ? &rest : max), ...);
    return *max;
}

////////////////////////////////////////////
// Utilities

/**
    Computes the maximum alignment required by `Ts...`.
    @ret
    - The maximum alignment in bytes.
**/
template<typename... Ts>
inln cons fn multi_align_of() noexce -> isize
{
    isize aln = DEF_ALIGN;
    ((aln = max(aln, align_of(Ts))), ...);
    return aln;
}

/**
    Computes the total size of `num` elements of each type in `Ts...`,
    including padding required to align each array.
    @ret
    - The total size in bytes.
**/
template<typename... Ts>
inln cons fn multi_size_of(isize num) noexce -> isize
{
    isize off = 0;
    ((off = align_up(off, align_of(Ts)), off += num * size_of(Ts)), ...);
    return off;
}

/**
    Let `off` be the offset of the first element of the current array of type
    `T` and length `num`. Computes the offset of the first element of the next
    array of type `U`.
    @ret
    - The offset in bytes of the next array.
**/
template<typename T, typename U>
inln cons fn multi_next_offset(isize off, isize num) noexce -> isize
{
    return align_up(off + num * size_of(T), align_of(U));
}

/**
    Computes the aligned offset of an array of type `T`.
    Useful when walking the layout type-by-type.
    @ret
    - The aligned offset in bytes.
**/
template<typename T>
inln cons fn multi_align_offset(isize off) noexce -> isize
{
    return align_up(off, align_of(T));
}

/**
    Computes the end offset after `num` elements of type `T`.
    Requires: `off` is already aligned for `T`.
    @ret
    - The offset immediately after the array.
**/
template<typename T>
inln cons fn multi_end_offset(isize off, isize num) noexce -> isize
{
    return off + num * size_of(T);
}

/**
    Computes the typed pointer at `off` bytes from `base`.
    Requires: `off` is correctly aligned for `T`.
    @ret
    - A typed pointer to the array of type `T`.
**/
template<typename T>
inln cons fn multi_ptr_at(mutaptr base, isize off) noexce -> T*
{
    return cast(T*, ptr_add(base, off));
}

/**
    Copies `num` elements of each type in `Ts...` from `src` to `dst`
    tuple-wise.
    Requires: each pointer in `src` and `dst` refers to `num` elements.
**/
template<CpAsble... Ts>
fn multi_mem_move(Tuple<Ts*...> const& dst, Tuple<Ts*...> const& src, isize num) -> void
{
    [&]<usize... I>(IndexSeq<I...>) inln_clos -> void {
        (mem_move(get<I>(dst), get<I>(src), num * size_of(Ts)), ...);
    }(index_seq_va<Ts...>{});
}

/**
    Copies `num` elements of each type described by `Arr::Types`
    from `src` to `dst`.
    Requires: each pointer in `src` and `dst` refers to `num` elements.
**/
template<StaticArrayOf<mutaptr> Arr, typename... Ts>
fn multi_mem_move(Arr& dst, Arr const& src, isize num) -> void
    where (type_seq_size<TypeSeq<Ts...>> <= Arr::cap) // TODO: add to other functions
{
    using Seq = TypeSeq<Ts...>;
    [&]<usize... I>(IndexSeq<I...>) inln_clos -> void {
        (mem_move(dst[I], src[I], num * size_of(TypeAt<I, Seq>)), ...);
    }(IndexSeq<type_seq_size<Seq>>{});
}
#ifndef cx_multi_mem_move_arr
    #define cx_multi_mem_move_arr(dst, src, num, ...) \
        multi_mem_move<declt(dst), __VA_ARGS__>(dst, src, num)
#endif

// TODO: should the functions be inln?

/**
    Moves `num` elements of each type in `Ts...` from `src` to `dst`
    tuple-wise.
    Requires: each pointer in `src` and `dst` refers to `num` elements.
**/
template<CpOrMvAsble... Ts>
fn multi_mem_take(Tuple<Ts*...> const& dst, Tuple<Ts*...> const& src, isize num) -> void
{
    [&]<usize... I>(IndexSeq<I...>) inln_clos -> void {
        (mem_take<Ts>(get<I>(dst), get<I>(src), num), ...);
    }(index_seq_va<Ts...>{});
}

/**
    Moves `num` elements of each type described by `Arr::Types`
    from `src` to `dst`.
    Requires: each pointer in `src` and `dst` refers to `num` elements.
**/
template<StaticArrayOf<mutaptr> Arr, typename... Ts>
fn multi_mem_take(Arr& dst, Arr& src, isize num) -> void
{
    using Seq = TypeSeq<Ts...>;
    isize i = 0;
    [&]<isize... I>(IndexSeq<I...>) inln_clos -> void {
        ((mem_take<TypeAt<I, Seq>>(dst[i], src[i], num), i++), ...);
    }(IndexSeq<type_seq_size<Seq>>{});
    // TODO: simplify with Seq::size
}
#ifndef cx_multi_mem_take_arr
    #define cx_multi_mem_take_arr(dst, src, num, ...) \
        multi_mem_take<declt(dst), __VA_ARGS__>(dst, src, num)
#endif

////////////////////////////////////////////
// Allocator

/// Allocates `num` elements of each type in `Ts...`.
/// The storage is *not* initialized, use `multi_init` for that.
/// Returns: `[ptr, null]` on success,
///          `[null, err]` if `num == 0` or allocation fails,
/// Note: `multi_size_of<Ts...>(num)` bytes aligned to `max(alignof(Ts)...)`.
template<typename... Ts>
fn multi_allocate(isize num) noexce -> Res<mutaptr, ErrorCode>
{
    if (num == 0) {
        return {null, Invalid_Arg};
    }
    isize bytes = multi_size_of<Ts...>(num);

    cx_unused(bytes);
    // TODO: from here till end
    // isize align = cx::max({align_of(Ts)...});
    // mutaptr res = ::operator new(bytes, Align{align}, nothrow);
    return {null, null};
}

/// Creates and places `num` zero-initialized objects of each type
/// in `Ts...` at `ptr`, with proper alignment per type.
/// Returns: `{ptr, null}` on success;
///          `{null, err}` if `ptr == null` or `num == 0`.
/// Layout: `[T₀[num]][padding(T₀)][T₁[num]][padding(T₁)] ... [Tₙ[num]]`.
template<DefInitble... Ts>
fn multi_init_zii(mutaptr ptr, isize num) noexce -> Res<mutaptr, ErrorCode>
{
    if (ptr == null) {
        return {null, Invalid_Ptr};
        // return {null, cx_null_err("`ptr` cannot be `null`")};
    }
    if (num == 0) {
        return {null, Invalid_Arg};
        // return {null, cx_arg_err("`num` cannot be `0`")};
    }

    mutaptr p = ptr;
    clos do_init = [&]<typename T>() inln_clos -> void {
        p = align_up<T>(p);
        for (isize i = 0; i < num; i++) {
            ::new ((T*) p + i) T();
        }
        p = (T*) p + num;
    };
    (do_init.template operator()<Ts>(), ...);

    return {ptr, null};
}

/// Creates and places at most `num` objects of each type in `Ts...` from `lists...`
/// at `ptr`, with proper alignment per type.
/// Returns: `{ptr, null}` on success,
///          `{null, err}` if `ptr == null`, `num == 0`, or `{list.size > num, ...}`.
/// Requires: valid storage at `ptr` for `num` objects of each `T` in `Ts...`.
///
template<typename... Ts>
fn multi_init(isize num, mutaptr ptr, initls<Ts>... lists) noexce -> Res<mutaptr, ErrorCode>
{
    if (!ptr) {
        return {null, Invalid_Ptr};
        // return {null, cx_arg_err("`ptr` cannot be `null`")};
    }
    if (num == 0) {
        return {null, Invalid_Arg};
        // return {null, cx_arg_err("`num` cannot be `0`")};
    }
    // TODO:
    // custom variadic max
    if (num < max({lists.size()...})) {
        return {null, Invalid_Arg};
        // return {null, cx_arg_err("`num` cannot be less than the largest list size")};
    }

    mutaptr p = ptr;
    clos do_init = [&]<typename T>(initls<T> list) inln_clos -> void {
        p = align_up<T>(p);
        auto it = list.begin();
        for (isize i = 0; i < list.size(); i++) {
            ::new ((T*) p + i) T(it[i]);
        }
        p = (T*) p + list.size();
    };
    (do_init(lists), ...);

    return {ptr, null};
}

/// Allocates storage and places contiguously `num` zero-initialized objects of
/// each type in `Ts`, with proper alignment per type.
/// Returns: `{ptr, null}` on success;
///          `{null, err}` if `num == 0` or allocation fails.
/// Layout: `[T₀[num]] [padding(T₀)] [T₁[num]] [padding(T₁)] ... [Tₙ[num]]`.
template<typename... Ts> fn multi_make_zii(isize num) -> Res<mutaptr>
{
    auto [ptr, err] = multi_allocate<Ts...>(num);
    if (err) {
        return {null, take(err)};
    }
    return multi_init_zii<Ts...>(ptr, num);
}

/// Allocates and places contiguously `num` elements of each type in `Ts` given
/// by `lists` at `ptr`, with proper alignment per type.
/// Returns: `{ptr, null}` on success;
///          `{null, err}` if `num == 0`, allocation fails or `{list.size > num, ...}`.
/// Layout: `[T₀[num]] [padding(T₀)] [T１[num]] [padding(T１)] ... [Tₙ[num]]`.
template<typename... Ts>
fn multi_make(isize num, initls<Ts>... lists) noexce -> Res<mutaptr, ErrorCode>
{
    auto [ptr, err] = multi_allocate<Ts...>(num);
    if (err) {
        return {null, take(err)};
    }
    return multi_init<Ts...>(num, ptr, lists...);
}

/// Deallocation procedure to release memory allocated with `multi_allocate`.
/// Returns: `{{}, null}` on success;
///          `{{}, err}` if `ptr == null`.
/// Note: does not destroy objects.
template<typename... Ts>
fn multi_deallocate(mutaptr ptr) noexce -> ErrorCode
{
    if (!ptr) {
        return Invalid_Ptr;
    }
    isize align = max({align_of(Ts)...});
    ::operator delete(ptr, max_align_t(align));
    return null;
}

/// Computes the begin pointers of each `[Ts[new_num]]` block inside `ptr`.
/// Returns: `Tuple<Ts*...>` begin pointers, in `Ts...` order.
template<typename... Ts>
inln cons fn _multi_bind_tup(mutaptr ptr, isize new_num) -> Tuple<Ts*...>
{
    clos bind_and_advance = [&]<typename T>() inln_clos -> T* {
        ptr = align_up<T>(ptr);
        T* beg = cast(T*, ptr);
        ptr = cast(mutaptr, beg + new_num);
        return beg;
    };
    return Tuple<Ts*...>{bind_and_advance.template operator()<Ts>()...};
}

///
template<typename... Ts>
inln cons fn _multi_bind_arr(mutaptr ptr, isize new_num) -> StaticArray<mutaptr, sizeof...(Ts)>
{
    clos bind_and_advance = [&]<typename T>() inln_clos -> mutaptr {
        ptr = align_up<T>(ptr);
        T* beg = cast(T*, ptr);
        ptr = beg + new_num;
        return beg;
    };

    return {bind_and_advance.template operator()<Ts>()...};
}

/// Reallocates `{Ts[num]...}` to `{Ts[new_num]...}`.
/// Returns: `[new_ptr, null]` on success;
///          `[old_ptr, err]` on failure (old storage is kept).
/// Note: does not destroy objects (trivially destructible types).
template<typename... Ts>
fn multi_reallocate(Tuple<Ts*...> src, isize num, isize new_num) noexce -> Res<mutaptr>
// assumes always grows
{
    mutaptr ptr = (mutaptr) get<0>(src);
    auto [new_ptr, err] = multi_make_zii<Ts...>(new_num);
    if (err) {
        return {ptr, take(err)};
    }
    Tuple<Ts*...> dst = multi__bind_tup<Ts...>(new_ptr, new_num);
    multi_mem_take<Ts...>(dst, src, num);
    multi_deallocate<Ts...>(ptr);
    return {new_ptr, null};
}

}       // namespace mem
}       // namespace cx
#endif  // CX_MEM_MULTI_HH

////////////////////////////////////////////
// Testing

#define CX_TEST_MEM_MULTI 1
#ifdef CX_TEST_MEM_MULTI

// template<typename T> fn print_from(mutaptr ptr, isize const off, isize const num) -> void
// {
//     using CX;
//     T* tmp = (T*) ptr_add(ptr, off);
//     for (isize i = 0; i < num; i++) {
//         io::printfn("{}", tmp[i]);
//     }
// }

// template<typename T> fn print_n(mutaptr ptr, isize const num) -> void
// {
//     using CX;
//     T* tmp = (T*) ptr;
//     for (isize i = 0; i < num; i++) {
//         io::printfn("{}", tmp[i]);
//     }
// }

// void multi_test_make_1()
// {
//     using CX;
//     auto [ptr, err] = make_multi<i32, char, f64>(
//         3, {1, 2, 3}, {'a', 'b', 'c'}, {1.11, 2.22, 3.33}
//     );
//     if (err != null) {
//         return;
//     }
//
//     print_from<i32>(ptr, 0, 3);
//     print_from<char>(ptr, 12, 3);
//     print_from<f64>(ptr, 16, 3);
// }

// fn test_memcopy_multi_1() -> void
// {
//     using CX;
//     auto [p, err] = make_multi<u8, f32, i16, f64>(
//         4, {42, 77, 50, 6}, {1.11, 2.22, 3.33, 4.44}, {1, 2, 3, 4},
//         {1.11, 2.22, 3.33, 4.44}
//     );
//     auto src = _bind_multi_tup<u8, f32, i16, f64>(p, 4);
//
//     mutaptr q = new u8[100];
//     auto dst = _bind_begs<u8, f32, i16, f64>(q, 4);
//
//     memcopy_multi<u8, f32, i16, f64>(dst, src, 4);
//     print_n<u8>(q, 4);
//
//     q = multi_align_up<u8, f32>(q, 4);
//     print_n<f32>(q, 4);
//
//     q = multi_align_up<f32, i16>(q, 4);
//     print_n<i16>(q, 4);
//
//     q = multi_align_up<i16, f64>(q, 4);
//     print_n<f64>(q, 4);
// }

// fn test_bind_multi_1() -> void
// {
//     using CX;
//     u8 backing[100]{};
//     auto a = _bind_multi_arr<u8, f32, i32>(backing, 10);
//     mutaptr ptr = a[0];
//     printf("ptr: %p    a[0]: %p\n", ptr, a[0]);
//     ptr = multi_align_up<u8, f32>(ptr, 10);
//     printf("ptr: %p    a[1]: %p\n", ptr, a[1]);
//     ptr = multi_align_up<f32, i32>(ptr, 10);
//     printf("ptr: %p    a[2]: %p\n", ptr, a[2]);
// }

#endif  // CX_TEST

///
// template<typename... Ts> inln cons Res<mutaptr>
// place_multi(isize num, mutaptr ptr, Ts(&...els)) noexce
// {
//   if (!ptr) {
//     return {null, cx_arg_err("`ptr` cannot be `null`")};
//   }
//   if (num == 0) {
//     return {null, cx_arg_err("`num` cannot be `0`")};
//   }
//   if (num < std::max({count_of(els)...})) {
//     return {null, cx_arg_err("`num` cannot be less than the longest array")};
//   }
//   mutaptr tmp = ptr;
//   ((tmp = align_up<Ts>(tmp),
//     tmp = unfold__init_multi<Ts>(num, tmp, els)),
//    ...);
//   return {ptr, null};
// }
