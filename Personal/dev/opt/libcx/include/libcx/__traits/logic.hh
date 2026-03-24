/** @File libcx/__traits/logic.hh **/
#ifndef CX___TRAITS_LOGIC_HH
#define CX___TRAITS_LOGIC_HH
#include <libcx/__config/macros.hh>
#include <libcx/__traits/relation.hh>
#include <libcx/__utils/ownership.hh>

namespace cx {

//------------------------------------------
// Operators

template<bool... Bs> predicate bvariand = (Bs && ...);
template<typename... Ts> predicate tvariand = (Ts::value && ...);
template<bool... Bs> predicate bvarior = (Bs || ...);
template<typename... Ts> predicate tvarior = (Ts::value || ...);

//------------------------------------------
// Assertble

template<typename T> concept ___assertble = is_convertible<T, bool>;

template<typename T>
predicate is_assertble = ___assertble<T> && requires(T&& t) {
  { !uti::forward<T>(t) } -> ___assertble;
};

template<typename T> concept CAssertble = is_assertble<T>;

}       // namespace cx
#endif  // CX___TRAITS_LOGIC_HH
