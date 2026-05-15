// #include <libcx/mem/multi.hh>

#include "stdio.h"
#include "_allocator.hh"

int main() {
    using namespace cx::mem;
    i8 arr[10];

    init_ls<u8>(arr, {1, 2, 3, 4, 5});
    for (int i = 0; i < 10; i++) {
        printf("%td\n", (isize) arr[i]);
    }
    return 0;
}
