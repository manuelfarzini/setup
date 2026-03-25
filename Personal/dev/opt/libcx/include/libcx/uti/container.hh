/// \file libcx/__utils/container.hh
#ifndef CX___UTILS_CONTAINER_HH
#define CX___UTILS_CONTAINER_HH
#include <libcx/config.hh>
#include <libcx/traits.hh>
#include <libcx/uti/sfinae.hh>
#include <libcx/uti/members.hh>

namespace cx {

//−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
// Type erasure

/// Predicate `true` if `T` is a container with a raw `storage`.
template<typename C> concept CErasedContainer = requires(C c) {
  requires is_raw_array<decltype(c.storage)>;
  requires same_as<
      cx::rm_extent<rm_ref<decltype(c.storage)>>,
      u8
  >;
};

/// XXX:
template<typename T, Requires(!is_func<T>, !is_void<T>)>
nodisc onedef cexpr proc launder(T* p) noexce -> T*
{
  return __builtin_launder(p);
}

/// Returns: a `T` pointer to the raw memory of an `ErasedContainer` `c`
/// Note: used to construct the element(s) in `c`.
template<CErasedContainer C>
finline onedef cexpr proc get_place(C& c) noexce -> uti::ElemIn<C>*
{
  return c.storage;
}

/// Returns: a `T` pointer to the object in an `ErasedContainer` `c`
/// Note: used to get the fully constructed object in `c`.
template<CErasedContainer C>
finline onedef cexpr proc get_object(C& c) noexce -> uti::ElemIn<C>*
{
  return launder(get_place<C>(c));
}

//−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
// Key type

template<typename T> concept HasKeyIn = requires { typename T::KeyType; };
template<typename T, typename = void> struct ___KeyOrSelf { using Type = T; };
template<typename T> struct ___KeyOrSelf<T, Void<typename T::KeyType>> {
  using Type = typename T::KeyType;
};
/// Extract the key type from a container. If the `ElemType` in the container
/// does not have a `KeyType`, it is set as the `ElemType` itself.
/// It relies on SFINAE: the `void` template parameter fails substituition if
/// the `ElemType` does not have a `KeyType`.
/// It is meant to be used as follows.
/// ```
///  template<typename T> struct Container {
///    using ElemType = T;
///    using KeyType = cx::uti::KeyOrSelf<T>;
///    // ...
///  };
/// ```
template<typename T> using KeyOrSelf = typename ___KeyOrSelf<T>::Type;

}       // namespace cx
#endif  // CX___UTILS_CONTAINER_HH
