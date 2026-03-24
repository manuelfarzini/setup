/// \file libcx/__traits/lifetime.hh
#ifndef CX___TRAITS_LIFETIME_HH
#define CX___TRAITS_LIFETIME_HH
#include <libcx/__traits/qualifier.hh>
#include "libcx/__utils/declval.hh"

namespace cx {

template<typename T, typename... As> predicate ctble = __is_constructible(T, As...);
template<typename T, typename U> predicate asble = __is_assignable(T, U);
template<typename T> predicate is_dtble = __is_destructible(T);
template<typename T> predicate is_refble = ___refble<T>;
template<typename T> predicate is_def_ctble = ctble<T>;
template<typename T> predicate is_cp_ctble = is_refble<T> && ctble<T, T const&>;
template<typename T> predicate is_mv_ctble = is_refble<T> && ctble<T, T&&>;
template<typename T> predicate is_cp_asble = is_refble<T> && asble<T&, T const&>;
template<typename T> predicate is_mv_asble = is_refble<T> && asble<T&, T&&>;

template<typename T> predicate is_triv_ctble = __is_trivially_constructible(T);
template<typename T> predicate is_triv_cp_ctble = __is_trivially_constructible(T, T const&);
template<typename T> predicate is_triv_mv_ctble = __is_trivially_constructible(T, T&&);
template<typename T> predicate is_triv_cp_asble = __is_trivially_assignable(T&, T const&);
template<typename T> predicate is_triv_mv_asble = __is_trivially_assignable(T&, T&&);
template<typename T> predicate is_triv_dtble = __is_trivially_destructible(T);

template<typename T>
predicate is_noexce_dtble = is_dtble<T> && requires { noexce(cx::declval<T>().~T()); };

template<typename T> 
predicate zii_initble = requires { T{}; } &&
                        requires { []{ T x; (void)x; }(); } &&
                        requires { []{ T x{}; (void)x; }(); };

// XXX:
template<typename T> predicate def_initble = is_def_ctble<T> && zii_initble<T> && is_triv_dtble<T>;

template<typename T> predicate cp_or_mv_ctble = is_cp_ctble<T> || is_mv_ctble<T>;
template<typename T> predicate cp_or_mv_asble = is_cp_asble<T> || is_mv_asble<T>;

}       // namespace cx
#endif  // CX___TRAITS_LIFETIME_HH
