/// \file libcx/str/stack_string.hpp
#pragma once
#include <libcx/str/zero_string.hh>
#include <libcx/err/error.hh>

namespace cx::str {

template<u8 C> requires (1 < C && C < 255)
struct StaticString;
CX_CONCEPT_GEN_TEMPL(StaticString, is_static_string, CStaticString, usize C, C);
#define Static_String CStaticString auto

/// A fixed character `buf`fer with a fixed `cap`acity `C`
/// and a variable `len`gth indicating current active characters.
template<u8 C> requires (1 < C && C < 255)
struct StaticString {
  CX_MEMBER_ALIASES(char, u8);
  using Self = StaticString<C>;

  /// The capacity of the string.
  static cexpr u8 cap = C;
  /// The internal character buffer.
  char buf[cap] = {};
  /// The length of the string.
  u8 len = 0;

  /// Default constructor
  finline cexpr StaticString() noexce = default;

  /// Creates a `StaticString` from the given literal `src` of size `N`.
  template<usize N> requires (N <= C)
  finline consteval StaticString(char const (&src)[N]) noexce : len(N)
  {
    for (u8 i = 0; i < N; i++) {
      buf[i] = src[i];
    }
  }

  /// Creates a `StaticString` from the given zero string `src` of size `N`.
  template<usize N> requires (N - 1 <= C)
  finline cexpr StaticString(ZeroString<N> src) noexce : len(N - 1)
  {
    for (u8 i = 0; i < len; i++) {
      buf[i] = src[i];
    }
  }

  /// Creates a `StaticString` from the given string view `src`.
  finline cexpr StaticString(std::string_view src) noexce : len(src.size())
  {
    for (u8 i = 0; i < len; i++) {
      buf[i] = src[i];
    }
  }

  /// Mutable begin iterator.
  finline cexpr Iter begin() noexce { return buf; }
  /// Mutable end iterator.
  finline cexpr Iter end() noexce { return buf + len; }
  /// Immutable begin iterator.
  finline cexpr Kter begin() const noexce { return buf; }
  /// Immutable end iterator.
  finline cexpr Kter end() const noexce { return buf + len; }

  /// Read only subscript operation with bounds checking.
  finline cexpr Result<char> at(u8 const i) noexce
  {
    if (i >= len) {
      return {'\0', cx_ope_err("index out of bounds")};
    }
    return {buf[i], nullptr};
  }

  /// Immutable subscript, no bounds checking.
  finline cexpr char operator[](u8 const i) const noexce { return buf[i]; }
  /// Mutable subscript, no bounds checking.
  finline cexpr char& operator[](u8 const i) noexce { return buf[i]; }

  /// Conversion operator to `string_view`.
  finline cexpr operator std::string_view() const noexce
  {
    return {&buf[0], len};
  }

  /// Clears the string.
  finline cexpr void clear() { len = 0; }
};

}  // namespace cx::str
CX_BASIC_TYPE_ASSERT(cx::str::StaticString<32>);
