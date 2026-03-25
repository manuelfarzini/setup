/// \file libcx/err/result_pair.hh
#ifndef CX_ERR_RESULT_PAIR_HH
#define CX_ERR_RESULT_PAIR_HH
#include <libcx/config.hh>
#include <libcx/traits.hh>
#include <libcx/uti/utilities.hh>
#include <libcx/err/error_message.hh>

namespace cx::err {
  
/// A pair of a value and an error. 
template<
  typename V = Empty, typename E = ErrorPointer,
  Requires(!is_error_pointer<rm_cvref<V>>)>
struct ResultPair {
  using Value = V;
  using Error = E;

  Value val{};
  Error err{null};
};

ResultPair() -> ResultPair<Empty, ErrorPointer>;

template<typename V>
ResultPair(V) -> ResultPair<uti::enable_if<!is_error_pointer<rm_cvref<V>>, V>>;

template<typename V>
ResultPair(V, ErrorPointer) -> ResultPair<uti::enable_if<!is_error_pointer<rm_cvref<V>>, V>>;

CX_CONCEPT_GEN_TEMPL(
  ResultPair, is_result_pair, CResultPair, 
  VA_(typename V, typename E), VA_(V, E));
#define Result_Pair cx::err::CResultPair auto


static_assert(is_def_ctble<ResultPair<Empty>>);
static_assert(is_mv_ctble<ResultPair<Empty>>);
static_assert(is_mv_asble<ResultPair<Empty>>);
static_assert(is_dtble<ResultPair<Empty>>);
}       // namespace cx::err
#endif  // CX_ERR_RESULT_PAIR_HH
