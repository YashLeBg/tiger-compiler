/**
 ** \file bind/binder.hxx
 ** \brief Inline methods of bind::Binder.
 **/

// FIXME: Some code was deleted here.
#pragma once

#include <bind/binder.hh>
#include <misc/contract.hh>

namespace bind
{
  inline void Binder::scope_begin()
  {
    vars_.scope_begin();
    funcs_.scope_begin();
    types_.scope_begin();
  }

  inline void Binder::scope_end()
  {
    vars_.scope_end();
    funcs_.scope_end();
    types_.scope_end();
  }

  template <class D, class M> void Binder::chunk_visit(ast::Chunk<D>& e, M& map)
  {
    std::unordered_map<misc::symbol, const D*> seen;
    for (const auto* dec : e)
      {
        auto dup = seen.find(dec->name_get());
        if (dup != seen.end())
          error_ << misc::error::error_type::bind;
        else
          seen.emplace(dec->name_get(), dec);
        map.put(dec->name_get(), const_cast<D*>(dec));
      }

    for (auto* dec : e)
      operator()(*dec);
  }

} // namespace bind