#define KONST(x) KONST##x
#define KONST_i8 Ki8
#define KONST_i16 Ki16
#define KONST_i32 Ki32
#define KONST_i64 Ki64
#define KONST_i128 Ki128
#define KONST_u8 Ku8
#define KONST_u16 Ku16
#define KONST_u32 Ku32
#define KONST_u64 Ku64
#define KONST_u128 Ku128
#define KONST_f16 Kf16
#define KONST_f32 Kf32
#define KONST_f64 Kf64
#define KONST_f80 Kf80
#define KONST_f128 Kf128
#define KONST_b8 Kb8
#define KONST_b16 Kb16
#define KONST_b32 Kb32
#define KONST_char Char

#define CX_KONST(T) onedef cons isize CX_JOIN2(K, T) = __COUNTER__;

#define MULTI_ROWS(X) \
    X(i32)            \
    X(f64)            \
    X(u8)
#define X(T)                                      \
    onedef cons isize k##T = (isize) __COUNTER__; \
    MULTI_ROWS(X);
#undef X

#define DEFINE_MULTI_ARRAY(Name, ...)     \
    CX_FOR_EACH(CX_KONST, __VA_ARGS__)    \
    using Name = MultiArray<__VA_ARGS__>;

/** @file libcx/arr/multi_array.hh **/

#ifndef CX_ARR_MULTI_ARRAY_HH
#define CX_ARR_MULTI_ARRAY_HH

#include "libcx/conf/macro.hh"
#include "libcx/arr/static_array.hh"
#include "libcx/mem/multi.hh"
#include "libcx/traits/multi.hh"
#include "libcx/uti/typeseq.hh"

#define TypesIn(arr) cx::rm_cvref<decltype(arr)>::Types

