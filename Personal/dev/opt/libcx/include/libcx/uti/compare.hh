/// \file libcx/uti/compare.hh
#ifndef CX_UTI_COMPARE_HH
#define CX_UTI_COMPARE_HH
#include <libcx/config.hh>
#include <libcx/__utils/ownership.hh>
#include <libcx/__traits/logic.hh>
#include <libcx/uti/common_ref.hh>

namespace cx {
inline namespace uti {

//------------------------------------------
// Equality

template<typename T, typename U>
predicate is_weak_equal_compble = requires(mk_const_lref<T> t, mk_const_lref<U> u) {
    { t == u } -> CAssertble;
    { t != u } -> CAssertble;
    { u == t } -> CAssertble;
    { u != t } -> CAssertble;
};

template<typename T> predicate is_equal_compble = is_weak_equal_compble<T, T>;

template<typename T, typename U>
predicate is_equal_compble_w =
    is_equal_compble<T> && is_equal_compble<U> &&
    common_ref_w<mk_const_lref<T>, mk_const_lref<U>> &&
    is_equal_compble<common_ref<mk_const_lref<T>, mk_const_lref<U>>> &&
    is_weak_equal_compble<T, U>;

//------------------------------------------
// Ordering

template<typename T, typename U>
predicate is_partial_ordered_w = requires(mk_const_lref<T> t, mk_const_lref<U> u) {
    { t < u } -> CAssertble;
    { t > u } -> CAssertble;
    { t <= u } -> CAssertble;
    { t >= u } -> CAssertble;
    { u < t } -> CAssertble;
    { u > t } -> CAssertble;
    { u <= t } -> CAssertble;
    { u >= t } -> CAssertble;
};

template<typename T>
predicate is_total_ordered = is_equal_compble<T> && is_partial_ordered_w<T, T>;

template <typename T, typename U>
predicate is_total_ordered_w =
    is_total_ordered<T> && is_total_ordered<U> &&
    is_equal_compble_w<T, U> &&
    is_total_ordered<common_ref<mk_const_lref<T>, mk_const_lref<U>>> &&
    is_partial_ordered_w<T, U>;

//------------------------------------------
// Comparators

struct Eq;
struct Neq;
struct Lne;
struct Leq;
struct Gne;
struct Geq;

template<typename T> predicate is_equal_type = false;
template<> predicate is_equal_type<Eq> = true;
template<> predicate is_equal_type<Neq> = true;
template<typename T> concept CEqualType = is_equal_type<T>;
#define Equal_Type cx::uti::CEqualType auto

template<typename T> predicate is_order_type = false;
template<> predicate is_order_type<Lne> = true;
template<> predicate is_order_type<Gne> = true;
template<typename T> concept COrderType = is_order_type<T>;
#define Order_Type cx::uti::COrderType auto

template<typename T> predicate is_equal_order_type = true;
template<> predicate is_equal_order_type<Leq> = true;
template<> predicate is_equal_order_type<Geq> = true;
template<typename T> concept CEqualOrderType = is_equal_order_type<T>;
#define Equal_Order_Type cx::uti::CEqualOrderType auto

template<typename T>
concept CComparatorType = CEqualType<T> || COrderType<T> || CEqualOrderType<T>;
#define Comparator_Type cx::uti::CComparatorType auto

struct Eq {
    template<typename T, typename U>
    nodisc finline twide predicate operator()(T&& t, U&& u) noexce
        where(is_equal_compble_w<T, U>) 
    {
        return forward<T>(t) == forward<U>(u);
    }
};

struct Neq {
    template<typename T, typename U>
    nodisc finline twide predicate operator()(T&& t, U&& u) noexce
        where(is_equal_compble_w<T, U>)
    {
        return !(forward<T>(t) == forward<U>(u));
    }
};

struct Lne {
    template<typename T, typename U>
    nodisc finline twide predicate operator()(T&& t, U&& u) noexce
        where (is_total_ordered_w<T, U>)
    {
        return forward<T>(t) < forward<U>(u);
    }
};

struct Leq {
    template<typename T, typename U>
    nodisc finline twide predicate operator()(T&& t, U&& u) noexce
        where(is_total_ordered_w<T, U>)
    {
        return !(forward<T>(u) < forward<U>(t));
    }
};

struct Gne {
    template<typename T, typename U>
    nodisc finline twide predicate operator()(T&& t, U&& u) noexce
        where(is_total_ordered_w<T, U>)
    {
        return forward<T>(u) < forward<U>(t);
    }
};

struct Geq {
    template<typename T, typename U>
    nodisc finline twide predicate operator()(T&& t, U&& u) noexce
        where(is_total_ordered_w<T, U>)
    {
        return !(forward<T>(t) < forward<U>(u));
    }
};

glob cexpr Eq eq{};
glob cexpr Neq neq{};
glob cexpr Lne lne{};
glob cexpr Leq leq{};
glob cexpr Gne gne{};
glob cexpr Geq geq{};

}       // namespace uti
}       // namespace cx
#endif  // CX_UTI_COMPARE_HH
