#include "libcx/conf/hal.hh"

enum struct dtype { f32, f64 };
template<dtype d, usize n> struct _simd;

#if CX_NEON

template<> struct _simd<dtype::f32, 4> {
    // using Elem = f32;
    using Type = float32x4_t;
};
template<> struct _simd<dtype::f64, 2> {
   using Type = float64x2_t; 
};


#endif
