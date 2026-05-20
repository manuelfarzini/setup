/** @file libcx/arr/dynamic_array.hh **/

#ifndef CX_ARR_DYNAMIC_ARRAY_HH
#define CX_ARR_DYNAMIC_ARRAY_HH

#include "libcx/config.hh"
#include "libcx/traits.hh"
#include "libcx/concepts.hh"
#include "libcx/uti/compare.hh"
#include "libcx/uti/iterator.hh"
#include "libcx/uti/members.hh"
#include "libcx/mem/_allocator.hh"

namespace cx {
inline namespace arr {

template<ZeroInitble E, typename S = isize, SomeAllocator A = HeapAllocator>
struct Array {
    CX_DEFINE_MEMBER_TYPES(E, S);
    using Alc = A;
    using Self = Array<E, S>;

    Iter ptr{null};
    Size len{0};
    Size cap{0};
    Alc  alc{};

    // this can also take by moving that is not always what we want to do
    // inln cons fn operator[](this auto&& arr, isize const idx) noexce -> auto&&
    // {
    //     return std::forward_like<declt(arr)>(arr.ptr[idx]);
    // }

    inln cons fn operator[](this auto& arr, isize const idx) noexce -> Elem&
    {
        return arr.ptr[idx];
    }
    inln cons fn operator[](this auto const& arr, isize const idx) noexce
    {
        return arr.ptr[idx];
    }

    inln cons fn begin() noexce  -> Iter { return ptr; }
    inln cons fn end() noexce -> Iter { return ptr + len; }
    inln cons fn begin() const noexce -> Kter { return ptr; }
    inln cons fn end() const noexce -> Kter { return ptr + len; }
};

template<typename E> Array(E) -> Array<E>;

template<typename E, typename... O>
Array(E, O...) -> Array<enable_if<bvariand<same_as<E, O>...>, E>>;

CX_CONCEPT_GEN_TEMPL(
    Array, is_array, SomeArray,
    VA_(typename E, typename S, SomeAllocator A), VA_(E, S, A)
);

template<typename T, typename Elm> predicate is_array_of = false;
template<typename T, typename S, SomeAllocator A> predicate is_array_of<Array<T, S, A>, T> = true;
template<typename Arr, typename Elm> concept SomeArrayOf = is_array_of<Arr, Elm>;

inln priv cons fn rep_ok(SomeArray auto& arr) noexce -> b32
{
    if ((arr.ptr == null && arr.len != 0) || arr.len < 0 || arr.cap < 0
        || arr.len > arr.cap || !is_power_of_two(arr.cap)
    ) {
        return false;
    }
    return true;
}

cons fn resize(SomeArray auto& arr, isize new_cap) noexce -> ErrorCode
{
    CX_USING_MEMBER_TYPES_OF(arr);
    assert(is_power_of_two(new_cap));

    isize bytes = size_of(Elem);
    auto [ptr, err] = aligned_resize(
        arr.alc, arr.ptr, new_cap * bytes, arr.cap * bytes, align_of(Elem)
    ) or_return err;

    arr.ptr = Iter(ptr);
    arr.cap = new_cap;
    return null;
}

cons fn resize(SomeArray auto& arr) noexce -> ErrorCode
{
    if (arr.cap == 0) {
        return resize(arr, 8);
    }
    return resize(arr, arr.cap * 2);
}

template<ZeroInitble E, typename S = isize, SomeAllocator A = HeapAllocator>
cons fn make(isize cap, A alc = heap_allocator()) noexce -> Res<Array<E, S, A>, ErrorCode>
{
    auto arr = Array<E, S, A>{.ptr{null}, .len{0}, .cap{0}, .alc{alc}};
    auto err = resize(arr, cap) or_return {Array<E, S, A>{}, err};
    err = init_ty<E>(arr.ptr, arr.cap) or_return {Array<E, S, A>{}, err};
    arr.cap = cap;
    return {arr, null};
}

// Forwarding, do I want to do that?
cons fn push_tail(SomeArray auto& arr, auto&& elm) noexce -> ErrorCode
    where same_or_cvref<elem_in(arr), declt(elm)>
{
    if (arr.len == arr.cap) {
        auto err = resize(arr) or_return err;
    }
    arr.ptr[arr.len] = forward<declt(elm)>(elm);
    arr.len++;
    return null;
}

// Now always returns a copy
cons fn pop_back(SomeArray auto& arr) noexce -> Res<elem_in(arr), ErrorCode>
{
    if (arr.len == 0) {
        return {null, Operation_Fail};
    }
    arr.len--;
    return {arr.ptr[arr.len], null};
}

cons fn lshift(SomeArray auto& arr, isize off) noexce -> ErrorCode
{
    // TODO:(manu) custom bound checking macro for the whole array framework
    printf("arr.len %ld\n", arr.len);
    printf("arr.cap %ld\n", arr.cap);
    printf("arr.ptr %p\n", arr.ptr);

    assert(rep_ok(arr));
    assert(off > 0);
    printf("%ld\n", arr.len);
    printf("%ld\n", arr.cap);
    printf("%ld\n", off);
    printf("%ld\n", arr.len + off);
    printf("%s\n", arr.len + off <= arr.cap ? "True" : "False");
    assert(off <= arr.len);

    mem_copy(arr.ptr, arr.ptr + off, arr.len - off);
    arr.len -= off;
    return null;
}

enum dtype { f32, f64 };
template<dtype d, usize n> struct _simd;

#if CX_NEON

template<> struct _simd<dtype::f32, 4> {
    // using Elem = f32;
    using Type = float32x4_t;
};
template<> struct _simd<dtype::f64, 2> {
   using Type = float64x2_t; 
};

#endif

template<dtype d, usize n> using simd = _simd<d, n>::Type;

// with duplicates
template<SomeArray Arr, typename Key, EqualOrderType Cmp = Leq>
cons fn find_last(Arr const& arr, Key const& key, Cmp cmp) noexce -> isize
    where is_total_ordered_w<ElemIn<Arr>, PlainT<Key>>
{
    isize j = !cmp(arr[arr.len / 2], key) ? 0 : arr.len / 2;
    for (; j < arr.len && cmp(arr[j], key);) {
        j = j + 1;
    }
    return -j - 1;
    // XXX: think about what to do if the key is a primitive, this can be optimized
    // passing a primitive or little struct rather than a reference
}


//
// /// XXX:(manu): possible unsafe shrink?
// inln onedef cons fn reallocate(dyn_arr& arr, isize const new_cap) noexce
//     -> Result<>
// {
//     arr.ptr = reallocate<elem_in(arr)>(arr.ptr, arr.cap, new_cap);
//     if (arr.ptr == null) {
//         return {cx::Empty_Value, cx_arg_err("failed to reallocate")};
//     }
//     arr.cap = arr.cap * 2;
//     return {cx::Empty_Value, null};
// }
//
// //  inline const priv fn allocate(arr: &[auto], cap: cons isize) -> void!
// //  {
// //      if cap == 0 {
// //          return _, ArgumentError("capacity must be 0")
// //      }
// //      arr.ptr, err = mem.allocate<elem_in>(arr, cap) !
// //      arr.cap = cap
// //      return _, nil
// //  }
//
// inln onedef cons fn alloc(dyn_arr& arr, isize const cap) noexce -> Result<>
// {
//     if (arr.ptr != null) {
//         return {cx::Empty_Value, cx_arg_err("`arr.ptr` must be `null`")};
//     }
//     if (cap == 0) {
//         return {cx::Empty_Value, cx_arg_err("the initial `cap` cannot be 0")};
//     }
//
//     auto [ptr, err] = allocate<elem_in(arr)>(cap);
//     if (err) {
//         return {cx::Empty_Value, take(err)};
//     }
//
//     arr.ptr = ptr;
//     arr.cap = cap;
//     return {cx::Empty_Value, null};
// }
//
// template<typename T>
// inln onedef cons fn create(isize const cap = 8) -> Result<dynarr<T>>
// {
//     Array<T> arr{};
//     auto [val, err] = arr::alloc(arr, cap);
//     return {arr, err};
// }

// =====================================
// Testing
// =====================================

#if CX_TEST_DYNARR
#include <stdio.h>

fn test_dyn_array_basic() -> void
{
    using namespace cx;
    auto [arr, err] = make<isize>(8);
    if (err) {
        printf("%d\n", err);
        return;
    }
    for (isize i = 0; i < 10; i++) {
        push_tail(arr, i);
    }
    assert(arr.len == 10);
    assert(arr.cap == 16);
    lshift(arr, 1);
    for (auto a : arr) {
        printf("%ld\n", a);
    }
    puts("done");
}

#endif


// struct Test {
//   uint32 a{0};
//   uint32 b{0};
// };
//
// struct String {
//   isize len{0};
//   isize cap{0};
//   char* ptr{null};
//
//   inln cons char operator[](isize const i) const noexce { return ptr[i]; }
//   inln cons char& operator[](isize const i) noexce { return ptr[i]; }
// };
//
// String to_string(String s, Test t) {
//   // FIX:(manu): calculate number of digits
//   if (s.cap > s.len + 5) {
//     snprintf(s.ptr + s.len, 5, "%d %d", t.a, t.b);
//     s.len += 5;
//     s.ptr[s.len] = '\0';
//     return s;
//   }
//
//   // s.ptr = make<char>(5);
//     // s.len = 5;
//     // s.cap = 5;
//   return s;
// }
//
// String append(String s, String t) {
//   if (s.len + t.len < s.cap) {
//     s.len += t.len;
//     memcopy<char>(s.ptr + s.len, t.ptr, t.len);
//     s[s.len + t.len] = '\0';
//     return s;
//   }
//   // FIX:(manu): handle other cases...
//   return s;
// }
//
// inln cons String to_string(dyn_arr& arr) noexce {
//   String res;
//   String s = to_string(arr[0]);
//
//   res.ptr = allocate<char>(arr.len * (s.len + 1) + 1);
//   res.len = 1 + arr.len * s.len;
//
//   for (isize i = 0; i < arr.len; i++) {
//     to_string(res, arr[i]);
//     res[res.len] = ',';
//   }
//   res[res.len] = '\0';
//   return res;
// }

}  // namespace arr
}  // namespace cx

// NOTE: facendo partire la capacita' da un minimo, come 8 o 16, risparmio le prime
// reallocazioni, inoltre posso anche fare in modo di taggare il puntatore per avere
// la capacita' in multipli di 2 usando pochi bit, cosi' entro una certa lunghezza e
// capacita' e' addirittura possibile taggare nel puntatore entrambe!


#endif  // CX_ARR_DYNAMIC_ARRAY_HH
