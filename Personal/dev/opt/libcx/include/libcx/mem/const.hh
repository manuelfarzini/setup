#include <libcx/mem/common.hh>

cexpr usize MEM_LIMIT = 1024 * 1024 * 1024;

intern u8 global_buf[MEM_LIMIT] = {};

onedef ceval proc get_global_buf() -> u8*
{
    return global_buf;
}

#define CX_TEST 1
#if CX_TEST

onedef ceval proc test_const_1() -> void
{
    auto buf = get_global_buf();
}
#endif

