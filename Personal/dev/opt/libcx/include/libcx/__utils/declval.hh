#ifndef CX_UTI_DECLVAL_HH
#define CX_UTI_DECLVAL_HH
#include <libcx/config.hh>
#include <libcx/__traits/qualifier.hh>

namespace cx {

/// Declval implementation
template<typename T> cexpr proc declval() noexce -> add_rref<T>;

}       // namespace cx
#endif  // CX_UTI_DECLVAL_HH
