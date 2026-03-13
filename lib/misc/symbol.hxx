/**
 ** \file misc/symbol.hxx
 ** \brief Inline implementation of misc::symbol.
 */

#pragma once

#include <misc/symbol.hh>

namespace misc
{
  inline symbol& symbol::operator=(const symbol& rhs)
  {
    // FIXME: Some code was deleted here.
    unique<std::string>::operator=(rhs);
    return *this;
  }

  inline bool symbol::operator==(const symbol& rhs) const
  {
    // FIXME: Some code was deleted here.
    return unique<std::string>::operator==(rhs);
  }

  inline bool symbol::operator!=(const symbol& rhs) const
  {
    // FIXME: Some code was deleted here.
    return unique<std::string>::operator!=(rhs);
  }

  inline std::ostream& operator<<(std::ostream& ostr, const symbol& the)
  {
    // FIXME: Some code was deleted here.
    return ostr << the.get();
  }

} // namespace misc
