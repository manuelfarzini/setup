// type_sprint.cpp
#include <cstdio>
#include <cstdint>
#include <cinttypes>
#include <cmath>
#include <cstring>

namespace typing {
namespace core {

struct TinyVec2 {
  double x, y;
};
struct PixelRGB {
  uint8_t R, G, B;
};
struct BigNumber {
  long long A, B;
};

static inline int clamp_i(int v, int lo, int hi) {
  return v < lo ? lo : (v > hi ? hi : v);
}
static inline double lerp(double a, double b, double t) {
  return a + (b - a) * t;
}
static inline uint32_t rotl32(uint32_t x, int r) {
  return (x << (r & 31)) | (x >> ((32 - r) & 31));
}

}  // namespace core

namespace io {

using ::std::fprintf;
using ::std::fputs;
using ::std::printf;
using ::std::putchar;
using ::std::puts;
using ::std::snprintf;

void short_printfs() {
  printf("d=%d\n", 42);
  printf("%+d % d %05d\n", -7, 7, 9);
  printf("%#x %#o %#X\n", 0x2A, 077, 0xDE);
  printf("%.2f %.6f %g\n", 3.14159, 2.718281, 1.0 / 3.0);
  printf("%zu %td %" PRIdMAX "\n", sizeof(long double), (ptrdiff_t) 16,
         (intmax_t) 9007199254740991LL);
  printf("s='%s' c='%c'\n", "[]{}()!@#$%^&*_-+=|~<>?:;.,/\\\0hidden", 'Z');
  printf("escapes:\\n\\t\\r\\v\\f\\a\\\\\\\"\\'\\?\\0\n");
  printf("nulls: a\\0b\\0c\\0 end\n");
}

void math_snips() {
  double pi = ::acos(-1.0);
  double e = ::exp(1.0);
  double z = ::sqrt(2.0) * pi / e;
  printf("pi=%.9f\ne=%.9f\nz=%.9f\n", pi, e, z);
  printf("mix=%g\n", ::pow(1.5, 5.25) + ::hypot(pi, z) - ::fmod(e * 123.0, 7.0));
  printf("hex-float %a %A\n", z, z);
}

void tiny_table() {
  puts("+- ---+---- -+");
  puts("| i  |  i*i-3i  |  rotl    |");
  puts("+----+----------+----------+");
  for (int i = 0; i < 12; ++i) {
    uint32_t r = typing::core::rotl32((uint32_t) (i * i + 0xA), (i * 7) % 31);
    printf("|%3d |%10d|%#10x|\n", i, i * i - 3 * i, r);
  }
  puts("+----+----------+----------+");
}

void format_minis() {
  int a = 42, b = -17;
  printf("[%8d]\n", a);
  printf("[%-8d]\n", a);
  printf("[%08d]\n", b);
  printf("[%#8x]\n", a * 313);
  printf("[% 8d][%+8d]\n", b, b);
  printf("[%10.4f]\n", (a - b) / 3.14159);
  printf("[%#10.4g]\n", (double) (a | 7));
  printf("[%*.*f]\n", 12, 5, 1.0 * a / b);
}

void stderr_bits() {
  fprintf(stderr, "err: code=%#x\n", 0xBEEF);
  fputs("err: message=segfault?\n", stderr);
  putchar('\n');
}

}  // namespace io

namespace util {

using ::std::printf;
using ::std::snprintf;

void buffer_roundtrip() {
  char buf[256];
  int n = snprintf(buf, sizeof(buf), "{id:%d,x:%+.3f,y:%+.3f,ok:%s}\\n\\0\\n", 7,
                   ::sin(7.0), ::cos(7.0), (7 % 2 ? "true" : "false"));
  printf("n=%d\n", n);
  printf("buf='%s'\n", buf);
}

typing::core::TinyVec2 make_pos(double t) {
  double r = 10.0 + 2.0 * ::sin(t * 3.0);
  return {r * ::cos(t), r * ::sin(t)};
}

void coords_demo() {
  for (int k = 0; k < 5; ++k) {
    double t = 0.75 * k + 0.25;
    auto p = make_pos(t);
    printf("t=%.2f\nx=%+.4f\ny=%+.4f\n", t, p.x, p.y);
  }
}

void color_dump() {
  typing::core::PixelRGB c{0x2A, 0x7F, 0xC3};
  printf("rgb=(%3u,%3u,%3u)\n", c.R, c.G, c.B);
  printf("hex=%#06x\n", (unsigned) ((c.R << 16) | (c.G << 8) | c.B));
}

}  // namespace util

namespace algo {

using ::std::printf;

int clamp_and_spin(int v) {
  int r = typing::core::clamp_i(v * 17 - 93 + (v % 5 ? 13 : -11), -128, 255);
  printf("v=%d\nr=%d\n", v, r);
  return r;
}

typing::core::BigNumber mul_add(long long a, long long b) {
  typing::core::BigNumber out{a * a + b, b * b + a};
  printf("A=%lld\nB=%lld\n", out.A, out.B);
  return out;
}

}  // namespace algo
}  // namespace typing

int main() {
  using typing::algo::clamp_and_spin;
  using typing::algo::mul_add;
  using typing::io::format_minis;
  using typing::io::math_snips;
  using typing::io::short_printfs;
  using typing::io::stderr_bits;
  using typing::io::tiny_table;
  using typing::util::buffer_roundtrip;
  using typing::util::color_dump;
  using typing::util::coords_demo;

  short_printfs();
  math_snips();
  tiny_table();
  format_minis();
  stderr_bits();
  buffer_roundtrip();
  coords_demo();
  color_dump();
  clamp_and_spin(42);
  mul_add(12345, -6789);

  // more symbols to type:
  std::printf("{a:%d,b:%d,ops:\"+= -= *= /= %%= |&^ ~ ! ? : () [] {} <>\"}\\n\\0\\n",
              1, 2);
  std::printf("+===[ HEADER ]== = =+\n");
  std::printf("| key:%-8s | val:%#08x |\n", "alpha", 0xC0FFEE);
  std::printf("| key:%-8s | val:%#08o |\n", "beta", 012345);
  std::printf("| key:%-8s | val:%#08X |\n", "gamma", 0xDEAD);
  std::printf("+== == === ===+\n");

  return 0;
}

