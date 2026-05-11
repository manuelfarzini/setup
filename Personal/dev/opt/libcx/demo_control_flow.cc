#include "libcx/libcx.hh"

using CX;

fn test_help() -> Res<i32, i32> {
    return {1, 2};
}

fn test() -> Res<i32, i32> {
    auto [val, err] = test_help() or_return {0, 0};
    return {val, err};
}

int main() {
   auto [val, err] = test() or_else {
       puts("err");
       return 1;
   }
   return 0;
}
