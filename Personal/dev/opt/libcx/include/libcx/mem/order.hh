/** @file libcx/mem/order.hh **/

#ifndef CX_MEM_ORDER_HH
#define CX_MEM_ORDER_HH

#include <atomic>
#include <compare>

#include "libcx/conf/macro.hh"

namespace cx::mem {

    onedef cons auto order_relaxed = std::memory_order::relaxed;
    onedef cons auto order_acquire = std::memory_order::acquire;
    onedef cons auto order_release = std::memory_order::release;
    onedef cons auto order_acq_rel = std::memory_order::acq_rel;
    onedef cons auto order_seq_cst = std::memory_order::seq_cst;

}       // namespace cx::mem
#endif  // CX_MEM_ORDER_HH
