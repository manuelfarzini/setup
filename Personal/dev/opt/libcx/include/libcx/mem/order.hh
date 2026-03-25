/// \file libcx/mem/order.hh
#ifndef CX_MEM_ORDER_HH
#define CX_MEM_ORDER_HH
#include <libcx/config.hh>
#include <atomic>

namespace cx::mem {

  glob cexpr auto order_relaxed = std::memory_order::relaxed;
  glob cexpr auto order_acquire = std::memory_order::acquire;
  glob cexpr auto order_release = std::memory_order::release;
  glob cexpr auto order_acq_rel = std::memory_order::acq_rel;
  glob cexpr auto order_seq_cst = std::memory_order::seq_cst;

}  // namespace cx::mem
#endif  // CX_MEM_ORDER_HH
