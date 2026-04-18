/** @file libcx/__config/types.hh **/
#ifndef CX___CONFIG_TYPES_HH
#define CX___CONFIG_TYPES_HH
#include <libcx/__config/platform.hh>
#include <libcx/__config/macros.hh>
#include <initializer_list>

namespace cx {

template<bool B> 
predicate kpred = B;

template<typename T>
using Void = void;

struct EmptyType {};
onedef cexpr EmptyType empty;

template<typename T> 
using initls = std::initializer_list<T>;

struct ZeroType {
    implicit cexpr operator isize() const noexce { return 0; }
    implicit cexpr operator void*() const noexce { return nullptr; }
    implicit cexpr operator decltype(nullptr)() const noexce { return nullptr; }
};
onedef cexpr ZeroType null;
onedef cexpr ZeroType none;

template<typename V, typename E>
struct Res {
    using Val = V;
    using Err = E;
    Val val;
    Err err;
};

// struct Res<V, E> {
//     Val := V
//     Err := E
//     val: Val
//     err: Err
// };

// struct Res<V, E>  {
//     type Val = V
//     type Err = E
//     Val val
//     Err err
// };

//  proc do_something_special<type V, type E, int n>(Res<V, E> res) -> void, where:
//      is_error.<E>, is_arithmetic.<V>
//  {
//  }

//  proc do_something_special(type $V, type $E, Res(V, E) res) -> void, where:
//      is_error(E), is_arithmetic(V)


}       // namespace cx
#endif  // CX___CONFIG_TYPES_HH
