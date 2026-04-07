/** \file libcx/uti/tuple.hh **/
#ifndef CX_UTI_TUPLE_HH
#define CX_UTI_TUPLE_HH
#include <libcx/config.hh>
#include <libcx/uti/sfinae.hh>
#include <libcx/uti/typeseq.hh>

namespace cx {
inline namespace uti {

template<typename... Ts> requires (sizeof...(Ts) > 0)
struct Tuple;

template<typename T, typename... Ts> requires (sizeof...(Ts) > 0)
struct Tuple<T, Ts...> {
    using Elem = T;
    using Types = TypeSeq<T, Ts...>;
    using Self = Tuple<T, Ts...>;
    using Rest = Tuple<Ts...>;

    onedef glob cexpr usize size = 1 + sizeof...(Ts);

    Elem elem{};
    Rest rest{};
};

template<typename T> struct Tuple<T> {
    using Elem = T;
    using Types = TypeSeq<T>;
    using Self = Tuple<T>;
    using Rest = void;

    onedef glob cexpr usize size = 1;

    Elem elem{};
};

template<typename... Ts> Tuple(Ts...) -> Tuple<Ts...>;
template<typename T, typename... Ts> Tuple(T, Ts...) -> Tuple<T, Ts...>;
template<typename T> Tuple(T) -> Tuple<T>;

CX_CONCEPT_GEN_TEMPL(Tuple, is_tuple, CTupleType, typename... Ts, Ts...);


//------------------------------------------
// Operations

template<usize I, CTupleType Tup, Requires(I < Tup::size)>
finline onedef cexpr proc get(Tup& tup) -> auto&
{
    if cexpr (I == 0) {
        return tup.elem;
    } else {
        return get<I - 1, Tup::Rest>(tup.rest);
    }
}

template<usize I, CTupleType Tup, Requires(I < Tup::size)>
finline onedef cexpr proc get(Tup const& tup) -> auto const&
{
    if cexpr (I == 0) {
        return tup.elem;
    } else {
        return get<I - 1, Tup::Rest>(tup.rest);
    }
}

template<typename T, CTupleType Tup, typename L = Tup::Types>
finline onedef cexpr proc get(Tup& tup) -> auto&
{
    cexpr usize I = type_idx<T, L>;
    if cexpr (I == 0) {
        return tup.elem;
    } else {
        return get<I - 1, typename Tup::Rest>(tup.rest);
    }
}

template<typename T, CTupleType Tup, typename L = Tup::Types>
finline onedef cexpr proc get(Tup const& tup) -> auto const&
{
    cexpr usize I = type_idx<T, L>;
    if cexpr (I == 0) {
        return tup.elem;
    } else {
        return get<I - 1, typename Tup::Rest>(tup.rest);
    }
}

// #define CX_TEST 1
#if defined(CX_TEST)
proc test() -> void
{
    Tuple tup = {i64(42), 'c', 3.14};
    usize size = tup.size;
    i64 x = get<0>(tup);
    auto c = get<char>(tup);
    printf("%zu\n", size);
    printf("%lld\n", x);
    printf("%c\n", c);
}
#endif  // CX_TEST
}       // namespace cx
}       // namespace cx
#endif  // CX_UTI_TUPLE_HH
