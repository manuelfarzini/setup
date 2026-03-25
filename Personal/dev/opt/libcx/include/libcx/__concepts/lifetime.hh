/// \file libcx/__concepts/lifetime.hh
#ifndef CX___CONCEPTS_LIFETIME_HH
#define CX___CONCEPTS_LIFETIME_HH
#include <libcx/config.hh>
#include <libcx/traits.hh>

namespace cx {

template<typename... Ts> concept DefCtble = (is_def_ctble<Ts> && ...);
template<typename... Ts> concept CpCtble = (is_cp_ctble<Ts> && ...);
template<typename... Ts> concept MvCtble = (is_mv_ctble<Ts> && ...);
template<typename... Ts> concept CpAsble = (is_cp_asble<Ts> && ...);
template<typename... Ts> concept MvAsble = (is_mv_asble<Ts> && ...);
template<typename... Ts> concept Dtble = (is_dtble<Ts> && ...);
template<typename... Ts> concept CpOrMvCtble = (cp_or_mv_ctble<Ts> && ...);
template<typename... Ts> concept CpOrMvAsble = (cp_or_mv_asble<Ts> && ...);
template<typename... Ts> concept ZiiInitble = (zii_initble<Ts> && ...);
template<typename... Ts> concept DefInitble = (def_initble<Ts> && ...);
// FIX:(manu): should impose sizeof...(Ts) >= 1 ?
// static_assert(DefCtble<>); // e.g. this one should work? Now does.

}       // namespace cx
#endif  // CX___CONCEPTS_LIFETIME_HH
