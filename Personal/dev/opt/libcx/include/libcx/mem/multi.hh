/** @file libcx/mem/multi.hh **/
// TODO:
// - documentation (especilly requires)
#ifndef CX_MEM_MULTI_HH
#define CX_MEM_MULTI_HH
#include <libcx/uti/typeseq.hh>
#include <libcx/mem/common.hh>
#include <libcx/mem/heap.hh>
#include <libcx/arr/array_v2.hh>

namespace cx::mem {

//−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
// Utilities

/**
    Computes the total size of `num` elements of each type in `Ts` with proper
    alignment per type.

    @ret
    - the total size in bytes
**/
template<typename... Ts> inln cons fn size_of_multi(isize num) noexce -> isize
{
    isize tot = 0;
    ((tot = (mem::align_up(tot, align_of(Ts))), tot += size_of(Ts) * num), ...);
    return tot;
}

/// Let `ptr` be a pointer, let be `num` the number of elements of type `T`, let
/// `off` be the current offset `off` from `ptr`, let `U` be the type of the next
/// element. This operation computes the offset to next element with proper
/// alignment. Returns: the computed offset.
template<typename T, typename U>
inln cons fn align_up_multi(anyptr ptr, usize num) noexce -> anyptr
{
  ptr = (anyptr) ((uptr) ptr + num * size_of(T));
  uptr aln = align_of(U);
  return (anyptr) ((uptr(ptr) + (aln - 1)) & ~(aln - 1));
}

/// Copies `num` elements of each type in `Ts...` from `src` to `dst` tuple-wise.
/// Requires: each pointer in `src` and `dst` refers to `num` elements.
template<CpAsble... Ts>
fn copy_multi(uti::Tuple<Ts*...> const& dst, uti::Tuple<Ts*...> const& src, usize num) -> void
{
  [&]<usize... I>(uti::IndexSeq<I...>) inln_clos -> void {
    (mem::mem_copy(uti::get<I>(dst), uti::get<I>(src), num * size_of(Ts)), ...);
  }(uti::index_seq_pack<Ts...>{});
}

/// XXX:
template<arr::CArrayType<anyptr> Arr, CpAsble... Ts>
fn copy_multi(Arr& dst, Arr& src, usize num) -> void
{
  usize i = 0;
  [&]() inln_clos -> void {
    ((mem::mem_copy(dst[i], src[i], num * size_of(Ts)), i++), ...);
  }();
}

/// Moves `num` elements of each type in `Ts...` from `src` to `dst` tuple-wise.
/// Requires: each pointer in `src` and `dst` refers to `num` elements.
template<CpOrMvAsble... Ts>
fn take_multi(uti::Tuple<Ts*...> const& dst, uti::Tuple<Ts*...> const& src, usize num) -> void
{
  [&]<usize... I>(uti::IndexSeq<I...>) inln_clos -> void {
    (mem::mem_take_type<Ts>(uti::get<I>(dst), uti::get<I>(src), num * size_of(Ts)), ...);
  }(uti::index_seq_pack<Ts...>{});
}
/// XXX:
///
template<arr::CArrayType<anyptr> Arr>
fn take_multi(Arr& dst, Arr& src, usize num) -> void
{
  using Seq = Arr::Types;
  usize i = 0;
  [&]<usize... I>(uti::IndexSeq<I...>) inln_clos -> void {
    ((mem::mem_take_type<uti::TypeAt<I, Seq>>(dst[i], src[i], num), i++), ...);
  }(uti::IndexSeq<uti::type_seq_size<Seq>>{});
}

// −−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
//  Allocator

/// Allocates `num` elements of each type in `Ts...`.
/// The storage is *not* initialized, use `place_multi` for that.
/// Returns: `[ptr, null]` on success,
///          `[null, err]` if `num == 0` or allocation fails,
/// Note: `size_of_multi<Ts...>(num)` bytes aligned to `max(alignof(Ts)...)`.
template<typename... Ts> fn allocate_multi(usize num) noexce -> Res<anyptr, ErrorCode>
{
  if (num == 0) {
    return {nullptr, Invalid_Arg};
  }
  usize bytes = mem::size_of_multi<Ts...>(num);

  cx_unused(bytes);
  // TODO: from here till end
  // isize align = cx::max({align_of(Ts)...});
  // anyptr res = ::operator new(bytes, Align{align}, nothrow);
  return {null, none};
}

/// Creates and places `num` zero-initialized objects of each type
/// in `Ts...` at `ptr`, with proper alignment per type.
/// Returns: `{ptr, null}` on success;
///          `{null, err}` if `ptr == null` or `num == 0`.
/// Layout: `[T₀[num]][padding(T₀)][T₁[num]][padding(T₁)] ... [Tₙ[num]]`.
template<DefInitble... Ts>
fn place_multi_zii(anyptr ptr, usize num) noexce -> Result<anyptr>
{
  if (ptr == nullptr) {
    return {nullptr, cx_null_err("`ptr` cannot be `null`")};
  }
  if (num == 0) {
    return {nullptr, cx_arg_err("`num` cannot be `0`")};
  }

  anyptr p = ptr;
  fn do_place = [&]<typename T>() inln_clos -> void {
    p = mem::align_up<T>(p);
    for (usize i = 0; i < num; i++) {
      ::new ((T*) p + i) T();
    }
    p = (T*) p + num;
  };
  (do_place.template operator()<Ts>(), ...);

  return {ptr, nullptr};
}

/// Creates and places at most `num` objects of each type in `Ts...` from `lists...`
/// at `ptr`, with proper alignment per type.
/// Returns: `{ptr, null}` on success,
///          `{null, err}` if `ptr == null`, `num == 0`, or `{list.size > num, ...}`.
/// Requires: valid storage at `ptr` for `num` objects of each `T` in `Ts...`.
///
template<typename... Ts>
fn place_multi(usize num, anyptr ptr, initls<Ts>... lists) noexce -> Result<anyptr>
{
  if (!ptr) {
    return {nullptr, cx_arg_err("`ptr` cannot be `null`")};
  }
  if (num == 0) {
    return {nullptr, cx_arg_err("`num` cannot be `0`")};
  }
  if (num < std::max({lists.size()...})) {
    return {nullptr, cx_arg_err("`num` cannot be less than the largest list size")};
  }

  anyptr p = ptr;
  fn do_place = [&]<typename T>(initls<T> list) inln_clos -> void {
    p = mem::align_up<T>(p);
    auto it = list.begin();
    for (usize i = 0; i < list.size(); i++) {
      ::new ((T*) p + i) T(it[i]);
    }
    p = (T*) p + list.size();
  };
  (do_place(lists), ...);

  return {ptr, nullptr};
}

/// Allocates storage and places contiguously `num` zero-initialized objects of
/// each type in `Ts`, with proper alignment per type.
/// Returns: `{ptr, null}` on success;
///          `{null, err}` if `num == 0` or allocation fails.
/// Layout: `[T₀[num]] [padding(T₀)] [T₁[num]] [padding(T₁)] ... [Tₙ[num]]`.
template<typename... Ts> fn make_multi_zii(usize num) -> Result<anyptr>
{
  auto [ptr, err] = mem::allocate_multi<Ts...>(num);
  if (err) {
    return {nullptr, uti::take(err)};
  }
  return mem::place_multi_zii<Ts...>(ptr, num);
}

/// Allocates and places contiguously `num` elements of each type in `Ts` given
/// by `lists` at `ptr`, with proper alignment per type.
/// Returns: `{ptr, null}` on success;
///          `{null, err}` if `num == 0`, allocation fails or `{list.size > num, ...}`.
/// Layout: `[T₀[num]] [padding(T₀)] [T１[num]] [padding(T１)] ... [Tₙ[num]]`.
template<typename... Ts>
fn make_multi(usize num, initls<Ts>... lists) noexce -> Result<anyptr>
{
  auto [ptr, err] = mem::allocate_multi<Ts...>(num);
  if (err) {
    return {nullptr, uti::take(err)};
  }
  return mem::place_multi<Ts...>(num, ptr, lists...);
}

/// Deallocation procedure to release memory allocated with `allocate_multi`.
/// Returns: `{{}, null}` on success;
///          `{{}, err}` if `ptr == null`.
/// Note: does not destroy objects.
template<typename... Ts> fn deallocate_multi(anyptr ptr) noexce -> Result<>
{
  if (!ptr) {
    return {empty, cx_null_err("`ptr` cannot be `null`")};
  }
  usize align = std::max({align_of(Ts)...});
  ::operator delete(ptr, Align{align}, nothrow);
  return {empty, nullptr};
}

/// Computes the begin pointers of each `[Ts[new_num]]` block inside `ptr`.
/// Returns: `Tuple<Ts*...>` begin pointers, in `Ts...` order.
template<typename... Ts>
inln cons fn priv__bind_multi_tup(anyptr ptr, usize new_num) -> Tuple<Ts*...>
{
  fn bind_and_advance = [&]<typename T>() inln_clos -> T* {
    ptr = mem::align_up<T>(ptr);
    T* beg = cast(T*, ptr);
    ptr = cast(anyptr, beg + new_num);
    return beg;
  };

  return Tuple<Ts*...>{bind_and_advance.template operator()<Ts>()...};
}

///
template<typename... Ts>
inln cons fn priv__bind_multi_arr(anyptr ptr, usize new_num)
  -> arr::Array<anyptr, sizeof...(Ts)>
{
  fn bind_and_advance = [&]<typename T>() inln_clos -> anyptr {
    ptr = mem::align_up<T>(ptr);
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
fn
reallocate_multi(Tuple<Ts*...> src, usize num, usize new_num) noexce -> Result<anyptr>
// assumes always grows
{
  anyptr ptr = (anyptr) std::get<0>(src);
  auto [new_ptr, err] = mem::make_multi_zii<Ts...>(new_num);
  if (err) {
    return {ptr, uti::take(err)};
  }
  Tuple<Ts*...> dst = mem::priv__bind_multi_tup<Ts...>(new_ptr, new_num);
  mem::take_multi<Ts...>(dst, src, num);
  mem::deallocate_multi<Ts...>(ptr);
  return {new_ptr, nullptr};
}

}  // namespace cx::mem
#endif  // CX_MEM_MULTI_HH

// −−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
//  Testing

// #define CX_TEST 1
#ifdef CX_TEST
#include <libcx/io/io.hh>

template<typename T> fn print_from(anyptr ptr, usize const off, usize const num) -> void
{
  using CX;
  T* tmp = (T*) mem::ptr_add(ptr, off);
  for (usize i = 0; i < num; i++) {
    io::printfn("{}", tmp[i]);
  }
}

template<typename T> fn print_n(anyptr ptr, usize const num) -> void
{
  using CX;
  T* tmp = (T*) ptr;
  for (usize i = 0; i < num; i++) {
    io::printfn("{}", tmp[i]);
  }
}

void test_make_multi_1()
{
  using CX;
  auto [ptr, err] = mem::make_multi<i32, char, f64>(
    3, {1, 2, 3}, {'a', 'b', 'c'}, {1.11, 2.22, 3.33});
  if (err != nullptr) {
    return;
  }

  print_from<i32>(ptr, 0, 3);
  print_from<char>(ptr, 12, 3);
  print_from<f64>(ptr, 16, 3);
}

fn test_memcopy_multi_1() -> void
{
  using CX;
  auto [p, err] = mem::make_multi<u8, f32, i16, f64>(
    4,
    {42, 77, 50, 6},
    {1.11, 2.22, 3.33, 4.44},
    {1, 2, 3, 4},
    {1.11, 2.22, 3.33, 4.44});
  auto src = mem::priv__bind_begs<u8, f32, i16, f64>(p, 4);

  anyptr q = new u8[100];
  auto dst = mem::priv__bind_begs<u8, f32, i16, f64>(q, 4);

  mem::memcopy_multi<u8, f32, i16, f64>(dst, src, 4);
  print_n<u8>(q, 4);

  q = mem::align_up_multi<u8, f32>(q, 4);
  print_n<f32>(q, 4);

  q = mem::align_up_multi<f32, i16>(q, 4);
  print_n<i16>(q, 4);

  q = mem::align_up_multi<i16, f64>(q, 4);
  print_n<f64>(q, 4);
}

fn test_bind_multi_1() -> void
{
  using CX;
  u8 backing[100]{};
  auto a = mem::priv__bind_multi<u8, f32, i32>(backing, 10);
  anyptr ptr = a[0];
  printf("ptr: %p    a[0]: %p\n", ptr, a[0]);
  ptr = mem::align_up_multi<u8, f32>(ptr, 10);
  printf("ptr: %p    a[1]: %p\n", ptr, a[1]);
  ptr = mem::align_up_multi<f32, i32>(ptr, 10);
  printf("ptr: %p    a[2]: %p\n", ptr, a[2]);
}

#endif  // CX_TEST

///
// template<typename... Ts> inln cons Result<anyptr>
// place_multi(usize num, anyptr ptr, Ts(&...els)) noexce
// {
//   if (!ptr) {
//     return {nullptr, cx_arg_err("`ptr` cannot be `null`")};
//   }
//   if (num == 0) {
//     return {nullptr, cx_arg_err("`num` cannot be `0`")};
//   }
//   if (num < std::max({count_of(els)...})) {
//     return {nullptr, cx_arg_err("`num` cannot be less than the longest array")};
//   }
//   anyptr tmp = ptr;
//   ((tmp = mem::align_up<Ts>(tmp),
//     tmp = mem::unfold__place_multi<Ts>(num, tmp, els)),
//    ...);
//   return {ptr, nullptr};
// }
