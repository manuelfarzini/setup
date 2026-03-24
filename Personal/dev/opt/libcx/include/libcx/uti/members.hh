#ifndef CX_UTI_MEMBERS_HH
#define CX_UTI_MEMBERS_HH
#include <libcx/config.hh>
#include <libcx/__traits/lifetime.hh>

namespace cx {
inline namespace uti {

template<typename T> struct ___ElemIn { using Type = typename rm_cvref<T>::Elem; };
template<typename T> using ElemIn = typename ___ElemIn<T>::Type;
template<typename T> struct ___elem_in { using Type = typename rm_cvref<T>::Elem; };
#ifndef elemIn
  #define elemIn(v) typename cx::uti::___elem_in<decltype(v)>::Type
#endif

template<typename T> struct ___SizeIn { using Type = typename rm_cvref<T>::Size; };
template<typename T> using SizeIn = typename ___SizeIn<T>::Type;
template<typename T> struct ___size_in { using Type = typename rm_cvref<T>::Size; };
#ifndef sizeIn
  #define sizeIn(v) typename cx::uti::___size_in<decltype(v)>::Type
#endif

template<typename T> struct ___IterIn { using Type = typename rm_cvref<T>::Iter; };
template<typename T> using IterIn = typename ___IterIn<T>::Type;
template<typename T> struct ___iter_in { using Type = typename rm_cvref<T>::Iter; };
#ifndef iterIn
  #define iterIn(v) typename cx::uti::___iter_in<decltype(v)>::Type
#endif

template<typename T> struct ___CterIn { using Type = typename rm_cvref<T>::Kter; };
template<typename T> using CterIn = typename ___CterIn<T>::Type;
template<typename T> struct ___cter_in { using Type = typename rm_cvref<T>::Kter; };
#ifndef kterIn
  #define kterIn(v) typename cx::uti::___cter_in<decltype(v)>::Type
#endif

template<typename T> struct ___RterIn { using Type = typename rm_cvref<T>::Rter; };
template<typename T> using RterIn = typename ___RterIn<T>::Type;
template<typename T> struct ___rter_in { using Type = typename rm_cvref<T>::Rter; };
#ifndef rterIn
  #define rterIn(v) typename cx::uti::___rter_in<decltype(v)>::Type
#endif

template<typename T> struct ___CRterIn { using Type = typename rm_cvref<T>::KRter; };
template<typename T> using CRterIn = typename ___CRterIn<T>::Type;
template<typename T> struct ___crter_in { using Type = typename rm_cvref<T>::KRter; };
#ifndef krterIn
  #define krterIn(v) typename cx::uti::___crter_in<decltype(v)>::Type
#endif

template<typename T> struct ___PlainT { using Type = rm_cvref<T>; };
template<typename T> using PlainT = typename ___PlainT<T>::Type;
template<typename T> struct ___plain_t { using Type = rm_cvref<T>; };
#ifndef plainT
  #define plainT(v) typename cx::uti::___plain_t<decltype(v)>::Type
#endif

#ifndef selfT 
  #define selfT(cont) typename cx::uti::rm_cvref<decltype(cont)>::Self
#endif

}       // namespace cx
}       // namespace cx
#endif  // CX_UTI_MEMBERS_HH