namespace cx {
inline namespace arr {

///////////////////////////////////////////
// Utilities

template<typename U> concept SomeLegalType = CPlainArithmetic<U> || CPlainRawPointer<U>;

template<typename U, typename... Ts>
concept LegalTypeAmong = PlainArithmeticAmong<U, Ts...> || PlainRawPointerAmong<U, Ts...>;

#define Legal_Type cx::arr::SomeLegalType auto
#define Legal_Type_Among cx::arr::LegalTypeAmong auto


///////////////////////////////////////////
// Multi Array

/// A set of parallel dynamic arrays of different `Types (Ts`).
/// It is composed by the following:
/// - a tuple of pointers to each array `ptrs (std::tuple<Ts*...>)`;
/// - a number of active elements `len (isize)`;
/// - a dynamic storage capacity of `cap  (isize)`.
template<SomeLegalType... Ts> requires (sizeof...(Ts) > 1)
struct MultiArray {
    using Types = TypeSeq<Ts...>;
    using Self = MultiArray<Ts...>;
    onedef glob cons isize rows = sizeof...(Ts);
    onedef glob cons StaticArray<isize, rows> sizes{size_of(Ts)...};
    onedef glob cons StaticArray<isize, rows> aligns{align_of(Ts)...};

    StaticArray<mutaptr, rows> ptrs;
    isize len{};
    isize cap{};
};


///////////////////////////////////////////
// Utilities

CX_CONCEPT_GEN_TEMPL(
    MultiArray, is_multi_array, SomeMultiArray, VA_(typename... Ts), VA_(Ts...)
);
    #define Multi_Array cx::arr::SomeMultiArray auto

///////////////////////////////////////////
// Operations

/// Get…
///
template<SomeMultiArray Arr>
fn get(SomeMultiArray auto& arr, isize row, isize col) -> mutaptr
{
    return *ptr_add(arr.ptrs[row], col * Arr::sizes[row]);
}

///
///
template<SomeMultiArray Arr, isize Row>
fn get(Arr& arr, isize col) -> TypeAt<Row, typename Arr::Types>&
{
    return *ptr_add(arr.ptrs[Row], col * Arr::sizes[Row]);
}

///
///
template<isize N, SomeLegalType... Ts>
fn initialize(MultiArray<Ts...>& arr) -> ErrorCode where(N >= 8 && bool(is_power_of_two(N)))
{
    auto [ptr, err] = multi_make_zii<Ts...>(N) or_return err;
    arr.ptrs = _multi_bind_arr<Ts...>(ptr, N);
    arr.len = 0;
    arr.cap = N;
    return null;
}

///
// template<isize N, SomeLegalType... Ts,
//          EnableIf(N >= 8 && is_power_of_two(N))>
// fn initialize(MultiArray<Ts...>& arr) -> Result<>
// {
//   auto [ptr, err] = multi_make_zii<Ts...>(N);
//   if (err) {
//     return {cx_empty{}, move(err)};
//   }
//   arr.ptrs = priv__bind_begs<Ts...>(ptr, N);
//   arr.len = 0;
//   arr.cap = N;
//   return {cx_empty{}, nullptr};
// }

    #define base_ptr(multi_arr) multi_arr.ptrs[0]

// template<SomeLegalType... Ts> fn reallocate(MultiArray<Ts...>& arr) -> ErrorCode
// {
//   arr::Array old_begs = arr.ptrs;
//   isize new_cap = arr.cap * 2;
//
//   auto [new_ptr, err_0] = multi_make_zii<Ts...>(new_cap);
//   if (err_0) {
//     return {cx_empty{}, take(err_0)};
//   }
//
//   arr.ptrs = priv__bind_multi_arr<Ts...>(new_ptr, new_cap);
//   take_multi(arr.ptrs, old_begs, arr.len);
//
//   auto [_, err_1] = deallocate_multi<Ts...>(base_ptr(arr));
//   if (err_1) {
//     return {cx_empty{}, take(err_1)};
//   }
//
//   arr.cap = new_cap;
//   return {cx_empty{}, nullptr};
// }

///
// template<SomeLegalType... Ts> fn reallocate(MultiArray<Ts...>& arr) -> Result<>
// {
//   Tuple<Ts*...> old_begs = arr.ptrs;
//   isize new_cap = arr.cap * 2;
//
//   auto [new_ptr, err_0] = make_multi_zii<Ts...>(new_cap);
//   if (err_0) {
//     return {cx_empty{}, move(err_0)};
//   }
//
//   arr.ptrs = priv__bind_begs<Ts...>(new_ptr, new_cap);
//   memmove_multi<Ts...>(arr.ptrs, old_begs, arr.len);
//
//   auto [_, err_1] = mem::deallocate_multi<Ts...>(std::get<0>(old_begs));
//   if (err_1) {
//     return {cx_empty{}, move(err_1)};
//   }
//
//   arr.cap = new_cap;
//   return {cx_empty{}, nullptr};
// }

///
// template<SomeMultiArray Arr, typename... Ts,
//          Requires(multi_same_or_ref<typename Arr::Types, TypeSeq<Ts...>>)>
// fn push_back(Arr& arr, Ts&&... els) -> Result<>
// {
//   if (arr.cap == arr.len) {
//     auto [_, err] = arr::reallocate(arr);
//     if (err) {
//       return {cx_empty{}, take(err)};
//     }
//   }
//
//   auto tup = std::forward_as_tuple(forward<Ts>(els)...);
//
//   [&]<isize... I>(std::index_sequence<I...>) inln_clos -> void {
//     ((std::get<I>(arr.ptrs)[arr.len] = std::get<I>(tup)), ...);
//   }(std::index_sequence_for<Ts...>{});
//
//   arr.len += 1;
//   return {cx_empty{}, nullptr};
// }

/// TODO:!!!!
template<SomeMultiArray Arr, typename... Ts>
fn push_back(Arr& arr, Ts&&... els) -> ErrorCode
    where (multi_same_or_ref<typename Arr::Types, TypeSeq<Ts...>>)
{
    using L = TypeSeq<Ts...>;
    if (arr.cap == arr.len) {
        auto [_, err] = reallocate(arr) or_return err;
    }

    clos unfold = [&]<isize I>(auto&& elm) inln_clos -> void {
        using T = rm_cvref<TypeAt<I, L>>;
        T* dst = (T*) mem::ptr_add(arr.ptrs[I], arr.len * Arr::sizes[I]);
        *dst = forward<decltype(elm)>(elm);
    };

    [&]<isize... I>(IndexSeq<I...>) inln_clos -> void {
        (unfold.template operator()<I>(forward<Ts>(els)), ...);
    }(index_seq_pack<Ts...>{});

    arr.len += 1;
    return null;
}

}       // namespace arr
}       // namespace cx
#endif  // CX_ARR_MULTI_ARRAY_HH

////////////////////////////////////////////
// Testing

#define CX_TEST_MULTI_ARRAY 1
#ifdef CX_TEST_MULTI_ARRAY

fn test_multi_array_1() -> void
{
    using namespace cx;
    MultiArray<isize, f32> arr{};

    using First = TypeAt<0, MultiArray<isize, f32>::Types>;
    cast(First*, arr[0])[0] = 3;
}

#endif      // CX_TEST


//
//
//
//
///////////////////////////////////////////
// Padding
///////////////////////////////////////////
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
//      PlainRawPointerAmong<U, Ts...>, rm_cvref<U>, std::conditional_t<
//      PlainSmartPointerAmong<U, Ts...>, rm_cvref<U>,
//      std::decay_t<U>>>;


// NOTE:(manu): this is defined here since it requires a defined multi array,

// TODO: reason about if this is a good design choice. I plan to add an allocator in
// the API with default behaviour as a standard heap allocator. If i do that it is
// surel better to pass the multi array aned not a raw pointer, on the other hand
// passing a pointer, a cap etc what is needed is more consistent with the rest of
// the API i have made so far.
