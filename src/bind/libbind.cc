/**
 ** \file bind/libbind.cc
 ** \brief Define exported bind functions.
 */

#include <bind/binder.hh>
#include <bind/libbind.hh>
#include <bind/renamer.cc>
namespace bind
{
  // function to
  misc::error bind(ast::ChunkList& e)
  {
    Binder b;
    b(e);
    return b.error_get();
  }
void rename(ast::Ast& e)
  {
    Renamer rename_visitor;
    rename_visitor(e);
  }

} // namespace bind
