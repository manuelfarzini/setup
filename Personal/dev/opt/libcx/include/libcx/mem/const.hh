#include "libcx/mem/common.hh"

cons usize MEM_LIMIT = 1024 * 1024 * 1024;

priv u8 global_buf[MEM_LIMIT] = {};

comp fn get_global_buf() -> ptru8
{
    return global_buf;
}

#define CX_TEST 1
#if CX_TEST

comp fn test_const_1() -> void
{
    auto buf = get_global_buf();
}
#endif

