#ifndef CX_UTI_TRAITS_HH
#define CX_UTI_TRAITS_HH
#include <libcx/__traits/qualifier.hh>
#include <libcx/__traits/lifetime.hh>
#include <libcx/__traits/types.hh>
#include <type_traits>

template<isize bytes> struct ___BytesToSint
{};
template<> struct ___BytesToSint<1> { using T = i8; };
template<> struct ___BytesToSint<2> { using T = i16; };
template<> struct ___BytesToSint<4> { using T = i32; };
template<> struct ___BytesToSint<8> { using T = i64; };

template<usize bytes>
using BytesToSint = ___BytesToSint<bytes>::T;

template<isize bytes> struct ___BytesToUint
{};
template <> struct ___BytesToUint<1> { using T = u8; };
template <> struct ___BytesToUint<2> { using T = u16; };
template <> struct ___BytesToUint<4> { using T = u32; };
template <> struct ___BytesToUint<8> { using T = u64; };

template<usize bytes>
using BytesToUint = ___BytesToUint<bytes>::T;

template<typename T>
using UnderSint = BytesToSint<sizeof(T)>;

template<typename T>
using UnderUint = BytesToUint<usize_of(T)>;

template<typename T>
glob cexpr isize tsize_of = isize(sizeof(T));

template<auto v>
glob cexpr isize vsize_of = isize(sizeof(v));

// template<typename T>
// finline onedef cexpr proc size_of(T const & t) noexce -> isize
// {
//     return isize(sizeof(t));
// }
//
// template<typename T>
// finline onedef cexpr proc size_of() noexce -> isize
// {
//     return isize(sizeof(T));
// }

#endif  // CX_UTI_TRAITS_HH
