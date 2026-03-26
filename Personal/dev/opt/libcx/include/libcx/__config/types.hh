/** @file libcx/__config/types.hh **/
#ifndef CX___CONFIG_TYPES_HH
#define CX___CONFIG_TYPES_HH
#include <libcx/__config/platform.hh>
#include <libcx/__config/macros.hh>
#include <initializer_list>

namespace cx {

template<typename...> using Void = void;

template<bool B> predicate kpred = B;

struct Empty {};
glob cexpr Empty empty;

template<typename T> using initls = std::initializer_list<T>;

struct ZeroType {
    implicit cexpr operator isize() const noexce { return 0; }
    implicit cexpr operator void*() const noexce { return nullptr; }
    implicit cexpr operator decltype(nullptr)() const noexce { return nullptr; }
};
glob cexpr ZeroType null;
glob cexpr ZeroType none;

template<typename V, typename E> struct Res {
    using Val = V;
    using Err = E;
    Val val;
    Err err;
};

}  // namespace cx
#endif  // CX___CONFIG_TYPES_HH
