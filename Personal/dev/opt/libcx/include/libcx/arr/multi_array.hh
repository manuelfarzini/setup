#define KONST(x) KONST##x
#define KONST_i8    Ki8
#define KONST_i16   Ki16
#define KONST_i32   Ki32
#define KONST_i64   Ki64
#define KONST_i128  Ki128
#define KONST_u8    Ku8
#define KONST_u16   Ku16
#define KONST_u32   Ku32
#define KONST_u64   Ku64
#define KONST_u128  Ku128
#define KONST_f16   Kf16
#define KONST_f32   Kf32
#define KONST_f64   Kf64
#define KONST_f80   Kf80
#define KONST_f128  Kf128
#define KONST_b8    Kb8
#define KONST_b16   Kb16
#define KONST_b32   Kb32
#define KONST_char  Char

#define CX_EXPAND(x) x

#define CX__COUNT_ARGS(                                                        \
    _1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,                 \
    _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32,  \
    N, ...)  N

#define CX_COUNT_ARGS(...)                                       \
  CX_EXPAND(CX__COUNT_ARGS(__VA_ARGS__,                         \
      32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, \
      17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1))

// applica M(a) per ogni argomento (unrolled)
#define CX_FOR_EACH_1(M, a) M(a)
#define CX_FOR_EACH_2(M, a, ...) M(a) CX_FOR_EACH_1(M, __VA_ARGS__)
#define CX_FOR_EACH_3(M, a, ...) M(a) CX_FOR_EACH_2(M, __VA_ARGS__)
#define CX_FOR_EACH_4(M, a, ...) M(a) CX_FOR_EACH_3(M, __VA_ARGS__)
#define CX_FOR_EACH_5(M, a, ...) M(a) CX_FOR_EACH_4(M, __VA_ARGS__)
#define CX_FOR_EACH_6(M, a, ...) M(a) CX_FOR_EACH_5(M, __VA_ARGS__)
#define CX_FOR_EACH_7(M, a, ...) M(a) CX_FOR_EACH_6(M, __VA_ARGS__)
#define CX_FOR_EACH_8(M, a, ...) M(a) CX_FOR_EACH_7(M, __VA_ARGS__)
#define CX_FOR_EACH_9(M, a, ...) M(a) CX_FOR_EACH_8(M, __VA_ARGS__)
#define CX_FOR_EACH_10(M, a, ...) M(a) CX_FOR_EACH_9(M, __VA_ARGS__)
#define CX_FOR_EACH_11(M, a, ...) M(a) CX_FOR_EACH_10(M, __VA_ARGS__)
#define CX_FOR_EACH_12(M, a, ...) M(a) CX_FOR_EACH_11(M, __VA_ARGS__)
#define CX_FOR_EACH_13(M, a, ...) M(a) CX_FOR_EACH_12(M, __VA_ARGS__)
#define CX_FOR_EACH_14(M, a, ...) M(a) CX_FOR_EACH_13(M, __VA_ARGS__)
#define CX_FOR_EACH_15(M, a, ...) M(a) CX_FOR_EACH_14(M, __VA_ARGS__)
#define CX_FOR_EACH_16(M, a, ...) M(a) CX_FOR_EACH_15(M, __VA_ARGS__)
#define CX_FOR_EACH_17(M, a, ...) M(a) CX_FOR_EACH_16(M, __VA_ARGS__)
#define CX_FOR_EACH_18(M, a, ...) M(a) CX_FOR_EACH_17(M, __VA_ARGS__)
#define CX_FOR_EACH_19(M, a, ...) M(a) CX_FOR_EACH_18(M, __VA_ARGS__)
#define CX_FOR_EACH_20(M, a, ...) M(a) CX_FOR_EACH_19(M, __VA_ARGS__)
#define CX_FOR_EACH_21(M, a, ...) M(a) CX_FOR_EACH_20(M, __VA_ARGS__)
#define CX_FOR_EACH_22(M, a, ...) M(a) CX_FOR_EACH_21(M, __VA_ARGS__)
#define CX_FOR_EACH_23(M, a, ...) M(a) CX_FOR_EACH_22(M, __VA_ARGS__)
#define CX_FOR_EACH_24(M, a, ...) M(a) CX_FOR_EACH_23(M, __VA_ARGS__)
#define CX_FOR_EACH_25(M, a, ...) M(a) CX_FOR_EACH_24(M, __VA_ARGS__)
#define CX_FOR_EACH_26(M, a, ...) M(a) CX_FOR_EACH_25(M, __VA_ARGS__)
#define CX_FOR_EACH_27(M, a, ...) M(a) CX_FOR_EACH_26(M, __VA_ARGS__)
#define CX_FOR_EACH_28(M, a, ...) M(a) CX_FOR_EACH_27(M, __VA_ARGS__)
#define CX_FOR_EACH_29(M, a, ...) M(a) CX_FOR_EACH_28(M, __VA_ARGS__)
#define CX_FOR_EACH_30(M, a, ...) M(a) CX_FOR_EACH_29(M, __VA_ARGS__)
#define CX_FOR_EACH_31(M, a, ...) M(a) CX_FOR_EACH_30(M, __VA_ARGS__)
#define CX_FOR_EACH_32(M, a, ...) M(a) CX_FOR_EACH_31(M, __VA_ARGS__)

