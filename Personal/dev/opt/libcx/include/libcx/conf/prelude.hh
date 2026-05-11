/** @file conf/prelude.hh **/

#ifndef CX_CONF_PRELUDE_HH
#define CX_CONF_PRELUDE_HH

#include <libcx/conf/macro.hh>

namespace cx {

template<bool B> predicate kpred = B;

template<isize bit>
onedef cons isize BIT = 1ull << bit;

using ErrorCode = u8;
onedef cons u8 Error_None = 0;
onedef cons u8 Operation_Fail = 1;
onedef cons u8 Invalid_Ptr = 2;
onedef cons u8 Invalid_Arg = 3;
onedef cons u8 Bad_Alloc = 4;
onedef cons u8 Invalid_Mode = 5;

}

#endif  // CX_CONF_PRELUDE_HH
