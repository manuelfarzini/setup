/// \file libcx/ite/iterator.hh
#ifndef CX_UTI_ITERATOR_HH
#define CX_UTI_ITERATOR_HH
#include <libcx/config.hh>

namespace cx {

inline namespace uti {

template<typename It> struct RevIterator {
  using Iter = It;
  Iter cur;

  cexpr RevIterator() = default;
  cexpr RevIterator(Iter it) : cur(it) {}

  cexpr Iter base(this auto const& self) noexce { return self.cur; }

  cexpr proc operator*(this auto const& self) noexce -> decltype(auto)
  {
    auto tmp = self.cur;
    --tmp;
    return *tmp;
  }

  cexpr proc operator->(this auto const& self) noexce -> auto
  {
    return &(*self);  // XXX: proxy?
  }

  cexpr proc operator++(this auto& self) noexce -> decltype(self)
  {
    --self.cur;
    return self;
  }

  cexpr proc operator++(this auto& self, int) noexce -> auto
  {
    auto tmp = self;
    --self.cur;
    return tmp;
  }

  cexpr proc operator--(this auto& self) noexce -> decltype(self)
  {
    ++self.cur;
    return self;
  }

  cexpr proc operator--(this auto& self, int) noexce -> auto
  {
    auto tmp = self;
    ++self.cur;
    return tmp;
  }

  friend cexpr proc operator==(
    RevIterator const& a, RevIterator const& b) noexce -> b32
  {
    return a.cur == b.cur;
  }

  friend cexpr proc operator!=(
    RevIterator const& a, RevIterator const& b) noexce -> b32
  {
    return !(a == b);
  }
};

template<typename It> cexpr auto rev_iterator(It it) noexce -> RevIterator<It>
{
  return RevIterator<It>(it);
}

finline cexpr proc beg(auto& c) noexce -> decltype(auto)
{
  return iter_in(c)(c.begin());
}
finline cexpr proc beg(auto const& c) noexce -> decltype(auto)
{
  return cter_in(c)(c.begin());
}
finline cexpr proc rbeg(auto& c) noexce -> decltype(auto)
{
  return rter_in(c)(c.rbegin());
}
finline cexpr proc rbeg(auto const& c) noexce -> decltype(auto)
{
  return crter_in(c)(c.rbegin());
}

finline cexpr proc end(auto& c) noexce -> decltype(auto)
{
  return iter_in(c)(c.end());
}
finline cexpr proc end(auto const& c) noexce -> decltype(auto)
{
  return cter_in(c)(c.end());
}
finline cexpr proc rend(auto& c) noexce -> decltype(auto)
{
  return rter_in(c)(c.rend());
}
finline cexpr proc rend(auto const& c) noexce -> decltype(auto)
{
  return crter_in(c)(c.rend());
}

}       // namespace uti
}       // namespace cx 
#endif  // CX_ITE_ITERATOR_HH
