/// \file libcx/arr/dynamic_array.hh
#ifndef CX_ARR_DYNAMIC_ARRAY_HH
#define CX_ARR_DYNAMIC_ARRAY_HH
#include <libcx/config.hh>
#include <libcx/mem/memory.hh>


namespace cx::arr {

template<typename T, typename S = usize> struct DynamicArray {
  using Elem = T;
  using Size = S;
  using Iter = T*;
  using Kter = T const*;
  using Self = DynamicArray<T>;

  Elem* ptr{null};
  Size len{0};
  Size cap{0};

  finline cexpr T& operator[](usize const idx) noexce {
    return ptr[idx];
  }
  finline cexpr T const& operator[](usize const idx) const noexce {
    return ptr[idx];
  }

  finline cexpr Iter begin() noexce { return ptr; }
  finline cexpr Iter end() noexce { return ptr + len; }
  finline cexpr Kter begin() const noexce { return ptr; }
  finline cexpr Kter end() const noexce { return ptr + len; }
};

template<typename T> DynamicArray(T) -> DynamicArray<T>;

template<typename T, typename... U>
DynamicArray(T, U...) -> DynamicArray<std::enable_if_t<uti::bvariand<std::is_same_v<T, U>...>, T>>;

CX_CONCEPT_GEN_TEMPL(DynamicArray, is_dynamic_array, typename T, T);
#define Dynamic_Array cx::arr::CDynamicArray auto

// with duplicates
template<CDynamicArray Arr, typename Key, uti::CEqualOrderComp Cmp = uti::Leq,
         EnableIf(std::totally_ordered_with<uti::ElemIn<Arr>, uti::PlainT<Key>>)>
cexpr proc find_last(Arr const& arr, Key const& key, Cmp cmp) noexce -> isize
{
  isize j = !cmp(arr[arr.len / 2], key) ? 0 : arr.len /2;
  for (; j < arr.len && cmp(arr[j], key) ;) {
      j = j + 1;
    }
  return -j - 1;
  // XXX: think about what to do if the key is a primitive, this can be optimized
  // passing a primitive or little struct rather than a reference
}

// always increase capacity
finline cexpr proc reallocate(Dynamic_Array& arr) noexce -> Result<>
{
  if (arr.ptr == null) {
    return {cx::empty, cx_null_err("`arr.ptr` is `null`")};
  }
  auto [new_ptr, err] = mem::reallocate<elemIn(arr)>(arr.ptr, arr.cap, arr.cap * 2);
  if (err) {
    return {cx::empty, uti::take(err)};
  }
  arr.ptr = new_ptr;
  arr.cap = arr.cap * 2;
  return {cx::empty, null};
}

/// XXX:(manu): possible unsafe shrink?
finline onedef cexpr proc reallocate(Dynamic_Array& arr, usize const new_cap) noexce
  -> Result<>
{
  arr.ptr = mem::reallocate<elemIn(arr)>(arr.ptr, arr.cap, new_cap);
  if (arr.ptr == null) {
    return {cx::empty, cx_arg_err("failed to reallocate")};
  }
  arr.cap = arr.cap * 2;
  return {cx::empty, null};
}

//  inline const priv proc allocate(arr: &[auto], cap: cons usize) -> void!
//  {
//      if cap == 0 {
//          return _, ArgumentError("capacity must be 0")
//      }
//      arr.ptr, err = mem.allocate<elemIn>(arr, cap) !
//      arr.cap = cap
//      return _, nil
//  }

finline onedef cexpr proc alloc(Dynamic_Array& arr, usize const cap) noexce
  -> Result<>
{
  if (arr.ptr != null) {
    return {cx::empty, cx_arg_err("`arr.ptr` must be `null`")};
  }
  if (cap == 0) {
    return {cx::empty, cx_arg_err("the initial `cap` cannot be 0")};
  }

  auto [ptr, err] = mem::allocate<elemIn(arr)>(cap);
  if (err) {
    return {cx::empty, uti::take(err)};
  }

  arr.ptr = ptr;
  arr.cap = cap;
  return {cx::empty, null};
}

template<typename T>
finline onedef cexpr proc create(usize const cap = 8) -> Result<DynamicArray<T>>
{
  DynamicArray<T> arr{};
  auto [val, err] = arr::alloc(arr, cap);
  return {arr, err};
}

/// TODO:
template<CDynamicArray Arr, typename Elm,
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


finline intern cexpr bool rep_ok(Dynamic_Array& arr) noexce {
  if (arr.len > arr.cap) {
    return false;
  }
  if (arr.ptr == null && arr.len != 0) {
    return false;
  }
  return true;
}

proc test() -> void {
  using CX;
  DynamicArray<i32> arr{};
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
//   usize len{0};
//   usize cap{0};
//   char* ptr{null};
//
//   finline cexpr char operator[](usize const i) const noexce { return ptr[i]; }
//   finline cexpr char& operator[](usize const i) noexce { return ptr[i]; }
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
// finline cexpr String to_string(Dynamic_Array& arr) noexce { 
//   String res;
//   String s = to_string(arr[0]);
//
//   res.ptr = mem::allocate<char>(arr.len * (s.len + 1) + 1);
//   res.len = 1 + arr.len * s.len;
//   
//   for (usize i = 0; i < arr.len; i++) {
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
