/** @file libcx/__utils/container.hh **/

#ifndef CX_UTILS_CONTAINER_HH
#define CX_UTILS_CONTAINER_HH

#include <libcx/config.hh>
#include <libcx/traits.hh>
#include <libcx/uti/sfinae.hh>
#include <libcx/uti/members.hh>

namespace cx {
inline namespace uti {

// =========================================
//  Type erasure
// =========================================

/** Predicate `true` if `T` is a container with a raw `storage`. **/
template<typename C>
predicate is_erased_container = requires(C c) {
    requires is_raw_array<declt(c.storage)>;
    requires same_as<
        cx::rm_extent<rm_ref<declt(c.storage)>>, u8
    >;
};

/** Predicate `true` if `T` is a container with a raw `storage`.   **/
template<typename C>
concept SomeErasedContainer = is_erased_container<C>;

/** 
    Returns a pointer to the object at `p` with its lifetime refreshed.
    @ret 
    - A pointer to the object at `p`, with its lifetime refreshed.
    @nota
    - To be used after constructing or re-constructing an object in existing
      storage, so the returned pointer refers to the current live object.
**/
template<typename T req (!is_func<T> && !is_void<T>)>
nodisc cons fn launder(T* p) noexce -> T*
{
    return __builtin_launder(p);
}

/** 
    Returns the raw memory of an `ErasedContainer` `c`.
    @ret
    - A `T` pointer to the raw memory of an `ErasedContainer` `c`
    @nota
     - To be used to construct the element(s) in `c`.
**/
template<SomeErasedContainer C>
inln cons fn get_place(C& c) noexce -> ElemIn<C>*
{
  return c.storage;
}

/** 
    Returns a pointer to a live object stored in an `ErasedContainer` `c`.
    @ret
     - A `T` pointer to a live object stored in an `ErasedContainer` `c`.
    @nota
     - To be used to access a fully constructed object in `c`.
**/
template<SomeErasedContainer C>
inln cons fn get_object(C& c) noexce -> ElemIn<C>*
{
    return launder(get_place<C>(c));
}

// NOTE:(manu) possible version
// template <SomeEraseContainer C>
// inln cons fn(get_object, C& c) ElemIn<C>*
// {
//    return launder(get_place(c));
// }

// =========================================
// Key type
// =========================================

template<typename T>
concept HasKeyIn = requires { typename T::KeyType; };

template<typename T, typename = void>
struct ___KeyOrSelf { using Type = T; };

template<typename T>
struct ___KeyOrSelf<T, Void<typename T::KeyType>> { using Type = typename T::KeyType; };
/** 
    Extract the key type from a container. If the `ElemType` in the container
    does not have a `KeyType`, it is set as the `ElemType` itself.
    It relies on SFINAE: the `void` template parameter fails substituition if
    the `ElemType` does not have a `KeyType`.
    It is meant to be used as follows.
    ```
        template<typename T> struct Container {
            using ElemType = T;
            using KeyType = cx::KeyOrSelf<T>;
            // ...
        };
    ```
**/
template<typename T>
using KeyOrSelf = typename ___KeyOrSelf<T>::Type;

}       // namespace uti
}       // namespace cx
#endif  // CX_UTILS_CONTAINER_HH
