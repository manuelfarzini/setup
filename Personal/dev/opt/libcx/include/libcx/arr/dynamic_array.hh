/// @file libcx/arr/dynamic_array.hh
#ifndef CX_ARR_DYNAMIC_ARRAY_HH
#define CX_ARR_DYNAMIC_ARRAY_HH
#include <concepts>
#include <libcx/config.hh>
#include <libcx/traits.hh>
#include <libcx/concepts.hh>
#include <libcx/uti/compare.hh>
#include <libcx/uti/iterator.hh>
#include "libcx/uti/members.hh"
#include <libcx/mem/allocator.hh>

#include <utility> // XXX:

// #include <libcx/mem/memory.hh>


namespace cx::arr {

template<typename Tp, typename Sz = isize>
struct Array {
    CX_MEMBER_ALIASES(Tp, Sz);
    using Self = Array<Tp, Sz>;

    Elem* ptr{null};
    Size len{0};
    Size cap{0};
    mem::Allocator a = mem::heap_allocator();

    // this can also take by moving that is not always what we want to do
    finline cexpr proc operator[](this auto&& arr, isize const idx) noexce -> auto&& {
        return std::forward_like<declt(arr)>(arr.ptr[idx]);
    }

    finline cexpr auto operator[](this auto& arr, isize const idx) noexce -> Elem&
    {
        return arr.ptr[idx];
    }
    // finline cexpr auto operator[](this auto const& arr, isize const idx) noexce
    // {
    //     return arr.ptr[idx];
    // }

    finline cexpr Iter beg() noexce { return ptr; }
    finline cexpr Iter end() noexce { return ptr + len; }
    finline cexpr Kter beg() const noexce { return ptr; }
    finline cexpr Kter end() const noexce { return ptr + len; }
};

template<typename Tp> Array(Tp) -> Array<Tp>;

template<typename Tp, typename... Up>
Array(Tp, Up...)
    -> Array<std::enable_if_t<uti::bvariand<std::is_same_v<Tp, Up>...>, Tp>>;

CX_CONCEPT_GEN_TEMPL(Array, is_array_type, SomeArray, typename Tp, Tp);

// with duplicates
template<
    SomeArray         Arr,
    typename          Key,
    EqualOrderType    Cmp = uti::Leq
>
cexpr proc find_last(Arr const& arr, Key const& key, Cmp cmp) noexce -> isize
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

// always increase capacity
finline cexpr proc reallocate(SomeArray auto& arr) noexce -> Result<>
{
    if (arr.ptr == null) {
        return {cx::empty, cx_null_err("`arr.ptr` is `null`")};
    }
    auto [new_ptr, err] = mem::reallocate<elem_in(arr)>(arr.ptr, arr.cap, arr.cap * 2);
    if (err) {
        return {cx::empty, uti::take(err)};
    }
    arr.ptr = new_ptr;
    arr.cap = arr.cap * 2;
    return {cx::empty, null};
}

/// XXX:(manu): possible unsafe shrink?
finline onedef cexpr proc reallocate(dyn_arr& arr, isize const new_cap) noexce
    -> Result<>
{
    arr.ptr = mem::reallocate<elem_in(arr)>(arr.ptr, arr.cap, new_cap);
    if (arr.ptr == null) {
        return {cx::empty, cx_arg_err("failed to reallocate")};
    }
    arr.cap = arr.cap * 2;
    return {cx::empty, null};
}

//  inline const priv proc allocate(arr: &[auto], cap: cons isize) -> void!
//  {
//      if cap == 0 {
//          return _, ArgumentError("capacity must be 0")
//      }
//      arr.ptr, err = mem.allocate<elem_in>(arr, cap) !
//      arr.cap = cap
//      return _, nil
//  }

finline onedef cexpr proc alloc(dyn_arr& arr, isize const cap) noexce -> Result<>
{
    if (arr.ptr != null) {
        return {cx::empty, cx_arg_err("`arr.ptr` must be `null`")};
    }
    if (cap == 0) {
        return {cx::empty, cx_arg_err("the initial `cap` cannot be 0")};
    }

    auto [ptr, err] = mem::allocate<elem_in(arr)>(cap);
    if (err) {
        return {cx::empty, uti::take(err)};
    }

    arr.ptr = ptr;
    arr.cap = cap;
    return {cx::empty, null};
}

template<typename Tp>
finline onedef cexpr proc create(isize const cap = 8) -> Result<dynarr<Tp>>
{
    Array<Tp> arr{};
    auto [val, err] = arr::alloc(arr, cap);
    return {arr, err};
}

/// TODO:
template<
    SomeArray Arr,
    typename Elm,
    Requires(uti::same_or_cvref<uti::ElemIn<Arr>, Elm>)>
onedef cexpr proc push(Arr& arr, Elm&& elm) noexce -> Result<>
{
    if (arr.len == arr.cap) {
        auto [val, err] = arr::reallocate(arr);
        if (err) {
            return {empty, uti::take(err)};
        }
    }
    arr[arr.len] = uti::forward<Elm>(elm);
    arr.len += 1;
    return {empty, null};
}


finline intern cexpr bool rep_ok(dyn_arr& arr) noexce
{
    if (arr.len > arr.cap) {
        return false;
    }
    if (arr.ptr == null && arr.len != 0) {
        return false;
    }
    return true;
}

proc test() -> void
{
    using CX;
    dynarr<i32> arr{};
    auto [_, err] = arr::alloc(arr, 8);
    i32 x = 42;
    i32& lvr = x;
    i32 const&& rvr = 42;

    arr::push(arr, lvr);
    arr::push(arr, x);
    arr::push(arr, 42);
    arr::push(arr, rvr);
}


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
//   finline cexpr char operator[](isize const i) const noexce { return ptr[i]; }
//   finline cexpr char& operator[](isize const i) noexce { return ptr[i]; }
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
//   // s.ptr = mem::make<char>(5);
//     // s.len = 5;
//     // s.cap = 5;
//   return s;
// }
//
// String append(String s, String t) {
//   if (s.len + t.len < s.cap) {
//     s.len += t.len;
//     mem::memcopy<char>(s.ptr + s.len, t.ptr, t.len);
//     s[s.len + t.len] = '\0';
//     return s;
//   }
//   // FIX:(manu): handle other cases...
//   return s;
// }
//
// finline cexpr String to_string(dyn_arr& arr) noexce {
//   String res;
//   String s = to_string(arr[0]);
//
//   res.ptr = mem::allocate<char>(arr.len * (s.len + 1) + 1);
//   res.len = 1 + arr.len * s.len;
//
//   for (isize i = 0; i < arr.len; i++) {
//     to_string(res, arr[i]);
//     res[res.len] = ',';
//   }
//   res[res.len] = '\0';
//   return res;
// }

}  // namespace cx::arr

// NOTE: facendo partire la capacita' da un minimo, come 8 o 16, risparmio le prime
// reallocazioni, inoltre posso anche fare in modo di taggare il puntatore per avere
// la capacita' in multipli di 2 usando pochi bit, cosi' entro una certa lunghezza e
// capacita' e' addirittura possibile taggare nel puntatore entrambe!


#endif  // CX_ARR_DYNAMIC_ARRAY_HH
