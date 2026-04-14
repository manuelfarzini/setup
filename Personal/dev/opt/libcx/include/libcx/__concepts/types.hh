/// \file libcx/__concepts/types.hh
#ifndef CX___CONCEPTS_TYPES_HH
#define CX___CONCEPTS_TYPES_HH
#include <libcx/config.hh>
#include <libcx/traits.hh>

namespace cx {
inline namespace uti {
  
template<typename T> concept CSintegral = is_sintegral<T>;
template<typename T> concept CUintegral = is_uintegral<T>;
template<typename T> concept CIntegral = is_sintegral<T> || is_uintegral<T>;

template<typename T> concept CFunctionAny = is_func_any<T>;
template<typename T> concept CFunctorType = is_fntor_type<T>;
template<typename T> concept CCallableType = is_callable<T>;

template<typename T> concept CRawPointer = is_raw_ptr<T>;
template<typename U> concept CPlainArithmetic = is_arithmetic<U> && !is_cv_qual<U>;

template<typename T>
concept CPlainRawPointer = CRawPointer<T> && !is_cv_qual<T> &&
                           !is_cv_qual<rm_ptr<T>>;

// template<typename U> concept CStringLike = is_convertible<U, std::string_view>;

template<typename U, typename... Ts> 
concept PlainArithmeticAmong = CPlainArithmetic<U> && one_of<U, Ts...>;

template<typename U, typename... Ts>
concept PlainRawPointerAmong = CPlainRawPointer<U> && one_of<U, Ts...>;

// template<typename U, typename... Ts>
// concept StringLikeAmong = CStringLike<U> && one_of<U, Ts...>; // XXX: make a pure version?

}       // namespace cx
}       // namespace uti
#endif  // CX___CONCEPTS_TYPES_HH
