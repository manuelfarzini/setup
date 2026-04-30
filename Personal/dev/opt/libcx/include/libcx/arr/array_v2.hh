/// \file libcx/arr/array_v2.hh
#ifndef CX_ARR_ARRAY_V2_HH
#define CX_ARR_ARRAY_V2_HH
#include <libcx/config.hh>
#include <libcx/traits.hh>
#include <libcx/concepts.hh>
#include <libcx/uti/utilities.hh>

namespace cx::arr {

template<typename T, usize N>
struct Array {
    using Elem = T;
    using Size = usize;
    using Iter = Elem*;
    using Kter = Elem const*;
    using Rter = uti::RevIterator<Iter>;
    using KRter = uti::RevIterator<Kter>;
    using Self = Array<T, N>;

    Elem buf[N];
    glob onedef cexpr Size len = N;

    finline cexpr proc operator[](Size const idx) noexce -> T&
    {
        return buf[idx];
    }
    finline cexpr proc operator[](Size const idx) const noexce -> T const&
    {
        return buf[idx];
    }

    finline cexpr proc begin()       noexce -> Iter { return buf; }
    finline cexpr proc end()         noexce -> Iter { return buf + N; }
    finline cexpr proc begin() const noexce -> Kter { return buf; }
    finline cexpr proc end()   const noexce -> Kter { return buf + N; }
};

template<typename T> Array(T) -> Array<T, 1>;

template<typename T, typename... U>
Array(T, U...) -> Array<uti::enable_if<(same_as<T, U> && ...), T>, 1 + sizeof...(U)>;

template<typename T> predicate is_array = false;

template<typename T, usize N> predicate is_array<Array<T, N>> = true;

template<typename T, typename U = void>
concept CArrayType = is_array<rm_cvref<T>> && same_as<rm_cvref<U>, uti::ElemIn<T>>;

#define Array_Type cx::uti::CArrayType auto

CX_TRIVIAL_TYPE_ASSERT(Array<VA_(Array<VA_(f32, 8u)>, 8u)>);
}       // namespace cx::arr
#endif  // CX_ARR_ARRAY_V2_HH


//−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
//  TODO:consider this design
//  template<typename T, auto N, uti::u32or64 S = decltype(N)>
//  struct Array {
//    using Elem = T;
//    using Size = S;
//    using Iter = T*;
//    using Cter = T const*;
//    using Self = Array<T, N>;
//    Elem buf[N];
//    static cexpr Size len = N;
//  }

