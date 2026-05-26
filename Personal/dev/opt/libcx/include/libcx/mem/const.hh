#include "libcx/conf/hal.hh"
#include "libcx/conf/macro.hh"

cons usize STATIC_ALLOC_LIMIT = 1024 * 1024 * 1024;

#define clos_tag []{}

template<isize S, auto tag> priv u8 static_buff[S];

cons byteptr global_buff = static_buff<STATIC_ALLOC_LIMIT, clos_tag>;
// cons byteptr another_buff = static_buff<STATIC_ALLOC_LIMIT, clos_tag>;

comp fn get_global_buf() -> byteptr
{
    return global_buff;
}

#define CX_TEST_CONST_ALLOCATOR 1
#if CX_TEST_CONST_ALLOCATOR

comp fn test_const_alloc_base() -> void
{
    auto buf = get_global_buf();
}

comp fn test_launch_const_alloc_base() -> void
{
    test_const_alloc_base();
}

#endif

