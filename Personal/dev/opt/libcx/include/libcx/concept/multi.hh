#ifndef CX_CONCEPTS_MULTI_HH
#define CX_CONCEPTS_MULTI_HH

#include "libcx/traits/relation.hh"

template<typename U, typename... Ts> 
concept PlainArithmeticAmong = CPlainArithmetic<U> && one_of<U, Ts...>;

template<typename U, typename... Ts>
concept PlainRawPointerAmong = CPlainRawPointer<U> && one_of<U, Ts...>;

#endif  // CX_CONCEPTS_MULTI_HH