#define CX_FOR_EACH(M, ...) \
  CX_EXPAND(CX_JOIN2(CX_FOR_EACH_, CX_COUNT_ARGS(__VA_ARGS__))(M, __VA_ARGS__))

#define CX_KONST(T) onedef cons usize CX_JOIN2(K, T) = (usize)__COUNTER__;

/// \file libcx/arr/multi_array.hh
#ifndef CX_ARR_MULTI_ARRAY_HH
#define CX_ARR_MULTI_ARRAY_HH
#include <libcx/mem/multi.hh>
#include <libcx/uti/typeseq.hh>
#include <libcx/arr/array_v2.hh>

#define Types_In(arr) cx::uti::rm_cvref<decltype(arr)>::Types

namespace cx::arr {

//−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
// Utilities

// Same type or reference of the same type
// Multi typewise version
template<uti::CTypeList L1, uti::CTypeList L2, usize I = 0,
         Requires(uti::type_list_len<L1> == uti::type_list_len<L2>)>
struct ___same_or_ref_multi {
  using T = uti::TypeAt<I, L1>;
  using U = uti::TypeAt<I, L2>;
  glob predicate value = uti::same_or_ref<T, U> &&
                           ___same_or_ref_multi<L1, L2, I + 1>::value;
};

template<uti::CTypeList L1, uti::CTypeList L2> 
struct ___same_or_ref_multi<L1, L2> { glob predicate value = true; };

template<uti::CTypeList Ts, uti::CTypeList Us>
predicate same_or_ref_multi = ___same_or_ref_multi<Ts, Us>::value;

template<typename U> concept CLegalType = 
    uti::CPlainArithmetic<U> || uti::CPlainRawPointer<U>;

template<typename U, typename... Ts> concept LegalTypeAmong =
    uti::PlainArithmeticAmong<U, Ts...> || uti::PlainRawPointerAmong<U, Ts...>;

#define Legal_Type cx::arr::CLegalType auto
#define Legal_Type_Among cx::arr::LegalTypeAmong auto
  

//−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
// Multi Array

/// A set of parallel dynamic arrays of different `Types (Ts`).
/// It is composed by the following:
/// - a tuple of pointers to each array `ptrs (std::tuple<Ts*...>)`;
/// - a number of active elements `len (usize)`;
/// - a dynamic storage capacity of `cap  (usize)`.
///
template<CLegalType... Ts> requires(sizeof...(Ts) > 1)
struct MultiArray {
  using Types = uti::TypeList<Ts...>;
  glob onedef cons usize rows = sizeof...(Ts);
  glob onedef cons Array<usize, rows> sizes{size_of(Ts)...};
  glob onedef cons Array<usize, rows> aligns{align_of(Ts)...};

  arr::Array<void*, rows> ptrs;
  usize len{};
  usize cap{};

