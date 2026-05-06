#include "libcx/libcx.hh"

#define or_return ; if (err) return

using CX;

proc test_help() -> Res<i32, i32> {
    return {1, 2};
}

proc test() -> Res<i32, i32> {
    auto [res, err] = test_help() or_return {0, 0};
    return {res, err};
}

int main() {
   auto [res, err] = test() or_return 1;
    
}
