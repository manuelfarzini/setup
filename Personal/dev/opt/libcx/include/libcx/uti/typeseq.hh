/** @file libcx/uti/typeseq.hh **/

#ifndef CX_UTI_TYPESEQ_HH
#define CX_UTI_TYPESEQ_HH

#include "libcx/conf/macro.hh"
#include "libcx/concept/types.hh"

namespace cx {
inline namespace uti {

/*                                         *
* Type sequence                            *
*                                         */

template<typename... Ts> struct TypeSeq {};
CX_CONCEPT_GEN_TEMPL(TypeSeq, is_type_seq, SomeTypeSeq, typename... Ts, Ts...);

/*                                         *
* Type at a given index in a type sequence *
*                                         */

template<usize Idx, SomeTypeSeq Seq> struct ___TypeAt;

template<usize Idx, typename Head, typename... Rest>
struct ___TypeAt<Idx, TypeSeq<Head, Rest...>>
    : ___TypeAt<Idx - 1, TypeSeq<Rest...>> {};

template<typename Head, typename... Rest>
struct ___TypeAt<0, TypeSeq<Head, Rest...>> { using Type = Head; };

template<usize Idx, SomeTypeSeq Seq>
using TypeAt = typename ___TypeAt<Idx, Seq>::Type;

template<usize Idx, typename... Ts>
using TypeAtVA = TypeAt<Idx, TypeSeq<Ts...>>::Type;

/*                                         *
* Index of a given type in a type sequence *
*                                         */

template<typename T, SomeTypeSeq Seq> struct ___type_idx;

template<typename T, typename Head, typename... Rest>
struct ___type_idx<T, TypeSeq<Head, Rest...>> {
  glob cons usize value =  1 + ___type_idx<T, TypeSeq<Rest...>>::value;
};

template<typename T, typename... Rest> 
struct ___type_idx<T, TypeSeq<T, Rest...>> { glob cons usize value = 0; };

template<typename T, SomeTypeSeq Seq>
onedef cons usize type_idx = ___type_idx<T, Seq>::value;

/*                                         *
* Size of a type sequence                  *
*                                         */

template<SomeTypeSeq Seq> struct ___type_seq_size;

template<typename... Ts> 
struct ___type_seq_size<TypeSeq<Ts...>> {
  glob onedef cons usize value = sizeof...(Ts);
};

template<SomeTypeSeq Seq>
onedef cons usize type_seq_size = ___type_seq_size<Seq>::value;

template<typename... Ts> struct ___pack_size {
  glob onedef cons usize value = type_seq_size<TypeSeq<Ts...>>;
};

template<typename... Ts> onedef cons usize va_size = ___pack_size<Ts...>::value;

/*                                         *
* Integer sequence                         *
*                                         */

template<CIntegral Int, Int... Is> 
struct IntegerSeq {
  using Elem = Int;
  onedef glob cons Elem size = sizeof...(Is);
};
CX_CONCEPT_GEN_TEMPL(IntegerSeq, is_integer_seq, CIntegerSeq,
                     VA_(CIntegral Int, Int... Is), VA_(Int, Is...));

template<CIntegral Int, usize N, usize... Is>
struct ___make_integer_seq : ___make_integer_seq<Int, N-1, N-1, Is...> {};

template<CIntegral Int, usize... Is>
struct ___make_integer_seq<Int, usize{0}, Is...> { using Type = IntegerSeq<Int, Int{Is}...>; };

template<CIntegral Int, usize N>
using make_integer_seq = typename ___make_integer_seq<Int, N>::Type;

template<CIntegral Int, typename... Ts>
using integer_seq_pack = make_integer_seq<Int, usize{sizeof...(Ts)}>;

template<CIntegral Int, typename T>
using integer_seq_for = make_integer_seq<Int, usize{T::size}>;

template<usize... Is> 
using IndexSeq = IntegerSeq<usize, Is...>;

template<usize N>
using make_index_seq = make_integer_seq<usize, N>;

template<typename... Ts>
using index_seq_pack = make_index_seq<usize{sizeof...(Ts)}>;

template<typename T>
using index_seq_for = make_index_seq<usize{T::size}>;

}       // namespace uti
}       // namespace cx
#endif  // CX_UTI_TYPESEQ_HH

/*                                         *
* Testing                                  *
*                                         */

// // #define CX_TEST 1
// #ifdef CX_TEST
// fn test_type_seq() -> void
// {
//   using CX;
//   printf("%zu\n", uti::type_idx<char, uti::TypeSeq<i32, char, f64>>);
// }
// #endif