  // cons fn operator[](this auto& arr, usize row, usize col) noexce
  //   -> void* { return mem::ptr_add(arr.ptrs[row], col * sizes[row]); }
};


//−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
// Utilities

CX_CONCEPT_GEN_TEMPL(MultiArray, is_multi_array, CMultiArray, 
                      VA_(typename... Ts), VA_(Ts...));
#define Multi_Array cx::arr::CMultiArray auto

#define MULTI_ROWS(X) \
  X(i32)              \
  X(f64)              \
  X(u8)
#define X(T) \
  onedef cons usize k##T = (usize) __COUNTER__;
MULTI_ROWS(X);
#undef X

#define DEFINE_MULTI_ARRAY(Name, ...) \
  CX_FOR_EACH(CX_KONST, __VA_ARGS__) \
  using Name = MultiArray<__VA_ARGS__>;

//−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
// Operations

/// Get…
///
template<CMultiArray Arr> fn get(Arr& arr, usize row, usize col) -> void*
{
  return *mem::ptr_add(arr.ptrs[row], col * Arr::sizes[row]);
}

///
///
template<CMultiArray Arr, usize Row>
fn get(Arr& arr, usize col) -> uti::TypeAt<Row, typename Arr::Types>&
{
  return *mem::ptr_add(arr.ptrs[Row], col * Arr::sizes[Row]);
}

///
///
template<usize N, CLegalType... Ts, 
         EnableIf(N >= 8 && uti::is_power_of_two(N))>
fn initialize(MultiArray<Ts...>& arr) -> Result<>
{
  auto [ptr, err] = mem::make_multi_zii<Ts...>(N);
  if (err) {
    return {cx_empty{}, uti::take(err)};
  }
  arr.ptrs = mem::priv__bind_multi_arr<Ts...>(ptr, N);
  arr.len = 0;
  arr.cap = N;
  return {cx_empty{}, nullptr};
}

///
// template<usize N, CLegalType... Ts, 
//          EnableIf(N >= 8 && uti::is_power_of_two(N))>
// fn initialize(MultiArray<Ts...>& arr) -> Result<>
// {
//   auto [ptr, err] = mem::make_multi_zii<Ts...>(N);
//   if (err) {
//     return {cx_empty{}, uti::move(err)};
//   }
//   arr.ptrs = mem::priv__bind_begs<Ts...>(ptr, N);
//   arr.len = 0;
//   arr.cap = N;
//   return {cx_empty{}, nullptr};
// }

#define base_ptr(multi_arr) multi_arr.ptrs[0]

template<CLegalType... Ts> fn reallocate(MultiArray<Ts...>& arr) -> Result<>
{
  arr::Array old_begs = arr.ptrs;
  usize new_cap = arr.cap * 2;

  auto [new_ptr, err_0] = mem::make_multi_zii<Ts...>(new_cap);
  if (err_0) {
    return {cx_empty{}, uti::take(err_0)};
  }

  arr.ptrs = mem::priv__bind_multi_arr<Ts...>(new_ptr, new_cap);
  mem::take_multi(arr.ptrs, old_begs, arr.len);

  auto [_, err_1] = mem::deallocate_multi<Ts...>(base_ptr(arr));
  if (err_1) {
    return {cx_empty{}, uti::take(err_1)};
  }

  arr.cap = new_cap;
  return {cx_empty{}, nullptr};
}

///
// template<CLegalType... Ts> fn reallocate(MultiArray<Ts...>& arr) -> Result<>
// {
//   Tuple<Ts*...> old_begs = arr.ptrs;
//   usize new_cap = arr.cap * 2;
//
//   auto [new_ptr, err_0] = mem::make_multi_zii<Ts...>(new_cap);
//   if (err_0) {
//     return {cx_empty{}, uti::move(err_0)};
//   }
//
//   arr.ptrs = mem::priv__bind_begs<Ts...>(new_ptr, new_cap);
//   mem::memmove_multi<Ts...>(arr.ptrs, old_begs, arr.len);
//
//   auto [_, err_1] = mem::deallocate_multi<Ts...>(std::get<0>(old_begs));
//   if (err_1) {
//     return {cx_empty{}, uti::move(err_1)};
//   }
//
//   arr.cap = new_cap;
//   return {cx_empty{}, nullptr};
// }

///
// template<CMultiArray Arr, typename... Ts,
//          Requires(same_or_ref_multi<typename Arr::Types, uti::TypeList<Ts...>>)>
// fn push_back(Arr& arr, Ts&&... els) -> Result<>
// {
//   if (arr.cap == arr.len) {
//     auto [_, err] = arr::reallocate(arr);
//     if (err) {
//       return {cx_empty{}, uti::take(err)};
//     }
//   }
//
//   auto tup = std::forward_as_tuple(uti::forward<Ts>(els)...);
//
//   [&]<usize... I>(std::index_sequence<I...>) inln_clos -> void {
//     ((std::get<I>(arr.ptrs)[arr.len] = std::get<I>(tup)), ...);
//   }(std::index_sequence_for<Ts...>{});
//
//   arr.len += 1;
//   return {cx_empty{}, nullptr};
// }

/// Questo qui!!!!
template<CMultiArray Arr, typename... Ts,
         Requires(same_or_ref_multi<typename Arr::Types, uti::TypeList<Ts...>>)>
fn push_back(Arr& arr, Ts&&... els) -> Result<>
{
  using L = uti::TypeList<Ts...>;
  if (arr.cap == arr.len) {
    auto [_, err] = arr::reallocate(arr);
    if (err) {
      return {cx_empty{}, uti::take(err)};
    }
  }
 
  fn unfold = [&]<usize I>(auto&& elm) inln_clos -> void {
    using T = uti::rm_cvref<uti::TypeAt<I, L>>;
    T* dst = (T*) mem::ptr_add(arr.ptrs[I], arr.len * Arr::sizes[I]);
    *dst = uti::forward<decltype(elm)>(elm);
  };
    
  [&]<usize... I>(std::index_sequence<I...>) inln_clos -> void {
    (unfold.template operator()<I>(uti::forward<Ts>(els)), ...);
  }(std::index_sequence_for<Ts...>{});

  arr.len += 1;
  return {cx_empty{}, nullptr};
}


// #define CX_TEST 1
#ifdef CX_TEST

void test_multi_array_1() {
  using CX;
  arr::MultiArray<usize, f32> arr{};
  arr::initialize<8>(arr);
  arr::push_back(arr, usize(42), f32(4.2));
  arr::push_back(arr, usize(43), f32(4.3));

  printf("len: %zu\n", arr.len);
  printf("cap: %zu\n", arr.cap);
  // printf("arr[0, 0]: %zu\n", std::get<0>(arr.ptrs)[0]);
  // printf("arr[0, 1]: %zu\n", std::get<0>(arr.ptrs)[1]);
  // printf("arr[1, 0]: %.1f\n", std::get<1>(arr.ptrs)[0]);
  // printf("arr[1, 1]: %.1f\n", std::get<1>(arr.ptrs)[1]);

  arr::push_back(arr, usize(44), f32(4.4));
  printf("len: %zu\n", arr.len);
  printf("cap: %zu\n", arr.cap);
  // printf("arr[0, 2]: %zu\n", std::get<0>(arr.ptrs)[2]);
  // printf("arr[1, 2]: %.1f\n", std::get<1>(arr.ptrs)[2]);
}

}      // namespace cx::arr
#endif // CX_TEST
#endif // CX_ARR_MULTI_ARRAY_HH


//
//
//
//
//−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
// Padding
//−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
//
//
//
//
//



/// Defines a cast semantics where the target type `U` is a `Legal_Type_Among`.
//  XXX: 
//  maybe should add arithmetic case too
//  maybe useless, should review
//  template<typename U, typename... Ts> using to_legal = std::conditional_t<
//      uti::PlainRawPointerAmong<U, Ts...>, uti::rm_cvref<U>, std::conditional_t<
//      uti::PlainSmartPointerAmong<U, Ts...>, uti::rm_cvref<U>,
//      std::decay_t<U>>>;


// NOTE:(manu): this is defined here since it requires a defined multi array,

// TODO: reason about if this is a good design choice. I plan to add an allocator in
// the API with default behaviour as a standard heap allocator. If i do that it is
// surel better to pass the multi array aned not a raw pointer, on the other hand
// passing a pointer, a cap etc what is needed is more consistent with the rest of
// the API i have made so far.
