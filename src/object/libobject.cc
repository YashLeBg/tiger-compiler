/**
 ** \file object/libobject.cc
 ** \brief Define exported object functions.
 */

#include <object/binder.hh>
#include <object/libobject.hh>
#include <object/renamer.hh>

namespace object
{
  misc::error bind(ast::ChunkList& e)
  {
    Binder b;
    b(e);
    return b.error_get();
  }

  class_names_type* rename(ast::Ast& e)
  {
    Renamer r;
    r(e);
    return r.class_names_get();
  }
} // namespace object
