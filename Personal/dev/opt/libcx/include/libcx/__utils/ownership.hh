/// \file libcx/uti/ownership.hh
#ifndef CX_UTI_OWNERSHIP_HH
#define CX_UTI_OWNERSHIP_HH
#include <libcx/config.hh>
#include <libcx/traits.hh>
#include <libcx/uti/sfinae.hh>

namespace cx {
inline namespace uti {

template<typename T>
nodisc finline onedef cexpr proc forward(rm_ref<T>& x) noexce -> T&&
{
  return static_cast<T&&>(x);
}

template<typename T, Requires(!is_lref<T>)>
nodisc finline onedef cexpr proc forward(rm_ref<T>&& x) noexce -> T&&
{
  return static_cast<T&&>(x);
}

template<typename T>
nodisc finline onedef cexpr proc take(T&& x) noexce -> rm_ref<T>&&
{
  return static_cast<rm_ref<T>&&>(x);
}

// only xvalues/prvalues are passed as T&&
template<typename T>
nodisc finline onedef cexpr proc pass(T&& x) noexce -> T&&
{
  return static_cast<rm_ref<T>&&>((x));
}

}       // namespace uti
}       // namespace cx
#endif  // CX_UTI_OWNERSHIP_HH
