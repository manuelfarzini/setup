/// \file libcx/str/string_reference
#pragma once
#include <assert.h>
#include <libcx/uti/utilities.hh>
#include <libcx/uti/iterator.hh>

namespace cx::str {

struct StringReference;
CX_CONCEPT_GEN(StringReference, is_string_reference, CStringReference);
#define String_Reference cx::str::CStringReference auto

/// Non-owning reference to non-null terminated char buffer.
/// Maintains a pointer to the underlying char buffer `data` and his `size`.
struct StringReference {
  CX_MEMBER_ALIASES(char, usize);
  using Self = StringReference;

  twide cexpr usize npos = ~usize(0);
  Kter ptr{nullptr};             
  Size len{0};                   

  /// Default constructor.
  cexpr StringReference() = default;
  /// Disable conversion from nullptr.
  cexpr StringReference(nullptr_t) = delete;
  /// Construct a string from a cstring.
  cexpr StringReference(char const* s)
      : ptr(s), len(s ? __builtin_strlen(s) : 0) {}
  /// Construct a string from pointer and size
  cexpr StringReference(char const* data, usize size) : ptr(data), len(size) {}
  /// Construct a string from a std::string
  cexpr StringReference(std::string const& s) : ptr(s.data()), len(s.size()) {}
  /// Construct a string refernece from a std::string_view.
  cexpr StringReference(std::string_view s) : ptr(s.data()), len(s.size()) {}

  /// Get the first character of the string.
  finline cexpr proc get_front() const noexce -> char
  {
    assert(!is_empty());
    return ptr[0];
  }
  /// Get the last character of the string.
  finline cexpr proc get_back() const noexce -> char
  {
    assert(!is_empty());
    return ptr[len - 1];
  }
  /// Check if the string is empty.
  finline cexpr proc is_empty() const noexce -> bool { return len == 0; }

  // Iterators
  finline cexpr proc begin() const noexce -> Kter { return ptr; }
  finline cexpr proc end() const noexce -> Kter { return ptr + len; }
  finline cexpr proc rbegin() const noexce -> KRter { return uti::rev_iterator(end()); }
  finline cexpr proc rend() const noexce -> KRter { return uti::rev_iterator(begin()); }

  /// Member access operator.
  /// Returns the `char` at the given index in the underlying buffer.
  /// @param index the index.
  finline cexpr proc operator[](usize index) const noexce -> char
  {
    assert(index < len && "Invalid index");
    return ptr[index];
  }

  /// Convert a `StringReference` to a `std::string_view`
  implicit cexpr operator std::string_view() const noexce
  {
    return std::string_view(ptr, len);
  }
};

/// Comptime wrapper around a string literal.
/// Used to construct tables of `StringReferences`es with comptime length.
/// Should *only* be used in cexpr contexts.
struct StringReferenceLiteral : public StringReference {

  template<usize N>
  consteval StringReferenceLiteral(char const (&s)[N]) : StringReference(s, N - 1) {}
};

/// Provides a formatter for String to enable integration with the fmt API.
finline cexpr fmt::string_view format_as(StringReference const& lit) noexce {
  return {lit.ptr, lit.len};
}

}  // namespace cx::str


CX_BASIC_TYPE_ASSERT(cx::str::StringReference);
