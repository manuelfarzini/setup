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
#include "libcx/mem/_allocator.hh"
#include "libcx/arr/dynamic_array.hh"

namespace cx {
inline namespace mem {

template<typename T> inln cons fn max(T& x) noexce -> T& { return x; }

/**
    Computes the maximum element in `head` and `rest.`.
**/
template<typename Head, typename... Rest>
inln cons fn max(Head& head, Rest&... rest) noexce -> Head&
    where (sizeof...(Rest) > 0 && (same_as<Head, Rest> && ...))
{
    Head& tail = max(rest...);
    if (head < tail) {
        return tail;
    }
    return head;
}

// =========================================
//  Utilities
// =========================================

/**
    Computes the total size of `num` elements of each type in `Ts` with proper
    alignment per type.
    @ret
    - The total size in bytes.
**/
template<typename... Ts>
inln cons fn size_of_multi(isize num) noexce -> isize
{
    isize tot = 0;
    ((tot = (align_up(tot, align_of(Ts))), tot += size_of(Ts) * num), ...);
    return tot;
}


/**
    Let `ptr` be the start of the current array of type `T` and length `num`.
    Computes the address of the first element of the next array of type `U`
    with proper alignment.
    @ret
    - A pointer to the first element of the next array.
**/
template<typename T, typename U>
inln cons fn align_up_multi(ptrany ptr, usize num) noexce -> ptrany
{
    ptr = (ptrany) ((uptr) ptr + num * size_of(T));
    uptr aln = align_of(U);
    return (ptrany) ((uptr(ptr) + (aln - 1)) & ~(aln - 1));
}

/// Copies `num` elements of each type in `Ts...` from `src` to `dst` tuple-wise.
/// Requires: each pointer in `src` and `dst` refers to `num` elements.
template<CpAsble... Ts>
fn copy_multi(Tuple<Ts*...> const& dst, Tuple<Ts*...> const& src, usize num) -> void
{
    [&]<usize... I>(IndexSeq<I...>) inln_clos -> void {
        (mem_copy(get<I>(dst), get<I>(src), num * size_of(Ts)), ...);
    }(index_seq_pack<Ts...>{});
}

///
template<arr::SomeArrayOf<ptrany> Arr, CpAsble... Ts>
fn copy_multi(Arr& dst, Arr& src, usize num) -> void
{
    usize i = 0;
    [&]() inln_clos -> void {
        ((mem_copy(dst[i], src[i], num * size_of(Ts)), i++), ...);
    }();
}

/// Moves `num` elements of each type in `Ts...` from `src` to `dst` tuple-wise.
/// Requires: each pointer in `src` and `dst` refers to `num` elements.
template<CpOrMvAsble... Ts>
fn take_multi(Tuple<Ts*...> const& dst, Tuple<Ts*...> const& src, usize num)
    -> void
{
    [&]<usize... I>(IndexSeq<I...>) inln_clos -> void {
        (mem_take<Ts>(get<I>(dst), get<I>(src), num * size_of(Ts)),
         ...);
    }(index_seq_pack<Ts...>{});
}

///
///
template<arr::SomeArrayOf<ptrany> Arr>
fn take_multi(Arr& dst, Arr& src, usize num) -> void
{
    using Seq = Arr::Types;
    usize i = 0;
    [&]<usize... I>(IndexSeq<I...>) inln_clos -> void {
        ((mem_take<TypeAt<I, Seq>>(dst[i], src[i], num), i++), ...);
    }(IndexSeq<type_seq_size<Seq>>{});
}

// =========================================
// Allocator
// =========================================

/// Allocates `num` elements of each type in `Ts...`.
/// The storage is *not* initialized, use `place_multi` for that.
/// Returns: `[ptr, null]` on success,
///          `[null, err]` if `num == 0` or allocation fails,
/// Note: `size_of_multi<Ts...>(num)` bytes aligned to `max(alignof(Ts)...)`.
template<typename... Ts>
fn allocate_multi(usize num) noexce -> Res<ptrany, ErrorCode>
{
    if (num == 0) {
        return {null, Invalid_Arg};
    }
    usize bytes = size_of_multi<Ts...>(num);

    cx_unused(bytes);
    // TODO: from here till end
    // isize align = cx::max({align_of(Ts)...});
    // ptrany res = ::operator new(bytes, Align{align}, nothrow);
    return {null, null};
}

/// Creates and places `num` zero-initialized objects of each type
/// in `Ts...` at `ptr`, with proper alignment per type.
/// Returns: `{ptr, null}` on success;
///          `{null, err}` if `ptr == null` or `num == 0`.
/// Layout: `[T₀[num]][padding(T₀)][T₁[num]][padding(T₁)] ... [Tₙ[num]]`.
template<DefInitble... Ts>
fn place_multi_zii(ptrany ptr, usize num) noexce -> Res<ptrany, ErrorCode>
{
    if (ptr == null) {
        return {null, Invalid_Ptr};
        // return {null, cx_null_err("`ptr` cannot be `null`")};
    }
    if (num == 0) {
        return {null, Invalid_Arg};
        // return {null, cx_arg_err("`num` cannot be `0`")};
    }

    ptrany p = ptr;
    clos do_place = [&]<typename T>() inln_clos -> void {
        p = align_up<T>(p);
        for (usize i = 0; i < num; i++) {
            ::new ((T*) p + i) T();
        }
        p = (T*) p + num;
    };
    (do_place.template operator()<Ts>(), ...);

    return {ptr, null};
}

/// Creates and places at most `num` objects of each type in `Ts...` from `lists...`
/// at `ptr`, with proper alignment per type.
/// Returns: `{ptr, null}` on success,
///          `{null, err}` if `ptr == null`, `num == 0`, or `{list.size > num, ...}`.
/// Requires: valid storage at `ptr` for `num` objects of each `T` in `Ts...`.
///
template<typename... Ts>
fn place_multi(usize num, ptrany ptr, initls<Ts>... lists) noexce -> Res<ptrany, ErrorCode>
{
    if (!ptr) {
        return {null, Invalid_Ptr};
        // return {null, cx_arg_err("`ptr` cannot be `null`")};
    }
    if (num == 0) {
        return {null, Invalid_Arg};
        // return {null, cx_arg_err("`num` cannot be `0`")};
    }
    // custom variadic max
    if (num < max({lists.size()...})) {
        return {null, Invalid_Arg};
        // return {null, cx_arg_err("`num` cannot be less than the largest list size")};
    }

    ptrany p = ptr;
    clos do_place = [&]<typename T>(initls<T> list) inln_clos -> void {
        p = align_up<T>(p);
        auto it = list.begin();
        for (usize i = 0; i < list.size(); i++) {
            ::new ((T*) p + i) T(it[i]);
        }
        p = (T*) p + list.size();
    };
    (do_place(lists), ...);

    return {ptr, null};
}

/// Allocates storage and places contiguously `num` zero-initialized objects of
/// each type in `Ts`, with proper alignment per type.
/// Returns: `{ptr, null}` on success;
///          `{null, err}` if `num == 0` or allocation fails.
/// Layout: `[T₀[num]] [padding(T₀)] [T₁[num]] [padding(T₁)] ... [Tₙ[num]]`.
template<typename... Ts> fn make_multi_zii(usize num) -> Res<ptrany>
{
    auto [ptr, err] = allocate_multi<Ts...>(num);
    if (err) {
        return {null, take(err)};
    }
    return place_multi_zii<Ts...>(ptr, num);
}

/// Allocates and places contiguously `num` elements of each type in `Ts` given
/// by `lists` at `ptr`, with proper alignment per type.
/// Returns: `{ptr, null}` on success;
///          `{null, err}` if `num == 0`, allocation fails or `{list.size > num, ...}`.
/// Layout: `[T₀[num]] [padding(T₀)] [T１[num]] [padding(T１)] ... [Tₙ[num]]`.
template<typename... Ts>
fn make_multi(usize num, initls<Ts>... lists) noexce -> Res<ptrany, ErrorCode>
{
    auto [ptr, err] = allocate_multi<Ts...>(num);
    if (err) {
        return {null, take(err)};
    }
    return place_multi<Ts...>(num, ptr, lists...);
}

/// Deallocation procedure to release memory allocated with `allocate_multi`.
/// Returns: `{{}, null}` on success;
///          `{{}, err}` if `ptr == null`.
/// Note: does not destroy objects.
template<typename... Ts>
fn deallocate_multi(ptrany ptr) noexce -> ErrorCode
{
    if (!ptr) {
        return Invalid_Ptr;
    }
    usize align = max({align_of(Ts)...});
    ::operator delete(ptr, max_align_t(align));
    return null;
}

/// Computes the begin pointers of each `[Ts[new_num]]` block inside `ptr`.
/// Returns: `Tuple<Ts*...>` begin pointers, in `Ts...` order.
template<typename... Ts>
inln cons fn _bind_multi_tup(ptrany ptr, usize new_num) -> Tuple<Ts*...>
{
    clos bind_and_advance = [&]<typename T>() inln_clos -> T* {
        ptr = align_up<T>(ptr);
        T* beg = cast(T*, ptr);
        ptr = cast(ptrany, beg + new_num);
        return beg;
    };

    return Tuple<Ts*...>{bind_and_advance.template operator()<Ts>()...};
}

// ///
// template<typename... Ts>
// inln cons fn _bind_multi_arr(ptrany ptr, usize new_num) -> arr::Array<ptrany>
// {
//     clos bind_and_advance = [&]<typename T>() inln_clos -> ptrany {
//         ptr = align_up<T>(ptr);
//         T* beg = cast(T*, ptr);
//         ptr = beg + new_num;
//         return beg;
//     };
//
//     return {bind_and_advance.template operator()<Ts>()...};
// }

/// Reallocates `{Ts[num]...}` to `{Ts[new_num]...}`.
/// Returns: `[new_ptr, null]` on success;
///          `[old_ptr, err]` on failure (old storage is kept).
/// Note: does not destroy objects (trivially destructible types).
template<typename... Ts>
fn reallocate_multi(Tuple<Ts*...> src, usize num, usize new_num) noexce -> Res<ptrany>
// assumes always grows
{
    ptrany ptr = (ptrany) get<0>(src);
    auto [new_ptr, err] = make_multi_zii<Ts...>(new_num);
    if (err) {
        return {ptr, take(err)};
    }
    Tuple<Ts*...> dst = _bind_multi_tup<Ts...>(new_ptr, new_num);
    take_multi<Ts...>(dst, src, num);
    deallocate_multi<Ts...>(ptr);
    return {new_ptr, null};
}

}       // namespace mem
}       // namespace cx
#endif  // CX_MEM_MULTI_HH

