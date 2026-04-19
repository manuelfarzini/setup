/// \file libcx/arr/inline_array.hh
#ifndef CX_ARR_INLINE_ARRAY_HH
#define CX_ARR_INLINE_ARRAY_HH
#include <libcx/io/io.hh>
#include <libcx/err/error.hh>
#include <libcx/mem/memory.hh>
#include <libcx/uti/utilities.hh>

// TODO:
// - To manage `StaticString` or  StackArray` maybe should use Base<Derived>
//   pattern with crtp; temp workaround is notifed.
// - Maybe should use StaticString directly instead of string_view for
//   `StringLikeC` or create another concept.
// - Now StringLikeT does a totally different thing that does not fit here.
//   i want to explicitly pass the buffer types, no conversion.
/// - should the functions be inlined?

// Policy:
// - sorted
// - no duplicates
// Alternative names:
// - InlineBuffer
// - FlexBuffer
// - FlexArray


namespace cx::arr {

template<typename T, usize C, typename S = usize> struct InlineArray;
CX_CONCEPT_GEN_TEMPL(
    InlineArray, is_inline_array, CInlineArray,
    VA_(typename T, usize C, typename S), VA_(T, C, S));
#define Inline_Array cx::arr::CInlineArray auto

/// Contiguous `buf`fer of `T` objects with a fixed `cap`acity `C`
/// and a `len`gth indicating current active elements.
template<typename T, usize C, typename S> struct InlineArray {
  static_assert(C > 0, "Array capacity must be greater than zero");
  static_assert(
      std::default_initializable<T>,
      "Element type `T` must be default initializable");
  /// FIX: Is it better to not impose triviality here?
  static_assert(
      std::is_trivially_destructible_v<T>,
      "Element type `T` must be trivially destructible");

  using Elem = T;
  using Size = S;
  using Iter = Elem*;
  using CIter = Elem const*;
  using Self = InlineArray<Elem, C, Size>;

  inline static cexpr Size cap = C;
  Elem buf[C]{};
  Size len{};

  finline cexpr proc operator[](usize const i) noexce
    -> Elem& { return buf[i]; }
  finline cexpr proc operator[](usize const i) const noexce
    -> Elem const& { return buf[i]; }

  finline cexpr proc begin() noexce -> Iter { return buf; }
  finline cexpr proc end() noexce -> Iter { return buf + len; }
  finline cexpr proc begin() const noexce -> CIter { return buf; }
  finline cexpr proc end() const noexce -> CIter { return buf + len; }
};

/// Equality operator.
cexpr proc operator==(Inline_Array const& a, Inline_Array const& b) noexce -> bool
{
  if (&a == &b) {
    return true;
  }
  if (a.len != b.len) {
    return false;
  }

  for (usize i = 0; i < a.len; i++) {
    if (a[i] != b[i]) {
      return false;
    }
  }

  return true;
}

/// Comparison operator.
cexpr proc operator<=>(Inline_Array const& a, Inline_Array const& b) noexce -> auto
{
  if (a.len < b.len) {
    return std::strong_ordering::less;
  }
  if (a.len > b.len) {
    return std::strong_ordering::greater;
  }
  for (usize i = 0; i < a.len; i++) {
    if (a[i] < b[i]) {
      return std::strong_ordering::less;
    }
    if (a[i] > b[i]) {
      return std::strong_ordering::greater;
    }
  }
  return std::strong_ordering::equal;
}

/// Returns `true` if the array is empty.
cexpr proc is_empty(Inline_Array arr) noexce -> bool { return arr.len == 0; }

/// Returns `true` if the array is full.
cexpr proc is_full(Inline_Array arr) noexce -> bool { return arr.len == arr.cap; }

/// Shifts the `arr`'s elements to the right from `i` to `j` by `off` positions.
/// Returns `true` if the shift was successful.
///
template<CInlineArray Arr, typename Size = uti::SizeIn<Arr>>
finline cexpr proc shift_right(Arr& arr, Size i, Size j, Size off) noexce -> bool
{
  if (off == 0 || j >= arr.cap || i >= j || i + off > j) {
    return false;
  }

  for (; j >= i; j--) {
    arr[j] = arr[j - off];
  }

  return true;
}

/// Shifts the `arr`'s elements to the left from `i` to `j` by `off` positions.
/// Returns `true` if the shift was successful.
///
template<CInlineArray Arr, typename Size = uti::SizeIn<Arr>>
finline cexpr proc shift_left(Arr& arr, Size i, Size j, Size off) noexce
    -> bool
{
  if (off == 0 || j >= arr.cap || i >= j || i + off > j) {
    return false;
  }
  for (; i < j - off; i++) {
    arr[i] = arr[i + off];
  }
  return true;
}

///
template<
    CInlineArray Arr, typename Size = uti::SizeIn<Arr>, typename... Args,
    Requires(uti::same_or_cvref<uti::ElemIn<Arr>, Args>&&...)>
cexpr proc push_back(Arr& arr, Size num, Args&&... args) noexce -> bool
{
  if (num == 0) {
    return false;
  }
  mem::place<ElemIn(arr)>(&arr.buf[arr.len], num, args...);
  arr.len += num;
  return true;
}

///
template<CInlineArray Arr, typename Elm = uti::ElemIn<Arr>>
onedef proc push_back(Arr& arr, initlist<Elm> lst) noexce -> bool
{
  if (lst.size() == 0 && lst.size() + arr.len > arr.cap) {
    return false;
  }
  mem::place<ElemIn(arr)>(&arr.buf[arr.len], lst);
  arr.len += lst.size();
  return true;
}

///
template<CInlineArray Arr, typename Elm = uti::ElemIn<Arr>>
onedef proc pop_back(Arr& arr, Elm& res) noexce -> bool
{
  if (arr.len == 0) {
    return false;
  }
  res = arr[arr.len - 1];
  mem::unplace<ElemIn(arr)>(&arr.buf[arr.len - 1]);  // trival dtble → nop
  arr.len--;
  return true;
}

/// Find operation with `ElemType` and an order comparator provided by `cx::uti::`
template<CInlineArray Arr, typename Elm, uti::COrderComp Cmp = uti::Lne,
         Requires(std::totally_ordered<uti::ElemIn<Arr>> &&
                      uti::same_or_cvref<uti::ElemIn<Arr>, Elm>)>
cexpr proc find(Arr& arr, Elm&& elm, Cmp cmp = uti::lne) noexce -> isize
{
  usize i;
  if cexpr (uti::same_as<decltype(cmp), uti::Lne>) {
    i = 0;
    while (i < arr.len && cmp(arr[i], elm)) {
      i++;
    }
    if (i < arr.len && !cmp(arr[i], elm) && uti::eq(arr[i], elm)) {
      return (isize) i;
    }

  } else if cexpr (uti::same_as<decltype(cmp), uti::Gne>) {
    i = arr.len;
    while (i > 0 && cmp(arr[i - 1], elm)) {
      i--;
    }
    if (i > 0 && !cmp(arr[i - 1], elm) && uti::eq(arr[i - 1], elm)) {
      return (isize) i - 1;
    }
  }

  return -i - 1;
}

/// Find operation with `key` whose type may differ from the `ElemType`
/// in `arr` and using a custom comparator not provided by `cx::uti::`.
/// Assumes the array is sorted in ascending order.
/// Returns the insert position `-i - 1` if not found, otherwise `i`,
/// where `i` is the index.
///
template<CInlineArray Arr, typename Key, typename Cmp,
         Requires(!uti::SomeComparator<Cmp> && // disambiguate
                    std::totally_ordered<uti::ElemIn<Arr>> &&
                    std::totally_ordered_with<uti::ElemIn<Arr>, uti::PlainT<Key>> &&
                    std::is_invocable_v<Cmp, uti::ElemIn<Arr>, uti::PlainT<Key>>)>
cexpr proc find(Arr& arr, auto&& key, auto&& cmp) noexce -> isize
{
  usize i = 0;
  while (i < arr.len && cmp(arr[i], key)) {
    i++;
  }
  if (i < arr.len && !cmp(arr[i], key) && uti::eq(arr[i], key)) {
    return (isize) i;
  }
  return -i - 1;
}

///
template<CInlineArray Arr, typename Elm, uti::COrderComp Cmp = uti::Lne,
         Requires(std::totally_ordered<uti::ElemIn<Arr>>&&
                    uti::same_or_cvref<uti::ElemIn<Arr>, Elm>)>
cexpr proc insert(Arr& arr, Elm&& elm, Cmp cmp = uti::lne) noexce -> isize
{
  if (arr.len == arr.cap) {
    return -1;
  }

  isize i = arr::find(arr, elm, cmp);
  if (i >= 0) {
    return -1;
  }
  i = -i - 1;

  for (isize j = arr.len; j > i; j--) {
    arr[j] = arr[j - 1];
  }

  arr[i] = elm;
  arr.len++;
  return i;
}

// cexpr proc remove(Inline_Array& arr, KeyIn(arr) key) -> isize {
// }

}  // namespace cx::arr

#endif  // CX_ARR_INLINE_ARRAY_HH
