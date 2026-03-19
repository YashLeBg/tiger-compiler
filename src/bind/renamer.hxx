/**
 ** \file bind/renamer.hxx
 ** \brief Template methods of bind::Renamer.
 */

#pragma once

#include <bind/renamer.hh>

namespace bind
{
  // FIXME: Some code was deleted here.

  template <class E, class Def>
  inline void Renamer::visit(E& e, const Def* def)
  {
    // FIXME: Some code was deleted here.
    if (def)
      {
        auto it= new_names_.find(def);
        if (it!=new_names_.end())
          {
            e.name_set(it->second);
          }
      }
  }
} // namespace bind