// −−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
//  Testing

#define CX_TEST 1
#ifdef CX_TEST
#include <stdio.h>

// template<typename T> fn print_from(ptrany ptr, usize const off, usize const num) -> void
// {
//     using CX;
//     T* tmp = (T*) ptr_add(ptr, off);
//     for (usize i = 0; i < num; i++) {
//         io::printfn("{}", tmp[i]);
//     }
// }

// template<typename T> fn print_n(ptrany ptr, usize const num) -> void
// {
//     using CX;
//     T* tmp = (T*) ptr;
//     for (usize i = 0; i < num; i++) {
//         io::printfn("{}", tmp[i]);
//     }
// }

// void test_make_multi_1()
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
//     ptrany q = new u8[100];
//     auto dst = _bind_begs<u8, f32, i16, f64>(q, 4);
//
//     memcopy_multi<u8, f32, i16, f64>(dst, src, 4);
//     print_n<u8>(q, 4);
//
//     q = align_up_multi<u8, f32>(q, 4);
//     print_n<f32>(q, 4);
//
//     q = align_up_multi<f32, i16>(q, 4);
//     print_n<i16>(q, 4);
//
//     q = align_up_multi<i16, f64>(q, 4);
//     print_n<f64>(q, 4);
// }

// fn test_bind_multi_1() -> void
// {
//     using CX;
//     u8 backing[100]{};
//     auto a = _bind_multi_arr<u8, f32, i32>(backing, 10);
//     ptrany ptr = a[0];
//     printf("ptr: %p    a[0]: %p\n", ptr, a[0]);
//     ptr = align_up_multi<u8, f32>(ptr, 10);
//     printf("ptr: %p    a[1]: %p\n", ptr, a[1]);
//     ptr = align_up_multi<f32, i32>(ptr, 10);
//     printf("ptr: %p    a[2]: %p\n", ptr, a[2]);
// }

#endif  // CX_TEST

///
// template<typename... Ts> inln cons Res<ptrany>
// place_multi(usize num, ptrany ptr, Ts(&...els)) noexce
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
//   ptrany tmp = ptr;
//   ((tmp = align_up<Ts>(tmp),
//     tmp = unfold__place_multi<Ts>(num, tmp, els)),
//    ...);
//   return {ptr, null};
// }
